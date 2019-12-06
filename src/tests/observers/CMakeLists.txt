

INCLUDE_DIRECTORIES("${PROJECT_SOURCE_DIR}/src" "${PROJECT_BINARY_DIR}/src")


OFBUILD_ADD_OBSERVER(tests.empty ${OFBUILD_TESTS_BINARY_DIR})
OFBUILD_ADD_OBSERVER(tests.primitives.time ${OFBUILD_TESTS_BINARY_DIR})

###########################################################################


OFBUILD_ADD_OBSERVER(tests.hopla ${OFBUILD_TESTS_BINARY_DIR})
OPENFLUID_ADD_TEST(NAME observers-Hopla 
                   COMMAND "${OFBUILD_DIST_BIN_DIR}/${OPENFLUID_CMD_APP}" 
                                 "run"
                                 "${OFBUILD_TESTS_INPUT_DATASETS_DIR}/OPENFLUID.IN.PrimitivesHopla"
                                 "${OFBUILD_TESTS_OUTPUT_DATA_DIR}/OPENFLUID.OUT.PrimitivesHopla" 
                                 "-p" "${OFBUILD_TESTS_BINARY_DIR}"
                                 "-n" "${OFBUILD_TESTS_BINARY_DIR}"
                    PRE_TEST REMOVE_DIRECTORY "${OFBUILD_TESTS_OUTPUT_DATA_DIR}/OPENFLUID.OUT.PrimitivesHopla"
                   )


###########################################################################


OFBUILD_ADD_OBSERVER(tests.exceptions ${OFBUILD_TESTS_BINARY_DIR})
OPENFLUID_ADD_TEST(NAME observers-Exceptions 
                   COMMAND "${OFBUILD_DIST_BIN_DIR}/${OPENFLUID_CMD_APP}" 
                           "run"
                           "${OFBUILD_TESTS_INPUT_DATASETS_DIR}/OPENFLUID.IN.ObsExceptions"
                           "${OFBUILD_TESTS_OUTPUT_DATA_DIR}/OPENFLUID.OUT.ObsExceptions" 
                           "-p" "${OFBUILD_TESTS_BINARY_DIR}"
                           "-n" "${OFBUILD_TESTS_BINARY_DIR}"
                    PRE_TEST REMOVE_DIRECTORY "${OFBUILD_TESTS_OUTPUT_DATA_DIR}/OPENFLUID.OUT.ObsExceptions"
                    POST_TEST CHECK_FILE_EXIST "${OFBUILD_TESTS_OUTPUT_DATA_DIR}/OPENFLUID.OUT.ObsExceptions/openfluid-messages.log"
                   )
SET_TESTS_PROPERTIES(observers-Exceptions PROPERTIES WILL_FAIL TRUE)


###########################################################################

                                 
OPENFLUID_ADD_TEST(NAME observers-CSVFiles 
                   COMMAND "${OFBUILD_DIST_BIN_DIR}/${OPENFLUID_CMD_APP}" 
                        "run"
                        "${OFBUILD_TESTS_INPUT_DATASETS_DIR}/OPENFLUID.IN.CSVObserver"
                        "${OFBUILD_TESTS_OUTPUT_DATA_DIR}/OPENFLUID.OUT.CSVObserver" 
                        "-p" "${OFBUILD_TESTS_BINARY_DIR}"
                        "-n" "${OFBUILD_TESTS_BINARY_DIR}"
                    PRE_TEST REMOVE_DIRECTORY "${OFBUILD_TESTS_OUTPUT_DATA_DIR}/OPENFLUID.OUT.CSVObserver"
                    POST_TEST CHECK_FILE_EXIST "${OFBUILD_TESTS_OUTPUT_DATA_DIR}/OPENFLUID.OUT.CSVObserver/fullfull_TestUnits1_tests.double.csv"
                              CHECK_FILE_EXIST "${OFBUILD_TESTS_OUTPUT_DATA_DIR}/OPENFLUID.OUT.CSVObserver/fullfull_TestUnits5_tests.double.csv"
                              CHECK_FILE_EXIST "${OFBUILD_TESTS_OUTPUT_DATA_DIR}/OPENFLUID.OUT.CSVObserver/some_TestUnits11_tests.string.csv"
                              CHECK_FILE_EXIST "${OFBUILD_TESTS_OUTPUT_DATA_DIR}/OPENFLUID.OUT.CSVObserver/someunits_TestUnits9_tests.matrix.dt.csv"
                              CHECK_FILE_EXIST "${OFBUILD_TESTS_OUTPUT_DATA_DIR}/OPENFLUID.OUT.CSVObserver/somevars_TestUnits7_tests.vector.csv"
                   )


