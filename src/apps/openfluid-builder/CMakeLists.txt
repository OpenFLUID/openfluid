

FILE(READ "${PROJECT_SOURCE_DIR}/COPYING" BUILDER_LICENSE_TEXT)
STRING(REGEX REPLACE "\n" "\\\\n" BUILDER_LICENSE_TEXT "${BUILDER_LICENSE_TEXT}")
STRING(REGEX REPLACE "\"" "''" BUILDER_LICENSE_TEXT "${BUILDER_LICENSE_TEXT}")

FILE(READ "${PROJECT_SOURCE_DIR}/AUTHORS" BUILDER_AUTHORS_TEXT)
STRING(REGEX REPLACE "\n" "\\\\n" BUILDER_AUTHORS_TEXT "${BUILDER_AUTHORS_TEXT}")
STRING(REGEX REPLACE "\"" "''" BUILDER_AUTHORS_TEXT "${BUILDER_AUTHORS_TEXT}")

CONFIGURE_FILE(builderconfig.hpp.in "${CMAKE_CURRENT_BINARY_DIR}/builderconfig.hpp" @ONLY)
INCLUDE_DIRECTORIES("${CMAKE_CURRENT_BINARY_DIR}")


INCLUDE_DIRECTORIES(base datastore model monitoring spatial outputs runconfig extensions)
  

### BASE #

SET(BUILDER_BASE_CPP base/AppTools.cpp
                     base/AppCoordinator.cpp base/AppActions.cpp
                     base/BuilderApp.cpp base/MainWindow.cpp 
                     base/HomeModule.cpp base/HomeWidget.cpp                       
                     base/ProjectModule.cpp base/ProjectWidget.cpp base/DashboardWidget.cpp                     
                     base/ProjectCentral.cpp
                     base/ClickableLabel.cpp base/ActionLabel.cpp
                     base/NewProjectDialog.cpp
                     base/PreferencesDialog.cpp base/WaresSearchPathsWidget.cpp base/EditMarketplaceDialog.cpp
                     base/AboutDialog.cpp
                     base/OpenExampleProjectDialog.cpp
                     base/WaresManagementWidget.cpp base/WareWidget.cpp base/SignatureWidget.cpp
                     )
                     
SET(BUILDER_BASE_HPP base/AppTools.hpp
                     base/AppCoordinator.hpp base/AppActions.hpp
                     base/BuilderApp.hpp base/MainWindow.hpp
                     base/AbstractModule.hpp 
                     base/HomeModule.hpp base/HomeWidget.hpp base/DashboardWidget.hpp
                     base/ProjectModule.hpp base/ProjectWidget.hpp                     
                     base/ProjectCentral.hpp base/ProjectCheck.hpp
                     base/ClickableLabel.hpp base/ActionLabel.hpp
                     base/WorkspaceWidget.hpp
                     base/NewProjectDialog.hpp
                     base/PreferencesDialog.hpp base/WaresSearchPathsWidget.hpp base/EditMarketplaceDialog.hpp
                     base/AboutDialog.hpp
                     base/OpenExampleProjectDialog.hpp
                     base/WaresManagementWidget.hpp base/WareWidget.hpp base/SignatureWidget.hpp
                     )

QT4_WRAP_UI(BUILDER_BASE_UI base/HomeWidget.ui base/ProjectWidget.ui base/DashboardWidget.ui
                            base/NewProjectDialog.ui
                            base/PreferencesDialog.ui base/WaresSearchPathsWidget.ui base/EditMarketplaceDialog.ui
                            base/AboutDialog.ui
                            base/OpenExampleProjectDialog.ui
                            base/WaresManagementWidget.ui base/WareWidget.ui base/SignatureWidget.ui)


### MODEL #

SET(BUILDER_MODEL_CPP model/ModelWidget.cpp
                      model/SimulatorWidget.cpp
                      model/GeneratorWidget.cpp
                     )

SET(BUILDER_MODEL_HPP model/ModelWidget.hpp
                      model/SimulatorWidget.hpp
                      model/GeneratorWidget.hpp
                     )

