CMAKE_MINIMUM_REQUIRED (VERSION 2.6)


ADD_DEFINITIONS(${LibXML2_CFLAGS})

INCLUDE_DIRECTORIES(${LibXML2_INCLUDE_DIRS})
LINK_DIRECTORIES(${LibXML2_LIBRARY_DIRS})


ADD_EXECUTABLE(${OPENFLUID_CMD_APP}
               DefaultIOListener.hpp 
               DefaultBuddiesListener.hpp 
               DefaultMachineListener.hpp VerboseMachineListener.hpp 
               OpenFLUID.cpp OpenFLUID.hpp             
               main.cpp
              )
SET_TARGET_PROPERTIES(${OPENFLUID_CMD_APP} PROPERTIES RUNTIME_OUTPUT_DIRECTORY "${BIN_OUTPUT_PATH}")

TARGET_LINK_LIBRARIES(${OPENFLUID_CMD_APP} 
                      ${Boost_SYSTEM_LIBRARY} ${Boost_FILESYSTEM_LIBRARY} 
                      ${Boost_DATE_TIME_LIBRARY} ${Boost_PROGRAM_OPTIONS_LIBRARY} 
                      ${Boost_REGEX_LIBRARY}
                      ${LibXML2_LIBRARIES}
                      ${QT_QTCORE_LIBRARIES}
                      openfluid-base
                      openfluid-buddies
                      openfluid-core
                      openfluid-machine
                      openfluid-tools
                      openfluid-fluidx)



INSTALL(TARGETS ${OPENFLUID_CMD_APP}
        RUNTIME DESTINATION ${BIN_INSTALL_PATH})

       
      

