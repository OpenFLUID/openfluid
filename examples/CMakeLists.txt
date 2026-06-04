
FILE(COPY "${OFBUILD_SOURCE_EXAMPLES_WARESDEV_DIR}" 
     DESTINATION "${OFBUILD_DIST_DIR}/${OPENFLUID_EXAMPLES_INSTALL_PATH}")
FILE(COPY "${OFBUILD_SOURCE_EXAMPLES_PROJECTS_DIR}" 
     DESTINATION "${OFBUILD_DIST_DIR}/${OPENFLUID_EXAMPLES_INSTALL_PATH}")

ADD_SUBDIRECTORY(wares-dev)


OPENFLUID_ADD_TEST(NAME examples-Primitives 
                   COMMAND "${OFBUILD_DIST_BIN_DIR}/${OPENFLUID_CMD_APP}" 
                              "run"
                              "${CMAKE_CURRENT_SOURCE_DIR}/projects/Primitives/IN"
                              "${OFBUILD_TESTS_OUTPUT_DATA_DIR}/OPENFLUID.OUT.Examples.Primitives" 
                    PRE_TEST REMOVE_DIRECTORY "${OFBUILD_TESTS_OUTPUT_DATA_DIR}/OPENFLUID.OUT.Primitives"
                    POST_TEST CHECK_FILE_EXIST "${OFBUILD_TESTS_OUTPUT_DATA_DIR}/OPENFLUID.OUT.Examples.Primitives/openfluid-messages.log"
                              COMPARE_DIRECTORIES "${OFBUILD_TESTS_OUTPUT_DATA_DIR}/OPENFLUID.OUT.Examples.Primitives" 
                                                  "${CMAKE_CURRENT_SOURCE_DIR}/projects/Primitives/REF"
                                                  "basic_output_check"
                   )

OPENFLUID_ADD_TEST(NAME examples-Manhattan 
                   COMMAND "${OFBUILD_DIST_BIN_DIR}/${OPENFLUID_CMD_APP}" 
                              "run"
                              "${CMAKE_CURRENT_SOURCE_DIR}/projects/Manhattan/IN"
                              "${OFBUILD_TESTS_OUTPUT_DATA_DIR}/OPENFLUID.OUT.Examples.Manhattan" 
                    PRE_TEST REMOVE_DIRECTORY "${OFBUILD_TESTS_OUTPUT_DATA_DIR}/OPENFLUID.OUT.Manhattan"
                    POST_TEST CHECK_FILE_EXIST "${OFBUILD_TESTS_OUTPUT_DATA_DIR}/OPENFLUID.OUT.Examples.Manhattan/openfluid-messages.log"
                              CHECK_FILE_EXIST "${OFBUILD_TESTS_OUTPUT_DATA_DIR}/OPENFLUID.OUT.Examples.Manhattan/examples.RU11.stock.pdf"
                              COMPARE_DIRECTORIES "${OFBUILD_TESTS_OUTPUT_DATA_DIR}/OPENFLUID.OUT.Examples.Manhattan" 
                                                  "${CMAKE_CURRENT_SOURCE_DIR}/projects/Manhattan/REF"
                                                  "basic_output_check"
                   )

OPENFLUID_ADD_TEST(NAME examples-Firespread 
                   HEAVY
                   COMMAND "${OFBUILD_DIST_BIN_DIR}/${OPENFLUID_CMD_APP}" 
                              "run"
                              "${CMAKE_CURRENT_SOURCE_DIR}/projects/Firespread/IN"
                              "${OFBUILD_TESTS_OUTPUT_DATA_DIR}/OPENFLUID.OUT.Examples.Firespread"
                    PRE_TEST REMOVE_DIRECTORY "${OFBUILD_TESTS_OUTPUT_DATA_DIR}/OPENFLUID.OUT.Firespread"
                    POST_TEST CHECK_FILE_EXIST "${OFBUILD_TESTS_OUTPUT_DATA_DIR}/OPENFLUID.OUT.Examples.Firespread/openfluid-messages.log"
                              CHECK_FILE_EXIST "${OFBUILD_TESTS_OUTPUT_DATA_DIR}/OPENFLUID.OUT.Examples.Firespread/firespread-anim.kmz"
                   )

