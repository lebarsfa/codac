/** 
 *  IntervalMatrixBase binding
 * ----------------------------------------------------------------------------
 *  \date       2024
 *  \author     Simon Rohou
 *  \copyright  Copyright 2024 Codac Team
 *  \license    GNU Lesser General Public License (LGPL)
 */

#pragma once

#include <sstream>
#include <pybind11/pybind11.h>
#include <pybind11/operators.h>
#include <pybind11/stl.h>

#include "codac2_py_MatrixBase.h"

using namespace std;
using namespace codac2;
namespace py = pybind11;
using namespace pybind11::literals;

template<typename S,typename V,bool VECTOR_INHERITANCE>
void export_IntervalMatrixBase(py::module& m, py::class_<S>& pyclass)
{
  export_MatrixBase<S,Interval,VECTOR_INHERITANCE>(m, pyclass);

  pyclass

    .def("volume", &S::volume,
      INTERVALMATRIXBASE_EIGENADDONS_DOUBLE_VOLUME_CONST)

    .def("is_empty", &S::is_empty,
      EIGENADDONS_TEST_BOOL_IS_EMPTY_CONST)

    .def("set_empty", &S::set_empty,
      INTERVALMATRIXBASE_EIGENADDONS_VOID_SET_EMPTY)

    .def("lb", [](const S& x) { return x.lb(); },
      INTERVALMATRIXBASE_EIGENADDONS_AUTO_LB_CONST)

    .def("ub", [](const S& x) { return x.ub(); },
      INTERVALMATRIXBASE_EIGENADDONS_AUTO_UB_CONST)

    .def("mid", [](const S& x) { return x.mid(); },
      INTERVALMATRIXBASE_EIGENADDONS_AUTO_MID_CONST)

    .def("mag", [](const S& x) { return x.mag(); },
      INTERVALMATRIXBASE_EIGENADDONS_AUTO_MAG_CONST)

    .def("mig", [](const S& x) { return x.mig(); },
      INTERVALMATRIXBASE_EIGENADDONS_AUTO_MIG_CONST)

    .def("rand", [](const S& x) { return x.rand(); },
      INTERVALMATRIXBASE_EIGENADDONS_AUTO_RAND_CONST)

    .def("rad", [](const S& x) { return x.rad(); },
      INTERVALMATRIXBASE_EIGENADDONS_AUTO_RAD_CONST)

    .def("diam", [](const S& x) { return x.diam(); },
      INTERVALMATRIXBASE_EIGENADDONS_AUTO_DIAM_CONST)

    .def("min_diam", [](const S& x) { return x.min_diam(); },
      INTERVALMATRIXBASE_EIGENADDONS_DOUBLE_MIN_DIAM_CONST)

    .def("max_diam", [](const S& x) { return x.max_diam(); },
      INTERVALMATRIXBASE_EIGENADDONS_DOUBLE_MAX_DIAM_CONST)

    .def("min_diam_index", [](const S& x)
        {
          return matlab::output_index(x.min_diam_index());
        },
      INTERVALMATRIXBASE_EIGENADDONS_SIZET_MIN_DIAM_INDEX_CONST)

    .def("max_diam_index", [](const S& x)
        {
          return matlab::output_index(x.max_diam_index());
        },
      INTERVALMATRIXBASE_EIGENADDONS_SIZET_MAX_DIAM_INDEX_CONST)

    .def("extr_diam_index", [](const S& x, bool min)
        {
          return matlab::output_index(x.extr_diam_index(min));
        },
      INTERVALMATRIXBASE_EIGENADDONS_SIZET_EXTR_DIAM_INDEX_BOOL_CONST,
      "min"_a)

    .def("__contains__", &S::contains,
      INTERVALMATRIXBASE_EIGENADDONS_BOOL_CONTAINS_CONST_MATRIX_DOUBLEROWSATCOMPILETIMECOLSATCOMPILETIME_REF_CONST)

    .def("contains", &S::contains,
      INTERVALMATRIXBASE_EIGENADDONS_BOOL_CONTAINS_CONST_MATRIX_DOUBLEROWSATCOMPILETIMECOLSATCOMPILETIME_REF_CONST)

    .def("interior_contains", &S::interior_contains,
      INTERVALMATRIXBASE_EIGENADDONS_BOOL_INTERIOR_CONTAINS_CONST_MATRIX_DOUBLEROWSATCOMPILETIMECOLSATCOMPILETIME_REF_CONST)

    .def("is_unbounded", &S::is_unbounded,
      INTERVALMATRIXBASE_EIGENADDONS_BOOL_IS_UNBOUNDED_CONST)

    .def("is_degenerated", &S::is_degenerated,
      INTERVALMATRIXBASE_EIGENADDONS_BOOL_IS_DEGENERATED_CONST)

    .def("is_flat", &S::is_flat,
      INTERVALMATRIXBASE_EIGENADDONS_BOOL_IS_FLAT_CONST)

    .def("intersects", &S::intersects,
      INTERVALMATRIXBASE_EIGENADDONS_BOOL_INTERSECTS_CONST_MATRIX_INTERVALROWSATCOMPILETIMECOLSATCOMPILETIME_REF_CONST)

    .def("is_disjoint", &S::is_disjoint,
      INTERVALMATRIXBASE_EIGENADDONS_BOOL_IS_DISJOINT_CONST_MATRIX_INTERVALROWSATCOMPILETIMECOLSATCOMPILETIME_REF_CONST)

    .def("overlaps", &S::overlaps,
      INTERVALMATRIXBASE_EIGENADDONS_BOOL_OVERLAPS_CONST_MATRIX_INTERVALROWSATCOMPILETIMECOLSATCOMPILETIME_REF_CONST)

    .def("is_subset", &S::is_subset,
      INTERVALMATRIXBASE_EIGENADDONS_BOOL_IS_SUBSET_CONST_MATRIX_INTERVALROWSATCOMPILETIMECOLSATCOMPILETIME_REF_CONST)

    .def("is_strict_subset", &S::is_strict_subset,
      INTERVALMATRIXBASE_EIGENADDONS_BOOL_IS_STRICT_SUBSET_CONST_MATRIX_INTERVALROWSATCOMPILETIMECOLSATCOMPILETIME_REF_CONST)

    .def("is_interior_subset", &S::is_interior_subset,
      INTERVALMATRIXBASE_EIGENADDONS_BOOL_IS_INTERIOR_SUBSET_CONST_MATRIX_INTERVALROWSATCOMPILETIMECOLSATCOMPILETIME_REF_CONST)

    .def("is_strict_interior_subset", &S::is_strict_interior_subset,
      INTERVALMATRIXBASE_EIGENADDONS_BOOL_IS_STRICT_INTERIOR_SUBSET_CONST_MATRIX_INTERVALROWSATCOMPILETIMECOLSATCOMPILETIME_REF_CONST)

    .def("is_superset", &S::is_superset,
      INTERVALMATRIXBASE_EIGENADDONS_BOOL_IS_SUPERSET_CONST_MATRIX_INTERVALROWSATCOMPILETIMECOLSATCOMPILETIME_REF_CONST)

    .def("is_strict_superset", &S::is_strict_superset,
      INTERVALMATRIXBASE_EIGENADDONS_BOOL_IS_STRICT_SUPERSET_CONST_MATRIX_INTERVALROWSATCOMPILETIMECOLSATCOMPILETIME_REF_CONST)

    .def("is_bisectable", &S::is_bisectable,
      INTERVALMATRIXBASE_EIGENADDONS_BOOL_IS_BISECTABLE_CONST)

    .def("inflate", (S&(S::*)(double))&S::inflate,
      INTERVALMATRIXBASE_EIGENADDONS_AUTO_REF_INFLATE_DOUBLE,
      "r"_a)

    .def("inflate", (S&(S::*)(const V&))&S::inflate,
      INTERVALMATRIXBASE_EIGENADDONS_AUTO_REF_INFLATE_CONST_MATRIX_DOUBLEROWSATCOMPILETIMECOLSATCOMPILETIME_REF,
      "r"_a)

    .def("bisect", [](const S& x, size_t_type i, double ratio)
        {
          matlab::test_integer(i);
          return x.bisect(matlab::input_index(i),ratio);
        },
      INTERVALMATRIXBASE_EIGENADDONS_AUTO_BISECT_SIZET_FLOAT_CONST,
      "i"_a, "ratio"_a = 0.49)

    .def("bisect_largest", [](const S& x, double ratio = 0.49) { return x.bisect_largest(); },
      INTERVALMATRIXBASE_EIGENADDONS_AUTO_BISECT_LARGEST_FLOAT_CONST,
      "ratio"_a = 0.49)

    .def(py::self &= py::self,
      INTERVALMATRIXBASE_EIGENADDONS_AUTO_REF_OPERATORANDEQ_CONST_MATRIX_U_ROWSATCOMPILETIMECOLSATCOMPILETIME_REF
      "x"_a)

    .def(py::self |= py::self,
      INTERVALMATRIXBASE_EIGENADDONS_AUTO_REF_OPERATOROREQ_CONST_MATRIX_U_ROWSATCOMPILETIMECOLSATCOMPILETIME_REF
      "x"_a)

    .def(py::self & py::self,
      INTERVALMATRIXBASE_EIGENADDONS_AUTO_OPERATORAND_CONST_MATRIX_INTERVALROWSATCOMPILETIMECOLSATCOMPILETIME_REF_CONST
      "x"_a)

    .def(py::self | py::self,
      INTERVALMATRIXBASE_EIGENADDONS_AUTO_OPERATOROR_CONST_MATRIX_INTERVALROWSATCOMPILETIMECOLSATCOMPILETIME_REF_CONST,
      "x"_a)
  ;

  py::implicitly_convertible<V,S>();
}
