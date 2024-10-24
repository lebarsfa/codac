/** 
 *  \file codac2_ColorMap.h
 * ----------------------------------------------------------------------------
 *  \date       2024
 *  \author     Simon Rohou, Maël Godard
 *  \copyright  Copyright 2024 Codac Team
 *  \license    GNU Lesser General Public License (LGPL)
 */

#pragma once

#include <map>
#include "codac2_Color.h"

namespace codac2
{
  /**
   * \struct ColorMap
   * \brief Represents a set of RGB values
   */
  struct ColorMap
  {
    std::map<float,Color> m_colormap; //!< map of colors

    void add_color_point(Color color, float index);

    static ColorMap haxby();
  };
}