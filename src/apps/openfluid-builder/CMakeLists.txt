INCLUDE(${OpenFLUID_DIR}/QtNamespaceFacade.cmake)

CONFIGURE_FILE(builderconfig.hpp.in "${CMAKE_CURRENT_BINARY_DIR}/builderconfig.hpp" @ONLY)
INCLUDE_DIRECTORIES("${CMAKE_CURRENT_BINARY_DIR}")

INCLUDE_DIRECTORIES(base common datastore model monitoring spatial outputs runconfig extensions)


### BASE #

SET(BUILDER_BASE_CPP base/AppCoordinator.cpp base/AppActions.cpp base/WaresTranslationsRegistry.cpp
                     base/BuilderApp.cpp base/MainWindow.cpp
                     base/HomeModuleWidget.cpp base/ProjectModuleWidget.cpp
                     base/ProjectCentral.cpp base/ProjectCheckInfos.cpp
                     base/NewProjectDialog.cpp
                     base/OpenExampleProjectDialog.cpp
                     base/StatusMessagesWidget.cpp
                     base/DashboardFrame.cpp
                     base/DashboardWidget.cpp base/DashboardInfosWidget.cpp base/DashboardStatusWidget.cpp
                     base/SaveAsDialog.cpp
                     base/NewslineWidget.cpp base/NewsItemWidget.cpp base/NewslineDownloadWorker.cpp
                     base/RecentProjectLabel.cpp
                     base/WorkspaceTabWidget.cpp
                     base/EditProjectPropertiesDialog.cpp
                     )

SET(BUILDER_BASE_HPP base/AppCoordinator.hpp base/AppActions.hpp base/WaresTranslationsRegistry.hpp
                     base/BuilderApp.hpp base/MainWindow.hpp
                     base/HomeModuleWidget.cpp base/ProjectModuleWidget.hpp
                     base/ProjectCentral.hpp base/ProjectCheckInfos.hpp
                     base/NewProjectDialog.hpp
                     base/OpenExampleProjectDialog.hpp
                     base/StatusMessagesWidget.hpp
                     base/DashboardFrame.hpp
                     base/DashboardWidget.hpp base/DashboardInfosWidget.hpp base/DashboardStatusWidget.hpp
                     base/SaveAsDialog.hpp
                     base/NewslineWidget.hpp base/NewsItemWidget.hpp base/NewslineDownloadWorker.hpp
                     base/RecentProjectLabel.hpp
                     base/WorkspaceTabWidget.hpp
                     base/EditProjectPropertiesDialog.hpp
                     )

# Note: tried a wrapper to handle both qt5 and 6 names but issue with arg forwarding
IF (QT_VERSION_MAJOR LESS 6)
  QT5_WRAP_UI(BUILDER_BASE_UI base/HomeModuleWidget.ui base/ProjectModuleWidget.ui
                            base/NewProjectDialog.ui
                            base/OpenExampleProjectDialog.ui
                            base/StatusMessagesWidget.ui
                            base/DashboardInfosWidget.ui base/DashboardStatusWidget.ui
                            base/SaveAsDialog.ui
                            base/NewslineWidget.ui base/NewsItemWidget.ui
                            base/EditProjectPropertiesDialog.ui
                            )
ELSE()
  QT_WRAP_UI(BUILDER_BASE_UI base/HomeModuleWidget.ui base/ProjectModuleWidget.ui
                            base/NewProjectDialog.ui
                            base/OpenExampleProjectDialog.ui
                            base/StatusMessagesWidget.ui
                            base/DashboardInfosWidget.ui base/DashboardStatusWidget.ui
                            base/SaveAsDialog.ui
                            base/NewslineWidget.ui base/NewsItemWidget.ui
                            base/EditProjectPropertiesDialog.ui
                            )
ENDIF()



### COMMON #

