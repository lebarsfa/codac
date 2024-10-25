/** 
 *  codac2_ColorMap.cpp
 * ----------------------------------------------------------------------------
 *  \date       2024
 *  \author     Simon Rohou, Maël Godard
 *  \copyright  Copyright 2024 Codac Team
 *  \license    GNU Lesser General Public License (LGPL)
 */

#include "codac2_ColorMap.h"

using namespace std;
using namespace codac2;


ColorMap::ColorMap()
  : colormap()
{}

void ColorMap::add_color_point(Color color, float index)
{
  colormap[index] = color;
}

Color ColorMap::color(float r) const
{
    assert (colormap.size() >= 2);
    if(std::isnan(r)) // undefined ratio
      return Color((float)0.5, 0.5, 0.5);
    assert(Interval(0.,1.).contains(r));

    Interval map_domain = Interval(colormap.begin()->first,prev(colormap.end())->first);
    float real_index = map_domain.lb() + r*map_domain.diam();

    if(colormap.find(real_index) == colormap.end()) // color interpolation
    {
      typename map<float,Color>::const_iterator it_ub;
      it_ub = colormap.lower_bound(real_index);
      Color color_lb = prev(it_ub)->second;
      Color color_ub = it_ub->second;

      float local_ratio = (real_index - prev(it_ub)->first) / (it_ub->first - prev(it_ub)->first);
 
    return Color((float)(color_lb.r + (color_ub.r - color_lb.r) * local_ratio),
                    (float)(color_lb.g + (color_ub.g - color_lb.g) * local_ratio),
                    (float)(color_lb.b + (color_ub.b - color_lb.b) * local_ratio),
                    (float)(color_lb.alpha + (color_ub.alpha - color_lb.alpha) * local_ratio));

    }

    else // color key
      return colormap.at(real_index);
}

ColorMap make_haxby()
  {
    ColorMap map;
    map.add_color_point(Color(39,90,211), 0);
    map.add_color_point(Color(40,123,245), 1);
    map.add_color_point(Color(45,155,253), 2);
    map.add_color_point(Color(73,209,255), 3);
    map.add_color_point(Color(100,230,254), 4);
    map.add_color_point(Color(118,235,226), 5);
    map.add_color_point(Color(135,236,187), 6);
    map.add_color_point(Color(194,252,165), 7);
    map.add_color_point(Color(217,251,151), 8);
    map.add_color_point(Color(233,241,131), 9);
    map.add_color_point(Color(252,201,96), 10);
    map.add_color_point(Color(255,184,84), 11);
    map.add_color_point(Color(255,170,75), 12);
    map.add_color_point(Color(255,167,83), 13);
    map.add_color_point(Color(255,200,158), 14);
    map.add_color_point(Color(255,233,217), 15);
    return map;
  }

const ColorMap ColorMap::HAXBY = make_haxby();

ColorMap make_blue_tube()
{
  ColorMap map;
  map.add_color_point(Color(76,110,127), 0.);
  map.add_color_point(Color(136,197,228), 1.);
  return map;
}

const ColorMap ColorMap::BLUE_TUBE = make_blue_tube();

ColorMap make_red_tube()
{
  ColorMap map;
  map.add_color_point(Color(169,55,0), 0.);
  map.add_color_point(Color(241,140,54), 1.);
  return map;
}

const ColorMap ColorMap::RED_TUBE = make_red_tube();