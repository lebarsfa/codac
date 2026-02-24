# From https://gitlab.com/CLIUtils/modern-cmake/-/blob/b47192867e86e5f00f70ec860638a28473edc8bf/examples/extended-project/src/CMakeLists.txt

# Note that headers are optional, and do not affect add_library, but they will not
# show up in IDEs unless they are listed in add_library.

# Optionally glob, but only for CMake 3.12 or later:
# file(GLOB HEADER_LIST CONFIGURE_DEPENDS "${codac_SOURCE_DIR}/include/modern/*.hpp")
#set(HEADER_LIST "${codac_SOURCE_DIR}/include/modern/lib.hpp")
#set(SOURCE_LIST 
#codac.cpp
#)
#file(GLOB HEADER_LIST CONFIGURE_DEPENDS "${codac_SOURCE_DIR}/codac/*.h" "${codac_SOURCE_DIR}/codac/*.hpp")
#file(GLOB OTHER_LIST CONFIGURE_DEPENDS "${codac_SOURCE_DIR}/codac/*.ypp")

# Make an automatic library - will be static or dynamic based on user setting
#add_library(codac ${SOURCE_LIST} ${HEADER_LIST} ${OTHER_LIST})

#file(GLOB_RECURSE CODAC_CORE_SRC CONFIGURE_DEPENDS 
#    "${CMAKE_CURRENT_SOURCE_DIR}/*.cpp"
#    "${CMAKE_CURRENT_SOURCE_DIR}/*.h"
#)
#
## Exclusions...
#list(FILTER CODAC_CORE_SRC EXCLUDE REGEX ".*/contractors/codac2_CtcPolar_codac1\\.cpp$")
#list(FILTER CODAC_CORE_SRC EXCLUDE REGEX ".*/functions/analytic/codac2_AnalyticFunction_impl\\.h$")

  list(APPEND CODAC_CORE_SRC

    ${CMAKE_CURRENT_SOURCE_DIR}/actions/codac2_OctaSym.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/actions/codac2_OctaSym.h

    ${CMAKE_CURRENT_SOURCE_DIR}/contractors/codac2_Ctc.h
    ${CMAKE_CURRENT_SOURCE_DIR}/contractors/codac2_CtcAction.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/contractors/codac2_CtcAction.h
    ${CMAKE_CURRENT_SOURCE_DIR}/contractors/codac2_CtcCartProd.h
    ${CMAKE_CURRENT_SOURCE_DIR}/contractors/codac2_CtcConstell.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/contractors/codac2_CtcConstell.h
    ${CMAKE_CURRENT_SOURCE_DIR}/contractors/codac2_CtcCross.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/contractors/codac2_CtcCross.h
    ${CMAKE_CURRENT_SOURCE_DIR}/contractors/codac2_CtcCtcBoundary.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/contractors/codac2_CtcCtcBoundary.h
    ${CMAKE_CURRENT_SOURCE_DIR}/contractors/codac2_CtcDeriv.h
    ${CMAKE_CURRENT_SOURCE_DIR}/contractors/codac2_CtcDist.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/contractors/codac2_CtcDist.h
    ${CMAKE_CURRENT_SOURCE_DIR}/contractors/codac2_CtcEllipse.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/contractors/codac2_CtcEllipse.h
    ${CMAKE_CURRENT_SOURCE_DIR}/contractors/codac2_CtcEmpty.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/contractors/codac2_CtcEmpty.h
    ${CMAKE_CURRENT_SOURCE_DIR}/contractors/codac2_CtcFixpoint.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/contractors/codac2_CtcFixpoint.h
    ${CMAKE_CURRENT_SOURCE_DIR}/contractors/codac2_CtcIdentity.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/contractors/codac2_CtcIdentity.h
    ${CMAKE_CURRENT_SOURCE_DIR}/contractors/codac2_CtcInnerOuter.h
    ${CMAKE_CURRENT_SOURCE_DIR}/contractors/codac2_CtcInter.h
    ${CMAKE_CURRENT_SOURCE_DIR}/contractors/codac2_CtcInverse.h
    ${CMAKE_CURRENT_SOURCE_DIR}/contractors/codac2_CtcInverseNotIn.h
    ${CMAKE_CURRENT_SOURCE_DIR}/contractors/codac2_CtcLazy.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/contractors/codac2_CtcLazy.h
    ${CMAKE_CURRENT_SOURCE_DIR}/contractors/codac2_CtcNot.h
    ${CMAKE_CURRENT_SOURCE_DIR}/contractors/codac2_CtcUnion.h
    ${CMAKE_CURRENT_SOURCE_DIR}/contractors/codac2_CtcPointCloud.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/contractors/codac2_CtcPointCloud.h
    ${CMAKE_CURRENT_SOURCE_DIR}/contractors/codac2_CtcPolar.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/contractors/codac2_CtcPolar.h
    ${CMAKE_CURRENT_SOURCE_DIR}/contractors/codac2_CtcPolygon.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/contractors/codac2_CtcPolygon.h
    ${CMAKE_CURRENT_SOURCE_DIR}/contractors/codac2_CtcProj.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/contractors/codac2_CtcProj.h
    ${CMAKE_CURRENT_SOURCE_DIR}/contractors/codac2_CtcSegment.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/contractors/codac2_CtcSegment.h
    ${CMAKE_CURRENT_SOURCE_DIR}/contractors/codac2_CtcWrapper.h
    ${CMAKE_CURRENT_SOURCE_DIR}/contractors/codac2_linear_ctc.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/contractors/codac2_linear_ctc.h

    ${CMAKE_CURRENT_SOURCE_DIR}/domains/codac2_BoolInterval.h
    ${CMAKE_CURRENT_SOURCE_DIR}/domains/codac2_Domain.h
    ${CMAKE_CURRENT_SOURCE_DIR}/domains/codac2_Wrapper.h
    ${CMAKE_CURRENT_SOURCE_DIR}/domains/ellipsoid/codac2_Ellipsoid.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/domains/ellipsoid/codac2_Ellipsoid.h
    ${CMAKE_CURRENT_SOURCE_DIR}/domains/ellipsoid/codac2_Ellipsoid_utils.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/domains/ellipsoid/codac2_Ellipsoid_utils.h
    ${CMAKE_CURRENT_SOURCE_DIR}/domains/interval/codac2_Interval.h
    ${CMAKE_CURRENT_SOURCE_DIR}/domains/interval/codac2_Interval_impl.h
    ${CMAKE_CURRENT_SOURCE_DIR}/domains/interval/codac2_Interval_operations.h
    ${CMAKE_CURRENT_SOURCE_DIR}/domains/interval/codac2_Interval_operations_impl.h
    ${CMAKE_CURRENT_SOURCE_DIR}/domains/interval/codac2_IntervalMatrix.h
    ${CMAKE_CURRENT_SOURCE_DIR}/domains/interval/codac2_IntervalRow.h
    ${CMAKE_CURRENT_SOURCE_DIR}/domains/interval/codac2_IntervalVector.h
    ${CMAKE_CURRENT_SOURCE_DIR}/domains/paving/codac2_Paving.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/domains/paving/codac2_Paving.h
    ${CMAKE_CURRENT_SOURCE_DIR}/domains/paving/codac2_PavingNode.h
    ${CMAKE_CURRENT_SOURCE_DIR}/domains/paving/codac2_Subpaving.h
    ${CMAKE_CURRENT_SOURCE_DIR}/domains/tube/codac2_Slice.h
    ${CMAKE_CURRENT_SOURCE_DIR}/domains/tube/codac2_SliceBase.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/domains/tube/codac2_SliceBase.h
    ${CMAKE_CURRENT_SOURCE_DIR}/domains/tube/codac2_SlicedTube.h
    ${CMAKE_CURRENT_SOURCE_DIR}/domains/tube/codac2_SlicedTube_integral_impl.h
    ${CMAKE_CURRENT_SOURCE_DIR}/domains/tube/codac2_SlicedTubeBase.h
    ${CMAKE_CURRENT_SOURCE_DIR}/domains/tube/codac2_TDomain.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/domains/tube/codac2_TDomain.h
    ${CMAKE_CURRENT_SOURCE_DIR}/domains/tube/codac2_TSlice.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/domains/tube/codac2_TSlice.h
    ${CMAKE_CURRENT_SOURCE_DIR}/domains/tube/codac2_TubeBase.h
    ${CMAKE_CURRENT_SOURCE_DIR}/domains/tube/codac2_tube_cart_prod.h

    ${CMAKE_CURRENT_SOURCE_DIR}/functions/codac2_ExprBase.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/functions/codac2_ExprBase.h
    ${CMAKE_CURRENT_SOURCE_DIR}/functions/codac2_FunctionArgsList.h
    ${CMAKE_CURRENT_SOURCE_DIR}/functions/codac2_FunctionBase.h
    ${CMAKE_CURRENT_SOURCE_DIR}/functions/codac2_VarBase.h
    ${CMAKE_CURRENT_SOURCE_DIR}/functions/analytic/codac2_analytic_constants.h
    ${CMAKE_CURRENT_SOURCE_DIR}/functions/analytic/codac2_analytic_variables.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/functions/analytic/codac2_analytic_variables.h
    ${CMAKE_CURRENT_SOURCE_DIR}/functions/analytic/codac2_AnalyticExpr.h
    ${CMAKE_CURRENT_SOURCE_DIR}/functions/analytic/codac2_AnalyticExprWrapper.h
    ${CMAKE_CURRENT_SOURCE_DIR}/functions/analytic/codac2_AnalyticFunction.h
    ${CMAKE_CURRENT_SOURCE_DIR}/functions/analytic/codac2_AnalyticType.h
    ${CMAKE_CURRENT_SOURCE_DIR}/functions/analytic/codac2_ExprType.h
    ${CMAKE_CURRENT_SOURCE_DIR}/functions/set/codac2_set_operations.h
    ${CMAKE_CURRENT_SOURCE_DIR}/functions/set/codac2_set_operators.h
    ${CMAKE_CURRENT_SOURCE_DIR}/functions/set/codac2_set_variables.h
    ${CMAKE_CURRENT_SOURCE_DIR}/functions/set/codac2_SetExpr.h
    ${CMAKE_CURRENT_SOURCE_DIR}/functions/set/codac2_SetFunction.h
    ${CMAKE_CURRENT_SOURCE_DIR}/functions/set/codac2_set_constants.h

    ${CMAKE_CURRENT_SOURCE_DIR}/geometry/codac2_ConvexPolygon.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/geometry/codac2_ConvexPolygon.h
    ${CMAKE_CURRENT_SOURCE_DIR}/geometry/codac2_geometry.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/geometry/codac2_geometry.h
    ${CMAKE_CURRENT_SOURCE_DIR}/geometry/codac2_Polygon.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/geometry/codac2_Polygon.h
    ${CMAKE_CURRENT_SOURCE_DIR}/geometry/codac2_Segment.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/geometry/codac2_Segment.h
    
    ${CMAKE_CURRENT_SOURCE_DIR}/operators/codac2_abs.h
    ${CMAKE_CURRENT_SOURCE_DIR}/operators/codac2_acos.h
    ${CMAKE_CURRENT_SOURCE_DIR}/operators/codac2_arith_add.h
    ${CMAKE_CURRENT_SOURCE_DIR}/operators/codac2_arith_div.h
    ${CMAKE_CURRENT_SOURCE_DIR}/operators/codac2_arith_mul.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/operators/codac2_arith_mul.h
    ${CMAKE_CURRENT_SOURCE_DIR}/operators/codac2_arith_sub.h
    ${CMAKE_CURRENT_SOURCE_DIR}/operators/codac2_asin.h
    ${CMAKE_CURRENT_SOURCE_DIR}/operators/codac2_atan.h
    ${CMAKE_CURRENT_SOURCE_DIR}/operators/codac2_atan2.h
    ${CMAKE_CURRENT_SOURCE_DIR}/operators/codac2_ceil.h
    ${CMAKE_CURRENT_SOURCE_DIR}/operators/codac2_chi.h
    ${CMAKE_CURRENT_SOURCE_DIR}/operators/codac2_component.h
    ${CMAKE_CURRENT_SOURCE_DIR}/operators/codac2_cos.h
    ${CMAKE_CURRENT_SOURCE_DIR}/operators/codac2_cosh.h
    ${CMAKE_CURRENT_SOURCE_DIR}/operators/codac2_cross_prod.h
    ${CMAKE_CURRENT_SOURCE_DIR}/operators/codac2_det.h
    ${CMAKE_CURRENT_SOURCE_DIR}/operators/codac2_exp.h
    ${CMAKE_CURRENT_SOURCE_DIR}/operators/codac2_extend.h
    ${CMAKE_CURRENT_SOURCE_DIR}/operators/codac2_flatten.h
    ${CMAKE_CURRENT_SOURCE_DIR}/operators/codac2_floor.h
    ${CMAKE_CURRENT_SOURCE_DIR}/operators/codac2_log.h
    ${CMAKE_CURRENT_SOURCE_DIR}/operators/codac2_mat.h
    ${CMAKE_CURRENT_SOURCE_DIR}/operators/codac2_max.h
    ${CMAKE_CURRENT_SOURCE_DIR}/operators/codac2_min.h
    ${CMAKE_CURRENT_SOURCE_DIR}/operators/codac2_mod.h
    ${CMAKE_CURRENT_SOURCE_DIR}/operators/codac2_operators.h
    ${CMAKE_CURRENT_SOURCE_DIR}/operators/codac2_pow.h
    ${CMAKE_CURRENT_SOURCE_DIR}/operators/codac2_sign.h
    ${CMAKE_CURRENT_SOURCE_DIR}/operators/codac2_sin.h
    ${CMAKE_CURRENT_SOURCE_DIR}/operators/codac2_sinh.h
    ${CMAKE_CURRENT_SOURCE_DIR}/operators/codac2_sqr.h
    ${CMAKE_CURRENT_SOURCE_DIR}/operators/codac2_sqrt.h
    ${CMAKE_CURRENT_SOURCE_DIR}/operators/codac2_subvector.h
    ${CMAKE_CURRENT_SOURCE_DIR}/operators/codac2_tan.h
    ${CMAKE_CURRENT_SOURCE_DIR}/operators/codac2_tanh.h
    ${CMAKE_CURRENT_SOURCE_DIR}/operators/codac2_transpose.h
    ${CMAKE_CURRENT_SOURCE_DIR}/operators/codac2_vec.h

    ${CMAKE_CURRENT_SOURCE_DIR}/matrices/eigen/Matrix_addons/codac2_Matrix_addons_Base.h
    ${CMAKE_CURRENT_SOURCE_DIR}/matrices/eigen/Matrix_addons/codac2_Matrix_addons_include.h
    ${CMAKE_CURRENT_SOURCE_DIR}/matrices/eigen/Matrix_addons/codac2_Matrix_addons_IntervalMatrix.h
    ${CMAKE_CURRENT_SOURCE_DIR}/matrices/eigen/Matrix_addons/codac2_Matrix_addons_IntervalMatrixBase.h
    ${CMAKE_CURRENT_SOURCE_DIR}/matrices/eigen/Matrix_addons/codac2_Matrix_addons_IntervalVector.h
    ${CMAKE_CURRENT_SOURCE_DIR}/matrices/eigen/Matrix_addons/codac2_Matrix_addons_Matrix.h
    ${CMAKE_CURRENT_SOURCE_DIR}/matrices/eigen/Matrix_addons/codac2_Matrix_addons_MatrixBase.h
    ${CMAKE_CURRENT_SOURCE_DIR}/matrices/eigen/Matrix_addons/codac2_Matrix_addons_Vector.h
    ${CMAKE_CURRENT_SOURCE_DIR}/matrices/eigen/Matrix_addons/codac2_Matrix_addons_VectorBase.h

    ${CMAKE_CURRENT_SOURCE_DIR}/matrices/eigen/MatrixBase_addons/codac2_MatrixBase_addons_Base.h
    ${CMAKE_CURRENT_SOURCE_DIR}/matrices/eigen/MatrixBase_addons/codac2_MatrixBase_addons_include.h
    ${CMAKE_CURRENT_SOURCE_DIR}/matrices/eigen/MatrixBase_addons/codac2_MatrixBase_addons_IntervalMatrix.h
    ${CMAKE_CURRENT_SOURCE_DIR}/matrices/eigen/MatrixBase_addons/codac2_MatrixBase_addons_IntervalMatrixBase.h
    ${CMAKE_CURRENT_SOURCE_DIR}/matrices/eigen/MatrixBase_addons/codac2_MatrixBase_addons_IntervalVector.h
    ${CMAKE_CURRENT_SOURCE_DIR}/matrices/eigen/MatrixBase_addons/codac2_MatrixBase_addons_Matrix.h
    ${CMAKE_CURRENT_SOURCE_DIR}/matrices/eigen/MatrixBase_addons/codac2_MatrixBase_addons_MatrixBase.h
    ${CMAKE_CURRENT_SOURCE_DIR}/matrices/eigen/MatrixBase_addons/codac2_MatrixBase_addons_Vector.h
    ${CMAKE_CURRENT_SOURCE_DIR}/matrices/eigen/MatrixBase_addons/codac2_MatrixBase_addons_VectorBase.h
    
    ${CMAKE_CURRENT_SOURCE_DIR}/matrices/codac2_cart_prod.h
    ${CMAKE_CURRENT_SOURCE_DIR}/matrices/codac2_GaussJordan.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/matrices/codac2_GaussJordan.h
    ${CMAKE_CURRENT_SOURCE_DIR}/matrices/codac2_hull.h
    ${CMAKE_CURRENT_SOURCE_DIR}/matrices/codac2_inversion.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/matrices/codac2_inversion.h
    ${CMAKE_CURRENT_SOURCE_DIR}/matrices/codac2_matrices.h
    ${CMAKE_CURRENT_SOURCE_DIR}/matrices/codac2_Matrix.h
    ${CMAKE_CURRENT_SOURCE_DIR}/matrices/codac2_Row.h
    ${CMAKE_CURRENT_SOURCE_DIR}/matrices/codac2_Vector.h
    ${CMAKE_CURRENT_SOURCE_DIR}/matrices/codac2_IntvFullPivLU.h
    ${CMAKE_CURRENT_SOURCE_DIR}/matrices/codac2_IntvFullPivLU.cpp


    ${CMAKE_CURRENT_SOURCE_DIR}/paver/codac2_pave.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/paver/codac2_pave.h

    ${CMAKE_CURRENT_SOURCE_DIR}/proj/codac2_ProjBase.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/proj/codac2_ProjBase.h

    ${CMAKE_CURRENT_SOURCE_DIR}/separators/codac2_Sep.h
    ${CMAKE_CURRENT_SOURCE_DIR}/separators/codac2_SepAction.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/separators/codac2_SepAction.h
    ${CMAKE_CURRENT_SOURCE_DIR}/separators/codac2_SepCartProd.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/separators/codac2_SepCartProd.h
    ${CMAKE_CURRENT_SOURCE_DIR}/separators/codac2_SepChi.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/separators/codac2_SepChi.h
    ${CMAKE_CURRENT_SOURCE_DIR}/separators/codac2_SepCross.h
    ${CMAKE_CURRENT_SOURCE_DIR}/separators/codac2_SepCtcBoundary.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/separators/codac2_SepCtcBoundary.h
    ${CMAKE_CURRENT_SOURCE_DIR}/separators/codac2_SepCtcPair.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/separators/codac2_SepCtcPair.h
    ${CMAKE_CURRENT_SOURCE_DIR}/separators/codac2_SepEllipse.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/separators/codac2_SepEllipse.h
    ${CMAKE_CURRENT_SOURCE_DIR}/separators/codac2_SepInter.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/separators/codac2_SepInter.h
    ${CMAKE_CURRENT_SOURCE_DIR}/separators/codac2_SepInverse.h
    ${CMAKE_CURRENT_SOURCE_DIR}/separators/codac2_SepNot.h
    ${CMAKE_CURRENT_SOURCE_DIR}/separators/codac2_SepPolygon.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/separators/codac2_SepPolygon.h
    ${CMAKE_CURRENT_SOURCE_DIR}/separators/codac2_SepProj.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/separators/codac2_SepProj.h
    ${CMAKE_CURRENT_SOURCE_DIR}/separators/codac2_SepTransform.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/separators/codac2_SepTransform.h
    ${CMAKE_CURRENT_SOURCE_DIR}/separators/codac2_SepUnion.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/separators/codac2_SepUnion.h
    ${CMAKE_CURRENT_SOURCE_DIR}/separators/codac2_SepWrapper.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/separators/codac2_SepWrapper.h

    ${CMAKE_CURRENT_SOURCE_DIR}/tools/codac2_Approx.h
    ${CMAKE_CURRENT_SOURCE_DIR}/tools/codac2_assert.h
    ${CMAKE_CURRENT_SOURCE_DIR}/tools/codac2_Collection.h
    ${CMAKE_CURRENT_SOURCE_DIR}/tools/codac2_fixpoint.h
    ${CMAKE_CURRENT_SOURCE_DIR}/tools/codac2_Index.h
    ${CMAKE_CURRENT_SOURCE_DIR}/tools/codac2_math.h
    ${CMAKE_CURRENT_SOURCE_DIR}/tools/codac2_object_file_format.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/tools/codac2_object_file_format.h
    ${CMAKE_CURRENT_SOURCE_DIR}/tools/codac2_RobotSimulator.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/tools/codac2_RobotSimulator.h
    ${CMAKE_CURRENT_SOURCE_DIR}/tools/codac2_serialize.h
    ${CMAKE_CURRENT_SOURCE_DIR}/tools/codac2_template_tools.h
    ${CMAKE_CURRENT_SOURCE_DIR}/tools/codac2_transformations.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/tools/codac2_transformations.h
    ${CMAKE_CURRENT_SOURCE_DIR}/tools/codac2_TypeInfo.h

    ${CMAKE_CURRENT_SOURCE_DIR}/trajectory/codac2_AnalyticTraj.h
    ${CMAKE_CURRENT_SOURCE_DIR}/trajectory/codac2_SampledTraj.h
    ${CMAKE_CURRENT_SOURCE_DIR}/trajectory/codac2_SampledTraj_operations.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/trajectory/codac2_SampledTraj_operations.h
    ${CMAKE_CURRENT_SOURCE_DIR}/trajectory/codac2_Traj_operator.h
    ${CMAKE_CURRENT_SOURCE_DIR}/trajectory/codac2_TrajBase.h
  )