###########################################################################


OPENFLUID_ADD_TEST(NAME observers-MulticolsCSVFiles 
                   COMMAND "${OFBUILD_DIST_BIN_DIR}/${OPENFLUID_CMD_APP}" 
                        "run"
                        "${OFBUILD_TESTS_INPUT_DATASETS_DIR}/OPENFLUID.IN.MulticolsCSVObserver"
                        "${OFBUILD_TESTS_OUTPUT_DATA_DIR}/OPENFLUID.OUT.MulticolsCSVObserver" 
                        "-p" "${OFBUILD_TESTS_BINARY_DIR}"
                        "-n" "${OFBUILD_TESTS_BINARY_DIR}"
                   PRE_TEST REMOVE_DIRECTORY "${OFBUILD_TESTS_OUTPUT_DATA_DIR}/OPENFLUID.OUT.MulticolsCSVObserver"
                   POST_TEST CHECK_FILE_EXIST "${OFBUILD_TESTS_OUTPUT_DATA_DIR}/OPENFLUID.OUT.MulticolsCSVObserver/autoAll.csv"
                   )


###########################################################################


OPENFLUID_ADD_TEST(NAME observers-MulticolsCSVFiles-KeepEmpty 
                   COMMAND "${OFBUILD_DIST_BIN_DIR}/${OPENFLUID_CMD_APP}" 
                        "run"
                        "${OFBUILD_TESTS_INPUT_DATASETS_DIR}/OPENFLUID.IN.MulticolsCSVObserverKeepEmpty"
                        "${OFBUILD_TESTS_OUTPUT_DATA_DIR}/OPENFLUID.OUT.MulticolsCSVObserverKeepEmpty" 
                        "-p" "${OFBUILD_TESTS_BINARY_DIR}"
                        "-n" "${OFBUILD_TESTS_BINARY_DIR}"
                   PRE_TEST REMOVE_DIRECTORY "${OFBUILD_TESTS_OUTPUT_DATA_DIR}/OPENFLUID.OUT.MulticolsCSVObserverKeepEmpty"
                   POST_TEST CHECK_FILE_EXIST "${OFBUILD_TESTS_OUTPUT_DATA_DIR}/OPENFLUID.OUT.MulticolsCSVObserverKeepEmpty/autoAll.csv"
                   )


###########################################################################


OPENFLUID_ADD_TEST(NAME observers-CSVObserverPrecision
                   COMMAND "${OFBUILD_DIST_BIN_DIR}/${OPENFLUID_CMD_APP}" 
                        "run"
                        "${OFBUILD_TESTS_INPUT_DATASETS_DIR}/OPENFLUID.IN.CSVObserverPrecision"
                        "${OFBUILD_TESTS_OUTPUT_DATA_DIR}/OPENFLUID.OUT.CSVObserverPrecision" 
                        "-p" "${OFBUILD_TESTS_BINARY_DIR}"
                        "-n" "${OFBUILD_TESTS_BINARY_DIR}"
                   PRE_TEST REMOVE_DIRECTORY "${OFBUILD_TESTS_OUTPUT_DATA_DIR}/OPENFLUID.OUT.CSVObserverPrecision"
                   POST_TEST CHECK_FILE_EXIST "${OFBUILD_TESTS_OUTPUT_DATA_DIR}/OPENFLUID.OUT.CSVObserverPrecision/MC_sciTU1.csv" 
                             CHECK_FILE_EXIST "${OFBUILD_TESTS_OUTPUT_DATA_DIR}/OPENFLUID.OUT.CSVObserverPrecision/MC_customTU1.csv"
                             CHECK_FILE_EXIST "${OFBUILD_TESTS_OUTPUT_DATA_DIR}/OPENFLUID.OUT.CSVObserverPrecision/default_TU1_prec.bigger.csv"
                             CHECK_FILE_EXIST "${OFBUILD_TESTS_OUTPUT_DATA_DIR}/OPENFLUID.OUT.CSVObserverPrecision/default_TU1_prec.smaller.csv"
                             CHECK_FILE_EXIST "${OFBUILD_TESTS_OUTPUT_DATA_DIR}/OPENFLUID.OUT.CSVObserverPrecision/fixed_TU1_prec.bigger.csv"
                             CHECK_FILE_EXIST "${OFBUILD_TESTS_OUTPUT_DATA_DIR}/OPENFLUID.OUT.CSVObserverPrecision/fixed_TU1_prec.smaller.csv"
                             CHECK_FILE_EXIST "${OFBUILD_TESTS_OUTPUT_DATA_DIR}/OPENFLUID.OUT.CSVObserverPrecision/sci_TU1_prec.bigger.csv"
                             CHECK_FILE_EXIST "${OFBUILD_TESTS_OUTPUT_DATA_DIR}/OPENFLUID.OUT.CSVObserverPrecision/sci_TU1_prec.smaller.csv"
                             CHECK_FILE_EXIST "${OFBUILD_TESTS_OUTPUT_DATA_DIR}/OPENFLUID.OUT.CSVObserverPrecision/autofloat_TU1_prec.bigger.csv"
                             CHECK_FILE_EXIST "${OFBUILD_TESTS_OUTPUT_DATA_DIR}/OPENFLUID.OUT.CSVObserverPrecision/autofloat_TU1_prec.smaller.csv"
                             
                   )


