FIND_PACKAGE(Boost REQUIRED COMPONENTS unit_test_framework)  


INCLUDE_DIRECTORIES(${Boost_INCLUDE_DIRS}
                    "${PROJECT_SOURCE_DIR}/src" "${PROJECT_BINARY_DIR}/src/tests")


###########################################################################


OPENFLUID_ADD_TEST(NAME integration-DisplayHelp
                   COMMAND "${OFBUILD_DIST_BIN_DIR}/${OPENFLUID_CMD_APP}" "-h")
SET_TESTS_PROPERTIES(integration-DisplayHelp 
                     PROPERTIES PASS_REGULAR_EXPRESSION "openfluid \\[<command>\\] \\[<options>\\] \\[<args>\\]")  


###########################################################################


OPENFLUID_ADD_TEST(NAME integration-DisplayHelpWhenNothing
                   COMMAND "${OFBUILD_DIST_BIN_DIR}/${OPENFLUID_CMD_APP}")
SET_TESTS_PROPERTIES(integration-DisplayHelpWhenNothing 
                     PROPERTIES PASS_REGULAR_EXPRESSION "openfluid \\[<command>\\] \\[<options>\\] \\[<args>\\]")  


###########################################################################


OPENFLUID_ADD_TEST(NAME integration-DisplayVersion 
                   COMMAND "${OFBUILD_DIST_BIN_DIR}/${OPENFLUID_CMD_APP}" 
                           "--version")
  SET_TESTS_PROPERTIES(integration-DisplayVersion PROPERTIES PASS_REGULAR_EXPRESSION ${OPENFLUID_VERSION_FULL})  


###########################################################################


OPENFLUID_ADD_TEST(NAME integration-ShowPaths 
                   COMMAND "${OFBUILD_DIST_BIN_DIR}/${OPENFLUID_CMD_APP}" 
                       "show-paths"
                   )  
SET_TESTS_PROPERTIES(integration-ShowPaths PROPERTIES FAIL_REGULAR_EXPRESSION "#1 foobar")
SET_TESTS_PROPERTIES(integration-ShowPaths PROPERTIES FAIL_REGULAR_EXPRESSION "#1 barfoo")
SET_TESTS_PROPERTIES(integration-ShowPaths PROPERTIES PASS_REGULAR_EXPRESSION "${OFBUILD_TESTS_USERDATA_DIR}")
SET_TESTS_PROPERTIES(integration-ShowPaths PROPERTIES PASS_REGULAR_EXPRESSION "${OFBUILD_TESTS_TEMP_DIR}")
IF(OFBUILD_ENABLE_MARKET)
  SET_TESTS_PROPERTIES(integration-ShowPaths PROPERTIES PASS_REGULAR_EXPRESSION "${OPENFLUID_MARKETBAGDIR}")  
ELSE()
  SET_TESTS_PROPERTIES(integration-ShowPaths PROPERTIES FAIL_REGULAR_EXPRESSION "${OPENFLUID_MARKETBAGDIR}")
ENDIF()



###########################################################################


OPENFLUID_ADD_TEST(NAME integration-ShowPathsNoEnvVars
                   COMMAND "${OFBUILD_DIST_BIN_DIR}/${OPENFLUID_CMD_APP}" 
                       "show-paths"
                       "-p" "foobar"
                       "-n" "barfoo")
SET_TESTS_PROPERTIES(integration-ShowPathsNoEnvVars PROPERTIES ENVIRONMENT "")                  
SET_TESTS_PROPERTIES(integration-ShowPathsNoEnvVars PROPERTIES PASS_REGULAR_EXPRESSION "#1 foobar")
SET_TESTS_PROPERTIES(integration-ShowPathsNoEnvVars PROPERTIES PASS_REGULAR_EXPRESSION "#1 barfoo")
SET_TESTS_PROPERTIES(integration-ShowPathsNoEnvVars PROPERTIES FAIL_REGULAR_EXPRESSION "${OFBUILD_TESTS_USERDATA_DIR}")
SET_TESTS_PROPERTIES(integration-ShowPathsNoEnvVars PROPERTIES FAIL_REGULAR_EXPRESSION "${OFBUILD_TESTS_TEMP_DIR}")
  

###########################################################################


OPENFLUID_ADD_TEST(NAME integration-SimulatorsList 
                   COMMAND "${OFBUILD_DIST_BIN_DIR}/${OPENFLUID_CMD_APP}" 
                           "report" "simulators" "--list"
                           "-p" "${OFBUILD_TESTS_BINARY_DIR}")
SET_TESTS_PROPERTIES(integration-SimulatorsList PROPERTIES PASS_REGULAR_EXPRESSION "tests.tools")  
SET_TESTS_PROPERTIES(integration-SimulatorsList PROPERTIES FAIL_REGULAR_EXPRESSION "tests.empty")  


###########################################################################


OPENFLUID_ADD_TEST(NAME integration-SimulatorsReport 
                   COMMAND "${OFBUILD_DIST_BIN_DIR}/${OPENFLUID_CMD_APP}"
                           "report" "simulators"
                           "-p" "${OFBUILD_TESTS_BINARY_DIR}")
SET_TESTS_PROPERTIES(integration-SimulatorsReport PROPERTIES PASS_REGULAR_EXPRESSION "tests.tools")  
SET_TESTS_PROPERTIES(integration-SimulatorsReport PROPERTIES FAIL_REGULAR_EXPRESSION "tests.empty")  


###########################################################################


OPENFLUID_ADD_TEST(NAME integration-SimulatorsListJSON 
                   COMMAND "${OFBUILD_DIST_BIN_DIR}/${OPENFLUID_CMD_APP}" 
                           "report" "simulators" "--list" "--format=json"
                           "-p" "${OFBUILD_TESTS_BINARY_DIR}")
SET_TESTS_PROPERTIES(integration-SimulatorsListJSON PROPERTIES PASS_REGULAR_EXPRESSION "tests.tools")  
SET_TESTS_PROPERTIES(integration-SimulatorsListJSON PROPERTIES FAIL_REGULAR_EXPRESSION "tests.empty")  


###########################################################################


OPENFLUID_ADD_TEST(NAME integration-SimulatorsReportJSON 
                   COMMAND "${OFBUILD_DIST_BIN_DIR}/${OPENFLUID_CMD_APP}"
                           "report" "simulators" "--format=json"
                           "-p" "${OFBUILD_TESTS_BINARY_DIR}")
SET_TESTS_PROPERTIES(integration-SimulatorsReportJSON PROPERTIES PASS_REGULAR_EXPRESSION "tests.tools")  
SET_TESTS_PROPERTIES(integration-SimulatorsReportJSON PROPERTIES FAIL_REGULAR_EXPRESSION "tests.empty")  


###########################################################################


OPENFLUID_ADD_TEST(NAME integration-ObserversList 
                   COMMAND "${OFBUILD_DIST_BIN_DIR}/${OPENFLUID_CMD_APP}" 
                           "report" "observers" "--list"
                           "-n" "${OFBUILD_TESTS_BINARY_DIR}")
SET_TESTS_PROPERTIES(integration-ObserversList PROPERTIES PASS_REGULAR_EXPRESSION "tests.empty")
SET_TESTS_PROPERTIES(integration-ObserversList PROPERTIES FAIL_REGULAR_EXPRESSION "tests.tools")


