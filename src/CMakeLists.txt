cmake_minimum_required(VERSION 3.1 FATAL_ERROR)

# general configuration for compilation:
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_POSITION_INDEPENDENT_CODE ON)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -O3 -g")


project(edep2supera)

set(CMAKE_PACKAGE_DIR "edep2supera")
set(PROJECT_SOURCE_DIR "./")

include_directories("./")

message("    Configuring edep2supera build")

add_definitions(-DBUILD_PYTHON_BINDINGS)
#########
# Python
#########
find_package(PythonLibs REQUIRED)
message("    Adding Python")
message("    INCLUDE: " , ${PYTHON_INCLUDE_DIRS})
include_directories(${PYTHON_INCLUDE_DIRS})

if(NOT PYTHON_EXECUTABLE)
    message( FATAL_ERROR "PYTHON_EXECUTABLE not found. Cannot build edep2supera...")
endif()

########
# Numpy
########
execute_process(
    COMMAND "${PYTHON_EXECUTABLE}" -c
    "from __future__ import print_function\ntry: import numpy; print(numpy.get_include(), end='')\nexcept:pass\n"
    OUTPUT_VARIABLE __numpy_path)

execute_process(
    COMMAND "${PYTHON_EXECUTABLE}" -c
    "from __future__ import print_function\ntry: import numpy; print(numpy.__version__, end='')\nexcept:pass\n"
    OUTPUT_VARIABLE __numpy_version)

find_path(PYTHON_NUMPY_INCLUDE_DIR numpy/arrayobject.h
    HINTS "${__numpy_path}" "${PYTHON_INCLUDE_PATH}" NO_DEFAULT_PATH)

if(PYTHON_NUMPY_INCLUDE_DIR)
    set(PYTHON_NUMPY_FOUND 1 CACHE INTERNAL "Python numpy found")
else()
    message( FATAL_ERROR "Numpy could not be located. Cannot build edep2supera...")
endif()

message("    Adding Numpy")
message("    INCLUDE: " ${PYTHON_NUMPY_INCLUDE_DIR})
include_directories(${PYTHON_NUMPY_INCLUDE_DIR})

#######
# ROOT
#######
find_package(ROOT REQUIRED
  COMPONENTS Geom Physics Matrix MathCore Tree RIO)
if(ROOT_FOUND)
  include(${ROOT_USE_FILE})
else()
  message( FATAL_ERROR "ROOT is needed but not found. Cannot build edep2supera...")
endif(ROOT_FOUND)

##########
# EDepSim
##########
find_package(EDepSim REQUIRED)

if(EDepSim_FOUND)
  set(EDepSim_DIR "${EDepSim_DIR}/../../../")
  include_directories(${EDepSim_DIR}/include)
else()
  message( FATAL_ERROR "EDepSim is needed but not found. Cannot build edep2supera...")
endif(EDepSim_FOUND)

#########
# Supera
#########
execute_process(COMMAND 
    "${PYTHON_EXECUTABLE}" -c "import supera;print(supera.get_lib_dir(),end='');"
    OUTPUT_VARIABLE
    SUPERA_LIB_DIR)
execute_process(COMMAND 
    "${PYTHON_EXECUTABLE}" -c "import supera;print(supera.get_includes(),end='');"
    OUTPUT_VARIABLE
    SUPERA_INCLUDE_DIR)

message("    Adding supera")
message("    INCLUDE: " ${SUPERA_INCLUDE_DIR})
message("    LIB:     " ${SUPERA_LIB_DIR}    )
include_directories(${SUPERA_INCLUDE_DIR})

####################################
# Add HDF5
####################################
#find_package(HDF5 REQUIRED)
#message("    Adding HDF5")
#message("    INCLUDE: " ${HDF5_INCLUDE_DIR})
#include_directories(${HDF5_INCLUDE_DIR})

####################################
# Add larcv
# ###################################
#execute_process(COMMAND
# "${PYTHON_EXECUTABLE}" -c "import larcv;print(larcv.get_lib_dir(),end='');"
# OUTPUT_VARIABLE
# LARCV_LIB_DIR)
#execute_process(COMMAND
# "${PYTHON_EXECUTABLE}" -c "import larcv;print(larcv.get_includes(),end='');"
# OUTPUT_VARIABLE
# LARCV_INCLUDE_DIR)
#
# message("    Adding larcv")
# message("    INCLUDE: " ${LARCV_INCLUDE_DIR})
# message("    LIB:     " ${LARCV_LIB_DIR}    )
# include_directories(${LARCV_INCLUDE_DIR})

add_subdirectory(edep2supera)
