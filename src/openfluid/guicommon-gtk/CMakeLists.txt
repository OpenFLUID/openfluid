

PKG_CHECK_MODULES(GLIBMM REQUIRED glibmm-2.4)
PKG_CHECK_MODULES(GTKMM REQUIRED gtkmm-2.4)


ADD_DEFINITIONS(${LibXML2_CFLAGS})

INCLUDE_DIRECTORIES(${Boost_INCLUDE_DIRS} ${GLIBMM_INCLUDE_DIRS} ${GTKMM_INCLUDE_DIRS} ${LibXML2_INCLUDE_DIRS})
LINK_DIRECTORIES(${GLIBMM_LIBRARY_DIRS} ${GTKMM_LIBRARY_DIRS} ${LibXML2_LIBRARY_DIRS})


FILE(GLOB OPENFLUID_GUICOMMON_CPP *.cpp)
FILE(GLOB OPENFLUID_GUICOMMON_HPP *.hpp)


ADD_LIBRARY(openfluid-guicommon-gtk SHARED ${OPENFLUID_GUICOMMON_CPP} ${OPENFLUID_GUICOMMON_HPP})

SET_TARGET_PROPERTIES(openfluid-guicommon-gtk PROPERTIES VERSION "${VERSION_MAJOR}.${VERSION_MINOR}" 
                                        SOVERSION "${VERSION_MAJOR}.${VERSION_MINOR}.${VERSION_PATCH}"
                                        LIBRARY_OUTPUT_DIRECTORY "${LIB_OUTPUT_PATH}"
                                        RUNTIME_OUTPUT_DIRECTORY "${LIB_OUTPUT_PATH}"
                                        ARCHIVE_OUTPUT_DIRECTORY "${LIB_OUTPUT_PATH}")

TARGET_LINK_LIBRARIES(openfluid-guicommon-gtk
                      openfluid-base
                      openfluid-core
                      openfluid-tools
                      openfluid-machine
                      openfluid-market
                      openfluid-fluidx
                      ${LibXML2_LIBRARIES}  
                      ${GLIBMM_LIBRARIES}
                      ${GTKMM_LIBRARIES}  
                      ${Boost_SYSTEM_LIBRARY} ${Boost_FILESYSTEM_LIBRARY})



INSTALL(TARGETS openfluid-guicommon-gtk
        RUNTIME DESTINATION ${BIN_INSTALL_PATH}
        LIBRARY DESTINATION ${LIB_INSTALL_PATH}
        ARCHIVE DESTINATION ${LIB_INSTALL_PATH})
   

INSTALL(FILES ${OPENFLUID_GUICOMMON_HPP} DESTINATION ${INCLUDE_OPENFLUID_INSTALL_PATH}/guicommon)


IF(NOT (CMAKE_BUILD_TYPE MATCHES "Release"))
  ADD_SUBDIRECTORY(tests)
ENDIF(NOT (CMAKE_BUILD_TYPE MATCHES "Release"))  


   