###########################################################################

OPENFLUID_ADD_TEST(NAME integration-ObserversReport
                   COMMAND "${OFBUILD_DIST_BIN_DIR}/${OPENFLUID_CMD_APP}" 
                           "report" "observers"
                           "-n" "${OFBUILD_TESTS_BINARY_DIR}")  
SET_TESTS_PROPERTIES(integration-ObserversReport PROPERTIES PASS_REGULAR_EXPRESSION "tests.empty")
SET_TESTS_PROPERTIES(integration-ObserversReport PROPERTIES FAIL_REGULAR_EXPRESSION "tests.tools")

 
###########################################################################


OPENFLUID_ADD_TEST(NAME integration-ObserversListJSON
                   COMMAND "${OFBUILD_DIST_BIN_DIR}/${OPENFLUID_CMD_APP}" 
                           "report" "observers" "--list" "--format=json"
                           "-n" "${OFBUILD_TESTS_BINARY_DIR}")
SET_TESTS_PROPERTIES(integration-ObserversListJSON PROPERTIES PASS_REGULAR_EXPRESSION "tests.empty")
SET_TESTS_PROPERTIES(integration-ObserversListJSON PROPERTIES FAIL_REGULAR_EXPRESSION "tests.tools")

 
###########################################################################

OPENFLUID_ADD_TEST(NAME integration-ObserversReportJSON
                   COMMAND "${OFBUILD_DIST_BIN_DIR}/${OPENFLUID_CMD_APP}" 
                           "report" "observers" "--format=json"
                           "-n" "${OFBUILD_TESTS_BINARY_DIR}")  
SET_TESTS_PROPERTIES(integration-ObserversReportJSON PROPERTIES PASS_REGULAR_EXPRESSION "tests.empty")
SET_TESTS_PROPERTIES(integration-ObserversReportJSON PROPERTIES FAIL_REGULAR_EXPRESSION "tests.tools")


###########################################################################

  
IF(OFBUILD_CUSTOM_CMAKE_VERSION VERSION_EQUAL "2.8.0" OR OFBUILD_CUSTOM_CMAKE_VERSION VERSION_GREATER "2.8.0") 
  OPENFLUID_ADD_TEST(NAME integration-SimulatorsReportEnvVar 
                     COMMAND "${OFBUILD_DIST_BIN_DIR}/${OPENFLUID_CMD_APP}" 
                              "report" 
                              "simulators")
  SET_TESTS_PROPERTIES(integration-SimulatorsReportEnvVar 
                       PROPERTIES ENVIRONMENT "OPENFLUID_SIMS_PATH=${OFBUILD_TESTS_BINARY_DIR};OPENFLUID_TEMP_PATH=${OFBUILD_TESTS_TEMP_DIR};OPENFLUID_USERDATA_PATH=${OFBUILD_TESTS_USERDATA_DIR}")                           
  SET_TESTS_PROPERTIES(integration-SimulatorsReportEnvVar PROPERTIES PASS_REGULAR_EXPRESSION "tests.primitives.variables.use")  
  SET_TESTS_PROPERTIES(integration-SimulatorsReportEnvVar PROPERTIES PASS_REGULAR_EXPRESSION "tests.primitives.variables.prod")    
ENDIF()


###########################################################################


OPENFLUID_ADD_TEST(NAME integration-SimulatorsReportNoEnvVar 
                   COMMAND "${OFBUILD_DIST_BIN_DIR}/${OPENFLUID_CMD_APP}" 
                           "report" 
                           "simulators")                           
SET_TESTS_PROPERTIES(integration-SimulatorsReportNoEnvVar PROPERTIES PASS_REGULAR_EXPRESSION "tests.primitives.variables.use")  
SET_TESTS_PROPERTIES(integration-SimulatorsReportNoEnvVar PROPERTIES PASS_REGULAR_EXPRESSION "tests.primitives.variables.prod")
SET_TESTS_PROPERTIES(integration-SimulatorsReportNoEnvVar PROPERTIES WILL_FAIL TRUE)        


###########################################################################

  
IF(OFBUILD_CUSTOM_CMAKE_VERSION VERSION_EQUAL "2.8.0" OR OFBUILD_CUSTOM_CMAKE_VERSION VERSION_GREATER "2.8.0") 
  OPENFLUID_ADD_TEST(NAME integration-ObserversReportEnvVar 
                     COMMAND "${OFBUILD_DIST_BIN_DIR}/${OPENFLUID_CMD_APP}" 
                              "report" 
                              "observers")
  SET_TESTS_PROPERTIES(integration-ObserversReportEnvVar 
                       PROPERTIES ENVIRONMENT "OPENFLUID_OBSS_PATH=${OFBUILD_TESTS_BINARY_DIR};OPENFLUID_TEMP_PATH=${OFBUILD_TESTS_TEMP_DIR};OPENFLUID_USERDATA_PATH=${OFBUILD_TESTS_USERDATA_DIR}")                           
  SET_TESTS_PROPERTIES(integration-ObserversReportEnvVar PROPERTIES PASS_REGULAR_EXPRESSION "tests.empty")  
ENDIF()


###########################################################################


OPENFLUID_ADD_TEST(NAME integration-ObserversReportNoEnvVar 
                   COMMAND "${OFBUILD_DIST_BIN_DIR}/${OPENFLUID_CMD_APP}" 
                           "report" 
                           "observers")                           
SET_TESTS_PROPERTIES(integration-ObserversReportNoEnvVar  PROPERTIES PASS_REGULAR_EXPRESSION "tests.empty")
SET_TESTS_PROPERTIES(integration-ObserversReportNoEnvVar  PROPERTIES WILL_FAIL TRUE)
  

###########################################################################


OPENFLUID_ADD_TEST(NAME integration-InputDoesNotExist 
                   COMMAND "${OFBUILD_DIST_BIN_DIR}/${OPENFLUID_CMD_APP}" 
                           "run"
                           "${CMAKE_BINARY_DIR}/path/does/not/exist")                           
SET_TESTS_PROPERTIES(integration-InputDoesNotExist PROPERTIES WILL_FAIL TRUE)


###########################################################################


OPENFLUID_ADD_TEST(NAME integration-InputWithNoModel
                   COMMAND "${OFBUILD_DIST_BIN_DIR}/${OPENFLUID_CMD_APP}"
                           "run"
                           "${OFBUILD_TESTS_INPUT_DATASETS_DIR}/OPENFLUID.IN.NoModel"
                           "${OFBUILD_TESTS_OUTPUT_DATA_DIR}/OPENFLUID.OUT.NoModel"
                           "-p" "${OFBUILD_TESTS_BINARY_DIR}"
                    PRE_TEST REMOVE_DIRECTORY "${OFBUILD_TESTS_OUTPUT_DATA_DIR}/OPENFLUID.OUT.NoModel"
                   )               
SET_TESTS_PROPERTIES(integration-InputWithNoModel PROPERTIES WILL_FAIL TRUE)


###########################################################################


