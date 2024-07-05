
OPENFLUID_ADD_TEST(NAME cmdline-openfluid-Help
                   COMMAND "${OFBUILD_DIST_BIN_DIR}/${OPENFLUID_CMD_APP}" "-h")
SET_TESTS_PROPERTIES(cmdline-openfluid-Help
                     PROPERTIES PASS_REGULAR_EXPRESSION "openfluid \\[<command>\\] \\[<options>\\] \\[<args>\\]")


OPENFLUID_ADD_TEST(NAME cmdline-openfluid-HelpWhenNothing
                   COMMAND "${OFBUILD_DIST_BIN_DIR}/${OPENFLUID_CMD_APP}")
SET_TESTS_PROPERTIES(cmdline-openfluid-HelpWhenNothing
                     PROPERTIES PASS_REGULAR_EXPRESSION "openfluid \\[<command>\\] \\[<options>\\] \\[<args>\\]")


###########################################################################


OPENFLUID_ADD_TEST(NAME cmdline-openfluid-Version
                   COMMAND "${OFBUILD_DIST_BIN_DIR}/${OPENFLUID_CMD_APP}" "version")
SET_TESTS_PROPERTIES(cmdline-openfluid-Version PROPERTIES PASS_REGULAR_EXPRESSION ${OPENFLUID_VERSION_FULL})


OPENFLUID_ADD_TEST(NAME cmdline-openfluid-VersionNumeric
                   COMMAND "${OFBUILD_DIST_BIN_DIR}/${OPENFLUID_CMD_APP}" "version" "-n")
SET_TESTS_PROPERTIES(cmdline-openfluid-VersionNumeric PROPERTIES PASS_REGULAR_EXPRESSION ${OPENFLUID_VERSION_NUMERIC})


OPENFLUID_ADD_TEST(NAME cmdline-openfluid-VersionAsOption
                   COMMAND "${OFBUILD_DIST_BIN_DIR}/${OPENFLUID_CMD_APP}" "--version")
SET_TESTS_PROPERTIES(cmdline-openfluid-VersionAsOption PROPERTIES PASS_REGULAR_EXPRESSION ${OPENFLUID_VERSION_FULL})


###########################################################################


OPENFLUID_ADD_TEST(NAME cmdline-openfluid-Info
                   COMMAND "${OFBUILD_DIST_BIN_DIR}/${OPENFLUID_CMD_APP}" "info")
SET_TESTS_PROPERTIES(cmdline-openfluid-Info PROPERTIES FAIL_REGULAR_EXPRESSION "foobar")
SET_TESTS_PROPERTIES(cmdline-openfluid-Info PROPERTIES FAIL_REGULAR_EXPRESSION "barfoo")
SET_TESTS_PROPERTIES(cmdline-openfluid-Info PROPERTIES PASS_REGULAR_EXPRESSION "${OFBUILD_TESTS_USERDATA_DIR}")
SET_TESTS_PROPERTIES(cmdline-openfluid-Info PROPERTIES PASS_REGULAR_EXPRESSION "${OFBUILD_TESTS_TEMP_DIR}")


###########################################################################


OPENFLUID_ADD_TEST(NAME cmdline-openfluid-ReportSimulatorsList
                   COMMAND "${OFBUILD_DIST_BIN_DIR}/${OPENFLUID_CMD_APP}" "report" "simulators" "--list"
                           "-p" "${OFBUILD_TESTS_BINARY_DIR}")
SET_TESTS_PROPERTIES(cmdline-openfluid-ReportSimulatorsList PROPERTIES PASS_REGULAR_EXPRESSION "tests.tools")
SET_TESTS_PROPERTIES(cmdline-openfluid-ReportSimulatorsList PROPERTIES FAIL_REGULAR_EXPRESSION "tests.empty")


OPENFLUID_ADD_TEST(NAME cmdline-openfluid-ReportSimulators
                   COMMAND "${OFBUILD_DIST_BIN_DIR}/${OPENFLUID_CMD_APP}"
                           "report" "simulators"
                           "-p" "${OFBUILD_TESTS_BINARY_DIR}")
SET_TESTS_PROPERTIES(cmdline-openfluid-ReportSimulators PROPERTIES PASS_REGULAR_EXPRESSION "tests.tools")
SET_TESTS_PROPERTIES(cmdline-openfluid-ReportSimulators PROPERTIES FAIL_REGULAR_EXPRESSION "tests.empty")


OPENFLUID_ADD_TEST(NAME cmdline-openfluid-ReportSimulatorsListJSON
                   COMMAND "${OFBUILD_DIST_BIN_DIR}/${OPENFLUID_CMD_APP}"
                           "report" "simulators" "--list" "--format=json"
                           "-p" "${OFBUILD_TESTS_BINARY_DIR}")
SET_TESTS_PROPERTIES(cmdline-openfluid-ReportSimulatorsListJSON PROPERTIES PASS_REGULAR_EXPRESSION "tests.tools")
SET_TESTS_PROPERTIES(cmdline-openfluid-ReportSimulatorsListJSON PROPERTIES FAIL_REGULAR_EXPRESSION "tests.empty")


OPENFLUID_ADD_TEST(NAME cmdline-openfluid-ReportSimulatorsJSON
                   COMMAND "${OFBUILD_DIST_BIN_DIR}/${OPENFLUID_CMD_APP}"
                           "report" "simulators" "--format=json"
                           "-p" "${OFBUILD_TESTS_BINARY_DIR}")
SET_TESTS_PROPERTIES(cmdline-openfluid-ReportSimulatorsJSON PROPERTIES PASS_REGULAR_EXPRESSION "tests.tools")
SET_TESTS_PROPERTIES(cmdline-openfluid-ReportSimulatorsJSON PROPERTIES FAIL_REGULAR_EXPRESSION "tests.empty")


OPENFLUID_ADD_TEST(NAME cmdline-openfluid-ReportObserversList
                   COMMAND "${OFBUILD_DIST_BIN_DIR}/${OPENFLUID_CMD_APP}"
                           "report" "observers" "--list"
                           "-n" "${OFBUILD_TESTS_BINARY_DIR}")
SET_TESTS_PROPERTIES(cmdline-openfluid-ReportObserversList PROPERTIES PASS_REGULAR_EXPRESSION "tests.empty")
SET_TESTS_PROPERTIES(cmdline-openfluid-ReportObserversList PROPERTIES FAIL_REGULAR_EXPRESSION "tests.tools")


OPENFLUID_ADD_TEST(NAME cmdline-openfluid-ReportObservers
                   COMMAND "${OFBUILD_DIST_BIN_DIR}/${OPENFLUID_CMD_APP}"
                           "report" "observers"
                           "-n" "${OFBUILD_TESTS_BINARY_DIR}")
SET_TESTS_PROPERTIES(cmdline-openfluid-ReportObservers PROPERTIES PASS_REGULAR_EXPRESSION "tests.empty")
SET_TESTS_PROPERTIES(cmdline-openfluid-ReportObservers PROPERTIES FAIL_REGULAR_EXPRESSION "tests.tools")


OPENFLUID_ADD_TEST(NAME cmdline-openfluid-ReportObserversListJSON
                   COMMAND "${OFBUILD_DIST_BIN_DIR}/${OPENFLUID_CMD_APP}"
                           "report" "observers" "--list" "--format=json"
                           "-n" "${OFBUILD_TESTS_BINARY_DIR}")
SET_TESTS_PROPERTIES(cmdline-openfluid-ReportObserversListJSON PROPERTIES PASS_REGULAR_EXPRESSION "tests.empty")
SET_TESTS_PROPERTIES(cmdline-openfluid-ReportObserversListJSON PROPERTIES FAIL_REGULAR_EXPRESSION "tests.tools")


OPENFLUID_ADD_TEST(NAME cmdline-openfluid-ReportObserversReport
                   COMMAND "${OFBUILD_DIST_BIN_DIR}/${OPENFLUID_CMD_APP}"
                           "report" "observers" "--format=json"
                           "-n" "${OFBUILD_TESTS_BINARY_DIR}")
SET_TESTS_PROPERTIES(cmdline-openfluid-ReportObserversReport PROPERTIES PASS_REGULAR_EXPRESSION "tests.empty")
SET_TESTS_PROPERTIES(cmdline-openfluid-ReportObserversReport PROPERTIES FAIL_REGULAR_EXPRESSION "tests.tools")


OPENFLUID_ADD_TEST(NAME cmdline-openfluid-ReportSimulatorsReport
                   COMMAND "${OFBUILD_DIST_BIN_DIR}/${OPENFLUID_CMD_APP}"
                           "report"
                           "simulators")
SET_TESTS_PROPERTIES(cmdline-openfluid-ReportSimulatorsReport
                     PROPERTIES ENVIRONMENT "OPENFLUID_SIMS_PATH=${OFBUILD_TESTS_BINARY_DIR};OPENFLUID_TEMP_PATH=${OFBUILD_TESTS_TEMP_DIR};OPENFLUID_USERDATA_PATH=${OFBUILD_TESTS_USERDATA_DIR}")
SET_TESTS_PROPERTIES(cmdline-openfluid-ReportSimulatorsReport PROPERTIES PASS_REGULAR_EXPRESSION "tests.primitives.variables.use")
SET_TESTS_PROPERTIES(cmdline-openfluid-ReportSimulatorsReport PROPERTIES PASS_REGULAR_EXPRESSION "tests.primitives.variables.prod")


OPENFLUID_ADD_TEST(NAME cmdline-openfluid-ReportSimulatorsNoEnvVar
                   COMMAND "${OFBUILD_DIST_BIN_DIR}/${OPENFLUID_CMD_APP}"
                           "report" "simulators")
