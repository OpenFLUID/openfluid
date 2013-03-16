INCLUDE_DIRECTORIES("${PROJECT_BINARY_DIR}/src/tests")


SET(UNITTEST_LINK_LIBRARIES ${Boost_REGEX_LIBRARIES} openfluid-core openfluid-base openfluid-ware openfluid-fluidx openfluid-machine)

OPNFLD_DISCOVER_UNITTESTS(api)