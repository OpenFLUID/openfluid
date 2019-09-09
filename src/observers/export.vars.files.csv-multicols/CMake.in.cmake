INCLUDE_DIRECTORIES(.)

SET(OBS_CPP export.vars.files.csv-multicols/MultiCSVFilesObs.cpp export.vars.files.csv-multicols/MultiCSVObsTools.cpp)
SET(OBS_INSTALL_ENABLED ON)
SET(OBS_TRANSLATIONS_ENABLED ON)

SET(EXT_CPP export.vars.files.csv-multicols/MultiEditFormatDialog.cpp export.vars.files.csv-multicols/MultiEditSetDialog.cpp
            export.vars.files.csv-multicols/MultiCSVObsParamsWidget.cpp export.vars.files.csv-multicols/MultiCSVObsTools.cpp
            export.vars.files.csv-multicols/EditClassIDVarDialog.cpp)
SET(EXT_UIFILES export.vars.files.csv-multicols/MultiEditFormatDialog.ui export.vars.files.csv-multicols/MultiEditSetDialog.ui
                export.vars.files.csv-multicols/MultiCSVObsParamsWidget.ui export.vars.files.csv-multicols/EditClassIDVarDialog.ui)
SET(EXT_INSTALL_ENABLED ON)
SET(EXT_TRANSLATIONS_ENABLED OFF)
SET(EXT_BINDIR ${OFBUILD_DIST_BUILDEREXTS_DIR})
SET(EXT_EXTRA_LINKS openfluid-ui-common openfluid-tools)
SET(EXT_ID_SUFFIX "-parameterizationUI")

SET(WARE_LINKUID "{8b43cf34-f95f-4052-9640-6b20e6f54089}")