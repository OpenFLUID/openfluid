
INCLUDE_DIRECTORIES("${PROJECT_SOURCE_DIR}/src" "${PROJECT_BINARY_DIR}/src")


###########################################################################


OFBUILD_ADD_SIMULATOR(tests.coupling.A ${OFBUILD_TESTS_BINARY_DIR})
OFBUILD_ADD_SIMULATOR(tests.coupling.B ${OFBUILD_TESTS_BINARY_DIR})
OFBUILD_ADD_SIMULATOR(tests.coupling.C ${OFBUILD_TESTS_BINARY_DIR})
OFBUILD_ADD_SIMULATOR(tests.coupling.D ${OFBUILD_TESTS_BINARY_DIR})
OFBUILD_ADD_SIMULATOR(tests.coupling.E ${OFBUILD_TESTS_BINARY_DIR})

OFBUILD_ADD_SIMULATOR(tests.dotobs ${OFBUILD_TESTS_BINARY_DIR})

OFBUILD_ADD_SIMULATOR(tests.generators.cycle.prod ${OFBUILD_TESTS_BINARY_DIR})
OFBUILD_ADD_SIMULATOR(tests.generators.cycle.use ${OFBUILD_TESTS_BINARY_DIR})
                                                   

###########################################################################


IF(OFBUILD_TESTS_ENABLE_FORTRAN)
  ENABLE_LANGUAGE(Fortran)

  OFBUILD_ADD_SIMULATOR(tests.fortran ${OFBUILD_TESTS_BINARY_DIR})
  OPENFLUID_ADD_TEST(NAME simulators-Fortran
                     COMMAND "${OFBUILD_DIST_BIN_DIR}/${OPENFLUID_CMD_APP}"
                        "run"
                        "${OFBUILD_TESTS_INPUT_DATASETS_DIR}/OPENFLUID.IN.Fortran"
                        "${OFBUILD_TESTS_OUTPUT_DATA_DIR}/OPENFLUID.OUT.Fortran" 
                        "-p" "${OFBUILD_TESTS_BINARY_DIR}"
                     PRE_TEST REMOVE_DIRECTORY "${OFBUILD_TESTS_OUTPUT_DATA_DIR}/OPENFLUID.OUT.Fortran")
                     
  OFBUILD_ADD_SIMULATOR(tests.fortran90 ${OFBUILD_TESTS_BINARY_DIR})
  OPENFLUID_ADD_TEST(NAME simulators-Fortran90
                     COMMAND "${OFBUILD_DIST_BIN_DIR}/${OPENFLUID_CMD_APP}" 
                        "run"
                        "${OFBUILD_TESTS_INPUT_DATASETS_DIR}/OPENFLUID.IN.Fortran90"
                        "${OFBUILD_TESTS_OUTPUT_DATA_DIR}/OPENFLUID.OUT.Fortran90" 
                        "-p" "${OFBUILD_TESTS_BINARY_DIR}"
                     PRE_TEST REMOVE_DIRECTORY "${OFBUILD_TESTS_OUTPUT_DATA_DIR}/OPENFLUID.OUT.Fortran90")                       
ENDIF()


###########################################################################


IF(OFBUILD_TESTS_ENABLE_EMBEDDING_R)
  OFBUILD_ADD_SIMULATOR(tests.embedding.R ${OFBUILD_TESTS_BINARY_DIR})
  OPENFLUID_ADD_TEST(NAME simulators-Embedding-R
                     COMMAND "${OFBUILD_DIST_BIN_DIR}/${OPENFLUID_CMD_APP}" 
                             "run"
                             "${OFBUILD_TESTS_INPUT_DATASETS_DIR}/OPENFLUID.IN.R"
                             "${OFBUILD_TESTS_OUTPUT_DATA_DIR}/OPENFLUID.OUT.R" 
                             "-p" "${OFBUILD_TESTS_BINARY_DIR}")
ENDIF()


###########################################################################


