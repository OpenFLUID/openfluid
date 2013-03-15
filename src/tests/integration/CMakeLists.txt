FIND_PACKAGE(Boost REQUIRED COMPONENTS unit_test_framework)  

MACRO(ADD_INTEGRATIONTEST TEST_NAME EXE_NAME)
  ADD_EXECUTABLE(${EXE_NAME} ${ARGN})
  TARGET_LINK_LIBRARIES(${EXE_NAME} ${Boost_UNIT_TEST_FRAMEWORK_LIBRARY} ${Boost_REGEX_LIBRARY} ${LibXML2_LIBRARIES} openfluid-base openfluid-core)
  SET_TARGET_PROPERTIES(${EXE_NAME} PROPERTIES RUNTIME_OUTPUT_DIRECTORY "${TEST_OUTPUT_PATH}")
  ADD_TEST(${TEST_NAME} "${TEST_OUTPUT_PATH}/${EXE_NAME}")
ENDMACRO(ADD_INTEGRATIONTEST)

ADD_DEFINITIONS(-DOPENFLUID_VERSION=${FULL_VERSION})

INCLUDE_DIRECTORIES(${Boost_INCLUDE_DIRS} ${LibXML2_INCLUDE_DIRS}
                    "${PROJECT_SOURCE_DIR}/src" "${PROJECT_BINARY_DIR}/src/tests")


LINK_DIRECTORIES(${LibXML2_LIBRARY_DIRS})


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


ADD_TEST(integration-FunctionsList "${BIN_OUTPUT_PATH}/${OPENFLUID_CMD_APP}" 
                         "-f"
                         "-p${TEST_OUTPUT_PATH}")
  

###########################################################################


