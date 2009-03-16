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

#include <wx/regex.h>

#include <wx/listimpl.cpp>




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

// checks if var exists
/*
            CHECK_VAR(GetVectorNamedVariableName(HData.RequiredVars[i].Name),
                       mp_CoreData->getSpatialData()->getSUsCollection(),openfluid::core::SUMap,
                       getSimulatedVectorVars(),IsOK);

*/
// TODO check that
#define DECLARE_CHECK_VAR \
    openfluid::core::UnitsList_t::const_iterator _M_CHV_UnitIter; \
    openfluid::core::UnitsList_t* _M_CHV_UnitList; \

// TODO check that
#define CHECK_VAR(varname,unitclass,status) \
    _M_CHV_UnitList = NULL; \
    if (mp_CoreData->isUnitsClassExists(unitclass)) _M_CHV_UnitList = mp_CoreData->getUnits(unitclass)->getList(); \
    _M_CHV_UnitIter = _M_CHV_UnitList->begin(); \
    while (status && (_M_CHV_UnitIter != _M_CHV_UnitList->end()))\
    {\
      if (IsVectorNamedVariable(varname) status = (*_M_CHV_UnitIter)->getVectorVariables()->IsVariableExist(GetVectorNamedVariableName(varname));\
      else status = (*_M_CHV_UnitIter)->getScalarVariables()->IsVariableExist(varname); \
      ++_M_CHV_UnitIter; \
    }\
/*
// adds a new var, returns false status if already exists
#define CREATE_VAR(name,objects,objshashtype,vars,varshashtype,datatype,status) \
    objshashtype::iterator _M_it;\
    _M_it = objects->begin(); \
    while (status && (_M_it != objects->end()))\
    {\
      status = (_M_it->second->vars->find(name) == _M_it->second->vars->end()); \
      ++_M_it; \
    }\
    if (status) \
    {\
      for(_M_it = objects->begin(); _M_it != objects->end(); ++_M_it ) \
      { \
        _M_it->second->vars->insert(varshashtype::value_type(name,new datatype)); \
      }\
    }\

// adds a new var if doesn't exist
#define UPDATE_VAR(name,objects,objshashtype,vars,varshashtype,datatype,status) \
    objshashtype::iterator _M_it;\
    _M_it = objects->begin(); \
    while (status && (_M_it != objects->end()))\
    {\
      if (_M_it->second->vars->find(name) == _M_it->second->vars->end()) \
      {\
        _M_it->second->vars->insert(varshashtype::value_type(name,new datatype)); \
      }\
      ++_M_it;\
    }\

*/
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

  m_ModelConfig.SimulationID = "";

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


//  FunctionConfigsList:: *FuncNode = m_ModelConfig.FuncConfigs.front();
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

bool Engine::checkSimulationVarsProduction(int ExpectedVarsCount, std::string* Message)
{
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

  DECLARE_CHECK_VAR;

  PluginsList::iterator FuncIter;
  openfluid::base::SignatureHandledData HData;

  // TODO complete or rewrite this

  FuncIter = m_Functions.begin();

  while (FuncIter != m_Functions.end() && IsOK)
  {




  }


  // TODO enable or rewrite this
  /*
  PluginsList::Node *FuncNode = NULL;
  openfluid::base::SignatureHandledData HData;

  int i;


  FuncNode = m_Functions.GetFirst();
  while (FuncNode && IsOK)
  {
    PluginContainer* CurrentFunction = (PluginContainer*)FuncNode->GetData();
    if (CurrentFunction != NULL)
    {

      HData = CurrentFunction->Signature->HandledData;

      // required vars
      i = 0;
      while (IsOK && i<HData.RequiredVars.size())
      {

        if (HData.RequiredVars[i].Distribution == wxT("SU"))
        {
          if (IsVectorNamedVariable(HData.RequiredVars[i].Name))
          {
            CHECK_VAR(GetVectorNamedVariableName(HData.RequiredVars[i].Name),
                       mp_CoreData->getSpatialData()->getSUsCollection(),openfluid::core::SUMap,
                       getSimulatedVectorVars(),IsOK);
          }
          else
          {
            CHECK_VAR(HData.RequiredVars[i].Name,
                      mp_CoreData->getSpatialData()->getSUsCollection(),openfluid::core::SUMap,
                      getSimulatedVars(),IsOK);
          }

        }

        if (HData.RequiredVars[i].Distribution == wxT("RS"))
        {
          if (IsVectorNamedVariable(HData.RequiredVars[i].Name))
          {
            CHECK_VAR(GetVectorNamedVariableName(HData.RequiredVars[i].Name),
                      mp_CoreData->getSpatialData()->getRSsCollection(),openfluid::core::RSMap,
                      getSimulatedVectorVars(),IsOK);
          }
          else
          {
            CHECK_VAR(HData.RequiredVars[i].Name,
                       mp_CoreData->getSpatialData()->getRSsCollection(),openfluid::core::RSMap,
                       getSimulatedVars(),IsOK);
          }
        }

        if (HData.RequiredVars[i].Distribution == wxT("GU"))
        {
          if (IsVectorNamedVariable(HData.RequiredVars[i].Name))
          {
            CHECK_VAR(GetVectorNamedVariableName(HData.RequiredVars[i].Name),
                      mp_CoreData->getSpatialData()->getGUsCollection(),openfluid::core::GUMap,
                      getSimulatedVectorVars(),IsOK);
          }
          else
          {
            CHECK_VAR(HData.RequiredVars[i].Name,
                      mp_CoreData->getSpatialData()->getGUsCollection(),openfluid::core::GUMap,
                      getSimulatedVars(),IsOK);
          }
        }

        if (!IsOK) throw openfluid::base::OFException("kernel","Engine::checkModelConsistency",_S(HData.RequiredVars[i].Name) + " variable required by " + _S(CurrentFunction->Signature->ID) + " is not previously created");
        else i++;
      }


      // produced vars
      i = 0;
      while (IsOK && i<HData.ProducedVars.size())
      {


        if (HData.ProducedVars[i].Distribution == wxT("SU"))
        {
          if (IsVectorNamedVariable(HData.ProducedVars[i].Name))
          {
            CREATE_VAR(GetVectorNamedVariableName(HData.ProducedVars[i].Name),
                       mp_CoreData->getSpatialData()->getSUsCollection(),openfluid::core::SUMap,
                       getSimulatedVectorVars(),openfluid::core::SimulatedVectorVarsMap,
                       openfluid::core::SerieOfVectorValue,IsOK);
          }
          else
          {
            CREATE_VAR(HData.ProducedVars[i].Name,
                     mp_CoreData->getSpatialData()->getSUsCollection(),openfluid::core::SUMap,
                     getSimulatedVars(),openfluid::core::SimulatedVarsMap,
                     openfluid::core::SerieOfScalarValue,IsOK);

          }
        }

        if (HData.ProducedVars[i].Distribution == wxT("RS"))
        {
          if (IsVectorNamedVariable(HData.ProducedVars[i].Name))
          {
            CREATE_VAR(GetVectorNamedVariableName(HData.ProducedVars[i].Name),
                       mp_CoreData->getSpatialData()->getRSsCollection(),openfluid::core::RSMap,
                       getSimulatedVectorVars(),openfluid::core::SimulatedVectorVarsMap,
                       openfluid::core::SerieOfVectorValue,IsOK);
          }
          else
          {
            CREATE_VAR(HData.ProducedVars[i].Name,
                       mp_CoreData->getSpatialData()->getRSsCollection(),openfluid::core::RSMap,
                       getSimulatedVars(),openfluid::core::SimulatedVarsMap,
                       openfluid::core::SerieOfScalarValue,IsOK);
          }
        }

        if (HData.ProducedVars[i].Distribution == wxT("GU"))
        {
          if (IsVectorNamedVariable(HData.ProducedVars[i].Name))
          {
            CREATE_VAR(GetVectorNamedVariableName(HData.ProducedVars[i].Name),
                       mp_CoreData->getSpatialData()->getGUsCollection(),openfluid::core::GUMap,
                       getSimulatedVectorVars(),openfluid::core::SimulatedVectorVarsMap,
                       openfluid::core::SerieOfVectorValue,IsOK);
          }
          else
          {
            CREATE_VAR(HData.ProducedVars[i].Name,
                       mp_CoreData->getSpatialData()->getGUsCollection(),openfluid::core::GUMap,
                       getSimulatedVars(),openfluid::core::SimulatedVarsMap,
                       openfluid::core::SerieOfScalarValue,IsOK);
          }

        }


        if (!IsOK) throw openfluid::base::OFException("kernel","Engine::checkModelConsistency",_S(HData.ProducedVars[i].Name) + " variable produced by " + _S(CurrentFunction->Signature->ID) + " cannot be created because it is previously created");
        else i++;
      }


      // updated vars
      i = 0;
      while (IsOK && i<HData.UpdatedVars.size())
      {

        if (HData.UpdatedVars[i].Distribution == wxT("SU"))
        {
          if (IsVectorNamedVariable(HData.UpdatedVars[i].Name))
          {
            UPDATE_VAR(GetVectorNamedVariableName(HData.UpdatedVars[i].Name),
                       mp_CoreData->getSpatialData()->getSUsCollection(),openfluid::core::SUMap,
                       getSimulatedVectorVars(),openfluid::core::SimulatedVectorVarsMap,
                       openfluid::core::SerieOfVectorValue,IsOK);
          }
          else
          {
            UPDATE_VAR(HData.UpdatedVars[i].Name,
                       mp_CoreData->getSpatialData()->getSUsCollection(),openfluid::core::SUMap,
                       getSimulatedVars(),openfluid::core::SimulatedVarsMap,
                       openfluid::core::SerieOfScalarValue,IsOK);
          }
        }

        if (HData.UpdatedVars[i].Distribution == wxT("RS"))
        {
          if (IsVectorNamedVariable(HData.UpdatedVars[i].Name))
          {
            UPDATE_VAR(GetVectorNamedVariableName(HData.UpdatedVars[i].Name),
                       mp_CoreData->getSpatialData()->getRSsCollection(),openfluid::core::RSMap,
                       getSimulatedVectorVars(),openfluid::core::SimulatedVectorVarsMap,
                       openfluid::core::SerieOfVectorValue,IsOK);
          }
          else
          {

            UPDATE_VAR(HData.UpdatedVars[i].Name,
                       mp_CoreData->getSpatialData()->getRSsCollection(),openfluid::core::RSMap,
                       getSimulatedVars(),openfluid::core::SimulatedVarsMap,
                       openfluid::core::SerieOfScalarValue,IsOK);
          }
        }

        if (HData.UpdatedVars[i].Distribution == wxT("GU"))
        {
          if (IsVectorNamedVariable(HData.UpdatedVars[i].Name))
          {
            UPDATE_VAR(GetVectorNamedVariableName(HData.UpdatedVars[i].Name),
                       mp_CoreData->getSpatialData()->getGUsCollection(),openfluid::core::GUMap,
                       getSimulatedVectorVars(),openfluid::core::SimulatedVectorVarsMap,
                       openfluid::core::SerieOfVectorValue,IsOK);
          }
          else
          {
            UPDATE_VAR(HData.UpdatedVars[i].Name,
                       mp_CoreData->getSpatialData()->getGUsCollection(),openfluid::core::GUMap,
                       getSimulatedVars(),openfluid::core::SimulatedVarsMap,
                       openfluid::core::SerieOfScalarValue,IsOK);
          }
        }

        if (!IsOK) throw openfluid::base::OFException("kernel","Engine::checkModelConsistency","Problem handling of " + _S(HData.ProducedVars[i].Name) + " updated variable declared by " + _S(CurrentFunction->Signature->ID));
        else i++;
      }

    }

    FuncNode = FuncNode->GetNext();
  }



  // prev vars

  FuncNode = m_Functions.GetFirst();
  while (FuncNode && IsOK)
  {
    PluginContainer* CurrentFunction = (PluginContainer*)FuncNode->GetData();
    if (CurrentFunction != NULL)
    {
      HData = CurrentFunction->Signature->HandledData;

      // required vars
      i = 0;
      while (IsOK && i<HData.RequiredPrevVars.size())
      {

        if (HData.RequiredPrevVars[i].Distribution == wxT("SU"))
        {
          if (IsVectorNamedVariable(HData.RequiredPrevVars[i].Name))
          {
            CHECK_VAR(GetVectorNamedVariableName(HData.RequiredPrevVars[i].Name),
                      mp_CoreData->getSpatialData()->getSUsCollection(),openfluid::core::SUMap,
                      getSimulatedVectorVars(),IsOK);

          }
          else
          {
            CHECK_VAR(HData.RequiredPrevVars[i].Name,
                       mp_CoreData->getSpatialData()->getSUsCollection(),openfluid::core::SUMap,
                       getSimulatedVars(),IsOK);
          }
        }

        if (HData.RequiredPrevVars[i].Distribution == wxT("RS"))
        {
          if (IsVectorNamedVariable(HData.RequiredPrevVars[i].Name))
          {
            CHECK_VAR(GetVectorNamedVariableName(HData.RequiredPrevVars[i].Name),
                      mp_CoreData->getSpatialData()->getRSsCollection(),openfluid::core::RSMap,
                      getSimulatedVectorVars(),IsOK);
          }
          else
          {
            CHECK_VAR(HData.RequiredPrevVars[i].Name,
                       mp_CoreData->getSpatialData()->getRSsCollection(),openfluid::core::RSMap,
                       getSimulatedVars(),IsOK);
          }

        }

        if (HData.RequiredPrevVars[i].Distribution == wxT("GU"))
        {
          if (IsVectorNamedVariable(HData.RequiredPrevVars[i].Name))
          {
            CHECK_VAR(GetVectorNamedVariableName(HData.RequiredPrevVars[i].Name),
                      mp_CoreData->getSpatialData()->getGUsCollection(),openfluid::core::GUMap,
                      getSimulatedVectorVars(),IsOK);
          }
          else
          {
            CHECK_VAR(HData.RequiredPrevVars[i].Name,
                       mp_CoreData->getSpatialData()->getGUsCollection(),openfluid::core::GUMap,
                       getSimulatedVars(),IsOK);
          }

        }

        if (!IsOK) throw openfluid::base::OFException("kernel","Engine::checkModelConsistency",_S(HData.RequiredPrevVars[i].Name) + " variable required at previous step by " + _S(CurrentFunction->Signature->ID) + " does not exist");
        else i++;
      }

    }

    FuncNode = FuncNode->GetNext();
  }

*/

  return IsOK;
}

// =====================================================================
// =====================================================================



bool Engine::checkDataConsistency()
{

  bool IsOK = true;
  // TODO enable this
/*
  PluginsList::Node *FuncNode = NULL;
  openfluid::base::SignatureHandledData HData;
  PluginContainer* CurrentFunction;

  int i;



  // check variable name against nomenclature
  if (mp_RunEnv->isCheckVarNames())
  {

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





  // check data consistency
  FuncNode = m_Functions.GetFirst();
  while (FuncNode && IsOK)
  {
    CurrentFunction = (PluginContainer*)FuncNode->GetData();
    if (CurrentFunction != NULL)
    {
      HData = CurrentFunction->Signature->HandledData;

      // required props
      i = 0;
      while (IsOK && i<HData.RequiredProps.size())
      {

        if (HData.RequiredProps[i].Distribution == wxT("SU"))
        {
          CHECK_VAR(HData.RequiredProps[i].Name,
                     mp_CoreData->getSpatialData()->getSUsCollection(),openfluid::core::SUMap,
                     getProperties(),IsOK);

        }

        if (HData.RequiredProps[i].Distribution == wxT("RS"))
        {
          CHECK_VAR(HData.RequiredProps[i].Name,
                     mp_CoreData->getSpatialData()->getRSsCollection(),openfluid::core::RSMap,
                     getProperties(),IsOK);

        }

        if (HData.RequiredProps[i].Distribution == wxT("GU"))
        {
          CHECK_VAR(HData.RequiredProps[i].Name,
                     mp_CoreData->getSpatialData()->getGUsCollection(),openfluid::core::GUMap,
                     getProperties(),IsOK);

        }


        if (!IsOK)
        {
          throw openfluid::base::OFException("kernel","Engine::checkDataConsistency",_S(HData.RequiredProps[i].Name) + " distributed property required by " + _S(CurrentFunction->Signature->ID) + " is missing");
          return false;
        }
        else i++;
      }


      // required initial conditions
      i = 0;
      if (IsOK)
      {
        while (IsOK && i<HData.RequiredIniconds.size())
        {

          if (IsOK && HData.RequiredIniconds[i].Distribution == wxT("SU"))
          {
            CHECK_VAR(HData.RequiredIniconds[i].Name,
                mp_CoreData->getSpatialData()->getSUsCollection(),openfluid::core::SUMap,
                getIniConditions(),IsOK);
          }

          if (IsOK && HData.RequiredIniconds[i].Distribution == wxT("RS"))
          {
            CHECK_VAR(HData.RequiredIniconds[i].Name,
                mp_CoreData->getSpatialData()->getRSsCollection(),openfluid::core::RSMap,
                getIniConditions (),IsOK);

          }

          if (IsOK && HData.RequiredIniconds[i].Distribution == wxT("GU"))
          {
            CHECK_VAR(HData.RequiredIniconds[i].Name,
                mp_CoreData->getSpatialData()->getGUsCollection(),openfluid::core::GUMap,
                getIniConditions(),IsOK);

          }


          if (!IsOK)
          {
            throw openfluid::base::OFException("kernel","Engine::checkDataConsistency",_S(HData.RequiredIniconds[i].Name) + " distributed initial condition required by " + _S(CurrentFunction->Signature->ID) + " is missing");
            return false;
          }
          else i++;
        }
      }

    }

    FuncNode = FuncNode->GetNext();
  }
*/


  return IsOK;
}

// =====================================================================
// =====================================================================


bool Engine::checkExtraFilesConsistency()
{
  // TODO enable this

  /*
  PluginsList::Node *FuncNode = NULL;
  int i;
  openfluid::base::SignatureHandledData HData;




  FuncNode = m_Functions.GetFirst();
  while (FuncNode)
  {
    PluginContainer* CurrentFunction = (PluginContainer*)FuncNode->GetData();
    if (CurrentFunction != NULL)
    {
      HData = CurrentFunction->Signature->HandledData;

      for (i=0;i<HData.RequiredExtraFiles.size();i++)
      {
        if (!wxFileExists(mp_RunEnv->getInputFullPath(HData.RequiredExtraFiles[i])))
        {
          throw openfluid::base::OFException("kernel","Engine::checkExtraFilesConsistency","File " + _S(HData.RequiredExtraFiles[i]) + " required by " + _S(CurrentFunction->Signature->ID) + " not found");
          return false;
        }
      }


    }
    FuncNode = FuncNode->GetNext();
  }


*/
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

  // TODO check if complete

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
  // TODO check that
  DECLARE_FUNCTION_PARSER;



  // builds topology by linking objects
  // TODO check if necessary, i think no
/*
  if (!mp_CoreData->getSpatialData()->buildObjectLinkedTopologyFromIDs())
  {
    throw openfluid::base::OFException("kernel","Engine::prepareDataAndCheckConsistency","Topology rebuild error");
    return false;
  }
*/

  // builds process orders lists
// TODO check if necessary, i think no
  /*
  if (!mp_CoreData->getSpatialData()->buildProcessOrders())
  {
    throw openfluid::base::OFException("kernel","Engine::prepareDataAndCheckConsistency","Process orders build error");
    return false;
  }
*/

  // check simulation functions count

  if (m_Functions.size() == 0)
  {
    throw openfluid::base::OFException("kernel","Engine::prepareDataAndCheckConsistency","No simulation function in model");
    return false;
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
    // TODO check that
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



  // inits the simulation infos and status


  mp_SimStatus = new openfluid::base::SimulationStatus(m_RunConfig.BeginDate,
                                                      m_RunConfig.EndDate,
                                                      m_RunConfig.DeltaT);



  // preparation des donnees de simulation
  // TODO check that
  //mp_CoreData->getSpatialData()->reserveSimulationVars(mp_SimStatus->getStepsCount());

  // TODO check that
  //
  /*if (mp_RunEnv->isTraceMode())
  {
    if (!mp_IOMan->prepareTraceDir(mp_CoreData)) return false;
  }*/

  if (mp_RunEnv->isWriteResults())
  {
    if (!mp_IOMan->prepareOutputDir()) return false;
  }


  return true;
}


// =====================================================================
// =====================================================================


bool Engine::run()
{
  bool IsOK = true;
  // TODO check that
  DECLARE_FUNCTION_PARSER;

  std::string ProdMessage;
  // Check for simulation vars production before init
  // TODO enable this
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
    // TODO check that
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

  // TODO check that
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
      // TODO check that
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


    // TODO check this (progressive output)
    // progressive output
    if (mp_MemMon->isMemReleaseStep(mp_SimStatus->getCurrentStep()))
    {
      std::cout << std::endl << "        ---- Saving to disk and releasing memory ";
      //mp_IOMan->saveOutputs(CurrentStep,false);
      //mp_CoreData->doMemRelease(CurrentStep,false);
      mp_MemMon->setLastMemoryRelease(mp_SimStatus->getCurrentStep());
      std::cout << "[OK] ----" << std::endl << std::endl ;
    }


    // TODO check that
    //if (mp_RunEnv->isTraceMode()) mp_IOMan->saveTrace(mp_CoreData,mp_SimStatus->getCurrentStep(), mp_SimStatus->getCurrentTime());

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

    // TODO check that
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


  // TODO final progressive output
  // -1 because of the previous loop, index exceeds range
  //IOMan->saveOutputs(CurrentStep-1,true);
  //TheRepos->doMemRelease(CurrentStep-1,true);



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

bool Engine::saveReports(ExtraSimInfos ExSI, std::string ErrorMsg)
{
  mp_ExecMsgs->resetWarningFlag();
  return (mp_IOMan->saveSimulationInfos(ExSI,(openfluid::base::SimulationInfo*)mp_SimStatus,ErrorMsg));
}



// =====================================================================
// =====================================================================


