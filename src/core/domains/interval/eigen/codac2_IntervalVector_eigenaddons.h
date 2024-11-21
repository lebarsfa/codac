/** 
 *  \file codac2_IntervalVector_eigenaddons.h
 *  
 *  This class reuses some of the functions developed for ibex::IntervalVector. 
 *  The original IBEX code is revised in modern C++ and adapted to the template 
 *  structure proposed in Codac, based on the Eigen library.
 *  See ibex::IntervalVector (IBEX lib, author: Gilles Chabert)
 * 
 *  This file is included in the declaration of Eigen::MatrixBase,
 *  thanks to the preprocessor token EIGEN_MATRIX_PLUGIN.
 *  See: https://eigen.tuxfamily.org/dox/TopicCustomizing_Plugins.html
 *  and the file codac2_matrices.h
 *
 * ----------------------------------------------------------------------------
 *  \date       2024
 *  \author     Simon Rohou, Gilles Chabert
 *  \copyright  Copyright 2023 Codac Team
 *  \license    GNU Lesser General Public License (LGPL)
 */

template<typename U=Scalar,int R=RowsAtCompileTime,int C=ColsAtCompileTime>
  requires IsIntervalDomain<U> && IsVectorOrRow<R,C>
Matrix(const std::initializer_list<double>& l)
  : Matrix<codac2::Interval,R,C>(
    [&]() -> int { if(R == 1) return 1; else return l.size(); }(),
    [&]() -> int { if(C == 1) return 1; else return l.size(); }()
  )
{
  assert_release(!std::empty(l));
  Index i = 0;
  for(const auto& li : l)
    (*this)[i++] = codac2::Interval(li);
}

template<typename U=Scalar,int R=RowsAtCompileTime,int C=ColsAtCompileTime>
  requires IsIntervalDomain<U> && IsVectorOrRow<R,C>
Matrix(const std::initializer_list<std::initializer_list<double>>& l)
  : Matrix<codac2::Interval,R,C>(
    [&]() -> int { if(R == 1) return 1; else return l.size(); }(),
    [&]() -> int { if(C == 1) return 1; else return l.size(); }()
  )
{
  assert_release(!std::empty(l));
  Index i = 0;
  for(const auto& li : l)
    (*this)[i++] = codac2::Interval(li);
}

template<typename U=Scalar,int R=RowsAtCompileTime,int C=ColsAtCompileTime>
  requires IsIntervalDomain<U> && IsVectorOrRow<R,C>
Matrix(const std::initializer_list<codac2::Interval>& l)
  : Matrix<codac2::Interval,R,C>(
    [&]() -> int { if(R == 1) return 1; else return l.size(); }(),
    [&]() -> int { if(C == 1) return 1; else return l.size(); }()
  )
{
  assert_release(!std::empty(l));
  Index i = 0;
  for(const auto& li : l)
    (*this)[i++] = li;
}

template<typename U=Scalar,int R=RowsAtCompileTime,int C=ColsAtCompileTime>
  requires IsIntervalDomain<U> && IsVectorOrRow<R,C>
explicit Matrix(int n)
  : Matrix<codac2::Interval,R,C>(
    [&]() -> int { if(R == 1) return 1; else return n; }(),
    [&]() -> int { if(C == 1) return 1; else return n; }()
  )
{
  assert_release(n >= 0);
  this->init(codac2::Interval());
}

template<typename U=Scalar,int R=RowsAtCompileTime,int C=ColsAtCompileTime>
  requires IsIntervalDomain<U> && IsVectorOrRow<R,C>
Matrix(int n, const double bounds[][2])
  : Matrix<codac2::Interval,R,C>(
    [&]() -> int { if(R == 1) return 1; else return n; }(),
    [&]() -> int { if(C == 1) return 1; else return n; }(),
    bounds
  )
{
  assert_release(n > 0);
}

//template<typename U=Scalar,int R=RowsAtCompileTime,int C=ColsAtCompileTime>
//  requires IsIntervalDomain<U> && IsVectorOrRow<R,C>
//explicit Matrix(std::initializer_list<std::initializer_list<double>> l)
//  : Matrix<codac2::Interval,R,C>(
//    [&]() -> int { if constexpr(R == 1) return 1; else return l.size(); }(),
//    [&]() -> int { if constexpr(C == 1) return 1; else return l.size(); }()
//  )
//{
//  assert_release(!std::empty(l));
//  Index i = 0;
//  for(const auto& li : l)
//  {
//    assert_release(!std::empty(li));
//    (*this)[i++] = codac2::Interval(li);
//  }
//}

template<typename U=Scalar,int R=RowsAtCompileTime,int C=ColsAtCompileTime>
  requires IsIntervalDomain<U> && IsVectorOrRow<R,C>
inline static auto empty(Index n)
{
  assert_release(n >= 0);
  if constexpr(R == 1)
    return Matrix<codac2::Interval,R,C>((int)n,codac2::Interval::empty());
  else
    return Matrix<codac2::Interval,R,C>((int)n,codac2::Interval::empty());
}

template<typename U=Scalar,int R=RowsAtCompileTime,int C=ColsAtCompileTime>
  requires IsIntervalDomain<U> && IsVectorOrRow<R,C>
inline auto complementary() const
{
  return Matrix<codac2::Interval,R,C>((int)this->size()).diff(*this);
}

template<typename U=Scalar,int R=RowsAtCompileTime,int C=ColsAtCompileTime>
  requires IsIntervalDomain<U> && IsVectorOrRow<R,C>
inline std::list<Matrix<codac2::Interval,R,C>> diff(const Matrix<codac2::Interval,R,C>& y, bool compactness = true) const
{
  // This code originates from the ibex-lib
  // See: ibex_TemplateVector.h
  // Author: Gilles Chabert
  // It has been revised with modern C++ and templated types

  const Index n = this->size();
  assert_release(y.size() == n);

  if(y == *this)
    return { };

  Matrix<codac2::Interval,R,C> x = *this;
  Matrix<codac2::Interval,R,C> z = x & y;

  if(z.is_empty())
  {
    if(x.is_empty()) return { };
    else return { x };
  }

  else
  {
    // Check if in one dimension y is flat and x not,
    // in which case the diff returns also x directly
    if(compactness)
      for(Index i = 0 ; i < n ; i++)
        if(z[i].is_degenerated() && !x[i].is_degenerated())
        {
          if(x.is_empty()) return { };
          else return { x };
        }
  }

  std::list<Matrix<codac2::Interval,R,C>> l;

  for(Index var = 0 ; var < n ; var++)
  {
    codac2::Interval c1, c2;
    
    for(const auto& ci : x[var].diff(y[var], compactness))
    {
      assert(!ci.is_empty());

      Matrix<codac2::Interval,R,C> v(n);
      for(Index i = 0 ; i < var ; i++)
        v[i] = x[i];
      v[var] = ci;
      for(Index i = var+1 ; i < n ; i++)
        v[i] = x[i];
      if(!v.is_empty())
        l.push_back(v);
    }

    x[var] = z[var];
  }

  return l;
}