ADD_TEST(integration-FunctionsReport "${BIN_OUTPUT_PATH}/${OPENFLUID_CMD_APP}"
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
  ADD_TEST(integration-FunctionsReportEnvVar "${BIN_OUTPUT_PATH}/${OPENFLUID_CMD_APP}" "-r")
  SET_TESTS_PROPERTIES(integration-FunctionsReportEnvVar PROPERTIES ENVIRONMENT "OPENFLUID_FUNCS_PATH=${TEST_OUTPUT_PATH}")                           
  SET_TESTS_PROPERTIES(integration-FunctionsReportEnvVar PROPERTIES PASS_REGULAR_EXPRESSION "tests.primitives.use")  
  SET_TESTS_PROPERTIES(integration-FunctionsReportEnvVar PROPERTIES PASS_REGULAR_EXPRESSION "tests.primitives.prod")    
ENDIF(CUSTOM_CMAKE_VERSION VERSION_EQUAL "2.8.0" OR CUSTOM_CMAKE_VERSION VERSION_GREATER "2.8.0")



###########################################################################


ADD_TEST(integration-FunctionsReportNoEnvVar "${BIN_OUTPUT_PATH}/${OPENFLUID_CMD_APP}" "-r")                           
SET_TESTS_PROPERTIES(integration-FunctionsReportNoEnvVar PROPERTIES PASS_REGULAR_EXPRESSION "tests.primitives.use")  
SET_TESTS_PROPERTIES(integration-FunctionsReportNoEnvVar PROPERTIES PASS_REGULAR_EXPRESSION "tests.primitives.prod")
SET_TESTS_PROPERTIES(integration-FunctionsReportNoEnvVar PROPERTIES WILL_FAIL TRUE)        


###########################################################################


ADD_TEST(integration-FunctionsMatchReport "${BIN_OUTPUT_PATH}/${OPENFLUID_CMD_APP}"
                                "-u" "tests.primitives.*"
                                "-p${TEST_OUTPUT_PATH}")
SET_TESTS_PROPERTIES(integration-FunctionsMatchReport PROPERTIES PASS_REGULAR_EXPRESSION "tests.primitives.use")  
SET_TESTS_PROPERTIES(integration-FunctionsMatchReport PROPERTIES PASS_REGULAR_EXPRESSION "tests.primitives.prod")
                           

###########################################################################

ADD_TEST(integration-FunctionsXMLReport "${BIN_OUTPUT_PATH}/${OPENFLUID_CMD_APP}"
                              "-x"
                              "-p${TEST_OUTPUT_PATH}")
SET_TESTS_PROPERTIES(integration-FunctionsXMLReport PROPERTIES WILL_FAIL TRUE)        
                              
  

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


OPENFLUID_ADD_TEST(NAME integration-MissingFunction
                   COMMAND "${BIN_OUTPUT_PATH}/${OPENFLUID_CMD_APP}"
                                        "-i" "${TESTS_DATASETS_PATH}/OPENFLUID.IN.MissingFunc"
                                        "-o" "${TESTS_OUTPUTDATA_PATH}/OPENFLUID.OUT.MissingFunc"
                                        "-p" "${TEST_OUTPUT_PATH}")               
SET_TESTS_PROPERTIES(integration-MissingFunction PROPERTIES WILL_FAIL TRUE)



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


OPENFLUID_ADD_TEST(NAME integration-MissingInputdata
                   COMMAND "${BIN_OUTPUT_PATH}/${OPENFLUID_CMD_APP}"
                                        "-i" "${TESTS_DATASETS_PATH}/OPENFLUID.IN.MissingData"
                                        "-o" "${TESTS_OUTPUTDATA_PATH}/OPENFLUID.OUT.MissingData"
                                        "-p" "${TEST_OUTPUT_PATH}")               
SET_TESTS_PROPERTIES(integration-MissingInputdata PROPERTIES WILL_FAIL TRUE)



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
SET_PROPERTY(TEST integration-FluidXWriterSingle APPEND PROPERTY DEPENDS api-FluidXDescriptor_TEST)


###########################################################################


OPENFLUID_ADD_TEST(NAME integration-FluidXWriterMany
                   COMMAND "${BIN_OUTPUT_PATH}/${OPENFLUID_CMD_APP}"
                               "-i" "${TESTS_OUTPUTDATA_PATH}/OPENFLUID.OUT.FluidXWriterMany"
                               "-o" "${TESTS_OUTPUTDATA_PATH}/OPENFLUID.OUT.FluidXWriterRunMany"
                               "-p" "${TEST_OUTPUT_PATH}")
SET_PROPERTY(TEST integration-FluidXWriterMany APPEND PROPERTY DEPENDS api-FluidXDescriptor_TEST)
  

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


FILE(MAKE_DIRECTORY "${TESTS_OUTPUTDATA_PATH}/tests.func.cmake")
  
OPENFLUID_ADD_TEST(NAME integration-CMakeModuleFunctionConfigure
                   COMMAND "${CMAKE_COMMAND}"
                             "-E" "chdir" "${TESTS_OUTPUTDATA_PATH}/tests.func.cmake" 
                             "${CMAKE_COMMAND}" "-G" "${CMAKE_GENERATOR}"
                             "${CMAKE_CURRENT_SOURCE_DIR}/tests.func.cmake"
                             "-DOpenFLUID_DIR=${CMAKE_MODULES_OUTPUT_PATH}"
                             "-DOpenFLUID_ROOT_DIR=${BUILD_OUTPUT_PATH}"
                             "-DOpenFLUID_EXTRA_SEARCH_PATHS=${CMAKE_BINARY_DIR}/src"
                             "-DFUNC_INCLUDE_DIRS=${CMAKE_SOURCE_DIR}/src"
                   PRE_TEST EMPTY_DIRECTORY "${TESTS_OUTPUTDATA_PATH}/tests.func.cmake")                                                                                          


###########################################################################


OPENFLUID_ADD_TEST(NAME integration-CMakeModuleFunctionBuild
                   COMMAND "${CMAKE_COMMAND}"
                           "-E" "chdir" "${TESTS_OUTPUTDATA_PATH}/tests.func.cmake" 
                           "${CMAKE_COMMAND}" "--build" "${TESTS_OUTPUTDATA_PATH}/tests.func.cmake")
SET_PROPERTY(TEST integration-CMakeModuleFunctionBuild APPEND PROPERTY DEPENDS integration-CMakeModuleFunctionConfigure)                                                            



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
                           "-p${TESTS_OUTPUTDATA_PATH}/tests.func.cmake"
                           "-n${TESTS_OUTPUTDATA_PATH}/tests.obs.cmake"
                    PRE_TEST REMOVE_DIRECTORY "${TESTS_OUTPUTDATA_PATH}/OPENFLUID.OUT.CMakeModule"
                    POST_TEST CHECK_FILE_EXIST "${TESTS_OUTPUTDATA_PATH}/OPENFLUID.OUT.CMakeModule/tests.cmake_ofware-func.log"
                              CHECK_FILE_EXIST "${TESTS_OUTPUTDATA_PATH}/OPENFLUID.OUT.CMakeModule/tests.cmake_ofware-obs.log")
