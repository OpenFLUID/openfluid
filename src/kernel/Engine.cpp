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


// =====================================================================
// =====================================================================

#define DECLARE_FUNCTION_PARSER \
    PluginsList::iterator _M_FuncIter;

/**
  Macro for parsing the functions list and calling the given method of each function of the list
  \param[in] calledmethod the method to call on each function
  \param[out] statevar the globalized return of the method calls
 */
#define PARSE_FUNCTION_LIST(calledmethod,statevar) \
    _M_FuncIter = m_Functions.begin(); \
    while (_M_FuncIter != m_Functions.end() && statevar) \
    { \
      PluginContainer* CurrentFunction = *_M_FuncIter; \
      if (CurrentFunction != NULL) \
      { \
        if (mp_RunEnv->isVerboseRun()) \
        { \
          std::cout << std::endl << std::setw(50) << CurrentFunction->Signature->ID; \
          std::cout.flush(); \
        } \
        statevar = (statevar && CurrentFunction->Function->calledmethod); \
        if (mp_RunEnv->isVerboseRun()) \
        { \
          if (!statevar) \
          { \
            std::cout << "  " << "[Error]";\
            throw openfluid::base::OFException("kernel","Bad return value while calling function method"); \
          }  \
          else \
          { \
            if (mp_ExecMsgs->isWarningFlag()) std::cout << "  " << "[Warning]"; \
            else std::cout << "  " << "[OK]"; \
            mp_ExecMsgs->resetWarningFlag(); \
          } \
          std::cout.flush(); \
        } \
      } \
      _M_FuncIter++; \
    } \
    if (mp_RunEnv->isVerboseRun()) std::cout << std::endl; std::cout.flush();


/**
  Macro for parsing the functions list and calling the given two methods of each function of the list
  \param[in] calledmethod1 the first method to call on each function
  \param[in] calledmethod2 the first method to call on each function
  \param[out] statevar the globalized return of the method calls
 */

#define PARSE_FUNCTION_LIST_TWO(calledmethod1,calledmethod2,statevar) \
    _M_FuncIter = m_Functions.begin(); \
    while (_M_FuncIter != m_Functions.end() && statevar) \
    { \
      PluginContainer* CurrentFunction = *_M_FuncIter; \
      if (CurrentFunction != NULL) \
      { \
        if (mp_RunEnv->isVerboseRun()) \
        { \
          std::cout << std::endl << std::setw(50) << CurrentFunction->Signature->ID; \
          std::cout.flush(); \
        } \
        statevar = (statevar && (CurrentFunction->Function->calledmethod1 && CurrentFunction->Function->calledmethod2)); \
        if (mp_RunEnv->isVerboseRun()) \
        { \
          if (!statevar) \
          { \
            std::cout << "  " << "[Error]";\
            throw openfluid::base::OFException("kernel","Bad return value while calling function method"); \
          }  \
          else \
          { \
            if (mp_ExecMsgs->isWarningFlag()) std::cout << "  " << "[Warning]"; \
            else std::cout << "  " << "[OK]"; \
            mp_ExecMsgs->resetWarningFlag(); \
          } \
          std::cout.flush(); \
        } \
      } \
      _M_FuncIter++; \
    } \
    if (mp_RunEnv->isVerboseRun()) std::cout << std::endl; std::cout.flush();




// =====================================================================
// =====================================================================





Engine::Engine(openfluid::core::CoreRepository* CoreData, openfluid::base::ExecutionMessages* ExecMsgs,
               RuntimeEnvironment* RunEnv, PluginManager* PlugMan)
{


  mp_CoreData = CoreData;
  mp_ExecMsgs = ExecMsgs;
  mp_RunEnv = RunEnv;
  mp_PlugMan = PlugMan;

  mp_MemMon = openfluid::core::MemoryMonitor::getInstance();


  m_Functions.clear();

  mp_IOMan = IOManager::getInstance();
  mp_IOMan->setCoreRepository(CoreData);
  mp_IOMan->setExecutionMessages(ExecMsgs);
  mp_IOMan->setRunEnvironment(RunEnv);

  mp_SimStatus = NULL;

}

