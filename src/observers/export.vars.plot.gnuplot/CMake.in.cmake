SET(OBS_CPP export.vars.plot.gnuplot/GNUplotObs.cpp export.vars.plot.gnuplot/GNUplotObsTools.cpp)
SET(OBS_INSTALL_ENABLED ON)
SET(OBS_TRANSLATIONS_ENABLED OFF)

SET(EXT_CPP export.vars.plot.gnuplot/GNUplotObsParamsWidget.cpp
            export.vars.plot.gnuplot/GNUplotObsTools.cpp)
SET(EXT_UIFILES export.vars.plot.gnuplot/GNUplotObsParamsWidget.ui)

SET(EXT_INSTALL_ENABLED ON)
SET(EXT_TRANSLATIONS_ENABLED OFF)
SET(EXT_BINDIR ${OFBUILD_DIST_BUILDEREXTS_DIR})
SET(EXT_EXTRA_LINKS openfluid-ui-common openfluid-tools)
SET(EXT_ID_SUFFIX "-parameterizationUI")

SET(WARE_LINKUID "{24c8551d-2660-4fe0-9085-ecb60a7dcebe}")
