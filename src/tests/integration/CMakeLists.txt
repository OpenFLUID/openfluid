FIND_PACKAGE(Boost REQUIRED COMPONENTS unit_test_framework)  


ADD_DEFINITIONS(-DOPENFLUID_VERSION=${FULL_VERSION})

INCLUDE_DIRECTORIES(${Boost_INCLUDE_DIRS}
                    "${PROJECT_SOURCE_DIR}/src" "${PROJECT_BINARY_DIR}/src/tests")


###########################################################################


OPENFLUID_ADD_TEST(NAME integration-DisplayHelp
                   COMMAND "${BIN_OUTPUT_PATH}/${OPENFLUID_CMD_APP}" "-h")
SET_TESTS_PROPERTIES(integration-DisplayHelp PROPERTIES PASS_REGULAR_EXPRESSION "${OPENFLUID_CMD_APP} allowed options")  


###########################################################################


OPENFLUID_ADD_TEST(NAME integration-DisplayVersion 
                   COMMAND "${BIN_OUTPUT_PATH}/${OPENFLUID_CMD_APP}" 
                           "--version")
  SET_TESTS_PROPERTIES(integration-DisplayVersion PROPERTIES PASS_REGULAR_EXPRESSION ${FULL_VERSION})  


###########################################################################


OPENFLUID_ADD_TEST(NAME integration-ShowPaths 
                   COMMAND "${BIN_OUTPUT_PATH}/${OPENFLUID_CMD_APP}" 
                       "--show-paths"
                   )  
SET_TESTS_PROPERTIES(integration-ShowPaths PROPERTIES FAIL_REGULAR_EXPRESSION "#1 foobar")
SET_TESTS_PROPERTIES(integration-ShowPaths PROPERTIES FAIL_REGULAR_EXPRESSION "#1 barfoo")
SET_TESTS_PROPERTIES(integration-ShowPaths PROPERTIES PASS_REGULAR_EXPRESSION "${OPENFLUID_TESTS_USERDATA_PATH}")
SET_TESTS_PROPERTIES(integration-ShowPaths PROPERTIES PASS_REGULAR_EXPRESSION "${OPENFLUID_TESTS_TEMP_PATH}")


###########################################################################


OPENFLUID_ADD_TEST(NAME integration-ShowPathsNoEnvVars
                   COMMAND "${BIN_OUTPUT_PATH}/${OPENFLUID_CMD_APP}" 
                       "--show-paths"
                       "-p" "foobar"
                       "-n" "barfoo")
SET_TESTS_PROPERTIES(integration-ShowPathsNoEnvVars PROPERTIES ENVIRONMENT "")                  
SET_TESTS_PROPERTIES(integration-ShowPathsNoEnvVars PROPERTIES PASS_REGULAR_EXPRESSION "#1 foobar")
SET_TESTS_PROPERTIES(integration-ShowPathsNoEnvVars PROPERTIES PASS_REGULAR_EXPRESSION "#1 barfoo")
SET_TESTS_PROPERTIES(integration-ShowPathsNoEnvVars PROPERTIES FAIL_REGULAR_EXPRESSION "${OPENFLUID_TESTS_USERDATA_PATH}")
SET_TESTS_PROPERTIES(integration-ShowPathsNoEnvVars PROPERTIES FAIL_REGULAR_EXPRESSION "${OPENFLUID_TESTS_TEMP_PATH}")
  

###########################################################################


OPENFLUID_ADD_TEST(NAME integration-SimulatorsList 
                   COMMAND "${BIN_OUTPUT_PATH}/${OPENFLUID_CMD_APP}" 
                           "-f"
                           "-p${TEST_OUTPUT_PATH}")
  

###########################################################################


OPENFLUID_ADD_TEST(NAME integration-SimulatorsReport 
                   COMMAND "${BIN_OUTPUT_PATH}/${OPENFLUID_CMD_APP}"
                           "-r"
                           "-p${TEST_OUTPUT_PATH}")
  

###########################################################################


OPENFLUID_ADD_TEST(NAME integration-ObserversList 
                   COMMAND "${BIN_OUTPUT_PATH}/${OPENFLUID_CMD_APP}" 
                           "-e"
                           "-n${TEST_OUTPUT_PATH}")
SET_TESTS_PROPERTIES(integration-ObserversList PROPERTIES PASS_REGULAR_EXPRESSION "tests.empty")  
                         
 
###########################################################################


OPENFLUID_ADD_TEST(NAME integration-ObserversReport
                   COMMAND "${BIN_OUTPUT_PATH}/${OPENFLUID_CMD_APP}" 
                           "-l"
                           "-n${TEST_OUTPUT_PATH}")  
SET_TESTS_PROPERTIES(integration-ObserversReport PROPERTIES PASS_REGULAR_EXPRESSION "empty observer for tests")
  

