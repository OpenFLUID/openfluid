INCLUDE_DIRECTORIES("${PROJECT_BINARY_DIR}/src/tests")


SET(UNITTEST_LINK_LIBRARIES openfluid-base openfluid-tools openfluid-waresdev openfluid-utils)

OFBUILD_DISCOVER_UNITTESTS(waresdev)

SET_TESTS_PROPERTIES(unit-waresdev-WareSrcFactory_TEST PROPERTIES RUN_SERIAL true)
SET_TESTS_PROPERTIES(unit-waresdev-WareSrcContainer_TEST PROPERTIES RUN_SERIAL true)
