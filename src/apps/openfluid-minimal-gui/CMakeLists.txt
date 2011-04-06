CMAKE_MINIMUM_REQUIRED (VERSION 2.6)

PKG_CHECK_MODULES(GTKMM REQUIRED gtkmm-2.4)
PKG_CHECK_MODULES(GLIBMM REQUIRED glibmm-2.4)


INCLUDE_DIRECTORIES(${GTKMM_INCLUDE_DIRS})
LINK_DIRECTORIES(${GTKMM_LIBRARY_DIRS})

INCLUDE_DIRECTORIES(${GLIBMM_INCLUDE_DIRS})
LINK_DIRECTORIES(${GLIBMM_LIBRARY_DIRS})

#INCLUDE_DIRECTORIES(${GTHREAD_INCLUDE_DIRS})
#LINK_DIRECTORIES(${GTHREAD_LIBRARY_DIRS})

ADD_DEFINITIONS(${LibXML2_CFLAGS})

INCLUDE_DIRECTORIES(${LibXML2_INCLUDE_DIRS})
LINK_DIRECTORIES(${LibXML2_LIBRARY_DIRS})


ADD_EXECUTABLE(openfluid-minimal-gui                   
               main.cpp)

SET_TARGET_PROPERTIES(openfluid-minimal-gui PROPERTIES RUNTIME_OUTPUT_DIRECTORY "${BIN_OUTPUT_PATH}")

TARGET_LINK_LIBRARIES(openfluid-minimal-gui
                      ${GTKMM_LIBRARIES} ${GLIBMM_LIBRARIES} ${GTHREAD_LIBRARIES}
                      openfluid-base
                      openfluid-buddies
                      openfluid-core
                      openfluid-machine
                      openfluid-io
                      openfluid-tools
                      openfluid-guicommon)



#INSTALL(TARGETS openfluid-minimal
#        RUNTIME DESTINATION ${BIN_INSTALL_PATH})

       
      

