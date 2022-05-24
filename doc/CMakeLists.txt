

IF(DOXYGEN_FOUND)

  ADD_SUBDIRECTORY(snippets)

  FILE(MAKE_DIRECTORY "${OFBUILD_DIST_MAIN_DOC_DIR}")

  IF(OPENFLUID_GA_ID)
    CONFIGURE_FILE(layout/ga_snippet.js.in layout/ga_snippet.js @ONLY)
    FILE(READ ${CMAKE_CURRENT_BINARY_DIR}/layout/ga_snippet.js OFBUILD_GA_SNIPPET)
  ENDIF()

  CONFIGURE_FILE(HTML-Doxyfile.in HTML-Doxyfile)
  CONFIGURE_FILE(layout/header.html.in layout/header.html @ONLY)
  CONFIGURE_FILE(contents/mainpage.md.in contents/mainpage.md @ONLY)
  CONFIGURE_FILE(contents/foreword.md.in contents/foreword.md @ONLY)


  ADD_CUSTOM_COMMAND(
    OUTPUT "${OFBUILD_DIST_MAIN_DOC_DIR}/html/index.html"
    DEPENDS "${CMAKE_CURRENT_BINARY_DIR}/HTML-Doxyfile"
    COMMAND "${DOXYGEN}"
       ARGS "${CMAKE_CURRENT_BINARY_DIR}/HTML-Doxyfile")

  ADD_CUSTOM_TARGET(openfluid-doc-html ALL DEPENDS "${CMAKE_CURRENT_BINARY_DIR}/HTML-Doxyfile"
    "${OFBUILD_DIST_MAIN_DOC_DIR}/html/index.html" VERBATIM)


  IF(PDFLATEX_COMPILER)
    CONFIGURE_FILE(LaTeX-Doxyfile.in LaTeX-Doxyfile)

    STRING(REPLACE "~" "$\\sim$" OFBUILD_LATEX_VERSION_FULL "${OPENFLUID_VERSION_FULL}")

    FILE(MAKE_DIRECTORY "${OFBUILD_DOCBUILD_DIR}")
    FILE(COPY "${CMAKE_CURRENT_SOURCE_DIR}/contents/" 
       DESTINATION "${OFBUILD_DOCBUILD_DIR}/latex"
       FILES_MATCHING
       PATTERN "*.png"
       PATTERN "*.pdf")
    FILE(COPY "${CMAKE_CURRENT_SOURCE_DIR}/layout/" 
       DESTINATION "${OFBUILD_DOCBUILD_DIR}/latex"
       FILES_MATCHING
       PATTERN "*.sty")

    CONFIGURE_FILE(layout/main.tex.in "${OFBUILD_DOCBUILD_DIR}/latex/main.tex")

    ADD_CUSTOM_COMMAND(
      OUTPUT  "${OFBUILD_DIST_MAIN_DOC_DIR}/openfluid_manual_${OPENFLUID_VERSION_FULL}.pdf"
      DEPENDS "${CMAKE_CURRENT_BINARY_DIR}/LaTeX-Doxyfile"
      DEPENDS "${OFBUILD_DOCBUILD_DIR}/latex/main.tex"
      COMMAND "${DOXYGEN}"
         ARGS "${CMAKE_CURRENT_BINARY_DIR}/LaTeX-Doxyfile"
      COMMAND ${CMAKE_COMMAND} -E chdir ${CMAKE_CURRENT_SOURCE_DIR} ${CMAKE_COMMAND} 
              "-DDOC_BUILD_DIR=${OFBUILD_DOCBUILD_DIR}" "-DPDFLATEX_COMPILER=${PDFLATEX_COMPILER}"
              "-P" "BuildPDF.cmake"
      COMMAND ${CMAKE_COMMAND} -E copy "${OFBUILD_DOCBUILD_DIR}/latex/main.pdf" 
              "${OFBUILD_DIST_MAIN_DOC_DIR}/openfluid_manual_${OPENFLUID_VERSION_FULL}.pdf"
    )

    ADD_CUSTOM_TARGET(openfluid-doc-pdf ALL DEPENDS "${OFBUILD_DIST_MAIN_DOC_DIR}/openfluid_manual_${OPENFLUID_VERSION_FULL}.pdf" VERBATIM)   

  ENDIF() 

  INSTALL(DIRECTORY "${OFBUILD_DIST_MAIN_DOC_DIR}/"
          DESTINATION "${OPENFLUID_MAIN_DOC_INSTALL_PATH}")


ENDIF()

