#
# Macros and functions file for CMakeLists.txt files
#
# Author : Jean-Christophe FABRE <fabrejc@supagro.inra.fr>
#
# This file is included by the main CMakeLists.txt file, and defines macros
# and functions that are used in CMakeLists.txt files 
#



# Macro for compiling a simulation function
MACRO(ADD_FUNCTION FUNC_NAME FUNC_SRCDIR FUNC_BINDIR)

  FILE(GLOB FUNC_CPP ${FUNC_SRCDIR}/*.cpp)  
    
  ADD_LIBRARY("${FUNC_NAME}${OPENFLUID_FUNCTIONS_SUFFIX}" MODULE ${FUNC_CPP})
  
  SET_TARGET_PROPERTIES("${FUNC_NAME}${OPENFLUID_FUNCTIONS_SUFFIX}" PROPERTIES 
                        PREFIX ""
                        SUFFIX "${PLUGINS_BINARY_EXTENSION}"
                        LIBRARY_OUTPUT_DIRECTORY "${FUNC_BINDIR}")

  # Fix for win32 compatibility
  IF(WIN32)
    SET_TARGET_PROPERTIES("${FUNC_NAME}${OPENFLUID_FUNCTIONS_SUFFIX}" PROPERTIES LINK_FLAGS "-shared")                                                
  ENDIF(WIN32)
                                                
  TARGET_LINK_LIBRARIES("${FUNC_NAME}${OPENFLUID_FUNCTIONS_SUFFIX}"
  						${Boost_REGEX_LIBRARY}
                        openfluid-core
                        openfluid-base
                        openfluid-ware
                        openfluid-tools)
      
ENDMACRO(ADD_FUNCTION)


# Macro for compiling a simulation observer
MACRO(ADD_OBSERVER OBS_NAME OBS_SRCDIR OBS_BINDIR)

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
      
ENDMACRO(ADD_OBSERVER)


# Macro for compiling a simulation observer and add installation directives
MACRO(ADD_OBSERVER_WITH_INSTALL OBS_NAME OBS_SRCDIR OBS_BINDIR)

  ADD_OBSERVER(${OBS_NAME} ${OBS_SRCDIR} ${OBS_BINDIR})

  INSTALL(TARGETS "${OBS_NAME}${OPENFLUID_OBSERVERS_SUFFIX}"
          DESTINATION "${OBSERVERS_INSTALL_PATH}")
        
ENDMACRO()


# Macro for compiling a builder extension
MACRO(ADD_BUILDER_EXTENSION EXT_NAME EXT_SRCDIR EXT_BINDIR)

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
    
ENDMACRO(ADD_BUILDER_EXTENSION)




MACRO(COMPILE_PDFLATEX_DOC DOC_NAME DOC_BUILDDIR DOC_OUTDIR DOC_INSTALL_PATH)
  
  ADD_CUSTOM_COMMAND(
    OUTPUT  "${DOC_BUILDDIR}/${DOC_NAME}.pdf"
    DEPENDS "${DOC_BUILDDIR}"
    DEPENDS "${DOC_BUILDDIR}/${DOC_NAME}.tex"           
    COMMAND ${CMAKE_COMMAND} -E chdir ${DOC_BUILDDIR} ${PDFLATEX_COMPILER}
    ARGS "${DOC_BUILDDIR}/${DOC_NAME}.tex"       
    COMMAND ${CMAKE_COMMAND} -E chdir ${DOC_BUILDDIR} ${PDFLATEX_COMPILER}
    ARGS "${DOC_BUILDDIR}/${DOC_NAME}.tex"    
    )  


  ADD_CUSTOM_COMMAND(    
    OUTPUT  "${DOC_OUTDIR}/${DOC_NAME}.pdf"
    DEPENDS "${DOC_BUILDDIR}/${DOC_NAME}.pdf"      
    COMMAND ${CMAKE_COMMAND} -E copy ${DOC_BUILDDIR}/${DOC_NAME}.pdf ${DOC_OUTDIR}/${DOC_NAME}.pdf        
    )  


  ADD_CUSTOM_TARGET(${DOC_NAME}-doc-pdf ALL
    DEPENDS ${DOC_OUTDIR}/${DOC_NAME}.pdf
   )
 
 
  INSTALL(FILES ${DOC_OUTDIR}/${DOC_NAME}.pdf 
          DESTINATION ${DOC_INSTALL_PATH})
 
ENDMACRO(COMPILE_PDFLATEX_DOC DOC_NAME DOC_BUILDDIR DOC_OUTDIR DOC_INSTALL_PATH)





MACRO(COMPILE_LATEX2HTML_DOC DOC_NAME DOC_BUILDDIR DOC_OUTDIR DOC_INSTALL_PATH)

  ADD_CUSTOM_COMMAND(
    OUTPUT  "${DOC_BUILDDIR}/html"        
    DEPENDS "${DOC_BUILDDIR}"
    COMMAND ${CMAKE_COMMAND} -E make_directory ${DOC_BUILDDIR}/html       
   )  
    

  ADD_CUSTOM_COMMAND(
    OUTPUT  "${DOC_BUILDDIR}/html/${DOC_NAME}/index.html"
    DEPENDS "${DOC_BUILDDIR}/${DOC_NAME}.tex"
    DEPENDS "${DOC_BUILDDIR}/html"    
    COMMAND ${CMAKE_COMMAND} -E chdir ${DOC_BUILDDIR} ${LATEX2HTML_CONVERTER}
    ARGS -mkdir -split +1 -html_version 4.0,latin1,unicode -show_section_numbers -noinfo -local_icons -dir ${DOC_BUILDDIR}/html/${DOC_NAME} ${DOC_BUILDDIR}/${DOC_NAME}.tex       
    )  


  ADD_CUSTOM_COMMAND(    
    OUTPUT  "${DOC_OUTDIR}/${DOC_NAME}/"
    DEPENDS  "${DOC_BUILDDIR}/html/${DOC_NAME}/index.html"      
    COMMAND ${CMAKE_COMMAND} -E copy_directory ${DOC_BUILDDIR}/html/${DOC_NAME}/ ${DOC_OUTDIR}/${DOC_NAME}        
    )  


  ADD_CUSTOM_TARGET(${DOC_NAME}-doc-html ALL
    DEPENDS "${DOC_OUTDIR}/${DOC_NAME}/"
   )

ENDMACRO(COMPILE_LATEX2HTML_DOC DOC_NAME DOC_BUILDDIR DOC_OUTDIR DOC_INSTALL_PATH)


###
# unit-api-
# unit-builder-
# function-
# integration-engine-


MACRO(ADD_UNITTEST TEST_NAME EXE_NAME)
  ADD_EXECUTABLE(${EXE_NAME} ${ARGN})
  TARGET_LINK_LIBRARIES(${EXE_NAME} 
                       ${Boost_UNIT_TEST_FRAMEWORK_LIBRARY}
                       ${Boost_SYSTEM_LIBRARY} ${Boost_FILESYSTEM_LIBRARY} 
                       ${Boost_DATE_TIME_LIBRARY} ${Boost_PROGRAM_OPTIONS_LIBRARY} 
                       ${Boost_REGEX_LIBRARY}                      
                       openfluid-base openfluid-buddies
                       openfluid-core openfluid-machine openfluid-ware
                       openfluid-io openfluid-market openfluid-tools openfluid-guicommon)
  SET_TARGET_PROPERTIES(${EXE_NAME} PROPERTIES RUNTIME_OUTPUT_DIRECTORY "${TEST_OUTPUT_PATH}")
  ADD_TEST(${TEST_NAME} "${TEST_OUTPUT_PATH}/${EXE_NAME}")
ENDMACRO(ADD_UNITTEST)


MACRO(ADD_OPENFLUID_UNITTEST TEST_CAT TEST_NAME)
  ADD_EXECUTABLE("${TEST_CAT}-${TEST_NAME}" ${ARGN})
  TARGET_LINK_LIBRARIES("${TEST_CAT}-${TEST_NAME}" 
                       ${Boost_UNIT_TEST_FRAMEWORK_LIBRARY}
                       ${Boost_SYSTEM_LIBRARY} ${Boost_FILESYSTEM_LIBRARY} 
                       ${Boost_DATE_TIME_LIBRARY}
                       ${UNITTEST_LINK_LIBRARIES})
  SET_TARGET_PROPERTIES("${TEST_CAT}-${TEST_NAME}" PROPERTIES RUNTIME_OUTPUT_DIRECTORY "${TEST_OUTPUT_PATH}")
  ADD_TEST("unit-${TEST_CAT}-${TEST_NAME}" "${TEST_OUTPUT_PATH}/${TEST_CAT}-${TEST_NAME}")
ENDMACRO(ADD_OPENFLUID_UNITTEST)


MACRO(DISCOVER_OPENFLUID_UNITTESTS TEST_CAT)
  FILE(GLOB UNIT_TESTS_CPP *_TEST.cpp)
  FOREACH(CPPFILE ${UNIT_TESTS_CPP})
    GET_FILENAME_COMPONENT(CPPFILE_WE ${CPPFILE} NAME_WE)
    ADD_OPENFLUID_UNITTEST(${TEST_CAT} ${CPPFILE_WE} ${CPPFILE})
  ENDFOREACH(CPPFILE ${UNIT_TESTS_CPP})
ENDMACRO(DISCOVER_OPENFLUID_UNITTESTS TEST_CAT)


MACRO(ADD_OPENFLUID_GEOS_DEFINITIONS)
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

