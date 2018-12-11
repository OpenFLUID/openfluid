INCLUDE_DIRECTORIES(${GDAL_INCLUDE_DIR} ${RapidJSON_INCLUDE_DIR})


SET(OPENFLUID_UTILS_CPP ExternalProgram.cpp
                        CMakeProxy.cpp GitProxy.cpp GrassGISProxy.cpp
                        FileDownloader.cpp FluidHubAPIClient.cpp RESTClient.cpp
                        GDALHelpers.cpp
                        )

SET(OPENFLUID_UTILS_HPP ExternalProgram.hpp 
                        Binding.hpp BindingAbstractOutErr.hpp BindingVerboseMachineListener.hpp
                        ProgramProxy.hpp CMakeProxy.hpp GitProxy.hpp GrassGISProxy.hpp
                        FileDownloader.hpp FluidHubAPIClient.hpp RESTClient.hpp
                        KillableSingleton.hpp MeyerSingleton.hpp SingletonMacros.hpp
                        CommandLineParser.hpp
                        GDALHelpers.hpp GDALCompatibility.hpp
                        )


ADD_LIBRARY(openfluid-utils SHARED ${OPENFLUID_UTILS_CPP} ${OPENFLUID_UTILS_HPP})

SET_SOURCE_FILES_PROPERTIES(GitProxy.cpp PROPERTIES COMPILE_DEFINITIONS OFBUILD_GITASKPASS_ENVVAR="${OFBUILD_GITASKPASS_ENVVAR}")

SET_TARGET_PROPERTIES(openfluid-utils PROPERTIES VERSION "${OPENFLUID_VERSION_MAJOR}.${OPENFLUID_VERSION_MINOR}" 
                                        SOVERSION "${OPENFLUID_VERSION_MAJOR}.${OPENFLUID_VERSION_MINOR}.${OPENFLUID_VERSION_PATCH}"
                                        LIBRARY_OUTPUT_DIRECTORY "${OFBUILD_DIST_LIB_DIR}"
                                        RUNTIME_OUTPUT_DIRECTORY "${OFBUILD_DIST_LIB_DIR}"
                                        ARCHIVE_OUTPUT_DIRECTORY "${OFBUILD_DIST_LIB_DIR}"
                                        AUTOMOC ON
                                        COMPILE_DEFINITIONS "OPENFLUID_DLL"
                                        DEFINE_SYMBOL "OPENFLUID_DLL_EXPORTS")

TARGET_LINK_LIBRARIES(openfluid-utils
                      openfluid-base
                      Qt5::Core
                      Qt5::Network
                      ${GDAL_LIBRARIES})




INSTALL(TARGETS openfluid-utils
        RUNTIME DESTINATION ${OFBUILD_BIN_INSTALL_PATH}
        LIBRARY DESTINATION ${OFBUILD_LIB_INSTALL_PATH}
        ARCHIVE DESTINATION ${OFBUILD_LIB_INSTALL_PATH})
   

INSTALL(FILES ${OPENFLUID_UTILS_HPP} DESTINATION ${OPENFLUID_INCLUDE_INSTALL_PATH}/utils)

   
IF(OFBUILD_ENABLE_TESTING)
  ADD_SUBDIRECTORY(tests)
ENDIF()  
   
   
