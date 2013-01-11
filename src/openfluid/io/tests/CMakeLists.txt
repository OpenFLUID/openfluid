INCLUDE_DIRECTORIES("${PROJECT_BINARY_DIR}/src/tests")


SET(UNITTEST_LINK_LIBRARIES ${Boost_REGEX_LIBRARIES} openfluid-core openfluid-base openfluid-machine openfluid-io openfluid-fluidx)

DISCOVER_OPENFLUID_UNITTESTS(api)