FIND_PACKAGE(Boost REQUIRED COMPONENTS unit_test_framework)  

MACRO(ADD_INTEGRATIONTEST TEST_NAME EXE_NAME)
  ADD_EXECUTABLE(${EXE_NAME} ${ARGN})
  TARGET_LINK_LIBRARIES(${EXE_NAME} ${Boost_UNIT_TEST_FRAMEWORK_LIBRARY} ${LibXML2_LIBRARIES} ofelib)
  SET_TARGET_PROPERTIES(${EXE_NAME} PROPERTIES RUNTIME_OUTPUT_DIRECTORY "${TEST_OUTPUT_PATH}")
  ADD_TEST(${TEST_NAME} "${TEST_OUTPUT_PATH}/${EXE_NAME}")
ENDMACRO(ADD_INTEGRATIONTEST)

ADD_DEFINITIONS(-DOFELIB_VERSION=${ENGINE_FULL_VERSION})

INCLUDE_DIRECTORIES(${Boost_INCLUDE_DIRS} ${LibXML2_INCLUDE_DIRS})
INCLUDE_DIRECTORIES(. ${CMAKE_SOURCE_DIR}/include ${CMAKE_BINARY_DIR})

LINK_DIRECTORIES(${LibXML2_LIBRARY_DIRS})


  ADD_TEST(integration-DisplayHelp "${BIN_OUTPUT_PATH}/openfluid-engine" 
                       "-h")
  SET_TESTS_PROPERTIES(integration-DisplayHelp PROPERTIES PASS_REGULAR_EXPRESSION "openfluid-engine allowed options")  

  ADD_TEST(integration-DisplayVersion "${BIN_OUTPUT_PATH}/openfluid-engine" 
                       "--version")
  SET_TESTS_PROPERTIES(integration-DisplayVersion PROPERTIES PASS_REGULAR_EXPRESSION ${ENGINE_FULL_VERSION})  
                       
  ADD_TEST(integration-ShowPaths "${BIN_OUTPUT_PATH}/openfluid-engine" 
                       "--show-paths"
                       "-p" "foobar")  
  SET_TESTS_PROPERTIES(integration-ShowPaths PROPERTIES PASS_REGULAR_EXPRESSION "#1 foobar")
  
  
  ADD_TEST(integration-FunctionsList "${BIN_OUTPUT_PATH}/openfluid-engine" 
                         "-f"
                         "-p${TEST_OUTPUT_PATH}")
  
  ADD_TEST(integration-FunctionsReport "${BIN_OUTPUT_PATH}/openfluid-engine"
                           "-r"
                           "-p${TEST_OUTPUT_PATH}")
  
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

  ADD_FUNCTION(tests.outputs tests.outputs ${TEST_OUTPUT_PATH})
  ADD_INTEGRATIONTEST(integration-Outputs integrationtest_outputs Outputs_TEST.cpp)

  ADD_TEST(integration-Verbose "${BIN_OUTPUT_PATH}/openfluid-engine"
                               "-i" "${TESTS_DATASETS_PATH}/OPENFLUID.IN.CheckOutputsAtEnd"
                               "-o" "${TESTS_OUTPUTDATA_PATH}/OPENFLUID.OUT.CheckVerbose"
                               "-p" "${TEST_OUTPUT_PATH}"
                               "-v")
  
  ADD_TEST(integration-Quiet "${BIN_OUTPUT_PATH}/openfluid-engine"
                               "-i" "${TESTS_DATASETS_PATH}/OPENFLUID.IN.CheckOutputsAtEnd"
                               "-o" "${TESTS_OUTPUTDATA_PATH}/OPENFLUID.OUT.CheckQuiet"
                               "-p" "${TEST_OUTPUT_PATH}"
                               "-q")
                               

  ADD_TEST(integration-BuddyFunc2DocHelp "${BIN_OUTPUT_PATH}/openfluid-engine"
                              "--buddyhelp" "func2doc"
                              )


  ADD_TEST(integration-BuddyFunc2Doc "${BIN_OUTPUT_PATH}/openfluid-engine"
                              "--buddy" "func2doc"
                              "--buddyopts" "inputcpp=${CMAKE_SOURCE_DIR}/tests/functions/tests.ofefunc2doc/OFEFunc2DocFunc.cpp,outputdir=${TESTS_OUTPUTDATA_PATH}/func2doc,tplfile=${CMAKE_SOURCE_DIR}/resources/func2doc/func2doc_tpl.tex"
                              )


  ADD_TEST(integration-BuddyFunc2DocPDF "${BIN_OUTPUT_PATH}/openfluid-engine"
                              "--buddy" "func2doc"
                              "--buddyopts" "inputcpp=${CMAKE_SOURCE_DIR}/tests/functions/tests.ofefunc2doc/OFEFunc2DocFunc.cpp,outputdir=${TESTS_OUTPUTDATA_PATH}/func2doc-PDF,tplfile=${CMAKE_SOURCE_DIR}/resources/func2doc/func2doc_tpl.tex,pdf=1"
                              )


  ADD_TEST(integration-BuddyFunc2DocPDFHTML "${BIN_OUTPUT_PATH}/openfluid-engine"
                              "--buddy" "func2doc"
                              "--buddyopts" "inputcpp=${CMAKE_SOURCE_DIR}/tests/functions/tests.ofefunc2doc/OFEFunc2DocFunc.cpp,outputdir=${TESTS_OUTPUTDATA_PATH}/func2doc-PDF-HTML,tplfile=${CMAKE_SOURCE_DIR}/resources/func2doc/func2doc_tpl.tex,pdf=1,html=1"
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

  ADD_TEST(integration-BuddyNewFuncBuild "${CMAKE_COMMAND}"
                              "-E" "chdir" "${TESTS_OUTPUTDATA_PATH}/tests.buddy.newfunc" "${CMAKE_BUILD_TOOL}"
                              )


  ADD_TEST(integration-BuddyNewFuncCheck "${BIN_OUTPUT_PATH}/openfluid-engine" 
                         "-r"
                         "-p" "${TESTS_OUTPUTDATA_PATH}/tests.buddy.newfunc")
  SET_TESTS_PROPERTIES(integration-BuddyNewFuncCheck PROPERTIES PASS_REGULAR_EXPRESSION "tests.buddy.newfunc"
                                                                PASS_REGULAR_EXPRESSION "John Doe"
                                                                PASS_REGULAR_EXPRESSION "doe@foo.bar.org")  
                       
  ADD_TEST(integration-BuddyNewFuncRun "${BIN_OUTPUT_PATH}/openfluid-engine" 
                                       "-i" "${TESTS_DATASETS_PATH}/OPENFLUID.IN.CheckBuddyNewFunc"
                                       "-o" "${TESTS_OUTPUTDATA_PATH}/OPENFLUID.OUT.CheckBuddyNewFunc" 
                                       "-p" "${TESTS_OUTPUTDATA_PATH}/tests.buddy.newfunc"
                                       "-v")



  ADD_TEST(integration-BuddyConvertHelp "${BIN_OUTPUT_PATH}/openfluid-engine"
                              "--buddyhelp" "convert"
                              )

  ADD_TEST(integration-BuddyConvert1314 "${BIN_OUTPUT_PATH}/openfluid-engine"
                              "--buddy" "convert"
                              "--buddyopts" "convmode=13_14,inputdir=${TESTS_DATASETS_PATH}/OPENFLUID.IN.V13,outputdir=${TESTS_OUTPUTDATA_PATH}/OPENFLUID.IN.Converted13to14"
                              )

  ADD_TEST(integration-BuddyConvert1314Check "${BIN_OUTPUT_PATH}/openfluid-engine"
                                       "-i" "${TESTS_OUTPUTDATA_PATH}/OPENFLUID.IN.Converted13to14"
                                       "-o" "${TESTS_OUTPUTDATA_PATH}/OPENFLUID.OUT.Converted13to14"
                                       "-p" "${TEST_OUTPUT_PATH}"
                              )


  ADD_TEST(integration-BuddyConvert1415 "${BIN_OUTPUT_PATH}/openfluid-engine"
                              "--buddy" "convert"
                              "--buddyopts" "convmode=14_15,inputdir=${TESTS_OUTPUTDATA_PATH}/OPENFLUID.IN.Converted13to14,outputdir=${TESTS_OUTPUTDATA_PATH}/OPENFLUID.IN.Converted14to15"
                              )

  ADD_TEST(integration-BuddyConvert1415Check "${BIN_OUTPUT_PATH}/openfluid-engine"
                                       "-i" "${TESTS_OUTPUTDATA_PATH}/OPENFLUID.IN.Converted14to15"
                                       "-o" "${TESTS_OUTPUTDATA_PATH}/OPENFLUID.OUT.Converted14to15"
                                       "-p" "${TEST_OUTPUT_PATH}"
                              )



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


  