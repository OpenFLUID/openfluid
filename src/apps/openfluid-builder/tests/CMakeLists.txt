
INCLUDE_DIRECTORIES(${LibXML2_INCLUDE_DIRS}
                    ${GTKMM_INCLUDE_DIRS})
                    
LINK_DIRECTORIES(${LibXML2_LIBRARY_DIRS}
                 ${GTKMM_LIBRARY_DIRS})


INCLUDE_DIRECTORIES(${Boost_INCLUDE_DIRS} ../main ../main/model ../main/domain ../main/simulation ../main/results)
LINK_DIRECTORIES(${Boost_LIBRARY_DIRS})

CONFIGURE_FILE(../main/builderconfig.hpp.in "${CMAKE_CURRENT_BINARY_DIR}/builderconfig.hpp" @ONLY)
INCLUDE_DIRECTORIES("${CMAKE_CURRENT_BINARY_DIR}")                     


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
                       openfluid-builderext
                       openfluid-builder-gdl)
                       
  SET_TARGET_PROPERTIES(${EXE_NAME} PROPERTIES RUNTIME_OUTPUT_DIRECTORY "${TEST_OUTPUT_PATH}")
  ADD_TEST(${TEST_NAME} "${TEST_OUTPUT_PATH}/${EXE_NAME}")
ENDMACRO(ADD_BUILDER_UNITTEST)


FILE(GLOB BUILDER_ENGINE_INCLUDES
                     ../main/EngineProjectFactory.hpp
                     ../main/EngineProject.cpp
                     ../main/BuilderMachineListener.cpp
                     ../main/BuilderAppDialogFactory.cpp
                     ../main/BuilderRunDialog.cpp)
                     
FILE(GLOB BUILDER_LISTSTORE_INCLUDES
                     ../main/BuilderClassListStore.cpp
                     ../main/BuilderListStore.cpp)
                     
MACRO(BUILDER_ADD_COMPONENT_INCLUDES COMPONENT_FULL_NAME)
    FILE(GLOB COMPONENT_INCLUDES "${COMPONENT_FULL_NAME}*.cpp")
ENDMACRO(BUILDER_ADD_COMPONENT_INCLUDES COMPONENT_FULL_NAME)
                      

ADD_BUILDER_UNITTEST(builder-unittest-GeneratorSignature
                     builder_unittest_GeneratorSignature_TEST
                     GeneratorSignature_TEST.cpp
                     ../main/model/GeneratorSignature.hpp )
ADD_BUILDER_UNITTEST(builder-unittest-FunctionSignatureRegistry
                     builder_unittest_FunctionSignatureRegistry_TEST
                     FunctionSignatureRegistry_TEST.cpp
                     ../main/model/FunctionSignatureRegistry.hpp )
ADD_BUILDER_UNITTEST(builder-unittest-EngineProject
                     builder_unittest_EngineProject_TEST
                     EngineProject_TEST.cpp
                     ${BUILDER_ENGINE_INCLUDES})
ADD_BUILDER_UNITTEST(builder-unittest-BuilderClassListStore
                     builder_unittest_BuilderClassListStore_TEST
                     BuilderClassListStore_TEST.cpp
                     ../main/BuilderClassListStore.cpp
                     ../main/BuilderListStore.cpp)
ADD_BUILDER_UNITTEST(builder-unittest-ModelItemInstanceFactory
                     builder_unittest_ModelItemInstanceFactory_TEST
                     ModelItemInstanceFactory_TEST.cpp
                     ../main/model/ModelItemInstanceFactory.hpp )

### MODEL ###

BUILDER_ADD_COMPONENT_INCLUDES(../main/model/ModelAvailFct)
ADD_BUILDER_UNITTEST(builder-unittest-ModelAvailFctPresenter
                     builder_unittest_ModelAvailFctPresenter_TEST
                     ModelAvailFctPresenter_TEST.cpp
                     ${COMPONENT_INCLUDES}
                     ../main/BuilderTreeStore.cpp
                     ../main/BuilderGraphicsHelper.cpp)
                     
ADD_BUILDER_UNITTEST(builder-unittest-ModelFctDetailAdapterModel
                     builder_unittest_ModelFctDetailAdapterModel_TEST
                     ModelFctDetailAdapterModel_TEST.cpp
                     ../main/model/ModelFctDetailAdapterModel.cpp
                     ../main/BuilderTreeStore.cpp)
BUILDER_ADD_COMPONENT_INCLUDES(../main/model/ModelFctDetail)
ADD_BUILDER_UNITTEST(builder-unittest-ModelFctDetailPresenter
                     builder_unittest_ModelFctDetailPresenter_TEST
                     ModelFctDetailPresenter_TEST.cpp
                     ${COMPONENT_INCLUDES}
                     ../main/model/ModelFctDetailInfoPage.hpp
                     ../main/model/ModelFctDetailInfoTableRow.hpp
                     ../main/BuilderTableRowWidget.cpp
                     ../main/BuilderTreeStore.cpp)
                       
