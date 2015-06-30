# Observer ID
# ex: SET(OBS_ID "my.observer.id")
SET(OBS_ID "%%WAREID%%")

# list of CPP files
# ex: SET(OBS_CPP MyObserver.cpp)
SET(OBS_CPP %%ROOTCPPFILENAME%%)

# list of Fortran files, if any
# ex: SET(OBS_FORTRAN Calc.f)
#SET(OBS_FORTRAN )


# list of extra OpenFLUID libraries required
# ex: SET(OBS_OPENFLUID_COMPONENTS tools)
SET(OBS_OPENFLUID_COMPONENTS )

# set this to add include directories
# ex: SET(OBS_INCLUDE_DIRS /path/to/include/A/ /path/to/include/B/)
#SET(OBS_INCLUDE_DIRS )

# set this to add libraries directories
# ex: SET(OBS_INCLUDE_DIRS /path/to/libA/ /path/to/libB/)
#SET(OBS_LIBRARY_DIRS )

# set this to add linked libraries
# ex: SET(OBS_LINK_LIBS libA libB)
#SET(OBS_LINK_LIBS )

# set this to add definitions
# ex: SET(OBS_DEFINITIONS "-DDebug")
#SET(OBS_DEFINITIONS )


# unique ID for linking parameterization UI extension (if any)
SET(WARE_LINK_UID "%%WARELINKUID%%")

# set this to ON to enable parameterization widget
# ex: SET(OBS_PARAMSUI_ENABLED ON)
SET(OBS_PARAMSUI_ENABLED %%PARAMSUIENABLED%%)

# list of CPP files for parameterization widget, if any
# ex: SET(OBS_PARAMSUI_CPP MyWidget.cpp)
SET(OBS_PARAMSUI_CPP %%PARAMSUIROOTCPPFILENAME%%)

# list of UI files for parameterization widget, if any
# ex: SET(OBS_PARAMSUI_UI MyWidget.ui)
SET(OBS_PARAMSUI_UI )

# list of RC files for parameterization widget, if any
# ex: SET(OBS_PARAMSUI_RC MyWidget.rc)
SET(OBS_PARAMSUI_RC )


# set this to ON to enable translations
#SET(OBS_TRANSLATIONS_ENABLED ON)

# set this to list the languages for translations
#SET(OBS_TRANSLATIONS_LANGS fr_FR)

# set this to list the extra files or directories to scan for strings to translate
#SET(OBS_TRANSLATIONS_EXTRASCANS )


# set this to force an install path to replace the default one
#SET(OBS_INSTALL_PATH "/my/install/path/")


# set this if you want to add tests 
# given tests names must be datasets placed in a subdir named "tests"
# each dataset in the subdir must be names using the test name and suffixed by .IN
# ex for tests/test01.IN and tests/test02.IN: SET(OBS_TESTS_DATASETS test01 test02)
#SET(OBS_TESTS_DATASETS )
 