OFBUILD_ADD_SIMULATOR(tests.exceptions ${OFBUILD_TESTS_BINARY_DIR})
OPENFLUID_ADD_TEST(NAME simulators-Exceptions 
                   COMMAND "${OFBUILD_DIST_BIN_DIR}/${OPENFLUID_CMD_APP}"
                           "run" 
                           "${OFBUILD_TESTS_INPUT_DATASETS_DIR}/OPENFLUID.IN.SimExceptions"
                           "${OFBUILD_TESTS_OUTPUT_DATA_DIR}/OPENFLUID.OUT.SimExceptions" 
                           "-p" "${OFBUILD_TESTS_BINARY_DIR}"
                           "-n" "${OFBUILD_TESTS_BINARY_DIR}"
                    PRE_TEST REMOVE_DIRECTORY "${OFBUILD_TESTS_OUTPUT_DATA_DIR}/OPENFLUID.OUT.SimExceptions"
                    POST_TEST CHECK_FILE_EXIST "${OFBUILD_TESTS_OUTPUT_DATA_DIR}/OPENFLUID.OUT.SimExceptions/openfluid-messages.log"
                   )
SET_TESTS_PROPERTIES(simulators-Exceptions PROPERTIES WILL_FAIL TRUE)


###########################################################################


OFBUILD_ADD_SIMULATOR(tests.primitives.runenv ${OFBUILD_TESTS_BINARY_DIR})
OPENFLUID_ADD_TEST(NAME simulators-PrimitivesRunEnv 
                   COMMAND "${OFBUILD_DIST_BIN_DIR}/${OPENFLUID_CMD_APP}"
                           "run" 
                           "${OFBUILD_TESTS_INPUT_DATASETS_DIR}/OPENFLUID.IN.PrimitivesRunEnv"
                           "${OFBUILD_TESTS_OUTPUT_DATA_DIR}/OPENFLUID.OUT.PrimitivesRunEnv" 
                           "-p" "${OFBUILD_TESTS_BINARY_DIR}"
                           "-n" "${OFBUILD_TESTS_BINARY_DIR}"
                    PRE_TEST REMOVE_DIRECTORY "${OFBUILD_TESTS_OUTPUT_DATA_DIR}/OPENFLUID.OUT.PrimitivesRunEnv"
                    POST_TEST CHECK_FILE_EXIST "${OFBUILD_TESTS_OUTPUT_DATA_DIR}/OPENFLUID.OUT.PrimitivesRunEnv/openfluid-messages.log"
                   )


###########################################################################


OFBUILD_ADD_SIMULATOR(tests.primitives.time ${OFBUILD_TESTS_BINARY_DIR})
OPENFLUID_ADD_TEST(NAME simulators-PrimitivesTime 
                   COMMAND "${OFBUILD_DIST_BIN_DIR}/${OPENFLUID_CMD_APP}" 
                           "run"
                           "${OFBUILD_TESTS_INPUT_DATASETS_DIR}/OPENFLUID.IN.PrimitivesTime"
                           "${OFBUILD_TESTS_OUTPUT_DATA_DIR}/OPENFLUID.OUT.PrimitivesTime" 
                           "-p" "${OFBUILD_TESTS_BINARY_DIR}"
                           "-n" "${OFBUILD_TESTS_BINARY_DIR}"
                    PRE_TEST REMOVE_DIRECTORY "${OFBUILD_TESTS_OUTPUT_DATA_DIR}/OPENFLUID.OUT.PrimitivesTime"
                    POST_TEST CHECK_FILE_EXIST "${OFBUILD_TESTS_OUTPUT_DATA_DIR}/OPENFLUID.OUT.PrimitivesTime/openfluid-messages.log"
                   )


###########################################################################


OFBUILD_ADD_SIMULATOR(tests.primitives.simparams ${OFBUILD_TESTS_BINARY_DIR})
OPENFLUID_ADD_TEST(NAME simulators-PrimitivesSimParams 
                   COMMAND "${OFBUILD_DIST_BIN_DIR}/${OPENFLUID_CMD_APP}" 
                           "run"
                           "${OFBUILD_TESTS_INPUT_DATASETS_DIR}/OPENFLUID.IN.PrimitivesSimParams"
                           "${OFBUILD_TESTS_OUTPUT_DATA_DIR}/OPENFLUID.OUT.PrimitivesSimParams" 
                           "-p" "${OFBUILD_TESTS_BINARY_DIR}"
                           "-n" "${OFBUILD_TESTS_BINARY_DIR}"
                    PRE_TEST REMOVE_DIRECTORY "${OFBUILD_TESTS_OUTPUT_DATA_DIR}/OPENFLUID.OUT.PrimitivesSimParams"
                    POST_TEST CHECK_FILE_EXIST "${OFBUILD_TESTS_OUTPUT_DATA_DIR}/OPENFLUID.OUT.PrimitivesSimParams/openfluid-messages.log"
                   )


