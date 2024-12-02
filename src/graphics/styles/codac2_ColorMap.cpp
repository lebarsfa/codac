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
      return Color(0.5, 0.5, 0.5);
    assert(Interval(0.,1.).contains(r));

    Interval map_domain = Interval(colormap.begin()->first,prev(colormap.end())->first);
    float real_index = map_domain.lb() + r*map_domain.diam();

    if(colormap.find(real_index) == colormap.end()) // color interpolation
    {
      typename map<float,Color>::const_iterator it_ub;
      it_ub = colormap.lower_bound(real_index);
      Color color_lb = prev(it_ub)->second.toRGB();
      Color color_ub = it_ub->second.toRGB();

      float local_ratio = (real_index - prev(it_ub)->first) / (it_ub->first - prev(it_ub)->first);
 
    return Color((float)(color_lb.data[0] + (color_ub.data[0] - color_lb.data[0]) * local_ratio),
                    (float)(color_lb.data[1] + (color_ub.data[1] - color_lb.data[1]) * local_ratio),
                    (float)(color_lb.data[2] + (color_ub.data[2] - color_lb.data[2]) * local_ratio),
                    (float)(color_lb.data[3] + (color_ub.data[3] - color_lb.data[3]) * local_ratio));

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

ColorMap make_default()
  {
    ColorMap map;
    map.add_color_point(Color(10,0,121), 0);
    map.add_color_point(Color(40,0,150), 1);
    map.add_color_point(Color(20,5,175), 2);
    map.add_color_point(Color(0,10,200), 3);
    map.add_color_point(Color(0,25,212), 4);
    map.add_color_point(Color(0,40,224), 5);
    map.add_color_point(Color(26,102,240), 6);
    map.add_color_point(Color(13,129,248), 7);
    map.add_color_point(Color(25,175,255), 8);
    map.add_color_point(Color(50,190,255), 9);
    map.add_color_point(Color(68,202,255), 10);
    map.add_color_point(Color(97,225,240), 11);
    map.add_color_point(Color(106,235,225), 12);
    map.add_color_point(Color(124,235,200), 13);
    map.add_color_point(Color(138,236,174), 14);
    map.add_color_point(Color(172,245,168), 15);
    map.add_color_point(Color(205,255,162), 16);
    map.add_color_point(Color(223,245,141), 17);
    map.add_color_point(Color(240,236,121), 18);
    map.add_color_point(Color(247,215,104), 19);
    map.add_color_point(Color(255,189,87), 20);
    map.add_color_point(Color(255,160,69), 21);
    map.add_color_point(Color(244,117,75), 22);
    map.add_color_point(Color(238,80,78), 23);
    map.add_color_point(Color(255,90,90), 24);
    map.add_color_point(Color(255,124,124), 25);
    map.add_color_point(Color(255,158,158), 26);
    map.add_color_point(Color(245,179,174), 27);
    map.add_color_point(Color(255,196,196), 28);
    map.add_color_point(Color(255,215,215), 29);
    map.add_color_point(Color(255,235,235), 31);
    map.add_color_point(Color(255,254,253), 32);
    return map;
  }

  const ColorMap ColorMap::DEFAULT = make_default();

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

ColorMap make_rainbow()
  {
    ColorMap map;
    for(int h = 300 ; h > 0 ; h-=10)
      map.add_color_point(Color(h,100,100,100,InterpolMode::HSV), (300.-h)/300.);
    return map;
  }

  const ColorMap ColorMap::RAINBOW = make_rainbow();