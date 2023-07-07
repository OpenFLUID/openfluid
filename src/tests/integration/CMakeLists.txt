FIND_PACKAGE(Boost REQUIRED COMPONENTS unit_test_framework)


INCLUDE_DIRECTORIES("${PROJECT_SOURCE_DIR}/src" "${PROJECT_BINARY_DIR}/src/tests")


###########################################################################


OPENFLUID_ADD_TEST(NAME integration-MissingSimulator
                   COMMAND "${OFBUILD_DIST_BIN_DIR}/${OPENFLUID_CMD_APP}"
                           "run"
                           "${OFBUILD_TESTS_INPUT_DATASETS_DIR}/OPENFLUID.IN.MissingSim"
                           "${OFBUILD_TESTS_OUTPUT_DATA_DIR}/OPENFLUID.OUT.MissingSim"
                           "-p" "${OFBUILD_TESTS_BINARY_DIR}")
SET_TESTS_PROPERTIES(integration-MissingSimulator PROPERTIES WILL_FAIL TRUE)



###########################################################################


OPENFLUID_ADD_TEST(NAME integration-MissingUnitsClass
                   COMMAND "${OFBUILD_DIST_BIN_DIR}/${OPENFLUID_CMD_APP}"
                           "run"
                           "${OFBUILD_TESTS_INPUT_DATASETS_DIR}/OPENFLUID.IN.MissingClass"
                           "${OFBUILD_TESTS_OUTPUT_DATA_DIR}/OPENFLUID.OUT.MissingClass"
                           "-p" "${OFBUILD_TESTS_BINARY_DIR}")
SET_TESTS_PROPERTIES(integration-MissingUnitsClass PROPERTIES WILL_FAIL TRUE)


###########################################################################


OPENFLUID_ADD_TEST(NAME integration-MissingVariable
                   COMMAND "${OFBUILD_DIST_BIN_DIR}/${OPENFLUID_CMD_APP}"
                           "run"
                           "${OFBUILD_TESTS_INPUT_DATASETS_DIR}/OPENFLUID.IN.MissingVar"
                           "${OFBUILD_TESTS_OUTPUT_DATA_DIR}/OPENFLUID.OUT.MissingVar"
                           "-p" "${OFBUILD_TESTS_BINARY_DIR}")
SET_TESTS_PROPERTIES(integration-MissingVariable PROPERTIES WILL_FAIL TRUE)



###########################################################################


OPENFLUID_ADD_TEST(NAME integration-MissingAttributes
                   COMMAND "${OFBUILD_DIST_BIN_DIR}/${OPENFLUID_CMD_APP}"
                           "run"
                           "${OFBUILD_TESTS_INPUT_DATASETS_DIR}/OPENFLUID.IN.MissingAttributes"
                           "${OFBUILD_TESTS_OUTPUT_DATA_DIR}/OPENFLUID.OUT.MissingAttributes"
                           "-p" "${OFBUILD_TESTS_BINARY_DIR}")
SET_TESTS_PROPERTIES(integration-MissingAttributes PROPERTIES WILL_FAIL TRUE)



###########################################################################


OPENFLUID_ADD_TEST(NAME integration-DuplicateVariable
                   COMMAND "${OFBUILD_DIST_BIN_DIR}/${OPENFLUID_CMD_APP}"
                           "run"
                           "${OFBUILD_TESTS_INPUT_DATASETS_DIR}/OPENFLUID.IN.DuplicateVar"
                           "${OFBUILD_TESTS_OUTPUT_DATA_DIR}/OPENFLUID.OUT.DuplicateVar"
                           "-p" "${OFBUILD_TESTS_BINARY_DIR}")
SET_TESTS_PROPERTIES(integration-DuplicateVariable PROPERTIES WILL_FAIL TRUE)


###########################################################################


FILE(COPY "${OFBUILD_TESTS_INPUT_DATASETS_DIR}/OPENFLUID.PRJ.Primitives"
       DESTINATION "${OFBUILD_TESTS_OUTPUT_DATA_DIR}"
       PATTERN ".svn" EXCLUDE)

OPENFLUID_ADD_TEST(NAME integration-Project
                   COMMAND "${OFBUILD_DIST_BIN_DIR}/${OPENFLUID_CMD_APP}"
                           "run"
                           "${OFBUILD_TESTS_OUTPUT_DATA_DIR}/OPENFLUID.PRJ.Primitives"
                           "-p" "${OFBUILD_TESTS_BINARY_DIR}"
                           "-c")