###########################################################################

  
IF(CUSTOM_CMAKE_VERSION VERSION_EQUAL "2.8.0" OR CUSTOM_CMAKE_VERSION VERSION_GREATER "2.8.0") 
  OPENFLUID_ADD_TEST(NAME integration-SimulatorsReportEnvVar 
                     COMMAND "${BIN_OUTPUT_PATH}/${OPENFLUID_CMD_APP}" "-r")
  SET_TESTS_PROPERTIES(integration-SimulatorsReportEnvVar 
                       PROPERTIES ENVIRONMENT "OPENFLUID_SIMS_PATH=${TEST_OUTPUT_PATH};OPENFLUID_TEMP_PATH=${OPENFLUID_TESTS_TEMP_PATH};OPENFLUID_USERDATA_PATH=${OPENFLUID_TESTS_USERDATA_PATH}")                           
  SET_TESTS_PROPERTIES(integration-SimulatorsReportEnvVar PROPERTIES PASS_REGULAR_EXPRESSION "tests.primitives.variables.use")  
  SET_TESTS_PROPERTIES(integration-SimulatorsReportEnvVar PROPERTIES PASS_REGULAR_EXPRESSION "tests.primitives.variables.prod")    
ENDIF(CUSTOM_CMAKE_VERSION VERSION_EQUAL "2.8.0" OR CUSTOM_CMAKE_VERSION VERSION_GREATER "2.8.0")



###########################################################################


OPENFLUID_ADD_TEST(NAME integration-SimulatorsReportNoEnvVar 
                   COMMAND "${BIN_OUTPUT_PATH}/${OPENFLUID_CMD_APP}" "-r")                           
SET_TESTS_PROPERTIES(integration-SimulatorsReportNoEnvVar PROPERTIES PASS_REGULAR_EXPRESSION "tests.primitives.variables.use")  
SET_TESTS_PROPERTIES(integration-SimulatorsReportNoEnvVar PROPERTIES PASS_REGULAR_EXPRESSION "tests.primitives.variables.prod")
SET_TESTS_PROPERTIES(integration-SimulatorsReportNoEnvVar PROPERTIES WILL_FAIL TRUE)        


###########################################################################


OPENFLUID_ADD_TEST(NAME integration-SimulatorsMatchReport 
                   COMMAND "${BIN_OUTPUT_PATH}/${OPENFLUID_CMD_APP}"
                           "-u" "tests.primitives.*"
                           "-p${TEST_OUTPUT_PATH}")
SET_TESTS_PROPERTIES(integration-SimulatorsMatchReport PROPERTIES PASS_REGULAR_EXPRESSION "tests.primitives.variables.use")  
SET_TESTS_PROPERTIES(integration-SimulatorsMatchReport PROPERTIES PASS_REGULAR_EXPRESSION "tests.primitives.variables.prod")
                           

###########################################################################

OPENFLUID_ADD_TEST(NAME integration-SimulatorsXMLReport 
                   COMMAND "${BIN_OUTPUT_PATH}/${OPENFLUID_CMD_APP}"
                           "-x"
                           "-p${TEST_OUTPUT_PATH}")
SET_TESTS_PROPERTIES(integration-SimulatorsXMLReport PROPERTIES WILL_FAIL TRUE)        
                              
  
###########################################################################


OPENFLUID_ADD_TEST(NAME integration-InputDoesNotExist 
                   COMMAND "${BIN_OUTPUT_PATH}/${OPENFLUID_CMD_APP}" 
                           "-i" "${CMAKE_BINARY_DIR}/path/does/not/exist")                           
SET_TESTS_PROPERTIES(integration-InputDoesNotExist PROPERTIES WILL_FAIL TRUE)


###########################################################################


OPENFLUID_ADD_TEST(NAME integration-InputWithNoModel
                   COMMAND "${BIN_OUTPUT_PATH}/${OPENFLUID_CMD_APP}"
                           "-i" "${TESTS_DATASETS_PATH}/OPENFLUID.IN.NoModel"
                           "-o" "${TESTS_OUTPUTDATA_PATH}/OPENFLUID.OUT.NoModel"
                           "-p" "${TEST_OUTPUT_PATH}"
                    PRE_TEST REMOVE_DIRECTORY "${TESTS_OUTPUTDATA_PATH}/OPENFLUID.OUT.NoModel"
                   )               
SET_TESTS_PROPERTIES(integration-InputWithNoModel PROPERTIES WILL_FAIL TRUE)


###########################################################################


OPENFLUID_ADD_TEST(NAME integration-InputWithNoRun
                   COMMAND "${BIN_OUTPUT_PATH}/${OPENFLUID_CMD_APP}"
                           "-i" "${TESTS_DATASETS_PATH}/OPENFLUID.IN.NoRun"
                           "-o" "${TESTS_OUTPUTDATA_PATH}/OPENFLUID.OUT.NoRun"
                           "-p" "${TEST_OUTPUT_PATH}")                           
SET_TESTS_PROPERTIES(integration-InputWithNoRun PROPERTIES WILL_FAIL TRUE)



###########################################################################


OPENFLUID_ADD_TEST(NAME integration-MissingSimulator
                   COMMAND "${BIN_OUTPUT_PATH}/${OPENFLUID_CMD_APP}"
                                        "-i" "${TESTS_DATASETS_PATH}/OPENFLUID.IN.MissingSim"
                                        "-o" "${TESTS_OUTPUTDATA_PATH}/OPENFLUID.OUT.MissingSim"
                                        "-p" "${TEST_OUTPUT_PATH}")               
SET_TESTS_PROPERTIES(integration-MissingSimulator PROPERTIES WILL_FAIL TRUE)



###########################################################################


