/** 
 *  \file codac2_Vector_eigenaddons.h
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

template<typename U=Scalar,int R=RowsAtCompileTime,int C=ColsAtCompileTime>
  requires (!IsIntervalDomain<U>) && (IsVectorOrRow<R,C>)
Matrix(std::initializer_list<double> l)
  : Matrix<double,R,C>(
    [&]() -> int { if(R == 1) return 1; else return l.size(); }(),
    [&]() -> int { if(C == 1) return 1; else return l.size(); }()
  )
{
  assert_release(!std::empty(l));
  size_t i = 0;
  for(const auto& li : l)
    (*this)[i++] = li;
}

template<typename U=Scalar,int R=RowsAtCompileTime,int C=ColsAtCompileTime>
  requires (!IsIntervalDomain<U>) && (IsVectorOrRow<R,C>)
explicit Matrix(int n)
  : Matrix<double,R,C>(
    [&]() -> int { if(R == 1) return 1; else return n; }(),
    [&]() -> int { if(C == 1) return 1; else return n; }()
  )
{
  assert_release(n >= 0);
  this->init(0.);
}

template<typename U=Scalar,int R=RowsAtCompileTime,int C=ColsAtCompileTime>
  requires (!IsIntervalDomain<U>) && (IsVectorOrRow<R,C>)
explicit Matrix(int n, double values[])
  : Matrix<double,R,C>(
    [&]() -> int { if(R == 1) return 1; else return n; }(),
    [&]() -> int { if(C == 1) return 1; else return n; }(),
    values
  )
{
  assert_release(n > 0);
}

template<typename U=Scalar,int R=RowsAtCompileTime,int C=ColsAtCompileTime>
  requires (!IsIntervalDomain<U>) && (IsVectorOrRow<R,C>)
inline size_t min_coeff_index() const
{
  size_t r,c;
  this->minCoeff(&r,&c);
  assert(c == 0);
  return r;
}

template<typename U=Scalar,int R=RowsAtCompileTime,int C=ColsAtCompileTime>
  requires (!IsIntervalDomain<U>) && (IsVectorOrRow<R,C>)
inline size_t max_coeff_index() const
{
  size_t r,c;
  this->maxCoeff(&r,&c);
  assert(c == 0);
  return r;
}