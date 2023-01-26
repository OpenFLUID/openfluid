
INCLUDE_DIRECTORIES(${GDAL_INCLUDE_DIR} ${CURL_INCLUDE_DIRS})

# Hack for boost::process on Windows/MinGW only
IF(MINGW)
  FIND_PACKAGE(Boost REQUIRED filesystem)
  IF(Boost_VERSION GREATER 107000)
    ADD_DEFINITIONS(-DBOOST_USE_WINDOWS_H -DWIN32_LEAN_AND_MEAN)
  ENDIF()
ENDIF()


SET(OPENFLUID_UTILS_CPP GDALHelpers.cpp
                        Process.cpp
                        HTTPClient.cpp FluidHubAPIClient.cpp
                        ExternalProgram.cpp 
                        CMakeProxy.cpp GitProxy.cpp GrassGISProxy.cpp
                        )

SET(OPENFLUID_UTILS_HPP Binding.hpp BindingAbstractOutErr.hpp BindingVerboseMachineListener.hpp
                        GDALHelpers.hpp GDALCompatibility.hpp
                        KillableSingleton.hpp MeyerSingleton.hpp SingletonMacros.hpp
                        CppLangHelpers.hpp
                        Process.hpp
                        HTTPClient.hpp FluidHubAPIClient.hpp
                        ExternalProgram.hpp
                        ProgramProxy.hpp CMakeProxy.hpp GitProxy.hpp GrassGISProxy.hpp
                        CommandLineParser.hpp
                        )


ADD_LIBRARY(openfluid-utils SHARED ${OPENFLUID_UTILS_CPP} ${OPENFLUID_UTILS_HPP})

SET_TARGET_PROPERTIES(openfluid-utils PROPERTIES VERSION "${OPENFLUID_VERSION_MAJOR}.${OPENFLUID_VERSION_MINOR}" 
                                        SOVERSION "${OPENFLUID_VERSION_MAJOR}.${OPENFLUID_VERSION_MINOR}.${OPENFLUID_VERSION_PATCH}"
                                        LIBRARY_OUTPUT_DIRECTORY "${OFBUILD_DIST_LIB_DIR}"
                                        RUNTIME_OUTPUT_DIRECTORY "${OFBUILD_DIST_LIB_DIR}"
                                        ARCHIVE_OUTPUT_DIRECTORY "${OFBUILD_DIST_LIB_DIR}"
                                        COMPILE_DEFINITIONS "OPENFLUID_DLL"
                                        DEFINE_SYMBOL "OPENFLUID_DLL_EXPORTS")

TARGET_LINK_LIBRARIES(openfluid-utils 
                      openfluid-base
                      openfluid-tools
                      ${GDAL_LIBRARIES}
                      ${CURL_LIBRARIES}
                      Boost::boost 
                      $<$<BOOL:${MINGW}>:Boost::filesystem> $<$<BOOL:${MINGW}>:ws2_32> # MinGW hack
                      )



INSTALL(TARGETS openfluid-utils
        RUNTIME DESTINATION ${OFBUILD_BIN_INSTALL_PATH}
        LIBRARY DESTINATION ${OFBUILD_LIB_INSTALL_PATH}
        ARCHIVE DESTINATION ${OFBUILD_LIB_INSTALL_PATH})

INSTALL(FILES ${OPENFLUID_UTILS_HPP} DESTINATION ${OPENFLUID_INCLUDE_INSTALL_PATH}/utils)

   
IF(OFBUILD_ENABLE_TESTING)
  ADD_SUBDIRECTORY(tests)
ENDIF()  
   
   
