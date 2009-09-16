

INCLUDE_DIRECTORIES(. ${CMAKE_SOURCE_DIR}/include)
INCLUDE_DIRECTORIES(${wxWidgets_INCLUDE_DIRS} ${Boost_INCLUDE_DIRS})

ADD_UNITTEST(UnitTest-AppTools unittest_apptools AppTools_TEST.cpp)

ADD_UNITTEST(UnitTest-RuntimeEnv unittest_runtimeenv RuntimeEnv_TEST.cpp)