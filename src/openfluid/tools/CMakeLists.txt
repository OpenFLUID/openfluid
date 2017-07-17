INCLUDE_DIRECTORIES(${Boost_INCLUDE_DIRS})

FILE(GLOB OPENFLUID_TOOLS_CPP *.cpp)
FILE(GLOB OPENFLUID_TOOLS_HPP *.hpp)


ADD_LIBRARY(openfluid-tools SHARED ${OPENFLUID_TOOLS_CPP} ${OPENFLUID_TOOLS_HPP})

SET_TARGET_PROPERTIES(openfluid-tools PROPERTIES VERSION "${OPENFLUID_VERSION_MAJOR}.${OPENFLUID_VERSION_MINOR}" 
                                        SOVERSION "${OPENFLUID_VERSION_MAJOR}.${OPENFLUID_VERSION_MINOR}.${OPENFLUID_VERSION_PATCH}"
                                        LIBRARY_OUTPUT_DIRECTORY "${OFBUILD_DIST_LIB_DIR}"
                                        RUNTIME_OUTPUT_DIRECTORY "${OFBUILD_DIST_LIB_DIR}"
                                        ARCHIVE_OUTPUT_DIRECTORY "${OFBUILD_DIST_LIB_DIR}"
                                        AUTOMOC ON
                                        COMPILE_DEFINITIONS "OPENFLUID_DLL"
                                        DEFINE_SYMBOL "OPENFLUID_DLL_EXPORTS")

TARGET_LINK_LIBRARIES(openfluid-tools 
                      openfluid-core
                      Qt5::Core
                      Qt5::Xml
                     )


INSTALL(TARGETS openfluid-tools
        RUNTIME DESTINATION ${OFBUILD_BIN_INSTALL_PATH}
        LIBRARY DESTINATION ${OFBUILD_LIB_INSTALL_PATH}
        ARCHIVE DESTINATION ${OFBUILD_LIB_INSTALL_PATH})
   
INSTALL(FILES ${OPENFLUID_TOOLS_HPP} DESTINATION ${OPENFLUID_INCLUDE_INSTALL_PATH}/tools)

   
IF(OFBUILD_ENABLE_TESTING)
  ADD_SUBDIRECTORY(tests)
ENDIF()  
   
   