###########################################################################


OPENFLUID_ADD_TEST(NAME observers-DotFiles 
                   COMMAND "${OFBUILD_DIST_BIN_DIR}/${OPENFLUID_CMD_APP}" 
                        "run"
                        "${OFBUILD_TESTS_INPUT_DATASETS_DIR}/OPENFLUID.IN.DotObserver"
                        "${OFBUILD_TESTS_OUTPUT_DATA_DIR}/OPENFLUID.OUT.DotObserver" 
                        "-p" "${OFBUILD_TESTS_BINARY_DIR}"
                        "-n" "${OFBUILD_TESTS_BINARY_DIR}"
                    PRE_TEST REMOVE_DIRECTORY "${OFBUILD_TESTS_OUTPUT_DATA_DIR}/OPENFLUID.OUT.DotObserver"
                    POST_TEST CHECK_FILE_EXIST "${OFBUILD_TESTS_OUTPUT_DATA_DIR}/OPENFLUID.OUT.DotObserver/spatial-graph_final.dot"
                              CHECK_FILE_EXIST "${OFBUILD_TESTS_OUTPUT_DATA_DIR}/OPENFLUID.OUT.DotObserver/graph_vars_init.dot"
                              CHECK_FILE_EXIST "${OFBUILD_TESTS_OUTPUT_DATA_DIR}/OPENFLUID.OUT.DotObserver/graph_vars_final.dot"
                              CHECK_FILE_EXIST "${OFBUILD_TESTS_OUTPUT_DATA_DIR}/OPENFLUID.OUT.DotObserver/graph_attrs_t3600.dot"
                              CHECK_FILE_EXIST "${OFBUILD_TESTS_OUTPUT_DATA_DIR}/OPENFLUID.OUT.DotObserver/graph_full_init.dot"
                              CHECK_FILE_EXIST "${OFBUILD_TESTS_OUTPUT_DATA_DIR}/OPENFLUID.OUT.DotObserver/graph_full_final.dot"
                              CHECK_FILE_EXIST "${OFBUILD_TESTS_OUTPUT_DATA_DIR}/OPENFLUID.OUT.DotObserver/graph_full_t10800.dot"
                   )


###########################################################################

                        
OPENFLUID_ADD_TEST(NAME observers-KmlFiles 
                   COMMAND "${OFBUILD_DIST_BIN_DIR}/${OPENFLUID_CMD_APP}" 
                        "run"
                        "${OFBUILD_TESTS_INPUT_DATASETS_DIR}/OPENFLUID.IN.KmlObservers"
                        "${OFBUILD_TESTS_OUTPUT_DATA_DIR}/OPENFLUID.OUT.KmlObservers" 
                        "-p" "${OFBUILD_TESTS_BINARY_DIR}"
                        "-n" "${OFBUILD_TESTS_BINARY_DIR}"
                   PRE_TEST REMOVE_DIRECTORY "${OFBUILD_TESTS_OUTPUT_DATA_DIR}/OPENFLUID.OUT.KmlObservers"
                   POST_TEST CHECK_FILE_EXIST "${OFBUILD_TESTS_OUTPUT_DATA_DIR}/OPENFLUID.OUT.KmlObservers/kmlplot.kmz"
                             CHECK_FILE_EXIST "${OFBUILD_TESTS_OUTPUT_DATA_DIR}/OPENFLUID.OUT.KmlObservers/SU.kmz"
                             CHECK_FILE_EXIST "${OFBUILD_TESTS_OUTPUT_DATA_DIR}/OPENFLUID.OUT.KmlObservers/RS.kmz"
                  )


