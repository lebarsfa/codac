/** 
 *  \file codac2_IntervalMatrixBase_eigenaddons.h
 *  
 *  This class reuses some of the functions developed for ibex::IntervalMatrixBase. 
 *  The original IBEX code is revised in modern C++ and adapted to the template 
 *  structure proposed in Codac, based on the Eigen library.
 *  See ibex::IntervalMatrixBase (IBEX lib, author: Gilles Chabert)
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

template<typename U=Scalar>
  requires IsIntervalDomain<U>
Matrix(const Matrix<double,RowsAtCompileTime,ColsAtCompileTime>& x)
  : Matrix<U,RowsAtCompileTime,ColsAtCompileTime>(x.template cast<codac2::Interval>())
{ }

template<typename U=Scalar,int R=RowsAtCompileTime,int C=ColsAtCompileTime>
  requires IsIntervalDomain<U>
Matrix(const Matrix<double,R,C>& lb, const Matrix<double,R,C>& ub)
  : Matrix<U,R,C>(lb)
{
  assert_release(lb.size() == ub.size());

  for(Index i = 0 ; i < this->size() ; i++)
  {
    auto& lbi = *(this->data()+i);
    const auto& ubi = *(ub.data()+i);

    if(lbi.lb() > ubi)
    {
      set_empty();
      break;
    }

    else
      lbi |= ubi;
  }
}

template<typename U=Scalar,int R=RowsAtCompileTime,int C=ColsAtCompileTime>
  requires IsIntervalDomain<U>
Matrix(int r, int c, const double bounds[][2])
  : Matrix<U,R,C>(r,c)
{
  assert_release(r > 0 && c > 0);

  Index k = 0;
  for(Index i = 0 ; i < this->rows() ; i++)
    for(Index j = 0 ; j < this->cols() ; j++)
    {
      (*this)(i,j) = codac2::Interval(bounds[k][0],bounds[k][1]);
      k++;
    }
  assert_release(k == this->size() && "incorrect array size");
}

template<typename U=Scalar,int R=RowsAtCompileTime,int C=ColsAtCompileTime,typename OtherDerived>
  requires IsIntervalDomain<U>
Matrix(const MatrixBase<OtherDerived>& x)
  : Matrix<U,R,C>(x.eval().template cast<codac2::Interval>())
{ }

template<typename U=Scalar,int R=RowsAtCompileTime,int C=ColsAtCompileTime,typename OtherDerived>
  requires IsIntervalDomain<U>
inline bool operator==(const MatrixBase<OtherDerived>& x) const
{
  return operator==(x.eval().template cast<codac2::Interval>());
}

template<typename U=Scalar>
  requires IsIntervalDomain<U>
inline double volume() const
{
  if(this->is_empty())
    return 0.;

  double v = 0.;
  for(Index i = 0 ; i < this->size() ; i++)
  {
    if((this->data()+i)->is_unbounded()) return codac2::oo;
    if((this->data()+i)->is_degenerated()) return 0.;
    v += std::log((this->data()+i)->diam());
  }
  return std::exp(v);
}

#define degenerate_mat(op) \
  Matrix<double,RowsAtCompileTime,ColsAtCompileTime> op(this->rows(),this->cols()); \
  \
  if(this->is_empty()) \
    op.init(std::numeric_limits<double>::quiet_NaN()); \
  \
  else \
  { \
    for(Index i = 0 ; i < this->size() ; i++) \
      *(op.data()+i) = (this->data()+i)->op(); \
  } \
  \
  return op; \

template<typename U=Scalar>
  requires IsIntervalDomain<U>
inline auto lb() const
{
  degenerate_mat(lb);
}

template<typename U=Scalar>
  requires IsIntervalDomain<U>
inline auto ub() const
{
  degenerate_mat(ub);
}

template<typename U=Scalar>
  requires IsIntervalDomain<U>
inline auto mid() const
{
  degenerate_mat(mid);
}

template<typename U=Scalar>
  requires IsIntervalDomain<U>
inline auto mag() const
{
  degenerate_mat(mag);
}

template<typename U=Scalar>
  requires IsIntervalDomain<U>
inline auto mig() const
{
  degenerate_mat(mig);
}

template<typename U=Scalar>
  requires IsIntervalDomain<U>
inline auto rand() const
{
  srand(time(NULL));
  degenerate_mat(rand);
}

template<typename U=Scalar>
  requires IsIntervalDomain<U>
inline auto rad() const
{
  degenerate_mat(rad);
}

template<typename U=Scalar>
  requires IsIntervalDomain<U>
inline auto diam() const
{
  degenerate_mat(diam);
}

template<typename U=Scalar>
  requires IsIntervalDomain<U>
inline double min_diam() const
{
  return (this->data()+extr_diam_index(true))->diam();
}

template<typename U=Scalar>
  requires IsIntervalDomain<U>
inline double max_diam() const
{
  return (this->data()+extr_diam_index(false))->diam();
}

template<typename U=Scalar>
  requires IsIntervalDomain<U>
inline Index min_diam_index() const
{
  return extr_diam_index(true);
}

template<typename U=Scalar>
  requires IsIntervalDomain<U>
inline Index max_diam_index() const
{
  return extr_diam_index(false);
}

template<typename U=Scalar>
  requires IsIntervalDomain<U>
inline Index extr_diam_index(bool min) const
{
  // This code originates from the ibex-lib
  // See: ibex_TemplateVector.h
  // Author: Gilles Chabert

  double d = min ? codac2::oo : -1; // -1 to be sure that even a 0-diameter interval can be selected
  int selected_index = -1;
  bool unbounded = false;
  assert_release(!this->is_empty() && "Diameter of an empty IntervalVector is undefined");

  Index i;

  for(i = 0 ; i < this->size() ; i++) 
  {
    if((this->data()+i)->is_unbounded()) 
    {
      unbounded = true;
      if(!min) break;
    }
    else
    {
      double w = (this->data()+i)->diam();
      if(min ? w<d : w>d)
      {
        selected_index = i;
        d = w;
      }
    }
  }

  if(min && selected_index == -1)
  {
    assert(unbounded);
    // the selected interval is the first one.
    i = 0;
  }

  // The unbounded intervals are not considered if we look for the minimal diameter
  // and some bounded intervals have been found (selected_index!=-1)
  if(unbounded && (!min || selected_index == -1))
  {
    double pt = min ? -codac2::oo : codac2::oo; // keep the point less/most distant from +oo (we normalize if the lower bound is -oo)
    selected_index = i;

    for(; i < this->size() ; i++)
    {
      if((this->data()+i)->lb() == -codac2::oo)
      {
        if((this->data()+i)->ub() == codac2::oo)
          if(!min)
          {
            selected_index = i;
            break;
          }

        if((min && (-(this->data()+i)->ub() > pt)) || (!min && (-(this->data()+i)->ub() < pt)))
        {
          selected_index = i;
          pt = -(this->data()+i)->ub();
        }
      }

      else if((this->data()+i)->ub() == codac2::oo)
        if((min && ((this->data()+i)->lb() > pt)) || (!min && ((this->data()+i)->lb() < pt)))
        {
          selected_index = i;
          pt = (this->data()+i)->lb();
        }
    }
  }

  return selected_index;
}

// to MatrixBase template<typename U=Scalar>
// to MatrixBase   requires IsIntervalDomain<U>
// to MatrixBase inline bool is_empty() const
// to MatrixBase {
// to MatrixBase   for(Index i = 0 ; i < this->size() ; i++)
// to MatrixBase     if((this->data()+i)->is_empty())
// to MatrixBase       return true;
// to MatrixBase   return false;
// to MatrixBase }

template<typename U=Scalar>
  requires IsIntervalDomain<U>
inline void set_empty()
{
  this->init(codac2::Interval::empty());
}

template<typename U=Scalar>
  requires IsIntervalDomain<U>
inline bool contains(const Matrix<double,RowsAtCompileTime,ColsAtCompileTime>& x) const
{
  assert_release(x.size() == this->size());

  if(this->is_empty())
    return false;

  for(Index i = 0 ; i < this->size() ; i++)
    if(!(this->data()+i)->contains(*(x.data()+i)))
      return false;

  return true;
}

template<typename U=Scalar>
  requires IsIntervalDomain<U>
inline bool interior_contains(const Matrix<double,RowsAtCompileTime,ColsAtCompileTime>& x) const
{
  assert_release(x.size() == this->size());

  if(this->is_empty())
    return false;

  for(Index i = 0 ; i < this->size() ; i++)
    if(!(this->data()+i)->interior_contains(*(x.data()+i)))
      return false;

  return true;
}

template<typename U=Scalar>
  requires IsIntervalDomain<U>
inline bool is_unbounded() const
{
  if(this->is_empty()) return false;
  for(Index i = 0 ; i < this->size() ; i++)
    if((this->data()+i)->is_unbounded())
      return true;
  return false;
}

template<typename U=Scalar>
  requires IsIntervalDomain<U>
inline bool is_degenerated() const
{
  for(Index i = 0 ; i < this->size() ; i++)
    if(!(this->data()+i)->is_degenerated())
      return false;
  return true;
}

template<typename U=Scalar>
  requires IsIntervalDomain<U>
inline bool is_flat() const
{
  if(this->is_empty()) return true;
  for(Index i = 0 ; i < this->size() ; i++)
    if((this->data()+i)->is_degenerated()) // don't use diam() because of roundoff
      return true;
  return false;
}

template<typename U=Scalar>
  requires IsIntervalDomain<U>
inline bool intersects(const Matrix<codac2::Interval,RowsAtCompileTime,ColsAtCompileTime>& x) const
{
  assert_release(this->size() == x.size());

  if(this->is_empty())
    return false;

  for(Index i = 0 ; i < this->size() ; i++)
    if(!(this->data()+i)->intersects(*(x.data()+i)))
      return false;

  return true;
}

template<typename U=Scalar>
  requires IsIntervalDomain<U>
inline bool is_disjoint(const Matrix<codac2::Interval,RowsAtCompileTime,ColsAtCompileTime>& x) const
{
  assert_release(this->size() == x.size());

  if(this->is_empty())
    return true;

  for(Index i = 0 ; i < this->size() ; i++)
    if((this->data()+i)->is_disjoint(*(x.data()+i)))
      return true;

  return false;
}

template<typename U=Scalar>
  requires IsIntervalDomain<U>
inline bool overlaps(const Matrix<codac2::Interval,RowsAtCompileTime,ColsAtCompileTime>& x) const
{
  assert_release(this->size() == x.size());

  if(this->is_empty())
    return false;

  for(Index i = 0 ; i < this->size() ; i++)
    if(!(this->data()+i)->overlaps(*(x.data()+i)))
      return false;

  return true;
}

template<typename U=Scalar>
  requires IsIntervalDomain<U>
inline bool is_subset(const Matrix<codac2::Interval,RowsAtCompileTime,ColsAtCompileTime>& x) const
{
  assert_release(this->size() == x.size());

  if(this->is_empty())
    return true;

  for(Index i = 0 ; i < this->size() ; i++)
    if(!(this->data()+i)->is_subset(*(x.data()+i)))
      return false;

  return true;
}

template<typename U=Scalar>
  requires IsIntervalDomain<U>
inline bool is_strict_subset(const Matrix<codac2::Interval,RowsAtCompileTime,ColsAtCompileTime>& x) const
{
  assert_release(this->size() == x.size());

  if(this->is_empty())
    return true;

  if(!is_subset(x))
    return false;

  for(Index i = 0 ; i < this->size() ; i++)
    if((this->data()+i)->is_strict_subset(*(x.data()+i)))
      return true;

  return false;
}

template<typename U=Scalar>
  requires IsIntervalDomain<U>
inline bool is_interior_subset(const Matrix<codac2::Interval,RowsAtCompileTime,ColsAtCompileTime>& x) const
{
  assert_release(this->size() == x.size());

  if(this->is_empty())
    return true;

  for(Index i = 0 ; i < this->size() ; i++)
    if(!(this->data()+i)->is_interior_subset(*(x.data()+i)))
      return false;

  return true;
}

template<typename U=Scalar>
  requires IsIntervalDomain<U>
inline bool is_strict_interior_subset(const Matrix<codac2::Interval,RowsAtCompileTime,ColsAtCompileTime>& x) const
{
  assert_release(this->size() == x.size());

  if(this->is_empty())
    return true;

  for(Index i = 0 ; i < this->size() ; i++)
    if(!(this->data()+i)->is_strict_interior_subset(*(x.data()+i)))
      return false;

  return true;
}

template<typename U=Scalar>
  requires IsIntervalDomain<U>
inline bool is_superset(const Matrix<codac2::Interval,RowsAtCompileTime,ColsAtCompileTime>& x) const
{
  assert_release(this->size() == x.size());

  if(this->is_empty())
    return false;

  for(Index i = 0 ; i < this->size() ; i++)
    if(!(x.data()+i)->is_subset(*(this->data()+i)))
      return false;

  return true;
}

template<typename U=Scalar>
  requires IsIntervalDomain<U>
inline bool is_strict_superset(const Matrix<codac2::Interval,RowsAtCompileTime,ColsAtCompileTime>& x) const
{
  assert_release(this->size() == x.size());

  if(this->is_empty())
    return false;

  if(!is_superset(x))
    return false;

  for(Index i = 0 ; i < this->size() ; i++)
    if((x.data()+i)->is_strict_subset(*(this->data()+i)))
      return true;

  return false;
}

template<typename U=Scalar>
  requires IsIntervalDomain<U>
inline bool is_bisectable() const
{
  for(Index i = 0 ; i < this->size() ; i++)
    if((this->data()+i)->is_bisectable())
      return true;
  return false;
}

template<typename U=Scalar>
  requires IsIntervalDomain<U>
inline auto& inflate(double r)
{
  assert_release(r >= 0.);

  for(Index i = 0 ; i < this->size() ; i++)
    (this->data()+i)->inflate(r);
  return *this;
}

template<typename U=Scalar>
  requires IsIntervalDomain<U>
inline auto& inflate(const Matrix<double,RowsAtCompileTime,ColsAtCompileTime>& r)
{
  assert_release(this->size() == r.size());
  assert_release(r.min_coeff() >= 0.);

  for(Index i = 0 ; i < this->size() ; i++)
    (this->data()+i)->inflate(*(r.data()+i));
  return *this;
}

template<typename U=Scalar,typename U_>
  requires IsIntervalDomain<U>
inline auto& operator&=(const Matrix<U_,RowsAtCompileTime,ColsAtCompileTime>& x)
{
  assert_release(this->size() == x.size());

  if constexpr(std::is_same_v<U_,codac2::Interval>)
  {
    if(x.is_empty())
    {
      set_empty();
      return *this;
    }
  }
  
  for(Index i = 0 ; i < this->size() ; i++)
    *(this->data()+i) &= *(x.data()+i);
  return *this;
}

template<typename U=Scalar,typename OtherDerived>
  requires IsIntervalDomain<U>
inline auto& operator&=(const MatrixBase<OtherDerived>& x)
{
  assert_release(this->size() == x.size());

  for(Index i = 0 ; i < this->rows() ; i++)
    for(Index j = 0 ; j < this->cols() ; j++)
      (*this)(i,j) &= x(i,j);
  return *this;
}

template<typename U=Scalar,typename U_>
  requires IsIntervalDomain<U>
inline auto& operator|=(const Matrix<U_,RowsAtCompileTime,ColsAtCompileTime>& x)
{
  assert_release(this->size() == x.size());

  if constexpr(std::is_same_v<U_,codac2::Interval>)
  {
    if(x.is_empty())
      return *this;
  }

  for(Index i = 0 ; i < this->size() ; i++)
    *(this->data()+i) |= *(x.data()+i);
  return *this;
}

template<typename U=Scalar>
  requires IsIntervalDomain<U>
inline auto operator&(const Matrix<codac2::Interval,RowsAtCompileTime,ColsAtCompileTime>& x) const
{
  auto y = *this;
  return y &= x;
}

template<typename U=Scalar>
  requires IsIntervalDomain<U>
inline auto operator|(const Matrix<codac2::Interval,RowsAtCompileTime,ColsAtCompileTime>& x) const
{
  auto y = *this;
  return y |= x;
}

template<typename U=Scalar,int R=RowsAtCompileTime,int C=ColsAtCompileTime>
  requires IsIntervalDomain<U>
inline static auto empty(Index r, Index c)
{
  assert_release(r >= 0 && c >= 0);
  Matrix<codac2::Interval,R,C> e(r,c);
  return e.init(codac2::Interval::empty());
}

template<typename U=Scalar,int R=RowsAtCompileTime,int C=ColsAtCompileTime>
  requires IsIntervalDomain<U>
inline auto bisect(Index i, float ratio = 0.49) const
{
  assert_release(i >= 0 && i < this->size());
  assert_release((this->data()+i)->is_bisectable());
  assert_release(codac2::Interval(0,1).interior_contains(ratio));

  auto p = std::make_pair(*this,*this);
  auto pi = (this->data()+i)->bisect(ratio);
  *(p.first.data()+i) = pi.first;
  *(p.second.data()+i) = pi.second;
  return p;
}

template<typename U=Scalar,int R=RowsAtCompileTime,int C=ColsAtCompileTime>
  requires IsIntervalDomain<U>
inline auto bisect_largest(float ratio = 0.49) const
{
  return bisect(max_diam_index(), ratio);
}