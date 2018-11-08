##
#
#  This file is part of OpenFLUID software
#  Copyright(c) 2007, INRA - Montpellier SupAgro
#
#
# == GNU General Public License Usage ==
#
#  OpenFLUID is free software: you can redistribute it and/or modify
#  it under the terms of the GNU General Public License as published by
#  the Free Software Foundation, either version 3 of the License, or
#  (at your option) any later version.
#
#  OpenFLUID is distributed in the hope that it will be useful,
#  but WITHOUT ANY WARRANTY; without even the implied warranty of
#  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
#  GNU General Public License for more details.
#
#  You should have received a copy of the GNU General Public License
#  along with OpenFLUID. If not, see <http://www.gnu.org/licenses/>.
#
#
# == Other Usage ==
#
#  Other Usage means a use of OpenFLUID that is inconsistent with the GPL
#  license, and requires a written agreement between You and INRA.
#  Licensees for Other Usage of OpenFLUID may use this file in accordance
#  with the terms contained in the written agreement between You and INRA.
#
##
#
#
# Module for CMake build system helping to find GEOS library
# This module is designed for internal use by the OpenFLUID build configuration
#
# This module provides the following variables
# GEOS_FOUND
# GEOS_VERSION
# GEOS_INCLUDE_DIR
# GEOS_LIBRARY
#
# This module uses hints about search locations from variables
# GEOS_ROOT_DIR
# GEOS_CONFIG_EXTRA_SEARCH_PATHS
# $ENV{OFBUILD_SUPPORT_DIR}
# $ENV{GEOS_HOME}
#
##


IF(GEOS_FOUND)
  RETURN()
ENDIF()


IF(WIN32)

  INCLUDE($ENV{OFBUILD_SUPPORT_DIR}/share/geos-config.cmake)

  FIND_PATH(GEOS_INCLUDE_DIR
    NAMES
      geos.h
    HINTS
      $ENV{GEOS_ROOT_DIR}
      $ENV{OFBUILD_SUPPORT_DIR}
      c:/msys/local
    PATH_SUFFIXES
      include
  )

  FIND_LIBRARY(GEOS_LIBRARY
    NAMES
      geos
    PATHS
      $ENV{GEOS_ROOT_DIR}
      $ENV{OFBUILD_SUPPORT_DIR}
      c:/msys/local
    PATH_SUFFIXES
      bin lib
  )