ADD_BUILDER_UNITTEST(builder-unittest-ModelStructureModel
                     builder_unittest_ModelStructureModel_TEST
                     ModelStructureModel_TEST.cpp
                     ../main/model/ModelStructureModel.cpp
                     ${BUILDER_ENGINE_INCLUDES})
BUILDER_ADD_COMPONENT_INCLUDES(../main/model/ModelStructure)
ADD_BUILDER_UNITTEST(builder-unittest-ModelStructurePresenter
                     builder_unittest_ModelStructurePresenter_TEST
                     ModelStructurePresenter_TEST.cpp
                     ${COMPONENT_INCLUDES}
                     ${BUILDER_ENGINE_INCLUDES})
                     
BUILDER_ADD_COMPONENT_INCLUDES(../main/model/Model)
ADD_BUILDER_UNITTEST(builder-unittest-ModelCoordinator
                     builder_unittest_ModelCoordinator_TEST
                     ModelCoordinator_TEST.cpp
                     ../main/BuilderListToolBoxFactory.cpp
                     ../main/BuilderListToolBox.cpp
                     ../main/model/TestModelInstanceWrapper.hpp
                     ../main/model/FunctionSignatureRegistry.hpp
                     ../main/model/BuilderModelModule.cpp
                     ../main/BuilderTreeStore.cpp
                     ../main/BuilderGraphicsHelper.cpp
                     ../main/BuilderTableRowWidget.cpp                     
                     ${BUILDER_ENGINE_INCLUDES}
                     ${COMPONENT_INCLUDES})

### DOMAIN ###
                   
ADD_BUILDER_UNITTEST(builder-unittest-DomainStructureModel
                     builder_unittest_DomainStructureModel_TEST
                     DomainStructureModel_TEST.cpp
                     ../main/domain/DomainStructureModel.cpp
                     ${BUILDER_ENGINE_INCLUDES})
ADD_BUILDER_UNITTEST(builder-unittest-DomainStructureAdapterModel
                     builder_unittest_DomainStructureAdapterModel_TEST
                     DomainStructureAdapterModel_TEST.cpp
                     ../main/domain/DomainStructureAdapterModel.cpp
                     ${BUILDER_ENGINE_INCLUDES}
                     ${BUILDER_LISTSTORE_INCLUDES})
BUILDER_ADD_COMPONENT_INCLUDES(../main/domain/DomainStructure)
ADD_BUILDER_UNITTEST(builder-unittest-DomainStructurePresenter
                     builder_unittest_DomainStructurePresenter_TEST
                     DomainStructurePresenter_TEST.cpp
                     ${COMPONENT_INCLUDES}
                     ${BUILDER_ENGINE_INCLUDES}
                     ${BUILDER_LISTSTORE_INCLUDES}
                     ../main/BuilderByClassTreeView.cpp)             

ADD_BUILDER_UNITTEST(builder-unittest-DomainUnitEditionModel
                     builder_unittest_DomainUnitEditionModel_TEST
                     DomainUnitEditionModel_TEST.cpp
                     ../main/domain/DomainUnitEditionModel.cpp
                     ${BUILDER_ENGINE_INCLUDES})
BUILDER_ADD_COMPONENT_INCLUDES(../main/domain/DomainUnitEdition)
ADD_BUILDER_UNITTEST(builder-unittest-DomainUnitEditionPresenter
                     builder_unittest_DomainUnitEditionPresenter_TEST
                     DomainUnitEditionPresenter_TEST.cpp
                     ${COMPONENT_INCLUDES}
                     ${BUILDER_ENGINE_INCLUDES}
                     ../main/BuilderListToolBoxFactory.cpp
                     ../main/BuilderListToolBox.cpp)
                     
BUILDER_ADD_COMPONENT_INCLUDES(../main/domain/DomainUnitCreation)
ADD_BUILDER_UNITTEST(builder-unittest-DomainUnitCreationPresenter
                     builder_unittest_DomainUnitCreationPresenter_TEST
                     DomainUnitCreationPresenter_TEST.cpp
                     ${COMPONENT_INCLUDES}
                     ${BUILDER_ENGINE_INCLUDES})

ADD_BUILDER_UNITTEST(builder-unittest-DomainIDataModel
                     builder_unittest_DomainIDataModel_TEST
                     DomainIDataModel_TEST.cpp
                     ../main/domain/DomainIDataModel.cpp
                     ${BUILDER_ENGINE_INCLUDES}
                     ${BUILDER_LISTSTORE_INCLUDES})
