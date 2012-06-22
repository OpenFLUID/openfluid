

IF(BUILD_FORTRAN_FUNCTIONS)

  SET(CMAKE_Fortran_FLAGS_RELEASE "-funroll-all-loops -fno-f2c -O3")

  ADD_LIBRARY(tests.fortran MODULE FortranFunc.cpp SbrFortran.f90)
  SET_TARGET_PROPERTIES(tests.fortran 
                        PROPERTIES PREFIX "" SUFFIX "${FUNCTIONS_BINARY_EXTENSION}" 
                        LIBRARY_OUTPUT_DIRECTORY "${TEST_OUTPUT_PATH}")
  TARGET_LINK_LIBRARIES(tests.fortran openfluid-base openfluid-core gfortran)

ENDIF(BUILD_FORTRAN_FUNCTIONS)
