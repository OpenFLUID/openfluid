INCLUDE_DIRECTORIES(${Boost_INCLUDE_DIRS})


SET(OPENFLUID_FLUIDX_CPP WareDescriptor.cpp SimulatorDescriptor.cpp GeneratorDescriptor.cpp
                         CoupledModelDescriptor.cpp MonitoringDescriptor.cpp
                         SpatialUnitDescriptor.cpp SpatialDomainDescriptor.cpp
                         AttributesTableDescriptor.cpp
                         EventDescriptor.cpp
                         DatastoreDescriptor.cpp DatastoreItemDescriptor.cpp
                         RunConfigurationDescriptor.cpp
                         FluidXIO.cpp
                         )

SET(OPENFLUID_FLUIDX_HPP WareDescriptor.hpp ModelItemDescriptor.hpp 
                         SimulatorDescriptor.hpp GeneratorDescriptor.hpp ObserverDescriptor.hpp
                         WareSetDescriptor.hpp CoupledModelDescriptor.hpp MonitoringDescriptor.hpp
                         SpatialUnitDescriptor.hpp SpatialDomainDescriptor.hpp
                         AttributesTableDescriptor.hpp
                         EventDescriptor.hpp
                         DatastoreDescriptor.hpp DatastoreItemDescriptor.hpp
                         RunConfigurationDescriptor.hpp
                         FluidXDescriptor.hpp
                         FluidXIO.hpp
                         )


ADD_LIBRARY(openfluid-fluidx SHARED ${OPENFLUID_FLUIDX_CPP} ${OPENFLUID_FLUIDX_HPP})

SET_TARGET_PROPERTIES(openfluid-fluidx PROPERTIES VERSION "${OPENFLUID_VERSION_MAJOR}.${OPENFLUID_VERSION_MINOR}" 
                                        SOVERSION "${OPENFLUID_VERSION_MAJOR}.${OPENFLUID_VERSION_MINOR}.${OPENFLUID_VERSION_PATCH}"
                                        LIBRARY_OUTPUT_DIRECTORY "${OFBUILD_DIST_LIB_DIR}"
                                        RUNTIME_OUTPUT_DIRECTORY "${OFBUILD_DIST_LIB_DIR}"
                                        ARCHIVE_OUTPUT_DIRECTORY "${OFBUILD_DIST_LIB_DIR}"
                                        COMPILE_DEFINITIONS "OPENFLUID_DLL"
                                        DEFINE_SYMBOL "OPENFLUID_DLL_EXPORTS")

TARGET_LINK_LIBRARIES(openfluid-fluidx
                      openfluid-base
                      openfluid-core
                      thirdparty-tinyxml2)


INSTALL(TARGETS openfluid-fluidx
        RUNTIME DESTINATION ${OFBUILD_BIN_INSTALL_PATH}
        LIBRARY DESTINATION ${OFBUILD_LIB_INSTALL_PATH}
        ARCHIVE DESTINATION ${OFBUILD_LIB_INSTALL_PATH})
   
INSTALL(FILES ${OPENFLUID_FLUIDX_HPP} DESTINATION ${OPENFLUID_INCLUDE_INSTALL_PATH}/fluidx)


IF(OFBUILD_ENABLE_TESTING)
  ADD_SUBDIRECTORY(tests)
ENDIF()  