ADD_BUILDER_UNITTEST(builder-unittest-DomainIDataAdapterModel
                     builder_unittest_DomainIDataAdapterModel_TEST
                     DomainIDataAdapterModel_TEST.cpp
                     ../main/domain/DomainIDataAdapterModel.cpp
                     ${BUILDER_ENGINE_INCLUDES}
                     ${BUILDER_LISTSTORE_INCLUDES})
BUILDER_ADD_COMPONENT_INCLUDES(../main/domain/DomainIData)
ADD_BUILDER_UNITTEST(builder-unittest-DomainIDataPresenter
                     builder_unittest_DomainIDataPresenter_TEST
                     DomainIDataPresenter_TEST.cpp
                     ${COMPONENT_INCLUDES}
                     ${BUILDER_ENGINE_INCLUDES}
                     ${BUILDER_LISTSTORE_INCLUDES}
                     ../main/BuilderByClassTreeView.cpp)

ADD_BUILDER_UNITTEST(builder-unittest-SimulRunModel
                     builder_unittest_SimulRunModel_TEST
                     SimulRunModel_TEST.cpp
                     ../main/simulation/SimulRunModel.cpp
                     ${BUILDER_ENGINE_INCLUDES})
BUILDER_ADD_COMPONENT_INCLUDES(../main/simulation/SimulRun)
ADD_BUILDER_UNITTEST(builder-unittest-SimulRunPresenter
                     builder_unittest_SimulRunPresenter_TEST
                     SimulRunPresenter_TEST.cpp
                     ${COMPONENT_INCLUDES}
                     ${BUILDER_ENGINE_INCLUDES})

ADD_BUILDER_UNITTEST(builder-unittest-SimulOutFilesModel
                     builder_unittest_SimulOutFilesModel_TEST
                     SimulOutFilesModel_TEST.cpp
                     ../main/simulation/SimulOutFilesModel.cpp
                     ${BUILDER_ENGINE_INCLUDES})
BUILDER_ADD_COMPONENT_INCLUDES(../main/simulation/SimulOutFiles)
ADD_BUILDER_UNITTEST(builder-unittest-SimulOutFilesPresenter
                     builder_unittest_SimulOutFilesPresenter_TEST
                     SimulOutFilesPresenter_TEST.cpp
                     ${COMPONENT_INCLUDES}
                     ${BUILDER_ENGINE_INCLUDES}
                     ../main/BuilderListStore.cpp)

BUILDER_ADD_COMPONENT_INCLUDES(../main/simulation/SimulOutFileDesc)
ADD_BUILDER_UNITTEST(builder-unittest-SimulOutFileDescPresenter
                     builder_unittest_SimulOutFileDescPresenter_TEST
                     SimulOutFileDescPresenter_TEST.cpp
                     ${COMPONENT_INCLUDES}
                     ${BUILDER_ENGINE_INCLUDES}
                     ../main/simulation/SimulOutFilesModel.cpp)

ADD_BUILDER_UNITTEST(builder-unittest-SimulOutSetsModel
                     builder_unittest_SimulOutSetsModel_TEST
                     SimulOutSetsModel_TEST.cpp
                     ../main/simulation/SimulOutSetsModel.cpp
                     ${BUILDER_ENGINE_INCLUDES})
BUILDER_ADD_COMPONENT_INCLUDES(../main/simulation/SimulOutSets)
ADD_BUILDER_UNITTEST(builder-unittest-SimulOutSetsPresenter
                     builder_unittest_SimulOutSetsPresenter_TEST
                     SimulOutSetsPresenter_TEST.cpp
                     ${COMPONENT_INCLUDES}
                     ${BUILDER_ENGINE_INCLUDES}
                     ../main/BuilderListStore.cpp)

BUILDER_ADD_COMPONENT_INCLUDES(../main/simulation/SimulOutSetDesc)
ADD_BUILDER_UNITTEST(builder-unittest-SimulOutSetDescPresenter
                     builder_unittest_SimulOutSetDescPresenter_TEST
                     SimulOutSetDescPresenter_TEST.cpp
                     ${COMPONENT_INCLUDES}
                     ${BUILDER_ENGINE_INCLUDES}
                     ../main/simulation/SimulOutFilesModel.cpp
                     ../main/simulation/SimulOutSetsModel.cpp)

BUILDER_ADD_COMPONENT_INCLUDES(../main/results/ResChooser)
ADD_BUILDER_UNITTEST(builder-unittest-ResChooserPresenter
                     builder_unittest_ResChooserPresenter_TEST
                     ResChooserPresenter_TEST.cpp
                     ${COMPONENT_INCLUDES}
                     ${BUILDER_ENGINE_INCLUDES})
                     