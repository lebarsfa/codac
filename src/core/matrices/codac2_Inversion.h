/** 
 *  \file codac2_Inversion.h
 * ----------------------------------------------------------------------------
 *  \date       2024
 *  \author     Damien Massé
 *  \copyright  Copyright 2024 Codac Team
 *  \license    GNU Lesser General Public License (LGPL)
 */

#pragma once

#include <ostream>
#include "codac2_Matrix.h"
#include "codac2_IntervalMatrix.h"

namespace codac2
{
  enum LeftOrRightInv { LEFT_INV, RIGHT_INV };

  /** \brief Compute an upper bound of A+A^2+A^3 with A a matrix of intervals
   *  as an "error term" (use only bounds on coefficients)
   *  \param A matrix of intervals (supposed around 0)
   *  \param mrad the maximum radius of the result added (output argument)
   *  \return the enclosure. May include (-oo,oo) 
   */
  IntervalMatrix infinite_sum_enclosure(const IntervalMatrix& A, double &mrad);

  /** \brief Correct the approximate inverse of a matrix
   *  \tparam O if LEFT_INV, use the inverse of BA (otherwise use the inverse of AB,
   *   left inverse is normally better)
   *  \param A_ a matrix expression
   *  \param B_ a (almost punctual) approximation of its inverse,
   *  \return the enclosure
   */
  template<LeftOrRightInv O=LEFT_INV,typename OtherDerived,typename OtherDerived_>
  inline IntervalMatrix inverse_correction(const Eigen::MatrixBase<OtherDerived>& A_, const Eigen::MatrixBase<OtherDerived_>& B_)
  {
    assert_release(A_.is_squared());
    assert_release(B_.is_squared());

    auto A = A_.template cast<Interval>();
    auto B = B_.template cast<Interval>();

    Index N = A.rows();
    assert_release(N==B.rows());

    auto Id = IntervalMatrix::Identity(N,N);
    auto erMat = [&]() { if constexpr(O == LEFT_INV) return -B*A+Id; else return -A*B+Id; }();
    
    double mrad=0.0;
    IntervalMatrix E = infinite_sum_enclosure(erMat,mrad);
    IntervalMatrix Ep = Id+erMat*(Id+E); 
        /* one could use several iterations here, either
           using mrad, or directly */

    auto res = (O == LEFT_INV) ? IntervalMatrix(Ep*B) : IntervalMatrix(B*Ep);

    /* small problem with the matrix product : 0*oo = 0. We correct that
       "by hand" (?) */
    if (mrad==oo) {
       for (Index c=0;c<N;c++) {
         for (Index r=0;r<N;r++) {
           if (Ep(r,c).is_unbounded()) {
              for (Index k=0;k<N;k++) {
                   if constexpr(O == LEFT_INV)
                       res(r,k) = Interval();
                   else 
                       res(k,c) = Interval();
              }
           }
         }
       }
    }
    return  res;
  }

  /** \brief Enclosure of the inverse of a (non-singular) matrix expression
   *  \param A matrix expression
   *  \return the enclosure. Can have (-oo,oo) coefficients if A is singular
   *  or almost singular
   */
  template<typename OtherDerived>
  inline IntervalMatrix inverse_enclosure(const Eigen::MatrixBase<OtherDerived>& A)
  {
    assert_release(A.is_squared());
    Index N=A.rows();
    return inverse_correction<LEFT_INV>(A, 
      A.fullPivLu().solve(Matrix::Identity(N,N)));
  }


  /** \brief Enclosure of the inverse of a matrix of intervals
   *  \param A matrix of intervals
   *  \return the enclosure. Can have (-oo,oo) coefficients if the 
   *  inversion "failed"
   */
  IntervalMatrix inverse_enclosure(const IntervalMatrix &A);
}