###########################################################################


OPENFLUID_ADD_TEST(NAME integration-Profiling
                   COMMAND "${OFBUILD_DIST_BIN_DIR}/${OPENFLUID_CMD_APP}"
                           "run"
                           "${OFBUILD_TESTS_INPUT_DATASETS_DIR}/OPENFLUID.IN.Coupling"
                           "${OFBUILD_TESTS_OUTPUT_DATA_DIR}/OPENFLUID.OUT.Profiling"
                           "-p" "${OFBUILD_TESTS_BINARY_DIR}"
                           "-k")


###########################################################################


OPENFLUID_ADD_TEST(NAME integration-FluidXWriterSingle
                   COMMAND "${OFBUILD_DIST_BIN_DIR}/${OPENFLUID_CMD_APP}"
                           "run"
                           "${OFBUILD_TESTS_OUTPUT_DATA_DIR}/OPENFLUID.OUT.FluidXWriterSingle"
                           "${OFBUILD_TESTS_OUTPUT_DATA_DIR}/OPENFLUID.OUT.FluidXWriterRunSingle"
                           "-p" "${OFBUILD_TESTS_BINARY_DIR}")
SET_PROPERTY(TEST integration-FluidXWriterSingle APPEND PROPERTY DEPENDS fluidx-FluidXIO_TEST)


###########################################################################


OPENFLUID_ADD_TEST(NAME integration-FluidXWriterMany
                   COMMAND "${OFBUILD_DIST_BIN_DIR}/${OPENFLUID_CMD_APP}"
                            "run"
                            "${OFBUILD_TESTS_OUTPUT_DATA_DIR}/OPENFLUID.OUT.FluidXWriterMany"
                            "${OFBUILD_TESTS_OUTPUT_DATA_DIR}/OPENFLUID.OUT.FluidXWriterRunMany"
                            "-p" "${OFBUILD_TESTS_BINARY_DIR}")
SET_PROPERTY(TEST integration-FluidXWriterMany APPEND PROPERTY DEPENDS fluidx-FluidXIO_TEST)


###########################################################################
# integration-GeneratorsCycle is a test in 2 steps to check that inject generator and observer work correctly together
#    Step 1 generates the data file in its OUT via a regular simulator.
#           The output data file is copied to a temporary dir containing IN files of step 2
#    Step 2 uses the data file as source for inject generator and regenerates it as OUT
#           the two OUT data files are then compared to check identity through observer and generator

OPENFLUID_ADD_TEST(NAME integration-GeneratorsCycleStep1Sim
                   COMMAND "${OFBUILD_DIST_BIN_DIR}/${OPENFLUID_CMD_APP}"
                           "run"
                           "${OFBUILD_TESTS_INPUT_DATASETS_DIR}/OPENFLUID.IN.SimGenConsistencyStep1Sim"
                           "${OFBUILD_TESTS_OUTPUT_DATA_DIR}/OPENFLUID.OUT.SimGenConsistencyStep1Sim"
                           "-p" "${OFBUILD_TESTS_BINARY_DIR}" "-v"
                   )

FILE(COPY "${OFBUILD_TESTS_INPUT_DATASETS_DIR}/OPENFLUID.IN.SimGenConsistencyStep2Gen"
     DESTINATION "${OFBUILD_TESTS_TEMP_DIR}")
OPENFLUID_ADD_TEST(NAME integration-GeneratorsCycleStep2Gen
                   COMMAND "${OFBUILD_DIST_BIN_DIR}/${OPENFLUID_CMD_APP}"
                           "run"
                           "${OFBUILD_TESTS_TEMP_DIR}/OPENFLUID.IN.SimGenConsistencyStep2Gen"
                           "${OFBUILD_TESTS_OUTPUT_DATA_DIR}/OPENFLUID.OUT.SimGenConsistencyStep2Gen"
                           "-p" "${OFBUILD_TESTS_BINARY_DIR}" "-v"

                   PRE_TEST COPY_FILE "${OFBUILD_TESTS_OUTPUT_DATA_DIR}/OPENFLUID.OUT.SimGenConsistencyStep1Sim/autoTU_TU1_var.rand.prod.csv"
                                      "${OFBUILD_TESTS_TEMP_DIR}/OPENFLUID.IN.SimGenConsistencyStep2Gen/"
                   POST_TEST CHECK_FILE_IDENTICAL "${OFBUILD_TESTS_OUTPUT_DATA_DIR}/OPENFLUID.OUT.SimGenConsistencyStep1Sim/"
                                                  "${OFBUILD_TESTS_OUTPUT_DATA_DIR}/OPENFLUID.OUT.SimGenConsistencyStep2Gen/"
                                                  "autoTU_TU1_var.rand.prod.csv"
                   )
