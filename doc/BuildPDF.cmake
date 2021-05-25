# Required vars
# DOC_BUILD_DIR
# PDFLATEX_COMPILER


# Builds the pdf doc in 3 passes
FOREACH(PASS RANGE 1 3)
    EXECUTE_PROCESS(
      COMMAND ${CMAKE_COMMAND} -E chdir "${DOC_BUILD_DIR}/latex"
              ${PDFLATEX_COMPILER} "-interaction=nonstopmode" "main.tex"
      OUTPUT_FILE "${DOC_BUILD_DIR}/latex/buildpass${PASS}_out.txt"
    )
ENDFOREACH()
