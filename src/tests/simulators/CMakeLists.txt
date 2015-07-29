
INCLUDE_DIRECTORIES("${PROJECT_SOURCE_DIR}/src" "${PROJECT_BINARY_DIR}/src")


###########################################################################

OPNFLD_ADD_SIMULATOR(tests.coupling.A ${TEST_OUTPUT_PATH})
OPNFLD_ADD_SIMULATOR(tests.coupling.B ${TEST_OUTPUT_PATH})
OPNFLD_ADD_SIMULATOR(tests.coupling.C ${TEST_OUTPUT_PATH})
OPNFLD_ADD_SIMULATOR(tests.coupling.D ${TEST_OUTPUT_PATH})
OPNFLD_ADD_SIMULATOR(tests.coupling.E ${TEST_OUTPUT_PATH})
                                                   

###########################################################################


IF(BUILD_FORTRAN_SIMULATORS)
  ENABLE_LANGUAGE(Fortran)
  ADD_SUBDIRECTORY(tests.fortran)  
  OPENFLUID_ADD_TEST(NAME simulators-Fortran
                     COMMAND "${BIN_OUTPUT_PATH}/${OPENFLUID_CMD_APP}"
                        "run"
                        "${TESTS_DATASETS_PATH}/OPENFLUID.IN.Fortran"
                        "${TESTS_OUTPUTDATA_PATH}/OPENFLUID.OUT.Fortran" 
                        "-p" "${TEST_OUTPUT_PATH}"
                     PRE_TEST REMOVE_DIRECTORY "${TESTS_OUTPUTDATA_PATH}/OPENFLUID.OUT.Fortran")
                     
  ADD_SUBDIRECTORY(tests.fortran90)  
  OPENFLUID_ADD_TEST(NAME simulators-Fortran90
                     COMMAND "${BIN_OUTPUT_PATH}/${OPENFLUID_CMD_APP}" 
                        "run"
                        "${TESTS_DATASETS_PATH}/OPENFLUID.IN.Fortran90"
                        "${TESTS_OUTPUTDATA_PATH}/OPENFLUID.OUT.Fortran90" 
                        "-p" "${TEST_OUTPUT_PATH}"
                     PRE_TEST REMOVE_DIRECTORY "${TESTS_OUTPUTDATA_PATH}/OPENFLUID.OUT.Fortran90")                       
ENDIF(BUILD_FORTRAN_SIMULATORS)


###########################################################################


OPNFLD_ADD_SIMULATOR(tests.exceptions ${TEST_OUTPUT_PATH})
OPENFLUID_ADD_TEST(NAME simulators-Exceptions 
                   COMMAND "${BIN_OUTPUT_PATH}/${OPENFLUID_CMD_APP}"
                           "run" 
                           "${TESTS_DATASETS_PATH}/OPENFLUID.IN.SimExceptions"
                           "${TESTS_OUTPUTDATA_PATH}/OPENFLUID.OUT.SimExceptions" 
                           "-p" "${TEST_OUTPUT_PATH}"
                           "-n" "${TEST_OUTPUT_PATH}"
                    PRE_TEST REMOVE_DIRECTORY "${TESTS_OUTPUTDATA_PATH}/OPENFLUID.OUT.SimExceptions"
                    POST_TEST CHECK_FILE_EXIST "${TESTS_OUTPUTDATA_PATH}/OPENFLUID.OUT.SimExceptions/openfluid-messages.log"
                   )
SET_TESTS_PROPERTIES(simulators-Exceptions PROPERTIES WILL_FAIL TRUE)


###########################################################################


OPNFLD_ADD_SIMULATOR(tests.primitives.runenv ${TEST_OUTPUT_PATH})
OPENFLUID_ADD_TEST(NAME simulators-PrimitivesRunEnv 
                   COMMAND "${BIN_OUTPUT_PATH}/${OPENFLUID_CMD_APP}"
                           "run" 
                           "${TESTS_DATASETS_PATH}/OPENFLUID.IN.PrimitivesRunEnv"
                           "${TESTS_OUTPUTDATA_PATH}/OPENFLUID.OUT.PrimitivesRunEnv" 
                           "-p" "${TEST_OUTPUT_PATH}"
                           "-n" "${TEST_OUTPUT_PATH}"
                    PRE_TEST REMOVE_DIRECTORY "${TESTS_OUTPUTDATA_PATH}/OPENFLUID.OUT.PrimitivesRunEnv"
                    POST_TEST CHECK_FILE_EXIST "${TESTS_OUTPUTDATA_PATH}/OPENFLUID.OUT.PrimitivesRunEnv/openfluid-messages.log"
                   )


