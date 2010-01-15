CMAKE_MINIMUM_REQUIRED (VERSION 2.6)


ADD_DEFINITIONS(-DOFELIB_MAJORVER=${ENGINE_VERSION_MAJOR})
ADD_DEFINITIONS(-DOFELIB_MINORVER=${ENGINE_VERSION_MINOR})
ADD_DEFINITIONS(-DOFELIB_REVISION=${ENGINE_VERSION_PATCH})
ADD_DEFINITIONS(${LIBXML2_DEFINITIONS})

CONFIGURE_FILE(${CMAKE_CURRENT_SOURCE_DIR}/config.h.in ${CMAKE_BINARY_DIR}/config.h)

INCLUDE_DIRECTORIES(${CMAKE_BINARY_DIR} ${CMAKE_CURRENT_SOURCE_DIR} 
                    ${CMAKE_CURRENT_SOURCE_DIR}/model ${CMAKE_CURRENT_SOURCE_DIR}/run ${CMAKE_CURRENT_SOURCE_DIR}/output
                    ${CMAKE_SOURCE_DIR}/include 
                    ${Boost_INCLUDE_DIRS}
                    ${LIBXML2_INCLUDE_DIR})

ADD_EXECUTABLE(openfluid-engine
               xml/tinystr.cpp xml/tinyxml.cpp xml/tinyxmlerror.cpp xml/tinyxmlparser.cpp
               generators/FixedGenerator.cpp generators/RandomGenerator.cpp generators/InterpGenerator.cpp
               model/ModelItemDescriptor.cpp model/GeneratorDescriptor.cpp model/FunctionDescriptor.cpp
               model/ModelDescriptor.cpp model/ModelInstance.cpp model/ModelFactory.cpp
               run/RunDescriptor.cpp
               output/OutputSetDescriptor.cpp output/OutputFilesDescriptor.cpp output/OutputDescriptor.cpp
               io/DomainReader.cpp io/InputDataReader.cpp io/EventsReader.cpp io/ModelReader.cpp io/RunReader.cpp io/OutputReader.cpp 
               io/MessagesWriter.cpp io/SimReportWriter.cpp io/OutputsWriter.cpp 
               buddies/OFBuddy.cpp buddies/NewFuncBuddy.cpp buddies/Func2DocBuddy.cpp
               buddies/NewDataBuddy.cpp buddies/ConvertBuddy.cpp
               AppTools.cpp RuntimeEnv.cpp
               DynamicLib.cpp PluginManager.cpp 
               IOMan.cpp Engine.cpp
               OpenFLUID.cpp             
               main.cpp
              )
SET_TARGET_PROPERTIES(openfluid-engine PROPERTIES RUNTIME_OUTPUT_DIRECTORY "${BIN_OUTPUT_PATH}")

TARGET_LINK_LIBRARIES(openfluid-engine ofelib ${Boost_SYSTEM_LIBRARY} ${Boost_FILESYSTEM_LIBRARY} 
                                       ${Boost_DATE_TIME_LIBRARY} ${Boost_PROGRAM_OPTIONS_LIBRARY} 
                                       ${Boost_REGEX_LIBRARY}
                                       ${LIBXML2_LIBRARIES})
IF(UNIX)
  TARGET_LINK_LIBRARIES(openfluid-engine dl)
ENDIF(UNIX)

INSTALL(TARGETS openfluid-engine
        RUNTIME DESTINATION ${BIN_INSTALL_PATH})

INSTALL(FILES "${CMAKE_SOURCE_DIR}/resources/func2doc/func2doc_tpl.tex" 
        DESTINATION ${FUNC2DOC_TPL_INSTALL_PATH})
       
      

