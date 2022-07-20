INCLUDE_DIRECTORIES("${PROJECT_BINARY_DIR}/src/tests")


SET(UNITTEST_LINK_LIBRARIES openfluid-base openfluid-waresdev openfluid-ui-waresdev)

OFBUILD_DISCOVER_UNITTESTS(uiwaresdev)

SET_TESTS_PROPERTIES(unit-uiwaresdev-WaresDevPackage_TEST PROPERTIES RUN_SERIAL true)