SET(BUILDER_COMMON_CPP common/AppTools.cpp
                       common/WaresManagementWidget.cpp common/WareWidget.cpp
                       common/SignatureWidget.cpp common/ParameterWidget.cpp
                       common/AddWareDialog.cpp
                       common/AddParamDialog.cpp
                       )

SET(BUILDER_COMMON_HPP common/AbstractModuleWidget.hpp
                       common/WorkspaceWidget.hpp
                       common/AppTools.hpp
                       common/WaresManagementWidget.hpp common/WareWidget.hpp 
                       common/SignatureWidget.hpp common/ParameterWidget.hpp
                       common/AddWareDialog.hpp
                       common/AddParamDialog.hpp
                      )

IF (QT_VERSION_MAJOR LESS 6)
  QT5_WRAP_UI(BUILDER_COMMON_UI common/WaresManagementWidget.ui common/WareWidget.ui common/SignatureWidget.ui common/ParameterWidget.ui
                              common/AddWareDialog.ui
                              common/AddParamDialog.ui
                              )
ELSE()
  QT_WRAP_UI(BUILDER_COMMON_UI common/WaresManagementWidget.ui common/WareWidget.ui common/SignatureWidget.ui common/ParameterWidget.ui
                              common/AddWareDialog.ui
                              common/AddParamDialog.ui
                              )
ENDIF()



### MODEL #

SET(BUILDER_MODEL_CPP model/ModelWidget.cpp
                      model/ModelItemWidget.cpp
                      model/SimulatorWidget.cpp
                      model/GeneratorWidget.cpp
                      model/ModelScene.cpp model/ModelView.cpp
                      model/ModelItemGraphics.cpp
                      model/SimulatorGraphics.cpp model/GeneratorGraphics.cpp
                      model/ConnectorGraphics.cpp
                      model/AddSimulatorDialog.cpp model/AddGeneratorDialog.cpp
                     )

SET(BUILDER_MODEL_HPP model/ModelWidget.hpp
                      model/ModelItemWidget.hpp
                      model/SimulatorWidget.hpp
                      model/GeneratorWidget.hpp
                      model/ModelScene.hpp model/ModelView.hpp
                      model/ModelItemGraphics.hpp
                      model/SimulatorGraphics.hpp model/GeneratorGraphics.hpp
                      model/ConnectorGraphics.hpp
                      model/AddSimulatorDialog.hpp model/AddGeneratorDialog.hpp
                     )


IF (QT_VERSION_MAJOR LESS 6)
  QT5_WRAP_UI(BUILDER_MODEL_UI model/ModelWidget.ui model/AddGeneratorDialog.ui)
ELSE()
  QT_WRAP_UI(BUILDER_MODEL_UI model/ModelWidget.ui model/AddGeneratorDialog.ui)
ENDIF()


### SPATIAL #

SET(BUILDER_SPATIAL_CPP spatial/SpatialDomainWidget.cpp
                        spatial/UnitsClassWidget.cpp
                        spatial/EditAttributesValuesDialog.cpp
                        spatial/EditAttributeNameDialog.cpp
                        spatial/AddUnitDialog.cpp spatial/AddUnitsClassDialog.cpp spatial/AddUnitToClassDialog.cpp
                        spatial/AddConnectionDialog.cpp
                        spatial/EditEventDialog.cpp
                        spatial/AddEventDialog.cpp
                       )

SET(BUILDER_SPATIAL_HPP spatial/SpatialDomainWidget.hpp
                        spatial/UnitsClassWidget.hpp
                        spatial/EditAttributesValuesDialog.hpp
                        spatial/EditAttributeNameDialog.hpp
                        spatial/AddUnitDialog.hpp spatial/AddUnitsClassDialog.hpp spatial/AddUnitToClassDialog.hpp
                        spatial/AddConnectionDialog.hpp
                        spatial/EditEventDialog.hpp
                        spatial/AddEventDialog.hpp
                       )

