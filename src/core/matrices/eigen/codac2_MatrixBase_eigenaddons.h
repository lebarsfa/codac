/** 
 *  \file codac2_MatrixBase_eigenaddons.h
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

template<int R=RowsAtCompileTime,int C=ColsAtCompileTime>
  requires (!IsVectorOrRow<R,C>)
explicit Matrix(int r, int c, const Scalar& x)
  : Matrix<Scalar,R,C>(r,c)
{
  assert((R==(int)r || R==-1) && (C==(int)c || C==-1));
  assert(r >= 0 && c >= 0);
  init(x);
}

template<int R=RowsAtCompileTime,int C=ColsAtCompileTime>
  requires (!IsVectorOrRow<R,C>)
explicit Matrix(int r, int c, const Scalar values[])
  : Matrix<Scalar,R,C>(r,c)
{
  assert((R==(int)r || R==-1) && (C==(int)c || C==-1));
  assert(r >= 0 && c >= 0);

  if(values == 0)
    init(Scalar(0.)); // in case the user called Matrix(r,c,0) and 0 is interpreted as NULL!

  else
  {
    size_t k = 0;
    for(size_t i = 0 ; i < this->rows() ; i++)
      for(size_t j = 0 ; j < this->cols() ; j++)
        (*this)(i,j) = values[k++];
    assert(k == this->size());
  }
}

template<int R=RowsAtCompileTime,int C=ColsAtCompileTime>
  requires (!IsVectorOrRow<R,C>)
inline auto block(size_t i, size_t j, size_t p, size_t q)
{
  assert_release(i >= 0 && p > 0 && i+p <= this->rows());
  assert_release(j >= 0 && q > 0 && j+q <= this->cols());
  return this->PlainObjectBase<Matrix<Scalar,RowsAtCompileTime,ColsAtCompileTime>>::block(i,j,p,q);
}

template<int R=RowsAtCompileTime,int C=ColsAtCompileTime>
  requires (!IsVectorOrRow<R,C>)
inline auto block(size_t i, size_t j, size_t p, size_t q) const
{
  assert_release(i >= 0 && p > 0 && i+p <= this->rows());
  assert_release(j >= 0 && q > 0 && j+q <= this->cols());
  return this->PlainObjectBase<Matrix<Scalar,RowsAtCompileTime,ColsAtCompileTime>>::block(i,j,p,q);
}

template<int R=RowsAtCompileTime,int C=ColsAtCompileTime>
  requires (!IsVectorOrRow<R,C>)
inline static Matrix<Scalar,R,C> zeros(size_t r, size_t c)
{
  assert_release(r >= 0 && c >= 0);
  return Matrix<Scalar,R,C>::Zero(r,c);
}

template<int R=RowsAtCompileTime,int C=ColsAtCompileTime>
  requires (!IsVectorOrRow<R,C>)
inline static Matrix<Scalar,R,C> ones(size_t r, size_t c)
{
  assert_release(r >= 0 && c >= 0);
  return Matrix<Scalar,R,C>::Ones(r,c);
}

template<int R=RowsAtCompileTime,int C=ColsAtCompileTime>
  requires (!IsVectorOrRow<R,C>)
inline static Matrix<Scalar,R,C> eye(size_t r, size_t c)
{
  assert_release(r >= 0 && c >= 0);
  return Matrix<Scalar,R,C>::Identity(r,c);
}

template<int R=RowsAtCompileTime,int C=ColsAtCompileTime>
  requires (!IsVectorOrRow<R,C>)
inline void resize_save_values(size_t r, size_t c)
{
  // With resize() of Eigen, the data is reallocated and all previous values are lost.
  auto copy = *this;
  this->resize(r,c);
  for(size_t i = 0 ; i < std::min((size_t)copy.rows(),r) ; i++)
    for(size_t j = 0 ; j < std::min((size_t)copy.cols(),c) ; j++)
      (*this)(i,j) = copy(i,j);
}