/** 
 *  \file codac2_Base_eigenaddons.h
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

template<typename Scalar_,int R_,int C_>
  requires (R_ != RowsAtCompileTime || C_ != ColsAtCompileTime)
Matrix(const Matrix<Scalar_,R_,C_>& x)
  : Matrix<Scalar,RowsAtCompileTime,ColsAtCompileTime>(x.rows(),x.cols())
{
  *this = x.template cast<Scalar>();
}

template<int R=RowsAtCompileTime,int C=ColsAtCompileTime>
inline Scalar& operator()(Index i, Index j)
{
    return const_cast<Scalar&>(static_cast<const Matrix<Scalar,R,C>&>(*this).operator()(i,j));
}

template<int R=RowsAtCompileTime,int C=ColsAtCompileTime>
inline const Scalar& operator()(Index i, Index j) const
{
  assert_release(i >= 0 && i < this->rows() && j >= 0 && j < this->cols());

  if constexpr(!IsVectorOrRow<R,C>)
    return this->PlainObjectBase<Matrix<Scalar,R,C>>::operator()(i,j);

  else
  {
    if constexpr(R == 1)
      return (*this)[j];
    return (*this)[i];
  }
}

inline auto& init(const Scalar& x)
{
  for(Index i = 0 ; i < this->size() ; i++)
    *(this->data()+i) = x;
  return *this;
}

inline auto& init(const Matrix<Scalar,RowsAtCompileTime,ColsAtCompileTime>& x) // todo: keep this?
{
  *this = x;
  return *this;
}

inline bool is_squared() const
{
  return this->rows() == this->cols();
}

#define minmax_item(op) \
  Scalar m = *(this->data()); /* first element */ \
  for(Index i = 1 ; i < this->size() ; i++) \
  { \
    if constexpr(std::is_same_v<Scalar,codac2::Interval>) \
      m = codac2::op(m,*(this->data()+i)); \
    else \
      m = std::op(m,*(this->data()+i)); \
  } \
  return m; \

inline Scalar min_coeff() const
{
  minmax_item(min);
}

inline Scalar max_coeff() const
{
  minmax_item(max);
}

template<typename U_,int R_,int C_>
inline bool operator==(const Matrix<U_,R_,C_>& x) const
{
  if(this->size() != x.size())
    return false;

  if constexpr(std::is_same_v<Scalar,codac2::Interval> && std::is_same_v<U_,codac2::Interval>)
  {
    if(this->is_empty() || x.is_empty())
      return this->is_empty() && x.is_empty();
  }

  if(this->rows() != x.rows() || this->cols() != x.cols())
    return false;

  for(Index i = 0 ; i < this->size() ; i++)
    if(*(this->data()+i) != *(x.data()+i))
      return false;

  return true;
}