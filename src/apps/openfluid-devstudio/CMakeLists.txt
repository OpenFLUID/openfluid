

CONFIGURE_FILE(devstudioconfig.hpp.in "${CMAKE_CURRENT_BINARY_DIR}/devstudioconfig.hpp" @ONLY)
INCLUDE_DIRECTORIES("${CMAKE_CURRENT_BINARY_DIR}")

SET(DEVSTUDIO_CPP MainWindow.cpp WareSrcExplorer.cpp WareSrcExplorerModel.cpp DevStudioPreferencesManager.cpp)
                     
SET(DEVSTUDIO_HPP MainWindow.hpp WareSrcExplorer.hpp WareSrcExplorerModel.hpp DevStudioPreferencesManager.hpp)

QT4_WRAP_UI(DEVSTUDIO_UI MainWindow.ui)

### RESOURCES #

QT4_ADD_RESOURCES(DEVSTUDIO_QRC resources/openfluiddevstudio.qrc)

SET(BUILDER_WIN32RC)

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
                      ${QT_QTCORE_LIBRARIES}
                      ${QT_QTGUI_LIBRARIES}
                      openfluid-ui-common
                      openfluid-waresdev
                      openfluid-ui-waresdev
                     )
       
       
                      
SET_TARGET_PROPERTIES(openfluid-devstudio-main PROPERTIES VERSION "${VERSION_MAJOR}.${VERSION_MINOR}" 
                                        SOVERSION "${VERSION_MAJOR}.${VERSION_MINOR}.${VERSION_PATCH}"
                                        LIBRARY_OUTPUT_DIRECTORY "${LIB_OUTPUT_PATH}"
                                        RUNTIME_OUTPUT_DIRECTORY "${LIB_OUTPUT_PATH}"
                                        ARCHIVE_OUTPUT_DIRECTORY "${LIB_OUTPUT_PATH}"
                                        AUTOMOC ON
                                        COMPILE_DEFINITIONS "OPENFLUID_DLL"
                                        DEFINE_SYMBOL "OPENFLUID_DLL_EXPORTS")


SET(DEVSTUDIO_GUI_OPTION)

IF(WIN32)
  SET(DEVSTUDIO_GUI_OPTION WIN32)
ENDIF()

ADD_EXECUTABLE(openfluid-devstudio ${DEVSTUDIO_GUI_OPTION} main.cpp ${DEVSTUDIO_WIN32RC})

SET_TARGET_PROPERTIES(openfluid-devstudio PROPERTIES RUNTIME_OUTPUT_DIRECTORY "${BIN_OUTPUT_PATH}"
                                                   AUTOMOC ON)

IF(APPLE)
  SET_TARGET_PROPERTIES(openfluid-devstudio PROPERTIES OUTPUT_NAME "OpenFLUID-DevStudio")
ENDIF()

TARGET_LINK_LIBRARIES(openfluid-devstudio
                      openfluid-devstudio-main
                      openfluid-ui-common
                      openfluid-waresdev
                      openfluid-ui-waresdev
                     )
                                   
INSTALL(TARGETS openfluid-devstudio-main
        RUNTIME DESTINATION ${BIN_INSTALL_PATH}
        LIBRARY DESTINATION ${LIB_INSTALL_PATH}
        ARCHIVE DESTINATION ${LIB_INSTALL_PATH})
               
INSTALL(TARGETS openfluid-devstudio
        RUNTIME DESTINATION ${BIN_INSTALL_PATH})       
      