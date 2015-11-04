
INCLUDE_DIRECTORIES(${Boost_INCLUDE_DIRS})


FILE(GLOB OPENFLUID_WARE_CPP *.cpp)
FILE(GLOB OPENFLUID_WARE_HPP *.hpp)


ADD_LIBRARY(openfluid-ware SHARED ${OPENFLUID_WARE_CPP} ${OPENFLUID_WARE_HPP})

SET_TARGET_PROPERTIES(openfluid-ware PROPERTIES VERSION "${OPENFLUID_VERSION_MAJOR}.${OPENFLUID_VERSION_MINOR}" 
                                        SOVERSION "${OPENFLUID_VERSION_MAJOR}.${OPENFLUID_VERSION_MINOR}.${OPENFLUID_VERSION_PATCH}"
                                        LIBRARY_OUTPUT_DIRECTORY "${OFBUILD_DIST_LIB_DIR}"
                                        RUNTIME_OUTPUT_DIRECTORY "${OFBUILD_DIST_LIB_DIR}"
                                        ARCHIVE_OUTPUT_DIRECTORY "${OFBUILD_DIST_LIB_DIR}"
                                        COMPILE_DEFINITIONS "OPENFLUID_DLL"
                                        DEFINE_SYMBOL "OPENFLUID_DLL_EXPORTS")

TARGET_LINK_LIBRARIES(openfluid-ware
                      openfluid-core
                      openfluid-base
                      openfluid-fluidx
                      openfluid-tools)


INSTALL(TARGETS openfluid-ware
        RUNTIME DESTINATION ${OFBUILD_BIN_INSTALL_PATH}
        LIBRARY DESTINATION ${OFBUILD_LIB_INSTALL_PATH}
        ARCHIVE DESTINATION ${OFBUILD_LIB_INSTALL_PATH})
   
INSTALL(FILES ${OPENFLUID_WARE_HPP} DESTINATION ${OPENFLUID_INCLUDE_INSTALL_PATH}/ware)


IF(OFBUILD_ENABLE_TESTING)
  ADD_SUBDIRECTORY(tests)
ENDIF()  


   