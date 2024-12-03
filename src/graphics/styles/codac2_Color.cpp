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
  : m(RGB)
{ 
  (*this)[0] = 0.;
  (*this)[1] = 0.;
  (*this)[2] = 0.;
  (*this)[3] = 0.;
}

Color::Color(const std::array<float,3>& xyz, Model m_)
  : m(m_)
{
  (*this)[0] = xyz[0];
  (*this)[1] = xyz[1];
  (*this)[2] = xyz[2];
  (*this)[3] = (m == RGB ? 255. : 100.);
}

Color::Color(const std::array<float,4>& xyza, Model m_)
  : m(m_)
{
  if (m_==RGB)
    assert(xyza[0] >= 0. && xyza[0] <= 255. && xyza[1]>=0. && xyza[1] <= 255. && xyza[2]>=0. && xyza[2] <= 255. && xyza[3]>=0. && xyza[3] <= 255.);
  else if (m_==HSV)
    assert(xyza[0] >= 0. && xyza[0] <= 360. && xyza[1]>=0. && xyza[1] <= 100. && xyza[2]>=0. && xyza[2] <= 100. && xyza[3]>=0. && xyza[3] <= 100.);
  (*this)[0] = xyza[0];
  (*this)[1] = xyza[1];
  (*this)[2] = xyza[2];
  (*this)[3] = xyza[3];
}

Color::Color(const std::initializer_list<float> xyza, Model m_)
  : Color(xyza.size() == 3 ? Color(to_array<3>(xyza), m_) : Color(to_array<4>(xyza), m_)) 
{ }

Color::Color(const std::string& hex_str)
{
  assert(hex_str.size() == 7 || hex_str.size() == 9);
  assert(hex_str[0] == '#');

  m = RGB;

  int red,green,blue,a;
  std::istringstream(hex_str.substr(1,2)) >> std::hex >> red;
  std::istringstream(hex_str.substr(3,2)) >> std::hex >> green;
  std::istringstream(hex_str.substr(5,2)) >> std::hex >> blue;
  (*this)[0] = (float) (red);
  (*this)[1] = (float) (green);
  (*this)[2] = (float) (blue);

  // Alpha (transparency) component may be appended to the #hexa notation.
  // Value is '255.' (max opacity) by default.
  if(hex_str.size() == 9)
  {
    std::istringstream(hex_str.substr(7,2)) >> std::hex >> a;
    (*this)[3] = (float) (a);
  }
  else
    (*this)[3] = 255.;
}

Color Color::rgb() const
{
  if (m==RGB)
    return *this;
  else
  {
    float r = 0., g = 0., b = 0.;

    // Normalisation des valeurs
    float h = (*this)[0] / 360.; // Hue normalisée (0 à 1)
    float s = (*this)[1] / 100.; // Saturation normalisée (0 à 1)
    float v = (*this)[2] / 100.; // Value normalisée (0 à 1)

    int i = static_cast<int>(h * 6);
    float f = (h * 6) - i;
    i = i % 6;

    float p = v * (1 - s);
    float q = v * (1 - f * s);
    float t = v * (1 - (1 - f) * s);

    switch (i) {
        case 0: r = v; g = t; b = p; break;
        case 1: r = q; g = v; b = p; break;
        case 2: r = p; g = v; b = t; break;
        case 3: r = p; g = q; b = v; break;
        case 4: r = t; g = p; b = v; break;
        case 5: r = v; g = p; b = q; break;
    }

    // Conversion vers l'échelle [0, 255]
    r *= 255.;
    g *= 255.;
    b *= 255.;

    return Color({r, g, b,(float) ((*this)[3]*2.55)},RGB);
  }
}

Color Color::hsv() const
{
  if (m==HSV)
    return *this;
  else
  {
    float c_max = std::max({(*this)[0], (*this)[1], (*this)[2]});
    float c_min = std::min({(*this)[0], (*this)[1], (*this)[2]});
    float delta = c_max - c_min;

    float h = 0.0;
    if (delta != 0) {
        if (c_max == (*this)[0]) {
            h = fmod(((*this)[1] - (*this)[2]) / delta, 6.0);
        } else if (c_max == (*this)[1]) {
            h = ((*this)[2] - (*this)[0]) / delta + 2.0;
        } else if (c_max == (*this)[2]) {
            h = ((*this)[0] - (*this)[1]) / delta + 4.0;
        }
        h /= 6.0;
        if (h < 0) {
            h += 1.0;
        }
    }

    float s = (c_max == 0) ? 0 : (delta / c_max);

    float v = c_max;

    h*=360.;
    s*=100.;
    v*=100.;

    return Color({h, s, v,(float) ((*this)[3]/2.55)},HSV);
  }
}

std::string Color::hex_str() const
{
  if (m == RGB)
    {
      std::stringstream s;
      s << std::hex << std::setfill('0');
      s << std::setw(2) << (int)((*this)[0]) << std::setw(2) << (int)((*this)[1]) << std::setw(2) << (int)((*this)[2]);
      if((*this)[3] != 1.)
        s << std::setw(2) << (int)((*this)[3]);
      return "#"+s.str();
    }
  else
    return rgb().hex_str();
}