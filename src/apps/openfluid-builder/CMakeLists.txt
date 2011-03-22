## OpenFLUID/src/apps/openfluid-builder/CMakeLists.txt


FILE(MAKE_DIRECTORY "${BUILDEREXT_OUTPUT_PATH}")

PKG_CHECK_MODULES(GTKMM REQUIRED gtkmm-2.4)


INCLUDE_DIRECTORIES(${CMAKE_CURRENT_SOURCE_DIR}/contrib)

ADD_SUBDIRECTORY(contrib)

ADD_SUBDIRECTORY(builder-ext)

ADD_SUBDIRECTORY(builder-main)

ADD_SUBDIRECTORY(extensions)

ADD_SUBDIRECTORY(tests)


CONFIGURE_FILE(builderconfig.hpp.in "${CMAKE_CURRENT_BINARY_DIR}/builderconfig.hpp" @ONLY)
INCLUDE_DIRECTORIES("${CMAKE_CURRENT_BINARY_DIR}")


INCLUDE_DIRECTORIES(${LibXML2_INCLUDE_DIRS}
                    ${GTKMM_INCLUDE_DIRS})
                    
LINK_DIRECTORIES(${LibXML2_LIBRARY_DIRS}
                 ${GTKMM_LIBRARY_DIRS})


INCLUDE_DIRECTORIES(${Boost_INCLUDE_DIRS} builder-main builder-main/common)
LINK_DIRECTORIES(${Boost_LIBRARY_DIRS})


ADD_EXECUTABLE(openfluid-builder main.cpp)

SET_TARGET_PROPERTIES(openfluid-builder PROPERTIES RUNTIME_OUTPUT_DIRECTORY "${BIN_OUTPUT_PATH}")

TARGET_LINK_LIBRARIES(openfluid-builder 
                      ${Boost_SYSTEM_LIBRARY} ${Boost_FILESYSTEM_LIBRARY} 
                      ${Boost_DATE_TIME_LIBRARY} ${Boost_PROGRAM_OPTIONS_LIBRARY} 
                      ${Boost_REGEX_LIBRARY}
                      ${LibXML2_LIBRARIES}
                      ${GTKMM_LIBRARIES}
                      openfluid-base
                      openfluid-buddies
                      openfluid-core
                      openfluid-machine
                      openfluid-io
                      openfluid-tools
                      openfluid-builder-ext
                      openfluid-builder-gdl
                      openfluid-builder-main)


# comment to skip install - think to comment local params in CMake.in.local.config before install
INSTALL(TARGETS openfluid-builder
        RUNTIME DESTINATION ${BIN_INSTALL_PATH})



