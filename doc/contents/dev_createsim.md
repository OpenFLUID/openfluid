@page dev_createsim Creation of a simulator

[TOC]


The minimal source code of an OpenFLUID simuilator is made of a C++ file and a build configuration for CMake tool. 
Using the CMake build tool, the simulator source code is built into a binary plugin for OpenFLUID and automatically installed 
in the dedicated location to be usable by the OpenFLUID platform.  
<br/>

See also the @ref apdx_workspace appendix for sources codes location in workspaces.


# Required tools for development environment {#dev_createsim_env}

In order to build and develop a simulator, the following tools are required:

* GCC as the C++/C/Fortran compiler (version 4.9 or later for C++14 compatibility, version 6.x or later is recommended)
* CMake as the build configuration tool (version 3.1 or later). OpenFLUID provides a CMake module to ease the build of simulators. 

Detailed instructions for installation of these tools are available on the OpenFLUID Community web site
(<http://community.openfluid-project.org>). 

Even if simulators can be developped using any text editor, the OpenFLUID-DevStudio is the recommended environment for simulators development.


# Creation of a simulator using OpenFLUID-DevStudio {#dev_createsim_ds}

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


# Complete source code example {#dev_createsim_exmpl}

The example below show a complete example of an empty simulator, 
including source code and build configuration using the OpenFLUID CMake module.  


## File ExampleSimulator.cpp containing the simulator source code {#dev_createsim_exmpl_cpp}

```cpp
#include <openfluid/ware/PluggableSimulator.hpp>


// =====================================================================
// =====================================================================


BEGIN_SIMULATOR_SIGNATURE("example.simulator")

  DECLARE_NAME("Example simulator");
  DECLARE_DESCRIPTION("This simulator is an example");
  DECLARE_VERSION("14.07");
  DECLARE_STATUS(openfluid::ware::EXPERIMENTAL);

  DECLARE_AUTHOR("John","john@foobar.org");
  DECLARE_AUTHOR("Dave","dave@foobar.org");
  DECLARE_AUTHOR("Mike","mike@foobar.org");

END_SIMULATOR_SIGNATURE


// =====================================================================
// =====================================================================


class ExampleSimulator : public openfluid::ware::PluggableSimulator
{
  private:  

  public:


    Example(): PluggableSimulator()
    {
      // Here is source code for constructor
    }


    // =====================================================================
    // =====================================================================


    ~Example()
    {
      // Here is source code for destructor
    }


    // =====================================================================
    // =====================================================================


    void initParams(const openfluid::ware::WareParams_t& /*Params*/)
    {
      // Here is source code for processing simulator parameters
    }


    // =====================================================================
    // =====================================================================


    void prepareData()
    {
      // Here is source code for data preparation
    }


    // =====================================================================
    // =====================================================================


    void checkConsistency()
    {  
      // Here is source code for specific consistency checking
    }


    // =====================================================================
    // =====================================================================


    openfluid::base::SchedulingRequest initializeRun()
    {
      // Here is source code for initialization

      return DefaultDeltaT();
    }


    // =====================================================================
    // =====================================================================


    openfluid::base::SchedulingRequest runStep()
    {
      // Here is source code for each time step

      return DefaultDeltaT();
    }


    // =====================================================================
    // =====================================================================


    void finalizeRun()
    {
      // Here is source code for finalization
    }

};


DEFINE_SIMULATOR_CLASS(ExampleSimulator)

DEFINE_WARE_LINKUID(WARE_LINKUID)
```
  

## File CMake.in.config containing the build configuration {#dev_createsim_exmpl_config}
 
```cmake
# Simulator ID
# ex: SET(SIM_ID "my.simulator.id")
SET(SIM_ID "example.simulator")

# list of CPP files, the sim2doc tag must be contained in the first one
# ex: SET(SIM_CPP MySimulator.cpp)
SET(SIM_CPP ExampleSimulator.cpp)

# list of Fortran files, if any
# ex: SET(SIM_FORTRAN Calc.f)
#SET(SIM_FORTRAN )


# list of extra OpenFLUID libraries required
# ex: SET(SIM_OPENFLUID_COMPONENTS tools)
SET(SIM_OPENFLUID_COMPONENTS )

# set this to add include directories
# ex: SET(SIM_INCLUDE_DIRS /path/to/include/A/ /path/to/include/B/)
#SET(SIM_INCLUDE_DIRS )

# set this to add libraries directories
# ex: SET(SIM_INCLUDE_DIRS /path/to/libA/ /path/to/libB/)
#SET(SIM_LIBRARY_DIRS )

# set this to add linked libraries
# ex: SET(SIM_LINK_LIBS libA libB)
#SET(SIM_LINK_LIBS )

# set this to add definitions
# ex: SET(SIM_DEFINITIONS "-DDebug")
#SET(SIM_DEFINITIONS )


# unique ID for linking parameterization UI extension (if any)
#SET(WARE_LINK_UID "{xxxxxxxx-xxxx-xxxx-xxxx-xxxxxxxxxxxx}")

# set this to ON to enable parameterization widget
# ex: SET(SIM_PARAMSUI_ENABLED ON)
SET(SIM_PARAMSUI_ENABLED OFF)

# list of CPP files for parameterization widget, if any
# ex: SET(SIM_PARAMSUI_CPP MyWidget.cpp)
SET(SIM_PARAMSUI_CPP )

# list of UI files for parameterization widget, if any
# ex: SET(SIM_PARAMSUI_UI MyWidget.ui)
SET(SIM_PARAMSUI_UI )

# list of RC files for parameterization widget, if any
# ex: SET(SIM_PARAMSUI_RC MyWidget.rc)
SET(SIM_PARAMSUI_RC )


# set this to ON to enable translations
#SET(SIM_TRANSLATIONS_ENABLED ON)

# set this to list the languages for translations
#SET(SIM_TRANSLATIONS_LANGS fr_FR)

# set this to list the extra files or directories to scan for strings to translate
#SET(SIM_TRANSLATIONS_EXTRASCANS )


# set this to force an install path to replace the default one
#SET(SIM_INSTALL_PATH "/my/install/path/")


# set this if you want to use a specific sim2doc template
#SET(SIM_SIM2DOC_TPL "/path/to/template")


# set this if you want to add tests 
# given tests names must be datasets placed in a subdir named "tests"
# each dataset in the subdir must be names using the test name and suffixed by .IN
# ex for tests/test01.IN and tests/test02.IN: SET(SIM_TESTS_DATASETS test01 test02)
#SET(SIM_TESTS_DATASETS )
```



## File CMakeLists.txt defining the build process {#dev_createsim_exmpl_cmakelists}
 
```
CMAKE_MINIMUM_REQUIRED(VERSION 3.1)

INCLUDE(CMake.in.config)

FIND_PACKAGE(OpenFLUIDHelpers REQUIRED)

OPENFLUID_ADD_SIMULATOR(SIM)
```