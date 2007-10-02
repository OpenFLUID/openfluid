/**
  \file PlugFunction.cpp
  \brief implementation file for generic pluggable function interface definition

  \author Jean-Christophe FABRE <fabrejc@ensam.inra.fr>
*/

#include <iostream>

#include "PlugFunction.h"



// =====================================================================
// =====================================================================




namespace mhydasdk { namespace base {



PluggableFunction::PluggableFunction()

{

  mp_CoreData = NULL;

  mp_ExecMsgs = NULL;

  mp_Signature = new mhydasdk::base::Signature();
  
}


// =====================================================================
// =====================================================================


bool PluggableFunction::MHYDAS_GetDistributedVarValue(mhydasdk::core::HydroObject *HO, wxString VarName, int Step, mhydasdk::core::MHYDASValue *Value)
{
  if (HO != NULL)  
  {
    mhydasdk::core::SimulatedVarsMap::iterator it; 
    it = HO->getSimulatedVars()->find(VarName);
    
    if (it != HO->getSimulatedVars()->end())
    {    
      mhydasdk::core::VectorOfMHYDASValue* ValuesVect  = it->second;
      
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

bool PluggableFunction::MHYDAS_GetDistributedProperty(mhydasdk::core::HydroObject *HO, wxString PropName, mhydasdk::core::PropertyValue *Value)
{
  // attention, la verif de l'existence de la prop bouffe beaucoup de temps
  
  if (HO != NULL)  
  {
    mhydasdk::core::PropertiesMap::iterator it; 
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

bool PluggableFunction::MHYDAS_IsDistributedPropertyExists(mhydasdk::core::HydroObject *HO, wxString PropName)
{

  if (HO != NULL)
   {
     mhydasdk::core::PropertiesMap::iterator it; 
     it = HO->getProperties()->find(PropName);

     if (it != HO->getProperties()->end())
     {
       return true;            
     }
     else return false;              
   }
   else return false;  
  
}


// =====================================================================
// =====================================================================

    
bool PluggableFunction::MHYDAS_GetDistributedIniCondition(mhydasdk::core::HydroObject *HO, wxString IniCondName, mhydasdk::core::PropertyValue *Value)
{
  if (HO != NULL)  
  {
    mhydasdk::core::PropertiesMap::iterator it; 
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



bool PluggableFunction::MHYDAS_IsDistributedIniConditionExists(mhydasdk::core::HydroObject *HO, wxString IniCondName)
{


  if (HO != NULL)
   {
     mhydasdk::core::PropertiesMap::iterator it; 
     it = HO->getIniConditions()->find(IniCondName);

     if (it != HO->getIniConditions()->end())
     {
       return true;            
     }
     else return false;              
   }
   else return false;  

  
}


// =====================================================================
// =====================================================================

bool PluggableFunction::MHYDAS_GetDistributedRainValue(mhydasdk::core::SurfaceUnit *SU, int Step, mhydasdk::core::RainValue *Value)
{

  if (SU->getRainSource() != NULL)
  {
  
    mhydasdk::core::RainValue *TmpValues = SU->getRainSource()->getProcessedData();
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
      mhydasdk::core::VectorOfMHYDASValue* ValuesVect = it->second;
      if (ValuesVect != NULL && Step < ValuesVect->size()) return true;
      else return false;            
    }
    else return false;              
  }
  else return false;  
}

// =====================================================================
// =====================================================================


    
bool PluggableFunction::MHYDAS_AppendDistributedVarValue(mhydasdk::core::HydroObject *HO, wxString VarName, mhydasdk::core::MHYDASValue Value)
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


    
bool PluggableFunction::MHYDAS_SetDistributedVarValue(mhydasdk::core::HydroObject *HO, wxString VarName, int Step, mhydasdk::core::MHYDASValue Value)
{
  if (HO != NULL)  
  {
    mhydasdk::core::SimulatedVarsMap::iterator it; 
    it = HO->getSimulatedVars()->find(VarName);
    
    if (it != HO->getSimulatedVars()->end())
    {    
      mhydasdk::core::VectorOfMHYDASValue* ValuesVect = it->second;
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


bool PluggableFunction::MHYDAS_GetFunctionParam(mhydasdk::core::ParamsMap Params, wxString ParamName, double *Value)
{
  wxString TmpStr;
  
  if (Params.find(ParamName) != Params.end())
  {
    TmpStr = Params[ParamName];
    
    return TmpStr.ToDouble(Value);
    
  }
  else return false;  
}

// =====================================================================
// =====================================================================


    
bool PluggableFunction::MHYDAS_GetFunctionParam(mhydasdk::core::ParamsMap Params, wxString ParamName, long *Value)
{
  wxString TmpStr;
  
  if (Params.find(ParamName) != Params.end())
  {
    TmpStr = Params[ParamName];

    return TmpStr.ToLong(Value);
  }
  else return false;  
  
}


// =====================================================================
// =====================================================================


bool PluggableFunction::MHYDAS_GetFunctionParam(mhydasdk::core::ParamsMap Params, wxString ParamName, float *Value)
{
  wxString TmpStr;
  bool IsOK = true;
  double TmpValue;
  
  if (Params.find(ParamName) != Params.end())
  {
    TmpStr = Params[ParamName];
    IsOK = TmpStr.ToDouble(&TmpValue);    
    if (IsOK) *Value = (float)TmpValue;
    
    return IsOK;
    
  }
  else return false;  
}

// =====================================================================
// =====================================================================


    
bool PluggableFunction::MHYDAS_GetFunctionParam(mhydasdk::core::ParamsMap Params, wxString ParamName, int *Value)
{
  wxString TmpStr;
  bool IsOK = true;
  long TmpValue;
  
  if (Params.find(ParamName) != Params.end())
  {
    TmpStr = Params[ParamName];
    IsOK = TmpStr.ToLong(&TmpValue);    
    if (IsOK) *Value = (int)TmpValue;
    
    return IsOK;
  }
  else return false;  
  
}



// =====================================================================
// =====================================================================

    
bool PluggableFunction::MHYDAS_GetFunctionParam(mhydasdk::core::ParamsMap Params, wxString ParamName, wxString *Value)
{
  wxString TmpStr;
  
  if (Params.find(ParamName) != Params.end())
  {
    TmpStr = Params[ParamName];

    return true;
  }
  else return false;  
  
}

// =====================================================================
// =====================================================================


void PluggableFunction::MHYDAS_RaiseWarning(wxString Sender, int TimeStep, wxString WarningMsg)
{
  mp_ExecMsgs->addWarning(Sender,TimeStep,WarningMsg);
}
    

// =====================================================================
// =====================================================================


void PluggableFunction::MHYDAS_RaiseWarning(wxString Sender, wxString WarningMsg)
{
  mp_ExecMsgs->addWarning(Sender,WarningMsg);
}

// =====================================================================
// =====================================================================


void PluggableFunction::MHYDAS_RaiseError(wxString Sender, int TimeStep, wxString WarningMsg)
{
  mp_ExecMsgs->setError(Sender,TimeStep,WarningMsg);
}

// =====================================================================
// =====================================================================


void PluggableFunction::MHYDAS_RaiseError(wxString Sender, wxString WarningMsg)
{
  mp_ExecMsgs->setError(Sender,WarningMsg);
}



} } // namespace mhydasdk::base




