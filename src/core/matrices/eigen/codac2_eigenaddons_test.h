/** 
 *  \file codac2_eigenaddons.h
 * 
 *  This file is included in the declaration of Eigen::MatrixBase,
 *  thanks to the preprocessor token EIGEN_MATRIXBASE_PLUGIN.
 *  See: https://eigen.tuxfamily.org/dox/TopicCustomizing_Plugins.html
 *  and the file codac2_matrices.h
 * 
 * ----------------------------------------------------------------------------
 *  \date       2024
 *  \author     Simon Rohou
 *  \copyright  Copyright 2023 Codac Team
 *  \license    GNU Lesser General Public License (LGPL)
 */



inline bool is_empty() const
{
  for(Index i = 0 ; i < rows() ; i++)
    for(Index j = 0 ; j < cols() ; j++)
      if((*this)(i,j).is_empty())
        return true;
  return false;
}


#define degenerate_matt(op) \
  Matrix<double,RowsAtCompileTime,ColsAtCompileTime> op(this->rows(),this->cols()); \
  \
  if(this->is_empty()) \
    op.init(std::numeric_limits<double>::quiet_NaN()); \
  \
  else \
  { \
    for(Index i = 0 ; i < this->rows() ; i++) \
      for(Index j = 0 ; j < this->cols() ; j++) \
        op(i,j) = (*this)(i,j).mid(); \
  } \
  \
  return op; \

inline auto mid() const
{
  degenerate_matt(mid);
}