###########################################################################


OFBUILD_ADD_SIMULATOR(tests.primitives.attributes.prod ${OFBUILD_TESTS_BINARY_DIR})
OFBUILD_ADD_SIMULATOR(tests.primitives.attributes.use ${OFBUILD_TESTS_BINARY_DIR})
OPENFLUID_ADD_TEST(NAME simulators-PrimitivesAttributes 
                   COMMAND "${OFBUILD_DIST_BIN_DIR}/${OPENFLUID_CMD_APP}" 
                           "run"
                           "${OFBUILD_TESTS_INPUT_DATASETS_DIR}/OPENFLUID.IN.PrimitivesAttributes"
                           "${OFBUILD_TESTS_OUTPUT_DATA_DIR}/OPENFLUID.OUT.PrimitivesAttributes" 
                           "-p" "${OFBUILD_TESTS_BINARY_DIR}"
                           "-n" "${OFBUILD_TESTS_BINARY_DIR}"
                    PRE_TEST REMOVE_DIRECTORY "${OFBUILD_TESTS_OUTPUT_DATA_DIR}/OPENFLUID.OUT.PrimitivesAttributes"
                    POST_TEST CHECK_FILE_EXIST "${OFBUILD_TESTS_OUTPUT_DATA_DIR}/OPENFLUID.OUT.PrimitivesAttributes/openfluid-messages.log"
                   )


###########################################################################


OFBUILD_ADD_SIMULATOR(tests.primitives.events ${OFBUILD_TESTS_BINARY_DIR})
OPENFLUID_ADD_TEST(NAME simulators-PrimitivesEvents 
                   COMMAND "${OFBUILD_DIST_BIN_DIR}/${OPENFLUID_CMD_APP}" 
                           "run"
                           "${OFBUILD_TESTS_INPUT_DATASETS_DIR}/OPENFLUID.IN.PrimitivesEvents"
                           "${OFBUILD_TESTS_OUTPUT_DATA_DIR}/OPENFLUID.OUT.PrimitivesEvents" 
                           "-p" "${OFBUILD_TESTS_BINARY_DIR}"
                           "-n" "${OFBUILD_TESTS_BINARY_DIR}"
                    PRE_TEST REMOVE_DIRECTORY "${OFBUILD_TESTS_OUTPUT_DATA_DIR}/OPENFLUID.OUT.PrimitivesEvents"
                    POST_TEST CHECK_FILE_EXIST "${OFBUILD_TESTS_OUTPUT_DATA_DIR}/OPENFLUID.OUT.PrimitivesEvents/openfluid-messages.log"
                   )
                   

###########################################################################


OFBUILD_ADD_SIMULATOR(tests.primitives.variables.prod ${OFBUILD_TESTS_BINARY_DIR})
OFBUILD_ADD_SIMULATOR(tests.primitives.variables.use ${OFBUILD_TESTS_BINARY_DIR})
OPENFLUID_ADD_TEST(NAME simulators-PrimitivesVariables 
                   COMMAND "${OFBUILD_DIST_BIN_DIR}/${OPENFLUID_CMD_APP}" 
                           "run"
                           "${OFBUILD_TESTS_INPUT_DATASETS_DIR}/OPENFLUID.IN.PrimitivesVariables"
                           "${OFBUILD_TESTS_OUTPUT_DATA_DIR}/OPENFLUID.OUT.PrimitivesVariables" 
                           "-p" "${OFBUILD_TESTS_BINARY_DIR}"
                           "-n" "${OFBUILD_TESTS_BINARY_DIR}"
                    PRE_TEST REMOVE_DIRECTORY "${OFBUILD_TESTS_OUTPUT_DATA_DIR}/OPENFLUID.OUT.PrimitivesVariables"
                    POST_TEST CHECK_FILE_EXIST "${OFBUILD_TESTS_OUTPUT_DATA_DIR}/OPENFLUID.OUT.PrimitivesVariables/openfluid-messages.log"
                   )


###########################################################################


