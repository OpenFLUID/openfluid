
SET(OPENFLUID_UTILSQTDEP_CPP ExternalProgram.cpp
                        CMakeProxy.cpp GitProxy.cpp GrassGISProxy.cpp
                        FileDownloader.cpp FluidHubAPIClient.cpp RESTClient.cpp
                        )

SET(OPENFLUID_UTILSQTDEP_HPP ExternalProgram.hpp 
                        ProgramProxy.hpp CMakeProxy.hpp GitProxy.hpp GrassGISProxy.hpp
                        FileDownloader.hpp FluidHubAPIClient.hpp RESTClient.hpp
                        )

SET_SOURCE_FILES_PROPERTIES(GitProxy.cpp PROPERTIES COMPILE_DEFINITIONS "OFBUILD_GITASKPASS_ENVVAR=\"${OFBUILD_GITASKPASS_ENVVAR}\";OFBUILD_GITASKUSER_ENVVAR=\"${OFBUILD_GITASKUSER_ENVVAR}\"")



ADD_LIBRARY(openfluid-utilsq SHARED ${OPENFLUID_UTILSQTDEP_CPP} ${OPENFLUID_UTILSQTDEP_HPP})

SET_TARGET_PROPERTIES(openfluid-utilsq PROPERTIES VERSION "${OPENFLUID_VERSION_MAJOR}.${OPENFLUID_VERSION_MINOR}" 
                                        SOVERSION "${OPENFLUID_VERSION_MAJOR}.${OPENFLUID_VERSION_MINOR}.${OPENFLUID_VERSION_PATCH}"
                                        LIBRARY_OUTPUT_DIRECTORY "${OFBUILD_DIST_LIB_DIR}"
                                        RUNTIME_OUTPUT_DIRECTORY "${OFBUILD_DIST_LIB_DIR}"
                                        ARCHIVE_OUTPUT_DIRECTORY "${OFBUILD_DIST_LIB_DIR}"
                                        AUTOMOC ON
                                        COMPILE_DEFINITIONS "OPENFLUID_DLL"
                                        DEFINE_SYMBOL "OPENFLUID_DLL_EXPORTS")

TARGET_LINK_LIBRARIES(openfluid-utilsq
                      openfluid-base
                      Qt5::Core
                      Qt5::Network
                      )



INSTALL(TARGETS openfluid-utilsq
        RUNTIME DESTINATION ${OFBUILD_BIN_INSTALL_PATH}
        LIBRARY DESTINATION ${OFBUILD_LIB_INSTALL_PATH}
        ARCHIVE DESTINATION ${OFBUILD_LIB_INSTALL_PATH})
   

INSTALL(FILES ${OPENFLUID_UTILSQTDEP_HPP} DESTINATION ${OPENFLUID_INCLUDE_INSTALL_PATH}/utilsq)

   
IF(OFBUILD_ENABLE_TESTING)
  ADD_SUBDIRECTORY(tests)
ENDIF()  
   
   
