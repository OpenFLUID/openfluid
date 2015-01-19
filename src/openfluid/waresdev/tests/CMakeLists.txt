INCLUDE_DIRECTORIES("${PROJECT_BINARY_DIR}/src/tests")


SET(UNITTEST_LINK_LIBRARIES openfluid-base openfluid-waresdev openfluid-utils)

OPNFLD_DISCOVER_UNITTESTS(api)