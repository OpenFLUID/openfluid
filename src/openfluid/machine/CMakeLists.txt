
INCLUDE_DIRECTORIES(${Boost_INCLUDE_DIRS})


FILE(GLOB OPENFLUID_MACHINE_CPP *.cpp)
FILE(GLOB OPENFLUID_MACHINE_HPP *.hpp)


ADD_LIBRARY(openfluid-machine SHARED ${OPENFLUID_MACHINE_CPP} ${OPENFLUID_MACHINE_HPP})

SET_TARGET_PROPERTIES(openfluid-machine PROPERTIES VERSION "${OPENFLUID_VERSION_MAJOR}.${OPENFLUID_VERSION_MINOR}" 
                                        SOVERSION "${OPENFLUID_VERSION_MAJOR}.${OPENFLUID_VERSION_MINOR}.${OPENFLUID_VERSION_PATCH}"
                                        LIBRARY_OUTPUT_DIRECTORY "${OFBUILD_DIST_LIB_DIR}"
                                        RUNTIME_OUTPUT_DIRECTORY "${OFBUILD_DIST_LIB_DIR}"
                                        ARCHIVE_OUTPUT_DIRECTORY "${OFBUILD_DIST_LIB_DIR}"
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
        RUNTIME DESTINATION ${OFBUILD_BIN_INSTALL_PATH}
        LIBRARY DESTINATION ${OFBUILD_LIB_INSTALL_PATH}
        ARCHIVE DESTINATION ${OFBUILD_LIB_INSTALL_PATH})
   
INSTALL(FILES ${OPENFLUID_MACHINE_HPP} DESTINATION ${OPENFLUID_INCLUDE_INSTALL_PATH}/machine)


IF(OFBUILD_ENABLE_TESTING)
  ADD_SUBDIRECTORY(tests)
ENDIF()  


   