SET_PROPERTY(TEST integration-CMakeModuleRun APPEND PROPERTY DEPENDS integration-CMakeModuleFunctionBuild)
SET_PROPERTY(TEST integration-CMakeModuleRun APPEND PROPERTY DEPENDS integration-CMakeModuleObserverBuild)
SET_TESTS_PROPERTIES(integration-CMakeModuleRun PROPERTIES PASS_REGULAR_EXPRESSION "CMake module built function"
                                                           PASS_REGULAR_EXPRESSION "CMake module built observer")
                                                             

###########################################################################


ADD_TEST(integration-BuddyFunc2DocHelp "${BIN_OUTPUT_PATH}/${OPENFLUID_CMD_APP}"
                              "--buddyhelp" "func2doc"
                              )


###########################################################################


OPENFLUID_ADD_TEST(NAME integration-BuddyFunc2Doc
                   COMMAND "${BIN_OUTPUT_PATH}/${OPENFLUID_CMD_APP}"
                              "--buddy" "func2doc"
                              "--buddyopts" "inputcpp=${CMAKE_SOURCE_DIR}/src/tests/functions/tests.ofefunc2doc/OFEFunc2DocFunc.cpp,outputdir=${TESTS_OUTPUTDATA_PATH}/func2doc"
                              )
  

###########################################################################


OPENFLUID_ADD_TEST(NAME integration-BuddyFunc2DocPDF
                   COMMAND "${BIN_OUTPUT_PATH}/${OPENFLUID_CMD_APP}"
                              "--buddy" "func2doc"
                              "--buddyopts" "inputcpp=${CMAKE_SOURCE_DIR}/src/tests/functions/tests.ofefunc2doc/OFEFunc2DocFunc.cpp,outputdir=${TESTS_OUTPUTDATA_PATH}/func2doc-PDF,tplfile=${CMAKE_SOURCE_DIR}/${SHARE_COMMON_INSTALL_PATH}/func2doc_tpl.tex,pdf=1"
                              )


###########################################################################


OPENFLUID_ADD_TEST(NAME integration-BuddyFunc2DocPDFHTML
                   COMMAND "${BIN_OUTPUT_PATH}/${OPENFLUID_CMD_APP}"
                              "--buddy" "func2doc"
                              "--buddyopts" "inputcpp=${CMAKE_SOURCE_DIR}/src/tests/functions/tests.ofefunc2doc/OFEFunc2DocFunc.cpp,outputdir=${TESTS_OUTPUTDATA_PATH}/func2doc-PDF-HTML,pdf=1,html=1"
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
                              CHECK_FILE_EXIST "${TESTS_OUTPUTDATA_PATH}/examplesbuddy/projects/Primitives/OUT/examples.primitives.unitsA.prod_ofware-func.log"
                              CHECK_FILE_EXIST "${TESTS_OUTPUTDATA_PATH}/examplesbuddy/projects/Primitives/OUT/examples.primitives.unitsB.prod_ofware-func.log"
                              CHECK_FILE_EXIST "${TESTS_OUTPUTDATA_PATH}/examplesbuddy/projects/Primitives/OUT/fullA_unitsA1_var1.csv"
                              CHECK_FILE_EXIST "${TESTS_OUTPUTDATA_PATH}/examplesbuddy/projects/Primitives/OUT/fullB_unitsB2_var5.csv"
                   )
SET_PROPERTY(TEST integration-BuddyExamplesRunPrimitives APPEND PROPERTY DEPENDS integration-BuddyExamplesInstallAll)


###########################################################################


ADD_TEST(integration-BuddyNewFuncHelp "${BIN_OUTPUT_PATH}/${OPENFLUID_CMD_APP}"
                              "--buddyhelp" "newfunc"
                              )



###########################################################################


FILE(MAKE_DIRECTORY "${TESTS_OUTPUTDATA_PATH}/tests.buddy.newfunc")
CONFIGURE_FILE("${TESTS_DATASETS_PATH}/tests.buddy.newfunc/CMakeLists.txt.in" "${TESTS_OUTPUTDATA_PATH}/tests.buddy.newfunc/CMakeLists.txt" @ONLY)
    
OPENFLUID_ADD_TEST(NAME integration-BuddyNewFunc
                   COMMAND "${BIN_OUTPUT_PATH}/${OPENFLUID_CMD_APP}"
                              "--buddy" "newfunc"
                              "--buddyopts" "funcid=tests.buddy.newfunc,cppclass=NewFunctionUsingBuddy,outputdir=${TESTS_OUTPUTDATA_PATH}/tests.buddy.newfunc,authorname=John Doe,authoremail=doe@foo.bar.org"
                             )