OPENFLUID_ADD_TEST(NAME integration-InputWithNoRun
                   COMMAND "${OFBUILD_DIST_BIN_DIR}/${OPENFLUID_CMD_APP}"
                           "run"
                           "${OFBUILD_TESTS_INPUT_DATASETS_DIR}/OPENFLUID.IN.NoRun"
                           "${OFBUILD_TESTS_OUTPUT_DATA_DIR}/OPENFLUID.OUT.NoRun"
                           "-p" "${OFBUILD_TESTS_BINARY_DIR}")                           
SET_TESTS_PROPERTIES(integration-InputWithNoRun PROPERTIES WILL_FAIL TRUE)



###########################################################################


OPENFLUID_ADD_TEST(NAME integration-MissingSimulator
                   COMMAND "${OFBUILD_DIST_BIN_DIR}/${OPENFLUID_CMD_APP}"
                           "run"
                           "${OFBUILD_TESTS_INPUT_DATASETS_DIR}/OPENFLUID.IN.MissingSim"
                           "${OFBUILD_TESTS_OUTPUT_DATA_DIR}/OPENFLUID.OUT.MissingSim"
                           "-p" "${OFBUILD_TESTS_BINARY_DIR}")               
SET_TESTS_PROPERTIES(integration-MissingSimulator PROPERTIES WILL_FAIL TRUE)



###########################################################################


OPENFLUID_ADD_TEST(NAME integration-MissingUnitsClass
                   COMMAND "${OFBUILD_DIST_BIN_DIR}/${OPENFLUID_CMD_APP}"
                           "run"
                           "${OFBUILD_TESTS_INPUT_DATASETS_DIR}/OPENFLUID.IN.MissingClass"
                           "${OFBUILD_TESTS_OUTPUT_DATA_DIR}/OPENFLUID.OUT.MissingClass"
                           "-p" "${OFBUILD_TESTS_BINARY_DIR}")               
SET_TESTS_PROPERTIES(integration-MissingUnitsClass PROPERTIES WILL_FAIL TRUE)


###########################################################################


OPENFLUID_ADD_TEST(NAME integration-MissingVariable
                   COMMAND "${OFBUILD_DIST_BIN_DIR}/${OPENFLUID_CMD_APP}"
                           "run"
                           "${OFBUILD_TESTS_INPUT_DATASETS_DIR}/OPENFLUID.IN.MissingVar"
                           "${OFBUILD_TESTS_OUTPUT_DATA_DIR}/OPENFLUID.OUT.MissingVar"
                           "-p" "${OFBUILD_TESTS_BINARY_DIR}")               
SET_TESTS_PROPERTIES(integration-MissingVariable PROPERTIES WILL_FAIL TRUE)



###########################################################################


OPENFLUID_ADD_TEST(NAME integration-MissingAttributes
                   COMMAND "${OFBUILD_DIST_BIN_DIR}/${OPENFLUID_CMD_APP}"
                           "run"
                           "${OFBUILD_TESTS_INPUT_DATASETS_DIR}/OPENFLUID.IN.MissingAttributes"
                           "${OFBUILD_TESTS_OUTPUT_DATA_DIR}/OPENFLUID.OUT.MissingAttributes"
                           "-p" "${OFBUILD_TESTS_BINARY_DIR}")               
SET_TESTS_PROPERTIES(integration-MissingAttributes PROPERTIES WILL_FAIL TRUE)



###########################################################################


OPENFLUID_ADD_TEST(NAME integration-DuplicateVariable
                   COMMAND "${OFBUILD_DIST_BIN_DIR}/${OPENFLUID_CMD_APP}"
                           "run"
                           "${OFBUILD_TESTS_INPUT_DATASETS_DIR}/OPENFLUID.IN.DuplicateVar"
                           "${OFBUILD_TESTS_OUTPUT_DATA_DIR}/OPENFLUID.OUT.DuplicateVar"
                           "-p" "${OFBUILD_TESTS_BINARY_DIR}")               
SET_TESTS_PROPERTIES(integration-DuplicateVariable PROPERTIES WILL_FAIL TRUE)


###########################################################################


OPENFLUID_ADD_TEST(NAME integration-Verbose
                   COMMAND "${OFBUILD_DIST_BIN_DIR}/${OPENFLUID_CMD_APP}"
                           "run"
                           "${OFBUILD_TESTS_INPUT_DATASETS_DIR}/OPENFLUID.IN.VariableTimeProd"
                           "${OFBUILD_TESTS_OUTPUT_DATA_DIR}/OPENFLUID.OUT.Verbose"
                           "-p" "${OFBUILD_TESTS_BINARY_DIR}"
                           "-v")
  

###########################################################################


OPENFLUID_ADD_TEST(NAME integration-Quiet
                   COMMAND "${OFBUILD_DIST_BIN_DIR}/${OPENFLUID_CMD_APP}"
                           "run"
                           "${OFBUILD_TESTS_INPUT_DATASETS_DIR}/OPENFLUID.IN.VariableTimeProd"
                           "${OFBUILD_TESTS_OUTPUT_DATA_DIR}/OPENFLUID.OUT.Quiet"
                           "-p" "${OFBUILD_TESTS_BINARY_DIR}"
                           "-q")


###########################################################################


FILE(COPY "${OFBUILD_TESTS_INPUT_DATASETS_DIR}/OPENFLUID.PRJ.Primitives" 
       DESTINATION "${OFBUILD_TESTS_OUTPUT_DATA_DIR}"
       PATTERN ".svn" EXCLUDE)

OPENFLUID_ADD_TEST(NAME integration-Project
                   COMMAND "${OFBUILD_DIST_BIN_DIR}/${OPENFLUID_CMD_APP}"
                           "run"  
                           "${OFBUILD_TESTS_OUTPUT_DATA_DIR}/OPENFLUID.PRJ.Primitives"
                           "-p" "${OFBUILD_TESTS_BINARY_DIR}"
                           "-c")


###########################################################################


OPENFLUID_ADD_TEST(NAME integration-Profiling
                   COMMAND "${OFBUILD_DIST_BIN_DIR}/${OPENFLUID_CMD_APP}"
                           "run"
                           "${OFBUILD_TESTS_INPUT_DATASETS_DIR}/OPENFLUID.IN.Coupling"
                           "${OFBUILD_TESTS_OUTPUT_DATA_DIR}/OPENFLUID.OUT.Profiling"
                           "-p" "${OFBUILD_TESTS_BINARY_DIR}"
                           "-k")


###########################################################################


OPENFLUID_ADD_TEST(NAME integration-FluidXWriterSingle
                   COMMAND "${OFBUILD_DIST_BIN_DIR}/${OPENFLUID_CMD_APP}"
                           "run"
                           "${OFBUILD_TESTS_OUTPUT_DATA_DIR}/OPENFLUID.OUT.FluidXWriterSingle"
                           "${OFBUILD_TESTS_OUTPUT_DATA_DIR}/OPENFLUID.OUT.FluidXWriterRunSingle"
                           "-p" "${OFBUILD_TESTS_BINARY_DIR}")
SET_PROPERTY(TEST integration-FluidXWriterSingle APPEND PROPERTY DEPENDS unit-api-FluidXDescriptor_TEST)


###########################################################################