SET_TESTS_PROPERTIES(cmdline-openfluid-ReportSimulatorsNoEnvVar PROPERTIES PASS_REGULAR_EXPRESSION "tests.primitives.variables.use")
SET_TESTS_PROPERTIES(cmdline-openfluid-ReportSimulatorsNoEnvVar PROPERTIES PASS_REGULAR_EXPRESSION "tests.primitives.variables.prod")
SET_TESTS_PROPERTIES(cmdline-openfluid-ReportSimulatorsNoEnvVar PROPERTIES WILL_FAIL TRUE)


OPENFLUID_ADD_TEST(NAME cmdline-openfluid-ReportObserversEnvVar
                   COMMAND "${OFBUILD_DIST_BIN_DIR}/${OPENFLUID_CMD_APP}"
                           "report" "observers")
SET_TESTS_PROPERTIES(cmdline-openfluid-ReportObserversEnvVar
                     PROPERTIES ENVIRONMENT "OPENFLUID_OBSS_PATH=${OFBUILD_TESTS_BINARY_DIR};OPENFLUID_TEMP_PATH=${OFBUILD_TESTS_TEMP_DIR};OPENFLUID_USERDATA_PATH=${OFBUILD_TESTS_USERDATA_DIR}")
SET_TESTS_PROPERTIES(cmdline-openfluid-ReportObserversEnvVar PROPERTIES PASS_REGULAR_EXPRESSION "tests.empty")


OPENFLUID_ADD_TEST(NAME cmdline-openfluid-ReportObserversNoEnvVar
                   COMMAND "${OFBUILD_DIST_BIN_DIR}/${OPENFLUID_CMD_APP}"
                           "report" "observers")
SET_TESTS_PROPERTIES(cmdline-openfluid-ReportObserversNoEnvVar  PROPERTIES PASS_REGULAR_EXPRESSION "tests.empty")
SET_TESTS_PROPERTIES(cmdline-openfluid-ReportObserversNoEnvVar  PROPERTIES WILL_FAIL TRUE)


###########################################################################


OPENFLUID_ADD_TEST(NAME cmdline-openfluid-RunVerbose
                   COMMAND "${OFBUILD_DIST_BIN_DIR}/${OPENFLUID_CMD_APP}"
                           "run"
                           "${OFBUILD_TESTS_INPUT_DATASETS_DIR}/OPENFLUID.IN.Coupling"
                           "${OFBUILD_TESTS_OUTPUT_DATA_DIR}/OPENFLUID.OUT.Verbose"
                           "-p" "${OFBUILD_TESTS_BINARY_DIR}"
                           "-v")


OPENFLUID_ADD_TEST(NAME cmdline-openfluid-RunQuiet
                   COMMAND "${OFBUILD_DIST_BIN_DIR}/${OPENFLUID_CMD_APP}"
                           "run"
                           "${OFBUILD_TESTS_INPUT_DATASETS_DIR}/OPENFLUID.IN.Coupling"
                           "${OFBUILD_TESTS_OUTPUT_DATA_DIR}/OPENFLUID.OUT.Quiet"
                           "-p" "${OFBUILD_TESTS_BINARY_DIR}"
                           "-q")


OPENFLUID_ADD_TEST(NAME cmdline-openfluid-RunInputDoesNotExist
                   COMMAND "${OFBUILD_DIST_BIN_DIR}/${OPENFLUID_CMD_APP}"
                           "run" "${CMAKE_BINARY_DIR}/path/does/not/exist")
SET_TESTS_PROPERTIES(cmdline-openfluid-RunInputDoesNotExist PROPERTIES WILL_FAIL TRUE)


OPENFLUID_ADD_TEST(NAME cmdline-openfluid-RunInputWithNoModel
                   COMMAND "${OFBUILD_DIST_BIN_DIR}/${OPENFLUID_CMD_APP}"
                           "run"
                           "${OFBUILD_TESTS_INPUT_DATASETS_DIR}/OPENFLUID.IN.NoModel"
                           "${OFBUILD_TESTS_OUTPUT_DATA_DIR}/OPENFLUID.OUT.NoModel"
                           "-p" "${OFBUILD_TESTS_BINARY_DIR}"
 PRE_TEST REMOVE_DIRECTORY "${OFBUILD_TESTS_OUTPUT_DATA_DIR}/OPENFLUID.OUT.NoModel"
)
SET_TESTS_PROPERTIES(cmdline-openfluid-RunInputWithNoModel PROPERTIES WILL_FAIL TRUE)


OPENFLUID_ADD_TEST(NAME cmdline-openfluid-RunInputWithNoRunconfig
                   COMMAND "${OFBUILD_DIST_BIN_DIR}/${OPENFLUID_CMD_APP}"
                           "run"
                           "${OFBUILD_TESTS_INPUT_DATASETS_DIR}/OPENFLUID.IN.NoRun"
                           "${OFBUILD_TESTS_OUTPUT_DATA_DIR}/OPENFLUID.OUT.NoRun"
                           "-p" "${OFBUILD_TESTS_BINARY_DIR}")
SET_TESTS_PROPERTIES(cmdline-openfluid-RunInputWithNoRunconfig PROPERTIES WILL_FAIL TRUE)


###########################################################################


OPENFLUID_ADD_TEST(NAME cmdline-openfluid-InstallExamplesHelp
                   COMMAND "${OFBUILD_DIST_BIN_DIR}/${OPENFLUID_CMD_APP}"
                           "install-examples" "--help"
                  )


OPENFLUID_ADD_TEST(NAME cmdline-openfluid-InstallExamplesSimulatorsSelection
                   COMMAND "${OFBUILD_DIST_BIN_DIR}/${OPENFLUID_CMD_APP}"
                           "install-examples" "--simulators-only=fire.surf.prod-spread,water.atm-surf.rain-su.files"
                           "--resources-path=${OFBUILD_SOURCE_EXAMPLES_DIR}" "--install-path=${OFBUILD_TESTS_OUTPUT_DATA_DIR}/cmdline/install-examples/simsel" "-f"
                   PRE_TEST REMOVE_DIRECTORY "${OFBUILD_TESTS_OUTPUT_DATA_DIR}/cmdline/install-examples/simsel"
                   POST_TEST CHECK_FILE_EXIST "${OFBUILD_TESTS_OUTPUT_DATA_DIR}/cmdline/install-examples/simsel/wares-dev/simulators/fire.surf.prod-spread/src/WareMain.cpp"
                   POST_TEST CHECK_FILE_EXIST "${OFBUILD_TESTS_OUTPUT_DATA_DIR}/cmdline/install-examples/simsel/wares-dev/simulators/water.atm-surf.rain-su.files/CMakeLists.txt"
                  )


OPENFLUID_ADD_TEST(NAME cmdline-openfluid-InstallExamplesSimulatorsAll
                   COMMAND "${OFBUILD_DIST_BIN_DIR}/${OPENFLUID_CMD_APP}"
                           "install-examples" "--simulators-only"
                           "--resources-path=${OFBUILD_SOURCE_EXAMPLES_DIR}" "--install-path=${OFBUILD_TESTS_OUTPUT_DATA_DIR}/cmdline/install-examples/simall" "-f"
                   PRE_TEST REMOVE_DIRECTORY "${OFBUILD_TESTS_OUTPUT_DATA_DIR}/cmdline/install-examples/simall"
                   POST_TEST CHECK_FILE_EXIST "${OFBUILD_TESTS_OUTPUT_DATA_DIR}/cmdline/install-examples/simall/wares-dev/simulators/examples.primitives.unitsA.prod/src/WareMain.cpp"
                  )


OPENFLUID_ADD_TEST(NAME cmdline-openfluid-InstallExamplesSimulatorMissing
                   COMMAND "${OFBUILD_DIST_BIN_DIR}/${OPENFLUID_CMD_APP}"
                           "install-examples" "--simulators-only=fake.name"
                           "--resources-path=${OFBUILD_SOURCE_EXAMPLES_DIR}" "--install-path=${OFBUILD_TESTS_OUTPUT_DATA_DIR}/cmdline/install-examples/simmis" "-f"
                   PRE_TEST REMOVE_DIRECTORY "${OFBUILD_TESTS_OUTPUT_DATA_DIR}/cmdline/install-examples/simmis"
                  )


OPENFLUID_ADD_TEST(NAME cmdline-openfluid-InstallExamplesProjectsSelection
                   COMMAND "${OFBUILD_DIST_BIN_DIR}/${OPENFLUID_CMD_APP}"
                           "install-examples" "--projects-only=Firespread,Primitives"
                           "--resources-path=${OFBUILD_SOURCE_EXAMPLES_DIR}" "--install-path=${OFBUILD_TESTS_OUTPUT_DATA_DIR}/cmdline/install-examples/prjsel" "-f"
                   PRE_TEST REMOVE_DIRECTORY "${OFBUILD_TESTS_OUTPUT_DATA_DIR}/cmdline/install-examples/prjsel"
                   POST_TEST CHECK_FILE_EXIST "${OFBUILD_TESTS_OUTPUT_DATA_DIR}/cmdline/install-examples/prjsel/projects/Firespread/IN/model.fluidx"
                   POST_TEST CHECK_FILE_EXIST "${OFBUILD_TESTS_OUTPUT_DATA_DIR}/cmdline/install-examples/prjsel/projects/Primitives/IN/model.fluidx"
                  )


OPENFLUID_ADD_TEST(NAME cmdline-openfluid-InstallExamplesProjectsAll
                   COMMAND "${OFBUILD_DIST_BIN_DIR}/${OPENFLUID_CMD_APP}"
                           "install-examples" "--projects-only"
                           "--resources-path=${OFBUILD_SOURCE_EXAMPLES_DIR}" "--install-path=${OFBUILD_TESTS_OUTPUT_DATA_DIR}/cmdline/install-examples/prjall" "-f"
                   PRE_TEST REMOVE_DIRECTORY "${OFBUILD_TESTS_OUTPUT_DATA_DIR}/cmdline/install-examples/prjall"
                   POST_TEST CHECK_FILE_EXIST "${OFBUILD_TESTS_OUTPUT_DATA_DIR}/cmdline/install-examples/prjall/projects/Manhattan/IN/model.fluidx"
                  )


