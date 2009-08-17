OpenFLUID-engine sources
========================

The OpenFLUID-engine source tree is organized as follow
- src/kernel : sources of the kernel
- src/ofelib : sources of the support library and API
- src/functions/standard : sources of the standard simulation functions 
- src/functions/mhydas : sources of the simulation functions of the mhydas model
- include : include files of the API
- resources/dist : files to include in packages for distribution
- resources/doxygen : doxygen files for API documentation
- resources/ofefunc2doc : the ofefunc2doc tool (Python sources)
- tests : data and configuration for tests


The required libraries are:
- C++ STL 
- wxWidgets (base only) 2.8.x


In order to build the binaries and to pack the application, CMake 2.6.x 
is required.
1) create a directory named "build" (or any other name)
2) "cd" to this directory
3) run "cmake .."    
4) run "make"


To build packages, 
1) "cd" to the "build" directory
2) run "cpack --config <config file>"

<config file> can be:
- engine.cpack to build the package with the main app, the libs and the standard 
  functions
- enginewodev.cpack to build the package with the main app, the libs and the 
  standard functions, but without the API included
- mhydas.cpack to build the package containing the simulation functions for the 
  mhydas model


  