OPENFLUID_ADD_TEST(NAME integration-FluidXWriterMany
                   COMMAND "${OFBUILD_DIST_BIN_DIR}/${OPENFLUID_CMD_APP}"
                            "run"
                            "${OFBUILD_TESTS_OUTPUT_DATA_DIR}/OPENFLUID.OUT.FluidXWriterMany"
                            "${OFBUILD_TESTS_OUTPUT_DATA_DIR}/OPENFLUID.OUT.FluidXWriterRunMany"
                            "-p" "${OFBUILD_TESTS_BINARY_DIR}")
SET_PROPERTY(TEST integration-FluidXWriterMany APPEND PROPERTY DEPENDS unit-api-FluidXDescriptor_TEST)
                                 

###########################################################################
# integration-GeneratorsCycle is a test in 2 steps to check that inject generator and observer work correctly together
#    Step 1 generates the data file in its OUT via a regular simulator. 
#           The output data file is copied to a temporary dir containing IN files of step 2
#    Step 2 uses the data file as source for inject generator and regenerates it as OUT
#           the two OUT data files are then compared to check identity through observer and generator

OPENFLUID_ADD_TEST(NAME integration-GeneratorsCycleStep1Sim
                   COMMAND "${OFBUILD_DIST_BIN_DIR}/${OPENFLUID_CMD_APP}" 
                           "run"
                           "${OFBUILD_TESTS_INPUT_DATASETS_DIR}/OPENFLUID.IN.SimGenConsistencyStep1Sim"
                           "${OFBUILD_TESTS_OUTPUT_DATA_DIR}/OPENFLUID.OUT.SimGenConsistencyStep1Sim" 
                           "-p" "${OFBUILD_TESTS_BINARY_DIR}" "-v"
                   )

FILE(COPY "${OFBUILD_TESTS_INPUT_DATASETS_DIR}/OPENFLUID.IN.SimGenConsistencyStep2Gen" 
     DESTINATION "${OFBUILD_TESTS_TEMP_DIR}")
OPENFLUID_ADD_TEST(NAME integration-GeneratorsCycleStep2Gen
                   COMMAND "${OFBUILD_DIST_BIN_DIR}/${OPENFLUID_CMD_APP}" 
                           "run"
                           "${OFBUILD_TESTS_TEMP_DIR}/OPENFLUID.IN.SimGenConsistencyStep2Gen"
                           "${OFBUILD_TESTS_OUTPUT_DATA_DIR}/OPENFLUID.OUT.SimGenConsistencyStep2Gen" 
                           "-p" "${OFBUILD_TESTS_BINARY_DIR}" "-v"
                   
                   PRE_TEST COPY_FILE "${OFBUILD_TESTS_OUTPUT_DATA_DIR}/OPENFLUID.OUT.SimGenConsistencyStep1Sim/autoTU_TU1_var.rand.prod.csv" 
                                      "${OFBUILD_TESTS_TEMP_DIR}/OPENFLUID.IN.SimGenConsistencyStep2Gen/"
                   POST_TEST CHECK_FILE_IDENTICAL "${OFBUILD_TESTS_OUTPUT_DATA_DIR}/OPENFLUID.OUT.SimGenConsistencyStep1Sim/"
                                                  "${OFBUILD_TESTS_OUTPUT_DATA_DIR}/OPENFLUID.OUT.SimGenConsistencyStep2Gen/"
                                                  "autoTU_TU1_var.rand.prod.csv"
                   )
SET_TESTS_PROPERTIES(integration-GeneratorsCycleStep2Gen PROPERTIES DEPENDS integration-GeneratorsCycleStep1Sim)


###########################################################################


FILE(MAKE_DIRECTORY "${OFBUILD_TESTS_OUTPUT_DATA_DIR}/FindOpenFLUID")
  
OPENFLUID_ADD_TEST(NAME integration-CMakeModuleCheck
                   COMMAND "${CMAKE_COMMAND}"
                           "-E" "chdir" "${OFBUILD_TESTS_OUTPUT_DATA_DIR}/FindOpenFLUID" 
                           "${CMAKE_COMMAND}" "-G" "${CMAKE_GENERATOR}"
                           "${CMAKE_CURRENT_SOURCE_DIR}/FindOpenFLUID"
                           "-DOpenFLUID_DIR=${OFBUILD_DIST_CMAKE_MODULES_DIR}"
                           "-DOpenFLUID_ROOT_DIR=${OFBUILD_DIST_DIR}"
                           "-DTEST_SEARCH_PATHS1=${CMAKE_BINARY_DIR}/src"
                           "-DTEST_SEARCH_PATHS2=${CMAKE_SOURCE_DIR}/src"
                   PRE_TEST EMPTY_DIRECTORY "${OFBUILD_TESTS_OUTPUT_DATA_DIR}/FindOpenFLUID")                                                                                          


###########################################################################


FILE(MAKE_DIRECTORY "${OFBUILD_TESTS_OUTPUT_DATA_DIR}/tests.sim.cmake")
  
OPENFLUID_ADD_TEST(NAME integration-CMakeModuleSimulatorConfigure
                   COMMAND "${CMAKE_COMMAND}"
                             "-E" "chdir" "${OFBUILD_TESTS_OUTPUT_DATA_DIR}/tests.sim.cmake" 
                             "${CMAKE_COMMAND}" "-G" "${CMAKE_GENERATOR}"
                             "${CMAKE_CURRENT_SOURCE_DIR}/tests.sim.cmake"
                             "-DOpenFLUIDHelpers_DIR=${OFBUILD_DIST_CMAKE_MODULES_DIR}"
                             "-DOpenFLUID_DIR=${OFBUILD_DIST_CMAKE_MODULES_DIR}"
                             "-DOpenFLUID_ROOT_DIR=${OFBUILD_DIST_DIR}"
                             "-DOpenFLUID_EXTRA_SEARCH_PATHS=${CMAKE_BINARY_DIR}/src"
                             "-DSIM_INCLUDE_DIRS=${CMAKE_SOURCE_DIR}/src"                             
                   PRE_TEST EMPTY_DIRECTORY "${OFBUILD_TESTS_OUTPUT_DATA_DIR}/tests.sim.cmake")                                                                                          


###########################################################################


OPENFLUID_ADD_TEST(NAME integration-CMakeModuleSimulatorBuild
                   COMMAND "${CMAKE_COMMAND}"
                           "-E" "chdir" "${OFBUILD_TESTS_OUTPUT_DATA_DIR}/tests.sim.cmake" 
                           "${CMAKE_COMMAND}" "--build" "${OFBUILD_TESTS_OUTPUT_DATA_DIR}/tests.sim.cmake"
                           "--target" "install"
                           "--clean-first")
SET_PROPERTY(TEST integration-CMakeModuleSimulatorBuild APPEND PROPERTY DEPENDS integration-CMakeModuleSimulatorConfigure)                                                            


###########################################################################


OPENFLUID_ADD_TEST(NAME integration-CMakeModuleSimulatorTest
                   COMMAND "${CMAKE_COMMAND}"
                           "-E" "chdir" "${OFBUILD_TESTS_OUTPUT_DATA_DIR}/tests.sim.cmake" 
                           "${CMAKE_CTEST_COMMAND}" "-V")