###########################################################################


OPENFLUID_ADD_TEST(NAME integration-BuddyNewFuncConfigure
                   COMMAND "${CMAKE_COMMAND}"
                              "-E" "chdir" "${TESTS_OUTPUTDATA_PATH}/tests.buddy.newfunc" "${CMAKE_COMMAND}" "-G" "${CMAKE_GENERATOR}" "." 
                              )
SET_PROPERTY(TEST integration-BuddyNewFuncConfigure APPEND PROPERTY DEPENDS integration-BuddyNewFunc)



###########################################################################


OPENFLUID_ADD_TEST(NAME integration-BuddyNewFuncBuild
                   COMMAND "${CMAKE_COMMAND}"
                              "-E" "chdir" "${TESTS_OUTPUTDATA_PATH}/tests.buddy.newfunc" "${CMAKE_BUILD_TOOL}"
                              )
SET_PROPERTY(TEST integration-BuddyNewFuncBuild APPEND PROPERTY DEPENDS integration-BuddyNewFuncConfigure)


###########################################################################


OPENFLUID_ADD_TEST(NAME integration-BuddyNewFuncCheck
                   COMMAND "${BIN_OUTPUT_PATH}/${OPENFLUID_CMD_APP}" 
                         "-r"
                         "-p" "${TESTS_OUTPUTDATA_PATH}/tests.buddy.newfunc")
SET_TESTS_PROPERTIES(integration-BuddyNewFuncCheck PROPERTIES PASS_REGULAR_EXPRESSION "tests.buddy.newfunc"
                                                                PASS_REGULAR_EXPRESSION "John Doe"
                                                                PASS_REGULAR_EXPRESSION "doe@foo.bar.org") 
SET_PROPERTY(TEST integration-BuddyNewFuncCheck APPEND PROPERTY DEPENDS integration-BuddyNewFuncBuild)                                                                 
                       


###########################################################################


OPENFLUID_ADD_TEST(NAME integration-BuddyNewFuncRun
                   COMMAND "${BIN_OUTPUT_PATH}/${OPENFLUID_CMD_APP}" 
                                       "-i" "${TESTS_DATASETS_PATH}/OPENFLUID.IN.BuddyNewFunc"
                                       "-o" "${TESTS_OUTPUTDATA_PATH}/OPENFLUID.OUT.BuddyNewFunc" 
                                       "-p" "${TESTS_OUTPUTDATA_PATH}/tests.buddy.newfunc"
                                       "-v")
SET_PROPERTY(TEST integration-BuddyNewFuncRun APPEND PROPERTY DEPENDS integration-BuddyNewFuncBuild)


###########################################################################


ADD_TEST(integration-BuddyConvertHelp "${BIN_OUTPUT_PATH}/${OPENFLUID_CMD_APP}"
                              "--buddyhelp" "convert"
                              )


###########################################################################


IF(${FULL_VERSION} VERSION_GREATER "1.3") 
  OPENFLUID_ADD_TEST(NAME integration-BuddyConvert1314
                     COMMAND "${BIN_OUTPUT_PATH}/${OPENFLUID_CMD_APP}"
                              "--buddy" "convert"
                              "--buddyopts" "convmode=13_14,inputdir=${TESTS_DATASETS_PATH}/OPENFLUID.IN.V13,outputdir=${TESTS_OUTPUTDATA_PATH}/OPENFLUID.IN.Converted13to14"
                              )                              
ENDIF() 


###########################################################################

IF(${FULL_VERSION} EQUAL "1.4")  
  OPENFLUID_ADD_TEST(NAME integration-BuddyConvert1314Check
                     COMMAND "${BIN_OUTPUT_PATH}/${OPENFLUID_CMD_APP}"
                                       "-i" "${TESTS_OUTPUTDATA_PATH}/OPENFLUID.IN.Converted13to14"
                                       "-o" "${TESTS_OUTPUTDATA_PATH}/OPENFLUID.OUT.Converted13to14"
                                       "-p" "${TEST_OUTPUT_PATH}"
                              )
  SET_PROPERTY(TEST integration-BuddyConvert1314Check APPEND PROPERTY DEPENDS integration-BuddyConvert1314)
ENDIF()  


###########################################################################


