cmake_minimum_required(VERSION 3.14...3.25)

list(APPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/scripts/CMakeModules/)
include(version_from_git)
version_from_git() # Obtains the version number from Git tags

project(codac VERSION ${VERSION} LANGUAGES CXX)

if(NOT VERSION_ID)
  set(PROJECT_VERSION_FULL ${PROJECT_VERSION})
else()
  set(PROJECT_VERSION_FULL "${PROJECT_VERSION}${VERSION_ID}")
endif()
message(STATUS "Full project version is ${PROJECT_VERSION_FULL}")

set(PROJECT_DESCRIPTION
    "Codac is a library providing tools for constraint programming over reals and trajectories.")
set(PROJECT_LONG_DESCRIPTION
    "${PROJECT_DESCRIPTION}")
set(PROJECT_HOMEPAGE_URL "http://codac.io")
message(STATUS "Configuring build for ${PROJECT_NAME} ${PROJECT_VERSION}")

include(GNUInstallDirs)

# Only do these if this is the main project, and not if it is included through add_subdirectory
if(CMAKE_PROJECT_NAME STREQUAL PROJECT_NAME)

  # Let's nicely support folders in IDEs
  set_property(GLOBAL PROPERTY USE_FOLDERS ON)

  option(BUILD_SHARED_LIBS "Build shared libs" OFF) # To rename to CODAC_BUILD_SHARED_LIBS...?#####################################
  if(NOT BUILD_SHARED_LIBS)
    set(CMAKE_POSITION_INDEPENDENT_CODE ON)
  endif()

  if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release)
  endif()

  if(MSVC)
    add_compile_options(/W4)
  else()
    add_compile_options(-Wall -Wextra -Wpedantic)
  endif()

  option(CODAC_DISABLE_PARALLEL_BUILD "Disable parallel build" OFF)
  if(NOT CODAC_DISABLE_PARALLEL_BUILD)
    if(MSVC)
      add_compile_options(/MP)
    endif()
    cmake_host_system_information(RESULT _cores QUERY NUMBER_OF_LOGICAL_CORES)
    if(NOT _cores OR _cores LESS 1)
      set(_cores 1)
    endif()
    #if(_cores LESS 8)
      set(_parallel_level ${_cores})
    #else()
    #  set(_parallel_level 8)
    #endif()
    if(NOT DEFINED ENV{CMAKE_BUILD_PARALLEL_LEVEL})
      set(ENV{CMAKE_BUILD_PARALLEL_LEVEL} "${_parallel_level}")
      message(STATUS "Defaulting CMAKE_BUILD_PARALLEL_LEVEL to ${_parallel_level}")
    endif()
  endif()

endif()

option(CODAC_INSTALL "Generate the install target" ON)
  
if(FAST_RELEASE) # To rename to CODAC_FAST_RELEASE...? Why it is not explicitely an option?#####################################
  add_compile_definitions(FAST_RELEASE)
endif()

if(NOT WIN32)
  string(ASCII 27 Esc)
  set(COLOR_RED "${Esc}[31m")
  set(COLOR_BLUE "${Esc}[36m")
  set(COLOR_RESET "${Esc}[m")
else()
  set(COLOR_RED "")
  set(COLOR_BLUE "")
  set(COLOR_RESET "")
endif()

message(STATUS "${COLOR_BLUE}Configuring ${PROJECT_NAME} in ${CMAKE_BUILD_TYPE} mode.${COLOR_RESET}")

if(FAST_RELEASE)
  if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    message(STATUS "${COLOR_RED}Option FAST_RELEASE cannot be used in Debug mode.${COLOR_RESET}")
  else()
    message(STATUS "${COLOR_BLUE}Option FAST_RELEASE enabled.${COLOR_RESET}")
  endif()
else()
  message(STATUS "Note: in Release build mode, the option FAST_RELEASE=ON would allow faster computations if enabled.")
endif()

# Temporary attempts to fix errors similar to: 
# _ number of sections exceeded object file format limit.
# _ out of memory allocating XXX bytes.
# _ string table overflow.
# _ .obj: file too big.
# See also per-file compile options...
if(MSVC)
  add_compile_options(/bigobj)
elseif(MINGW)
  add_compile_options(-Wa,-mbig-obj)
  #if (CMAKE_SIZEOF_VOID_P EQUAL 4)
  #  add_compile_options(-g0)
  #endif()
  #if (CMAKE_BUILD_TYPE STREQUAL "Debug")
  #  add_compile_options(-Os)
  #endif()
endif()

include(FetchContent)