OFBUILD_ADD_SIMULATOR(tests.na.deltat ${OFBUILD_TESTS_BINARY_DIR})
OFBUILD_ADD_SIMULATOR(tests.na.alternate ${OFBUILD_TESTS_BINARY_DIR})
OFBUILD_ADD_SIMULATOR(tests.na.end ${OFBUILD_TESTS_BINARY_DIR})
OFBUILD_ADD_SIMULATOR(tests.na.randomtime ${OFBUILD_TESTS_BINARY_DIR})
OFBUILD_ADD_SIMULATOR(tests.na.noupdate ${OFBUILD_TESTS_BINARY_DIR})

OFBUILD_ADD_SIMULATOR(tests.precision ${OFBUILD_TESTS_BINARY_DIR})


###########################################################################


OFBUILD_ADD_SIMULATOR(tests.primitives.land ${OFBUILD_TESTS_BINARY_DIR})
OFBUILD_ADD_SIMULATOR(tests.primitives.land.prod ${OFBUILD_TESTS_BINARY_DIR})
OFBUILD_ADD_SIMULATOR(tests.primitives.land.use ${OFBUILD_TESTS_BINARY_DIR})
OPENFLUID_ADD_TEST(NAME simulators-PrimitivesLand 
                   COMMAND "${OFBUILD_DIST_BIN_DIR}/${OPENFLUID_CMD_APP}" 
                           "run"
                           "${OFBUILD_TESTS_INPUT_DATASETS_DIR}/OPENFLUID.IN.PrimitivesLand"
                           "${OFBUILD_TESTS_OUTPUT_DATA_DIR}/OPENFLUID.OUT.PrimitivesLand" 
                           "-p" "${OFBUILD_TESTS_BINARY_DIR}"
                    PRE_TEST REMOVE_DIRECTORY "${OFBUILD_TESTS_OUTPUT_DATA_DIR}/OPENFLUID.OUT.PrimitivesLand"
                    POST_TEST CHECK_FILE_EXIST "${OFBUILD_TESTS_OUTPUT_DATA_DIR}/OPENFLUID.OUT.PrimitivesLand/openfluid-messages.log"                                                     
                    )


###########################################################################


OPENFLUID_ADD_TEST(NAME simulators-PrimitivesAll 
                   COMMAND "${OFBUILD_DIST_BIN_DIR}/${OPENFLUID_CMD_APP}" 
                           "run"
                           "${OFBUILD_TESTS_INPUT_DATASETS_DIR}/OPENFLUID.IN.PrimitivesAll"
                           "${OFBUILD_TESTS_OUTPUT_DATA_DIR}/OPENFLUID.OUT.PrimitivesAll" 
                           "-p" "${OFBUILD_TESTS_BINARY_DIR}"
                    PRE_TEST REMOVE_DIRECTORY "${OFBUILD_TESTS_OUTPUT_DATA_DIR}/OPENFLUID.OUT.PrimitivesAll"
                    )


###########################################################################


OFBUILD_ADD_SIMULATOR(tests.primitives.benchmarking ${OFBUILD_TESTS_BINARY_DIR})
OPENFLUID_ADD_TEST(NAME simulators-PrimitivesBenchmarking
                   COMMAND "${OFBUILD_DIST_BIN_DIR}/${OPENFLUID_CMD_APP}" 
                           "run"
                           "${OFBUILD_TESTS_INPUT_DATASETS_DIR}/OPENFLUID.IN.PrimitivesBenchmarking"
                           "${OFBUILD_TESTS_OUTPUT_DATA_DIR}/OPENFLUID.OUT.PrimitivesBenchmarking" 
                           "-p" "${OFBUILD_TESTS_BINARY_DIR}"
                    PRE_TEST REMOVE_DIRECTORY "${OFBUILD_TESTS_OUTPUT_DATA_DIR}/OPENFLUID.OUT.PrimitivesBenchmarking"
                    )


###########################################################################


