OpenFLUID
=========

[![Build Status](https://travis-ci.org/OpenFLUID/openfluid.svg?branch=develop)](https://travis-ci.org/OpenFLUID/openfluid)
[![Build status](https://ci.appveyor.com/api/projects/status/6icywvsumu608uft/branch/develop?svg=true)](https://ci.appveyor.com/project/fabrejc/openfluid-hm6ac/branch/develop)


## General informations

**OpenFLUID is a software environnement for modelling and simulation
of complex landscape systems**

Further informations are available on the OpenFLUID web site and community site:  
- http://www.openfluid-project.org/
- http://www.openfluid-project.org/community/

See also LICENSE and AUTHORS files included in the sources


## OpenFLUID sources 

The OpenFLUID source tree is organized as follow
- cmake : CMake files included in main build system 
- doc: Doxygen and LaTeX sources for users and developers manuals
- examples : examples of projects and simulators
- resources : general resources (translations, building, testing and packaging)
- share : shared resources used at runtime
- src : sources of the OpenFLUID framework and applications


### Requirements

OpenFLUID is written in C++11. It relies on open-source libraries required to build or use OpenFLUID.

For the OpenFLUID framework libraries:
  - C++ STL
  - Boost (headers only)
  - Qt5 : Core, GUI (optional), Network, XML
  - RapidJSON (automatically downloaded if locally missing)
  - GDAL/OGR
  - GEOS (optional)

For openfluid command line application:
  - OpenFLUID framework libraries
  - C++ STL
  - Qt5 : Core 

For openfluid-builder GUI application:
  - OpenFLUID framework libraries
  - C++ STL
  - Qt5 : Core, GUI, SVG
  - GDAL/OGR

For openfluid-devstudio GUI application:
  - OpenFLUID framework libraries
  - C++ STL
  - Qt5 : Core, GUI

For unit testing, the Boost unit testing framework is also required 
(unit_test_framework)

For building Latex documents, required tools and packages are:
  - pdflatex
  - latex2html
  - packages: a4wide, babel, ltxtable, pgf/tikz, tabularx, verbatim
  - fonts: cmbright

OpenFLUID uses CMake version 2.8.12 or higher for build configuration, 
and has been tested with the GCC 4.8+ compiling system


### Building from source

Commands to be executed from source tree for building and packaging on linux (Ubuntu, Debian, Fedora):

    mkdir _build
    cd _build
    cmake .. -DCMAKE_BUILD_TYPE=Release -DCMAKE_INSTALL_PREFIX=/usr
    make
    cpack


The following options are available to control the configure and build process
Option | Default Value | Description
-------| ------------- | -----------
-DOFBUILD_ENABLE_SANITIZER | OFF | Enable the compiler sanitizer to track memory address errors
-DOFBUILD_ENABLE_TESTING | ON (Debug), OFF (Release) | Enable the build of tests
-DOFBUILD_ENABLE_HEAVYTESTING | OFF | Enable the build of tests

    
Detailed informations about building OpenFLUID from sources for Linux, Windows and macOS systems 
are available on the community site: 
http://www.openfluid-project.org/community/index.php/How_to_build_OpenFLUID

