CMAKE_MINIMUM_REQUIRED (VERSION 2.6)


ADD_DEFINITIONS(-DOPENFLUID_VERSION=${FULL_VERSION})
ADD_DEFINITIONS(-DBUILDINGDLL)

INCLUDE_DIRECTORIES("${PROJECT_SOURCE_DIR}/src" "${PROJECT_BINARY_DIR}/src")

LINK_DIRECTORIES(${LibXML2_LIBRARY_DIRS})

ADD_FUNCTION(tests.coupling.A tests.coupling.A ${TEST_OUTPUT_PATH})
ADD_FUNCTION(tests.coupling.B tests.coupling.B ${TEST_OUTPUT_PATH})
ADD_FUNCTION(tests.coupling.C tests.coupling.C ${TEST_OUTPUT_PATH})

IF(BUILD_FORTRAN_FUNCTIONS)
  ENABLE_LANGUAGE(Fortran)
  ADD_SUBDIRECTORY(tests.fortran)  
  ADD_TEST(functions-Fortran "${BIN_OUTPUT_PATH}/openfluid-engine" 
                        "-i${TESTS_DATASETS_PATH}/OPENFLUID.IN.Fortran"
                        "-o${TESTS_OUTPUTDATA_PATH}/OPENFLUID.OUT.Fortran" 
                        "-p${TEST_OUTPUT_PATH}")  
ENDIF(BUILD_FORTRAN_FUNCTIONS)


ADD_FUNCTION(tests.primitives.prod tests.primitives.prod ${TEST_OUTPUT_PATH})
ADD_FUNCTION(tests.primitives.use tests.primitives.use ${TEST_OUTPUT_PATH})
ADD_TEST(functions-Primitives "${BIN_OUTPUT_PATH}/openfluid-engine" 
                           "-i${TESTS_DATASETS_PATH}/OPENFLUID.IN.Primitives"
                           "-o${TESTS_OUTPUTDATA_PATH}/OPENFLUID.OUT.Primitives" 
                           "-p${TEST_OUTPUT_PATH}")                              


ADD_FUNCTION(tests.landprimitives tests.landprimitives ${TEST_OUTPUT_PATH})
ADD_TEST(functions-LandPrimitives "${BIN_OUTPUT_PATH}/openfluid-engine" 
                           "-i${TESTS_DATASETS_PATH}/OPENFLUID.IN.LandPrimitives"
                           "-o${TESTS_OUTPUTDATA_PATH}/OPENFLUID.OUT.LandPrimitives" 
                           "-p${TEST_OUTPUT_PATH}")

ADD_FUNCTION(tests.primitivesvalues.prod tests.primitivesvalues.prod ${TEST_OUTPUT_PATH})
ADD_FUNCTION(tests.primitivesvalues.use tests.primitivesvalues.use ${TEST_OUTPUT_PATH})
ADD_TEST(functions-PrimitivesValues "${BIN_OUTPUT_PATH}/openfluid-engine" 
                           "-i${TESTS_DATASETS_PATH}/OPENFLUID.IN.PrimitivesValues"
                           "-o${TESTS_OUTPUTDATA_PATH}/OPENFLUID.OUT.PrimitivesValues" 
                           "-p${TEST_OUTPUT_PATH}")

ADD_FUNCTION(tests.primitivestypedvalues.prod tests.primitivestypedvalues.prod ${TEST_OUTPUT_PATH})
ADD_FUNCTION(tests.primitivestypedvalues.use tests.primitivestypedvalues.use ${TEST_OUTPUT_PATH})
ADD_TEST(functions-PrimitivesTypedValues "${BIN_OUTPUT_PATH}/openfluid-engine" 
                           "-i${TESTS_DATASETS_PATH}/OPENFLUID.IN.PrimitivesTypedValues"
                           "-o${TESTS_OUTPUTDATA_PATH}/OPENFLUID.OUT.PrimitivesTypedValues" 
                           "-p${TEST_OUTPUT_PATH}")

