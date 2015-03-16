
ADD_DEFINITIONS(-DOPENFLUID_VERSION=${FULL_VERSION})

INCLUDE_DIRECTORIES("${PROJECT_SOURCE_DIR}/src" "${PROJECT_BINARY_DIR}/src")


OPNFLD_ADD_OBSERVER(tests.empty tests.empty ${TEST_OUTPUT_PATH} OFF)
OPNFLD_ADD_OBSERVER(tests.primitives.time tests.primitives.time ${TEST_OUTPUT_PATH} OFF)

###########################################################################


OPNFLD_ADD_OBSERVER(tests.paramswidget tests.paramswidget ${TEST_OUTPUT_PATH} ON)
                                                                                    

###########################################################################


OPNFLD_ADD_OBSERVER(tests.hopla tests.hopla ${TEST_OUTPUT_PATH} OFF)
OPENFLUID_ADD_TEST(NAME observers-Hopla 
                   COMMAND "${BIN_OUTPUT_PATH}/${OPENFLUID_CMD_APP}" 
                                 "-i${TESTS_DATASETS_PATH}/OPENFLUID.IN.PrimitivesHopla"
                                 "-o${TESTS_OUTPUTDATA_PATH}/OPENFLUID.OUT.PrimitivesHopla" 
                                 "-p${TEST_OUTPUT_PATH}"
                                 "-n${TEST_OUTPUT_PATH}"
                    PRE_TEST REMOVE_DIRECTORY "${TESTS_OUTPUTDATA_PATH}/OPENFLUID.OUT.PrimitivesHopla"
                   )


###########################################################################


OPNFLD_ADD_OBSERVER(tests.exceptions tests.exceptions ${TEST_OUTPUT_PATH} OFF)
OPENFLUID_ADD_TEST(NAME observers-Exceptions 
                   COMMAND "${BIN_OUTPUT_PATH}/${OPENFLUID_CMD_APP}" 
                           "-i${TESTS_DATASETS_PATH}/OPENFLUID.IN.ObsExceptions"
                           "-o${TESTS_OUTPUTDATA_PATH}/OPENFLUID.OUT.ObsExceptions" 
                           "-p${TEST_OUTPUT_PATH}"
                           "-n${TEST_OUTPUT_PATH}"
                    PRE_TEST REMOVE_DIRECTORY "${TESTS_OUTPUTDATA_PATH}/OPENFLUID.OUT.ObsExceptions"
                    POST_TEST CHECK_FILE_EXIST "${TESTS_OUTPUTDATA_PATH}/OPENFLUID.OUT.ObsExceptions/openfluid-messages.log"
                   )
SET_TESTS_PROPERTIES(observers-Exceptions PROPERTIES WILL_FAIL TRUE)


###########################################################################

                                 
OPENFLUID_ADD_TEST(NAME observers-CSVFiles 
                   COMMAND "${BIN_OUTPUT_PATH}/${OPENFLUID_CMD_APP}" 
                        "-i${TESTS_DATASETS_PATH}/OPENFLUID.IN.CSVObserver"
                        "-o${TESTS_OUTPUTDATA_PATH}/OPENFLUID.OUT.CSVObserver" 
                        "-p${TEST_OUTPUT_PATH}"
                        "-n${TEST_OUTPUT_PATH}"
                    PRE_TEST REMOVE_DIRECTORY "${TESTS_OUTPUTDATA_PATH}/OPENFLUID.OUT.CSVObserver"
                    POST_TEST CHECK_FILE_EXIST "${TESTS_OUTPUTDATA_PATH}/OPENFLUID.OUT.CSVObserver/fullfull_TestUnits1_tests.double.csv"
                              CHECK_FILE_EXIST "${TESTS_OUTPUTDATA_PATH}/OPENFLUID.OUT.CSVObserver/fullfull_TestUnits5_tests.double.csv"
                              CHECK_FILE_EXIST "${TESTS_OUTPUTDATA_PATH}/OPENFLUID.OUT.CSVObserver/some_TestUnits11_tests.string.csv"
                              CHECK_FILE_EXIST "${TESTS_OUTPUTDATA_PATH}/OPENFLUID.OUT.CSVObserver/someunits_TestUnits9_tests.matrix.dt.csv"
                              CHECK_FILE_EXIST "${TESTS_OUTPUTDATA_PATH}/OPENFLUID.OUT.CSVObserver/somevars_TestUnits7_tests.vector.csv"
                   )


###########################################################################

                        
OPENFLUID_ADD_TEST(NAME observers-DotFiles 
                   COMMAND "${BIN_OUTPUT_PATH}/${OPENFLUID_CMD_APP}" 
                        "-i${TESTS_DATASETS_PATH}/OPENFLUID.IN.DotObserver"
                        "-o${TESTS_OUTPUTDATA_PATH}/OPENFLUID.OUT.DotObserver" 
                        "-p${TEST_OUTPUT_PATH}"
                        "-n${TEST_OUTPUT_PATH}"
                    PRE_TEST REMOVE_DIRECTORY "${TESTS_OUTPUTDATA_PATH}/OPENFLUID.OUT.DotObserver"
                    POST_TEST CHECK_FILE_EXIST "${TESTS_OUTPUTDATA_PATH}/OPENFLUID.OUT.DotObserver/spatial-graph_t0.dot"
                              CHECK_FILE_EXIST "${TESTS_OUTPUTDATA_PATH}/OPENFLUID.OUT.DotObserver/spatial-graph_init.dot"
                              CHECK_FILE_EXIST "${TESTS_OUTPUTDATA_PATH}/OPENFLUID.OUT.DotObserver/spatial-graph_final.dot"
                   )


