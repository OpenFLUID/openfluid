
ADD_DEFINITIONS(-DOPENFLUID_VERSION=${FULL_VERSION})
ADD_DEFINITIONS(-DBUILDINGDLL)

INCLUDE_DIRECTORIES("${PROJECT_SOURCE_DIR}/src" "${PROJECT_BINARY_DIR}/src"
                    ${Boost_INCLUDE_DIRS} ${GLIBMM_INCLUDE_DIRS})
                    
LINK_DIRECTORIES(${Boost_LIBRARY_DIRS} ${GLIBMM_LIBRARY_DIRS} )

OPNFLD_ADD_SIMULATOR_WITH_INSTALL(examples.primitives.unitsA.prod examples.primitives.unitsA.prod ${SIMULATORS_OUTPUT_PATH})
OPNFLD_ADD_SIMULATOR_WITH_INSTALL(examples.primitives.unitsA.up examples.primitives.unitsA.up ${SIMULATORS_OUTPUT_PATH})
OPNFLD_ADD_SIMULATOR_WITH_INSTALL(examples.primitives.unitsB.prod examples.primitives.unitsB.prod ${SIMULATORS_OUTPUT_PATH})
OPNFLD_ADD_SIMULATOR_WITH_INSTALL(examples.trafficlight.state examples.trafficlight.state ${SIMULATORS_OUTPUT_PATH})
OPNFLD_ADD_SIMULATOR_WITH_INSTALL(examples.road.traffic examples.road.traffic ${SIMULATORS_OUTPUT_PATH})

