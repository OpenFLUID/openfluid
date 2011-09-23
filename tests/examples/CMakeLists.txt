
  FILE(MAKE_DIRECTORY "${TESTS_OUTPUTDATA_PATH}/examples")
    
  ADD_TEST(examples-Configure "${CMAKE_COMMAND}"
                              "-E" "chdir" "${TESTS_OUTPUTDATA_PATH}/examples"
                              "${CMAKE_COMMAND}" 
                              "-G" "${CMAKE_GENERATOR}"
                              "-Dopenfluid_FOUND=1"
                              "-Dopenfluid_LIBRARY_DIRS=${LIB_OUTPUT_PATH}"
                              "-Dopenfluid_LIBRARIES=-L${LIB_OUTPUT_PATH} -lopenfluid-base -lopenfluid-core -lopenfluid-tools -pthread -lglibmm-2.4 -lgobject-2.0 -lsigc-2.0 -lgthread-2.0 -lrt -lglib-2.0"
                              #"-Dopenfluid_INCLUDE_DIRS=${CMAKE_SOURCE_DIR}/src;${GLIBMM_INCLUDE_DIRS}"
                              "-Dopenfluid_INCLUDE_DIRS=${PROJECT_SOURCE_DIR}/src;${PROJECT_BINARY_DIR}/src;${GLIBMM_INCLUDE_DIRS}"
                              "-Dopenfluid_VERSION=${FULL_VERSION}"
                              "-DINSTALL_PATH=${TEST_OUTPUT_PATH}"                              
                              "${CMAKE_SOURCE_DIR}/resources/examples/src" 
                              )

  ADD_TEST(examples-Build "${CMAKE_COMMAND}"
                              "-E" "chdir" "${TESTS_OUTPUTDATA_PATH}/examples" "${CMAKE_BUILD_TOOL}" 
                              )

  ADD_TEST(examples-Install "${CMAKE_COMMAND}"
                              "-E" "chdir" "${TESTS_OUTPUTDATA_PATH}/examples" "${CMAKE_BUILD_TOOL}" "install"
                              )

                              
  ADD_TEST(examples-RunPrimitives "${BIN_OUTPUT_PATH}/openfluid-engine" 
                              "-i" "${EXAMPLES_DATASETS_PATH}/OPENFLUID.IN.PrimitivesExample"
                              "-o" "${TESTS_OUTPUTDATA_PATH}/OPENFLUID.OUT.PrimitivesExample" 
                              "-p" "${TEST_OUTPUT_PATH}"
                              )                              
