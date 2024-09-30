INCLUDE_DIRECTORIES("${PROJECT_BINARY_DIR}/src/tests")

SET(UNITTEST_LINK_LIBRARIES openfluid-base openfluid-utils openfluid-waresdev openfluid-ui-common openfluid-ui-spatial)

OFBUILD_DISCOVER_UNITTESTS(uispatial)
OFBUILD_DISCOVER_HEAVYUNITTESTS(uispatial)