OPENFLUID_ADD_TEST(NAME simulators-PrimitivesWithGhosts 
                   COMMAND "${OFBUILD_DIST_BIN_DIR}/${OPENFLUID_CMD_APP}" 
                           "run"
                           "${OFBUILD_TESTS_INPUT_DATASETS_DIR}/OPENFLUID.IN.PrimitivesWithGhosts"
                           "${OFBUILD_TESTS_OUTPUT_DATA_DIR}/OPENFLUID.OUT.PrimitivesWithGhosts" 
                           "-p" "${OFBUILD_TESTS_BINARY_DIR}:${OFBUILD_TESTS_INPUT_MISCDATA_DIR}"
                    PRE_TEST REMOVE_DIRECTORY "${OFBUILD_TESTS_OUTPUT_DATA_DIR}/OPENFLUID.OUT.PrimitivesWithGhosts"
                    )
SET_TESTS_PROPERTIES(simulators-PrimitivesWithGhosts PROPERTIES WILL_FAIL TRUE)


###########################################################################


OFBUILD_ADD_SIMULATOR(tests.deltaTtime.prod ${OFBUILD_TESTS_BINARY_DIR})
OFBUILD_ADD_SIMULATOR(tests.variabletime.prod ${OFBUILD_TESTS_BINARY_DIR})
OPENFLUID_ADD_TEST(NAME simulators-VariableTimeProd
                   COMMAND "${OFBUILD_DIST_BIN_DIR}/${OPENFLUID_CMD_APP}" 
                           "run"
                           "${OFBUILD_TESTS_INPUT_DATASETS_DIR}/OPENFLUID.IN.VariableTimeProd"
                           "${OFBUILD_TESTS_OUTPUT_DATA_DIR}/OPENFLUID.OUT.VariableTimeProd" 
                           "-p" "${OFBUILD_TESTS_BINARY_DIR}"
                           )


###########################################################################


OFBUILD_ADD_SIMULATOR(tests.variable.memorylimit.use ${OFBUILD_TESTS_BINARY_DIR})
OPENFLUID_ADD_TEST(NAME simulators-MemoryRange
                   COMMAND "${OFBUILD_DIST_BIN_DIR}/${OPENFLUID_CMD_APP}" 
                           "run"
                           "${OFBUILD_TESTS_INPUT_DATASETS_DIR}/OPENFLUID.IN.DataMemory"
                           "${OFBUILD_TESTS_OUTPUT_DATA_DIR}/OPENFLUID.OUT.DataMemory" 
                           "-p" "${OFBUILD_TESTS_BINARY_DIR}"
                           )
SET_TESTS_PROPERTIES(simulators-MemoryRange PROPERTIES PASS_REGULAR_EXPRESSION "VARIABLE EXISTS for deltaT x 2")
SET_TESTS_PROPERTIES(simulators-MemoryRange PROPERTIES PASS_REGULAR_EXPRESSION "VARIABLE DOES NOT EXISTS for deltaT x 3")


###########################################################################


OFBUILD_ADD_SIMULATOR(tests.loops ${OFBUILD_TESTS_BINARY_DIR})
OPENFLUID_ADD_TEST(NAME simulators-Loops
                   COMMAND "${OFBUILD_DIST_BIN_DIR}/${OPENFLUID_CMD_APP}" 
                        "run"
                        "${OFBUILD_TESTS_INPUT_DATASETS_DIR}/OPENFLUID.IN.Loops"
                        "${OFBUILD_TESTS_OUTPUT_DATA_DIR}/OPENFLUID.OUT.Loops" 
                        "-p" "${OFBUILD_TESTS_BINARY_DIR}")


###########################################################################


OFBUILD_ADD_SIMULATOR(tests.threadedloops ${OFBUILD_TESTS_BINARY_DIR})
OPENFLUID_ADD_TEST(NAME simulators-ThreadedLoopsDefault
                   COMMAND "${OFBUILD_DIST_BIN_DIR}/${OPENFLUID_CMD_APP}" 
                        "run"
                        "${OFBUILD_TESTS_INPUT_DATASETS_DIR}/OPENFLUID.IN.ThreadedLoops"
                        "${OFBUILD_TESTS_OUTPUT_DATA_DIR}/OPENFLUID.OUT.ThreadedLoops" 
                        "-p" "${OFBUILD_TESTS_BINARY_DIR}")             
