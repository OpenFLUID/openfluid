FIND_PACKAGE(Boost REQUIRED COMPONENTS unit_test_framework)  

MACRO(ADD_INTEGRATIONTEST TEST_NAME EXE_NAME)
  ADD_EXECUTABLE(${EXE_NAME} ${ARGN})
  TARGET_LINK_LIBRARIES(${EXE_NAME} ${Boost_UNIT_TEST_FRAMEWORK_LIBRARY} ${Boost_REGEX_LIBRARY} openfluid-base openfluid-core)
  SET_TARGET_PROPERTIES(${EXE_NAME} PROPERTIES RUNTIME_OUTPUT_DIRECTORY "${TEST_OUTPUT_PATH}")
  ADD_TEST(${TEST_NAME} "${TEST_OUTPUT_PATH}/${EXE_NAME}")
ENDMACRO(ADD_INTEGRATIONTEST)

ADD_DEFINITIONS(-DOPENFLUID_VERSION=${FULL_VERSION})

INCLUDE_DIRECTORIES(${Boost_INCLUDE_DIRS}
                    "${PROJECT_SOURCE_DIR}/src" "${PROJECT_BINARY_DIR}/src/tests")


###########################################################################


ADD_TEST(integration-DisplayHelp "${BIN_OUTPUT_PATH}/${OPENFLUID_CMD_APP}" 
                       "-h")
SET_TESTS_PROPERTIES(integration-DisplayHelp PROPERTIES PASS_REGULAR_EXPRESSION "${OPENFLUID_CMD_APP} allowed options")  


###########################################################################


ADD_TEST(integration-DisplayVersion "${BIN_OUTPUT_PATH}/${OPENFLUID_CMD_APP}" 
                       "--version")
  SET_TESTS_PROPERTIES(integration-DisplayVersion PROPERTIES PASS_REGULAR_EXPRESSION ${FULL_VERSION})  
                       

###########################################################################

ADD_TEST(integration-ShowPaths "${BIN_OUTPUT_PATH}/${OPENFLUID_CMD_APP}" 
                       "--show-paths"
                       "-p" "foobar"
                       "-n" "barfoo")  
SET_TESTS_PROPERTIES(integration-ShowPaths PROPERTIES PASS_REGULAR_EXPRESSION "#1 foobar")
SET_TESTS_PROPERTIES(integration-ShowPaths PROPERTIES PASS_REGULAR_EXPRESSION "#1 barfoo")
  
  

###########################################################################


ADD_TEST(integration-SimulatorsList "${BIN_OUTPUT_PATH}/${OPENFLUID_CMD_APP}" 
                         "-f"
                         "-p${TEST_OUTPUT_PATH}")
  

###########################################################################


ADD_TEST(integration-SimulatorsReport "${BIN_OUTPUT_PATH}/${OPENFLUID_CMD_APP}"
                           "-r"
                           "-p${TEST_OUTPUT_PATH}")
  

###########################################################################


ADD_TEST(integration-ObserversList "${BIN_OUTPUT_PATH}/${OPENFLUID_CMD_APP}" 
                         "-e"
                         "-n${TEST_OUTPUT_PATH}")
SET_TESTS_PROPERTIES(integration-ObserversList PROPERTIES PASS_REGULAR_EXPRESSION "tests.empty")  
                         
 

###########################################################################


ADD_TEST(integration-ObserversReport "${BIN_OUTPUT_PATH}/${OPENFLUID_CMD_APP}" 
                         "-l"
                         "-n${TEST_OUTPUT_PATH}")  
SET_TESTS_PROPERTIES(integration-ObserversReport PROPERTIES PASS_REGULAR_EXPRESSION "empty observer for tests")
  


###########################################################################

  
IF(CUSTOM_CMAKE_VERSION VERSION_EQUAL "2.8.0" OR CUSTOM_CMAKE_VERSION VERSION_GREATER "2.8.0") 
  ADD_TEST(integration-SimulatorsReportEnvVar "${BIN_OUTPUT_PATH}/${OPENFLUID_CMD_APP}" "-r")
  SET_TESTS_PROPERTIES(integration-SimulatorsReportEnvVar PROPERTIES ENVIRONMENT "OPENFLUID_SIMS_PATH=${TEST_OUTPUT_PATH}")                           
  SET_TESTS_PROPERTIES(integration-SimulatorsReportEnvVar PROPERTIES PASS_REGULAR_EXPRESSION "tests.primitives.variables.use")  
  SET_TESTS_PROPERTIES(integration-SimulatorsReportEnvVar PROPERTIES PASS_REGULAR_EXPRESSION "tests.primitives.variables.prod")    
