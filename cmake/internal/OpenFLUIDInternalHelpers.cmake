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


###########################################################################


MACRO(OFBUILD_ADD_WARE_TRANSLATIONS_WITH_INSTALL WARE_NAME WARE_SRCDIR WARE_BINDIR WARE_SUFFIX WARE_INSTALL_PATH)

  IF(LUPDATE_EXECUTABLE)

    ADD_CUSTOM_TARGET(update-translations-strings-${WARE_NAME})

    FOREACH(_LANG ${OPENFLUID_TRANSLATIONS_LANGS})
      SET(_LANG_TSFILE ${CMAKE_CURRENT_SOURCE_DIR}/${WARE_SRCDIR}/${WARE_NAME}-${_LANG}.ts)

      ADD_CUSTOM_TARGET(update-translation-strings-${WARE_NAME}-${_LANG}
                        COMMENT "Updating ${_LANG} translation strings for ${WARE_NAME}"
                        COMMAND ${LUPDATE_EXECUTABLE}
                                "${CMAKE_CURRENT_SOURCE_DIR}/${WARE_SRCDIR}"
                                "-I" "${CMAKE_CURRENT_SOURCE_DIR}/${WARE_SRCDIR}"
                                "-ts" ${_LANG_TSFILE})
      ADD_DEPENDENCIES(update-translations-strings-${WARE_NAME} update-translation-strings-${WARE_NAME}-${_LANG})
    ENDFOREACH()

    ADD_DEPENDENCIES(update-translations-strings update-translations-strings-${WARE_NAME})
  ENDIF()


  IF(LRELEASE_EXECUTABLE)
    ADD_CUSTOM_TARGET(release-translations-${WARE_NAME})

    FOREACH(_LANG ${OPENFLUID_TRANSLATIONS_LANGS})
      SET(_LANG_TSFILE ${CMAKE_CURRENT_SOURCE_DIR}/${WARE_SRCDIR}/${WARE_NAME}-${_LANG}.ts)
      SET(_LANG_QMFILE ${WARE_BINDIR}/${WARE_NAME}${WARE_SUFFIX}-${_LANG}.qm)

      ADD_CUSTOM_COMMAND(DEPENDS ${_LANG_TSFILE}
                         COMMAND ${LRELEASE_EXECUTABLE}
                                 ${_LANG_TSFILE}
                                 "-qm" ${_LANG_QMFILE}
                          OUTPUT ${_LANG_QMFILE})
      ADD_CUSTOM_TARGET(release-translation-${WARE_NAME}-${_LANG}
                        COMMENT "Releasing ${_LANG} translation for ${WARE_NAME}"
                        DEPENDS ${_LANG_QMFILE})
      ADD_DEPENDENCIES(release-translations-${WARE_NAME} release-translation-${WARE_NAME}-${_LANG})

      INSTALL(FILES "${_LANG_QMFILE}" DESTINATION "${WARE_INSTALL_PATH}")
    ENDFOREACH()

    ADD_DEPENDENCIES(release-translations release-translations-${WARE_NAME})
  ENDIF()


ENDMACRO()


###########################################################################


