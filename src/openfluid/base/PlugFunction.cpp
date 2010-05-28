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
  \file PlugFunction.cpp
  \brief implementation file for generic pluggable function interface definition

  \author Jean-Christophe FABRE <fabrejc@supagro.inra.fr>
*/

#include <iostream>

#include <openfluid/base/PlugFunction.hpp>
#include <openfluid/base/OFException.hpp>


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
    {
      std::string TimeStr;
      openfluid::tools::ConvertValue(Step,&TimeStr);
      throw OFException("OpenFLUID framework","PluggableFunction::OPENFLUID_GetVariable","Value for scalar variable "+ VarName +" does not exist at time step "+ TimeStr);
    }
  }
  else throw OFException("OpenFLUID framework","PluggableFunction::OPENFLUID_GetVariable","Unit is NULL");;
}


// =====================================================================
// =====================================================================

void PluggableFunction::OPENFLUID_GetVariable(openfluid::core::Unit *UnitPtr, openfluid::core::VariableName_t VarName, openfluid::core::TimeStep_t Step, openfluid::core::VectorValue *Value)
{
  if (UnitPtr != NULL)
  {
    if (!UnitPtr->getVectorVariables()->getValue(VarName,Step,Value))
    {
      std::string TimeStr;
      openfluid::tools::ConvertValue(Step,&TimeStr);
      throw OFException("OpenFLUID framework","PluggableFunction::OPENFLUID_GetVariable","Value for vector variable "+ VarName +" does not exist at time step "+ TimeStr);
    }
  }
  else throw OFException("OpenFLUID framework","PluggableFunction::OPENFLUID_GetVariable","Unit is NULL");;
}


// =====================================================================
// =====================================================================

void PluggableFunction::OPENFLUID_GetInputData(openfluid::core::Unit *UnitPtr, openfluid::core::InputDataName_t InputName, double *Value)
{
  if (UnitPtr != NULL)
  {
    if (!UnitPtr->getInputData()->getValueAsDouble(InputName,Value))
      throw OFException("OpenFLUID framework","PluggableFunction::OPENFLUID_GetInputData","Double value for input data "+ InputName +" does not exist");
  }
  else throw OFException("OpenFLUID framework","PluggableFunction::OPENFLUID_GetInputData","Unit is NULL");
}


// =====================================================================
// =====================================================================

void PluggableFunction::OPENFLUID_GetInputData(openfluid::core::Unit *UnitPtr, openfluid::core::InputDataName_t InputName, long *Value)
{
  if (UnitPtr != NULL)
  {
    if (!UnitPtr->getInputData()->getValueAsLong(InputName,Value))
      throw OFException("OpenFLUID framework","PluggableFunction::OPENFLUID_GetInputData","Long integer for input data "+ InputName +" does not exist");
  }
  else throw OFException("OpenFLUID framework","PluggableFunction::OPENFLUID_GetInputData","Unit is NULL");
}


// =====================================================================
// =====================================================================

void PluggableFunction::OPENFLUID_GetInputData(openfluid::core::Unit *UnitPtr, openfluid::core::InputDataName_t InputName, std::string *Value)
{
  if (UnitPtr != NULL)
  {
    if (!UnitPtr->getInputData()->getValue(InputName,Value))
      throw OFException("OpenFLUID framework","PluggableFunction::OPENFLUID_GetInputData","String value for input data "+ InputName +" does not exist");
  }
  else throw OFException("OpenFLUID framework","PluggableFunction::OPENFLUID_GetInputData","Unit is NULL");
}


// =====================================================================
// =====================================================================

bool PluggableFunction::OPENFLUID_IsInputDataExist(openfluid::core::Unit *UnitPtr, openfluid::core::InputDataName_t InputName)
{
  if (UnitPtr != NULL)
  {
    return UnitPtr->getInputData()->isDataExist(InputName);
  }
  else throw OFException("OpenFLUID framework","PluggableFunction::OPENFLUID_IsInputDataExist","Unit is NULL");;

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
      throw OFException("OpenFLUID framework","PluggableFunction::OPENFLUID_AppendVariable","Error appending value for scalar variable "+ VarName);
  }
  else throw OFException("OpenFLUID framework","PluggableFunction::OPENFLUID_AppendVariable","Unit is NULL");;
}

// =====================================================================
// =====================================================================


void PluggableFunction::OPENFLUID_AppendVariable(openfluid::core::Unit *UnitPtr, openfluid::core::VariableName_t VarName, openfluid::core::VectorValue& Value)
{
  if (UnitPtr != NULL)
  {
    if (!UnitPtr->getVectorVariables()->appendValue(VarName,Value))
      throw OFException("OpenFLUID framework","PluggableFunction::OPENFLUID_AppendVariable","Error appending value for vector variable "+ VarName);
  }
  else throw OFException("OpenFLUID framework","PluggableFunction::OPENFLUID_AppendVariable","Unit is NULL");
}

// =====================================================================
// =====================================================================



void PluggableFunction::OPENFLUID_SetVariable(openfluid::core::Unit *UnitPtr, openfluid::core::VariableName_t VarName, openfluid::core::TimeStep_t Step, openfluid::core::ScalarValue Value)
{

  if (UnitPtr != NULL)
  {
    if (!UnitPtr->getScalarVariables()->modifyValue(VarName,Step,Value))
      throw OFException("OpenFLUID framework","PluggableFunction::OPENFLUID_SetVariable","Error setting value for scalar variable "+ VarName);
  }
  else throw OFException("OpenFLUID framework","PluggableFunction::OPENFLUID_SetVariable","Unit is NULL");;

}

// =====================================================================
// =====================================================================

void PluggableFunction::OPENFLUID_SetVariable(openfluid::core::Unit *UnitPtr, openfluid::core::VariableName_t VarName, openfluid::core::TimeStep_t Step, openfluid::core::VectorValue Value)
{

  if (UnitPtr != NULL)
  {
    if (!UnitPtr->getVectorVariables()->modifyValue(VarName,Step,Value))
      throw OFException("OpenFLUID framework","PluggableFunction::OPENFLUID_SetVariable","Error setting value for vector variable "+ VarName);
  }
  else throw OFException("OpenFLUID framework","PluggableFunction::OPENFLUID_SetVariable","Unit is NULL");;

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

void PluggableFunction::OPENFLUID_AppendEvent(openfluid::core::Unit *UnitPtr,openfluid::core::Event& Ev)
{
  UnitPtr->getEvents()->addEvent(&Ev);
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


void PluggableFunction::OPENFLUID_RaiseWarning(std::string Sender, std::string Source, openfluid::core::TimeStep_t TimeStep, std::string Msg)
{
  mp_ExecMsgs->addWarning(Sender, Source, TimeStep,Msg);
}


// =====================================================================
// =====================================================================


void PluggableFunction::OPENFLUID_RaiseWarning(std::string Sender, std::string Source, std::string Msg)
{
  mp_ExecMsgs->addWarning(Sender,Source, Msg);
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




