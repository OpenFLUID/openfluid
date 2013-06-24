CMAKE_MINIMUM_REQUIRED (VERSION 2.6)


ADD_DEFINITIONS(-DOPENFLUID_VERSION=${FULL_VERSION})
ADD_DEFINITIONS(-DBUILDINGDLL)

INCLUDE_DIRECTORIES("${PROJECT_SOURCE_DIR}/src" "${PROJECT_BINARY_DIR}/src")

LINK_DIRECTORIES(${LibXML2_LIBRARY_DIRS})


###########################################################################

OPNFLD_ADD_SIMULATOR(tests.coupling.A tests.coupling.A ${TEST_OUTPUT_PATH})
OPNFLD_ADD_SIMULATOR(tests.coupling.B tests.coupling.B ${TEST_OUTPUT_PATH})
OPNFLD_ADD_SIMULATOR(tests.coupling.C tests.coupling.C ${TEST_OUTPUT_PATH})
OPNFLD_ADD_SIMULATOR(tests.coupling.D tests.coupling.D ${TEST_OUTPUT_PATH})
OPNFLD_ADD_SIMULATOR(tests.coupling.E tests.coupling.E ${TEST_OUTPUT_PATH})


###########################################################################


IF(BUILD_FORTRAN_SIMULATORS)
  ENABLE_LANGUAGE(Fortran)
  ADD_SUBDIRECTORY(tests.fortran)  
  OPENFLUID_ADD_TEST(NAME simulators-Fortran
                     COMMAND "${BIN_OUTPUT_PATH}/${OPENFLUID_CMD_APP}" 
                        "-i${TESTS_DATASETS_PATH}/OPENFLUID.IN.Fortran"
                        "-o${TESTS_OUTPUTDATA_PATH}/OPENFLUID.OUT.Fortran" 
                        "-p${TEST_OUTPUT_PATH}"
                     PRE_TEST REMOVE_DIRECTORY "${TESTS_OUTPUTDATA_PATH}/OPENFLUID.OUT.Fortran"
                     POST_TEST CHECK_FILE_EXIST "${TESTS_OUTPUTDATA_PATH}/OPENFLUID.OUT.Fortran/tests.fortran_ofware-sim.log"                        )  
ENDIF(BUILD_FORTRAN_SIMULATORS)


###########################################################################


OPNFLD_ADD_SIMULATOR(tests.primitives.runenv tests.primitives.runenv ${TEST_OUTPUT_PATH})
OPENFLUID_ADD_TEST(NAME simulators-PrimitivesRunEnv 
                   COMMAND "${BIN_OUTPUT_PATH}/${OPENFLUID_CMD_APP}" 
                           "-i${TESTS_DATASETS_PATH}/OPENFLUID.IN.PrimitivesRunEnv"
                           "-o${TESTS_OUTPUTDATA_PATH}/OPENFLUID.OUT.PrimitivesRunEnv" 
                           "-p${TEST_OUTPUT_PATH}"
                           "-n${TEST_OUTPUT_PATH}"
                    PRE_TEST REMOVE_DIRECTORY "${TESTS_OUTPUTDATA_PATH}/OPENFLUID.OUT.PrimitivesRunEnv"
                    POST_TEST CHECK_FILE_EXIST "${TESTS_OUTPUTDATA_PATH}/OPENFLUID.OUT.PrimitivesRunEnv/openfluid-messages.log"
                              CHECK_FILE_EXIST "${TESTS_OUTPUTDATA_PATH}/OPENFLUID.OUT.PrimitivesRunEnv/tests.primitives.runenv_ofware-sim.log"
                   )


###########################################################################