# Macro for compiling a simulator
FUNCTION(OFBUILD_ADD_SIMULATOR SIM_SRCDIR SIM_BINDIR)

  SET(SIM_CPP)
  SET(SIM_FORTRAN)
  SET(SIM_INSTALL_ENABLED)
  SET(SIM_EXTRA_LINKS)
  SET(_FORTRAN_LINK_LIBS)

  # TOIMPL to refactor
  INCLUDE("${CMAKE_CURRENT_SOURCE_DIR}/${SIM_SRCDIR}/build-config.cmake" OPTIONAL)
  INCLUDE("${CMAKE_CURRENT_SOURCE_DIR}/${SIM_SRCDIR}/CMake.in.cmake" OPTIONAL) 

  IF(NOT SIM_CPP)
    FILE(GLOB SIM_CPP ${SIM_SRCDIR}/*.cpp ${SIM_SRCDIR}/*.c)
  ENDIF()

  IF(NOT SIM_ID)
    GET_FILENAME_COMPONENT(SIM_ID ${SIM_SRCDIR} NAME)
  ENDIF()

  IF(SIM_FORTRAN)
    ENABLE_LANGUAGE(Fortran)
    SET(CMAKE_Fortran_FLAGS_RELEASE "-funroll-all-loops -fno-f2c -O3")
    SET(_FORTRAN_LINK_LIBS "gfortran")
  ENDIF()

  INCLUDE_DIRECTORIES(${SIM_SRCDIR} ${SIM_INCLUDE_DIRS})
  LINK_DIRECTORIES(${SIM_LIBRARY_DIRS})

  ADD_LIBRARY("${SIM_ID}${OPENFLUID_SIMULATORS_SUFFIX}" MODULE ${SIM_CPP} ${SIM_FORTRAN})

  SET_TARGET_PROPERTIES("${SIM_ID}${OPENFLUID_SIMULATORS_SUFFIX}" PROPERTIES
                        PREFIX ""
                        SUFFIX "${OPENFLUID_PLUGINS_BINARY_EXTENSION}"
                        LIBRARY_OUTPUT_DIRECTORY "${SIM_BINDIR}")

  # Fix for win32 compatibility
  IF(WIN32)
    SET_TARGET_PROPERTIES("${SIM_ID}${OPENFLUID_SIMULATORS_SUFFIX}" PROPERTIES LINK_FLAGS "-shared")
  ENDIF()

  TARGET_LINK_LIBRARIES("${SIM_ID}${OPENFLUID_SIMULATORS_SUFFIX}"
                        openfluid-core
                        openfluid-base
                        openfluid-ware
                        openfluid-tools
                        Qt5::Core
                        ${SIM_LINK_LIBS}
                        ${SIM_EXTRA_LINKS}
                        ${_FORTRAN_LINK_LIBS})

  TARGET_COMPILE_DEFINITIONS("${SIM_ID}${OPENFLUID_SIMULATORS_SUFFIX}" PRIVATE 
                             WAREBUILD_BUILD_TYPE="${OFBUILD_BUILD_TYPE}"
                             WAREBUILD_COMPILER_ID="${CMAKE_CXX_COMPILER_ID}"
                             WAREBUILD_COMPILER_VERSION="${CMAKE_CXX_COMPILER_VERSION}"
                             WAREBUILD_COMPILATION_FLAGS="${OFBUILD_ALL_COMPILE_FLAGS_STR}")

  IF(WARE_LINKUID)
    TARGET_COMPILE_DEFINITIONS("${SIM_ID}${OPENFLUID_SIMULATORS_SUFFIX}" PRIVATE 
                               "WARE_LINKUID=\"${WARE_LINKUID}\"")
  ENDIF()

  IF(${SIM_INSTALL_ENABLED})
    INSTALL(TARGETS "${SIM_ID}${OPENFLUID_SIMULATORS_SUFFIX}"
            DESTINATION "${OPENFLUID_SIMULATORS_INSTALL_PATH}")

    IF(SIM_TRANSLATIONS_ENABLED)
      OFBUILD_ADD_WARE_TRANSLATIONS_WITH_INSTALL(${SIM_ID} ${SIM_SRCDIR} ${SIM_BINDIR}
                                                ${OPENFLUID_SIMULATORS_SUFFIX} ${OPENFLUID_SIMULATORS_INSTALL_PATH})
    ENDIF()
  ENDIF()


  IF(OBS_PARAMSUI_ENABLED AND EXT_CPP)
    OFBUILD_ADD_BUILDER_EXTENSION(${SIM_ID} ${SIM_SRCDIR} ${SIM_BINDIR})
  ENDIF()

  SET(SIM_CPP)
  SET(SIM_FORTRAN)
  SET(SIM_INSTALL_ENABLED)
  SET(SIM_EXTRA_LINKS)
  SET(_FORTRAN_LINK_LIBS)

ENDFUNCTION()


###########################################################################


FUNCTION(OFBUILD_ADD_EXAMPLE_DOC_BUILD_TARGET WAREID ROOTCPPFILENAME OUTPUTDIR)

  # TOIMPL

ENDFUNCTION()


###########################################################################


FUNCTION(OFBUILD_ADD_EXAMPLE_SIMULATOR SIMID ROOTCPPFILENAME DOCBUILD)
  
  # TOIMPL use standard ware sources tree
  # TOIMPL to refactor once exaples uses standard wares sources tree

  # SET(WAREID ${SIMID})
  # SET(ROOTCPPFILENAME ${ROOTCPPFILENAME})
  # SET(TEMPLATE_DIR ${CMAKE_SOURCE_DIR}/${OPENFLUID_SHARE_WARESDEV_INSTALL_PATH}/templates/simulator/skeleton)
  # SET(DIST_DEV_SIM ${OFBUILD_DIST_DIR}/${OPENFLUID_EXAMPLES_WARESDEV_INSTALL_PATH}/simulators)
  # SET(CURR_WARE_DIR ${DIST_DEV_SIM}/${WAREID})
  
  # OFBUILD_ADD_SIMULATOR(${WAREID} ${OFBUILD_DIST_SIMULATORS_DIR})
    
  # # build-config.cmake
  # FILE(READ "${TEMPLATE_DIR}/src/build-config.cmake" BUILDCONFIG_TPL)
  # STRING(REPLACE "%%" "@" BUILDCONFIG_ARO "${BUILDCONFIG_TPL}")
  # FILE(WRITE "${CURR_WARE_DIR}/build-config.tpl" "${BUILDCONFIG_ARO}")
  # CONFIGURE_FILE("${CURR_WARE_DIR}/build-config.tpl" "${CURR_WARE_DIR}/build-config.cmake")
  # FILE(REMOVE "${CURR_WARE_DIR}/build-config.tpl")

  # # CMakeLists.txt
  # FILE(READ "${TEMPLATE_DIR}/src/CMakeLists.txt" CMAKELISTS_TPL)
  # STRING(REPLACE "%%" "@" CMAKELISTS_ARO "${CMAKELISTS_TPL}")
  # FILE(WRITE "${CURR_WARE_DIR}/CMakeLists.tpl" "${CMAKELISTS_ARO}")
  # CONFIGURE_FILE("${CURR_WARE_DIR}/CMakeLists.tpl" "${CURR_WARE_DIR}/CMakeLists.txt")
  # FILE(REMOVE "${CURR_WARE_DIR}/CMakeLists.tpl")

  
  # IF (DOCBUILD)
  #   # Add target for doc update
  #   OFBUILD_ADD_EXAMPLE_DOC_BUILD_TARGET(${WAREID} ${CURR_WARE_DIR}/${ROOTCPPFILENAME} ${OFBUILD_EXAMPLES_SIMULATORS_DOC_DIR})
  # ENDIF()
  
ENDFUNCTION()


###########################################################################


# Macro for compiling a simulation observer
FUNCTION(OFBUILD_ADD_OBSERVER OBS_SRCDIR OBS_BINDIR)

  SET(OBS_CPP)
  SET(OBS_INSTALL_ENABLED)
  SET(OBS_EXTRA_LINKS)

  # TODO to refactor
  INCLUDE("${CMAKE_CURRENT_SOURCE_DIR}/${OBS_SRCDIR}/build-config.cmake" OPTIONAL)
  INCLUDE("${CMAKE_CURRENT_SOURCE_DIR}/${OBS_SRCDIR}/CMake.in.cmake" OPTIONAL)

  IF(NOT OBS_CPP)
    FILE(GLOB OBS_CPP ${OBS_SRCDIR}/*.cpp ${OBS_SRCDIR}/*.c)
  ENDIF()

  IF(NOT OBS_ID)
    GET_FILENAME_COMPONENT(OBS_ID ${OBS_SRCDIR} NAME)
  ENDIF()

  INCLUDE_DIRECTORIES(${OBS_SRCDIR})

  ADD_LIBRARY("${OBS_ID}${OPENFLUID_OBSERVERS_SUFFIX}" MODULE ${OBS_CPP})

  SET_TARGET_PROPERTIES("${OBS_ID}${OPENFLUID_OBSERVERS_SUFFIX}" PROPERTIES
                        PREFIX ""
                        SUFFIX "${OPENFLUID_PLUGINS_BINARY_EXTENSION}"
                        LIBRARY_OUTPUT_DIRECTORY "${OBS_BINDIR}")

  # Fix for win32 compatibility
  IF(WIN32)
    SET_TARGET_PROPERTIES("${OBS_ID}${OPENFLUID_OBSERVERS_SUFFIX}" PROPERTIES LINK_FLAGS "-shared")
  ENDIF()

  TARGET_LINK_LIBRARIES("${OBS_ID}${OPENFLUID_OBSERVERS_SUFFIX}"
                        openfluid-core
                        openfluid-base
                        openfluid-ware
                        openfluid-tools
                        Qt5::Core
                        ${OBS_EXTRA_LINKS})

  TARGET_COMPILE_DEFINITIONS("${OBS_ID}${OPENFLUID_OBSERVERS_SUFFIX}" PRIVATE 
                             WAREBUILD_BUILD_TYPE="${OFBUILD_BUILD_TYPE}"
                             WAREBUILD_COMPILER_ID="${CMAKE_CXX_COMPILER_ID}"
                             WAREBUILD_COMPILER_VERSION="${CMAKE_CXX_COMPILER_VERSION}"
                             WAREBUILD_COMPILATION_FLAGS="${OFBUILD_ALL_COMPILE_FLAGS_STR}")

  IF(WARE_LINKUID)
    TARGET_COMPILE_DEFINITIONS("${OBS_ID}${OPENFLUID_OBSERVERS_SUFFIX}" PRIVATE
                               "WARE_LINKUID=\"${WARE_LINKUID}\"")
  ENDIF()

  IF(${OBS_INSTALL_ENABLED})
    INSTALL(TARGETS "${OBS_ID}${OPENFLUID_OBSERVERS_SUFFIX}"
            DESTINATION "${OPENFLUID_OBSERVERS_INSTALL_PATH}")

    IF(OBS_TRANSLATIONS_ENABLED)
      OFBUILD_ADD_WARE_TRANSLATIONS_WITH_INSTALL(${OBS_ID} ${OBS_SRCDIR} ${OBS_BINDIR}
                                                ${OPENFLUID_OBSERVERS_SUFFIX} ${OPENFLUID_OBSERVERS_INSTALL_PATH})
    ENDIF()
  ENDIF()


  IF(OBS_PARAMSUI_ENABLED AND EXT_CPP)
    OFBUILD_ADD_BUILDER_EXTENSION(${OBS_ID} ${OBS_SRCDIR} ${OBS_BINDIR})
  ENDIF()

  SET(OBS_CPP)
  SET(OBS_INSTALL_ENABLED)
  SET(OBS_EXTRA_LINKS)

ENDFUNCTION()


###########################################################################


# Macro for compiling a builder extension
FUNCTION(OFBUILD_ADD_BUILDER_EXTENSION EXT_SRCDIR EXT_BINDIR)

  SET(EXT_CPP)
  SET(EXT_UIFILES)
  SET(EXT_RCFILES)
  SET(EXT_INSTALL_ENABLED)
  SET(EXT_EXTRA_LINKS)

  # TODO to refactor
  INCLUDE("${CMAKE_CURRENT_SOURCE_DIR}/${EXT_SRCDIR}/build-config.cmake" OPTIONAL)
  INCLUDE("${CMAKE_CURRENT_SOURCE_DIR}/${EXT_SRCDIR}/CMake.in.cmake" OPTIONAL)

  IF(NOT EXT_CPP)
    FILE(GLOB EXT_CPP ${EXT_SRCDIR}/*.cpp ${EXT_SRCDIR}/*.c)
  ENDIF()

  IF(NOT EXT_UIFILES)
    FILE(GLOB EXT_UIFILES ${EXT_SRCDIR}/*.ui)
  ENDIF()

  IF(NOT EXT_RCFILES)
    FILE(GLOB EXT_RCFILES ${EXT_SRCDIR}/*.qrc)
  ENDIF()


  QT5_WRAP_UI(EXT_UI ${EXT_UIFILES})
  QT5_ADD_RESOURCES(EXT_RC ${EXT_RCFILES})

  INCLUDE_DIRECTORIES(${EXT_SRCDIR})

  IF(NOT EXT_ID)
    GET_FILENAME_COMPONENT(EXT_ID ${EXT_SRCDIR} NAME)
  ENDIF()

  IF(EXT_ID_SUFFIX)
    SET(EXT_ID ${EXT_ID}${EXT_ID_SUFFIX})
  ENDIF()

  ADD_LIBRARY(${EXT_ID}${OPENFLUID_BUILDEREXTS_SUFFIX} MODULE ${EXT_CPP} ${EXT_UI} ${EXT_RC})

  SET_TARGET_PROPERTIES(${EXT_ID}${OPENFLUID_BUILDEREXTS_SUFFIX} PROPERTIES
                        PREFIX ""
                        SUFFIX "${OPENFLUID_PLUGINS_BINARY_EXTENSION}"
                        LIBRARY_OUTPUT_DIRECTORY "${EXT_BINDIR}"
                        AUTOMOC ON)

  # Fix for win32 compatibility
  IF(WIN32)
    SET_TARGET_PROPERTIES(${EXT_ID}${OPENFLUID_BUILDEREXTS_SUFFIX} PROPERTIES LINK_FLAGS "-shared")
  ENDIF()


  TARGET_LINK_LIBRARIES(${EXT_ID}${OPENFLUID_BUILDEREXTS_SUFFIX}
                        openfluid-core
                        openfluid-base
                        openfluid-tools
                        openfluid-ware
                        openfluid-ui-builderext
                        Qt5::Core
                        Qt5::Widgets
                        ${EXT_EXTRA_LINKS}
                       )

  TARGET_COMPILE_DEFINITIONS(${EXT_ID}${OPENFLUID_BUILDEREXTS_SUFFIX} PRIVATE 
                             WAREBUILD_BUILD_TYPE="${OFBUILD_BUILD_TYPE}"
                             WAREBUILD_COMPILER_ID="${CMAKE_CXX_COMPILER_ID}"
                             WAREBUILD_COMPILER_VERSION="${CMAKE_CXX_COMPILER_VERSION}"
                             WAREBUILD_COMPILATION_FLAGS="${OFBUILD_ALL_COMPILE_FLAGS_STR}")

  SET(_TEMP_COMPDEFS "WARE_ID=\"${EXT_ID}\"")
  IF(WARE_LINKUID)
    SET(_TEMP_COMPDEFS "${_TEMP_COMPDEFS}" "WARE_LINKUID=\"${WARE_LINKUID}\"")
  ENDIF()

  TARGET_COMPILE_DEFINITIONS(${EXT_ID}${OPENFLUID_BUILDEREXTS_SUFFIX} PRIVATE
                             "${_TEMP_COMPDEFS}")

  IF(${EXT_INSTALL_ENABLED})
    INSTALL(TARGETS "${EXT_ID}${OPENFLUID_BUILDEREXTS_SUFFIX}"
            DESTINATION "${OPENFLUID_BUILDEREXTS_INSTALL_PATH}")

     IF(EXT_TRANSLATIONS_ENABLED)
       OFBUILD_ADD_WARE_TRANSLATIONS_WITH_INSTALL(${EXT_ID} ${EXT_SRCDIR} ${EXT_BINDIR}
                                                 ${OPENFLUID_BUILDEREXTS_SUFFIX} ${OPENFLUID_BUILDEREXTS_INSTALL_PATH})
     ENDIF()
  ENDIF()


  SET(EXT_CPP)
  SET(EXT_UIFILES)
  SET(EXT_RCFILES)
  SET(EXT_INSTALL_ENABLED)
  SET(EXT_EXTRA_LINKS)
ENDFUNCTION()


###########################################################################


# Macro for compiling a builder extension and add installation directives
#MACRO(OFBUILD_ADD_BUILDER_EXTENSION_WITH_INSTALL EXT_NAME EXT_SRCDIR EXT_BINDIR)

#  OFBUILD_ADD_BUILDER_EXTENSION(${EXT_NAME} ${EXT_SRCDIR} ${EXT_BINDIR})

#  INSTALL(TARGETS "${EXT_NAME}${OPENFLUID_BUILDEREXTS_SUFFIX}"
#          DESTINATION "${OPENFLUID_BUILDEREXTS_INSTALL_PATH}")


#  OFBUILD_ADD_WARE_TRANSLATIONS_WITH_INSTALL(${EXT_NAME} ${EXT_SRCDIR} ${EXT_BINDIR}
#                                            ${OPENFLUID_BUILDEREXTS_SUFFIX} ${OPENFLUID_BUILDEREXTS_INSTALL_PATH})

#ENDMACRO()


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