OPENFLUID_ADD_TEST(NAME integration-MissingUnitClass
                   COMMAND "${BIN_OUTPUT_PATH}/${OPENFLUID_CMD_APP}"
                                        "-i" "${TESTS_DATASETS_PATH}/OPENFLUID.IN.MissingClass"
                                        "-o" "${TESTS_OUTPUTDATA_PATH}/OPENFLUID.OUT.MissingClass"
                                        "-p" "${TEST_OUTPUT_PATH}")               
SET_TESTS_PROPERTIES(integration-MissingUnitClass PROPERTIES WILL_FAIL TRUE)


###########################################################################


OPENFLUID_ADD_TEST(NAME integration-MissingVariable
                   COMMAND "${BIN_OUTPUT_PATH}/${OPENFLUID_CMD_APP}"
                                        "-i" "${TESTS_DATASETS_PATH}/OPENFLUID.IN.MissingVar"
                                        "-o" "${TESTS_OUTPUTDATA_PATH}/OPENFLUID.OUT.MissingVar"
                                        "-p" "${TEST_OUTPUT_PATH}")               
SET_TESTS_PROPERTIES(integration-MissingVariable PROPERTIES WILL_FAIL TRUE)



###########################################################################


OPENFLUID_ADD_TEST(NAME integration-MissingAttributes
                   COMMAND "${BIN_OUTPUT_PATH}/${OPENFLUID_CMD_APP}"
                                        "-i" "${TESTS_DATASETS_PATH}/OPENFLUID.IN.MissingAttributes"
                                        "-o" "${TESTS_OUTPUTDATA_PATH}/OPENFLUID.OUT.MissingAttributes"
                                        "-p" "${TEST_OUTPUT_PATH}")               
SET_TESTS_PROPERTIES(integration-MissingAttributes PROPERTIES WILL_FAIL TRUE)



###########################################################################


OPENFLUID_ADD_TEST(NAME integration-DuplicateVariable
                   COMMAND "${BIN_OUTPUT_PATH}/${OPENFLUID_CMD_APP}"
                                        "-i" "${TESTS_DATASETS_PATH}/OPENFLUID.IN.DuplicateVar"
                                        "-o" "${TESTS_OUTPUTDATA_PATH}/OPENFLUID.OUT.DuplicateVar"
                                        "-p" "${TEST_OUTPUT_PATH}")               
SET_TESTS_PROPERTIES(integration-DuplicateVariable PROPERTIES WILL_FAIL TRUE)


###########################################################################


OPENFLUID_ADD_TEST(NAME integration-Verbose
                   COMMAND "${BIN_OUTPUT_PATH}/${OPENFLUID_CMD_APP}"
                               "-i" "${TESTS_DATASETS_PATH}/OPENFLUID.IN.VariableTimeProd"
                               "-o" "${TESTS_OUTPUTDATA_PATH}/OPENFLUID.OUT.Verbose"
                               "-p" "${TEST_OUTPUT_PATH}"
                               "-v")
  

###########################################################################


OPENFLUID_ADD_TEST(NAME integration-Quiet
                   COMMAND "${BIN_OUTPUT_PATH}/${OPENFLUID_CMD_APP}"
                               "-i" "${TESTS_DATASETS_PATH}/OPENFLUID.IN.VariableTimeProd"
                               "-o" "${TESTS_OUTPUTDATA_PATH}/OPENFLUID.OUT.Quiet"
                               "-p" "${TEST_OUTPUT_PATH}"
                               "-q")


###########################################################################


FILE(COPY "${TESTS_DATASETS_PATH}/OPENFLUID.PRJ.Primitives" 
       DESTINATION "${TESTS_OUTPUTDATA_PATH}"
       PATTERN ".svn" EXCLUDE)

OPENFLUID_ADD_TEST(NAME integration-Project
                   COMMAND  "${BIN_OUTPUT_PATH}/${OPENFLUID_CMD_APP}"
                               "-w" "${TESTS_OUTPUTDATA_PATH}/OPENFLUID.PRJ.Primitives"
                               "-p" "${TEST_OUTPUT_PATH}"
                               "-c")


###########################################################################


OPENFLUID_ADD_TEST(NAME integration-Profiling
                   COMMAND "${BIN_OUTPUT_PATH}/${OPENFLUID_CMD_APP}"
                               "-i" "${TESTS_DATASETS_PATH}/OPENFLUID.IN.Coupling"
                               "-o" "${TESTS_OUTPUTDATA_PATH}/OPENFLUID.OUT.Profiling"
                               "-p" "${TEST_OUTPUT_PATH}"
                               "-k")


###########################################################################


OPENFLUID_ADD_TEST(NAME integration-FluidXWriterSingle
                   COMMAND "${BIN_OUTPUT_PATH}/${OPENFLUID_CMD_APP}"
                               "-i" "${TESTS_OUTPUTDATA_PATH}/OPENFLUID.OUT.FluidXWriterSingle"
                               "-o" "${TESTS_OUTPUTDATA_PATH}/OPENFLUID.OUT.FluidXWriterRunSingle"
                               "-p" "${TEST_OUTPUT_PATH}")
SET_PROPERTY(TEST integration-FluidXWriterSingle APPEND PROPERTY DEPENDS unit-api-FluidXDescriptor_TEST)


###########################################################################


