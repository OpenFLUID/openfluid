/**
  \file PlugFunction.cpp
  \brief implementation file for generic pluggable function interface definition

  \author Jean-Christophe FABRE <fabrejc@supagro.inra.fr>
*/

#include <iostream>

#include "PlugFunction.h"
#include "OFException.h"

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


bool PluggableFunction::OPENFLUID_GetVariable(openfluid::core::Unit *UnitPtr, wxString VarName, int Step, openfluid::core::ScalarValue *Value)
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

bool PluggableFunction::OPENFLUID_GetVariable(openfluid::core::HydroObject *HO, wxString VarName, int Step, openfluid::core::VectorValue*& Value)
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
        Value = &(ValuesVect->at(Step));
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

bool PluggableFunction::OPENFLUID_GetProperty(openfluid::core::HydroObject *HO, wxString PropName, openfluid::core::ScalarValue *Value)
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

bool PluggableFunction::OPENFLUID_IsPropertyExists(openfluid::core::HydroObject *HO, wxString PropName)
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


bool PluggableFunction::OPENFLUID_SetProperty(openfluid::core::HydroObject *HO, wxString PropName, openfluid::core::ScalarValue Value)
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


bool PluggableFunction::OPENFLUID_GetIniCondition(openfluid::core::HydroObject *HO, wxString IniCondName, openfluid::core::ScalarValue *Value)
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



bool PluggableFunction::OPENFLUID_IsIniConditionExists(openfluid::core::HydroObject *HO, wxString IniCondName)
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


bool PluggableFunction::OPENFLUID_IsScalarVariableExists(openfluid::core::HydroObject *HO, wxString VarName)
{
  return (HO != NULL && (HO->getSimulatedVars()->find(VarName) != HO->getSimulatedVars()->end()));
}

bool PluggableFunction::OPENFLUID_IsVariableExists(openfluid::core::HydroObject *HO, wxString VarName)
{
  return OPENFLUID_IsScalarVariableExists(HO,VarName);
}

// =====================================================================
// =====================================================================


bool PluggableFunction::OPENFLUID_IsVectorVariableExists(openfluid::core::HydroObject *HO, wxString VarName)
{
  return (HO != NULL && (HO->getSimulatedVectorVars()->find(VarName) != HO->getSimulatedVectorVars()->end()));
}

// =====================================================================
// =====================================================================


bool PluggableFunction::OPENFLUID_IsScalarVariableExists(openfluid::core::HydroObject *HO, wxString VarName, int Step)
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



bool PluggableFunction::OPENFLUID_IsVariableExists(openfluid::core::HydroObject *HO, wxString VarName, int Step)
{
  return OPENFLUID_IsScalarVariableExists(HO,VarName,Step);
}

// =====================================================================
// =====================================================================


bool PluggableFunction::OPENFLUID_IsVectorVariableExists(openfluid::core::HydroObject *HO, wxString VarName, int Step)
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



bool PluggableFunction::OPENFLUID_AppendVariable(openfluid::core::HydroObject *HO, wxString VarName, openfluid::core::ScalarValue Value)
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


bool PluggableFunction::OPENFLUID_AppendVariable(openfluid::core::HydroObject *HO, wxString VarName, openfluid::core::VectorValue& Value)
{
  if (HO != NULL)
  {
    openfluid::core::SimulatedVectorVarsMap::iterator it;
    it = HO->getSimulatedVectorVars()->find(VarName);

    if (it != HO->getSimulatedVectorVars()->end())
    {
//      std::cerr << "Append 1" << std::endl; std::cerr.flush();
      it->second->push_back(Value);
//      std::cerr << "Append 2" << std::endl; std::cerr.flush();
      return true;
    }
    else return false;
  }
  else return false;

}

// =====================================================================
// =====================================================================



bool PluggableFunction::OPENFLUID_SetVariable(openfluid::core::HydroObject *HO, wxString VarName, int Step, openfluid::core::ScalarValue Value)
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

bool PluggableFunction::OPENFLUID_SetVariable(openfluid::core::HydroObject *HO, wxString VarName, int Step, openfluid::core::VectorValue Value)
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