OPNFLD_ADD_SIMULATOR(tests.primitives.time tests.primitives.time ${TEST_OUTPUT_PATH})
OPENFLUID_ADD_TEST(NAME simulators-PrimitivesTime 
                   COMMAND "${BIN_OUTPUT_PATH}/${OPENFLUID_CMD_APP}" 
                           "-i${TESTS_DATASETS_PATH}/OPENFLUID.IN.PrimitivesTime"
                           "-o${TESTS_OUTPUTDATA_PATH}/OPENFLUID.OUT.PrimitivesTime" 
                           "-p${TEST_OUTPUT_PATH}"
                           "-n${TEST_OUTPUT_PATH}"
                    PRE_TEST REMOVE_DIRECTORY "${TESTS_OUTPUTDATA_PATH}/OPENFLUID.OUT.PrimitivesTime"
                    POST_TEST CHECK_FILE_EXIST "${TESTS_OUTPUTDATA_PATH}/OPENFLUID.OUT.PrimitivesTime/openfluid-messages.log"
                              CHECK_FILE_EXIST "${TESTS_OUTPUTDATA_PATH}/OPENFLUID.OUT.PrimitivesTime/tests.primitives.time_ofware-sim.log"
                              CHECK_FILE_EXIST "${TESTS_OUTPUTDATA_PATH}/OPENFLUID.OUT.PrimitivesTime/tests.primitives.time_ofware-obs.log"
                   )


###########################################################################


OPNFLD_ADD_SIMULATOR(tests.primitives.simparams tests.primitives.simparams ${TEST_OUTPUT_PATH})
OPENFLUID_ADD_TEST(NAME simulators-PrimitivesSimParams 
                   COMMAND "${BIN_OUTPUT_PATH}/${OPENFLUID_CMD_APP}" 
                           "-i${TESTS_DATASETS_PATH}/OPENFLUID.IN.PrimitivesSimParams"
                           "-o${TESTS_OUTPUTDATA_PATH}/OPENFLUID.OUT.PrimitivesSimParams" 
                           "-p${TEST_OUTPUT_PATH}"
                           "-n${TEST_OUTPUT_PATH}"
                    PRE_TEST REMOVE_DIRECTORY "${TESTS_OUTPUTDATA_PATH}/OPENFLUID.OUT.PrimitivesSimParams"
                    POST_TEST CHECK_FILE_EXIST "${TESTS_OUTPUTDATA_PATH}/OPENFLUID.OUT.PrimitivesSimParams/openfluid-messages.log"
                              CHECK_FILE_EXIST "${TESTS_OUTPUTDATA_PATH}/OPENFLUID.OUT.PrimitivesSimParams/tests.primitives.simparams_ofware-sim.log"
                   )


###########################################################################


OPNFLD_ADD_SIMULATOR(tests.primitives.attributes.prod tests.primitives.attributes.prod ${TEST_OUTPUT_PATH})
OPNFLD_ADD_SIMULATOR(tests.primitives.attributes.use tests.primitives.attributes.use ${TEST_OUTPUT_PATH})
OPENFLUID_ADD_TEST(NAME simulators-PrimitivesAttributes 
                   COMMAND "${BIN_OUTPUT_PATH}/${OPENFLUID_CMD_APP}" 
                           "-i${TESTS_DATASETS_PATH}/OPENFLUID.IN.PrimitivesAttributes"
                           "-o${TESTS_OUTPUTDATA_PATH}/OPENFLUID.OUT.PrimitivesAttributes" 
                           "-p${TEST_OUTPUT_PATH}"
                           "-n${TEST_OUTPUT_PATH}"
                    PRE_TEST REMOVE_DIRECTORY "${TESTS_OUTPUTDATA_PATH}/OPENFLUID.OUT.PrimitivesAttributes"
                    POST_TEST CHECK_FILE_EXIST "${TESTS_OUTPUTDATA_PATH}/OPENFLUID.OUT.PrimitivesAttributes/openfluid-messages.log"
                              CHECK_FILE_EXIST "${TESTS_OUTPUTDATA_PATH}/OPENFLUID.OUT.PrimitivesAttributes/tests.primitives.attributes.prod_ofware-sim.log"
                              CHECK_FILE_EXIST "${TESTS_OUTPUTDATA_PATH}/OPENFLUID.OUT.PrimitivesAttributes/tests.primitives.attributes.use_ofware-sim.log"
                   )


###########################################################################


