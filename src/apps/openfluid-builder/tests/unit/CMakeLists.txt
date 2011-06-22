
FIND_PACKAGE(Boost REQUIRED COMPONENTS unit_test_framework)  


ADD_DEFINITIONS(-DOPENFLUID_VERSION=${FULL_VERSION})


INCLUDE_DIRECTORIES(${LibXML2_INCLUDE_DIRS}
                    ${GTKMM_INCLUDE_DIRS})
                    
LINK_DIRECTORIES(${LibXML2_LIBRARY_DIRS}
                 ${GTKMM_LIBRARY_DIRS})


INCLUDE_DIRECTORIES(${Boost_INCLUDE_DIRS})
LINK_DIRECTORIES(${Boost_LIBRARY_DIRS})


SET(RELDIR_BUILDERMAIN ../../builder-main)

INCLUDE_DIRECTORIES(${RELDIR_BUILDERMAIN} 
                    ${RELDIR_BUILDERMAIN}/common ${RELDIR_BUILDERMAIN}/model 
                    ${RELDIR_BUILDERMAIN}/domain ${RELDIR_BUILDERMAIN}/simulation 
                    ${RELDIR_BUILDERMAIN}/results 
                    "${PROJECT_BINARY_DIR}/src/tests")


MACRO(ADD_BUILDER_UNITTEST TEST_NAME EXE_NAME)
  ADD_EXECUTABLE(${EXE_NAME} ${ARGN})
  TARGET_LINK_LIBRARIES(${EXE_NAME} 
                       ${Boost_UNIT_TEST_FRAMEWORK_LIBRARY}
                       ${Boost_SYSTEM_LIBRARY} ${Boost_FILESYSTEM_LIBRARY} 
                       ${Boost_DATE_TIME_LIBRARY} ${Boost_PROGRAM_OPTIONS_LIBRARY} 
                       ${Boost_REGEX_LIBRARY}
                       ${LibXML2_LIBRARIES}
                       ${GTKMM_LIBRARIES}                 
                       openfluid-base openfluid-buddies
                       openfluid-core openfluid-machine
                       openfluid-io openfluid-tools openfluid-market
                       openfluid-builder-main)
                       
  SET_TARGET_PROPERTIES(${EXE_NAME} PROPERTIES RUNTIME_OUTPUT_DIRECTORY "${TEST_OUTPUT_PATH}")
  ADD_TEST(${TEST_NAME} "${TEST_OUTPUT_PATH}/${EXE_NAME}")
ENDMACRO(ADD_BUILDER_UNITTEST)


                      

ADD_BUILDER_UNITTEST(builder-unittest-BuilderExtensionsManager
                     builder_unittest_BuilderExtensionsManager_TEST
                     BuilderExtensionsManager_TEST.cpp)

ADD_BUILDER_UNITTEST(builder-unittest-GeneratorSignature
                     builder_unittest_GeneratorSignature_TEST
                     GeneratorSignature_TEST.cpp)
                     
ADD_BUILDER_UNITTEST(builder-unittest-FunctionSignatureRegistry
                     builder_unittest_FunctionSignatureRegistry_TEST
                     FunctionSignatureRegistry_TEST.cpp)
                     
ADD_BUILDER_UNITTEST(builder-unittest-EngineProject
                     builder_unittest_EngineProject_TEST
                     EngineProject_TEST.cpp)
                     
ADD_BUILDER_UNITTEST(builder-unittest-BuilderClassListStore
                     builder_unittest_BuilderClassListStore_TEST
                     BuilderClassListStore_TEST.cpp)

ADD_BUILDER_UNITTEST(builder-unittest-BuilderTreeStore
                     builder_unittest_BuilderTreeStore_TEST
                     BuilderTreeStore_TEST.cpp) 
                                          
ADD_BUILDER_UNITTEST(builder-unittest-ModelItemInstanceFactory
                     builder_unittest_ModelItemInstanceFactory_TEST
                     ModelItemInstanceFactory_TEST.cpp)
                     
ADD_BUILDER_UNITTEST(builder-unittest-ProjectExplorerAdapterModel
                     builder_unittest_ProjectExplorerAdapterModel_TEST
                     ProjectExplorerAdapterModel_TEST.cpp)

ADD_BUILDER_UNITTEST(builder-unittest-ProjectExplorerPresenter
                     builder_unittest_ProjectExplorerPresenter_TEST
                     ProjectExplorerPresenter_TEST.cpp)
                     
#ADD_BUILDER_UNITTEST(builder-unittest-ProjectCoordinator
#                     builder_unittest_ProjectCoordinator_TEST
#                     ProjectCoordinator_TEST.cpp)

### MODEL ###


ADD_BUILDER_UNITTEST(builder-unittest-ModelAvailFctPresenter
                     builder_unittest_ModelAvailFctPresenter_TEST
                     ModelAvailFctPresenter_TEST.cpp)
                     
ADD_BUILDER_UNITTEST(builder-unittest-ModelFctDetailAdapterModel
                     builder_unittest_ModelFctDetailAdapterModel_TEST
                     ModelFctDetailAdapterModel_TEST.cpp)


