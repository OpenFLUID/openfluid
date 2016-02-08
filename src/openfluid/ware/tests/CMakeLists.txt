INCLUDE_DIRECTORIES("${PROJECT_BINARY_DIR}/src/tests")


SET(UNITTEST_LINK_LIBRARIES openfluid-core openfluid-base openfluid-ware openfluid-fluidx openfluid-machine)

OFBUILD_DISCOVER_UNITTESTS(ware)