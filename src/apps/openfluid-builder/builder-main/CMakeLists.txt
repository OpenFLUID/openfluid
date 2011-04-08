## OpenFLUID/src/apps/openfluid-builder/main/CMakeLists.txt


CMAKE_MINIMUM_REQUIRED (VERSION 2.6)

    
ADD_SUBDIRECTORY(po)


ADD_DEFINITIONS(${LibXML2_CFLAGS})


INCLUDE_DIRECTORIES(${LibXML2_INCLUDE_DIRS}
                    ${GTKMM_INCLUDE_DIRS})
                    
LINK_DIRECTORIES(${LibXML2_LIBRARY_DIRS}
                 ${GTKMM_LIBRARY_DIRS})


INCLUDE_DIRECTORIES(${Boost_INCLUDE_DIRS} . common model domain simulation results)
LINK_DIRECTORIES(${Boost_LIBRARY_DIRS})


FILE(GLOB OPENFLUID_BUILDER_MAINLIB_CPP common/*.cpp model/*.cpp domain/*.cpp simulation/*.cpp results/*.cpp)
FILE(GLOB OPENFLUID_BUILDER_MAINLIB_HPP common/*.hpp model/*.hpp domain/*.hpp simulation/*.hpp results/*.hpp)

ADD_LIBRARY(openfluid-builder-main SHARED ${OPENFLUID_BUILDER_MAINLIB_CPP} ${OPENFLUID_BUILDER_MAINLIB_HPP}
                                          ${OPENFLUID_BUILDER_MARKETCLIENT_CPP} ${OPENFLUID_BUILDER_MARKETCLIENT_HPP}
                                          ${OPENFLUID_BUILDER_MINIMALGUI_CPP} ${OPENFLUID_BUILDER_MINIMALGUI_HPP})

TARGET_LINK_LIBRARIES(openfluid-builder-main 
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
                      openfluid-market
                      openfluid-guicommon)
#                      openfluid-builder-ext)


SET_TARGET_PROPERTIES(openfluid-builder-main PROPERTIES VERSION "${VERSION_MAJOR}.${VERSION_MINOR}" 
                                        SOVERSION "${VERSION_MAJOR}.${VERSION_MINOR}.${VERSION_PATCH}"
                                        LIBRARY_OUTPUT_DIRECTORY "${LIB_OUTPUT_PATH}"
                                        RUNTIME_OUTPUT_DIRECTORY "${LIB_OUTPUT_PATH}"
                                        ARCHIVE_OUTPUT_DIRECTORY "${LIB_OUTPUT_PATH}")


#FILE(GLOB Builder_RESOURCE_FILES ${CMAKE_CURRENT_SOURCE_DIR}/resources/[^.]*)
#INSTALL(FILES ${Builder_RESOURCE_FILES}
#        DESTINATION ${BUILDER_RESOURCE_PATH})

INSTALL(TARGETS openfluid-builder-main
        RUNTIME DESTINATION ${BIN_INSTALL_PATH}
        LIBRARY DESTINATION ${LIB_INSTALL_PATH}
        ARCHIVE DESTINATION ${LIB_INSTALL_PATH})