ENDIF(CUSTOM_CMAKE_VERSION VERSION_EQUAL "2.8.0" OR CUSTOM_CMAKE_VERSION VERSION_GREATER "2.8.0")



###########################################################################


ADD_TEST(integration-SimulatorsReportNoEnvVar "${BIN_OUTPUT_PATH}/${OPENFLUID_CMD_APP}" "-r")                           
SET_TESTS_PROPERTIES(integration-SimulatorsReportNoEnvVar PROPERTIES PASS_REGULAR_EXPRESSION "tests.primitives.variables.use")  
SET_TESTS_PROPERTIES(integration-SimulatorsReportNoEnvVar PROPERTIES PASS_REGULAR_EXPRESSION "tests.primitives.variables.prod")
SET_TESTS_PROPERTIES(integration-SimulatorsReportNoEnvVar PROPERTIES WILL_FAIL TRUE)        


###########################################################################


ADD_TEST(integration-SimulatorsMatchReport "${BIN_OUTPUT_PATH}/${OPENFLUID_CMD_APP}"
                                "-u" "tests.primitives.*"
                                "-p${TEST_OUTPUT_PATH}")
SET_TESTS_PROPERTIES(integration-SimulatorsMatchReport PROPERTIES PASS_REGULAR_EXPRESSION "tests.primitives.variables.use")  
SET_TESTS_PROPERTIES(integration-SimulatorsMatchReport PROPERTIES PASS_REGULAR_EXPRESSION "tests.primitives.variables.prod")
                           

###########################################################################

ADD_TEST(integration-SimulatorsXMLReport "${BIN_OUTPUT_PATH}/${OPENFLUID_CMD_APP}"
                              "-x"
                              "-p${TEST_OUTPUT_PATH}")
SET_TESTS_PROPERTIES(integration-SimulatorsXMLReport PROPERTIES WILL_FAIL TRUE)        
                              
  

###########################################################################


ADD_TEST(integration-InputDoesNotExist "${BIN_OUTPUT_PATH}/${OPENFLUID_CMD_APP}" "-i" "${CMAKE_BINARY_DIR}/path/does/not/exist")                           
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
                             "-DOpenFLUID_DIR=${CMAKE_MODULES_OUTPUT_PATH}"
                             "-DOpenFLUID_ROOT_DIR=${BUILD_OUTPUT_PATH}"
                             "-DOpenFLUID_EXTRA_SEARCH_PATHS=${CMAKE_BINARY_DIR}/src"
                             "-DSIM_INCLUDE_DIRS=${CMAKE_SOURCE_DIR}/src"
                   PRE_TEST EMPTY_DIRECTORY "${TESTS_OUTPUTDATA_PATH}/tests.sim.cmake")                                                                                          


###########################################################################


OPENFLUID_ADD_TEST(NAME integration-CMakeModuleSimulatorBuild
                   COMMAND "${CMAKE_COMMAND}"
                           "-E" "chdir" "${TESTS_OUTPUTDATA_PATH}/tests.sim.cmake" 
                           "${CMAKE_COMMAND}" "--build" "${TESTS_OUTPUTDATA_PATH}/tests.sim.cmake")
SET_PROPERTY(TEST integration-CMakeModuleSimulatorBuild APPEND PROPERTY DEPENDS integration-CMakeModuleSimulatorConfigure)                                                            



###########################################################################


FILE(MAKE_DIRECTORY "${TESTS_OUTPUTDATA_PATH}/tests.obs.cmake")

OPENFLUID_ADD_TEST(NAME integration-CMakeModuleObserverConfigure
                   COMMAND "${CMAKE_COMMAND}"
                              "-E" "chdir" "${TESTS_OUTPUTDATA_PATH}/tests.obs.cmake" 
                              "${CMAKE_COMMAND}" "-G" "${CMAKE_GENERATOR}"
                              "${CMAKE_CURRENT_SOURCE_DIR}/tests.obs.cmake"
                              "-DOpenFLUID_DIR=${CMAKE_MODULES_OUTPUT_PATH}"
                              "-DOpenFLUID_ROOT_DIR=${BUILD_OUTPUT_PATH}"
                              "-DOpenFLUID_EXTRA_SEARCH_PATHS=${CMAKE_BINARY_DIR}/src"                              
                              "-DOBS_INCLUDE_DIRS=${CMAKE_SOURCE_DIR}/src"
                    PRE_TEST EMPTY_DIRECTORY "${TESTS_OUTPUTDATA_PATH}/tests.obs.cmake")                              
                              