OPENFLUID_ADD_TEST(NAME simulators-ThreadedLoops8
                   COMMAND "${OFBUILD_DIST_BIN_DIR}/${OPENFLUID_CMD_APP}" 
                        "run"
                        "${OFBUILD_TESTS_INPUT_DATASETS_DIR}/OPENFLUID.IN.ThreadedLoops"
                        "${OFBUILD_TESTS_OUTPUT_DATA_DIR}/OPENFLUID.OUT.ThreadedLoops" 
                        "-p" "${OFBUILD_TESTS_BINARY_DIR}"
                        "-t" "8")             
OPENFLUID_ADD_TEST(NAME simulators-ThreadedLoops4
                   COMMAND "${OFBUILD_DIST_BIN_DIR}/${OPENFLUID_CMD_APP}" 
                        "run"
                        "${OFBUILD_TESTS_INPUT_DATASETS_DIR}/OPENFLUID.IN.ThreadedLoops"
                        "${OFBUILD_TESTS_OUTPUT_DATA_DIR}/OPENFLUID.OUT.ThreadedLoops" 
                        "-p" "${OFBUILD_TESTS_BINARY_DIR}"
                        "-t" "4")
OPENFLUID_ADD_TEST(NAME simulators-ThreadedLoops1
                   COMMAND "${OFBUILD_DIST_BIN_DIR}/${OPENFLUID_CMD_APP}" 
                        "run"
                        "${OFBUILD_TESTS_INPUT_DATASETS_DIR}/OPENFLUID.IN.ThreadedLoops"
                        "${OFBUILD_TESTS_OUTPUT_DATA_DIR}/OPENFLUID.OUT.ThreadedLoops" 
                        "-p" "${OFBUILD_TESTS_BINARY_DIR}"
                        "-t" "1")                        


###########################################################################


OPENFLUID_ADD_TEST(NAME simulators-DeltaTChecked
                   COMMAND "${OFBUILD_DIST_BIN_DIR}/${OPENFLUID_CMD_APP}" 
                           "run"
                           "${OFBUILD_TESTS_INPUT_DATASETS_DIR}/OPENFLUID.IN.DeltaTChecked"
                           "${OFBUILD_TESTS_OUTPUT_DATA_DIR}/OPENFLUID.OUT.DeltaTChecked" 
                           "-p" "${OFBUILD_TESTS_BINARY_DIR}"
                           )


###########################################################################


OPENFLUID_ADD_TEST(NAME simulators-DeltaTCheckedFails
                   COMMAND "${OFBUILD_DIST_BIN_DIR}/${OPENFLUID_CMD_APP}" 
                           "run"
                           "${OFBUILD_TESTS_INPUT_DATASETS_DIR}/OPENFLUID.IN.DeltaTCheckedFails"
                           "${OFBUILD_TESTS_OUTPUT_DATA_DIR}/OPENFLUID.OUT.DeltaTCheckedFails" 
                           "-p" "${OFBUILD_TESTS_BINARY_DIR}"
                           )
SET_TESTS_PROPERTIES(simulators-DeltaTCheckedFails PROPERTIES WILL_FAIL TRUE)


###########################################################################


OPENFLUID_ADD_TEST(NAME simulators-DeltaTForced
                   COMMAND "${OFBUILD_DIST_BIN_DIR}/${OPENFLUID_CMD_APP}" 
                           "run"
                           "${OFBUILD_TESTS_INPUT_DATASETS_DIR}/OPENFLUID.IN.DeltaTForced"
                           "${OFBUILD_TESTS_OUTPUT_DATA_DIR}/OPENFLUID.OUT.DeltaTForced" 
                           "-p" "${OFBUILD_TESTS_BINARY_DIR}"
                           )


###########################################################################


OPENFLUID_ADD_TEST(NAME simulators-WithBuffer
                   COMMAND "${OFBUILD_DIST_BIN_DIR}/${OPENFLUID_CMD_APP}" 
                           "run"
                           "${OFBUILD_TESTS_INPUT_DATASETS_DIR}/OPENFLUID.IN.CouplingWithBuffer"
                           "${OFBUILD_TESTS_OUTPUT_DATA_DIR}/OPENFLUID.OUT.CouplingWithBuffer" 
                           "-p" "${OFBUILD_TESTS_BINARY_DIR}"
                           )
  

###########################################################################