OPENFLUID_ADD_TEST(NAME integration-FluidXWriterMany
                   COMMAND "${BIN_OUTPUT_PATH}/${OPENFLUID_CMD_APP}"
                               "-i" "${TESTS_OUTPUTDATA_PATH}/OPENFLUID.OUT.FluidXWriterMany"
                               "-o" "${TESTS_OUTPUTDATA_PATH}/OPENFLUID.OUT.FluidXWriterRunMany"
                               "-p" "${TEST_OUTPUT_PATH}")
SET_PROPERTY(TEST integration-FluidXWriterMany APPEND PROPERTY DEPENDS unit-api-FluidXDescriptor_TEST)


###########################################################################


OPENFLUID_ADD_TEST(NAME integration-ParameterizationUI
                   COMMAND "${BIN_OUTPUT_PATH}/${OPENFLUID_CMD_APP}"
                               "-i" "${TESTS_DATASETS_PATH}/OPENFLUID.IN.PrimitivesWithParameterizationUI"
                               "-o" "${TESTS_OUTPUTDATA_PATH}/OPENFLUID.OUT.PrimitivesWithParameterizationUI"
                               "-p" "${TEST_OUTPUT_PATH}"
                               "-n" "${TEST_OUTPUT_PATH}")
                                 

###########################################################################


FILE(MAKE_DIRECTORY "${TESTS_OUTPUTDATA_PATH}/FindOpenFLUID")
  
OPENFLUID_ADD_TEST(NAME integration-CMakeModuleCheck
                   COMMAND "${CMAKE_COMMAND}"
                             "-E" "chdir" "${TESTS_OUTPUTDATA_PATH}/FindOpenFLUID" 
                             "${CMAKE_COMMAND}" "-G" "${CMAKE_GENERATOR}"
                             "${CMAKE_CURRENT_SOURCE_DIR}/FindOpenFLUID"
                             "-DOpenFLUID_DIR=${CMAKE_MODULES_OUTPUT_PATH}"
                             "-DOpenFLUID_ROOT_DIR=${BUILD_OUTPUT_PATH}"
                             "-DTEST_SEARCH_PATHS1=${CMAKE_BINARY_DIR}/src"
                             "-DTEST_SEARCH_PATHS2=${CMAKE_SOURCE_DIR}/src"
                   PRE_TEST EMPTY_DIRECTORY "${TESTS_OUTPUTDATA_PATH}/FindOpenFLUID")                                                                                          


###########################################################################


FILE(MAKE_DIRECTORY "${TESTS_OUTPUTDATA_PATH}/tests.sim.cmake")
  
OPENFLUID_ADD_TEST(NAME integration-CMakeModuleSimulatorConfigure
                   COMMAND "${CMAKE_COMMAND}"
                             "-E" "chdir" "${TESTS_OUTPUTDATA_PATH}/tests.sim.cmake" 
                             "${CMAKE_COMMAND}" "-G" "${CMAKE_GENERATOR}"
                             "${CMAKE_CURRENT_SOURCE_DIR}/tests.sim.cmake"
                             "-DOpenFLUIDHelpers_DIR=${CMAKE_MODULES_OUTPUT_PATH}"
                             "-DOpenFLUID_DIR=${CMAKE_MODULES_OUTPUT_PATH}"
                             "-DOpenFLUID_ROOT_DIR=${BUILD_OUTPUT_PATH}"
                             "-DOpenFLUID_EXTRA_SEARCH_PATHS=${CMAKE_BINARY_DIR}/src"
                             "-DSIM_INCLUDE_DIRS=${CMAKE_SOURCE_DIR}/src"
                             "-DSIM_SIM2DOC_MODE=off"
                   PRE_TEST EMPTY_DIRECTORY "${TESTS_OUTPUTDATA_PATH}/tests.sim.cmake")                                                                                          


###########################################################################


OPENFLUID_ADD_TEST(NAME integration-CMakeModuleSimulatorBuild
                   COMMAND "${CMAKE_COMMAND}"
                           "-E" "chdir" "${TESTS_OUTPUTDATA_PATH}/tests.sim.cmake" 
                           "${CMAKE_COMMAND}" "--build" "${TESTS_OUTPUTDATA_PATH}/tests.sim.cmake"
                           "--target" "install"
                           "--clean-first")
SET_PROPERTY(TEST integration-CMakeModuleSimulatorBuild APPEND PROPERTY DEPENDS integration-CMakeModuleSimulatorConfigure)                                                            


###########################################################################


FILE(MAKE_DIRECTORY "${TESTS_OUTPUTDATA_PATH}/tests.simui.cmake")

OPENFLUID_ADD_TEST(NAME integration-CMakeModuleSimulatorParamsUIConfigure
                   COMMAND "${CMAKE_COMMAND}"
                              "-E" "chdir" "${TESTS_OUTPUTDATA_PATH}/tests.simui.cmake" 
                              "${CMAKE_COMMAND}" "-G" "${CMAKE_GENERATOR}"
                              "${CMAKE_CURRENT_SOURCE_DIR}/tests.simui.cmake"
                              "-DOpenFLUIDHelpers_DIR=${CMAKE_MODULES_OUTPUT_PATH}"
                              "-DOpenFLUID_DIR=${CMAKE_MODULES_OUTPUT_PATH}"
                              "-DOpenFLUID_ROOT_DIR=${BUILD_OUTPUT_PATH}"
                              "-DOpenFLUID_EXTRA_SEARCH_PATHS=${CMAKE_BINARY_DIR}/src"                              
                              "-DSIM_INCLUDE_DIRS=${CMAKE_SOURCE_DIR}/src"
                    PRE_TEST EMPTY_DIRECTORY "${TESTS_OUTPUTDATA_PATH}/tests.simui.cmake")                              
                              

