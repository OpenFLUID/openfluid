INCLUDE_DIRECTORIES("${PROJECT_SOURCE_DIR}/src" "${PROJECT_BINARY_DIR}/src")                   


OFBUILD_ADD_EXAMPLE_SIMULATOR(examples.primitives.unitsA.prod ExampleUnitsAProd.cpp)

OFBUILD_ADD_EXAMPLE_SIMULATOR(examples.primitives.unitsA.up ExampleUnitsAUp.cpp)

OFBUILD_ADD_EXAMPLE_SIMULATOR(examples.primitives.unitsB.prod ExampleUnitsBProd.cpp)


OFBUILD_ADD_EXAMPLE_SIMULATOR(traffic.surf.trafficlights-state TLUSim.cpp)

OFBUILD_ADD_EXAMPLE_SIMULATOR(traffic.surf.car-transfer RUSim.cpp)


OFBUILD_ADD_EXAMPLE_SIMULATOR(spatial.atm.grid.connection-dynamics WindFireConnectSim.cpp)

OFBUILD_ADD_EXAMPLE_SIMULATOR(fire.surf.prod-spread FireProdSpreadSim.cpp)


OFBUILD_ADD_EXAMPLE_SIMULATOR(water.atm-surf.rain-su.files RainSUFromFilesSim.cpp)

OFBUILD_ADD_EXAMPLE_SIMULATOR(water.surf.transfer-rs.hayami HayamiRSSim.cpp)

OFBUILD_ADD_EXAMPLE_SIMULATOR(water.surf.transfer-su.hayami HayamiSUSim.cpp)

OFBUILD_ADD_EXAMPLE_SIMULATOR(water.surf-uz.runoff-infiltration.mseytoux MorelSeytouxSim.cpp)