option(WITH_CAPD "Using CAPD for accurate integration of ODEs" OFF) # To rename to CODAC_WITH_CAPD...?#####################################
if(WITH_CAPD)
  find_package(CAPD REQUIRED)
  message(STATUS "Found CAPD version ${CAPD_VERSION} in ${CAPD_DIR}")
endif()
# Adds capd::capd

option(WITH_IBEX "Using IBEX" OFF) # To rename to CODAC_WITH_IBEX...?#####################################
if(WITH_IBEX)
  find_package(IBEX 2.8.9.20260215 REQUIRED)
  ibex_init_common() # IBEX should have installed this function
  message(STATUS "Found IBEX version ${IBEX_VERSION} in ${IBEX_DIR}")
endif()
# Adds Ibex::ibex

option(CODAC_ENABLE_FIND_PACKAGE_GAOL "CODAC_ENABLE_FIND_PACKAGE_GAOL" OFF)
if(CODAC_ENABLE_FIND_PACKAGE_GAOL)
    find_package(gaol 4.2.3.20260215 QUIET) # Use gaol_DIR or CMAKE_PREFIX_PATH to specify a custom gaol installation path...
endif()
if(gaol_FOUND)
	message(STATUS "Found gaol version ${gaol_VERSION} in ${gaol_DIR}")