###########################################################################


OPENFLUID_ADD_TEST(NAME integration-CMakeModuleObserverBuild
                   COMMAND "${CMAKE_COMMAND}"
                              "-E" "chdir" "${TESTS_OUTPUTDATA_PATH}/tests.obs.cmake" 
                              "${CMAKE_COMMAND}" "--build" "${TESTS_OUTPUTDATA_PATH}/tests.obs.cmake")
SET_PROPERTY(TEST integration-CMakeModuleObserverBuild APPEND PROPERTY DEPENDS integration-CMakeModuleObserverConfigure)                                                            



###########################################################################


OPENFLUID_ADD_TEST(NAME integration-CMakeModuleRun
                   COMMAND "${BIN_OUTPUT_PATH}/${OPENFLUID_CMD_APP}" 
                           "-i${TESTS_DATASETS_PATH}/OPENFLUID.IN.CMakeModule"
                           "-o${TESTS_OUTPUTDATA_PATH}/OPENFLUID.OUT.CMakeModule" 
                           "-p${TESTS_OUTPUTDATA_PATH}/tests.sim.cmake"
                           "-n${TESTS_OUTPUTDATA_PATH}/tests.obs.cmake"
                    PRE_TEST REMOVE_DIRECTORY "${TESTS_OUTPUTDATA_PATH}/OPENFLUID.OUT.CMakeModule"
                    POST_TEST CHECK_FILE_EXIST "${TESTS_OUTPUTDATA_PATH}/OPENFLUID.OUT.CMakeModule/tests.cmake_ofware-sim.log"
                              CHECK_FILE_EXIST "${TESTS_OUTPUTDATA_PATH}/OPENFLUID.OUT.CMakeModule/tests.cmake_ofware-obs.log")
SET_PROPERTY(TEST integration-CMakeModuleRun APPEND PROPERTY DEPENDS integration-CMakeModuleSimulatorBuild)
SET_PROPERTY(TEST integration-CMakeModuleRun APPEND PROPERTY DEPENDS integration-CMakeModuleObserverBuild)
SET_TESTS_PROPERTIES(integration-CMakeModuleRun PROPERTIES PASS_REGULAR_EXPRESSION "CMake module built simulator"
                                                           PASS_REGULAR_EXPRESSION "CMake module built observer")
                                                             

###########################################################################


ADD_TEST(integration-BuddySim2DocHelp "${BIN_OUTPUT_PATH}/${OPENFLUID_CMD_APP}"
                              "--buddyhelp" "sim2doc"
                              )


###########################################################################


OPENFLUID_ADD_TEST(NAME integration-BuddySim2Doc
                   COMMAND "${BIN_OUTPUT_PATH}/${OPENFLUID_CMD_APP}"
                              "--buddy" "sim2doc"
                              "--buddyopts" "inputcpp=${CMAKE_SOURCE_DIR}/src/tests/simulators/tests.sim2doc/Sim2DocSim.cpp,outputdir=${TESTS_OUTPUTDATA_PATH}/sim2doc"
                   PRE_TEST REMOVE_DIRECTORY "${TESTS_OUTPUTDATA_PATH}/sim2doc"                              
                  )
  

###########################################################################


OPENFLUID_ADD_TEST(NAME integration-BuddySim2DocPDF
                   COMMAND "${BIN_OUTPUT_PATH}/${OPENFLUID_CMD_APP}"
                              "--buddy" "sim2doc"
                              "--buddyopts" "inputcpp=${CMAKE_SOURCE_DIR}/src/tests/simulators/tests.sim2doc/Sim2DocSim.cpp,outputdir=${TESTS_OUTPUTDATA_PATH}/sim2doc-PDF,tplfile=${CMAKE_SOURCE_DIR}/${SHARE_COMMON_INSTALL_PATH}/sim2doc_tpl.tex,pdf=1"
                   PRE_TEST REMOVE_DIRECTORY "${TESTS_OUTPUTDATA_PATH}/sim2doc-PDF"           
                  )