OPENFLUID_ADD_TEST(NAME simulators-WithSmallBuffer
                   COMMAND "${OFBUILD_DIST_BIN_DIR}/${OPENFLUID_CMD_APP}" 
                           "run"
                           "${OFBUILD_TESTS_INPUT_DATASETS_DIR}/OPENFLUID.IN.CouplingWithSmallBuffer"
                           "${OFBUILD_TESTS_OUTPUT_DATA_DIR}/OPENFLUID.OUT.CouplingWithSmallBuffer" 
                           "-p" "${OFBUILD_TESTS_BINARY_DIR}"
                           )
SET_TESTS_PROPERTIES(simulators-WithSmallBuffer PROPERTIES PASS_REGULAR_EXPRESSION "buffers for variables is set to 2")


###########################################################################


OFBUILD_ADD_SIMULATOR(tests.messages ${OFBUILD_TESTS_BINARY_DIR})
OPENFLUID_ADD_TEST(NAME simulators-Messages
                   COMMAND "${OFBUILD_DIST_BIN_DIR}/${OPENFLUID_CMD_APP}" 
                         "run"
                         "${OFBUILD_TESTS_INPUT_DATASETS_DIR}/OPENFLUID.IN.Messages"
                         "${OFBUILD_TESTS_OUTPUT_DATA_DIR}/OPENFLUID.OUT.Messages" 
                         "-p" "${OFBUILD_TESTS_BINARY_DIR}")
OPENFLUID_ADD_TEST(NAME simulators-ManyMessages
                   COMMAND "${OFBUILD_DIST_BIN_DIR}/${OPENFLUID_CMD_APP}" 
                             "run"
                             "${OFBUILD_TESTS_INPUT_DATASETS_DIR}/OPENFLUID.IN.ManyMessages"
                             "${OFBUILD_TESTS_OUTPUT_DATA_DIR}/OPENFLUID.OUT.ManyMessages" 
                             "-p" "${OFBUILD_TESTS_BINARY_DIR}")                              


###########################################################################


OFBUILD_ADD_SIMULATOR(tests.parseunits ${OFBUILD_TESTS_BINARY_DIR})
OPENFLUID_ADD_TEST(NAME simulators-ManyUnits
                   COMMAND "${OFBUILD_DIST_BIN_DIR}/${OPENFLUID_CMD_APP}" 
                           "run"
                           "${OFBUILD_TESTS_INPUT_DATASETS_DIR}/OPENFLUID.IN.ManyUnits"
                           "${OFBUILD_TESTS_OUTPUT_DATA_DIR}/OPENFLUID.OUT.ManyUnits" 
                           "-p" "${OFBUILD_TESTS_BINARY_DIR}")                              
             

###########################################################################


OFBUILD_ADD_SIMULATOR(tests.tools ${OFBUILD_TESTS_BINARY_DIR})
OPENFLUID_ADD_TEST(NAME simulators-Tools
                   COMMAND "${OFBUILD_DIST_BIN_DIR}/${OPENFLUID_CMD_APP}" 
                        "run"
                        "${OFBUILD_TESTS_INPUT_DATASETS_DIR}/OPENFLUID.IN.Tools"
                        "${OFBUILD_TESTS_OUTPUT_DATA_DIR}/OPENFLUID.OUT.Tools" 
                        "-p" "${OFBUILD_TESTS_BINARY_DIR}")                 


###########################################################################

                                     
OFBUILD_ADD_SIMULATOR(tests.fakesimulator ${OFBUILD_TESTS_BINARY_DIR})


###########################################################################


OFBUILD_ADD_SIMULATOR(tests.wrongprod ${OFBUILD_TESTS_BINARY_DIR})
OPENFLUID_ADD_TEST(NAME simulators-WrongProduction
                   COMMAND "${OFBUILD_DIST_BIN_DIR}/${OPENFLUID_CMD_APP}" 
                        "run"
                        "${OFBUILD_TESTS_INPUT_DATASETS_DIR}/OPENFLUID.IN.WrongProd"
                        "${OFBUILD_TESTS_OUTPUT_DATA_DIR}/OPENFLUID.OUT.WrongProd" 
                        "-p" "${OFBUILD_TESTS_BINARY_DIR}")                              
SET_TESTS_PROPERTIES(simulators-WrongProduction PROPERTIES WILL_FAIL TRUE)        


###########################################################################


