


INCLUDE_DIRECTORIES(${QT_INCLUDES})


FILE(GLOB OPENFLUID_WARESDEV_UI_CPP WareSrcWidget.cpp WareSrcToolbar.cpp WareSrcFileEditor.cpp)

FILE(GLOB OPENFLUID_WARESDEV_UI_HPP WareSrcWidget.hpp WareSrcToolbar.hpp WareSrcFileEditor.hpp)

QT4_WRAP_UI(OPENFLUID_WARESDEV_UI_UI WareSrcWidget.ui)
                                   

QT4_ADD_RESOURCES(OPENFLUID_WARESDEV_UI_RC resources/openfluidwaresdevui.qrc)


ADD_LIBRARY(openfluid-waresdev-ui SHARED ${OPENFLUID_WARESDEV_UI_CPP} ${OPENFLUID_WARESDEV_UI_HPP}
                                         ${OPENFLUID_WARESDEV_UI_UI}
                                         ${OPENFLUID_WARESDEV_UI_RC}
                                         )

SET_TARGET_PROPERTIES(openfluid-waresdev-ui PROPERTIES VERSION "${VERSION_MAJOR}.${VERSION_MINOR}" 
                                        SOVERSION "${VERSION_MAJOR}.${VERSION_MINOR}.${VERSION_PATCH}"
                                        LIBRARY_OUTPUT_DIRECTORY "${LIB_OUTPUT_PATH}"
                                        RUNTIME_OUTPUT_DIRECTORY "${LIB_OUTPUT_PATH}"
                                        ARCHIVE_OUTPUT_DIRECTORY "${LIB_OUTPUT_PATH}"
                                        AUTOMOC ON
                                        )

TARGET_LINK_LIBRARIES(openfluid-waresdev-ui
                      #openfluid-base
                      #openfluid-core
                      #openfluid-ware
                      #openfluid-tools                                           
                      #openfluid-fluidx
                      ${QT_QTCORE_LIBRARIES}
                      ${QT_QTGUI_LIBRARIES}    
                     )



INSTALL(TARGETS openfluid-waresdev-ui
        RUNTIME DESTINATION ${BIN_INSTALL_PATH}
        LIBRARY DESTINATION ${LIB_INSTALL_PATH}
        ARCHIVE DESTINATION ${LIB_INSTALL_PATH})
   

INSTALL(FILES ${OPENFLUID_WARESDEV_UI_HPP} DESTINATION ${INCLUDE_OPENFLUID_INSTALL_PATH}/waresdev-ui)



   