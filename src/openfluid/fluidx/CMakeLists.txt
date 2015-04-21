INCLUDE_DIRECTORIES(${Boost_INCLUDE_DIRS})

FILE(GLOB OPENFLUID_FLUIDX_CPP *.cpp)
FILE(GLOB OPENFLUID_FLUIDX_HPP *.hpp)


ADD_LIBRARY(openfluid-fluidx SHARED ${OPENFLUID_FLUIDX_CPP} ${OPENFLUID_FLUIDX_HPP})

SET_TARGET_PROPERTIES(openfluid-fluidx PROPERTIES VERSION "${VERSION_MAJOR}.${VERSION_MINOR}" 
                                        SOVERSION "${VERSION_MAJOR}.${VERSION_MINOR}.${VERSION_PATCH}"
                                        LIBRARY_OUTPUT_DIRECTORY "${LIB_OUTPUT_PATH}"
                                        RUNTIME_OUTPUT_DIRECTORY "${LIB_OUTPUT_PATH}"
                                        ARCHIVE_OUTPUT_DIRECTORY "${LIB_OUTPUT_PATH}"
                                        COMPILE_DEFINITIONS "OPENFLUID_DLL"
                                        DEFINE_SYMBOL "OPENFLUID_DLL_EXPORTS")

TARGET_LINK_LIBRARIES(openfluid-fluidx
                      openfluid-base
                      openfluid-core
                      ${QT_QTXML_LIBRARY})

INSTALL(TARGETS openfluid-fluidx
        RUNTIME DESTINATION ${BIN_INSTALL_PATH}
        LIBRARY DESTINATION ${LIB_INSTALL_PATH}
        ARCHIVE DESTINATION ${LIB_INSTALL_PATH})
   

INSTALL(FILES ${OPENFLUID_FLUIDX_HPP} DESTINATION ${INCLUDE_OPENFLUID_INSTALL_PATH}/fluidx)

IF(NOT (CMAKE_BUILD_TYPE MATCHES "Release"))
  ADD_SUBDIRECTORY(tests)
ENDIF(NOT (CMAKE_BUILD_TYPE MATCHES "Release"))  

   