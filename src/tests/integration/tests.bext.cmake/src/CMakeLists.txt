
OPENFLUID_ADD_WAREPLUGIN(
  # list of C++ files
  CPP_FILES CMakeBExt.cpp 
  # list of Fortran files
  #FORTRAN_FILES
  # list of UI files (mainly for builderexts)
  UI_FILES bext.ui
  # list of RC files (mainly for builderexts)
  RC_FILES resources/bext.qrc
  # list of languages for translation
  #I18N_LANGS fr_FR
  # list of supplementary OpenFLUID libraries (e.g. tools, ...)
  OPENFLUID_COMPONENTS tools
  # custom target name, automatically generated if not provided
  #TARGET
  # custom installation path, standard path is used if not provided
  #INSTALL_PATH


  # enable build of parameterization UI (ON/OFF, OFF is default)
  WITH_PARAMSUI OFF
  # list of C++ files for parametrization UI
  #PARAMSUI_CPP_FILES
  # list of UI files for parametrization UI
  #PARAMSUI_UI_FILES
  # list of RC files for parametrization UI
  #PARAMSUI_RC_FILES
  # list of supplementary OpenFLUID libraries (e.g. tools, ...) for parametrization UI
  #PARAMSUI_OPENFLUID_COMPONENTS
  # custom target name for parametrization UI, automatically generated by default
  #PARAMSUI_TARGET
  # custom installation path for parametrization UI, standard path is used by default
  #PARAMSUI_INSTALL_PATH
)

