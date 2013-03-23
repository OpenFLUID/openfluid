CMAKE_MINIMUM_REQUIRED (VERSION 2.6)


ADD_DEFINITIONS(-DOPENFLUID_VERSION=${FULL_VERSION})
ADD_DEFINITIONS(-DBUILDINGDLL)

INCLUDE_DIRECTORIES("${PROJECT_SOURCE_DIR}/src" "${PROJECT_BINARY_DIR}/src")

LINK_DIRECTORIES(${LibXML2_LIBRARY_DIRS})


###########################################################################

OPNFLD_ADD_FUNCTION(tests.coupling.A tests.coupling.A ${TEST_OUTPUT_PATH})
OPNFLD_ADD_FUNCTION(tests.coupling.B tests.coupling.B ${TEST_OUTPUT_PATH})
OPNFLD_ADD_FUNCTION(tests.coupling.C tests.coupling.C ${TEST_OUTPUT_PATH})


###########################################################################


IF(BUILD_FORTRAN_FUNCTIONS)
  ENABLE_LANGUAGE(Fortran)
  ADD_SUBDIRECTORY(tests.fortran)  
  OPENFLUID_ADD_TEST(NAME functions-Fortran
                     COMMAND "${BIN_OUTPUT_PATH}/${OPENFLUID_CMD_APP}" 
                        "-i${TESTS_DATASETS_PATH}/OPENFLUID.IN.Fortran"
                        "-o${TESTS_OUTPUTDATA_PATH}/OPENFLUID.OUT.Fortran" 
                        "-p${TEST_OUTPUT_PATH}"
                     PRE_TEST REMOVE_DIRECTORY "${TESTS_OUTPUTDATA_PATH}/OPENFLUID.OUT.Fortran"
                     POST_TEST CHECK_FILE_EXIST "${TESTS_OUTPUTDATA_PATH}/OPENFLUID.OUT.Fortran/tests.fortran_ofware-func.log"                        )  
ENDIF(BUILD_FORTRAN_FUNCTIONS)


###########################################################################


OPNFLD_ADD_FUNCTION(tests.primitives.runenv tests.primitives.runenv ${TEST_OUTPUT_PATH})
OPENFLUID_ADD_TEST(NAME functions-PrimitivesRunEnv 
                   COMMAND "${BIN_OUTPUT_PATH}/${OPENFLUID_CMD_APP}" 
                           "-i${TESTS_DATASETS_PATH}/OPENFLUID.IN.PrimitivesRunEnv"
                           "-o${TESTS_OUTPUTDATA_PATH}/OPENFLUID.OUT.PrimitivesRunEnv" 
                           "-p${TEST_OUTPUT_PATH}"
                           "-n${TEST_OUTPUT_PATH}"
                    PRE_TEST REMOVE_DIRECTORY "${TESTS_OUTPUTDATA_PATH}/OPENFLUID.OUT.PrimitivesRunEnv"
                    POST_TEST CHECK_FILE_EXIST "${TESTS_OUTPUTDATA_PATH}/OPENFLUID.OUT.PrimitivesRunEnv/openfluid-messages.log"
                              CHECK_FILE_EXIST "${TESTS_OUTPUTDATA_PATH}/OPENFLUID.OUT.PrimitivesRunEnv/tests.primitives.runenv_ofware-func.log"
                   )


###########################################################################


OPNFLD_ADD_FUNCTION(tests.primitives.time tests.primitives.time ${TEST_OUTPUT_PATH})
OPENFLUID_ADD_TEST(NAME functions-PrimitivesTime 
                   COMMAND "${BIN_OUTPUT_PATH}/${OPENFLUID_CMD_APP}" 
                           "-i${TESTS_DATASETS_PATH}/OPENFLUID.IN.PrimitivesTime"
                           "-o${TESTS_OUTPUTDATA_PATH}/OPENFLUID.OUT.PrimitivesTime" 
                           "-p${TEST_OUTPUT_PATH}"
                           "-n${TEST_OUTPUT_PATH}"
                    PRE_TEST REMOVE_DIRECTORY "${TESTS_OUTPUTDATA_PATH}/OPENFLUID.OUT.PrimitivesTime"
                    POST_TEST CHECK_FILE_EXIST "${TESTS_OUTPUTDATA_PATH}/OPENFLUID.OUT.PrimitivesTime/openfluid-messages.log"
                              CHECK_FILE_EXIST "${TESTS_OUTPUTDATA_PATH}/OPENFLUID.OUT.PrimitivesTime/tests.primitives.time_ofware-func.log"
                              CHECK_FILE_EXIST "${TESTS_OUTPUTDATA_PATH}/OPENFLUID.OUT.PrimitivesTime/tests.primitives.time_ofware-obs.log"
                   )


