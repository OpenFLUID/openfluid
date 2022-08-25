

SET(OPENFLUID_WARESDEV_CPP WareSrcEnquirer.cpp 
                           WareSrcContainer.cpp 
                           WareSrcFactory.cpp
                           SimulatorSignatureSerializer.cpp ObserverSignatureSerializer.cpp BuilderextSignatureSerializer.cpp
                           SimulatorSignatureXMLReader.cpp
                           GhostsHelpers.cpp
                           )

SET(OPENFLUID_WARESDEV_HPP WareSrcEnquirer.hpp 
                           WareSrcContainer.hpp WareSrcContainerOps.hpp 
                           WareSrcFactory.hpp 
                           WareBuildOptions.hpp
                           WareSignatureSerializer.hpp
                           SimulatorSignatureSerializer.hpp ObserverSignatureSerializer.hpp BuilderextSignatureSerializer.hpp
                           SimulatorSignatureXMLReader.hpp
                           GhostsHelpers.hpp
                           )


ADD_LIBRARY(openfluid-waresdev SHARED ${OPENFLUID_WARESDEV_CPP} ${OPENFLUID_WARESDEV_HPP})

SET_TARGET_PROPERTIES(openfluid-waresdev PROPERTIES VERSION "${OPENFLUID_VERSION_MAJOR}.${OPENFLUID_VERSION_MINOR}" 
                                        SOVERSION "${OPENFLUID_VERSION_MAJOR}.${OPENFLUID_VERSION_MINOR}.${OPENFLUID_VERSION_PATCH}"
                                        LIBRARY_OUTPUT_DIRECTORY "${OFBUILD_DIST_LIB_DIR}"
                                        RUNTIME_OUTPUT_DIRECTORY "${OFBUILD_DIST_LIB_DIR}"
                                        ARCHIVE_OUTPUT_DIRECTORY "${OFBUILD_DIST_LIB_DIR}"
                                        COMPILE_DEFINITIONS "OPENFLUID_DLL"
                                        DEFINE_SYMBOL "OPENFLUID_DLL_EXPORTS")

TARGET_LINK_LIBRARIES(openfluid-waresdev
                      openfluid-base
                      openfluid-core
                      openfluid-ware
                      openfluid-thirdparty-tinyxml2
                     )


INSTALL(TARGETS openfluid-waresdev
        RUNTIME DESTINATION ${OFBUILD_BIN_INSTALL_PATH}
        LIBRARY DESTINATION ${OFBUILD_LIB_INSTALL_PATH}
        ARCHIVE DESTINATION ${OFBUILD_LIB_INSTALL_PATH})
   
INSTALL(FILES ${OPENFLUID_WARESDEV_HPP} DESTINATION ${OPENFLUID_INCLUDE_INSTALL_PATH}/waresdev)


IF(OFBUILD_ENABLE_TESTING)
  ADD_SUBDIRECTORY(tests)
ENDIF()  
