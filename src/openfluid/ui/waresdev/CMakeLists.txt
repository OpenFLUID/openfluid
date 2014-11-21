


INCLUDE_DIRECTORIES(${QT_INCLUDES})


SET(OPENFLUID_UI_WARESDEV_CPP WareSrcWidget.cpp WareSrcToolbar.cpp WareSrcFileEditor.cpp WareSrcActions.cpp)

SET(OPENFLUID_UI_WARESDEV_HPP WareSrcWidget.hpp WareSrcToolbar.hpp WareSrcFileEditor.hpp WareSrcActions.hpp)

QT4_WRAP_UI(OPENFLUID_UI_WARESDEV_UI WareSrcWidget.ui)
                                   

#QT4_ADD_RESOURCES(OPENFLUID_UI_WARESDEV_RC resources/openfluiduiwaresdev.qrc)


ADD_LIBRARY(openfluid-ui-waresdev SHARED ${OPENFLUID_UI_WARESDEV_CPP} ${OPENFLUID_UI_WARESDEV_HPP}
                                         ${OPENFLUID_UI_WARESDEV_UI}
                                         #${OPENFLUID_UI_WARESDEV_RC}
                                         )

SET_TARGET_PROPERTIES(openfluid-ui-waresdev PROPERTIES VERSION "${VERSION_MAJOR}.${VERSION_MINOR}" 
                                        SOVERSION "${VERSION_MAJOR}.${VERSION_MINOR}.${VERSION_PATCH}"
                                        LIBRARY_OUTPUT_DIRECTORY "${LIB_OUTPUT_PATH}"
                                        RUNTIME_OUTPUT_DIRECTORY "${LIB_OUTPUT_PATH}"
                                        ARCHIVE_OUTPUT_DIRECTORY "${LIB_OUTPUT_PATH}"
                                        AUTOMOC ON
                                        )

TARGET_LINK_LIBRARIES(openfluid-ui-waresdev
                      #openfluid-base
                      #openfluid-core
                      #openfluid-ware
                      #openfluid-tools                                           
                      #openfluid-fluidx
                      openfluid-ui-common
                      ${QT_QTCORE_LIBRARIES}
                      ${QT_QTGUI_LIBRARIES}    
                     )



INSTALL(TARGETS openfluid-ui-waresdev
        RUNTIME DESTINATION ${BIN_INSTALL_PATH}
        LIBRARY DESTINATION ${LIB_INSTALL_PATH}
        ARCHIVE DESTINATION ${LIB_INSTALL_PATH})
   

INSTALL(FILES ${OPENFLUID_UI_WARESDEV_HPP} DESTINATION ${INCLUDE_OPENFLUID_INSTALL_PATH}/ui/waresdev)



   