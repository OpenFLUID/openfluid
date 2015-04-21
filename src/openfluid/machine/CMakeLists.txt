
INCLUDE_DIRECTORIES(${Boost_INCLUDE_DIRS})


FILE(GLOB OPENFLUID_MACHINE_CPP *.cpp)
FILE(GLOB OPENFLUID_MACHINE_HPP *.hpp)


ADD_LIBRARY(openfluid-machine SHARED ${OPENFLUID_MACHINE_CPP} ${OPENFLUID_MACHINE_HPP})

SET_TARGET_PROPERTIES(openfluid-machine PROPERTIES VERSION "${VERSION_MAJOR}.${VERSION_MINOR}" 
                                        SOVERSION "${VERSION_MAJOR}.${VERSION_MINOR}.${VERSION_PATCH}"
                                        LIBRARY_OUTPUT_DIRECTORY "${LIB_OUTPUT_PATH}"
                                        RUNTIME_OUTPUT_DIRECTORY "${LIB_OUTPUT_PATH}"
                                        ARCHIVE_OUTPUT_DIRECTORY "${LIB_OUTPUT_PATH}"
                                        COMPILE_DEFINITIONS "OPENFLUID_DLL"
                                        DEFINE_SYMBOL "OPENFLUID_DLL_EXPORTS")

TARGET_LINK_LIBRARIES(openfluid-machine
                      openfluid-tools
                      openfluid-core
                      openfluid-base
                      openfluid-ware
                      openfluid-fluidx
                      ${QT_QTCORE_LIBRARY})


INSTALL(TARGETS openfluid-machine
        RUNTIME DESTINATION ${BIN_INSTALL_PATH}
        LIBRARY DESTINATION ${LIB_INSTALL_PATH}
        ARCHIVE DESTINATION ${LIB_INSTALL_PATH})
   

INSTALL(FILES ${OPENFLUID_MACHINE_HPP} DESTINATION ${INCLUDE_OPENFLUID_INSTALL_PATH}/machine)


IF(NOT (CMAKE_BUILD_TYPE MATCHES "Release"))
  ADD_SUBDIRECTORY(tests)
ENDIF(NOT (CMAKE_BUILD_TYPE MATCHES "Release"))  


   