// =====================================================================
// =====================================================================

Engine::~Engine()
{

}

// =====================================================================
// =====================================================================




bool Engine::processConfig()
{
  /** \internal

    uses the PluginManager to
      - load and parameterize functions
  */


  std::list<FunctionConfig>::iterator FuncIt;

  FunctionConfig* FConf;
  PluginContainer* FuncToAdd;

  m_Functions.clear();


  // on each function
  for (FuncIt = m_ModelConfig.FuncConfigs.begin(); FuncIt != m_ModelConfig.FuncConfigs.end(); ++FuncIt)
  {
    FConf = &(*FuncIt);

    // instanciates and gets a pointer to the function
    FuncToAdd = mp_PlugMan->getPlugin(FConf->FileID,mp_CoreData);

    if (FuncToAdd != NULL)
    {
       if (FuncToAdd->Function->initParams(FConf->Params))
       {
         m_Functions.push_back(FuncToAdd);
       }
       else
       {
         throw openfluid::base::OFException("kernel","Engine::processConfig","Initializing params function error");
         return false;
       }

    }
    else
    {
      throw openfluid::base::OFException("kernel","Engine::processConfig","Loading function from plugin " + FConf->FileID);
      return false;
    }
  }


  return true;
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



bool Engine::checkSimulationVarsProduction(int ExpectedVarsCount, std::string* Message)
{

  // TODO check this (method to check simulation vars production)
/*
  openfluid::core::UnitsListByClassMap_t::const_iterator UnitsClassesIter;
  openfluid::core::UnitsList_t::const_iterator UnitsIter;
  const openfluid::core::UnitsListByClassMap_t* AllUnits;
  const openfluid::core::UnitsList_t* UnitsList;
  std::vector<std::string> VarsNames;
  unsigned int i;

  AllUnits = mp_CoreData->getUnits();

  for (UnitsClassesIter = AllUnits->begin(); UnitsClassesIter != AllUnits->end();++UnitsClassesIter)
  {
    UnitsList = UnitsClassesIter->second.getList();

    for (UnitsIter = UnitsList->begin();UnitsIter !=UnitsList->end();++UnitsIter)
    {
      //scalars
      VarsNames = UnitsIter->getScalarVariables()->getVariablesNames();
      for (i=0;i<VarsNames.size();i++)
      {
        if (UnitsIter->getScalarVariables()->getVariableValuesCount(VarsNames[i]) != ExpectedVarsCount) return false;
      }


      //vectors
    }

  }
*/
  /*
  openfluid::core::SurfaceUnit *SU;
  openfluid::core::SUMap *SUsMap = mp_CoreData->getSpatialData()->getSUsCollection();
  openfluid::core::SUMap::iterator SUiter;

  openfluid::core::ReachSegment *RS;
  openfluid::core::RSMap *RSsMap = mp_CoreData->getSpatialData()->getRSsCollection();
  openfluid::core::RSMap::iterator RSiter;

  openfluid::core::GroundwaterUnit *GU;
  openfluid::core::GUMap *GUsMap = mp_CoreData->getSpatialData()->getGUsCollection();
  openfluid::core::GUMap::iterator GUiter;

  openfluid::core::SimulatedVarsMap *VarsMap;
  openfluid::core::SimulatedVarsMap::iterator VMiter;

  openfluid::core::SimulatedVectorVarsMap *VectVarsMap;
  openfluid::core::SimulatedVectorVarsMap::iterator VVMiter;

  (*Message) = wxT("");

  // checking SUs
  for(SUiter = SUsMap->begin(); SUiter != SUsMap->end(); ++SUiter)
  {
    VarsMap = SUiter->second->getSimulatedVars();
    VMiter = VarsMap->begin();

    for(VMiter = VarsMap->begin(); VMiter != VarsMap->end(); ++VMiter)
    {
      if (VMiter->second->size() != ExpectedVarsCount)
      {
        (*Message) = wxT("variable ") + VMiter->first + wxT(" on SU ") + wxString::Format(wxT("%d"),SUiter->first) +
                     wxT(" has ") + wxString::Format(wxT("%d"),VMiter->second->size()) + wxT(" values but ") +
                     wxString::Format(wxT("%d"),ExpectedVarsCount) + wxT(" were expected");
        return false;  // checks correct vars count
      }
      if (ExpectedVarsCount > 0 && isnan(VMiter->second->at(ExpectedVarsCount-1)))
      {
        (*Message) = wxT("variable ") + VMiter->first + wxT(" on SU ") + wxString::Format(wxT("%d"),SUiter->first) +
                     wxT(" is NaN");
        return false; // checks if vars are not NaN
      }

    }


    // vector vars
    VectVarsMap = SUiter->second->getSimulatedVectorVars();
    VVMiter = VectVarsMap->begin();

    for(VVMiter = VectVarsMap->begin(); VVMiter != VectVarsMap->end(); ++VVMiter)
    {
      if (VVMiter->second->size() != ExpectedVarsCount)
      {
        (*Message) = wxT("variable ") + VVMiter->first + wxT("[] on SU ") + wxString::Format(wxT("%d"),SUiter->first) +
                     wxT(" has ") + wxString::Format(wxT("%d"),VVMiter->second->size()) + wxT(" values but ") +
                     wxString::Format(wxT("%d"),ExpectedVarsCount) + wxT(" were expected");
        return false;  // checks correct vars count
      }
    }



  }


  // checking RSs
  for(RSiter = RSsMap->begin(); RSiter != RSsMap->end(); ++RSiter)
  {
    VarsMap = RSiter->second->getSimulatedVars();
    VMiter = VarsMap->begin();

    for(VMiter = VarsMap->begin(); VMiter != VarsMap->end(); ++VMiter)
    {
      if (VMiter->second->size() != ExpectedVarsCount)
      {
        (*Message) = wxT("variable ") + VMiter->first + wxT(" on RS ") + wxString::Format(wxT("%d"),RSiter->first) +
                     wxT(" has ") + wxString::Format(wxT("%d"),VMiter->second->size()) + wxT(" values but ") +
                     wxString::Format(wxT("%d"),ExpectedVarsCount) + wxT(" were expected");
        return false;
      }
      if (ExpectedVarsCount > 0 && isnan(VMiter->second->at(ExpectedVarsCount-1)))
      {
        (*Message) = wxT("variable ") + VMiter->first + wxT(" on RS ") + wxString::Format(wxT("%d"),RSiter->first) +
                     wxT(" is NaN");
        return false;
      }
    }


    // vector vars
    VectVarsMap = RSiter->second->getSimulatedVectorVars();
    VVMiter = VectVarsMap->begin();

    for(VVMiter = VectVarsMap->begin(); VVMiter != VectVarsMap->end(); ++VVMiter)
    {
      if (VVMiter->second->size() != ExpectedVarsCount)
      {
        (*Message) = wxT("variable ") + VVMiter->first + wxT("[] on RS ") + wxString::Format(wxT("%d"),RSiter->first) +
                             wxT(" has ") + wxString::Format(wxT("%d"),VVMiter->second->size()) + wxT(" values but ") +
                             wxString::Format(wxT("%d"),ExpectedVarsCount) + wxT(" were expected");
        return false;  // checks correct vars count
      }
    }


  }


  // checking GUs
  for(GUiter = GUsMap->begin(); GUiter != GUsMap->end(); ++GUiter)
  {
    VarsMap = GUiter->second->getSimulatedVars();
    VMiter = VarsMap->begin();

    for(VMiter = VarsMap->begin(); VMiter != VarsMap->end(); ++VMiter)
    {
      if (VMiter->second->size() != ExpectedVarsCount)
      {
        (*Message) = wxT("variable ") + VMiter->first + wxT(" on GU ") + wxString::Format(wxT("%d"),GUiter->first) +
                             wxT(" has ") + wxString::Format(wxT("%d"),VMiter->second->size()) + wxT(" values but ") +
                             wxString::Format(wxT("%d"),ExpectedVarsCount) + wxT(" were expected");
        return false;
      }
      if (ExpectedVarsCount > 0 && isnan(VMiter->second->at(ExpectedVarsCount-1)))
      {
        (*Message) = wxT("variable ") + VMiter->first + wxT(" on GU ") + wxString::Format(wxT("%d"),GUiter->first) +
                     wxT(" is NaN");
        return false;
      }

    }


    // vector vars
    VectVarsMap = GUiter->second->getSimulatedVectorVars();
    VVMiter = VectVarsMap->begin();

    for(VVMiter = VectVarsMap->begin(); VVMiter != VectVarsMap->end(); ++VVMiter)
    {
      if (VVMiter->second->size() != ExpectedVarsCount)
      {
        (*Message) = wxT("variable ") + VVMiter->first + wxT("[] on GU ") + wxString::Format(wxT("%d"),GUiter->first) +
                             wxT(" has ") + wxString::Format(wxT("%d"),VVMiter->second->size()) + wxT(" values but ") +
                             wxString::Format(wxT("%d"),ExpectedVarsCount) + wxT(" were expected");
        return false;  // checks correct vars count
      }
    }


  }


*/
  return true;

}

// =====================================================================
// =====================================================================

bool Engine::checkModelConsistency()
{

  bool IsOK = true;

  PluginsList::iterator FuncIter;
  openfluid::base::SignatureHandledData HData;
  PluginContainer* CurrentFunction;
  unsigned int i;


  /* Variables processing order is important
     A) first pass
        1) required vars
        2) produced vars
        3) updated vars
     B) second pass
        4) required vars at t-1+
  */

  FuncIter = m_Functions.begin();

  while (FuncIter != m_Functions.end() && IsOK)
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



  FuncIter = m_Functions.begin();

  while (FuncIter != m_Functions.end() && IsOK)
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


  return IsOK;
}

// =====================================================================
// =====================================================================



bool Engine::checkDataConsistency()
{

  bool IsOK = true;
  // TODO enable this (check data and var name against nomenclature)
/*
  PluginsList::Node *FuncNode = NULL;
  openfluid::base::SignatureHandledData HData;
  PluginContainer* CurrentFunction;

  int i;



  // check variable name against nomenclature
  if (mp_RunEnv->isCheckVarNames())
  {  std::list<FunctionConfig>::iterator FuncIt;

  FunctionConfig* FConf;
  PluginContainer* FuncToAdd;

  m_Functions.clear();


  // on each function
  for (FuncIt = m_ModelConfig.FuncConfigs.begin(); FuncIt != m_ModelConfig.FuncConfigs.end(); ++FuncIt)
  {
    FConf = &(*FuncIt);


    wxRegEx RegExp(wxT("[a-zA-Z,\\-]+\\.[a-zA-Z\\-]+\\.[a-zA-Z\\-\\#]+\\.[a-zA-Z\\-]+"));


    FuncNode = m_Functions.GetFirst();
    while (FuncNode && IsOK)
    {

      CurrentFunction = (PluginContainer*)FuncNode->GetData();
      if (CurrentFunction != NULL)
      {
        HData = CurrentFunction->Signature->HandledData;


        // produced vars
        i = 0;
        while (IsOK && i<HData.ProducedVars.size())
        {
          IsOK = RegExp.Matches(HData.ProducedVars[i].Name);

          if (!IsOK)
          {
            throw openfluid::base::OFException("kernel","Engine::checkDataConsistency",_S(HData.ProducedVars[i].Name) + " variable name does not match nomenclature");
            return false;
          }
          i++;
        }


        // required vars
        i = 0;
        while (IsOK && i<HData.RequiredVars.size())
        {
          IsOK = RegExp.Matches(HData.RequiredVars[i].Name);

          if (!IsOK)
          {
            throw openfluid::base::OFException("kernel","Engine::checkDataConsistency",_S(HData.RequiredVars[i].Name) + " variable name does not match nomenclature");
            return false;
          }
          i++;
        }


        // used vars
        i = 0;
        while (IsOK && i<HData.UsedVars.size())
        {
          IsOK = RegExp.Matches(HData.UsedVars[i].Name);

          if (!IsOK)
          {
            throw openfluid::base::OFException("kernel","Engine::checkDataConsistency",_S(HData.UsedVars[i].Name) + " variable name does not match nomenclature");
            return false;
          }
          i++;
        }

        // updated
        i = 0;
        while (IsOK && i<HData.UpdatedVars.size())
        {
          IsOK = RegExp.Matches(HData.UpdatedVars[i].Name);

          if (!IsOK)
          {
            throw openfluid::base::OFException("kernel","Engine::checkDataConsistency",_S(HData.UpdatedVars[i].Name) + " variable name does not match nomenclature");
            return false;
          }
          i++;
        }

        // required prev
        i = 0;
        while (IsOK && i<HData.RequiredPrevVars.size())
        {
          IsOK = RegExp.Matches(HData.RequiredPrevVars[i].Name);

          if (!IsOK)
          {
            throw openfluid::base::OFException("kernel","Engine::checkDataConsistency",_S(HData.RequiredPrevVars[i].Name) + " variable name does not match nomenclature");
            return false;
          }
          i++;
        }


        // used prev
        i = 0;
        while (IsOK && i<HData.UsedPrevVars.size())
        {
          IsOK = RegExp.Matches(HData.UsedPrevVars[i].Name);

          if (!IsOK)
          {
            throw openfluid::base::OFException("kernel","Engine::checkDataConsistency",_S(HData.UsedPrevVars[i].Name) + " variable name does not match nomenclature");
            return false;
          }
          i++;
        }



        FuncNode = FuncNode->GetNext();

      }
    }
  }
*/



  PluginsList::iterator FuncIter;
  openfluid::base::SignatureHandledData HData;
  PluginContainer* CurrentFunction;
  unsigned int i;


  FuncIter = m_Functions.begin();

  while (FuncIter != m_Functions.end() && IsOK)
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



  return IsOK;
}

// =====================================================================
// =====================================================================


bool Engine::checkExtraFilesConsistency()
{

  PluginsList::iterator FuncIt;
  openfluid::base::SignatureHandledData HData;
  PluginContainer* CurrentFunction;


  // on each function
  for (FuncIt = m_Functions.begin(); FuncIt != m_Functions.end(); ++FuncIt)
  {
    CurrentFunction = *FuncIt;

    HData = CurrentFunction->Signature->HandledData;

    for (unsigned int i=0;i<HData.RequiredExtraFiles.size();i++)
    {

      boost::filesystem::path ReqExtraFilePath(mp_RunEnv->getInputFullPath(HData.RequiredExtraFiles[i]));
      if (!boost::filesystem::exists(ReqExtraFilePath))
      {
        throw openfluid::base::OFException("kernel","Engine::checkExtraFilesConsistency","File " + HData.RequiredExtraFiles[i] + " required by " + CurrentFunction->Signature->ID + " not found");
        return false;
      }
    }


  }
  return true;
}

// =====================================================================
// =====================================================================



bool Engine::buildModel()
{
  /** \internal

    builds the model from the config file, calling:
    - loadModelConfig() from mp_IOMan
    - processConfig()
    - plugFunctions

  */

  try
  {
    return (mp_IOMan->loadModelConfig(&m_ModelConfig) && processConfig());
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


}


// =====================================================================
// =====================================================================

bool Engine::loadData()
{
  bool IsOK;

  IsOK =  mp_IOMan->loadRunConfig(&m_RunConfig);

  if (IsOK) IsOK = mp_IOMan->loadDomainFromFiles();

  if (IsOK) IsOK = mp_IOMan->loadInputDataFromFiles();

  if (IsOK) IsOK = mp_IOMan->loadEventsFromFiles();

  if (IsOK && mp_RunEnv->isWriteResults()) IsOK = IsOK && mp_IOMan->loadOutputConfig();

  return IsOK;

}


// =====================================================================
// =====================================================================

bool Engine::prepareDataAndCheckConsistency()
{

  bool IsOK = true;
  DECLARE_FUNCTION_PARSER;


  // check simulation functions count

  if (m_Functions.size() == 0)
  {
    throw openfluid::base::OFException("kernel","Engine::prepareDataAndCheckConsistency","No simulation function in model");
    return false;
  }

  // inits the simulation infos and status

  mp_SimStatus = new openfluid::base::SimulationStatus(m_RunConfig.BeginDate,
                                                      m_RunConfig.EndDate,
                                                      m_RunConfig.DeltaT);

  if (mp_RunEnv->isProgressiveOutput())
  {
    mp_MemMon->setPacketAndKeep(mp_RunEnv->getProgressiveOutputPacket(),mp_RunEnv->getProgressiveOutputKeep());
  }
  else
  {
    mp_MemMon->setPacketAndKeep(mp_SimStatus->getStepsCount(),1);
  }


  IsOK = checkModelConsistency();
  if (!IsOK)
  {
    return false;
  }



  IsOK = checkDataConsistency();
  if (!IsOK)
  {
    return false;
  }


  try
  {
    PARSE_FUNCTION_LIST_TWO(prepareData(),checkConsistency(),IsOK);
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


  IsOK = checkExtraFilesConsistency();
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
  DECLARE_FUNCTION_PARSER;

  std::string ProdMessage;
  openfluid::core::TimeStep_t SaveReleaseBegin, SaveReleaseEnd;


  // Check for simulation vars production before init
  // TODO enable this (vars production at before init)
  /*if (!checkSimulationVarsProduction(0, &ProdMessage))
  {
    throw openfluid::base::OFException("kernel","Engine::run","Wrong simulation variable production before simulation initilialization : " + std::string(ProdMessage.mb_str(wxConvUTF8)));
    return false;
  }*/

  // ============= initializeRun() =============

  if (!mp_RunEnv->isQuietRun())
  {
    std::cout << std::endl;
    std::cout << std::setw(16) << "Initialize...";
    std::cout.flush();
  }

  try
  {
    PARSE_FUNCTION_LIST(initializeRun((openfluid::base::SimulationStatus*)mp_SimStatus),IsOK);
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

  // TODO check this (vars production at after init)
  // check simulation vars production after init
  if (!checkSimulationVarsProduction(0,&ProdMessage))
  {
    throw openfluid::base::OFException("kernel","Engine::run","Wrong simulation variable production before simulation first step: " + ProdMessage);
    return false;
  }



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
      PARSE_FUNCTION_LIST(runStep(mp_SimStatus),IsOK);

      // check simulation vars production at each time step
      if (!checkSimulationVarsProduction(mp_SimStatus->getCurrentStep()+1,&ProdMessage))
      {
        throw openfluid::base::OFException("kernel","Engine::run",mp_SimStatus->getCurrentStep(),"Wrong simulation variable production : " + ProdMessage);
      }
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
    PARSE_FUNCTION_LIST(finalizeRun((openfluid::base::SimulationStatus*)mp_SimStatus),IsOK)
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
  if (!checkSimulationVarsProduction(mp_SimStatus->getCurrentStep()+1,&ProdMessage))
  {
    throw openfluid::base::OFException("kernel","Engine::run","Wrong simulation variable production during run finalization");
    return false;
  }

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


