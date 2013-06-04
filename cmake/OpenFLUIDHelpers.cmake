##
# This file is part of OpenFLUID software
# Copyright (c) 2007-2010 INRA-Montpellier SupAgro
#
#
# == GNU General Public License Usage ==
#
# OpenFLUID is free software: you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation, either version 3 of the License, or
# (at your option) any later version.
#
# OpenFLUID is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with OpenFLUID. If not, see <http://www.gnu.org/licenses/>.
#
# In addition, as a special exception, INRA gives You the additional right
# to dynamically link the code of OpenFLUID with code not covered
# under the GNU General Public License ("Non-GPL Code") and to distribute
# linked combinations including the two, subject to the limitations in this
# paragraph. Non-GPL Code permitted under this exception must only link to
# the code of OpenFLUID dynamically through the OpenFLUID libraries
# interfaces, and only for building OpenFLUID plugins. The files of
# Non-GPL Code may be link to the OpenFLUID libraries without causing the
# resulting work to be covered by the GNU General Public License. You must
# obey the GNU General Public License in all respects for all of the
# OpenFLUID code and other code used in conjunction with OpenFLUID
# except the Non-GPL Code covered by this exception. If you modify
# this OpenFLUID, you may extend this exception to your version of the file,
# but you are not obligated to do so. If you do not wish to provide this
# exception without modification, you must delete this exception statement
# from your version and license this OpenFLUID solely under the GPL without
# exception.
#
#
# == Other Usage ==
#
# Other Usage means a use of OpenFLUID that is inconsistent with the GPL
# license, and requires a written agreement between You and INRA.
# Licensees for Other Usage of OpenFLUID may use this file in accordance
# with the terms contained in the written agreement between You and INRA.
##


MACRO(_OPENFLUID_ADD_WARE_MESSAGES _WAREID _WARETYPE _WAREFILES _WARETARGET)
  MESSAGE(STATUS "***** ${_WARETYPE} ${_WAREID} *****")
  
  SET(_FILESMSG "  Files:")
  FOREACH(_WFILE ${_WAREFILES})
    SET(_FILESMSG "${_FILESMSG} ${_WFILE}")
  ENDFOREACH()

  MESSAGE(STATUS ${_FILESMSG})
  MESSAGE(STATUS "  Target: ${_WARETARGET}")

ENDMACRO()


###########################################################################


MACRO(OPENFLUID_SHOW_CMAKE_VARIABLES)
  MESSAGE("OpenFLUID_FOUND: " ${OpenFLUID_FOUND})
  MESSAGE("OpenFLUID_VERSION: " ${OpenFLUID_VERSION})
  MESSAGE("OpenFLUID_DIR: " ${OpenFLUID_DIR})
  MESSAGE("OpenFLUID_HOME_DIR: " ${OpenFLUID_HOME_DIR})
  MESSAGE("OpenFLUID_INCLUDE_DIR: " ${OpenFLUID_INCLUDE_DIR})
  MESSAGE("OpenFLUID_INCLUDE_DIRS: " ${OpenFLUID_INCLUDE_DIR})
  MESSAGE("OpenFLUID_LIBRARY_DIR: " ${OpenFLUID_LIBRARY_DIR})
  MESSAGE("OpenFLUID_LIBRARY_DIRS: " ${OpenFLUID_LIBRARY_DIR})
  MESSAGE("OpenFLUID_CORE_LIBRARY: " ${OpenFLUID_CORE_LIBRARY})
  MESSAGE("OpenFLUID_BASE_LIBRARY: " ${OpenFLUID_BASE_LIBRARY})
  MESSAGE("OpenFLUID_TOOLS_LIBRARY: " ${OpenFLUID_TOOLS_LIBRARY})
  MESSAGE("OpenFLUID_WARE_LIBRARY: " ${OpenFLUID_WARE_LIBRARY})
  MESSAGE("OpenFLUID_LIBRARIES: " ${OpenFLUID_LIBRARIES})
  MESSAGE("OpenFLUID_PROGRAM: " ${OpenFLUID_CMD_PROGRAM})
  MESSAGE("OpenFLUID_OBSERVER_FILENAME_SUFFIX: " ${OpenFLUID_OBSERVER_FILENAME_SUFFIX})
  MESSAGE("OpenFLUID_SIMULATOR_FILENAME_SUFFIX: " ${OpenFLUID_SIMULATOR_FILENAME_SUFFIX})
