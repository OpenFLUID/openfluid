/***copyright>=========================================================

  OpenFLUID - Software environment for modelling Fluxes In Landscapes

  This file is a part of OpenFLUID software
  http://www.umr-lisah.fr/openfluid
  Copyright (c) INRA
  See the file "copyright" for the full license governing this code.

=========================================================<copyright***/


/**
  \file
  \brief implements ...

  \author Jean-Christophe FABRE <fabrejc@supagro.inra.fr>
*/


#include "Engine.h"
#include "AppTools.h"

#include <iostream>
#include <iomanip>

#include <math.h>
#include <boost/filesystem.hpp>

#include "model/ModelFactory.h"
#include "domain/DomainFactory.h"




// =====================================================================
// =====================================================================


Engine::Engine()
{


  mp_CoreData = openfluid::core::CoreRepository::getInstance();
  mp_ExecMsgs = openfluid::base::ExecutionMessages::getInstance();
  mp_RunEnv = RuntimeEnvironment::getInstance();
  mp_PlugMan = PluginManager::getInstance();

  mp_MemMon = openfluid::core::MemoryMonitor::getInstance();

  mp_IOMan = IOManager::getInstance();

  mp_SimStatus = NULL;

}

// =====================================================================
// =====================================================================

Engine::~Engine()
{

}


// =====================================================================
// =====================================================================


bool Engine::checkExistingVariable(openfluid::core::VariableName_t VarName,
                                   openfluid::core::UnitClass_t ClassName)
{
  openfluid::core::UnitsList_t::const_iterator UnitIter;
  openfluid::core::UnitsList_t* UnitList;

  UnitList = NULL;
  if (mp_CoreData->isUnitsClassExist(ClassName)) UnitList = mp_CoreData->getUnits(ClassName)->getList();
  else return false;

  bool Status = true;
  UnitIter = UnitList->begin();
  while (Status && (UnitIter != UnitList->end()))
  {
    if (IsVectorNamedVariable(VarName)) Status = (*UnitIter).getVectorVariables()->isVariableExist(GetVectorNamedVariableName(VarName));
    else Status = (*UnitIter).getScalarVariables()->isVariableExist(VarName);
    ++UnitIter;
  }

  return Status;
}

// =====================================================================
// =====================================================================


bool Engine::createVariable(openfluid::core::VariableName_t VarName,
                            openfluid::core::UnitClass_t ClassName,
                            bool UpdateMode)
{
  openfluid::core::UnitsList_t::iterator UnitIter;
  openfluid::core::UnitsList_t* UnitList;

  UnitList = NULL;
  if (mp_CoreData->isUnitsClassExist(ClassName)) UnitList = mp_CoreData->getUnits(ClassName)->getList();
  else return false;

  bool Status = true;

  // if not update mode, variables must not exist before creation
  if (!UpdateMode)
  {
    UnitIter = UnitList->begin();
    while (Status && (UnitIter != UnitList->end()))
    {
      if (IsVectorNamedVariable(VarName)) Status = !((*UnitIter).getVectorVariables()->isVariableExist(GetVectorNamedVariableName(VarName)));
      else Status = !((*UnitIter).getScalarVariables()->isVariableExist(VarName));
      ++UnitIter;
    }
  }

  if (Status)
  {
    for(UnitIter = UnitList->begin(); UnitIter != UnitList->end(); ++UnitIter )
    {
      if (IsVectorNamedVariable(VarName)) (*UnitIter).getVectorVariables()->createVariable(GetVectorNamedVariableName(VarName));
      else (*UnitIter).getScalarVariables()->createVariable(VarName);
    }
  }

  return Status;
}

// =====================================================================
// =====================================================================



bool Engine::checkExistingInputData(openfluid::core::InputDataName_t DataName,
                                    openfluid::core::UnitClass_t ClassName)
{
  openfluid::core::UnitsList_t::const_iterator UnitIter;
  openfluid::core::UnitsList_t* UnitList;


  UnitList = NULL;
  if (mp_CoreData->isUnitsClassExist(ClassName)) UnitList = mp_CoreData->getUnits(ClassName)->getList();
  else return false;

  bool Status = true;

  UnitIter = UnitList->begin();
  while (Status && (UnitIter != UnitList->end()))
  {
    Status = (*UnitIter).getInputData()->isDataExist(DataName);
    ++UnitIter;
  }


  return Status;
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

  AllUnits = mp_CoreData->getUnits();

  for (UnitsClassesIter = AllUnits->begin(); UnitsClassesIter != AllUnits->end();++UnitsClassesIter)
  {
    UnitsList = UnitsClassesIter->second.getList();

    for (UnitsIter = UnitsList->begin();UnitsIter != UnitsList->end();++UnitsIter)
    {

      //scalars
      if (!((*UnitsIter).getScalarVariables()->isAllVariablesCount(ExpectedVarsCount)))
        throw openfluid::base::OFException("kernel","Engine::checkSimulationVarsProduction","Scalar variable production error");

      //vectors
      if (!((*UnitsIter).getVectorVariables()->isAllVariablesCount(ExpectedVarsCount)))
        throw openfluid::base::OFException("kernel","Engine::checkSimulationVarsProduction","Vector variable production error");
    }

  }

}

