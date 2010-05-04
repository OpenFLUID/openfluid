

INCLUDE_DIRECTORIES(. ${CMAKE_SOURCE_DIR}/include
                    ${Boost_INCLUDE_DIRS}
                    ${LibXML2_INCLUDE_DIRS})

LINK_DIRECTORIES(${LibXML2_LIBRARY_DIRS})

ADD_UNITTEST(unit-MemMonitor unittest_memmonitor MemMonitor_TEST.cpp)

ADD_UNITTEST(unit-SSerieValues unittest_sserievalues SSerieValues_TEST.cpp)

ADD_UNITTEST(unit-DateTime unittest_datetime DateTime_TEST.cpp)

ADD_UNITTEST(unit-Event unittest_event  DistributedEvent_TEST.cpp)

ADD_UNITTEST(unit-EventsCollection unittest_eventscoll EventsColl_TEST.cpp)

ADD_UNITTEST(unit-InputData unittest_inputdata InputData_TEST.cpp)

ADD_UNITTEST(unit-Variable unittest_variable Variable_TEST.cpp)

ADD_UNITTEST(unit-Vector unittest_vector Vector_TEST.cpp)

ADD_UNITTEST(unit-Unit unittest_unit Unit_TEST.cpp)

ADD_UNITTEST(unit-UnitsCollection unittest_unitscoll UnitsColl_TEST.cpp)

ADD_UNITTEST(unit-CoreRepository unittest_corerepository CoreRepository_TEST.cpp)

ADD_UNITTEST(unit-ExecMsgs unittest_execmsgs ExecMsgs_TEST.cpp)

ADD_UNITTEST(unit-FuncSignature unittest_funcsignature FuncSignature_TEST.cpp)

ADD_UNITTEST(unit-SimStatus unittest_simstatus SimStatus_TEST.cpp)

ADD_UNITTEST(unit-FunctionEnv unittest_functionenv FunctionEnv_TEST.cpp)

ADD_UNITTEST(unit-SwissTools unittest_swisstools SwissTools_TEST.cpp)

