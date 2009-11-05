

  # distribution files installation
  INSTALL(FILES "dist/copyright"
                "dist/AUTHORS"
                "dist/README"   
                "dist/CHANGELOG"
          DESTINATION ${DOC_INSTALL_PATH} COMPONENT engine)
  
  INSTALL(FILES "dist/copyright"
                "dist/AUTHORS"
                "dist/README"
                "dist/CHANGELOG"
          DESTINATION ${DOC_INSTALL_PATH} COMPONENT engine-wodev)

  # examples installation
  INSTALL(DIRECTORY "examples/src" 
          DESTINATION "${EXAMPLES_INSTALL_PATH}" COMPONENT engine PATTERN ".svn" EXCLUDE)
  
  INSTALL(DIRECTORY "examples/datasets" 
          DESTINATION "${EXAMPLES_INSTALL_PATH}" COMPONENT engine PATTERN ".svn" EXCLUDE)