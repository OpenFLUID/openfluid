INCLUDE_DIRECTORIES("${PROJECT_SOURCE_DIR}/src" "${PROJECT_BINARY_DIR}/src")                   


OFBUILD_ADD_EXAMPLE_SIMULATOR(examples.primitives.unitsA.prod ExampleUnitsAProd.cpp NO)

OFBUILD_ADD_EXAMPLE_SIMULATOR(examples.primitives.unitsA.up ExampleUnitsAUp.cpp NO)

OFBUILD_ADD_EXAMPLE_SIMULATOR(examples.primitives.unitsB.prod ExampleUnitsBProd.cpp NO)


OFBUILD_ADD_EXAMPLE_SIMULATOR(traffic.surf.trafficlights-state TLUSim.cpp YES)

OFBUILD_ADD_EXAMPLE_SIMULATOR(traffic.surf.car-transfer RUSim.cpp YES)


OFBUILD_ADD_EXAMPLE_SIMULATOR(spatial.atm.grid.connection-dynamics WindFireConnectSim.cpp YES)

OFBUILD_ADD_EXAMPLE_SIMULATOR(fire.surf.prod-spread FireProdSpreadSim.cpp YES)


OFBUILD_ADD_EXAMPLE_SIMULATOR(water.atm-surf.rain-su.files RainSUFromFilesSim.cpp YES)

OFBUILD_ADD_EXAMPLE_SIMULATOR(water.surf.transfer-rs.hayami HayamiRSSim.cpp YES)

OFBUILD_ADD_EXAMPLE_SIMULATOR(water.surf.transfer-su.hayami HayamiSUSim.cpp YES)

OFBUILD_ADD_EXAMPLE_SIMULATOR(water.surf-uz.runoff-infiltration.mseytoux MorelSeytouxSim.cpp YES)


FILE(GLOB DOC_FILES "${OFBUILD_EXAMPLES_SIMULATORS_DOC_DIR}/*.pdf") 
INSTALL(FILES ${DOC_FILES}
        DESTINATION ${OPENFLUID_SIMULATORS_INSTALL_PATH})