SET_TESTS_PROPERTIES(integration-GeneratorsCycleStep2Gen PROPERTIES DEPENDS integration-GeneratorsCycleStep1Sim)


###########################################################################


FILE(MAKE_DIRECTORY "${OFBUILD_TESTS_OUTPUT_DATA_DIR}/FindOpenFLUID")

OPENFLUID_ADD_TEST(NAME integration-CMakeModuleCheck
                   COMMAND "${CMAKE_COMMAND}"
                           "-E" "chdir" "${OFBUILD_TESTS_OUTPUT_DATA_DIR}/FindOpenFLUID"
                           "${CMAKE_COMMAND}" "-G" "${CMAKE_GENERATOR}"
                           "${CMAKE_CURRENT_SOURCE_DIR}/FindOpenFLUID"
                           "-DOpenFLUID_DIR=${OFBUILD_DIST_CMAKE_MODULES_DIR}"
                           "-DOpenFLUID_ROOT_DIR=${OFBUILD_DIST_DIR}"
                           "-DTEST_SEARCH_PATHS1=${CMAKE_BINARY_DIR}/src"
                           "-DTEST_SEARCH_PATHS2=${CMAKE_SOURCE_DIR}/src"
                   PRE_TEST EMPTY_DIRECTORY "${OFBUILD_TESTS_OUTPUT_DATA_DIR}/FindOpenFLUID")


###########################################################################


FILE(MAKE_DIRECTORY "${OFBUILD_TESTS_OUTPUT_DATA_DIR}/tests.sim.cmake")

OPENFLUID_ADD_TEST(NAME integration-CMakeModuleSimulatorConfigure
                   COMMAND "${CMAKE_COMMAND}"
                             "-E" "chdir" "${OFBUILD_TESTS_OUTPUT_DATA_DIR}/tests.sim.cmake"
                             "${CMAKE_COMMAND}" "-G" "${CMAKE_GENERATOR}"
                             "${CMAKE_CURRENT_SOURCE_DIR}/tests.sim.cmake"
                             "-DOpenFLUID_DIR=${OFBUILD_DIST_CMAKE_MODULES_DIR}"
                             "-DOpenFLUID_ROOT_DIR=${OFBUILD_DIST_DIR}"
                             "-DOpenFLUID_EXTRA_SEARCH_PATHS=${CMAKE_BINARY_DIR}/src"
                             "-DWARE_INCLUDE_DIRS=${CMAKE_SOURCE_DIR}/src"
                   PRE_TEST EMPTY_DIRECTORY "${OFBUILD_TESTS_OUTPUT_DATA_DIR}/tests.sim.cmake")


###########################################################################


OPENFLUID_ADD_TEST(NAME integration-CMakeModuleSimulatorBuild
                   COMMAND "${CMAKE_COMMAND}"
                           "-E" "chdir" "${OFBUILD_TESTS_OUTPUT_DATA_DIR}/tests.sim.cmake"
                           "${CMAKE_COMMAND}" "--build" "${OFBUILD_TESTS_OUTPUT_DATA_DIR}/tests.sim.cmake"
                           "--target" "install"
                           "--clean-first")
SET_PROPERTY(TEST integration-CMakeModuleSimulatorBuild APPEND PROPERTY DEPENDS integration-CMakeModuleSimulatorConfigure)


###########################################################################


OPENFLUID_ADD_TEST(NAME integration-CMakeModuleSimulatorTest
                   COMMAND "${CMAKE_COMMAND}"
                           "-E" "chdir" "${OFBUILD_TESTS_OUTPUT_DATA_DIR}/tests.sim.cmake"
                           "${CMAKE_CTEST_COMMAND}" "-V")
SET_PROPERTY(TEST integration-CMakeModuleSimulatorTest APPEND PROPERTY DEPENDS integration-CMakeModuleSimulatorBuild)