###########################################################################


OPNFLD_ADD_FUNCTION(tests.primitives.funcparams tests.primitives.funcparams ${TEST_OUTPUT_PATH})
OPENFLUID_ADD_TEST(NAME functions-PrimitivesFuncParams 
                   COMMAND "${BIN_OUTPUT_PATH}/${OPENFLUID_CMD_APP}" 
                           "-i${TESTS_DATASETS_PATH}/OPENFLUID.IN.PrimitivesFuncParams"
                           "-o${TESTS_OUTPUTDATA_PATH}/OPENFLUID.OUT.PrimitivesFuncParams" 
                           "-p${TEST_OUTPUT_PATH}"
                           "-n${TEST_OUTPUT_PATH}"
                    PRE_TEST REMOVE_DIRECTORY "${TESTS_OUTPUTDATA_PATH}/OPENFLUID.OUT.PrimitivesFuncParams"
                    POST_TEST CHECK_FILE_EXIST "${TESTS_OUTPUTDATA_PATH}/OPENFLUID.OUT.PrimitivesFuncParams/openfluid-messages.log"
                              CHECK_FILE_EXIST "${TESTS_OUTPUTDATA_PATH}/OPENFLUID.OUT.PrimitivesFuncParams/tests.primitives.funcparams_ofware-func.log"
                   )


###########################################################################


OPNFLD_ADD_FUNCTION(tests.primitives.inputdata.prod tests.primitives.inputdata.prod ${TEST_OUTPUT_PATH})
OPNFLD_ADD_FUNCTION(tests.primitives.inputdata.use tests.primitives.inputdata.use ${TEST_OUTPUT_PATH})
OPENFLUID_ADD_TEST(NAME functions-PrimitivesInputdata 
                   COMMAND "${BIN_OUTPUT_PATH}/${OPENFLUID_CMD_APP}" 
                           "-i${TESTS_DATASETS_PATH}/OPENFLUID.IN.PrimitivesInputdata"
                           "-o${TESTS_OUTPUTDATA_PATH}/OPENFLUID.OUT.PrimitivesInputdata" 
                           "-p${TEST_OUTPUT_PATH}"
                           "-n${TEST_OUTPUT_PATH}"
                    PRE_TEST REMOVE_DIRECTORY "${TESTS_OUTPUTDATA_PATH}/OPENFLUID.OUT.PrimitivesInputdata"
                    POST_TEST CHECK_FILE_EXIST "${TESTS_OUTPUTDATA_PATH}/OPENFLUID.OUT.PrimitivesInputdata/openfluid-messages.log"
                              CHECK_FILE_EXIST "${TESTS_OUTPUTDATA_PATH}/OPENFLUID.OUT.PrimitivesInputdata/tests.primitives.inputdata.prod_ofware-func.log"
                              CHECK_FILE_EXIST "${TESTS_OUTPUTDATA_PATH}/OPENFLUID.OUT.PrimitivesInputdata/tests.primitives.inputdata.use_ofware-func.log"
                   )


###########################################################################


OPNFLD_ADD_FUNCTION(tests.primitives.events tests.primitives.events ${TEST_OUTPUT_PATH})
OPENFLUID_ADD_TEST(NAME functions-PrimitivesEvents 
                   COMMAND "${BIN_OUTPUT_PATH}/${OPENFLUID_CMD_APP}" 
                           "-i${TESTS_DATASETS_PATH}/OPENFLUID.IN.PrimitivesEvents"
                           "-o${TESTS_OUTPUTDATA_PATH}/OPENFLUID.OUT.PrimitivesEvents" 
                           "-p${TEST_OUTPUT_PATH}"
                           "-n${TEST_OUTPUT_PATH}"
                    PRE_TEST REMOVE_DIRECTORY "${TESTS_OUTPUTDATA_PATH}/OPENFLUID.OUT.PrimitivesEvents"
                    POST_TEST CHECK_FILE_EXIST "${TESTS_OUTPUTDATA_PATH}/OPENFLUID.OUT.PrimitivesEvents/openfluid-messages.log"
                              CHECK_FILE_EXIST "${TESTS_OUTPUTDATA_PATH}/OPENFLUID.OUT.PrimitivesEvents/tests.primitives.events_ofware-func.log"
                   )
                   

