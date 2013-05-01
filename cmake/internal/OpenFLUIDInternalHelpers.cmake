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


INCLUDE(CMakeParseArguments)


###########################################################################


# Macro for compiling a simulator
MACRO(OPNFLD_ADD_SIMULATOR SIM_NAME SIM_SRCDIR SIM_BINDIR)

  FILE(GLOB SIM_CPP ${SIM_SRCDIR}/*.cpp)  
    
  ADD_LIBRARY("${SIM_NAME}${OPENFLUID_SIMULATORS_SUFFIX}" MODULE ${SIM_CPP})
  
  SET_TARGET_PROPERTIES("${SIM_NAME}${OPENFLUID_SIMULATORS_SUFFIX}" PROPERTIES 
                        PREFIX ""
                        SUFFIX "${PLUGINS_BINARY_EXTENSION}"
                        LIBRARY_OUTPUT_DIRECTORY "${SIM_BINDIR}")

  # Fix for win32 compatibility
  IF(WIN32)
    SET_TARGET_PROPERTIES("${SIM_NAME}${OPENFLUID_SIMULATORS_SUFFIX}" PROPERTIES LINK_FLAGS "-shared")                                                
  ENDIF(WIN32)
                                                
  TARGET_LINK_LIBRARIES("${SIM_NAME}${OPENFLUID_SIMULATORS_SUFFIX}"
  						${Boost_REGEX_LIBRARY}
                        openfluid-core
                        openfluid-base
                        openfluid-ware
                        openfluid-tools)
      
ENDMACRO()


###########################################################################


# Macro for compiling a simulation observer and add installation directives
MACRO(OPNFLD_ADD_SIMULATOR_WITH_INSTALL SIM_NAME SIM_SRCDIR SIM_BINDIR)

  OPNFLD_ADD_SIMULATOR(${SIM_NAME} ${SIM_SRCDIR} ${SIM_BINDIR})

  INSTALL(TARGETS "${SIM_NAME}${OPENFLUID_SIMULATORS_SUFFIX}"
          DESTINATION "${SIMULATORS_INSTALL_PATH}")
        
ENDMACRO()


###########################################################################


# Macro for compiling a simulation observer
MACRO(OPNFLD_ADD_OBSERVER OBS_NAME OBS_SRCDIR OBS_BINDIR)

  FILE(GLOB OBS_CPP ${OBS_SRCDIR}/*.cpp ${OBS_SRCDIR}/*.c)  
    
  ADD_LIBRARY("${OBS_NAME}${OPENFLUID_OBSERVERS_SUFFIX}" MODULE ${OBS_CPP})
  
  SET_TARGET_PROPERTIES("${OBS_NAME}${OPENFLUID_OBSERVERS_SUFFIX}" PROPERTIES 
                        PREFIX ""
                        SUFFIX "${PLUGINS_BINARY_EXTENSION}"
                        LIBRARY_OUTPUT_DIRECTORY "${OBS_BINDIR}")

  # Fix for win32 compatibility
  IF(WIN32)
    SET_TARGET_PROPERTIES("${OBS_NAME}${OPENFLUID_OBSERVERS_SUFFIX}" PROPERTIES LINK_FLAGS "-shared")                                                
  ENDIF(WIN32)
                                                
  TARGET_LINK_LIBRARIES("${OBS_NAME}${OPENFLUID_OBSERVERS_SUFFIX}"
                        openfluid-core
                        openfluid-base
                        openfluid-ware
                        openfluid-tools)
      
ENDMACRO()


###########################################################################


# Macro for compiling a simulation observer and add installation directives
MACRO(OPNFLD_ADD_OBSERVER_WITH_INSTALL OBS_NAME OBS_SRCDIR OBS_BINDIR)

  OPNFLD_ADD_OBSERVER(${OBS_NAME} ${OBS_SRCDIR} ${OBS_BINDIR})

  INSTALL(TARGETS "${OBS_NAME}${OPENFLUID_OBSERVERS_SUFFIX}"
          DESTINATION "${OBSERVERS_INSTALL_PATH}")
        
ENDMACRO()


###########################################################################


# Macro for compiling a builder extension
MACRO(OPNFLD_ADD_BUILDER_EXTENSION EXT_NAME EXT_SRCDIR EXT_BINDIR)

  FILE(GLOB EXT_CPP ${EXT_SRCDIR}/*.cpp)
  
  ADD_LIBRARY(${EXT_NAME} MODULE ${EXT_CPP})
  
  INCLUDE_DIRECTORIES(${LibXML2_INCLUDE_DIRS})
  LINK_DIRECTORIES(${LibXML2_LIBRARY_DIRS})
  
  SET_TARGET_PROPERTIES(${EXT_NAME} PROPERTIES 
                        PREFIX "" 
                        SUFFIX "${BUILDEREXTENSION_BINARY_EXTENSION}"
                        LIBRARY_OUTPUT_DIRECTORY "${EXT_BINDIR}")

  # Fix for win32 compatibility                                              
  IF(WIN32)
    SET_TARGET_PROPERTIES(${EXT_NAME} PROPERTIES LINK_FLAGS "-shared")                                                
  ENDIF(WIN32)
                                              
                                                
  TARGET_LINK_LIBRARIES(${EXT_NAME} 
                        openfluid-core
                        openfluid-base 
                        openfluid-tools
                        openfluid-guicommon
                        ${LibXML2_LIBRARIES}  
                        ${GTKMM_LIBRARIES})
    
ENDMACRO()


###########################################################################


MACRO(OPNFLD_ADD_UNITTEST TEST_CAT TEST_NAME)
  ADD_EXECUTABLE("${TEST_CAT}-${TEST_NAME}" ${ARGN})
  TARGET_LINK_LIBRARIES("${TEST_CAT}-${TEST_NAME}" 
                       ${Boost_UNIT_TEST_FRAMEWORK_LIBRARY}
                       ${Boost_SYSTEM_LIBRARY} ${Boost_FILESYSTEM_LIBRARY} 
                       ${Boost_DATE_TIME_LIBRARY}
                       ${UNITTEST_LINK_LIBRARIES})
  SET_TARGET_PROPERTIES("${TEST_CAT}-${TEST_NAME}" PROPERTIES RUNTIME_OUTPUT_DIRECTORY "${TEST_OUTPUT_PATH}")
  ADD_TEST("unit-${TEST_CAT}-${TEST_NAME}" "${TEST_OUTPUT_PATH}/${TEST_CAT}-${TEST_NAME}")
ENDMACRO()


###########################################################################

MACRO(OPNFLD_DISCOVER_UNITTESTS TEST_CAT)
  FILE(GLOB UNIT_TESTS_CPP *_TEST.cpp)
  FOREACH(CPPFILE ${UNIT_TESTS_CPP})
    GET_FILENAME_COMPONENT(CPPFILE_WE ${CPPFILE} NAME_WE)
    OPNFLD_ADD_UNITTEST(${TEST_CAT} ${CPPFILE_WE} ${CPPFILE})
  ENDFOREACH(CPPFILE ${UNIT_TESTS_CPP})
ENDMACRO()




