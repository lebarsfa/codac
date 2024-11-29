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

std::string DataRGB::to_hex_str() const
{
  stringstream ss;
  ss << "#" << hex << setfill('0') << setw(2) << (int)(r*255) << setw(2) << (int)(g*255) << setw(2) << (int)(b*255) << setw(2) << (int)(a*255);
  return ss.str();
}

DataRGB DataHSV::to_rgb() const
{
  float r = 1., g = 1., b = 1.;

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

  return DataRGB{r, g, b, a};
}

std::string DataHSV::to_hex_str() const
{
  return to_rgb().to_hex_str();
}

Color::Color()
  : data("#FFFFFFFF")
{}

Color::Color(float x1, float x2, float x3, float alpha,InterpolMode interpol_mode)
{ 
  assert(x1 >= 0. && x1 <= 1. && x2 >= 0. && x2 <= 1. && x3 >= 0. && x3 <= 1. && alpha >= 0. && alpha <= 1.);
  if(interpol_mode == InterpolMode::RGB)
    data = DataRGB{x1, x2, x3, alpha};
  else
    data = DataHSV{x1, x2, x3, alpha};
}

Color::Color(int x1, int x2, int x3, int alpha,InterpolMode interpol_mode)
  : Color((float)(x1/255.), (float)(x2/255.), (float)(x3/255.), (float)(alpha/255.),interpol_mode)
{
  assert(x1 >= 0 && x1 <= 255 && x2 >= 0 && x2 <= 255 && x3 >= 0 && x3 <= 255 && alpha >= 0 && alpha <= 255);
}

Color::Color(const std::string& hex_str)
  : data(hex_str)
{
  assert(hex_str.size() == 7 || hex_str.size() == 9);
  assert(hex_str[0] == '#');
}

std::string Color::hex_str() const
{
  switch(data.index())
  {
    case 0: return std::get<0>(data);
    case 1: return std::get<1>(data).to_hex_str();
    case 2: return std::get<2>(data).to_hex_str();
  }
  return "";
}

float Color::r() const
{
  switch(data.index())
  {
    case 0:
      int r;
      std::istringstream(std::get<0>(data).substr(1,2)) >> std::hex >> r;
      return (float) r/255.;
    case 1: return std::get<1>(data).r;
    case 2: return std::get<2>(data).to_rgb().r;
  }
  return 0.;
}

float Color::g() const
{
  switch(data.index())
  {
    case 0:
      int g;
      std::istringstream(std::get<0>(data).substr(3,2)) >> std::hex >> g;
      return (float) g/255.;
    case 1: return std::get<1>(data).g;
    case 2: return std::get<2>(data).to_rgb().g;
  }
  return 0.;
}

float Color::b() const
{
  switch(data.index())
  {
    case 0:
      int b;
      std::istringstream(std::get<0>(data).substr(5,2)) >> std::hex >> b;
      return (float) b/255.;
    case 1: return std::get<1>(data).b;
    case 2: return std::get<2>(data).to_rgb().b;
  }
  return 0.;
}

float Color::alpha() const
{
  switch(data.index())
  {
    case 0: 
      float alpha;
      if (std::get<0>(data).size() == 7)
        return 1.;
      else
      {
        std::istringstream(std::get<0>(data).substr(7,2)) >> std::hex >> alpha;
        return alpha/255.;
      }
    case 1: return std::get<1>(data).a;
    case 2: return std::get<2>(data).a;
  }
  return 0.;
}