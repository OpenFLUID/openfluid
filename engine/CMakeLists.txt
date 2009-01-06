CMAKE_MINIMUM_REQUIRED (VERSION 2.4)

PROJECT(OpenFLUID-Engine CXX)

INCLUDE(CMake.in.common)
FIND_PACKAGE(Doxygen)
FIND_PACKAGE(wxWidgets)


IF(wxWidgets_FOUND)

  INCLUDE(${wxWidgets_USE_FILE})
  INCLUDE_DIRECTORIES( ${wxWidgets_INCLUDE_DIRS} )

  FILE(MAKE_DIRECTORY "${LIBRARY_OUTPUT_PATH}")
  FILE(MAKE_DIRECTORY "${EXECUTABLE_OUTPUT_PATH}")

  ADD_SUBDIRECTORY(src)

ENDIF(wxWidgets_FOUND)


CONFIGURE_FILE(kernel.cpack.in kernel.cpack @ONLY)
CONFIGURE_FILE(ofelib.cpack.in ofelib.cpack @ONLY)
CONFIGURE_FILE(functions.cpack.in functions.cpack @ONLY)
CONFIGURE_FILE(ofelib-dev.cpack.in ofelib-dev.cpack @ONLY)
CONFIGURE_FILE(full.cpack.in full.cpack @ONLY)


MESSAGE(STATUS "Build type : ${CMAKE_BUILD_TYPE}")
MESSAGE(STATUS "Compiler : ${CMAKE_CXX_COMPILER}")
MESSAGE(STATUS "Compiler flags: " ${CMAKE_CXX_FLAGS} )
MESSAGE(STATUS "Compiler flags added for release build: " ${CMAKE_CXX_FLAGS_RELEASE})
MESSAGE(STATUS "Compiler flags added for debug build: " ${CMAKE_CXX_FLAGS_DEBUG})

