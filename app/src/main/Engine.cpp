/**
  \file
  \brief implements ...

  \author Jean-Christophe FABRE <fabrejc@ensam.inra.fr>
*/


#include "Engine.h"
#include "AppTools.h"

#include <iostream>
#include <iomanip>

#include <math.h>

#include <wx/regex.h>

#include <wx/listimpl.cpp>
WX_DEFINE_LIST(PluginsList);


// =====================================================================
// =====================================================================

#define DECLARE_FUNCTION_PARSER \
    PluginsList::Node *_M_FuncNode = NULL;


#define PARSE_FUNCTION_LIST(calledmethod,statevar) \
    _M_FuncNode = m_Functions.GetFirst(); \
    while (_M_FuncNode && statevar) \
    { \
      PluginContainer* CurrentFunction = (PluginContainer*)_M_FuncNode->GetData(); \
      if (CurrentFunction != NULL) statevar = (statevar && CurrentFunction->Function->calledmethod); \
      _M_FuncNode = _M_FuncNode->GetNext(); \
    }

#define PARSE_FUNCTION_LIST_TWO(calledmethod1,calledmethod2,statevar) \
    _M_FuncNode = m_Functions.GetFirst(); \
    while (_M_FuncNode && statevar) \
    { \
      PluginContainer* CurrentFunction = (PluginContainer*)_M_FuncNode->GetData(); \
      if (CurrentFunction != NULL) statevar = (statevar && (CurrentFunction->Function->calledmethod1 && CurrentFunction->Function->calledmethod2)); \
      _M_FuncNode = _M_FuncNode->GetNext(); \
    }

#define PARSE_FUNCTION_LIST_FOUR(calledmethod1,calledmethod2,calledmethod3,calledmethod4,statevar) \
    _M_FuncNode = m_Functions.GetFirst(); \
    while (_M_FuncNode && statevar) \
    { \
      PluginContainer* CurrentFunction = (PluginContainer*)_M_FuncNode->GetData(); \
      if (CurrentFunction != NULL) statevar = (statevar && ((CurrentFunction->Function->calledmethod1 && CurrentFunction->Function->calledmethod2) && (CurrentFunction->Function->calledmethod3 && CurrentFunction->Function->calledmethod4))); \
      _M_FuncNode = _M_FuncNode->GetNext(); \
    }

// =====================================================================
// =====================================================================

// checks if var exists
#define CHECK_VAR(name,objects,objshashtype,vars,status) \
    objshashtype::iterator _M_it;\
    _M_it = objects->begin(); \
    while (status && (_M_it != objects->end()))\
    {\
      status = (!(_M_it->second->vars->find(name) == _M_it->second->vars->end()));\
      ++_M_it; \
    }\

// adds a new var, returns false status if already exits
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


// =====================================================================
// =====================================================================




Engine::Engine(mhydasdk::core::CoreRepository* CoreData, mhydasdk::base::ExecutionMessages* ExecMsgs,
               RuntimeEnvironment* RunEnv, PluginManager* PlugMan)
{

  mp_CoreData = CoreData;
  mp_ExecMsgs = ExecMsgs;
  mp_RunEnv = RunEnv;
  mp_PlugMan = PlugMan;


  m_Functions.Clear();

  mp_IOMan = new IOManager(mp_ExecMsgs,mp_RunEnv);

  m_ModelConfig.SimulationID = wxT("");
  
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
      - load and parametrize functions
  */


  FunctionConfigsList::Node *FuncNode = m_ModelConfig.FuncConfigs.GetFirst();
  FunctionConfig *FConf;
  PluginContainer* FuncToAdd;
  
  m_Functions.clear();



  while (FuncNode)
  {
    FConf = (FunctionConfig*)(FuncNode->GetData());
   
    FuncToAdd = mp_PlugMan->getPlugin(FConf->FileID,mhydasdk::base::SIMULATION,mp_CoreData);
    
    if (FuncToAdd != NULL)
    {
       if (FuncToAdd->Function->initParams(FConf->Params))
       {
         m_Functions.Append((PluginContainer**)FuncToAdd);
       }
       else
       {
         mp_ExecMsgs->setError(wxT("Engine"),wxT("Initializing params function error."));
         return false;
       }

    }
    else
    {
      mp_ExecMsgs->setError(wxT("Engine"),wxT("Loading function from plugin ") + FConf->FileID + wxT(" error."));
      return false;
    }

    FuncNode = FuncNode->GetNext();
  }


  return true;
}