ADD_BUILDER_UNITTEST(builder-unittest-ModelFctDetailPresenter
                     builder_unittest_ModelFctDetailPresenter_TEST
                     ModelFctDetailPresenter_TEST.cpp)
                       
ADD_BUILDER_UNITTEST(builder-unittest-ModelStructureModel
                     builder_unittest_ModelStructureModel_TEST
                     ModelStructureModel_TEST.cpp)

ADD_BUILDER_UNITTEST(builder-unittest-ModelStructurePresenter
                     builder_unittest_ModelStructurePresenter_TEST
                     ModelStructurePresenter_TEST.cpp)

ADD_BUILDER_UNITTEST(builder-unittest-ModelGlobalParamsModel
                     builder_unittest_ModelGlobalParamsModel_TEST
                     ModelGlobalParamsModel_TEST.cpp)
                     
ADD_BUILDER_UNITTEST(builder-unittest-ModelGlobalParamsPresenter
                     builder_unittest_ModelGlobalParamsPresenter_TEST
                     ModelGlobalParamsPresenter_TEST.cpp)

### DOMAIN ###
                   
ADD_BUILDER_UNITTEST(builder-unittest-DomainStructureModel
                     builder_unittest_DomainStructureModel_TEST
                     DomainStructureModel_TEST.cpp)
                     
ADD_BUILDER_UNITTEST(builder-unittest-DomainStructureAdapterModel
                     builder_unittest_DomainStructureAdapterModel_TEST
                     DomainStructureAdapterModel_TEST.cpp)
                     
ADD_BUILDER_UNITTEST(builder-unittest-DomainStructurePresenter
                     builder_unittest_DomainStructurePresenter_TEST
                     DomainStructurePresenter_TEST.cpp)             

ADD_BUILDER_UNITTEST(builder-unittest-DomainUnitEditionModel
                     builder_unittest_DomainUnitEditionModel_TEST
                     DomainUnitEditionModel_TEST.cpp)
                     
ADD_BUILDER_UNITTEST(builder-unittest-DomainUnitEditionPresenter
                     builder_unittest_DomainUnitEditionPresenter_TEST
                     DomainUnitEditionPresenter_TEST.cpp)
                     
ADD_BUILDER_UNITTEST(builder-unittest-DomainUnitCreationPresenter
                     builder_unittest_DomainUnitCreationPresenter_TEST
                     DomainUnitCreationPresenter_TEST.cpp)

ADD_BUILDER_UNITTEST(builder-unittest-DomainIDataModel
                     builder_unittest_DomainIDataModel_TEST
                     DomainIDataModel_TEST.cpp)
                     
ADD_BUILDER_UNITTEST(builder-unittest-DomainIDataAdapterModel
                     builder_unittest_DomainIDataAdapterModel_TEST
                     DomainIDataAdapterModel_TEST.cpp)
                     
ADD_BUILDER_UNITTEST(builder-unittest-DomainIDataPresenter
                     builder_unittest_DomainIDataPresenter_TEST
                     DomainIDataPresenter_TEST.cpp)

### SIMULATION ###

ADD_BUILDER_UNITTEST(builder-unittest-SimulRunModel
                     builder_unittest_SimulRunModel_TEST
                     SimulRunModel_TEST.cpp)
                     
ADD_BUILDER_UNITTEST(builder-unittest-SimulRunPresenter
                     builder_unittest_SimulRunPresenter_TEST
                     SimulRunPresenter_TEST.cpp)

ADD_BUILDER_UNITTEST(builder-unittest-SimulOutFilesModel
                     builder_unittest_SimulOutFilesModel_TEST
                     SimulOutFilesModel_TEST.cpp)
                     
ADD_BUILDER_UNITTEST(builder-unittest-SimulOutFilesPresenter
                     builder_unittest_SimulOutFilesPresenter_TEST
                     SimulOutFilesPresenter_TEST.cpp)

ADD_BUILDER_UNITTEST(builder-unittest-SimulOutFileDescPresenter
                     builder_unittest_SimulOutFileDescPresenter_TEST
                     SimulOutFileDescPresenter_TEST.cpp)

ADD_BUILDER_UNITTEST(builder-unittest-SimulOutSetsModel
                     builder_unittest_SimulOutSetsModel_TEST
                     SimulOutSetsModel_TEST.cpp)
                     
ADD_BUILDER_UNITTEST(builder-unittest-SimulOutSetsPresenter
                     builder_unittest_SimulOutSetsPresenter_TEST
                     SimulOutSetsPresenter_TEST.cpp)

ADD_BUILDER_UNITTEST(builder-unittest-SimulOutSetDescPresenter
                     builder_unittest_SimulOutSetDescPresenter_TEST
                     SimulOutSetDescPresenter_TEST.cpp)

### RESULTS ###

ADD_BUILDER_UNITTEST(builder-unittest-ResUnitChooserModel
                     builder_unittest_ResUnitChooserModel_TEST
                     ResUnitChooserModel_TEST.cpp)
                                          