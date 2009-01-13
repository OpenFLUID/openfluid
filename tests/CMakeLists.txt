
  ADD_TEST(DisplayHelp "${EXECUTABLE_OUTPUT_PATH}/openfluid-engine" 
                       "-h")
  SET_TESTS_PROPERTIES(DisplayHelp PROPERTIES PASS_REGULAR_EXPRESSION "Usage: openfluid-engine")  

  ADD_TEST(DisplayVersion "${EXECUTABLE_OUTPUT_PATH}/openfluid-engine" 
                       "--version")
  SET_TESTS_PROPERTIES(DisplayVersion PROPERTIES PASS_REGULAR_EXPRESSION ${ENGINE_FULL_VERSION})  
                       
  
  ADD_TEST(FunctionsList "${EXECUTABLE_OUTPUT_PATH}/openfluid-engine" 
                         "-f"
                         "-p${FUNCTIONS_OUTPUT_PATH}")
  
  ADD_TEST(FunctionsReport "${EXECUTABLE_OUTPUT_PATH}/openfluid-engine"
                           "-r"
                           "-p${FUNCTIONS_OUTPUT_PATH}")
  
  ADD_TEST(FunctionsXMLReport "${EXECUTABLE_OUTPUT_PATH}/openfluid-engine"
                              "-x"
                              "-p${FUNCTIONS_OUTPUT_PATH}")
  
  ADD_TEST(BasicRun "${EXECUTABLE_OUTPUT_PATH}/openfluid-engine" 
                    "-i${TESTS_DATASETS_PATH}/OPENFLUID.IN.BasicRun" 
                    "-p${FUNCTIONS_OUTPUT_PATH}")

  ADD_TEST(BasicRunQuiet "${EXECUTABLE_OUTPUT_PATH}/openfluid-engine" 
                    "-i${TESTS_DATASETS_PATH}/OPENFLUID.IN.BasicRun" 
                    "-p${FUNCTIONS_OUTPUT_PATH}"
                    "-q")
    
  ADD_TEST(BasicRunVerbose "${EXECUTABLE_OUTPUT_PATH}/openfluid-engine" 
                    "-i${TESTS_DATASETS_PATH}/OPENFLUID.IN.BasicRun" 
                    "-p${FUNCTIONS_OUTPUT_PATH}"
                    "-v")

  ADD_TEST(BasicRunTrace "${EXECUTABLE_OUTPUT_PATH}/openfluid-engine" 
                    "-i${TESTS_DATASETS_PATH}/OPENFLUID.IN.BasicRun" 
                    "-p${FUNCTIONS_OUTPUT_PATH}"
                    "-t")

                    
  ADD_TEST(BasicRunWithNoSimreport "${EXECUTABLE_OUTPUT_PATH}/openfluid-engine"
                              "-s"
                              "-i${TESTS_DATASETS_PATH}/OPENFLUID.IN.BasicRun" 
                              "-p${FUNCTIONS_OUTPUT_PATH}")

  ADD_TEST(BasicRunWithNoSave "${EXECUTABLE_OUTPUT_PATH}/openfluid-engine" 
                         "-z"
                         "-i${TESTS_DATASETS_PATH}/OPENFLUID.IN.BasicRun" 
                         "-p${FUNCTIONS_OUTPUT_PATH}")

