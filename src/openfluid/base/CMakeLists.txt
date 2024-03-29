

SET (OPENFLUID_BASE_CPP Environment.cpp RunContextManager.cpp ExamplesManager.cpp
                        SimulationStatus.cpp
                        SimulationLogger.cpp
                        PreferencesManager.cpp WorkspaceManager.cpp  
                        )

SET (OPENFLUID_BASE_HPP Init.hpp
                        Environment.hpp RunContextManager.hpp ExamplesManager.hpp
                        Exception.hpp OtherException.hpp FrameworkException.hpp ApplicationException.hpp ExceptionContext.hpp
                        Listener.hpp IOListener.hpp
                        SchedulingRequest.hpp
                        SimulationStatus.hpp
                        SimulationLogger.hpp
                        PreferencesManager.hpp WorkspaceManager.hpp
                        )


ADD_LIBRARY(openfluid-base SHARED ${OPENFLUID_BASE_CPP} ${OPENFLUID_BASE_HPP})

SET_TARGET_PROPERTIES(openfluid-base PROPERTIES VERSION "${OPENFLUID_VERSION_MAJOR}.${OPENFLUID_VERSION_MINOR}" 
                                        SOVERSION "${OPENFLUID_VERSION_MAJOR}.${OPENFLUID_VERSION_MINOR}.${OPENFLUID_VERSION_PATCH}"
                                        LIBRARY_OUTPUT_DIRECTORY "${OFBUILD_DIST_LIB_DIR}"
                                        RUNTIME_OUTPUT_DIRECTORY "${OFBUILD_DIST_LIB_DIR}"
                                        ARCHIVE_OUTPUT_DIRECTORY "${OFBUILD_DIST_LIB_DIR}"
                                        COMPILE_DEFINITIONS "OPENFLUID_DLL"
                                        DEFINE_SYMBOL "OPENFLUID_DLL_EXPORTS")

TARGET_LINK_LIBRARIES(openfluid-base
                      openfluid-core
                      openfluid-tools
                      )


INSTALL(TARGETS openfluid-base
        RUNTIME DESTINATION ${OFBUILD_BIN_INSTALL_PATH}
        LIBRARY DESTINATION ${OFBUILD_LIB_INSTALL_PATH}
        ARCHIVE DESTINATION ${OFBUILD_LIB_INSTALL_PATH})
   
INSTALL(FILES ${OPENFLUID_BASE_HPP} DESTINATION ${OPENFLUID_INCLUDE_INSTALL_PATH}/base)


IF(OFBUILD_ENABLE_TESTING)
  ADD_SUBDIRECTORY(tests)
ENDIF()  