###########################################################################


IF(OFBUILD_TESTS_ENABLE_DOCALYZER)
  OPENFLUID_ADD_TEST(NAME integration-CMakeModuleSimulatorDoc
                     COMMAND "${CMAKE_COMMAND}"
                             "-E" "chdir" "${OFBUILD_TESTS_OUTPUT_DATA_DIR}/tests.sim.cmake"
                             "${CMAKE_COMMAND}" "--build" "${OFBUILD_TESTS_OUTPUT_DATA_DIR}/tests.sim.cmake"
                             "--target" "docalyze-install")
ENDIF()


###########################################################################


IF(OFBUILD_ENABLE_GUI)
  FILE(MAKE_DIRECTORY "${OFBUILD_TESTS_OUTPUT_DATA_DIR}/tests.simui.cmake")

  OPENFLUID_ADD_TEST(NAME integration-CMakeModuleSimulatorParamsUIConfigure
                     COMMAND "${CMAKE_COMMAND}"
                              "-E" "chdir" "${OFBUILD_TESTS_OUTPUT_DATA_DIR}/tests.simui.cmake"
                              "${CMAKE_COMMAND}" "-G" "${CMAKE_GENERATOR}"
                              "${CMAKE_CURRENT_SOURCE_DIR}/tests.simui.cmake"
                              "-DOpenFLUID_DIR=${OFBUILD_DIST_CMAKE_MODULES_DIR}"
                              "-DOpenFLUID_ROOT_DIR=${OFBUILD_DIST_DIR}"
                              "-DOpenFLUID_EXTRA_SEARCH_PATHS=${CMAKE_BINARY_DIR}/src"
                              "-DWARE_INCLUDE_DIRS=${CMAKE_SOURCE_DIR}/src"
                     PRE_TEST EMPTY_DIRECTORY "${OFBUILD_TESTS_OUTPUT_DATA_DIR}/tests.simui.cmake")
ENDIF()


###########################################################################


IF(OFBUILD_ENABLE_GUI)
  OPENFLUID_ADD_TEST(NAME integration-CMakeModuleSimulatorParamsUIBuild
                     COMMAND "${CMAKE_COMMAND}"
                              "-E" "chdir" "${OFBUILD_TESTS_OUTPUT_DATA_DIR}/tests.simui.cmake"
                              "${CMAKE_COMMAND}" "--build" "${OFBUILD_TESTS_OUTPUT_DATA_DIR}/tests.simui.cmake"
                              "--target" "install"
                              "--clean-first")
  SET_PROPERTY(TEST integration-CMakeModuleSimulatorParamsUIBuild APPEND PROPERTY DEPENDS integration-CMakeModuleSimulatorParamsUIConfigure)
ENDIF()


###########################################################################


FILE(MAKE_DIRECTORY "${OFBUILD_TESTS_OUTPUT_DATA_DIR}/tests.obs.cmake")

OPENFLUID_ADD_TEST(NAME integration-CMakeModuleObserverConfigure
                   COMMAND "${CMAKE_COMMAND}"
                              "-E" "chdir" "${OFBUILD_TESTS_OUTPUT_DATA_DIR}/tests.obs.cmake"
                              "${CMAKE_COMMAND}" "-G" "${CMAKE_GENERATOR}"
                              "${CMAKE_CURRENT_SOURCE_DIR}/tests.obs.cmake"
                              "-DOpenFLUID_DIR=${OFBUILD_DIST_CMAKE_MODULES_DIR}"
                              "-DOpenFLUID_ROOT_DIR=${OFBUILD_DIST_DIR}"
                              "-DOpenFLUID_EXTRA_SEARCH_PATHS=${CMAKE_BINARY_DIR}/src"
                              "-DWARE_INCLUDE_DIRS=${CMAKE_SOURCE_DIR}/src"
                    PRE_TEST EMPTY_DIRECTORY "${OFBUILD_TESTS_OUTPUT_DATA_DIR}/tests.obs.cmake")


###########################################################################


OPENFLUID_ADD_TEST(NAME integration-CMakeModuleObserverBuild
                   COMMAND "${CMAKE_COMMAND}"
                              "-E" "chdir" "${OFBUILD_TESTS_OUTPUT_DATA_DIR}/tests.obs.cmake"
                              "${CMAKE_COMMAND}" "--build" "${OFBUILD_TESTS_OUTPUT_DATA_DIR}/tests.obs.cmake"
                              "--target" "install"
                              "--clean-first")