// =====================================================================
// =====================================================================

bool Engine::checkSimulationVarsProduction(int ExpectedVarsCount, wxString* Message)
{
  
  mhydasdk::core::SurfaceUnit *SU;
  mhydasdk::core::SUMap *SUsMap = mp_CoreData->getSpatialData()->getSUsCollection();
  mhydasdk::core::SUMap::iterator SUiter;

  mhydasdk::core::ReachSegment *RS;
  mhydasdk::core::RSMap *RSsMap = mp_CoreData->getSpatialData()->getRSsCollection();
  mhydasdk::core::RSMap::iterator RSiter;
  
  mhydasdk::core::GroundwaterUnit *GU;
  mhydasdk::core::GUMap *GUsMap = mp_CoreData->getSpatialData()->getGUsCollection();
  mhydasdk::core::GUMap::iterator GUiter;
  
  mhydasdk::core::SimulatedVarsMap *VarsMap;
  mhydasdk::core::SimulatedVarsMap::iterator VMiter;
  
  mhydasdk::core::SimulatedVectorVarsMap *VectVarsMap;
  mhydasdk::core::SimulatedVectorVarsMap::iterator VVMiter;

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
  
  
  
  return true;
  
}

// =====================================================================
// =====================================================================

bool Engine::checkModelConsistency()
{

  PluginsList::Node *FuncNode = NULL;
  mhydasdk::base::SignatureHandledData HData;
  
  int i;
  bool IsOK = true; 
  
  
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
                       mp_CoreData->getSpatialData()->getSUsCollection(),mhydasdk::core::SUMap,
                       getSimulatedVectorVars(),IsOK);
          }  
          else 
          {
            CHECK_VAR(HData.RequiredVars[i].Name,
                      mp_CoreData->getSpatialData()->getSUsCollection(),mhydasdk::core::SUMap,
                      getSimulatedVars(),IsOK);            
          }
          
        }

        if (HData.RequiredVars[i].Distribution == wxT("RS"))
        {
          if (IsVectorNamedVariable(HData.RequiredVars[i].Name))
          {  
            CHECK_VAR(GetVectorNamedVariableName(HData.RequiredVars[i].Name),
                      mp_CoreData->getSpatialData()->getRSsCollection(),mhydasdk::core::RSMap,
                      getSimulatedVectorVars(),IsOK);
          }
          else
          {            
            CHECK_VAR(HData.RequiredVars[i].Name,
                       mp_CoreData->getSpatialData()->getRSsCollection(),mhydasdk::core::RSMap,
                       getSimulatedVars(),IsOK);
          }
        }

        if (HData.RequiredVars[i].Distribution == wxT("GU"))
        {
          if (IsVectorNamedVariable(HData.RequiredVars[i].Name))
          {  
            CHECK_VAR(GetVectorNamedVariableName(HData.RequiredVars[i].Name),
                      mp_CoreData->getSpatialData()->getGUsCollection(),mhydasdk::core::GUMap,
                      getSimulatedVectorVars(),IsOK);
          }
          else
          {
            CHECK_VAR(HData.RequiredVars[i].Name,
                      mp_CoreData->getSpatialData()->getGUsCollection(),mhydasdk::core::GUMap,
                      getSimulatedVars(),IsOK);
          }
        }
                
        if (!IsOK) mp_ExecMsgs->setError(wxT("Engine"),HData.RequiredVars[i].Name+wxT(" variable required by ") + CurrentFunction->Signature->ID + wxT(" is not previously created"));
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
                       mp_CoreData->getSpatialData()->getSUsCollection(),mhydasdk::core::SUMap,
                       getSimulatedVectorVars(),mhydasdk::core::SimulatedVectorVarsMap,
                       mhydasdk::core::SerieOfMHYDASVectorValue,IsOK);            
          }
          else
          {
            CREATE_VAR(HData.ProducedVars[i].Name,
                     mp_CoreData->getSpatialData()->getSUsCollection(),mhydasdk::core::SUMap,
                     getSimulatedVars(),mhydasdk::core::SimulatedVarsMap,
                     mhydasdk::core::SerieOfMHYDASScalarValue,IsOK);
            
          }
        }

        if (HData.ProducedVars[i].Distribution == wxT("RS"))
        {
          if (IsVectorNamedVariable(HData.ProducedVars[i].Name))
          {
            CREATE_VAR(GetVectorNamedVariableName(HData.ProducedVars[i].Name),
                       mp_CoreData->getSpatialData()->getRSsCollection(),mhydasdk::core::RSMap,
                       getSimulatedVectorVars(),mhydasdk::core::SimulatedVectorVarsMap,
                       mhydasdk::core::SerieOfMHYDASVectorValue,IsOK);            
          }
          else
          {          
            CREATE_VAR(HData.ProducedVars[i].Name,
                       mp_CoreData->getSpatialData()->getRSsCollection(),mhydasdk::core::RSMap,
                       getSimulatedVars(),mhydasdk::core::SimulatedVarsMap,
                       mhydasdk::core::SerieOfMHYDASScalarValue,IsOK);
          }
        }

        if (HData.ProducedVars[i].Distribution == wxT("GU"))
        {
          if (IsVectorNamedVariable(HData.ProducedVars[i].Name))
          {
            CREATE_VAR(GetVectorNamedVariableName(HData.ProducedVars[i].Name),
                       mp_CoreData->getSpatialData()->getGUsCollection(),mhydasdk::core::GUMap,
                       getSimulatedVectorVars(),mhydasdk::core::SimulatedVectorVarsMap,
                       mhydasdk::core::SerieOfMHYDASVectorValue,IsOK);                        
          }
          else
          {          
            CREATE_VAR(HData.ProducedVars[i].Name,
                       mp_CoreData->getSpatialData()->getGUsCollection(),mhydasdk::core::GUMap,
                       getSimulatedVars(),mhydasdk::core::SimulatedVarsMap,
                       mhydasdk::core::SerieOfMHYDASScalarValue,IsOK);
          }  
          
        }
       
        
        if (!IsOK) mp_ExecMsgs->setError(wxT("Engine"),HData.ProducedVars[i].Name+wxT(" variable produced by ") + CurrentFunction->Signature->ID + wxT(" cannot be created because it is previously created"));
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
                       mp_CoreData->getSpatialData()->getSUsCollection(),mhydasdk::core::SUMap,
                       getSimulatedVectorVars(),mhydasdk::core::SimulatedVectorVarsMap,
                       mhydasdk::core::SerieOfMHYDASVectorValue,IsOK);            
          }
          else
          {          
            UPDATE_VAR(HData.UpdatedVars[i].Name,
                       mp_CoreData->getSpatialData()->getSUsCollection(),mhydasdk::core::SUMap,
                       getSimulatedVars(),mhydasdk::core::SimulatedVarsMap,
                       mhydasdk::core::SerieOfMHYDASScalarValue,IsOK);
          }
        }

        if (HData.UpdatedVars[i].Distribution == wxT("RS"))
        {
          if (IsVectorNamedVariable(HData.UpdatedVars[i].Name))
          {
            UPDATE_VAR(GetVectorNamedVariableName(HData.UpdatedVars[i].Name),
                       mp_CoreData->getSpatialData()->getRSsCollection(),mhydasdk::core::RSMap,
                       getSimulatedVectorVars(),mhydasdk::core::SimulatedVectorVarsMap,
                       mhydasdk::core::SerieOfMHYDASVectorValue,IsOK);                        
          }
          else
          {          
          
            UPDATE_VAR(HData.UpdatedVars[i].Name,
                       mp_CoreData->getSpatialData()->getRSsCollection(),mhydasdk::core::RSMap,
                       getSimulatedVars(),mhydasdk::core::SimulatedVarsMap,
                       mhydasdk::core::SerieOfMHYDASScalarValue,IsOK);
          }
        }

        if (HData.UpdatedVars[i].Distribution == wxT("GU"))
        {
          if (IsVectorNamedVariable(HData.UpdatedVars[i].Name))
          {
            UPDATE_VAR(GetVectorNamedVariableName(HData.UpdatedVars[i].Name),
                       mp_CoreData->getSpatialData()->getGUsCollection(),mhydasdk::core::GUMap,
                       getSimulatedVectorVars(),mhydasdk::core::SimulatedVectorVarsMap,
                       mhydasdk::core::SerieOfMHYDASVectorValue,IsOK);                        
          }
          else
          {
            UPDATE_VAR(HData.UpdatedVars[i].Name,
                       mp_CoreData->getSpatialData()->getGUsCollection(),mhydasdk::core::GUMap,
                       getSimulatedVars(),mhydasdk::core::SimulatedVarsMap,
                       mhydasdk::core::SerieOfMHYDASScalarValue,IsOK);
          }
        }        
        
        if (!IsOK) mp_ExecMsgs->setError(wxT("Engine"),wxT("Problem handling of ")+HData.ProducedVars[i].Name+wxT(" updated variable declared by ") + CurrentFunction->Signature->ID);
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
                      mp_CoreData->getSpatialData()->getSUsCollection(),mhydasdk::core::SUMap,
                      getSimulatedVectorVars(),IsOK);
            
          }
          else
          {          
            CHECK_VAR(HData.RequiredPrevVars[i].Name,
                       mp_CoreData->getSpatialData()->getSUsCollection(),mhydasdk::core::SUMap,
                       getSimulatedVars(),IsOK);
          }          
        }

        if (HData.RequiredPrevVars[i].Distribution == wxT("RS"))
        {
          if (IsVectorNamedVariable(HData.RequiredPrevVars[i].Name))
          {
            CHECK_VAR(GetVectorNamedVariableName(HData.RequiredPrevVars[i].Name),
                      mp_CoreData->getSpatialData()->getRSsCollection(),mhydasdk::core::RSMap,
                      getSimulatedVectorVars(),IsOK);            
          }
          else
          {          
            CHECK_VAR(HData.RequiredPrevVars[i].Name,
                       mp_CoreData->getSpatialData()->getRSsCollection(),mhydasdk::core::RSMap,
                       getSimulatedVars(),IsOK);
          }  
          
        }

        if (HData.RequiredPrevVars[i].Distribution == wxT("GU"))
        {
          if (IsVectorNamedVariable(HData.RequiredPrevVars[i].Name))
          {
            CHECK_VAR(GetVectorNamedVariableName(HData.RequiredPrevVars[i].Name),
                      mp_CoreData->getSpatialData()->getGUsCollection(),mhydasdk::core::GUMap,
                      getSimulatedVectorVars(),IsOK);            
          }
          else
          {          
            CHECK_VAR(HData.RequiredPrevVars[i].Name,
                       mp_CoreData->getSpatialData()->getGUsCollection(),mhydasdk::core::GUMap,
                       getSimulatedVars(),IsOK);
          }  
          
        }
                
        if (!IsOK) mp_ExecMsgs->setError(wxT("Engine"),HData.RequiredPrevVars[i].Name+wxT(" variable required at previous step by ") + CurrentFunction->Signature->ID + wxT(" does not exist"));
        else i++;        
      }
    
    }

    FuncNode = FuncNode->GetNext();   
  }
  
  
  
  return IsOK;
}

