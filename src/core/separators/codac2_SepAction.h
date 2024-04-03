/** 
 *  \file
 *  SepAction class
 * ----------------------------------------------------------------------------
 *  \date       2024
 *  \author     Simon Rohou
 *  \copyright  Copyright 2024 Codac Team
 *  \license    This program is distributed under the terms of
 *              the GNU Lesser General Public License (LGPL).
 */

#pragma once

#include <type_traits>
#include "codac2_Sep.h"
#include "codac2_Collection.h"
#include "codac2_OctaSym.h"

namespace codac2
{
  class SepAction : public Sep
  {
    public:

      template<typename S, typename = typename std::enable_if<
          std::is_base_of_v<Sep,S>
        >::type>
      SepAction(const S& s, const OctaSym& a)
        : _sep(s), _s(a), __s(a.invert())
      { }

      virtual std::shared_ptr<Sep> copy() const;
      virtual BoxPair separate(const IntervalVector& x) const;

    protected:

      const Collection<Sep> _sep;
      const OctaSym _s, __s;
  };
  
  template<typename S, typename = typename std::enable_if<
      std::is_base_of_v<Sep,S>
    >::type>
  inline SepAction OctaSym::operator()(const S& s) const
  {
    return SepAction(s, *this);
  }
}