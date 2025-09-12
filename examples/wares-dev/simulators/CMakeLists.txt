
OFBUILD_ADD_EXAMPLE_SIMULATOR(examples.primitives.unitsA.prod)
OFBUILD_ADD_EXAMPLE_SIMULATOR(examples.primitives.unitsA.up)
OFBUILD_ADD_EXAMPLE_SIMULATOR(examples.primitives.unitsB.prod)

OFBUILD_ADD_EXAMPLE_SIMULATOR(traffic.surf.trafficlights-state)
OFBUILD_ADD_EXAMPLE_SIMULATOR(traffic.surf.car-transfer)

OFBUILD_ADD_EXAMPLE_SIMULATOR(spatial.atm.grid.connection-dynamics)
OFBUILD_ADD_EXAMPLE_SIMULATOR(fire.surf.prod-spread)


INSTALL(DIRECTORY "${OFBUILD_DIST_DIR}/${OPENFLUID_SIMULATORS_INSTALL_PATH}/"
        DESTINATION "${OPENFLUID_SIMULATORS_INSTALL_PATH}")


FUNCTION(OPENFLUID_ADD_SIM_TARGET_AND_TEST _SIM_NAME _DATASET_LIST)
  OFBUILD_ADD_EXAMPLE_SIMULATOR(${_SIM_NAME})

  IF(OFBUILD_ENABLE_TESTING)
    FOREACH(_DATASET_NAME ${_DATASET_LIST})
      OPENFLUID_ADD_DATASETTEST("${OFBUILD_DIST_BIN_DIR}/${OPENFLUID_CMD_APP}"
                                "${CMAKE_CURRENT_SOURCE_DIR}/${_SIM_NAME}/tests/${_DATASET_NAME}.IN"
                                "basic_output_check"
                                "simulators-example-${_SIM_NAME}")
    ENDFOREACH()
  ENDIF()
ENDFUNCTION()


OPENFLUID_ADD_SIM_TARGET_AND_TEST("water.atm-surf.rain-su.files" "SubdomainAll")
OPENFLUID_ADD_SIM_TARGET_AND_TEST("water.surf.transfer-su.hayami" "SubdomainAll;01_OneRain")
OPENFLUID_ADD_SIM_TARGET_AND_TEST("water.surf.transfer-rs.hayami" "SubdomainAll;01_OneDownstream")
OPENFLUID_ADD_SIM_TARGET_AND_TEST("water.surf-uz.runoff-infiltration.mseytoux" "SubdomainAll")