SET_PROPERTY(TEST integration-CMakeModuleObserverBuild APPEND PROPERTY DEPENDS integration-CMakeModuleObserverConfigure)


###########################################################################


OPENFLUID_ADD_TEST(NAME integration-CMakeModuleObserverTest
                   COMMAND "${CMAKE_COMMAND}"
                              "-E" "chdir" "${OFBUILD_TESTS_OUTPUT_DATA_DIR}/tests.obs.cmake"
                              "${CMAKE_CTEST_COMMAND}" "-V")
SET_PROPERTY(TEST integration-CMakeModuleObserverTest APPEND PROPERTY DEPENDS integration-CMakeModuleObserverBuild)


###########################################################################

IF(OFBUILD_ENABLE_GUI)
  FILE(MAKE_DIRECTORY "${OFBUILD_TESTS_OUTPUT_DATA_DIR}/tests.obsui.cmake")

  OPENFLUID_ADD_TEST(NAME integration-CMakeModuleObserverParamsUIConfigure
                     COMMAND "${CMAKE_COMMAND}"
                              "-E" "chdir" "${OFBUILD_TESTS_OUTPUT_DATA_DIR}/tests.obsui.cmake"
                              "${CMAKE_COMMAND}" "-G" "${CMAKE_GENERATOR}"
                              "${CMAKE_CURRENT_SOURCE_DIR}/tests.obsui.cmake"
                              "-DOpenFLUID_DIR=${OFBUILD_DIST_CMAKE_MODULES_DIR}"
                              "-DOpenFLUID_ROOT_DIR=${OFBUILD_DIST_DIR}"
                              "-DOpenFLUID_EXTRA_SEARCH_PATHS=${CMAKE_BINARY_DIR}/src"
                              "-DWARE_INCLUDE_DIRS=${CMAKE_SOURCE_DIR}/src"
                      PRE_TEST EMPTY_DIRECTORY "${OFBUILD_TESTS_OUTPUT_DATA_DIR}/tests.obsui.cmake")
ENDIF()


###########################################################################


IF(OFBUILD_ENABLE_GUI)
  OPENFLUID_ADD_TEST(NAME integration-CMakeModuleObserverParamsUIBuild
                     COMMAND "${CMAKE_COMMAND}"
                              "-E" "chdir" "${OFBUILD_TESTS_OUTPUT_DATA_DIR}/tests.obsui.cmake"
                              "${CMAKE_COMMAND}" "--build" "${OFBUILD_TESTS_OUTPUT_DATA_DIR}/tests.obsui.cmake"
                              "--target" "install"
                              "--clean-first")
  SET_PROPERTY(TEST integration-CMakeModuleObserverParamsUIBuild APPEND PROPERTY DEPENDS integration-CMakeModuleObserverParamsUIConfigure)
ENDIF()


###########################################################################


IF(OFBUILD_ENABLE_APP_BUILDER)
  FILE(MAKE_DIRECTORY "${OFBUILD_TESTS_OUTPUT_DATA_DIR}/tests.bext.cmake")
  OPENFLUID_ADD_TEST(NAME integration-CMakeModuleBuilderextConfigure
                     COMMAND "${CMAKE_COMMAND}"
                        "-E" "chdir" "${OFBUILD_TESTS_OUTPUT_DATA_DIR}/tests.bext.cmake"
                        "${CMAKE_COMMAND}" "-G" "${CMAKE_GENERATOR}"
                        "${CMAKE_CURRENT_SOURCE_DIR}/tests.bext.cmake"
                        "-DOpenFLUID_DIR=${OFBUILD_DIST_CMAKE_MODULES_DIR}"
                        "-DOpenFLUID_ROOT_DIR=${OFBUILD_DIST_DIR}"
                        "-DOpenFLUID_EXTRA_SEARCH_PATHS=${CMAKE_BINARY_DIR}/src"
                        "-DWARE_INCLUDE_DIRS=${CMAKE_SOURCE_DIR}/src"
                      PRE_TEST EMPTY_DIRECTORY "${OFBUILD_TESTS_OUTPUT_DATA_DIR}/tests.bext.cmake")
ENDIF()


###########################################################################


