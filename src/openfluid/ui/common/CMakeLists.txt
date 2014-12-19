

SET(OPENFLUID_UI_COMMON_CPP OpenFLUIDDialog.cpp AboutDialog.cpp
                            RunSimulationDialog.cpp RunSimulationWorker.cpp                                  
                            ViewLogFileWindow.cpp
                            ClickableLabel.cpp ActionLabel.cpp ElidedLabel.cpp                             
                            PathsManagementWidget.cpp WaresSearchPathsWidget.cpp 
                            EditMarketplaceDialog.cpp
                            PreferencesDialog.cpp
                            )

SET(OPENFLUID_UI_COMMON_HPP OpenFLUIDDialog.hpp AboutDialog.hpp
                            RunSimulationDialog.hpp RunSimulationWorker.hpp
                            ViewLogFileWindow.hpp
                            ClickableLabel.hpp ActionLabel.hpp ElidedLabel.hpp                            
                            PathsManagementWidget.hpp WaresSearchPathsWidget.hpp
                            EditMarketplaceDialog.hpp
                            PreferencesDialog.hpp
                            )


QT4_WRAP_UI(OPENFLUID_UI_COMMON_UI RunSimulationDialog.ui AboutDialog.ui
                                   PathsManagementWidget.ui WaresSearchPathsWidget.ui
                                   PreferencesDialog.ui
                                   EditMarketplaceDialog.ui
                                   )
                                   

QT4_ADD_RESOURCES(OPENFLUID_UI_COMMON_RC resources/openfluiduicommon.qrc)

ADD_LIBRARY(openfluid-ui-common SHARED ${OPENFLUID_UI_COMMON_CPP} ${OPENFLUID_UI_COMMON_HPP}
                                       ${OPENFLUID_UI_COMMON_RC} ${OPENFLUID_UI_COMMON_UI})

SET_TARGET_PROPERTIES(openfluid-ui-common PROPERTIES VERSION "${VERSION_MAJOR}.${VERSION_MINOR}" 
                                          SOVERSION "${VERSION_MAJOR}.${VERSION_MINOR}.${VERSION_PATCH}"
                                          LIBRARY_OUTPUT_DIRECTORY "${LIB_OUTPUT_PATH}"
                                          RUNTIME_OUTPUT_DIRECTORY "${LIB_OUTPUT_PATH}"
                                          ARCHIVE_OUTPUT_DIRECTORY "${LIB_OUTPUT_PATH}"
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
        RUNTIME DESTINATION ${BIN_INSTALL_PATH}
        LIBRARY DESTINATION ${LIB_INSTALL_PATH}
        ARCHIVE DESTINATION ${LIB_INSTALL_PATH})
   

INSTALL(FILES ${OPENFLUID_UI_COMMON_HPP} DESTINATION ${INCLUDE_OPENFLUID_INSTALL_PATH}/ui/common)



   