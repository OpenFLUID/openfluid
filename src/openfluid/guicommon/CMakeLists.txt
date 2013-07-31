


INCLUDE_DIRECTORIES(${QT_INCLUDES})


FILE(GLOB OPENFLUID_GUICOMMON_CPP OpenProjectDialog.cpp)
FILE(GLOB OPENFLUID_GUICOMMON_HPP OpenProjectDialog.hpp)


ADD_LIBRARY(openfluid-guicommon SHARED ${OPENFLUID_GUICOMMON_CPP} ${OPENFLUID_GUICOMMON_HPP})

SET_TARGET_PROPERTIES(openfluid-guicommon PROPERTIES VERSION "${VERSION_MAJOR}.${VERSION_MINOR}" 
                                        SOVERSION "${VERSION_MAJOR}.${VERSION_MINOR}.${VERSION_PATCH}"
                                        LIBRARY_OUTPUT_DIRECTORY "${LIB_OUTPUT_PATH}"
                                        RUNTIME_OUTPUT_DIRECTORY "${LIB_OUTPUT_PATH}"
                                        ARCHIVE_OUTPUT_DIRECTORY "${LIB_OUTPUT_PATH}")

TARGET_LINK_LIBRARIES(openfluid-guicommon
                      openfluid-base
                      openfluid-core
                      openfluid-tools
                      openfluid-machine
                      openfluid-market
                      openfluid-fluidx
                      ${QT_LIBRARIES}  
                     )



INSTALL(TARGETS openfluid-guicommon
        RUNTIME DESTINATION ${BIN_INSTALL_PATH}
        LIBRARY DESTINATION ${LIB_INSTALL_PATH}
        ARCHIVE DESTINATION ${LIB_INSTALL_PATH})
   

INSTALL(FILES ${OPENFLUID_GUICOMMON_HPP} DESTINATION ${INCLUDE_OPENFLUID_INSTALL_PATH}/guicommon)


IF(NOT (CMAKE_BUILD_TYPE MATCHES "Release"))
  ADD_SUBDIRECTORY(tests)
ENDIF(NOT (CMAKE_BUILD_TYPE MATCHES "Release"))  


   