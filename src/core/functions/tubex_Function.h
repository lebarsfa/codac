/* ============================================================================
 *  tubex-lib - Function class
 * ============================================================================
 *  Copyright : Copyright 2017 Simon Rohou
 *  License   : This program is distributed under the terms of
 *              the GNU Lesser General Public License (LGPL).
 *
 *  Author(s) : Simon Rohou
 *  Bug fixes : -
 *  Created   : 2018
 * ---------------------------------------------------------------------------- */

#ifndef __TUBEX_FUNCTION_H__
#define __TUBEX_FUNCTION_H__

#include "ibex_Function.h"
#include "tubex_Fnc.h"

namespace tubex
{
  class TubeSlice;
  
  class Function : public Fnc
  {
    public:

      Function(const char* y);
      Function(const char* x1, const char* y);
      Function(const char* x1, const char* x2, const char* y);
      Function(const char* x1, const char* x2, const char* x3, const char* y);
      Function(const char* x1, const char* x2, const char* x3, const char* x4, const char* y);
      Function(const char* x1, const char* x2, const char* x3, const char* x4, const char* x5, const char* y);
      Function(const char* x1, const char* x2, const char* x3, const char* x4, const char* x5, const char* x6, const char* y);
      Function(const char* x1, const char* x2, const char* x3, const char* x4, const char* x5, const char* x6, const char* x7, const char* y);
      Function(const char* x1, const char* x2, const char* x3, const char* x4, const char* x5, const char* x6, const char* x7, const char* x8, const char* y);
      Function(int n, const char** x, const char* y);
      Function(const tubex::Function& f);
      ~Function();
      const Function& operator=(const Function& f);

      using Fnc::eval;
      const TubeVector eval(const TubeVector& x) const;
      const ibex::IntervalVector eval(const ibex::Interval& t) const;
      const ibex::IntervalVector eval(const TubeSlice& x) const;
      const ibex::IntervalVector eval(const ibex::Interval& t, const TubeVector& x) const;

    protected:

      void construct_from_array(int n, const char** x, const char* y);

      ibex::Function *m_ibex_f = NULL;
  };
}

#endif