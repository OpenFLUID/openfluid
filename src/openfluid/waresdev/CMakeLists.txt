


INCLUDE_DIRECTORIES(${QT_INCLUDES})


FILE(GLOB OPENFLUID_WARESDEV_CPP WareSrcManager.cpp WareSrcContainer.cpp OStreamMsgStream.cpp WareSrcFactory.cpp)

FILE(GLOB OPENFLUID_WARESDEV_HPP WareSrcManager.hpp WareSrcContainer.hpp WareSrcMsgStream.hpp OStreamMsgStream.cpp WareSrcFactory.hpp)


ADD_LIBRARY(openfluid-waresdev SHARED ${OPENFLUID_WARESDEV_CPP} ${OPENFLUID_WARESDEV_HPP})

SET_TARGET_PROPERTIES(openfluid-waresdev PROPERTIES VERSION "${VERSION_MAJOR}.${VERSION_MINOR}" 
                                        SOVERSION "${VERSION_MAJOR}.${VERSION_MINOR}.${VERSION_PATCH}"
                                        LIBRARY_OUTPUT_DIRECTORY "${LIB_OUTPUT_PATH}"
                                        RUNTIME_OUTPUT_DIRECTORY "${LIB_OUTPUT_PATH}"
                                        ARCHIVE_OUTPUT_DIRECTORY "${LIB_OUTPUT_PATH}"
                                        AUTOMOC ON
                                        COMPILE_DEFINITIONS "OPENFLUID_DLL"
                                        DEFINE_SYMBOL "OPENFLUID_DLL_EXPORTS")

TARGET_LINK_LIBRARIES(openfluid-waresdev
                      openfluid-base
                      openfluid-utils
                      ${QT_QTCORE_LIBRARIES}
                     )



INSTALL(TARGETS openfluid-waresdev
        RUNTIME DESTINATION ${BIN_INSTALL_PATH}
        LIBRARY DESTINATION ${LIB_INSTALL_PATH}
        ARCHIVE DESTINATION ${LIB_INSTALL_PATH})
   

INSTALL(FILES ${OPENFLUID_WARESDEV_HPP} DESTINATION ${INCLUDE_OPENFLUID_INSTALL_PATH}/waresdev)

IF(NOT (CMAKE_BUILD_TYPE MATCHES "Release"))
  ADD_SUBDIRECTORY(tests)
ENDIF(NOT (CMAKE_BUILD_TYPE MATCHES "Release"))  

   