IF (QT_VERSION_MAJOR LESS 6)
  QT5_WRAP_UI(BUILDER_SPATIAL_UI spatial/SpatialDomainWidget.ui
                               spatial/UnitsClassWidget.ui
                               spatial/EditAttributesValuesDialog.ui
                               spatial/EditAttributeNameDialog.ui
                               spatial/AddUnitDialog.ui
                               spatial/AddConnectionDialog.ui
                               spatial/EditEventDialog.ui
                             )
ELSE()
  QT_WRAP_UI(BUILDER_SPATIAL_UI spatial/SpatialDomainWidget.ui
                               spatial/UnitsClassWidget.ui
                               spatial/EditAttributesValuesDialog.ui
                               spatial/EditAttributeNameDialog.ui
                               spatial/AddUnitDialog.ui
                               spatial/AddConnectionDialog.ui
                               spatial/EditEventDialog.ui
                             )
ENDIF()


### DATASTORE #

SET(BUILDER_DATASTORE_CPP datastore/DatastoreWidget.cpp
                          datastore/AddDatastoreItemDialog.cpp
                          )

SET(BUILDER_DATASTORE_HPP datastore/DatastoreWidget.hpp
                          datastore/AddDatastoreItemDialog.hpp
                          )

IF (QT_VERSION_MAJOR LESS 6)
  QT5_WRAP_UI(BUILDER_DATASTORE_UI datastore/DatastoreWidget.ui
                                 datastore/EditDatastoreItemDialog.ui
                                 )
ELSE()
  QT_WRAP_UI(BUILDER_DATASTORE_UI datastore/DatastoreWidget.ui
                                 datastore/EditDatastoreItemDialog.ui
                                 )
ENDIF()

### MONITORING #

SET(BUILDER_MONITORING_CPP monitoring/MonitoringWidget.cpp
                           monitoring/ObserverWidget.cpp
                           monitoring/AddObserverDialog.cpp
                          )

SET(BUILDER_MONITORING_HPP monitoring/MonitoringWidget.hpp
                           monitoring/ObserverWidget.hpp
                           monitoring/AddObserverDialog.hpp
                           )

IF (QT_VERSION_MAJOR LESS 6)
  QT5_WRAP_UI(BUILDER_MONITORING_UI monitoring/MonitoringWidget.ui
                                  )
ELSE()
  QT_WRAP_UI(BUILDER_MONITORING_UI monitoring/MonitoringWidget.ui
                                  )
ENDIF()


### SIMCONFIG #

SET(BUILDER_RUNCONFIG_CPP runconfig/RunConfigurationWidget.cpp)

SET(BUILDER_RUNCONFIG_HPP runconfig/RunConfigurationWidget.hpp)

IF (QT_VERSION_MAJOR LESS 6)
  QT5_WRAP_UI(BUILDER_RUNCONFIG_UI runconfig/RunConfigurationWidget.ui)
ELSE()
  QT_WRAP_UI(BUILDER_RUNCONFIG_UI runconfig/RunConfigurationWidget.ui)
ENDIF()



### OUTPUTS #

SET(BUILDER_OUTPUTS_CPP outputs/OutputsWidget.cpp
                          )

SET(BUILDER_OUTPUTS_HPP outputs/OutputsWidget.hpp
                           )

IF (QT_VERSION_MAJOR LESS 6)
  QT5_WRAP_UI(BUILDER_OUTPUTS_UI outputs/OutputsWidget.ui
                                 )
ELSE()
  QT_WRAP_UI(BUILDER_OUTPUTS_UI outputs/OutputsWidget.ui
                                 )
ENDIF()


### EXTENSION #

SET(BUILDER_EXTENSIONS_CPP extensions/ExtensionPluginsManager.cpp
                           extensions/ExtensionsRegistry.cpp
                          )

SET(BUILDER_EXTENSIONS_HPP extensions/ExtensionPluginsManager.hpp
                           extensions/ExtensionInstance.hpp
                           extensions/ExtensionsRegistry.hpp
                           )

