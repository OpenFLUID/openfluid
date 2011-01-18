CMAKE_MINIMUM_REQUIRED (VERSION 2.6)

PKG_CHECK_MODULES(GTKMM REQUIRED gtkmm-2.4)

INCLUDE_DIRECTORIES(${GTKMM_INCLUDE_DIRS})
LINK_DIRECTORIES(${GTKMM_LIBRARY_DIRS})


FILE(GLOB OPENFLUID_MARKETCLIENT_CPP *.cpp)
FILE(GLOB OPENFLUID_MARKETCLIENT_HPP *.hpp)


ADD_EXECUTABLE(openfluid-market-client    
               ${OPENFLUID_MARKETCLIENT_CPP} ${OPENFLUID_MARKETCLIENT_HPP})

SET_TARGET_PROPERTIES(openfluid-market-client PROPERTIES RUNTIME_OUTPUT_DIRECTORY "${BIN_OUTPUT_PATH}")

TARGET_LINK_LIBRARIES(openfluid-market-client
                      ${Boost_SYSTEM_LIBRARY} ${Boost_FILESYSTEM_LIBRARY} 
#                      ${Boost_DATE_TIME_LIBRARY} ${Boost_PROGRAM_OPTIONS_LIBRARY} 
#                      ${Boost_REGEX_LIBRARY}
                      ${GTKMM_LIBRARIES}
                      openfluid-base
                      openfluid-core
                      openfluid-tools
                      openfluid-market)



#INSTALL(TARGETS openfluid-minimal
#        RUNTIME DESTINATION ${BIN_INSTALL_PATH})

       
      

