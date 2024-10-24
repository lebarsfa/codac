/** 
 *  Codac binding (core)
 * ----------------------------------------------------------------------------
 *  \date       2024
 *  \author     Simon Rohou, Maël Godard
 *  \copyright  Copyright 2024 Codac Team
 *  \license    GNU Lesser General Public License (LGPL)
 */

#include <pybind11/pybind11.h>
#include <pybind11/operators.h>
#include <pybind11/stl.h>
#include <codac2_ColorMap.h>
#include "codac2_py_ColorMap_docs.h" // Generated file from Doxygen XML (doxygen2docstring.py):

using namespace std;
using namespace codac2;
namespace py = pybind11;
using namespace pybind11::literals;

void export_ColorMap(py::module& m)
{
  py::class_<ColorMap> exported_colormap(m, "ColorMap", COLORMAP_MAIN);
  exported_colormap

    .def_readwrite("colormap", &ColorMap::colormap)

    .def(py::init<>(),
      COLORMAP_COLORMAP)

    .def("add_color_point", &ColorMap::add_color_point,
        VOID_COLORMAP_ADD_COLOR_POINT_COLOR_FLOAT,
        "color"_a, "index"_a)

    .def("color", &ColorMap::color,
        COLOR_COLORMAP_COLOR_FLOAT_CONST,
        "r"_a)

    // Predifined color maps

    .def_readonly_static("HAXBY", &ColorMap::HAXBY,
      CONST_COLORMAP_COLORMAP_HAXBY)

  ;
}