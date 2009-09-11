CMAKE_MINIMUM_REQUIRED (VERSION 2.6)


ADD_DEFINITIONS(-DOFELIB_VERSION=${ENGINE_FULL_VERSION})

INCLUDE_DIRECTORIES(${CMAKE_SOURCE_DIR}/include ${CMAKE_SOURCE_DIR}/include/core ${CMAKE_SOURCE_DIR}/include/base ${CMAKE_SOURCE_DIR}/include/tools)

IF(BUILD_FORTRAN_FUNCTIONS)
  ENABLE_LANGUAGE(Fortran)
ENDIF(BUILD_FORTRAN_FUNCTIONS)


ADD_FUNCTION(tests.vector.prod tests.vector.prod ${TEST_OUTPUT_PATH})
ADD_FUNCTION(tests.vector.use tests.vector.use ${TEST_OUTPUT_PATH})
ADD_FUNCTION(tests.largevector.prod tests.largevector.prod ${TEST_OUTPUT_PATH})
ADD_FUNCTION(tests.largevector.use tests.largevector.use ${TEST_OUTPUT_PATH})
ADD_FUNCTION(tests.primitives.prod tests.primitives.prod ${TEST_OUTPUT_PATH})
ADD_FUNCTION(tests.primitives.use tests.primitives.use ${TEST_OUTPUT_PATH})

IF(BUILD_FORTRAN_FUNCTIONS)
  ADD_SUBDIRECTORY(tests.fortran)
ENDIF(BUILD_FORTRAN_FUNCTIONS)

ADD_FUNCTION(tests.events tests.events ${TEST_OUTPUT_PATH})
ADD_FUNCTION(tests.loops tests.loops ${TEST_OUTPUT_PATH})
ADD_FUNCTION(tests.messages tests.messages ${TEST_OUTPUT_PATH})
ADD_FUNCTION(tests.ofefunc2doc tests.ofefunc2doc ${TEST_OUTPUT_PATH})
ADD_SUBDIRECTORY(tests.tools tests.tools)



