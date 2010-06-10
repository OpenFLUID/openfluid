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
  \file
  \brief implements ...

  \author Jean-Christophe FABRE <fabrejc@supagro.inra.fr>
*/


#include <iostream>
#include <iomanip>

#include <math.h>
#include <boost/filesystem.hpp>

#include <openfluid/machine/Engine.hpp>
#include <openfluid/tools.hpp>
#include <openfluid/machine/Factory.hpp>


namespace openfluid { namespace machine {


// =====================================================================
// =====================================================================


Engine::Engine(openfluid::machine::MachineListener* MachineListener,
               openfluid::io::IOListener* IOListener)
{
  mp_CoreData = openfluid::core::CoreRepository::getInstance();
  mp_ExecMsgs = openfluid::base::ExecutionMessages::getInstance();
  mp_RunEnv = openfluid::base::RuntimeEnvironment::getInstance();
  mp_PlugMan = PluginManager::getInstance();

  mp_MemMon = openfluid::core::MemoryMonitor::getInstance();

  mp_IOMan = openfluid::io::IOManager::getInstance();
  mp_IOMan->setListener(IOListener);

  mp_Listener = MachineListener;
  if (mp_Listener == NULL) mp_Listener = new openfluid::machine::MachineListener();

  mp_SimStatus = NULL;
}

// =====================================================================
// =====================================================================

Engine::~Engine()
{

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
  if (mp_CoreData->isUnitsClassExist(ClassName)) UnitList = mp_CoreData->getUnits(ClassName)->getList();
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
  if (mp_CoreData->isUnitsClassExist(ClassName)) UnitList = mp_CoreData->getUnits(ClassName)->getList();
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
  if (mp_CoreData->isUnitsClassExist(ClassName)) UnitList = mp_CoreData->getUnits(ClassName)->getList();
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

  AllUnits = mp_CoreData->getUnits();

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



  FuncIter = mp_ModelInstance->getItems().begin();

  while (FuncIter != mp_ModelInstance->getItems().end() && IsOK)
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
  for (FuncIter = mp_ModelInstance->getItems().begin(); FuncIter != mp_ModelInstance->getItems().end(); ++FuncIter)
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



bool Engine::buildModel()
{
  bool IsVerbose = false;
  openfluid::base::RuntimeEnvironment::getInstance()->getExtraProperties().getValue("display.verbose",&IsVerbose);
  bool IsQuiet = false;
  openfluid::base::RuntimeEnvironment::getInstance()->getExtraProperties().getValue("display.quiet",&IsQuiet);

  try
  {
    Factory MF = Factory();
    mp_ModelInstance = MF.buildInstanceFromDescriptor(m_ModelDesc);
  }
  catch (openfluid::base::OFException& E)
  {

    if (!IsQuiet)
    {
      if (!IsVerbose)
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

  Factory REF = Factory();
  REF.fillRunEnvironmentFromDescriptor(m_RunDesc);

  return true;
}

// =====================================================================
// =====================================================================


bool Engine::buildSpatialDomain()
{

  Factory DF = Factory();

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
  bool IsVerbose = false;
  openfluid::base::RuntimeEnvironment::getInstance()->getExtraProperties().getValue("display.verbose",&IsVerbose);
  bool IsQuiet = false;
  openfluid::base::RuntimeEnvironment::getInstance()->getExtraProperties().getValue("display.quiet",&IsQuiet);

  bool IsOK = true;


  // inits the simulation infos and status

  mp_SimStatus = new openfluid::base::SimulationStatus(mp_RunEnv->getSimulationStartTime(),
                                                       mp_RunEnv->getSimulationEndTime(),
                                                       mp_RunEnv->getSimulationTimeStep());

  if (mp_RunEnv->isProgressiveOutput())
  {
    mp_MemMon->setPacketAndKeep(mp_RunEnv->getProgressiveOutputPacket(),
                                mp_RunEnv->getProgressiveOutputKeep());
  }
  else
  {
    mp_MemMon->setPacketAndKeep(mp_SimStatus->getStepsCount(),1);
  }



  // check simulation functions count

  try
  {
    if (mp_ModelInstance->getItemsCount() == 0)
      throw openfluid::base::OFException("OpenFLUID framework","Engine::prepareDataAndCheckConsistency","No simulation function in model");

    checkExtraFilesConsistency();

    checkModelConsistency();

    checkDataConsistency();
  }
  catch (openfluid::base::OFException& E)
  {
    if (!IsQuiet)
    {
      if (!IsVerbose)
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
    if (!IsQuiet)
    {
      if (!IsVerbose)
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

  mp_IOMan->initOutputs();

  mp_IOMan->prepareOutputDir();
  if (mp_RunEnv->isWriteResults())
  {
    mp_IOMan->prepareOutputs();
  }


  mp_IOMan->clearFluidXData();

  return true;
}


// =====================================================================
// =====================================================================


bool Engine::run()
{
  bool IsOK = true;
  bool IsVerbose = false;
  openfluid::base::RuntimeEnvironment::getInstance()->getExtraProperties().getValue("display.verbose",&IsVerbose);
  bool IsQuiet = false;
  openfluid::base::RuntimeEnvironment::getInstance()->getExtraProperties().getValue("display.quiet",&IsQuiet);


  std::string ProdMessage;
  openfluid::core::TimeStep_t SaveReleaseBegin, SaveReleaseEnd;


  // Check for simulation vars production before init
  checkSimulationVarsProduction(0);


  // ============= initializeRun() =============

  if (!IsQuiet)
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
    if (!IsQuiet)
    {
      if (!IsVerbose)
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

  if (!IsQuiet && !IsVerbose)
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


  if (!IsQuiet)
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
    if (!IsQuiet)
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

      if (!IsQuiet)
      {
        if (!IsVerbose)
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

    if (!IsQuiet && !IsVerbose)
    {

      if (mp_ExecMsgs->isWarningFlag()) std::cout << std::setw(12) << "[Warning]";
      else std::cout << std::setw(12) << "[OK]";
      std::cout.flush();
    }


    if (!IsQuiet)
    {
      std::cout << std::endl;
      std::cout.flush();
    }

    if (mp_RunEnv->isWriteResults())
    {
      mp_IOMan->saveOutputs(mp_SimStatus->getCurrentTime());
    }


    // progressive output
    if (mp_RunEnv->isProgressiveOutput() && mp_MemMon->isMemReleaseStep(mp_SimStatus->getCurrentStep()))
    {
      mp_MemMon->getMemoryReleaseRange(mp_SimStatus->getCurrentStep(),false,&SaveReleaseBegin, &SaveReleaseEnd);
      std::cout << std::endl << " -- Releasing memory (" << SaveReleaseBegin << " -> " << SaveReleaseEnd << ") "; std::cout.flush();
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

  if (!IsQuiet)
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
    if (!IsQuiet)
    {
      if (!IsVerbose)
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


  if (!IsQuiet && !IsVerbose)
  {
    if (mp_ExecMsgs->isWarningFlag()) std::cout << std::setw(12) << "[Warning]";
    else std::cout << std::setw(12) << "[OK]";
    std::cout << std::endl;
    std::cout.flush();
  }

  if (!IsQuiet)
  {
    std::cerr << std::endl;
  }

  mp_ExecMsgs->resetWarningFlag();


  // check simulation vars production after finalize
  checkSimulationVarsProduction(mp_SimStatus->getCurrentStep()+1);

  // final progressive output
  mp_MemMon->getMemoryReleaseRange(mp_SimStatus->getStepsCount()-1,true,&SaveReleaseBegin, &SaveReleaseEnd);
  std::cout << std::endl << "  -- Releasing memory (" << SaveReleaseBegin << " -> " << SaveReleaseEnd << ") "; std::cout.flush();
  mp_IOMan->closeOutputs();
  mp_IOMan->saveMessages();
  mp_CoreData->doMemRelease(mp_SimStatus->getStepsCount()-1,true);
  mp_ExecMsgs->doMemRelease();
  std::cout << "[OK] --" << std::endl << std::endl ;

  return IsOK;
}


// =====================================================================
// =====================================================================

bool Engine::saveReports()
{
  mp_ExecMsgs->resetWarningFlag();
  return (mp_IOMan->saveSimulationInfos((openfluid::base::SimulationInfo*)mp_SimStatus));
}


// =====================================================================
// =====================================================================


bool Engine::saveMessages()
{
  mp_ExecMsgs->resetWarningFlag();
  return (mp_IOMan->saveMessages());
}


// =====================================================================
// =====================================================================


void Engine::closeOutputs()
{
  mp_IOMan->closeOutputs();
}


} } //namespaces