OPENFLUID_ADD_TEST(NAME cmdline-openfluid-InstallExamplesAll
                   COMMAND "${OFBUILD_DIST_BIN_DIR}/${OPENFLUID_CMD_APP}"
                           "install-examples"
                           "--resources-path=${OFBUILD_SOURCE_EXAMPLES_DIR}" "--install-path=${OFBUILD_TESTS_OUTPUT_DATA_DIR}/cmdline/install-examples/all" "-f"
                   PRE_TEST REMOVE_DIRECTORY "${OFBUILD_TESTS_OUTPUT_DATA_DIR}/cmdline/install-examples/all"
                  )
SET_PROPERTY(TEST cmdline-openfluid-InstallExamplesAll APPEND PROPERTY DEPENDS cmdline-openfluid-InstallExamplesSimulatorsSelection)
SET_PROPERTY(TEST cmdline-openfluid-InstallExamplesAll APPEND PROPERTY DEPENDS cmdline-openfluid-InstallExamplesSimulatorsAll)
SET_PROPERTY(TEST cmdline-openfluid-InstallExamplesAll APPEND PROPERTY DEPENDS cmdline-openfluid-InstallExamplesSimulatorMissing)
SET_PROPERTY(TEST cmdline-openfluid-InstallExamplesAll APPEND PROPERTY DEPENDS cmdline-openfluid-InstallExamplesProjectsSelection)
SET_PROPERTY(TEST cmdline-openfluid-InstallExamplesAll APPEND PROPERTY DEPENDS cmdline-openfluid-InstallExamplesProjectsAll)


###########################################################################


OPENFLUID_ADD_TEST(NAME cmdline-openfluid-InstallExamplesRunPrimitives
                   COMMAND "${OFBUILD_DIST_BIN_DIR}/${OPENFLUID_CMD_APP}"
                           "run"
                           "${OFBUILD_TESTS_OUTPUT_DATA_DIR}/cmdline/install-examples/all/projects/Primitives"
                    PRE_TEST REMOVE_DIRECTORY "${OFBUILD_TESTS_OUTPUT_DATA_DIR}/cmdline/install-examples/all/projects/Primitives/OUT"
                    POST_TEST CHECK_FILE_EXIST "${OFBUILD_TESTS_OUTPUT_DATA_DIR}/cmdline/install-examples/all/projects/Primitives/OUT/openfluid-messages.log"
                              CHECK_FILE_EXIST "${OFBUILD_TESTS_OUTPUT_DATA_DIR}/cmdline/install-examples/all/projects/Primitives/OUT/fullA_unitsA1_var1.csv"
                              CHECK_FILE_EXIST "${OFBUILD_TESTS_OUTPUT_DATA_DIR}/cmdline/install-examples/all/projects/Primitives/OUT/fullB_unitsB2_var5.csv"
                   )
SET_PROPERTY(TEST cmdline-openfluid-InstallExamplesRunPrimitives APPEND PROPERTY DEPENDS cmdline-openfluid-InstallExamplesAll)


###########################################################################


OPENFLUID_ADD_TEST(NAME cmdline-openfluid-PrepareWorkspaceHelp
                   COMMAND "${OFBUILD_DIST_BIN_DIR}/${OPENFLUID_CMD_APP}"
                           "prepare-workspace" "--help"
                  )


ADD_TEST(NAME cmdline-openfluid-PrepareWorkspace
         COMMAND "${OFBUILD_DIST_BIN_DIR}/${OPENFLUID_CMD_APP}"
                 "prepare-workspace" "--path=${OFBUILD_TESTS_OUTPUT_DATA_DIR}/cmdline/prepare-workspace"
         )


###########################################################################


OPENFLUID_ADD_TEST(NAME cmdline-openfluid-CreateWareHelp
                   COMMAND "${OFBUILD_DIST_BIN_DIR}/${OPENFLUID_CMD_APP}"
                           "create-ware" "--help"
                  )


###########################################################################


OPENFLUID_ADD_TEST(NAME cmdline-openfluid-CreateWareSimulator
                   COMMAND "${OFBUILD_DIST_BIN_DIR}/${OPENFLUID_CMD_APP}"
                           "create-ware" "--type=simulator" "--id=tests.cmdline.sim"
                           "-p" "${OFBUILD_TESTS_OUTPUT_DATA_DIR}/cmdline/create-ware/sim"
                   PRE_TEST REMOVE_DIRECTORY "${OFBUILD_TESTS_OUTPUT_DATA_DIR}/cmdline/create-ware/sim/tests.cmdline.sim"
                   POST_TEST CHECK_FILE_EXIST "${OFBUILD_TESTS_OUTPUT_DATA_DIR}/cmdline/create-ware/sim/tests.cmdline.sim/CMakeLists.txt")


ADD_TEST(NAME cmdline-openfluid-CheckSimulator
        COMMAND "${OFBUILD_DIST_BIN_DIR}/${OPENFLUID_CMD_APP}"
                "check" "--src-path=${OFBUILD_TESTS_OUTPUT_DATA_DIR}/cmdline/create-ware/sim/tests.cmdline.sim" "-w")
SET_PROPERTY(TEST cmdline-openfluid-CheckSimulator APPEND PROPERTY DEPENDS cmdline-openfluid-CreateWareSimulator)


OPENFLUID_ADD_TEST(NAME cmdline-openfluid-ConfigureSimulator
                   COMMAND "${OFBUILD_DIST_BIN_DIR}/${OPENFLUID_CMD_APP}"
                           "configure"
                           "--src-path=${OFBUILD_TESTS_OUTPUT_DATA_DIR}/cmdline/create-ware/sim/tests.cmdline.sim"
                           "--build-path=${OFBUILD_TESTS_OUTPUT_DATA_DIR}/cmdline/create-ware/sim/tests.cmdline.sim/_build-release-${OPENFLUID_VERSION_MAJOR}.${OPENFLUID_VERSION_MINOR}"
                           "--generator=${CMAKE_GENERATOR}"
                           "---"
                           "-DOpenFLUID_DIR=${OFBUILD_DIST_CMAKE_MODULES_DIR}"
                           "-DOpenFLUID_ROOT_DIR=${OFBUILD_DIST_DIR}"
                           "-DOpenFLUID_EXTRA_SEARCH_PATHS=${CMAKE_BINARY_DIR}/src"
                           "-DWARE_INCLUDE_DIRS=${CMAKE_SOURCE_DIR}/src")
SET_PROPERTY(TEST cmdline-openfluid-ConfigureSimulator APPEND PROPERTY DEPENDS cmdline-openfluid-CreateWareSimulator)


OPENFLUID_ADD_TEST(NAME cmdline-openfluid-BuildSimulator
                   COMMAND "${OFBUILD_DIST_BIN_DIR}/${OPENFLUID_CMD_APP}"
                           "build" "--build-path=${OFBUILD_TESTS_OUTPUT_DATA_DIR}/cmdline/create-ware/sim/tests.cmdline.sim/_build-release-${OPENFLUID_VERSION_MAJOR}.${OPENFLUID_VERSION_MINOR}"
                           "-t" "install" "-j" "2")
SET_PROPERTY(TEST cmdline-openfluid-BuildSimulator APPEND PROPERTY DEPENDS cmdline-openfluid-ConfigureSimulator)


IF(OFBUILD_TESTS_ENABLE_DOCALYZER)
  OPENFLUID_ADD_TEST(NAME cmdline-openfluid-DocalyzeSimulator
          COMMAND "${OFBUILD_DIST_BIN_DIR}/${OPENFLUID_CMD_APP}"
                  "docalyze"
                  "--src-path=${OFBUILD_TESTS_OUTPUT_DATA_DIR}/cmdline/create-ware/sim/tests.cmdline.sim"
                  "--output-path=${OFBUILD_TESTS_OUTPUT_DATA_DIR}/cmdline/create-ware/sim"
          POST_TEST CHECK_FILE_EXIST "${OFBUILD_TESTS_OUTPUT_DATA_DIR}/cmdline/create-ware/sim/tests.cmdline.sim.pdf")
  SET_PROPERTY(TEST cmdline-openfluid-DocalyzeSimulator APPEND PROPERTY DEPENDS cmdline-openfluid-CreateWareSimulator)
ENDIF()


ADD_TEST(NAME cmdline-openfluid-PurgeSimulator
         COMMAND "${OFBUILD_DIST_BIN_DIR}/${OPENFLUID_CMD_APP}"
                  "purge" "--src-path=${OFBUILD_TESTS_OUTPUT_DATA_DIR}/cmdline/create-ware/sim/tests.cmdline.sim")
SET_PROPERTY(TEST cmdline-openfluid-PurgeSimulator APPEND PROPERTY DEPENDS cmdline-openfluid-BuildSimulator)


###########################################################################


