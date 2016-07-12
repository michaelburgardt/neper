# This file is part of the Neper software package.
# Copyright (C) 2003-2015, Romain Quey.
# See the COPYING file in the top-level directory.

cmake_minimum_required(VERSION 2.6)
project(prc)

set (PRC_VERSION_MAJOR 1)
set (PRC_VERSION_MINOR 1)
set (PRC_VERSION_PATCH 0)

# configure a header file to pass some of the CMake settings
# to the source code
configure_file (
  "${PROJECT_SOURCE_DIR}/prc_config.h.in"
  "${PROJECT_BINARY_DIR}/prc_config.h"
)

include_directories("${PROJECT_BINARY_DIR}")
include_directories("${PROJECT_SOURCE_DIR}")

add_definitions(-Wall -W -g -O3)
set (CMAKE_EXE_LINKER_FLAGS ${CMAKE_EXE_LINKER_FLAGS})

add_library (prc
PRCbitStream.cc
PRCdouble.cc
oPRCFile.cc
writePRC.cc      
)

## Link libraries

set(CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/cmake ${CMAKE_MODULE_PATH})

find_library (MATHLIB m)
if (MATHLIB)
  set (EXTRA_LIBS ${EXTRA_LIBS} ${MATHLIB})
endif ()

target_link_libraries (prc ${EXTRA_LIBS})

# Installation of the library
# SET (CMAKE_INSTALL_PREFIX /usr/local/lib/ut)
# SET (DESTDIR /usr/local/lib/ut)

set (INCLUDE_FILES ${INCLUDE_FILES} ${PROJECT_BINARY_DIR}/PRCbitStream.h)
set (INCLUDE_FILES ${INCLUDE_FILES} ${PROJECT_BINARY_DIR}/PRCdouble.h)
set (INCLUDE_FILES ${INCLUDE_FILES} ${PROJECT_BINARY_DIR}/PRC.h)
set (INCLUDE_FILES ${INCLUDE_FILES} ${PROJECT_BINARY_DIR}/oPRCFile.h)
set (INCLUDE_FILES ${INCLUDE_FILES} ${PROJECT_BINARY_DIR}/writePRC.h)
