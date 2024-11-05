/** 
 *  \file codac2_MatrixBase.h
 * ----------------------------------------------------------------------------
 *  \date       2024
 *  \author     Simon Rohou
 *  \copyright  Copyright 2024 Codac Team
 *  \license    GNU Lesser General Public License (LGPL)
 */

#pragma once

#include <ostream>
#include "codac2_eigen.h"
#include "codac2_assert.h"
#include "codac2_MatrixBase_fwd.h"
#include "codac2_MatrixBaseBlock.h"

namespace codac2
{
  template<typename S,typename M,typename T>
  class VectorBase;

  template<typename Q,typename T>
  struct MatrixBaseBlock;

  template<typename T,int Rows=-1,int Cols=-1>
  using EigenMatrix = Eigen::Matrix<T,Rows,Cols>;

  template<typename S,typename T,int Rows,int Cols>
  class MatrixBase
  {
    public:

      explicit MatrixBase(size_t r, size_t c)
        : _e(EigenMatrix<T,Rows,Cols>(r,c))
      {
        assert((Rows == (int)r || Rows == -1) && (Cols == (int)c || Cols == -1));
        assert(r >= 0 && c >= 0);
      }

      explicit MatrixBase(size_t r, size_t c, const T& x)
        : MatrixBase<S,T,Rows,Cols>(r,c)
      {
        assert((Rows == (int)r || Rows == -1) && (Cols == (int)c || Cols == -1));
        init(x);
      }

      explicit MatrixBase(size_t r, size_t c, const T values[])
        : MatrixBase<S,T,Rows,Cols>(r,c)
      {
        assert((Rows == (int)r || Rows == -1) && (Cols == (int)c || Cols == -1));

        if(values == 0)
          init(T(0.)); // in case the user called MatrixBase(r,c,0) and 0 is interpreted as NULL!

        else
        {
          size_t k = 0;
          for(size_t i = 0 ; i < nb_rows() ; i++)
            for(size_t j = 0 ; j < nb_cols() ; j++)
              (*this)(i,j) = values[k++];
          assert(k == size());
        }
      }

      explicit MatrixBase(const std::vector<T>& x)
        : MatrixBase<S,T,-1,-1>(x.size(),1,&x[0])
      {
        assert(Rows == -1 && Cols == -1);
        assert(!x.empty());
      }

      MatrixBase(std::initializer_list<std::initializer_list<T>> l)
        : MatrixBase<S,T,-1,-1>(0,0 /* will be resized thereafter */)
      {
        assert(Rows == -1 && Cols == -1);
        assert(!std::empty(l));

        int cols = -1;
        for(const auto& ri : l) {
          assert_release((cols == -1 || cols == (int)ri.size()) && "ill-formed matrix");
          cols = (int)ri.size();
        }

        resize(l.size(),cols);
        size_t i = 0;
        for(const auto& ri : l)
        {
          size_t j = 0;
          for(const auto& ci : ri)
            (*this)(i,j++) = ci;
          i++;
        }
        // todo: use this instead (faster?) ? std::copy(l.begin(), l.end(), vec);
      }

      MatrixBase(const std::vector<std::vector<T>>& l)
        : MatrixBase<S,T,-1,-1>(0,0 /* will be resized thereafter */)
      {
        assert(Rows == -1 && Cols == -1);
        assert(!std::empty(l));

        int cols = -1;
        for(const auto& ri : l) {
          assert_release((cols == -1 || cols == (int)ri.size()) && "ill-formed matrix");
          cols = (int)ri.size();
        }

        resize(l.size(),cols);
        size_t i = 0;
        for(const auto& ri : l)
        {
          size_t j = 0;
          for(const auto& ci : ri)
            (*this)(i,j++) = ci;
          i++;
        }
        // todo: use this instead (faster?) ? std::copy(l.begin(), l.end(), vec);
      }

      template<typename OtherDerived>
      MatrixBase(const Eigen::MatrixBase<OtherDerived>& x)
        : _e(x)
      { }
      
      virtual ~MatrixBase()
      { }

      MatrixBase<S,T,Rows,Cols>& operator=(const MatrixBase<S,T,Rows,Cols>&) = default;

      size_t size() const
      {
        return _e.size();
      }

      size_t nb_rows() const
      {
        return _e.rows();
      }

      size_t nb_cols() const
      {
        return _e.cols();
      }

      bool is_squared() const
      {
        return nb_rows() == nb_cols();
      }

      #define minmax_item(op) \
        T m = *_e.data(); /* first element */ \
        for(size_t i = 1 ; i < size() ; i++) \
        { \
          if constexpr(std::is_same_v<T,Interval>) \
            m = codac2::op(m,*(_e.data()+i)); \
          else \
            m = std::op(m,*(_e.data()+i)); \
        } \
        return m; \

      T min_coeff() const
      {
        minmax_item(min);
      }

      T max_coeff() const
      {
        minmax_item(max);
      }

      friend bool operator==(const MatrixBase<S,T,Rows,Cols>& x1, const MatrixBase<S,T,Rows,Cols>& x2)
      {
        if(x1.nb_rows() != x2.nb_rows() || x1.nb_cols() != x2.nb_cols())
          return false;

        for(size_t i = 0 ; i < x1.nb_rows() ; i++)
          for(size_t j = 0 ; j < x1.nb_cols() ; j++)
            if(x1(i,j) != x2(i,j))
              return false;

        return true;
      }

      T& operator()(size_t i, size_t j)
      {
        return const_cast<T&>(const_cast<const MatrixBase<S,T,Rows,Cols>*>(this)->operator()(i,j));
      }