###########################################################################


OPNFLD_ADD_SIMULATOR(tests.primitives.time ${TEST_OUTPUT_PATH})
OPENFLUID_ADD_TEST(NAME simulators-PrimitivesTime 
                   COMMAND "${BIN_OUTPUT_PATH}/${OPENFLUID_CMD_APP}" 
                           "run"
                           "${TESTS_DATASETS_PATH}/OPENFLUID.IN.PrimitivesTime"
                           "${TESTS_OUTPUTDATA_PATH}/OPENFLUID.OUT.PrimitivesTime" 
                           "-p" "${TEST_OUTPUT_PATH}"
                           "-n" "${TEST_OUTPUT_PATH}"
                    PRE_TEST REMOVE_DIRECTORY "${TESTS_OUTPUTDATA_PATH}/OPENFLUID.OUT.PrimitivesTime"
                    POST_TEST CHECK_FILE_EXIST "${TESTS_OUTPUTDATA_PATH}/OPENFLUID.OUT.PrimitivesTime/openfluid-messages.log"
                   )


###########################################################################


OPNFLD_ADD_SIMULATOR(tests.primitives.simparams ${TEST_OUTPUT_PATH})
OPENFLUID_ADD_TEST(NAME simulators-PrimitivesSimParams 
                   COMMAND "${BIN_OUTPUT_PATH}/${OPENFLUID_CMD_APP}" 
                           "run"
                           "${TESTS_DATASETS_PATH}/OPENFLUID.IN.PrimitivesSimParams"
                           "${TESTS_OUTPUTDATA_PATH}/OPENFLUID.OUT.PrimitivesSimParams" 
                           "-p" "${TEST_OUTPUT_PATH}"
                           "-n" "${TEST_OUTPUT_PATH}"
                    PRE_TEST REMOVE_DIRECTORY "${TESTS_OUTPUTDATA_PATH}/OPENFLUID.OUT.PrimitivesSimParams"
                    POST_TEST CHECK_FILE_EXIST "${TESTS_OUTPUTDATA_PATH}/OPENFLUID.OUT.PrimitivesSimParams/openfluid-messages.log"
                   )


###########################################################################


OPNFLD_ADD_SIMULATOR(tests.primitives.attributes.prod ${TEST_OUTPUT_PATH})
OPNFLD_ADD_SIMULATOR(tests.primitives.attributes.use ${TEST_OUTPUT_PATH})
OPENFLUID_ADD_TEST(NAME simulators-PrimitivesAttributes 
                   COMMAND "${BIN_OUTPUT_PATH}/${OPENFLUID_CMD_APP}" 
                           "run"
                           "${TESTS_DATASETS_PATH}/OPENFLUID.IN.PrimitivesAttributes"
                           "${TESTS_OUTPUTDATA_PATH}/OPENFLUID.OUT.PrimitivesAttributes" 
                           "-p" "${TEST_OUTPUT_PATH}"
                           "-n" "${TEST_OUTPUT_PATH}"
                    PRE_TEST REMOVE_DIRECTORY "${TESTS_OUTPUTDATA_PATH}/OPENFLUID.OUT.PrimitivesAttributes"
                    POST_TEST CHECK_FILE_EXIST "${TESTS_OUTPUTDATA_PATH}/OPENFLUID.OUT.PrimitivesAttributes/openfluid-messages.log"
                   )


###########################################################################


OPNFLD_ADD_SIMULATOR(tests.primitives.events ${TEST_OUTPUT_PATH})
OPENFLUID_ADD_TEST(NAME simulators-PrimitivesEvents 
                   COMMAND "${BIN_OUTPUT_PATH}/${OPENFLUID_CMD_APP}" 
                           "run"
                           "${TESTS_DATASETS_PATH}/OPENFLUID.IN.PrimitivesEvents"
                           "${TESTS_OUTPUTDATA_PATH}/OPENFLUID.OUT.PrimitivesEvents" 
                           "-p" "${TEST_OUTPUT_PATH}"
                           "-n" "${TEST_OUTPUT_PATH}"
                    PRE_TEST REMOVE_DIRECTORY "${TESTS_OUTPUTDATA_PATH}/OPENFLUID.OUT.PrimitivesEvents"
                    POST_TEST CHECK_FILE_EXIST "${TESTS_OUTPUTDATA_PATH}/OPENFLUID.OUT.PrimitivesEvents/openfluid-messages.log"
                   )
                   

