

PKG_CHECK_MODULES(GLIBMM REQUIRED glibmm-2.4)
PKG_CHECK_MODULES(GTKMM REQUIRED gtkmm-2.4)


ADD_DEFINITIONS(${LibXML2_CFLAGS})

INCLUDE_DIRECTORIES(${Boost_INCLUDE_DIRS} ${GLIBMM_INCLUDE_DIRS} ${GTKMM_INCLUDE_DIRS})
LINK_DIRECTORIES(${GLIBMM_LIBRARY_DIRS} ${GTKMM_LIBRARY_DIRS})


FILE(GLOB OPENFLUID_GUICOMMON_CPP *.cpp)
FILE(GLOB OPENFLUID_GUICOMMON_HPP *.hpp)


ADD_LIBRARY(openfluid-guicommon SHARED ${OPENFLUID_GUICOMMON_CPP} ${OPENFLUID_GUICOMMON_HPP})

SET_TARGET_PROPERTIES(openfluid-guicommon PROPERTIES VERSION "${VERSION_MAJOR}.${VERSION_MINOR}" 
                                        SOVERSION "${VERSION_MAJOR}.${VERSION_MINOR}.${VERSION_PATCH}"
                                        LIBRARY_OUTPUT_DIRECTORY "${LIB_OUTPUT_PATH}"
                                        RUNTIME_OUTPUT_DIRECTORY "${LIB_OUTPUT_PATH}"
                                        ARCHIVE_OUTPUT_DIRECTORY "${LIB_OUTPUT_PATH}")

TARGET_LINK_LIBRARIES(openfluid-guicommon
                      openfluid-base
                      openfluid-core
                      openfluid-tools
                      openfluid-machine
                      openfluid-market
                      ${GLIBMM_LIBRARIES}
                      ${GTKMM_LIBRARIES}  
                      ${Boost_SYSTEM_LIBRARY} ${Boost_FILESYSTEM_LIBRARY})



INSTALL(TARGETS openfluid-guicommon
        RUNTIME DESTINATION ${BIN_INSTALL_PATH}
        LIBRARY DESTINATION ${LIB_INSTALL_PATH}
        ARCHIVE DESTINATION ${LIB_INSTALL_PATH})
   

INSTALL(FILES ${OPENFLUID_GUICOMMON_HPP} DESTINATION ${INCLUDE_INSTALL_PATH}/guicommon)

   