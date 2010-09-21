## OpenFLUID/src/apps/openfluid-builder/main/CMakeLists.txt


CMAKE_MINIMUM_REQUIRED (VERSION 2.6)


# ! let this before CONFIGURE_FILE(builderconfig.hpp.in)
SET(BUILDER_NLS_PACKAGE openfluid-builder)
SET(BUILDER_NLS_LOCALEDIR ${CMAKE_INSTALL_PREFIX}/share/locale)
    
ADD_SUBDIRECTORY(po)

CONFIGURE_FILE(builderconfig.hpp.in "${CMAKE_CURRENT_BINARY_DIR}/builderconfig.hpp" @ONLY)
INCLUDE_DIRECTORIES("${CMAKE_CURRENT_BINARY_DIR}")


ADD_DEFINITIONS(${LibXML2_CFLAGS})


INCLUDE_DIRECTORIES(${LibXML2_INCLUDE_DIRS}
                    ${GTKMM_INCLUDE_DIRS})
                    
LINK_DIRECTORIES(${LibXML2_LIBRARY_DIRS}
                 ${GTKMM_LIBRARY_DIRS})


INCLUDE_DIRECTORIES(${Boost_INCLUDE_DIRS})
LINK_DIRECTORIES(${Boost_LIBRARY_DIRS})

FILE(GLOB OPENFLUID_BUILDER_CPP *.cpp)
FILE(GLOB OPENFLUID_BUILDER_HPP *.hpp)


ADD_EXECUTABLE(openfluid-builder ${OPENFLUID_BUILDER_CPP} ${OPENFLUID_BUILDER_HPP})

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
                      openfluid-builderext
                      openfluid-builder-gdl)


# comment to skip install - think to comment local params in CMake.in.local.config before install
INSTALL(TARGETS openfluid-builder
        RUNTIME DESTINATION ${BIN_INSTALL_PATH})

FILE(GLOB Builder_RESOURCE_FILES ${CMAKE_CURRENT_SOURCE_DIR}/resources/[^.]*)
INSTALL(FILES ${Builder_RESOURCE_FILES}
        DESTINATION ${BUILDER_RESOURCE_PATH})