IF(OFBUILD_ENABLE_GUI)
  OPENFLUID_ADD_TEST(NAME cmdline-openfluid-CreateWareSimulatorParamsUI
                     COMMAND "${OFBUILD_DIST_BIN_DIR}/${OPENFLUID_CMD_APP}"
                             "create-ware" "--type=simulator" "--id=tests.cmdline.sim-ui" "--with-paramsui"
                             "-p" "${OFBUILD_TESTS_OUTPUT_DATA_DIR}/cmdline/create-ware/sim"
                     PRE_TEST REMOVE_DIRECTORY "${OFBUILD_TESTS_OUTPUT_DATA_DIR}/cmdline/create-ware/sim/tests.cmdline.sim-ui"
                     POST_TEST CHECK_FILE_EXIST "${OFBUILD_TESTS_OUTPUT_DATA_DIR}/cmdline/create-ware/sim/tests.cmdline.sim-ui/CMakeLists.txt"
                     POST_TEST CHECK_FILE_EXIST "${OFBUILD_TESTS_OUTPUT_DATA_DIR}/cmdline/create-ware/sim/tests.cmdline.sim-ui/src/WareUI.cpp")


  OPENFLUID_ADD_TEST(NAME cmdline-openfluid-ConfigureSimulatorParamsUI
                     COMMAND "${OFBUILD_DIST_BIN_DIR}/${OPENFLUID_CMD_APP}"
                             "configure"
                             "--src-path=${OFBUILD_TESTS_OUTPUT_DATA_DIR}/cmdline/create-ware/sim/tests.cmdline.sim-ui"
                             "--build-path=${OFBUILD_TESTS_OUTPUT_DATA_DIR}/cmdline/build/sim/tests.cmdline.sim-ui"
                             "--generator=${CMAKE_GENERATOR}"
                             "---"
                             "-DOpenFLUID_DIR=${OFBUILD_DIST_CMAKE_MODULES_DIR}"
                             "-DOpenFLUID_ROOT_DIR=${OFBUILD_DIST_DIR}"
                             "-DOpenFLUID_EXTRA_SEARCH_PATHS=${CMAKE_BINARY_DIR}/src"
                             "-DWARE_INCLUDE_DIRS=${CMAKE_SOURCE_DIR}/src")
  SET_PROPERTY(TEST cmdline-openfluid-ConfigureSimulatorParamsUI APPEND PROPERTY DEPENDS cmdline-openfluid-CreateWareSimulatorParamsUI)


  OPENFLUID_ADD_TEST(NAME cmdline-openfluid-BuildSimulatorParamsUI
                     COMMAND "${OFBUILD_DIST_BIN_DIR}/${OPENFLUID_CMD_APP}"
                             "build"
                             "--build-path=${OFBUILD_TESTS_OUTPUT_DATA_DIR}/cmdline/build/sim/tests.cmdline.sim-ui"
                             "--with-install")
  SET_PROPERTY(TEST cmdline-openfluid-BuildSimulatorParamsUI APPEND PROPERTY DEPENDS cmdline-openfluid-ConfigureSimulatorParamsUI)
ENDIF()


###########################################################################


OPENFLUID_ADD_TEST(NAME cmdline-openfluid-CreateWareObserver
                   COMMAND "${OFBUILD_DIST_BIN_DIR}/${OPENFLUID_CMD_APP}"
                           "create-ware" "--type=observer" "--id=tests.cmdline.obs"
                           "-p" "${OFBUILD_TESTS_OUTPUT_DATA_DIR}/cmdline/create-ware/obs"
                   PRE_TEST REMOVE_DIRECTORY "${OFBUILD_TESTS_OUTPUT_DATA_DIR}/cmdline/create-ware/obs/tests.cmdline.obs"
                   POST_TEST CHECK_FILE_EXIST "${OFBUILD_TESTS_OUTPUT_DATA_DIR}/cmdline/create-ware/obs/tests.cmdline.obs/CMakeLists.txt")


ADD_TEST(NAME cmdline-openfluid-CheckObserver
         COMMAND "${OFBUILD_DIST_BIN_DIR}/${OPENFLUID_CMD_APP}"
                 "check" "--src-path=${OFBUILD_TESTS_OUTPUT_DATA_DIR}/cmdline/create-ware/obs/tests.cmdline.obs" "-w" "-f")
SET_PROPERTY(TEST cmdline-openfluid-CheckObserver
             APPEND PROPERTY DEPENDS cmdline-openfluid-CreateWareObserver
             APPEND PROPERTY FAIL_REGULAR_EXPRESSION "warning")


OPENFLUID_ADD_TEST(NAME cmdline-openfluid-ConfigureObserver
                   COMMAND "${OFBUILD_DIST_BIN_DIR}/${OPENFLUID_CMD_APP}"
                           "configure"
                           "--src-path=${OFBUILD_TESTS_OUTPUT_DATA_DIR}/cmdline/create-ware/obs/tests.cmdline.obs"
                           "--build-path=${OFBUILD_TESTS_OUTPUT_DATA_DIR}/cmdline/build/obs/tests.cmdline.obs"
                           "--generator=${CMAKE_GENERATOR}"
                           "---"
                           "-DOpenFLUID_DIR=${OFBUILD_DIST_CMAKE_MODULES_DIR}"
                           "-DOpenFLUID_ROOT_DIR=${OFBUILD_DIST_DIR}"
                           "-DOpenFLUID_EXTRA_SEARCH_PATHS=${CMAKE_BINARY_DIR}/src"
                           "-DWARE_INCLUDE_DIRS=${CMAKE_SOURCE_DIR}/src")
SET_PROPERTY(TEST cmdline-openfluid-ConfigureObserver APPEND PROPERTY DEPENDS cmdline-openfluid-CreateWareObserver)


OPENFLUID_ADD_TEST(NAME cmdline-openfluid-BuildObserver
                   COMMAND "${OFBUILD_DIST_BIN_DIR}/${OPENFLUID_CMD_APP}"
                           "build" "--build-path=${OFBUILD_TESTS_OUTPUT_DATA_DIR}/cmdline/build/obs/tests.cmdline.obs"
                           "-i")
SET_PROPERTY(TEST cmdline-openfluid-BuildObserver APPEND PROPERTY DEPENDS cmdline-openfluid-ConfigureObserver)


###########################################################################


IF(OFBUILD_ENABLE_GUI)
  OPENFLUID_ADD_TEST(NAME cmdline-openfluid-CreateWareObserverParamsUI
                     COMMAND "${OFBUILD_DIST_BIN_DIR}/${OPENFLUID_CMD_APP}"
                             "create-ware" "--type=observer" "--id=tests.cmdline.obs-ui" "--with-paramsui"
                             "-p" "${OFBUILD_TESTS_OUTPUT_DATA_DIR}/cmdline/create-ware/obs"
                     PRE_TEST REMOVE_DIRECTORY "${OFBUILD_TESTS_OUTPUT_DATA_DIR}/cmdline/create-ware/obs/tests.cmdline.obs-ui"
                     POST_TEST CHECK_FILE_EXIST "${OFBUILD_TESTS_OUTPUT_DATA_DIR}/cmdline/create-ware/obs/tests.cmdline.obs-ui/CMakeLists.txt"
                     POST_TEST CHECK_FILE_EXIST "${OFBUILD_TESTS_OUTPUT_DATA_DIR}/cmdline/create-ware/obs/tests.cmdline.obs-ui/src/WareUI.cpp")


  OPENFLUID_ADD_TEST(NAME cmdline-openfluid-ConfigureObserverParamsUI
                    COMMAND "${OFBUILD_DIST_BIN_DIR}/${OPENFLUID_CMD_APP}"
                            "configure"
                            "--src-path=${OFBUILD_TESTS_OUTPUT_DATA_DIR}/cmdline/create-ware/obs/tests.cmdline.obs-ui"
                            "--generator=${CMAKE_GENERATOR}"
                            "---"
                            "-DOpenFLUID_DIR=${OFBUILD_DIST_CMAKE_MODULES_DIR}"
                            "-DOpenFLUID_ROOT_DIR=${OFBUILD_DIST_DIR}"
                            "-DOpenFLUID_EXTRA_SEARCH_PATHS=${CMAKE_BINARY_DIR}/src"
                            "-DWARE_INCLUDE_DIRS=${CMAKE_SOURCE_DIR}/src"
                    POST_TEST CHECK_DIRECTORY_EXIST "${OFBUILD_TESTS_OUTPUT_DATA_DIR}/cmdline/create-ware/obs/tests.cmdline.obs-ui/_build-release-${OPENFLUID_VERSION_MAJOR}.${OPENFLUID_VERSION_MINOR}")
  SET_PROPERTY(TEST cmdline-openfluid-ConfigureObserverParamsUI APPEND PROPERTY DEPENDS cmdline-openfluid-CreateWareObserverParamsUI)


  OPENFLUID_ADD_TEST(NAME cmdline-openfluid-BuildObserverParamsUI
                     COMMAND "${OFBUILD_DIST_BIN_DIR}/${OPENFLUID_CMD_APP}"
                             "build"
                             "--src-path=${OFBUILD_TESTS_OUTPUT_DATA_DIR}/cmdline/create-ware/obs/tests.cmdline.obs-ui"
                     PRE_TEST CHECK_DIRECTORY_EXIST "${OFBUILD_TESTS_OUTPUT_DATA_DIR}/cmdline/create-ware/obs/tests.cmdline.obs-ui/_build-release-${OPENFLUID_VERSION_MAJOR}.${OPENFLUID_VERSION_MINOR}")
  SET_PROPERTY(TEST cmdline-openfluid-BuildObserverParamsUI APPEND PROPERTY DEPENDS cmdline-openfluid-ConfigureObserverParamsUI)

ENDIF()

###########################################################################


