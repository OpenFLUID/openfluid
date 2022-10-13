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
# Module for CMake build system helping to find the R environment
# and specific packages for embedding R in C++ code
#
# This module provides the following variables
# R_FOUND
# R_VERSION
# R_HOME
# R_CMD_PROGRAM
# R_SCRIPT_PROGRAM
# R_LDFLAGS
# R_CPPFLAGS
# R_INCLUDE_DIR
# R_INCLUDE_DIRS
# R_LIBRARY_DIR
# R_LIBRARY_DIRS
# R_LIBRARIES
# R_RCPP_FOUND (only when Rcpp component is searched)
# R_RCPP_INCLUDE_DIR (only when Rcpp component is searched)
# R_RCPP_INCLUDE_DIRS (only when Rcpp component is searched)
# R_RCPP_LIBRARY_DIR (only when Rcpp component is searched)
# R_RCPP_LIBRARY_DIRS (only when Rcpp component is searched)
# R_RCPP_LIBRARIES (only when Rcpp component is searched)
# R_RINSIDE_FOUND (only when RInside component is searched)
# R_RINSIDE_INCLUDE_DIR (only when RInside component is searched)
# R_RINSIDE_INCLUDE_DIRS (only when RInside component is searched)
# R_RINSIDE_LIBRARY_DIR (only when RInside component is searched)
# R_RINSIDE_LIBRARY_DIRS (only when RInside component is searched)
# R_RINSIDE_LIBRARIES (only when RInside component is searched)
#
# This module uses hints about search locations from variables
# R_PROGRAM_DIR
#
##


CMAKE_MINIMUM_REQUIRED(VERSION 3.10)

IF(R_FOUND)
  RETURN()
ENDIF()


MACRO(_R_RETURN MSG)
  IF (R_FIND_REQUIRED)
    MESSAGE(FATAL_ERROR "${MSG}")
  ELSE()
    MESSAGE(SEND_ERROR "${MSG}")
  ENDIF()
  SET(R_FOUND 0)
  RETURN()
ENDMACRO()


#######################################################


FUNCTION(_PROCESS_CXX_FLAGS _R_FLAGS _R_VAR)
  SEPARATE_ARGUMENTS(_FLAGSARGS NATIVE_COMMAND ${_R_FLAGS})

  FOREACH(_ARG IN LISTS _FLAGSARGS)
    IF("${_ARG}" MATCHES "^-I(.*)$")
     STRING(SUBSTRING "${_ARG}" 2 -1 _PATH)
      LIST(APPEND _INCLIST "${_PATH}")
    ENDIF()
  ENDFOREACH()

  IF(_INCLIST)
    SET(_${_R_VAR}_INCLUDE_DIRS ${_INCLIST} PARENT_SCOPE)
  ENDIF()
ENDFUNCTION()


#######################################################


FUNCTION(_PROCESS_LD_FLAGS _R_FLAGS _R_VAR)
  SEPARATE_ARGUMENTS(_FLAGSARGS NATIVE_COMMAND ${_R_FLAGS})

  FOREACH(_ARG IN LISTS _FLAGSARGS)
    IF("${_ARG}" MATCHES "^-L(.*)$")
      STRING(SUBSTRING "${_ARG}" 2 -1 _PATH)
      LIST(APPEND _PATHLIST "${_PATH}")
    ELSEIF("${_ARG}" MATCHES "^-l(.*)$")
      STRING(SUBSTRING "${_ARG}" 2 -1 _LIB)
      LIST(APPEND _LIBLIST "${_LIB}")
    ELSE()
      LIST(APPEND _LIBLIST "${_ARG}")
    ENDIF()
  ENDFOREACH()

  IF(_PATHLIST)
    SET(_${_R_VAR}_LIBRARY_DIRS ${_PATHLIST} PARENT_SCOPE)
  ENDIF()
  IF(_LIBLIST)
    SET(_${_R_VAR}_LIBRARIES ${_LIBLIST} PARENT_SCOPE)
  ENDIF()
ENDFUNCTION()


#######################################################