else()
	message(STATUS "Attempt to get gaol automatically...")
	set(GAOL_ENABLE_PRESERVE_ROUNDING OFF CACHE BOOL "Transparently manipulates the FPU control flags")
	set(GAOL_ENABLE_OPTIMIZE ON CACHE BOOL "Compile gaol with full optimization")
	set(GAOL_ENABLE_VERBOSE_MODE OFF CACHE BOOL "Use verbose mode")
	#FetchContent_Declare(gaol URL https://github.com/lebarsfa/gaol/archive/refs/heads/master.zip)
	FetchContent_Declare(gaol URL https://github.com/lebarsfa/gaol/archive/refs/heads/prerelease.zip)
	#FetchContent_Declare(gaol URL https://github.com/lebarsfa/gaol/archive/refs/heads/dev.zip)
	#FetchContent_Declare(gaol SOURCE_DIR "${CMAKE_SOURCE_DIR}/../GAOL")
	FetchContent_MakeAvailable(gaol)
    if(TARGET gaol::gaol)
        set_target_properties(gaol PROPERTIES
            CXX_STANDARD 11
            CXX_STANDARD_REQUIRED ON
            CXX_STANDARD_EXTENSIONS OFF
        )
    endif()
    if(TARGET mathlib::ultim)
        target_compile_options(ultim PRIVATE
            $<$<CXX_COMPILER_ID:GNU>:-Wno-missing-braces>
        )
        set_target_properties(ultim PROPERTIES
            CXX_STANDARD 11
            CXX_STANDARD_REQUIRED ON
            CXX_STANDARD_EXTENSIONS OFF
        )
    endif()
endif()
# Adds gaol::gaol and mathlib::ultim

#option(CODAC_ENABLE_FIND_PACKAGE_EIGEN3 "CODAC_ENABLE_FIND_PACKAGE_EIGEN3" OFF)
#if(CODAC_ENABLE_FIND_PACKAGE_EIGEN3)
#  find_package(Eigen3 3.4.0 QUIET) # Use Eigen3_DIR or CMAKE_PREFIX_PATH to specify a custom Eigen3 installation path...
#endif()
#if(Eigen3_FOUND)
#  message(STATUS "Found Eigen3 version ${Eigen3_VERSION} in ${Eigen3_DIR}")
#else()
  #message(STATUS "Attempt to get Eigen3 automatically...")
  message(STATUS "Attempt to get custom Eigen3...")
  FetchContent_Declare(Eigen3 URL http://github.com/codac-team/eigen/archive/refs/heads/eigen-5.0.zip)
  #FetchContent_Declare(Eigen3 URL ${CMAKE_CURRENT_SOURCE_DIR}/3rd/eigen-a0dc3994bd5d4e804dab58decb78c56a5af516ba.zip) # If needed to be self-contained...
  #
  #FetchContent_MakeAvailable(Eigen3)
  #
  # This avoids the install part of Eigen3 to override the system-wide Eigen3 installation if any...
  cmake_policy(PUSH)
  if (POLICY CMP0169)
    cmake_policy(SET CMP0169 OLD)
  endif()
  FetchContent_Populate(Eigen3)
  cmake_policy(POP)
  add_library(${PROJECT_NAME}-eigen INTERFACE)
  target_include_directories(${PROJECT_NAME}-eigen 
      INTERFACE
          "$<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}/codac-eigen>"
          "$<BUILD_INTERFACE:${eigen3_SOURCE_DIR}>"
  )
  if (CODAC_INSTALL)
    install(DIRECTORY ${eigen3_SOURCE_DIR}/unsupported/Eigen DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/codac-eigen/unsupported)
    install(DIRECTORY ${eigen3_SOURCE_DIR}/Eigen DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/codac-eigen)
  endif()
  add_library(${PROJECT_NAME}::${PROJECT_NAME}-eigen ALIAS ${PROJECT_NAME}-eigen)
  #
#endif()
## Adds Eigen3::Eigen
# Adds ${PROJECT_NAME}-eigen
 
find_package(Threads REQUIRED)
# Adds Threads::Threads

add_subdirectory(src) # C++ sources
add_library(${PROJECT_NAME}::${PROJECT_NAME}-core ALIAS ${PROJECT_NAME}-core)
add_library(${PROJECT_NAME}::${PROJECT_NAME}-graphics ALIAS ${PROJECT_NAME}-graphics)
add_library(${PROJECT_NAME}::${PROJECT_NAME}-unsupported ALIAS ${PROJECT_NAME}-unsupported)

option(WITH_PYTHON "Build Python binding" OFF) # To rename to CODAC_WITH_PYTHON...?#####################################
if(WITH_PYTHON)
  option(WITH_DOC "Build documentation (Doxygen + Sphinx manual)" ON) # To rename to CODAC_WITH_DOC...?#####################################
else()
  option(WITH_DOC "Build documentation (Doxygen + Sphinx manual)" OFF)
endif()

if(WITH_DOC)
  add_subdirectory(doc)
endif()

if(WITH_PYTHON)
  set(CMAKE_POSITION_INDEPENDENT_CODE TRUE)
  add_subdirectory(python)
endif()


#####################################

  option(BUILD_TESTS "Build test" OFF) # To rename to CODAC_BUILD_TESTS...?#####################################
  if(${CMAKE_VERSION} VERSION_LESS "3.14.0")
    # Disable tests due to FetchContent_MakeAvailable which needs CMake >= 3.14.
    set(BUILD_TESTS OFF CACHE BOOL "Build test" FORCE)
  endif()
  if(BUILD_TESTS)
    include(CTest)
    add_custom_target(check#####################################
                      COMMAND ${CMAKE_CTEST_COMMAND} --output-on-failure $(ARGS)
                      DEPENDS ${PROJECT_NAME} COMMENT "Running the tests")

    add_subdirectory(tests)
  endif()

  option(TEST_EXAMPLES "Testing examples" OFF) # To rename to CODAC_TEST_EXAMPLES...?#####################################
  add_subdirectory(examples) # examples are tested as integration tests

#####################################


if (CODAC_INSTALL)
    # From https://gitlab.com/CLIUtils/modern-cmake/-/blob/b47192867e86e5f00f70ec860638a28473edc8bf/chapters/install/installing.md
    # and https://unclejimbo.github.io/2018/06/08/Modern-CMake-for-Library-Developers/
    install(TARGETS ${PROJECT_NAME}-core ${PROJECT_NAME}-graphics ${PROJECT_NAME}-unsupported ${PROJECT_NAME}-eigen #eigen
            EXPORT ${PROJECT_NAME}Targets
            LIBRARY DESTINATION lib
            ARCHIVE DESTINATION lib
    #        RUNTIME DESTINATION bin
    #        INCLUDES DESTINATION include
            )

    include(CMakePackageConfigHelpers)
    write_basic_package_version_file(
        ${PROJECT_NAME}ConfigVersion.cmake
        #VERSION ${PACKAGE_VERSION}
        COMPATIBILITY AnyNewerVersion
        )

    install(EXPORT ${PROJECT_NAME}Targets
            #FILE ${PROJECT_NAME}Targets.cmake
            NAMESPACE ${PROJECT_NAME}::
            DESTINATION share/cmake/codac
             )

    configure_file(codacConfig.cmake.in ${PROJECT_NAME}Config.cmake @ONLY)
    install(FILES "${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}Config.cmake"
                  "${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}ConfigVersion.cmake"
            DESTINATION share/cmake/codac
            )

    export(TARGETS ${PROJECT_NAME}-core ${PROJECT_NAME}-graphics ${PROJECT_NAME}-unsupported ${PROJECT_NAME}-eigen #eigen
           NAMESPACE ${PROJECT_NAME}::
           FILE ${PROJECT_NAME}Targets.cmake)
endif()
