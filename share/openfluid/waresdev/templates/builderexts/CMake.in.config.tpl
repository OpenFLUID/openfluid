# Builder-extension ID
# ex: SET(BEXT_ID "my.builderext.id")
SET(BEXT_ID "%%WAREID%%")

# list of CPP files
# ex: SET(BEXT_CPP MyBuilderext.cpp)
SET(BEXT_CPP %%ROOTCPPFILENAME%%)

# list of UI files, if any
# ex: SET(BEXT_UI MyWidget.ui)
SET(BEXT_UI )

# list of RC files, if any
# ex: SET(BEXT_RC MyWidget.rc)
SET(BEXT_RC )


# list of extra OpenFLUID libraries required
# ex: SET(BEXT_OPENFLUID_COMPONENTS tools)
SET(BEXT_OPENFLUID_COMPONENTS )

# set this to add include directories
# ex: SET(BEXT_INCLUDE_DIRS /path/to/include/A/ /path/to/include/B/)
#SET(BEXT_INCLUDE_DIRS )

# set this to add libraries directories
# ex: SET(BEXT_INCLUDE_DIRS /path/to/libA/ /path/to/libB/)
#SET(BEXT_LIBRARY_DIRS )

# set this to add linked libraries
# ex: SET(BEXT_LINK_LIBS libA libB)
#SET(BEXT_LINK_LIBS )

# set this to add definitions
# ex: SET(BEXT_DEFINITIONS "-DDebug")
#SET(BEXT_DEFINITIONS )


# set this to ON to enable translations
#SET(BEXT_TRANSLATIONS_ENABLED ON)

# set this to list the languages for translations
#SET(BEXT_TRANSLATIONS_LANGS fr_FR)

# set this to list the extra files or directories to scan for strings to translate
#SET(BEXT_TRANSLATIONS_EXTRASCANS )


# set this to force an install path to replace the default one
#SET(BEXT_INSTALL_PATH "/my/install/path/")

 
 
