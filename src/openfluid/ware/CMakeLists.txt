
INCLUDE_DIRECTORIES(${Boost_INCLUDE_DIRS})


SET(OPENFLUID_WARE_CPP SimulatorSignature.cpp GeneratorSignature.cpp
                       PluggableWare.cpp SimulationDrivenWare.cpp SimulationInspectorWare.cpp SimulationContributorWare.cpp
                       PluggableSimulator.cpp PluggableObserver.cpp
                       WareParamsTree.cpp
                       )

SET(OPENFLUID_WARE_HPP WareSignature.hpp SimulatorSignature.hpp GeneratorSignature.hpp ObserverSignature.hpp
                       PluggableWare.hpp SimulationDrivenWare.hpp SimulationInspectorWare.hpp SimulationContributorWare.hpp
                       PluggableSimulator.hpp PluggableObserver.hpp                       
                       TypeDefs.hpp
                       WareSignatureMacros.hpp SimulatorSignatureMacros.hpp ObserverSignatureMacros.hpp
                       LoopMacros.hpp ThreadedLoopMacros.hpp
                       WareException.hpp
                       WareParamsTree.hpp
                       )


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


   