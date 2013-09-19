

INCLUDE_DIRECTORIES(${Boost_INCLUDE_DIRS})
LINK_DIRECTORIES(${Boost_LIBRARY_DIRS})

FILE(GLOB OPENFLUID_BUDDIES_CPP *.cpp)
FILE(GLOB OPENFLUID_BUDDIES_HPP *.hpp)


ADD_LIBRARY(openfluid-buddies SHARED ${OPENFLUID_BUDDIES_CPP} ${OPENFLUID_BUDDIES_HPP})

SET_TARGET_PROPERTIES(openfluid-buddies PROPERTIES VERSION "${VERSION_MAJOR}.${VERSION_MINOR}" 
                                        SOVERSION "${VERSION_MAJOR}.${VERSION_MINOR}.${VERSION_PATCH}"
                                        LIBRARY_OUTPUT_DIRECTORY "${LIB_OUTPUT_PATH}"
                                        RUNTIME_OUTPUT_DIRECTORY "${LIB_OUTPUT_PATH}"
                                        ARCHIVE_OUTPUT_DIRECTORY "${LIB_OUTPUT_PATH}")

TARGET_LINK_LIBRARIES(openfluid-buddies 
                      openfluid-base
                      openfluid-tools
                      openfluid-core                      
                      ${Boost_SYSTEM_LIBRARY} ${Boost_FILESYSTEM_LIBRARY}
                      ${Boost_REGEX_LIBRARY})




INSTALL(TARGETS openfluid-buddies
        RUNTIME DESTINATION ${BIN_INSTALL_PATH}
        LIBRARY DESTINATION ${LIB_INSTALL_PATH}
        ARCHIVE DESTINATION ${LIB_INSTALL_PATH})
   

INSTALL(FILES ${OPENFLUID_BUDDIES_HPP} DESTINATION ${INCLUDE_OPENFLUID_INSTALL_PATH}/buddies)

   