bool PluggableFunction::OPENFLUID_GetFunctionParameter(openfluid::core::ParamsMap Params, wxString ParamName, double *Value)
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



bool PluggableFunction::OPENFLUID_GetFunctionParameter(openfluid::core::ParamsMap Params, wxString ParamName, long *Value)
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


bool PluggableFunction::OPENFLUID_GetFunctionParameter(openfluid::core::ParamsMap Params, wxString ParamName, float *Value)
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



bool PluggableFunction::OPENFLUID_GetFunctionParameter(openfluid::core::ParamsMap Params, wxString ParamName, int *Value)
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


bool PluggableFunction::OPENFLUID_GetFunctionParameter(openfluid::core::ParamsMap Params, wxString ParamName, wxString *Value)
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


bool PluggableFunction::OPENFLUID_GetFunctionParameter(openfluid::core::ParamsMap Params, wxString ParamName, std::vector<wxString> *Values)
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


bool PluggableFunction::OPENFLUID_GetFunctionParameter(openfluid::core::ParamsMap Params, wxString ParamName, std::vector<double> *Values)
{
  std::vector<wxString> StrVect;
  double TmpValue;

  // gets the param as a vector of string
  bool IsOK = OPENFLUID_GetFunctionParameter(Params,ParamName,&StrVect);

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


bool PluggableFunction::OPENFLUID_GetFunctionParameter(openfluid::core::ParamsMap Params, wxString ParamName, std::vector<long> *Values)
{
  std::vector<wxString> StrVect;
  long TmpValue;

  // gets the param as a vector of string
  bool IsOK = OPENFLUID_GetFunctionParameter(Params,ParamName,&StrVect);

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

bool PluggableFunction::OPENFLUID_GetEvents(openfluid::core::HydroObject *HO, openfluid::core::DateTime BeginDate, openfluid::core::DateTime EndDate, openfluid::core::EventCollection* EventColl)
{

  HO->getEvents()->getEventsBetween(BeginDate,EndDate,EventColl);

  return true;
}

// =====================================================================
// =====================================================================



void PluggableFunction::OPENFLUID_RaiseWarning(wxString Sender, int TimeStep, wxString Msg)
{
  mp_ExecMsgs->addWarning(Sender,TimeStep,Msg);
}


// =====================================================================
// =====================================================================


void PluggableFunction::OPENFLUID_RaiseWarning(wxString Sender, wxString Msg)
{
  mp_ExecMsgs->addWarning(Sender,Msg);
}

// =====================================================================
// =====================================================================


void PluggableFunction::OPENFLUID_RaiseError(wxString Sender, int TimeStep, wxString Msg)
{
  throw OFException(std::string(Sender.mb_str(wxConvUTF8)),TimeStep,std::string(Msg.mb_str(wxConvUTF8)));
}

// =====================================================================
// =====================================================================


void PluggableFunction::OPENFLUID_RaiseError(wxString Sender, wxString Msg)
{
  throw OFException(std::string(Sender.mb_str(wxConvUTF8)),std::string(Msg.mb_str(wxConvUTF8)));
}

// =====================================================================
// =====================================================================

void PluggableFunction::OPENFLUID_RaiseError(wxString Sender, wxString Source, int TimeStep, wxString Msg)
{
  throw OFException(std::string(Sender.mb_str(wxConvUTF8)),std::string(Source.mb_str(wxConvUTF8)),TimeStep,std::string(Msg.mb_str(wxConvUTF8)));
}

// =====================================================================
// =====================================================================


void PluggableFunction::OPENFLUID_RaiseError(wxString Sender, wxString Source, wxString Msg)
{
  throw OFException(std::string(Sender.mb_str(wxConvUTF8)),std::string(Source.mb_str(wxConvUTF8)),std::string(Msg.mb_str(wxConvUTF8)));
}

// =====================================================================
// =====================================================================


void PluggableFunction::OPENFLUID_GetRunEnvironment(wxString Key, wxString *Value)
{
  mp_FunctionEnv->getValue(Key,Value);
}

// =====================================================================
// =====================================================================


void PluggableFunction::OPENFLUID_GetRunEnvironment(wxString Key, bool *Value)
{
  mp_FunctionEnv->getValue(Key,Value);
}




} } // namespace openfluid::base




