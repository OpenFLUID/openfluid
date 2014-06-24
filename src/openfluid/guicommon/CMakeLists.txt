


INCLUDE_DIRECTORIES(${QT_INCLUDES})


FILE(GLOB OPENFLUID_GUICOMMON_CPP PreferencesManager.cpp
                                  RunSimulationDialog.cpp RunSimulationWorker.cpp
                                  MarketBuildOptionsDialog.cpp MarketClientAssistant.cpp MarketWizardPage.cpp 
                                  MarketPackWidget.cpp MarketPackWidgetFormat.cpp
                                  ViewLogFileWindow.cpp
                                  )

FILE(GLOB OPENFLUID_GUICOMMON_HPP PreferencesManager.hpp
                                  RunSimulationDialog.hpp RunSimulationWorker.hpp
                                  MarketBuildOptionsDialog.hpp MarketClientAssistant.hpp MarketWizardPage.hpp 
                                  MarketPackWidget.hpp MarketPackWidgetFormat.hpp
                                  ViewLogFileWindow.hpp                                                                     
                                  )

QT4_WRAP_UI(OPENFLUID_GUICOMMON_UI RunSimulationDialog.ui)

QT4_ADD_RESOURCES(OPENFLUID_GUICOMMON_RC resources/openfluidmarket.qrc)

ADD_LIBRARY(openfluid-guicommon SHARED ${OPENFLUID_GUICOMMON_CPP} ${OPENFLUID_GUICOMMON_HPP}
                                       ${OPENFLUID_GUICOMMON_RC} ${OPENFLUID_GUICOMMON_UI})

SET_TARGET_PROPERTIES(openfluid-guicommon PROPERTIES VERSION "${VERSION_MAJOR}.${VERSION_MINOR}" 
                                        SOVERSION "${VERSION_MAJOR}.${VERSION_MINOR}.${VERSION_PATCH}"
                                        LIBRARY_OUTPUT_DIRECTORY "${LIB_OUTPUT_PATH}"
                                        RUNTIME_OUTPUT_DIRECTORY "${LIB_OUTPUT_PATH}"
                                        ARCHIVE_OUTPUT_DIRECTORY "${LIB_OUTPUT_PATH}"
                                        AUTOMOC ON
                                        )

TARGET_LINK_LIBRARIES(openfluid-guicommon
                      openfluid-base
                      openfluid-core
                      openfluid-tools
                      openfluid-machine
                      openfluid-market
                      openfluid-fluidx
                      ${QT_QTCORE_LIBRARIES}
                      ${QT_QTGUI_LIBRARIES}    
                     )



INSTALL(TARGETS openfluid-guicommon
        RUNTIME DESTINATION ${BIN_INSTALL_PATH}
        LIBRARY DESTINATION ${LIB_INSTALL_PATH}
        ARCHIVE DESTINATION ${LIB_INSTALL_PATH})
   

INSTALL(FILES ${OPENFLUID_GUICOMMON_HPP} DESTINATION ${INCLUDE_OPENFLUID_INSTALL_PATH}/guicommon)



   