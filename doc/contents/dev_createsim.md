# Creation of a simulator {#dev_createsim}

[TOC]


The minimal source code of an OpenFLUID simuilator is made of a C++ file and a build configuration for CMake tool. 
Using the CMake build tool, the simulator source code is built into a binary plugin for OpenFLUID and automatically installed 
in the dedicated location to be usable by the OpenFLUID platform.  
<br/>

See also the @ref apdx_workspace appendix for sources codes location in workspaces.


## Required tools for development environment {#dev_createsim_env}

In order to build and develop a simulator, the following tools are required:

* GCC as the C++/C/Fortran compiler (version 7 or later for C++17 compatibility)
* CMake as the build configuration tool (version 3.10 or later). OpenFLUID provides a CMake module to ease the build of simulators. 

Detailed instructions for installation of these tools are available on the OpenFLUID Community web site
(<http://community.openfluid-project.org>). 

Even if simulators can be developped using any text editor, the OpenFLUID-DevStudio is the recommended environment for simulators development.


## Creation of a simulator using OpenFLUID-DevStudio {#dev_createsim_ds}


@note As the OpenFLUID-DevStudio UI is multilingual, the items cited below such as menu names or labels can be in another language than english for you installation.

The OpenFLUID-DevStudio application is made of a main toolbar located on left, 
a file navigator on the left side and a file editor on the right side.

@image html screenshot_devstudio_overview.png "Screenshot of OpenFLUID-DevStudio workspace"
@image latex screenshot_devstudio_overview.png "Screenshot of OpenFLUID-DevStudio workspace" width=11.5cm

To create a new simulator, go to menu _File > New ware > Simulator..._ This opens the new simulator dialog. 
In this dialog, set the simulator ID and source files names then click _OK_. 
The Source code of a new simulator is created.  

@image html screenshot_devstudio_newware.png "Screenshot of new simulator dialog"
@image latex screenshot_devstudio_newware.png "Screenshot of new simulator dialog" width=5.5cm

### Configuration phase
Once created, the configuration phase must be performed at least once. Click on the _Configure_ button of the main toolbar. 
This phase checks the dependencies (tools and libraries) required to build the simulator. It can be performed either in _Release_ mode for performance optimization (mode by default, recommended) or in _Debug_ mode to be used with an external debugger.

### Build phase
The build phase must be performed each time the source code has been modified. Once the configure process is completed, 
click on the _Build_ button to effectively build the simulator. 

This phase builds thes simulator source code into a binary plugin for the OpenFLUID platform. It can be performed either in _Build and install_ mode to make the simulator immediately available for simulations (mode by default, recommended) or in _Build only_ mode for intermediate builds for example.



## Complete source code example {#dev_createsim_exmpl}

The example below show a complete example of an empty simulator, 
including source code and build configuration using the OpenFLUID CMake module.  


### File ExampleSimulator.cpp containing the simulator source code {#dev_createsim_exmpl_cpp}

@snippet wares/ExampleSimulator.cpp example_full


### File CMakeLists.txt containing the ware configuration {#dev_createsim_exmpl_cmakelists}

@include wares/CMakeLists.txt.snippet


### File src/CMakeLists.txt defining the plugin build {#dev_createsim_exmpl_src_cmakelists}
 
@include wares/src.CMakeLists.txt.snippet

