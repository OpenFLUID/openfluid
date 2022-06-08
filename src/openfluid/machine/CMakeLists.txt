
INCLUDE_DIRECTORIES(${Boost_INCLUDE_DIRS})


SET(OPENFLUID_MACHINE_CPP GhostSimulatorFileIO.cpp
                          Generator.cpp FixedGenerator.cpp RandomGenerator.cpp InjectGenerator.cpp InterpGenerator.cpp
                          GeneratorSignature.cpp
                          ModelInstance.cpp MonitoringInstance.cpp
                          DynamicLib.cpp
                          SimulatorPluginsManager.cpp ObserverPluginsManager.cpp
                          SimulatorRegistry.cpp ObserverRegistry.cpp
                          ExecutionTimePoint.cpp
                          SimulationProfiler.cpp
                          SimulationBlob.cpp
                          Factory.cpp Engine.cpp MachineListener.cpp
                          )

SET(OPENFLUID_MACHINE_HPP ModelItemInstance.hpp GhostSimulatorFileIO.hpp
                          Generator.hpp FixedGenerator.hpp RandomGenerator.hpp InjectGenerator.hpp InterpGenerator.hpp
                          GeneratorSignature.hpp
                          WareInstance.hpp ObserverInstance.hpp
                          ModelInstance.hpp MonitoringInstance.hpp
                          ExecutionTimePoint.hpp
                          SimulationProfiler.hpp
                          SimulationBlob.hpp
                          WareContainer.hpp
                          DynamicLib.hpp
                          WarePluginsManager.hpp WareRegistry.hpp WareRegistrySerializer.hpp 
                          SimulatorPluginsManager.hpp SimulatorRegistry.hpp
                          ObserverPluginsManager.hpp ObserverRegistry.hpp
                          Factory.hpp Engine.hpp MachineListener.hpp
                          )


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
                      openfluid-thirdparty-tinyxml2
                     )

IF(UNIX)
  TARGET_LINK_LIBRARIES(openfluid-machine dl)
ENDIF(UNIX)


INSTALL(TARGETS openfluid-machine
        RUNTIME DESTINATION ${OFBUILD_BIN_INSTALL_PATH}
        LIBRARY DESTINATION ${OFBUILD_LIB_INSTALL_PATH}
        ARCHIVE DESTINATION ${OFBUILD_LIB_INSTALL_PATH})
   
INSTALL(FILES ${OPENFLUID_MACHINE_HPP} DESTINATION ${OPENFLUID_INCLUDE_INSTALL_PATH}/machine)


IF(OFBUILD_ENABLE_TESTING)
  ADD_SUBDIRECTORY(tests)
ENDIF()  
