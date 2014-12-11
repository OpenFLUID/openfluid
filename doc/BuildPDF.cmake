# Required vars
# DOC_BUILD_DIR
# PDFLATEX_COMPILER


    EXECUTE_PROCESS(
      COMMAND ${CMAKE_COMMAND} -E chdir ${DOC_BUILD_DIR}/latex ${PDFLATEX_COMPILER}
              "-interaction=nonstopmode" "main.tex"
    )
    
    EXECUTE_PROCESS(       
      COMMAND ${CMAKE_COMMAND} -E chdir ${DOC_BUILD_DIR}/latex ${PDFLATEX_COMPILER}
              "-interaction=nonstopmode" "main.tex"
    )
    
    EXECUTE_PROCESS(
      COMMAND ${CMAKE_COMMAND} -E chdir ${DOC_BUILD_DIR}/latex ${PDFLATEX_COMPILER}
              "-interaction=nonstopmode" "main.tex"              
    )
    
    