#
# CMake build for OpenFLUID-engine
#
# Author : Jean-Christophe FABRE <fabrejc@supagro.inra.fr>
#
#
# CPack components for packaging:
# - engine : OpenFLUID-engine, with SDK and standard simulation functions  
# - engine-wodev : OpenFLUID-engine, without SDK, and with standard simulation functions
#
#


CMAKE_MINIMUM_REQUIRED (VERSION 2.6)

PROJECT(OpenFLUID-Engine CXX)

INCLUDE(CTest)
ENABLE_TESTING()

INCLUDE(CMake.in.config)

IF (EXISTS ${CMAKE_SOURCE_DIR}/CMake.in.local.config)
  INCLUDE(CMake.in.local.config)
ENDIF (EXISTS ${CMAKE_SOURCE_DIR}/CMake.in.local.config)

GET_FILENAME_COMPONENT(BUILDNAME_COMPILER ${CMAKE_CXX_COMPILER} NAME)
SET(BUILDNAME ${CMAKE_SYSTEM_NAME}-${CMAKE_SYSTEM_PROCESSOR}-${BUILDNAME_COMPILER})

SET (CTEST_INITIAL_CACHE "
BUILDNAME:STRING=${CMAKE_SYSTEM_NAME}-${CMAKE_SYSTEM_PROCESSOR}-${BUILDNAME_COMPILER}
")

INCLUDE(CMake.in.sub)

FIND_PACKAGE(Doxygen)
FIND_PACKAGE(LATEX)
FIND_PACKAGE(Boost REQUIRED COMPONENTS system filesystem date_time program_options regex)

IF(Boost_FOUND)


  FILE(MAKE_DIRECTORY "${LIB_OUTPUT_PATH}")
  FILE(MAKE_DIRECTORY "${BIN_OUTPUT_PATH}")
  FILE(MAKE_DIRECTORY "${FUNCTIONS_DOC_OUTPUT_PATH}")

  # hack for compiler flags semicolon problem with MSYS
  IF(MSYS)
    FOREACH(arg ${CMAKE_CXX_FLAGS})
      SET(MSYS_ADAPTED_CMAKE_CXX_FLAGS "${MSYS_ADAPTED_CMAKE_CXX_FLAGS} ${arg}")
    ENDFOREACH(arg ${CMAKE_CXX_FLAGS})
    SET(CMAKE_CXX_FLAGS "${MSYS_ADAPTED_CMAKE_CXX_FLAGS}")
  ENDIF(MSYS)


  ADD_DEFINITIONS(-DBOOST_ALL_DYN_LINK)

  ADD_SUBDIRECTORY(src)


  INSTALL(FILES "${CMAKE_SOURCE_DIR}/resources/dist/copyright"
                "${CMAKE_SOURCE_DIR}/resources/dist/AUTHORS"
                "${CMAKE_SOURCE_DIR}/resources/dist/README"   
                "${CMAKE_SOURCE_DIR}/resources/dist/CHANGELOG"
          DESTINATION ${DOC_INSTALL_PATH} COMPONENT engine)
  
  INSTALL(FILES "${CMAKE_SOURCE_DIR}/resources/dist/copyright"
                "${CMAKE_SOURCE_DIR}/resources/dist/AUTHORS"
                "${CMAKE_SOURCE_DIR}/resources/dist/README"
                "${CMAKE_SOURCE_DIR}/resources/dist/CHANGELOG"
          DESTINATION ${DOC_INSTALL_PATH} COMPONENT engine-wodev)


  CONFIGURE_FILE(engine.cpack.in engine.cpack @ONLY)
  CONFIGURE_FILE(enginewodev.cpack.in enginewodev.cpack @ONLY)

  
  ADD_SUBDIRECTORY(tests)

  MESSAGE(STATUS "Build name: ${BUILDNAME}")
  MESSAGE(STATUS "Build type : ${CMAKE_BUILD_TYPE}")
  MESSAGE(STATUS "Compiler : ${CMAKE_CXX_COMPILER}")
  MESSAGE(STATUS "Compiler flags: ${CMAKE_CXX_FLAGS}")
  MESSAGE(STATUS "Compiler flags added for release build:  ${CMAKE_CXX_FLAGS_RELEASE}")
  MESSAGE(STATUS "Compiler flags added for debug build: ${CMAKE_CXX_FLAGS_DEBUG}")

ENDIF(Boost_FOUND)


# Doxygen-built docs for core developers

IF(DOXYGEN AND BUILD_DOCFORDEV)
  
  MESSAGE(STATUS "Build Doxygen documentation for core developers : yes")
  
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


ELSE(DOXYGEN AND BUILD_DOCFORDEV)
  MESSAGE(STATUS "Build Doxygen documentation for core developers : no")
ENDIF(DOXYGEN AND BUILD_DOCFORDEV)



# Latex docs compilation
IF (BUILD_LATEX_DOCS)
  MESSAGE(STATUS "Build LaTeX documentation : yes")

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
    COMPILE_PDFLATEX_DOC(openfluid-engine_quickref_en ${DOCPDF_OUTPUT_PATH})
  ENDIF(PDFLATEX_COMPILER)
  
  IF(LATEX2HTML_CONVERTER)  
    COMPILE_LATEX2HTML_DOC(openfluid-engine_quickref_en ${DOCHTML_OUTPUT_PATH})
  ENDIF(LATEX2HTML_CONVERTER)
  
ENDIF(PDFLATEX_COMPILER  OR LATEX2HTML_CONVERTER)
ELSE (BUILD_LATEX_DOCS)
  MESSAGE(STATUS "Build LaTeX documentation : no")
ENDIF (BUILD_LATEX_DOCS)



