/**
  \file PlugFunction.cpp
  \brief implementation file for generic pluggable function interface definition

  \author Jean-Christophe FABRE <fabrejc@ensam.inra.fr>
*/

#include <iostream>

#include "PlugFunction.h"


// =====================================================================
// =====================================================================

// checks if var exists
#define CHECK_VAR(name,objects,objshashtype,vars,status) \
    objshashtype::iterator _M_it;\
    _M_it = objects->begin(); \
    while (status && (_M_it != objects->end()))\
    {\
      status = (!(_M_it->second->vars->find(name) == _M_it->second->vars->end())); \
      ++_M_it; \
    }\

// adds a new var, returns false status if already exits
#define CREATE_VAR(name,objects,objshashtype,vars,varshashtype,status) \
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
        _M_it->second->vars->insert(varshashtype::value_type(name,new std::vector<double>)); \
      } \
    }

// adds a new var if doesn't exist
#define UPDATE_VAR(name,objects,objshashtype,vars,varshashtype,status) \
    objshashtype::iterator _M_it;\
    _M_it = objects->begin(); \
    while (status && (_M_it != objects->end()))\
    {\
      if (_M_it->second->vars->find(name) == _M_it->second->vars->end()) \
      {\
        _M_it->second->vars->insert(varshashtype::value_type(name,new std::vector<double>)); \
      }\
      ++_M_it; \
    }\


// =====================================================================
// =====================================================================