###########################################################################


OPENFLUID_ADD_TEST(NAME integration-CMakeModuleSimulatorParamsUIBuild
                   COMMAND "${CMAKE_COMMAND}"
                              "-E" "chdir" "${TESTS_OUTPUTDATA_PATH}/tests.simui.cmake" 
                              "${CMAKE_COMMAND}" "--build" "${TESTS_OUTPUTDATA_PATH}/tests.simui.cmake"
                              "--target" "install"
                              "--clean-first")
SET_PROPERTY(TEST integration-CMakeModuleSimulatorParamsUIBuild APPEND PROPERTY DEPENDS integration-CMakeModuleSimulatorParamsUIConfigure)


###########################################################################


FILE(MAKE_DIRECTORY "${TESTS_OUTPUTDATA_PATH}/tests.obs.cmake")

OPENFLUID_ADD_TEST(NAME integration-CMakeModuleObserverConfigure
                   COMMAND "${CMAKE_COMMAND}"
                              "-E" "chdir" "${TESTS_OUTPUTDATA_PATH}/tests.obs.cmake" 
                              "${CMAKE_COMMAND}" "-G" "${CMAKE_GENERATOR}"
                              "${CMAKE_CURRENT_SOURCE_DIR}/tests.obs.cmake"
                              "-DOpenFLUIDHelpers_DIR=${CMAKE_MODULES_OUTPUT_PATH}"
                              "-DOpenFLUID_DIR=${CMAKE_MODULES_OUTPUT_PATH}"
                              "-DOpenFLUID_ROOT_DIR=${BUILD_OUTPUT_PATH}"
                              "-DOpenFLUID_EXTRA_SEARCH_PATHS=${CMAKE_BINARY_DIR}/src"                              
                              "-DOBS_INCLUDE_DIRS=${CMAKE_SOURCE_DIR}/src"
                    PRE_TEST EMPTY_DIRECTORY "${TESTS_OUTPUTDATA_PATH}/tests.obs.cmake")                              
                              

###########################################################################


OPENFLUID_ADD_TEST(NAME integration-CMakeModuleObserverBuild
                   COMMAND "${CMAKE_COMMAND}"
                              "-E" "chdir" "${TESTS_OUTPUTDATA_PATH}/tests.obs.cmake" 
                              "${CMAKE_COMMAND}" "--build" "${TESTS_OUTPUTDATA_PATH}/tests.obs.cmake"
                              "--target" "install"
                              "--clean-first")
SET_PROPERTY(TEST integration-CMakeModuleObserverBuild APPEND PROPERTY DEPENDS integration-CMakeModuleObserverConfigure)                                                            


###########################################################################


FILE(MAKE_DIRECTORY "${TESTS_OUTPUTDATA_PATH}/tests.obsui.cmake")

OPENFLUID_ADD_TEST(NAME integration-CMakeModuleObserverParamsUIConfigure
                   COMMAND "${CMAKE_COMMAND}"
                              "-E" "chdir" "${TESTS_OUTPUTDATA_PATH}/tests.obsui.cmake" 
                              "${CMAKE_COMMAND}" "-G" "${CMAKE_GENERATOR}"
                              "${CMAKE_CURRENT_SOURCE_DIR}/tests.obsui.cmake"
                              "-DOpenFLUIDHelpers_DIR=${CMAKE_MODULES_OUTPUT_PATH}"
                              "-DOpenFLUID_DIR=${CMAKE_MODULES_OUTPUT_PATH}"
                              "-DOpenFLUID_ROOT_DIR=${BUILD_OUTPUT_PATH}"
                              "-DOpenFLUID_EXTRA_SEARCH_PATHS=${CMAKE_BINARY_DIR}/src"                              
                              "-DOBS_INCLUDE_DIRS=${CMAKE_SOURCE_DIR}/src"
                    PRE_TEST EMPTY_DIRECTORY "${TESTS_OUTPUTDATA_PATH}/tests.obsui.cmake")                              
                              

###########################################################################


OPENFLUID_ADD_TEST(NAME integration-CMakeModuleObserverParamsUIBuild
                   COMMAND "${CMAKE_COMMAND}"
                              "-E" "chdir" "${TESTS_OUTPUTDATA_PATH}/tests.obsui.cmake" 
                              "${CMAKE_COMMAND}" "--build" "${TESTS_OUTPUTDATA_PATH}/tests.obsui.cmake"
                              "--target" "install"
                              "--clean-first")
SET_PROPERTY(TEST integration-CMakeModuleObserverParamsUIBuild APPEND PROPERTY DEPENDS integration-CMakeModuleObserverParamsUIConfigure)                                                            



###########################################################################


IF(BUILD_APP_BUILDER AND OPENFLUID_ENABLE_GUI)

FILE(MAKE_DIRECTORY "${TESTS_OUTPUTDATA_PATH}/tests.bext.cmake")

