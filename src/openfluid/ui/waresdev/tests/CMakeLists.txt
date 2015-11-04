INCLUDE_DIRECTORIES("${PROJECT_BINARY_DIR}/src/tests")


SET(UNITTEST_LINK_LIBRARIES openfluid-base openfluid-waresdev openfluid-ui-waresdev)

OFBUILD_DISCOVER_UNITTESTS(api)