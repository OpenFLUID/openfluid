
FUNCTION(OPENFLUID_ADD_SIM_TARGET_AND_TEST _SIM_NAME _DATASET_LIST _HEAVY_TEST)
OFBUILD_ADD_EXAMPLE_SIMULATOR(${_SIM_NAME})

  IF(OFBUILD_ENABLE_TESTING)
    FOREACH(_DATASET_NAME ${_DATASET_LIST})
      OPENFLUID_ADD_DATASETTEST("${OFBUILD_DIST_BIN_DIR}/${OPENFLUID_CMD_APP}"
                                "${CMAKE_CURRENT_SOURCE_DIR}/${_SIM_NAME}/tests/${_DATASET_NAME}.IN"
                                "basic_output_check"
                                "simulators-example-${_SIM_NAME}"
                                ${_HEAVY_TEST})
    ENDFOREACH()
  ENDIF()
ENDFUNCTION()


OPENFLUID_ADD_SIM_TARGET_AND_TEST("water.atm-surf.rain-su.files" "SubdomainAll" OFF)
OPENFLUID_ADD_SIM_TARGET_AND_TEST("water.surf.transfer-su.hayami" "SubdomainAll;01_OneRain" OFF)
OPENFLUID_ADD_SIM_TARGET_AND_TEST("water.surf.transfer-rs.hayami" "SubdomainAll;01_OneDownstream" OFF)
OPENFLUID_ADD_SIM_TARGET_AND_TEST("water.surf-uz.runoff-infiltration.mseytoux" "SubdomainAll" OFF)

OPENFLUID_ADD_SIM_TARGET_AND_TEST("examples.primitives.unitsA.prod" "Primitives" OFF)
OPENFLUID_ADD_SIM_TARGET_AND_TEST("examples.primitives.unitsA.up" "Primitives" OFF)
OPENFLUID_ADD_SIM_TARGET_AND_TEST("examples.primitives.unitsB.prod" "Primitives" OFF)

OPENFLUID_ADD_SIM_TARGET_AND_TEST("traffic.surf.trafficlights-state" "Manhattan" OFF)
OPENFLUID_ADD_SIM_TARGET_AND_TEST("traffic.surf.car-transfer" "Manhattan" OFF)

OPENFLUID_ADD_SIM_TARGET_AND_TEST("spatial.atm.grid.connection-dynamics" "Firespread_seeded" OFF)
OPENFLUID_ADD_SIM_TARGET_AND_TEST("fire.surf.prod-spread" "Firespread_seeded" ON)


INSTALL(DIRECTORY "${OFBUILD_DIST_DIR}/${OPENFLUID_SIMULATORS_INSTALL_PATH}/"
        DESTINATION "${OPENFLUID_SIMULATORS_INSTALL_PATH}")
