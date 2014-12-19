


INCLUDE_DIRECTORIES(${QT_INCLUDES})


SET(OPENFLUID_UI_MARKET_CPP MarketBuildOptionsDialog.cpp MarketClientAssistant.cpp MarketWizardPage.cpp 
                            MarketPackWidget.cpp MarketPackWidgetFormat.cpp                                  
                            )

SET(OPENFLUID_UI_MARKET_HPP MarketBuildOptionsDialog.hpp MarketClientAssistant.hpp MarketWizardPage.hpp 
                            MarketPackWidget.hpp MarketPackWidgetFormat.hpp                                           
                            )


QT4_ADD_RESOURCES(OPENFLUID_UI_MARKET_RC resources/openfluiduimarket.qrc)


ADD_LIBRARY(openfluid-ui-market SHARED ${OPENFLUID_UI_MARKET_CPP} ${OPENFLUID_UI_MARKET_HPP}
                                       ${OPENFLUID_UI_MARKET_RC})


SET_TARGET_PROPERTIES(openfluid-ui-market PROPERTIES VERSION "${VERSION_MAJOR}.${VERSION_MINOR}" 
                                          SOVERSION "${VERSION_MAJOR}.${VERSION_MINOR}.${VERSION_PATCH}"
                                          LIBRARY_OUTPUT_DIRECTORY "${LIB_OUTPUT_PATH}"
                                          RUNTIME_OUTPUT_DIRECTORY "${LIB_OUTPUT_PATH}"
                                          ARCHIVE_OUTPUT_DIRECTORY "${LIB_OUTPUT_PATH}"
                                          AUTOMOC ON
                                          COMPILE_DEFINITIONS "OPENFLUID_DLL"
                                          DEFINE_SYMBOL "OPENFLUID_DLL_EXPORTS")

TARGET_LINK_LIBRARIES(openfluid-ui-market
                      openfluid-ui-common
                      openfluid-base
                      openfluid-core
                      openfluid-tools
                      openfluid-machine
                      openfluid-market
                      openfluid-fluidx
                      ${QT_QTCORE_LIBRARIES}
                      ${QT_QTGUI_LIBRARIES}    
                     )



INSTALL(TARGETS openfluid-ui-market
        RUNTIME DESTINATION ${BIN_INSTALL_PATH}
        LIBRARY DESTINATION ${LIB_INSTALL_PATH}
        ARCHIVE DESTINATION ${LIB_INSTALL_PATH})
   

INSTALL(FILES ${OPENFLUID_UI_MARKET_HPP} DESTINATION ${INCLUDE_OPENFLUID_INSTALL_PATH}/ui/market)



   