CMAKE_MINIMUM_REQUIRED (VERSION 2.6)


ADD_DEFINITIONS(-DOFELIB_VERSION=${ENGINE_FULL_VERSION})

INCLUDE_DIRECTORIES(${CMAKE_SOURCE_DIR}/include 
                    ${CMAKE_SOURCE_DIR}/include/core ${CMAKE_SOURCE_DIR}/include/base 
                    ${CMAKE_SOURCE_DIR}/include/tools)

LINK_DIRECTORIES(${LibXML2_LIBRARY_DIRS})



IF(BUILD_FORTRAN_FUNCTIONS)
  ENABLE_LANGUAGE(Fortran)
  ADD_SUBDIRECTORY(tests.fortran)  
  ADD_TEST(functions-Fortran "${BIN_OUTPUT_PATH}/openfluid-engine" 
                        "-i${TESTS_DATASETS_PATH}/OPENFLUID.IN.CheckFortran"
                        "-o${TESTS_OUTPUTDATA_PATH}/OPENFLUID.OUT.CheckFortran" 
                        "-p${TEST_OUTPUT_PATH}")  
ENDIF(BUILD_FORTRAN_FUNCTIONS)


ADD_FUNCTION(tests.vector.prod tests.vector.prod ${TEST_OUTPUT_PATH})
ADD_FUNCTION(tests.vector.use tests.vector.use ${TEST_OUTPUT_PATH})
ADD_TEST(functions-Vector "${BIN_OUTPUT_PATH}/openfluid-engine" 
                              "-i${TESTS_DATASETS_PATH}/OPENFLUID.IN.CheckVector"
                              "-o${TESTS_OUTPUTDATA_PATH}/OPENFLUID.OUT.CheckVector" 
                              "-p${TEST_OUTPUT_PATH}")                              

IF(TESTS_RUNHIGHLOAD)
  ADD_FUNCTION(tests.largevector.prod tests.largevector.prod ${TEST_OUTPUT_PATH})
  ADD_FUNCTION(tests.largevector.use tests.largevector.use ${TEST_OUTPUT_PATH})
  ADD_TEST(functions-LargeVector "${BIN_OUTPUT_PATH}/openfluid-engine" 
                                 "-i${TESTS_DATASETS_PATH}/OPENFLUID.IN.CheckLargeVector"
                                 "-o${TESTS_OUTPUTDATA_PATH}/OPENFLUID.OUT.CheckLargeVector" 
                                 "-p${TEST_OUTPUT_PATH}")
ENDIF(TESTS_RUNHIGHLOAD)                                                               



ADD_FUNCTION(tests.primitives.prod tests.primitives.prod ${TEST_OUTPUT_PATH})
ADD_FUNCTION(tests.primitives.use tests.primitives.use ${TEST_OUTPUT_PATH})
ADD_TEST(functions-Primitives "${BIN_OUTPUT_PATH}/openfluid-engine" 
                           "-i${TESTS_DATASETS_PATH}/OPENFLUID.IN.CheckPrimitives"
                           "-o${TESTS_OUTPUTDATA_PATH}/OPENFLUID.OUT.CheckPrimitives" 
                           "-p${TEST_OUTPUT_PATH}")                              






ADD_FUNCTION(tests.loops tests.loops ${TEST_OUTPUT_PATH})
ADD_TEST(functions-Loops "${BIN_OUTPUT_PATH}/openfluid-engine" 
                        "-i${TESTS_DATASETS_PATH}/OPENFLUID.IN.CheckLoops"
                        "-o${TESTS_OUTPUTDATA_PATH}/OPENFLUID.OUT.CheckLoops" 
                        "-p${TEST_OUTPUT_PATH}")                              



ADD_FUNCTION(tests.messages tests.messages ${TEST_OUTPUT_PATH})
ADD_TEST(functions-Messages "${BIN_OUTPUT_PATH}/openfluid-engine" 
                         "-i${TESTS_DATASETS_PATH}/OPENFLUID.IN.CheckMessages"
                         "-o${TESTS_OUTPUTDATA_PATH}/OPENFLUID.OUT.CheckMessages" 
                         "-p${TEST_OUTPUT_PATH}")
