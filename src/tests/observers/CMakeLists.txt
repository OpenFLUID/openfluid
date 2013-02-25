CMAKE_MINIMUM_REQUIRED (VERSION 2.6)


ADD_DEFINITIONS(-DOPENFLUID_VERSION=${FULL_VERSION})
ADD_DEFINITIONS(-DBUILDINGDLL)

INCLUDE_DIRECTORIES("${PROJECT_SOURCE_DIR}/src" "${PROJECT_BINARY_DIR}/src")

ADD_OBSERVER(tests.empty tests.empty ${TEST_OUTPUT_PATH})

ADD_OBSERVER(tests.hopla tests.hopla ${TEST_OUTPUT_PATH})

ADD_TEST(observers-Hopla "${BIN_OUTPUT_PATH}/openfluid-engine" 
                                 "-i${TESTS_DATASETS_PATH}/OPENFLUID.IN.PrimitivesHopla"
                                 "-o${TESTS_OUTPUTDATA_PATH}/OPENFLUID.OUT.PrimitivesHopla" 
                                 "-p${TEST_OUTPUT_PATH}"
                                 "-n${TEST_OUTPUT_PATH}")
                                 
ADD_TEST(observers-CSVFiles "${BIN_OUTPUT_PATH}/openfluid-engine" 
                        "-i${TESTS_DATASETS_PATH}/OPENFLUID.IN.CSVObserver"
                        "-o${TESTS_OUTPUTDATA_PATH}/OPENFLUID.OUT.CSVObserver" 
                        "-p${TEST_OUTPUT_PATH}"
                        "-n${TEST_OUTPUT_PATH}")
                        
ADD_TEST(observers-DotFiles "${BIN_OUTPUT_PATH}/openfluid-engine" 
                        "-i${TESTS_DATASETS_PATH}/OPENFLUID.IN.DotObserver"
                        "-o${TESTS_OUTPUTDATA_PATH}/OPENFLUID.OUT.DotObserver" 
                        "-p${TEST_OUTPUT_PATH}"
                        "-n${TEST_OUTPUT_PATH}")
                        
ADD_TEST(observers-KmlFiles "${BIN_OUTPUT_PATH}/openfluid-engine" 
                        "-i${TESTS_DATASETS_PATH}/OPENFLUID.IN.KmlObservers"
                        "-o${TESTS_OUTPUTDATA_PATH}/OPENFLUID.OUT.KmlObservers" 
                        "-p${TEST_OUTPUT_PATH}"
                        "-n${TEST_OUTPUT_PATH}")
                        
ADD_TEST(observers-VtkFiles "${BIN_OUTPUT_PATH}/openfluid-engine" 
                        "-i${TESTS_DATASETS_PATH}/OPENFLUID.IN.VtkObserver"
                        "-o${TESTS_OUTPUTDATA_PATH}/OPENFLUID.OUT.VtkObserver" 
                        "-p${TEST_OUTPUT_PATH}"
                        "-n${TEST_OUTPUT_PATH}")                        
                                                