QT4_WRAP_UI(BUILDER_MODEL_UI 
                                 )


### SPATIAL #

SET(BUILDER_SPATIAL_CPP spatial/SpatialDomainWidget.cpp
                          )

SET(BUILDER_SPATIAL_HPP spatial/SpatialDomainWidget.hpp
                           )

QT4_WRAP_UI(BUILDER_SPATIAL_UI spatial/SpatialDomainWidget.ui
                                 )


### DATASTORE #

SET(BUILDER_DATASTORE_CPP datastore/DatastoreWidget.cpp
                          )

SET(BUILDER_DATASTORE_HPP datastore/DatastoreWidget.hpp
                           )

QT4_WRAP_UI(BUILDER_DATASTORE_UI datastore/DatastoreWidget.ui
      
                                 )

### MONITORING #

SET(BUILDER_MONITORING_CPP monitoring/MonitoringWidget.cpp
                           monitoring/ObserverWidget.cpp
                          )

SET(BUILDER_MONITORING_HPP monitoring/MonitoringWidget.hpp
                           monitoring/ObserverWidget.hpp
                           )

QT4_WRAP_UI(BUILDER_MONITORING_UI 
                                 )


### SIMCONFIG #

SET(BUILDER_RUNCONFIG_CPP runconfig/RunConfigurationWidget.cpp)

SET(BUILDER_RUNCONFIG_HPP runconfig/RunConfigurationWidget.hpp)

QT4_WRAP_UI(BUILDER_RUNCONFIG_UI runconfig/RunConfigurationWidget.ui)



### OUTPUTS #

SET(BUILDER_OUTPUTS_CPP outputs/OutputsWidget.cpp
                          )

SET(BUILDER_OUTPUTS_HPP outputs/OutputsWidget.hpp
                           )

QT4_WRAP_UI(BUILDER_OUTPUTS_UI outputs/OutputsWidget.ui
                                 )


### EXTENSION #

SET(BUILDER_EXTENSIONS_CPP extensions/ExtensionPluginsManager.cpp 
                           extensions/ExtensionContainer.cpp
                           extensions/ExtensionsRegistry.cpp
                          )

SET(BUILDER_EXTENSIONS_HPP extensions/ExtensionPluginsManager.hpp 
                           extensions/ExtensionContainer.hpp
                           extensions/ExtensionsRegistry.hpp
                           )

QT4_WRAP_UI(BUILDER_EXTENSIONS_UI )



QT4_ADD_RESOURCES(BUILDER_RC resources/openfluidbuilder.qrc)


ADD_LIBRARY(openfluid-builder-main SHARED ${BUILDER_BASE_CPP} ${BUILDER_BASE_HPP} ${BUILDER_BASE_UI}
                                          ${BUILDER_MODEL_CPP} ${BUILDER_MODEL_HPP} ${BUILDER_MODEL_UI}
                                          ${BUILDER_SPATIAL_CPP} ${BUILDER_SPATIAL_HPP} ${BUILDER_SPATIAL_UI}
                                          ${BUILDER_DATASTORE_CPP} ${BUILDER_DATASTORE_HPP} ${BUILDER_DATASTORE_UI}
                                          ${BUILDER_MONITORING_CPP} ${BUILDER_MONITORING_HPP} ${BUILDER_MONITORING_UI}
                                          ${BUILDER_RUNCONFIG_CPP} ${BUILDER_RUNCONFIG_HPP} ${BUILDER_RUNCONFIG_UI}
                                          ${BUILDER_OUTPUTS_CPP} ${BUILDER_OUTPUTS_HPP} ${BUILDER_OUTPUTS_UI}
                                          ${BUILDER_EXTENSIONS_CPP} ${BUILDER_EXTENSIONS_HPP} ${BUILDER_EXTENSIONS_UI}
                                          ${BUILDER_RC})


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
                                   
                                   
IF(NOT (CMAKE_BUILD_TYPE MATCHES "Release"))
  ADD_SUBDIRECTORY(extensions/tests)
ENDIF()         