###########################################################################


OPNFLD_ADD_FUNCTION(tests.primitives.variables.prod tests.primitives.variables.prod ${TEST_OUTPUT_PATH})
OPNFLD_ADD_FUNCTION(tests.primitives.variables.use tests.primitives.variables.use ${TEST_OUTPUT_PATH})
OPENFLUID_ADD_TEST(NAME functions-PrimitivesVariables 
                   COMMAND "${BIN_OUTPUT_PATH}/${OPENFLUID_CMD_APP}" 
                           "-i${TESTS_DATASETS_PATH}/OPENFLUID.IN.PrimitivesVariables"
                           "-o${TESTS_OUTPUTDATA_PATH}/OPENFLUID.OUT.PrimitivesVariables" 
                           "-p${TEST_OUTPUT_PATH}"
                           "-n${TEST_OUTPUT_PATH}"
                    PRE_TEST REMOVE_DIRECTORY "${TESTS_OUTPUTDATA_PATH}/OPENFLUID.OUT.PrimitivesVariables"
                    POST_TEST CHECK_FILE_EXIST "${TESTS_OUTPUTDATA_PATH}/OPENFLUID.OUT.PrimitivesVariables/openfluid-messages.log"
                              CHECK_FILE_EXIST "${TESTS_OUTPUTDATA_PATH}/OPENFLUID.OUT.PrimitivesVariables/tests.primitives.variables.prod_ofware-func.log"
                              CHECK_FILE_EXIST "${TESTS_OUTPUTDATA_PATH}/OPENFLUID.OUT.PrimitivesVariables/tests.primitives.variables.use_ofware-func.log"
                   )


###########################################################################


OPNFLD_ADD_FUNCTION(tests.primitives.land tests.primitives.land ${TEST_OUTPUT_PATH})
OPENFLUID_ADD_TEST(NAME functions-PrimitivesLand 
                   COMMAND "${BIN_OUTPUT_PATH}/${OPENFLUID_CMD_APP}" 
                           "-i${TESTS_DATASETS_PATH}/OPENFLUID.IN.PrimitivesLand"
                           "-o${TESTS_OUTPUTDATA_PATH}/OPENFLUID.OUT.PrimitivesLand" 
                           "-p${TEST_OUTPUT_PATH}"
                    PRE_TEST REMOVE_DIRECTORY "${TESTS_OUTPUTDATA_PATH}/OPENFLUID.OUT.PrimitivesLand"
                    POST_TEST CHECK_FILE_EXIST "${TESTS_OUTPUTDATA_PATH}/OPENFLUID.OUT.PrimitivesLand/openfluid-messages.log"
                              CHECK_FILE_EXIST "${TESTS_OUTPUTDATA_PATH}/OPENFLUID.OUT.PrimitivesLand/tests.primitives.land_ofware-func.log"                                                     
                    )


###########################################################################


OPENFLUID_ADD_TEST(NAME functions-PrimitivesAll 
                   COMMAND "${BIN_OUTPUT_PATH}/${OPENFLUID_CMD_APP}" 
                           "-i${TESTS_DATASETS_PATH}/OPENFLUID.IN.PrimitivesAll"
                           "-o${TESTS_OUTPUTDATA_PATH}/OPENFLUID.OUT.PrimitivesAll" 
                           "-p${TEST_OUTPUT_PATH}"
                    PRE_TEST REMOVE_DIRECTORY "${TESTS_OUTPUTDATA_PATH}/OPENFLUID.OUT.PrimitivesAll"
                    )


###########################################################################