###########################################################################


OPNFLD_ADD_SIMULATOR(tests.primitives.variables.prod ${TEST_OUTPUT_PATH})
OPNFLD_ADD_SIMULATOR(tests.primitives.variables.use ${TEST_OUTPUT_PATH})
OPENFLUID_ADD_TEST(NAME simulators-PrimitivesVariables 
                   COMMAND "${BIN_OUTPUT_PATH}/${OPENFLUID_CMD_APP}" 
                           "run"
                           "${TESTS_DATASETS_PATH}/OPENFLUID.IN.PrimitivesVariables"
                           "${TESTS_OUTPUTDATA_PATH}/OPENFLUID.OUT.PrimitivesVariables" 
                           "-p" "${TEST_OUTPUT_PATH}"
                           "-n" "${TEST_OUTPUT_PATH}"
                    PRE_TEST REMOVE_DIRECTORY "${TESTS_OUTPUTDATA_PATH}/OPENFLUID.OUT.PrimitivesVariables"
                    POST_TEST CHECK_FILE_EXIST "${TESTS_OUTPUTDATA_PATH}/OPENFLUID.OUT.PrimitivesVariables/openfluid-messages.log"
                   )


###########################################################################


OPNFLD_ADD_SIMULATOR(tests.primitives.land ${TEST_OUTPUT_PATH})
OPNFLD_ADD_SIMULATOR(tests.primitives.land.prod ${TEST_OUTPUT_PATH})
OPNFLD_ADD_SIMULATOR(tests.primitives.land.use ${TEST_OUTPUT_PATH})
OPENFLUID_ADD_TEST(NAME simulators-PrimitivesLand 
                   COMMAND "${BIN_OUTPUT_PATH}/${OPENFLUID_CMD_APP}" 
                           "run"
                           "${TESTS_DATASETS_PATH}/OPENFLUID.IN.PrimitivesLand"
                           "${TESTS_OUTPUTDATA_PATH}/OPENFLUID.OUT.PrimitivesLand" 
                           "-p" "${TEST_OUTPUT_PATH}"
                    PRE_TEST REMOVE_DIRECTORY "${TESTS_OUTPUTDATA_PATH}/OPENFLUID.OUT.PrimitivesLand"
                    POST_TEST CHECK_FILE_EXIST "${TESTS_OUTPUTDATA_PATH}/OPENFLUID.OUT.PrimitivesLand/openfluid-messages.log"                                                     
                    )


###########################################################################


OPENFLUID_ADD_TEST(NAME simulators-PrimitivesAll 
                   COMMAND "${BIN_OUTPUT_PATH}/${OPENFLUID_CMD_APP}" 
                           "run"
                           "${TESTS_DATASETS_PATH}/OPENFLUID.IN.PrimitivesAll"
                           "${TESTS_OUTPUTDATA_PATH}/OPENFLUID.OUT.PrimitivesAll" 
                           "-p" "${TEST_OUTPUT_PATH}"
                    PRE_TEST REMOVE_DIRECTORY "${TESTS_OUTPUTDATA_PATH}/OPENFLUID.OUT.PrimitivesAll"
                    )


###########################################################################


OPENFLUID_ADD_TEST(NAME simulators-PrimitivesWithGhosts 
                   COMMAND "${BIN_OUTPUT_PATH}/${OPENFLUID_CMD_APP}" 
                           "run"
                           "${TESTS_DATASETS_PATH}/OPENFLUID.IN.PrimitivesWithGhosts"
                           "${TESTS_OUTPUTDATA_PATH}/OPENFLUID.OUT.PrimitivesWithGhosts" 
                           "-p" "${TEST_OUTPUT_PATH}:${TESTS_MISCDATA_PATH}"
                    PRE_TEST REMOVE_DIRECTORY "${TESTS_OUTPUTDATA_PATH}/OPENFLUID.OUT.PrimitivesWithGhosts"
                    )