# Make an automatic library - will be static or dynamic based on user setting
add_library(codac-core ${CODAC_CORE_SRC})

# We need this directory, and users of our library will need it too
#target_include_directories(codac PUBLIC ../include)
# Inspired from https://unclejimbo.github.io/2018/06/08/Modern-CMake-for-Library-Developers/
target_include_directories(codac-core 
	PUBLIC
		"$<INSTALL_INTERFACE:include>"
		"$<BUILD_INTERFACE:${PROJECT_BINARY_DIR}>"
		"$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>"
        "$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/actions>"
        "$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/contractors>"
        "$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/domains>"
        "$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/domains/ellipsoid>"
        "$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/domains/interval>"
        "$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/domains/interval/eigen>"
        "$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/domains/paving>"
        "$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/domains/tube>"
        "$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/functions>"
        "$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/functions/analytic>"
        "$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/functions/set>"
        "$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/geometry>"
        #"$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/graphics>"
        #"$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/graphics/ipe>"
        #"$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/graphics/vibes>"
        "$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/matrices>"
        "$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/matrices/eigen/Matrix_addons>"
        "$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/matrices/eigen/MatrixBase_addons>"
        "$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/operators>"
        "$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/paver>"
        "$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/proj>"
        "$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/separators>"
        "$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/tools>"
        "$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/trajectory>"
)

