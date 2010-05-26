

FIND_PACKAGE(Boost REQUIRED COMPONENTS unit_test_framework)  

INCLUDE_DIRECTORIES(${Boost_INCLUDE_DIRS} ${LibXML2_INCLUDE_DIRS}
                    "${PROJECT_SOURCE_DIR}/src" "${PROJECT_BINARY_DIR}/src/tests")
                    
LINK_DIRECTORIES(${LibXML2_LIBRARY_DIRS})
                    

MACRO(ADD_UNITTEST TEST_NAME EXE_NAME)
  ADD_EXECUTABLE(${EXE_NAME} ${ARGN})
  TARGET_LINK_LIBRARIES(${EXE_NAME} 
                       ${Boost_UNIT_TEST_FRAMEWORK_LIBRARY}
                       ${Boost_SYSTEM_LIBRARY} ${Boost_FILESYSTEM_LIBRARY} 
                       ${Boost_DATE_TIME_LIBRARY} ${Boost_PROGRAM_OPTIONS_LIBRARY} 
                       ${Boost_REGEX_LIBRARY}                      
                       openfluid-base openfluid-buddies
                       openfluid-core openfluid-machine
                       openfluid-io openfluid-tools)
  SET_TARGET_PROPERTIES(${EXE_NAME} PROPERTIES RUNTIME_OUTPUT_DIRECTORY "${TEST_OUTPUT_PATH}")
  ADD_TEST(${TEST_NAME} "${TEST_OUTPUT_PATH}/${EXE_NAME}")
ENDMACRO(ADD_UNITTEST)

#ADD_SUBDIRECTORY(ofelib)
#ADD_SUBDIRECTORY(kernel)

FILE(GLOB UNIT_TESTS_CPP *.cpp)

FOREACH(CPPFILE ${UNIT_TESTS_CPP})
  GET_FILENAME_COMPONENT(CPPFILE_WE ${CPPFILE} NAME_WE)
  ADD_UNITTEST(unit-${CPPFILE_WE} "unittest_${CPPFILE_WE}" ${CPPFILE})
ENDFOREACH(CPPFILE ${UNIT_TESTS_CPP})


#ADD_UNITTEST(unit-MemMonitor unittest_memmonitor MemMonitor_TEST.cpp)
