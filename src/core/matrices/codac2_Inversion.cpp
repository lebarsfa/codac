/** 
 *  \file codac2_Inversion.cpp
 * ----------------------------------------------------------------------------
 *  \date       2024
 *  \author     Damien Massé
 *  \copyright  Copyright 2024 Codac Team
 *  \license    GNU Lesser General Public License (LGPL)
 */

#include <ostream>
#include "codac2_Inversion.h"
#include "codac2_Matrix.h"
#include "codac2_IntervalMatrix.h"

using namespace std;

namespace codac2
{
  /** \brief Compute an upper bound of A+A^2+A^3 with A a matrix of intervals
   *  \param A matrix of intervals (supposed around 0)
   *  \param mrad the maximum radius of the result added (output argument)
   *  \return the enclosure. May include (-oo,oo) 
   */
  IntervalMatrix infinite_sum_enclosure(const IntervalMatrix& A, double &mrad) {
      assert_release(A.is_squared());
      Index N = A.rows();

      Matrix B = A.mag();
      
      // modified Floyd algorithm
      for (Index k=0;k<N;k++) {
         for (Index r=0;r<N;r++) {
            if (r==k) continue;
            if (B(k,r)>0.0 && B(k,r)<B(k,k)) B(k,r)=B(k,k);
            if (B(r,k)>0.0 && B(r,k)<B(k,k)) B(r,k)=B(k,k);
         } 
         for (Index c=0;c<N;c++) {
            if (c==k) continue;
	    if (B(k,c)==0.0) continue;
            for (Index r=0;r<N;r++) {
                if (r==k) continue;
                if (B(r,k)==0.0) continue;
                if (B(r,c)<B(r,k)) B(r,c)=B(r,k);
                if (B(r,c)<B(k,c)) B(r,c)=B(k,c);
            }
         }
      }

      IntervalMatrix res(N,N);
      Interval v(-1.0,1.0);
      mrad=0.0;
      for (Index c=0;c<N;c++) {
         for (Index r=0;r<N;r++) {
            Interval sum=0.0;
            for (Index k=0;k<N;k++) {
               sum += std::min(B(r,k),B(k,c));
            }
            if (sum==0.0) {
               res(r,c)=A(r,c);
            } else if (sum.ub()>=1.0) {
               mrad=oo;
               res(r,c)=Interval(); 
            } else {
               Interval sumprod=1.0/(1.0-sum);
               if (sumprod.ub()>mrad) mrad=sumprod.ub();
               res(r,c)=(v*B(r,c))*sumprod;
            }
         }
      }
      return res;
  }
      
  /** \brief Correct the approximate inverse of a matrix
   *  \param A a matrix
   *  \param B a punctual approximation of its inverse
   *  \param left use BA
   *  \return the enclosure
   */
  IntervalMatrix inverse_correction(const Matrix &A, const Matrix &B, bool left) {
      assert_release(A.is_squared());
      assert_release(B.is_squared());
      Index N = A.rows();
      assert_release(N==B.rows());
  
      IntervalMatrix Id = IntervalMatrix::Identity(N,N);
      IntervalMatrix erMat = (left ? 
		IntervalMatrix(-IntervalMatrix(B)*IntervalMatrix(A)+Id) : 
	        IntervalMatrix(-IntervalMatrix(A)*IntervalMatrix(B)+Id));
      
      double mrad=0.0;
      IntervalMatrix E = infinite_sum_enclosure(erMat,mrad);
      IntervalMatrix Ep = Id+erMat*(Id+E); 
          /* one could use several iterations here, either
             using mrad, or directly */
      
      IntervalMatrix res(N,N);
      if (left) 
          res = Ep*IntervalMatrix(B);
      else
          res = IntervalMatrix(B)*Ep;
      /* small problem with the matrix product : 0*oo = 0. We correct that
         "by hand" (?) */
      if (mrad==oo) {
         for (Index c=0;c<N;c++) {
           for (Index r=0;r<N;r++) {
             if (Ep(r,c).is_unbounded()) {
                for (Index k=0;k<N;k++) {
                     if (left) 
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

  /** \brief Correct the approximate inverse of a matrix
   *  \param A a matrix
   *  \param B a punctual approximation of its inverse
   *  \param left use BA
   *  \return the enclosure
   */
  IntervalMatrix inverse_correction(const IntervalMatrix &A, const Matrix &B, bool left) {
      assert_release(A.is_squared());
      assert_release(B.is_squared());
      Index N = A.rows();
      assert_release(N==B.rows());
  
      IntervalMatrix Id = IntervalMatrix::Identity(N,N);
      IntervalMatrix erMat = (left ? 
		IntervalMatrix(-IntervalMatrix(B)*IntervalMatrix(A)+Id) : 
	        IntervalMatrix(-IntervalMatrix(A)*IntervalMatrix(B)+Id));
      
      double mrad=0.0;
      IntervalMatrix E = infinite_sum_enclosure(erMat,mrad);
      IntervalMatrix Ep = Id+erMat*(Id+E); 
          /* one could use several iterations here, either
             using mrad, or directly */
 
      IntervalMatrix res(N,N);
      if (left) 
          res = Ep*IntervalMatrix(B);
      else
          res = IntervalMatrix(B)*Ep;
      /* small problem with the matrix product : 0*oo = 0. We correct that
         "by hand" (?) */
      if (mrad==oo) {
         for (Index c=0;c<N;c++) {
           for (Index r=0;r<N;r++) {
             if (Ep(r,c).is_unbounded()) {
                for (Index k=0;k<N;k++) {
                     if (left) 
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

  /** \brief Enclosure of the inverse of a (non-singular) matrix
   *  \param A matrix
   *  \return the enclosure. Can have (-oo,oo) coefficients if A is singular
   *  or almost singular
   */
  IntervalMatrix inverse_enclosure(const Matrix &A) {
     assert_release(A.is_squared());
     Index N=A.rows();
     return inverse_correction(A, 
		A.fullPivLu().solve(Matrix::Identity(N,N)));
  }
 
  /** \brief Enclosure of the inverse of a matrix of intervals
   *  \param A matrix of intervals
   *  \return the enclosure. Can have (-oo,oo) coefficients if the 
   *  inversion "failed"
   */
  IntervalMatrix inverse_enclosure(const IntervalMatrix &A) {
     assert_release(A.is_squared());
     Index N=A.rows();
     return inverse_correction(A, 
	(A.mid()).fullPivLu().solve(Matrix::Identity(N,N)));
  }
}
