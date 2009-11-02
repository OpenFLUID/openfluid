
  FILE(MAKE_DIRECTORY "${TESTS_OUTPUTDATA_PATH}/examples")
    
  ADD_TEST(Examples-Configure "${CMAKE_COMMAND}"
                              "-E" "chdir" "${TESTS_OUTPUTDATA_PATH}/examples"
                              "${CMAKE_COMMAND}" 
                              "-G" "${CMAKE_GENERATOR}"
                              "-Dofelib_FOUND=1"
                              "-Dofelib_LIBRARY_DIRS=${LIB_OUTPUT_PATH}"
                              "-Dofelib_LIBRARIES=-L${LIB_OUTPUT_PATH} -lofelib"
                              "-Dofelib_INCLUDE_DIRS=${CMAKE_SOURCE_DIR}/include"
                              "-DINSTALL_PATH=${TEST_OUTPUT_PATH}"                              
                              "${CMAKE_SOURCE_DIR}/resources/examples/src" 
                              )

  ADD_TEST(Examples-Build "${CMAKE_COMMAND}"
                              "-E" "chdir" "${TESTS_OUTPUTDATA_PATH}/examples" "${CMAKE_BUILD_TOOL}" 
                              )

  ADD_TEST(Examples-Install "${CMAKE_COMMAND}"
                              "-E" "chdir" "${TESTS_OUTPUTDATA_PATH}/examples" "${CMAKE_BUILD_TOOL}" "install"
                              )
                              
  ADD_TEST(Examples-RunPrimitives "${BIN_OUTPUT_PATH}/openfluid-engine" 
                              "-i" "${EXAMPLES_DATASETS_PATH}/OPENFLUID.IN.PrimitivesExample"
                              "-o" "${TESTS_OUTPUTDATA_PATH}/OPENFLUID.OUT.PrimitivesExample" 
                              "-p" "${TEST_OUTPUT_PATH}"
                              )                              