ADD_FUNCTION(tests.deltaTtime.prod tests.deltaTtime.prod ${TEST_OUTPUT_PATH})
ADD_FUNCTION(tests.variabletime.prod tests.variabletime.prod ${TEST_OUTPUT_PATH})
ADD_TEST(functions-VariableTimeProd "${BIN_OUTPUT_PATH}/openfluid-engine" 
                           "-i${TESTS_DATASETS_PATH}/OPENFLUID.IN.VariableTimeProd"
                           "-o${TESTS_OUTPUTDATA_PATH}/OPENFLUID.OUT.VariableTimeProd" 
                           "-p${TEST_OUTPUT_PATH}"
                           )


ADD_FUNCTION(tests.loops tests.loops ${TEST_OUTPUT_PATH})
ADD_TEST(functions-Loops "${BIN_OUTPUT_PATH}/openfluid-engine" 
                        "-i${TESTS_DATASETS_PATH}/OPENFLUID.IN.Loops"
                        "-o${TESTS_OUTPUTDATA_PATH}/OPENFLUID.OUT.Loops" 
                        "-p${TEST_OUTPUT_PATH}")

ADD_FUNCTION(tests.threadedloops tests.threadedloops ${TEST_OUTPUT_PATH})
TARGET_LINK_LIBRARIES(tests.threadedloops${OPENFLUID_FUNCTIONS_SUFFIX} ${GLIBMM_LIBRARIES})
ADD_TEST(functions-ThreadedLoopsDefault "${BIN_OUTPUT_PATH}/openfluid-engine" 
                        "-i${TESTS_DATASETS_PATH}/OPENFLUID.IN.ThreadedLoops"
                        "-o${TESTS_OUTPUTDATA_PATH}/OPENFLUID.OUT.ThreadedLoops" 
                        "-p${TEST_OUTPUT_PATH}")             
ADD_TEST(functions-ThreadedLoops9 "${BIN_OUTPUT_PATH}/openfluid-engine" 
                        "-i${TESTS_DATASETS_PATH}/OPENFLUID.IN.ThreadedLoops"
                        "-o${TESTS_OUTPUTDATA_PATH}/OPENFLUID.OUT.ThreadedLoops" 
                        "-p${TEST_OUTPUT_PATH}"
                        "-t" "9")             



ADD_FUNCTION(tests.messages tests.messages ${TEST_OUTPUT_PATH})
ADD_TEST(functions-Messages "${BIN_OUTPUT_PATH}/openfluid-engine" 
                         "-i${TESTS_DATASETS_PATH}/OPENFLUID.IN.Messages"
                         "-o${TESTS_OUTPUTDATA_PATH}/OPENFLUID.OUT.Messages" 
                         "-p${TEST_OUTPUT_PATH}")
ADD_TEST(functions-ManyMessages "${BIN_OUTPUT_PATH}/openfluid-engine" 
                             "-i${TESTS_DATASETS_PATH}/OPENFLUID.IN.ManyMessages"
                             "-o${TESTS_OUTPUTDATA_PATH}/OPENFLUID.OUT.ManyMessages" 
                             "-p${TEST_OUTPUT_PATH}"
                             "-z")                              


ADD_FUNCTION(tests.parseunits tests.parseunits ${TEST_OUTPUT_PATH})
ADD_TEST(functions-ManyUnits "${BIN_OUTPUT_PATH}/openfluid-engine" 
                             "-i${TESTS_DATASETS_PATH}/OPENFLUID.IN.ManyUnits"
                             "-o${TESTS_OUTPUTDATA_PATH}/OPENFLUID.OUT.ManyUnits" 
                             "-p${TEST_OUTPUT_PATH}")                              
             
                         
ADD_FUNCTION(tests.ofefunc2doc tests.ofefunc2doc ${TEST_OUTPUT_PATH})