SET_TESTS_PROPERTIES(simulators-PrimitivesWithGhosts PROPERTIES WILL_FAIL TRUE)


###########################################################################


OPNFLD_ADD_SIMULATOR(tests.deltaTtime.prod ${TEST_OUTPUT_PATH})
OPNFLD_ADD_SIMULATOR(tests.variabletime.prod ${TEST_OUTPUT_PATH})
OPENFLUID_ADD_TEST(NAME simulators-VariableTimeProd
                   COMMAND "${BIN_OUTPUT_PATH}/${OPENFLUID_CMD_APP}" 
                           "run"
                           "${TESTS_DATASETS_PATH}/OPENFLUID.IN.VariableTimeProd"
                           "${TESTS_OUTPUTDATA_PATH}/OPENFLUID.OUT.VariableTimeProd" 
                           "-p" "${TEST_OUTPUT_PATH}"
                           )


###########################################################################


OPNFLD_ADD_SIMULATOR(tests.loops ${TEST_OUTPUT_PATH})
OPENFLUID_ADD_TEST(NAME simulators-Loops
                   COMMAND "${BIN_OUTPUT_PATH}/${OPENFLUID_CMD_APP}" 
                        "run"
                        "${TESTS_DATASETS_PATH}/OPENFLUID.IN.Loops"
                        "${TESTS_OUTPUTDATA_PATH}/OPENFLUID.OUT.Loops" 
                        "-p" "${TEST_OUTPUT_PATH}")


###########################################################################


OPNFLD_ADD_SIMULATOR(tests.threadedloops ${TEST_OUTPUT_PATH})
OPENFLUID_ADD_TEST(NAME simulators-ThreadedLoopsDefault
                   COMMAND "${BIN_OUTPUT_PATH}/${OPENFLUID_CMD_APP}" 
                        "run"
                        "${TESTS_DATASETS_PATH}/OPENFLUID.IN.ThreadedLoops"
                        "${TESTS_OUTPUTDATA_PATH}/OPENFLUID.OUT.ThreadedLoops" 
                        "-p" "${TEST_OUTPUT_PATH}")             
OPENFLUID_ADD_TEST(NAME simulators-ThreadedLoops9
                   COMMAND "${BIN_OUTPUT_PATH}/${OPENFLUID_CMD_APP}" 
                        "run"
                        "${TESTS_DATASETS_PATH}/OPENFLUID.IN.ThreadedLoops"
                        "${TESTS_OUTPUTDATA_PATH}/OPENFLUID.OUT.ThreadedLoops" 
                        "-p" "${TEST_OUTPUT_PATH}"
                        "-t" "9")             
                        

###########################################################################


OPENFLUID_ADD_TEST(NAME simulators-DeltaTChecked
                   COMMAND "${BIN_OUTPUT_PATH}/${OPENFLUID_CMD_APP}" 
                           "run"
                           "${TESTS_DATASETS_PATH}/OPENFLUID.IN.DeltaTChecked"
                           "${TESTS_OUTPUTDATA_PATH}/OPENFLUID.OUT.DeltaTChecked" 
                           "-p" "${TEST_OUTPUT_PATH}"
                           )


###########################################################################


OPENFLUID_ADD_TEST(NAME simulators-DeltaTCheckedFails
                   COMMAND "${BIN_OUTPUT_PATH}/${OPENFLUID_CMD_APP}" 
                           "run"
                           "${TESTS_DATASETS_PATH}/OPENFLUID.IN.DeltaTCheckedFails"
                           "${TESTS_OUTPUTDATA_PATH}/OPENFLUID.OUT.DeltaTCheckedFails" 
                           "-p" "${TEST_OUTPUT_PATH}"
                           )
SET_TESTS_PROPERTIES(simulators-DeltaTCheckedFails PROPERTIES WILL_FAIL TRUE)


###########################################################################


OPENFLUID_ADD_TEST(NAME simulators-DeltaTForced
                   COMMAND "${BIN_OUTPUT_PATH}/${OPENFLUID_CMD_APP}" 
                           "run"
                           "${TESTS_DATASETS_PATH}/OPENFLUID.IN.DeltaTForced"
                           "${TESTS_OUTPUTDATA_PATH}/OPENFLUID.OUT.DeltaTForced" 
                           "-p" "${TEST_OUTPUT_PATH}"
                           )


