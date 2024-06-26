/** 
 *  \file codac2_template_tools.h
 * ----------------------------------------------------------------------------
 *  \date       2024
 *  \author     Simon Rohou
 *  \copyright  Copyright 2024 Codac Team
 *  \license    GNU Lesser General Public License (LGPL)
 */

#pragma once

#include <memory>

namespace codac2
{
  inline bool same_size(const std::shared_ptr<Ctc_<IntervalVector>>& x1, const std::shared_ptr<Ctc_<IntervalVector>>& x2)
  {
    return x1->size() == x2->size();
  }

  inline bool same_size(const Ctc_<IntervalVector>& x1, const Ctc_<IntervalVector>& x2)
  {
    return x1.size() == x2.size();
  }

  inline bool same_size(const std::shared_ptr<Sep>& x1, const std::shared_ptr<Sep>& x2)
  {
    return x1->size() == x2->size();
  }

  inline bool same_size(const Sep& x1, const Sep& x2)
  {
    return x1.size() == x2.size();
  }

  template<class... T>
  bool all_same_size(const T&... x)
  {
    return (... && (same_size(std::get<0>(std::make_tuple(x...)), x)));
  }

  inline size_t size_of(const Ctc_<IntervalVector>& x)
  {
    return x.size();
  }

  inline size_t size_of(const std::shared_ptr<Ctc_<IntervalVector>>& x)
  {
    return x->size();
  }

  inline size_t size_of(const Sep& x)
  {
    return x.size();
  }

  inline size_t size_of(const std::shared_ptr<Sep>& x)
  {
    return x->size();
  }

  template<class... T>
  size_t size_first_item(const T&... x)
  {
    return size_of(std::get<0>(std::make_tuple(x...)));
  }
}