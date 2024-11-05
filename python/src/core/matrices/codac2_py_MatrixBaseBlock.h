/** 
 *  MatrixBase binding
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
#include <codac2_py_MatrixBaseBlock.h>
#include "codac2_py_MatrixBaseBlock_docs.h" // Generated file from Doxygen XML (doxygen2docstring.py)

using namespace std;
using namespace codac2;
namespace py = pybind11;
using namespace pybind11::literals;

template<typename S,typename T>
py::class_<MatrixBaseBlock<typename S::EigenType&,T>> export_MatrixBaseBlock(py::module& m, const std::string& name)
{
  py::class_<MatrixBaseBlock<typename S::EigenType&,T>> exported_mbb_class(m, name.c_str(), MATRIXBASEBLOCK_MAIN);

  exported_mbb_class

    .def("init", [](MatrixBaseBlock<typename S::EigenType&,T>& x, const S& a)
        {
          return x = a;
        },
      VOID_MATRIXBASEBLOCK_QT_INIT_CONST_S__REF,
      "x"_a)

    .def("nb_rows", &MatrixBaseBlock<typename S::EigenType&,T>::nb_rows,
      SIZET_MATRIXBASEBLOCK_QT_NB_ROWS_CONST)

    .def("nb_cols", &MatrixBaseBlock<typename S::EigenType&,T>::nb_cols,
      SIZET_MATRIXBASEBLOCK_QT_NB_COLS_CONST)

    .def("eval", &MatrixBaseBlock<typename S::EigenType&,T>::eval,
      AUTO_MATRIXBASEBLOCK_QT_EVAL_CONST)
    
    .def(py::self == py::self)
    .def(py::self != py::self)

    .def("__repr__", [](const MatrixBaseBlock<typename S::EigenType&,T>& x)
        {
          std::ostringstream s;
          s << x;
          return string(s.str()); 
        },
      OSTREAM_REF_OPERATOROUT_OSTREAM_REF_CONST_MATRIXBASEBLOCK_QT_REF)
    
  ;
  
  return exported_mbb_class;
}