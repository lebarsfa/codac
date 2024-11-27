/** 
 *  Inversion of matrices
 * ----------------------------------------------------------------------------
 *  \date       2024
 *  \author     Damien
 *  \copyright  Copyright 2024 Codac Team
 *  \license    GNU Lesser General Public License (LGPL)
 */

#include <sstream>
#include <pybind11/pybind11.h>
#include <pybind11/operators.h>
#include <pybind11/stl.h>
#include <codac2_Matrix.h>
#include <codac2_IntervalMatrix.h>
#include <codac2_Inversion.h>

#include "codac2_py_doc.h"
#include "codac2_py_Inversion_docs.h" // Generated file from Doxygen 


using namespace std;
using namespace codac2;
namespace py = pybind11;
using namespace pybind11::literals;

using B = Eigen::Block<Matrix>;
using IB = Eigen::Block<IntervalMatrix>;

void export_Inversion(py::module& m)
{
  m

   .def("infinite_sum_enclosure", 
	(IntervalMatrix(*)(const IntervalMatrix&,double&))
	 &codac2::infinite_sum_enclosure,
    INTERVALMATRIX_INFINITE_SUM_ENCLOSURE_CONST_INTERVALMATRIX_REF_DOUBLE_REF,
	"A"_a, "mrad"_a)

   .def("inverse_correction", 
	(IntervalMatrix(*)(const Matrix&,const Matrix&,bool))
	 &codac2::inverse_correction,
    INTERVALMATRIX_INVERSE_CORRECTION_CONST_MATRIX_REF_CONST_MATRIX_REF_BOOL,
	"A"_a, "B"_a, "left"_a)

   .def("inverse_correction", 
	(IntervalMatrix(*)(const IntervalMatrix&,const Matrix&,bool))
	 &codac2::inverse_correction,
   INTERVALMATRIX_INVERSE_CORRECTION_CONST_INTERVALMATRIX_REF_CONST_MATRIX_REF_BOOL,
	"A"_a, "B"_a, "left"_a)

   .def("inverse_enclosure", 
	(IntervalMatrix(*)(const Matrix&))
	 &codac2::inverse_enclosure,
  INTERVALMATRIX_INVERSE_ENCLOSURE_CONST_MATRIX_REF,
	"A"_a)

   .def("inverse_enclosure", 
	(IntervalMatrix(*)(const IntervalMatrix&))
	 &codac2::inverse_enclosure,
  INTERVALMATRIX_INVERSE_ENCLOSURE_CONST_MATRIX_REF,
	"A"_a)

   ;
}