SET_PROPERTY(TEST integration-CMakeModuleSimulatorTest APPEND PROPERTY DEPENDS integration-CMakeModuleSimulatorBuild)


###########################################################################


IF(OFBUILD_ENABLE_GUI)
  FILE(MAKE_DIRECTORY "${OFBUILD_TESTS_OUTPUT_DATA_DIR}/tests.simui.cmake")

  OPENFLUID_ADD_TEST(NAME integration-CMakeModuleSimulatorParamsUIConfigure
                     COMMAND "${CMAKE_COMMAND}"
                              "-E" "chdir" "${OFBUILD_TESTS_OUTPUT_DATA_DIR}/tests.simui.cmake" 
                              "${CMAKE_COMMAND}" "-G" "${CMAKE_GENERATOR}"
                              "${CMAKE_CURRENT_SOURCE_DIR}/tests.simui.cmake"
                              "-DOpenFLUIDHelpers_DIR=${OFBUILD_DIST_CMAKE_MODULES_DIR}"
                              "-DOpenFLUID_DIR=${OFBUILD_DIST_CMAKE_MODULES_DIR}"
                              "-DOpenFLUID_ROOT_DIR=${OFBUILD_DIST_DIR}"
                              "-DOpenFLUID_EXTRA_SEARCH_PATHS=${CMAKE_BINARY_DIR}/src"                              
                              "-DSIM_INCLUDE_DIRS=${CMAKE_SOURCE_DIR}/src"
                     PRE_TEST EMPTY_DIRECTORY "${OFBUILD_TESTS_OUTPUT_DATA_DIR}/tests.simui.cmake")                              
ENDIF()


###########################################################################


IF(OFBUILD_ENABLE_GUI)
  OPENFLUID_ADD_TEST(NAME integration-CMakeModuleSimulatorParamsUIBuild
                     COMMAND "${CMAKE_COMMAND}"
                              "-E" "chdir" "${OFBUILD_TESTS_OUTPUT_DATA_DIR}/tests.simui.cmake" 
                              "${CMAKE_COMMAND}" "--build" "${OFBUILD_TESTS_OUTPUT_DATA_DIR}/tests.simui.cmake"
                              "--target" "install"
                              "--clean-first")
  SET_PROPERTY(TEST integration-CMakeModuleSimulatorParamsUIBuild APPEND PROPERTY DEPENDS integration-CMakeModuleSimulatorParamsUIConfigure)
ENDIF()


###########################################################################


FILE(MAKE_DIRECTORY "${OFBUILD_TESTS_OUTPUT_DATA_DIR}/tests.obs.cmake")

OPENFLUID_ADD_TEST(NAME integration-CMakeModuleObserverConfigure
                   COMMAND "${CMAKE_COMMAND}"
                              "-E" "chdir" "${OFBUILD_TESTS_OUTPUT_DATA_DIR}/tests.obs.cmake" 
                              "${CMAKE_COMMAND}" "-G" "${CMAKE_GENERATOR}"
                              "${CMAKE_CURRENT_SOURCE_DIR}/tests.obs.cmake"
                              "-DOpenFLUIDHelpers_DIR=${OFBUILD_DIST_CMAKE_MODULES_DIR}"
                              "-DOpenFLUID_DIR=${OFBUILD_DIST_CMAKE_MODULES_DIR}"
                              "-DOpenFLUID_ROOT_DIR=${OFBUILD_DIST_DIR}"
                              "-DOpenFLUID_EXTRA_SEARCH_PATHS=${CMAKE_BINARY_DIR}/src"                              
                              "-DOBS_INCLUDE_DIRS=${CMAKE_SOURCE_DIR}/src"
                    PRE_TEST EMPTY_DIRECTORY "${OFBUILD_TESTS_OUTPUT_DATA_DIR}/tests.obs.cmake")                              
                              

###########################################################################


OPENFLUID_ADD_TEST(NAME integration-CMakeModuleObserverBuild
                   COMMAND "${CMAKE_COMMAND}"
                              "-E" "chdir" "${OFBUILD_TESTS_OUTPUT_DATA_DIR}/tests.obs.cmake" 
                              "${CMAKE_COMMAND}" "--build" "${OFBUILD_TESTS_OUTPUT_DATA_DIR}/tests.obs.cmake"
                              "--target" "install"
                              "--clean-first")
SET_PROPERTY(TEST integration-CMakeModuleObserverBuild APPEND PROPERTY DEPENDS integration-CMakeModuleObserverConfigure)                                                            


###########################################################################


OPENFLUID_ADD_TEST(NAME integration-CMakeModuleObserverTest
                   COMMAND "${CMAKE_COMMAND}"
                              "-E" "chdir" "${OFBUILD_TESTS_OUTPUT_DATA_DIR}/tests.obs.cmake" 
                              "${CMAKE_CTEST_COMMAND}" "-V")
SET_PROPERTY(TEST integration-CMakeModuleObserverTest APPEND PROPERTY DEPENDS integration-CMakeModuleObserverBuild)                                                            


###########################################################################

IF(OFBUILD_ENABLE_GUI)
  FILE(MAKE_DIRECTORY "${OFBUILD_TESTS_OUTPUT_DATA_DIR}/tests.obsui.cmake")

  OPENFLUID_ADD_TEST(NAME integration-CMakeModuleObserverParamsUIConfigure
                     COMMAND "${CMAKE_COMMAND}"
                              "-E" "chdir" "${OFBUILD_TESTS_OUTPUT_DATA_DIR}/tests.obsui.cmake" 
                              "${CMAKE_COMMAND}" "-G" "${CMAKE_GENERATOR}"
                              "${CMAKE_CURRENT_SOURCE_DIR}/tests.obsui.cmake"
                              "-DOpenFLUIDHelpers_DIR=${OFBUILD_DIST_CMAKE_MODULES_DIR}"
                              "-DOpenFLUID_DIR=${OFBUILD_DIST_CMAKE_MODULES_DIR}"
                              "-DOpenFLUID_ROOT_DIR=${OFBUILD_DIST_DIR}"
                              "-DOpenFLUID_EXTRA_SEARCH_PATHS=${CMAKE_BINARY_DIR}/src"                              
                              "-DOBS_INCLUDE_DIRS=${CMAKE_SOURCE_DIR}/src"
                      PRE_TEST EMPTY_DIRECTORY "${OFBUILD_TESTS_OUTPUT_DATA_DIR}/tests.obsui.cmake")                              
ENDIF()


###########################################################################


IF(OFBUILD_ENABLE_GUI)
  OPENFLUID_ADD_TEST(NAME integration-CMakeModuleObserverParamsUIBuild
                     COMMAND "${CMAKE_COMMAND}"
                              "-E" "chdir" "${OFBUILD_TESTS_OUTPUT_DATA_DIR}/tests.obsui.cmake" 
                              "${CMAKE_COMMAND}" "--build" "${OFBUILD_TESTS_OUTPUT_DATA_DIR}/tests.obsui.cmake"
                              "--target" "install"
                              "--clean-first")
  SET_PROPERTY(TEST integration-CMakeModuleObserverParamsUIBuild APPEND PROPERTY DEPENDS integration-CMakeModuleObserverParamsUIConfigure)                                                            
