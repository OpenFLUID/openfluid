INCLUDE_DIRECTORIES("${PROJECT_BINARY_DIR}/src/tests")


SET(UNITTEST_LINK_LIBRARIES openfluid-core openfluid-base 
                            openfluid-ware openfluid-machine openfluid-fluidx)

OFBUILD_DISCOVER_UNITTESTS(machine)

SET_TESTS_PROPERTIES(unit-machine-SimulatorSignatureRegistry_TEST PROPERTIES ENVIRONMENT "OPENFLUID_USERDATA_PATH=null;OPENFLUID_TEMP_PATH=${OFBUILD_TESTS_TEMP_DIRS}")