
ADD_EXECUTABLE(${OPENFLUID_CMD_APP}
               DefaultIOListener.hpp 
               DefaultBuddiesListener.hpp 
               DefaultMachineListener.hpp VerboseMachineListener.hpp 
               OpenFLUID.cpp OpenFLUID.hpp             
               main.cpp
              )
SET_TARGET_PROPERTIES(${OPENFLUID_CMD_APP} PROPERTIES RUNTIME_OUTPUT_DIRECTORY "${OFBUILD_DIST_BIN_DIR}")

TARGET_LINK_LIBRARIES(${OPENFLUID_CMD_APP} 
                      ${QT_QTCORE_LIBRARIES}
                      openfluid-base
                      openfluid-buddies
                      openfluid-core
                      openfluid-machine
                      openfluid-tools
                      openfluid-fluidx
                      )



INSTALL(TARGETS ${OPENFLUID_CMD_APP}
        RUNTIME DESTINATION ${OFBUILD_BIN_INSTALL_PATH})

       
      

