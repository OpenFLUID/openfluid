OpenFLUID
=========

[![Build Status](https://travis-ci.org/OpenFLUID/openfluid.svg?branch=develop)](https://travis-ci.org/OpenFLUID/openfluid)


## General informations

**OpenFLUID is a software environnement for modelling and simulation
of complex landscape systems**

Further informations are available on the OpenFLUID web site and community site:  
- http://www.openfluid-project.org/
- http://www.openfluid-project.org/community/

See also COPYING and AUTHORS files included in the sources


## OpenFLUID sources 

The OpenFLUID source tree is organized as follow
- cmake : CMake files included in main build system 
- doc: Doxygen and LaTeX sources for users and developers manuals
- examples : examples of projects and simulators
- resources : general resources (translations, building, testing and packaging)
- share : shared resources used at runtime
- src : sources of the OpenFLUID framework and applications


### Requirements

OpenFLUID relies on open-source libraries, required to build or use OpenFLUID.

For the OpenFLUID framework libraries: 
  - C++ STL
  - Boost (system, filesystem)
  - Qt : Core, XML, Network, GUI (optional)
  - GDAL/OGR
  - GEOS (optional)

For openfluid command line application: 
  - OpenFLUID framework libraries
  - C++ STL
  - Boost (system, filesystem, program_options, date_time, regex)  

For openfluid-builder GUI application: 
  - OpenFLUID framework libraries
  - C++ STL
  - Boost
  - Qt : GUI

For unit testing, the Boost unit testing framework is also required 
(unit_test_framework)

For building Latex documents, required tools and packages are:
- pdflatex
- latex2html
- packages: a4wide, babel, ltxtable, pgf/tikz, tabularx, verbatim
- fonts: cmbright

OpenFLUID uses CMake version 2.8.9 or higher for build configuration, 
and has been tested with the GCC compiling system


### Building from source

Commands to be executed from source tree for building and packaging in Ubuntu linux:

    mkdir _build
    cd _build
    cmake .. -DCMAKE_BUILD_TYPE=Release -DCMAKE_INSTALL_PREFIX=/usr
    make
    cpack -G DEB
    
Detailed informations about building OpenFLUID from sources for Linux and Windows systems 
are available on the community site: 
http://www.openfluid-project.org/community/index.php/How_to_build_OpenFLUID