FUNCTION(_DETECT_RBIND_FLAGS _RBIND_NAME _RBIND_VAR)

  MESSAGE(STATUS "  ${_RBIND_NAME}")

  EXECUTE_PROCESS(COMMAND ${_R_SCRIPT_PROGRAM} "-e" "${_RBIND_NAME}:::CxxFlags()"
                  OUTPUT_VARIABLE _CXXOut
                  RESULT_VARIABLE _CXXRes
                  OUTPUT_STRIP_TRAILING_WHITESPACE)

  IF(${_CXXRES})
    _R_RETURN("Unable to get ${_RBIND_NAME} Cxx flags")
  ENDIF()

  EXECUTE_PROCESS(COMMAND ${_R_SCRIPT_PROGRAM} "-e" "${_RBIND_NAME}:::LdFlags()"
                  OUTPUT_VARIABLE _LDOut
                  RESULT_VARIABLE _LDRes
                  OUTPUT_STRIP_TRAILING_WHITESPACE)

  IF(${_LDRES})
    _R_RETURN("Unable to get ${_RBIND_NAME} Ld flags")
  ENDIF()

  _PROCESS_CXX_FLAGS(${_CXXOut} "CXX")
  SET(_${_RBIND_VAR}_INCLUDE_DIR ${_CXX_INCLUDE_DIRS} PARENT_SCOPE)

  IF(_LDOut)
    _PROCESS_LD_FLAGS(${_LDOut} "LD")
  ENDIF()

  SET(_${_RBIND_VAR}_FOUND 1 PARENT_SCOPE)
  SET(_${_RBIND_VAR}_LIBRARY_DIR ${_LD_LIBRARY_DIRS} PARENT_SCOPE)
  SET(_${_RBIND_VAR}_LIBRARIES ${_LD_LIBRARIES} PARENT_SCOPE)

ENDFUNCTION()


#######################################################


SET(_R_FOUND 1)
SET(_R_RCPP_FOUND 0)
SET(_R_RINSIDE_FOUND 0)


# Find R main program

FIND_PROGRAM(_R_CMD_PROGRAM NAMES R R.exe
                            HINTS /usr/local /usr ${R_PROGRAM_DIR})

IF(NOT _R_CMD_PROGRAM)
  _R_RETURN("Unable to find the R program")
ENDIF()


# Find Rscript program

FIND_PROGRAM(_R_SCRIPT_PROGRAM NAMES Rscript Rscript.exe
                               HINTS /usr/local /usr ${R_PROGRAM_DIR})

IF(NOT _R_SCRIPT_PROGRAM)
  _R_RETURN("Unable to find the Rscript program")
ENDIF()


# Get R version

EXECUTE_PROCESS(COMMAND ${_R_CMD_PROGRAM} --slave -e "cat(paste0(R.version[c('major', 'minor')], collapse='.'))"
                OUTPUT_VARIABLE _R_VERSION
                RESULT_VARIABLE _Res
                OUTPUT_STRIP_TRAILING_WHITESPACE)

IF(NOT _R_VERSION)
  _R_RETURN("Unable to determine the R version")
ENDIF()

MESSAGE(STATUS "Found R version ${_R_VERSION}")


# Get R HOME variable

EXECUTE_PROCESS(COMMAND ${_R_CMD_PROGRAM} "RHOME"
                OUTPUT_VARIABLE _R_HOME
                RESULT_VARIABLE _Res
                OUTPUT_STRIP_TRAILING_WHITESPACE)

IF(NOT _R_HOME)
  _R_RETURN("Unable to determine the R home")
ENDIF()


# Get R ldflags

EXECUTE_PROCESS(COMMAND ${_R_CMD_PROGRAM} "CMD" "config" "--ldflags"
                OUTPUT_VARIABLE _R_LDFLAGS
                RESULT_VARIABLE _Res
                OUTPUT_STRIP_TRAILING_WHITESPACE)

IF(NOT _R_LDFLAGS)
  _R_RETURN("Unable to determine the R ld flags")
ENDIF()

_PROCESS_LD_FLAGS(${_R_LDFLAGS} "R")


# Get R cppflags

EXECUTE_PROCESS(COMMAND ${_R_CMD_PROGRAM} "CMD" "config" "--cppflags"
                OUTPUT_VARIABLE _R_CPPFLAGS
                RESULT_VARIABLE _Res
                OUTPUT_STRIP_TRAILING_WHITESPACE)

