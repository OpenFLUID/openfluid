CMAKE_MINIMUM_REQUIRED (VERSION 2.6)




SET(OFELIB_CORE_CPP 
    core/HydroObject.cpp core/SurfaceUnit.cpp core/GroundwaterUnit.cpp core/ReachSegment.cpp
    core/DistributedEvent.cpp core/EventCollection.cpp
    core/DateTime.cpp
    core/CoreRepository.cpp core/SpatialRepository.cpp
   )

SET(OFELIB_BASE_CPP 
    base/ExecMsgs.cpp
    base/FunctionEnv.cpp
    base/PlugFunction.cpp
    base/SimStatus.cpp
   )

SET(OFELIB_TOOLS_CPP 
    tools/ColTextParser.cpp tools/DataSrcFile.cpp
    tools/DTSerie.cpp tools/DistribInterp.cpp
    tools/Tools.cpp
    tools/tinystr.cpp tools/tinyxml.cpp tools/tinyxmlerror.cpp tools/tinyxmlparser.cpp
)


ADD_DEFINITIONS(-DBUILDINGDLL)

INCLUDE_DIRECTORIES(${CMAKE_SOURCE_DIR}/include ${CMAKE_SOURCE_DIR}/include/core ${CMAKE_SOURCE_DIR}/include/base ${CMAKE_SOURCE_DIR}/include/tools)

ADD_LIBRARY(ofelib SHARED ${OFELIB_CORE_CPP} ${OFELIB_BASE_CPP} ${OFELIB_TOOLS_CPP})

SET_TARGET_PROPERTIES(ofelib PROPERTIES VERSION "${ENGINE_VERSION_MAJOR}.${ENGINE_VERSION_MINOR}" SOVERSION "${ENGINE_VERSION_MAJOR}.${ENGINE_VERSION_MINOR}.${ENGINE_VERSION_PATCH}")

TARGET_LINK_LIBRARIES(ofelib ${wxWidgets_LIBRARIES})



INSTALL(TARGETS ofelib 
        LIBRARY DESTINATION ${LIB_INSTALL_PATH} COMPONENT ofelib)



################### ofelib-dev include files ###################


ADD_CUSTOM_COMMAND(TARGET ofelib POST_BUILD
                   COMMAND ${CMAKE_COMMAND} -E make_directory ${OFELIBDEV_INCLUDE_OUTPUT_PATH}
                   COMMAND ${CMAKE_COMMAND} -E copy_directory ${OFELIBDEV_INCLUDE_DIR}/ ${OFELIBDEV_INCLUDE_OUTPUT_PATH})


INSTALL(DIRECTORY "${CMAKE_SOURCE_DIR}/include/" 
        DESTINATION "${OFELIBDEV_INCLUDE_INSTALL_PATH}" COMPONENT ofelibdev)


################### ofelib-dev ofefunc2doc tool ###################


CONFIGURE_FILE(${PROJECT_SOURCE_DIR}/resources/ofefunc2doc/ofefunc2doc.py "${EXECUTABLE_OUTPUT_PATH}/ofefunc2doc" COPYONLY)

INSTALL(FILES "${EXECUTABLE_OUTPUT_PATH}/ofefunc2doc" 
        DESTINATION ${BIN_INSTALL_PATH} COMPONENT ofelibdev
        PERMISSIONS OWNER_EXECUTE OWNER_WRITE OWNER_READ GROUP_EXECUTE GROUP_READ WORLD_EXECUTE WORLD_READ)

INSTALL(FILES "${CMAKE_SOURCE_DIR}/resources/ofefunc2doc/ofefunc2doc_tpl.tex" 
        DESTINATION ${OFEFUNC2DOC_TPL_INSTALL_PATH} COMPONENT ofelibdev)


################### ofelib-dev ofelib-config tool ###################

CONFIGURE_FILE(ofelib-config.in "${EXECUTABLE_OUTPUT_PATH}/ofelib-config" @ONLY)

INSTALL(FILES "${EXECUTABLE_OUTPUT_PATH}/ofelib-config" 
        DESTINATION ${BIN_INSTALL_PATH} COMPONENT ofelibdev
        PERMISSIONS OWNER_EXECUTE OWNER_WRITE OWNER_READ GROUP_EXECUTE GROUP_READ WORLD_EXECUTE WORLD_READ)


################### ofelib-dev doxygen documentation ###################

IF(DOXYGEN)
  
  CONFIGURE_FILE(${PROJECT_SOURCE_DIR}/Doxyfile.in ${PROJECT_BINARY_DIR}/Doxyfile)

  FILE(MAKE_DIRECTORY ${DOXYGEN_OUTPUT_PATH})

  # TODO A voir autrement    
  CONFIGURE_FILE(${DOXYGEN_RESOURCES_DIR}/openfluid.png ${DOXYGEN_OUTPUT_PATH}/html/openfluid.png COPYONLY)
  CONFIGURE_FILE(${DOXYGEN_RESOURCES_DIR}/API_stack.png ${DOXYGEN_OUTPUT_PATH}/html/API_stack.png COPYONLY)
  
  ADD_CUSTOM_COMMAND(
    OUTPUT "${DOXYGEN_OUTPUT_PATH}/html/index.html"
    DEPENDS "${PROJECT_BINARY_DIR}/Doxyfile"
    COMMAND "${DOXYGEN}"
    ARGS "${PROJECT_BINARY_DIR}/Doxyfile")

  ADD_CUSTOM_TARGET(ofelib-doc ALL DEPENDS "${PROJECT_BINARY_DIR}/Doxyfile"
    "${DOXYGEN_OUTPUT_PATH}/html/index.html" VERBATIM)


  INSTALL(DIRECTORY "${DOXYGEN_OUTPUT_PATH}/"
          DESTINATION "${OFELIBDEV_DOC_INSTALL_PATH}" COMPONENT ofelibdev)

ENDIF(DOXYGEN)


