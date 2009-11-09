OpenFLUID-engine sources
========================

The OpenFLUID-engine source tree is organized as follow
- doc/manuals : latex sources of the manuals
- doc/API-doxygen : doxygen files for API documentation
- doc/examples : latex sources of the examples documentation
- include : include files of the API
- packs : configuration files for packaging using CPack
- resources/dist : files to include in packages for distribution
- resources/examples : examples of simulation functions, with build system files
- resources/func2doc : func2doc buddy resources files
- resources/graphics : source graphics used in documentation
- src/kernel : sources of the kernel
- src/ofelib : sources of the support library and API
- tests : data, sources and configuration for testing using CTest


The required libraries are:
for ofelib: 
  - C++ STL
  - boost (system, filesystem) 
for the kernel: 
  - ofelib 
  - C++ STL
  - boost (system, filesystem, program_options, date_time)

For unit testing, the boost test framework is also required (unit_test_framework)


For building Latex documents, required tools and packages are:
- pdflatex
- latex2html
- packages: a4wide, babel, ltxtable, pgf/tikz, tabularx, verbatim
- fonts: cmbright


In order to build the binaries and pack the application, CMake 2.6.x 
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
  