IF(OFBUILD_ENABLE_GUI)
  OPENFLUID_ADD_TEST(NAME cmdline-openfluid-CreateWareBuilderext
                     COMMAND "${OFBUILD_DIST_BIN_DIR}/${OPENFLUID_CMD_APP}"
                             "create-ware" "--type=builderext" "--id=tests.cmdline.bext"
                             "--bext-mode=modal" "--bext-category=spatial" "--bext-menutext=CmdLine generated"
                             "-p" "${OFBUILD_TESTS_OUTPUT_DATA_DIR}/cmdline/create-ware/bext"
                     PRE_TEST REMOVE_DIRECTORY "${OFBUILD_TESTS_OUTPUT_DATA_DIR}/cmdline/create-ware/bext/tests.cmdline.bext"
                     POST_TEST CHECK_FILE_EXIST "${OFBUILD_TESTS_OUTPUT_DATA_DIR}/cmdline/create-ware/bext/tests.cmdline.bext/CMakeLists.txt")


  ADD_TEST(NAME cmdline-openfluid-CheckBuilderext
          COMMAND "${OFBUILD_DIST_BIN_DIR}/${OPENFLUID_CMD_APP}"
                  "check" "--src-path=${OFBUILD_TESTS_OUTPUT_DATA_DIR}/cmdline/create-ware/bext/tests.cmdline.bext" "-w" "-f")
  SET_PROPERTY(TEST cmdline-openfluid-CheckBuilderext APPEND PROPERTY DEPENDS cmdline-openfluid-CreateWareBuilderext)


  ADD_TEST(NAME cmdline-openfluid-ConfigureBuilderext
           COMMAND "${OFBUILD_DIST_BIN_DIR}/${OPENFLUID_CMD_APP}"
                   "configure"
                   "--src-path=${OFBUILD_TESTS_OUTPUT_DATA_DIR}/cmdline/create-ware/bext/tests.cmdline.bext"
                   "--build-path=${OFBUILD_TESTS_OUTPUT_DATA_DIR}/cmdline/build/bext/tests.cmdline.bext"
                   "--generator=${CMAKE_GENERATOR}"
                   "---"
                   "-DOpenFLUID_DIR=${OFBUILD_DIST_CMAKE_MODULES_DIR}"
                   "-DOpenFLUID_ROOT_DIR=${OFBUILD_DIST_DIR}"
                   "-DOpenFLUID_EXTRA_SEARCH_PATHS=${CMAKE_BINARY_DIR}/src"
                   "-DWARE_INCLUDE_DIRS=${CMAKE_SOURCE_DIR}/src")
  SET_PROPERTY(TEST cmdline-openfluid-ConfigureBuilderext APPEND PROPERTY DEPENDS cmdline-openfluid-CreateWareBuilderext)


  ADD_TEST(NAME cmdline-openfluid-BuildBuilderext
           COMMAND "${OFBUILD_DIST_BIN_DIR}/${OPENFLUID_CMD_APP}"
                    "build"
                    "--build-path=${OFBUILD_TESTS_OUTPUT_DATA_DIR}/cmdline/build/bext/tests.cmdline.bext")
  SET_PROPERTY(TEST cmdline-openfluid-BuildBuilderext APPEND PROPERTY DEPENDS cmdline-openfluid-ConfigureBuilderext)

  IF(OFBUILD_TESTS_ENABLE_DOCALYZER)
    OPENFLUID_ADD_TEST(NAME cmdline-openfluid-DocalyzeBuilderext
                       COMMAND "${OFBUILD_DIST_BIN_DIR}/${OPENFLUID_CMD_APP}"
                              "docalyze"
                              "--src-path=${OFBUILD_TESTS_OUTPUT_DATA_DIR}/cmdline/create-ware/bext/tests.cmdline.bext"
                              "--output-path=${OFBUILD_TESTS_OUTPUT_DATA_DIR}/cmdline/create-ware/bext"
                       POST_TEST CHECK_FILE_EXIST "${OFBUILD_TESTS_OUTPUT_DATA_DIR}/cmdline/create-ware/bext/tests.cmdline.bext.pdf")
    SET_PROPERTY(TEST cmdline-openfluid-DocalyzeBuilderext APPEND PROPERTY DEPENDS cmdline-openfluid-CreateWareBuilderext)
  ENDIF()
ENDIF()


###########################################################################


OPENFLUID_ADD_TEST(NAME cmdline-openfluid-MigrateSimulator
                   COMMAND "${OFBUILD_DIST_BIN_DIR}/${OPENFLUID_CMD_APP}"
                           "migrate-ware"
                           "--src-path=${OFBUILD_TMP_DIR}/cmdline/migrate-ware/sim/tests.cmdline.sim-migration"
                   PRE_TEST REMOVE_DIRECTORY "${OFBUILD_TMP_DIR}/cmdline/migrate-ware/sim/tests.cmdline.sim-migration"
                   PRE_TEST COPY_DIRECTORY "${OFBUILD_TESTS_INPUT_MISCSRC_DIR}/wares-dev-201xx/sim/tests.cmdline.sim-migration"
                                           "${OFBUILD_TMP_DIR}/cmdline/migrate-ware/sim/"
                   POST_TEST CHECK_FILE_EXIST "${OFBUILD_TMP_DIR}/cmdline/migrate-ware/sim/tests.cmdline.sim-migration/src/CMakeLists.txt"
                   POST_TEST CHECK_FILE_EXIST "${OFBUILD_TMP_DIR}/cmdline/migrate-ware/sim/tests.cmdline.sim-migration/doc/doc.tex")
SET_PROPERTY(TEST cmdline-openfluid-MigrateSimulator
             APPEND PROPERTY ENVIRONMENT "OpenFLUID_DIR=${OFBUILD_DIST_CMAKE_MODULES_DIR};OpenFLUID_EXTRA_SEARCH_PATHS=${CMAKE_BINARY_DIR}/src\;${OFBUILD_DIST_DIR};OpenFLUID_MIGRATION_INCLUDE_PATHS=${CMAKE_SOURCE_DIR}/src")

ADD_TEST(NAME cmdline-openfluid-CheckMigratedSimulator
COMMAND "${OFBUILD_DIST_BIN_DIR}/${OPENFLUID_CMD_APP}"
        "check" "--src-path=${OFBUILD_TMP_DIR}/cmdline/migrate-ware/sim/tests.cmdline.sim-migration" "--format=json")
SET_PROPERTY(TEST cmdline-openfluid-CheckMigratedSimulator
            APPEND PROPERTY DEPENDS cmdline-openfluid-MigrateSimulator
            APPEND PROPERTY PASS_REGULAR_EXPRESSION "\"migration_isclean_no_comments\"\: \"warning\"")


OPENFLUID_ADD_TEST(NAME cmdline-openfluid-ConfigureMigratedSimulator
                   COMMAND "${OFBUILD_DIST_BIN_DIR}/${OPENFLUID_CMD_APP}"
                           "configure"
                           "--src-path=${OFBUILD_TMP_DIR}/cmdline/migrate-ware/sim/tests.cmdline.sim-migration"
                           "--generator=${CMAKE_GENERATOR}"
                           "---"
                           "-DOpenFLUID_DIR=${OFBUILD_DIST_CMAKE_MODULES_DIR}"
                           "-DOpenFLUID_ROOT_DIR=${OFBUILD_DIST_DIR}"
                           "-DOpenFLUID_EXTRA_SEARCH_PATHS=${CMAKE_BINARY_DIR}/src"
                           "-DWARE_INCLUDE_DIRS=${CMAKE_SOURCE_DIR}/src")
SET_PROPERTY(TEST cmdline-openfluid-ConfigureMigratedSimulator APPEND PROPERTY DEPENDS cmdline-openfluid-MigrateSimulator)


OPENFLUID_ADD_TEST(NAME cmdline-openfluid-BuildMigratedSimulator
                   COMMAND "${OFBUILD_DIST_BIN_DIR}/${OPENFLUID_CMD_APP}"
                           "build" "--src-path=${OFBUILD_TMP_DIR}/cmdline/migrate-ware/sim/tests.cmdline.sim-migration"
                           "-t" "install" "-j" "2")
SET_PROPERTY(TEST cmdline-openfluid-BuildMigratedSimulator APPEND PROPERTY DEPENDS cmdline-openfluid-ConfigureMigratedSimulator)


###########################################################################


