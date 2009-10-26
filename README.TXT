OpenFLUID-engine sources
========================

The OpenFLUID-engine source tree is organized as follow
- src/kernel : sources of the kernel
- src/ofelib : sources of the support library and API
- src/functions/examples : sources of examples simulation functions 
- include : include files of the API
- resources/dist : files to include in packages for distribution
- resources/doxygen : doxygen files for API documentation
- tests : data, sources and configuration for tests


The required libraries are:
for ofelib: 
  - C++ STL
  - boost (system, filesystem) 
for the kernel: 
  - ofelib 
  - C++ STL
  - boost (system, filesystem, program_options, date_time)
  - wxWidgets (base only) 2.8.x

For unit testing, the boost test framework is also required (unit_test_framework)


In order to build the binaries and to pack the application, CMake 2.6.x 
is required.
1) create a directory named "_build" (or any other name)
2) "cd" to this directory
3) run "cmake .."    
4) run "make"
5) run "ctest" to run tests


To build packages, 
1) "cd" to the "build" directory
2) run "cpack --config <config file>"

<config file> can be:
- engine.cpack to build the package with the main app and the ofelib library
- enginewodev.cpack to build the package with the main appand the ofelib library,
    but without the API included



OpenFLUID is property of 
Institut National de La recherche Agronomique 
and 
Montpellier SupAgro
Inter Deposit Digital Number : IDDN.FR.001.360006.000.R.P.2009.000.30100
  