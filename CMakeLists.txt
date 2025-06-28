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

endif()

####################################### TO IMPROVE (BEGINNING) #########################################

  if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Debug)
    if(MSVC)
      ##########################add_compile_options(/MD) # Temporary fix to avoid debug vs release inconsistencies...
    else()
      add_compile_options(-O3)
    endif()
  endif()
  
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

  if(MSVC)
    add_compile_options(/W4)
  else()
    add_compile_options(-Wall -Wextra -Wpedantic)
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

find_package(gaol QUIET)
if(gaol_FOUND)
	message(STATUS "Found gaol version ${gaol_VERSION} in ${gaol_DIR}")
else()
	message(STATUS "Attempt to install gaol automatically...")
	set(GAOL_ENABLE_PRESERVE_ROUNDING OFF CACHE BOOL "Transparently manipulates the FPU control flags")
	set(GAOL_ENABLE_OPTIMIZE ON CACHE BOOL "Compile gaol with full optimization")
	set(GAOL_ENABLE_VERBOSE_MODE OFF CACHE BOOL "Use verbose mode")
	#FetchContent_Declare(gaol URL https://github.com/lebarsfa/gaol/archive/refs/heads/master.zip)
	#FetchContent_Declare(gaol URL https://github.com/lebarsfa/gaol/archive/refs/heads/prerelease.zip)
	FetchContent_Declare(gaol URL https://github.com/lebarsfa/gaol/archive/refs/heads/dev.zip)
	#FetchContent_Declare(gaol SOURCE_DIR "P:/devel/GitHub/GAOL")
	FetchContent_MakeAvailable(gaol)
	# Adds gaol::gaol
    if(TARGET gaol::gaol)
        set_target_properties(gaol PROPERTIES
            CXX_STANDARD 11
            CXX_STANDARD_REQUIRED ON
            CXX_STANDARD_EXTENSIONS OFF
        )
    endif()
endif()

message(STATUS "Installing custom Eigen3...")
FetchContent_Declare(Eigen3 URL ${CMAKE_SOURCE_DIR}/../codac/3rd/eigen)
#FetchContent_Declare(Eigen3 SOURCE_DIR "P:/devel/GitHub/codac/3rd/eigen")
FetchContent_MakeAvailable(Eigen3)
# Adds Eigen3::Eigen
  
  add_subdirectory(src) # C++ sources
####################################  add_subdirectory(doc) # documentation (Doxygen + Sphinx manual)

  # Python binding:
  option(WITH_PYTHON "Build Python binding" OFF)
  if(WITH_PYTHON)
    set(CMAKE_POSITION_INDEPENDENT_CODE TRUE)
    add_subdirectory(python)
  endif()

if (CMAKE_PROJECT_NAME STREQUAL PROJECT_NAME)
    # Only at top-level do we generate install() and install(EXPORT)

    # From https://gitlab.com/CLIUtils/modern-cmake/-/blob/b47192867e86e5f00f70ec860638a28473edc8bf/chapters/install/installing.md
    # and https://unclejimbo.github.io/2018/06/08/Modern-CMake-for-Library-Developers/
    install(TARGETS codac-core codac-graphics #####################################
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

    # Inspired from https://github.com/fmtlib/fmt/blob/7d4662f7ab39f70107b6e8899bcc4076fe947a39/CMakeLists.txt
    export(TARGETS codac-core codac-graphics #####################################
            NAMESPACE codac::
            FILE codacTargets.cmake)

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
else()
    # Sub-project: give users namespace aliases

    add_library(codac::codac-core ALIAS codac-core)
    add_library(codac::codac-graphics ALIAS codac-graphics)
endif()
