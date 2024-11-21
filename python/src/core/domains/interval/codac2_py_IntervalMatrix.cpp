/** 
 *  Interval Python binding
 *  Originated from the former pyIbex library (Benoît Desrochers)
 * ----------------------------------------------------------------------------
 *  \date       2024
 *  \author     Benoît Desrochers, Simon Rohou, Fabrice Le Bars
 *  \copyright  Copyright 2024 Codac Team
 *  \license    GNU Lesser General Public License (LGPL)
 */

#include <sstream>
#include <pybind11/pybind11.h>
#include <pybind11/operators.h>
#include <pybind11/stl.h>
#include <codac2_Row.h>
#include <codac2_IntervalRow.h>
#include <codac2_IntervalVector.h>
#include <codac2_IntervalMatrix.h>

#include "codac2_py_doc.h"
#include "codac2_py_MatrixBase_eigenaddons_docs.h" // Generated file from Doxygen XML (doxygen2docstring.py)
#include "codac2_py_IntervalMatrixBase_eigenaddons_docs.h" // Generated file from Doxygen XML (doxygen2docstring.py)
#include "codac2_py_IntervalMatrix_docs.h" // Generated file from Doxygen XML (doxygen2docstring.py)
#include "codac2_py_Base_eigenaddons_docs.h" // Generated file from Doxygen XML (doxygen2docstring.py)
#include "codac2_py_eigenaddons_test_docs.h" // Generated file from Doxygen XML (doxygen2docstring.py)
#include "codac2_py_matrices_docs.h" // Generated file from Doxygen XML (doxygen2docstring.py)

#include "codac2_py_IntervalMatrixBase.h"

using namespace std;
using namespace codac2;
namespace py = pybind11;
using namespace pybind11::literals;

py::class_<IntervalMatrix> export_IntervalMatrix(py::module& m)
{
  py::class_<IntervalMatrix> exported_intervalmatrix_class(m, "IntervalMatrix", DOC_TO_BE_DEFINED);
  export_IntervalMatrixBase<IntervalMatrix,Matrix,false>(m, exported_intervalmatrix_class);

  exported_intervalmatrix_class

    .def(py::init(
        [](size_t_type r, size_t_type c)
        {
          matlab::test_integer(r,c);
          return std::make_unique<IntervalMatrix>(r,c);
        }),
      DOC_TO_BE_DEFINED,
      "r"_a, "c"_a)

    .def(py::init(
        [](size_t_type r, size_t_type c, const Interval& x)
        {
          matlab::test_integer(r,c);
          return std::make_unique<IntervalMatrix>(r,c,x);
        }),
      MATRIXBASE_EIGENADDONS_MATRIX_INT_INT_CONST_SCALAR_REF,
      "r"_a, "c"_a, "x"_a)

    .def(py::init<const IntervalMatrix&>(),
      "x"_a)

    .def(py::init<const Matrix&>(),
      INTERVALMATRIXBASE_EIGENADDONS_MATRIX_CONST_MATRIX_DOUBLEROWSATCOMPILETIMECOLSATCOMPILETIME_REF,
      "x"_a)

    .def(py::init<const Matrix&,const Matrix&>(),
      INTERVALMATRIXBASE_EIGENADDONS_MATRIX_CONST_MATRIX_DOUBLERC_REF_CONST_MATRIX_DOUBLERC_REF,
      "lb"_a, "ub"_a)

    .def(py::init<const Row&>(),
      DOC_TO_BE_DEFINED,
      "x"_a)

    .def(py::init<const IntervalRow&>(),
      DOC_TO_BE_DEFINED,
      "x"_a)

    .def(py::init<const Vector&>(),
      DOC_TO_BE_DEFINED,
      "x"_a)

    .def(py::init<const IntervalVector&>(),
      DOC_TO_BE_DEFINED,
      "x"_a)

    .def(py::init<const Eigen::Block<Matrix>&>(),
      DOC_TO_BE_DEFINED,
      "x"_a)

    .def(py::init<const Eigen::Block<IntervalMatrix>&>(),
      DOC_TO_BE_DEFINED,
      "x"_a)

    .def(py::init( // this constructor must be the last one to be declared
        [](const std::vector<IntervalVector>& v)
        {
          assert_release(!std::empty(v));
          auto im = std::make_unique<IntervalMatrix>(v.size(),v[0].size());
          for(size_t i = 0 ; i < v.size() ; i++)
          {
            assert_release(v[i].size() == im->cols() && "IntervalVector objects of different size");
            im->row(i) = v[i].transpose();
          }
          return im;
        }),
      DOC_TO_BE_DEFINED,
      "v"_a)
    
    .def_static("empty", [](size_t_type r, size_t_type c)
        {
          matlab::test_integer(r,c);
          return IntervalMatrix::empty(r,c);
        },
      INTERVALMATRIXBASE_EIGENADDONS_STATIC_AUTO_EMPTY_SIZET_SIZET,
      "r"_a, "c"_a)

    .def("__repr__", [](const IntervalMatrix& x)
        {
          std::ostringstream s;
          s << x;
          return string(s.str()); 
        },
      OSTREAM_REF_OPERATOROUT_OSTREAM_REF_CONST_INTERVALMATRIX_REF)
  ;
  
  return exported_intervalmatrix_class;
}