INCLUDE_DIRECTORIES(${Boost_INCLUDE_DIRS} ${QT_INCLUDES} ${GDAL_INCLUDE_DIR})

FILE(GLOB OPENFLUID_TOOLS_CPP *.cpp)
FILE(GLOB OPENFLUID_TOOLS_HPP *.hpp)


ADD_LIBRARY(openfluid-tools SHARED ${OPENFLUID_TOOLS_CPP} ${OPENFLUID_TOOLS_HPP})

SET_TARGET_PROPERTIES(openfluid-tools PROPERTIES VERSION "${VERSION_MAJOR}.${VERSION_MINOR}" 
                                        SOVERSION "${VERSION_MAJOR}.${VERSION_MINOR}.${VERSION_PATCH}"
                                        LIBRARY_OUTPUT_DIRECTORY "${LIB_OUTPUT_PATH}"
                                        RUNTIME_OUTPUT_DIRECTORY "${LIB_OUTPUT_PATH}"
                                        ARCHIVE_OUTPUT_DIRECTORY "${LIB_OUTPUT_PATH}"
                                        AUTOMOC ON)

TARGET_LINK_LIBRARIES(openfluid-tools 
                      openfluid-core
                      ${Boost_SYSTEM_LIBRARY} ${Boost_FILESYSTEM_LIBRARY}
                      ${QT_QTCORE_LIBRARY}
                      ${QT_QTNETWORK_LIBRARY}
                      ${QT_QTXML_LIBRARY}
                      ${GDAL_LIBRARIES})




INSTALL(TARGETS openfluid-tools
        RUNTIME DESTINATION ${BIN_INSTALL_PATH}
        LIBRARY DESTINATION ${LIB_INSTALL_PATH}
        ARCHIVE DESTINATION ${LIB_INSTALL_PATH})
   

INSTALL(FILES ${OPENFLUID_TOOLS_HPP} DESTINATION ${INCLUDE_OPENFLUID_INSTALL_PATH}/tools)

   
IF(NOT (CMAKE_BUILD_TYPE MATCHES "Release"))
  ADD_SUBDIRECTORY(tests)
ENDIF(NOT (CMAKE_BUILD_TYPE MATCHES "Release"))  
   
   
