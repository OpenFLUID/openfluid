# Creation of a simulator {#dev_createsim}

[TOC]


The minimal source code of an OpenFLUID simuilator is made of a C++ file and a build configuration for CMake tool. 
Using the CMake build tool, the simulator source code is built into a binary plugin for OpenFLUID and automatically installed 
in the dedicated location to be usable by the OpenFLUID platform.  
<br/>

See also the @ref apdx_workspace appendix for sources codes location in workspaces.


## Required tools for development environment {#dev_createsim_env}

In order to build and develop a simulator, the following tools are required:

* GCC as the C++/C/Fortran compiler (version 4.9 or later for C++14 compatibility, version 6.x or later is recommended)
* CMake as the build configuration tool (version 3.1 or later). OpenFLUID provides a CMake module to ease the build of simulators. 

Detailed instructions for installation of these tools are available on the OpenFLUID Community web site
(<http://community.openfluid-project.org>). 

Even if simulators can be developped using any text editor, the OpenFLUID-DevStudio is the recommended environment for simulators development.


## Creation of a simulator using OpenFLUID-DevStudio {#dev_createsim_ds}

The OpenFLUID-DevStudio application is made of a main toolbar located on top, 
a file navigator on the left side and a file editor on the right side.

@image html screenshot_devstudio_overview_html.png "Screenshot of OpenFLUID-DevStudio workspace"
@image latex screenshot_devstudio_overview.png "Screenshot of OpenFLUID-DevStudio workspace" width=11.5cm

To create a new simulator, go to menu _File > New ware > Simulator..._ This opens the new simulator dialog dox. 
In this dialog box, set the simulator ID and source files names then click _OK_. 
The Source code of a new simulator is created.  

@image html screenshot_devstudio_newware_html.png "Screenshot of new simulator dialog"
@image latex screenshot_devstudio_newware.png "Screenshot of new simulator dialog" width=5.5cm

Once created, click on the _Configure_ button of the main toolbar. Once the configure process is completed, 
click on the _Build_ button to effectively build the simulator. Once the build process is completed, 
the simulator is ready to use for simulations. The build process must be run each time 
the simulator source code is modified to take into account these modifications.


## Complete source code example {#dev_createsim_exmpl}

The example below show a complete example of an empty simulator, 
including source code and build configuration using the OpenFLUID CMake module.  


### File ExampleSimulator.cpp containing the simulator source code {#dev_createsim_exmpl_cpp}

@snippet help.snippets.example/ExampleSimulator.cpp example_full


### File CMake.in.config containing the build configuration {#dev_createsim_exmpl_config}

@include help.snippets.example/CMake.in.cmake.snippet


### File CMakeLists.txt defining the build process {#dev_createsim_exmpl_cmakelists}
 
@include help.snippets.example/CMakeLists.txt.snippet
