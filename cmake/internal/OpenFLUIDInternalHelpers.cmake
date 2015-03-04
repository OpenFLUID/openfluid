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


# Macro for compiling a simulator
MACRO(OPNFLD_ADD_SIMULATOR SIM_NAME SIM_SRCDIR SIM_BINDIR SIM_PARAMSUI)

  SET(SIM_UI)
  FILE(GLOB SIM_CPP ${SIM_SRCDIR}/*.cpp)  
  
  IF(${SIM_PARAMSUI})
    FILE(GLOB SIM_UIFILES ${SIM_SRCDIR}/*.ui)
    QT4_WRAP_UI(SIM_UI ${SIM_UIFILES})
  ENDIF()
    
  ADD_LIBRARY("${SIM_NAME}${OPENFLUID_SIMULATORS_SUFFIX}" MODULE ${SIM_CPP} ${SIM_UI})
  
  SET_TARGET_PROPERTIES("${SIM_NAME}${OPENFLUID_SIMULATORS_SUFFIX}" PROPERTIES 
                        PREFIX ""
                        SUFFIX "${PLUGINS_BINARY_EXTENSION}"
                        LIBRARY_OUTPUT_DIRECTORY "${SIM_BINDIR}")

  # Fix for win32 compatibility
  IF(WIN32)
    SET_TARGET_PROPERTIES("${SIM_NAME}${OPENFLUID_SIMULATORS_SUFFIX}" PROPERTIES LINK_FLAGS "-shared")                                                
  ENDIF(WIN32)
                                                
  TARGET_LINK_LIBRARIES("${SIM_NAME}${OPENFLUID_SIMULATORS_SUFFIX}"
                        openfluid-core
                        openfluid-base
                        openfluid-ware
                        openfluid-tools
                        ${QT_QTCORE_LIBRARIES})
      
  IF(${SIM_PARAMSUI})
    SET_TARGET_PROPERTIES("${SIM_NAME}${OPENFLUID_SIMULATORS_SUFFIX}" PROPERTIES AUTOMOC ON)
  
    TARGET_LINK_LIBRARIES("${SIM_NAME}${OPENFLUID_SIMULATORS_SUFFIX}"
                          openfluid-ui-ware
                          ${QT_QTGUI_LIBRARIES})  
  ENDIF()      
      
ENDMACRO()


###########################################################################


# Macro for compiling a simulation observer and add installation directives
MACRO(OPNFLD_ADD_SIMULATOR_WITH_INSTALL SIM_NAME SIM_SRCDIR SIM_BINDIR SIM_PARAMSUI)

  OPNFLD_ADD_SIMULATOR(${SIM_NAME} ${SIM_SRCDIR} ${SIM_BINDIR} ${SIM_PARAMSUI})

  INSTALL(TARGETS "${SIM_NAME}${OPENFLUID_SIMULATORS_SUFFIX}"
          DESTINATION "${SIMULATORS_INSTALL_PATH}")
        
ENDMACRO()


###########################################################################


# Macro for compiling a simulation observer
MACRO(OPNFLD_ADD_OBSERVER OBS_NAME OBS_SRCDIR OBS_BINDIR OBS_PARAMSUI)

  SET(OBS_UI)
  FILE(GLOB OBS_CPP ${OBS_SRCDIR}/*.cpp ${OBS_SRCDIR}/*.c)
  
  IF(${OBS_PARAMSUI})
    FILE(GLOB OBS_UIFILES ${OBS_SRCDIR}/*.ui)
    QT4_WRAP_UI(OBS_UI ${OBS_UIFILES})
  ENDIF()
  
  INCLUDE_DIRECTORIES(${OBS_SRCDIR})
    
  ADD_LIBRARY("${OBS_NAME}${OPENFLUID_OBSERVERS_SUFFIX}" MODULE ${OBS_CPP} ${OBS_UI})
  
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
                        openfluid-tools
                        ${QT_QTCORE_LIBRARIES})
  
  IF(${OBS_PARAMSUI})
    SET_TARGET_PROPERTIES("${OBS_NAME}${OPENFLUID_OBSERVERS_SUFFIX}" PROPERTIES AUTOMOC ON)
  
    TARGET_LINK_LIBRARIES("${OBS_NAME}${OPENFLUID_OBSERVERS_SUFFIX}"
                          openfluid-ui-ware
                          ${QT_QTGUI_LIBRARIES})  
  ENDIF()                     
      
ENDMACRO()


###########################################################################


# Macro for compiling a simulation observer and add installation directives
MACRO(OPNFLD_ADD_OBSERVER_WITH_INSTALL OBS_NAME OBS_SRCDIR OBS_BINDIR OBS_PARAMSUI)

  OPNFLD_ADD_OBSERVER(${OBS_NAME} ${OBS_SRCDIR} ${OBS_BINDIR} ${OBS_PARAMSUI})

  INSTALL(TARGETS "${OBS_NAME}${OPENFLUID_OBSERVERS_SUFFIX}"
          DESTINATION "${OBSERVERS_INSTALL_PATH}")
        
ENDMACRO()


###########################################################################


MACRO(OPNFLD_ADD_BUILDER_EXTENSION EXT_NAME EXT_SRCDIR EXT_BINDIR)

  FILE(GLOB EXT_CPP ${EXT_SRCDIR}/*.cpp)  
  FILE(GLOB EXT_UIFILES ${EXT_SRCDIR}/*.ui)
  FILE(GLOB EXT_RCFILES ${EXT_SRCDIR}/*.qrc)
    
  QT4_WRAP_UI(EXT_UI ${EXT_UIFILES})
  QT4_ADD_RESOURCES(EXT_RC ${EXT_RCFILES})
  
  INCLUDE_DIRECTORIES(${EXT_SRCDIR})
  INCLUDE_DIRECTORIES(${QT_INCLUDES})  
  
  ADD_LIBRARY(${EXT_NAME}${OPENFLUID_BUILDEREXTS_SUFFIX} MODULE ${EXT_CPP} ${EXT_UI} ${EXT_RC})
    
  SET_TARGET_PROPERTIES(${EXT_NAME}${OPENFLUID_BUILDEREXTS_SUFFIX} PROPERTIES 
                        PREFIX "" 
                        SUFFIX "${PLUGINS_BINARY_EXTENSION}"
                        LIBRARY_OUTPUT_DIRECTORY "${EXT_BINDIR}"
                        AUTOMOC ON)

  # Fix for win32 compatibility                                              
  IF(WIN32)
    SET_TARGET_PROPERTIES(${EXT_NAME}${OPENFLUID_BUILDEREXTS_SUFFIX} PROPERTIES LINK_FLAGS "-shared")                                                
  ENDIF(WIN32)
                                              
                                                
  TARGET_LINK_LIBRARIES(${EXT_NAME}${OPENFLUID_BUILDEREXTS_SUFFIX} 
                        openfluid-core
                        openfluid-base 
                        openfluid-tools                        
                        openfluid-ware
                        openfluid-builderext                        
                        ${QT_QTCORE_LIBRARIES}
                        ${QT_QTGUI_LIBRARIES}                      
                       )
    
ENDMACRO()


###########################################################################


# Macro for compiling a builder extension and add installation directives
MACRO(OPNFLD_ADD_BUILDER_EXTENSION_WITH_INSTALL EXT_NAME EXT_SRCDIR EXT_BINDIR)

  OPNFLD_ADD_BUILDER_EXTENSION(${EXT_NAME} ${EXT_SRCDIR} ${EXT_BINDIR})

  INSTALL(TARGETS "${EXT_NAME}${OPENFLUID_BUILDEREXTS_SUFFIX}"
          DESTINATION "${BUILDEREXTS_INSTALL_PATH}")
        
ENDMACRO()


###########################################################################


MACRO(OPNFLD_ADD_UNITTEST TEST_CAT TEST_NAME)
  ADD_EXECUTABLE("${TEST_CAT}-${TEST_NAME}" ${ARGN})
  TARGET_LINK_LIBRARIES("${TEST_CAT}-${TEST_NAME}" 
                       ${Boost_UNIT_TEST_FRAMEWORK_LIBRARY}
                       ${Boost_SYSTEM_LIBRARY} 
                       ${Boost_DATE_TIME_LIBRARY}
                       ${UNITTEST_LINK_LIBRARIES})
  SET_TARGET_PROPERTIES("${TEST_CAT}-${TEST_NAME}" PROPERTIES RUNTIME_OUTPUT_DIRECTORY "${TEST_OUTPUT_PATH}")
  ADD_TEST("unit-${TEST_CAT}-${TEST_NAME}" "${TEST_OUTPUT_PATH}/${TEST_CAT}-${TEST_NAME}")
  
  SET_TESTS_PROPERTIES("unit-${TEST_CAT}-${TEST_NAME}" 
                       PROPERTIES ENVIRONMENT "OPENFLUID_TEMP_PATH=${OPENFLUID_TESTS_TEMP_PATH};OPENFLUID_USERDATA_PATH=${OPENFLUID_TESTS_USERDATA_PATH}")
ENDMACRO()


###########################################################################


MACRO(OPNFLD_DISCOVER_UNITTESTS TEST_CAT)
  FILE(GLOB UNIT_TESTS_CPP *_TEST.cpp)
  FOREACH(CPPFILE ${UNIT_TESTS_CPP})
    GET_FILENAME_COMPONENT(CPPFILE_WE ${CPPFILE} NAME_WE)
    OPNFLD_ADD_UNITTEST(${TEST_CAT} ${CPPFILE_WE} ${CPPFILE})
  ENDFOREACH(CPPFILE ${UNIT_TESTS_CPP})  
ENDMACRO()


###########################################################################


MACRO(OPNFLD_DISCOVER_HEAVYUNITTESTS TEST_CAT)
  IF(OPENFLUID_ENABLE_HEAVYTESTING)
    FILE(GLOB UNIT_TESTS_CPP *_HEAVYTEST.cpp)
    FOREACH(CPPFILE ${UNIT_TESTS_CPP})
      GET_FILENAME_COMPONENT(CPPFILE_WE ${CPPFILE} NAME_WE)
      OPNFLD_ADD_UNITTEST(${TEST_CAT} ${CPPFILE_WE} ${CPPFILE})
    ENDFOREACH(CPPFILE ${UNIT_TESTS_CPP})
  ENDIF()
ENDMACRO()