ENDIF()


###########################################################################


IF(OFBUILD_ENABLE_APP_BUILDER)
  FILE(MAKE_DIRECTORY "${OFBUILD_TESTS_OUTPUT_DATA_DIR}/tests.bext.cmake")
  OPENFLUID_ADD_TEST(NAME integration-CMakeModuleBuilderextConfigure
                     COMMAND "${CMAKE_COMMAND}"
                        "-E" "chdir" "${OFBUILD_TESTS_OUTPUT_DATA_DIR}/tests.bext.cmake" 
                        "${CMAKE_COMMAND}" "-G" "${CMAKE_GENERATOR}"
                        "${CMAKE_CURRENT_SOURCE_DIR}/tests.bext.cmake"
                        "-DOpenFLUIDHelpers_DIR=${OFBUILD_DIST_CMAKE_MODULES_DIR}"
                        "-DOpenFLUID_DIR=${OFBUILD_DIST_CMAKE_MODULES_DIR}"
                        "-DOpenFLUID_ROOT_DIR=${OFBUILD_DIST_DIR}"
                        "-DOpenFLUID_EXTRA_SEARCH_PATHS=${CMAKE_BINARY_DIR}/src"                              
                        "-DBEXT_INCLUDE_DIRS=${CMAKE_SOURCE_DIR}/src"
                      PRE_TEST EMPTY_DIRECTORY "${OFBUILD_TESTS_OUTPUT_DATA_DIR}/tests.bext.cmake")         
ENDIF()


###########################################################################


IF(OFBUILD_ENABLE_APP_BUILDER)
  OPENFLUID_ADD_TEST(NAME integration-CMakeModuleBuilderextBuild
                     COMMAND "${CMAKE_COMMAND}"
                             "-E" "chdir" "${OFBUILD_TESTS_OUTPUT_DATA_DIR}/tests.bext.cmake" 
                             "${CMAKE_COMMAND}" "--build" "${OFBUILD_TESTS_OUTPUT_DATA_DIR}/tests.bext.cmake")
  SET_PROPERTY(TEST integration-CMakeModuleBuilderextBuild APPEND PROPERTY DEPENDS integration-CMakeModuleBuilderextConfigure)                                                            
ENDIF()


###########################################################################


IF(OFBUILD_ENABLE_GUI)
  OPENFLUID_ADD_TEST(NAME integration-CMakeModuleRunWithUI
                     COMMAND "${OFBUILD_DIST_BIN_DIR}/${OPENFLUID_CMD_APP}" 
                             "run"
                             "${OFBUILD_TESTS_INPUT_DATASETS_DIR}/OPENFLUID.IN.CMakeModuleWithUI"
                             "${OFBUILD_TESTS_OUTPUT_DATA_DIR}/OPENFLUID.OUT.CMakeModuleWithUI" 
                     PRE_TEST REMOVE_DIRECTORY "${OFBUILD_TESTS_OUTPUT_DATA_DIR}/OPENFLUID.OUT.CMakeModuleWithUI")
  SET_PROPERTY(TEST integration-CMakeModuleRunWithUI APPEND PROPERTY ENVIRONMENT "LD_LIBRARY_PATH=${OFBUILD_DIST_LIB_DIR}")
  SET_PROPERTY(TEST integration-CMakeModuleRunWithUI APPEND PROPERTY DEPENDS integration-CMakeModuleSimulatorBuild)
  SET_PROPERTY(TEST integration-CMakeModuleRunWithUI APPEND PROPERTY DEPENDS integration-CMakeModuleSimulatorParamsUIBuild)
  SET_PROPERTY(TEST integration-CMakeModuleRunWithUI APPEND PROPERTY DEPENDS integration-CMakeModuleObserverBuild)
  SET_PROPERTY(TEST integration-CMakeModuleRunWithUI APPEND PROPERTY DEPENDS integration-CMakeModuleObserverParamsUIBuild)
ELSE()
  OPENFLUID_ADD_TEST(NAME integration-CMakeModuleRun
                     COMMAND "${OFBUILD_DIST_BIN_DIR}/${OPENFLUID_CMD_APP}" 
                             "run"
                             "${OFBUILD_TESTS_INPUT_DATASETS_DIR}/OPENFLUID.IN.CMakeModule"
                             "${OFBUILD_TESTS_OUTPUT_DATA_DIR}/OPENFLUID.OUT.CMakeModule" 
                     PRE_TEST REMOVE_DIRECTORY "${OFBUILD_TESTS_OUTPUT_DATA_DIR}/OPENFLUID.OUT.CMakeModule")
  SET_PROPERTY(TEST integration-CMakeModuleRun APPEND PROPERTY ENVIRONMENT "LD_LIBRARY_PATH=${OFBUILD_DIST_LIB_DIR}")
  SET_PROPERTY(TEST integration-CMakeModuleRun APPEND PROPERTY DEPENDS integration-CMakeModuleSimulatorBuild)
  SET_PROPERTY(TEST integration-CMakeModuleRun APPEND PROPERTY DEPENDS integration-CMakeModuleObserverBuild)
ENDIF()


###########################################################################


IF(OFBUILD_ENABLE_SIM2DOC)
  OPENFLUID_ADD_TEST(NAME integration-BuddySim2DocHelp 
                     COMMAND "${OFBUILD_DIST_BIN_DIR}/${OPENFLUID_CMD_APP}"
                             "buddy" 
                             "--buddy-help" 
                             "sim2doc"
                    )
ENDIF()


###########################################################################


IF(OFBUILD_ENABLE_SIM2DOC)
  OPENFLUID_ADD_TEST(NAME integration-BuddySim2Doc
                     COMMAND "${OFBUILD_DIST_BIN_DIR}/${OPENFLUID_CMD_APP}"
                             "buddy" 
                             "sim2doc"
                             "--options=inputcpp=${CMAKE_SOURCE_DIR}/src/tests/simulators/tests.sim2doc/Sim2DocSim.cpp,outputdir=${OFBUILD_TESTS_OUTPUT_DATA_DIR}/sim2doc"
                     PRE_TEST REMOVE_DIRECTORY "${OFBUILD_TESTS_OUTPUT_DATA_DIR}/sim2doc"                              
                    )
ENDIF()  


###########################################################################


IF(OFBUILD_ENABLE_SIM2DOC)
  OPENFLUID_ADD_TEST(NAME integration-BuddySim2DocPDF
                     COMMAND "${OFBUILD_DIST_BIN_DIR}/${OPENFLUID_CMD_APP}"
                                "buddy" 
                                "sim2doc"
                                "-o" "inputcpp=${CMAKE_SOURCE_DIR}/src/tests/simulators/tests.sim2doc/Sim2DocSim.cpp,outputdir=${OFBUILD_TESTS_OUTPUT_DATA_DIR}/sim2doc-PDF,tplfile=${CMAKE_SOURCE_DIR}/${OPENFLUID_SHARE_COMMON_INSTALL_PATH}/sim2doc_tpl.tex,pdf=1"
                     PRE_TEST REMOVE_DIRECTORY "${OFBUILD_TESTS_OUTPUT_DATA_DIR}/sim2doc-PDF"           
                    )
ENDIF()


###########################################################################


