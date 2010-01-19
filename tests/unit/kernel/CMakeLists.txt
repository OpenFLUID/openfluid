
INCLUDE_DIRECTORIES(. ${CMAKE_BINARY_DIR} ${CMAKE_SOURCE_DIR}/include ${CMAKE_SOURCE_DIR}/src/kernel ${CMAKE_SOURCE_DIR}/src/kernel/model ${CMAKE_SOURCE_DIR}/src/kernel/io)
INCLUDE_DIRECTORIES(${Boost_INCLUDE_DIRS} ${LIBXML2_INCLUDE_DIR})



ADD_UNITTEST(unit-AppTools unittest_apptools AppTools_TEST.cpp ${CMAKE_SOURCE_DIR}/src/kernel/AppTools.cpp)

ADD_UNITTEST(unit-RuntimeEnv unittest_runtimeenv RuntimeEnv_TEST.cpp ${CMAKE_SOURCE_DIR}/src/kernel/RuntimeEnv.cpp ${CMAKE_SOURCE_DIR}/src/kernel/AppTools.cpp)

ADD_UNITTEST(unit-DynamicLib unittest_dynamiclib DynamicLib_TEST.cpp ${CMAKE_SOURCE_DIR}/src/kernel/DynamicLib.cpp)
IF(UNIX)
  TARGET_LINK_LIBRARIES(unittest_dynamiclib dl)
ENDIF(UNIX)

ADD_UNITTEST(unit-ModelDescriptor unittest_modeldesc ModelDesc_TEST.cpp ${CMAKE_SOURCE_DIR}/src/kernel/model/ModelItemDescriptor.cpp ${CMAKE_SOURCE_DIR}/src/kernel/model/GeneratorDescriptor.cpp ${CMAKE_SOURCE_DIR}/src/kernel/model/FunctionDescriptor.cpp ${CMAKE_SOURCE_DIR}/src/kernel/model/ModelDescriptor.cpp)

ADD_UNITTEST(unit-RunDescriptor unittest_rundesc RunDesc_TEST.cpp ${CMAKE_SOURCE_DIR}/src/kernel/run/RunDescriptor.cpp)

ADD_UNITTEST(unit-OutputDescriptor unittest_outputdesc OutputDesc_TEST.cpp ${CMAKE_SOURCE_DIR}/src/kernel/output/OutputSetDescriptor.cpp ${CMAKE_SOURCE_DIR}/src/kernel/output/OutputFilesDescriptor.cpp ${CMAKE_SOURCE_DIR}/src/kernel/output/OutputDescriptor.cpp)

ADD_UNITTEST(unit-ModelInstance unittest_modelinstance ModelInstance_TEST.cpp )

ADD_UNITTEST(unit-ModelReader unittest_modelreader ModelReader_TEST.cpp ${CMAKE_SOURCE_DIR}/src/kernel/io/ModelReader.cpp ${CMAKE_SOURCE_DIR}/src/kernel/model/ModelItemDescriptor.cpp ${CMAKE_SOURCE_DIR}/src/kernel/model/GeneratorDescriptor.cpp ${CMAKE_SOURCE_DIR}/src/kernel/model/FunctionDescriptor.cpp ${CMAKE_SOURCE_DIR}/src/kernel/model/ModelDescriptor.cpp)
TARGET_LINK_LIBRARIES(unittest_modelreader ${LIBXML2_LIBRARIES})

ADD_UNITTEST(unit-RunReader unittest_runreader RunReader_TEST.cpp ${CMAKE_SOURCE_DIR}/src/kernel/io/RunReader.cpp ${CMAKE_SOURCE_DIR}/src/kernel/run/RunDescriptor.cpp)
TARGET_LINK_LIBRARIES(unittest_runreader ${LIBXML2_LIBRARIES})

ADD_UNITTEST(unit-OutputReader unittest_outputreader OutputReader_TEST.cpp ${CMAKE_SOURCE_DIR}/src/kernel/io/OutputReader.cpp ${CMAKE_SOURCE_DIR}/src/kernel/output/OutputSetDescriptor.cpp ${CMAKE_SOURCE_DIR}/src/kernel/output/OutputFilesDescriptor.cpp ${CMAKE_SOURCE_DIR}/src/kernel/output/OutputDescriptor.cpp ${CMAKE_SOURCE_DIR}/src/kernel/AppTools.cpp)
TARGET_LINK_LIBRARIES(unittest_outputreader ${LIBXML2_LIBRARIES})

ADD_UNITTEST(unit-DomainReader unittest_domainreader DomainReader_TEST.cpp)
TARGET_LINK_LIBRARIES(unittest_domainreader ${LIBXML2_LIBRARIES})

ADD_UNITTEST(unit-InputDataReader unittest_inputdatareader InputDataReader_TEST.cpp)
TARGET_LINK_LIBRARIES(unittest_inputdatareader ${LIBXML2_LIBRARIES})

ADD_UNITTEST(unit-EventsReader unittest_eventsreader EventsReader_TEST.cpp)
TARGET_LINK_LIBRARIES(unittest_eventsreader ${LIBXML2_LIBRARIES})
