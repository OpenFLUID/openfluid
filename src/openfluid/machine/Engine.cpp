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


#include <iostream>
#include <iomanip>
#include <set>

#include <math.h>
#include <boost/filesystem.hpp>

#include <openfluid/machine/Engine.hpp>
#include <openfluid/tools.hpp>
#include <openfluid/machine/Factory.hpp>
#include <openfluid/core/ValuesBuffer.hpp>
#include <openfluid/config.hpp>


namespace openfluid { namespace machine {


// =====================================================================
// =====================================================================


Engine::Engine(SimulationBlob& SimBlob, ModelInstance& MInstance,
               openfluid::machine::MachineListener* MachineListener,
               openfluid::io::IOListener* IOListener)
       : m_SimulationBlob(SimBlob), m_ModelInstance(MInstance)
{

  mp_RunEnv = openfluid::base::RuntimeEnvironment::getInstance();

  mp_OutputsWriter = NULL;
  mp_MessagesWriter = NULL;


  mp_MachineListener = MachineListener;
  if (mp_MachineListener == NULL) mp_MachineListener = new openfluid::machine::MachineListener();

  mp_IOListener = IOListener;
  if (mp_IOListener == NULL) mp_IOListener = new openfluid::io::IOListener();


  mp_SimStatus = NULL;


  prepareOutputDir();
  m_ModelInstance.initialize();
}

// =====================================================================
// =====================================================================

Engine::~Engine()
{
  closeOutputs();

  delete mp_OutputsWriter;
  delete mp_MessagesWriter;
}


// =====================================================================
// =====================================================================


void Engine::checkExistingVariable(openfluid::core::VariableName_t VarName,
                                   openfluid::core::UnitClass_t ClassName,
                                   std::string FunctionName)
{
  openfluid::core::UnitsList_t::const_iterator UnitIter;
  openfluid::core::UnitsList_t* UnitList;

  UnitList = NULL;
  if (m_SimulationBlob.getCoreRepository().isUnitsClassExist(ClassName)) UnitList = m_SimulationBlob.getCoreRepository().getUnits(ClassName)->getList();
  else throw openfluid::base::OFException("OpenFLUID framework","Engine::checkExistingVariable","Unit class " + ClassName + " does not exist for " + VarName + " variable required by " + FunctionName);

  bool Status = true;

  UnitIter = UnitList->begin();
  while (UnitIter != UnitList->end())
  {
    if (openfluid::tools::IsVectorNamedVariable(VarName)) Status = (*UnitIter).getVectorVariables()->isVariableExist(openfluid::tools::GetVectorNamedVariableName(VarName));
    else Status = (*UnitIter).getScalarVariables()->isVariableExist(VarName);

    if (!Status)
      throw openfluid::base::OFException("OpenFLUID framework","Engine::checkExistingVariable",VarName + " variable on " + ClassName + " required by " + FunctionName + " is not previously created");

    ++UnitIter;
  }


}

// =====================================================================
// =====================================================================


void Engine::createVariable(openfluid::core::VariableName_t VarName,
                            openfluid::core::UnitClass_t ClassName,
                            bool UpdateMode,
                            std::string FunctionName)
{
  openfluid::core::UnitsList_t::iterator UnitIter;
  openfluid::core::UnitsList_t* UnitList;

  UnitList = NULL;
  if (m_SimulationBlob.getCoreRepository().isUnitsClassExist(ClassName)) UnitList = m_SimulationBlob.getCoreRepository().getUnits(ClassName)->getList();
  else throw openfluid::base::OFException("OpenFLUID framework","Engine::createVariable","Unit class " + ClassName + " does not exist for " + VarName + " variable produced by " + FunctionName);

  bool Status = true;

  // if not update mode, variables must not exist before creation
  if (!UpdateMode)
  {
    UnitIter = UnitList->begin();
    while (UnitIter != UnitList->end())
    {
      if (openfluid::tools::IsVectorNamedVariable(VarName)) Status = !((*UnitIter).getVectorVariables()->isVariableExist(openfluid::tools::GetVectorNamedVariableName(VarName)));
      else Status = !((*UnitIter).getScalarVariables()->isVariableExist(VarName));

      if (!Status)
        throw openfluid::base::OFException("OpenFLUID framework","Engine::createVariable",VarName + " variable on " + ClassName + " produced by " + FunctionName + " cannot be created because it is previously created");

      ++UnitIter;
    }
  }

  for(UnitIter = UnitList->begin(); UnitIter != UnitList->end(); ++UnitIter )
  {
    if (openfluid::tools::IsVectorNamedVariable(VarName)) (*UnitIter).getVectorVariables()->createVariable(openfluid::tools::GetVectorNamedVariableName(VarName));
    else (*UnitIter).getScalarVariables()->createVariable(VarName);
  }

}

// =====================================================================
// =====================================================================



void Engine::checkExistingInputData(openfluid::core::InputDataName_t DataName,
                                    openfluid::core::UnitClass_t ClassName,
                                    std::string FunctionName)
{
  openfluid::core::UnitsList_t::const_iterator UnitIter;
  openfluid::core::UnitsList_t* UnitList;


  UnitList = NULL;
  if (m_SimulationBlob.getCoreRepository().isUnitsClassExist(ClassName)) UnitList = m_SimulationBlob.getCoreRepository().getUnits(ClassName)->getList();
  else throw openfluid::base::OFException("OpenFLUID framework","Engine::checkExistingInputData","Unit " + ClassName + " class does not exist for " + DataName + " input data required by " + FunctionName);

  bool Status = true;

  UnitIter = UnitList->begin();
  while (Status && (UnitIter != UnitList->end()))
  {
    Status = (*UnitIter).getInputData()->isDataExist(DataName);
    if (!Status)
      throw openfluid::base::OFException("OpenFLUID framework","Engine::checkExistingInputData",DataName + " input data on " + ClassName + " required by " + FunctionName + " is not available");

    ++UnitIter;
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

      //scalars
      if (!((*UnitsIter).getScalarVariables()->isAllVariablesCount(ExpectedVarsCount)))
        throw openfluid::base::OFException("OpenFLUID framework","Engine::checkSimulationVarsProduction","Scalar variable production error");

      //vectors
      if (!((*UnitsIter).getVectorVariables()->isAllVariablesCount(ExpectedVarsCount)))
        throw openfluid::base::OFException("OpenFLUID framework","Engine::checkSimulationVarsProduction","Vector variable production error");
    }

  }

}

// =====================================================================
// =====================================================================

void Engine::checkModelConsistency()
{
  std::list<ModelItemInstance*>::const_iterator FuncIter;
  openfluid::base::SignatureHandledData HData;
  ModelItemInstance* CurrentFunction;
  unsigned int i;


  /* Variables processing order is important
     A) first pass
        1) required vars
        2) produced vars
        3) updated vars
     B) second pass
        4) required vars at t-1+
  */

  FuncIter = m_ModelInstance.getItems().begin();

  while (FuncIter != m_ModelInstance.getItems().end())
  {
    CurrentFunction = (*FuncIter);
    HData = CurrentFunction->Signature->HandledData;

    // checking required variables
    for (i=0;i< HData.RequiredVars.size();i++)
      checkExistingVariable(HData.RequiredVars[i].DataName,
                            HData.RequiredVars[i].UnitClass,
                            CurrentFunction->Signature->ID);

    // checking variables to create (produced)

    for (i=0;i< HData.ProducedVars.size();i++)
      createVariable(HData.ProducedVars[i].DataName,
                     HData.ProducedVars[i].UnitClass,
                     false,
                     CurrentFunction->Signature->ID);


    // checking variables to update
    for (i=0;i<HData.UpdatedVars.size();i++)
      createVariable(HData.UpdatedVars[i].DataName,
                     HData.UpdatedVars[i].UnitClass,
                     true,
                     CurrentFunction->Signature->ID);

    FuncIter++;
  }



  FuncIter = m_ModelInstance.getItems().begin();

  while (FuncIter != m_ModelInstance.getItems().end())
  {
    CurrentFunction = (*FuncIter);
    HData = CurrentFunction->Signature->HandledData;

    // checking required variables at t-1+
    for (i=0;i< HData.RequiredPrevVars.size();i++)
      checkExistingVariable(HData.RequiredPrevVars[i].DataName,
                            HData.RequiredPrevVars[i].UnitClass,
                            CurrentFunction->Signature->ID);

    FuncIter++;
  }

}

// =====================================================================
// =====================================================================



void Engine::checkInputDataConsistency()
{
  std::list<ModelItemInstance*>::const_iterator FuncIter;
  openfluid::base::SignatureHandledData HData;
  ModelItemInstance* CurrentFunction;
  unsigned int i;


  FuncIter = m_ModelInstance.getItems().begin();

  while (FuncIter != m_ModelInstance.getItems().end())
  {
    CurrentFunction = (*FuncIter);
    HData = CurrentFunction->Signature->HandledData;


    // checking required input data
    for(i=0; i < HData.RequiredInput.size();i++)
      checkExistingInputData(HData.RequiredInput[i].DataName,
          HData.RequiredInput[i].UnitClass,
          CurrentFunction->Signature->ID);

    FuncIter++;
  }

}

// =====================================================================
// =====================================================================


void Engine::checkExtraFilesConsistency()
{
  std::list<ModelItemInstance*>::const_iterator FuncIter;
  openfluid::base::SignatureHandledData HData;
  ModelItemInstance* CurrentFunction;


  // on each function
  for (FuncIter = m_ModelInstance.getItems().begin(); FuncIter != m_ModelInstance.getItems().end(); ++FuncIter)
  {
    CurrentFunction = *FuncIter;

    HData = CurrentFunction->Signature->HandledData;

    for (unsigned int i=0;i<HData.RequiredExtraFiles.size();i++)
    {

      boost::filesystem::path ReqExtraFilePath(mp_RunEnv->getInputFullPath(HData.RequiredExtraFiles[i]));
      if (!boost::filesystem::exists(ReqExtraFilePath))
        throw openfluid::base::OFException("OpenFLUID framework","Engine::checkExtraFilesConsistency","File " + HData.RequiredExtraFiles[i] + " required by " + CurrentFunction->Signature->ID + " not found");
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


  // create empty message file

  std::ofstream OutMsgFile;

  boost::filesystem::path OutMsgFilePath = boost::filesystem::path(mp_RunEnv->getOutputFullPath(openfluid::config::OUTMSGSFILE));
  OutMsgFile.open(OutMsgFilePath.string().c_str(),std::ios::out);
  OutMsgFile.close();

}



// =====================================================================
// =====================================================================



void Engine::prepareOutputs()
{
  if (mp_OutputsWriter != NULL) mp_OutputsWriter->prepareDirectory();
  if (mp_MessagesWriter != NULL) mp_MessagesWriter->initializeFile();
}


// =====================================================================
// =====================================================================


void Engine::saveOutputs(const openfluid::core::DateTime& CurrentDT)
{
  if (mp_OutputsWriter != NULL) mp_OutputsWriter->saveToDirectory(CurrentDT);
  if (mp_MessagesWriter != NULL) mp_MessagesWriter->saveToFile(m_SimulationBlob.getExecutionMessages(),true);
}


// =====================================================================
// =====================================================================


void Engine::saveSimulationInfos()
{
  openfluid::io::SimulationReportWriter::saveToFile(mp_RunEnv->getOutputFullPath(openfluid::config::SIMINFOFILE),
                                                    (openfluid::base::SimulationInfo*)mp_SimStatus,
                                                    m_SimulationBlob.getCoreRepository());
}


// =====================================================================
// =====================================================================


void Engine::pretestConsistency(PretestInfos_t& PretestInfos)
{
  if (PretestInfos.ExtraFiles)
  {
    PretestInfos.ExtraFilesMsg.clear();

    try
    {
      checkExtraFilesConsistency();
    }
    catch (openfluid::base::OFException& E)
    {
      PretestInfos.ExtraFiles = false;
      PretestInfos.ExtraFilesMsg.assign(E.what());
    }
  }


  if (PretestInfos.Inputdata)
  {
    PretestInfos.InputdataMsg.clear();

    try
    {
      checkInputDataConsistency();
    }
    catch (openfluid::base::OFException& E)
    {
      PretestInfos.Inputdata = false;
      PretestInfos.InputdataMsg.assign(E.what());
    }
  }


  if (PretestInfos.Model)
  {
    PretestInfos.ModelMsg.clear();

    try
    {
      std::set<std::pair<openfluid::core::UnitClass_t,openfluid::core::VariableName_t> > VarsUnits;
      std::list<ModelItemInstance*>::const_iterator FuncIter;
      openfluid::base::SignatureHandledData HData;
      ModelItemInstance* CurrentFunction;
      unsigned int i;

      /*
         A) first pass
            1) required vars
            2) produced vars
            3) updated vars
         B) second pass
            4) required vars at t-1+
      */

      VarsUnits.clear();

      FuncIter = m_ModelInstance.getItems().begin();

      while (FuncIter != m_ModelInstance.getItems().end())
      {
        CurrentFunction = (*FuncIter);
        HData = CurrentFunction->Signature->HandledData;


        // checking required variables
        for (i=0;i< HData.RequiredVars.size();i++)
        {
          if (!(VarsUnits.find(std::make_pair(HData.RequiredVars[i].UnitClass,HData.RequiredVars[i].DataName))!=VarsUnits.end()))
            throw openfluid::base::OFException("OpenFLUID framework","Engine::pretestConsistency",
                HData.RequiredVars[i].DataName + " variable on " + HData.RequiredVars[i].UnitClass + " required by " + CurrentFunction->Signature->ID + " is not previously created");
        }


        // checking variables to create (produced)
        for (i=0;i< HData.ProducedVars.size();i++)
        {
          if ((VarsUnits.find(std::make_pair(HData.ProducedVars[i].UnitClass,HData.ProducedVars[i].DataName))==VarsUnits.end()))
            VarsUnits.insert(std::make_pair(HData.ProducedVars[i].UnitClass,HData.ProducedVars[i].DataName));
          else
            throw openfluid::base::OFException("OpenFLUID framework","Engine::pretestConsistency",
                HData.ProducedVars[i].DataName + " variable on " + HData.ProducedVars[i].UnitClass + " produced by " + CurrentFunction->Signature->ID + " cannot be created because it is previously created");
        }


        // checking variables to update
        for (i=0;i<HData.UpdatedVars.size();i++)
        {
          if (!(VarsUnits.find(std::make_pair(HData.UpdatedVars[i].UnitClass,HData.UpdatedVars[i].DataName))!=VarsUnits.end()))
            VarsUnits.insert(std::make_pair(HData.UpdatedVars[i].UnitClass,HData.UpdatedVars[i].DataName));
        }


        FuncIter++;
      }



      FuncIter = m_ModelInstance.getItems().begin();

      while (FuncIter != m_ModelInstance.getItems().end())
      {
        CurrentFunction = (*FuncIter);
        HData = CurrentFunction->Signature->HandledData;

        // checking required variables at t-1+
        for (i=0;i< HData.RequiredPrevVars.size();i++)
        {
          if (!(VarsUnits.find(std::make_pair(HData.RequiredPrevVars[i].UnitClass,HData.RequiredPrevVars[i].DataName))!=VarsUnits.end()))
            throw openfluid::base::OFException("OpenFLUID framework","Engine::pretestConsistency",
                HData.RequiredPrevVars[i].DataName + " variable on " + HData.RequiredPrevVars[i].UnitClass + " required by " + CurrentFunction->Signature->ID + " is not previously created");
        }


        FuncIter++;
      }


    }
    catch (openfluid::base::OFException& E)
    {
      PretestInfos.Model = false;
      PretestInfos.ModelMsg.assign(E.what());
    }
  }
}


// =====================================================================
// =====================================================================


void Engine::initParams()
{

  mp_MachineListener->onInitParams();
  try
  {
    m_ModelInstance.call_initParams();
  }
  catch (openfluid::base::OFException& E)
  {
    mp_MachineListener->onInitParamsDone(openfluid::machine::MachineListener::ERROR);
    throw;
  }
  if (m_SimulationBlob.getExecutionMessages().isWarningFlag()) mp_MachineListener->onInitParamsDone(openfluid::machine::MachineListener::WARNING);
  else mp_MachineListener->onInitParamsDone(openfluid::machine::MachineListener::OK);
  m_SimulationBlob.getExecutionMessages().resetWarningFlag();

}


// =====================================================================
// =====================================================================

void Engine::prepareData()
{
  try
  {
    m_ModelInstance.call_prepareData();
  }
  catch (openfluid::base::OFException& E)
  {
    mp_MachineListener->onPrepareDataDone(openfluid::machine::MachineListener::ERROR);
    throw;
  }

  mp_MachineListener->onPrepareDataDone(openfluid::machine::MachineListener::OK);

}

// =====================================================================
// =====================================================================

void Engine::checkConsistency()
{

  // inits the simulation infos and status

  mp_SimStatus = new openfluid::base::SimulationStatus(mp_RunEnv->getSimulationStartTime(),
                                                       mp_RunEnv->getSimulationEndTime(),
                                                       mp_RunEnv->getSimulationTimeStep());

  if (mp_RunEnv->isUserValuesBufferSize())
  {
    openfluid::core::ValuesBufferProperties::setBufferSize(mp_RunEnv->getValuesBufferSize());
  }
  else
  {
    openfluid::core::ValuesBufferProperties::setBufferSize(mp_SimStatus->getStepsCount());
  }


  // check simulation functions count

  try
  {
    if (m_ModelInstance.getItemsCount() == 0)
      throw openfluid::base::OFException("OpenFLUID framework","Engine::checkConsistency","No simulation function in model");

    checkExtraFilesConsistency();

    checkModelConsistency();

    checkInputDataConsistency();
  }
  catch (openfluid::base::OFException& E)
  {
    mp_MachineListener->onCheckConsistencyDone(openfluid::machine::MachineListener::ERROR);
    throw;
  }


  try
  {
    m_ModelInstance.call_checkConsistency();
  }
  catch (openfluid::base::OFException& E)
  {
    mp_MachineListener->onCheckConsistencyDone(openfluid::machine::MachineListener::ERROR);
    throw;
  }



  mp_OutputsWriter = new openfluid::io::OutputsWriter(mp_RunEnv->getOutputDir(),const_cast<openfluid::base::OutputDescriptor&>(m_SimulationBlob.getOutputDescriptor()),
                                                      m_SimulationBlob.getCoreRepository());
  mp_MessagesWriter = new openfluid::io::MessagesWriter(mp_RunEnv->getOutputFullPath(openfluid::config::OUTMSGSFILE));

  if (mp_RunEnv->isWriteResults()) prepareOutputs();



  mp_MachineListener->onCheckConsistencyDone(openfluid::machine::MachineListener::OK);
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
    m_ModelInstance.call_initializeRun((openfluid::base::SimulationStatus*)mp_SimStatus);
  }
  catch (openfluid::base::OFException& E)
  {
    mp_MachineListener->onInitializeRunDone(openfluid::machine::MachineListener::ERROR);
    throw;
  }

  if (m_SimulationBlob.getExecutionMessages().isWarningFlag()) mp_MachineListener->onInitializeRunDone(openfluid::machine::MachineListener::WARNING);
  else mp_MachineListener->onInitializeRunDone(openfluid::machine::MachineListener::OK);


  m_SimulationBlob.getExecutionMessages().resetWarningFlag();

  // check simulation vars production after init
  checkSimulationVarsProduction(0);


  // ============= runStep() =============



  mp_MachineListener->onBeforeRunSteps();

  do // time loop
  {

    mp_MachineListener->onRunStep(mp_SimStatus);

    m_SimulationBlob.getExecutionMessages().resetWarningFlag();

    try
    {
      m_ModelInstance.call_runStep(mp_SimStatus);

      // check simulation vars production at each time step
      checkSimulationVarsProduction(mp_SimStatus->getCurrentStep()+1);
    }
    catch (openfluid::base::OFException& E)
    {
      mp_MachineListener->onRunStepDone(openfluid::machine::MachineListener::ERROR);
      throw;
    }


    if (m_SimulationBlob.getExecutionMessages().isWarningFlag()) mp_MachineListener->onRunStepDone(openfluid::machine::MachineListener::WARNING);
    mp_MachineListener->onRunStepDone(openfluid::machine::MachineListener::OK);

    if (mp_RunEnv->isWriteResults()) saveOutputs(mp_SimStatus->getCurrentTime());

  } while (mp_SimStatus->switchToNextStep());  // end time loop


  mp_MachineListener->onAfterRunSteps();

  m_SimulationBlob.getExecutionMessages().resetWarningFlag();


  // ============= finalizeRun() =============

  mp_MachineListener->onFinalizeRun();

  try
  {
    m_ModelInstance.call_finalizeRun((openfluid::base::SimulationStatus*)mp_SimStatus);
  }
  catch (openfluid::base::OFException& E)
  {
    mp_MachineListener->onFinalizeRunDone(openfluid::machine::MachineListener::ERROR);
    throw;
  }


  if (m_SimulationBlob.getExecutionMessages().isWarningFlag()) mp_MachineListener->onFinalizeRunDone(openfluid::machine::MachineListener::WARNING);
  else mp_MachineListener->onFinalizeRunDone(openfluid::machine::MachineListener::OK);


  m_SimulationBlob.getExecutionMessages().resetWarningFlag();

  // check simulation vars production after finalize
  checkSimulationVarsProduction(mp_SimStatus->getCurrentStep()+1);

  // final save
  closeOutputs();

}


// =====================================================================
// =====================================================================

void Engine::saveReports()
{
  m_SimulationBlob.getExecutionMessages().resetWarningFlag();
  saveSimulationInfos();
}


// =====================================================================
// =====================================================================


void Engine::closeOutputs()
{
  if (mp_OutputsWriter != NULL) mp_OutputsWriter->closeFiles();
  if (mp_MessagesWriter != NULL) mp_MessagesWriter->closeFile(m_SimulationBlob.getExecutionMessages(),true);

}


} } //namespaces



