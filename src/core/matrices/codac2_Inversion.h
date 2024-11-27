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
  /** \brief Compute an upper bound of A+A^2+A^3 with A a matrix of intervals
   *  as an "error term" (use only bounds on coefficients)
   *  \param A matrix of intervals (supposed around 0)
   *  \param mrad the maximum radius of the result added (output argument)
   *  \return the enclosure. May include (-oo,oo) 
   */
  IntervalMatrix infinite_sum_enclosure(const IntervalMatrix& A, double &mrad);

  /** \brief Correct the approximate inverse of a matrix
   *  \param A a matrix
   *  \param B a punctual  approximation of its inverse
   *  \param left use the inverse of BA (otherwise use the inverse of AB,
   *   true is normally better)
   *  \return the enclosure
   */
  IntervalMatrix inverse_correction(const Matrix &A, const Matrix &B, 
		 bool left=true);

  /** \brief Correct the approximate inverse of a matrix
   *  \param A a matrix
   *  \param B a (almost punctual) approximation of its inverse,
   *  \param left use the inverse of BA (otherwise use the inverse of AB,
   *   left is normally better)
   *  \return the enclosure
   */
  IntervalMatrix inverse_correction(const IntervalMatrix &A, const Matrix &B,
		 bool left=true);

  /** \brief Enclosure of the inverse of a (non-singular) matrix
   *  \param A matrix
   *  \return the enclosure. Can have (-oo,oo) coefficients if A is singular
   *  or almost singular
   */
  IntervalMatrix inverse_enclosure(const Matrix &A);
 
  /** \brief Enclosure of the inverse of a matrix of intervals
   *  \param A matrix of intervals
   *  \return the enclosure. Can have (-oo,oo) coefficients if the 
   *  inversion "failed"
   */
  IntervalMatrix inverse_enclosure(const IntervalMatrix &A);
}
