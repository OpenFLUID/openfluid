CMAKE_MINIMUM_REQUIRED (VERSION 2.6)

PROJECT(${SIM_ID} CXX)

INCLUDE(CMake.in.config)

ENABLE_TESTING()



# =========================================================================
#   checking libraries and tools
# =========================================================================

IF(NOT openfluid_FOUND)
  FIND_PACKAGE(PkgConfig REQUIRED)
  PKG_CHECK_MODULES(openfluid REQUIRED openfluid)
ENDIF(NOT openfluid_FOUND)

IF(NOT NOFUNC2DOC)
  FIND_PACKAGE(LATEX)
ENDIF(NOT NOFUNC2DOC)


# =========================================================================
#   include and link directories
# =========================================================================

INCLUDE_DIRECTORIES(${openfluid_INCLUDE_DIRS} ${Boost_INCLUDE_DIRS})
INCLUDE_DIRECTORIES(${SIM_INCLUDE_DIRS})

LINK_DIRECTORIES(${openfluid_LIBRARY_DIRS} ${Boost_LIBRARY_DIRS})
LINK_DIRECTORIES(${SIM_LIBRARY_DIRS})


# =========================================================================   
#   process fortran source files if present
# ========================================================================= 

IF(SIM_FORTRAN)
  ENABLE_LANGUAGE(Fortran)
  SET(CMAKE_Fortran_FLAGS_RELEASE "-funroll-all-loops -fno-f2c -O3")
  SET(FORTRAN_LINK_LIBS "gfortran")
ENDIF(SIM_FORTRAN)


# =========================================================================
#   definitions
# =========================================================================

ADD_DEFINITIONS(-DOPENFLUID_VERSION=${openfluid_VERSION})
ADD_DEFINITIONS(${openfluid_CFLAGS})
ADD_DEFINITIONS(${SIM_DEFINITIONS})


# =========================================================================
#   function build
# =========================================================================

ADD_LIBRARY(${SIM_ID} MODULE ${SIM_CPP} ${SIM_FORTRAN})
SET_TARGET_PROPERTIES(${SIM_ID} PROPERTIES PREFIX "" SUFFIX "${CMAKE_SHARED_LIBRARY_SUFFIX}mpi")
  
TARGET_LINK_LIBRARIES(${SIM_ID} ${openfluid_LIBRARIES} ${Boost_LIBRARIES} ${FORTRAN_LINK_LIBS} ${SIM_LINK_LIBS})


# =========================================================================
#   function documentation
# =========================================================================

IF(REPOS_NOFUNC2DOC)
 SET(NOFUNC2DOC 1)
ENDIF(REPOS_NOFUNC2DOC)

IF(PDFLATEX_COMPILER AND NOT NOFUNC2DOC)

  LIST(GET SIM_CPP 0 CPP_FOR_DOC)

  IF(NOT DOCS_OUTPUT_PATH)
    SET(DOCS_OUTPUT_PATH "${PROJECT_BINARY_DIR}")
  ENDIF(NOT DOCS_OUTPUT_PATH)

  IF(FUNC2DOC_TPLFILE)
    SET (TPLFILE_OPTION ",tplfile=${FUNC2DOC_TPLFILE}")
  ENDIF(FUNC2DOC_TPLFILE)

  ADD_CUSTOM_COMMAND(
    OUTPUT "${DOCS_OUTPUT_PATH}/${SIM_ID}.pdf"
    DEPENDS "${CMAKE_CURRENT_SOURCE_DIR}/${CPP_FOR_DOC}"
    COMMAND "openfluid-engine"
    ARGS "--buddy" "func2doc" "--buddyopts" "inputcpp=${CMAKE_CURRENT_SOURCE_DIR}/${CPP_FOR_DOC},outputdir=${DOCS_OUTPUT_PATH},pdf=1${TPLFILE_OPTION}"     
  )

  ADD_CUSTOM_TARGET(${SIM_ID}-doc ALL DEPENDS "${DOCS_OUTPUT_PATH}/${SIM_ID}.pdf")

ENDIF(PDFLATEX_COMPILER AND NOT NOFUNC2DOC)



# =========================================================================
#   function tests
# =========================================================================  

IF(REPOS_BUILD)
  SET(SIMMPI_DIR ${LIBRARY_OUTPUT_PATH})
  SET(SIMTEST_OUTPUT_DIR ${REPOS_TESTS_OUTPUTDATA_PATH})
ELSE(REPOS_BUILD)
  SET(SIMMPI_DIR ${PROJECT_BINARY_DIR})
  SET(SIMTEST_OUTPUT_DIR ${PROJECT_BINARY_DIR}/tests-output)  
  FILE(MAKE_DIRECTORY "${SIMTEST_OUTPUT_DIR}")
ENDIF(REPOS_BUILD)

FOREACH(currtest ${TESTS_DATASETS})
  ADD_TEST(${SIM_ID}-${currtest} "openfluid-engine"
                            "-i" "${CMAKE_CURRENT_SOURCE_DIR}/tests/${currtest}.IN"
                            "-o" "${SIMTEST_OUTPUT_DIR}/${currtest}.OUT"
                            "-p" "${SIMMPI_DIR}")
  MESSAGE(STATUS "Added simulator test ${SIM_ID}-${currtest}")                            
ENDFOREACH(currtest ${TESTS_DATASETS})



# =========================================================================
#   install directives
# =========================================================================

IF(NOT USER_SIMULATORS_INSTALL_PATH)
 SET(USER_SIMULATORS_INSTALL_PATH "$ENV{HOME}/.openfluid/simulators")
 IF(WIN32)
   SET(USER_SIMULATORS_INSTALL_PATH "$ENV{APPDATA}/openfluid/simulators") 
 ENDIF(WIN32)
ENDIF(NOT USER_SIMULATORS_INSTALL_PATH)

IF(REPOS_INSTALL_COMPONENT)
  INSTALL(TARGETS ${SIM_ID} DESTINATION ${REPOS_SIMULATORS_INSTALL_PATH} COMPONENT ${REPOS_INSTALL_COMPONENT})
  IF(PDFLATEX_COMPILER AND NOT NOFUNC2DOC)
    INSTALL(FILES "${DOCS_OUTPUT_PATH}/${SIM_ID}.pdf" DESTINATION ${REPOS_FUNCDOCS_INSTALL_PATH} COMPONENT ${REPOS_INSTALL_COMPONENT})
  ENDIF(PDFLATEX_COMPILER AND NOT NOFUNC2DOC) 
ELSE(REPOS_INSTALL_COMPONENT)
  INSTALL(TARGETS ${SIM_ID} DESTINATION "${USER_SIMULATORS_INSTALL_PATH}")
ENDIF(REPOS_INSTALL_COMPONENT) 