OPNFLD_ADD_SIMULATOR(tests.primitives.events tests.primitives.events ${TEST_OUTPUT_PATH})
OPENFLUID_ADD_TEST(NAME simulators-PrimitivesEvents 
                   COMMAND "${BIN_OUTPUT_PATH}/${OPENFLUID_CMD_APP}" 
                           "-i${TESTS_DATASETS_PATH}/OPENFLUID.IN.PrimitivesEvents"
                           "-o${TESTS_OUTPUTDATA_PATH}/OPENFLUID.OUT.PrimitivesEvents" 
                           "-p${TEST_OUTPUT_PATH}"
                           "-n${TEST_OUTPUT_PATH}"
                    PRE_TEST REMOVE_DIRECTORY "${TESTS_OUTPUTDATA_PATH}/OPENFLUID.OUT.PrimitivesEvents"
                    POST_TEST CHECK_FILE_EXIST "${TESTS_OUTPUTDATA_PATH}/OPENFLUID.OUT.PrimitivesEvents/openfluid-messages.log"
                              CHECK_FILE_EXIST "${TESTS_OUTPUTDATA_PATH}/OPENFLUID.OUT.PrimitivesEvents/tests.primitives.events_ofware-sim.log"
                   )
                   

###########################################################################


OPNFLD_ADD_SIMULATOR(tests.primitives.variables.prod tests.primitives.variables.prod ${TEST_OUTPUT_PATH})
OPNFLD_ADD_SIMULATOR(tests.primitives.variables.use tests.primitives.variables.use ${TEST_OUTPUT_PATH})
OPENFLUID_ADD_TEST(NAME simulators-PrimitivesVariables 
                   COMMAND "${BIN_OUTPUT_PATH}/${OPENFLUID_CMD_APP}" 
                           "-i${TESTS_DATASETS_PATH}/OPENFLUID.IN.PrimitivesVariables"
                           "-o${TESTS_OUTPUTDATA_PATH}/OPENFLUID.OUT.PrimitivesVariables" 
                           "-p${TEST_OUTPUT_PATH}"
                           "-n${TEST_OUTPUT_PATH}"
                    PRE_TEST REMOVE_DIRECTORY "${TESTS_OUTPUTDATA_PATH}/OPENFLUID.OUT.PrimitivesVariables"
                    POST_TEST CHECK_FILE_EXIST "${TESTS_OUTPUTDATA_PATH}/OPENFLUID.OUT.PrimitivesVariables/openfluid-messages.log"
                              CHECK_FILE_EXIST "${TESTS_OUTPUTDATA_PATH}/OPENFLUID.OUT.PrimitivesVariables/tests.primitives.variables.prod_ofware-sim.log"
                              CHECK_FILE_EXIST "${TESTS_OUTPUTDATA_PATH}/OPENFLUID.OUT.PrimitivesVariables/tests.primitives.variables.use_ofware-sim.log"
                   )


###########################################################################


OPNFLD_ADD_SIMULATOR(tests.primitives.land tests.primitives.land ${TEST_OUTPUT_PATH})
OPENFLUID_ADD_TEST(NAME simulators-PrimitivesLand 
                   COMMAND "${BIN_OUTPUT_PATH}/${OPENFLUID_CMD_APP}" 
                           "-i${TESTS_DATASETS_PATH}/OPENFLUID.IN.PrimitivesLand"
                           "-o${TESTS_OUTPUTDATA_PATH}/OPENFLUID.OUT.PrimitivesLand" 
                           "-p${TEST_OUTPUT_PATH}"
                    PRE_TEST REMOVE_DIRECTORY "${TESTS_OUTPUTDATA_PATH}/OPENFLUID.OUT.PrimitivesLand"
                    POST_TEST CHECK_FILE_EXIST "${TESTS_OUTPUTDATA_PATH}/OPENFLUID.OUT.PrimitivesLand/openfluid-messages.log"
                              CHECK_FILE_EXIST "${TESTS_OUTPUTDATA_PATH}/OPENFLUID.OUT.PrimitivesLand/tests.primitives.land_ofware-sim.log"                                                     
                    )


###########################################################################


OPENFLUID_ADD_TEST(NAME simulators-PrimitivesAll 
                   COMMAND "${BIN_OUTPUT_PATH}/${OPENFLUID_CMD_APP}" 
                           "-i${TESTS_DATASETS_PATH}/OPENFLUID.IN.PrimitivesAll"
                           "-o${TESTS_OUTPUTDATA_PATH}/OPENFLUID.OUT.PrimitivesAll" 
                           "-p${TEST_OUTPUT_PATH}"
                    PRE_TEST REMOVE_DIRECTORY "${TESTS_OUTPUTDATA_PATH}/OPENFLUID.OUT.PrimitivesAll"
                    )


###########################################################################