###########################################################################

                        
OPENFLUID_ADD_TEST(NAME observers-KmlAnimFilesMinDelay 
                   COMMAND "${OFBUILD_DIST_BIN_DIR}/${OPENFLUID_CMD_APP}" 
                        "run"
                        "${OFBUILD_TESTS_INPUT_DATASETS_DIR}/OPENFLUID.IN.KmlAnimObserverMinDelay"
                        "${OFBUILD_TESTS_OUTPUT_DATA_DIR}/OPENFLUID.OUT.KmlAnimObserverMinDelay" 
                        "-p" "${OFBUILD_TESTS_BINARY_DIR}"
                        "-n" "${OFBUILD_TESTS_BINARY_DIR}"
                   PRE_TEST REMOVE_DIRECTORY "${OFBUILD_TESTS_OUTPUT_DATA_DIR}/OPENFLUID.OUT.KmlAnimObserverMinDelay"
                   POST_TEST CHECK_FILE_EXIST "${OFBUILD_TESTS_OUTPUT_DATA_DIR}/OPENFLUID.OUT.KmlAnimObserverMinDelay/SU-mindelay.kmz"
                             CHECK_FILE_EXIST "${OFBUILD_TESTS_OUTPUT_DATA_DIR}/OPENFLUID.OUT.KmlAnimObserverMinDelay/RS-mindelay.kmz"
                  )

###########################################################################

                        
OPENFLUID_ADD_TEST(NAME observers-GNUplot
                   COMMAND "${OFBUILD_DIST_BIN_DIR}/${OPENFLUID_CMD_APP}" 
                        "run"
                        "${OFBUILD_TESTS_INPUT_DATASETS_DIR}/OPENFLUID.IN.GNUplotObserver"
                        "${OFBUILD_TESTS_OUTPUT_DATA_DIR}/OPENFLUID.OUT.GNUplotObserver" 
                        "-p" "${OFBUILD_TESTS_BINARY_DIR}"
                        "-n" "${OFBUILD_TESTS_BINARY_DIR}"
                   PRE_TEST REMOVE_DIRECTORY "${OFBUILD_TESTS_OUTPUT_DATA_DIR}/OPENFLUID.OUT.GNUplotObserver"
                   POST_TEST CHECK_FILE_EXIST "${OFBUILD_TESTS_OUTPUT_DATA_DIR}/OPENFLUID.OUT.GNUplotObserver/script.gnuplot"
                             CHECK_FILE_EXIST "${OFBUILD_TESTS_OUTPUT_DATA_DIR}/OPENFLUID.OUT.GNUplotObserver/s1_data.gnuplot"
                             CHECK_FILE_EXIST "${OFBUILD_TESTS_OUTPUT_DATA_DIR}/OPENFLUID.OUT.GNUplotObserver/s2_data.gnuplot"
                             CHECK_FILE_EXIST "${OFBUILD_TESTS_OUTPUT_DATA_DIR}/OPENFLUID.OUT.GNUplotObserver/s4_data.gnuplot"
                  )
                   
                   
###########################################################################

                        
OPENFLUID_ADD_TEST(NAME observers-GDALGeoVector
                   COMMAND "${OFBUILD_DIST_BIN_DIR}/${OPENFLUID_CMD_APP}" 
                        "run"
                        "${OFBUILD_TESTS_INPUT_DATASETS_DIR}/OPENFLUID.IN.GDALGeoVector"
                        "${OFBUILD_TESTS_OUTPUT_DATA_DIR}/OPENFLUID.OUT.GDALGeoVector" 
                        "-p" "${OFBUILD_TESTS_BINARY_DIR}"
                        "-n" "${OFBUILD_TESTS_BINARY_DIR}"
                   PRE_TEST REMOVE_DIRECTORY "${OFBUILD_TESTS_OUTPUT_DATA_DIR}/OPENFLUID.OUT.GDALGeoVector"
                   POST_TEST CHECK_FILE_EXIST "${OFBUILD_TESTS_OUTPUT_DATA_DIR}/OPENFLUID.OUT.GDALGeoVector/ContDelaySU_init.shp"
                             CHECK_FILE_EXIST "${OFBUILD_TESTS_OUTPUT_DATA_DIR}/OPENFLUID.OUT.GDALGeoVector/geovector-continuous/ContSU_252000.shp"
                             CHECK_FILE_EXIST "${OFBUILD_TESTS_OUTPUT_DATA_DIR}/OPENFLUID.OUT.GDALGeoVector/geovector-continuous-delay/ContDelayRS_324000.json"
                  )                             