// =====================================================================
// =====================================================================



bool Engine::checkDataConsistency()
{

  PluginsList::Node *FuncNode = NULL;
  mhydasdk::base::SignatureHandledData HData;
  PluginContainer* CurrentFunction;
  
  int i;
  bool IsOK = true; 
  
  
  
  
  
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
            mp_ExecMsgs->setError(wxT("Engine"),HData.ProducedVars[i].Name+wxT(" variable name does not match nomenclature."));
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
            mp_ExecMsgs->setError(wxT("Engine"),HData.RequiredVars[i].Name+wxT(" variable name does not match nomenclature."));
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
            mp_ExecMsgs->setError(wxT("Engine"),HData.UsedVars[i].Name+wxT(" variable name does not match nomenclature."));
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
            mp_ExecMsgs->setError(wxT("Engine"),HData.UpdatedVars[i].Name+wxT(" variable name does not match nomenclature."));
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
            mp_ExecMsgs->setError(wxT("Engine"),HData.RequiredPrevVars[i].Name+wxT(" variable name does not match nomenclature."));
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
            mp_ExecMsgs->setError(wxT("Engine"),HData.UsedPrevVars[i].Name+wxT(" variable name does not match nomenclature."));
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
                     mp_CoreData->getSpatialData()->getSUsCollection(),mhydasdk::core::SUMap,
                     getProperties(),IsOK);
          
        }

        if (HData.RequiredProps[i].Distribution == wxT("RS"))
        {
          CHECK_VAR(HData.RequiredProps[i].Name,
                     mp_CoreData->getSpatialData()->getRSsCollection(),mhydasdk::core::RSMap,
                     getProperties(),IsOK);
          
        }

        if (HData.RequiredProps[i].Distribution == wxT("GU"))
        {
          CHECK_VAR(HData.RequiredProps[i].Name,
                     mp_CoreData->getSpatialData()->getGUsCollection(),mhydasdk::core::GUMap,
                     getProperties(),IsOK);
          
        }

        
        if (!IsOK)
        {
          mp_ExecMsgs->setError(wxT("Engine"),HData.RequiredProps[i].Name+wxT(" distributed property required by ") + CurrentFunction->Signature->ID + wxT(" is missing"));
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
                mp_CoreData->getSpatialData()->getSUsCollection(),mhydasdk::core::SUMap,
                getIniConditions(),IsOK);          
          }

          if (IsOK && HData.RequiredIniconds[i].Distribution == wxT("RS"))
          {
            CHECK_VAR(HData.RequiredIniconds[i].Name,
                mp_CoreData->getSpatialData()->getRSsCollection(),mhydasdk::core::RSMap,
                getIniConditions (),IsOK);

          }

          if (IsOK && HData.RequiredIniconds[i].Distribution == wxT("GU"))
          {
            CHECK_VAR(HData.RequiredIniconds[i].Name,
                mp_CoreData->getSpatialData()->getGUsCollection(),mhydasdk::core::GUMap,
                getIniConditions(),IsOK);

          }


          if (!IsOK)
          {
            mp_ExecMsgs->setError(wxT("Engine"),HData.RequiredIniconds[i].Name+wxT(" distributed initial condition required by ") + CurrentFunction->Signature->ID + wxT(" is missing"));
            return false;
          }
          else i++;        
        }
      }

    }

    FuncNode = FuncNode->GetNext();   
  }
  
  
  
  return IsOK;
}

