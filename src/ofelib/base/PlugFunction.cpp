/***copyright>=========================================================

  OpenFLUID - Software environment for modelling Fluxes In Landscapes

  This file is a part of OpenFLUID software
  http://www.umr-lisah.fr/openfluid
  Copyright (c) INRA
  See the file "copyright" for the full license governing this code.

=========================================================<copyright***/


/**
  \file PlugFunction.cpp
  \brief implementation file for generic pluggable function interface definition

  \author Jean-Christophe FABRE <fabrejc@supagro.inra.fr>
*/

#include <iostream>

#include "PlugFunction.h"
#include "OFException.h"





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


void PluggableFunction::OPENFLUID_GetVariable(openfluid::core::Unit *UnitPtr, openfluid::core::VariableName_t VarName, openfluid::core::TimeStep_t Step, openfluid::core::ScalarValue *Value)
{
  if (UnitPtr != NULL)
  {
    if (!UnitPtr->getScalarVariables()->getValue(VarName,Step,Value))
      throw OFException("ofelib","PluggableFunction::OPENFLUID_GetVariable","Value for scalar variable "+ VarName +" does not exist");
  }
  else throw OFException("ofelib","PluggableFunction::OPENFLUID_GetVariable","Unit is NULL");;
}


// =====================================================================
// =====================================================================

void PluggableFunction::OPENFLUID_GetVariable(openfluid::core::Unit *UnitPtr, openfluid::core::VariableName_t VarName, openfluid::core::TimeStep_t Step, openfluid::core::VectorValue *Value)
{
  if (UnitPtr != NULL)
  {
    if (!UnitPtr->getVectorVariables()->getValue(VarName,Step,Value))
      throw OFException("ofelib","PluggableFunction::OPENFLUID_GetVariable","Value for vector variable "+ VarName +" does not exist");
  }
  else throw OFException("ofelib","PluggableFunction::OPENFLUID_GetVariable","Unit is NULL");;
}


// =====================================================================
// =====================================================================

void PluggableFunction::OPENFLUID_GetInputData(openfluid::core::Unit *UnitPtr, openfluid::core::InputDataName_t InputName, openfluid::core::ScalarValue *Value)
{
  if (UnitPtr != NULL)
  {
    if (!UnitPtr->getInputData()->getValue(InputName,Value))
      throw OFException("ofelib","PluggableFunction::OPENFLUID_GetInputData","Value for input data "+ InputName +" does not exist");
  }
  else throw OFException("ofelib","PluggableFunction::OPENFLUID_GetInputData","Unit is NULL");
}

// =====================================================================
// =====================================================================

bool PluggableFunction::OPENFLUID_IsInputDataExist(openfluid::core::Unit *UnitPtr, openfluid::core::InputDataName_t InputName)
{
  if (UnitPtr != NULL)
  {
    return UnitPtr->getInputData()->isDataExist(InputName);
  }
  else throw OFException("ofelib","PluggableFunction::OPENFLUID_IsInputDataExist","Unit is NULL");;

  return false;
}




// =====================================================================
// =====================================================================


bool PluggableFunction::OPENFLUID_IsScalarVariableExist(openfluid::core::Unit *UnitPtr, openfluid::core::VariableName_t VarName)
{
  return (UnitPtr != NULL && UnitPtr->getScalarVariables()->isVariableExist(VarName));
}

// =====================================================================
// =====================================================================

bool PluggableFunction::OPENFLUID_IsVariableExist(openfluid::core::Unit *UnitPtr, openfluid::core::VariableName_t VarName)
{

  return (OPENFLUID_IsScalarVariableExist(UnitPtr,VarName) || OPENFLUID_IsVectorVariableExist(UnitPtr,VarName));

}

// =====================================================================
// =====================================================================


bool PluggableFunction::OPENFLUID_IsVectorVariableExist(openfluid::core::Unit *UnitPtr, openfluid::core::VariableName_t VarName)
{
  return (UnitPtr != NULL && UnitPtr->getVectorVariables()->isVariableExist(VarName));
}

// =====================================================================
// =====================================================================


bool PluggableFunction::OPENFLUID_IsScalarVariableExist(openfluid::core::Unit *UnitPtr, openfluid::core::VariableName_t VarName, openfluid::core::TimeStep_t Step)
{

  if (UnitPtr != NULL)
  {
    return UnitPtr->getScalarVariables()->isVariableExist(VarName,Step);
  }
  return false;

}



bool PluggableFunction::OPENFLUID_IsVariableExist(openfluid::core::Unit *UnitPtr, openfluid::core::VariableName_t VarName, openfluid::core::TimeStep_t Step)
{

  return (OPENFLUID_IsScalarVariableExist(UnitPtr,VarName, Step) || OPENFLUID_IsVectorVariableExist(UnitPtr,VarName, Step));

}

// =====================================================================
// =====================================================================


bool PluggableFunction::OPENFLUID_IsVectorVariableExist(openfluid::core::Unit *UnitPtr, openfluid::core::VariableName_t VarName, openfluid::core::TimeStep_t Step)
{

  if (UnitPtr != NULL)
  {
    return UnitPtr->getVectorVariables()->isVariableExist(VarName,Step);
  }
  return false;

}


