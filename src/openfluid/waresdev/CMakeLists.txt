

SET(OPENFLUID_WARESDEV_CPP WareSrcEnquirer.cpp 
                           WareSrcContainer.cpp 
                           OStreamMsgStream.cpp
                           WareSrcFactory.cpp 
                           WareSrcMsgParser.cpp
                           WaresDevPackage.cpp 
                           FluidHubWaresImportWorker.cpp
                           CompletionProvider.cpp
                           )

SET(OPENFLUID_WARESDEV_HPP WareSrcEnquirer.hpp 
                           WareSrcContainer.hpp WareSrcContainerOps.hpp 
                           WareSrcMsgStream.hpp 
                           OStreamMsgStream.cpp
                           WareSrcFactory.hpp 
                           WareSrcMsgParser.hpp 
                           WaresDevPackage.hpp 
                           FluidHubWaresImportWorker.hpp
                           CompletionProvider.hpp
                           WareBuildOptions.hpp
                           )


ADD_LIBRARY(openfluid-waresdev SHARED ${OPENFLUID_WARESDEV_CPP} ${OPENFLUID_WARESDEV_HPP})

SET_TARGET_PROPERTIES(openfluid-waresdev PROPERTIES VERSION "${OPENFLUID_VERSION_MAJOR}.${OPENFLUID_VERSION_MINOR}" 
                                        SOVERSION "${OPENFLUID_VERSION_MAJOR}.${OPENFLUID_VERSION_MINOR}.${OPENFLUID_VERSION_PATCH}"
                                        LIBRARY_OUTPUT_DIRECTORY "${OFBUILD_DIST_LIB_DIR}"
                                        RUNTIME_OUTPUT_DIRECTORY "${OFBUILD_DIST_LIB_DIR}"
                                        ARCHIVE_OUTPUT_DIRECTORY "${OFBUILD_DIST_LIB_DIR}"
                                        AUTOMOC ON
                                        COMPILE_DEFINITIONS "OPENFLUID_DLL"
                                        DEFINE_SYMBOL "OPENFLUID_DLL_EXPORTS")

TARGET_LINK_LIBRARIES(openfluid-waresdev
                      openfluid-base
                      openfluid-utils
                      Qt5::Core
                     )


INSTALL(TARGETS openfluid-waresdev
        RUNTIME DESTINATION ${OFBUILD_BIN_INSTALL_PATH}
        LIBRARY DESTINATION ${OFBUILD_LIB_INSTALL_PATH}
        ARCHIVE DESTINATION ${OFBUILD_LIB_INSTALL_PATH})
   
INSTALL(FILES ${OPENFLUID_WARESDEV_HPP} DESTINATION ${OPENFLUID_INCLUDE_INSTALL_PATH}/waresdev)


IF(OFBUILD_ENABLE_TESTING)
  ADD_SUBDIRECTORY(tests)
ENDIF()  
