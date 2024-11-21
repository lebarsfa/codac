/** 
 *  \file codac2_IntervalMatrix_eigenaddons.h
 * 
 *  This file is included in the declaration of Eigen::MatrixBase,
 *  thanks to the preprocessor token EIGEN_MATRIX_PLUGIN.
 *  See: https://eigen.tuxfamily.org/dox/TopicCustomizing_Plugins.html
 *  and the file codac2_matrices.h
 * 
 * ----------------------------------------------------------------------------
 *  \date       2024
 *  \author     Simon Rohou
 *  \copyright  Copyright 2023 Codac Team
 *  \license    GNU Lesser General Public License (LGPL)
 */

//template<typename U=Scalar,int R=RowsAtCompileTime,int C=ColsAtCompileTime,typename... IV>
//  requires (IsIntervalDomain<U> && (!IsVectorOrRow<R,C>)
//    && (std::is_same_v<Matrix<codac2::Interval,-1,1>,IV> && ...))
//Matrix(const IV&... x)
//  : Matrix<codac2::Interval,-1,-1>(sizeof...(IV), std::get<0>(std::tie(x...)).size())
//{
//  Index i = 0;
//  ((this->row(i++) = x), ...);
//  assert_release(i == rows() && "invalid input size");
//}