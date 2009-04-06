
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
                    "-o${TESTS_OUTPUTDATA_PATH}/OPENFLUID.OUT.BasicRun"                     
                    "-p${FUNCTIONS_OUTPUT_PATH}")

  ADD_TEST(BasicRunQuiet "${EXECUTABLE_OUTPUT_PATH}/openfluid-engine" 
                    "-i${TESTS_DATASETS_PATH}/OPENFLUID.IN.BasicRun"
                    "-o${TESTS_OUTPUTDATA_PATH}/OPENFLUID.IN.BasicRunQuiet"                     
                    "-p${FUNCTIONS_OUTPUT_PATH}"
                    "-q")
    
  ADD_TEST(BasicRunVerbose "${EXECUTABLE_OUTPUT_PATH}/openfluid-engine" 
                    "-i${TESTS_DATASETS_PATH}/OPENFLUID.IN.BasicRun"
                    "-o${TESTS_OUTPUTDATA_PATH}/OPENFLUID.IN.BasicRunVerbose" 
                    "-p${FUNCTIONS_OUTPUT_PATH}"
                    "-v")

  ADD_TEST(BasicRunProgOut "${EXECUTABLE_OUTPUT_PATH}/openfluid-engine" 
                    "-i${TESTS_DATASETS_PATH}/OPENFLUID.IN.BasicRunProgOut"
                    "-o${TESTS_OUTPUTDATA_PATH}/OPENFLUID.OUT.BasicRunProgOut"                     
                    "-p${FUNCTIONS_OUTPUT_PATH}")

#  ADD_TEST(BasicRunTrace "${EXECUTABLE_OUTPUT_PATH}/openfluid-engine" 
#                    "-i${TESTS_DATASETS_PATH}/OPENFLUID.IN.BasicRun"
#                    "-o${TESTS_OUTPUTDATA_PATH}/OPENFLUID.OUT.BasicRunTrace"                     
#                    "-m${TESTS_OUTPUTDATA_PATH}/OPENFLUID.TRACE.BasicRunTrace"
#                    "-p${FUNCTIONS_OUTPUT_PATH}"
#                    "-t")

                    
  ADD_TEST(BasicRunNoSimreport "${EXECUTABLE_OUTPUT_PATH}/openfluid-engine"
                              "-s"
                              "-i${TESTS_DATASETS_PATH}/OPENFLUID.IN.BasicRun"
                              "-o${TESTS_OUTPUTDATA_PATH}/OPENFLUID.OUT.BasicRunNoSimreport" 
                              "-p${FUNCTIONS_OUTPUT_PATH}")

  ADD_TEST(BasicRunNoSave "${EXECUTABLE_OUTPUT_PATH}/openfluid-engine" 
                              "-z"
                              "-i${TESTS_DATASETS_PATH}/OPENFLUID.IN.BasicRun"
                              "-o${TESTS_OUTPUTDATA_PATH}/OPENFLUID.OUT.BasicRunNoSave" 
                              "-p${FUNCTIONS_OUTPUT_PATH}")
                              
  ADD_TEST(CheckVector "${EXECUTABLE_OUTPUT_PATH}/openfluid-engine" 
                              "-i${TESTS_DATASETS_PATH}/OPENFLUID.IN.CheckVector"
                              "-o${TESTS_OUTPUTDATA_PATH}/OPENFLUID.OUT.CheckVector" 
                              "-p${FUNCTIONS_OUTPUT_PATH}")                              
                              
  ADD_TEST(CheckLargeVector "${EXECUTABLE_OUTPUT_PATH}/openfluid-engine" 
                              "-i${TESTS_DATASETS_PATH}/OPENFLUID.IN.CheckLargeVector"
                              "-o${TESTS_OUTPUTDATA_PATH}/OPENFLUID.OUT.CheckLargeVector" 
                              "-p${FUNCTIONS_OUTPUT_PATH}")                              

  ADD_TEST(CheckPrimitives "${EXECUTABLE_OUTPUT_PATH}/openfluid-engine" 
                           "-i${TESTS_DATASETS_PATH}/OPENFLUID.IN.CheckPrimitives"
                           "-o${TESTS_OUTPUTDATA_PATH}/OPENFLUID.OUT.CheckPrimitives" 
                           "-p${FUNCTIONS_OUTPUT_PATH}")                              

  ADD_TEST(CheckFortran "${EXECUTABLE_OUTPUT_PATH}/openfluid-engine" 
                        "-i${TESTS_DATASETS_PATH}/OPENFLUID.IN.CheckFortran"
                        "-o${TESTS_OUTPUTDATA_PATH}/OPENFLUID.OUT.CheckFortran" 
                        "-p${FUNCTIONS_OUTPUT_PATH}")                              
                           