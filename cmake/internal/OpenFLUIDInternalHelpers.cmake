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


INCLUDE(CMakeParseArguments)
INCLUDE(ExternalProject)


###########################################################################


FUNCTION(OFBUILD_ADD_WARE)

  SET(_ARGS_OPTIONS "")
  SET(_ARGS_ONEVAL "DIRID;TYPE;TARGET_PREFIX;INSTALL_PATH;PREFIX_INSTALL_PATH")
  SET(_ARGS_MULTIVAL "")
  CMAKE_PARSE_ARGUMENTS(_BUILDARGS "${_ARGS_OPTIONS}" "${_ARGS_ONEVAL}" "${_ARGS_MULTIVAL}" ${ARGN})

  # libs dependencies 
  SET(_TARGET_DEPENDS ${OPENFLUID_CMD_APP} openfluid-core openfluid-ware)
  IF(OFBUILD_ENABLE_GUI)
    SET(_TARGET_DEPENDS ${_TARGET_DEPENDS} openfluid-ui-common openfluid-ui-waresdev openfluid-ui-builderext)
  ENDIF()

  # expected ware suffix (binary file)
  IF(_BUILDARGS_TYPE STREQUAL "simulator")
    SET(_WAREBIN_SUFFIX ${OPENFLUID_SIMULATORS_SUFFIX}${CMAKE_SHARED_LIBRARY_SUFFIX})
  ELSEIF(_BUILDARGS_TYPE STREQUAL "observer")
    SET(_WAREBIN_SUFFIX ${OPENFLUID_OBSERVERS_SUFFIX}${CMAKE_SHARED_LIBRARY_SUFFIX})
  ELSEIF(_BUILDARGS_TYPE STREQUAL "builderext")
    SET(_WAREBIN_SUFFIX ${OPENFLUID_BUILDEREXTS_SUFFIX}${CMAKE_SHARED_LIBRARY_SUFFIX})
  ELSE()
    MESSAGE(FATAL_ERROR "wrong ware type (${_BUILDARGS_TYPE})")
  ENDIF()

  # files dependencies and expected output
  FILE(GLOB_RECURSE _FILE_DEPENDS "${CMAKE_CURRENT_SOURCE_DIR}/${_BUILDARGS_DIRID}/*")
  SET(_EXPECTED_OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/${_BUILDARGS_DIRID}/${_BUILDARGS_DIRID}${_WAREBIN_SUFFIX})


  # command and target to run the build script
  ADD_CUSTOM_COMMAND(
    DEPENDS ${_FILE_DEPENDS}
    OUTPUT ${_EXPECTED_OUTPUT}
    COMMENT "Configuring and building ${_BUILDARGS_TYPE} ${_BUILDARGS_DIRID}"
    COMMAND "${CMAKE_COMMAND}" 
       ARGS "-DWARE_SOURCE_DIR=${CMAKE_CURRENT_SOURCE_DIR}/${_BUILDARGS_DIRID}" "-DWARE_BUILD_DIR=${CMAKE_CURRENT_BINARY_DIR}/${_BUILDARGS_DIRID}"
            "-DWARE_INSTALL_PATH=${_BUILDARGS_INSTALL_PATH}" "-DWARES_PREFIX_INSTALL_PATH=${_BUILDARGS_PREFIX_INSTALL_PATH}"
            "-P" "${CMAKE_BINARY_DIR}/cmake/internal/OpenFLUIDWareBuildScript.cmake"
    COMMAND_EXPAND_LISTS)

  ADD_CUSTOM_TARGET(${_BUILDARGS_TARGET_PREFIX}-${_BUILDARGS_DIRID} ALL 
                    DEPENDS "${_EXPECTED_OUTPUT}"
                    COMMENT "Built ${_BUILDARGS_TYPE} ${_BUILDARGS_DIRID}"
                    VERBATIM)
  ADD_DEPENDENCIES(${_BUILDARGS_TARGET_PREFIX}-${_BUILDARGS_DIRID} ${_TARGET_DEPENDS})

ENDFUNCTION()


###########################################################################


FUNCTION(OFBUILD_ADD_SIMULATOR DIRID)

  OFBUILD_ADD_WARE(DIRID "${DIRID}" TYPE "simulator" 
                   TARGET_PREFIX "simulator" PREFIX_INSTALL_PATH "${OFBUILD_DIST_DIR}/${OPENFLUID_WARES_INSTALL_PATH}")

ENDFUNCTION()


###########################################################################

FUNCTION(OFBUILD_ADD_TEST_SIMULATOR DIRID)
  
  OFBUILD_ADD_WARE(DIRID "${DIRID}" TYPE "simulator" 
                   TARGET_PREFIX "test-simulator" INSTALL_PATH "${OFBUILD_TESTS_BINARY_DIR}")

ENDFUNCTION()


###########################################################################

FUNCTION(OFBUILD_ADD_EXAMPLE_SIMULATOR DIRID)
  
  OFBUILD_ADD_WARE(DIRID "${DIRID}" TYPE "simulator" 
                   TARGET_PREFIX "example-simulator" PREFIX_INSTALL_PATH "${OFBUILD_DIST_DIR}/${OPENFLUID_WARES_INSTALL_PATH}")

  # TOIMPL add doc build

ENDFUNCTION()


###########################################################################


