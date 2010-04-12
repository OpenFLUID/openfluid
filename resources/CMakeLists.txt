


  # examples installation
  INSTALL(DIRECTORY "examples/src" 
          DESTINATION "${EXAMPLES_INSTALL_PATH}" PATTERN ".svn" EXCLUDE)
  
  INSTALL(DIRECTORY "examples/datasets" 
          DESTINATION "${EXAMPLES_INSTALL_PATH}" PATTERN ".svn" EXCLUDE)