OPENFLUID_ADD_TEST(NAME integration-CMakeModuleBuilderextConfigure
                   COMMAND "${CMAKE_COMMAND}"
                              "-E" "chdir" "${TESTS_OUTPUTDATA_PATH}/tests.bext.cmake" 
                              "${CMAKE_COMMAND}" "-G" "${CMAKE_GENERATOR}"
                              "${CMAKE_CURRENT_SOURCE_DIR}/tests.bext.cmake"
                              "-DOpenFLUIDHelpers_DIR=${CMAKE_MODULES_OUTPUT_PATH}"
                              "-DOpenFLUID_DIR=${CMAKE_MODULES_OUTPUT_PATH}"
                              "-DOpenFLUID_ROOT_DIR=${BUILD_OUTPUT_PATH}"
                              "-DOpenFLUID_EXTRA_SEARCH_PATHS=${CMAKE_BINARY_DIR}/src"                              
                              "-DBEXT_INCLUDE_DIRS=${CMAKE_SOURCE_DIR}/src"
                    PRE_TEST EMPTY_DIRECTORY "${TESTS_OUTPUTDATA_PATH}/tests.bext.cmake")         
                                         
ENDIF()


###########################################################################


IF(BUILD_APP_BUILDER AND OPENFLUID_ENABLE_GUI)

OPENFLUID_ADD_TEST(NAME integration-CMakeModuleBuilderextBuild
                   COMMAND "${CMAKE_COMMAND}"
                              "-E" "chdir" "${TESTS_OUTPUTDATA_PATH}/tests.bext.cmake" 
                              "${CMAKE_COMMAND}" "--build" "${TESTS_OUTPUTDATA_PATH}/tests.bext.cmake")
SET_PROPERTY(TEST integration-CMakeModuleBuilderextBuild APPEND PROPERTY DEPENDS integration-CMakeModuleBuilderextConfigure)                                                            

ENDIF()


###########################################################################


OPENFLUID_ADD_TEST(NAME integration-CMakeModuleRun
                   COMMAND "${BIN_OUTPUT_PATH}/${OPENFLUID_CMD_APP}" 
                           "-i${TESTS_DATASETS_PATH}/OPENFLUID.IN.CMakeModule"
                           "-o${TESTS_OUTPUTDATA_PATH}/OPENFLUID.OUT.CMakeModule" 
#                           "-p${TESTS_OUTPUTDATA_PATH}/tests.sim.cmake"
#                           "-n${TESTS_OUTPUTDATA_PATH}/tests.obs.cmake"
                    PRE_TEST REMOVE_DIRECTORY "${TESTS_OUTPUTDATA_PATH}/OPENFLUID.OUT.CMakeModule")
SET_PROPERTY(TEST integration-CMakeModuleRun APPEND PROPERTY DEPENDS integration-CMakeModuleSimulatorBuild)
SET_PROPERTY(TEST integration-CMakeModuleRun APPEND PROPERTY DEPENDS integration-CMakeModuleObserverBuild)
SET_PROPERTY(TEST integration-CMakeModuleRun APPEND PROPERTY DEPENDS integration-CMakeModuleObserverParamsUIBuild)
                                                             

###########################################################################


IF(NOT APPLE)
  OPENFLUID_ADD_TEST(NAME integration-BuddySim2DocHelp 
                     COMMAND "${BIN_OUTPUT_PATH}/${OPENFLUID_CMD_APP}"
                             "--buddyhelp" "sim2doc"
                    )
ENDIF()


###########################################################################


IF(NOT APPLE)
  OPENFLUID_ADD_TEST(NAME integration-BuddySim2Doc
                     COMMAND "${BIN_OUTPUT_PATH}/${OPENFLUID_CMD_APP}"
                             "--buddy" "sim2doc"
                             "--buddyopts" "inputcpp=${CMAKE_SOURCE_DIR}/src/tests/simulators/tests.sim2doc/Sim2DocSim.cpp,outputdir=${TESTS_OUTPUTDATA_PATH}/sim2doc"
                     PRE_TEST REMOVE_DIRECTORY "${TESTS_OUTPUTDATA_PATH}/sim2doc"                              
                    )
ENDIF()  


###########################################################################


IF(NOT APPLE)
  OPENFLUID_ADD_TEST(NAME integration-BuddySim2DocPDF
                     COMMAND "${BIN_OUTPUT_PATH}/${OPENFLUID_CMD_APP}"
                                "--buddy" "sim2doc"
                                "--buddyopts" "inputcpp=${CMAKE_SOURCE_DIR}/src/tests/simulators/tests.sim2doc/Sim2DocSim.cpp,outputdir=${TESTS_OUTPUTDATA_PATH}/sim2doc-PDF,tplfile=${CMAKE_SOURCE_DIR}/${SHARE_COMMON_INSTALL_PATH}/sim2doc_tpl.tex,pdf=1"
                     PRE_TEST REMOVE_DIRECTORY "${TESTS_OUTPUTDATA_PATH}/sim2doc-PDF"           
                    )
ENDIF()


###########################################################################