###########################################################################


OPENFLUID_ADD_TEST(NAME simulators-WithBuffer
                   COMMAND "${BIN_OUTPUT_PATH}/${OPENFLUID_CMD_APP}" 
                           "run"
                           "${TESTS_DATASETS_PATH}/OPENFLUID.IN.CouplingWithBuffer"
                           "${TESTS_OUTPUTDATA_PATH}/OPENFLUID.OUT.CouplingWithBuffer" 
                           "-p" "${TEST_OUTPUT_PATH}"
                           )
  

###########################################################################


OPENFLUID_ADD_TEST(NAME simulators-WithSmallBuffer
                   COMMAND "${BIN_OUTPUT_PATH}/${OPENFLUID_CMD_APP}" 
                           "run"
                           "${TESTS_DATASETS_PATH}/OPENFLUID.IN.CouplingWithSmallBuffer"
                           "${TESTS_OUTPUTDATA_PATH}/OPENFLUID.OUT.CouplingWithSmallBuffer" 
                           "-p" "${TEST_OUTPUT_PATH}"
                           )
SET_TESTS_PROPERTIES(simulators-WithSmallBuffer PROPERTIES PASS_REGULAR_EXPRESSION "buffers for variables is set to 2")


###########################################################################


OPNFLD_ADD_SIMULATOR(tests.messages ${TEST_OUTPUT_PATH})
OPENFLUID_ADD_TEST(NAME simulators-Messages
                   COMMAND "${BIN_OUTPUT_PATH}/${OPENFLUID_CMD_APP}" 
                         "run"
                         "${TESTS_DATASETS_PATH}/OPENFLUID.IN.Messages"
                         "${TESTS_OUTPUTDATA_PATH}/OPENFLUID.OUT.Messages" 
                         "-p" "${TEST_OUTPUT_PATH}")
OPENFLUID_ADD_TEST(NAME simulators-ManyMessages
                   COMMAND "${BIN_OUTPUT_PATH}/${OPENFLUID_CMD_APP}" 
                             "run"
                             "${TESTS_DATASETS_PATH}/OPENFLUID.IN.ManyMessages"
                             "${TESTS_OUTPUTDATA_PATH}/OPENFLUID.OUT.ManyMessages" 
                             "-p" "${TEST_OUTPUT_PATH}")                              


###########################################################################


OPNFLD_ADD_SIMULATOR(tests.parseunits ${TEST_OUTPUT_PATH})
OPENFLUID_ADD_TEST(NAME simulators-ManyUnits
                   COMMAND "${BIN_OUTPUT_PATH}/${OPENFLUID_CMD_APP}" 
                           "run"
                           "${TESTS_DATASETS_PATH}/OPENFLUID.IN.ManyUnits"
                           "${TESTS_OUTPUTDATA_PATH}/OPENFLUID.OUT.ManyUnits" 
                           "-p" "${TEST_OUTPUT_PATH}")                              
             

###########################################################################

                         
OPNFLD_ADD_SIMULATOR(tests.sim2doc ${TEST_OUTPUT_PATH})


###########################################################################


OPNFLD_ADD_SIMULATOR(tests.tools ${TEST_OUTPUT_PATH})
OPENFLUID_ADD_TEST(NAME simulators-Tools
                   COMMAND "${BIN_OUTPUT_PATH}/${OPENFLUID_CMD_APP}" 
                        "run"
                        "${TESTS_DATASETS_PATH}/OPENFLUID.IN.Tools"
                        "${TESTS_OUTPUTDATA_PATH}/OPENFLUID.OUT.Tools" 
                        "-p" "${TEST_OUTPUT_PATH}")                 


###########################################################################

                                     
OPNFLD_ADD_SIMULATOR(tests.fakesimulator ${TEST_OUTPUT_PATH})


###########################################################################


OPNFLD_ADD_SIMULATOR(tests.wrongprod tests.wrongprod ${TEST_OUTPUT_PATH} OFF)
OPENFLUID_ADD_TEST(NAME simulators-WrongProduction
                   COMMAND "${BIN_OUTPUT_PATH}/${OPENFLUID_CMD_APP}" 
                        "run"
                        "${TESTS_DATASETS_PATH}/OPENFLUID.IN.WrongProd"
                        "${TESTS_OUTPUTDATA_PATH}/OPENFLUID.OUT.WrongProd" 
                        "-p" "${TEST_OUTPUT_PATH}")                              
