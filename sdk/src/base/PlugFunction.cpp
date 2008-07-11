/**
  \file PlugFunction.cpp
  \brief implementation file for generic pluggable function interface definition

  \author Jean-Christophe FABRE <fabrejc@ensam.inra.fr>
*/

#include <iostream>

#include "PlugFunction.h"

#include <wx/tokenzr.h>


// =====================================================================
// =====================================================================




namespace openfluid { namespace base {



PluggableFunction::PluggableFunction()
{
  mp_CoreData = NULL;
  mp_InternalCoreData = NULL;

  mp_ExecMsgs = NULL;
  
}

// =====================================================================
// =====================================================================


PluggableFunction::~PluggableFunction()
{ 
  
  
}


// =====================================================================
// =====================================================================


bool PluggableFunction::OPENFLUID_GetDistributedVarValue(openfluid::core::HydroObject *HO, wxString VarName, int Step, openfluid::core::ScalarValue *Value)
{
  if (HO != NULL)  
  {
    openfluid::core::SimulatedVarsMap::iterator it; 
    it = HO->getSimulatedVars()->find(VarName);
    
    if (it != HO->getSimulatedVars()->end())
    {    
      openfluid::core::SerieOfScalarValue* ValuesVect  = it->second;
      
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

bool PluggableFunction::OPENFLUID_GetDistributedVarValue(openfluid::core::HydroObject *HO, wxString VarName, int Step, openfluid::core::VectorValue *Value)
{
  if (HO != NULL)  
  {
    openfluid::core::SimulatedVectorVarsMap::iterator it; 
    it = HO->getSimulatedVectorVars()->find(VarName);
    
    if (it != HO->getSimulatedVectorVars()->end())
    {    
      openfluid::core::SerieOfVectorValue* ValuesVect  = it->second;
      
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

bool PluggableFunction::OPENFLUID_GetDistributedProperty(openfluid::core::HydroObject *HO, wxString PropName, openfluid::core::ScalarValue *Value)
{
  // NOTICE: the checking of properties costs execution time
  
  if (HO != NULL)  
  {
    openfluid::core::PropertiesMap::iterator it; 
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

bool PluggableFunction::OPENFLUID_IsDistributedPropertyExists(openfluid::core::HydroObject *HO, wxString PropName)
{

  if (HO != NULL)
   {
     openfluid::core::PropertiesMap::iterator it; 
     it = HO->getProperties()->find(PropName);

     if (it != HO->getProperties()->end())
     {
       return true;            
     }
     else return false;              
   }
   else return false;  
  
}


bool PluggableFunction::OPENFLUID_SetDistributedProperty(openfluid::core::HydroObject *HO, wxString PropName, openfluid::core::ScalarValue Value)
{

  if (HO != NULL)
   {
     openfluid::core::PropertiesMap::iterator it; 
     it = HO->getProperties()->find(PropName);

     if (it != HO->getProperties()->end())
     {
       it->second = Value;
       return true;            
     }
     else return false;              
   }
   else return false;  

  
}


// =====================================================================
// =====================================================================

    
bool PluggableFunction::OPENFLUID_GetDistributedIniCondition(openfluid::core::HydroObject *HO, wxString IniCondName, openfluid::core::ScalarValue *Value)
{
  if (HO != NULL)  
  {
    openfluid::core::PropertiesMap::iterator it; 
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



bool PluggableFunction::OPENFLUID_IsDistributedIniConditionExists(openfluid::core::HydroObject *HO, wxString IniCondName)
{


  if (HO != NULL)
   {
     openfluid::core::PropertiesMap::iterator it; 
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


bool PluggableFunction::OPENFLUID_IsDistributedScalarVarExists(openfluid::core::HydroObject *HO, wxString VarName)
{
  return (HO != NULL && (HO->getSimulatedVars()->find(VarName) != HO->getSimulatedVars()->end()));  
}

bool PluggableFunction::OPENFLUID_IsDistributedVarExists(openfluid::core::HydroObject *HO, wxString VarName)
{
  return OPENFLUID_IsDistributedScalarVarExists(HO,VarName);
}

// =====================================================================
// =====================================================================


bool PluggableFunction::OPENFLUID_IsDistributedVectorVarExists(openfluid::core::HydroObject *HO, wxString VarName)
{
  return (HO != NULL && (HO->getSimulatedVectorVars()->find(VarName) != HO->getSimulatedVectorVars()->end()));  
}

// =====================================================================
// =====================================================================


bool PluggableFunction::OPENFLUID_IsDistributedScalarVarValueExists(openfluid::core::HydroObject *HO, wxString VarName, int Step)
{
  if (HO != NULL)
  {
    openfluid::core::SimulatedVarsMap::iterator it; 
    it = HO->getSimulatedVars()->find(VarName);

    if (it != HO->getSimulatedVars()->end())
    {
      openfluid::core::SerieOfScalarValue* ValuesVect = it->second;
      if (ValuesVect != NULL && Step < ValuesVect->size()) return true;
      else return false;            
    }
    else return false;              
  }
  else return false;  
}



bool PluggableFunction::OPENFLUID_IsDistributedVarValueExists(openfluid::core::HydroObject *HO, wxString VarName, int Step)
{
  return OPENFLUID_IsDistributedScalarVarValueExists(HO,VarName,Step);
}

// =====================================================================
// =====================================================================


bool PluggableFunction::OPENFLUID_IsDistributedVectorVarValueExists(openfluid::core::HydroObject *HO, wxString VarName, int Step)
{
  if (HO != NULL)
  {
    openfluid::core::SimulatedVectorVarsMap::iterator it; 
    it = HO->getSimulatedVectorVars()->find(VarName);

    if (it != HO->getSimulatedVectorVars()->end())
    {
      openfluid::core::SerieOfVectorValue* ValuesVect = it->second;
      if (ValuesVect != NULL && Step < ValuesVect->size()) return true;
      else return false;            
    }
    else return false;              
  }
  else return false;  
}


// =====================================================================
// =====================================================================


    
bool PluggableFunction::OPENFLUID_AppendDistributedVarValue(openfluid::core::HydroObject *HO, wxString VarName, openfluid::core::ScalarValue Value)
{
  if (HO != NULL)  
  {
    openfluid::core::SimulatedVarsMap::iterator it; 
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


bool PluggableFunction::OPENFLUID_AppendDistributedVarValue(openfluid::core::HydroObject *HO, wxString VarName, openfluid::core::VectorValue Value)
{
  if (HO != NULL)  
  {
    openfluid::core::SimulatedVectorVarsMap::iterator it; 
    it = HO->getSimulatedVectorVars()->find(VarName);
    
    if (it != HO->getSimulatedVectorVars()->end())
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


    
bool PluggableFunction::OPENFLUID_SetDistributedVarValue(openfluid::core::HydroObject *HO, wxString VarName, int Step, openfluid::core::ScalarValue Value)
{
  if (HO != NULL)  
  {
    openfluid::core::SimulatedVarsMap::iterator it; 
    it = HO->getSimulatedVars()->find(VarName);
    
    if (it != HO->getSimulatedVars()->end())
    {    
      openfluid::core::SerieOfScalarValue* ValuesVect = it->second;
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

bool PluggableFunction::OPENFLUID_SetDistributedVarValue(openfluid::core::HydroObject *HO, wxString VarName, int Step, openfluid::core::VectorValue Value)
{
  if (HO != NULL)  
  {
    openfluid::core::SimulatedVectorVarsMap::iterator it; 
    it = HO->getSimulatedVectorVars()->find(VarName);
    
    if (it != HO->getSimulatedVectorVars()->end())
    {    
      openfluid::core::SerieOfVectorValue* ValuesVect = it->second;
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



bool PluggableFunction::OPENFLUID_GetFunctionParam(openfluid::core::ParamsMap Params, wxString ParamName, double *Value)
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


    
bool PluggableFunction::OPENFLUID_GetFunctionParam(openfluid::core::ParamsMap Params, wxString ParamName, long *Value)
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


bool PluggableFunction::OPENFLUID_GetFunctionParam(openfluid::core::ParamsMap Params, wxString ParamName, float *Value)
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



bool PluggableFunction::OPENFLUID_GetFunctionParam(openfluid::core::ParamsMap Params, wxString ParamName, int *Value)
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


bool PluggableFunction::OPENFLUID_GetFunctionParam(openfluid::core::ParamsMap Params, wxString ParamName, wxString *Value)
{
  wxString TmpStr;
  
  if (Params.find(ParamName) != Params.end())
  {
    TmpStr = Params[ParamName];
    *Value = TmpStr;
    return true;
  }
  else return false;  
  
}


// =====================================================================
// =====================================================================

    
bool PluggableFunction::OPENFLUID_GetFunctionParam(openfluid::core::ParamsMap Params, wxString ParamName, std::vector<wxString> *Values)
{
	wxString TmpStr;

	if (Params.find(ParamName) != Params.end())
	{
		TmpStr = Params[ParamName];

		wxStringTokenizer TmpTkz(TmpStr, wxT(";"),wxTOKEN_STRTOK);
		wxString TmpToken;

		(*Values).clear();

		while (TmpTkz.HasMoreTokens() )
		{
			TmpToken = TmpTkz.GetNextToken();
			(*Values).push_back(TmpToken);      
		}        
		return true;
	}
	else return false;  

}

// =====================================================================
// =====================================================================


bool PluggableFunction::OPENFLUID_GetFunctionParam(openfluid::core::ParamsMap Params, wxString ParamName, std::vector<double> *Values)
{
  std::vector<wxString> StrVect;
  double TmpValue;
  
  // gets the param as a vector of string  
  bool IsOK = OPENFLUID_GetFunctionParam(Params,ParamName,&StrVect);

  // clears the double values vector
  (*Values).clear();
  
  int i=0;
  
  while (IsOK && i < StrVect.size())
  {    
    IsOK = IsOK && StrVect[i].ToDouble(&TmpValue);    
    
    // if conversion is OK, add the value and continue
    if (IsOK) Values->push_back(TmpValue);
    else (*Values).clear();
    
    i++;
  }   
  
  return IsOK;
  
}

// =====================================================================
// =====================================================================


bool PluggableFunction::OPENFLUID_GetFunctionParam(openfluid::core::ParamsMap Params, wxString ParamName, std::vector<long> *Values)
{
  std::vector<wxString> StrVect;
  long TmpValue;
  
  // gets the param as a vector of string  
  bool IsOK = OPENFLUID_GetFunctionParam(Params,ParamName,&StrVect);

  // clears the double values vector
  (*Values).clear();
  
  int i=0;
  
  while (IsOK && i < StrVect.size())
  {    
    IsOK = IsOK && StrVect[i].ToLong(&TmpValue);    
    
    // if conversion is OK, add the value and continue
    if (IsOK) Values->push_back(TmpValue);
    else (*Values).clear();
    
    i++;
  }   
  
  return IsOK;
  
}

// =====================================================================
// =====================================================================

bool PluggableFunction::OPENFLUID_GetEvents(openfluid::core::HydroObject *HO, wxDateTime BeginDate, wxDateTime EndDate, openfluid::core::EventCollection* EventColl)
{
  
  HO->getEvents()->getEventsBetween(BeginDate,EndDate,EventColl);
  
  return true;
}

// =====================================================================
// =====================================================================



void PluggableFunction::OPENFLUID_RaiseWarning(wxString Sender, int TimeStep, wxString WarningMsg)
{
  mp_ExecMsgs->addWarning(Sender,TimeStep,WarningMsg);
}
    

// =====================================================================
// =====================================================================


void PluggableFunction::OPENFLUID_RaiseWarning(wxString Sender, wxString WarningMsg)
{
  mp_ExecMsgs->addWarning(Sender,WarningMsg);
}

// =====================================================================
// =====================================================================


void PluggableFunction::OPENFLUID_RaiseError(wxString Sender, int TimeStep, wxString WarningMsg)
{
  mp_ExecMsgs->setError(Sender,TimeStep,WarningMsg);
}

// =====================================================================
// =====================================================================


void PluggableFunction::OPENFLUID_RaiseError(wxString Sender, wxString WarningMsg)
{
  mp_ExecMsgs->setError(Sender,WarningMsg);
}

// =====================================================================
// =====================================================================


void PluggableFunction::OPENFLUID_GetEnvironmentInputDir(wxString *Directory)
{
  *Directory = m_FunctionEnv.InputDir;
}

// =====================================================================
// =====================================================================


void PluggableFunction::OPENFLUID_GetEnvironmentOutputDir(wxString *Directory)
{
  *Directory = m_FunctionEnv.OutputDir;
}



} } // namespace openfluid::base




