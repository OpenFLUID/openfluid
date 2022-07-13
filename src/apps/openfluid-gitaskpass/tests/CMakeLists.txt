
OPENFLUID_ADD_TEST(NAME cmdline-gitaskpass-Run
                   COMMAND "${OFBUILD_DIST_BIN_DIR}/${OPENFLUID_GITASKPASS_APP}")
SET_TESTS_PROPERTIES(cmdline-gitaskpass-Run PROPERTIES ENVIRONMENT "${OFBUILD_GITASKPASS_ENVVAR}=mysecrtpasswrd")                           
SET_TESTS_PROPERTIES(cmdline-gitaskpass-Run PROPERTIES PASS_REGULAR_EXPRESSION "mysecrtpasswrd")
