

INCLUDE_DIRECTORIES(${Boost_INCLUDE_DIRS})


SET(OPENFLUID_BUDDIES_CPP OpenFLUIDBuddy.cpp
                          ExamplesBuddy.cpp NewSimBuddy.cpp Sim2DocBuddy.cpp NewDataBuddy.cpp
                          BuddiesListener.cpp
                          )
    
SET(OPENFLUID_BUDDIES_HPP OpenFLUIDBuddy.hpp 
                          ExamplesBuddy.hpp NewSimBuddy.hpp Sim2DocBuddy.hpp NewDataBuddy.hpp
                          BuddiesListener.hpp
                          )


ADD_LIBRARY(openfluid-buddies SHARED ${OPENFLUID_BUDDIES_CPP} ${OPENFLUID_BUDDIES_HPP})

SET_TARGET_PROPERTIES(openfluid-buddies PROPERTIES VERSION "${OPENFLUID_VERSION_MAJOR}.${OPENFLUID_VERSION_MINOR}" 
                                        SOVERSION "${OPENFLUID_VERSION_MAJOR}.${OPENFLUID_VERSION_MINOR}.${OPENFLUID_VERSION_PATCH}"
                                        LIBRARY_OUTPUT_DIRECTORY "${OFBUILD_DIST_LIB_DIR}"
                                        RUNTIME_OUTPUT_DIRECTORY "${OFBUILD_DIST_LIB_DIR}"
                                        ARCHIVE_OUTPUT_DIRECTORY "${OFBUILD_DIST_LIB_DIR}"
                                        COMPILE_DEFINITIONS "OPENFLUID_DLL"
                                        DEFINE_SYMBOL "OPENFLUID_DLL_EXPORTS")

TARGET_LINK_LIBRARIES(openfluid-buddies 
                      openfluid-core
                      openfluid-base
                      openfluid-tools
                      openfluid-utils)




INSTALL(TARGETS openfluid-buddies
        RUNTIME DESTINATION ${OFBUILD_BIN_INSTALL_PATH}
        LIBRARY DESTINATION ${OFBUILD_LIB_INSTALL_PATH}
        ARCHIVE DESTINATION ${OFBUILD_LIB_INSTALL_PATH})
   

INSTALL(FILES ${OPENFLUID_BUDDIES_HPP} DESTINATION ${OPENFLUID_INCLUDE_INSTALL_PATH}/buddies)

   