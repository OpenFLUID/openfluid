
INCLUDE_DIRECTORIES(. ${CMAKE_BINARY_DIR} ${CMAKE_SOURCE_DIR}/include ${CMAKE_SOURCE_DIR}/src/kernel)
INCLUDE_DIRECTORIES(${Boost_INCLUDE_DIRS})




ADD_UNITTEST(UnitTest-AppTools unittest_apptools AppTools_TEST.cpp ${CMAKE_SOURCE_DIR}/src/kernel/AppTools.cpp)

ADD_UNITTEST(UnitTest-RuntimeEnv unittest_runtimeenv RuntimeEnv_TEST.cpp ${CMAKE_SOURCE_DIR}/src/kernel/RuntimeEnv.cpp ${CMAKE_SOURCE_DIR}/src/kernel/AppTools.cpp)

ADD_UNITTEST(UnitTest-DynamicLib unittest_dynamiclib DynamicLib_TEST.cpp ${CMAKE_SOURCE_DIR}/src/kernel/DynamicLib.cpp)
IF(UNIX)
  TARGET_LINK_LIBRARIES(unittest_dynamiclib dl)
ENDIF(UNIX)