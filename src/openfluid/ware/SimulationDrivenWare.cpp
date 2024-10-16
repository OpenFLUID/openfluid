/*

  This file is part of OpenFLUID software
  Copyright(c) 2007, INRA - Montpellier SupAgro


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
  along with OpenFLUID. If not, see <http://www.gnu.org/licenses/>.


 == Other Usage ==

  Other Usage means a use of OpenFLUID that is inconsistent with the GPL
  license, and requires a written agreement between You and INRA.
  Licensees for Other Usage of OpenFLUID may use this file in accordance
  with the terms contained in the written agreement between You and INRA.
  
*/


/**
  @file SimulationDrivenWare.cpp

  @author Jean-Christophe FABRE <jean-christophe.fabre@inra.fr>
 */


#include <openfluid/config.hpp>
#include <openfluid/ware/SimulationDrivenWare.hpp>


namespace openfluid { namespace ware {


openfluid::base::ExceptionContext SimulationDrivenWare::computeWareContext(const std::string& CodeLoc) const
{
  openfluid::base::ExceptionContext Context;


  if (mp_SimStatus->getCurrentStage() == openfluid::base::SimulationStatus::INITIALIZERUN ||
      mp_SimStatus->getCurrentStage() == openfluid::base::SimulationStatus::RUNSTEP)
  {
    Context = openfluid::ware::WareException::computeContext(OPENFLUID_GetWareType(),
                                                             OPENFLUID_GetWareID(),
                                                             mp_SimStatus->getCurrentStage(),
                                                             mp_SimStatus->getCurrentTimeIndex());
  }
  else
  {
    Context = openfluid::ware::WareException::computeContext(OPENFLUID_GetWareType(),
                                                             OPENFLUID_GetWareID(),
                                                             mp_SimStatus->getCurrentStage());
  }

  if (!CodeLoc.empty())
  {
    Context.addCodeLocation(CodeLoc);
  }

  return Context;
}


// =====================================================================
// =====================================================================


openfluid::base::ExceptionContext SimulationDrivenWare::computeFrameworkContext(const std::string& CodeLoc) const
{
  openfluid::base::ExceptionContext Context = computeWareContext(CodeLoc);
  Context["source"] = "framework";

  return Context;
}


// =====================================================================
// =====================================================================


void SimulationDrivenWare::appendToLog(openfluid::tools::FileLogger::LogType LType, const std::string& Msg) const
{
  if (mp_SimStatus == nullptr || mp_SimLogger == nullptr)
  {
    throw openfluid::base::FrameworkException(computeFrameworkContext(OPENFLUID_CODE_LOCATION),
                                              "Simulation status or execution messages not set");
  }

  mp_SimLogger->add(LType,computeWareContext().toString(),Msg);
}


// =====================================================================
// =====================================================================


void SimulationDrivenWare::displayToConsole(openfluid::tools::FileLogger::LogType LType, const std::string& Msg) const
{
  if (mp_SimStatus == nullptr || mp_SimLogger == nullptr)
  {
    throw openfluid::base::FrameworkException(computeFrameworkContext(OPENFLUID_CODE_LOCATION),
                                              "Simulation status or execution messages not set");
  }

  std::cout << "[" << openfluid::tools::FileLogger::logTypeToString(LType) << "] " << Msg << std::endl;
}


// =====================================================================
// =====================================================================


void SimulationDrivenWare::linkToSimulation(const openfluid::base::SimulationStatus* SimStatus)
{
  if (!isLinked())
  {
    mp_SimStatus = SimStatus;
  }
}


// =====================================================================
// =====================================================================


openfluid::core::DateTime SimulationDrivenWare::OPENFLUID_GetBeginDate() const
{
  if (mp_SimStatus == nullptr)
  {
    throw openfluid::base::FrameworkException(computeFrameworkContext(OPENFLUID_CODE_LOCATION),
                                              "Simulation status is not set");
  }

  return mp_SimStatus->getBeginDate();
}


// =====================================================================
// =====================================================================


openfluid::core::DateTime SimulationDrivenWare::OPENFLUID_GetEndDate() const
{
  if (mp_SimStatus == nullptr)
  {
    throw openfluid::base::FrameworkException(computeFrameworkContext(OPENFLUID_CODE_LOCATION),
                                              "Simulation status is not set");
  }

  return mp_SimStatus->getEndDate();
}


// =====================================================================
// =====================================================================


openfluid::core::DateTime SimulationDrivenWare::OPENFLUID_GetCurrentDate() const
{
  if (mp_SimStatus == nullptr)
  {
    throw openfluid::base::FrameworkException(computeFrameworkContext(OPENFLUID_CODE_LOCATION),
                                              "Simulation status is not set");
  }

  return mp_SimStatus->getCurrentDate();
}


// =====================================================================
// =====================================================================


openfluid::core::Duration_t SimulationDrivenWare::OPENFLUID_GetSimulationDuration() const
{
  if (mp_SimStatus == nullptr)
  {
    throw openfluid::base::FrameworkException(computeFrameworkContext(OPENFLUID_CODE_LOCATION),
                                              "Simulation status is not set");
  }

  return mp_SimStatus->getSimulationDuration();
}


// =====================================================================
// =====================================================================


openfluid::core::Duration_t SimulationDrivenWare::OPENFLUID_GetDefaultDeltaT() const
{
  if (mp_SimStatus == nullptr)
  {
    throw openfluid::base::FrameworkException(computeFrameworkContext(OPENFLUID_CODE_LOCATION),
                                              "Simulation status is not set");
  }

  return mp_SimStatus->getDefaultDeltaT();
}


// =====================================================================
// =====================================================================


openfluid::core::TimeIndex_t SimulationDrivenWare::OPENFLUID_GetCurrentTimeIndex() const
{
  REQUIRE_SIMULATION_STAGE_GE(openfluid::base::SimulationStatus::INITIALIZERUN,
                              "Current run time index cannot be accessed before INITIALIZERUN");

  if (mp_SimStatus == nullptr)
  {
    throw openfluid::base::FrameworkException(computeFrameworkContext(OPENFLUID_CODE_LOCATION),
                                              "Simulation status is not set");
  }

  return mp_SimStatus->getCurrentTimeIndex();
}


// =====================================================================
// =====================================================================


openfluid::core::TimeIndex_t SimulationDrivenWare::OPENFLUID_GetPreviousRunTimeIndex() const
{
  REQUIRE_SIMULATION_STAGE_GE(openfluid::base::SimulationStatus::RUNSTEP,
                              "Previous run time index cannot be accessed outside RUNSTEP or FINALIZERUN stages");

  if (mp_SimStatus == nullptr)
  {
    throw openfluid::base::FrameworkException(computeFrameworkContext(OPENFLUID_CODE_LOCATION),
                                              "Simulation status is not set");
  }

  return m_PreviousTimeIndex;
}


// =====================================================================
// =====================================================================


openfluid::base::SimulationStatus::SimulationStage SimulationDrivenWare::OPENFLUID_GetCurrentStage() const
{
  if (mp_SimStatus == nullptr)
  {
    throw openfluid::base::FrameworkException(computeFrameworkContext(OPENFLUID_CODE_LOCATION),
                                              "Simulation status is not set");
  }

  return mp_SimStatus->getCurrentStage();
}


// =====================================================================
// =====================================================================


std::string SimulationDrivenWare::OPENFLUID_GetCurrentStageAsString() const
{
  if (mp_SimStatus == nullptr)
  {
    throw openfluid::base::FrameworkException(computeFrameworkContext(OPENFLUID_CODE_LOCATION),
                                              "Simulation status is not set");
  }

  return mp_SimStatus->getCurrentStageAsString();
}


// =====================================================================
// =====================================================================


openfluid::base::SimulationStatus::SchedulingConstraint SimulationDrivenWare::OPENFLUID_GetSchedulingConstraint() const
{
  if (mp_SimStatus == nullptr)
  {
    throw openfluid::base::FrameworkException(computeFrameworkContext(OPENFLUID_CODE_LOCATION),
                                              "Simulation status is not set");
  }

  return mp_SimStatus->getSchedulingConstraint();
}


// =====================================================================
// =====================================================================


void SimulationDrivenWare::OPENFLUID_RaiseWarning(const std::string& Msg)
{
  appendToLog(openfluid::tools::FileLogger::LogType::WARNING_MSG,Msg);
}


// =====================================================================
// =====================================================================


void SimulationDrivenWare::OPENFLUID_RaiseWarning(const std::string& /*Source*/, const std::string& Msg)
{
  appendToLog(openfluid::tools::FileLogger::LogType::WARNING_MSG,Msg);
}


// =====================================================================
// =====================================================================


void SimulationDrivenWare::OPENFLUID_RaiseError(const std::string& Msg)
{
  throw WareException(computeWareContext(),Msg);
}


// =====================================================================
// =====================================================================


void SimulationDrivenWare::OPENFLUID_RaiseError(const std::string& /*Source*/, const std::string& Msg)
{
  SimulationDrivenWare::OPENFLUID_RaiseError(Msg);
}


// =====================================================================
// =====================================================================


void SimulationDrivenWare::initializeWare(const WareID_t& ID)
{
  if (m_Initialized)
  {
    return;
  }

  PluggableWare::initializeWare(ID);

  m_Initialized = true;
}


// =====================================================================
// =====================================================================


void SimulationDrivenWare::finalizeWare()
{

}


// =====================================================================
// =====================================================================


bool SimulationDrivenWare::OPENFLUID_IsWareParameterExist(const openfluid::ware::WareParams_t& Params,
                                                             const openfluid::ware::WareParamKey_t& ParamName) const
{
  return (Params.find(ParamName) != Params.end());
}


// =====================================================================
// =====================================================================


openfluid::core::StringValue SimulationDrivenWare::OPENFLUID_GetWareParameter(
                                                 const openfluid::ware::WareParams_t& Params,
                                                 const openfluid::ware::WareParamKey_t& ParamName) const
{
  openfluid::ware::WareParams_t::const_iterator it = Params.find(ParamName);

  if (it != Params.end())
  {
    return it->second;
  }
  else
  {
    throw openfluid::base::FrameworkException(computeFrameworkContext(OPENFLUID_CODE_LOCATION),
                                              "Parameter "+ParamName+ " does not exist");
  }
}


// =====================================================================
// =====================================================================


bool SimulationDrivenWare::OPENFLUID_GetWareParameter(const openfluid::ware::WareParams_t& Params,
                                                         const openfluid::ware::WareParamKey_t& ParamName,
                                                         openfluid::core::StringValue& Val) const
{
  openfluid::ware::WareParams_t::const_iterator it = Params.find(ParamName);

  if (it != Params.end())
  {
    Val = it->second;
    return true;
  }

  return false;
}


// =====================================================================
// =====================================================================


bool SimulationDrivenWare::OPENFLUID_GetWareParameter(const openfluid::ware::WareParams_t& Params,
                                                         const openfluid::ware::WareParamKey_t& ParamName,
                                                         openfluid::core::DoubleValue& Val) const
{
  openfluid::ware::WareParams_t::const_iterator it = Params.find(ParamName);

  if (it != Params.end())
  {
    return it->second.toDoubleValue(Val);
  }

  return false;
}


// =====================================================================
// =====================================================================


bool SimulationDrivenWare::OPENFLUID_GetWareParameter(const openfluid::ware::WareParams_t& Params,
                                                       const openfluid::ware::WareParamKey_t& ParamName,
                                                       openfluid::core::VectorValue& Val) const
{
  openfluid::ware::WareParams_t::const_iterator it = Params.find(ParamName);

  if (it != Params.end())
  {
    return it->second.toVectorValue(Val);
  }

  return false;
}


// =====================================================================
// =====================================================================


bool SimulationDrivenWare::OPENFLUID_GetWareParameter(const openfluid::ware::WareParams_t& Params,
                                                         const openfluid::ware::WareParamKey_t& ParamName,
                                                         openfluid::core::MatrixValue& Val) const
{
  openfluid::ware::WareParams_t::const_iterator it = Params.find(ParamName);

  if (it != Params.end())
  {
    return it->second.toMatrixValue(Val);
  }

  return false;
}


// =====================================================================
// =====================================================================


bool SimulationDrivenWare::OPENFLUID_GetWareParameter(const openfluid::ware::WareParams_t& Params,
                                                         const openfluid::ware::WareParamKey_t& ParamName,
                                                         bool& Val) const
{
  openfluid::ware::WareParams_t::const_iterator it = Params.find(ParamName);

  if (it != Params.end())
  {
    return it->second.toBoolean(Val);
  }

  return false;
}


// =====================================================================
// =====================================================================


bool SimulationDrivenWare::OPENFLUID_GetWareParameter(const openfluid::ware::WareParams_t& Params,
                                                         const openfluid::ware::WareParamKey_t& ParamName,
                                                         double& Val) const
{
  openfluid::ware::WareParams_t::const_iterator it = Params.find(ParamName);

  if (it != Params.end())
  {
    return it->second.toDouble(Val);
  }

  return false;
}


// =====================================================================
// =====================================================================


bool SimulationDrivenWare::OPENFLUID_GetWareParameter(const openfluid::ware::WareParams_t& Params,
                                                         const openfluid::ware::WareParamKey_t& ParamName,
                                                         long& Val) const
{
  openfluid::ware::WareParams_t::const_iterator it = Params.find(ParamName);

  if (it != Params.end())
  {
    return it->second.toInteger(Val);
  }

  return false;
}


// =====================================================================
// =====================================================================


bool SimulationDrivenWare::OPENFLUID_GetWareParameter(const openfluid::ware::WareParams_t& Params,
                                                         const openfluid::ware::WareParamKey_t& ParamName,
                                                         float& Val) const
{
  openfluid::ware::WareParams_t::const_iterator it = Params.find(ParamName);

  if (it != Params.end())
  {
    double TmpDbl;

    if(it->second.toDouble(TmpDbl))
    {
      Val = static_cast<float>(TmpDbl);
      return true;
    }
  }

  return false;
}


// =====================================================================
// =====================================================================


bool SimulationDrivenWare::OPENFLUID_GetWareParameter(const openfluid::ware::WareParams_t& Params,
                                                         const openfluid::ware::WareParamKey_t& ParamName,
                                                         int& Val) const
{
  openfluid::ware::WareParams_t::const_iterator it = Params.find(ParamName);

  if (it != Params.end())
  {
    long TmpInt;

    if(it->second.toInteger(TmpInt))
    {
      Val = static_cast<int>(TmpInt);
      return true;
    }
  }

  return false;
}


// =====================================================================
// =====================================================================


bool SimulationDrivenWare::OPENFLUID_GetWareParameter(const openfluid::ware::WareParams_t& Params,
                                                         const openfluid::ware::WareParamKey_t& ParamName,
                                                         std::string& Value) const
{
  openfluid::ware::WareParams_t::const_iterator it = Params.find(ParamName);

  if (it != Params.end())
  {
    Value = it->second.data();

    return true;
  }

  return false;
}


// =====================================================================
// =====================================================================


bool SimulationDrivenWare::OPENFLUID_GetWareParameter(const openfluid::ware::WareParams_t& Params,
                                                         const openfluid::ware::WareParamKey_t& ParamName,
                                                         std::vector<std::string>& Vals) const
{
  openfluid::ware::WareParams_t::const_iterator it = Params.find(ParamName);

  if (it != Params.end())
  {
    std::vector<std::string> Tokens = openfluid::tools::split(it->second.data(),";");

    Vals.clear();

    for (unsigned int i=0;i<Tokens.size();i++)
    {
      Vals.push_back(Tokens[i]);
    }

    return true;
  }

  return false;
}


// =====================================================================
// =====================================================================


bool SimulationDrivenWare::OPENFLUID_GetWareParameter(const openfluid::ware::WareParams_t& Params,
                                                         const openfluid::ware::WareParamKey_t& ParamName,
                                                         std::vector<double>& Vals) const
{
  bool IsOK = false;

  openfluid::ware::WareParams_t::const_iterator it = Params.find(ParamName);

  if (it != Params.end())
  {
    openfluid::core::VectorValue Vect;

    IsOK = it->second.toVectorValue(Vect);

    if(IsOK)
    {
      Vals.clear();

      for (unsigned long i=0;i<Vect.size();i++)
      {
        Vals.push_back(Vect[i]);
      }
    }
  }

  return IsOK;
}


// =====================================================================
// =====================================================================


bool SimulationDrivenWare::OPENFLUID_GetWareParameter(const openfluid::ware::WareParams_t& Params,
                                                         const openfluid::ware::WareParamKey_t& ParamName,
                                                         std::vector<long>& Vals) const
{
  bool IsOK = false;

  openfluid::ware::WareParams_t::const_iterator it = Params.find(ParamName);

  if (it != Params.end())
  {
    openfluid::core::VectorValue Vect;

    IsOK = it->second.toVectorValue(Vect);

    if(IsOK)
    {
      Vals.clear();

      for (unsigned long i=0;i<Vect.size();i++)
      {
        Vals.push_back(static_cast<int>(Vect[i]));
      }
    }
  }

  return IsOK;
}


}  }  //namespaces
