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


  ADD_TEST(integration-DisplayHelp "${BIN_OUTPUT_PATH}/openfluid-engine" 
                       "-h")
  SET_TESTS_PROPERTIES(integration-DisplayHelp PROPERTIES PASS_REGULAR_EXPRESSION "openfluid-engine allowed options")  

  ADD_TEST(integration-DisplayVersion "${BIN_OUTPUT_PATH}/openfluid-engine" 
                       "--version")
  SET_TESTS_PROPERTIES(integration-DisplayVersion PROPERTIES PASS_REGULAR_EXPRESSION ${FULL_VERSION})  
                       
  ADD_TEST(integration-ShowPaths "${BIN_OUTPUT_PATH}/openfluid-engine" 
                       "--show-paths"
                       "-p" "foobar"
                       "-n" "barfoo")  
  SET_TESTS_PROPERTIES(integration-ShowPaths PROPERTIES PASS_REGULAR_EXPRESSION "#1 foobar")
  SET_TESTS_PROPERTIES(integration-ShowPaths PROPERTIES PASS_REGULAR_EXPRESSION "#1 barfoo")
  
  
  ADD_TEST(integration-FunctionsList "${BIN_OUTPUT_PATH}/openfluid-engine" 
                         "-f"
                         "-p${TEST_OUTPUT_PATH}")
  
  ADD_TEST(integration-FunctionsReport "${BIN_OUTPUT_PATH}/openfluid-engine"
                           "-r"
                           "-p${TEST_OUTPUT_PATH}")
  
  ADD_TEST(integration-ObserversList "${BIN_OUTPUT_PATH}/openfluid-engine" 
                         "-e"
                         "-n${TEST_OUTPUT_PATH}")
  SET_TESTS_PROPERTIES(integration-ObserversList PROPERTIES PASS_REGULAR_EXPRESSION "tests.empty")  
                         
 
  ADD_TEST(integration-ObserversReport "${BIN_OUTPUT_PATH}/openfluid-engine" 
                         "-l"
                         "-n${TEST_OUTPUT_PATH}")  
  SET_TESTS_PROPERTIES(integration-ObserversReport PROPERTIES PASS_REGULAR_EXPRESSION "empty observer for tests")
  
  
  IF(CUSTOM_CMAKE_VERSION VERSION_EQUAL "2.8.0" OR CUSTOM_CMAKE_VERSION VERSION_GREATER "2.8.0") 
    ADD_TEST(integration-FunctionsReportEnvVar "${BIN_OUTPUT_PATH}/openfluid-engine" "-r")
    SET_TESTS_PROPERTIES(integration-FunctionsReportEnvVar PROPERTIES ENVIRONMENT "OPENFLUID_FUNCS_PATH=${TEST_OUTPUT_PATH}")                           
    SET_TESTS_PROPERTIES(integration-FunctionsReportEnvVar PROPERTIES PASS_REGULAR_EXPRESSION "tests.primitives.use")  
    SET_TESTS_PROPERTIES(integration-FunctionsReportEnvVar PROPERTIES PASS_REGULAR_EXPRESSION "tests.primitives.prod")    
  ENDIF(CUSTOM_CMAKE_VERSION VERSION_EQUAL "2.8.0" OR CUSTOM_CMAKE_VERSION VERSION_GREATER "2.8.0")

  ADD_TEST(integration-FunctionsReportNoEnvVar "${BIN_OUTPUT_PATH}/openfluid-engine" "-r")                           
  SET_TESTS_PROPERTIES(integration-FunctionsReportNoEnvVar PROPERTIES PASS_REGULAR_EXPRESSION "tests.primitives.use")  
  SET_TESTS_PROPERTIES(integration-FunctionsReportNoEnvVar PROPERTIES PASS_REGULAR_EXPRESSION "tests.primitives.prod")
  SET_TESTS_PROPERTIES(integration-FunctionsReportNoEnvVar PROPERTIES WILL_FAIL TRUE)        

  ADD_TEST(integration-FunctionsMatchReport "${BIN_OUTPUT_PATH}/openfluid-engine"
                                "-u" "tests.primitives.*"
                                "-p${TEST_OUTPUT_PATH}")
  SET_TESTS_PROPERTIES(integration-FunctionsMatchReport PROPERTIES PASS_REGULAR_EXPRESSION "tests.primitives.use")  
  SET_TESTS_PROPERTIES(integration-FunctionsMatchReport PROPERTIES PASS_REGULAR_EXPRESSION "tests.primitives.prod")
                           
  
  ADD_TEST(integration-FunctionsXMLReport "${BIN_OUTPUT_PATH}/openfluid-engine"
                              "-x"
                              "-p${TEST_OUTPUT_PATH}")
  SET_TESTS_PROPERTIES(integration-FunctionsXMLReport PROPERTIES WILL_FAIL TRUE)        
                              
  
  ADD_TEST(integration-InputDoesNotExist "${BIN_OUTPUT_PATH}/openfluid-engine" "-i" "${CMAKE_BINARY_DIR}/path/does/not/exist")                           
  SET_TESTS_PROPERTIES(integration-InputDoesNotExist PROPERTIES WILL_FAIL TRUE)

  ADD_TEST(integration-InputWithNoModel "${BIN_OUTPUT_PATH}/openfluid-engine"
                                        "-i" "${TESTS_DATASETS_PATH}/OPENFLUID.IN.NoModel"
                                        "-o" "${TESTS_OUTPUTDATA_PATH}/OPENFLUID.OUT.NoModel"
                                        "-p" "${TEST_OUTPUT_PATH}")               
  SET_TESTS_PROPERTIES(integration-InputWithNoModel PROPERTIES WILL_FAIL TRUE)


  ADD_TEST(integration-InputWithNoRun "${BIN_OUTPUT_PATH}/openfluid-engine"
                                        "-i" "${TESTS_DATASETS_PATH}/OPENFLUID.IN.NoRun"
                                        "-o" "${TESTS_OUTPUTDATA_PATH}/OPENFLUID.OUT.NoRun"
                                        "-p" "${TEST_OUTPUT_PATH}")                           
  SET_TESTS_PROPERTIES(integration-InputWithNoRun PROPERTIES WILL_FAIL TRUE)


  ADD_TEST(integration-MissingFunction "${BIN_OUTPUT_PATH}/openfluid-engine"
                                        "-i" "${TESTS_DATASETS_PATH}/OPENFLUID.IN.MissingFunc"
                                        "-o" "${TESTS_OUTPUTDATA_PATH}/OPENFLUID.OUT.MissingFunc"
                                        "-p" "${TEST_OUTPUT_PATH}")               
  SET_TESTS_PROPERTIES(integration-MissingFunction PROPERTIES WILL_FAIL TRUE)


  ADD_TEST(integration-MissingUnitClass "${BIN_OUTPUT_PATH}/openfluid-engine"
                                        "-i" "${TESTS_DATASETS_PATH}/OPENFLUID.IN.MissingClass"
                                        "-o" "${TESTS_OUTPUTDATA_PATH}/OPENFLUID.OUT.MissingClass"
                                        "-p" "${TEST_OUTPUT_PATH}")               
  SET_TESTS_PROPERTIES(integration-MissingUnitClass PROPERTIES WILL_FAIL TRUE)


  ADD_TEST(integration-MissingVariable "${BIN_OUTPUT_PATH}/openfluid-engine"
                                        "-i" "${TESTS_DATASETS_PATH}/OPENFLUID.IN.MissingVar"
                                        "-o" "${TESTS_OUTPUTDATA_PATH}/OPENFLUID.OUT.MissingVar"
                                        "-p" "${TEST_OUTPUT_PATH}")               
  SET_TESTS_PROPERTIES(integration-MissingVariable PROPERTIES WILL_FAIL TRUE)


  ADD_TEST(integration-MissingInputdata "${BIN_OUTPUT_PATH}/openfluid-engine"
                                        "-i" "${TESTS_DATASETS_PATH}/OPENFLUID.IN.MissingData"
                                        "-o" "${TESTS_OUTPUTDATA_PATH}/OPENFLUID.OUT.MissingData"
                                        "-p" "${TEST_OUTPUT_PATH}")               
  SET_TESTS_PROPERTIES(integration-MissingInputdata PROPERTIES WILL_FAIL TRUE)

  ADD_TEST(integration-DuplicateVariable "${BIN_OUTPUT_PATH}/openfluid-engine"
                                        "-i" "${TESTS_DATASETS_PATH}/OPENFLUID.IN.DuplicateVar"
                                        "-o" "${TESTS_OUTPUTDATA_PATH}/OPENFLUID.OUT.DuplicateVar"
                                        "-p" "${TEST_OUTPUT_PATH}")               
  SET_TESTS_PROPERTIES(integration-DuplicateVariable PROPERTIES WILL_FAIL TRUE)



  ADD_TEST(integration-Verbose "${BIN_OUTPUT_PATH}/openfluid-engine"
                               "-i" "${TESTS_DATASETS_PATH}/OPENFLUID.IN.VariableTimeProd"
                               "-o" "${TESTS_OUTPUTDATA_PATH}/OPENFLUID.OUT.Verbose"
                               "-p" "${TEST_OUTPUT_PATH}"
                               "-v")
  
  ADD_TEST(integration-Quiet "${BIN_OUTPUT_PATH}/openfluid-engine"
                               "-i" "${TESTS_DATASETS_PATH}/OPENFLUID.IN.VariableTimeProd"
                               "-o" "${TESTS_OUTPUTDATA_PATH}/OPENFLUID.OUT.Quiet"
                               "-p" "${TEST_OUTPUT_PATH}"
                               "-q")


  FILE(COPY "${TESTS_DATASETS_PATH}/OPENFLUID.PRJ.Primitives" 
       DESTINATION "${TESTS_OUTPUTDATA_PATH}"
       PATTERN ".svn" EXCLUDE)

  ADD_TEST(integration-Project "${BIN_OUTPUT_PATH}/openfluid-engine"
                               "-w" "${TESTS_OUTPUTDATA_PATH}/OPENFLUID.PRJ.Primitives"
                               "-p" "${TEST_OUTPUT_PATH}"
                               "-c")

  ADD_TEST(integration-Profiling "${BIN_OUTPUT_PATH}/openfluid-engine"
                               "-i" "${TESTS_DATASETS_PATH}/OPENFLUID.IN.Primitives"
                               "-o" "${TESTS_OUTPUTDATA_PATH}/OPENFLUID.OUT.Profiling"
                               "-p" "${TEST_OUTPUT_PATH}"
                               "-k")


  ADD_TEST(integration-FluidXWriterSingle "${BIN_OUTPUT_PATH}/openfluid-engine"
                               "-i" "${TESTS_OUTPUTDATA_PATH}/OPENFLUID.OUT.FluidXWriterSingle"
                               "-o" "${TESTS_OUTPUTDATA_PATH}/OPENFLUID.OUT.FluidXWriterRunSingle"
                               "-p" "${TEST_OUTPUT_PATH}")
  SET_PROPERTY(TEST integration-FluidXWriterSingle APPEND PROPERTY DEPENDS api-FLuidXDescriptor_TEST)

  ADD_TEST(integration-FluidXWriterMany "${BIN_OUTPUT_PATH}/openfluid-engine"
                               "-i" "${TESTS_OUTPUTDATA_PATH}/OPENFLUID.OUT.FluidXWriterMany"
                               "-o" "${TESTS_OUTPUTDATA_PATH}/OPENFLUID.OUT.FluidXWriterRunMany"
                               "-p" "${TEST_OUTPUT_PATH}")
  SET_PROPERTY(TEST integration-FluidXWriterMany APPEND PROPERTY DEPENDS api-FLuidXDescriptor_TEST)
  

  FILE(MAKE_DIRECTORY "${TESTS_OUTPUTDATA_PATH}/tests.func.cmake")
  
  ADD_TEST(integration-CMakeModuleFunctionConfigure
                              "${CMAKE_COMMAND}"
                              "-E" "chdir" "${TESTS_OUTPUTDATA_PATH}/tests.func.cmake" 
                              "${CMAKE_COMMAND}" "-G" "${CMAKE_GENERATOR}"
                              "${CMAKE_CURRENT_SOURCE_DIR}/tests.func.cmake"
                              "-DOpenFLUID_DIR=${CMAKE_MODULES_OUTPUT_PATH}"
                              "-DOpenFLUID_ROOT_DIR=${BUILD_OUTPUT_PATH};${CMAKE_BINARY_DIR}/src"
                              "-DFUNC_INCLUDE_DIRS=${CMAKE_SOURCE_DIR}/src")                              
                              
  ADD_TEST(integration-CMakeModuleFunctionBuild
                              "${CMAKE_COMMAND}"
                              "-E" "chdir" "${TESTS_OUTPUTDATA_PATH}/tests.func.cmake" 
                              "${CMAKE_COMMAND}" "--build" "${TESTS_OUTPUTDATA_PATH}/tests.func.cmake")
  SET_PROPERTY(TEST integration-CMakeModuleFunctionBuild APPEND PROPERTY DEPENDS integration-CMakeModuleFunctionConfigure)                                                            


  FILE(MAKE_DIRECTORY "${TESTS_OUTPUTDATA_PATH}/tests.obs.cmake")

  ADD_TEST(integration-CMakeModuleObserverConfigure
                              "${CMAKE_COMMAND}"
                              "-E" "chdir" "${TESTS_OUTPUTDATA_PATH}/tests.obs.cmake" 
                              "${CMAKE_COMMAND}" "-G" "${CMAKE_GENERATOR}"
                              "${CMAKE_CURRENT_SOURCE_DIR}/tests.obs.cmake"
                              "-DOpenFLUID_DIR=${CMAKE_MODULES_OUTPUT_PATH}"
                              "-DOpenFLUID_ROOT_DIR=${BUILD_OUTPUT_PATH};${CMAKE_BINARY_DIR}/src"                              
                              "-DOBS_INCLUDE_DIRS=${CMAKE_SOURCE_DIR}/src")                              
                              
  ADD_TEST(integration-CMakeModuleObserverBuild
                              "${CMAKE_COMMAND}"
                              "-E" "chdir" "${TESTS_OUTPUTDATA_PATH}/tests.obs.cmake" 
                              "${CMAKE_COMMAND}" "--build" "${TESTS_OUTPUTDATA_PATH}/tests.obs.cmake")
  SET_PROPERTY(TEST integration-CMakeModuleObserverBuild APPEND PROPERTY DEPENDS integration-CMakeModuleObserverConfigure)                                                            

  ADD_TEST(integration-CMakeModuleRun "${BIN_OUTPUT_PATH}/openfluid-engine" 
                                      "-i" "${TESTS_DATASETS_PATH}/OPENFLUID.IN.CMakeModule"
                                      "-o" "${TESTS_OUTPUTDATA_PATH}/OPENFLUID.OUT.CMakeModule" 
                                      "-p" "${TESTS_OUTPUTDATA_PATH}/tests.func.cmake"
                                      "-n" "${TESTS_OUTPUTDATA_PATH}/tests.obs.cmake")
  SET_PROPERTY(TEST integration-CMakeModuleRun APPEND PROPERTY DEPENDS integration-CMakeModuleFunctionBuild)
  SET_PROPERTY(TEST integration-CMakeModuleRun APPEND PROPERTY DEPENDS integration-CMakeModuleObserverBuild)
  SET_TESTS_PROPERTIES(integration-CMakeModuleRun PROPERTIES PASS_REGULAR_EXPRESSION "CMake module built function"
                                                             PASS_REGULAR_EXPRESSION "CMake module built observer")
                                                             


  ADD_TEST(integration-BuddyFunc2DocHelp "${BIN_OUTPUT_PATH}/openfluid-engine"
                              "--buddyhelp" "func2doc"
                              )


  ADD_TEST(integration-BuddyFunc2Doc "${BIN_OUTPUT_PATH}/openfluid-engine"
                              "--buddy" "func2doc"
                              "--buddyopts" "inputcpp=${CMAKE_SOURCE_DIR}/src/tests/functions/tests.ofefunc2doc/OFEFunc2DocFunc.cpp,outputdir=${TESTS_OUTPUTDATA_PATH}/func2doc"
                              )
  

  ADD_TEST(integration-BuddyFunc2DocPDF "${BIN_OUTPUT_PATH}/openfluid-engine"
                              "--buddy" "func2doc"
                              "--buddyopts" "inputcpp=${CMAKE_SOURCE_DIR}/src/tests/functions/tests.ofefunc2doc/OFEFunc2DocFunc.cpp,outputdir=${TESTS_OUTPUTDATA_PATH}/func2doc-PDF,tplfile=${CMAKE_SOURCE_DIR}/${SHARE_COMMON_INSTALL_PATH}/func2doc_tpl.tex,pdf=1"
                              )


  ADD_TEST(integration-BuddyFunc2DocPDFHTML "${BIN_OUTPUT_PATH}/openfluid-engine"
                              "--buddy" "func2doc"
                              "--buddyopts" "inputcpp=${CMAKE_SOURCE_DIR}/src/tests/functions/tests.ofefunc2doc/OFEFunc2DocFunc.cpp,outputdir=${TESTS_OUTPUTDATA_PATH}/func2doc-PDF-HTML,pdf=1,html=1"
                              )


  ADD_TEST(integration-BuddyNewFuncHelp "${BIN_OUTPUT_PATH}/openfluid-engine"
                              "--buddyhelp" "newfunc"
                              )

  FILE(MAKE_DIRECTORY "${TESTS_OUTPUTDATA_PATH}/tests.buddy.newfunc")
  CONFIGURE_FILE("${TESTS_DATASETS_PATH}/tests.buddy.newfunc/CMakeLists.txt.in" "${TESTS_OUTPUTDATA_PATH}/tests.buddy.newfunc/CMakeLists.txt" @ONLY)
    
  ADD_TEST(integration-BuddyNewFunc "${BIN_OUTPUT_PATH}/openfluid-engine"
                              "--buddy" "newfunc"
                              "--buddyopts" "funcid=tests.buddy.newfunc,cppclass=NewFunctionUsingBuddy,outputdir=${TESTS_OUTPUTDATA_PATH}/tests.buddy.newfunc,authorname=John Doe,authoremail=doe@foo.bar.org"
                              )

  ADD_TEST(integration-BuddyNewFuncConfigure "${CMAKE_COMMAND}"
                              "-E" "chdir" "${TESTS_OUTPUTDATA_PATH}/tests.buddy.newfunc" "${CMAKE_COMMAND}" "-G" "${CMAKE_GENERATOR}" "." 
                              )
  SET_PROPERTY(TEST integration-BuddyNewFuncConfigure APPEND PROPERTY DEPENDS integration-BuddyNewFunc)

  ADD_TEST(integration-BuddyNewFuncBuild "${CMAKE_COMMAND}"
                              "-E" "chdir" "${TESTS_OUTPUTDATA_PATH}/tests.buddy.newfunc" "${CMAKE_BUILD_TOOL}"
                              )
  SET_PROPERTY(TEST integration-BuddyNewFuncBuild APPEND PROPERTY DEPENDS integration-BuddyNewFuncConfigure)

  ADD_TEST(integration-BuddyNewFuncCheck "${BIN_OUTPUT_PATH}/openfluid-engine" 
                         "-r"
                         "-p" "${TESTS_OUTPUTDATA_PATH}/tests.buddy.newfunc")
  SET_TESTS_PROPERTIES(integration-BuddyNewFuncCheck PROPERTIES PASS_REGULAR_EXPRESSION "tests.buddy.newfunc"
                                                                PASS_REGULAR_EXPRESSION "John Doe"
                                                                PASS_REGULAR_EXPRESSION "doe@foo.bar.org") 
  SET_PROPERTY(TEST integration-BuddyNewFuncCheck APPEND PROPERTY DEPENDS integration-BuddyNewFuncBuild)                                                                 
                       
  ADD_TEST(integration-BuddyNewFuncRun "${BIN_OUTPUT_PATH}/openfluid-engine" 
                                       "-i" "${TESTS_DATASETS_PATH}/OPENFLUID.IN.BuddyNewFunc"
                                       "-o" "${TESTS_OUTPUTDATA_PATH}/OPENFLUID.OUT.BuddyNewFunc" 
                                       "-p" "${TESTS_OUTPUTDATA_PATH}/tests.buddy.newfunc"
                                       "-v")
  SET_PROPERTY(TEST integration-BuddyNewFuncRun APPEND PROPERTY DEPENDS integration-BuddyNewFuncBuild)


  ADD_TEST(integration-BuddyConvertHelp "${BIN_OUTPUT_PATH}/openfluid-engine"
                              "--buddyhelp" "convert"
                              )

  IF(${FULL_VERSION} VERSION_GREATER "1.3") 
    ADD_TEST(integration-BuddyConvert1314 "${BIN_OUTPUT_PATH}/openfluid-engine"
                              "--buddy" "convert"
                              "--buddyopts" "convmode=13_14,inputdir=${TESTS_DATASETS_PATH}/OPENFLUID.IN.V13,outputdir=${TESTS_OUTPUTDATA_PATH}/OPENFLUID.IN.Converted13to14"
                              )                              
  ENDIF(${FULL_VERSION} VERSION_GREATER "1.3") 

  IF(${FULL_VERSION} EQUAL "1.4")  
    ADD_TEST(integration-BuddyConvert1314Check "${BIN_OUTPUT_PATH}/openfluid-engine"
                                       "-i" "${TESTS_OUTPUTDATA_PATH}/OPENFLUID.IN.Converted13to14"
                                       "-o" "${TESTS_OUTPUTDATA_PATH}/OPENFLUID.OUT.Converted13to14"
                                       "-p" "${TEST_OUTPUT_PATH}"
                              )
    SET_PROPERTY(TEST integration-BuddyConvert1314Check APPEND PROPERTY DEPENDS integration-BuddyConvert1314)
  ENDIF(${FULL_VERSION} EQUAL "1.4")  

  IF(${FULL_VERSION} VERSION_GREATER "1.4") 
    ADD_TEST(integration-BuddyConvert1415 "${BIN_OUTPUT_PATH}/openfluid-engine"
                              "--buddy" "convert"
                              "--buddyopts" "convmode=14_15,inputdir=${TESTS_OUTPUTDATA_PATH}/OPENFLUID.IN.Converted13to14,outputdir=${TESTS_OUTPUTDATA_PATH}/OPENFLUID.IN.Converted14to15"
                              )
    SET_PROPERTY(TEST integration-BuddyConvert1415 APPEND PROPERTY DEPENDS integration-BuddyConvert1314)                              
  ENDIF(${FULL_VERSION} VERSION_GREATER "1.4") 

  IF(${FULL_VERSION} EQUAL "1.5")  
    ADD_TEST(integration-BuddyConvert1415Check "${BIN_OUTPUT_PATH}/openfluid-engine"
                                       "-i" "${TESTS_OUTPUTDATA_PATH}/OPENFLUID.IN.Converted14to15"
                                       "-o" "${TESTS_OUTPUTDATA_PATH}/OPENFLUID.OUT.Converted14to15"
                                       "-p" "${TEST_OUTPUT_PATH}"
                              )
    SET_PROPERTY(TEST integration-BuddyConvert1415Check APPEND PROPERTY DEPENDS integration-BuddyConvert1415)                              
  ENDIF(${FULL_VERSION} EQUAL "1.5")  

  IF(${FULL_VERSION} VERSION_GREATER "1.5") 
    ADD_TEST(integration-BuddyConvert1516 "${BIN_OUTPUT_PATH}/openfluid-engine"
                              "--buddy" "convert"
                              "--buddyopts" "convmode=15_16,inputdir=${TESTS_OUTPUTDATA_PATH}/OPENFLUID.IN.Converted14to15,outputdir=${TESTS_OUTPUTDATA_PATH}/OPENFLUID.IN.Converted15to16"
                              )
    SET_PROPERTY(TEST integration-BuddyConvert1516 APPEND PROPERTY DEPENDS integration-BuddyConvert1415)                              
  ENDIF(${FULL_VERSION} VERSION_GREATER "1.5")
 
  IF(${FULL_VERSION} EQUAL "1.6")  
    ADD_TEST(integration-BuddyConvert1516Check "${BIN_OUTPUT_PATH}/openfluid-engine"
                                       "-i" "${TESTS_OUTPUTDATA_PATH}/OPENFLUID.IN.Converted15to16"
                                       "-o" "${TESTS_OUTPUTDATA_PATH}/OPENFLUID.OUT.Converted15to16"
                                       "-p" "${TEST_OUTPUT_PATH}"
                              )
    SET_PROPERTY(TEST integration-BuddyConvert1516Check APPEND PROPERTY DEPENDS integration-BuddyConvert1516)                              
  ENDIF(${FULL_VERSION} EQUAL "1.6")  

  IF(${FULL_VERSION} VERSION_GREATER "1.5")
    ADD_TEST(integration-BuddyConvert1516wBuff "${BIN_OUTPUT_PATH}/openfluid-engine"
                              "--buddy" "convert"
                              "--buddyopts" "convmode=15_16,inputdir=${TESTS_DATASETS_PATH}/OPENFLUID.IN.V15,outputdir=${TESTS_OUTPUTDATA_PATH}/OPENFLUID.IN.Converted15to16wBuff"
                              )                                  
  ENDIF(${FULL_VERSION} VERSION_GREATER "1.5")
  
  IF(${FULL_VERSION} EQUAL "1.6")  
    ADD_TEST(integration-BuddyConvert1516wBuffCheck "${BIN_OUTPUT_PATH}/openfluid-engine"
                                       "-i" "${TESTS_OUTPUTDATA_PATH}/OPENFLUID.IN.Converted15to16wBuff"
                                       "-o" "${TESTS_OUTPUTDATA_PATH}/OPENFLUID.OUT.Converted15to16wBuff"
                                       "-p" "${TEST_OUTPUT_PATH}"
                              )
    SET_PROPERTY(TEST integration-BuddyConvert1516wBuffCheck APPEND PROPERTY DEPENDS integration-BuddyConvert1516wBuff)                              
  ENDIF(${FULL_VERSION} EQUAL "1.6")


  ADD_TEST(integration-BuddyNewDataHelp "${BIN_OUTPUT_PATH}/openfluid-engine"
                              "--buddyhelp" "newdata"
                              )

  ADD_TEST(integration-BuddyNewData "${BIN_OUTPUT_PATH}/openfluid-engine"
                              "--buddy" "newdata"
                              "--buddyopts" "outputdir=${TESTS_OUTPUTDATA_PATH}/OPENFLUID.IN.NewData"
                              )

  ADD_TEST(integration-BuddyNewDataCheck "${BIN_OUTPUT_PATH}/openfluid-engine"
                                       "-i" "${TESTS_OUTPUTDATA_PATH}/OPENFLUID.IN.NewData"
                                       "-o" "${TESTS_OUTPUTDATA_PATH}/OPENFLUID.OUT.NewData"
                                       "-p" "${TEST_OUTPUT_PATH}"
                              )
  SET_PROPERTY(TEST integration-BuddyNewDataCheck APPEND PROPERTY DEPENDS integration-BuddyNewData)

  ADD_TEST(integration-MinimalRun "${BIN_OUTPUT_PATH}/openfluid-minimal"
                                  "${TESTS_DATASETS_PATH}/OPENFLUID.IN.Primitives"
                                  "${TESTS_OUTPUTDATA_PATH}/OPENFLUID.OUT.MinimalRun"
                                  "${TEST_OUTPUT_PATH}"
                              )
  
  ADD_TEST(integration-MinimalFailsArgs "${BIN_OUTPUT_PATH}/openfluid-minimal"
                                  "${TESTS_DATASETS_PATH}/OPENFLUID.IN.VariableTimeProd"
                                  "${TESTS_OUTPUTDATA_PATH}/OPENFLUID.OUT.MinimalFailsArgs"
                              )
  SET_TESTS_PROPERTIES(integration-MinimalFailsArgs PROPERTIES WILL_FAIL TRUE)
                              
  ADD_TEST(integration-MinimalFailsInput "${BIN_OUTPUT_PATH}/openfluid-minimal"
                                  "${TESTS_DATASETS_PATH}/OPENFLUID.IN.doesnotexist"
                                  "${TESTS_OUTPUTDATA_PATH}/OPENFLUID.OUT.MinimalFailsInput"
                                  "${TEST_OUTPUT_PATH}"
                              )                              
  SET_TESTS_PROPERTIES(integration-MinimalFailsInput PROPERTIES WILL_FAIL TRUE)
 
 
  ADD_TEST(integration-Debug "${BIN_OUTPUT_PATH}/openfluid-engine" 
                             "-f" "-p${TEST_OUTPUT_PATH}")
  IF(${CMAKE_BUILD_TYPE} STREQUAL "Debug")
    SET_TESTS_PROPERTIES(integration-Debug PROPERTIES PASS_REGULAR_EXPRESSION "OpenFLUID debugging mode is enabled")
  ELSE(${CMAKE_BUILD_TYPE} STREQUAL "Debug")
    SET_TESTS_PROPERTIES(integration-Debug PROPERTIES FAIL_REGULAR_EXPRESSION "OpenFLUID debugging mode is enabled")
  ENDIF(${CMAKE_BUILD_TYPE} STREQUAL "Debug") 
  
  