FUNCTION(OFBUILD_ADD_OBSERVER DIRID)

  OFBUILD_ADD_WARE(DIRID "${DIRID}" TYPE "observer" 
                   TARGET_PREFIX "observer" PREFIX_INSTALL_PATH "${OFBUILD_DIST_DIR}/${OPENFLUID_WARES_INSTALL_PATH}")

ENDFUNCTION()


###########################################################################

FUNCTION(OFBUILD_ADD_TEST_OBSERVER DIRID)
  
  OFBUILD_ADD_WARE(DIRID "${DIRID}" TYPE "observer" 
                   TARGET_PREFIX "test-observer" INSTALL_PATH "${OFBUILD_TESTS_BINARY_DIR}")

ENDFUNCTION()


###########################################################################


FUNCTION(OFBUILD_ADD_BUILDEREXT DIRID)

  OFBUILD_ADD_WARE(DIRID "${DIRID}" TYPE "builderext" 
                   TARGET_PREFIX "builderext" PREFIX_INSTALL_PATH "${OFBUILD_DIST_DIR}/${OPENFLUID_WARES_INSTALL_PATH}")

ENDFUNCTION()


###########################################################################


FUNCTION(OFBUILD_ADD_TEST_BUILDEREXT DIRID)

  OFBUILD_ADD_WARE(DIRID "${DIRID}" TYPE "builderext" 
                   TARGET_PREFIX "test-builderext" INSTALL_PATH "${OFBUILD_TESTS_BINARY_DIR}")

ENDFUNCTION()


###########################################################################


MACRO(OFBUILD_ADD_UNITTEST TEST_CAT TEST_NAME)
  ADD_EXECUTABLE("${TEST_CAT}-${TEST_NAME}" ${ARGN})
  TARGET_LINK_LIBRARIES("${TEST_CAT}-${TEST_NAME}"
                        Boost::boost Boost::unit_test_framework
                        ${UNITTEST_LINK_LIBRARIES})
  SET_TARGET_PROPERTIES("${TEST_CAT}-${TEST_NAME}" 
                        PROPERTIES RUNTIME_OUTPUT_DIRECTORY "${OFBUILD_TESTS_BINARY_DIR}")
  ADD_TEST("unit-${TEST_CAT}-${TEST_NAME}" "${OFBUILD_TESTS_BINARY_DIR}/${TEST_CAT}-${TEST_NAME}" ${OFBUILD_UNITTESTS_RUN_OPTIONS})

  SET_TESTS_PROPERTIES("unit-${TEST_CAT}-${TEST_NAME}"
                       PROPERTIES ENVIRONMENT "OPENFLUID_TEMP_PATH=${OFBUILD_TESTS_TEMP_DIR};OPENFLUID_USERDATA_PATH=${OFBUILD_TESTS_USERDATA_DIR}"
                                  SKIP_RETURN_CODE ${OFBUILD_UNITTESTS_SKIP_CODE})

  IF(NOT TARGET "unit-${TEST_CAT}-ALLTESTS")
    ADD_CUSTOM_TARGET("unit-${TEST_CAT}-ALLTESTS")
  ENDIF()
  ADD_DEPENDENCIES("unit-${TEST_CAT}-ALLTESTS" "${TEST_CAT}-${TEST_NAME}")

ENDMACRO()


###########################################################################


MACRO(OFBUILD_DISCOVER_UNITTESTS TEST_CAT)
  FILE(GLOB UNIT_TESTS_CPP *_TEST.cpp)
  FOREACH(CPPFILE ${UNIT_TESTS_CPP})
    GET_FILENAME_COMPONENT(CPPFILE_WE ${CPPFILE} NAME_WE)
    OFBUILD_ADD_UNITTEST(${TEST_CAT} ${CPPFILE_WE} ${CPPFILE})
  ENDFOREACH()
ENDMACRO()


###########################################################################


MACRO(OFBUILD_DISCOVER_HEAVYUNITTESTS TEST_CAT)
  IF(OFBUILD_TESTS_ENABLE_HEAVY)
    FILE(GLOB UNIT_TESTS_CPP *_HEAVYTEST.cpp)
    FOREACH(CPPFILE ${UNIT_TESTS_CPP})
      GET_FILENAME_COMPONENT(CPPFILE_WE ${CPPFILE} NAME_WE)
      OFBUILD_ADD_UNITTEST(${TEST_CAT} ${CPPFILE_WE} ${CPPFILE})
    ENDFOREACH()
  ENDIF()
ENDMACRO()


###########################################################################


MACRO(OFBUILD_CONFIGURE_SANITIZER)  
  IF(CMAKE_CXX_COMPILER_ID STREQUAL "GNU" AND NOT CMAKE_CXX_COMPILER_VERSION VERSION_LESS "4.8")
    ADD_COMPILE_OPTIONS("-fsanitize=address")
    SET(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -fsanitize=address")
  ELSE()
    SET(OFBUILD_ENABLE_SANITIZER OFF)    
  ENDIF()
ENDMACRO()


###########################################################################

FUNCTION(OFBUILD_CREATE_01_VAR ARGVAR)
  IF(${ARGVAR})
    SET(${ARGVAR}_01 1 PARENT_SCOPE)
  ELSE()
    SET(${ARGVAR}_01 0 PARENT_SCOPE)
  ENDIF()
ENDFUNCTION()