IF(NOT APPLE)
  OPENFLUID_ADD_TEST(NAME integration-BuddySim2DocPDFHTML
                     COMMAND "${BIN_OUTPUT_PATH}/${OPENFLUID_CMD_APP}"
                             "--buddy" "sim2doc"
                             "--buddyopts" "inputcpp=${CMAKE_SOURCE_DIR}/src/tests/simulators/tests.sim2doc/Sim2DocSim.cpp,outputdir=${TESTS_OUTPUTDATA_PATH}/sim2doc-PDF-HTML,tplfile=${CMAKE_SOURCE_DIR}/${SHARE_COMMON_INSTALL_PATH}/sim2doc_tpl.tex,pdf=1,html=1"
                     PRE_TEST REMOVE_DIRECTORY "${TESTS_OUTPUTDATA_PATH}/sim2doc-PDF-HTML"           
                    )
ENDIF()


###########################################################################


OPENFLUID_ADD_TEST(NAME integration-BuddyExamplesHelp 
                   COMMAND "${BIN_OUTPUT_PATH}/${OPENFLUID_CMD_APP}"
                           "--buddyhelp" "examples"
                  )


###########################################################################


OPENFLUID_ADD_TEST(NAME integration-BuddyExamplesInstallAll
                   COMMAND "${BIN_OUTPUT_PATH}/${OPENFLUID_CMD_APP}"
                              "--buddy" "examples"
                              "--buddyopts" "selection=*,sourcedir=${EXAMPLES_PROJECTS_PATH},installdir=${TESTS_OUTPUTDATA_PATH}/examplesbuddy/projects,force=1"
                   PRE_TEST REMOVE_DIRECTORY "${TESTS_OUTPUTDATA_PATH}/examplesbuddy"                              
                              )
  

###########################################################################


OPENFLUID_ADD_TEST(NAME integration-BuddyExamplesRunPrimitives 
                   COMMAND "${BIN_OUTPUT_PATH}/${OPENFLUID_CMD_APP}" 
                              "-w" "${TESTS_OUTPUTDATA_PATH}/examplesbuddy/projects/Primitives"                                                   
                    POST_TEST CHECK_FILE_EXIST "${TESTS_OUTPUTDATA_PATH}/examplesbuddy/projects/Primitives/OUT/openfluid-messages.log"
                              CHECK_FILE_EXIST "${TESTS_OUTPUTDATA_PATH}/examplesbuddy/projects/Primitives/OUT/fullA_unitsA1_var1.csv"
                              CHECK_FILE_EXIST "${TESTS_OUTPUTDATA_PATH}/examplesbuddy/projects/Primitives/OUT/fullB_unitsB2_var5.csv"
                   )
SET_PROPERTY(TEST integration-BuddyExamplesRunPrimitives APPEND PROPERTY DEPENDS integration-BuddyExamplesInstallAll)


###########################################################################


OPENFLUID_ADD_TEST(NAME integration-BuddyNewSimHelp
                   COMMAND "${BIN_OUTPUT_PATH}/${OPENFLUID_CMD_APP}"
                           "--buddyhelp" "newsim"
                  )



###########################################################################


FILE(MAKE_DIRECTORY "${TESTS_OUTPUTDATA_PATH}/tests.buddy.newsim")
CONFIGURE_FILE("${TESTS_MISCDATA_PATH}/tests.buddy.newsim/CMakeLists.txt.in" "${TESTS_OUTPUTDATA_PATH}/tests.buddy.newsim/CMakeLists.txt" @ONLY)
    
OPENFLUID_ADD_TEST(NAME integration-BuddyNewSim
                   COMMAND "${BIN_OUTPUT_PATH}/${OPENFLUID_CMD_APP}"
                              "--buddy" "newsim"
                              "--buddyopts" "simid=tests.buddy.newsim,cppclass=NewSimulatorUsingBuddy,outputdir=${TESTS_OUTPUTDATA_PATH}/tests.buddy.newsim,authorname=John Doe,authoremail=doe@foo.bar.org"
                             )


###########################################################################


OPENFLUID_ADD_TEST(NAME integration-BuddyNewSimConfigure
                   COMMAND "${CMAKE_COMMAND}"
                            "-E" "chdir" "${TESTS_OUTPUTDATA_PATH}/tests.buddy.newsim" "${CMAKE_COMMAND}" "-G" "${CMAKE_GENERATOR}"
                            "-DOpenFLUIDHelpers_DIR=${CMAKE_MODULES_OUTPUT_PATH}"
                            "-DOpenFLUID_DIR=${CMAKE_MODULES_OUTPUT_PATH}"
                            "-DOpenFLUID_ROOT_DIR=${BUILD_OUTPUT_PATH}"
                            "-DOpenFLUID_EXTRA_SEARCH_PATHS=${CMAKE_BINARY_DIR}/src"
                            "-DSIM_INCLUDE_DIRS=${CMAKE_SOURCE_DIR}/src"
                            "-DSIM_SIM2DOC_MODE=off" 
                            ".")
SET_PROPERTY(TEST integration-BuddyNewSimConfigure APPEND PROPERTY DEPENDS integration-BuddyNewSim)



###########################################################################


OPENFLUID_ADD_TEST(NAME integration-BuddyNewSimBuild
                   COMMAND "${CMAKE_COMMAND}"
                              "-E" "chdir" "${TESTS_OUTPUTDATA_PATH}/tests.buddy.newsim" "${CMAKE_BUILD_TOOL}"
                              )
SET_PROPERTY(TEST integration-BuddyNewSimBuild APPEND PROPERTY DEPENDS integration-BuddyNewSimConfigure)


