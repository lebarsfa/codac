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

  py::class_<DataRGB> exported_data_rgb(m, "DataRGB", DATARGB_MAIN);
  exported_data_rgb
    .def_readwrite("r", &DataRGB::r)
    .def_readwrite("g", &DataRGB::g)
    .def_readwrite("b", &DataRGB::b)
    .def_readwrite("a", &DataRGB::a)
    .def("to_hex_str", &DataRGB::to_hex_str, STRING_DATARGB_TO_HEX_STR_CONST)
  ;

  py::class_<DataHSV> exported_data_hsv(m, "DataHSV", DATAHSV_MAIN);
  exported_data_hsv
    .def_readwrite("h", &DataHSV::h)
    .def_readwrite("s", &DataHSV::s)
    .def_readwrite("v", &DataHSV::v)
    .def_readwrite("a", &DataHSV::a)
    .def("to_rgb", &DataHSV::to_rgb, DATARGB_DATAHSV_TO_RGB_CONST)
    .def("to_hex_str", &DataHSV::to_hex_str, STRING_DATAHSV_TO_HEX_STR_CONST)
  ;


  py::class_<Color> exported_color(m, "Color", COLOR_MAIN);
  exported_color

    .def_readwrite("data", &Color::data,
      VARIANT_STRINGDATARGBDATAHSV_COLOR_DATA)

    .def(py::init<>(),COLOR_COLOR)

    .def(py::init<float,float,float,float,InterpolMode>(),
      COLOR_COLOR_FLOAT_FLOAT_FLOAT_FLOAT_INTERPOLMODE,
      "x1"_a, "x2"_a, "x3"_a, "alpha"_a=1., "interpol_mode"_a=InterpolMode::RGB)

    .def(py::init<int,int,int,int,InterpolMode>(),
      COLOR_COLOR_INT_INT_INT_INT_INTERPOLMODE,
      "x1"_a, "x2"_a, "x3"_a, "alpha"_a=255, "interpol_mode"_a=InterpolMode::RGB)

    .def(py::init<const std::string&>(),
      COLOR_COLOR_CONST_STRING_REF,
      "hex_str"_a)

    // Properties

    .def("hex_str", &Color::hex_str,
      STRING_COLOR_HEX_STR_CONST)

    .def("r", &Color::r,
      FLOAT_COLOR_R_CONST)

    .def("g", &Color::g,
      FLOAT_COLOR_G_CONST)

    .def("b", &Color::b,
      FLOAT_COLOR_B_CONST)

    .def("alpha", &Color::alpha,
      FLOAT_COLOR_ALPHA_CONST)

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