IF(OFBUILD_ENABLE_SIM2DOC)
  OPENFLUID_ADD_TEST(NAME integration-BuddySim2DocPDFHTML
                     COMMAND "${OFBUILD_DIST_BIN_DIR}/${OPENFLUID_CMD_APP}"
                             "buddy" 
                             "sim2doc"
                             "-o" "inputcpp=${CMAKE_SOURCE_DIR}/src/tests/simulators/tests.sim2doc/Sim2DocSim.cpp,outputdir=${OFBUILD_TESTS_OUTPUT_DATA_DIR}/sim2doc-PDF-HTML,tplfile=${CMAKE_SOURCE_DIR}/${OPENFLUID_SHARE_COMMON_INSTALL_PATH}/sim2doc_tpl.tex,pdf=1,html=1"
                     PRE_TEST REMOVE_DIRECTORY "${OFBUILD_TESTS_OUTPUT_DATA_DIR}/sim2doc-PDF-HTML"           
                    )
ENDIF()


###########################################################################


OPENFLUID_ADD_TEST(NAME integration-BuddyExamplesHelp 
                   COMMAND "${OFBUILD_DIST_BIN_DIR}/${OPENFLUID_CMD_APP}"
                           "buddy"
                           "examples"
                           "--buddy-help" 
                  )


###########################################################################


OPENFLUID_ADD_TEST(NAME integration-BuddyExamplesInstallSimulator
                   COMMAND "${OFBUILD_DIST_BIN_DIR}/${OPENFLUID_CMD_APP}"
                              "buddy" "examples"
                              "-o" "selection=single,simulator=fire.surf.prod-spread,sourcedir=${OFBUILD_SOURCE_EXAMPLES_DIR},installdir=${OFBUILD_TESTS_OUTPUT_DATA_DIR}/examplesbuddy,force=1"
                   PRE_TEST REMOVE_DIRECTORY "${OFBUILD_TESTS_OUTPUT_DATA_DIR}/examplesbuddy"       
                   POST_TEST CHECK_FILE_EXIST "${OFBUILD_TESTS_OUTPUT_DATA_DIR}/examplesbuddy/wares-dev/simulators/fire.surf.prod-spread/FireProdSpreadSim.cpp"
                  )


###########################################################################


OPENFLUID_ADD_TEST(NAME integration-BuddyExamplesInstallMissingSimulator
                   COMMAND "${OFBUILD_DIST_BIN_DIR}/${OPENFLUID_CMD_APP}"
                              "buddy" "examples"
                              "-o" "selection=single,simulator=fake.name,sourcedir=${OFBUILD_SOURCE_EXAMPLES_DIR},installdir=${OFBUILD_TESTS_OUTPUT_DATA_DIR}/examplesbuddy,force=1"
                   PRE_TEST REMOVE_DIRECTORY "${OFBUILD_TESTS_OUTPUT_DATA_DIR}/examplesbuddy"
                  )


###########################################################################


OPENFLUID_ADD_TEST(NAME integration-BuddyExamplesInstallAll
                   COMMAND "${OFBUILD_DIST_BIN_DIR}/${OPENFLUID_CMD_APP}"
                              "buddy" "examples"
                              "-o" "selection=*,sourcedir=${OFBUILD_SOURCE_EXAMPLES_DIR},installdir=${OFBUILD_TESTS_OUTPUT_DATA_DIR}/examplesbuddy,force=1"
                   PRE_TEST REMOVE_DIRECTORY "${OFBUILD_TESTS_OUTPUT_DATA_DIR}/examplesbuddy"                              
                  )
SET_PROPERTY(TEST integration-BuddyExamplesInstallAll APPEND PROPERTY DEPENDS integration-BuddyExamplesInstallSimulator)
SET_PROPERTY(TEST integration-BuddyExamplesInstallAll APPEND PROPERTY DEPENDS integration-BuddyExamplesInstallMissingSimulator)


###########################################################################


OPENFLUID_ADD_TEST(NAME integration-BuddyExamplesRunPrimitives 
                   COMMAND "${OFBUILD_DIST_BIN_DIR}/${OPENFLUID_CMD_APP}"
                           "run" 
                           "${OFBUILD_TESTS_OUTPUT_DATA_DIR}/examplesbuddy/projects/Primitives"                                                   
                    POST_TEST CHECK_FILE_EXIST "${OFBUILD_TESTS_OUTPUT_DATA_DIR}/examplesbuddy/projects/Primitives/OUT/openfluid-messages.log"
                              CHECK_FILE_EXIST "${OFBUILD_TESTS_OUTPUT_DATA_DIR}/examplesbuddy/projects/Primitives/OUT/fullA_unitsA1_var1.csv"
                              CHECK_FILE_EXIST "${OFBUILD_TESTS_OUTPUT_DATA_DIR}/examplesbuddy/projects/Primitives/OUT/fullB_unitsB2_var5.csv"
                   )
SET_PROPERTY(TEST integration-BuddyExamplesRunPrimitives APPEND PROPERTY DEPENDS integration-BuddyExamplesInstallAll)


###########################################################################


OPENFLUID_ADD_TEST(NAME integration-BuddyNewSimHelp
                   COMMAND "${OFBUILD_DIST_BIN_DIR}/${OPENFLUID_CMD_APP}"
                           "buddy"
                           "--buddy-help" "newsim"
                  )



###########################################################################


FILE(MAKE_DIRECTORY "${OFBUILD_TESTS_OUTPUT_DATA_DIR}/tests.buddy.newsim")
CONFIGURE_FILE("${OFBUILD_TESTS_INPUT_MISCDATA_DIR}/tests.buddy.newsim/CMakeLists.txt.in" "${OFBUILD_TESTS_OUTPUT_DATA_DIR}/tests.buddy.newsim/CMakeLists.txt" @ONLY)
    
OPENFLUID_ADD_TEST(NAME integration-BuddyNewSim
                   COMMAND "${OFBUILD_DIST_BIN_DIR}/${OPENFLUID_CMD_APP}"
                              "buddy"
                              "newsim"
                              "-o" "simid=tests.buddy.newsim,cppclass=NewSimulatorUsingBuddy,outputdir=${OFBUILD_TESTS_OUTPUT_DATA_DIR}/tests.buddy.newsim,authorname=John Doe,authoremail=doe@foo.bar.org"
                             )


###########################################################################


OPENFLUID_ADD_TEST(NAME integration-BuddyNewSimConfigure
                   COMMAND "${CMAKE_COMMAND}"
                            "-E" "chdir" "${OFBUILD_TESTS_OUTPUT_DATA_DIR}/tests.buddy.newsim" "${CMAKE_COMMAND}" "-G" "${CMAKE_GENERATOR}"
                            "-DOpenFLUIDHelpers_DIR=${OFBUILD_DIST_CMAKE_MODULES_DIR}"
                            "-DOpenFLUID_DIR=${OFBUILD_DIST_CMAKE_MODULES_DIR}"
                            "-DOpenFLUID_ROOT_DIR=${OFBUILD_DIST_DIR}"
                            "-DOpenFLUID_EXTRA_SEARCH_PATHS=${CMAKE_BINARY_DIR}/src"
                            "-DSIM_INCLUDE_DIRS=${CMAKE_SOURCE_DIR}/src"
                            ".")
