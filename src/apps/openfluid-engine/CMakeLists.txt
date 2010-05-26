CMAKE_MINIMUM_REQUIRED (VERSION 2.6)


ADD_DEFINITIONS(-DOFELIB_MAJORVER=${ENGINE_VERSION_MAJOR})
ADD_DEFINITIONS(-DOFELIB_MINORVER=${ENGINE_VERSION_MINOR})
ADD_DEFINITIONS(-DOFELIB_REVISION=${ENGINE_VERSION_PATCH})
ADD_DEFINITIONS(${LibXML2_CFLAGS})

INCLUDE_DIRECTORIES(${Boost_INCLUDE_DIRS}
                    ${LibXML2_INCLUDE_DIRS})

LINK_DIRECTORIES(${LibXML2_LIBRARY_DIRS} ${Boost_LIBRARY_DIRS})




ADD_EXECUTABLE(openfluid-engine
               OpenFLUID.cpp OpenFLUID.hpp             
               main.cpp
              )
SET_TARGET_PROPERTIES(openfluid-engine PROPERTIES RUNTIME_OUTPUT_DIRECTORY "${BIN_OUTPUT_PATH}")

TARGET_LINK_LIBRARIES(openfluid-engine 
                      ${Boost_SYSTEM_LIBRARY} ${Boost_FILESYSTEM_LIBRARY} 
                      ${Boost_DATE_TIME_LIBRARY} ${Boost_PROGRAM_OPTIONS_LIBRARY} 
                      ${Boost_REGEX_LIBRARY}
                      ${LibXML2_LIBRARIES}
                      openfluid-base
                      openfluid-buddies
                      openfluid-core
                      openfluid-machine
                      openfluid-io
                      openfluid-tools)



INSTALL(TARGETS openfluid-engine
        RUNTIME DESTINATION ${BIN_INSTALL_PATH})

INSTALL(FILES "${CMAKE_SOURCE_DIR}/resources/func2doc/func2doc_tpl.tex" 
        DESTINATION ${FUNC2DOC_TPL_INSTALL_PATH})
       
      

