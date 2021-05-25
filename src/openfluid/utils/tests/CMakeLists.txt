INCLUDE_DIRECTORIES("${PROJECT_BINARY_DIR}/src/tests")


SET(UNITTEST_LINK_LIBRARIES openfluid-base openfluid-utils openfluid-ware 
                            openfluid-fluidx openfluid-machine)

OFBUILD_DISCOVER_UNITTESTS(utils)

IF(NOT WIN32)
  SET_TESTS_PROPERTIES(unit-utils-GitProxy_TEST PROPERTIES ENVIRONMENT "PATH=${OFBUILD_DIST_BIN_DIR}:$ENV{PATH}")
ENDIF()
