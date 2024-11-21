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
#include <codac2_IntervalVector.h>
#include <codac2_IntervalMatrix.h>

#include "codac2_py_doc.h"
#include "codac2_py_MatrixBase_eigenaddons_docs.h" // Generated file from Doxygen XML (doxygen2docstring.py)
#include "codac2_py_VectorBase_eigenaddons_docs.h" // Generated file from Doxygen XML (doxygen2docstring.py)
#include "codac2_py_IntervalMatrixBase_eigenaddons_docs.h" // Generated file from Doxygen XML (doxygen2docstring.py)
#include "codac2_py_IntervalVector_eigenaddons_docs.h" // Generated file from Doxygen XML (doxygen2docstring.py)
#include "codac2_py_IntervalVector_docs.h" // Generated file from Doxygen XML (doxygen2docstring.py)
#include "codac2_py_Base_eigenaddons_docs.h" // Generated file from Doxygen XML (doxygen2docstring.py)
#include "codac2_py_eigenaddons_test_docs.h" // Generated file from Doxygen XML (doxygen2docstring.py)
#include "codac2_py_matrices_docs.h" // Generated file from Doxygen XML (doxygen2docstring.py)

#include "codac2_py_VectorBase.h"
#include "codac2_py_IntervalMatrixBase.h"

using namespace std;
using namespace codac2;
namespace py = pybind11;
using namespace pybind11::literals;

py::class_<IntervalVector> export_IntervalVector(py::module& m)
{
  py::class_<IntervalVector> exported_intervalvector_class(m, "IntervalVector", DOC_TO_BE_DEFINED);
  export_IntervalMatrixBase<IntervalVector,Vector,true>(m, exported_intervalvector_class);
  export_VectorBase<IntervalVector,IntervalMatrix,Interval>(m, exported_intervalvector_class);

  exported_intervalvector_class

    .def(py::init(
        [](Index_type n)
        {
          matlab::test_integer(n);
          return std::make_unique<IntervalVector>(n);
        }),
      INTERVALVECTOR_EIGENADDONS_MATRIX_INT,
      "n"_a)

    .def(py::init(
        [](Index_type n, const Interval& x)
        {
          matlab::test_integer(n);
          return std::make_unique<IntervalVector>((int)n,x);
        }),
      VECTORBASE_EIGENADDONS_MATRIX_INT_CONST_SCALAR_REF,
      "n"_a, "x"_a)

    .def(py::init<const IntervalVector&>(),
      "x"_a)

    .def(py::init<const Vector&>(),
      INTERVALMATRIXBASE_EIGENADDONS_MATRIX_CONST_MATRIX_DOUBLEROWSATCOMPILETIMECOLSATCOMPILETIME_REF,
      "x"_a)

    .def(py::init<const Vector&,const Vector&>(),
      INTERVALMATRIXBASE_EIGENADDONS_MATRIX_CONST_MATRIX_DOUBLERC_REF_CONST_MATRIX_DOUBLERC_REF,
      "lb"_a, "ub"_a)

    .def(py::init( // this constructor must be the last one to be declared
        [](const std::vector<Interval>& v)
        {
          auto iv = std::make_unique<IntervalVector>(v.size());
          for(size_t i = 0 ; i < v.size() ; i++)
            (*iv)[i] = v[i];
          return iv;
        }),
      INTERVALVECTOR_EIGENADDONS_MATRIX_CONST_INITIALIZER_LIST_INTERVAL_REF,
      "v"_a)

    .def("complementary", [](const IntervalVector& x) { return x.complementary(); },
      INTERVALVECTOR_EIGENADDONS_AUTO_COMPLEMENTARY_CONST)

    .def("diff", [](const IntervalVector& x, const IntervalVector& y, bool compactness = true) { return x.diff(y,compactness); },
      INTERVALVECTOR_EIGENADDONS_LIST_MATRIX_INTERVALRC_DIFF_CONST_MATRIX_INTERVALRC_REF_BOOL_CONST,
      "y"_a, "compactness"_a = true)
    
    .def_static("empty", [](Index_type n)
        {
          matlab::test_integer(n);
          return IntervalVector::empty(n);
        },
      INTERVALVECTOR_EIGENADDONS_STATIC_AUTO_EMPTY_INDEX,
      "n"_a)

    .def("__repr__", [](const IntervalVector& x)
        {
          std::ostringstream s;
          s << x;
          return string(s.str()); 
        },
      OSTREAM_REF_OPERATOROUT_OSTREAM_REF_CONST_INTERVALVECTOR_REF)

  ;

  py::implicitly_convertible<py::list,IntervalVector>();
  py::implicitly_convertible<Vector,IntervalVector>();

  m.def("cart_prod_boxes", [](const std::list<IntervalVector>& l)
      {
        IntervalVector c = *l.begin();
        for(auto it = std::next(l.cbegin()); it != l.cend(); it++)
          c = cart_prod(c,*it);
        return c;
      },
    INTERVALVECTOR_CART_PROD_CONST_X_REF_VARIADIC);
  // The variadic version of this function is defined in __init__.py file

  m.def("hull", [](const std::list<IntervalVector>& l) { return hull(l); },
    AUTO_HULL_CONST_LIST_EIGEN_MATRIX_SCALARROWSATCOMPILETIMECOLSATCOMPILETIME_REF,
    "l"_a);

  return exported_intervalvector_class;
}