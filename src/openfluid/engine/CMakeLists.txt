
ADD_DEFINITIONS(${LibXML2_CFLAGS})

INCLUDE_DIRECTORIES(${Boost_INCLUDE_DIRS} ${LibXML2_INCLUDE_DIRS})
LINK_DIRECTORIES(${LibXML2_LIBRARY_DIRS})


FILE(GLOB OPENFLUID_ENGINE_CPP *.cpp)
FILE(GLOB OPENFLUID_ENGINE_HPP *.hpp)


ADD_LIBRARY(openfluid-engine SHARED ${OPENFLUID_ENGINE_CPP} ${OPENFLUID_ENGINE_HPP})

SET_TARGET_PROPERTIES(openfluid-engine PROPERTIES VERSION "${ENGINE_VERSION_MAJOR}.${ENGINE_VERSION_MINOR}" 
                                        SOVERSION "${ENGINE_VERSION_MAJOR}.${ENGINE_VERSION_MINOR}.${ENGINE_VERSION_PATCH}"
                                        LIBRARY_OUTPUT_DIRECTORY "${LIB_OUTPUT_PATH}"
                                        RUNTIME_OUTPUT_DIRECTORY "${LIB_OUTPUT_PATH}"
                                        ARCHIVE_OUTPUT_DIRECTORY "${LIB_OUTPUT_PATH}")

TARGET_LINK_LIBRARIES(openfluid-engine
                      openfluid-io 
                      openfluid-tools
                      openfluid-core
                      openfluid-base                     
                      ${Boost_SYSTEM_LIBRARY} ${Boost_FILESYSTEM_LIBRARY})

IF(UNIX)
  TARGET_LINK_LIBRARIES(openfluid-engine dl)
ENDIF(UNIX)



INSTALL(TARGETS openfluid-engine
        RUNTIME DESTINATION ${BIN_INSTALL_PATH}
        LIBRARY DESTINATION ${LIB_INSTALL_PATH}
        ARCHIVE DESTINATION ${LIB_INSTALL_PATH})
   

INSTALL(FILES ${OPENFLUID_ENGINE_HPP} DESTINATION ${INCLUDE_INSTALL_PATH}/io)

   