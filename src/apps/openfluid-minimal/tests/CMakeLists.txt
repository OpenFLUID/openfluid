
OPENFLUID_ADD_TEST(NAME cmdline-minimal-Run
                    COMMAND "${OFBUILD_DIST_BIN_DIR}/openfluid-minimal"
                                "${OFBUILD_TESTS_INPUT_DATASETS_DIR}/OPENFLUID.IN.PrimitivesHopla"
                                "${OFBUILD_TESTS_OUTPUT_DATA_DIR}/OPENFLUID.OUT.MinimalRun"
                                "${OFBUILD_TESTS_BINARY_DIR}"
                            )


###########################################################################


OPENFLUID_ADD_TEST(NAME cmdline-minimal-FailsArgs
                    COMMAND "${OFBUILD_DIST_BIN_DIR}/openfluid-minimal"
                                "${OFBUILD_TESTS_INPUT_DATASETS_DIR}/OPENFLUID.IN.VariableTimeProd"
                                "${OFBUILD_TESTS_OUTPUT_DATA_DIR}/OPENFLUID.OUT.MinimalFailsArgs"
                            )
SET_TESTS_PROPERTIES(cmdline-minimal-FailsArgs PROPERTIES WILL_FAIL TRUE)

###########################################################################


OPENFLUID_ADD_TEST(NAME cmdline-minimal-FailsInput
                  COMMAND "${OFBUILD_DIST_BIN_DIR}/openfluid-minimal"
                                "${OFBUILD_TESTS_INPUT_DATASETS_DIR}/OPENFLUID.IN.doesnotexist"
                                "${OFBUILD_TESTS_OUTPUT_DATA_DIR}/OPENFLUID.OUT.MinimalFailsInput"
                                "${OFBUILD_TESTS_BINARY_DIR}"
                            )                              
SET_TESTS_PROPERTIES(cmdline-minimal-FailsInput PROPERTIES WILL_FAIL TRUE)

 
