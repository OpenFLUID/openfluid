

INCLUDE_DIRECTORIES(. ${CMAKE_SOURCE_DIR}/include)
INCLUDE_DIRECTORIES(${wxWidgets_INCLUDE_DIRS} ${Boost_INCLUDE_DIRS})


ADD_EXECUTABLE(unittest_unit Unit_TEST.cpp)
TARGET_LINK_LIBRARIES(unittest_unit ${Boost_UNIT_TEST_FRAMEWORK_LIBRARY} ofelib)
ADD_TEST(UnitTest-Unit unittest_unit)

ADD_EXECUTABLE(unittest_unitscoll UnitsColl_TEST.cpp)
TARGET_LINK_LIBRARIES(unittest_unitscoll ${Boost_UNIT_TEST_FRAMEWORK_LIBRARY} ofelib)
ADD_TEST(UnitTest-UnitsCollection unittest_unitscoll)

ADD_EXECUTABLE(unittest_datetime DateTime_TEST.cpp)
TARGET_LINK_LIBRARIES(unittest_datetime ${Boost_UNIT_TEST_FRAMEWORK_LIBRARY} ofelib)
ADD_TEST(UnitTest-DateTime unittest_datetime)