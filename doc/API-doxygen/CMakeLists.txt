################### openfluid api documentation ###################

IF(DOXYGEN)
  
  CONFIGURE_FILE(Doxyfile.in ${PROJECT_BINARY_DIR}/API-Doxyfile)

  FILE(MAKE_DIRECTORY ${APIDOC_OUTPUT_PATH})

  # TODO A voir autrement    
  CONFIGURE_FILE(openfluid.png ${APIDOC_OUTPUT_PATH}/html/openfluid.png COPYONLY)
  CONFIGURE_FILE(API_stack.png ${APIDOC_OUTPUT_PATH}/html/API_stack.png COPYONLY)
    
  ADD_CUSTOM_COMMAND(
    OUTPUT "${APIDOC_OUTPUT_PATH}/html/index.html"
    DEPENDS "${PROJECT_BINARY_DIR}/API-Doxyfile"
    COMMAND "${DOXYGEN}"
    ARGS "${PROJECT_BINARY_DIR}/API-Doxyfile")

  ADD_CUSTOM_TARGET(openfluid-api-doc ALL DEPENDS "${PROJECT_BINARY_DIR}/API-Doxyfile"
    "${APIDOC_OUTPUT_PATH}/html/index.html" VERBATIM)


  INSTALL(DIRECTORY "${APIDOC_OUTPUT_PATH}/"
          DESTINATION "${API_DOC_INSTALL_PATH}")

ENDIF(DOXYGEN)
