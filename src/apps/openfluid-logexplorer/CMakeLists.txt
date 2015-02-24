

SET(LOGEXPLORER_GUI_OPTION)

IF(WIN32)
  SET(LOGEXPLORER_GUI_OPTION WIN32)
ENDIF()

ADD_EXECUTABLE(openfluid-logexplorer ${LOGEXPLORER_GUI_OPTION} main.cpp ${LOGEXPLORER_WIN32RC})

SET_TARGET_PROPERTIES(openfluid-logexplorer PROPERTIES RUNTIME_OUTPUT_DIRECTORY "${BIN_OUTPUT_PATH}"
                                                   AUTOMOC ON)


TARGET_LINK_LIBRARIES(openfluid-logexplorer
                      openfluid-tools
                      openfluid-ui-common
                      ${QT_QTCORE_LIBRARIES}
                      )
                                   
               
INSTALL(TARGETS openfluid-logexplorer
        RUNTIME DESTINATION ${BIN_INSTALL_PATH})       