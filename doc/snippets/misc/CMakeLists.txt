INCLUDE_DIRECTORIES(${RapidJSON_INCLUDE_DIR})

SET(MISC_CPP main.cpp singleton.cpp bindings.cpp mischelpers.cpp spatial.cpp filesystem.cpp values.cpp datetime.cpp)

ADD_EXECUTABLE(snippets_misc ${MISC_CPP})

TARGET_LINK_LIBRARIES(snippets_misc
                      openfluid-core
                      openfluid-base
                      openfluid-tools)

SET_TARGET_PROPERTIES(snippets_misc PROPERTIES
                      RUNTIME_OUTPUT_DIRECTORY "${OFBUILD_TESTS_BINARY_DIR}")