ENDMACRO()


###########################################################################

# This macro uses the following variables
# _SIMNAME_ID : simulator ID
# _SIMNAME_CPP : list of C++ files. The sim2doc tag must be contained in the first one
# _SIMNAME_FORTRAN : list of Fortran files, if any
# _SIMNAME_DEFINITIONS : definitions to add at compile time
# _SIMNAME_INCLUDE_DIRS : directories to include for simulator compilation
# _SIMNAME_LIBRARY_DIRS : directories to link for simulator compilation
# _SIMNAME_LINK_LIBS : libraries to link for function build
# _SIMNAME_INSTALL_PATH : install path, replacing the default one
# _SIMNAME_SIM2DOC_DISABLED : set to 1 to disable sim2doc pdf build
# _SIMNAME_SIM2DOC_INSTALL_DISABLED : set to 1 to disable installation of sim2doc built pdf
# _SIMNAME_SIM2DOC_TPL : user-defined template for documentation
# _SIMNAME_TEST_DATASETS : list of datasets for tests

MACRO(OPENFLUID_ADD_SIMULATOR _SIMNAME)

  FIND_PACKAGE(OpenFLUID REQUIRED core base tools ware)
  FIND_PACKAGE(PkgConfig REQUIRED)
  PKG_CHECK_MODULES(GLIBMM REQUIRED glibmm-2.4)
  PKG_CHECK_MODULES(GTHREAD REQUIRED gthread-2.0)
  
  IF (NOT ${_SIMNAME}_SOURCE_DIR)
    SET(${_SIMNAME}_SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR})
  ENDIF()  
  
  SET(${_SIMNAME}_TARGET ${${_SIMNAME}_ID}${OpenFLUID_SIMULATOR_FILENAME_SUFFIX})

  _OPENFLUID_ADD_WARE_MESSAGES(${${_SIMNAME}_ID} "Simulator" "${${_SIMNAME}_CPP}" ${${_SIMNAME}_TARGET})

  IF(${_SIMNAME}_FORTRAN)
    ENABLE_LANGUAGE(Fortran)
    SET(CMAKE_Fortran_FLAGS_RELEASE "-funroll-all-loops -fno-f2c -O3")
    SET(_FORTRAN_LINK_LIBS "gfortran")
  ENDIF()

  ADD_DEFINITIONS(${${_SIMNAME}_DEFINITIONS})
  INCLUDE_DIRECTORIES(${OpenFLUID_INCLUDE_DIRS} ${GLIBMM_INCLUDE_DIRS} ${${_SIMNAME}_INCLUDE_DIRS})
  LINK_DIRECTORIES(${OpenFLUID_LIBRARY_DIRS} ${GLIBMM_LIBRARY_DIRS} ${${_SIMNAME}_LIBRARY_DIRS}) 
    
  
  # ====== build ======
    
  IF(MINGW)
    # workaround for CMake bug with MinGW
    ADD_LIBRARY(${${_SIMNAME}_TARGET} SHARED ${${_SIMNAME}_CPP} ${${_SIMNAME}_FORTRAN})
  ELSE()
    ADD_LIBRARY(${${_SIMNAME}_TARGET} MODULE ${${_SIMNAME}_CPP} ${${_SIMNAME}_FORTRAN})
  ENDIF()
  
  SET_TARGET_PROPERTIES(${${_SIMNAME}_TARGET} PROPERTIES PREFIX "" SUFFIX ${CMAKE_SHARED_LIBRARY_SUFFIX})

  TARGET_LINK_LIBRARIES(${${_SIMNAME}_TARGET} ${OpenFLUID_LIBRARIES} ${GLIBMM_LIBRARIES} ${GTHREAD_LIBRARIES} ${${_SIMNAME}_LINK_LIBS})
  
    
  # ====== doc ======  
  
  IF(NOT ${_SIMNAME}_SIM2DOC_DISABLED)
    FIND_PACKAGE(LATEX)
 
    IF(PDFLATEX_COMPILER)
  
      LIST(GET ${_SIMNAME}_CPP 0 _CPP_FOR_DOC)
    
      IF(NOT ${_SIMNAME}_SIM2DOC_OUTPUT_PATH)
        SET(_DOCS_OUTPUT_PATH "${PROJECT_BINARY_DIR}")
      ELSE()
        SET(_DOCS_OUTPUT_PATH "${_SIMNAME}_SIM2DOC_OUTPUT_PATH")
      ENDIF()
    
      IF(${_SIMNAME}_SIM2DOC_TPL)
        SET (_TPL_OPTION ",tplfile=${${_SIMNAME}_SIM2DOC_TPL}")
      ENDIF()
    
      ADD_CUSTOM_COMMAND(
        OUTPUT "${_DOCS_OUTPUT_PATH}/${${_SIMNAME}_ID}.pdf"
        DEPENDS "${${_SIMNAME}_SOURCE_DIR}/${_CPP_FOR_DOC}"
        COMMAND "${OpenFLUID_CMD_PROGRAM}"
        ARGS "--buddy" "sim2doc" "--buddyopts" "inputcpp=${${_SIMNAME}_SOURCE_DIR}/${_CPP_FOR_DOC},outputdir=${_DOCS_OUTPUT_PATH},pdf=1${_TPL_OPTION}"     
      )
    
      ADD_CUSTOM_TARGET(${${_SIMNAME}_ID}-doc ALL DEPENDS "${_DOCS_OUTPUT_PATH}/${${_SIMNAME}_ID}.pdf")
      
      MESSAGE(STATUS "  sim2doc : enabled")      
    ELSE()
      MESSAGE(STATUS "  sim2doc : pdflatex not found")
    ENDIF()
  ELSE()
    MESSAGE(STATUS "  sim2doc : disabled")
  ENDIF()


  # ====== tests ======  

  IF(${_SIMNAME}_TESTS_DATASETS)
  
    ENABLE_TESTING()
  
    SET(_TESTS_SIM_SEARCHPATHS "${PROJECT_BINARY_DIR}")
  
    IF (${_SIMNAME}_TESTS_SIM_SEARCHPATHS)
      SET(_TESTS_SIM_SEARCHPATHS "${PROJECT_BINARY_DIR}")
    ENDIF()
    
    SET(_TESTS_OUTPUT_DIR ${PROJECT_BINARY_DIR}/tests-output)  
    FILE(MAKE_DIRECTORY "${_TESTS_OUTPUT_DIR}")
  
    FOREACH(_CURRTEST ${${_SIMNAME}_TESTS_DATASETS})
      ADD_TEST(${${_SIMNAME}_ID}-${_CURRTEST} "${OpenFLUID_CMD_PROGRAM}"
                                "-i" "${${_SIMNAME}_SOURCE_DIR}/tests/${_CURRTEST}.IN"
                                "-o" "${_TESTS_OUTPUT_DIR}/${_CURRTEST}.OUT"
                                "-p" "${_TESTS_SIM_SEARCHPATHS}")
      MESSAGE(STATUS "  Added test ${${_SIMNAME}_ID}-${_CURRTEST}")                            
    ENDFOREACH()
  ENDIF()  


  # ====== install ======
  
  IF(NOT ${_SIMNAME}_INSTALL_PATH)
    SET(_INSTALL_PATH "$ENV{HOME}/.openfluid/simulators")
    IF(WIN32)
      SET(_INSTALL_PATH "$ENV{USERPROFILE}/openfluid/simulators") 
    ENDIF()
  ELSE()
    SET(_INSTALL_PATH "${${_SIMNAME}_INSTALL_PATH}")   
  ENDIF()
  
  INSTALL(TARGETS ${${_SIMNAME}_TARGET} DESTINATION "${_INSTALL_PATH}")
  
  IF(PDFLATEX_COMPILER AND NOT ${_SIMNAME}_SIM2DOC_DISABLED AND NOT ${_SIMNAME}_SIM2DOC_INSTALL_DISABLED)
    INSTALL(FILES "${_DOCS_OUTPUT_PATH}/${${_SIMNAME}_ID}.pdf" DESTINATION "${_INSTALL_PATH}")
  ENDIF()
  
  MESSAGE(STATUS "  Install path : ${_INSTALL_PATH}")
  
