/*
  This file is part of OpenFLUID software
  Copyright (c) 2007-2010 INRA-Montpellier SupAgro


 == GNU General Public License Usage ==

  OpenFLUID is free software: you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation, either version 3 of the License, or
  (at your option) any later version.

  OpenFLUID is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with OpenFLUID.  If not, see <http://www.gnu.org/licenses/>.

  In addition, as a special exception, INRA gives You the additional right
  to dynamically link the code of OpenFLUID with code not covered
  under the GNU General Public License ("Non-GPL Code") and to distribute
  linked combinations including the two, subject to the limitations in this
  paragraph. Non-GPL Code permitted under this exception must only link to
  the code of OpenFLUID dynamically through the OpenFLUID libraries
  interfaces, and only for building OpenFLUID plugins. The files of
  Non-GPL Code may be link to the OpenFLUID libraries without causing the
  resulting work to be covered by the GNU General Public License. You must
  obey the GNU General Public License in all respects for all of the
  OpenFLUID code and other code used in conjunction with OpenFLUID
  except the Non-GPL Code covered by this exception. If you modify
  this OpenFLUID, you may extend this exception to your version of the file,
  but you are not obligated to do so. If you do not wish to provide this
  exception without modification, you must delete this exception statement
  from your version and license this OpenFLUID solely under the GPL without
  exception.


 == Other Usage ==

  Other Usage means a use of OpenFLUID that is inconsistent with the GPL
  license, and requires a written agreement between You and INRA.
  Licensees for Other Usage of OpenFLUID may use this file in accordance
  with the terms contained in the written agreement between You and INRA.
*/


/**
  @file
  @brief implements ...

  @author Jean-Christophe FABRE <fabrejc@supagro.inra.fr>
*/


#include <openfluid/machine/Engine.hpp>

#include <iostream>
#include <iomanip>
#include <set>
#include <math.h>
#include <boost/filesystem.hpp>

#include <openfluid/config.hpp>
#include <openfluid/base/RuntimeEnv.hpp>
#include <openfluid/machine/MachineListener.hpp>
#include <openfluid/machine/ModelInstance.hpp>
#include <openfluid/machine/ModelItemInstance.hpp>
#include <openfluid/machine/MonitoringInstance.hpp>
#include <openfluid/machine/SimulationBlob.hpp>


