
FIND_PACKAGE(Boost REQUIRED COMPONENTS unit_test_framework)  


ADD_DEFINITIONS(-DOPENFLUID_VERSION=${FULL_VERSION})


INCLUDE_DIRECTORIES(${LibXML2_INCLUDE_DIRS}
                    ${GTKMM_INCLUDE_DIRS})
                    
LINK_DIRECTORIES(${LibXML2_LIBRARY_DIRS}
                 ${GTKMM_LIBRARY_DIRS})


INCLUDE_DIRECTORIES(${Boost_INCLUDE_DIRS})
LINK_DIRECTORIES(${Boost_LIBRARY_DIRS})


SET(RELDIR_BUILDERMAIN ../../builder-main)

INCLUDE_DIRECTORIES(${RELDIR_BUILDERMAIN} 
                    ${RELDIR_BUILDERMAIN}/common ${RELDIR_BUILDERMAIN}/model 
                    ${RELDIR_BUILDERMAIN}/domain ${RELDIR_BUILDERMAIN}/simulation 
                    ${RELDIR_BUILDERMAIN}/results ${RELDIR_BUILDERMAIN}/mapview
                    "${PROJECT_BINARY_DIR}/src/tests")


MACRO(ADD_BUILDER_UNITTEST TEST_NAME EXE_NAME)
  ADD_EXECUTABLE(${EXE_NAME} ${ARGN})
  TARGET_LINK_LIBRARIES(${EXE_NAME} 
                       ${Boost_UNIT_TEST_FRAMEWORK_LIBRARY}
                       ${Boost_SYSTEM_LIBRARY} ${Boost_FILESYSTEM_LIBRARY} 
                       ${Boost_DATE_TIME_LIBRARY} ${Boost_PROGRAM_OPTIONS_LIBRARY} 
                       ${Boost_REGEX_LIBRARY}
                       ${LibXML2_LIBRARIES}
                       ${GTKMM_LIBRARIES}                 
                       openfluid-base openfluid-buddies
                       openfluid-core openfluid-machine
                       openfluid-io openfluid-tools openfluid-market
                       openfluid-builder-main)
                       
  SET_TARGET_PROPERTIES(${EXE_NAME} PROPERTIES RUNTIME_OUTPUT_DIRECTORY "${TEST_OUTPUT_PATH}")
  ADD_TEST(${TEST_NAME} "${TEST_OUTPUT_PATH}/${EXE_NAME}")
ENDMACRO(ADD_BUILDER_UNITTEST)


SET(UNITTEST_LINK_LIBRARIES ${GTKMM_LIBRARIES}                 
                       openfluid-base
                       openfluid-core openfluid-machine
                       openfluid-io openfluid-tools
                       openfluid-builder-main)

DISCOVER_OPENFLUID_UNITTESTS(builder)
                                          