OPNFLD_ADD_SIMULATOR(tests.deltaTtime.prod tests.deltaTtime.prod ${TEST_OUTPUT_PATH})
OPNFLD_ADD_SIMULATOR(tests.variabletime.prod tests.variabletime.prod ${TEST_OUTPUT_PATH})
OPENFLUID_ADD_TEST(NAME simulators-VariableTimeProd
                   COMMAND "${BIN_OUTPUT_PATH}/${OPENFLUID_CMD_APP}" 
                           "-i${TESTS_DATASETS_PATH}/OPENFLUID.IN.VariableTimeProd"
                           "-o${TESTS_OUTPUTDATA_PATH}/OPENFLUID.OUT.VariableTimeProd" 
                           "-p${TEST_OUTPUT_PATH}"
                           )


###########################################################################


OPNFLD_ADD_SIMULATOR(tests.loops tests.loops ${TEST_OUTPUT_PATH})
OPENFLUID_ADD_TEST(NAME simulators-Loops
                   COMMAND "${BIN_OUTPUT_PATH}/${OPENFLUID_CMD_APP}" 
                        "-i${TESTS_DATASETS_PATH}/OPENFLUID.IN.Loops"
                        "-o${TESTS_OUTPUTDATA_PATH}/OPENFLUID.OUT.Loops" 
                        "-p${TEST_OUTPUT_PATH}")


###########################################################################


OPNFLD_ADD_SIMULATOR(tests.threadedloops tests.threadedloops ${TEST_OUTPUT_PATH})
TARGET_LINK_LIBRARIES(tests.threadedloops${OPENFLUID_SIMULATORS_SUFFIX} ${GLIBMM_LIBRARIES})
OPENFLUID_ADD_TEST(NAME simulators-ThreadedLoopsDefault
                   COMMAND "${BIN_OUTPUT_PATH}/${OPENFLUID_CMD_APP}" 
                        "-i${TESTS_DATASETS_PATH}/OPENFLUID.IN.ThreadedLoops"
                        "-o${TESTS_OUTPUTDATA_PATH}/OPENFLUID.OUT.ThreadedLoops" 
                        "-p${TEST_OUTPUT_PATH}")             
OPENFLUID_ADD_TEST(NAME simulators-ThreadedLoops9
                   COMMAND "${BIN_OUTPUT_PATH}/${OPENFLUID_CMD_APP}" 
                        "-i${TESTS_DATASETS_PATH}/OPENFLUID.IN.ThreadedLoops"
                        "-o${TESTS_OUTPUTDATA_PATH}/OPENFLUID.OUT.ThreadedLoops" 
                        "-p${TEST_OUTPUT_PATH}"
                        "-t" "9")             


###########################################################################


OPENFLUID_ADD_TEST(NAME simulators-DeltaTChecked
                   COMMAND "${BIN_OUTPUT_PATH}/${OPENFLUID_CMD_APP}" 
                           "-i${TESTS_DATASETS_PATH}/OPENFLUID.IN.DeltaTChecked"
                           "-o${TESTS_OUTPUTDATA_PATH}/OPENFLUID.OUT.DeltaTChecked" 
                           "-p${TEST_OUTPUT_PATH}"
                           )


###########################################################################


OPENFLUID_ADD_TEST(NAME simulators-DeltaTCheckedFails
                   COMMAND "${BIN_OUTPUT_PATH}/${OPENFLUID_CMD_APP}" 
                           "-i${TESTS_DATASETS_PATH}/OPENFLUID.IN.DeltaTCheckedFails"
                           "-o${TESTS_OUTPUTDATA_PATH}/OPENFLUID.OUT.DeltaTCheckedFails" 
                           "-p${TEST_OUTPUT_PATH}"
                           )
SET_TESTS_PROPERTIES(simulators-DeltaTCheckedFails PROPERTIES WILL_FAIL TRUE)


###########################################################################


OPENFLUID_ADD_TEST(NAME simulators-DeltaTForced
                   COMMAND "${BIN_OUTPUT_PATH}/${OPENFLUID_CMD_APP}" 
                           "-i${TESTS_DATASETS_PATH}/OPENFLUID.IN.DeltaTForced"
                           "-o${TESTS_OUTPUTDATA_PATH}/OPENFLUID.OUT.DeltaTForced" 
                           "-p${TEST_OUTPUT_PATH}"
                           )


###########################################################################