// =====================================================================
// =====================================================================



void PluggableFunction::OPENFLUID_AppendVariable(openfluid::core::Unit *UnitPtr, openfluid::core::VariableName_t VarName, openfluid::core::ScalarValue Value)
{
  if (UnitPtr != NULL)
  {
    if (!UnitPtr->getScalarVariables()->appendValue(VarName,Value))
      throw OFException("ofelib","PluggableFunction::OPENFLUID_AppendVariable","Error appending value for scalar variable "+ VarName);
  }
  else throw OFException("ofelib","PluggableFunction::OPENFLUID_AppendVariable","Unit is NULL");;
}

// =====================================================================
// =====================================================================


void PluggableFunction::OPENFLUID_AppendVariable(openfluid::core::Unit *UnitPtr, openfluid::core::VariableName_t VarName, openfluid::core::VectorValue& Value)
{
  if (UnitPtr != NULL)
  {
    if (!UnitPtr->getVectorVariables()->appendValue(VarName,Value))
      throw OFException("ofelib","PluggableFunction::OPENFLUID_AppendVariable","Error appending value for vector variable "+ VarName);
  }
  else throw OFException("ofelib","PluggableFunction::OPENFLUID_AppendVariable","Unit is NULL");
}

// =====================================================================
// =====================================================================



void PluggableFunction::OPENFLUID_SetVariable(openfluid::core::Unit *UnitPtr, openfluid::core::VariableName_t VarName, openfluid::core::TimeStep_t Step, openfluid::core::ScalarValue Value)
{

  if (UnitPtr != NULL)
  {
    if (!UnitPtr->getScalarVariables()->modifyValue(VarName,Step,Value))
      throw OFException("ofelib","PluggableFunction::OPENFLUID_SetVariable","Error setting value for scalar variable "+ VarName);
  }
  else throw OFException("ofelib","PluggableFunction::OPENFLUID_SetVariable","Unit is NULL");;

}

// =====================================================================
// =====================================================================

void PluggableFunction::OPENFLUID_SetVariable(openfluid::core::Unit *UnitPtr, openfluid::core::VariableName_t VarName, openfluid::core::TimeStep_t Step, openfluid::core::VectorValue Value)
{

  if (UnitPtr != NULL)
  {
    if (!UnitPtr->getVectorVariables()->modifyValue(VarName,Step,Value))
      throw OFException("ofelib","PluggableFunction::OPENFLUID_SetVariable","Error setting value for vector variable "+ VarName);
  }
  else throw OFException("ofelib","PluggableFunction::OPENFLUID_SetVariable","Unit is NULL");;

}

// =====================================================================
// =====================================================================



bool PluggableFunction::OPENFLUID_GetFunctionParameter(openfluid::core::FuncParamsMap_t Params, openfluid::core::FuncParamKey_t ParamName, double *Value)
{
  openfluid::core::FuncParamKey_t TmpStr;

  if (Params.find(ParamName) != Params.end())
  {
    TmpStr = Params[ParamName];

    return openfluid::tools::ConvertString<double>(TmpStr,Value);

  }
  else return false;
}

// =====================================================================
// =====================================================================



bool PluggableFunction::OPENFLUID_GetFunctionParameter(openfluid::core::FuncParamsMap_t Params, openfluid::core::FuncParamKey_t ParamName, long *Value)
{
  std::string TmpStr;

  if (Params.find(ParamName) != Params.end())
  {
    TmpStr = Params[ParamName];

    return openfluid::tools::ConvertString<long>(TmpStr,Value);
  }
  else return false;

}


// =====================================================================
// =====================================================================


bool PluggableFunction::OPENFLUID_GetFunctionParameter(openfluid::core::FuncParamsMap_t Params, openfluid::core::FuncParamKey_t ParamName, float *Value)
{
  std::string TmpStr;

  if (Params.find(ParamName) != Params.end())
  {
    TmpStr = Params[ParamName];
    return openfluid::tools::ConvertString<float>(TmpStr,Value);

  }
  else return false;
}

// =====================================================================
// =====================================================================



bool PluggableFunction::OPENFLUID_GetFunctionParameter(openfluid::core::FuncParamsMap_t Params, openfluid::core::FuncParamKey_t ParamName, int *Value)
{
  std::string TmpStr;

    if (Params.find(ParamName) != Params.end())
    {
      TmpStr = Params[ParamName];
      return openfluid::tools::ConvertString<int>(TmpStr,Value);

    }
    else return false;
}


// =====================================================================
// =====================================================================


