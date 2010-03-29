#
# CMake build for OpenFLUID-engine
#
# Author : Jean-Christophe FABRE <fabrejc@supagro.inra.fr>
#
#
# CPack components for packaging:
# - engine : OpenFLUID-engine, with SDK and standard simulation functions  
# - engine-wodev : OpenFLUID-engine, without SDK, and with standard simulation functions
#
#


CMAKE_MINIMUM_REQUIRED (VERSION 2.6)

PROJECT(OpenFLUID-Engine CXX)

INCLUDE(CTest)
ENABLE_TESTING()

INCLUDE(CMake.in.config)

IF (EXISTS ${CMAKE_SOURCE_DIR}/CMake.in.local.config)
  INCLUDE(CMake.in.local.config)
ENDIF (EXISTS ${CMAKE_SOURCE_DIR}/CMake.in.local.config)

GET_FILENAME_COMPONENT(BUILDNAME_COMPILER ${CMAKE_CXX_COMPILER} NAME)
SET(BUILDNAME ${CMAKE_SYSTEM_NAME}-${CMAKE_SYSTEM_PROCESSOR}-${BUILDNAME_COMPILER}-v${ENGINE_VERSION_MAJOR}.${ENGINE_VERSION_MINOR}.${ENGINE_VERSION_PATCH})

SET (CTEST_INITIAL_CACHE "
BUILDNAME:STRING=${CMAKE_SYSTEM_NAME}-${CMAKE_SYSTEM_PROCESSOR}-${BUILDNAME_COMPILER}-v${ENGINE_VERSION_MAJOR}.${ENGINE_VERSION_MINOR}.${ENGINE_VERSION_PATCH}
")

INCLUDE(CMake.in.sub)

FIND_PACKAGE(Doxygen)
FIND_PACKAGE(LATEX)
FIND_PACKAGE(Boost 1.35.0 REQUIRED COMPONENTS system filesystem date_time program_options regex)
FIND_PACKAGE(LibXml2)


IF(Boost_FOUND)


  FILE(MAKE_DIRECTORY "${LIB_OUTPUT_PATH}")
  FILE(MAKE_DIRECTORY "${BIN_OUTPUT_PATH}")
  FILE(MAKE_DIRECTORY "${FUNCTIONS_DOC_OUTPUT_PATH}")

  ADD_SUBDIRECTORY(src)

  CONFIGURE_FILE(${CMAKE_SOURCE_DIR}/CTestCustom.cmake ${CMAKE_BINARY_DIR}/CTestCustom.cmake COPYONLY)
  
  ADD_SUBDIRECTORY(tests)

  ADD_SUBDIRECTORY(doc)

  ADD_SUBDIRECTORY(resources)

  INCLUDE(CPack.in.config)

  MESSAGE(STATUS "Build version: ${ENGINE_FULL_VERSION}")
  MESSAGE(STATUS "Build name: ${BUILDNAME}")
  MESSAGE(STATUS "Build type : ${CMAKE_BUILD_TYPE}")
  MESSAGE(STATUS "Compiler : ${CMAKE_CXX_COMPILER}")
  MESSAGE(STATUS "Compiler flags: ${CMAKE_CXX_FLAGS}")
  MESSAGE(STATUS "Compiler flags added for release build:  ${CMAKE_CXX_FLAGS_RELEASE}")
  MESSAGE(STATUS "Compiler flags added for debug build: ${CMAKE_CXX_FLAGS_DEBUG}")


ENDIF(Boost_FOUND)





