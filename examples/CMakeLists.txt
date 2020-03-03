
FILE(COPY "${OFBUILD_SOURCE_EXAMPLES_WARESDEV_DIR}" 
     DESTINATION "${OFBUILD_DIST_DIR}/${OPENFLUID_EXAMPLES_INSTALL_PATH}" PATTERN "CMakeLists.txt" EXCLUDE)
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
                              CHECK_FILE_EXIST "${OFBUILD_TESTS_OUTPUT_DATA_DIR}/OPENFLUID.OUT.Examples.Primitives/fullA_unitsA1_var1.csv"
                              CHECK_FILE_EXIST "${OFBUILD_TESTS_OUTPUT_DATA_DIR}/OPENFLUID.OUT.Examples.Primitives/fullB_unitsB2_var5.csv"
                   )
                   
                   
OPENFLUID_ADD_TEST(NAME examples-Manhattan 
                   COMMAND "${OFBUILD_DIST_BIN_DIR}/${OPENFLUID_CMD_APP}" 
                              "run"
                              "${CMAKE_CURRENT_SOURCE_DIR}/projects/Manhattan/IN"
                              "${OFBUILD_TESTS_OUTPUT_DATA_DIR}/OPENFLUID.OUT.Examples.Manhattan" 
                    PRE_TEST REMOVE_DIRECTORY "${OFBUILD_TESTS_OUTPUT_DATA_DIR}/OPENFLUID.OUT.Manhattan"
                    POST_TEST CHECK_FILE_EXIST "${OFBUILD_TESTS_OUTPUT_DATA_DIR}/OPENFLUID.OUT.Examples.Manhattan/openfluid-messages.log"
                              CHECK_FILE_EXIST "${OFBUILD_TESTS_OUTPUT_DATA_DIR}/OPENFLUID.OUT.Examples.Manhattan/manhattan-anim.kmz"
                              CHECK_FILE_EXIST "${OFBUILD_TESTS_OUTPUT_DATA_DIR}/OPENFLUID.OUT.Examples.Manhattan/examples.RU11.stock.pdf"
                              
                   )

                   
OPENFLUID_ADD_TEST(NAME examples-Firespread 
                   COMMAND "${OFBUILD_DIST_BIN_DIR}/${OPENFLUID_CMD_APP}" 
                              "run"
                              "${CMAKE_CURRENT_SOURCE_DIR}/projects/Firespread/IN"
                              "${OFBUILD_TESTS_OUTPUT_DATA_DIR}/OPENFLUID.OUT.Examples.Firespread"
                    PRE_TEST REMOVE_DIRECTORY "${OFBUILD_TESTS_OUTPUT_DATA_DIR}/OPENFLUID.OUT.Firespread"
                    POST_TEST CHECK_FILE_EXIST "${OFBUILD_TESTS_OUTPUT_DATA_DIR}/OPENFLUID.OUT.Examples.Firespread/openfluid-messages.log"
                              CHECK_FILE_EXIST "${OFBUILD_TESTS_OUTPUT_DATA_DIR}/OPENFLUID.OUT.Examples.Firespread/firespread-anim.kmz"                                                            
                   )                   
                                                       

OPENFLUID_ADD_TEST(NAME examples-MhydasRoujan
                   COMMAND "${OFBUILD_DIST_BIN_DIR}/${OPENFLUID_CMD_APP}" 
                              "run"
                              "${CMAKE_CURRENT_SOURCE_DIR}/projects/MHYDAS_Roujan/IN"
                              "${OFBUILD_TESTS_OUTPUT_DATA_DIR}/OPENFLUID.OUT.Examples.MHYDAS_Roujan"
                    PRE_TEST REMOVE_DIRECTORY "${OFBUILD_TESTS_OUTPUT_DATA_DIR}/OPENFLUID.OUT.MHYDAS_Roujan"
                    POST_TEST CHECK_FILE_EXIST "${OFBUILD_TESTS_OUTPUT_DATA_DIR}/OPENFLUID.OUT.Examples.MHYDAS_Roujan/openfluid-messages.log"                                                           
                   )  


 INSTALL(DIRECTORY "${OFBUILD_DIST_DIR}/${OPENFLUID_EXAMPLES_INSTALL_PATH}" 
         DESTINATION "${OPENFLUID_DOC_INSTALL_PATH}/")


FILE(REMOVE ${OFBUILD_DIST_DIR}/${OPENFLUID_EXAMPLES_WARESDEV_INSTALL_PATH}/CMakeLists.txt)
FILE(REMOVE ${OFBUILD_DIST_DIR}/${OPENFLUID_EXAMPLES_WARESDEV_INSTALL_PATH}/simulators/CMakeLists.txt)