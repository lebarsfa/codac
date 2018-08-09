/* ============================================================================
 *  tubex-lib - EmptyException class
 * ============================================================================
 *  Copyright : Copyright 2017 Simon Rohou
 *  License   : This program is distributed under the terms of
 *              the GNU Lesser General Public License (LGPL).
 *
 *  Author(s) : Simon Rohou
 *  Bug fixes : -
 *  Created   : 2015
 * ---------------------------------------------------------------------------- */

#ifndef __TUBEX_EMPTYEXCEPTION_H__
#define __TUBEX_EMPTYEXCEPTION_H__

#include "tubex_Exception.h"
#include "tubex_Slice.h"
#include "tubex_Tube.h"
#include "tubex_TubeVector.h"

namespace tubex
{
  /**
   * \brief Emptiness error exception.
   *
   * Thrown when a tube is empty.
   */
  class EmptyException : public Exception
  {
    public:

      EmptyException(const Slice& x);
      EmptyException(const Tube& x);
      EmptyException(const TubeVector& x);

      static void check(const Slice& x);
      static void check(const Tube& x);
      static void check(const TubeVector& x);
  };
}

#endif