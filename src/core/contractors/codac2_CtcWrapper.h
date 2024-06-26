/** 
 *  \file codac2_CtcWrapper.h
 * ----------------------------------------------------------------------------
 *  \date       2024
 *  \author     Simon Rohou
 *  \copyright  Copyright 2024 Codac Team
 *  \license    GNU Lesser General Public License (LGPL)
 */

#pragma once

#include "codac2_Ctc.h"

namespace codac2
{
  template<typename X>
  class CtcWrapper_ : public Ctc_<X>
  {
    public:

      CtcWrapper_(const X& y)
        : Ctc_<X>(y.size()), _y(y)
      { }

      std::shared_ptr<Ctc> copy() const
      {
        return std::make_shared<CtcWrapper_<X>>(*this);
      }

      void contract(X& x) const
      {
        x &= _y;
      }

    protected:

      const X _y;
  };
}