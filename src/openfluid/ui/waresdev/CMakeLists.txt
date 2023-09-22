INCLUDE(${OpenFLUID_DIR}/OpenFLUIDDetectQt.cmake) # FIXME fix this since we only need SVG in this case, and do so for every file using this line without needing all items

SET(OPENFLUID_UI_WARESDEV_CPP WareSrcUIContainer.cpp
                              OStreamMsgStream.cpp
                              WareSrcMsgParser.cpp
                              GitUIProxy.cpp
                              WareSrcWidget.cpp WareSrcFileEditor.cpp WareSrcWidgetCollection.cpp
                              WareSrcToolbar.cpp WareBuildOptionsWidget.cpp WareBuildOptionsDialog.cpp
                              WareSrcSyntaxHighlighter.cpp WareSrcFiletypeManager.cpp TextEditMsgStream.cpp
                              WareSrcExplorer.cpp WareSrcExplorerModel.cpp WareExplorerDialog.cpp
                              NewWareDialog.cpp FindReplaceDialog.cpp WareSrcMsgViewer.cpp
                              SrcImportSequenceManager.cpp
                              HubManager.cpp
                              WaresSrcExportDialog.cpp WaresSrcImportDialog.cpp FragmentsSrcImportDialog.cpp FragmentCreationDialog.cpp WaresSrcIOProgressDialog.cpp
                              WareSrcActionsCollection.cpp
                              WareGitDialog.cpp WareFileEditor.cpp WaresImportFilterWidget.cpp
                              WaresDevPackage.cpp 
                              AbstractSrcImportDialog.cpp GitImportWorker.cpp WaresImportWorker.cpp FragmentsImportWorker.cpp
                              WorkspaceDevDashboardWorker.cpp
                              WorkspaceDevProcessWorker.cpp 
                              WorkspaceDevBuildWorker.cpp WorkspaceDevCheckWorker.cpp WorkspaceDevPurgeWorker.cpp
                              WorkspaceDevDashboardDialog.cpp WorkspaceDevWaresWidget.cpp WorkspaceDevGitWidget.cpp WorkspaceDevActionsWidget.cpp
                              WorkspaceDevProcessDialog.cpp
                              CompletionProvider.cpp
                              WareMigrationListener.cpp
                              MigrationSetupDialog.cpp
                              WareStatusDashboardWidget.cpp
                              WareStatusItemWidget.cpp)

SET(OPENFLUID_UI_WARESDEV_HPP WareSrcUIContainer.hpp
                              WareSrcMsgStream.hpp 
                              OStreamMsgStream.hpp
                              WareSrcMsgParser.hpp
                              GitUIProxy.hpp
                              WareSrcWidget.hpp WareSrcFileEditor.hpp WareSrcWidgetCollection.hpp
                              WareSrcToolbar.hpp WareBuildOptionsWidget.hpp WareBuildOptionsDialog.hpp
                              WareSrcSyntaxHighlighter.hpp WareSrcFiletypeManager.hpp TextEditMsgStream.hpp
                              WareSrcExplorer.hpp WareSrcExplorerModel.hpp WareExplorerDialog.hpp
                              NewWareDialog.hpp FindReplaceDialog.hpp WareSrcMsgViewer.hpp
                              SrcImportSequenceManager.hpp
                              HubManager.hpp
                              WaresSrcExportDialog.hpp WaresSrcImportDialog.hpp FragmentsSrcImportDialog.hpp FragmentCreationDialog.hpp WaresSrcIOProgressDialog.hpp
                              WareSrcActionsCollection.hpp
                              WareGitDialog.hpp WareFileEditor.hpp WaresImportFilterWidget.hpp
                              WaresDevPackage.hpp 
                              AbstractSrcImportDialog.hpp GitImportWorker.hpp WaresImportWorker.hpp FragmentsImportWorker.hpp
                              WorkspaceDevDashboardTypes.hpp WorkspaceDevDashboardWorker.hpp 
                              WorkspaceDevProcessWorker.hpp 
                              WorkspaceDevBuildWorker.hpp WorkspaceDevCheckWorker.hpp WorkspaceDevPurgeWorker.hpp
                              WorkspaceDevDashboardDialog.hpp WorkspaceDevWaresWidget.hpp WorkspaceDevGitWidget.hpp WorkspaceDevActionsWidget.hpp
                              WorkspaceDevProcessDialog.hpp
                              CompletionProvider.hpp
                              WareMigrationListener.hpp
                              MigrationSetupDialog.hpp
                              WareStatusDashboardWidget.hpp
                              WareStatusItemWidget.hpp)