namespace mhydasdk { namespace base {



PluggableFunction::PluggableFunction()

{

  mp_CoreData = NULL;

  mp_ExecMsgs = NULL;

  mp_Signature = new mhydasdk::base::Signature();

  m_SUVarsToCreate.Clear();
  m_SUVarsToUpdate.Clear();
  m_SUVarsToCheck.Clear();
  m_SUInicondsToCheck.Clear();
  m_SUPropsToCheck.Clear();

  m_RSVarsToCreate.Clear();
  m_SUVarsToUpdate.Clear();
  m_RSVarsToCheck.Clear();
  m_RSInicondsToCheck.Clear();
  m_RSPropsToCheck.Clear();

  m_GUVarsToCreate.Clear();
  m_GUVarsToUpdate.Clear();
  m_GUVarsToCheck.Clear();
  m_GUInicondsToCheck.Clear();
  m_GUPropsToCheck.Clear();

}


// =====================================================================
// =====================================================================


bool PluggableFunction::prepareFunctionData()
{

  int i;
  bool IsOK = true;

  // ============ simulated vars to create ===============

  // create SUs vars
  i = 0;
  while (IsOK && i<m_SUVarsToCreate.GetCount())
  {
    CREATE_VAR(m_SUVarsToCreate[i],
               mp_CoreData->getSpatialData()->getSUsCollection(),mhydasdk::core::SUMap,
               getSimulatedVars(),mhydasdk::core::SimulatedVarsMap,IsOK);
    i++;
    if (!IsOK) mp_ExecMsgs->setError(mp_Signature->ID + wxT(" function, consistency checking"),-1,wxT("cannot create SU variable ") + m_SUVarsToCreate[i-1] + wxT(", it is previously produced"));
  }



  // create RSs vars
  i = 0;
  while (IsOK && i<m_RSVarsToCreate.GetCount())
  {
    CREATE_VAR(m_RSVarsToCreate[i],
               mp_CoreData->getSpatialData()->getRSsCollection(),mhydasdk::core::RSMap,
               getSimulatedVars(),mhydasdk::core::SimulatedVarsMap,IsOK);
    i++;
    if (!IsOK) mp_ExecMsgs->setError(mp_Signature->ID + wxT(" function, consistency checking"),-1,wxT("cannot create RS variable ") + m_RSVarsToCreate[i-1] + wxT(", it is previously produced"));
  }




  // create GUs vars
  i = 0;
  while (IsOK && i<m_GUVarsToCreate.GetCount())
  {
    CREATE_VAR(m_GUVarsToCreate[i],
               mp_CoreData->getSpatialData()->getGUsCollection(),mhydasdk::core::GUMap,
               getSimulatedVars(),mhydasdk::core::SimulatedVarsMap,IsOK);
    i++;
    if (!IsOK) mp_ExecMsgs->setError(mp_Signature->ID + wxT(" function, consistency checking"),-1,wxT("cannot create GU variable ") + m_GUVarsToCreate[i-1] + wxT(", it is previously produced"));
  }



  // ============ simulated vars to update ===============

  // update SUs vars
  i = 0;
  while (IsOK && i<m_SUVarsToUpdate.GetCount())
  {
    UPDATE_VAR(m_SUVarsToUpdate[i],
               mp_CoreData->getSpatialData()->getSUsCollection(),mhydasdk::core::SUMap,
               getSimulatedVars(),mhydasdk::core::SimulatedVarsMap,IsOK);
    i++;
  }


  // update RSs vars
  i = 0;
  while (IsOK && i<m_RSVarsToUpdate.GetCount())
  {
    UPDATE_VAR(m_RSVarsToUpdate[i],
               mp_CoreData->getSpatialData()->getRSsCollection(),mhydasdk::core::RSMap,
               getSimulatedVars(),mhydasdk::core::SimulatedVarsMap,IsOK);
    i++;
  }


  // update GUs vars
  i = 0;
  while (IsOK && i<m_GUVarsToUpdate.GetCount())
  {
    UPDATE_VAR(m_GUVarsToUpdate[i],
               mp_CoreData->getSpatialData()->getGUsCollection(),mhydasdk::core::GUMap,
               getSimulatedVars(),mhydasdk::core::SimulatedVarsMap,IsOK);
    i++;
  }


  return IsOK;
}




// =====================================================================
// =====================================================================

bool PluggableFunction::checkFunctionConsistency()
{

  int i;
  bool IsOK = true;


  // ============= Simulated vars =============

  // check SUs vars
  i = 0;
  while (IsOK && i<m_SUVarsToCheck.GetCount())
  {
    CHECK_VAR(m_SUVarsToCheck[i],
              mp_CoreData->getSpatialData()->getSUsCollection(),mhydasdk::core::SUMap,
              getSimulatedVars(),IsOK);
    i++;
    if (!IsOK) mp_ExecMsgs->setError(mp_Signature->ID + wxT(" function, consistency checking"),-1,wxT("requested SU variable ") + m_SUVarsToCheck[i-1] + wxT(" is not previously produced"));    
  }
  
  
  
  
  // check RSs vars
  i = 0;
  while (IsOK && i<m_RSVarsToCheck.GetCount())
  {
    CHECK_VAR(m_RSVarsToCheck[i],
              mp_CoreData->getSpatialData()->getRSsCollection(),mhydasdk::core::RSMap,
              getSimulatedVars(),IsOK);
    i++;
    if (!IsOK) mp_ExecMsgs->setError(mp_Signature->ID + wxT(" function, consistency checking"),-1,wxT("requested RS variable ") + m_RSVarsToCheck[i-1] + wxT(" is not previously produced"));
  }



  // check GUs vars
  i = 0;
  while (IsOK && i<m_GUVarsToCheck.GetCount())
  {
    CHECK_VAR(m_GUVarsToCheck[i],
              mp_CoreData->getSpatialData()->getGUsCollection(),mhydasdk::core::GUMap,
              getSimulatedVars(),IsOK);
    i++;
    if (!IsOK) mp_ExecMsgs->setError(mp_Signature->ID + wxT(" function, consistency checking"),-1,wxT("requested GU variable ") + m_GUVarsToCheck[i-1] + wxT(" is not previously produced"));    
  }
 

  // ============= Simulated properties =============

  // check SUs props
  i = 0;
  while (IsOK && i<m_SUPropsToCheck.GetCount())
  {
    CHECK_VAR(m_SUPropsToCheck[i],
              mp_CoreData->getSpatialData()->getSUsCollection(),mhydasdk::core::SUMap,
              getProperties(),IsOK);
    i++;
    if (!IsOK) mp_ExecMsgs->setError(mp_Signature->ID + wxT(" function, consistency checking"),-1,wxT("requested SU property ") + m_SUPropsToCheck[i-1] + wxT(" not found"));    
  }



  // check RSs props
  i = 0;
  while (IsOK && i<m_RSPropsToCheck.GetCount())
  {
    CHECK_VAR(m_RSPropsToCheck[i],
              mp_CoreData->getSpatialData()->getRSsCollection(),mhydasdk::core::RSMap,
              getProperties(),IsOK);
    i++;
    if (!IsOK) mp_ExecMsgs->setError(mp_Signature->ID + wxT(" function, consistency checking"),-1,wxT("requested RS property ") + m_RSPropsToCheck[i-1] + wxT(" not found"));    
  }



  // check GUs props
  i = 0;
  while (IsOK && i<m_GUPropsToCheck.GetCount())
  {
    CHECK_VAR(m_GUPropsToCheck[i],
              mp_CoreData->getSpatialData()->getGUsCollection(),mhydasdk::core::GUMap,
              getProperties(),IsOK);
    i++;
  }

  // ============= initial conditions =============

  // check SUs iniconds
  i = 0;
  while (IsOK && i<m_SUInicondsToCheck.GetCount())
  {
    CHECK_VAR(m_SUInicondsToCheck[i],
              mp_CoreData->getSpatialData()->getSUsCollection(),mhydasdk::core::SUMap,
              getIniConditions(),IsOK);
    i++;
    if (!IsOK) mp_ExecMsgs->setError(mp_Signature->ID + wxT(" function, consistency checking"),-1,wxT("requested SU initial condition ") + m_SUInicondsToCheck[i-1] + wxT(" not found"));    
  }



  // check RSs iniconds
  i = 0;
  while (IsOK && i<m_RSInicondsToCheck.GetCount())
  {
    CHECK_VAR(m_RSInicondsToCheck[i],
              mp_CoreData->getSpatialData()->getRSsCollection(),mhydasdk::core::RSMap,
              getIniConditions(),IsOK);
    i++;
    if (!IsOK) mp_ExecMsgs->setError(mp_Signature->ID + wxT(" function, consistency checking"),-1,wxT("requested RS initial condition ") + m_RSInicondsToCheck[i-1] + wxT(" not found"));

  }



  // check GUs iniconds
  i = 0;
  while (IsOK && i<m_GUInicondsToCheck.GetCount())
  {
    CHECK_VAR(m_GUInicondsToCheck[i],
              mp_CoreData->getSpatialData()->getGUsCollection(),mhydasdk::core::GUMap,
              getIniConditions(),IsOK);
    i++;
    if (!IsOK) mp_ExecMsgs->setError(mp_Signature->ID + wxT(" function, consistency checking"),-1,wxT("requested GU initial condition ") + m_GUInicondsToCheck[i-1] + wxT(" not found"));
  }



  return IsOK;
}

// =====================================================================
// =====================================================================


bool PluggableFunction::MHYDAS_GetDistributedVarValue(mhydasdk::core::HydroObject *HO, wxString VarName, int Step, float *Value)
{
  if (HO != NULL)  
  {
    mhydasdk::core::SimulatedVarsMap::iterator it; 
    it = HO->getSimulatedVars()->find(VarName);
    
    if (it != HO->getSimulatedVars()->end())
    {    
      mhydasdk::core::VectorOfDouble* ValuesVect  = it->second;
      
      if (Step < ValuesVect->size())
      {
        *Value = ValuesVect->at(Step);
        return true;
      }
      else return false;  
    }
    else return false;  
  }
  else return false;  
  
}

// =====================================================================
// =====================================================================

bool PluggableFunction::MHYDAS_GetDistributedProperty(mhydasdk::core::HydroObject *HO, wxString PropName, float *Value)
{
  // attention, la verif de l'existence de la prop bouffe beaucoup de temps
  
  if (HO != NULL)  
  {
    mhydasdk::core::ParamsMap::iterator it; 
    it = HO->getProperties()->find(PropName);
    
    if (it != HO->getProperties()->end())
    {
      *Value = it->second;
      return true;  
    }
    else return false;
    
  }
  else return false;  
}

// =====================================================================
// =====================================================================

    
bool PluggableFunction::MHYDAS_GetDistributedIniCondition(mhydasdk::core::HydroObject *HO, wxString IniCondName, float *Value)
{
  if (HO != NULL)  
  {
    mhydasdk::core::ParamsMap::iterator it; 
    it = HO->getIniConditions()->find(IniCondName);
    
    if (it != HO->getIniConditions()->end())
    {
      *Value = it->second;
      return true;  
    }
    else return false;
    
  }
  else return false;  
  
}

// =====================================================================
// =====================================================================

bool PluggableFunction::MHYDAS_GetDistributedRainValue(mhydasdk::core::SurfaceUnit *SU, int Step, float *Value)
{

  if (SU->getRainSource() != NULL)
  {
  
    float* TmpValues = SU->getRainSource()->getProcessedData();
    *Value = TmpValues[Step]; 
    return true;
  }
  else return false;  
}


// =====================================================================
// =====================================================================


bool PluggableFunction::MHYDAS_IsDistributedVarExists(mhydasdk::core::HydroObject *HO, wxString VarName)
{
  return (HO != NULL && (HO->getSimulatedVars()->find(VarName) != HO->getSimulatedVars()->end()));  
}

// =====================================================================
// =====================================================================


bool PluggableFunction::MHYDAS_IsDistributedVarValueExists(mhydasdk::core::HydroObject *HO, wxString VarName, int Step)
{
  if (HO != NULL)
  {
    mhydasdk::core::SimulatedVarsMap::iterator it; 
    it = HO->getSimulatedVars()->find(VarName);

    if (it != HO->getSimulatedVars()->end())
    {
      mhydasdk::core::VectorOfDouble* ValuesVect = it->second;
      if (ValuesVect != NULL && Step < ValuesVect->size()) return true;
      else return false;            
    }
    else return false;              
  }
  else return false;  
}

// =====================================================================
// =====================================================================


    
bool PluggableFunction::MHYDAS_AppendDistributedVarValue(mhydasdk::core::HydroObject *HO, wxString VarName, float Value)
{
  if (HO != NULL)  
  {
    mhydasdk::core::SimulatedVarsMap::iterator it; 
    it = HO->getSimulatedVars()->find(VarName);
    
    if (it != HO->getSimulatedVars()->end())
    {    
      it->second->push_back(Value);
      return true;
    }
    else return false;  
  }
  else return false;  

}

// =====================================================================
// =====================================================================


    
bool PluggableFunction::MHYDAS_SetDistributedVarValue(mhydasdk::core::HydroObject *HO, wxString VarName, int Step, float Value)
{
  if (HO != NULL)  
  {
    mhydasdk::core::SimulatedVarsMap::iterator it; 
    it = HO->getSimulatedVars()->find(VarName);
    
    if (it != HO->getSimulatedVars()->end())
    {    
      mhydasdk::core::VectorOfDouble* ValuesVect = it->second;
      if (Step < ValuesVect->size())
      {
        ValuesVect->at(Step) = Value;
        return true;
      }
      else return false;  
    }
    else return false;  
  }
  else return false;  
  
}

// =====================================================================
// =====================================================================


bool PluggableFunction::MHYDAS_GetFunctionParam(mhydasdk::core::ParamsMap Params, wxString ParamName, float *Value)
{
  if (Params.find(ParamName) != Params.end())
  {
    *Value = Params[ParamName];
  }
  else return false;  
}

// =====================================================================
// =====================================================================


    
bool PluggableFunction::MHYDAS_GetFunctionParam(mhydasdk::core::ParamsMap Params, wxString ParamName, int *Value)
{
  if (Params.find(ParamName) != Params.end())
  {
    *Value = (int)(Params[ParamName]);
  }
  else return false;  
  
}


// =====================================================================
// =====================================================================






} } // namespace mhydasdk::base




