CMAKE_MINIMUM_REQUIRED (VERSION 2.6)




SET(OFELIB_CORE_CPP 
    core/Unit.cpp core/UnitsColl.cpp
    core/Event.cpp core/EventsColl.cpp
    core/DateTime.cpp
    core/CoreRepository.cpp
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
    tools/SwissTools.cpp
    tools/tinystr.cpp tools/tinyxml.cpp tools/tinyxmlerror.cpp tools/tinyxmlparser.cpp    
)


ADD_DEFINITIONS(-DBUILDINGDLL)

INCLUDE_DIRECTORIES(${Boost_INCLUDE_DIRS})
INCLUDE_DIRECTORIES(${CMAKE_SOURCE_DIR}/include ${CMAKE_SOURCE_DIR}/include/core ${CMAKE_SOURCE_DIR}/include/base ${CMAKE_SOURCE_DIR}/include/tools)

ADD_LIBRARY(ofelib SHARED ${OFELIB_CORE_CPP} ${OFELIB_BASE_CPP} ${OFELIB_TOOLS_CPP})

SET_TARGET_PROPERTIES(ofelib PROPERTIES VERSION "${ENGINE_VERSION_MAJOR}.${ENGINE_VERSION_MINOR}" 
                                        SOVERSION "${ENGINE_VERSION_MAJOR}.${ENGINE_VERSION_MINOR}.${ENGINE_VERSION_PATCH}"
                                        LIBRARY_OUTPUT_DIRECTORY "${LIB_OUTPUT_PATH}"
                                        RUNTIME_OUTPUT_DIRECTORY "${LIB_OUTPUT_PATH}")

TARGET_LINK_LIBRARIES(ofelib ${Boost_SYSTEM_LIBRARY} ${Boost_FILESYSTEM_LIBRARY})



INSTALL(TARGETS ofelib 
        RUNTIME DESTINATION ${BIN_INSTALL_PATH}
        LIBRARY DESTINATION ${LIB_INSTALL_PATH})




################### ofelib-dev include files ###################


ADD_CUSTOM_COMMAND(TARGET ofelib POST_BUILD
                   COMMAND ${CMAKE_COMMAND} -E make_directory ${OFELIBDEV_INCLUDE_OUTPUT_PATH}
                   COMMAND ${CMAKE_COMMAND} -E copy_directory ${OFELIBDEV_INCLUDE_DIR}/ ${OFELIBDEV_INCLUDE_OUTPUT_PATH})


INSTALL(DIRECTORY "${CMAKE_SOURCE_DIR}/include/" 
        DESTINATION "${OFELIBDEV_INCLUDE_INSTALL_PATH}" PATTERN ".svn" EXCLUDE)




################### ofelib-dev ofelib-config tool ###################

CONFIGURE_FILE(ofelib-config.in "${BIN_OUTPUT_PATH}/ofelib-config" @ONLY)
INSTALL(FILES "${BIN_OUTPUT_PATH}/ofelib-config" 
        DESTINATION ${BIN_INSTALL_PATH}
        PERMISSIONS OWNER_EXECUTE OWNER_WRITE OWNER_READ GROUP_EXECUTE GROUP_READ WORLD_EXECUTE WORLD_READ)

CONFIGURE_FILE(ofelib.pc.in "${PKGCONFIG_OUTPUT_PATH}/ofelib.pc" @ONLY)
INSTALL(FILES "${PKGCONFIG_OUTPUT_PATH}/ofelib.pc" 
        DESTINATION ${PKGCONFIG_INSTALL_PATH}
        PERMISSIONS OWNER_EXECUTE OWNER_WRITE OWNER_READ GROUP_EXECUTE GROUP_READ WORLD_EXECUTE WORLD_READ)




