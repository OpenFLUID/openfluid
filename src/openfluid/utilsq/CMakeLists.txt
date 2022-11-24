
SET(OPENFLUID_UTILSQTDEP_CPP FileDownloader.cpp FluidHubAPIClient.cpp RESTClient.cpp)

SET(OPENFLUID_UTILSQTDEP_HPP FileDownloader.hpp FluidHubAPIClient.hpp RESTClient.hpp QtHelpers.hpp)



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
                      openfluid-utils
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
   
   