IF(OFBUILD_ENABLE_APP_BUILDER)
  OPENFLUID_ADD_TEST(NAME integration-CMakeModuleBuilderextBuild
                     COMMAND "${CMAKE_COMMAND}"
                             "-E" "chdir" "${OFBUILD_TESTS_OUTPUT_DATA_DIR}/tests.bext.cmake"
                             "${CMAKE_COMMAND}" "--build" "${OFBUILD_TESTS_OUTPUT_DATA_DIR}/tests.bext.cmake"
                             "--target" "install"
                             "--clean-first")
  SET_PROPERTY(TEST integration-CMakeModuleBuilderextBuild APPEND PROPERTY DEPENDS integration-CMakeModuleBuilderextConfigure)
ENDIF()


###########################################################################


IF(OFBUILD_ENABLE_GUI)
  OPENFLUID_ADD_TEST(NAME integration-CMakeModuleRunWithUI
                     COMMAND "${OFBUILD_DIST_BIN_DIR}/${OPENFLUID_CMD_APP}"
                             "run"
                             "${OFBUILD_TESTS_INPUT_DATASETS_DIR}/OPENFLUID.IN.CMakeModuleWithUI"
                             "${OFBUILD_TESTS_OUTPUT_DATA_DIR}/OPENFLUID.OUT.CMakeModuleWithUI"
                     PRE_TEST REMOVE_DIRECTORY "${OFBUILD_TESTS_OUTPUT_DATA_DIR}/OPENFLUID.OUT.CMakeModuleWithUI")
  SET_PROPERTY(TEST integration-CMakeModuleRunWithUI APPEND PROPERTY ENVIRONMENT "LD_LIBRARY_PATH=${OFBUILD_DIST_LIB_DIR}")
  SET_PROPERTY(TEST integration-CMakeModuleRunWithUI APPEND PROPERTY DEPENDS integration-CMakeModuleSimulatorBuild)
  SET_PROPERTY(TEST integration-CMakeModuleRunWithUI APPEND PROPERTY DEPENDS integration-CMakeModuleSimulatorParamsUIBuild)
  SET_PROPERTY(TEST integration-CMakeModuleRunWithUI APPEND PROPERTY DEPENDS integration-CMakeModuleObserverBuild)
  SET_PROPERTY(TEST integration-CMakeModuleRunWithUI APPEND PROPERTY DEPENDS integration-CMakeModuleObserverParamsUIBuild)
ELSE()
  OPENFLUID_ADD_TEST(NAME integration-CMakeModuleRun
                     COMMAND "${OFBUILD_DIST_BIN_DIR}/${OPENFLUID_CMD_APP}"
                             "run"
                             "${OFBUILD_TESTS_INPUT_DATASETS_DIR}/OPENFLUID.IN.CMakeModule"
                             "${OFBUILD_TESTS_OUTPUT_DATA_DIR}/OPENFLUID.OUT.CMakeModule"
                     PRE_TEST REMOVE_DIRECTORY "${OFBUILD_TESTS_OUTPUT_DATA_DIR}/OPENFLUID.OUT.CMakeModule")
  SET_PROPERTY(TEST integration-CMakeModuleRun APPEND PROPERTY ENVIRONMENT "LD_LIBRARY_PATH=${OFBUILD_DIST_LIB_DIR}")
  SET_PROPERTY(TEST integration-CMakeModuleRun APPEND PROPERTY DEPENDS integration-CMakeModuleSimulatorBuild)
  SET_PROPERTY(TEST integration-CMakeModuleRun APPEND PROPERTY DEPENDS integration-CMakeModuleObserverBuild)
ENDIF()


###########################################################################


OPENFLUID_ADD_TEST(NAME integration-Debug
                   COMMAND "${OFBUILD_DIST_BIN_DIR}/${OPENFLUID_CMD_APP}"
                           "report" "simulators" "--list"
                           "-p" "${OFBUILD_TESTS_BINARY_DIR}")
IF(${CMAKE_BUILD_TYPE} STREQUAL "Debug")
  SET_TESTS_PROPERTIES(integration-Debug PROPERTIES PASS_REGULAR_EXPRESSION "OpenFLUID debugging mode is enabled")
ELSE()
  SET_TESTS_PROPERTIES(integration-Debug PROPERTIES FAIL_REGULAR_EXPRESSION "OpenFLUID debugging mode is enabled")
ENDIF()
