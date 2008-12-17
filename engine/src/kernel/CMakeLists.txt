CMAKE_MINIMUM_REQUIRED (VERSION 2.6)


INCLUDE_DIRECTORIES(${CMAKE_SOURCE_DIR}/include)

ADD_DEFINITIONS(-DOFELIB_MAJORVER=${ENGINE_VERSION_MAJOR})
ADD_DEFINITIONS(-DOFELIB_MINORVER=${ENGINE_VERSION_MINOR})
ADD_DEFINITIONS(-DOFELIB_REVISION=${ENGINE_VERSION_PATCH})

CONFIGURE_FILE(config.h.in ${CMAKE_CURRENT_SOURCE_DIR}/config.h)


ADD_EXECUTABLE(openfluid-engine
               xml/tinystr.cpp xml/tinyxml.cpp xml/tinyxmlerror.cpp xml/tinyxmlparser.cpp
               AppTools.cpp Engine.cpp 
               IOMan.cpp PluginManager.cpp 
               RuntimeEnv.cpp main.cpp
              )

TARGET_LINK_LIBRARIES(openfluid-engine ofelib ${wxWidgets_LIBRARIES})

INSTALL(TARGETS openfluid-engine
        RUNTIME DESTINATION ${BIN_INSTALL_PATH} COMPONENT kernel
)