###########################################################################

                        
OPENFLUID_ADD_TEST(NAME observers-KmlFiles 
                   COMMAND "${BIN_OUTPUT_PATH}/${OPENFLUID_CMD_APP}" 
                        "-i${TESTS_DATASETS_PATH}/OPENFLUID.IN.KmlObservers"
                        "-o${TESTS_OUTPUTDATA_PATH}/OPENFLUID.OUT.KmlObservers" 
                        "-p${TEST_OUTPUT_PATH}"
                        "-n${TEST_OUTPUT_PATH}"
                   PRE_TEST REMOVE_DIRECTORY "${TESTS_OUTPUTDATA_PATH}/OPENFLUID.OUT.KmlObservers"
                   POST_TEST CHECK_FILE_EXIST "${TESTS_OUTPUTDATA_PATH}/OPENFLUID.OUT.KmlObservers/kmlplot.kmz"
                             CHECK_FILE_EXIST "${TESTS_OUTPUTDATA_PATH}/OPENFLUID.OUT.KmlObservers/SU.kmz"
                             CHECK_FILE_EXIST "${TESTS_OUTPUTDATA_PATH}/OPENFLUID.OUT.KmlObservers/RS.kmz"
                  )


###########################################################################

                        
OPENFLUID_ADD_TEST(NAME observers-KmlAnimFilesMinDelay 
                   COMMAND "${BIN_OUTPUT_PATH}/${OPENFLUID_CMD_APP}" 
                        "-i${TESTS_DATASETS_PATH}/OPENFLUID.IN.KmlAnimObserverMinDelay"
                        "-o${TESTS_OUTPUTDATA_PATH}/OPENFLUID.OUT.KmlAnimObserverMinDelay" 
                        "-p${TEST_OUTPUT_PATH}"
                        "-n${TEST_OUTPUT_PATH}"
                   PRE_TEST REMOVE_DIRECTORY "${TESTS_OUTPUTDATA_PATH}/OPENFLUID.OUT.KmlAnimObserverMinDelay"
                   POST_TEST CHECK_FILE_EXIST "${TESTS_OUTPUTDATA_PATH}/OPENFLUID.OUT.KmlAnimObserverMinDelay/SU-mindelay.kmz"
                             CHECK_FILE_EXIST "${TESTS_OUTPUTDATA_PATH}/OPENFLUID.OUT.KmlAnimObserverMinDelay/RS-mindelay.kmz"
                  )

###########################################################################

                        
OPENFLUID_ADD_TEST(NAME observers-GNUplot
                   COMMAND "${BIN_OUTPUT_PATH}/${OPENFLUID_CMD_APP}" 
                        "-i${TESTS_DATASETS_PATH}/OPENFLUID.IN.GNUplotObserver"
                        "-o${TESTS_OUTPUTDATA_PATH}/OPENFLUID.OUT.GNUplotObserver" 
                        "-p${TEST_OUTPUT_PATH}"
                        "-n${TEST_OUTPUT_PATH}"
                   PRE_TEST REMOVE_DIRECTORY "${TESTS_OUTPUTDATA_PATH}/OPENFLUID.OUT.GNUplotObserver"
                   POST_TEST CHECK_FILE_EXIST "${TESTS_OUTPUTDATA_PATH}/OPENFLUID.OUT.GNUplotObserver/script.gnuplot"
                             CHECK_FILE_EXIST "${TESTS_OUTPUTDATA_PATH}/OPENFLUID.OUT.GNUplotObserver/s1_data.gnuplot"
                             CHECK_FILE_EXIST "${TESTS_OUTPUTDATA_PATH}/OPENFLUID.OUT.GNUplotObserver/s2_data.gnuplot"
                             CHECK_FILE_EXIST "${TESTS_OUTPUTDATA_PATH}/OPENFLUID.OUT.GNUplotObserver/s4_data.gnuplot"
                  )
                   
                   
###########################################################################

                        
OPENFLUID_ADD_TEST(NAME observers-GDALGeoVector
                   COMMAND "${BIN_OUTPUT_PATH}/${OPENFLUID_CMD_APP}" 
                        "-i${TESTS_DATASETS_PATH}/OPENFLUID.IN.GDALGeoVector"
                        "-o${TESTS_OUTPUTDATA_PATH}/OPENFLUID.OUT.GDALGeoVector" 
                        "-p${TEST_OUTPUT_PATH}"
                        "-n${TEST_OUTPUT_PATH}"
                   PRE_TEST REMOVE_DIRECTORY "${TESTS_OUTPUTDATA_PATH}/OPENFLUID.OUT.GDALGeoVector"
                   POST_TEST CHECK_FILE_EXIST "${TESTS_OUTPUTDATA_PATH}/OPENFLUID.OUT.GDALGeoVector/ContDelaySU_init.shp"
                             CHECK_FILE_EXIST "${TESTS_OUTPUTDATA_PATH}/OPENFLUID.OUT.GDALGeoVector/geovector-continuous/ContSU_252000.shp"
                             CHECK_FILE_EXIST "${TESTS_OUTPUTDATA_PATH}/OPENFLUID.OUT.GDALGeoVector/geovector-continuous-delay/ContDelayRS_324000.json"
                  )                             