IF(NOT _R_CPPFLAGS)
  _R_RETURN("Unable to determine the R cpp flags")
ENDIF()

_PROCESS_CXX_FLAGS(${_R_CPPFLAGS} "R")


# Get flags for Rcpp and RInside components (packages)

FOREACH(_COMPONENT ${R_FIND_COMPONENTS})
  IF("${_COMPONENT}" STREQUAL "Rcpp")
    IF(NOT _R_RCPP_FOUND)
      _DETECT_RBIND_FLAGS("Rcpp" R_RCPP)
    ENDIF()
  ELSEIF(${_COMPONENT} STREQUAL "RInside")
    IF(NOT _R_RCPP_FOUND)
      _DETECT_RBIND_FLAGS("Rcpp" R_RCPP)
    ENDIF()
    IF(NOT _R_RINSIDE_FOUND)
      _DETECT_RBIND_FLAGS("RInside" R_RINSIDE)
    ENDIF()
  ELSE()
    _R_RETURN("Component ${_COMPONENT} is not recognized")
  ENDIF()
ENDFOREACH()


# Set up module variables

IF(_R_FOUND)

  SET(R_VERSION ${_R_VERSION} CACHE STRING "R version")

  SET(R_HOME ${_R_HOME} CACHE STRING "R home")

  SET(R_CMD_PROGRAM ${_R_CMD_PROGRAM} CACHE FILEPATH "R program")
  SET(R_SCRIPT_PROGRAM ${_R_SCRIPT_PROGRAM} CACHE FILEPATH "Rscript program")


  SET(R_LDFLAGS ${_R_LDFLAGS} CACHE FILEPATH "R ld flags")
  SET(R_CPPFLAGS ${_R_CPPFLAGS} CACHE FILEPATH "R cpp flags")

  SET(R_INCLUDE_DIR ${_R_INCLUDE_DIRS} CACHE PATH "R include directory")
  SET(R_INCLUDE_DIRS ${_R_INCLUDE_DIRS} CACHE PATH "R include directories")

  SET(R_LIBRARY_DIR ${_R_LIBRARY_DIRS} CACHE PATH "R library directory")
  SET(R_LIBRARY_DIRS ${_R_LIBRARY_DIRS} CACHE PATH "R library directories")

  SET(R_LIBRARIES ${_R_LIBRARIES} CACHE PATH "R libraries")


  SET(R_FOUND 1 CACHE STRING "R found!")


  IF(_R_RCPP_FOUND)
    SET(R_RCPP_FOUND 1 CACHE STRING "Rcpp found!")

    SET(R_RCPP_INCLUDE_DIR ${_R_RCPP_INCLUDE_DIR} CACHE PATH "Rcpp include directory")
    SET(R_RCPP_INCLUDE_DIRS ${_R_RCPP_INCLUDE_DIR} CACHE PATH "Rcpp include directories")

    SET(R_RCPP_LIBRARY_DIR ${_R_RCPP_LIBRARY_DIR} CACHE PATH "Rcpp library directory")
    SET(R_RCPP_LIBRARY_DIRS ${_R_RCPP_LIBRARY_DIR} CACHE PATH "Rcpp library directories")
    SET(R_RCPP_LIBRARIES ${_R_RCPP_LIBRARIES} CACHE PATH "Rcpp libraries")
  ENDIF()

  IF(_R_RINSIDE_FOUND)
    SET(R_RINSIDE_FOUND 1 CACHE STRING "RInside found!")

    SET(R_RINSIDE_INCLUDE_DIR ${_R_RINSIDE_INCLUDE_DIR} CACHE PATH "RInside include directory")
    SET(R_RINSIDE_INCLUDE_DIRS ${_R_RINSIDE_INCLUDE_DIR} CACHE PATH "RInside include directories")

    SET(R_RINSIDE_LIBRARY_DIR ${_R_RINSIDE_LIBRARY_DIR} CACHE PATH "Rinside library directory")
    SET(R_RINSIDE_LIBRARY_DIRS ${_R_RINSIDE_LIBRARY_DIR} CACHE PATH "Rinside library directories")
    SET(R_RINSIDE_LIBRARIES ${_R_RINSIDE_LIBRARIES} CACHE PATH "Rinside libraries")
  ENDIF()
ENDIF()