IF(${FULL_VERSION} VERSION_GREATER "1.4") 
  OPENFLUID_ADD_TEST(NAME integration-BuddyConvert1415
                     COMMAND "${BIN_OUTPUT_PATH}/${OPENFLUID_CMD_APP}"
                              "--buddy" "convert"
                              "--buddyopts" "convmode=14_15,inputdir=${TESTS_OUTPUTDATA_PATH}/OPENFLUID.IN.Converted13to14,outputdir=${TESTS_OUTPUTDATA_PATH}/OPENFLUID.IN.Converted14to15"
                              )
  SET_PROPERTY(TEST integration-BuddyConvert1415 APPEND PROPERTY DEPENDS integration-BuddyConvert1314)                              
ENDIF() 


###########################################################################


IF(${FULL_VERSION} EQUAL "1.5")  
  OPENFLUID_ADD_TEST(NAME integration-BuddyConvert1415Check
                     COMMAND "${BIN_OUTPUT_PATH}/${OPENFLUID_CMD_APP}"
                                       "-i" "${TESTS_OUTPUTDATA_PATH}/OPENFLUID.IN.Converted14to15"
                                       "-o" "${TESTS_OUTPUTDATA_PATH}/OPENFLUID.OUT.Converted14to15"
                                       "-p" "${TEST_OUTPUT_PATH}"
                              )
  SET_PROPERTY(TEST integration-BuddyConvert1415Check APPEND PROPERTY DEPENDS integration-BuddyConvert1415)                              
ENDIF()  


###########################################################################

IF(${FULL_VERSION} VERSION_GREATER "1.5") 
  OPENFLUID_ADD_TEST(NAME integration-BuddyConvert1516
                     COMMAND "${BIN_OUTPUT_PATH}/${OPENFLUID_CMD_APP}"
                              "--buddy" "convert"
                              "--buddyopts" "convmode=15_16,inputdir=${TESTS_OUTPUTDATA_PATH}/OPENFLUID.IN.Converted14to15,outputdir=${TESTS_OUTPUTDATA_PATH}/OPENFLUID.IN.Converted15to16"
                              )
  SET_PROPERTY(TEST integration-BuddyConvert1516 APPEND PROPERTY DEPENDS integration-BuddyConvert1415)                              
ENDIF()
 

###########################################################################


IF(${FULL_VERSION} EQUAL "1.6")  
  OPENFLUID_ADD_TEST(NAME integration-BuddyConvert1516Check
                     COMMAND "${BIN_OUTPUT_PATH}/${OPENFLUID_CMD_APP}"
                                     "-i" "${TESTS_OUTPUTDATA_PATH}/OPENFLUID.IN.Converted15to16"
                                       "-o" "${TESTS_OUTPUTDATA_PATH}/OPENFLUID.OUT.Converted15to16"
                                       "-p" "${TEST_OUTPUT_PATH}"
                              )
  SET_PROPERTY(TEST integration-BuddyConvert1516Check APPEND PROPERTY DEPENDS integration-BuddyConvert1516)                              
ENDIF()  



###########################################################################


IF(${FULL_VERSION} VERSION_GREATER "1.5")
  OPENFLUID_ADD_TEST(NAME integration-BuddyConvert1516wBuff
                     COMMAND "${BIN_OUTPUT_PATH}/${OPENFLUID_CMD_APP}"
                              "--buddy" "convert"
                              "--buddyopts" "convmode=15_16,inputdir=${TESTS_DATASETS_PATH}/OPENFLUID.IN.V15,outputdir=${TESTS_OUTPUTDATA_PATH}/OPENFLUID.IN.Converted15to16wBuff"
                              )                                  
ENDIF()
  

###########################################################################
  
  
IF(${FULL_VERSION} EQUAL "1.6")  
  OPENFLUID_ADD_TEST(NAME integration-BuddyConvert1516wBuffCheck
                     COMMAND "${BIN_OUTPUT_PATH}/${OPENFLUID_CMD_APP}"
                                       "-i" "${TESTS_OUTPUTDATA_PATH}/OPENFLUID.IN.Converted15to16wBuff"
                                       "-o" "${TESTS_OUTPUTDATA_PATH}/OPENFLUID.OUT.Converted15to16wBuff"
                                       "-p" "${TEST_OUTPUT_PATH}"
                              )
  SET_PROPERTY(TEST integration-BuddyConvert1516wBuffCheck APPEND PROPERTY DEPENDS integration-BuddyConvert1516wBuff)                              
ENDIF()


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
  
  