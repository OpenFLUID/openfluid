IF(CMAKE_BUILD_TYPE MATCHES "Debug" OR CMAKE_BUILD_TYPE MATCHES "RelWithDebInfo" OR CMAKE_BUILD_TYPE MATCHES "")

    FIND_PACKAGE(Catch2 3 REQUIRED)
    SET(CMAKE_CXX_FLAGS "-lstdc++fs -std=c++17")
    MESSAGE(STATUS "Addind HayamiTools unit test")

    ADD_EXECUTABLE(HayamiTools_TEST HayamiTools_TEST.cpp)
    TARGET_INCLUDE_DIRECTORIES(HayamiTools_TEST PRIVATE ${OpenFLUID_INCLUDE_DIRS})
    TARGET_LINK_LIBRARIES(HayamiTools_TEST PRIVATE Catch2::Catch2WithMain ${OpenFLUID_LIBRARIES})

    ADD_TEST(NAME "unit-HayamiTools_TEST" COMMAND HayamiTools_TEST)
ENDIF()
