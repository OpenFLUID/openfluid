
OFBUILD_ADD_EXAMPLE_SIMULATOR(examples.primitives.unitsA.prod)
OFBUILD_ADD_EXAMPLE_SIMULATOR(examples.primitives.unitsA.up)
OFBUILD_ADD_EXAMPLE_SIMULATOR(examples.primitives.unitsB.prod)

OFBUILD_ADD_EXAMPLE_SIMULATOR(traffic.surf.trafficlights-state)
OFBUILD_ADD_EXAMPLE_SIMULATOR(traffic.surf.car-transfer)

OFBUILD_ADD_EXAMPLE_SIMULATOR(spatial.atm.grid.connection-dynamics)
OFBUILD_ADD_EXAMPLE_SIMULATOR(fire.surf.prod-spread)

OFBUILD_ADD_EXAMPLE_SIMULATOR(water.atm-surf.rain-su.files)
OFBUILD_ADD_EXAMPLE_SIMULATOR(water.surf-uz.runoff-infiltration.mseytoux)
OFBUILD_ADD_EXAMPLE_SIMULATOR(water.surf.transfer-rs.hayami)
OFBUILD_ADD_EXAMPLE_SIMULATOR(water.surf.transfer-su.hayami)


INSTALL(DIRECTORY "${OFBUILD_DIST_DIR}/${OPENFLUID_SIMULATORS_INSTALL_PATH}/"
        DESTINATION "${OPENFLUID_SIMULATORS_INSTALL_PATH}")