namespace openfluid { namespace machine {


// =====================================================================
// =====================================================================


Engine::Engine(SimulationBlob& SimBlob,
               ModelInstance& MInstance, MonitoringInstance& OLInstance,
               openfluid::machine::MachineListener* MachineListener)
       : m_SimulationBlob(SimBlob), m_ModelInstance(MInstance), m_MonitoringInstance(OLInstance), mp_SimLogger(NULL)
{

  mp_RunEnv = openfluid::base::RuntimeEnvironment::getInstance();

  mp_MachineListener = MachineListener;
  if (mp_MachineListener == NULL) mp_MachineListener = new openfluid::machine::MachineListener();

  mp_SimStatus = &(m_SimulationBlob.getSimulationStatus());

  prepareOutputDir();

  mp_SimLogger = new openfluid::base::SimulationLogger(mp_RunEnv->getOutputFullPath(openfluid::config::MESSAGES_LOG_FILE));

  mp_SimLogger->addInfo("*** Execution information ********************************************");
  mp_SimLogger->addInfo("Date: " + boost::posix_time::to_simple_string(boost::posix_time::microsec_clock::local_time()));
  mp_SimLogger->addInfo("Computer: " + mp_RunEnv->getHostName());
  mp_SimLogger->addInfo("User: " + mp_RunEnv->getUserID());
  mp_SimLogger->addInfo("Input directory: " + mp_RunEnv->getInputDir());
  mp_SimLogger->addInfo("Output directory: " + mp_RunEnv->getOutputDir());
  mp_SimLogger->addInfo("*** End of execution information *************************************");
}

// =====================================================================
// =====================================================================

Engine::~Engine()
{
  if (mp_SimLogger != NULL) delete mp_SimLogger;
}


// =====================================================================
// =====================================================================


void Engine::checkExistingVariable(const openfluid::core::VariableName_t& VarName,
                                   const openfluid::core::Value::Type& VarType,
                                   const openfluid::core::UnitClass_t& ClassName,
                                   const std::string& SimulatorID)
{
  openfluid::core::UnitsList_t::const_iterator UnitIter;
  openfluid::core::UnitsList_t* UnitList;

  UnitList = NULL;
  if (m_SimulationBlob.getCoreRepository().isUnitsClassExist(ClassName)) UnitList = m_SimulationBlob.getCoreRepository().getUnits(ClassName)->getList();
  else throw openfluid::base::OFException("OpenFLUID framework","Engine::checkExistingVariable","Unit class " + ClassName + " does not exist for " + VarName + " variable required by " + SimulatorID);

  bool Status = true;

  UnitIter = UnitList->begin();
  while (UnitIter != UnitList->end())
  {
    if(VarType == openfluid::core::Value::NONE)
      Status = (*UnitIter).getVariables()->isVariableExist(VarName);
    else
      Status = (*UnitIter).getVariables()->isTypedVariableExist(VarName,VarType);

    if (!Status)
      throw openfluid::base::OFException("OpenFLUID framework","Engine::checkExistingVariable",VarName + " variable on " + ClassName + " required by " + SimulatorID + " does not exist");

    ++UnitIter;
  }


}

// =====================================================================
// =====================================================================


void Engine::createVariable(const openfluid::core::VariableName_t& VarName,
                            const openfluid::core::Value::Type& VarType,
                            const openfluid::core::UnitClass_t& ClassName,
                            bool UpdateMode,
                            const std::string& SimulatorID)
{
  openfluid::core::UnitsList_t::iterator UnitIter;
  openfluid::core::UnitsList_t* UnitList;

  UnitList = NULL;
  if (m_SimulationBlob.getCoreRepository().isUnitsClassExist(ClassName)) UnitList = m_SimulationBlob.getCoreRepository().getUnits(ClassName)->getList();
  else throw openfluid::base::OFException("OpenFLUID framework","Engine::createVariable","Unit class " + ClassName + " does not exist for " + VarName + " variable produced by " + SimulatorID);

  bool Status = true;

  // if not update mode, variables must not exist before creation
  if (!UpdateMode)
  {
    UnitIter = UnitList->begin();
    while (UnitIter != UnitList->end())
    {
       Status = !((*UnitIter).getVariables()->isVariableExist(VarName));

      if (!Status)
        throw openfluid::base::OFException("OpenFLUID framework","Engine::createVariable",VarName + " variable on " + ClassName + " produced by " + SimulatorID + " cannot be created because it is already created");

      ++UnitIter;
    }
  }

  for(UnitIter = UnitList->begin(); UnitIter != UnitList->end(); ++UnitIter )
  {
    (*UnitIter).getVariables()->createVariable(VarName,VarType);
  }

}


// =====================================================================
// =====================================================================


void Engine::checkExistingInputdata(openfluid::core::InputDataName_t DataName,
                                    openfluid::core::UnitClass_t ClassName,
                                    const std::string& SimulatorID)
{
  openfluid::core::UnitsList_t::const_iterator UnitIter;
  openfluid::core::UnitsList_t* UnitList;

  UnitList = NULL;
  if (m_SimulationBlob.getCoreRepository().isUnitsClassExist(ClassName)) UnitList = m_SimulationBlob.getCoreRepository().getUnits(ClassName)->getList();
  else throw openfluid::base::OFException("OpenFLUID framework","Engine::checkExistingInputData","Unit " + ClassName + " class does not exist for " + DataName + " input data required by " + SimulatorID);

  bool Status = true;

  UnitIter = UnitList->begin();
  while (Status && (UnitIter != UnitList->end()))
  {
    Status = (*UnitIter).getInputData()->isDataExist(DataName);
    if (!Status)
      throw openfluid::base::OFException("OpenFLUID framework","Engine::checkExistingInputData",DataName + " input data on " + ClassName + " required by " + SimulatorID + " is not available");

    ++UnitIter;
  }
}


// =====================================================================
// =====================================================================


void Engine::createInputdata(openfluid::core::InputDataName_t DataName,
                             openfluid::core::UnitClass_t ClassName,
                             const std::string& SimulatorID)
{
  openfluid::core::UnitsList_t::iterator UnitIter;
  openfluid::core::UnitsList_t* UnitList;

  UnitList = NULL;
  if (m_SimulationBlob.getCoreRepository().isUnitsClassExist(ClassName)) UnitList = m_SimulationBlob.getCoreRepository().getUnits(ClassName)->getList();
  else throw openfluid::base::OFException("OpenFLUID framework","Engine::createInputdata","Unit class " + ClassName + " does not exist for " + DataName + " inputdata produced by " + SimulatorID);


  for(UnitIter = UnitList->begin(); UnitIter != UnitList->end(); ++UnitIter )
  {
    (*UnitIter).getInputData()->setValue(DataName,openfluid::core::NullValue());
  }
}


// =====================================================================
// =====================================================================


void Engine::checkSimulationVarsProduction(int ExpectedVarsCount)
{

  openfluid::core::UnitsListByClassMap_t::const_iterator UnitsClassesIter;
  openfluid::core::UnitsList_t::const_iterator UnitsIter;
  const openfluid::core::UnitsListByClassMap_t* AllUnits;
  const openfluid::core::UnitsList_t* UnitsList;
  std::vector<std::string> VarsNames;


  AllUnits = m_SimulationBlob.getCoreRepository().getUnitsByClass();

  for (UnitsClassesIter = AllUnits->begin(); UnitsClassesIter != AllUnits->end();++UnitsClassesIter)
  {
    UnitsList = UnitsClassesIter->second.getList();

    for (UnitsIter = UnitsList->begin();UnitsIter != UnitsList->end();++UnitsIter)
    {
      if (!((*UnitsIter).getVariables()->isAllVariablesCount(ExpectedVarsCount)))
        throw openfluid::base::OFException("OpenFLUID framework","Engine::checkSimulationVarsProduction","Variable production error");

    }

  }
}

// =====================================================================
// =====================================================================

void Engine::checkModelConsistency()
{
  std::list<ModelItemInstance*>::const_iterator SimIter;
  openfluid::ware::SignatureHandledData HData;
  ModelItemInstance* CurrentSimulator;
  unsigned int i;


  /* Variables processing order is important
     A) first pass
        1) produced vars
        2) updated vars
     B) second pass
        3) required vars
  */

  SimIter = m_ModelInstance.getItems().begin();

  while (SimIter != m_ModelInstance.getItems().end())
  {
    CurrentSimulator = (*SimIter);
    HData = CurrentSimulator->Signature->HandledData;

    // checking variables to create (produced)
    for (i=0;i< HData.ProducedVars.size();i++)
      createVariable(HData.ProducedVars[i].DataName,
                     HData.ProducedVars[i].DataType,
                     HData.ProducedVars[i].UnitClass,
                     false,
                     CurrentSimulator->Signature->ID);


    // checking variables to update
    for (i=0;i<HData.UpdatedVars.size();i++)
      createVariable(HData.UpdatedVars[i].DataName,
                     HData.UpdatedVars[i].DataType,
                     HData.UpdatedVars[i].UnitClass,
                     true,
                     CurrentSimulator->Signature->ID);

    SimIter++;
  }


  SimIter = m_ModelInstance.getItems().begin();

  while (SimIter != m_ModelInstance.getItems().end())
  {
    CurrentSimulator = (*SimIter);
    HData = CurrentSimulator->Signature->HandledData;

    // checking required variables
    for (i=0;i< HData.RequiredVars.size();i++)
      checkExistingVariable(HData.RequiredVars[i].DataName,
                            HData.RequiredVars[i].DataType,
                            HData.RequiredVars[i].UnitClass,
                            CurrentSimulator->Signature->ID);

    SimIter++;
  }

}

// =====================================================================
// =====================================================================



void Engine::checkInputdataConsistency()
{
  std::list<ModelItemInstance*>::const_iterator SimIter;
  openfluid::ware::SignatureHandledData HData;
  ModelItemInstance* CurrentSimulator;
  unsigned int i;


  SimIter = m_ModelInstance.getItems().begin();

  while (SimIter != m_ModelInstance.getItems().end())
  {
    CurrentSimulator = (*SimIter);
    HData = CurrentSimulator->Signature->HandledData;

    // checking required input data
    for(i=0; i < HData.RequiredInputdata.size();i++)
      checkExistingInputdata(HData.RequiredInputdata[i].DataName,
          HData.RequiredInputdata[i].UnitClass,
          CurrentSimulator->Signature->ID);

    // checking produced input data
    for(i=0; i < HData.ProducedInputdata.size();i++)
      createInputdata(HData.ProducedInputdata[i].DataName,
          HData.ProducedInputdata[i].UnitClass,
          CurrentSimulator->Signature->ID);

    SimIter++;
  }

}

// =====================================================================
// =====================================================================


void Engine::checkExtraFilesConsistency()
{
  std::list<ModelItemInstance*>::const_iterator SimIter;
  openfluid::ware::SignatureHandledData HData;
  ModelItemInstance* CurrentSimulator;


  // on each simulator
  for (SimIter = m_ModelInstance.getItems().begin(); SimIter != m_ModelInstance.getItems().end(); ++SimIter)
  {
    CurrentSimulator = *SimIter;

    HData = CurrentSimulator->Signature->HandledData;

    for (unsigned int i=0;i<HData.RequiredExtraFiles.size();i++)
    {

      boost::filesystem::path ReqExtraFilePath(mp_RunEnv->getInputFullPath(HData.RequiredExtraFiles[i]));
      if (!boost::filesystem::exists(ReqExtraFilePath))
        throw openfluid::base::OFException("OpenFLUID framework","Engine::checkExtraFilesConsistency","File " + HData.RequiredExtraFiles[i] + " required by " + CurrentSimulator->Signature->ID + " not found");
    }
  }
}


// =====================================================================
// =====================================================================


void Engine::prepareOutputDir()
{

  boost::filesystem::path OutputDirPath(mp_RunEnv->getOutputDir());


  if (!boost::filesystem::exists(OutputDirPath))
  {
    boost::filesystem::create_directories(OutputDirPath);
    if (!boost::filesystem::exists(OutputDirPath))
      throw openfluid::base::OFException("OpenFLUID framework","IOManager::prepareOutputDir","Error creating output directory");

  }
  else
  {
    if (mp_RunEnv->isClearOutputDir())
    {
      openfluid::tools::EmptyDirectoryRecursively(mp_RunEnv->getOutputDir().c_str());
    }
  }
}


// =====================================================================
// =====================================================================


void Engine::initialize()
{
  openfluid::base::RuntimeEnvironment::getInstance()->resetSimulationID();
  openfluid::base::RuntimeEnvironment::getInstance()->resetIgnitionDateTime();
  m_ModelInstance.initialize(mp_SimLogger);
  m_MonitoringInstance.initialize(mp_SimLogger);
}


// =====================================================================
// =====================================================================


void Engine::initParams()
{

  mp_MachineListener->onInitParams();
  try
  {
    mp_SimStatus->setCurrentStage(openfluid::base::SimulationStatus::INITPARAMS);
    m_ModelInstance.call_initParams();
    m_MonitoringInstance.call_initParams();
  }
  catch (openfluid::base::OFException& E)
  {
    mp_MachineListener->onInitParamsDone(openfluid::machine::MachineListener::ERROR);
    throw;
  }
  if (mp_SimLogger->isWarningFlag()) mp_MachineListener->onInitParamsDone(openfluid::machine::MachineListener::WARNING);
  else mp_MachineListener->onInitParamsDone(openfluid::machine::MachineListener::OK);
  mp_SimLogger->resetWarningFlag();

}


// =====================================================================
// =====================================================================

void Engine::prepareData()
{
  try
  {
    mp_SimStatus->setCurrentStage(openfluid::base::SimulationStatus::PREPAREDATA);
    m_ModelInstance.call_prepareData();
  }
  catch (openfluid::base::OFException& E)
  {
    mp_MachineListener->onPrepareDataDone(openfluid::machine::MachineListener::ERROR);
    throw;
  }

  if (mp_SimLogger->isWarningFlag()) mp_MachineListener->onPrepareDataDone(openfluid::machine::MachineListener::WARNING);
  else  mp_MachineListener->onPrepareDataDone(openfluid::machine::MachineListener::OK);
  mp_SimLogger->resetWarningFlag();

}

// =====================================================================
// =====================================================================

void Engine::checkConsistency()
{

  // inits the simulation infos and status

  if (mp_RunEnv->isUserValuesBufferSize())
  {
    openfluid::core::ValuesBufferProperties::setBufferSize(mp_RunEnv->getValuesBufferSize());
  }
  else
  {
    openfluid::core::ValuesBufferProperties::setBufferSize((mp_SimStatus->getSimulationDuration()/mp_SimStatus->getDefaultDeltaT())+1);
  }


  // check simulators count

  try
  {
    if (m_ModelInstance.getItemsCount() == 0)
      throw openfluid::base::OFException("OpenFLUID framework","Engine::checkConsistency","No simulator in model");

    checkExtraFilesConsistency();

    checkModelConsistency();

    checkInputdataConsistency();
  }
  catch (openfluid::base::OFException& E)
  {
    mp_MachineListener->onCheckConsistencyDone(openfluid::machine::MachineListener::ERROR);
    throw;
  }


  try
  {
    mp_SimStatus->setCurrentStage(openfluid::base::SimulationStatus::CHECKCONSISTENCY);
    m_ModelInstance.call_checkConsistency();
    m_MonitoringInstance.call_onPrepared();
  }
  catch (openfluid::base::OFException& E)
  {
    mp_MachineListener->onCheckConsistencyDone(openfluid::machine::MachineListener::ERROR);
    throw;
  }

  if (mp_SimLogger->isWarningFlag()) mp_MachineListener->onCheckConsistencyDone(openfluid::machine::MachineListener::WARNING);
  else  mp_MachineListener->onCheckConsistencyDone(openfluid::machine::MachineListener::OK);
  mp_SimLogger->resetWarningFlag();

}


// =====================================================================
// =====================================================================


void Engine::run()
{

  std::string ProdMessage;

  // Check for simulation vars production before init
  checkSimulationVarsProduction(0);

  // ============= initializeRun() =============

  mp_MachineListener->onInitializeRun();


  try
  {
    mp_SimStatus->setCurrentStage(openfluid::base::SimulationStatus::INITIALIZERUN);
    m_ModelInstance.call_initializeRun();
    m_MonitoringInstance.call_onInitializedRun();
  }
  catch (openfluid::base::OFException& E)
  {
    mp_MachineListener->onInitializeRunDone(openfluid::machine::MachineListener::ERROR);
    throw;
  }

  if (mp_SimLogger->isWarningFlag()) mp_MachineListener->onInitializeRunDone(openfluid::machine::MachineListener::WARNING);
  else mp_MachineListener->onInitializeRunDone(openfluid::machine::MachineListener::OK);
  mp_SimLogger->resetWarningFlag();

  // check simulation vars production after init
  checkSimulationVarsProduction(1);


  // ============= runStep() =============



  mp_MachineListener->onBeforeRunSteps();
  mp_SimStatus->setCurrentStage(openfluid::base::SimulationStatus::RUNSTEP);


  while (m_ModelInstance.hasTimePointToProcess())
  {

    mp_SimLogger->resetWarningFlag();

    try
    {
      m_ModelInstance.processNextTimePoint();
      m_MonitoringInstance.call_onStepCompleted(mp_SimStatus->getCurrentTimeIndex());

      // TODO to remove? check simulation vars production at each time step
      //checkSimulationVarsProduction(mp_SimStatus->getCurrentStep()+1);
    }
    catch (openfluid::base::OFException& E)
    {
      mp_MachineListener->onRunStepDone(openfluid::machine::MachineListener::ERROR);
      throw;
    }
  }

  mp_MachineListener->onAfterRunSteps();

  mp_SimLogger->resetWarningFlag();


  // ============= finalizeRun() =============

  mp_MachineListener->onFinalizeRun();

  try
  {
    mp_SimStatus->setCurrentStage(openfluid::base::SimulationStatus::FINALIZERUN);
    m_ModelInstance.call_finalizeRun();
    m_MonitoringInstance.call_onFinalizedRun();
  }
  catch (openfluid::base::OFException& E)
  {
    mp_MachineListener->onFinalizeRunDone(openfluid::machine::MachineListener::ERROR);
    throw;
  }


  if (mp_SimLogger->isWarningFlag()) mp_MachineListener->onFinalizeRunDone(openfluid::machine::MachineListener::WARNING);
  else mp_MachineListener->onFinalizeRunDone(openfluid::machine::MachineListener::OK);
  mp_SimLogger->resetWarningFlag();

  // check simulation vars production after finalize
  //checkSimulationVarsProduction(mp_SimStatus->getCurrentStep()+1);

  mp_SimStatus->setCurrentStage(openfluid::base::SimulationStatus::POST);
}


// =====================================================================
// =====================================================================


void Engine::finalize()
{
  m_ModelInstance.finalize();
}


} } //namespaces



