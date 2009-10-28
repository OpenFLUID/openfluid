


# Latex docs compilation
IF (BUILD_LATEX_DOCS)
  MESSAGE(STATUS "Build LaTeX documentation : yes")

  ADD_SUBDIRECTORY(manuals)

  ADD_SUBDIRECTORY(examples)


ELSE (BUILD_LATEX_DOCS)
  MESSAGE(STATUS "Build LaTeX documentation : no")
ENDIF (BUILD_LATEX_DOCS)


ADD_SUBDIRECTORY(API-doxygen)

ADD_SUBDIRECTORY(docfordev-doxygen)
