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

# Only do these if this is the main project, and not if it is included through add_subdirectory
if(CMAKE_PROJECT_NAME STREQUAL PROJECT_NAME)

  # Let's nicely support folders in IDEs
  set_property(GLOBAL PROPERTY USE_FOLDERS ON)

  option(BUILD_SHARED_LIBS "Build shared libs" OFF)
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

####################################### TO REMOVE LATER (BEGINNING) #########################################
if(NOT DEFINED ENV{CMAKE_BUILD_PARALLEL_LEVEL})
  set(ENV{CMAKE_BUILD_PARALLEL_LEVEL} 8)
  message(STATUS "Defaulting CMAKE_BUILD_PARALLEL_LEVEL to 8")
endif()
if(MSVC)
  add_compile_options(/MP)
endif()
####################################### TO REMOVE LATER (END) #########################################

endif()

option(CODAC_INSTALL "Generate the install target" ON)

####################################### TO IMPROVE (BEGINNING) #########################################
  
  if(FAST_RELEASE)
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
  if(MSVC)
    add_compile_options(/bigobj)
  elseif(MINGW)
    add_compile_options(-Wa,-mbig-obj)
    if (CMAKE_SIZEOF_VOID_P EQUAL 4)
      add_compile_options(-g0)
    endif()
    if (CMAKE_BUILD_TYPE STREQUAL "Debug")
      add_compile_options(-Os)
    endif()
  endif()

####################################### TO IMPROVE (END) #########################################

include(FetchContent)

option(ENABLE_FIND_PACKAGE_GAOL "ENABLE_FIND_PACKAGE_GAOL" OFF)
if(ENABLE_FIND_PACKAGE_GAOL)
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
	#FetchContent_Declare(gaol URL https://github.com/lebarsfa/gaol/archive/refs/heads/prerelease.zip)
	FetchContent_Declare(gaol URL https://github.com/lebarsfa/gaol/archive/refs/heads/dev.zip)
	#FetchContent_Declare(gaol SOURCE_DIR "P:/devel/GitHub/GAOL")
	FetchContent_MakeAvailable(gaol)
	# Adds gaol::gaol and mathlib::ultim
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

  option(ENABLE_FIND_PACKAGE_EIGEN3 "ENABLE_FIND_PACKAGE_EIGEN3" OFF)
  if(ENABLE_FIND_PACKAGE_EIGEN3)
    find_package(Eigen3 3.4.0 QUIET) # Use Eigen3_DIR or CMAKE_PREFIX_PATH to specify a custom Eigen3 installation path...
  endif()
  if(Eigen3_FOUND)
    message(STATUS "Found Eigen3 version ${Eigen3_VERSION} in ${Eigen3_DIR}")
  else()
    message(STATUS "Attempt to install custom Eigen3 automatically...")
    FetchContent_Declare(Eigen3 URL http://github.com/codac-team/eigen/archive/refs/heads/eigen-5.0.zip)
    #FetchContent_Declare(Eigen3 URL ${CMAKE_CURRENT_SOURCE_DIR}/3rd/eigen-a0dc3994bd5d4e804dab58decb78c56a5af516ba.zip) # If needed to be self-contained...
    FetchContent_MakeAvailable(Eigen3)
  endif()
  # Adds Eigen3::Eigen

  option(WITH_IBEX "Using IBEX" OFF)
  if(WITH_IBEX)
    find_package(IBEX 2.8.9.20260215 REQUIRED)
    ibex_init_common() # IBEX should have installed this function
    message(STATUS "Found IBEX version ${IBEX_VERSION}")
  endif()

  option(WITH_CAPD "Using CAPD for accurate integration of ODEs" OFF)
  if(WITH_CAPD)
    find_package(CAPD REQUIRED)
    message(STATUS "Found CAPD version ${CAPD_VERSION}")
  endif()
  
  add_subdirectory(src) # C++ sources
  add_library(codac::codac-core ALIAS codac-core)
  add_library(codac::codac-graphics ALIAS codac-graphics)
####################################  add_subdirectory(doc) # documentation (Doxygen + Sphinx manual)

  # Python binding:
  option(WITH_PYTHON "Build Python binding" OFF)
  if(WITH_PYTHON)
    set(CMAKE_POSITION_INDEPENDENT_CODE TRUE)
    add_subdirectory(python)
  endif()

if (CODAC_INSTALL)
    # From https://gitlab.com/CLIUtils/modern-cmake/-/blob/b47192867e86e5f00f70ec860638a28473edc8bf/chapters/install/installing.md
    # and https://unclejimbo.github.io/2018/06/08/Modern-CMake-for-Library-Developers/
    install(TARGETS codac-core codac-graphics eigen #####################################
            EXPORT codacTargets
            LIBRARY DESTINATION lib
            ARCHIVE DESTINATION lib
    #        RUNTIME DESTINATION bin
    #        INCLUDES DESTINATION include
            )

    include(CMakePackageConfigHelpers)
    write_basic_package_version_file(
        codacConfigVersion.cmake
        #VERSION ${PACKAGE_VERSION}
        COMPATIBILITY AnyNewerVersion
        )

    install(EXPORT codacTargets
            #FILE codacTargets.cmake
            NAMESPACE codac::
            DESTINATION share/cmake/codac
             )

    configure_file(codacConfig.cmake.in codacConfig.cmake @ONLY)
    install(FILES "${CMAKE_CURRENT_BINARY_DIR}/codacConfig.cmake"
                  "${CMAKE_CURRENT_BINARY_DIR}/codacConfigVersion.cmake"
            DESTINATION share/cmake/codac
            )

    export(TARGETS codac-core codac-graphics eigen #####################################
           NAMESPACE codac::
           FILE codacTargets.cmake)
endif()
