#
# CMake build for OpenFLUID-engine
#
# Author : Jean-Christophe FABRE <fabrejc@supagro.inra.fr>
#
#
# CPack components for packaging:
# - engine : OpenFLUID-engine, with SDK and standard simulation functions  
# - engine-wodev : OpenFLUID-engine, without SDK, and with standard simulation functions
# - mhydas : Simulation functions of the MHYDAS model
#
#


CMAKE_MINIMUM_REQUIRED (VERSION 2.6)

PROJECT(OpenFLUID-Engine CXX)

ENABLE_TESTING()

INCLUDE(CMake.in.config)
INCLUDE(CMake.in.sub)

FIND_PACKAGE(Doxygen)
FIND_PACKAGE(wxWidgets)
FIND_PACKAGE(PythonInterp)
FIND_PACKAGE(LATEX)
#FIND_PACKAGE(Boost COMPONENTS date_time)

#IF(wxWidgets_FOUND AND Boost_FOUND)
IF(wxWidgets_FOUND)
  INCLUDE(${wxWidgets_USE_FILE})
  INCLUDE_DIRECTORIES(${wxWidgets_INCLUDE_DIRS})
#  INCLUDE_DIRECTORIES(${Boost_INCLUDE_DIRS})

  FILE(MAKE_DIRECTORY "${LIBRARY_OUTPUT_PATH}")
  FILE(MAKE_DIRECTORY "${EXECUTABLE_OUTPUT_PATH}")
  FILE(MAKE_DIRECTORY "${FUNCTIONS_DOC_OUTPUT_PATH}")

  # hack for compiler flags semicolon problem with MSYS
  IF(MSYS)
    FOREACH(arg ${CMAKE_CXX_FLAGS})
      SET(MSYS_ADAPTED_CMAKE_CXX_FLAGS "${MSYS_ADAPTED_CMAKE_CXX_FLAGS} ${arg}")
    ENDFOREACH(arg ${CMAKE_CXX_FLAGS})
    SET(CMAKE_CXX_FLAGS "${MSYS_ADAPTED_CMAKE_CXX_FLAGS}")
  ENDIF(MSYS)


  ADD_SUBDIRECTORY(src)


  INSTALL(FILES "${CMAKE_SOURCE_DIR}/resources/dist/COPYING"
                "${CMAKE_SOURCE_DIR}/resources/dist/AUTHORS"
                "${CMAKE_SOURCE_DIR}/resources/dist/README"   
                "${CMAKE_SOURCE_DIR}/resources/dist/CHANGELOG"
          DESTINATION ${DOC_INSTALL_PATH} COMPONENT engine)
  
  INSTALL(FILES "${CMAKE_SOURCE_DIR}/resources/dist/COPYING"
                "${CMAKE_SOURCE_DIR}/resources/dist/AUTHORS"
                "${CMAKE_SOURCE_DIR}/resources/dist/README"
                "${CMAKE_SOURCE_DIR}/resources/dist/CHANGELOG"
          DESTINATION ${DOC_INSTALL_PATH} COMPONENT engine-wodev)


  CONFIGURE_FILE(engine.cpack.in engine.cpack @ONLY)
  CONFIGURE_FILE(mhydas.cpack.in mhydas.cpack @ONLY)
  CONFIGURE_FILE(enginewodev.cpack.in enginewodev.cpack @ONLY)

  ADD_SUBDIRECTORY(tests)
  
  MESSAGE(STATUS "Build type : ${CMAKE_BUILD_TYPE}")
  MESSAGE(STATUS "Compiler : ${CMAKE_CXX_COMPILER}")
  MESSAGE(STATUS "Compiler flags: ${CMAKE_CXX_FLAGS}")
  MESSAGE(STATUS "Compiler flags added for release build:  ${CMAKE_CXX_FLAGS_RELEASE}")
  MESSAGE(STATUS "Compiler flags added for debug build: ${CMAKE_CXX_FLAGS_DEBUG}")

ENDIF(wxWidgets_FOUND)


# Doxygen-built docs for core developers

IF(DOXYGEN AND BUILD_DOCFORDEV)
  
  CONFIGURE_FILE(${PROJECT_SOURCE_DIR}/Doxyfile.docfordev.in ${PROJECT_BINARY_DIR}/Doxyfile.docfordev)

  FILE(MAKE_DIRECTORY ${DOXYGEN_DOCFORDEV_OUTPUT_PATH})

  CONFIGURE_FILE(${DOXYGEN_RESOURCES_DIR}/openfluid.png ${DOXYGEN_DOCFORDEV_OUTPUT_PATH}/html/openfluid.png COPYONLY)
  
  ADD_CUSTOM_COMMAND(
    OUTPUT "${DOXYGEN_DOCFORDEV_OUTPUT_PATH}/html/index.html"
    DEPENDS "${PROJECT_BINARY_DIR}/Doxyfile.docfordev"
    COMMAND "${DOXYGEN}"
    ARGS "${PROJECT_BINARY_DIR}/Doxyfile.docfordev")

  ADD_CUSTOM_TARGET(docfordev-doc DEPENDS "${PROJECT_BINARY_DIR}/Doxyfile.docfordev"
    "${DOXYGEN_DOCFORDEV_OUTPUT_PATH}/html/index.html" VERBATIM)


ENDIF(DOXYGEN AND BUILD_DOCFORDEV)



# Latex docs compilation

IF(PDFLATEX_COMPILER OR LATEX2HTML_CONVERTER)
   
  FILE(GLOB_RECURSE DOCFILES ${DOC_RESOURCES_DIR}/*.tex)

  ADD_CUSTOM_COMMAND(
    OUTPUT ${DOC_BUILD_DIR}
    DEPENDS ${DOCFILES}                  
    COMMAND ${CMAKE_COMMAND} -E remove_directory ${DOC_BUILD_DIR}
    COMMAND ${CMAKE_COMMAND} -E make_directory ${DOC_BUILD_DIR}
    COMMAND ${CMAKE_COMMAND} -E copy_directory ${DOC_RESOURCES_DIR}/ ${DOC_BUILD_DIR}
   )
   
  
  IF(PDFLATEX_COMPILER)  
    COMPILE_PDFLATEX_DOC(openfluid-engine_users_en ${DOCPDF_OUTPUT_PATH})
  ENDIF(PDFLATEX_COMPILER)
  
  IF(LATEX2HTML_CONVERTER)  
    COMPILE_LATEX2HTML_DOC(openfluid-engine_users_en ${DOCHTML_OUTPUT_PATH})
  ENDIF(LATEX2HTML_CONVERTER)
  
ENDIF(PDFLATEX_COMPILER  OR LATEX2HTML_CONVERTER)