ENDMACRO()


###########################################################################


MACRO(OPENFLUID_ADD_DEFAULT_SIMULATOR)
  OPENFLUID_ADD_SIMULATOR(SIM)
ENDMACRO()


###########################################################################


MACRO(OPENFLUID_ADD_OBSERVER _OBSNAME)

  FIND_PACKAGE(OpenFLUID REQUIRED core base tools ware)
  FIND_PACKAGE(PkgConfig REQUIRED)
  PKG_CHECK_MODULES(GLIBMM REQUIRED glibmm-2.4)
  PKG_CHECK_MODULES(GTHREAD REQUIRED gthread-2.0)
  
  SET(${_OBSNAME}_TARGET ${${_OBSNAME}_ID}${OpenFLUID_OBSERVER_FILENAME_SUFFIX})

  _OPENFLUID_ADD_WARE_MESSAGES(${${_OBSNAME}_ID} "Observer" ${${_OBSNAME}_CPP} ${${_OBSNAME}_TARGET})

  INCLUDE_DIRECTORIES(${OpenFLUID_INCLUDE_DIRS} ${${_OBSNAME}_INCLUDE_DIRS} ${GLIBMM_INCLUDE_DIRS})
  LINK_DIRECTORIES(${OpenFLUID_LIBRARY_DIRS} ${GLIBMM_LIBRARY_DIRS}) 


  ADD_LIBRARY(${${_OBSNAME}_TARGET} MODULE ${${_OBSNAME}_CPP})
  SET_TARGET_PROPERTIES(${${_OBSNAME}_TARGET} PROPERTIES PREFIX "" SUFFIX ${CMAKE_SHARED_LIBRARY_SUFFIX})

  TARGET_LINK_LIBRARIES(${${_OBSNAME}_TARGET} ${OpenFLUID_LIBRARIES} ${GLIBMM_LIBRARIES} ${GTHREAD_LIBRARIES})
  