OPNFLD_ADD_FUNCTION(tests.deltaTtime.prod tests.deltaTtime.prod ${TEST_OUTPUT_PATH})
OPNFLD_ADD_FUNCTION(tests.variabletime.prod tests.variabletime.prod ${TEST_OUTPUT_PATH})
OPENFLUID_ADD_TEST(NAME functions-VariableTimeProd
                   COMMAND "${BIN_OUTPUT_PATH}/${OPENFLUID_CMD_APP}" 
                           "-i${TESTS_DATASETS_PATH}/OPENFLUID.IN.VariableTimeProd"
                           "-o${TESTS_OUTPUTDATA_PATH}/OPENFLUID.OUT.VariableTimeProd" 
                           "-p${TEST_OUTPUT_PATH}"
                           )


###########################################################################


OPNFLD_ADD_FUNCTION(tests.loops tests.loops ${TEST_OUTPUT_PATH})
OPENFLUID_ADD_TEST(NAME functions-Loops
                   COMMAND "${BIN_OUTPUT_PATH}/${OPENFLUID_CMD_APP}" 
                        "-i${TESTS_DATASETS_PATH}/OPENFLUID.IN.Loops"
                        "-o${TESTS_OUTPUTDATA_PATH}/OPENFLUID.OUT.Loops" 
                        "-p${TEST_OUTPUT_PATH}")


###########################################################################


OPNFLD_ADD_FUNCTION(tests.threadedloops tests.threadedloops ${TEST_OUTPUT_PATH})
TARGET_LINK_LIBRARIES(tests.threadedloops${OPENFLUID_FUNCTIONS_SUFFIX} ${GLIBMM_LIBRARIES})
OPENFLUID_ADD_TEST(NAME functions-ThreadedLoopsDefault
                   COMMAND "${BIN_OUTPUT_PATH}/${OPENFLUID_CMD_APP}" 
                        "-i${TESTS_DATASETS_PATH}/OPENFLUID.IN.ThreadedLoops"
                        "-o${TESTS_OUTPUTDATA_PATH}/OPENFLUID.OUT.ThreadedLoops" 
                        "-p${TEST_OUTPUT_PATH}")             
OPENFLUID_ADD_TEST(NAME functions-ThreadedLoops9
                   COMMAND "${BIN_OUTPUT_PATH}/${OPENFLUID_CMD_APP}" 
                        "-i${TESTS_DATASETS_PATH}/OPENFLUID.IN.ThreadedLoops"
                        "-o${TESTS_OUTPUTDATA_PATH}/OPENFLUID.OUT.ThreadedLoops" 
                        "-p${TEST_OUTPUT_PATH}"
                        "-t" "9")             


###########################################################################


OPNFLD_ADD_FUNCTION(tests.messages tests.messages ${TEST_OUTPUT_PATH})
OPENFLUID_ADD_TEST(NAME functions-Messages
                   COMMAND "${BIN_OUTPUT_PATH}/${OPENFLUID_CMD_APP}" 
                         "-i${TESTS_DATASETS_PATH}/OPENFLUID.IN.Messages"
                         "-o${TESTS_OUTPUTDATA_PATH}/OPENFLUID.OUT.Messages" 
                         "-p${TEST_OUTPUT_PATH}")
OPENFLUID_ADD_TEST(NAME functions-ManyMessages
                   COMMAND "${BIN_OUTPUT_PATH}/${OPENFLUID_CMD_APP}" 
                             "-i${TESTS_DATASETS_PATH}/OPENFLUID.IN.ManyMessages"
                             "-o${TESTS_OUTPUTDATA_PATH}/OPENFLUID.OUT.ManyMessages" 
                             "-p${TEST_OUTPUT_PATH}"
                             "-z")                              


###########################################################################


OPNFLD_ADD_FUNCTION(tests.parseunits tests.parseunits ${TEST_OUTPUT_PATH})
OPENFLUID_ADD_TEST(NAME functions-ManyUnits
                   COMMAND "${BIN_OUTPUT_PATH}/${OPENFLUID_CMD_APP}" 
                           "-i${TESTS_DATASETS_PATH}/OPENFLUID.IN.ManyUnits"
                           "-o${TESTS_OUTPUTDATA_PATH}/OPENFLUID.OUT.ManyUnits" 
                           "-p${TEST_OUTPUT_PATH}")                              
             

###########################################################################

                         
OPNFLD_ADD_FUNCTION(tests.ofefunc2doc tests.ofefunc2doc ${TEST_OUTPUT_PATH})


###########################################################################