      const T& operator()(size_t i, size_t j) const
      {
        assert_release(i >= 0 && i < nb_rows() && j >= 0 && j < nb_cols());
        return this->_e(i,j);
      }

      void init(const T& x)
      {
        for(size_t i = 0 ; i < size() ; i++)
          *(_e.data()+i) = x;
      }

      void init(const S& x)
      {
        *this = x;
      }

      void resize(size_t nb_rows, size_t nb_cols)
      {
        assert_release(nb_rows >= 0 && nb_cols >= 0);

        // With resize() of Eigen, the data is reallocated and all previous values are lost.
        auto copy = _e;
        _e.resize(nb_rows, nb_cols);
        for(size_t i = 0 ; i < std::min((size_t)copy.rows(),nb_rows) ; i++)
          for(size_t j = 0 ; j < std::min((size_t)copy.cols(),nb_cols) ; j++)
            _e(i,j) = copy(i,j);
      }

      MatrixBaseBlock<EigenMatrix<T,Rows,Cols>&,T> block(size_t i, size_t j, size_t p, size_t q)
      {
        assert_release(i >= 0 && p > 0 && i+p <= nb_rows());
        assert_release(j >= 0 && q > 0 && j+q <= nb_cols());
        return { _e,i,j,p,q };
      }

      MatrixBaseBlock<const EigenMatrix<T,Rows,Cols>&,T> block(size_t i, size_t j, size_t p, size_t q) const
      {
        assert_release(i >= 0 && p > 0 && i+p <= nb_rows());
        assert_release(j >= 0 && q > 0 && j+q <= nb_cols());
        return { _e,i,j,p,q };
      }

      MatrixBaseBlock<EigenMatrix<T,Rows,Cols>&,T> col(size_t i)
      {
        return block(0,i,nb_rows(),1);
      }

      MatrixBaseBlock<const EigenMatrix<T,Rows,Cols>&,T> col(size_t i) const
      {
        return block(0,i,nb_rows(),1);
      }

      MatrixBaseBlock<EigenMatrix<T,Rows,Cols>&,T> row(size_t i)
      {
        return block(i,0,1,nb_cols());
      }

      MatrixBaseBlock<const EigenMatrix<T,Rows,Cols>&,T> row(size_t i) const
      {
        return block(i,0,1,nb_cols());
      }

      const auto& operator+() const
      {
        return _e;
      }

      S operator-() const
      {
        return -_e;
      }

      static S zeros(size_t r, size_t c)
      {
        assert_release(r >= 0 && c >= 0);
        return EigenMatrix<T,Rows,Cols>::Zero(r,c);
      }

      static S ones(size_t r, size_t c)
      {
        assert_release(r >= 0 && c >= 0);
        return EigenMatrix<T,Rows,Cols>::Ones(r,c);
      }

      static S eye(size_t r, size_t c)
      {
        assert_release(r >= 0 && c >= 0);
        return EigenMatrix<T,Rows,Cols>::Identity(r,c);
      }

      template<typename S_,typename T_,int Rows_,int Cols_>
      friend std::ostream& operator<<(std::ostream& os, const MatrixBase<S_,T_,Rows_,Cols_>& x);

      template<typename S_,typename T_,int Rows_,int Cols_>
      friend S_ abs(const MatrixBase<S_,T_,Rows_,Cols_>& x);

      operator EigenMatrix<T,Rows,Cols>()
      {
        return const_cast<EigenMatrix<T,Rows,Cols>&>(const_cast<const MatrixBase<S,T,Rows,Cols>*>(this)->operator EigenMatrix<T,Rows,Cols>());
      }

      operator EigenMatrix<T,Rows,Cols>() const
      {
        return _e;
      }

      using iterator = typename EigenMatrix<T,Rows,Cols>::iterator;
      using const_iterator = typename EigenMatrix<T,Rows,Cols>::const_iterator;
      
      iterator begin()
      {
        return const_cast<iterator>(const_cast<const MatrixBase<S,T,Rows,Cols>*>(this)->begin());
      }

      const_iterator begin() const
      {
        return _e.cbegin();
      }

      const_iterator end() const
      {
        return _e.cend();
      }

      iterator end()
      {
        return const_cast<iterator>(const_cast<const MatrixBase<S,T,Rows,Cols>*>(this)->end());
      }

      EigenMatrix<T,Rows,Cols> _e;
  };

  template<typename S,typename T,int Rows,int Cols>
  S abs(const MatrixBase<S,T,Rows,Cols>& x)
  {
    S a(x);

    for(size_t i = 0 ; i < x.size() ; i++)
    {
      if constexpr(std::is_same_v<T,double>)
        *(a._e.data()+i) = fabs(*(x._e.data()+i));
      else
        *(a._e.data()+i) = abs(*(x._e.data()+i));
    }

    return a;
  }

  template<typename S,typename T,int Rows,int Cols>
  std::ostream& operator<<(std::ostream& os, const MatrixBase<S,T,Rows,Cols>& x)
  {
    os << "(";
    for(size_t i = 0 ; i < x.nb_rows() ; i++)
    {
      os << (i!=0 ? " " : "") << "(";
      for(size_t j = 0 ; j < x.nb_cols() ; j++)
        os << x(i,j) << (j<x.nb_cols()-1 ? " ; " : "");
      os << ")";
      if(i < x.nb_rows()-1) os << std::endl;
    }
    os << ")";
    return os;
  }

  template<typename S,typename T,int Rows,int Cols>
  const auto& eigen(const MatrixBase<S,T,Rows,Cols>& x)
  {
    return x._e;
  }
}