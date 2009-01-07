CMAKE_MINIMUM_REQUIRED (VERSION 2.4)

PROJECT(OpenFLUID-Engine CXX)

INCLUDE(CMake.in.common)
FIND_PACKAGE(Doxygen)
FIND_PACKAGE(wxWidgets)
#FIND_PACKAGE(Boost COMPONENTS date_time)

#IF(wxWidgets_FOUND AND Boost_FOUND)
IF(wxWidgets_FOUND)
  INCLUDE(${wxWidgets_USE_FILE})
  INCLUDE_DIRECTORIES(${wxWidgets_INCLUDE_DIRS})
#  INCLUDE_DIRECTORIES(${Boost_INCLUDE_DIRS})

  FILE(MAKE_DIRECTORY "${LIBRARY_OUTPUT_PATH}")
  FILE(MAKE_DIRECTORY "${EXECUTABLE_OUTPUT_PATH}")

  # hack for compiler flags semicolon problem with MSYS
  IF(MSYS)
    FOREACH(arg ${CMAKE_CXX_FLAGS})
      SET(MSYS_ADAPTED_CMAKE_CXX_FLAGS "${MSYS_ADAPTED_CMAKE_CXX_FLAGS} ${arg}")
    ENDFOREACH(arg ${CMAKE_CXX_FLAGS})
    SET(CMAKE_CXX_FLAGS "${MSYS_ADAPTED_CMAKE_CXX_FLAGS}")
  ENDIF(MSYS)


  ADD_SUBDIRECTORY(src)


  CONFIGURE_FILE(kernel.cpack.in kernel.cpack @ONLY)
  CONFIGURE_FILE(ofelib.cpack.in ofelib.cpack @ONLY)
  CONFIGURE_FILE(functions.cpack.in functions.cpack @ONLY)
  CONFIGURE_FILE(ofelib-dev.cpack.in ofelib-dev.cpack @ONLY)
  CONFIGURE_FILE(full.cpack.in full.cpack @ONLY)



  MESSAGE(STATUS "Build type : ${CMAKE_BUILD_TYPE}")
  MESSAGE(STATUS "Compiler : ${CMAKE_CXX_COMPILER}")
  MESSAGE(STATUS "Compiler flags: ${CMAKE_CXX_FLAGS}")
  MESSAGE(STATUS "Compiler flags added for release build:  ${CMAKE_CXX_FLAGS_RELEASE}")
  MESSAGE(STATUS "Compiler flags added for debug build: ${CMAKE_CXX_FLAGS_DEBUG}")

ENDIF(wxWidgets_FOUND)
#ENDIF(wxWidgets_FOUND AND Boost_FOUND)



