


INCLUDE_DIRECTORIES(${QT_INCLUDES})


SET(OPENFLUID_UI_WARESDEV_CPP WareSrcWidget.cpp WareSrcToolbar.cpp WareSrcFileEditor.cpp WareSrcWidgetCollection.cpp
                              WareSrcSyntaxHighlighter.cpp WareSrcFiletypeManager.cpp TextEditMsgStream.cpp
                              WareSrcExplorer.cpp WareSrcExplorerModel.cpp WareExplorerDialog.cpp
                              NewSrcFileAssistant.cpp NewWareDialog.cpp FindReplaceDialog.cpp)

SET(OPENFLUID_UI_WARESDEV_HPP WareSrcWidget.hpp WareSrcToolbar.hpp WareSrcFileEditor.hpp WareSrcWidgetCollection.hpp
                              WareSrcSyntaxHighlighter.hpp WareSrcFiletypeManager.hpp TextEditMsgStream.hpp
                              WareSrcExplorer.hpp WareSrcExplorerModel.hpp WareExplorerDialog.hpp
                              NewSrcFileAssistant.hpp NewWareDialog.hpp FindReplaceDialog.hpp)

QT4_WRAP_UI(OPENFLUID_UI_WARESDEV_UI WareSrcWidget.ui WareExplorerDialog.ui CppPage.ui CMakeConfigPage.ui
                              NewSrcFileAssistant.ui NewWareDialog.ui FindReplaceDialog.ui)
                                   

#QT4_ADD_RESOURCES(OPENFLUID_UI_WARESDEV_RC resources/openfluiduiwaresdev.qrc)


ADD_LIBRARY(openfluid-ui-waresdev SHARED ${OPENFLUID_UI_WARESDEV_CPP} ${OPENFLUID_UI_WARESDEV_HPP}
                                         ${OPENFLUID_UI_WARESDEV_UI}
                                         #${OPENFLUID_UI_WARESDEV_RC}
                                         )

SET_TARGET_PROPERTIES(openfluid-ui-waresdev PROPERTIES VERSION "${VERSION_MAJOR}.${VERSION_MINOR}" 
                                        SOVERSION "${VERSION_MAJOR}.${VERSION_MINOR}.${VERSION_PATCH}"
                                        LIBRARY_OUTPUT_DIRECTORY "${LIB_OUTPUT_PATH}"
                                        RUNTIME_OUTPUT_DIRECTORY "${LIB_OUTPUT_PATH}"
                                        ARCHIVE_OUTPUT_DIRECTORY "${LIB_OUTPUT_PATH}"
                                        AUTOMOC ON
                                        COMPILE_DEFINITIONS "OPENFLUID_DLL"
                                        DEFINE_SYMBOL "OPENFLUID_DLL_EXPORTS")

TARGET_LINK_LIBRARIES(openfluid-ui-waresdev
                      ${QT_QTCORE_LIBRARIES}
                      ${QT_QTGUI_LIBRARIES}    
                      openfluid-ui-common
                      openfluid-waresdev
                      openfluid-utils
                      openfluid-tools
                     )



INSTALL(TARGETS openfluid-ui-waresdev
        RUNTIME DESTINATION ${BIN_INSTALL_PATH}
        LIBRARY DESTINATION ${LIB_INSTALL_PATH}
        ARCHIVE DESTINATION ${LIB_INSTALL_PATH})
   

INSTALL(FILES ${OPENFLUID_UI_WARESDEV_HPP} DESTINATION ${INCLUDE_OPENFLUID_INSTALL_PATH}/ui/waresdev)

IF(NOT (CMAKE_BUILD_TYPE MATCHES "Release"))
  ADD_SUBDIRECTORY(tests)
ENDIF(NOT (CMAKE_BUILD_TYPE MATCHES "Release"))  


   