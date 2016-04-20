

SET(OPENFLUID_UI_COMMON_CPP OpenFLUIDDialog.cpp AboutDialog.cpp OpenFLUIDSplashScreen.cpp
                            RunSimulationDialog.cpp RunSimulationWorker.cpp
                            SignatureDataEditorWidget.cpp
                            SignatureEditorWidget.cpp EditSignatureDialog.cpp                                
                            ClickableLabel.cpp ActionLabel.cpp ElidedLabel.cpp
                            ShortcutCompleter.cpp
                            PathsManagementWidget.cpp WaresSearchPathsWidget.cpp
                            LogExplorerDialog.cpp 
                            EditMarketplaceDialog.cpp
                            PreferencesDialog.cpp DetectQtDevToolsDialog.cpp
                            )

SET(OPENFLUID_UI_COMMON_HPP OpenFLUIDDialog.hpp AboutDialog.hpp OpenFLUIDSplashScreen.hpp
                            RunSimulationDialog.hpp RunSimulationWorker.hpp
                            SignatureDataEditorWidget.hpp
                            SignatureEditorWidget.hpp EditSignatureDialog.hpp
                            ClickableLabel.hpp ActionLabel.hpp ElidedLabel.hpp
                            ShortcutCompleter.hpp                         
                            PathsManagementWidget.hpp WaresSearchPathsWidget.hpp
                            LogExplorerDialog.hpp
                            EditMarketplaceDialog.hpp
                            PreferencesDialog.hpp DetectQtDevToolsDialog.hpp
                            UIHelpers.hpp
                            )


QT4_WRAP_UI(OPENFLUID_UI_COMMON_UI RunSimulationDialog.ui AboutDialog.ui
                                   SignatureEditorWidget.ui SignatureDataEditorWidget.ui EditSignatureDialog.ui
                                   PathsManagementWidget.ui WaresSearchPathsWidget.ui
                                   LogExplorerDialog.ui
                                   PreferencesDialog.ui DetectQtDevToolsDialog.ui
                                   EditMarketplaceDialog.ui
                                   )
                                   

QT4_ADD_RESOURCES(OPENFLUID_UI_COMMON_RC resources/openfluiduicommon.qrc)

ADD_LIBRARY(openfluid-ui-common SHARED ${OPENFLUID_UI_COMMON_CPP} ${OPENFLUID_UI_COMMON_HPP}
                                       ${OPENFLUID_UI_COMMON_RC} ${OPENFLUID_UI_COMMON_UI})

SET_TARGET_PROPERTIES(openfluid-ui-common PROPERTIES VERSION "${OPENFLUID_VERSION_MAJOR}.${OPENFLUID_VERSION_MINOR}" 
                                          SOVERSION "${OPENFLUID_VERSION_MAJOR}.${OPENFLUID_VERSION_MINOR}.${OPENFLUID_VERSION_PATCH}"
                                          LIBRARY_OUTPUT_DIRECTORY "${OFBUILD_DIST_LIB_DIR}"
                                          RUNTIME_OUTPUT_DIRECTORY "${OFBUILD_DIST_LIB_DIR}"
                                          ARCHIVE_OUTPUT_DIRECTORY "${OFBUILD_DIST_LIB_DIR}"
                                          AUTOMOC ON
                                          COMPILE_DEFINITIONS "OPENFLUID_DLL"
                                          DEFINE_SYMBOL "OPENFLUID_DLL_EXPORTS")

TARGET_LINK_LIBRARIES(openfluid-ui-common
                      openfluid-base
                      openfluid-core
                      openfluid-tools
                      openfluid-machine
                      openfluid-fluidx
                      ${QT_QTCORE_LIBRARIES}
                      ${QT_QTGUI_LIBRARIES}    
                     )



INSTALL(TARGETS openfluid-ui-common
        RUNTIME DESTINATION ${OFBUILD_BIN_INSTALL_PATH}
        LIBRARY DESTINATION ${OFBUILD_LIB_INSTALL_PATH}
        ARCHIVE DESTINATION ${OFBUILD_LIB_INSTALL_PATH})
   

INSTALL(FILES ${OPENFLUID_UI_COMMON_HPP} DESTINATION ${OPENFLUID_INCLUDE_INSTALL_PATH}/ui/common)



   