ADD_TEST(functions-ManyMessages "${BIN_OUTPUT_PATH}/openfluid-engine" 
                             "-i${TESTS_DATASETS_PATH}/OPENFLUID.IN.CheckManyMessages"
                             "-o${TESTS_OUTPUTDATA_PATH}/OPENFLUID.OUT.CheckManyMessages" 
                             "-p${TEST_OUTPUT_PATH}"
                             "-z")                              
                         
                         
ADD_FUNCTION(tests.ofefunc2doc tests.ofefunc2doc ${TEST_OUTPUT_PATH})


ADD_SUBDIRECTORY(tests.tools tests.tools)
ADD_TEST(functions-Tools "${BIN_OUTPUT_PATH}/openfluid-engine" 
                        "-i${TESTS_DATASETS_PATH}/OPENFLUID.IN.CheckTools"
                        "-o${TESTS_OUTPUTDATA_PATH}/OPENFLUID.OUT.CheckTools" 
                        "-p${TEST_OUTPUT_PATH}")


ADD_FUNCTION(tests.events tests.events ${TEST_OUTPUT_PATH})
ADD_TEST(functions-Events "${BIN_OUTPUT_PATH}/openfluid-engine" 
                        "-i${TESTS_DATASETS_PATH}/OPENFLUID.IN.CheckEvents"
                        "-o${TESTS_OUTPUTDATA_PATH}/OPENFLUID.OUT.CheckEvents" 
                        "-p${TEST_OUTPUT_PATH}")                              

ADD_FUNCTION(tests.progout tests.progout ${TEST_OUTPUT_PATH})
ADD_TEST(functions-ProgressiveOutput "${BIN_OUTPUT_PATH}/openfluid-engine" 
                        "-i${TESTS_DATASETS_PATH}/OPENFLUID.IN.CheckProgOut"
                        "-o${TESTS_OUTPUTDATA_PATH}/OPENFLUID.OUT.CheckProgOut" 
                        "-p${TEST_OUTPUT_PATH}")                              
                              
ADD_FUNCTION(tests.fakefunction tests.fakefunction ${TEST_OUTPUT_PATH})

ADD_FUNCTION(tests.wrongprod tests.wrongprod ${TEST_OUTPUT_PATH})
ADD_TEST(functions-WrongProduction "${BIN_OUTPUT_PATH}/openfluid-engine" 
                        "-i${TESTS_DATASETS_PATH}/OPENFLUID.IN.CheckWrongProd"
                        "-o${TESTS_OUTPUTDATA_PATH}/OPENFLUID.OUT.CheckWrongProd" 
                        "-p${TEST_OUTPUT_PATH}")                              
SET_TESTS_PROPERTIES(functions-WrongProduction PROPERTIES WILL_FAIL TRUE)        

ADD_FUNCTION(tests.globalparams tests.globalparams ${TEST_OUTPUT_PATH})
ADD_TEST(functions-GlobalParams "${BIN_OUTPUT_PATH}/openfluid-engine" 
                        "-i${TESTS_DATASETS_PATH}/OPENFLUID.IN.CheckGlobalParams"
                        "-o${TESTS_OUTPUTDATA_PATH}/OPENFLUID.OUT.CheckGlobalParams" 
                        "-p${TEST_OUTPUT_PATH}")                              
        
ADD_FUNCTION(tests.generators tests.generators ${TEST_OUTPUT_PATH})
ADD_TEST(functions-Generators "${BIN_OUTPUT_PATH}/openfluid-engine" 
                        "-i${TESTS_DATASETS_PATH}/OPENFLUID.IN.CheckGenerators"
                        "-o${TESTS_OUTPUTDATA_PATH}/OPENFLUID.OUT.CheckGenerators" 
                        "-p${TEST_OUTPUT_PATH}" "-v")                              

