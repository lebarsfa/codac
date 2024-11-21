/** 
 *  Vector binding
 * ----------------------------------------------------------------------------
 *  \date       2024
 *  \author     Simon Rohou
 *  \copyright  Copyright 2024 Codac Team
 *  \license    GNU Lesser General Public License (LGPL)
 */

#include <sstream>
#include <pybind11/pybind11.h>
#include <pybind11/operators.h>
#include <pybind11/stl.h>
#include <codac2_Vector.h>
#include <codac2_IntervalMatrix.h>

#include "codac2_py_doc.h"
#include "codac2_py_MatrixBase_eigenaddons_docs.h" // Generated file from Doxygen XML (doxygen2docstring.py)
#include "codac2_py_Vector_eigenaddons_docs.h" // Generated file from Doxygen XML (doxygen2docstring.py)
#include "codac2_py_VectorBase_eigenaddons_docs.h" // Generated file from Doxygen XML (doxygen2docstring.py)
#include "codac2_py_Vector_docs.h" // Generated file from Doxygen XML (doxygen2docstring.py)
#include "codac2_py_Base_eigenaddons_docs.h" // Generated file from Doxygen XML (doxygen2docstring.py)
#include "codac2_py_eigenaddons_test_docs.h" // Generated file from Doxygen XML (doxygen2docstring.py)
#include "codac2_py_matrices_docs.h" // Generated file from Doxygen XML (doxygen2docstring.py)

#include "codac2_py_VectorBase.h"

using namespace std;
using namespace codac2;
namespace py = pybind11;
using namespace pybind11::literals;

py::class_<Vector> export_Vector(py::module& m)
{
  py::class_<Vector> exported_vector_class(m, "Vector", DOC_TO_BE_DEFINED);
  export_MatrixBase<Vector,double,true>(m, exported_vector_class);
  export_VectorBase<Vector,Matrix,double>(m, exported_vector_class);

  exported_vector_class

    .def(py::init(
        [](size_t_type n)
        {
          matlab::test_integer(n);
          return std::make_unique<Vector>(n);
        }),
      VECTOR_EIGENADDONS_MATRIX_INT,
      "n"_a)

    .def(py::init<const Vector&>(),
      "x"_a)

    .def(py::init( // this constructor must be the last one to be declared
        [](const std::vector<double>& v_)
        {
          auto v = std::make_unique<Vector>(v_.size());
          for(size_t i = 0 ; i < v_.size() ; i++)
            (*v)[i] = v_[i];
          return v;
        }),
      VECTOR_EIGENADDONS_MATRIX_INITIALIZER_LIST_DOUBLE,
      "v"_a)

    .def("min_coeff_index", [](const Vector& x)
        {
          return matlab::output_index(x.min_coeff_index());
        },
      VECTOR_EIGENADDONS_SIZET_MIN_COEFF_INDEX_CONST)

    .def("max_coeff_index", [](const Vector& x)
        {
          return matlab::output_index(x.max_coeff_index());
        },
      VECTOR_EIGENADDONS_SIZET_MAX_COEFF_INDEX_CONST)

    .def("__repr__", [](const Vector& x)
        {
          std::ostringstream s;
          s << x;
          return string(s.str()); 
        },
      DOC_TO_BE_DEFINED)
    
  ;

  py::implicitly_convertible<py::list,Vector>();
  
  return exported_vector_class;
}