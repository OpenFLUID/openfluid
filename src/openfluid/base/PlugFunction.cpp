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
  @file
  @brief implementation file for generic pluggable function interface definition

  @author Jean-Christophe FABRE <fabrejc@supagro.inra.fr>
 */

#include <openfluid/base/PlugFunction.hpp>
#include <openfluid/base/OFException.hpp>
#include <openfluid/config.hpp>
#include <openfluid/core/BooleanValue.hpp>


// =====================================================================
// =====================================================================


namespace openfluid { namespace base {



PluggableFunction::PluggableFunction()
  : mp_InternalCoreData(NULL), mp_ExecMsgs(NULL),
    m_MaxThreads(openfluid::config::FUNCTIONS_MAXNUMTHREADS),
    m_Initialized(false), mp_CoreData(NULL)
{

}


// =====================================================================
// =====================================================================


PluggableFunction::~PluggableFunction()
{
  OPENFLUID_Logger.close();
}

// =====================================================================
// =====================================================================


void PluggableFunction::initializeFunction(openfluid::core::CoreRepository* CoreData,
    openfluid::base::ExecutionMessages* ExecMsgs,
    openfluid::base::EnvironmentProperties* FuncEnv,
    const unsigned int& MaxThreads,
    const openfluid::base::FuncID_t& FuncID)
{
  if (m_Initialized) return;


  mp_CoreData = CoreData;
  mp_ExecMsgs = ExecMsgs;
  mp_FunctionEnv = FuncEnv;
  m_FunctionID = FuncID;
  m_MaxThreads = MaxThreads;

  // initialize loggers

  std::string LogFile;
  std::string LogDir;

  mp_FunctionEnv->getValue("dir.output",&LogDir);
  LogFile = boost::filesystem::path(LogDir + "/" + m_FunctionID + ".log").string();

  OPENFLUID_Logger.open(LogFile.c_str());
}


// =====================================================================
// =====================================================================


bool PluggableFunction::IsUnitIDInPtrList(const openfluid::core::UnitsPtrList_t* UnitsList,
                                          const openfluid::core::UnitID_t& ID)
{
  if (UnitsList == NULL) return false;

  bool Found = false;
  openfluid::core::UnitsPtrList_t::const_iterator UnitsIt = UnitsList->begin();

  while (!Found && UnitsIt != UnitsList->end())
  {
    if ((*UnitsIt)->getID() == ID) Found = true;
    UnitsIt++;
  }

  return Found;
}


// =====================================================================
// =====================================================================


std::string PluggableFunction::generateDotEdge(std::string SrcClass, std::string SrcID,
                                               std::string DestClass, std::string DestID,
                                               std::string Options)
{
  return "\""+SrcClass+" #"+SrcID+"\" -> \""+DestClass+" #"+DestID+"\"" + Options;
}


// =====================================================================
// =====================================================================

void PluggableFunction::OPENFLUID_GetVariable(openfluid::core::Unit *UnitPtr,
                                        openfluid::core::VariableName_t VarName,
                                        openfluid::core::TimeStep_t Step,
                                        openfluid::core::Value* aValue)
{
  if (UnitPtr != NULL)
  {
    if (!UnitPtr->getVariables()->getValue(VarName,Step,aValue))
    {
      std::string TimeStr;
      openfluid::tools::ConvertValue(Step,&TimeStr);
      throw OFException("OpenFLUID framework","PluggableFunction::OPENFLUID_GetVariable","Value for variable "+ VarName +" does not exist or is not right type at time step "+ TimeStr);
    }
  }
  else throw OFException("OpenFLUID framework","PluggableFunction::OPENFLUID_GetVariable","Unit is NULL");
}


// =====================================================================
// =====================================================================

void PluggableFunction::OPENFLUID_GetVariable(openfluid::core::Unit* UnitPtr,
                                       openfluid::core::VariableName_t VarName,
                                       openfluid::core::TimeStep_t Step,
                                       double* aValue)
{
  openfluid::core::DoubleValue Val(*aValue);
  OPENFLUID_GetVariable(UnitPtr,VarName,Step,&Val);
  *aValue = Val.get();
}


// =====================================================================
// =====================================================================

void PluggableFunction::OPENFLUID_GetVariable(openfluid::core::Unit* UnitPtr,
                                       openfluid::core::VariableName_t VarName,
                                       openfluid::core::TimeStep_t Step,
                                       long* Value)
{
  openfluid::core::IntegerValue Val(*Value);
  OPENFLUID_GetVariable(UnitPtr,VarName,Step,&Val);
  *Value = Val.get();
}


// =====================================================================
// =====================================================================

void PluggableFunction::OPENFLUID_GetVariable(openfluid::core::Unit* UnitPtr,
                                       openfluid::core::VariableName_t VarName,
                                       openfluid::core::TimeStep_t Step,
                                       bool* Value)
{
  openfluid::core::BooleanValue Val(*Value);
  OPENFLUID_GetVariable(UnitPtr,VarName,Step,&Val);
  *Value = Val.get();
}


// =====================================================================
// =====================================================================

void PluggableFunction::OPENFLUID_GetVariable(openfluid::core::Unit* UnitPtr,
                                       openfluid::core::VariableName_t VarName,
                                       openfluid::core::TimeStep_t Step,
                                       std::string* Value)
{
  openfluid::core::StringValue Val(*Value);
  OPENFLUID_GetVariable(UnitPtr,VarName,Step,&Val);
  *Value = Val.get();
}


// =====================================================================
// =====================================================================

//void PluggableFunction::OPENFLUID_GetInputData(openfluid::core::Unit *UnitPtr,
//                            openfluid::core::InputDataName_t InputName,
//                            openfluid::core::DoubleValue* Val)
//{
//  if (UnitPtr != NULL)
//  {
//    double Dbl;
//    if (!UnitPtr->getInputData()->getValueAsDouble(InputName,&Dbl))
//      throw OFException("OpenFLUID framework","PluggableFunction::OPENFLUID_GetInputData","DoubleValue for input data "+ InputName +" does not exist");
//    else
//      Val->set(Dbl);
//  }
//  else throw OFException("OpenFLUID framework","PluggableFunction::OPENFLUID_GetInputData","Unit is NULL");
//}

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


void PluggableFunction::OPENFLUID_SetInputData(openfluid::core::Unit *UnitPtr,
                                               const openfluid::core::InputDataName_t& InputName,
                                               const double& Value)
{
  std::string StrValue;

  if (openfluid::tools::ConvertValue(Value,&StrValue))
    OPENFLUID_SetInputData(UnitPtr, InputName,StrValue);
  else
    throw OFException("OpenFLUID framework","PluggableFunction::OPENFLUID_SetInputData","Unable to format double value for input data "+ InputName);
}


// =====================================================================
// =====================================================================


void PluggableFunction::OPENFLUID_SetInputData(openfluid::core::Unit *UnitPtr,
                                               const openfluid::core::InputDataName_t& InputName,
                                               const long& Value)
{
  std::string StrValue;

  if (openfluid::tools::ConvertValue(Value,&StrValue))
    OPENFLUID_SetInputData(UnitPtr, InputName,StrValue);
  else
    throw OFException("OpenFLUID framework","PluggableFunction::OPENFLUID_SetInputData","Unable to format long value for input data "+ InputName);
}


// =====================================================================
// =====================================================================


void PluggableFunction::OPENFLUID_SetInputData(openfluid::core::Unit *UnitPtr,
                                               const openfluid::core::InputDataName_t& InputName,
                                               const std::string& Value)
{
  if (UnitPtr != NULL)
  {
    if (!UnitPtr->getInputData()->setValue(InputName,Value))
      throw OFException("OpenFLUID framework","PluggableFunction::OPENFLUID_SetInputData","Unable to set string value for input data "+ InputName);
  }
  else throw OFException("OpenFLUID framework","PluggableFunction::OPENFLUID_SetInputData","Unit is NULL");
}


// =====================================================================
// =====================================================================


bool PluggableFunction::OPENFLUID_IsInputDataExist(openfluid::core::Unit *UnitPtr, openfluid::core::InputDataName_t InputName)
{
  if (UnitPtr != NULL)
    return UnitPtr->getInputData()->isDataExist(InputName);

  throw OFException("OpenFLUID framework","PluggableFunction::OPENFLUID_IsInputDataExist","Unit is NULL");
  return false;
}


// =====================================================================
// =====================================================================

bool PluggableFunction::OPENFLUID_IsVariableExist(openfluid::core::Unit *UnitPtr,
                                        openfluid::core::VariableName_t VarName)
{
  return (UnitPtr != NULL && UnitPtr->getVariables()->isVariableExist(VarName));
}

// =====================================================================
// =====================================================================


bool PluggableFunction::OPENFLUID_IsVariableExist(openfluid::core::Unit *UnitPtr,
                                        openfluid::core::VariableName_t VarName,
                                        openfluid::core::TimeStep_t Step)
{
  if (UnitPtr != NULL)
    return UnitPtr->getVariables()->isVariableExist(VarName,Step);

  return false;
}

// =====================================================================
// =====================================================================

bool PluggableFunction::OPENFLUID_IsVariableExist(openfluid::core::Unit *UnitPtr,
                                        openfluid::core::VariableName_t VarName,
                                        openfluid::core::TimeStep_t Step,
                                        openfluid::core::Value::Type ValueType)
{
  if (UnitPtr != NULL)
    return UnitPtr->getVariables()->isVariableExist(VarName,Step,ValueType);

  return false;
}


// =====================================================================
// =====================================================================


void PluggableFunction::OPENFLUID_AppendVariable(openfluid::core::Unit *UnitPtr,
                                        openfluid::core::VariableName_t VarName,
                                        openfluid::core::Value& aValue)
{
  if (UnitPtr != NULL)
  {
    if (!UnitPtr->getVariables()->appendValue(VarName,aValue))
      throw OFException("OpenFLUID framework","PluggableFunction::OPENFLUID_AppendVariable","Error appending value for variable "+ VarName);
  }
  else throw OFException("OpenFLUID framework","PluggableFunction::OPENFLUID_AppendVariable","Unit is NULL");
}

// =====================================================================
// =====================================================================


void PluggableFunction::OPENFLUID_AppendVariable(openfluid::core::Unit *UnitPtr,
                                        openfluid::core::VariableName_t VarName,
                                        double Value)
{
  /* Do not call OPENFLUID_AppendVariable(UnitPtr,VarName,openfluid::core::DoubleValue(Value))
   * because of cast operator, THIS function is called (recursively)
   */
  if (UnitPtr != NULL)
  {
    if (!UnitPtr->getVariables()->appendValue(VarName,openfluid::core::DoubleValue(Value)))
      throw OFException("OpenFLUID framework","PluggableFunction::OPENFLUID_AppendVariable","Error appending double value for variable "+ VarName);
  }
  else throw OFException("OpenFLUID framework","PluggableFunction::OPENFLUID_AppendVariable","Unit is NULL");
}


// =====================================================================
// =====================================================================


void PluggableFunction::OPENFLUID_AppendVariable(openfluid::core::Unit *UnitPtr,
                                        openfluid::core::VariableName_t VarName,
                                        long Value)
{
  if (UnitPtr != NULL)
  {
    if (!UnitPtr->getVariables()->appendValue(VarName,openfluid::core::IntegerValue(Value)))
      throw OFException("OpenFLUID framework","PluggableFunction::OPENFLUID_AppendVariable","Error appending long value for variable "+ VarName);
  }
  else throw OFException("OpenFLUID framework","PluggableFunction::OPENFLUID_AppendVariable","Unit is NULL");
}


// =====================================================================
// =====================================================================


void PluggableFunction::OPENFLUID_AppendVariable(openfluid::core::Unit *UnitPtr,
                                        openfluid::core::VariableName_t VarName,
                                        bool Value)
{
  if (UnitPtr != NULL)
  {
    if (!UnitPtr->getVariables()->appendValue(VarName,openfluid::core::BooleanValue(Value)))
      throw OFException("OpenFLUID framework","PluggableFunction::OPENFLUID_AppendVariable","Error appending boolean value for variable "+ VarName);
  }
  else throw OFException("OpenFLUID framework","PluggableFunction::OPENFLUID_AppendVariable","Unit is NULL");
}


// =====================================================================
// =====================================================================


void PluggableFunction::OPENFLUID_AppendVariable(openfluid::core::Unit *UnitPtr,
                                        openfluid::core::VariableName_t VarName,
                                        std::string Value)
{
  if (UnitPtr != NULL)
  {
    if (!UnitPtr->getVariables()->appendValue(VarName,openfluid::core::StringValue(Value)))
      throw OFException("OpenFLUID framework","PluggableFunction::OPENFLUID_AppendVariable","Error appending string value for variable "+ VarName);
  }
  else throw OFException("OpenFLUID framework","PluggableFunction::OPENFLUID_AppendVariable","Unit is NULL");
}


// =====================================================================
// =====================================================================


void PluggableFunction::OPENFLUID_SetVariable(openfluid::core::Unit *UnitPtr,
                                        openfluid::core::VariableName_t VarName,
                                        openfluid::core::TimeStep_t Step,
                                        openfluid::core::Value& aValue)
{
  if (UnitPtr != NULL)
  {
    if (!UnitPtr->getVariables()->modifyValue(VarName,Step,aValue))
      throw OFException("OpenFLUID framework","PluggableFunction::OPENFLUID_SetVariable","Error setting value for variable "+ VarName);
  }
  else throw OFException("OpenFLUID framework","PluggableFunction::OPENFLUID_SetVariable","Unit is NULL");
}

// =====================================================================
// =====================================================================


void PluggableFunction::OPENFLUID_SetVariable(openfluid::core::Unit *UnitPtr,
                                        openfluid::core::VariableName_t VarName,
                                        openfluid::core::TimeStep_t Step,
                                        double Value)
{
  /* Do not call OPENFLUID_SetVariable(UnitPtr,VarName,openfluid::core::DoubleValue(Value))
   * because of cast operator, THIS function is called (recursively)
   */
  if (UnitPtr != NULL)
  {
    if (!UnitPtr->getVariables()->modifyValue(VarName,Step,openfluid::core::DoubleValue(Value)))
      throw OFException("OpenFLUID framework","PluggableFunction::OPENFLUID_SetVariable","Error setting double value for variable "+ VarName);
  }
  else throw OFException("OpenFLUID framework","PluggableFunction::OPENFLUID_SetVariable","Unit is NULL");
}


// =====================================================================
// =====================================================================


void PluggableFunction::OPENFLUID_SetVariable(openfluid::core::Unit *UnitPtr,
                                       openfluid::core::VariableName_t VarName,
                                       openfluid::core::TimeStep_t Step,
                                       long Value)
{
  if (UnitPtr != NULL)
  {
    if (!UnitPtr->getVariables()->modifyValue(VarName,Step,openfluid::core::IntegerValue(Value)))
      throw OFException("OpenFLUID framework","PluggableFunction::OPENFLUID_SetVariable","Error setting long value for variable "+ VarName);
  }
  else throw OFException("OpenFLUID framework","PluggableFunction::OPENFLUID_SetVariable","Unit is NULL");
}


// =====================================================================
// =====================================================================


void PluggableFunction::OPENFLUID_SetVariable(openfluid::core::Unit *UnitPtr,
                                       openfluid::core::VariableName_t VarName,
                                       openfluid::core::TimeStep_t Step,
                                       bool Value)
{
  if (UnitPtr != NULL)
  {
    if (!UnitPtr->getVariables()->modifyValue(VarName,Step,openfluid::core::BooleanValue(Value)))
      throw OFException("OpenFLUID framework","PluggableFunction::OPENFLUID_SetVariable","Error setting boolean value for variable "+ VarName);
  }
  else throw OFException("OpenFLUID framework","PluggableFunction::OPENFLUID_SetVariable","Unit is NULL");
}


// =====================================================================
// =====================================================================


void PluggableFunction::OPENFLUID_SetVariable(openfluid::core::Unit *UnitPtr,
                                       openfluid::core::VariableName_t VarName,
                                       openfluid::core::TimeStep_t Step,
                                       std::string Value)
{
  if (UnitPtr != NULL)
  {
    if (!UnitPtr->getVariables()->modifyValue(VarName,Step,openfluid::core::StringValue(Value)))
      throw OFException("OpenFLUID framework","PluggableFunction::OPENFLUID_SetVariable","Error setting string value for variable "+ VarName);
  }
  else throw OFException("OpenFLUID framework","PluggableFunction::OPENFLUID_SetVariable","Unit is NULL");
}

// =====================================================================
// =====================================================================


bool PluggableFunction::OPENFLUID_GetFunctionParameter(openfluid::core::FuncParamsMap_t Params,
                                      openfluid::core::FuncParamKey_t ParamName,
                                      openfluid::core::StringValue* Val)
{
  if (Params.find(ParamName) != Params.end())
  {
    *Val = Params[ParamName];

    return true;
  }

  return false;
}

// =====================================================================
// =====================================================================


bool PluggableFunction::OPENFLUID_GetFunctionParameter(openfluid::core::FuncParamsMap_t Params,
                                      openfluid::core::FuncParamKey_t ParamName,
                                      openfluid::core::DoubleValue* Val)
{
  if (Params.find(ParamName) != Params.end())
    return Params[ParamName].toDoubleValue(*Val);

  return false;
}


// =====================================================================
// =====================================================================


bool PluggableFunction::OPENFLUID_GetFunctionParameter(openfluid::core::FuncParamsMap_t Params,
                                      openfluid::core::FuncParamKey_t ParamName,
                                      openfluid::core::VectorValue* Val)
{
  if (Params.find(ParamName) != Params.end())
    return Params[ParamName].toVectorValue(";",*Val);

  return false;
}


// =====================================================================
// =====================================================================


bool PluggableFunction::OPENFLUID_GetFunctionParameter(openfluid::core::FuncParamsMap_t Params,
                                      openfluid::core::FuncParamKey_t ParamName,
                                      double *Value)
{
  if (Params.find(ParamName) != Params.end())
    return Params[ParamName].toDouble(*Value);

  return false;
}

// =====================================================================
// =====================================================================


bool PluggableFunction::OPENFLUID_GetFunctionParameter(openfluid::core::FuncParamsMap_t Params,
                                      openfluid::core::FuncParamKey_t ParamName,
                                      long *Value)
{
  if (Params.find(ParamName) != Params.end())
    return Params[ParamName].toInteger(*Value);

  return false;
}


// =====================================================================
// =====================================================================


bool PluggableFunction::OPENFLUID_GetFunctionParameter(openfluid::core::FuncParamsMap_t Params,
                                      openfluid::core::FuncParamKey_t ParamName,
                                      float *Value)
{
  bool IsOk = false;

  if (Params.find(ParamName) != Params.end())
  {
    double TmpDbl;

    IsOk = Params[ParamName].toDouble(TmpDbl);

    if(IsOk)
      *Value = static_cast<float>(TmpDbl);
  }

  return IsOk;
}

// =====================================================================
// =====================================================================


bool PluggableFunction::OPENFLUID_GetFunctionParameter(openfluid::core::FuncParamsMap_t Params,
                                      openfluid::core::FuncParamKey_t ParamName,
                                      int *Value)
{
  bool IsOk = false;

  if (Params.find(ParamName) != Params.end())
  {
    long TmpLong;

    IsOk = Params[ParamName].toInteger(TmpLong);

    if(IsOk)
      *Value = static_cast<int>(TmpLong);
  }

  return IsOk;
}


// =====================================================================
// =====================================================================


bool PluggableFunction::OPENFLUID_GetFunctionParameter(openfluid::core::FuncParamsMap_t Params,
                                      openfluid::core::FuncParamKey_t ParamName,
                                      std::string *Value)
{
  if (Params.find(ParamName) != Params.end())
  {
    *Value = Params[ParamName];

    return true;
  }

  return false;
}


// =====================================================================
// =====================================================================


bool PluggableFunction::OPENFLUID_GetFunctionParameter(openfluid::core::FuncParamsMap_t Params,
                                      openfluid::core::FuncParamKey_t ParamName,
                                      std::vector<std::string> *Values)
{
  if (Params.find(ParamName) != Params.end())
  {
    std::vector<std::string> Tokens;

    openfluid::tools::TokenizeString(Params[ParamName],Tokens,";");

    Values->clear();

    for (unsigned int i=0;i<Tokens.size();i++)
      Values->push_back(Tokens[i]);

    return true;
  }

 return false;
}

// =====================================================================
// =====================================================================


bool PluggableFunction::OPENFLUID_GetFunctionParameter(openfluid::core::FuncParamsMap_t Params,
                                      openfluid::core::FuncParamKey_t ParamName,
                                      std::vector<double> *Values)
{
  bool IsOK = false;

  if (Params.find(ParamName) != Params.end())
  {
    openfluid::core::VectorValue Vect;

    IsOK = Params[ParamName].toVectorValue(";",Vect);

    if(IsOK)
    {
      Values->clear();

      for (unsigned long i=0;i<Vect.size();i++)
        Values->push_back(Vect[i]);
    }
  }

  return IsOK;
}

// =====================================================================
// =====================================================================


bool PluggableFunction::OPENFLUID_GetFunctionParameter(openfluid::core::FuncParamsMap_t Params,
                                      openfluid::core::FuncParamKey_t ParamName,
                                      std::vector<long> *Values)
{
  bool IsOK = false;

  if (Params.find(ParamName) != Params.end())
  {
    openfluid::core::VectorValue Vect;

    IsOK = Params[ParamName].toVectorValue(";",Vect);

    if(IsOK)
    {
      Values->clear();

      for (unsigned long i=0;i<Vect.size();i++)
        Values->push_back(static_cast<long>(Vect[i]));
    }
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
  Ev.setInstantiationType(openfluid::core::InstantiationInfo::SIMULATION);
  UnitPtr->getEvents()->addEvent(&Ev);
}

// =====================================================================
// =====================================================================


bool PluggableFunction::OPENFLUID_IsUnitExist(openfluid::core::UnitClass_t ClassName,
                                              openfluid::core::UnitID_t ID)
{
  return (mp_CoreData->getUnit(ClassName,ID) != NULL);
}


// =====================================================================
// =====================================================================


bool PluggableFunction::OPENFLUID_IsUnitClassExist(openfluid::core::UnitClass_t ClassName)
{
  return mp_CoreData->isUnitsClassExist(ClassName);
}

// =====================================================================
// =====================================================================


bool PluggableFunction::OPENFLUID_GetUnitsCount(const openfluid::core::UnitClass_t ClassName,
                                                unsigned int& UnitsCount)
{
  UnitsCount = 0;
  if (mp_CoreData->isUnitsClassExist(ClassName))
  {
    UnitsCount = mp_CoreData->getUnits(ClassName)->getList()->size();
    return true;
  }
  else return false;
}


// =====================================================================
// =====================================================================


void PluggableFunction::OPENFLUID_GetUnitsCount(unsigned int& UnitsCount)
{
  UnitsCount = 0;
  openfluid::core::UnitsListByClassMap_t::const_iterator UnitsIt;

  for (UnitsIt = mp_CoreData->getUnitsByClass()->begin(); UnitsIt != mp_CoreData->getUnitsByClass()->end();++UnitsIt )
  {
    UnitsCount = UnitsCount + (*UnitsIt).second.getList()->size();
  }
}


// =====================================================================
// =====================================================================


bool PluggableFunction::OPENFLUID_GetUnit(const openfluid::core::UnitClass_t& ClassName,
                                          const openfluid::core::UnitID_t& ID,
                                          openfluid::core::Unit* aUnit)
{
  aUnit =  const_cast<openfluid::core::Unit*>(mp_CoreData->getUnit(ClassName,ID));
  return (aUnit != NULL);
}


// =====================================================================
// =====================================================================


openfluid::core::Unit* PluggableFunction::OPENFLUID_GetUnit(const openfluid::core::UnitClass_t& ClassName,
                                                            const openfluid::core::UnitID_t& ID)
{
  return mp_CoreData->getUnit(ClassName,ID);
}


// =====================================================================
// =====================================================================


void PluggableFunction::OPENFLUID_AddUnit(openfluid::core::UnitClass_t ClassName,
                                          openfluid::core::UnitID_t ID,
                                          openfluid::core::PcsOrd_t PcsOrder)
{
  if (!mp_CoreData->addUnit(openfluid::core::Unit(ClassName,ID,PcsOrder, openfluid::core::InstantiationInfo::SIMULATION)))
   throw OFException("OpenFLUID framework","PluggableFunction::OPENFLUID_AddUnit","Error adding unit");

  mp_CoreData->sortUnitsByProcessOrder();
}


// =====================================================================
// =====================================================================


void PluggableFunction::OPENFLUID_DeleteUnit(openfluid::core::UnitClass_t ClassName,
                                             openfluid::core::UnitID_t ID)
{

  openfluid::core::Unit* TheUnit = mp_CoreData->getUnit(ClassName,ID);

  if (TheUnit == NULL)
    OFException("OpenFLUID framework","PluggableFunction::OPENFLUID_DeleteUnit","error deleting unit");

  mp_CoreData->deleteUnit(TheUnit);

  mp_CoreData->sortUnitsByProcessOrder();
}

// =====================================================================
// =====================================================================


bool PluggableFunction::OPENFLUID_AddFromToConnection(openfluid::core::UnitClass_t ClassNameFrom,
                                                      openfluid::core::UnitID_t IDFrom,
                                                      openfluid::core::UnitClass_t ClassNameTo,
                                                      openfluid::core::UnitID_t IDTo)
{
  openfluid::core::Unit* FromUnit = mp_CoreData->getUnit(ClassNameFrom, IDFrom);
  openfluid::core::Unit* ToUnit = mp_CoreData->getUnit(ClassNameTo, IDTo);

  return OPENFLUID_AddFromToConnection(FromUnit, ToUnit);
}


// =====================================================================
// =====================================================================


bool PluggableFunction::OPENFLUID_AddFromToConnection(openfluid::core::Unit* FromUnit,
                                                      openfluid::core::Unit* ToUnit)
{
  if (FromUnit != NULL || ToUnit != NULL)
  {
    return (FromUnit->addToUnit(ToUnit) && ToUnit->addFromUnit(FromUnit));
  }
  else
    throw OFException("OpenFLUID framework","PluggableFunction::OPENFLUID_AddFromToConnection","Error adding from-to connection");
}


// =====================================================================
// =====================================================================


bool PluggableFunction::OPENFLUID_RemoveFromToConnection(openfluid::core::UnitClass_t ClassNameFrom,
                                                         openfluid::core::UnitID_t IDFrom,
                                                         openfluid::core::UnitClass_t ClassNameTo,
                                                         openfluid::core::UnitID_t IDTo)
{
  openfluid::core::Unit* FromUnit = mp_CoreData->getUnit(ClassNameFrom, IDFrom);
  openfluid::core::Unit* ToUnit = mp_CoreData->getUnit(ClassNameTo, IDTo);

  return OPENFLUID_RemoveFromToConnection(FromUnit, ToUnit);
}


// =====================================================================
// =====================================================================


bool PluggableFunction::OPENFLUID_RemoveFromToConnection(openfluid::core::Unit* FromUnit,
                                                         openfluid::core::Unit* ToUnit)
{
  if (FromUnit == NULL || ToUnit == NULL)
    throw OFException("OpenFLUID framework","CoreRepository::removeFromToConnection","Error removing from-to connection");
  else
    return mp_CoreData->removeFromToConnection(FromUnit,ToUnit);
}


// =====================================================================
// =====================================================================


bool PluggableFunction::OPENFLUID_AddChildParentConnection(openfluid::core::UnitClass_t ClassNameChild,
                                                           openfluid::core::UnitID_t IDChild,
                                                           openfluid::core::UnitClass_t ClassNameParent,
                                                           openfluid::core::UnitID_t IDParent)
{
  openfluid::core::Unit* ChildUnit = mp_CoreData->getUnit(ClassNameChild, IDChild);
  openfluid::core::Unit* ParentUnit = mp_CoreData->getUnit(ClassNameParent, IDParent);

  return OPENFLUID_AddChildParentConnection(ChildUnit, ParentUnit);
}


// =====================================================================
// =====================================================================


bool PluggableFunction::OPENFLUID_AddChildParentConnection(openfluid::core::Unit* ChildUnit,
                                                           openfluid::core::Unit* ParentUnit)
{
  if (ChildUnit != NULL || ParentUnit != NULL)
  {
    return (ChildUnit->addParentUnit(ParentUnit) && ParentUnit->addChildUnit(ChildUnit));
  }
  else
    throw OFException("OpenFLUID framework","PluggableFunction::OPENFLUID_AddChildParentConnection","Error adding child-parent connection");
}


// =====================================================================
// =====================================================================


bool PluggableFunction::OPENFLUID_RemoveChildParentConnection(openfluid::core::UnitClass_t ClassNameChild,
                                                           openfluid::core::UnitID_t IDChild,
                                                           openfluid::core::UnitClass_t ClassNameParent,
                                                           openfluid::core::UnitID_t IDParent)
{
  openfluid::core::Unit* ChildUnit = mp_CoreData->getUnit(ClassNameChild, IDChild);
  openfluid::core::Unit* ParentUnit = mp_CoreData->getUnit(ClassNameParent, IDParent);

  return OPENFLUID_RemoveChildParentConnection(ChildUnit, ParentUnit);
}


// =====================================================================
// =====================================================================


bool PluggableFunction::OPENFLUID_RemoveChildParentConnection(openfluid::core::Unit* ChildUnit,
                                                           openfluid::core::Unit* ParentUnit)
{
  if (ChildUnit == NULL || ParentUnit == NULL)
    throw OFException("OpenFLUID framework","CoreRepository::removeChildParentConnection","Error removing child-parent connection");
  else
    return mp_CoreData->removeChildParentConnection(ChildUnit,ParentUnit);
}


// =====================================================================
// =====================================================================

bool PluggableFunction::OPENFLUID_IsUnitConnectedTo(openfluid::core::Unit* aUnit,
                                                    const openfluid::core::UnitClass_t& ClassNameTo,
                                                    const openfluid::core::UnitID_t& IDTo)
{
  if (aUnit == NULL || !mp_CoreData->isUnitsClassExist(ClassNameTo)) return false;
  return IsUnitIDInPtrList(aUnit->getToUnits(ClassNameTo),IDTo);
}


// =====================================================================
// =====================================================================


bool PluggableFunction::OPENFLUID_IsUnitConnectedFrom(openfluid::core::Unit* aUnit,
                                                      const openfluid::core::UnitClass_t& ClassNameFrom,
                                                      const openfluid::core::UnitID_t& IDFrom)
{
  if (aUnit == NULL || !mp_CoreData->isUnitsClassExist(ClassNameFrom)) return false;
  return IsUnitIDInPtrList(aUnit->getFromUnits(ClassNameFrom),IDFrom);
}


// =====================================================================
// =====================================================================


bool PluggableFunction::OPENFLUID_IsUnitChildOf(openfluid::core::Unit* aUnit,
                                                const openfluid::core::UnitClass_t& ClassNameParent,
                                                const openfluid::core::UnitID_t& IDParent)
{
  if (aUnit == NULL || !mp_CoreData->isUnitsClassExist(ClassNameParent)) return false;
  return IsUnitIDInPtrList(aUnit->getParentUnits(ClassNameParent),IDParent);
}


// =====================================================================
// =====================================================================


bool PluggableFunction::OPENFLUID_IsUnitParentOf(openfluid::core::Unit* aUnit,
                                                 const openfluid::core::UnitClass_t& ClassNameChild,
                                                 const openfluid::core::UnitID_t& IDChild)
{
  if (aUnit == NULL || !mp_CoreData->isUnitsClassExist(ClassNameChild)) return false;
  return IsUnitIDInPtrList(aUnit->getChildrenUnits(ClassNameChild),IDChild);
}


// =====================================================================
// =====================================================================


void PluggableFunction::OPENFLUID_BuildUnitsMatrix(const openfluid::core::UnitClass_t& UnitsClass,
                                                   const unsigned int& ColsNbr,
                                                   const unsigned int& RowsNbr)
{
  openfluid::core::UnitID_t CurrID = 0;
  openfluid::core::UnitID_t ToID = 0;

  for (unsigned int i=0; i<RowsNbr;i++)
  {
    for (unsigned int j=0; j<ColsNbr;j++)
    {
      CurrID = 1+((ColsNbr*i)+j);
      OPENFLUID_AddUnit(UnitsClass,CurrID,1);
    }
  }

  for (unsigned int i=0; i<RowsNbr;i++)
  {
    for (unsigned int j=0; j<ColsNbr;j++)
    {
      CurrID = 1+((ColsNbr*i)+j);

      if (i == 0)
      {
        // top

        if (j == 0)
        {
          // top-left
          ToID = CurrID+1;
          OPENFLUID_AddFromToConnection(UnitsClass,CurrID,UnitsClass,ToID);

          ToID = CurrID+ColsNbr;
          OPENFLUID_AddFromToConnection(UnitsClass,CurrID,UnitsClass,ToID);

        }
        else
        {
          if (j == ColsNbr-1)
          {
            // top-right
            ToID = CurrID-1;
            OPENFLUID_AddFromToConnection(UnitsClass,CurrID,UnitsClass,ToID);

            ToID = CurrID+ColsNbr;
            OPENFLUID_AddFromToConnection(UnitsClass,CurrID,UnitsClass,ToID);

          }
          else
          {
            // top-center
            ToID = CurrID-1;
            OPENFLUID_AddFromToConnection(UnitsClass,CurrID,UnitsClass,ToID);

            ToID = CurrID+1;
            OPENFLUID_AddFromToConnection(UnitsClass,CurrID,UnitsClass,ToID);

            ToID = CurrID+ColsNbr;
            OPENFLUID_AddFromToConnection(UnitsClass,CurrID,UnitsClass,ToID);

          }
        }

      }
      else
      {
        if (i == RowsNbr-1)
        {
          // bottom

          if (j == 0)
          {
            // bottom-left
            ToID = CurrID+1;
            OPENFLUID_AddFromToConnection(UnitsClass,CurrID,UnitsClass,ToID);

            ToID = CurrID-ColsNbr;
            OPENFLUID_AddFromToConnection(UnitsClass,CurrID,UnitsClass,ToID);
          }
          else
          {
            if (j == ColsNbr-1)
            {
              // bottom-right
              ToID = CurrID-1;
              OPENFLUID_AddFromToConnection(UnitsClass,CurrID,UnitsClass,ToID);

              ToID = CurrID-ColsNbr;
              OPENFLUID_AddFromToConnection(UnitsClass,CurrID,UnitsClass,ToID);

            }
            else
            {
              // bottom-center
              ToID = CurrID-1;
              OPENFLUID_AddFromToConnection(UnitsClass,CurrID,UnitsClass,ToID);

              ToID = CurrID+1;
              OPENFLUID_AddFromToConnection(UnitsClass,CurrID,UnitsClass,ToID);

              ToID = CurrID-ColsNbr;
              OPENFLUID_AddFromToConnection(UnitsClass,CurrID,UnitsClass,ToID);
            }
          }

        }
        else
        {
          // middle

          if (j == 0)
          {
            // middle-left
            ToID = CurrID+1;
            OPENFLUID_AddFromToConnection(UnitsClass,CurrID,UnitsClass,ToID);

            ToID = CurrID-ColsNbr;
            OPENFLUID_AddFromToConnection(UnitsClass,CurrID,UnitsClass,ToID);

            ToID = CurrID+ColsNbr;
            OPENFLUID_AddFromToConnection(UnitsClass,CurrID,UnitsClass,ToID);

          }
          else
          {
            if (j == ColsNbr-1)
            {
              // middle-right
              ToID = CurrID-1;
              OPENFLUID_AddFromToConnection(UnitsClass,CurrID,UnitsClass,ToID);

              ToID = CurrID-ColsNbr;
              OPENFLUID_AddFromToConnection(UnitsClass,CurrID,UnitsClass,ToID);

              ToID = CurrID+ColsNbr;
              OPENFLUID_AddFromToConnection(UnitsClass,CurrID,UnitsClass,ToID);

            }
            else
            {
              // middle-center
              ToID = CurrID-1;
              OPENFLUID_AddFromToConnection(UnitsClass,CurrID,UnitsClass,ToID);

              ToID = CurrID+1;
              OPENFLUID_AddFromToConnection(UnitsClass,CurrID,UnitsClass,ToID);

              ToID = CurrID-ColsNbr;
              OPENFLUID_AddFromToConnection(UnitsClass,CurrID,UnitsClass,ToID);

              ToID = CurrID+ColsNbr;
              OPENFLUID_AddFromToConnection(UnitsClass,CurrID,UnitsClass,ToID);

            }
          }


        }
      }
    }
  }

//  throw openfluid::base::OFException("PluggableFunction::OPENFLUID_BuildUnitsMatrix : under construction");

}


// =====================================================================
// =====================================================================


void PluggableFunction::OPENFLUID_ExportUnitsGraphAsDotFile(const std::string& Filename)
{

  std::ofstream DotFile;
  std::string OutputDir;
  const openfluid::core::UnitsListByClassMap_t* UnitsByClass = mp_CoreData->getUnitsByClass();
  const openfluid::core::UnitsList_t* UnitsList = NULL;
  std::vector<openfluid::core::UnitClass_t> ClassVector;
  openfluid::core::Unit* TheUnit;

  openfluid::core::UnitsListByClassMap_t::const_iterator itUnitsClass;
  openfluid::core::UnitsList_t::const_iterator itUnitsList;

  OPENFLUID_GetRunEnvironment("dir.output",&OutputDir);

  DotFile.open(std::string(OutputDir+"/"+Filename).c_str());

  DotFile << "digraph landscape {" << "\n";
  DotFile << std::endl;
  DotFile << "label = \"Graph representation of the landscape\";" << "\n";
  DotFile << "fontsize = 24;" << "\n";
  DotFile << "\n";


  for (itUnitsClass=UnitsByClass->begin();itUnitsClass!=UnitsByClass->end();++itUnitsClass)
  {
    ClassVector.push_back((*itUnitsClass).first);

    UnitsList=((*itUnitsClass).second).getList();

    for (itUnitsList=UnitsList->begin();itUnitsList!=UnitsList->end();++itUnitsList)
    {
      TheUnit = const_cast<openfluid::core::Unit*>(&(*itUnitsList));
      std::string SrcClassStr = TheUnit->getClass();
      std::string SrcIDStr = "";
      openfluid::tools::ConvertValue(TheUnit->getID(),&SrcIDStr);
      DotFile << "\""+SrcClassStr+" #"+SrcIDStr+"\";" << "\n";
    }

  }

  DotFile << "\n";

  for (itUnitsClass=UnitsByClass->begin();itUnitsClass!=UnitsByClass->end();++itUnitsClass)
  {

    UnitsList=((*itUnitsClass).second).getList();

    for (itUnitsList=UnitsList->begin();itUnitsList!=UnitsList->end();++itUnitsList)
    {
      TheUnit = const_cast<openfluid::core::Unit*>(&(*itUnitsList));
      std::string SrcClassStr = TheUnit->getClass();
      std::string SrcIDStr = "";
      openfluid::tools::ConvertValue(TheUnit->getID(),&SrcIDStr);

      for (unsigned int i=0;i<ClassVector.size();i++)
      {
        const openfluid::core::UnitsPtrList_t* ToUnits = const_cast<openfluid::core::UnitsPtrList_t*>(TheUnit->getToUnits(ClassVector[i]));

        if (ToUnits != NULL)
        {
          std::string DestClassStr = ClassVector[i];
          openfluid::core::UnitsPtrList_t::const_iterator itToUnits;

          for (itToUnits=ToUnits->begin();itToUnits!=ToUnits->end();++itToUnits)
          {
            std::string DestIDStr = "";
            openfluid::tools::ConvertValue((*itToUnits)->getID(),&DestIDStr);

            DotFile << generateDotEdge(SrcClassStr,SrcIDStr,DestClassStr,DestIDStr,"") << "\n";

          }
        }

        const openfluid::core::UnitsPtrList_t* ParentUnits = const_cast<openfluid::core::UnitsPtrList_t*>(TheUnit->getParentUnits(ClassVector[i]));

        if (ParentUnits != NULL)
        {
          std::string DestClassStr = ClassVector[i];
          openfluid::core::UnitsPtrList_t::const_iterator itParentUnits;

          for (itParentUnits=ParentUnits->begin();itParentUnits!=ParentUnits->end();++itParentUnits)
          {
            std::string DestIDStr = "";
            openfluid::tools::ConvertValue((*itParentUnits)->getID(),&DestIDStr);

            DotFile << generateDotEdge(SrcClassStr,SrcIDStr,DestClassStr,DestIDStr,"[arrowhead=odiamond,color=grey,style=dashed]") << "\n";

          }
        }

      }

    }

  }

  DotFile << "\n";
  DotFile << "}" << "\n";

  DotFile.close();
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


// =====================================================================
// =====================================================================


void PluggableFunction::OPENFLUID_SetFunctionMaxThreads(const unsigned int& MaxNumThreads)
{
  if (MaxNumThreads > 0) m_MaxThreads = MaxNumThreads;
};


} } // namespace openfluid::base




