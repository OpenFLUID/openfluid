INCLUDE_DIRECTORIES("${PROJECT_BINARY_DIR}/src/tests")

INCLUDE_DIRECTORIES("${CMAKE_SOURCE_DIR}/src/openfluid/waresdev/tests")

SET(UNITTEST_LINK_LIBRARIES openfluid-base openfluid-utils openfluid-waresdev openfluid-ui-common)

OFBUILD_DISCOVER_UNITTESTS(uicommon)