ELSEIF(UNIX)

  # add research path for OSX
  IF (APPLE)
    SET(GEOS_CONFIG_EXTRA_SEARCH_PATHS ${GEOS_CONFIG_EXTRA_SEARCH_PATHS} /Library/Frameworks/GEOS.framework/unix)
  ENDIF()

  IF (NOT GEOS_CONFIG)
    # geos-config is not specified, try to find it
    FIND_PROGRAM(GEOS_CONFIG
      NAMES
        geos-config
      HINTS
        $ENV{GEOS_ROOT_DIR}
        $ENV{GEOS_HOME}
        /usr/local
        /usr
        ${GEOS_CONFIG_EXTRA_SEARCH_PATHS}
      PATH_SUFFIXES
        bin
    )
  ENDIF()

  IF (GEOS_CONFIG)
    # get GEOS version using "geos-config --version"
    EXECUTE_PROCESS(COMMAND ${GEOS_CONFIG} "--version"
                    OUTPUT_VARIABLE GEOS_VERSION
                    OUTPUT_STRIP_TRAILING_WHITESPACE)

    STRING(REGEX REPLACE "([0-9]+)\\.([0-9]+)\\.([0-9]+)" "\\1" GEOS_VERSION_MAJOR "${GEOS_VERSION}")
    STRING(REGEX REPLACE "([0-9]+)\\.([0-9]+)\\.([0-9]+)" "\\2" GEOS_VERSION_MINOR "${GEOS_VERSION}")
    STRING(REGEX REPLACE "([0-9]+)\\.([0-9]+)\\.([0-9]+)" "\\3" GEOS_VERSION_PATCH "${GEOS_VERSION}")


    IF(GEOS_VERSION_MAJOR LESS 3 OR (GEOS_VERSION_MAJOR EQUAL 3 AND GEOS_VERSION_MINOR LESS 3))
      MESSAGE(FATAL_ERROR "GEOS version must be 3.3.0 or higher (currently ${GEOS_VERSION})")
    ENDIF()


    # get GEOS install prefix using "geos-config --prefix"
    EXECUTE_PROCESS(COMMAND ${GEOS_CONFIG} "--prefix"
                    OUTPUT_VARIABLE GEOS_PREFIX
                    OUTPUT_STRIP_TRAILING_WHITESPACE)


    # find GEOS include directory
    FIND_PATH(GEOS_INCLUDE_DIR
      NAMES
        geos_c.h
      HINTS
        $ENV{GEOS_ROOT_DIR}
        ${GEOS_PREFIX}
        /usr
        /usr/local
        ${GEOS_CONFIG_EXTRA_SEARCH_PATHS}
      PATH_SUFFIXES
        include
    )


    # find GEOS library directory
    EXECUTE_PROCESS(COMMAND ${GEOS_CONFIG} "--libs"
                    OUTPUT_VARIABLE GEOS_CONFIG_LIBS
                    OUTPUT_STRIP_TRAILING_WHITESPACE)

    # extract library path (-L) and library name (-l) from string returned by "geos-config --libs"
    STRING(REPLACE " " ";" GEOS_CONFIG_LIBS_LIST ${GEOS_CONFIG_LIBS} )

    FOREACH(_LISTITEM ${GEOS_CONFIG_LIBS_LIST})
      STRING(LENGTH ${_LISTITEM} _LISTITEM_LENGTH)
      STRING(SUBSTRING ${_LISTITEM} 0 2 _LISTITEM_PREFIX)
      MATH(EXPR _LISTITEM_EXTRACTLEN ${_LISTITEM_LENGTH}-2)
      IF(${_LISTITEM_PREFIX} STREQUAL "-L")
        STRING(SUBSTRING ${_LISTITEM} 2 ${_LISTITEM_EXTRACTLEN} GEOS_LINK_DIRECTORIES)
      ELSEIF(${_LISTITEM_PREFIX} STREQUAL "-l")
        STRING(SUBSTRING ${_LISTITEM} 2 ${_LISTITEM_EXTRACTLEN} GEOS_LIB_NAME)
      ENDIF()
    ENDFOREACH()

    IF(APPLE)
      SET(GEOS_LIBRARY "${GEOS_LINK_DIRECTORIES}/lib${GEOS_LIB_NAME}.dylib")
    ELSE()
      SET(GEOS_LIBRARY "${GEOS_LINK_DIRECTORIES}/lib${GEOS_LIB_NAME}.so")
    ENDIF()
  ELSE()
    MESSAGE(FATAL_ERROR "FindGEOS.cmake: geos-config was not found. Set it manually using GEOS_CONFIG variable.")
  ENDIF()

ENDIF()


###########################################################################


IF(GEOS_INCLUDE_DIR AND GEOS_LIBRARY)
  SET(GEOS_FOUND TRUE CACHE STRING INTERNAL)
  SET(GEOS_VERSION ${GEOS_VERSION} CACHE STRING INTERNAL)
  SET(GEOS_INCLUDE_DIR ${GEOS_INCLUDE_DIR} CACHE STRING INTERNAL)
  SET(GEOS_LIBRARY ${GEOS_LIBRARY} CACHE STRING INTERNAL)
ENDIF()

IF(GEOS_FOUND)
  IF (NOT GEOS_FIND_QUIETLY)
    MESSAGE(STATUS "Found GEOS ${GEOS_VERSION}: ${GEOS_LIBRARY}")
  ENDIF (NOT GEOS_FIND_QUIETLY)
ELSE()
  MESSAGE(GEOS_INCLUDE_DIR=${GEOS_INCLUDE_DIR})
  MESSAGE(GEOS_LIBRARY=${GEOS_LIBRARY})
  MESSAGE(FATAL_ERROR "Could not find GEOS")
ENDIF()