OPENFLUID_ADD_TEST(NAME examples-Firespread-seeded
                   HEAVY
                   COMMAND "${OFBUILD_DIST_BIN_DIR}/${OPENFLUID_CMD_APP}" 
                              "run"
                              "${CMAKE_CURRENT_SOURCE_DIR}/projects/Firespread/IN_seeded"
                              "${OFBUILD_TESTS_OUTPUT_DATA_DIR}/OPENFLUID.OUT.Examples.Firespread_seeded"
                    PRE_TEST REMOVE_DIRECTORY "${OFBUILD_TESTS_OUTPUT_DATA_DIR}/OPENFLUID.OUT.Firespread_seeded"
                    POST_TEST CHECK_FILE_EXIST "${OFBUILD_TESTS_OUTPUT_DATA_DIR}/OPENFLUID.OUT.Examples.Firespread_seeded/openfluid-messages.log"
                              COMPARE_DIRECTORIES "${OFBUILD_TESTS_OUTPUT_DATA_DIR}/OPENFLUID.OUT.Examples.Firespread_seeded" 
                                                  "${CMAKE_CURRENT_SOURCE_DIR}/projects/Firespread/REF_seeded"
                                                  "basic_output_check"
                   )

OPENFLUID_ADD_TEST(NAME examples-Firespread-reduced 
                   COMMAND "${OFBUILD_DIST_BIN_DIR}/${OPENFLUID_CMD_APP}" 
                              "run"
                              "${CMAKE_SOURCE_DIR}/resources/tests/datasets/OPENFLUID.IN.FirespreadReduced"
                              "${OFBUILD_TESTS_OUTPUT_DATA_DIR}/OPENFLUID.OUT.FirespreadReduced"
                    PRE_TEST REMOVE_DIRECTORY "${OFBUILD_TESTS_OUTPUT_DATA_DIR}/OPENFLUID.OUT.FirespreadReduced"
                    POST_TEST CHECK_FILE_EXIST "${OFBUILD_TESTS_OUTPUT_DATA_DIR}/OPENFLUID.OUT.FirespreadReduced/openfluid-messages.log"
                              COMPARE_DIRECTORIES "${OFBUILD_TESTS_OUTPUT_DATA_DIR}/OPENFLUID.OUT.FirespreadReduced" 
                              "${CMAKE_SOURCE_DIR}/resources/tests/refs/OPENFLUID.REF.FirespreadReduced"
                              "basic_output_check"
                   )

OPENFLUID_ADD_TEST(NAME examples-MhydasRoujan
                   COMMAND "${OFBUILD_DIST_BIN_DIR}/${OPENFLUID_CMD_APP}" 
                              "run"
                              "${CMAKE_CURRENT_SOURCE_DIR}/projects/MHYDAS_Roujan/IN"
                              "${OFBUILD_TESTS_OUTPUT_DATA_DIR}/OPENFLUID.OUT.Examples.MHYDAS_Roujan"
                    PRE_TEST REMOVE_DIRECTORY "${OFBUILD_TESTS_OUTPUT_DATA_DIR}/OPENFLUID.OUT.Examples.MHYDAS_Roujan"
                    POST_TEST CHECK_FILE_EXIST "${OFBUILD_TESTS_OUTPUT_DATA_DIR}/OPENFLUID.OUT.Examples.MHYDAS_Roujan/openfluid-messages.log"
                              COMPARE_DIRECTORIES "${OFBUILD_TESTS_OUTPUT_DATA_DIR}/OPENFLUID.OUT.Examples.MHYDAS_Roujan" 
                                                  "${CMAKE_CURRENT_SOURCE_DIR}/projects/MHYDAS_Roujan/REF"
                                                  "basic_output_check"
                   )  

 INSTALL(DIRECTORY "${OFBUILD_DIST_DIR}/${OPENFLUID_EXAMPLES_INSTALL_PATH}" 
         DESTINATION "${OPENFLUID_DOC_INSTALL_PATH}/")