IF (QT_VERSION_MAJOR LESS 6)
  QT5_WRAP_UI(OPENFLUID_UI_WARESDEV_UI WareSrcWidget.ui WareExplorerDialog.ui WareBuildOptionsWidget.ui WareBuildOptionsDialog.ui
                                     EmptyPage.ui CppPage.ui CMakeConfigPage.ui
                                     NewWareDialog.ui FindReplaceDialog.ui
                                     WaresSrcExportDialog.ui WaresSrcImportDialog.ui FragmentsSrcImportDialog.ui FragmentCreationDialog.ui WaresSrcIOProgressDialog.ui
                                     WaresImportFilterWidget.ui WareGitDialog.ui
                                     WorkspaceDevProcessDialog.ui
                                     WorkspaceDevDashboardDialog.ui WorkspaceDevWaresWidget.ui WorkspaceDevGitWidget.ui
                                     MigrationSetupDialog.ui
                                     WareStatusDashboardWidget.ui WareStatusItemWidget.ui)
ELSE()
  QT_WRAP_UI(OPENFLUID_UI_WARESDEV_UI WareSrcWidget.ui WareExplorerDialog.ui WareBuildOptionsWidget.ui WareBuildOptionsDialog.ui
                                     EmptyPage.ui CppPage.ui CMakeConfigPage.ui
                                     NewWareDialog.ui FindReplaceDialog.ui
                                     WaresSrcExportDialog.ui WaresSrcImportDialog.ui FragmentsSrcImportDialog.ui FragmentCreationDialog.ui WaresSrcIOProgressDialog.ui
                                     WaresImportFilterWidget.ui WareGitDialog.ui
                                     WorkspaceDevProcessDialog.ui
                                     WorkspaceDevDashboardDialog.ui WorkspaceDevWaresWidget.ui WorkspaceDevGitWidget.ui
                                     MigrationSetupDialog.ui
                                     WareStatusDashboardWidget.ui WareStatusItemWidget.ui)
ENDIF()
                                   

#QT5_ADD_RESOURCES(OPENFLUID_UI_WARESDEV_RC resources/openfluiduiwaresdev.qrc)

SET_SOURCE_FILES_PROPERTIES(GitUIProxy.cpp PROPERTIES COMPILE_DEFINITIONS "OFBUILD_GITASKPASS_ENVVAR=\"${OFBUILD_GITASKPASS_ENVVAR}\";OFBUILD_GITASKUSER_ENVVAR=\"${OFBUILD_GITASKUSER_ENVVAR}\"")


ADD_LIBRARY(openfluid-ui-waresdev SHARED ${OPENFLUID_UI_WARESDEV_CPP} ${OPENFLUID_UI_WARESDEV_HPP}
                                         ${OPENFLUID_UI_WARESDEV_UI}
                                         #${OPENFLUID_UI_WARESDEV_RC}
                                         )

SET_TARGET_PROPERTIES(openfluid-ui-waresdev PROPERTIES VERSION "${OPENFLUID_VERSION_MAJOR}.${OPENFLUID_VERSION_MINOR}" 
                                        SOVERSION "${OPENFLUID_VERSION_MAJOR}.${OPENFLUID_VERSION_MINOR}.${OPENFLUID_VERSION_PATCH}"
                                        LIBRARY_OUTPUT_DIRECTORY "${OFBUILD_DIST_LIB_DIR}"
                                        RUNTIME_OUTPUT_DIRECTORY "${OFBUILD_DIST_LIB_DIR}"
                                        ARCHIVE_OUTPUT_DIRECTORY "${OFBUILD_DIST_LIB_DIR}"
                                        AUTOMOC ON
                                        COMPILE_DEFINITIONS "OPENFLUID_DLL"
                                        DEFINE_SYMBOL "OPENFLUID_DLL_EXPORTS")

MESSAGE(STATUS "In ui/waresdev ${QT_VERSION_MAJOR}")

TARGET_LINK_LIBRARIES(openfluid-ui-waresdev
                      openfluid-ui-common
                      openfluid-waresdev
                      openfluid-utils
                      openfluid-tools
                      ${QT_QTCORE_LIBRARY}
                      ${QT_QTWIDGETS_LIBRARIES}
                     )


INSTALL(TARGETS openfluid-ui-waresdev
        RUNTIME DESTINATION ${OFBUILD_BIN_INSTALL_PATH}
        LIBRARY DESTINATION ${OFBUILD_LIB_INSTALL_PATH}
        ARCHIVE DESTINATION ${OFBUILD_LIB_INSTALL_PATH})
   
INSTALL(FILES ${OPENFLUID_UI_WARESDEV_HPP} DESTINATION ${OPENFLUID_INCLUDE_INSTALL_PATH}/ui/waresdev)


IF(OFBUILD_ENABLE_TESTING)
  ADD_SUBDIRECTORY(tests)
ENDIF()