###########################################################################


OPENFLUID_ADD_TEST(NAME integration-BuddyNewSimCheck
                   COMMAND "${BIN_OUTPUT_PATH}/${OPENFLUID_CMD_APP}" 
                         "-r"
                         "-p" "${TESTS_OUTPUTDATA_PATH}/tests.buddy.newsim")
SET_TESTS_PROPERTIES(integration-BuddyNewSimCheck PROPERTIES PASS_REGULAR_EXPRESSION "tests.buddy.newsim"
                                                                PASS_REGULAR_EXPRESSION "John Doe"
                                                                PASS_REGULAR_EXPRESSION "doe@foo.bar.org") 
SET_PROPERTY(TEST integration-BuddyNewSimCheck APPEND PROPERTY DEPENDS integration-BuddyNewSimBuild)                                                                 
                       


###########################################################################


OPENFLUID_ADD_TEST(NAME integration-BuddyNewSimRun
                   COMMAND "${BIN_OUTPUT_PATH}/${OPENFLUID_CMD_APP}" 
                                       "-i" "${TESTS_DATASETS_PATH}/OPENFLUID.IN.BuddyNewSim"
                                       "-o" "${TESTS_OUTPUTDATA_PATH}/OPENFLUID.OUT.BuddyNewSim" 
                                       "-p" "${TESTS_OUTPUTDATA_PATH}/tests.buddy.newsim"
                                       "-v")
SET_PROPERTY(TEST integration-BuddyNewSimRun APPEND PROPERTY DEPENDS integration-BuddyNewSimBuild)


###########################################################################


OPENFLUID_ADD_TEST(NAME integration-BuddyNewDataHelp
                   COMMAND "${BIN_OUTPUT_PATH}/${OPENFLUID_CMD_APP}"
                              "--buddyhelp" "newdata"
                              )


###########################################################################


OPENFLUID_ADD_TEST(NAME integration-BuddyNewData
                   COMMAND "${BIN_OUTPUT_PATH}/${OPENFLUID_CMD_APP}"
                              "--buddy" "newdata"
                              "--buddyopts" "outputdir=${TESTS_OUTPUTDATA_PATH}/OPENFLUID.IN.NewData"
                              )


###########################################################################


OPENFLUID_ADD_TEST(NAME integration-BuddyNewDataCheck
                   COMMAND "${BIN_OUTPUT_PATH}/${OPENFLUID_CMD_APP}"
                                       "-i" "${TESTS_OUTPUTDATA_PATH}/OPENFLUID.IN.NewData"
                                       "-o" "${TESTS_OUTPUTDATA_PATH}/OPENFLUID.OUT.NewData"
                                       "-p" "${TEST_OUTPUT_PATH}"
                              )
SET_PROPERTY(TEST integration-BuddyNewDataCheck APPEND PROPERTY DEPENDS integration-BuddyNewData)


###########################################################################


OPENFLUID_ADD_TEST(NAME integration-MinimalRun
                   COMMAND "${BIN_OUTPUT_PATH}/openfluid-minimal"
                                  "${TESTS_DATASETS_PATH}/OPENFLUID.IN.PrimitivesHopla"
                                  "${TESTS_OUTPUTDATA_PATH}/OPENFLUID.OUT.MinimalRun"
                                  "${TEST_OUTPUT_PATH}"
                              )
  

###########################################################################


OPENFLUID_ADD_TEST(NAME integration-MinimalFailsArgs
                   COMMAND "${BIN_OUTPUT_PATH}/openfluid-minimal"
                                  "${TESTS_DATASETS_PATH}/OPENFLUID.IN.VariableTimeProd"
                                  "${TESTS_OUTPUTDATA_PATH}/OPENFLUID.OUT.MinimalFailsArgs"
                              )
SET_TESTS_PROPERTIES(integration-MinimalFailsArgs PROPERTIES WILL_FAIL TRUE)
                              

###########################################################################


OPENFLUID_ADD_TEST(NAME integration-MinimalFailsInput
                   COMMAND "${BIN_OUTPUT_PATH}/openfluid-minimal"
                                  "${TESTS_DATASETS_PATH}/OPENFLUID.IN.doesnotexist"
                                  "${TESTS_OUTPUTDATA_PATH}/OPENFLUID.OUT.MinimalFailsInput"
                                  "${TEST_OUTPUT_PATH}"
                              )                              
SET_TESTS_PROPERTIES(integration-MinimalFailsInput PROPERTIES WILL_FAIL TRUE)
 
 
###########################################################################


OPENFLUID_ADD_TEST(NAME integration-Debug 
                   COMMAND "${BIN_OUTPUT_PATH}/${OPENFLUID_CMD_APP}" 
                           "-f" "-p${TEST_OUTPUT_PATH}")
IF(${CMAKE_BUILD_TYPE} STREQUAL "Debug")
  SET_TESTS_PROPERTIES(integration-Debug PROPERTIES PASS_REGULAR_EXPRESSION "OpenFLUID debugging mode is enabled")
ELSE(${CMAKE_BUILD_TYPE} STREQUAL "Debug")
  SET_TESTS_PROPERTIES(integration-Debug PROPERTIES FAIL_REGULAR_EXPRESSION "OpenFLUID debugging mode is enabled")
ENDIF() 
  
  