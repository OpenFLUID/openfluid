SET(OBS_CPP export.vars.files.csv/CSVFilesObs.cpp export.vars.files.csv/CSVObsTools.cpp)
SET(OBS_INSTALL_ENABLED ON)
SET(OBS_TRANSLATIONS_ENABLED ON)

SET(EXT_CPP export.vars.files.csv/EditFormatDialog.cpp export.vars.files.csv/EditSetDialog.cpp
            export.vars.files.csv/CSVObsParamsWidget.cpp export.vars.files.csv/CSVObsTools.cpp)
SET(EXT_UIFILES export.vars.files.csv/EditFormatDialog.ui export.vars.files.csv/EditSetDialog.ui
                export.vars.files.csv/CSVObsParamsWidget.ui)
SET(EXT_INSTALL_ENABLED ON)
SET(EXT_TRANSLATIONS_ENABLED OFF)
SET(EXT_BINDIR ${BUILDEREXTS_OUTPUT_PATH})
SET(EXT_EXTRA_LINKS openfluid-ui-common openfluid-tools)
SET(EXT_ID_SUFFIX "-parameterizationUI")

SET(WARE_LINKUID "{105202cc-2b0f-43db-b9c8-2ed19a6b00a4}")