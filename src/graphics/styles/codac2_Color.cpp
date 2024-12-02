/** 
 *  codac2_Color.cpp
 * ----------------------------------------------------------------------------
 *  \date       2024
 *  \author     Simon Rohou, Maël Godard
 *  \copyright  Copyright 2024 Codac Team
 *  \license    GNU Lesser General Public License (LGPL)
 */

#include "codac2_Color.h"

using namespace std;
using namespace codac2;

Color::Color()
{ }

Color::Color(float x1, float x2, float x3, float alpha,InterpolMode interpol_mode)
  :data({x1,x2,x3,alpha}), interpol_mode(interpol_mode)
{ 
  assert(x1 >= 0. && x1 <= 1. && x2 >= 0. && x2 <= 1. && x3 >= 0. && x3 <= 1. && alpha >= 0. && alpha <= 1.);
}

Color::Color(double x1, double x2, double x3, double alpha, InterpolMode interpol_mode)
  : Color((float)x1, (float)x2, (float)x3, (float)alpha, interpol_mode)
{ }

Color::Color(float x1, float x2, float x3, InterpolMode interpol_mode)
  : Color(x1, x2, x3, (float) 1., interpol_mode)
{ }

Color::Color(double x1, double x2, double x3, InterpolMode interpol_mode)
  : Color((float)x1, (float)x2, (float) x3, (float) 1., interpol_mode)
{ }

Color::Color(const std::array<float,3>& xyz, InterpolMode interpol_mode)
  : Color(xyz[0], xyz[1], xyz[2],(float) 1., interpol_mode)
{ }

Color::Color(const std::array<float,4>& xyza, InterpolMode interpol_mode)
  : Color(xyza[0], xyza[1], xyza[2], xyza[3], interpol_mode)
{ }

Color::Color(int x1, int x2, int x3, int alpha,InterpolMode interpol_mode)
: Color(interpol_mode == InterpolMode::RGB ? (float) (x1/255.) : (float) (x1/360.),
        interpol_mode == InterpolMode::RGB ? (float) (x2/255.) : (float) (x2/100.),
        interpol_mode == InterpolMode::RGB ? (float) (x3/255.) : (float) (x3/100.),
        interpol_mode == InterpolMode::RGB ? (float) (alpha/255.) : (float) (alpha/100.),
        interpol_mode)
{
  if (interpol_mode == InterpolMode::RGB)
  {
    assert(x1 >= 0 && x1 <= 255 && x2 >= 0 && x2 <= 255 && x3 >= 0 && x3 <= 255 && alpha >= 0 && alpha <= 255);
  }
  else
  {
    assert(x1 >= 0 && x1 <= 360 && x2 >= 0 && x2 <= 100 && x3 >= 0 && x3 <= 100 && alpha >= 0 && alpha <= 100);
  }
    
}

Color::Color(int x1, int x2, int x3, InterpolMode interpol_mode)
  : Color(x1, x2, x3,interpol_mode == InterpolMode::RGB ? 255 : 100, interpol_mode)
{ }

Color::Color(const std::array<int,3>& xyz, InterpolMode interpol_mode)
  : Color(xyz[0], xyz[1], xyz[2], interpol_mode == InterpolMode::RGB ? 255 : 100, interpol_mode)
{ }

Color::Color(const std::array<int,4>& xyza, InterpolMode interpol_mode)
  : Color(xyza[0], xyza[1], xyza[2], xyza[3], interpol_mode)
{ }

Color::Color(const std::string& hex_str)
{
  assert(hex_str.size() == 7 || hex_str.size() == 9);
  assert(hex_str[0] == '#');

  interpol_mode = InterpolMode::RGB;

  int red,green,blue,a;
  std::istringstream(hex_str.substr(1,2)) >> std::hex >> red;
  std::istringstream(hex_str.substr(3,2)) >> std::hex >> green;
  std::istringstream(hex_str.substr(5,2)) >> std::hex >> blue;
  data[0] = (float) (red/255.);
  data[1] = (float) (green/255.);
  data[2] = (float) (blue/255.);

  // Alpha (transparency) component may be appended to the #hexa notation.
  // Value is '1' (max opacity) by default.
  if(hex_str.size() == 9)
  {
    std::istringstream(hex_str.substr(7,2)) >> std::hex >> a;
    data[3] = (float) (a/255.);
  }
}

Color Color::toRGB() const
{
  if (interpol_mode==InterpolMode::RGB)
    return *this;
  else
  {
    float r = 1., g = 1., b = 1.;

    int i = static_cast<int>(data[0] * 6);
    float f = (data[0] * 6) - i;
    i = i % 6;

    float p = data[2] * (1 - data[1]);
    float q = data[2] * (1 - f * data[1]);
    float t = data[2] * (1 - (1 - f) * data[1]);

    switch (i) {
        case 0: r = data[2]; g = t; b = p; break;
        case 1: r = q; g = data[2]; b = p; break;
        case 2: r = p; g = data[2]; b = t; break;
        case 3: r = p; g = q; b = data[2]; break;
        case 4: r = t; g = p; b = data[2]; break;
        case 5: r = data[2]; g = p; b = q; break;
    }

    return Color(r, g, b, data[3],InterpolMode::RGB);
  }
}

Color Color::toHSV() const
{
  if (interpol_mode==InterpolMode::HSV)
    return *this;
  else
  {
    float c_max = std::max({data[0], data[1], data[2]});
    float c_min = std::min({data[0], data[1], data[2]});
    float delta = c_max - c_min;

    float h = 0.0;
    if (delta != 0) {
        if (c_max == data[0]) {
            h = fmod((data[1] - data[2]) / delta, 6.0);
        } else if (c_max == data[1]) {
            h = (data[2] - data[0]) / delta + 2.0;
        } else if (c_max == data[2]) {
            h = (data[0] - data[1]) / delta + 4.0;
        }
        h /= 6.0;
        if (h < 0) {
            h += 1.0;
        }
    }

    float s = (c_max == 0) ? 0 : (delta / c_max);

    float v = c_max;

    return Color(h, s, v, data[3],InterpolMode::HSV);
  }
}

std::string Color::hex_str() const
{
  if (interpol_mode == InterpolMode::RGB)
    {
      std::stringstream s;
      s << std::hex << std::setfill('0');
      s << std::setw(2) << (int)(data[0]*255) << std::setw(2) << (int)(data[1]*255) << std::setw(2) << (int)(data[2]*255);
      if(data[3] != 1.)
        s << std::setw(2) << (int)(data[3]*255);
      return "#"+s.str();
    }
  else
    return toRGB().hex_str();
}

std::array<int,3> Color::rgb() const
{
  if (interpol_mode == InterpolMode::RGB)
    return { (int)(data[0]*255), (int)(data[1]*255), (int)(data[2]*255) };
  else
    return toRGB().rgb();
}

std::array<int,4> Color::rgba() const
{
  if (interpol_mode == InterpolMode::RGB)
    return { (int)(data[0]*255), (int)(data[1]*255), (int)(data[2]*255), (int)(data[3]*255) };
  else
    return toRGB().rgba();
}

std::array<int,3> Color::hsv() const
{
  if (interpol_mode == InterpolMode::HSV)
    return { (int)(data[0]*360), (int)(data[1]*100), (int)(data[2]*100) };
  else
    return toHSV().hsv();
}

std::array<int,4> Color::hsva() const
{
  if (interpol_mode == InterpolMode::HSV)
    return { (int)(data[0]*360), (int)(data[1]*100), (int)(data[2]*100), (int)(data[3]*100) };
  else
    return toHSV().hsva();
}