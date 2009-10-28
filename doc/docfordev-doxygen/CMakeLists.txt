# Doxygen-built docs for core developers

IF(DOXYGEN AND BUILD_DOCFORDEV)
  
  MESSAGE(STATUS "Build Doxygen documentation for core developers : yes")
  
  CONFIGURE_FILE(Doxyfile.in ${PROJECT_BINARY_DIR}/docfordev-Doxyfile)

  FILE(MAKE_DIRECTORY ${DOCFORDEV_OUTPUT_PATH})

  CONFIGURE_FILE(${APIDOC_DOCFILES_DIR}/openfluid.png ${DOCFORDEV_OUTPUT_PATH}/html/openfluid.png COPYONLY)
  
  ADD_CUSTOM_COMMAND(
    OUTPUT "${DOCFORDEV_OUTPUT_PATH}/html/index.html"
    DEPENDS "${PROJECT_BINARY_DIR}/docfordev-Doxyfile"
    COMMAND "${DOXYGEN}"
    ARGS "${PROJECT_BINARY_DIR}/docfordev-Doxyfile")

  ADD_CUSTOM_TARGET(docfordev-doc DEPENDS "${PROJECT_BINARY_DIR}/docfordev-Doxyfile"
    "${DOCFORDEV_OUTPUT_PATH}/html/index.html" VERBATIM)


ELSE(DOXYGEN AND BUILD_DOCFORDEV)
  MESSAGE(STATUS "Build Doxygen documentation for core developers : no")
ENDIF(DOXYGEN AND BUILD_DOCFORDEV)
