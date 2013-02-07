CMAKE_MINIMUM_REQUIRED (VERSION 2.6)


ADD_DEFINITIONS(${LibXML2_CFLAGS})

INCLUDE_DIRECTORIES(${LibXML2_INCLUDE_DIRS})
LINK_DIRECTORIES(${LibXML2_LIBRARY_DIRS})


ADD_EXECUTABLE(openfluid-minimal    
               main.cpp)

SET_TARGET_PROPERTIES(openfluid-minimal PROPERTIES RUNTIME_OUTPUT_DIRECTORY "${BIN_OUTPUT_PATH}")

TARGET_LINK_LIBRARIES(openfluid-minimal
                      ${Boost_REGEX_LIBRARY}
                      ${LibXML2_LIBRARIES}
                      openfluid-base
                      openfluid-core
                      openfluid-machine
                      openfluid-tools
                      openfluid-fluidx)



#INSTALL(TARGETS openfluid-minimal
#        RUNTIME DESTINATION ${BIN_INSTALL_PATH})

       
      