SET_TESTS_PROPERTIES(simulators-WrongProduction PROPERTIES WILL_FAIL TRUE)        


###########################################################################


OPNFLD_ADD_SIMULATOR(tests.globalparams ${TEST_OUTPUT_PATH})
OPENFLUID_ADD_TEST(NAME simulators-GlobalParams
                   COMMAND "${BIN_OUTPUT_PATH}/${OPENFLUID_CMD_APP}" 
                        "run"
                        "${TESTS_DATASETS_PATH}/OPENFLUID.IN.GlobalParams"
                        "${TESTS_OUTPUTDATA_PATH}/OPENFLUID.OUT.GlobalParams" 
                        "-p" "${TEST_OUTPUT_PATH}")                              


###########################################################################

        
OPNFLD_ADD_SIMULATOR(tests.generators ${TEST_OUTPUT_PATH})
OPENFLUID_ADD_TEST(NAME simulators-Generators
                   COMMAND "${BIN_OUTPUT_PATH}/${OPENFLUID_CMD_APP}" 
                        "run"
                        "${TESTS_DATASETS_PATH}/OPENFLUID.IN.Generators"
                        "${TESTS_OUTPUTDATA_PATH}/OPENFLUID.OUT.Generators" 
                        "-p" "${TEST_OUTPUT_PATH}" "-v")                              


###########################################################################


OPNFLD_ADD_SIMULATOR(tests.logger ${TEST_OUTPUT_PATH})
OPENFLUID_ADD_TEST(NAME simulators-Logger
                   COMMAND "${BIN_OUTPUT_PATH}/${OPENFLUID_CMD_APP}" 
                        "run"
                        "${TESTS_DATASETS_PATH}/OPENFLUID.IN.Logger"
                        "${TESTS_OUTPUTDATA_PATH}/OPENFLUID.OUT.Logger" 
                        "-p" "${TEST_OUTPUT_PATH}"
                        "-n" "${TEST_OUTPUT_PATH}")                              


###########################################################################


OPNFLD_ADD_SIMULATOR(tests.debug ${TEST_OUTPUT_PATH})
OPENFLUID_ADD_TEST(NAME simulators-Debug
                   COMMAND "${BIN_OUTPUT_PATH}/${OPENFLUID_CMD_APP}" 
                        "run"
                        "${TESTS_DATASETS_PATH}/OPENFLUID.IN.Debug"
                        "${TESTS_OUTPUTDATA_PATH}/OPENFLUID.OUT.Debug" 
                        "-p" "${TEST_OUTPUT_PATH}")                              
IF(${CMAKE_BUILD_TYPE} STREQUAL "Debug")
  SET_TESTS_PROPERTIES(simulators-Debug PROPERTIES PASS_REGULAR_EXPRESSION "TU#")  
ELSE(${CMAKE_BUILD_TYPE} STREQUAL "Debug")
  SET_TESTS_PROPERTIES(simulators-Debug PROPERTIES FAIL_REGULAR_EXPRESSION "TU#")
ENDIF(${CMAKE_BUILD_TYPE} STREQUAL "Debug") 


###########################################################################


OPNFLD_ADD_SIMULATOR(tests.wrongid ${TEST_OUTPUT_PATH})
OPENFLUID_ADD_TEST(NAME simulators-WrongID 
                   COMMAND "${BIN_OUTPUT_PATH}/${OPENFLUID_CMD_APP}" 
                                  "report" "simulators" "--list"
                                  "-p" "${TEST_OUTPUT_PATH}")
SET_TESTS_PROPERTIES(simulators-WrongID PROPERTIES FAIL_REGULAR_EXPRESSION "tests.wrongid")


###########################################################################


OPNFLD_ADD_SIMULATOR(tests.wrongversion ${TEST_OUTPUT_PATH})
OPENFLUID_ADD_TEST(NAME simulators-WrongVersion 
                   COMMAND "${BIN_OUTPUT_PATH}/${OPENFLUID_CMD_APP}" 
                                  "report" "simulators" "--list"
                                  "-p" "${TEST_OUTPUT_PATH}")
SET_TESTS_PROPERTIES(simulators-WrongVersion PROPERTIES FAIL_REGULAR_EXPRESSION "tests.wrongversion")

                            