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

list(APPEND CODAC_CORE_SRC

    actions/codac2_OctaSym.cpp
    actions/codac2_OctaSym.h

    contractors/codac2_Ctc.h
    contractors/codac2_CtcAction.cpp
    contractors/codac2_CtcAction.h
    contractors/codac2_CtcCartProd.h
    contractors/codac2_CtcCross.cpp
    contractors/codac2_CtcCross.h
    contractors/codac2_CtcCtcBoundary.cpp
    contractors/codac2_CtcCtcBoundary.h
    contractors/codac2_CtcDist.cpp
    contractors/codac2_CtcDist.h
    contractors/codac2_CtcEllipse.cpp
    contractors/codac2_CtcEllipse.h
    contractors/codac2_CtcEmpty.cpp
    contractors/codac2_CtcEmpty.h
    contractors/codac2_CtcFixpoint.cpp
    contractors/codac2_CtcFixpoint.h
    contractors/codac2_CtcIdentity.cpp
    contractors/codac2_CtcIdentity.h
    contractors/codac2_CtcInnerOuter.h
    contractors/codac2_CtcInter.h
    contractors/codac2_CtcInverse.h
    contractors/codac2_CtcInverseNotIn.h
    contractors/codac2_CtcLazy.cpp
    contractors/codac2_CtcLazy.h
    contractors/codac2_CtcNot.h
    contractors/codac2_CtcUnion.h
    contractors/codac2_CtcPointCloud.cpp
    contractors/codac2_CtcPointCloud.h
    contractors/codac2_CtcPolar.cpp
    contractors/codac2_CtcPolar.h
    contractors/codac2_CtcPolygon.cpp
    contractors/codac2_CtcPolygon.h
    contractors/codac2_CtcProj.cpp
    contractors/codac2_CtcProj.h
    contractors/codac2_CtcSegment.cpp
    contractors/codac2_CtcSegment.h
    contractors/codac2_CtcWrapper.h
    contractors/codac2_linear_ctc.cpp
    contractors/codac2_linear_ctc.h

    domains/codac2_BoolInterval.h
    domains/codac2_Domain.h
    domains/codac2_Wrapper.h
    domains/ellipsoid/codac2_Ellipsoid.cpp
    domains/ellipsoid/codac2_Ellipsoid.h
    domains/ellipsoid/codac2_Ellipsoid_utils.cpp
    domains/ellipsoid/codac2_Ellipsoid_utils.h
    domains/interval/codac2_Interval.h
    domains/interval/codac2_Interval_impl.h
    domains/interval/codac2_Interval_operations.h
    domains/interval/codac2_Interval_operations_impl.h
    domains/interval/codac2_IntervalMatrix.h
    domains/interval/codac2_IntervalRow.h
    domains/interval/codac2_IntervalVector.h
    domains/paving/codac2_Paving.cpp
    domains/paving/codac2_Paving.h
    domains/paving/codac2_PavingNode.h
    domains/paving/codac2_Subpaving.h
    domains/tube/codac2_Slice.h
    domains/tube/codac2_SliceBase.cpp
    domains/tube/codac2_SliceBase.h
    domains/tube/codac2_SlicedTube.h
    domains/tube/codac2_SlicedTube_integral_impl.h
    domains/tube/codac2_SlicedTubeBase.h
    domains/tube/codac2_TDomain.cpp
    domains/tube/codac2_TDomain.h
    domains/tube/codac2_TSlice.cpp
    domains/tube/codac2_TSlice.h
    domains/tube/codac2_TubeBase.h

    functions/codac2_ExprBase.cpp
    functions/codac2_ExprBase.h
    functions/codac2_FunctionArgsList.h
    functions/codac2_FunctionBase.h
    functions/codac2_VarBase.h
    functions/analytic/codac2_analytic_constants.h
    functions/analytic/codac2_analytic_variables.cpp
    functions/analytic/codac2_analytic_variables.h
    functions/analytic/codac2_AnalyticExpr.h
    functions/analytic/codac2_AnalyticExprWrapper.h
    functions/analytic/codac2_AnalyticFunction.h
    functions/analytic/codac2_AnalyticType.h
    functions/analytic/codac2_ExprType.h
    functions/set/codac2_set_operations.h
    functions/set/codac2_set_operators.h
    functions/set/codac2_set_variables.h
    functions/set/codac2_SetExpr.h
    functions/set/codac2_SetFunction.h
    functions/set/codac2_set_constants.h

    geometry/codac2_ConvexPolygon.cpp
    geometry/codac2_ConvexPolygon.h
    geometry/codac2_geometry.cpp
    geometry/codac2_geometry.h
    geometry/codac2_Polygon.cpp
    geometry/codac2_Polygon.h
    geometry/codac2_Segment.cpp
    geometry/codac2_Segment.h
    
    operators/codac2_abs.h
    operators/codac2_acos.h
    operators/codac2_arith_add.h
    operators/codac2_arith_div.h
    operators/codac2_arith_mul.cpp
    operators/codac2_arith_mul.h
    operators/codac2_arith_sub.h
    operators/codac2_asin.h
    operators/codac2_atan.h
    operators/codac2_atan2.h
    operators/codac2_ceil.h
    operators/codac2_chi.h
    operators/codac2_component.h
    operators/codac2_cos.h
    operators/codac2_cosh.h
    operators/codac2_cross_prod.h
    operators/codac2_det.h
    operators/codac2_exp.h
    operators/codac2_extend.h
    operators/codac2_flatten.h
    operators/codac2_floor.h
    operators/codac2_log.h
    operators/codac2_mat.h
    operators/codac2_max.h
    operators/codac2_min.h
    operators/codac2_mod.h
    operators/codac2_operators.h
    operators/codac2_pow.h
    operators/codac2_sign.h
    operators/codac2_sin.h
    operators/codac2_sinh.h
    operators/codac2_sqr.h
    operators/codac2_sqrt.h
    operators/codac2_subvector.h
    operators/codac2_tan.h
    operators/codac2_tanh.h
    operators/codac2_transpose.h
    operators/codac2_vec.h

    matrices/eigen/Matrix_addons/codac2_Matrix_addons_Base.h
    matrices/eigen/Matrix_addons/codac2_Matrix_addons_include.h
    matrices/eigen/Matrix_addons/codac2_Matrix_addons_IntervalMatrix.h
    matrices/eigen/Matrix_addons/codac2_Matrix_addons_IntervalMatrixBase.h
    matrices/eigen/Matrix_addons/codac2_Matrix_addons_IntervalVector.h
    matrices/eigen/Matrix_addons/codac2_Matrix_addons_Matrix.h
    matrices/eigen/Matrix_addons/codac2_Matrix_addons_MatrixBase.h
    matrices/eigen/Matrix_addons/codac2_Matrix_addons_Vector.h
    matrices/eigen/Matrix_addons/codac2_Matrix_addons_VectorBase.h

    matrices/eigen/MatrixBase_addons/codac2_MatrixBase_addons_Base.h
    matrices/eigen/MatrixBase_addons/codac2_MatrixBase_addons_include.h
    matrices/eigen/MatrixBase_addons/codac2_MatrixBase_addons_IntervalMatrix.h
    matrices/eigen/MatrixBase_addons/codac2_MatrixBase_addons_IntervalMatrixBase.h
    matrices/eigen/MatrixBase_addons/codac2_MatrixBase_addons_IntervalVector.h
    matrices/eigen/MatrixBase_addons/codac2_MatrixBase_addons_Matrix.h
    matrices/eigen/MatrixBase_addons/codac2_MatrixBase_addons_MatrixBase.h
    matrices/eigen/MatrixBase_addons/codac2_MatrixBase_addons_Vector.h
    matrices/eigen/MatrixBase_addons/codac2_MatrixBase_addons_VectorBase.h
    
    matrices/codac2_cart_prod.h
    matrices/codac2_GaussJordan.cpp
    matrices/codac2_GaussJordan.h
    matrices/codac2_hull.h
    matrices/codac2_inversion.cpp
    matrices/codac2_inversion.h
    matrices/codac2_matrices.h
    matrices/codac2_Matrix.h
    matrices/codac2_Row.h
    matrices/codac2_Vector.h
    matrices/codac2_IntvFullPivLU.h
    matrices/codac2_IntvFullPivLU.cpp


    paver/codac2_pave.cpp
    paver/codac2_pave.h

    proj/codac2_ProjBase.cpp
    proj/codac2_ProjBase.h

    separators/codac2_Sep.h
    separators/codac2_SepAction.cpp
    separators/codac2_SepAction.h
    separators/codac2_SepCartProd.cpp
    separators/codac2_SepCartProd.h
    separators/codac2_SepChi.cpp
    separators/codac2_SepChi.h
    separators/codac2_SepCross.h
    separators/codac2_SepCtcBoundary.cpp
    separators/codac2_SepCtcBoundary.h
    separators/codac2_SepCtcPair.cpp
    separators/codac2_SepCtcPair.h
    separators/codac2_SepEllipse.cpp
    separators/codac2_SepEllipse.h
    separators/codac2_SepInter.cpp
    separators/codac2_SepInter.h
    separators/codac2_SepInverse.h
    separators/codac2_SepNot.h
    separators/codac2_SepPolygon.cpp
    separators/codac2_SepPolygon.h
    separators/codac2_SepProj.cpp
    separators/codac2_SepProj.h
    separators/codac2_SepTransform.cpp
    separators/codac2_SepTransform.h
    separators/codac2_SepUnion.cpp
    separators/codac2_SepUnion.h
    separators/codac2_SepWrapper.cpp
    separators/codac2_SepWrapper.h

    tools/codac2_Approx.h
    tools/codac2_assert.h
    tools/codac2_Collection.h
    tools/codac2_Index.h
    tools/codac2_math.h
    tools/codac2_object_file_format.cpp
    tools/codac2_object_file_format.h
    tools/codac2_RobotSimulator.cpp
    tools/codac2_RobotSimulator.h
    tools/codac2_serialize.h
    tools/codac2_template_tools.h
    tools/codac2_transformations.cpp
    tools/codac2_transformations.h
    tools/codac2_TypeInfo.h

    trajectory/codac2_AnalyticTraj.h
    trajectory/codac2_SampledTraj.h
    trajectory/codac2_SampledTraj_operations.cpp
    trajectory/codac2_SampledTraj_operations.h
    trajectory/codac2_Traj_operator.h
    trajectory/codac2_TrajBase.h
)

# Make an automatic library - will be static or dynamic based on user setting
add_library(codac-core ${CODAC_CORE_SRC})

# Inspired from https://unclejimbo.github.io/2018/06/08/Modern-CMake-for-Library-Developers/
#add_library(codac::core ALIAS core)

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
        "$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/graphics>"
        "$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/graphics/ipe>"
        "$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/graphics/vibes>"
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

target_link_libraries(codac-core PUBLIC codac-graphics gaol::gaol Eigen3::Eigen)

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

  install(FILES ${CODAC_CORE_HDR} DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/codac-core)
  install(FILES ${CODAC_CORE_MAIN_HEADER} DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/codac-core)


# IDEs should put the headers in a nice place
#source_group(
#  TREE "${PROJECT_SOURCE_DIR}/include"
#  PREFIX "Header Files"
#  FILES ${HEADER_LIST})
#source_group(
#  TREE "${PROJECT_SOURCE_DIR}/core"
#  PREFIX "Other Files"
#  FILES ${OTHER_LIST})
