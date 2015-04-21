INCLUDE_DIRECTORIES("${PROJECT_BINARY_DIR}/src/tests")


SET(UNITTEST_LINK_LIBRARIES openfluid-core openfluid-base 
                            openfluid-ware openfluid-machine openfluid-fluidx)

OPNFLD_DISCOVER_UNITTESTS(api)

SET_TESTS_PROPERTIES(unit-api-SimulatorSignatureRegistry_TEST PROPERTIES ENVIRONMENT "OPENFLUID_USERDATA_PATH=null;OPENFLUID_TEMP_PATH=${OPENFLUID_TESTS_TEMP_PATHS}")