IF(OFBUILD_ENABLE_GUI)
  OPENFLUID_ADD_TEST(NAME cmdline-openfluid-MigrateSimulatorParamsUI
                     COMMAND "${OFBUILD_DIST_BIN_DIR}/${OPENFLUID_CMD_APP}"
                             "migrate-ware"
                             "-s" "${OFBUILD_TMP_DIR}/cmdline/migrate-ware/sim/tests.cmdline.sim-ui-migration"
                             "--dest-path=${OFBUILD_TMP_DIR}/cmdline/migrate-ware/sim/tests.cmdline.sim-ui-migrated"
                             "-v"
                     PRE_TEST REMOVE_DIRECTORY "${OFBUILD_TMP_DIR}/cmdline/migrate-ware/sim/tests.cmdline.sim-ui-migration"
                     PRE_TEST REMOVE_DIRECTORY "${OFBUILD_TMP_DIR}/cmdline/migrate-ware/sim/tests.cmdline.sim-ui-migrated"
                     PRE_TEST COPY_DIRECTORY "${OFBUILD_TESTS_INPUT_MISCSRC_DIR}/wares-dev-201xx/sim/tests.cmdline.sim-ui-migration"
                                            "${OFBUILD_TMP_DIR}/cmdline/migrate-ware/sim/"
                     POST_TEST CHECK_FILE_EXIST "${OFBUILD_TMP_DIR}/cmdline/migrate-ware/sim/tests.cmdline.sim-ui-migrated/src/CMakeLists.txt"
                     POST_TEST CHECK_FILE_EXIST "${OFBUILD_TMP_DIR}/cmdline/migrate-ware/sim/tests.cmdline.sim-ui-migrated/doc/main.tex"
                     POST_TEST CHECK_FILE_EXIST "${OFBUILD_TMP_DIR}/cmdline/migrate-ware/sim/tests.cmdline.sim-ui-migrated/doc/original_main.tex"
                     POST_TEST CHECK_FILE_EXIST "${OFBUILD_TMP_DIR}/cmdline/migrate-ware/sim/tests.cmdline.sim-ui-migrated/tests/CMakeLists.txt")
  SET_PROPERTY(TEST cmdline-openfluid-MigrateSimulatorParamsUI
               APPEND PROPERTY ENVIRONMENT "OpenFLUID_DIR=${OFBUILD_DIST_CMAKE_MODULES_DIR};OpenFLUID_EXTRA_SEARCH_PATHS=${CMAKE_BINARY_DIR}/src\;${OFBUILD_DIST_DIR};OpenFLUID_MIGRATION_INCLUDE_PATHS=${CMAKE_SOURCE_DIR}/src")


  ADD_TEST(NAME cmdline-openfluid-CheckMigratedSimulatorParamsUI
  COMMAND "${OFBUILD_DIST_BIN_DIR}/${OPENFLUID_CMD_APP}"
          "check" "--src-path=${OFBUILD_TMP_DIR}/cmdline/migrate-ware/sim/tests.cmdline.sim-ui-migrated")
  SET_PROPERTY(TEST cmdline-openfluid-CheckMigratedSimulatorParamsUI
               APPEND PROPERTY DEPENDS cmdline-openfluid-MigrateSimulatorParamsUI
               APPEND PROPERTY PASS_REGULAR_EXPRESSION "code/migration_isclean_no_comments \: warning")

  OPENFLUID_ADD_TEST(NAME cmdline-openfluid-ConfigureMigratedSimulatorParamsUI
                     COMMAND "${OFBUILD_DIST_BIN_DIR}/${OPENFLUID_CMD_APP}"
                             "configure"
                             "--src-path=${OFBUILD_TMP_DIR}/cmdline/migrate-ware/sim/tests.cmdline.sim-ui-migrated"
                             "--generator=${CMAKE_GENERATOR}"
                             "---"
                             "-DOpenFLUID_DIR=${OFBUILD_DIST_CMAKE_MODULES_DIR}"
                             "-DOpenFLUID_ROOT_DIR=${OFBUILD_DIST_DIR}"
                             "-DOpenFLUID_EXTRA_SEARCH_PATHS=${CMAKE_BINARY_DIR}/src"
                             "-DWARE_INCLUDE_DIRS=${CMAKE_SOURCE_DIR}/src")
  SET_PROPERTY(TEST cmdline-openfluid-ConfigureMigratedSimulatorParamsUI APPEND PROPERTY DEPENDS cmdline-openfluid-MigrateSimulatorParamsUI)


  OPENFLUID_ADD_TEST(NAME cmdline-openfluid-BuildMigratedSimulatorParamsUI
                     COMMAND "${OFBUILD_DIST_BIN_DIR}/${OPENFLUID_CMD_APP}"
                             "build" "--src-path=${OFBUILD_TMP_DIR}/cmdline/migrate-ware/sim/tests.cmdline.sim-ui-migrated"
                             "-t" "install" "-j" "2")
  SET_PROPERTY(TEST cmdline-openfluid-BuildMigratedSimulatorParamsUI APPEND PROPERTY DEPENDS cmdline-openfluid-ConfigureMigratedSimulatorParamsUI)

  IF(OFBUILD_TESTS_ENABLE_DOCALYZER)
    OPENFLUID_ADD_TEST(NAME cmdline-openfluid-DocalyzeMigratedSimulatorParamsUI
                       COMMAND "${OFBUILD_DIST_BIN_DIR}/${OPENFLUID_CMD_APP}"
                               "docalyze"
                               "--src-path=${OFBUILD_TMP_DIR}/cmdline/migrate-ware/sim/tests.cmdline.sim-ui-migrated"
                               "--output-path=${OFBUILD_TMP_DIR}/cmdline/migrate-ware/sim"
                       POST_TEST CHECK_FILE_EXIST "${OFBUILD_TMP_DIR}/cmdline/migrate-ware/sim/tests.cmdline.sim-ui-migration.pdf")
    SET_PROPERTY(TEST cmdline-openfluid-DocalyzeMigratedSimulatorParamsUI APPEND PROPERTY DEPENDS cmdline-openfluid-MigrateSimulatorParamsUI)
  ENDIF()
ENDIF()


###########################################################################


OPENFLUID_ADD_TEST(NAME cmdline-openfluid-MigrateObserver
                   COMMAND "${OFBUILD_DIST_BIN_DIR}/${OPENFLUID_CMD_APP}"
                           "migrate-ware"
                           "--src-path=${OFBUILD_TMP_DIR}/cmdline/migrate-ware/obs/tests.cmdline.obs-migration"
                           "--dest-path=${OFBUILD_TMP_DIR}/cmdline/migrate-ware/obs/tests.cmdline.obs-migrated"
                           "--verbose"
                   PRE_TEST REMOVE_DIRECTORY "${OFBUILD_TMP_DIR}/cmdline/migrate-ware/obs/tests.cmdline.obs-migration"
                   PRE_TEST REMOVE_DIRECTORY "${OFBUILD_TMP_DIR}/cmdline/migrate-ware/obs/tests.cmdline.obs-migrated"
                   PRE_TEST COPY_DIRECTORY "${OFBUILD_TESTS_INPUT_MISCSRC_DIR}/wares-dev-201xx/obs/tests.cmdline.obs-migration"
                                           "${OFBUILD_TMP_DIR}/cmdline/migrate-ware/obs/"
                   POST_TEST CHECK_FILE_EXIST "${OFBUILD_TMP_DIR}/cmdline/migrate-ware/obs/tests.cmdline.obs-migrated/CMakeLists.txt"
                   POST_TEST CHECK_FILE_EXIST "${OFBUILD_TMP_DIR}/cmdline/migrate-ware/obs/tests.cmdline.obs-migrated/src/CMakeLists.txt"
                   POST_TEST CHECK_FILE_EXIST "${OFBUILD_TMP_DIR}/cmdline/migrate-ware/obs/tests.cmdline.obs-migrated/doc/CMakeLists.txt"
                   POST_TEST CHECK_FILE_EXIST "${OFBUILD_TMP_DIR}/cmdline/migrate-ware/obs/tests.cmdline.obs-migrated/tests/CMakeLists.txt")
SET_PROPERTY(TEST cmdline-openfluid-MigrateObserver
             APPEND PROPERTY ENVIRONMENT "OpenFLUID_DIR=${OFBUILD_DIST_CMAKE_MODULES_DIR};OpenFLUID_EXTRA_SEARCH_PATHS=${CMAKE_BINARY_DIR}/src\;${OFBUILD_DIST_DIR};OpenFLUID_MIGRATION_INCLUDE_PATHS=${CMAKE_SOURCE_DIR}/src")


ADD_TEST(NAME cmdline-openfluid-CheckMigratedObserver
COMMAND "${OFBUILD_DIST_BIN_DIR}/${OPENFLUID_CMD_APP}"
        "check" "--src-path=${OFBUILD_TMP_DIR}/cmdline/migrate-ware/obs/tests.cmdline.obs-migrated")
SET_PROPERTY(TEST cmdline-openfluid-CheckMigratedObserver APPEND PROPERTY DEPENDS cmdline-openfluid-MigrateObserver)


OPENFLUID_ADD_TEST(NAME cmdline-openfluid-ConfigureMigratedObserver
COMMAND "${OFBUILD_DIST_BIN_DIR}/${OPENFLUID_CMD_APP}"
        "configure"
        "--src-path=${OFBUILD_TMP_DIR}/cmdline/migrate-ware/obs/tests.cmdline.obs-migrated"
        "--generator=${CMAKE_GENERATOR}"
        "---"
        "-DOpenFLUID_DIR=${OFBUILD_DIST_CMAKE_MODULES_DIR}"
        "-DOpenFLUID_ROOT_DIR=${OFBUILD_DIST_DIR}"
        "-DOpenFLUID_EXTRA_SEARCH_PATHS=${CMAKE_BINARY_DIR}/src"
        "-DWARE_INCLUDE_DIRS=${CMAKE_SOURCE_DIR}/src")
SET_PROPERTY(TEST cmdline-openfluid-ConfigureMigratedObserver APPEND PROPERTY DEPENDS cmdline-openfluid-MigrateObserver)


OPENFLUID_ADD_TEST(NAME cmdline-openfluid-BuildMigratedObserver
COMMAND "${OFBUILD_DIST_BIN_DIR}/${OPENFLUID_CMD_APP}"
        "build" "--src-path=${OFBUILD_TMP_DIR}/cmdline/migrate-ware/obs/tests.cmdline.obs-migrated"
        "-t" "install" "-j" "2")
SET_PROPERTY(TEST cmdline-openfluid-BuildMigratedObserver APPEND PROPERTY DEPENDS cmdline-openfluid-ConfigureMigratedObserver)


###########################################################################


