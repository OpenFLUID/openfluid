

IF(DOXYGEN)

  CONFIGURE_FILE(HTML-Doxyfile.in HTML-Doxyfile)

  FILE(MAKE_DIRECTORY "${DOC_MAIN_OUTPUT_PATH}")

  # TODO A voir autrement    
  CONFIGURE_FILE(${DOC_CONTENTS_DIR}/openfluid.png ${MAIN_DOC_OUTPUT_PATH}/html/openfluid.png COPYONLY)
  CONFIGURE_FILE(${DOC_CONTENTS_DIR}/API_stack.png ${MAIN_DOC_OUTPUT_PATH}/html/API_stack.png COPYONLY)
 
    
  ADD_CUSTOM_COMMAND(
    OUTPUT "${MAIN_DOC_OUTPUT_PATH}/html/index.html"
    DEPENDS "${CMAKE_CURRENT_BINARY_DIR}/HTML-Doxyfile"
    COMMAND "${DOXYGEN}"
    ARGS "${CMAKE_CURRENT_BINARY_DIR}/HTML-Doxyfile")

  ADD_CUSTOM_TARGET(openfluid-html-doc ALL DEPENDS "${CMAKE_CURRENT_BINARY_DIR}/HTML-Doxyfile"
    "${MAIN_DOC_OUTPUT_PATH}/html/index.html" VERBATIM)


  INSTALL(DIRECTORY "${MAIN_DOC_OUTPUT_PATH}/"
          DESTINATION "${MAIN_DOC_INSTALL_PATH}")
          
          
  IF(PDFLATEX_COMPILER)
    CONFIGURE_FILE(LaTeX-Doxyfile.in LaTeX-Doxyfile)
    
    STRING(REPLACE "~" "$\\sim$" LATEX_FULL_VERSION "${FULL_VERSION}")

    FILE(MAKE_DIRECTORY "${DOC_BUILD_DIR}")
    FILE(COPY "${CMAKE_CURRENT_SOURCE_DIR}/contents/" 
       DESTINATION "${DOC_BUILD_DIR}/latex"
       PATTERN "*.png"
       PATTERN "*.pdf")
    FILE(COPY "${CMAKE_CURRENT_SOURCE_DIR}/layout/" 
       DESTINATION "${DOC_BUILD_DIR}/latex"
       PATTERN "*.sty")
   
    CONFIGURE_FILE(layout/main.tex.in "${DOC_BUILD_DIR}/latex/main.tex")
   
    ADD_CUSTOM_COMMAND(
      OUTPUT "${DOC_BUILD_DIR}/latex/main.pdf"
      DEPENDS "${CMAKE_CURRENT_BINARY_DIR}/LaTeX-Doxyfile"
      COMMAND "${DOXYGEN}"
      ARGS "${CMAKE_CURRENT_BINARY_DIR}/LaTeX-Doxyfile"
      COMMAND ${CMAKE_COMMAND} -E chdir ${DOC_BUILD_DIR}/latex ${PDFLATEX_COMPILER}
      ARGS "main.tex"       
      COMMAND ${CMAKE_COMMAND} -E chdir ${DOC_BUILD_DIR}/latex ${PDFLATEX_COMPILER}
      ARGS "main.tex"
      COMMAND ${CMAKE_COMMAND} -E chdir ${DOC_BUILD_DIR}/latex ${PDFLATEX_COMPILER}
      ARGS "main.tex")

  ADD_CUSTOM_COMMAND(    
    OUTPUT  "${MAIN_DOC_OUTPUT_PATH}/openfluid_manual_${FULL_VERSION}.pdf"
    DEPENDS "${DOC_BUILD_DIR}/latex/main.pdf"      
    COMMAND ${CMAKE_COMMAND} -E copy "${DOC_BUILD_DIR}/latex/main.pdf" "${MAIN_DOC_OUTPUT_PATH}/openfluid_manual_${FULL_VERSION}.pdf"       
    )  


    ADD_CUSTOM_TARGET(openfluid-pdf-doc ALL DEPENDS "${MAIN_DOC_OUTPUT_PATH}/openfluid_manual_${FULL_VERSION}.pdf" VERBATIM)   

  ENDIF() 
    
ENDIF()

