INCLUDE_DIRECTORIES("${PROJECT_BINARY_DIR}/src/tests")


SET(UNITTEST_LINK_LIBRARIES openfluid-core openfluid-base openfluid-fluidx openfluid-ware openfluid-machine)

OFBUILD_DISCOVER_UNITTESTS(fluidx)
