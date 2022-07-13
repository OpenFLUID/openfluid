
ADD_EXECUTABLE(${OPENFLUID_CMD_APP}
                main.cpp
                InfoTasks.cpp RunTasks.cpp ReportTasks.cpp DataTasks.cpp WareTasks.cpp WorkspaceTasks.cpp
              )
SET_TARGET_PROPERTIES(${OPENFLUID_CMD_APP} PROPERTIES RUNTIME_OUTPUT_DIRECTORY "${OFBUILD_DIST_BIN_DIR}")

TARGET_LINK_LIBRARIES(${OPENFLUID_CMD_APP}
                      openfluid-base
                      openfluid-machine
                      openfluid-tools
                      openfluid-waresdev
                      openfluid-fluidx
                     )


FILE(GENERATE OUTPUT "${CMAKE_BINARY_DIR}/${OPENFLUID_CMD_APP}_path.txt" CONTENT $<TARGET_FILE:${OPENFLUID_CMD_APP}>)


INSTALL(TARGETS ${OPENFLUID_CMD_APP}
        RUNTIME DESTINATION ${OFBUILD_BIN_INSTALL_PATH})


IF(OFBUILD_ENABLE_TESTING)
  ADD_SUBDIRECTORY(tests)
ENDIF()
