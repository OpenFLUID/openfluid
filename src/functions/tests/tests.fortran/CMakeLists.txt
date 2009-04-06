
ENABLE_LANGUAGE (Fortran)

SET (CMAKE_Fortran_FLAGS_RELEASE "-funroll-all-loops -fno-f2c -O3")

ADD_LIBRARY(tests.fortran MODULE FortranFunc.cpp SbrFortran.f90)
SET_TARGET_PROPERTIES(tests.fortran 
                      PROPERTIES PREFIX "" SUFFIX "${CMAKE_SHARED_LIBRARY_SUFFIX}mpi" 
                      LIBRARY_OUTPUT_DIRECTORY "${FUNCTIONS_OUTPUT_PATH}")
TARGET_LINK_LIBRARIES(tests.fortran ofelib ${wxWidgets_LIBRARIES} gfortran)
