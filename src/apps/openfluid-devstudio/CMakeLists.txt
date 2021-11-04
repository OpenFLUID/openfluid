INCLUDE_DIRECTORIES("${CMAKE_CURRENT_BINARY_DIR}")

SET(DEVSTUDIO_CPP MainWindow.cpp WorkspaceToolbar.cpp WareBuildStatusWidget.cpp)

SET(DEVSTUDIO_HPP MainWindow.hpp WorkspaceToolbar.hpp WareBuildStatusWidget.cpp)

QT5_WRAP_UI(DEVSTUDIO_UI MainWindow.ui WareBuildStatusWidget.ui)

### RESOURCES #

QT5_ADD_RESOURCES(DEVSTUDIO_QRC resources/openfluiddevstudio.qrc)

SET(DEVSTUDIO_WIN32RC)

IF(WIN32)
  IF(MINGW)
    # resource compilation for MinGW
    ADD_CUSTOM_COMMAND(OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/resources/openfluiddevstudio-win32.o
                       COMMAND windres.exe
                       ARGS -I${CMAKE_CURRENT_SOURCE_DIR}/resources
                            -i${CMAKE_CURRENT_SOURCE_DIR}/resources/openfluiddevstudio-win32.rc
                            -o${CMAKE_CURRENT_BINARY_DIR}/resources/openfluiddevstudio-win32.o)
    SET(DEVSTUDIO_WIN32RC ${CMAKE_CURRENT_BINARY_DIR}/resources/openfluiddevstudio-win32.o)
  ELSE()
    SET(DEVSTUDIO_WIN32RC ${CMAKE_CURRENT_SOURCE_DIR}/resources/openfluiddevstudio-win32.rc)
  ENDIF()
ENDIF()



### MAIN LIB AND EXECUTABLE #

ADD_LIBRARY(openfluid-devstudio-main SHARED ${DEVSTUDIO_CPP} ${DEVSTUDIO_HPP}
                                            ${DEVSTUDIO_UI} ${DEVSTUDIO_QRC})


TARGET_LINK_LIBRARIES(openfluid-devstudio-main
                      openfluid-ui-common
                      openfluid-waresdev
                      openfluid-ui-waresdev
                      Qt5::Core
                      Qt5::Widgets                      
                     )



SET_TARGET_PROPERTIES(openfluid-devstudio-main PROPERTIES VERSION "${OPENFLUID_VERSION_MAJOR}.${OPENFLUID_VERSION_MINOR}"
                                        SOVERSION "${OPENFLUID_VERSION_MAJOR}.${OPENFLUID_VERSION_MINOR}.${OPENFLUID_VERSION_PATCH}"
                                        LIBRARY_OUTPUT_DIRECTORY "${OFBUILD_DIST_LIB_DIR}"
                                        RUNTIME_OUTPUT_DIRECTORY "${OFBUILD_DIST_LIB_DIR}"
                                        ARCHIVE_OUTPUT_DIRECTORY "${OFBUILD_DIST_LIB_DIR}"
                                        AUTOMOC ON
                                        COMPILE_DEFINITIONS "OPENFLUID_DLL"
                                        DEFINE_SYMBOL "OPENFLUID_DLL_EXPORTS")


SET(DEVSTUDIO_GUI_OPTION)

IF(WIN32)
  SET(DEVSTUDIO_GUI_OPTION WIN32)
ENDIF()

IF(APPLE)
  SET(DEVSTUDIO_GUI_OPTION MACOSX_BUNDLE)
  SET(DEVSTUDIO_ICON_FILE "resources/icons/openfluid_icon_devstudio.icns")
  SET(MACOSX_BUNDLE_ICON_FILE openfluid_icon_devstudio.icns)
  SET_SOURCE_FILES_PROPERTIES("resources/icons/openfluid_icon_devstudio.icns" PROPERTIES MACOSX_PACKAGE_LOCATION Resources)
ENDIF()

ADD_EXECUTABLE(openfluid-devstudio ${DEVSTUDIO_GUI_OPTION} main.cpp ${DEVSTUDIO_WIN32RC} ${DEVSTUDIO_ICON_FILE})

SET_TARGET_PROPERTIES(openfluid-devstudio PROPERTIES RUNTIME_OUTPUT_DIRECTORY "${OFBUILD_DIST_BIN_DIR}"
                                                   AUTOMOC ON)


SET_TARGET_PROPERTIES(openfluid-devstudio PROPERTIES OUTPUT_NAME ${OPENFLUID_DEVSTUDIO_APP})

IF(APPLE)
  SET_TARGET_PROPERTIES(openfluid-devstudio PROPERTIES MACOSX_BUNDLE_ICON_FILE openfluid_icon_devstudio.icns)
 
  # Workaround to disable the macOS dark mode in OpenFLUID apps (currently not handled correctly by Qt)
  SET_TARGET_PROPERTIES(openfluid-devstudio PROPERTIES MACOSX_BUNDLE_INFO_PLIST "${CMAKE_SOURCE_DIR}/resources/packaging/macosx/Info.plist.in")
ENDIF()


TARGET_LINK_LIBRARIES(openfluid-devstudio
                      openfluid-devstudio-main
                      openfluid-ui-common
                      openfluid-waresdev
                      openfluid-ui-waresdev
                     )

FILE(GENERATE OUTPUT "${CMAKE_BINARY_DIR}/openfluid-devstudio_path.txt" CONTENT $<TARGET_FILE:openfluid-devstudio>)

INSTALL(TARGETS openfluid-devstudio-main
        RUNTIME DESTINATION ${OFBUILD_BIN_INSTALL_PATH}
        LIBRARY DESTINATION ${OFBUILD_LIB_INSTALL_PATH}
        ARCHIVE DESTINATION ${OFBUILD_LIB_INSTALL_PATH})

INSTALL(TARGETS openfluid-devstudio
        RUNTIME DESTINATION ${OFBUILD_BIN_INSTALL_PATH}
        BUNDLE DESTINATION ${OFBUILD_BIN_INSTALL_PATH})
