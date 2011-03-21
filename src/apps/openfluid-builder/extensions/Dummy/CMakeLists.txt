CMAKE_MINIMUM_REQUIRED (VERSION 2.6)


ADD_DEFINITIONS(${LibXML2_CFLAGS})

INCLUDE_DIRECTORIES(${LibXML2_INCLUDE_DIRS})
LINK_DIRECTORIES(${LibXML2_LIBRARY_DIRS})


ADD_LIBRARY(dummy MODULE
            Dummy.hpp Dummy.cpp
           )

SET_TARGET_PROPERTIES(dummy PROPERTIES PREFIX "" SUFFIX "${CMAKE_SHARED_LIBRARY_SUFFIX}bext")

SET_TARGET_PROPERTIES(dummy PROPERTIES 
                      RUNTIME_OUTPUT_DIRECTORY "${BUILDEREXT_OUTPUT_PATH}"
                      LIBRARY_OUTPUT_DIRECTORY "${BUILDEREXT_OUTPUT_PATH}"
                      ARCHIVE_OUTPUT_DIRECTORY "${BUILDEREXT_OUTPUT_PATH}")

TARGET_LINK_LIBRARIES(dummy 
                      ${Boost_SYSTEM_LIBRARY} ${Boost_FILESYSTEM_LIBRARY} 
                      ${Boost_DATE_TIME_LIBRARY} ${Boost_PROGRAM_OPTIONS_LIBRARY} 
                      ${Boost_REGEX_LIBRARY}
                      ${LibXML2_LIBRARIES}
                      openfluid-base
                      openfluid-buddies
                      openfluid-core
                      openfluid-machine
                      openfluid-io
                      openfluid-tools
                      openfluid-builderext)



#INSTALL(TARGETS dummy
#        RUNTIME DESTINATION ${BUILDEREXT_INSTALL_PATH})
       
      

