
INCLUDE_DIRECTORIES(. ${CMAKE_BINARY_DIR} ${CMAKE_SOURCE_DIR}/include ${CMAKE_SOURCE_DIR}/src/kernel ${CMAKE_SOURCE_DIR}/src/kernel/model ${CMAKE_SOURCE_DIR}/src/kernel/io)
INCLUDE_DIRECTORIES(${Boost_INCLUDE_DIRS} ${LIBXML2_INCLUDE_DIR})



ADD_UNITTEST(unit-AppTools unittest_apptools 
             AppTools_TEST.cpp 
             ${CMAKE_SOURCE_DIR}/src/kernel/AppTools.cpp)

ADD_UNITTEST(unit-RuntimeEnv unittest_runtimeenv 
             RuntimeEnv_TEST.cpp 
             ${CMAKE_SOURCE_DIR}/src/kernel/RuntimeEnv.cpp 
             ${CMAKE_SOURCE_DIR}/src/kernel/AppTools.cpp)

ADD_UNITTEST(unit-DynamicLib unittest_dynamiclib 
             DynamicLib_TEST.cpp 
             ${CMAKE_SOURCE_DIR}/src/kernel/DynamicLib.cpp)
IF(UNIX)
  TARGET_LINK_LIBRARIES(unittest_dynamiclib dl)
ENDIF(UNIX)

ADD_UNITTEST(unit-ModelDescriptor unittest_modeldesc 
             ModelDesc_TEST.cpp 
             ${CMAKE_SOURCE_DIR}/src/kernel/model/ModelItemDescriptor.cpp
             ${CMAKE_SOURCE_DIR}/src/kernel/model/GeneratorDescriptor.cpp
             ${CMAKE_SOURCE_DIR}/src/kernel/model/FunctionDescriptor.cpp
             ${CMAKE_SOURCE_DIR}/src/kernel/model/ModelDescriptor.cpp)

ADD_UNITTEST(unit-RunDescriptor unittest_rundesc 
             RunDesc_TEST.cpp
             ${CMAKE_SOURCE_DIR}/src/kernel/run/RunDescriptor.cpp)

ADD_UNITTEST(unit-DomainDescriptor unittest_domaindesc 
             DomainDesc_TEST.cpp 
             ${CMAKE_SOURCE_DIR}/src/kernel/domain/UnitDescriptor.cpp
             ${CMAKE_SOURCE_DIR}/src/kernel/domain/IDataDescriptor.cpp
             ${CMAKE_SOURCE_DIR}/src/kernel/domain/EventDescriptor.cpp
             ${CMAKE_SOURCE_DIR}/src/kernel/domain/DomainDescriptor.cpp
             )

ADD_UNITTEST(unit-OutputDescriptor unittest_outputdesc 
             OutputDesc_TEST.cpp
             ${CMAKE_SOURCE_DIR}/src/kernel/output/OutputSetDescriptor.cpp
             ${CMAKE_SOURCE_DIR}/src/kernel/output/OutputFilesDescriptor.cpp
             ${CMAKE_SOURCE_DIR}/src/kernel/output/OutputDescriptor.cpp)

ADD_UNITTEST(unit-ModelInstance unittest_modelinstance 
             ModelInstance_TEST.cpp )

ADD_UNITTEST(unit-FluidXReader unittest_fluidxreader
             FluidXReader_TEST.cpp
             ${CMAKE_SOURCE_DIR}/src/kernel/model/ModelItemDescriptor.cpp
             ${CMAKE_SOURCE_DIR}/src/kernel/model/GeneratorDescriptor.cpp
             ${CMAKE_SOURCE_DIR}/src/kernel/model/FunctionDescriptor.cpp
             ${CMAKE_SOURCE_DIR}/src/kernel/model/ModelDescriptor.cpp
             ${CMAKE_SOURCE_DIR}/src/kernel/run/RunDescriptor.cpp
             ${CMAKE_SOURCE_DIR}/src/kernel/domain/UnitDescriptor.cpp
             ${CMAKE_SOURCE_DIR}/src/kernel/domain/IDataDescriptor.cpp
             ${CMAKE_SOURCE_DIR}/src/kernel/domain/EventDescriptor.cpp
             ${CMAKE_SOURCE_DIR}/src/kernel/domain/DomainDescriptor.cpp
             ${CMAKE_SOURCE_DIR}/src/kernel/output/OutputSetDescriptor.cpp
             ${CMAKE_SOURCE_DIR}/src/kernel/output/OutputFilesDescriptor.cpp
             ${CMAKE_SOURCE_DIR}/src/kernel/output/OutputDescriptor.cpp
             ${CMAKE_SOURCE_DIR}/src/kernel/AppTools.cpp
             ${CMAKE_SOURCE_DIR}/src/kernel/io/FluidXReader.cpp)
TARGET_LINK_LIBRARIES(unittest_fluidxreader ${LIBXML2_LIBRARIES})