SET_PROPERTY(TEST integration-BuddyNewSimConfigure APPEND PROPERTY DEPENDS integration-BuddyNewSim)



###########################################################################


OPENFLUID_ADD_TEST(NAME integration-BuddyNewSimBuild
                   COMMAND "${CMAKE_COMMAND}"
                              "-E" "chdir" "${OFBUILD_TESTS_OUTPUT_DATA_DIR}/tests.buddy.newsim" "${CMAKE_BUILD_TOOL}"
                              )
SET_PROPERTY(TEST integration-BuddyNewSimBuild APPEND PROPERTY DEPENDS integration-BuddyNewSimConfigure)


###########################################################################


OPENFLUID_ADD_TEST(NAME integration-BuddyNewSimCheck
                   COMMAND "${OFBUILD_DIST_BIN_DIR}/${OPENFLUID_CMD_APP}" 
                         "report" "simulators"
                         "-p" "${OFBUILD_TESTS_OUTPUT_DATA_DIR}/tests.buddy.newsim")
SET_TESTS_PROPERTIES(integration-BuddyNewSimCheck PROPERTIES PASS_REGULAR_EXPRESSION "tests.buddy.newsim"
                                                                PASS_REGULAR_EXPRESSION "John Doe"
                                                                PASS_REGULAR_EXPRESSION "doe@foo.bar.org") 
SET_PROPERTY(TEST integration-BuddyNewSimCheck APPEND PROPERTY DEPENDS integration-BuddyNewSimBuild)                                                                 
                       


###########################################################################


OPENFLUID_ADD_TEST(NAME integration-BuddyNewSimRun
                   COMMAND "${OFBUILD_DIST_BIN_DIR}/${OPENFLUID_CMD_APP}" 
                                       "run"
                                       "${OFBUILD_TESTS_INPUT_DATASETS_DIR}/OPENFLUID.IN.BuddyNewSim"
                                       "${OFBUILD_TESTS_OUTPUT_DATA_DIR}/OPENFLUID.OUT.BuddyNewSim" 
                                       "-p" "${OFBUILD_TESTS_OUTPUT_DATA_DIR}/tests.buddy.newsim"
                                       "-v")
SET_PROPERTY(TEST integration-BuddyNewSimRun APPEND PROPERTY DEPENDS integration-BuddyNewSimBuild)


###########################################################################


OPENFLUID_ADD_TEST(NAME integration-BuddyNewDataHelp
                   COMMAND "${OFBUILD_DIST_BIN_DIR}/${OPENFLUID_CMD_APP}"
                              "buddy"
                              "--buddy-help" "newdata"
                              )


###########################################################################


OPENFLUID_ADD_TEST(NAME integration-BuddyNewData
                   COMMAND "${OFBUILD_DIST_BIN_DIR}/${OPENFLUID_CMD_APP}"
                              "buddy" "newdata"
                              "--options=outputdir=${OFBUILD_TESTS_OUTPUT_DATA_DIR}/OPENFLUID.IN.NewData"
                              )


###########################################################################


OPENFLUID_ADD_TEST(NAME integration-BuddyNewDataCheck
                   COMMAND "${OFBUILD_DIST_BIN_DIR}/${OPENFLUID_CMD_APP}"
                           "run"
                                       "${OFBUILD_TESTS_OUTPUT_DATA_DIR}/OPENFLUID.IN.NewData"
                                       "${OFBUILD_TESTS_OUTPUT_DATA_DIR}/OPENFLUID.OUT.NewData"
                                       "-p" "${OFBUILD_TESTS_BINARY_DIR}"
                              )
SET_PROPERTY(TEST integration-BuddyNewDataCheck APPEND PROPERTY DEPENDS integration-BuddyNewData)


###########################################################################


OPENFLUID_ADD_TEST(NAME integration-GitAskPass
                   COMMAND "${OFBUILD_DIST_BIN_DIR}/${OPENFLUID_GITASKPASS_APP}")
SET_TESTS_PROPERTIES(integration-GitAskPass PROPERTIES ENVIRONMENT "${OFBUILD_GITASKPASS_ENVVAR}=mysecrtpasswrd")                           
SET_TESTS_PROPERTIES(integration-GitAskPass PROPERTIES PASS_REGULAR_EXPRESSION "mysecrtpasswrd")


###########################################################################


IF(OFBUILD_ENABLE_APP_MINIMAL)
  OPENFLUID_ADD_TEST(NAME integration-MinimalRun
                     COMMAND "${OFBUILD_DIST_BIN_DIR}/openfluid-minimal"
                                  "${OFBUILD_TESTS_INPUT_DATASETS_DIR}/OPENFLUID.IN.PrimitivesHopla"
                                  "${OFBUILD_TESTS_OUTPUT_DATA_DIR}/OPENFLUID.OUT.MinimalRun"
                                  "${OFBUILD_TESTS_BINARY_DIR}"
                              )
ENDIF()  

###########################################################################


IF (OFBUILD_ENABLE_APP_MINIMAL)
  OPENFLUID_ADD_TEST(NAME integration-MinimalFailsArgs
                     COMMAND "${OFBUILD_DIST_BIN_DIR}/openfluid-minimal"
                                  "${OFBUILD_TESTS_INPUT_DATASETS_DIR}/OPENFLUID.IN.VariableTimeProd"
                                  "${OFBUILD_TESTS_OUTPUT_DATA_DIR}/OPENFLUID.OUT.MinimalFailsArgs"
                              )
  SET_TESTS_PROPERTIES(integration-MinimalFailsArgs PROPERTIES WILL_FAIL TRUE)
ENDIF()

###########################################################################


IF(OFBUILD_ENABLE_APP_MINIMAL)
  OPENFLUID_ADD_TEST(NAME integration-MinimalFailsInput
                   COMMAND "${OFBUILD_DIST_BIN_DIR}/openfluid-minimal"
                                  "${OFBUILD_TESTS_INPUT_DATASETS_DIR}/OPENFLUID.IN.doesnotexist"
                                  "${OFBUILD_TESTS_OUTPUT_DATA_DIR}/OPENFLUID.OUT.MinimalFailsInput"
                                  "${OFBUILD_TESTS_BINARY_DIR}"
                              )                              
  SET_TESTS_PROPERTIES(integration-MinimalFailsInput PROPERTIES WILL_FAIL TRUE)
ENDIF() 
 
###########################################################################


OPENFLUID_ADD_TEST(NAME integration-Debug 
                   COMMAND "${OFBUILD_DIST_BIN_DIR}/${OPENFLUID_CMD_APP}" 
                           "report" "simulators" "--list"
                           "-p" "${OFBUILD_TESTS_BINARY_DIR}")
IF(${CMAKE_BUILD_TYPE} STREQUAL "Debug")
  SET_TESTS_PROPERTIES(integration-Debug PROPERTIES PASS_REGULAR_EXPRESSION "OpenFLUID debugging mode is enabled")
ELSE()
  SET_TESTS_PROPERTIES(integration-Debug PROPERTIES FAIL_REGULAR_EXPRESSION "OpenFLUID debugging mode is enabled")
ENDIF() 