// =====================================================================
// =====================================================================

void Engine::checkModelConsistency()
{

  bool IsOK = true;

//  PluginsList::iterator FuncIter;
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

  FuncIter = mp_ModelInstance->getItems().begin();

  while (FuncIter != mp_ModelInstance->getItems().end() && IsOK)
  {
    CurrentFunction = (*FuncIter);
    HData = CurrentFunction->Signature->HandledData;

    // checking required variables
    i = 0;
    while (IsOK && i < HData.RequiredVars.size())
    {
      IsOK = checkExistingVariable(HData.RequiredVars[i].DataName, HData.RequiredVars[i].UnitClass);

      if (!IsOK) throw openfluid::base::OFException("kernel","Engine::checkModelConsistency",HData.RequiredVars[i].DataName + " variable on " + HData.RequiredVars[i].UnitClass + " required by " + CurrentFunction->Signature->ID + " is not previously created");
      else i++;
    }

    // checking variables to create (produced)
    i = 0;
    while (IsOK && i < HData.ProducedVars.size())
    {
      IsOK = createVariable(HData.ProducedVars[i].DataName, HData.ProducedVars[i].UnitClass,false);

      if (!IsOK) throw openfluid::base::OFException("kernel","Engine::checkModelConsistency",HData.ProducedVars[i].DataName + " variable on " + HData.ProducedVars[i].UnitClass + " produced by " + CurrentFunction->Signature->ID + " cannot be created because it is previously created");
      else i++;
    }

    // checking variables to update
    i = 0;
    while (IsOK && i < HData.UpdatedVars.size())
    {
      IsOK = createVariable(HData.UpdatedVars[i].DataName, HData.UpdatedVars[i].UnitClass,true);

      if (!IsOK) throw openfluid::base::OFException("kernel","Engine::checkModelConsistency",HData.UpdatedVars[i].DataName + " variable on " + HData.UpdatedVars[i].UnitClass + " updated by " + CurrentFunction->Signature->ID + " cannot be handled");
      else i++;
    }

    FuncIter++;
  }



  FuncIter = mp_ModelInstance->getItems().begin();

  while (FuncIter != mp_ModelInstance->getItems().end() && IsOK)
  {
    CurrentFunction = (*FuncIter);
    HData = CurrentFunction->Signature->HandledData;

    // checking required variables at t-1+
    i = 0;
    while (IsOK && i < HData.RequiredPrevVars.size())
    {
      IsOK = checkExistingVariable(HData.RequiredPrevVars[i].DataName, HData.RequiredPrevVars[i].UnitClass);

      if (!IsOK) throw openfluid::base::OFException("kernel","Engine::checkModelConsistency",HData.RequiredPrevVars[i].DataName + " variable on " + HData.RequiredPrevVars[i].UnitClass + " required by " + CurrentFunction->Signature->ID + " is not created");
      else i++;
    }

    FuncIter++;
  }

}

// =====================================================================
// =====================================================================