OFBUILD_ADD_SIMULATOR(tests.globalparams ${OFBUILD_TESTS_BINARY_DIR})
OPENFLUID_ADD_TEST(NAME simulators-GlobalParams
                   COMMAND "${OFBUILD_DIST_BIN_DIR}/${OPENFLUID_CMD_APP}" 
                        "run"
                        "${OFBUILD_TESTS_INPUT_DATASETS_DIR}/OPENFLUID.IN.GlobalParams"
                        "${OFBUILD_TESTS_OUTPUT_DATA_DIR}/OPENFLUID.OUT.GlobalParams" 
                        "-p" "${OFBUILD_TESTS_BINARY_DIR}")                              


###########################################################################

        
OFBUILD_ADD_SIMULATOR(tests.generators ${OFBUILD_TESTS_BINARY_DIR})
OPENFLUID_ADD_TEST(NAME simulators-Generators
                   COMMAND "${OFBUILD_DIST_BIN_DIR}/${OPENFLUID_CMD_APP}" 
                        "run"
                        "${OFBUILD_TESTS_INPUT_DATASETS_DIR}/OPENFLUID.IN.Generators"
                        "${OFBUILD_TESTS_OUTPUT_DATA_DIR}/OPENFLUID.OUT.Generators" 
                        "-p" "${OFBUILD_TESTS_BINARY_DIR}" "-v")


###########################################################################


OFBUILD_ADD_SIMULATOR(tests.logger ${OFBUILD_TESTS_BINARY_DIR})
OPENFLUID_ADD_TEST(NAME simulators-Logger
                   COMMAND "${OFBUILD_DIST_BIN_DIR}/${OPENFLUID_CMD_APP}" 
                        "run"
                        "${OFBUILD_TESTS_INPUT_DATASETS_DIR}/OPENFLUID.IN.Logger"
                        "${OFBUILD_TESTS_OUTPUT_DATA_DIR}/OPENFLUID.OUT.Logger" 
                        "-p" "${OFBUILD_TESTS_BINARY_DIR}"
                        "-n" "${OFBUILD_TESTS_BINARY_DIR}")                              


###########################################################################


OFBUILD_ADD_SIMULATOR(tests.debug ${OFBUILD_TESTS_BINARY_DIR})
OPENFLUID_ADD_TEST(NAME simulators-Debug
                   COMMAND "${OFBUILD_DIST_BIN_DIR}/${OPENFLUID_CMD_APP}" 
                        "run"
                        "${OFBUILD_TESTS_INPUT_DATASETS_DIR}/OPENFLUID.IN.Debug"
                        "${OFBUILD_TESTS_OUTPUT_DATA_DIR}/OPENFLUID.OUT.Debug" 
                        "-p" "${OFBUILD_TESTS_BINARY_DIR}")                              
IF(${CMAKE_BUILD_TYPE} STREQUAL "Debug")
  SET_TESTS_PROPERTIES(simulators-Debug PROPERTIES PASS_REGULAR_EXPRESSION "TU#")  
ELSE()
  SET_TESTS_PROPERTIES(simulators-Debug PROPERTIES FAIL_REGULAR_EXPRESSION "TU#")
ENDIF() 


###########################################################################


OFBUILD_ADD_SIMULATOR(tests.wrongid ${OFBUILD_TESTS_BINARY_DIR})
OPENFLUID_ADD_TEST(NAME simulators-WrongID 
                   COMMAND "${OFBUILD_DIST_BIN_DIR}/${OPENFLUID_CMD_APP}" 
                                  "report" "simulators" "--list"
                                  "-p" "${OFBUILD_TESTS_BINARY_DIR}")
SET_TESTS_PROPERTIES(simulators-WrongID PROPERTIES FAIL_REGULAR_EXPRESSION "tests.wrongid")


###########################################################################


OFBUILD_ADD_SIMULATOR(tests.wrongversion ${OFBUILD_TESTS_BINARY_DIR})
OPENFLUID_ADD_TEST(NAME simulators-WrongVersion 
                   COMMAND "${OFBUILD_DIST_BIN_DIR}/${OPENFLUID_CMD_APP}" 
                                  "report" "simulators" "--list"
                                  "-p" "${OFBUILD_TESTS_BINARY_DIR}")
SET_TESTS_PROPERTIES(simulators-WrongVersion PROPERTIES FAIL_REGULAR_EXPRESSION "tests.wrongversion")