###########################################################################


OPENFLUID_ADD_TEST(NAME integration-BuddySim2DocPDFHTML
                   COMMAND "${BIN_OUTPUT_PATH}/${OPENFLUID_CMD_APP}"
                              "--buddy" "sim2doc"
                              "--buddyopts" "inputcpp=${CMAKE_SOURCE_DIR}/src/tests/simulators/tests.sim2doc/Sim2DocSim.cpp,outputdir=${TESTS_OUTPUTDATA_PATH}/sim2doc-PDF-HTML,tplfile=${CMAKE_SOURCE_DIR}/${SHARE_COMMON_INSTALL_PATH}/sim2doc_tpl.tex,pdf=1,html=1"
                   PRE_TEST REMOVE_DIRECTORY "${TESTS_OUTPUTDATA_PATH}/sim2doc-PDF-HTML"           
                  )


###########################################################################


ADD_TEST(integration-BuddyExamplesHelp "${BIN_OUTPUT_PATH}/${OPENFLUID_CMD_APP}"
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
                              CHECK_FILE_EXIST "${TESTS_OUTPUTDATA_PATH}/examplesbuddy/projects/Primitives/OUT/examples.primitives.unitsA.prod_ofware-sim.log"
                              CHECK_FILE_EXIST "${TESTS_OUTPUTDATA_PATH}/examplesbuddy/projects/Primitives/OUT/examples.primitives.unitsB.prod_ofware-sim.log"
                              CHECK_FILE_EXIST "${TESTS_OUTPUTDATA_PATH}/examplesbuddy/projects/Primitives/OUT/fullA_unitsA1_var1.csv"
                              CHECK_FILE_EXIST "${TESTS_OUTPUTDATA_PATH}/examplesbuddy/projects/Primitives/OUT/fullB_unitsB2_var5.csv"
                   )
SET_PROPERTY(TEST integration-BuddyExamplesRunPrimitives APPEND PROPERTY DEPENDS integration-BuddyExamplesInstallAll)


###########################################################################


ADD_TEST(integration-BuddyNewSimHelp "${BIN_OUTPUT_PATH}/${OPENFLUID_CMD_APP}"
                              "--buddyhelp" "newsim"
                              )



###########################################################################


FILE(MAKE_DIRECTORY "${TESTS_OUTPUTDATA_PATH}/tests.buddy.newsim")
CONFIGURE_FILE("${TESTS_DATASETS_PATH}/tests.buddy.newsim/CMakeLists.txt.in" "${TESTS_OUTPUTDATA_PATH}/tests.buddy.newsim/CMakeLists.txt" @ONLY)
    
OPENFLUID_ADD_TEST(NAME integration-BuddyNewSim
                   COMMAND "${BIN_OUTPUT_PATH}/${OPENFLUID_CMD_APP}"
                              "--buddy" "newsim"
                              "--buddyopts" "simid=tests.buddy.newsim,cppclass=NewSimulatorUsingBuddy,outputdir=${TESTS_OUTPUTDATA_PATH}/tests.buddy.newsim,authorname=John Doe,authoremail=doe@foo.bar.org"
                             )


###########################################################################


OPENFLUID_ADD_TEST(NAME integration-BuddyNewSimConfigure
                   COMMAND "${CMAKE_COMMAND}"
                              "-E" "chdir" "${TESTS_OUTPUTDATA_PATH}/tests.buddy.newsim" "${CMAKE_COMMAND}" "-G" "${CMAKE_GENERATOR}" "." 
                              )
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


ADD_TEST(integration-Debug "${BIN_OUTPUT_PATH}/${OPENFLUID_CMD_APP}" 
                             "-f" "-p${TEST_OUTPUT_PATH}")
IF(${CMAKE_BUILD_TYPE} STREQUAL "Debug")
  SET_TESTS_PROPERTIES(integration-Debug PROPERTIES PASS_REGULAR_EXPRESSION "OpenFLUID debugging mode is enabled")
ELSE(${CMAKE_BUILD_TYPE} STREQUAL "Debug")
  SET_TESTS_PROPERTIES(integration-Debug PROPERTIES FAIL_REGULAR_EXPRESSION "OpenFLUID debugging mode is enabled")
ENDIF() 
  
  