void Engine::checkDataConsistency()
{

  bool IsOK = true;


  std::list<ModelItemInstance*>::const_iterator FuncIter;
  openfluid::base::SignatureHandledData HData;
  ModelItemInstance* CurrentFunction;
  unsigned int i;


  FuncIter = mp_ModelInstance->getItems().begin();

  while (FuncIter != mp_ModelInstance->getItems().end() && IsOK)
  {
    CurrentFunction = (*FuncIter);
    HData = CurrentFunction->Signature->HandledData;


    // checking required input data
    i = 0;
    while (IsOK && i < HData.RequiredInput.size())
    {

      IsOK = checkExistingInputData(HData.RequiredInput[i].DataName, HData.RequiredInput[i].UnitClass);

      if (!IsOK) throw openfluid::base::OFException("kernel","Engine::checkDataConsistency",HData.RequiredInput[i].DataName + " input data on " + HData.RequiredInput[i].UnitClass + " required by " + CurrentFunction->Signature->ID + " is not available");
      else i++;
    }
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
  for (FuncIter = mp_ModelInstance->getItems().begin(); FuncIter != mp_ModelInstance->getItems().end(); ++FuncIter)
  {
    CurrentFunction = *FuncIter;

    HData = CurrentFunction->Signature->HandledData;

    for (unsigned int i=0;i<HData.RequiredExtraFiles.size();i++)
    {

      boost::filesystem::path ReqExtraFilePath(mp_RunEnv->getInputFullPath(HData.RequiredExtraFiles[i]));
      if (!boost::filesystem::exists(ReqExtraFilePath))
        throw openfluid::base::OFException("kernel","Engine::checkExtraFilesConsistency","File " + HData.RequiredExtraFiles[i] + " required by " + CurrentFunction->Signature->ID + " not found");
    }
  }
}

// =====================================================================
// =====================================================================



bool Engine::buildModel()
{

  try
  {
    ModelFactory MF = ModelFactory();
    mp_ModelInstance = MF.buildInstanceFromDescriptor(m_ModelDesc);
  }
  catch (openfluid::base::OFException& E)
  {

    if (!mp_RunEnv->isQuietRun())
    {
      if (!mp_RunEnv->isVerboseRun())
      {
        std::cout << std::setw(12) << "[Error]";
        std::cout << std::endl << std::endl;
        std::cout.flush();
      }
      else
      {
        std::cout << "  " << "[Error]";
        std::cout << std::endl << std::endl;
        std::cout.flush();

      }
    }
    throw;
  }
/*
  if (!mp_RunEnv->isQuietRun() && !mp_RunEnv->isVerboseRun())
  {

    if (mp_ExecMsgs->isWarningFlag()) std::cout << std::setw(12) << "[Warning]";
    else std::cout << std::setw(12) << "[OK]";
    std::cout.flush();
  }
*/
  return true;

}


// =====================================================================
// =====================================================================

bool Engine::loadData()
{

  mp_IOMan->loadInputs(m_ModelDesc, m_DomainDesc, m_RunDesc);

  return true;

}



// =====================================================================
// =====================================================================


bool Engine::processRunConfiguration()
{

  if (m_RunDesc.isProgressiveOutput())
  {
    mp_RunEnv->setProgressiveOutputKeep(m_RunDesc.getProgressiveOutputKeep());
    mp_RunEnv->setProgressiveOutputPacket(m_RunDesc.getProgressiveOutputPacket());
    mp_MemMon->setPacketAndKeep(m_RunDesc.getProgressiveOutputPacket(),m_RunDesc.getProgressiveOutputKeep());
  }

  if (m_RunDesc.isSimulationID())
  {
    mp_RunEnv->setSimulationID(m_RunDesc.getSimulationID());
  }


  return true;
}

// =====================================================================
// =====================================================================


bool Engine::buildSpatialDomain()
{

  DomainFactory DF = DomainFactory();

  DF.buildDomainFromDescriptor(m_DomainDesc);

  return true;
}

// =====================================================================
// =====================================================================

bool Engine::initParams()
{
  mp_ModelInstance->initParams();

  return true;
}

// =====================================================================
// =====================================================================

bool Engine::prepareDataAndCheckConsistency()
{

  bool IsOK = true;


  // inits the simulation infos and status

  mp_SimStatus = new openfluid::base::SimulationStatus(m_RunDesc.getBeginDate(),
                                                       m_RunDesc.getEndDate(),
                                                       m_RunDesc.getDeltaT());

  if (mp_RunEnv->isProgressiveOutput())
  {
    mp_MemMon->setPacketAndKeep(mp_RunEnv->getProgressiveOutputPacket(),mp_RunEnv->getProgressiveOutputKeep());
  }
  else
  {
    mp_MemMon->setPacketAndKeep(mp_SimStatus->getStepsCount(),1);
  }



  // check simulation functions count

  try
  {
    if (mp_ModelInstance->getItemsCount() == 0)
      throw openfluid::base::OFException("kernel","Engine::prepareDataAndCheckConsistency","No simulation function in model");

    checkExtraFilesConsistency();

    checkModelConsistency();

    checkDataConsistency();
  }
  catch (openfluid::base::OFException& E)
  {
    if (!mp_RunEnv->isQuietRun())
    {
      if (!mp_RunEnv->isVerboseRun())
      {
        std::cout << std::setw(12) << "[Error]";
        std::cout << std::endl << std::endl;
        std::cout.flush();
      }
      else
      {
        std::cout << "  " << "[Error]";
        std::cout << std::endl << std::endl;
        std::cout.flush();
      }
    }
    throw;
  }


  try
  {
    mp_ModelInstance->prepareDataAndCheckConsistency();
  }
  catch (openfluid::base::OFException& E)
  {
    if (!mp_RunEnv->isQuietRun())
    {
      if (!mp_RunEnv->isVerboseRun())
      {
        std::cout << std::setw(12) << "[Error]";
        std::cout << std::endl << std::endl;
        std::cout.flush();
      }
      else
      {
        std::cout << "  " << "[Error]";
        std::cout << std::endl << std::endl;
        std::cout.flush();
      }
    }
    throw;
  }




  if (!IsOK)
  {
    return false;
  }



  mp_IOMan->prepareOutputDir();
  if (mp_RunEnv->isWriteResults())
  {
    mp_IOMan->prepareOutputs();
  }


  return true;
}


// =====================================================================
// =====================================================================


bool Engine::run()
{
  bool IsOK = true;

  std::string ProdMessage;
  openfluid::core::TimeStep_t SaveReleaseBegin, SaveReleaseEnd;


  // Check for simulation vars production before init
  checkSimulationVarsProduction(0);


  // ============= initializeRun() =============

  if (!mp_RunEnv->isQuietRun())
  {
    std::cout << std::endl;
    std::cout << std::setw(16) << "Initialize...";
    std::cout.flush();
  }

  try
  {
    mp_ModelInstance->initializeRun((openfluid::base::SimulationStatus*)mp_SimStatus);
  }
  catch (openfluid::base::OFException& E)
  {
    if (!mp_RunEnv->isQuietRun())
    {
      if (!mp_RunEnv->isVerboseRun())
      {
        std::cout << std::setw(12) << "[Error]";
        std::cout << std::endl << std::endl;
        std::cout.flush();
      }
      else
      {
        std::cout << "  " << "[Error]";
        std::cout << std::endl << std::endl;
        std::cout.flush();

      }
    }
    throw;
  }

  if (!mp_RunEnv->isQuietRun() && !mp_RunEnv->isVerboseRun())
  {
    if (mp_ExecMsgs->isWarningFlag()) std::cout << std::setw(12) << "[Warning]";
    else std::cout << std::setw(12) << "[OK]";
    std::cout << std::endl;
    std::cout.flush();

  }


  mp_ExecMsgs->resetWarningFlag();

  // check simulation vars production after init
  checkSimulationVarsProduction(0);


  // ============= runStep() =============


  if (!mp_RunEnv->isQuietRun())
  {
    std::cout << std::endl;
    std::cout << std::setw(10) << "Time step";
    std::cout << std::setw(18) << "Real time";
    std::cout << std::setw(17) << "Status";
    std::cout << std::endl;
    std::cout << std::setw(10) << "---------";
    std::cout << std::setw(18) << "---------";
    std::cout << std::setw(17) << "------";
    std::cout << std::endl;
    std::cout.flush();
  }

  do // time loop
  {
    if (!mp_RunEnv->isQuietRun())
    {
      std::cout << std::setw(8) << mp_SimStatus->getCurrentStep();
      std::cout << std::setw(25) << mp_SimStatus->getCurrentTime().getAsISOString();
      std::cout.flush();
    }

    mp_ExecMsgs->resetWarningFlag();

    try
    {
      mp_ModelInstance->runStep(mp_SimStatus);

      // check simulation vars production at each time step
      checkSimulationVarsProduction(mp_SimStatus->getCurrentStep()+1);
    }
    catch (openfluid::base::OFException& E)
    {

      if (!mp_RunEnv->isQuietRun())
      {
        if (!mp_RunEnv->isVerboseRun())
        {
          std::cout << std::setw(12) << "[Error]";
          std::cout << std::endl << std::endl;
          std::cout.flush();
        }
        else
        {
          std::cout << "  " << "[Error]";
          std::cout << std::endl << std::endl;
          std::cout.flush();

        }
      }
      throw;
    }

    if (!mp_RunEnv->isQuietRun() && !mp_RunEnv->isVerboseRun())
    {

      if (mp_ExecMsgs->isWarningFlag()) std::cout << std::setw(12) << "[Warning]";
      else std::cout << std::setw(12) << "[OK]";
      std::cout.flush();
    }


    if (!mp_RunEnv->isQuietRun())
    {
      std::cout << std::endl;
      std::cout.flush();
    }

    // progressive output
    if (mp_RunEnv->isProgressiveOutput() && mp_MemMon->isMemReleaseStep(mp_SimStatus->getCurrentStep()))
    {
      mp_MemMon->getMemoryReleaseRange(mp_SimStatus->getCurrentStep(),false,&SaveReleaseBegin, &SaveReleaseEnd);
      std::cout << std::endl << " -- Saving outputs and releasing memory (" << SaveReleaseBegin << " -> " << SaveReleaseEnd << ") "; std::cout.flush();
      mp_IOMan->saveOutputs(mp_SimStatus->getCurrentStep(),(openfluid::base::SimulationInfo*)mp_SimStatus,false);
      mp_IOMan->saveMessages();
      mp_CoreData->doMemRelease(mp_SimStatus->getCurrentStep(),false);
      mp_ExecMsgs->doMemRelease();
      mp_MemMon->setLastMemoryRelease(mp_SimStatus->getCurrentStep());
      std::cout << "[OK] --" << std::endl << std::endl ;
    }


  } while (mp_SimStatus->switchToNextStep());  // end time loop


  std::cout << std::endl;

  mp_ExecMsgs->resetWarningFlag();


  // ============= finalizeRun() =============

  if (!mp_RunEnv->isQuietRun())
  {
    std::cout << std::setw(16) << "Finalize...";
    std::cout.flush();
  }

  try
  {
    mp_ModelInstance->finalizeRun((openfluid::base::SimulationStatus*)mp_SimStatus);
  }
  catch (openfluid::base::OFException& E)
  {
    if (!mp_RunEnv->isQuietRun())
    {
      if (!mp_RunEnv->isVerboseRun())
      {
        std::cout << std::setw(12) << "[Error]";
        std::cout << std::endl << std::endl;
        std::cout.flush();
      }
      else
      {
        std::cout << "  " << "[Error]";
        std::cout << std::endl << std::endl;
        std::cout.flush();

      }
    }
    throw;
  }


  if (!mp_RunEnv->isQuietRun() && !mp_RunEnv->isVerboseRun())
  {
    if (mp_ExecMsgs->isWarningFlag()) std::cout << std::setw(12) << "[Warning]";
    else std::cout << std::setw(12) << "[OK]";
    std::cout << std::endl;
    std::cout.flush();
  }

  if (!mp_RunEnv->isQuietRun())
  {
    std::cerr << std::endl;
  }

  mp_ExecMsgs->resetWarningFlag();


  // check simulation vars production after finalize
  checkSimulationVarsProduction(mp_SimStatus->getCurrentStep()+1);

  // final progressive output
  mp_MemMon->getMemoryReleaseRange(mp_SimStatus->getStepsCount()-1,true,&SaveReleaseBegin, &SaveReleaseEnd);
  std::cout << std::endl << "  -- Saving outputs and releasing memory (" << SaveReleaseBegin << " -> " << SaveReleaseEnd << ") "; std::cout.flush();
  mp_IOMan->saveOutputs(mp_SimStatus->getStepsCount()-1,(openfluid::base::SimulationInfo*)mp_SimStatus,true);
  mp_IOMan->saveMessages();
  mp_CoreData->doMemRelease(mp_SimStatus->getStepsCount()-1,true);
  mp_ExecMsgs->doMemRelease();
  std::cout << "[OK] --" << std::endl << std::endl ;


  return IsOK;
}


// =====================================================================
// =====================================================================

/*
bool Engine::saveResults(ExtraSimInfos ExSI)
{
  mp_ExecMsgs->resetWarningFlag();
  return (mp_IOMan->saveOutputs(m_RunConfig, mp_SimStatus->getStepsCount(),ExSI));
}
*/
// =====================================================================
// =====================================================================

bool Engine::saveReports(std::string ErrorMsg)
{
  mp_ExecMsgs->resetWarningFlag();
  return (mp_IOMan->saveSimulationInfos((openfluid::base::SimulationInfo*)mp_SimStatus,ErrorMsg));
}



// =====================================================================
// =====================================================================


bool Engine::saveMessages()
{
  mp_ExecMsgs->resetWarningFlag();
  return (mp_IOMan->saveMessages());
}