OPNFLD_ADD_FUNCTION(tests.tools tests.tools ${TEST_OUTPUT_PATH})
OPENFLUID_ADD_TEST(NAME functions-Tools
                   COMMAND "${BIN_OUTPUT_PATH}/${OPENFLUID_CMD_APP}" 
                        "-i${TESTS_DATASETS_PATH}/OPENFLUID.IN.Tools"
                        "-o${TESTS_OUTPUTDATA_PATH}/OPENFLUID.OUT.Tools" 
                        "-p${TEST_OUTPUT_PATH}")                 


###########################################################################

                                     
OPNFLD_ADD_FUNCTION(tests.fakefunction tests.fakefunction ${TEST_OUTPUT_PATH})


###########################################################################


OPNFLD_ADD_FUNCTION(tests.wrongprod tests.wrongprod ${TEST_OUTPUT_PATH})
OPENFLUID_ADD_TEST(NAME functions-WrongProduction
                   COMMAND "${BIN_OUTPUT_PATH}/${OPENFLUID_CMD_APP}" 
                        "-i${TESTS_DATASETS_PATH}/OPENFLUID.IN.WrongProd"
                        "-o${TESTS_OUTPUTDATA_PATH}/OPENFLUID.OUT.WrongProd" 
                        "-p${TEST_OUTPUT_PATH}")                              
SET_TESTS_PROPERTIES(functions-WrongProduction PROPERTIES WILL_FAIL TRUE)        


###########################################################################


OPNFLD_ADD_FUNCTION(tests.globalparams tests.globalparams ${TEST_OUTPUT_PATH})
OPENFLUID_ADD_TEST(NAME functions-GlobalParams
                   COMMAND "${BIN_OUTPUT_PATH}/${OPENFLUID_CMD_APP}" 
                        "-i${TESTS_DATASETS_PATH}/OPENFLUID.IN.GlobalParams"
                        "-o${TESTS_OUTPUTDATA_PATH}/OPENFLUID.OUT.GlobalParams" 
                        "-p${TEST_OUTPUT_PATH}")                              


###########################################################################

        
OPNFLD_ADD_FUNCTION(tests.generators tests.generators ${TEST_OUTPUT_PATH})
OPENFLUID_ADD_TEST(NAME functions-Generators
                   COMMAND "${BIN_OUTPUT_PATH}/${OPENFLUID_CMD_APP}" 
                        "-i${TESTS_DATASETS_PATH}/OPENFLUID.IN.Generators"
                        "-o${TESTS_OUTPUTDATA_PATH}/OPENFLUID.OUT.Generators" 
                        "-p${TEST_OUTPUT_PATH}" "-v")                              


###########################################################################


OPNFLD_ADD_FUNCTION(tests.logger tests.logger ${TEST_OUTPUT_PATH})
OPENFLUID_ADD_TEST(NAME functions-Logger
                   COMMAND "${BIN_OUTPUT_PATH}/${OPENFLUID_CMD_APP}" 
                        "-i${TESTS_DATASETS_PATH}/OPENFLUID.IN.Logger"
                        "-o${TESTS_OUTPUTDATA_PATH}/OPENFLUID.OUT.Logger" 
                        "-p${TEST_OUTPUT_PATH}"
                        "-n${TEST_OUTPUT_PATH}")                              


###########################################################################


OPNFLD_ADD_FUNCTION(tests.debug tests.debug ${TEST_OUTPUT_PATH})
OPENFLUID_ADD_TEST(NAME functions-Debug
                   COMMAND "${BIN_OUTPUT_PATH}/${OPENFLUID_CMD_APP}" 
                        "-i${TESTS_DATASETS_PATH}/OPENFLUID.IN.Debug"
                        "-o${TESTS_OUTPUTDATA_PATH}/OPENFLUID.OUT.Debug" 
                        "-p${TEST_OUTPUT_PATH}")                              
IF(${CMAKE_BUILD_TYPE} STREQUAL "Debug")
  SET_TESTS_PROPERTIES(functions-Debug PROPERTIES PASS_REGULAR_EXPRESSION "TU#")  
ELSE(${CMAKE_BUILD_TYPE} STREQUAL "Debug")
  SET_TESTS_PROPERTIES(functions-Debug PROPERTIES FAIL_REGULAR_EXPRESSION "TU#")
ENDIF(${CMAKE_BUILD_TYPE} STREQUAL "Debug") 

                            