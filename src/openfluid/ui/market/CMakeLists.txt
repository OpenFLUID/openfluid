

SET(OPENFLUID_UI_MARKET_CPP MarketBuildOptionsDialog.cpp MarketClientAssistant.cpp MarketWizardPage.cpp 
                            MarketPackWidget.cpp MarketPackWidgetFormat.cpp
                            ViewLogFileWindow.cpp                                                         
                            )

SET(OPENFLUID_UI_MARKET_HPP MarketBuildOptionsDialog.hpp MarketClientAssistant.hpp MarketWizardPage.hpp 
                            MarketPackWidget.hpp MarketPackWidgetFormat.hpp
                            ViewLogFileWindow.hpp                                                                       
                            )


QT5_ADD_RESOURCES(OPENFLUID_UI_MARKET_RC resources/openfluiduimarket.qrc)


ADD_LIBRARY(openfluid-ui-market SHARED ${OPENFLUID_UI_MARKET_CPP} ${OPENFLUID_UI_MARKET_HPP}
                                       ${OPENFLUID_UI_MARKET_RC})


SET_TARGET_PROPERTIES(openfluid-ui-market PROPERTIES VERSION "${OPENFLUID_VERSION_MAJOR}.${OPENFLUID_VERSION_MINOR}" 
                                          SOVERSION "${OPENFLUID_VERSION_MAJOR}.${OPENFLUID_VERSION_MINOR}.${OPENFLUID_VERSION_PATCH}"
                                          LIBRARY_OUTPUT_DIRECTORY "${OFBUILD_DIST_LIB_DIR}"
                                          RUNTIME_OUTPUT_DIRECTORY "${OFBUILD_DIST_LIB_DIR}"
                                          ARCHIVE_OUTPUT_DIRECTORY "${OFBUILD_DIST_LIB_DIR}"
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
                      Qt5::Core
                      Qt5::Widgets                          
                     )



INSTALL(TARGETS openfluid-ui-market
        RUNTIME DESTINATION ${OFBUILD_BIN_INSTALL_PATH}
        LIBRARY DESTINATION ${OFBUILD_LIB_INSTALL_PATH}
        ARCHIVE DESTINATION ${OFBUILD_LIB_INSTALL_PATH})
   

INSTALL(FILES ${OPENFLUID_UI_MARKET_HPP} DESTINATION ${OPENFLUID_INCLUDE_INSTALL_PATH}/ui/market)