// =====================================================================
// =====================================================================


bool Engine::checkExtraFilesConsistency()
{
  
  PluginsList::Node *FuncNode = NULL;
  int i;
  mhydasdk::base::SignatureHandledData HData;
  
  
  

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
          mp_ExecMsgs->setError(wxT("Engine"),wxT("File ") + HData.RequiredExtraFiles[i] + wxT(" required by ") + CurrentFunction->Signature->ID + wxT(" not found"));
          return false;          
        }
      }
      
      
    }
    FuncNode = FuncNode->GetNext(); 
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

  return (mp_IOMan->loadModelConfig(&m_ModelConfig) && processConfig());
}


// =====================================================================
// =====================================================================

bool Engine::loadData()
{
  bool IsOK;

  IsOK =  mp_IOMan->loadRunConfig(&m_RunConfig);  
  
  if (IsOK) IsOK =  mp_IOMan->loadHydroObjects(mp_CoreData->getSpatialData());
  
  if (IsOK) IsOK = mp_IOMan->loadDistributedData(mp_CoreData->getSpatialData());
  
  if (IsOK) IsOK = mp_IOMan->loadDistributedEvents(mp_CoreData->getSpatialData());

  if (IsOK && mp_RunEnv->isWriteResults()) IsOK = IsOK && mp_IOMan->loadOutputConfig();
    
  return IsOK;
  
}


