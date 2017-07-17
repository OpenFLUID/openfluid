

SET(LOGEXPLORER_GUI_OPTION)

IF(WIN32)
  SET(LOGEXPLORER_GUI_OPTION WIN32)
ENDIF()

ADD_EXECUTABLE(openfluid-logexplorer ${LOGEXPLORER_GUI_OPTION} main.cpp ${LOGEXPLORER_WIN32RC})

SET_TARGET_PROPERTIES(openfluid-logexplorer PROPERTIES RUNTIME_OUTPUT_DIRECTORY "${OFBUILD_DIST_BIN_DIR}"
                                                   AUTOMOC ON)
                                                   
SET_TARGET_PROPERTIES(openfluid-logexplorer PROPERTIES OUTPUT_NAME ${OPENFLUID_LOGEXPLORER_APP})


TARGET_LINK_LIBRARIES(openfluid-logexplorer
                      openfluid-tools
                      openfluid-ui-common
                      Qt5::Core
                     )
                                   
               
INSTALL(TARGETS openfluid-logexplorer
        RUNTIME DESTINATION ${OFBUILD_BIN_INSTALL_PATH})       