
  ADD_SUBDIRECTORY(src)


  ADD_TEST(DisplayHelp "${BIN_OUTPUT_PATH}/openfluid-engine" 
                       "-h")
  SET_TESTS_PROPERTIES(DisplayHelp PROPERTIES PASS_REGULAR_EXPRESSION "openfluid-engine allowed options")  

  ADD_TEST(DisplayVersion "${BIN_OUTPUT_PATH}/openfluid-engine" 
                       "--version")
  SET_TESTS_PROPERTIES(DisplayVersion PROPERTIES PASS_REGULAR_EXPRESSION ${ENGINE_FULL_VERSION})  
                       
  
  ADD_TEST(FunctionsList "${BIN_OUTPUT_PATH}/openfluid-engine" 
                         "-f"
                         "-p${FUNCTIONS_OUTPUT_PATH}")
  
  ADD_TEST(FunctionsReport "${BIN_OUTPUT_PATH}/openfluid-engine"
                           "-r"
                           "-p${FUNCTIONS_OUTPUT_PATH}")
  
# matching functions reporting test : not working!
#  ADD_TEST(FunctionsMatchReport "${BIN_OUTPUT_PATH}/openfluid-engine"
#                                "-u tests.primitives"
#                                "-p${FUNCTIONS_OUTPUT_PATH}")
#  SET_TESTS_PROPERTIES(FunctionsMatchReport PROPERTIES PASS_REGULAR_EXPRESSION "tests.primitives")  
                           
  
  ADD_TEST(FunctionsXMLReport "${BIN_OUTPUT_PATH}/openfluid-engine"
                              "-x"
                              "-p${FUNCTIONS_OUTPUT_PATH}")
  


  ADD_TEST(CheckLoops "${BIN_OUTPUT_PATH}/openfluid-engine" 
                        "-i${TESTS_DATASETS_PATH}/OPENFLUID.IN.CheckLoops"
                        "-o${TESTS_OUTPUTDATA_PATH}/OPENFLUID.OUT.CheckLoops" 
                        "-p${FUNCTIONS_OUTPUT_PATH}")                              
                              
  ADD_TEST(CheckVector "${BIN_OUTPUT_PATH}/openfluid-engine" 
                              "-i${TESTS_DATASETS_PATH}/OPENFLUID.IN.CheckVector"
                              "-o${TESTS_OUTPUTDATA_PATH}/OPENFLUID.OUT.CheckVector" 
                              "-p${FUNCTIONS_OUTPUT_PATH}")                              
                              
  ADD_TEST(CheckLargeVector "${BIN_OUTPUT_PATH}/openfluid-engine" 
                              "-i${TESTS_DATASETS_PATH}/OPENFLUID.IN.CheckLargeVector"
                              "-o${TESTS_OUTPUTDATA_PATH}/OPENFLUID.OUT.CheckLargeVector" 
                              "-p${FUNCTIONS_OUTPUT_PATH}")                              

  ADD_TEST(CheckPrimitives "${BIN_OUTPUT_PATH}/openfluid-engine" 
                           "-i${TESTS_DATASETS_PATH}/OPENFLUID.IN.CheckPrimitives"
                           "-o${TESTS_OUTPUTDATA_PATH}/OPENFLUID.OUT.CheckPrimitives" 
                           "-p${FUNCTIONS_OUTPUT_PATH}")                              

IF(BUILD_FORTRAN_FUNCTIONS)
  ADD_TEST(CheckFortran "${BIN_OUTPUT_PATH}/openfluid-engine" 
                        "-i${TESTS_DATASETS_PATH}/OPENFLUID.IN.CheckFortran"
                        "-o${TESTS_OUTPUTDATA_PATH}/OPENFLUID.OUT.CheckFortran" 
                        "-p${FUNCTIONS_OUTPUT_PATH}")                              
ENDIF(BUILD_FORTRAN_FUNCTIONS)


  ADD_TEST(CheckEvents "${BIN_OUTPUT_PATH}/openfluid-engine" 
                        "-i${TESTS_DATASETS_PATH}/OPENFLUID.IN.CheckEvents"
                        "-o${TESTS_OUTPUTDATA_PATH}/OPENFLUID.OUT.CheckEvents" 
                        "-p${FUNCTIONS_OUTPUT_PATH}")                              

  ADD_TEST(CheckMessages "${BIN_OUTPUT_PATH}/openfluid-engine" 
                         "-i${TESTS_DATASETS_PATH}/OPENFLUID.IN.CheckMessages"
                         "-o${TESTS_OUTPUTDATA_PATH}/OPENFLUID.OUT.CheckMessages" 
                         "-p${FUNCTIONS_OUTPUT_PATH}")                              

  ADD_TEST(CheckManyMessages "${BIN_OUTPUT_PATH}/openfluid-engine" 
                             "-i${TESTS_DATASETS_PATH}/OPENFLUID.IN.CheckManyMessages"
                             "-o${TESTS_OUTPUTDATA_PATH}/OPENFLUID.OUT.CheckManyMessages" 
                             "-p${FUNCTIONS_OUTPUT_PATH}"
                             "-z")                              

  ADD_TEST(CheckTools "${BIN_OUTPUT_PATH}/openfluid-engine" 
                        "-i${TESTS_DATASETS_PATH}/OPENFLUID.IN.CheckTools"
                        "-o${TESTS_OUTPUTDATA_PATH}/OPENFLUID.OUT.CheckTools" 
                        "-p${FUNCTIONS_OUTPUT_PATH}")

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


  ADD_TEST(OFEFunc2Doc "${BIN_OUTPUT_PATH}/ofefunc2doc" 
                       "${CMAKE_SOURCE_DIR}/src/functions/tests/tests.ofefunc2doc/OFEFunc2DocFunc.cpp"
                       "-t${CMAKE_SOURCE_DIR}/resources/ofefunc2doc/ofefunc2doc_tpl.tex"
                       "-o${TESTS_OUTPUTDATA_PATH}"
                       "-p")                              


  ADD_TEST(CheckBuddyFunc2DocHelp "${BIN_OUTPUT_PATH}/openfluid-engine"
                              "--buddyhelp" "func2doc"
                              )

  ADD_TEST(CheckBuddyFunc2Doc "${BIN_OUTPUT_PATH}/openfluid-engine"
                              "--buddy" "func2doc"
                              "--buddyopts" "inputcpp=${CMAKE_SOURCE_DIR}/src/functions/tests/tests.ofefunc2doc/OFEFunc2DocFunc.cpp,outputdir=${TESTS_OUTPUTDATA_PATH},pdf=1,html=1,tplfile=${CMAKE_SOURCE_DIR}/resources/ofefunc2doc/ofefunc2doc_tpl.tex"
                              )
                           