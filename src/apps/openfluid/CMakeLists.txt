
ADD_EXECUTABLE(${OPENFLUID_CMD_APP}
               DefaultIOListener.hpp 
               DefaultBuddiesListener.hpp 
               DefaultMachineListener.hpp VerboseMachineListener.hpp 
               OpenFLUID.cpp OpenFLUID.hpp             
               main.cpp
              )
SET_TARGET_PROPERTIES(${OPENFLUID_CMD_APP} PROPERTIES RUNTIME_OUTPUT_DIRECTORY "${BIN_OUTPUT_PATH}")

TARGET_LINK_LIBRARIES(${OPENFLUID_CMD_APP} 
                      ${Boost_SYSTEM_LIBRARY} 
                      ${Boost_DATE_TIME_LIBRARY} ${Boost_PROGRAM_OPTIONS_LIBRARY} 
                      ${Boost_REGEX_LIBRARY}
                      ${QT_QTCORE_LIBRARIES}
                      openfluid-base
                      openfluid-buddies
                      openfluid-core
                      openfluid-machine
                      openfluid-tools
                      openfluid-utils
                      openfluid-fluidx)



INSTALL(TARGETS ${OPENFLUID_CMD_APP}
        RUNTIME DESTINATION ${BIN_INSTALL_PATH})

       
      

