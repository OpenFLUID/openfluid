

INCLUDE_DIRECTORIES(. ${CMAKE_SOURCE_DIR}/include)
INCLUDE_DIRECTORIES(${wxWidgets_INCLUDE_DIRS} ${Boost_INCLUDE_DIRS})


ADD_UNITTEST(UnitTest-Unit unittest_unit Unit_TEST.cpp)

ADD_UNITTEST(UnitTest-UnitsCollection unittest_unitscoll UnitsColl_TEST.cpp)

ADD_UNITTEST(UnitTest-DateTime unittest_datetime DateTime_TEST.cpp)

ADD_UNITTEST(UnitTest-Event unittest_event  DistributedEvent_TEST.cpp)

ADD_UNITTEST(UnitTest-EventsCollection unittest_eventscoll EventsColl_TEST.cpp)

ADD_UNITTEST(UnitTest-InputData unittest_inputdata InputData_TEST.cpp)

ADD_UNITTEST(UnitTest-Variable unittest_variable Variable_TEST.cpp)

ADD_UNITTEST(UnitTest-Vector unittest_vector Vector_TEST.cpp)

ADD_UNITTEST(UnitTest-CoreRepository unittest_corerepository CoreRepository_TEST.cpp)

ADD_UNITTEST(UnitTest-ExecMsgs unittest_execmsgs ExecMsgs_TEST.cpp)

ADD_UNITTEST(UnitTest-FuncSignature unittest_funcsignature FuncSignature_TEST.cpp)

ADD_UNITTEST(UnitTest-SimStatus unittest_simstatus SimStatus_TEST.cpp)

ADD_UNITTEST(UnitTest-FunctionEnv unittest_functionenv FunctionEnv_TEST.cpp)

ADD_UNITTEST(UnitTest-DTSerie unittest_dtserie DTSerie_TEST.cpp)

ADD_UNITTEST(UnitTest-SwissTools unittest_swisstools SwissTools_TEST.cpp)

ADD_UNITTEST(UnitTest-ColTextParser unittest_coltextparser ColTextParser_TEST.cpp)

