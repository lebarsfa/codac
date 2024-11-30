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
#include <cmath>
#include "codac2_Color.h"
#include "codac2_Interval.h"
#include"codac2_assert.h"

namespace codac2
{
  /**
   * \struct ColorMap
   * \brief Represents a set of RGB values
   */
  struct ColorMap
  {

    ColorMap();

    std::map<float,Color> colormap; //!< map of colors

    void add_color_point(Color color, float index);

    Color color (float r) const;

    static const ColorMap HAXBY; //!< predefined HAXBY color map (mainly used for DEM)
    static const ColorMap DEFAULT; //!< a predefined default color map
    static const ColorMap BLUE_TUBE; //!< a predefined color map for tubes
    static const ColorMap RED_TUBE; //!< a predefined color map for tubes
    static const ColorMap RAINBOW; //!< a predefined color map
  };
}