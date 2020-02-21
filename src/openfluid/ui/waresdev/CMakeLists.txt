
SET(OPENFLUID_UI_WARESDEV_CPP WareSrcWidget.cpp WareSrcFileEditor.cpp WareSrcWidgetCollection.cpp
                              WareSrcToolbar.cpp WareBuildOptionsWidget.cpp                               
                              WareSrcSyntaxHighlighter.cpp WareSrcFiletypeManager.cpp TextEditMsgStream.cpp
                              WareSrcExplorer.cpp WareSrcExplorerModel.cpp WareExplorerDialog.cpp
                              NewSrcFileAssistant.cpp NewWareDialog.cpp FindReplaceDialog.cpp WareSrcMsgViewer.cpp
                              WaresSrcExportDialog.cpp WaresSrcImportDialog.cpp WaresSrcIOProgressDialog.cpp
                              WareGitDialog.cpp WareshubJsonEditor.cpp WareFileEditor.cpp WareshubIssueDialog.cpp
                              WorkspaceDevDashboardWorker.cpp
                              WorkspaceDevProcessWorker.cpp 
                              WorkspaceDevBuildWorker.cpp WorkspaceDevCheckWorker.cpp WorkspaceDevPurgeWorker.cpp
                              WorkspaceDevDashboardDialog.cpp WorkspaceDevWaresWidget.cpp WorkspaceDevGitWidget.cpp WorkspaceDevActionsWidget.cpp
                              WorkspaceDevProcessDialog.cpp)

SET(OPENFLUID_UI_WARESDEV_HPP WareSrcWidget.hpp WareSrcFileEditor.hpp WareSrcWidgetCollection.hpp
                              WareSrcToolbar.hpp WareBuildOptionsWidget.hpp
                              WareSrcSyntaxHighlighter.hpp WareSrcFiletypeManager.hpp TextEditMsgStream.hpp
                              WareSrcExplorer.hpp WareSrcExplorerModel.hpp WareExplorerDialog.hpp
                              NewSrcFileAssistant.hpp NewWareDialog.hpp FindReplaceDialog.hpp WareSrcMsgViewer.hpp
                              WaresSrcExportDialog.hpp WaresSrcImportDialog.hpp WaresSrcIOProgressDialog.hpp
                              WareGitDialog.hpp WareshubJsonEditor.hpp WareFileEditor.hpp WareshubIssueDialog.hpp
                              WorkspaceDevDashboardTypes.hpp WorkspaceDevDashboardWorker.hpp 
                              WorkspaceDevProcessWorker.hpp 
                              WorkspaceDevBuildWorker.hpp WorkspaceDevCheckWorker.hpp WorkspaceDevPurgeWorker.hpp
                              WorkspaceDevDashboardDialog.hpp WorkspaceDevWaresWidget.hpp WorkspaceDevGitWidget.hpp WorkspaceDevActionsWidget.hpp
                              WorkspaceDevProcessDialog.hpp)

QT5_WRAP_UI(OPENFLUID_UI_WARESDEV_UI WareSrcWidget.ui WareExplorerDialog.ui WareBuildOptionsWidget.ui
                                     EmptyPage.ui CppPage.ui CMakeConfigPage.ui
                                     NewSrcFileAssistant.ui NewWareDialog.ui FindReplaceDialog.ui
                                     WaresSrcExportDialog.ui WaresSrcImportDialog.ui WaresSrcIOProgressDialog.ui
                                     WareGitDialog.ui WareshubJsonEditor.ui WareshubIssueDialog.ui
                                     WorkspaceDevProcessDialog.ui
                                     WorkspaceDevDashboardDialog.ui WorkspaceDevWaresWidget.ui WorkspaceDevGitWidget.ui)
                                   

#QT5_ADD_RESOURCES(OPENFLUID_UI_WARESDEV_RC resources/openfluiduiwaresdev.qrc)


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

TARGET_LINK_LIBRARIES(openfluid-ui-waresdev
                      openfluid-ui-common
                      openfluid-waresdev
                      openfluid-utils
                      openfluid-tools
                      Qt5::Core
                      Qt5::Widgets                                            
                     )


INSTALL(TARGETS openfluid-ui-waresdev
        RUNTIME DESTINATION ${OFBUILD_BIN_INSTALL_PATH}
        LIBRARY DESTINATION ${OFBUILD_LIB_INSTALL_PATH}
        ARCHIVE DESTINATION ${OFBUILD_LIB_INSTALL_PATH})
   
INSTALL(FILES ${OPENFLUID_UI_WARESDEV_HPP} DESTINATION ${OPENFLUID_INCLUDE_INSTALL_PATH}/ui/waresdev)


IF(OFBUILD_ENABLE_TESTING)
  ADD_SUBDIRECTORY(tests)
ENDIF()  


   