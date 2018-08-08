/* ============================================================================
 *  tubex-lib - CtcHC4 class
 * ============================================================================
 *  Copyright : Copyright 2017 Simon Rohou
 *  License   : This program is distributed under the terms of
 *              the GNU Lesser General Public License (LGPL).
 *
 *  Author(s) : Simon Rohou
 *  Bug fixes : -
 *  Created   : 2015
 * ---------------------------------------------------------------------------- */

#ifndef __TUBEX_CTCHC4_H__
#define __TUBEX_CTCHC4_H__

#include "tubex_Ctc.h"
#include "ibex_CtcHC4.h"
#include "tubex_Slice.h"

namespace tubex
{
  /**
   * \brief CtcHC4 class.
   */
  class CtcHC4 : Ctc
  {
    public:

      CtcHC4(bool preserve_slicing = false);
      bool contract(ibex::CtcHC4& hc4, TubeVector& x) const;
      bool contract(ibex::CtcHC4& hc4, Slice& x) const;

    protected:

  };
}

#endif