CMAKE_MINIMUM_REQUIRED (VERSION 2.6)

ADD_DEFINITIONS(-DOFELIB_VERSION=${ENGINE_FULL_VERSION})

INCLUDE_DIRECTORIES(${CMAKE_SOURCE_DIR}/include ${CMAKE_SOURCE_DIR}/include/core ${CMAKE_SOURCE_DIR}/include/base ${CMAKE_SOURCE_DIR}/include/tools)


# Macro for compiling a simulation function
# FUNC_NAME : the name of the simulation function (without extension)
# FUNC_DIR : the sub-directory of the simulation function
# ARGN : install targets
MACRO(ADD_FUNCTION FUNC_NAME FUNC_DIR)

  FILE(GLOB FUNC_CPP ${FUNC_DIR}/*.cpp)
  ADD_LIBRARY(${FUNC_NAME} MODULE ${FUNC_CPP})
  SET_TARGET_PROPERTIES(${FUNC_NAME} PROPERTIES PREFIX "" SUFFIX "${CMAKE_SHARED_LIBRARY_SUFFIX}mpi" LIBRARY_OUTPUT_DIRECTORY "${FUNCTIONS_OUTPUT_PATH}")
  TARGET_LINK_LIBRARIES(${FUNC_NAME} ofelib ${wxWidgets_LIBRARIES})

  IF(MSYS)
    SET_TARGET_PROPERTIES(${FUNC_NAME} PROPERTIES LINK_FLAGS "-shared")
  ENDIF(MSYS)  

  
  # processing extra aguments as install components    
  FOREACH(cpnent ${ARGN})
    INSTALL(TARGETS ${FUNC_NAME}
            DESTINATION ${FUNCTIONS_INSTALL_PATH} COMPONENT ${cpnent})
  ENDFOREACH(cpnent ${ARGN})
  
     
ENDMACRO(ADD_FUNCTION)


# Macro for generating documentation from a simulation function source code
# FUNC_NAME : the name of the simulation function (without extension)
# FUNC_DIR : the sub-directory of the simulation function
# FUNC_CPP : the .cpp file to scan to extract documentation
# ARGN : install targets
MACRO(ADD_FUNCTION_DOC FUNC_NAME FUNC_DIR FUNC_CPP)

   ADD_CUSTOM_COMMAND(
     OUTPUT "${FUNCTIONS_DOC_OUTPUT_PATH}/${FUNC_NAME}.pdf"
     DEPENDS "${FUNC_DIR}/${FUNC_CPP}"
     COMMAND "${EXECUTABLE_OUTPUT_PATH}/ofefunc2doc"
     ARGS "${CMAKE_CURRENT_SOURCE_DIR}/${FUNC_DIR}/${FUNC_CPP}" "-t${CMAKE_SOURCE_DIR}/resources/ofefunc2doc/ofefunc2doc_tpl.tex" "-o${FUNCTIONS_DOC_OUTPUT_PATH}" "-p"     
   )

  ADD_CUSTOM_TARGET(${FUNC_NAME}-doc ALL DEPENDS "${FUNCTIONS_DOC_OUTPUT_PATH}/${FUNC_NAME}.pdf")

  
  # processing extra aguments as install components    
  FOREACH(cpnent ${ARGN})
    INSTALL(FILES "${FUNCTIONS_DOC_OUTPUT_PATH}/${FUNC_NAME}.pdf" 
            DESTINATION ${FUNCTIONS_DOC_INSTALL_PATH} COMPONENT ${cpnent}
            PERMISSIONS OWNER_WRITE OWNER_READ GROUP_READ WORLD_READ)            
  ENDFOREACH(cpnent ${ARGN})

  
ENDMACRO(ADD_FUNCTION_DOC)


MACRO(ADD_FUNCTION_WITH_DOC FUNC_NAME FUNC_DIR FUNC_CPP)
  ADD_FUNCTION(${FUNC_NAME} ${FUNC_DIR} ${ARGN})
  ADD_FUNCTION_DOC(${FUNC_NAME} ${FUNC_DIR} ${FUNC_CPP} ${ARGN})
ENDMACRO(ADD_FUNCTION_WITH_DOC FUNC_NAME FUNC_DIR FUNC_CPP)

ADD_SUBDIRECTORY(standard)
ADD_SUBDIRECTORY(mhydas)

IF(EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/foundry AND ${PROCESS_FUNCTIONS_FOUNDRY})
  ADD_SUBDIRECTORY(foundry)
ENDIF(EXISTS foundry)  




