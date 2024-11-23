/** 
 *  \file codac2_Matrix_addons_VectorBase.h
 * 
 *  This file is included in the declaration of Eigen::MatrixBase,
 *  thanks to the preprocessor token EIGEN_MATRIX_PLUGIN.
 *  See: https://eigen.tuxfamily.org/dox/TopicCustomizing_Plugins.html
 *  and the file codac2_matrices.h
 * 
 * ----------------------------------------------------------------------------
 *  \date       2024
 *  \author     Simon Rohou
 *  \copyright  Copyright 2024 Codac Team
 *  \license    GNU Lesser General Public License (LGPL)
 */

template<int R=RowsAtCompileTime,int C=ColsAtCompileTime>
  requires IsVectorOrRow<R,C>
explicit Matrix(int n, const Scalar& x)
  : Matrix<Scalar,R,C>(
    [&]() -> int { if(R == 1) return 1; else return n; }(),
    [&]() -> int { if(C == 1) return 1; else return n; }()
  )
{
  assert_release(n > 0);
  init(x);
}

template<int R=RowsAtCompileTime,int C=ColsAtCompileTime>
  requires IsVectorOrRow<R,C>
inline Scalar& operator()(Index i)
{
  return const_cast<Scalar&>(const_cast<const Matrix<Scalar,R,C>*>(this)->operator()(i));
}

template<int R=RowsAtCompileTime,int C=ColsAtCompileTime>
  requires IsVectorOrRow<R,C>
inline const Scalar& operator()(Index i) const
{
  assert_release(i >= 0 && i < this->size());
  return this->PlainObjectBase<Matrix<Scalar,R,C>>::operator()(i);
}

template<int R=RowsAtCompileTime,int C=ColsAtCompileTime>
  requires IsVectorOrRow<R,C>
inline Scalar& operator[](Index i)
{
  return const_cast<Scalar&>(const_cast<const Matrix<Scalar,R,C>*>(this)->operator[](i));
}

template<int R=RowsAtCompileTime,int C=ColsAtCompileTime>
  requires IsVectorOrRow<R,C>
inline const Scalar& operator[](Index i) const
{
  assert_release(i >= 0 && i < this->size());
  return this->PlainObjectBase<Matrix<Scalar,R,C>>::operator[](i);
}

template<int R=RowsAtCompileTime,int C=ColsAtCompileTime>
  requires IsVectorOrRow<R,C>
inline static Matrix<Scalar,R,C> zeros(Index n)
{
  assert_release(n >= 0);
  if constexpr(R == 1)
    return Matrix<Scalar,R,C>::Zero(1,n);
  else
    return Matrix<Scalar,R,C>::Zero(n,1);
}

template<int R=RowsAtCompileTime,int C=ColsAtCompileTime>
  requires IsVectorOrRow<R,C>
inline static Matrix<Scalar,R,C> ones(Index n)
{
  assert_release(n >= 0);
  if constexpr(R == 1)
    return Matrix<Scalar,R,C>::Ones(1,n);
  else
    return Matrix<Scalar,R,C>::Ones(n,1);
}

// Note that this static function is not called "rand"
// because of ambiguity with the member function "rand"
template<int R=RowsAtCompileTime,int C=ColsAtCompileTime>
  requires IsVectorOrRow<R,C>
inline static Matrix<Scalar,R,C> random(Index n)
{
  assert_release(n >= 0);
  return Matrix<Scalar,R,C>::Random(n);
}

template<typename OtherDerived,int R=RowsAtCompileTime,int C=ColsAtCompileTime>
  requires IsVectorOrRow<R,C> && IsVectorOrRow<MatrixBase<OtherDerived>::RowsAtCompileTime,MatrixBase<OtherDerived>::ColsAtCompileTime>
inline void put(Index start_id, const MatrixBase<OtherDerived>& x)
{
  assert_release(start_id >= 0 && start_id < this->size());
  assert_release(start_id+x.size() <= this->size());

  this->segment(start_id,x.size()) << x;
}

template<int R=RowsAtCompileTime,int C=ColsAtCompileTime>
  requires IsVectorOrRow<R,C>
inline void resize_save_values(Index n)
{
  // With resize() of Eigen, the data is reallocated and all previous values are lost.
  auto copy = *this;
  this->resize(n);
  for(Index i = 0 ; i < std::min((Index)copy.size(),n) ; i++)
    (*this)[i] = copy[i];
}