IF (QT_VERSION_MAJOR LESS 6)
  QT5_WRAP_UI(BUILDER_EXTENSIONS_UI )
ELSE()
  QT_WRAP_UI(BUILDER_EXTENSIONS_UI )
ENDIF()


### RESOURCES #

IF (QT_VERSION_MAJOR LESS 6)
  QT5_ADD_RESOURCES(BUILDER_QRC resources/openfluidbuilder.qrc)
ELSE()
  QT_ADD_RESOURCES(BUILDER_QRC resources/openfluidbuilder.qrc)
ENDIF()

SET(BUILDER_WIN32RC)

IF(WIN32)
  IF(MINGW)
    # resource compilation for MinGW
    ADD_CUSTOM_COMMAND(OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/resources/openfluidbuilder-win32.o
                       COMMAND windres.exe
                       ARGS -I${CMAKE_CURRENT_SOURCE_DIR}/resources
                            -i${CMAKE_CURRENT_SOURCE_DIR}/resources/openfluidbuilder-win32.rc
                            -o${CMAKE_CURRENT_BINARY_DIR}/resources/openfluidbuilder-win32.o)
    SET(BUILDER_WIN32RC ${CMAKE_CURRENT_BINARY_DIR}/resources/openfluidbuilder-win32.o)
  ELSE()
    SET(BUILDER_WIN32RC ${CMAKE_CURRENT_SOURCE_DIR}/resources/openfluidbuilder-win32.rc)
  ENDIF()
ENDIF()


### MAIN LIB AND EXECUTABLE #


IF(OFBUILD_ENABLE_WARESDEV_BUILDER_INTEGRATION)
  ADD_DEFINITIONS(-DENABLE_WARESDEV_INTEGRATION)
ENDIF()


ADD_LIBRARY(openfluid-builder-main SHARED ${BUILDER_BASE_CPP} ${BUILDER_BASE_HPP} ${BUILDER_BASE_UI}
                                          ${BUILDER_COMMON_CPP} ${BUILDER_COMMON_HPP} ${BUILDER_COMMON_UI}
                                          ${BUILDER_MODEL_CPP} ${BUILDER_MODEL_HPP} ${BUILDER_MODEL_UI}
                                          ${BUILDER_SPATIAL_CPP} ${BUILDER_SPATIAL_HPP} ${BUILDER_SPATIAL_UI}
                                          ${BUILDER_DATASTORE_CPP} ${BUILDER_DATASTORE_HPP} ${BUILDER_DATASTORE_UI}
                                          ${BUILDER_MONITORING_CPP} ${BUILDER_MONITORING_HPP} ${BUILDER_MONITORING_UI}
                                          ${BUILDER_RUNCONFIG_CPP} ${BUILDER_RUNCONFIG_HPP} ${BUILDER_RUNCONFIG_UI}
                                          ${BUILDER_OUTPUTS_CPP} ${BUILDER_OUTPUTS_HPP} ${BUILDER_OUTPUTS_UI}
                                          ${BUILDER_EXTENSIONS_CPP} ${BUILDER_EXTENSIONS_HPP} ${BUILDER_EXTENSIONS_UI}
                                          ${BUILDER_QRC})


TARGET_LINK_LIBRARIES(openfluid-builder-main
                      openfluid-base
                      openfluid-core
                      openfluid-machine
                      openfluid-ware
                      openfluid-fluidx
                      openfluid-tools
                      openfluid-utils
                      openfluid-ui-common
                      openfluid-ui-spatial
                      openfluid-ui-builderext
                      openfluid-ui-waresdev
                      openfluid-waresdev
                      ${QT_QTCORE_LIBRARY}
                      ${QT_QTWIDGETS_LIBRARIES}
                      ${QT_QTSVG_LIBRARY}
                      ${GDAL_LIBRARIES}
                     )


