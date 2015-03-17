# Simulator ID
# ex: SET(SIM_ID "my.simulator.id")
SET(SIM_ID "%%WAREID%%")

# list of CPP files, the sim2doc tag must be contained in the first one
# ex: SET(SIM_CPP MySimulator.cpp)
SET(SIM_CPP %%ROOTCPPFILENAME%%)

# list of Fortran files, if any
# ex: SET(SIM_FORTRAN Calc.f)
#SET(SIM_FORTRAN )


# set this to ON to enable parameterization widget
# ex: SET(SIM_PARAMSUI_ENABLED ON)
SET(SIM_PARAMSUI_ENABLED %%PARAMSUIENABLED%%)

# list of CPP files for parameterization widget, if any
# ex: SET(SIM_PARAMSUI_CPP MyWidget.cpp)
SET(SIM_PARAMSUI_CPP %%PARAMSUIROOTCPPFILENAME%%)

# list of UI files for parameterization widget, if any
# ex: SET(SIM_PARAMSUI_UI MyWidget.ui)
SET(SIM_PARAMSUI_UI )

# list of RC files for parameterization widget, if any
# ex: SET(SIM_PARAMSUI_RC MyWidget.rc)
SET(SIM_PARAMSUI_RC )


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


# set this to force an install path to replace the default one
#SET(SIM_INSTALL_PATH "/my/install/path/")


# set this to ON or AUTO for build of simulator documentation using sim2doc
SET(SIM_SIM2DOC_MODE %%SIM2DOCMODE%%)

#set to ON to disable installation of sim2doc built documentation
SET(SIM_SIM2DOC_INSTALL_DISABLED %%SIM2DOCINSTALLDISABLED%%)

# set this if you want to use a specific sim2doc template
#SET(SIM_SIM2DOC_TPL "/path/to/template")


# set this if you want to add tests 
# given tests names must be datasets placed in a subdir named "tests"
# each dataset in the subdir must be names using the test name and suffixed by .IN
# ex for tests/test01.IN and tests/test02.IN: SET(SIM_TESTS_DATASETS test01 test02)
#SET(SIM_TESTS_DATASETS )
 
