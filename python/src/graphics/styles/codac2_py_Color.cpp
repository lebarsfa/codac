/** 
 *  Codac binding (core)
 * ----------------------------------------------------------------------------
 *  \date       2024
 *  \author     Simon Rohou
 *  \copyright  Copyright 2024 Codac Team
 *  \license    GNU Lesser General Public License (LGPL)
 */

#include <pybind11/pybind11.h>
#include <pybind11/operators.h>
#include <pybind11/stl.h>
#include <codac2_Color.h>
#include <codac2_StyleProperties.h>
#include "codac2_py_Color_docs.h" // Generated file from Doxygen XML (doxygen2docstring.py):

using namespace std;
using namespace codac2;
namespace py = pybind11;
using namespace pybind11::literals;


void export_Color(py::module& m)
{
  py::enum_<InterpolMode>(m, "InterpolMode")
    .value("RGB", InterpolMode::RGB)
    .value("HSV", InterpolMode::HSV)
  ;

  py::class_<Color> exported_color(m, "Color", COLOR_MAIN);
  exported_color

    .def_readwrite("data", &Color::data,
      ARRAY_FLOAT4_COLOR_DATA)

    .def_readwrite("interpol_mode", &Color::interpol_mode,
      INTERPOLMODE_COLOR_INTERPOL_MODE)

    .def(py::init<>(),COLOR_COLOR)

    .def(py::init<float,float,float,float,InterpolMode>(),
      COLOR_COLOR_FLOAT_FLOAT_FLOAT_FLOAT_INTERPOLMODE,
      "x1"_a, "x2"_a, "x3"_a, "alpha"_a, "interpol_mode"_a=InterpolMode::RGB)

    .def(py::init<float,float,float,InterpolMode>(),
      COLOR_COLOR_FLOAT_FLOAT_FLOAT_INTERPOLMODE,
      "x1"_a, "x2"_a, "x3"_a, "interpol_mode"_a=InterpolMode::RGB)

    .def(py::init<const std::array<float,3>&,InterpolMode>(),
      COLOR_COLOR_CONST_ARRAY_FLOAT3_REF_INTERPOLMODE,
      "xyz"_a, "interpol_mode"_a=InterpolMode::RGB)

    .def(py::init<const std::array<float,4>&,InterpolMode>(),
      COLOR_COLOR_CONST_ARRAY_FLOAT4_REF_INTERPOLMODE,
      "xyza"_a, "interpol_mode"_a=InterpolMode::RGB)

    .def(py::init<int,int,int,int,InterpolMode>(),
      COLOR_COLOR_INT_INT_INT_INT_INTERPOLMODE,
      "x1"_a, "x2"_a, "x3"_a, "alpha"_a, "interpol_mode"_a=InterpolMode::RGB)

    .def(py::init<int,int,int,InterpolMode>(),
      COLOR_COLOR_INT_INT_INT_INTERPOLMODE,
      "x1"_a, "x2"_a, "x3"_a, "interpol_mode"_a=InterpolMode::RGB)

    .def(py::init<const std::array<int,3>&,InterpolMode>(),
      COLOR_COLOR_CONST_ARRAY_INT3_REF_INTERPOLMODE,
      "xyz"_a, "interpol_mode"_a=InterpolMode::RGB)

    .def(py::init<const std::array<int,4>&,InterpolMode>(),
      COLOR_COLOR_CONST_ARRAY_INT4_REF_INTERPOLMODE,
      "xyza"_a, "interpol_mode"_a=InterpolMode::RGB)

    .def(py::init<const std::string&>(),
      COLOR_COLOR_CONST_STRING_REF,
      "hex_str"_a)

    // Conversions

    .def("toRGB", &Color::toRGB,
      COLOR_COLOR_TORGB_CONST)

    .def("toHSV", &Color::toHSV,
      COLOR_COLOR_TOHSV_CONST)

    // Properties

    .def("hex_str", &Color::hex_str,
      STRING_COLOR_HEX_STR_CONST)

    .def("rgb", &Color::rgb,
      ARRAY_INT3_COLOR_RGB_CONST)

    .def("rgba", &Color::rgba,
      ARRAY_INT3_COLOR_RGB_CONST)

    .def("hsv", &Color::hsv,
      ARRAY_INT3_COLOR_RGB_CONST)

    .def("hsva", &Color::hsva,
      ARRAY_INT3_COLOR_RGB_CONST)

    // Predefined colors

    .def_static("none", &Color::none,
      STATIC_COLOR_COLOR_NONE)

    .def_static("black", &Color::black,
      STATIC_COLOR_COLOR_BLACK_FLOAT,
      "alpha"_a=1.)

    .def_static("white", &Color::white,
      STATIC_COLOR_COLOR_WHITE_FLOAT,
      "alpha"_a=1.)

    .def_static("green", &Color::green,
      STATIC_COLOR_COLOR_GREEN_FLOAT,
      "alpha"_a=1.)

    .def_static("blue", &Color::blue,
      STATIC_COLOR_COLOR_BLUE_FLOAT,
      "alpha"_a=1.)

    .def_static("cyan", &Color::cyan,
      STATIC_COLOR_COLOR_CYAN_FLOAT,
      "alpha"_a=1.)

    .def_static("yellow", &Color::yellow,
      STATIC_COLOR_COLOR_YELLOW_FLOAT,
      "alpha"_a=1.)

    .def_static("red", &Color::red,
      STATIC_COLOR_COLOR_RED_FLOAT,
      "alpha"_a=1.)

    .def_static("dark_gray", &Color::dark_gray,
      STATIC_COLOR_COLOR_DARK_GRAY_FLOAT,
      "alpha"_a=1.)
  ;
}