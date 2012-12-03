
INCLUDE_DIRECTORIES(${Boost_INCLUDE_DIRS})


FILE(GLOB OPENFLUID_WARE_CPP *.cpp)
FILE(GLOB OPENFLUID_WARE_HPP *.hpp)


ADD_LIBRARY(openfluid-ware SHARED ${OPENFLUID_WARE_CPP} ${OPENFLUID_WARE_HPP})

SET_TARGET_PROPERTIES(openfluid-ware PROPERTIES VERSION "${VERSION_MAJOR}.${VERSION_MINOR}" 
                                        SOVERSION "${VERSION_MAJOR}.${VERSION_MINOR}.${VERSION_PATCH}"
                                        LIBRARY_OUTPUT_DIRECTORY "${LIB_OUTPUT_PATH}"
                                        RUNTIME_OUTPUT_DIRECTORY "${LIB_OUTPUT_PATH}"
                                        ARCHIVE_OUTPUT_DIRECTORY "${LIB_OUTPUT_PATH}")

TARGET_LINK_LIBRARIES(openfluid-ware
                      openfluid-core
                      openfluid-base                     
                      ${Boost_SYSTEM_LIBRARY})


INSTALL(TARGETS openfluid-ware
        RUNTIME DESTINATION ${BIN_INSTALL_PATH}
        LIBRARY DESTINATION ${LIB_INSTALL_PATH}
        ARCHIVE DESTINATION ${LIB_INSTALL_PATH})
   

INSTALL(FILES ${OPENFLUID_ware_HPP} DESTINATION ${INCLUDE_OPENFLUID_INSTALL_PATH}/ware)


IF(NOT (CMAKE_BUILD_TYPE MATCHES "Release"))
  ADD_SUBDIRECTORY(tests)
ENDIF(NOT (CMAKE_BUILD_TYPE MATCHES "Release"))  


   