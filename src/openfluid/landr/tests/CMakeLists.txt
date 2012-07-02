INCLUDE_DIRECTORIES("${PROJECT_BINARY_DIR}/src/tests")

# !! -lgeos to avoid "/usr/lib/libgeos-3.2.2.so: could not read symbols: Invalid operation" on linkage
# cf. https://fedoraproject.org/wiki/UnderstandingDSOLinkChange
SET(UNITTEST_LINK_LIBRARIES openfluid-core openfluid-landr -lgeos)

DISCOVER_OPENFLUID_UNITTESTS(api)