ENDMACRO()


###########################################################################


MACRO(OPENFLUID_ADD_DEFAULT_OBSERVER)
  OPENFLUID_ADD_OBSERVER(OBS)
ENDMACRO()


###########################################################################


MACRO(OPENFLUID_ADD_TEST)

  SET(_ONEARGS_CMDS NAME)
  SET(_MANYARGS_CMDS COMMAND PRE_TEST POST_TEST)
  
  SET(_ADD_TEST)
  
  CMAKE_PARSE_ARGUMENTS(_ADD_TEST "" "${_ONEARGS_CMDS}" "${_MANYARGS_CMDS}" ${ARGN})
  
  ADD_TEST(NAME ${_ADD_TEST_NAME} COMMAND "${CMAKE_COMMAND}" "-E" "chdir" "${CMAKE_CURRENT_BINARY_DIR}"
                                             "${CMAKE_COMMAND}" 
                                               "-DCMD=${_ADD_TEST_COMMAND}"
                                               "-DPRECMDS=${_ADD_TEST_PRE_TEST}"
                                               "-DPOSTCMDS=${_ADD_TEST_POST_TEST}"
                                               "-P" "${OpenFLUID_DIR}/OpenFLUIDTestScript.cmake")
ENDMACRO()


###########################################################################


MACRO(OPENFLUID_ADD_GEOS_DEFINITIONS)
  STRING(COMPARE LESS ${GEOS_VERSION} "3.3.0" GEOS_VERSION_LESS_THAN_3_3_0)
  IF(GEOS_VERSION_LESS_THAN_3_3_0)
    SET(GEOS_VERSION_GREATER_OR_EQUAL_3_3_0  0)
  ELSE()
    SET(GEOS_VERSION_GREATER_OR_EQUAL_3_3_0  1)
  ENDIF()

  STRING(COMPARE LESS ${GEOS_VERSION} "3.3.2" GEOS_VERSION_LESS_THAN_3_3_2)
  IF(GEOS_VERSION_LESS_THAN_3_3_2)
    SET(GEOS_VERSION_GREATER_OR_EQUAL_3_3_2  0)
  ELSE()
    SET(GEOS_VERSION_GREATER_OR_EQUAL_3_3_2  1)
  ENDIF()
  
  ADD_DEFINITIONS(-DGEOS_VERSION_GREATER_OR_EQUAL_3_3_0=${GEOS_VERSION_GREATER_OR_EQUAL_3_3_0} -DGEOS_VERSION_GREATER_OR_EQUAL_3_3_2=${GEOS_VERSION_GREATER_OR_EQUAL_3_3_2})
ENDMACRO()  