IF(OFBUILD_ENABLE_GUI)
  OPENFLUID_ADD_TEST(NAME cmdline-openfluid-MigrateObserverParamsUI
                     COMMAND "${OFBUILD_DIST_BIN_DIR}/${OPENFLUID_CMD_APP}"
                             "migrate-ware"
                             "--src-path=${OFBUILD_TMP_DIR}/cmdline/migrate-ware/obs/tests.cmdline.obs-ui-migration"
                     PRE_TEST REMOVE_DIRECTORY "${OFBUILD_TMP_DIR}/cmdline/migrate-ware/obs/tests.cmdline.obs-ui-migration"
                     PRE_TEST COPY_DIRECTORY "${OFBUILD_TESTS_INPUT_MISCSRC_DIR}/wares-dev-201xx/obs/tests.cmdline.obs-ui-migration"
                                             "${OFBUILD_TMP_DIR}/cmdline/migrate-ware/obs/"
                     POST_TEST CHECK_FILE_EXIST "${OFBUILD_TMP_DIR}/cmdline/migrate-ware/obs/tests.cmdline.obs-ui-migration/CMakeLists.txt"
                     POST_TEST CHECK_FILE_EXIST "${OFBUILD_TMP_DIR}/cmdline/migrate-ware/obs/tests.cmdline.obs-ui-migration/src/CMakeLists.txt"
                     POST_TEST CHECK_FILE_EXIST "${OFBUILD_TMP_DIR}/cmdline/migrate-ware/obs/tests.cmdline.obs-ui-migration/doc/CMakeLists.txt"
                     POST_TEST CHECK_FILE_EXIST "${OFBUILD_TMP_DIR}/cmdline/migrate-ware/obs/tests.cmdline.obs-ui-migration/tests/CMakeLists.txt")
  SET_PROPERTY(TEST cmdline-openfluid-MigrateObserverParamsUI
               APPEND PROPERTY ENVIRONMENT "OpenFLUID_DIR=${OFBUILD_DIST_CMAKE_MODULES_DIR};OpenFLUID_EXTRA_SEARCH_PATHS=${CMAKE_BINARY_DIR}/src\;${OFBUILD_DIST_DIR};OpenFLUID_MIGRATION_INCLUDE_PATHS=${CMAKE_SOURCE_DIR}/src")


  ADD_TEST(NAME cmdline-openfluid-CheckMigratedObserverParamsUI
  COMMAND "${OFBUILD_DIST_BIN_DIR}/${OPENFLUID_CMD_APP}"
          "check" "--src-path=${OFBUILD_TMP_DIR}/cmdline/migrate-ware/obs/tests.cmdline.obs-ui-migration" "--format=json")
  SET_PROPERTY(TEST cmdline-openfluid-CheckMigratedObserverParamsUI APPEND PROPERTY DEPENDS cmdline-openfluid-MigrateObserverParamsUI)


  OPENFLUID_ADD_TEST(NAME cmdline-openfluid-ConfigureMigratedObserverParamsUI
                     COMMAND "${OFBUILD_DIST_BIN_DIR}/${OPENFLUID_CMD_APP}"
                             "configure"
                             "--src-path=${OFBUILD_TMP_DIR}/cmdline/migrate-ware/obs/tests.cmdline.obs-ui-migration"
                             "--generator=${CMAKE_GENERATOR}"
                             "---"
                             "-DOpenFLUID_DIR=${OFBUILD_DIST_CMAKE_MODULES_DIR}"
                             "-DOpenFLUID_ROOT_DIR=${OFBUILD_DIST_DIR}"
                             "-DOpenFLUID_EXTRA_SEARCH_PATHS=${CMAKE_BINARY_DIR}/src"
                             "-DWARE_INCLUDE_DIRS=${CMAKE_SOURCE_DIR}/src")
  SET_PROPERTY(TEST cmdline-openfluid-ConfigureMigratedObserverParamsUI APPEND PROPERTY DEPENDS cmdline-openfluid-MigrateObserverParamsUI)


  OPENFLUID_ADD_TEST(NAME cmdline-openfluid-BuildMigratedObserverParamsUI
                     COMMAND "${OFBUILD_DIST_BIN_DIR}/${OPENFLUID_CMD_APP}"
                             "build" "--src-path=${OFBUILD_TMP_DIR}/cmdline/migrate-ware/obs/tests.cmdline.obs-ui-migration"
                             "-t" "install" "-j" "2")
  SET_PROPERTY(TEST cmdline-openfluid-BuildMigratedSimulator APPEND PROPERTY DEPENDS cmdline-openfluid-ConfigureMigratedObserverParamsUI)
ENDIF()


###########################################################################


IF(OFBUILD_ENABLE_GUI)
  OPENFLUID_ADD_TEST(NAME cmdline-openfluid-MigrateBuilderext
                     COMMAND "${OFBUILD_DIST_BIN_DIR}/${OPENFLUID_CMD_APP}"
                             "migrate-ware"
                             "--src-path=${OFBUILD_TMP_DIR}/cmdline/migrate-ware/bext/tests.cmdline.bext-migration"
                             "--verbose"
                     PRE_TEST REMOVE_DIRECTORY "${OFBUILD_TMP_DIR}/cmdline/migrate-ware/bext/tests.cmdline.bext-migration"
                     PRE_TEST COPY_DIRECTORY "${OFBUILD_TESTS_INPUT_MISCSRC_DIR}/wares-dev-201xx/bext/tests.cmdline.bext-migration"
                                             "${OFBUILD_TMP_DIR}/cmdline/migrate-ware/bext/"
                     POST_TEST CHECK_FILE_EXIST "${OFBUILD_TMP_DIR}/cmdline/migrate-ware/bext/tests.cmdline.bext-migration/CMakeLists.txt"
                     POST_TEST CHECK_FILE_EXIST "${OFBUILD_TMP_DIR}/cmdline/migrate-ware/bext/tests.cmdline.bext-migration/src/CMakeLists.txt"
                     POST_TEST CHECK_FILE_EXIST "${OFBUILD_TMP_DIR}/cmdline/migrate-ware/bext/tests.cmdline.bext-migration/doc/CMakeLists.txt"
                     POST_TEST CHECK_FILE_EXIST "${OFBUILD_TMP_DIR}/cmdline/migrate-ware/bext/tests.cmdline.bext-migration/tests/CMakeLists.txt")
  SET_PROPERTY(TEST cmdline-openfluid-MigrateBuilderext
               APPEND PROPERTY ENVIRONMENT "OpenFLUID_DIR=${OFBUILD_DIST_CMAKE_MODULES_DIR};OpenFLUID_EXTRA_SEARCH_PATHS=${CMAKE_BINARY_DIR}/src\;${OFBUILD_DIST_DIR};OpenFLUID_MIGRATION_INCLUDE_PATHS=${CMAKE_SOURCE_DIR}/src")


  ADD_TEST(NAME cmdline-openfluid-CheckMigratedBuilderext
  COMMAND "${OFBUILD_DIST_BIN_DIR}/${OPENFLUID_CMD_APP}"
          "check" "--src-path=${OFBUILD_TMP_DIR}/cmdline/migrate-ware/bext/tests.cmdline.bext-migration" "--ignore=migration_isclean_no_comments,docdir_exists," "-f")
  SET_PROPERTY(TEST cmdline-openfluid-CheckMigratedBuilderext
               APPEND PROPERTY DEPENDS cmdline-openfluid-MigrateBuilderext
               APPEND PROPERTY FAIL_REGULAR_EXPRESSION "warning"
               APPEND PROPERTY PASS_REGULAR_EXPRESSION "code/migration_isclean_no_comments \: disabled"
               APPEND PROPERTY PASS_REGULAR_EXPRESSION "structure/docdir_exists \: disabled")


  OPENFLUID_ADD_TEST(NAME cmdline-openfluid-ConfigureMigratedBuilderext
                     COMMAND "${OFBUILD_DIST_BIN_DIR}/${OPENFLUID_CMD_APP}"
                             "configure"
                             "--src-path=${OFBUILD_TMP_DIR}/cmdline/migrate-ware/bext/tests.cmdline.bext-migration"
                             "--generator=${CMAKE_GENERATOR}"
                             "---"
                             "-DOpenFLUID_DIR=${OFBUILD_DIST_CMAKE_MODULES_DIR}"
                             "-DOpenFLUID_ROOT_DIR=${OFBUILD_DIST_DIR}"
                             "-DOpenFLUID_EXTRA_SEARCH_PATHS=${CMAKE_BINARY_DIR}/src"
                             "-DWARE_INCLUDE_DIRS=${CMAKE_SOURCE_DIR}/src")
  SET_PROPERTY(TEST cmdline-openfluid-ConfigureMigratedBuilderext APPEND PROPERTY DEPENDS cmdline-openfluid-MigrateBuilderext)


  OPENFLUID_ADD_TEST(NAME cmdline-openfluid-BuildMigratedBuilderext
                     COMMAND "${OFBUILD_DIST_BIN_DIR}/${OPENFLUID_CMD_APP}"
                             "build" "--src-path=${OFBUILD_TMP_DIR}/cmdline/migrate-ware/bext/tests.cmdline.bext-migration"
                             "-t" "install" "-j" "2")
  SET_PROPERTY(TEST cmdline-openfluid-BuildMigratedBuilderext APPEND PROPERTY DEPENDS cmdline-openfluid-ConfigureMigratedBuilderext)
ENDIF()


###########################################################################


ADD_TEST(NAME cmdline-openfluid-CheckWrongPath
COMMAND "${OFBUILD_DIST_BIN_DIR}/${OPENFLUID_CMD_APP}"
        "check" "--src-path=path/that/does/not/exist")
SET_PROPERTY(TEST cmdline-openfluid-CheckWrongPath APPEND PROPERTY WILL_FAIL TRUE)


ADD_TEST(NAME cmdline-openfluid-CheckWrongPathJSON
COMMAND "${OFBUILD_DIST_BIN_DIR}/${OPENFLUID_CMD_APP}"
        "check" "--src-path=path/that/does/not/exist" "--format=json")
SET_PROPERTY(TEST cmdline-openfluid-CheckWrongPathJSON APPEND PROPERTY WILL_FAIL TRUE)


ADD_TEST(NAME cmdline-openfluid-CheckWrongOldSimulator
COMMAND "${OFBUILD_DIST_BIN_DIR}/${OPENFLUID_CMD_APP}"
        "check" "--src-path=${OFBUILD_TESTS_INPUT_MISCSRC_DIR}/wares-dev-201xx/sim/tests.cmdline.sim-migration")
SET_PROPERTY(TEST cmdline-openfluid-CheckWrongOldSimulator APPEND PROPERTY WILL_FAIL TRUE)


###########################################################################


