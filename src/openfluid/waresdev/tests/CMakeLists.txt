INCLUDE_DIRECTORIES("${PROJECT_BINARY_DIR}/src/tests")


SET(UNITTEST_LINK_LIBRARIES openfluid-base openfluid-waresdev openfluid-utils)

OPNFLD_DISCOVER_UNITTESTS(api)

set_tests_properties(unit-api-WareSrcFactory_TEST PROPERTIES RUN_SERIAL true)
set_tests_properties(unit-api-WareSrcContainer_TEST PROPERTIES RUN_SERIAL true)
set_tests_properties(unit-api-WareSrcManager_TEST PROPERTIES RUN_SERIAL true)