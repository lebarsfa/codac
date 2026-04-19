# ==================================================================
#  codac / basics example - cmake configuration file
# ==================================================================

cmake_minimum_required(VERSION 3.5)
project(codac_example LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Adding gaol

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
  	FetchContent_Declare(gaol URL https://github.com/lebarsfa/gaol/archive/refs/heads/prerelease.zip)
  	#FetchContent_Declare(gaol URL https://github.com/lebarsfa/gaol/archive/refs/heads/dev.zip)
  	#FetchContent_Declare(gaol SOURCE_DIR "P:/devel/GitHub/GAOL")
  	FetchContent_MakeAvailable(gaol)
  	# Adds gaol::gaol
    if(TARGET gaol)
    set_target_properties(gaol PROPERTIES
        CXX_STANDARD 11
        CXX_STANDARD_REQUIRED ON
        CXX_STANDARD_EXTENSIONS OFF
    )
    endif()
  endif()

# Adding Codac

# In case you installed Codac in a local directory, you need 
# to specify its path with the CMAKE_PREFIX_PATH option.
# set(CMAKE_PREFIX_PATH "~/codac/build_install")

find_package(CODAC REQUIRED)
message(STATUS "Found Codac version ${CODAC_VERSION}")

# Compilation

if(FAST_RELEASE)
  add_compile_definitions(FAST_RELEASE)
  message(STATUS "You are running Codac in fast release mode. (option -DCMAKE_BUILD_TYPE=Release is required)")
endif()

add_executable(${PROJECT_NAME} main.cpp)
target_compile_options(${PROJECT_NAME} PUBLIC ${CODAC_CXX_FLAGS})
target_include_directories(${PROJECT_NAME} SYSTEM PUBLIC ${CODAC_INCLUDE_DIRS} ${CODAC_CAPD_INCLUDE_DIR})
target_link_libraries(${PROJECT_NAME} PRIVATE
  ${CODAC_LIBRARIES}
  ${CODAC_CAPD_LIBRARY}
  capd::capd
  gaol::gaol
)