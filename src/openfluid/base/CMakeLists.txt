


FILE(GLOB OPENFLUID_BASE_CPP *.cpp)
FILE(GLOB OPENFLUID_BASE_HPP *.hpp)


ADD_LIBRARY(openfluid-base SHARED ${OPENFLUID_BASE_CPP} ${OPENFLUID_BASE_HPP})

SET_TARGET_PROPERTIES(openfluid-base PROPERTIES VERSION "${VERSION_MAJOR}.${VERSION_MINOR}" 
                                        SOVERSION "${VERSION_MAJOR}.${VERSION_MINOR}.${VERSION_PATCH}"
                                        LIBRARY_OUTPUT_DIRECTORY "${LIB_OUTPUT_PATH}"
                                        RUNTIME_OUTPUT_DIRECTORY "${LIB_OUTPUT_PATH}"
                                        ARCHIVE_OUTPUT_DIRECTORY "${LIB_OUTPUT_PATH}"
                                        COMPILE_DEFINITIONS "OPENFLUID_DLL"
                                        DEFINE_SYMBOL "OPENFLUID_DLL_EXPORTS")

TARGET_LINK_LIBRARIES(openfluid-base
                      openfluid-core
                      openfluid-tools
                      )



INSTALL(TARGETS openfluid-base
        RUNTIME DESTINATION ${BIN_INSTALL_PATH}
        LIBRARY DESTINATION ${LIB_INSTALL_PATH}
        ARCHIVE DESTINATION ${LIB_INSTALL_PATH})
   

INSTALL(FILES ${OPENFLUID_BASE_HPP} DESTINATION ${INCLUDE_OPENFLUID_INSTALL_PATH}/base)

IF(NOT (CMAKE_BUILD_TYPE MATCHES "Release"))
  ADD_SUBDIRECTORY(tests)
ENDIF(NOT (CMAKE_BUILD_TYPE MATCHES "Release"))  

   