// =====================================================================
// =====================================================================

bool Engine::prepareDataAndCheckConsistency()
{

  bool IsOK = true;
  DECLARE_FUNCTION_PARSER;

  
  // builds topology by linking objects
  if (!mp_CoreData->getSpatialData()->buildObjectLinkedTopologyFromIDs())
  {
    mp_ExecMsgs->setError(wxT("Engine"),wxT("Topology rebuild error"));
    return false;
  }  


  // builds process orders lists
  if (!mp_CoreData->getSpatialData()->buildProcessOrders())
  {
    mp_ExecMsgs->setError(wxT("Engine"),wxT("Process orders build error"));
    return false;
  }  



  // check simulation functions count
  
  if (m_Functions.GetCount() == 0) 
  {
    mp_ExecMsgs->setError(wxT("Engine"),wxT("No simulation function"));
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

  
  PARSE_FUNCTION_LIST_TWO(prepareData(),checkConsistency(),IsOK);
 
    
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

    
  mp_SimStatus = new mhydasdk::base::SimulationStatus(m_RunConfig.BeginDate,
                                                      m_RunConfig.EndDate,
                                                      m_RunConfig.DeltaT);

  
  
  // preparation des donnees de simulation
  mp_CoreData->getSpatialData()->reserveSimulationVars(mp_SimStatus->getStepsCount());
 
  
  
  return true;
}


// =====================================================================
// =====================================================================


bool Engine::run()
{
  bool IsOK = true;
  DECLARE_FUNCTION_PARSER;
                                                      
  wxString ProdMessage;                                                      
  // Check for simulation vars production before init
  if (!checkSimulationVarsProduction(0, &ProdMessage))
  {
    mp_ExecMsgs->setError(wxT("Engine"),wxT("Wrong simulation variable production before run initialization : ")+ProdMessage);
    return false;      
  }
  
  
  PARSE_FUNCTION_LIST(initializeRun((mhydasdk::base::SimulationStatus*)mp_SimStatus),IsOK);

  if (mp_ExecMsgs->isErrorFlag())
  {
    return false;
  }

  // check simulation vars production after init
  if (!checkSimulationVarsProduction(0,&ProdMessage))
  {
    mp_ExecMsgs->setError(wxT("Engine"),wxT("Wrong simulation variable production before run initialization : ")+ProdMessage);
    return false;      
  }
  
  


  if (mp_RunEnv->isTraceMode()) mp_IOMan->prepareTraceDir(mp_CoreData);
  
  if (!mp_RunEnv->isQuietRun())
  {
    std::cout << std::endl;
    std::cout << std::setw(10) << "Time step";
    std::cout << std::setw(18) << "Real time";
    std::cout << std::setw(17) << "Status";
    std::cout << std::endl;
    std::cout << std::endl;
    std::cout.flush();
  }


  // run loop 
  
  do
  {
    if (!mp_RunEnv->isQuietRun())
    {
      std::cout << std::setw(8) << mp_SimStatus->getCurrentStep();
      std::cout << std::setw(25) << _C(mp_SimStatus->getCurrentTime().Format(wxT("%Y-%m-%d %H:%M:%S")));
      std::cout.flush();
    }  

    mp_ExecMsgs->resetWarningFlag();
   
    PARSE_FUNCTION_LIST(runStep(mp_SimStatus),IsOK);    

    // check simulation vars production at each time step
    if (!mp_ExecMsgs->isErrorFlag() && !checkSimulationVarsProduction(mp_SimStatus->getCurrentStep()+1,&ProdMessage))
    {
      mp_ExecMsgs->setError(wxT("Engine"),wxT("Wrong simulation variable production during run step ")+ wxString::Format(wxT("%d"),mp_SimStatus->getCurrentStep())+ wxT(" : ")+ProdMessage);
    }

    
    if (mp_ExecMsgs->isErrorFlag())
    {
      std::cout << std::setw(12) << "[Error]";
      std::cout << std::endl << std::endl;
      return false;
      
    }
    else
    {
      if (!mp_RunEnv->isQuietRun())
      {

        if (mp_ExecMsgs->isWarningFlag()) std::cout << std::setw(12) << "[Warning]";
        else std::cout << std::setw(12) << "[OK]";
      }  
    }

    if (!mp_RunEnv->isQuietRun())
    {
      std::cout << std::endl;
      std::cout.flush();
    }  

    
    if (mp_RunEnv->isTraceMode()) mp_IOMan->saveTrace(mp_CoreData,mp_SimStatus->getCurrentStep(), mp_SimStatus->getCurrentTime());
    
  } while (mp_SimStatus->switchToNextStep());


  std::cout << std::endl;

  // finalization of functions
  PARSE_FUNCTION_LIST(finalizeRun((mhydasdk::base::SimulationStatus*)mp_SimStatus),IsOK)  

  
  // check simulation vars production after finalize
  if (!checkSimulationVarsProduction(mp_SimStatus->getCurrentStep()+1,&ProdMessage))
  {
    mp_ExecMsgs->setError(wxT("Engine"),wxT("Wrong simulation variable production during run finalization"));
    return false;      
  }
  
  
  return IsOK;
}


// =====================================================================
// =====================================================================


bool Engine::saveResults(ExtraSimInfos ExSI)
{
  mp_ExecMsgs->resetWarningFlag();
  return (mp_IOMan->saveResults(mp_CoreData, m_RunConfig, mp_SimStatus->getStepsCount(),ExSI));
}

// =====================================================================
// =====================================================================

bool Engine::saveReports(ExtraSimInfos ExSI)
{
  mp_ExecMsgs->resetWarningFlag();
  return (mp_IOMan->saveSimulationInfos(mp_CoreData,ExSI,(mhydasdk::base::SimulationInfo*)mp_SimStatus));
}



// =====================================================================
// =====================================================================


