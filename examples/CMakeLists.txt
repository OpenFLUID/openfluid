

ADD_SUBDIRECTORY(functions)

INCLUDE(OpenFLUIDHelpers)

FILE(COPY "${EXAMPLES_PROJECTS_PATH}" 
     DESTINATION "${BUILD_OUTPUT_PATH}/${EXAMPLES_INSTALL_PATH}")
       
OPENFLUID_ADD_TEST(NAME examples-Primitives 
                   COMMAND "${BIN_OUTPUT_PATH}/${OPENFLUID_CMD_APP}" 
                              "-i" "${CMAKE_CURRENT_SOURCE_DIR}/projects/Primitives/IN"
                              "-o" "${TESTS_OUTPUTDATA_PATH}/OPENFLUID.OUT.Examples.Primitives" 
                    PRE_TEST REMOVE_DIRECTORY "${TESTS_OUTPUTDATA_PATH}/OPENFLUID.OUT.Primitives"
                    POST_TEST CHECK_FILE_EXIST "${TESTS_OUTPUTDATA_PATH}/OPENFLUID.OUT.Examples.Primitives/openfluid-messages.log"
                              CHECK_FILE_EXIST "${TESTS_OUTPUTDATA_PATH}/OPENFLUID.OUT.Examples.Primitives/examples.primitives.unitsA.prod_ofware-func.log"
                              CHECK_FILE_EXIST "${TESTS_OUTPUTDATA_PATH}/OPENFLUID.OUT.Examples.Primitives/examples.primitives.unitsB.prod_ofware-func.log"
                              CHECK_FILE_EXIST "${TESTS_OUTPUTDATA_PATH}/OPENFLUID.OUT.Examples.Primitives/fullA_unitsA1_var1.csv"
                              CHECK_FILE_EXIST "${TESTS_OUTPUTDATA_PATH}/OPENFLUID.OUT.Examples.Primitives/fullB_unitsB2_var5.csv"
                   )
                   
OPENFLUID_ADD_TEST(NAME examples-Manahattan 
                   COMMAND "${BIN_OUTPUT_PATH}/${OPENFLUID_CMD_APP}" 
                              "-i" "${CMAKE_CURRENT_SOURCE_DIR}/projects/Manhattan/IN"
                              "-o" "${TESTS_OUTPUTDATA_PATH}/OPENFLUID.OUT.Examples.Manhattan" 
                    PRE_TEST REMOVE_DIRECTORY "${TESTS_OUTPUTDATA_PATH}/OPENFLUID.OUT.Manhattan"
                    POST_TEST CHECK_FILE_EXIST "${TESTS_OUTPUTDATA_PATH}/OPENFLUID.OUT.Examples.Manhattan/openfluid-messages.log"
                              CHECK_FILE_EXIST "${TESTS_OUTPUTDATA_PATH}/OPENFLUID.OUT.Examples.Manhattan/kmlanim.kmz"
                   )                   
                   

INSTALL(DIRECTORY "projects/" 
        DESTINATION "${EXAMPLES_PROJECTS_INSTALL_PATH}" PATTERN ".svn" EXCLUDE)