OPENFLUID_ADD_TEST(NAME simulators-WithBuffer
                   COMMAND "${BIN_OUTPUT_PATH}/${OPENFLUID_CMD_APP}" 
                           "-i${TESTS_DATASETS_PATH}/OPENFLUID.IN.CouplingWithBuffer"
                           "-o${TESTS_OUTPUTDATA_PATH}/OPENFLUID.OUT.CouplingWithBuffer" 
                           "-p${TEST_OUTPUT_PATH}"
                           )
  

###########################################################################


OPENFLUID_ADD_TEST(NAME simulators-WithSmallBuffer
                   COMMAND "${BIN_OUTPUT_PATH}/${OPENFLUID_CMD_APP}" 
                           "-i${TESTS_DATASETS_PATH}/OPENFLUID.IN.CouplingWithSmallBuffer"
                           "-o${TESTS_OUTPUTDATA_PATH}/OPENFLUID.OUT.CouplingWithSmallBuffer" 
                           "-p${TEST_OUTPUT_PATH}"
                           )
SET_TESTS_PROPERTIES(simulators-WithSmallBuffer PROPERTIES PASS_REGULAR_EXPRESSION "buffers for variables is set to 2")


###########################################################################


OPNFLD_ADD_SIMULATOR(tests.messages tests.messages ${TEST_OUTPUT_PATH})
OPENFLUID_ADD_TEST(NAME simulators-Messages
                   COMMAND "${BIN_OUTPUT_PATH}/${OPENFLUID_CMD_APP}" 
                         "-i${TESTS_DATASETS_PATH}/OPENFLUID.IN.Messages"
                         "-o${TESTS_OUTPUTDATA_PATH}/OPENFLUID.OUT.Messages" 
                         "-p${TEST_OUTPUT_PATH}")
OPENFLUID_ADD_TEST(NAME simulators-ManyMessages
                   COMMAND "${BIN_OUTPUT_PATH}/${OPENFLUID_CMD_APP}" 
                             "-i${TESTS_DATASETS_PATH}/OPENFLUID.IN.ManyMessages"
                             "-o${TESTS_OUTPUTDATA_PATH}/OPENFLUID.OUT.ManyMessages" 
                             "-p${TEST_OUTPUT_PATH}"
                             "-z")                              


###########################################################################


OPNFLD_ADD_SIMULATOR(tests.parseunits tests.parseunits ${TEST_OUTPUT_PATH})
OPENFLUID_ADD_TEST(NAME simulators-ManyUnits
                   COMMAND "${BIN_OUTPUT_PATH}/${OPENFLUID_CMD_APP}" 
                           "-i${TESTS_DATASETS_PATH}/OPENFLUID.IN.ManyUnits"
                           "-o${TESTS_OUTPUTDATA_PATH}/OPENFLUID.OUT.ManyUnits" 
                           "-p${TEST_OUTPUT_PATH}")                              
             

###########################################################################

                         
OPNFLD_ADD_SIMULATOR(tests.sim2doc tests.sim2doc ${TEST_OUTPUT_PATH})


###########################################################################


OPNFLD_ADD_SIMULATOR(tests.tools tests.tools ${TEST_OUTPUT_PATH})
OPENFLUID_ADD_TEST(NAME simulators-Tools
                   COMMAND "${BIN_OUTPUT_PATH}/${OPENFLUID_CMD_APP}" 
                        "-i${TESTS_DATASETS_PATH}/OPENFLUID.IN.Tools"
                        "-o${TESTS_OUTPUTDATA_PATH}/OPENFLUID.OUT.Tools" 
                        "-p${TEST_OUTPUT_PATH}")                 


###########################################################################

                                     
OPNFLD_ADD_SIMULATOR(tests.fakesimulator tests.fakesimulator ${TEST_OUTPUT_PATH})


###########################################################################


OPNFLD_ADD_SIMULATOR(tests.wrongprod tests.wrongprod ${TEST_OUTPUT_PATH})
OPENFLUID_ADD_TEST(NAME simulators-WrongProduction
                   COMMAND "${BIN_OUTPUT_PATH}/${OPENFLUID_CMD_APP}" 
                        "-i${TESTS_DATASETS_PATH}/OPENFLUID.IN.WrongProd"
                        "-o${TESTS_OUTPUTDATA_PATH}/OPENFLUID.OUT.WrongProd" 
                        "-p${TEST_OUTPUT_PATH}")                              