bool PluggableFunction::OPENFLUID_GetFunctionParameter(openfluid::core::FuncParamsMap_t Params, openfluid::core::FuncParamKey_t ParamName, std::string *Value)
{
  std::string TmpStr;

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


bool PluggableFunction::OPENFLUID_GetFunctionParameter(openfluid::core::FuncParamsMap_t Params, openfluid::core::FuncParamKey_t ParamName, std::vector<std::string> *Values)
{
	std::string TmpStr;

	std::vector<std::string> Tokens;

	if (Params.find(ParamName) != Params.end())
	{
		TmpStr = Params[ParamName];

		openfluid::tools::TokenizeString(TmpStr,Tokens,";");

		(*Values).clear();
    for (unsigned int i=0;i<Tokens.size();i++) (*Values).push_back(Tokens[i]);

		return true;
	}
	else return false;

}

// =====================================================================
// =====================================================================


bool PluggableFunction::OPENFLUID_GetFunctionParameter(openfluid::core::FuncParamsMap_t Params, openfluid::core::FuncParamKey_t ParamName, std::vector<double> *Values)
{
  std::vector<std::string> StrVect;
  double TmpValue;

  // gets the param as a vector of string
  bool IsOK = OPENFLUID_GetFunctionParameter(Params,ParamName,&StrVect);

  // clears the double values vector
  (*Values).clear();

  unsigned int i=0;

  while (IsOK && i < StrVect.size())
  {
    IsOK = IsOK && openfluid::tools::ConvertString<double>(StrVect[i],&TmpValue);

    // if conversion is OK, add the value and continue
    if (IsOK) Values->push_back(TmpValue);
    else (*Values).clear();

    i++;
  }

  return IsOK;

}

// =====================================================================
// =====================================================================


bool PluggableFunction::OPENFLUID_GetFunctionParameter(openfluid::core::FuncParamsMap_t Params, openfluid::core::FuncParamKey_t ParamName, std::vector<long> *Values)
{
  std::vector<std::string> StrVect;
  long TmpValue;

  // gets the param as a vector of string
  bool IsOK = OPENFLUID_GetFunctionParameter(Params,ParamName,&StrVect);

  // clears the double values vector
  (*Values).clear();

  unsigned int i=0;

  while (IsOK && i < StrVect.size())
  {
    IsOK = IsOK && openfluid::tools::ConvertString<long>(StrVect[i],&TmpValue);

    // if conversion is OK, add the value and continue
    if (IsOK) Values->push_back(TmpValue);
    else (*Values).clear();

    i++;
  }

  return IsOK;

}

// =====================================================================
// =====================================================================

void PluggableFunction::OPENFLUID_GetEvents(openfluid::core::Unit *UnitPtr, openfluid::core::DateTime BeginDate, openfluid::core::DateTime EndDate, openfluid::core::EventsCollection* Events)
{
  UnitPtr->getEvents()->getEventsBetween(BeginDate,EndDate,Events);
}

// =====================================================================
// =====================================================================


bool PluggableFunction::OPENFLUID_IsUnitClassExist(openfluid::core::UnitClass_t ClassName)
{
  return mp_CoreData->isUnitsClassExist(ClassName);
}

// =====================================================================
// =====================================================================


bool PluggableFunction::OPENFLUID_GetUnitsCount(openfluid::core::UnitClass_t ClassName, unsigned int *UnitsCount)
{
  if (mp_CoreData->isUnitsClassExist(ClassName))
  {
    *UnitsCount = mp_CoreData->getUnits(ClassName)->getList()->size();
    return true;
  }
  else return false;
}


// =====================================================================
// =====================================================================



void PluggableFunction::OPENFLUID_RaiseWarning(std::string Sender, openfluid::core::TimeStep_t TimeStep, std::string Msg)
{
  mp_ExecMsgs->addWarning(Sender,TimeStep,Msg);
}


// =====================================================================
// =====================================================================


void PluggableFunction::OPENFLUID_RaiseWarning(std::string Sender, std::string Msg)
{
  mp_ExecMsgs->addWarning(Sender,Msg);
}

// =====================================================================
// =====================================================================


void PluggableFunction::OPENFLUID_RaiseError(std::string Sender, openfluid::core::TimeStep_t TimeStep, std::string Msg)
{
  throw OFException(Sender,TimeStep,Msg);
}

// =====================================================================
// =====================================================================


void PluggableFunction::OPENFLUID_RaiseError(std::string Sender, std::string Msg)
{
  throw OFException(Sender,Msg);
}

// =====================================================================
// =====================================================================

void PluggableFunction::OPENFLUID_RaiseError(std::string Sender, std::string Source, openfluid::core::TimeStep_t TimeStep, std::string Msg)
{
  throw OFException(Sender,Source,TimeStep,Msg);
}

// =====================================================================
// =====================================================================


void PluggableFunction::OPENFLUID_RaiseError(std::string Sender, std::string Source, std::string Msg)
{
  throw OFException(Sender,Source,Msg);
}

// =====================================================================
// =====================================================================


bool PluggableFunction::OPENFLUID_GetRunEnvironment(std::string Key, std::string *Value)
{
  return mp_FunctionEnv->getValue(Key,Value);
}

// =====================================================================
// =====================================================================


bool PluggableFunction::OPENFLUID_GetRunEnvironment(std::string Key, bool *Value)
{
  return mp_FunctionEnv->getValue(Key,Value);
}




} } // namespace openfluid::base




