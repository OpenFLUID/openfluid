  ADD_TEST(Integration-DisplayHelp "${BIN_OUTPUT_PATH}/openfluid-engine" 
                       "-h")
  SET_TESTS_PROPERTIES(Integration-DisplayHelp PROPERTIES PASS_REGULAR_EXPRESSION "openfluid-engine allowed options")  

  ADD_TEST(Integration-DisplayVersion "${BIN_OUTPUT_PATH}/openfluid-engine" 
                       "--version")
  SET_TESTS_PROPERTIES(Integration-DisplayVersion PROPERTIES PASS_REGULAR_EXPRESSION ${ENGINE_FULL_VERSION})  
                       
  
  ADD_TEST(Integration-FunctionsList "${BIN_OUTPUT_PATH}/openfluid-engine" 
                         "-f"
                         "-p${TEST_OUTPUT_PATH}")
  
  ADD_TEST(Integration-FunctionsReport "${BIN_OUTPUT_PATH}/openfluid-engine"
                           "-r"
                           "-p${TEST_OUTPUT_PATH}")
  

  ADD_TEST(Integration-FunctionsMatchReport "${BIN_OUTPUT_PATH}/openfluid-engine"
                                "-u" "tests.primitives.*"
                                "-p${TEST_OUTPUT_PATH}")
  SET_TESTS_PROPERTIES(Integration-FunctionsMatchReport PROPERTIES PASS_REGULAR_EXPRESSION "tests.primitives.use")  
  SET_TESTS_PROPERTIES(Integration-FunctionsMatchReport PROPERTIES PASS_REGULAR_EXPRESSION "tests.primitives.prod")
                           
  
  ADD_TEST(Integration-FunctionsXMLReport "${BIN_OUTPUT_PATH}/openfluid-engine"
                              "-x"
                              "-p${TEST_OUTPUT_PATH}")
  

#  ADD_TEST(Integration-OFEFunc2Doc "${BIN_OUTPUT_PATH}/ofefunc2doc" 
#                       "${CMAKE_SOURCE_DIR}/src/functions/tests/tests.ofefunc2doc/OFEFunc2DocFunc.cpp"
#                       "-t${CMAKE_SOURCE_DIR}/resources/ofefunc2doc/ofefunc2doc_tpl.tex"
#                       "-o${TEST_OUTPUT_PATH}"
#                       "-p")                              


  ADD_TEST(Integration-BuddyFunc2DocHelp "${BIN_OUTPUT_PATH}/openfluid-engine"
                              "--buddyhelp" "func2doc"
                              )

  ADD_TEST(Integration-BuddyFunc2Doc "${BIN_OUTPUT_PATH}/openfluid-engine"
                              "--buddy" "func2doc"
                              "--buddyopts" "inputcpp=${CMAKE_SOURCE_DIR}/tests/functions/tests.ofefunc2doc/OFEFunc2DocFunc.cpp,outputdir=${TESTS_OUTPUTDATA_PATH}/func2doc,tplfile=${CMAKE_SOURCE_DIR}/resources/func2doc/func2doc_tpl.tex,pdf=1,html=1"
                              )


  ADD_TEST(Integration-BuddyNewFuncHelp "${BIN_OUTPUT_PATH}/openfluid-engine"
                              "--buddyhelp" "newfunc"
                              )

  FILE(MAKE_DIRECTORY "${TESTS_OUTPUTDATA_PATH}/tests.buddy.newfunc")
  CONFIGURE_FILE("${TESTS_DATASETS_PATH}/tests.buddy.newfunc/CMakeLists.txt.in" "${TESTS_OUTPUTDATA_PATH}/tests.buddy.newfunc/CMakeLists.txt" @ONLY)
    
  ADD_TEST(Integration-BuddyNewFunc "${BIN_OUTPUT_PATH}/openfluid-engine"
                              "--buddy" "newfunc"
                              "--buddyopts" "funcid=tests.buddy.newfunc,cppclass=NewFunctionUsingBuddy,outputdir=${TESTS_OUTPUTDATA_PATH}/tests.buddy.newfunc,authorname=John Doe,authoremail=doe@foo.bar.org"
                              )

  ADD_TEST(Integration-BuddyNewFuncConfigure "${CMAKE_COMMAND}"
                              "-E" "chdir" "${TESTS_OUTPUTDATA_PATH}/tests.buddy.newfunc" "${CMAKE_COMMAND}" "-G" "${CMAKE_GENERATOR}" "." 
                              )

  ADD_TEST(Integration-BuddyNewFuncBuild "${CMAKE_COMMAND}"
                              "-E" "chdir" "${TESTS_OUTPUTDATA_PATH}/tests.buddy.newfunc" "${CMAKE_BUILD_TOOL}"
                              )


  ADD_TEST(Integration-BuddyNewFuncCheck "${BIN_OUTPUT_PATH}/openfluid-engine" 
                         "-r"
                         "-p" "${TESTS_OUTPUTDATA_PATH}/tests.buddy.newfunc")
  SET_TESTS_PROPERTIES(Integration-BuddyNewFuncCheck PROPERTIES PASS_REGULAR_EXPRESSION "tests.buddy.newfunc"
                                                                PASS_REGULAR_EXPRESSION "John Doe"
                                                                PASS_REGULAR_EXPRESSION "doe@foo.bar.org")  
                       
  ADD_TEST(Integration-BuddyNewFuncRun "${BIN_OUTPUT_PATH}/openfluid-engine" 
                                       "-i" "${TESTS_DATASETS_PATH}/OPENFLUID.IN.CheckBuddyNewFunc"
                                       "-o" "${TESTS_OUTPUTDATA_PATH}/OPENFLUID.OUT.CheckBuddyNewFunc" 
                                       "-p" "${TESTS_OUTPUTDATA_PATH}/tests.buddy.newfunc"
                                       "-v")



  ADD_TEST(Integration-BuddyConvertHelp "${BIN_OUTPUT_PATH}/openfluid-engine"
                              "--buddyhelp" "convert"
                              )

  ADD_TEST(Integration-BuddyConvert "${BIN_OUTPUT_PATH}/openfluid-engine"
                              "--buddy" "convert"
                              "--buddyopts" "convmode=13_14,inputdir=${TESTS_DATASETS_PATH}/OPENFLUID.IN.V13,outputdir=${TESTS_OUTPUTDATA_PATH}/OPENFLUID.IN.Converted13to14"
                              )

  ADD_TEST(Integration-BuddyConvertCheck "${BIN_OUTPUT_PATH}/openfluid-engine"
                                       "-i" "${TESTS_OUTPUTDATA_PATH}/OPENFLUID.IN.Converted13to14"
                                       "-o" "${TESTS_OUTPUTDATA_PATH}/OPENFLUID.OUT.Converted13to14"
                                       "-p" "${TEST_OUTPUT_PATH}"
                              )


  ADD_TEST(Integration-BuddyNewDatasetHelp "${BIN_OUTPUT_PATH}/openfluid-engine"
                              "--buddyhelp" "newdata"
                              )

  ADD_TEST(Integration-BuddyNewDataset "${BIN_OUTPUT_PATH}/openfluid-engine"
                              "--buddy" "newdata"
                              )



