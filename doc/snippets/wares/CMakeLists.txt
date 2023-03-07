
SET(WARES_CPP AttributesSim.cpp DebugSim.cpp EventsSim.cpp InfosDebugSim.cpp InternalStateSim.cpp ParamsEnvTimeSim.cpp
              SchedFixedSim.cpp SchedVariableSim.cpp SpatialConnectSim.cpp 
              SpatialParsingParallelSim.cpp SpatialParsingSeqSim.cpp
              VariablesSim.cpp)

SET(EXTRA_LINK_LIBS)


IF(OFBUILD_TESTS_ENABLE_FORTRAN)
  ENABLE_LANGUAGE(Fortran)
  SET(WARES_CPP ${WARES_CPP} FortranSim.cpp FortranMoreSim.cpp)
  SET(WARES_FORTRAN FortranSubr.f90 FortranMoreSubr.f90)
ENDIF()

IF(OFBUILD_TESTS_ENABLE_EMBEDDING_R)
  FIND_PACKAGE(R REQUIRED RInside)
  INCLUDE_DIRECTORIES(${R_INCLUDE_DIRS} ${R_RCPP_INCLUDE_DIR} ${R_RINSIDE_INCLUDE_DIR})
  LINK_DIRECTORIES(${R_LIBRARY_DIRS} ${R_RCPP_LIBRARY_DIR} ${R_RINSIDE_LIBRARY_DIR})
  SET(EXTRA_LINK_LIBS ${EXTRA_LINK_LIBS} ${R_LIBRARIES} ${R_RCPP_LIBRARIES} ${R_RINSIDE_LIBRARIES})
  SET(WARES_CPP ${WARES_CPP} RSim.cpp)
ENDIF()

ADD_LIBRARY(snippets_wares ${WARES_CPP} ${WARES_FORTRAN})

TARGET_LINK_LIBRARIES(snippets_wares STATIC
                      openfluid-core
                      openfluid-base
                      openfluid-tools
                      openfluid-utils
                      ${EXTRA_LINK_LIBS})

SET_TARGET_PROPERTIES(snippets_wares PROPERTIES
                      ARCHIVE_OUTPUT_DIRECTORY "${OFBUILD_TESTS_BINARY_DIR}")

# TOIMPL update referrences to snippets in docs, 
# TOIMPL review CMake.in.cmake.snippet and CMakeLists.txt.snippet