# See https://github.com/friendlyanon/cxx-static-shared-example
include(GenerateExportHeader)
generate_export_header(
    codac-core
    BASE_NAME codac-core
    EXPORT_FILE_NAME codac-core_export.h
)

target_link_libraries(codac-core PUBLIC gaol::gaol Eigen3::Eigen) # ${PROJECT_NAME}-graphics

if(NOT BUILD_SHARED_LIBS)
  target_compile_definitions(codac-core PUBLIC codac_STATIC_DEFINE)
else()
  target_compile_definitions(codac-core PUBLIC codac_SHARED_DEFINE) # For compatibility...
endif()

# TO CHECK DEFAULTS DEPENDING ON THE COMPILER, ALSO THE .CMAKE...



target_compile_features(codac-core PUBLIC cxx_std_20)



# Getting header files from sources

  foreach(srcfile ${CODAC_CORE_SRC})
    if(srcfile MATCHES "\\.h$" OR srcfile MATCHES "\\.hpp$")
      list(APPEND CODAC_CORE_HDR ${srcfile})
      file(COPY ${srcfile} DESTINATION ${PROJECT_BINARY_DIR}/../../include)
    endif()
  endforeach()

# Generating the file codac-core.h

  set(CODAC_CORE_MAIN_HEADER ${PROJECT_BINARY_DIR}/codac-core.h)
  set(CODAC_MAIN_SUBHEADERS ${CODAC_MAIN_SUBHEADERS} "codac-core.h" PARENT_SCOPE)
  file(WRITE ${CODAC_CORE_MAIN_HEADER} "/* This file is generated by CMake */\n\n")
  file(APPEND ${CODAC_CORE_MAIN_HEADER} "#pragma once\n\n")
  foreach(header_path ${CODAC_CORE_HDR})
    get_filename_component(header_name ${header_path} NAME)
    if((NOT header_name MATCHES "^.*_addons.*$") AND (NOT header_name MATCHES "^.*_impl.*$"))
      file(APPEND ${CODAC_CORE_MAIN_HEADER} "#include <${header_name}>\n")
    endif()
  endforeach()
  file(COPY ${CODAC_CORE_MAIN_HEADER} DESTINATION ${PROJECT_BINARY_DIR}/../../include)


# Install files in system directories

#if (CODAC_INSTALL) # To find how to set the default value of this option...
  install(FILES ${CODAC_CORE_HDR} DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/codac-core)
  install(FILES ${CODAC_CORE_MAIN_HEADER} DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/codac-core)
#endif()


# IDEs should put the headers in a nice place
#source_group(
#  TREE "${PROJECT_SOURCE_DIR}/include"
#  PREFIX "Header Files"
#  FILES ${HEADER_LIST})
#source_group(
#  TREE "${PROJECT_SOURCE_DIR}/core"
#  PREFIX "Other Files"
#  FILES ${OTHER_LIST})
