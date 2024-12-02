/** 
 *  \file codac2_Color.h
 * ----------------------------------------------------------------------------
 *  \date       2024
 *  \author     Simon Rohou, Maël Godard
 *  \copyright  Copyright 2024 Codac Team
 *  \license    GNU Lesser General Public License (LGPL)
 */

#pragma once

#include <string>
#include <iomanip>
#include <math.h> 
#include"codac2_assert.h"


namespace codac2
{
  enum class InterpolMode
  {
    RGB = 0x01,
    HSV = 0x02
  };

  struct Color
  {
    std::array<float,4> data = {0.,0.,0.,1.0}; // RGB or HSV values + alpha, between 0 and 1
    InterpolMode interpol_mode = InterpolMode::RGB;//RGB or HSV

    // Constructors

    explicit Color();
    explicit Color(float x1, float x2, float x3, float alpha, InterpolMode interpol_mode = InterpolMode::RGB); // RGBA constructor
    explicit Color(double x1, double x2, double x3, double alpha, InterpolMode interpol_mode = InterpolMode::RGB);
    explicit Color(float x1, float x2, float x3, InterpolMode interpol_mode = InterpolMode::RGB); // RGB constructor
    explicit Color(double x1, double x2, double x3, InterpolMode interpol_mode = InterpolMode::RGB);
    explicit Color(const std::array<float,3>& xyz, InterpolMode interpol_mode = InterpolMode::RGB);
    explicit Color(const std::array<float,4>& xyza, InterpolMode interpol_mode = InterpolMode::RGB);
    explicit Color(int x1, int x2, int x3, int alpha,InterpolMode interpol_mode = InterpolMode::RGB); 
    explicit Color(int x1, int x2, int x3, InterpolMode interpol_mode = InterpolMode::RGB);
    explicit Color(const std::array<int,3>& xyz, InterpolMode interpol_mode = InterpolMode::RGB);
    explicit Color(const std::array<int,4>& xyza, InterpolMode interpol_mode = InterpolMode::RGB);
    explicit Color(const std::string& hex_str);

    //Conversions

    Color toRGB() const;
    Color toHSV() const;

    // Properties

    std::string hex_str() const;
    std::array<int,3> rgb() const;
    std::array<int,4> rgba() const;
    std::array<int,3> hsv() const;
    std::array<int,4> hsva() const;

    // Predefined colors

    static Color none()                      { return Color(255, 255, 255, 0               ); };
    static Color black(float alpha = 1)      { return Color(0,   0,   0,   (int)(alpha*255)); };
    static Color white(float alpha = 1)      { return Color(255, 255, 255, (int)(alpha*255)); };
    static Color green(float alpha = 1)      { return Color(144, 242, 0,   (int)(alpha*255)); };
    static Color blue(float alpha = 1)       { return Color(0,   98,  198, (int)(alpha*255)); };
    static Color cyan(float alpha = 1)       { return Color(75,  207, 250, (int)(alpha*255)); };
    static Color yellow(float alpha = 1)     { return Color(255, 211, 42,  (int)(alpha*255)); };
    static Color red(float alpha = 1)        { return Color(209, 59,  0,   (int)(alpha*255)); };
    static Color dark_gray(float alpha = 1)  { return Color(112, 112, 112, (int)(alpha*255)); };
    static Color purple(float alpha = 1)     { return Color(154, 0,   170, (int)(alpha*255)); };
    static Color dark_green(float alpha = 1) { return Color(94,  158, 0,   (int)(alpha*255)); };
  };
}