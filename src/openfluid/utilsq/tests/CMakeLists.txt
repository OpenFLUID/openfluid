INCLUDE_DIRECTORIES("${PROJECT_BINARY_DIR}/src/tests")


SET(UNITTEST_LINK_LIBRARIES openfluid-base openfluid-utilsq openfluid-ware 
                            openfluid-fluidx openfluid-machine)

OFBUILD_DISCOVER_UNITTESTS(utils)
