
CONFIGURE_FILE(builderconfig.hpp.in "${CMAKE_CURRENT_BINARY_DIR}/builderconfig.hpp" @ONLY)
INCLUDE_DIRECTORIES("${CMAKE_CURRENT_BINARY_DIR}")


INCLUDE_DIRECTORIES(base)

SET(BUILDER_BASE_CPP base/AppTools.cpp
                     base/AppCoordinator.cpp base/AppActions.cpp
                     base/BuilderApp.cpp base/MainWindow.cpp 
                     base/HomeModule.cpp base/HomeWidget.cpp                       
                     base/ProjectModule.cpp base/ProjectWidget.cpp                     
                     base/ProjectCentral.cpp
                     base/NewProjectDialog.cpp
                     base/PreferencesDialog.cpp base/WaresSearchPathsWidget.cpp base/EditMarketplaceDialog.cpp
                     )
                     
SET(BUILDER_BASE_HPP base/AppTools.hpp
                     base/AppCoordinator.hpp base/AppActions.hpp
                     base/BuilderApp.hpp base/MainWindow.hpp
                     base/AbstractModule.hpp 
                     base/HomeModule.hpp base/HomeWidget.hpp 
                     base/ProjectModule.hpp base/ProjectWidget.hpp                     
                     base/ProjectCentral.hpp
                     base/NewProjectDialog.hpp
                     base/PreferencesDialog.hpp base/WaresSearchPathsWidget.hpp base/EditMarketplaceDialog.hpp
                     )


QT4_ADD_RESOURCES(BUILDER_RC resources/openfluidbuilder.qrc)

QT4_WRAP_UI(BUILDER_UI base/NewProjectDialog.ui
                       base/PreferencesDialog.ui
                       base/WaresSearchPathsWidget.ui
                       base/EditMarketplaceDialog.ui)


ADD_LIBRARY(openfluid-builder-main SHARED ${BUILDER_BASE_CPP} ${BUILDER_BASE_HPP}
                                          ${BUILDER_RC} ${BUILDER_UI})


TARGET_LINK_LIBRARIES(openfluid-builder-main 
                      ${Boost_SYSTEM_LIBRARY} ${Boost_FILESYSTEM_LIBRARY} 
                      ${Boost_DATE_TIME_LIBRARY} ${Boost_PROGRAM_OPTIONS_LIBRARY} 
                      ${Boost_REGEX_LIBRARY}
                      ${GDAL_LIBRARIES}
                      ${QT_QTCORE_LIBRARIES}
                      ${QT_QTGUI_LIBRARIES}                      
                      openfluid-base
                      openfluid-buddies
                      openfluid-core
                      openfluid-machine
                      openfluid-tools
                      openfluid-market
                      openfluid-guicommon
                      openfluid-fluidx)
                      
SET_TARGET_PROPERTIES(openfluid-builder-main PROPERTIES VERSION "${VERSION_MAJOR}.${VERSION_MINOR}" 
                                        SOVERSION "${VERSION_MAJOR}.${VERSION_MINOR}.${VERSION_PATCH}"
                                        LIBRARY_OUTPUT_DIRECTORY "${LIB_OUTPUT_PATH}"
                                        RUNTIME_OUTPUT_DIRECTORY "${LIB_OUTPUT_PATH}"
                                        ARCHIVE_OUTPUT_DIRECTORY "${LIB_OUTPUT_PATH}")

ADD_EXECUTABLE(openfluid-builder main.cpp)

SET_TARGET_PROPERTIES(openfluid-builder PROPERTIES RUNTIME_OUTPUT_DIRECTORY "${BIN_OUTPUT_PATH}")

IF(APPLE)
  set_property(TARGET openfluid-builder PROPERTY OUTPUT_NAME "OpenFLUID-Builder")
ENDIF()

TARGET_LINK_LIBRARIES(openfluid-builder 
                      ${Boost_SYSTEM_LIBRARY} ${Boost_FILESYSTEM_LIBRARY} 
                      ${Boost_DATE_TIME_LIBRARY} ${Boost_PROGRAM_OPTIONS_LIBRARY} 
                      ${Boost_REGEX_LIBRARY}
                      ${GDAL_LIBRARIES}
                      openfluid-base
                      openfluid-buddies
                      openfluid-core
                      openfluid-machine
                      openfluid-tools
                      openfluid-guicommon
                      openfluid-builder-main
                      openfluid-fluidx)
                                            