ADD_FUNCTION(tests.tools tests.tools ${TEST_OUTPUT_PATH})
ADD_TEST(functions-Tools "${BIN_OUTPUT_PATH}/openfluid-engine" 
                        "-i${TESTS_DATASETS_PATH}/OPENFLUID.IN.Tools"
                        "-o${TESTS_OUTPUTDATA_PATH}/OPENFLUID.OUT.Tools" 
                        "-p${TEST_OUTPUT_PATH}")


ADD_FUNCTION(tests.events tests.events ${TEST_OUTPUT_PATH})
ADD_TEST(functions-Events "${BIN_OUTPUT_PATH}/openfluid-engine" 
                        "-i${TESTS_DATASETS_PATH}/OPENFLUID.IN.Events"
                        "-o${TESTS_OUTPUTDATA_PATH}/OPENFLUID.OUT.Events" 
                        "-p${TEST_OUTPUT_PATH}")                              
                                     
ADD_FUNCTION(tests.fakefunction tests.fakefunction ${TEST_OUTPUT_PATH})

ADD_FUNCTION(tests.wrongprod tests.wrongprod ${TEST_OUTPUT_PATH})
ADD_TEST(functions-WrongProduction "${BIN_OUTPUT_PATH}/openfluid-engine" 
                        "-i${TESTS_DATASETS_PATH}/OPENFLUID.IN.WrongProd"
                        "-o${TESTS_OUTPUTDATA_PATH}/OPENFLUID.OUT.WrongProd" 
                        "-p${TEST_OUTPUT_PATH}")                              
SET_TESTS_PROPERTIES(functions-WrongProduction PROPERTIES WILL_FAIL TRUE)        

ADD_FUNCTION(tests.globalparams tests.globalparams ${TEST_OUTPUT_PATH})
ADD_TEST(functions-GlobalParams "${BIN_OUTPUT_PATH}/openfluid-engine" 
                        "-i${TESTS_DATASETS_PATH}/OPENFLUID.IN.GlobalParams"
                        "-o${TESTS_OUTPUTDATA_PATH}/OPENFLUID.OUT.GlobalParams" 
                        "-p${TEST_OUTPUT_PATH}")                              
        
ADD_FUNCTION(tests.generators tests.generators ${TEST_OUTPUT_PATH})
ADD_TEST(functions-Generators "${BIN_OUTPUT_PATH}/openfluid-engine" 
                        "-i${TESTS_DATASETS_PATH}/OPENFLUID.IN.Generators"
                        "-o${TESTS_OUTPUTDATA_PATH}/OPENFLUID.OUT.Generators" 
                        "-p${TEST_OUTPUT_PATH}" "-v")                              

ADD_FUNCTION(tests.logger tests.logger ${TEST_OUTPUT_PATH})
ADD_TEST(functions-Logger "${BIN_OUTPUT_PATH}/openfluid-engine" 
                        "-i${TESTS_DATASETS_PATH}/OPENFLUID.IN.Logger"
                        "-o${TESTS_OUTPUTDATA_PATH}/OPENFLUID.OUT.Logger" 
                        "-p${TEST_OUTPUT_PATH}"
                        "-n${TEST_OUTPUT_PATH}")                              

ADD_FUNCTION(tests.debug tests.debug ${TEST_OUTPUT_PATH})
ADD_TEST(functions-Debug "${BIN_OUTPUT_PATH}/openfluid-engine" 
                        "-i${TESTS_DATASETS_PATH}/OPENFLUID.IN.Debug"
                        "-o${TESTS_OUTPUTDATA_PATH}/OPENFLUID.OUT.Debug" 
                        "-p${TEST_OUTPUT_PATH}")                              
IF(${CMAKE_BUILD_TYPE} STREQUAL "Debug")
  SET_TESTS_PROPERTIES(functions-Debug PROPERTIES PASS_REGULAR_EXPRESSION "TU#")  
ELSE(${CMAKE_BUILD_TYPE} STREQUAL "Debug")
  SET_TESTS_PROPERTIES(functions-Debug PROPERTIES FAIL_REGULAR_EXPRESSION "TU#")
ENDIF(${CMAKE_BUILD_TYPE} STREQUAL "Debug") 

                            