SET_TESTS_PROPERTIES(simulators-WrongProduction PROPERTIES WILL_FAIL TRUE)        


###########################################################################


OPNFLD_ADD_SIMULATOR(tests.globalparams tests.globalparams ${TEST_OUTPUT_PATH})
OPENFLUID_ADD_TEST(NAME simulators-GlobalParams
                   COMMAND "${BIN_OUTPUT_PATH}/${OPENFLUID_CMD_APP}" 
                        "-i${TESTS_DATASETS_PATH}/OPENFLUID.IN.GlobalParams"
                        "-o${TESTS_OUTPUTDATA_PATH}/OPENFLUID.OUT.GlobalParams" 
                        "-p${TEST_OUTPUT_PATH}")                              


###########################################################################

        
OPNFLD_ADD_SIMULATOR(tests.generators tests.generators ${TEST_OUTPUT_PATH})
OPENFLUID_ADD_TEST(NAME simulators-Generators
                   COMMAND "${BIN_OUTPUT_PATH}/${OPENFLUID_CMD_APP}" 
                        "-i${TESTS_DATASETS_PATH}/OPENFLUID.IN.Generators"
                        "-o${TESTS_OUTPUTDATA_PATH}/OPENFLUID.OUT.Generators" 
                        "-p${TEST_OUTPUT_PATH}" "-v")                              


###########################################################################


OPNFLD_ADD_SIMULATOR(tests.logger tests.logger ${TEST_OUTPUT_PATH})
OPENFLUID_ADD_TEST(NAME simulators-Logger
                   COMMAND "${BIN_OUTPUT_PATH}/${OPENFLUID_CMD_APP}" 
                        "-i${TESTS_DATASETS_PATH}/OPENFLUID.IN.Logger"
                        "-o${TESTS_OUTPUTDATA_PATH}/OPENFLUID.OUT.Logger" 
                        "-p${TEST_OUTPUT_PATH}"
                        "-n${TEST_OUTPUT_PATH}")                              


###########################################################################


OPNFLD_ADD_SIMULATOR(tests.debug tests.debug ${TEST_OUTPUT_PATH})
OPENFLUID_ADD_TEST(NAME simulators-Debug
                   COMMAND "${BIN_OUTPUT_PATH}/${OPENFLUID_CMD_APP}" 
                        "-i${TESTS_DATASETS_PATH}/OPENFLUID.IN.Debug"
                        "-o${TESTS_OUTPUTDATA_PATH}/OPENFLUID.OUT.Debug" 
                        "-p${TEST_OUTPUT_PATH}")                              
IF(${CMAKE_BUILD_TYPE} STREQUAL "Debug")
  SET_TESTS_PROPERTIES(simulators-Debug PROPERTIES PASS_REGULAR_EXPRESSION "TU#")  
ELSE(${CMAKE_BUILD_TYPE} STREQUAL "Debug")
  SET_TESTS_PROPERTIES(simulators-Debug PROPERTIES FAIL_REGULAR_EXPRESSION "TU#")
ENDIF(${CMAKE_BUILD_TYPE} STREQUAL "Debug") 


###########################################################################


OPNFLD_ADD_SIMULATOR(tests.wrongid tests.wrongid ${TEST_OUTPUT_PATH})
OPENFLUID_ADD_TEST(NAME simulators-WrongID 
                   COMMAND "${BIN_OUTPUT_PATH}/${OPENFLUID_CMD_APP}" 
                                  "-f"
                                  "-p${TEST_OUTPUT_PATH}")
SET_TESTS_PROPERTIES(simulators-WrongID PROPERTIES FAIL_REGULAR_EXPRESSION "tests.wrongid")


###########################################################################


OPNFLD_ADD_SIMULATOR(tests.wrongversion tests.wrongversion ${TEST_OUTPUT_PATH})
OPENFLUID_ADD_TEST(NAME simulators-WrongVersion 
                   COMMAND "${BIN_OUTPUT_PATH}/${OPENFLUID_CMD_APP}" 
                                  "-f"
                                  "-p${TEST_OUTPUT_PATH}")
SET_TESTS_PROPERTIES(simulators-WrongVersion PROPERTIES FAIL_REGULAR_EXPRESSION "tests.wrongversion")

                            