IF(OFBUILD_TESTS_ENABLE_DOCALYZER)
  OPENFLUID_ADD_TEST(NAME cmdline-openfluid-DocalyzeFormatLatex
                     COMMAND "${OFBUILD_DIST_BIN_DIR}/${OPENFLUID_CMD_APP}" "docalyze"
                             "--src-path=${OFBUILD_TESTS_INPUT_MISCSRC_DIR}/cmdline-docalyze/sim/tests.cmdline-docalyze.tex"
                             "--output-path=${OFBUILD_TESTS_OUTPUT_DATA_DIR}/cmdline/docalyze/sim" "-k" "-e"
                     PRE_TEST REMOVE_FILE "${OFBUILD_TESTS_OUTPUT_DATA_DIR}/cmdline/docalyze/sim/tests.cmdline-docalyze.tex.pdf"
                     POST_TEST CHECK_FILE_EXIST "${OFBUILD_TESTS_OUTPUT_DATA_DIR}/cmdline/docalyze/sim/tests.cmdline-docalyze.tex.pdf")
  

  FIND_PROGRAM(_DOCALYZER_RMARKDOWN NAMES Rscript Rscript.exe) # check required Rscript tool
  # TODO add dependency to knitr & rmarkdown here (hard to check since R package)
  IF(_DOCALYZER_RMARKDOWN)
    ADD_TEST(NAME cmdline-openfluid-DocalyzeFormatRmarkdown
             COMMAND "${OFBUILD_DIST_BIN_DIR}/${OPENFLUID_CMD_APP}" "docalyze"
                               "--src-path=${OFBUILD_TESTS_INPUT_MISCSRC_DIR}/cmdline-docalyze/obs/tests.cmdline-docalyze.Rmd"
                               "--output-path=${OFBUILD_TESTS_OUTPUT_DATA_DIR}/cmdline/docalyze/obs" "-k" "-e"
                       PRE_TEST REMOVE_FILE "${OFBUILD_TESTS_OUTPUT_DATA_DIR}/cmdline/docalyze/obs/tests.cmdline-docalyze.Rmd.pdf"
                       POST_TEST CHECK_FILE_EXIST "${OFBUILD_TESTS_OUTPUT_DATA_DIR}/cmdline/docalyze/obs/tests.cmdline-docalyze.Rmd.pdf")
    SET_PROPERTY(TEST cmdline-openfluid-DocalyzeFormatRmarkdown APPEND PROPERTY SKIP_RETURN_CODE ${OPENFLUID_RETURN_CODE_DEPENDENCY_ISSUE})
  ENDIF()


  OPENFLUID_ADD_TEST(NAME cmdline-openfluid-DocalyzeFormatMarkdown
                     COMMAND "${OFBUILD_DIST_BIN_DIR}/${OPENFLUID_CMD_APP}" "docalyze"
                             "--src-path=${OFBUILD_TESTS_INPUT_MISCSRC_DIR}/cmdline-docalyze/sim/tests.cmdline-docalyze.md"
                             "--output-path=${OFBUILD_TESTS_OUTPUT_DATA_DIR}/cmdline/docalyze/sim" "-k"
                     PRE_TEST REMOVE_FILE "${OFBUILD_TESTS_OUTPUT_DATA_DIR}/cmdline/docalyze/sim/tests.cmdline-docalyze.md.pdf"
                     POST_TEST CHECK_FILE_EXIST "${OFBUILD_TESTS_OUTPUT_DATA_DIR}/cmdline/docalyze/sim/tests.cmdline-docalyze.md.pdf")
  
  
  OPENFLUID_ADD_TEST(NAME cmdline-openfluid-DocalyzeFormatReadme
           COMMAND "${OFBUILD_DIST_BIN_DIR}/${OPENFLUID_CMD_APP}" "docalyze"
                   "--src-path=${OFBUILD_TESTS_INPUT_MISCSRC_DIR}/cmdline-docalyze/bext/tests.cmdline-docalyze.readme"
                   "--output-path=${OFBUILD_TESTS_OUTPUT_DATA_DIR}/cmdline/docalyze/bext" "-k"
           PRE_TEST REMOVE_FILE "${OFBUILD_TESTS_OUTPUT_DATA_DIR}/cmdline/docalyze/bext/tests.cmdline-docalyze.readme.pdf"
           POST_TEST CHECK_FILE_EXIST "${OFBUILD_TESTS_OUTPUT_DATA_DIR}/cmdline/docalyze/bext/tests.cmdline-docalyze.readme.pdf")
  
  
  OPENFLUID_ADD_TEST(NAME cmdline-openfluid-DocalyzeFormatLatexCustom
           COMMAND "${OFBUILD_DIST_BIN_DIR}/${OPENFLUID_CMD_APP}" "docalyze"
                    "--src-path=${OFBUILD_TESTS_INPUT_MISCSRC_DIR}/cmdline-docalyze/sim/tests.cmdline-docalyze-custom.tex"
                    "--output-path=${OFBUILD_TESTS_OUTPUT_DATA_DIR}/cmdline/docalyze/sim" "-k"
           PRE_TEST REMOVE_FILE "${OFBUILD_TESTS_OUTPUT_DATA_DIR}/cmdline/docalyze/sim/tests.cmdline-docalyze-custom.tex.pdf"
           POST_TEST CHECK_FILE_EXIST "${OFBUILD_TESTS_OUTPUT_DATA_DIR}/cmdline/docalyze/sim/tests.cmdline-docalyze-custom.tex.pdf")
  
  
  ADD_TEST(NAME cmdline-openfluid-DocalyzeFormatNoDoc
           COMMAND "${OFBUILD_DIST_BIN_DIR}/${OPENFLUID_CMD_APP}" "docalyze"
                   "--src-path=${OFBUILD_TESTS_INPUT_MISCSRC_DIR}/cmdline-docalyze/obs/tests.cmdline-docalyze.nodoc"
                   "--output-path=${OFBUILD_TESTS_OUTPUT_DATA_DIR}/cmdline/docalyze/obs"
                   "--input-format=md")
  SET_TESTS_PROPERTIES(cmdline-openfluid-DocalyzeFormatNoDoc PROPERTIES
                       FAIL_REGULAR_EXPRESSION "cannot find main documentation file"
                       WILL_FAIL TRUE)
  
  
  ADD_TEST(NAME cmdline-openfluid-DocalyzeFormatNoMarkdown
           COMMAND "${OFBUILD_DIST_BIN_DIR}/${OPENFLUID_CMD_APP}" "docalyze"
                   "--src-path=${OFBUILD_TESTS_INPUT_MISCSRC_DIR}/cmdline-docalyze/sim/tests.cmdline-docalyze.tex"
                   "--output-path=${OFBUILD_TESTS_OUTPUT_DATA_DIR}/cmdline/docalyze/sim"
                   "--input-format=md")
  SET_TESTS_PROPERTIES(cmdline-openfluid-DocalyzeFormatNoMarkdown PROPERTIES
                       FAIL_REGULAR_EXPRESSION "cannot find main documentation file"
                       WILL_FAIL TRUE)
ENDIF()


###########################################################################


OPENFLUID_ADD_TEST(NAME cmdline-openfluid-CreateDataHelp
                   COMMAND "${OFBUILD_DIST_BIN_DIR}/${OPENFLUID_CMD_APP}"
                           "create-data" "--help")


OPENFLUID_ADD_TEST(NAME cmdline-openfluid-CreateDataset
                   COMMAND "${OFBUILD_DIST_BIN_DIR}/${OPENFLUID_CMD_APP}"
                              "create-data" "--type=dataset" "--name=NewDataset.IN" "--with-sample-data"
                              "--parent-path=${OFBUILD_TESTS_OUTPUT_DATA_DIR}/cmdline/create-data"
                   PRE_TEST REMOVE_DIRECTORY "${OFBUILD_TESTS_OUTPUT_DATA_DIR}/cmdline/create-data/NewDataset.IN")


OPENFLUID_ADD_TEST(NAME cmdline-openfluid-CreateDatasetCheck
                   COMMAND "${OFBUILD_DIST_BIN_DIR}/${OPENFLUID_CMD_APP}"
                           "run"
                           "${OFBUILD_TESTS_OUTPUT_DATA_DIR}/cmdline/create-data/NewDataset.IN"
                           "${OFBUILD_TESTS_OUTPUT_DATA_DIR}/cmdline/create-data/NewDataset.OUT"
                           "-p" "${OFBUILD_TESTS_BINARY_DIR}"
                  )
SET_PROPERTY(TEST cmdline-openfluid-CreateDatasetCheck APPEND PROPERTY DEPENDS cmdline-openfluid-CreateDataset)


OPENFLUID_ADD_TEST(NAME cmdline-openfluid-CreateEmptyDataset
                   COMMAND "${OFBUILD_DIST_BIN_DIR}/${OPENFLUID_CMD_APP}"
                              "create-data" "--type=dataset" "--name=NewEmptyDataset.IN"
                              "--parent-path=${OFBUILD_TESTS_OUTPUT_DATA_DIR}/cmdline/create-data"
                   PRE_TEST REMOVE_DIRECTORY "${OFBUILD_TESTS_OUTPUT_DATA_DIR}/cmdline/create-data/NewEmptyDataset.IN"
                   POST_TEST CHECK_FILE_EXIST "${OFBUILD_TESTS_OUTPUT_DATA_DIR}/cmdline/create-data/NewEmptyDataset.IN/model.fluidx")


OPENFLUID_ADD_TEST(NAME cmdline-openfluid-CreateProject
                   COMMAND "${OFBUILD_DIST_BIN_DIR}/${OPENFLUID_CMD_APP}"
                              "create-data" "--type=project" "--name=NewProject" "-s"
                              "--parent-path=${OFBUILD_TESTS_OUTPUT_DATA_DIR}/cmdline/create-data"
                              )


OPENFLUID_ADD_TEST(NAME cmdline-openfluid-CreateProjectRun
                   COMMAND "${OFBUILD_DIST_BIN_DIR}/${OPENFLUID_CMD_APP}"
                           "run"
                           "${OFBUILD_TESTS_OUTPUT_DATA_DIR}/cmdline/create-data/NewProject"
                  )
SET_PROPERTY(TEST cmdline-openfluid-CreateProjectRun APPEND PROPERTY DEPENDS cmdline-openfluid-CreateProject)


OPENFLUID_ADD_TEST(NAME cmdline-openfluid-CreateEmptyProject
                   COMMAND "${OFBUILD_DIST_BIN_DIR}/${OPENFLUID_CMD_APP}"
                              "create-data" "--type=project" "--name=NewEmptyProject"
                              "--parent-path=${OFBUILD_TESTS_OUTPUT_DATA_DIR}/cmdline/create-data"
                   POST_TEST CHECK_FILE_EXIST "${OFBUILD_TESTS_OUTPUT_DATA_DIR}/cmdline/create-data/NewEmptyProject/IN/model.fluidx"
                   POST_TEST CHECK_FILE_EXIST "${OFBUILD_TESTS_OUTPUT_DATA_DIR}/cmdline/create-data/NewEmptyProject/openfluid-project.json")