SET_TARGET_PROPERTIES(openfluid-builder-main PROPERTIES VERSION "${OPENFLUID_VERSION_MAJOR}.${OPENFLUID_VERSION_MINOR}"
                                        SOVERSION "${OPENFLUID_VERSION_MAJOR}.${OPENFLUID_VERSION_MINOR}.${OPENFLUID_VERSION_PATCH}"
                                        LIBRARY_OUTPUT_DIRECTORY "${OFBUILD_DIST_LIB_DIR}"
                                        RUNTIME_OUTPUT_DIRECTORY "${OFBUILD_DIST_LIB_DIR}"
                                        ARCHIVE_OUTPUT_DIRECTORY "${OFBUILD_DIST_LIB_DIR}"
                                        AUTOMOC ON
                                        COMPILE_DEFINITIONS "OPENFLUID_DLL"
                                        DEFINE_SYMBOL "OPENFLUID_DLL_EXPORTS")


SET(BUILDER_GUI_OPTION)

IF(WIN32)
  SET(BUILDER_GUI_OPTION WIN32)
ENDIF()

IF(APPLE)
  SET(BUILDER_GUI_OPTION MACOSX_BUNDLE )
  SET(BUILDER_ICON_FILE "resources/icons/openfluid_icon_builder.icns")
  SET(MACOSX_BUNDLE_ICON_FILE openfluid_icon_builder.icns)
  SET_SOURCE_FILES_PROPERTIES("resources/icons/openfluid_icon_builder.icns" PROPERTIES MACOSX_PACKAGE_LOCATION Resources)
ENDIF()

ADD_EXECUTABLE(openfluid-builder ${BUILDER_GUI_OPTION} main.cpp ${BUILDER_WIN32RC} ${BUILDER_ICON_FILE})

SET_TARGET_PROPERTIES(openfluid-builder PROPERTIES RUNTIME_OUTPUT_DIRECTORY "${OFBUILD_DIST_BIN_DIR}"
                                                   AUTOMOC ON)

SET_TARGET_PROPERTIES(openfluid-builder PROPERTIES OUTPUT_NAME ${OPENFLUID_BUILDER_APP})

IF(APPLE)
  SET_TARGET_PROPERTIES(openfluid-builder PROPERTIES MACOSX_BUNDLE_ICON_FILE openfluid_icon_builder.icns)

  # Workaround to disable the macOS dark mode in OpenFLUID apps (currently not handled correctly by Qt)
  SET_TARGET_PROPERTIES(openfluid-builder PROPERTIES MACOSX_BUNDLE_INFO_PLIST "${CMAKE_SOURCE_DIR}/resources/packaging/macosx/Info.plist.in")
ENDIF()

TARGET_LINK_LIBRARIES(openfluid-builder
                      ${GDAL_LIBRARIES}
                      openfluid-base
                      openfluid-core
                      openfluid-ware
                      openfluid-machine
                      openfluid-tools
                      openfluid-utils
                      openfluid-ui-common
                      openfluid-builder-main
                      openfluid-fluidx
                      openfluid-ui-waresdev
                      openfluid-waresdev)


FILE(GENERATE OUTPUT "${CMAKE_BINARY_DIR}/openfluid-builder_path.txt" CONTENT $<TARGET_FILE:openfluid-builder>)


INSTALL(TARGETS openfluid-builder-main
        RUNTIME DESTINATION ${OFBUILD_BIN_INSTALL_PATH}
        LIBRARY DESTINATION ${OFBUILD_LIB_INSTALL_PATH}
        ARCHIVE DESTINATION ${OFBUILD_LIB_INSTALL_PATH})

INSTALL(TARGETS openfluid-builder
        RUNTIME DESTINATION ${OFBUILD_BIN_INSTALL_PATH}
        BUNDLE DESTINATION ${OFBUILD_BIN_INSTALL_PATH})

IF(OFBUILD_ENABLE_TESTING AND NOT WIN32)
  ADD_SUBDIRECTORY(extensions/tests)
ENDIF()
