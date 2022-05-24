# Required vars
# DOC_BUILD_DIR
# PDFLATEX_COMPILER


SET(WORK_DIR "${DOC_BUILD_DIR}/latex")


# Builds the pdf doc in 3 passes
FOREACH(PASS RANGE 1 3)
  EXECUTE_PROCESS(
    COMMAND ${CMAKE_COMMAND} -E chdir "${WORK_DIR}"
            ${PDFLATEX_COMPILER} "-interaction=nonstopmode" "main.tex"
    OUTPUT_FILE "${WORK_DIR}/buildpass${PASS}_out.txt"
  )
ENDFOREACH()