#  ADD_TEST(BasicRunQuiet "${BIN_OUTPUT_PATH}/openfluid-engine" 
#                    "-i${TESTS_DATASETS_PATH}/OPENFLUID.IN.BasicRun"
#                    "-o${TESTS_OUTPUTDATA_PATH}/OPENFLUID.IN.BasicRunQuiet"                     
#                    "-p${FUNCTIONS_OUTPUT_PATH}"
#                    "-q")
    
#  ADD_TEST(BasicRunVerbose "${BIN_OUTPUT_PATH}/openfluid-engine" 
#                    "-i${TESTS_DATASETS_PATH}/OPENFLUID.IN.BasicRun"
#                    "-o${TESTS_OUTPUTDATA_PATH}/OPENFLUID.IN.BasicRunVerbose" 
#                    "-p${FUNCTIONS_OUTPUT_PATH}"
#                    "-v")

#  ADD_TEST(BasicRunProgOut "${BIN_OUTPUT_PATH}/openfluid-engine" 
#                    "-i${TESTS_DATASETS_PATH}/OPENFLUID.IN.BasicRunProgOut"
#                    "-o${TESTS_OUTPUTDATA_PATH}/OPENFLUID.OUT.BasicRunProgOut"                     
#                    "-p${FUNCTIONS_OUTPUT_PATH}")

#  ADD_TEST(BasicRunTrace "${BIN_OUTPUT_PATH}/openfluid-engine" 
#                    "-i${TESTS_DATASETS_PATH}/OPENFLUID.IN.BasicRun"
#                    "-o${TESTS_OUTPUTDATA_PATH}/OPENFLUID.OUT.BasicRunTrace"                     
#                    "-m${TESTS_OUTPUTDATA_PATH}/OPENFLUID.TRACE.BasicRunTrace"
#                    "-p${FUNCTIONS_OUTPUT_PATH}"
#                    "-t")

                    
#  ADD_TEST(BasicRunNoSimreport "${BIN_OUTPUT_PATH}/openfluid-engine"
#                              "-s"
#                              "-i${TESTS_DATASETS_PATH}/OPENFLUID.IN.BasicRun"
#                              "-o${TESTS_OUTPUTDATA_PATH}/OPENFLUID.OUT.BasicRunNoSimreport" 
#                              "-p${FUNCTIONS_OUTPUT_PATH}")

#  ADD_TEST(BasicRunNoSave "${BIN_OUTPUT_PATH}/openfluid-engine" 
#                              "-z"
#                              "-i${TESTS_DATASETS_PATH}/OPENFLUID.IN.BasicRun"
#                              "-o${TESTS_OUTPUTDATA_PATH}/OPENFLUID.OUT.BasicRunNoSave" 
#                              "-p${FUNCTIONS_OUTPUT_PATH}")


