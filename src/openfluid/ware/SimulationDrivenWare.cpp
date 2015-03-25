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

  @author Jean-Christophe FABRE <jean-christophe.fabre@supagro.inra.fr>
 */


#include <openfluid/ware/SimulationDrivenWare.hpp>
#include <openfluid/config.hpp>

namespace openfluid { namespace ware {


openfluid::base::ExceptionContext SimulationDrivenWare::computeWareContext() const
{
  openfluid::base::ExceptionContext Context;

  if (mp_SimStatus->getCurrentStage() == openfluid::base::SimulationStatus::INITIALIZERUN ||
      mp_SimStatus->getCurrentStage() == openfluid::base::SimulationStatus::RUNSTEP)
  {
    Context = openfluid::ware::WareException::computeContext(m_WareType,
                                                             OPENFLUID_GetWareID(),
                                                             mp_SimStatus->getCurrentStage(),
                                                             mp_SimStatus->getCurrentTimeIndex());
  }
  else
  {
    Context = openfluid::ware::WareException::computeContext(m_WareType,
                                                             OPENFLUID_GetWareID(),
                                                             mp_SimStatus->getCurrentStage());
  }

  return Context;
}


// =====================================================================
// =====================================================================


void SimulationDrivenWare::appendToLog(openfluid::tools::FileLogger::LogType LType, const std::string& Msg) const
{
  if (mp_SimStatus == NULL || mp_SimLogger == NULL)
    throw openfluid::base::FrameworkException(OPENFLUID_CODE_LOCATION,
                                              "Simulation status or execution messages not set");

  mp_SimLogger->add(LType,computeWareContext().toString(),Msg);
}


// =====================================================================
// =====================================================================


void SimulationDrivenWare::displayToConsole(openfluid::tools::FileLogger::LogType LType, const std::string& Msg) const
{
  if (mp_SimStatus == NULL || mp_SimLogger == NULL)
    throw openfluid::base::FrameworkException(OPENFLUID_CODE_LOCATION,
                                              "Simulation status or execution messages not set");

  std::cout << "[" << openfluid::tools::FileLogger::logTypeToString(LType) << "] " << Msg << std::endl;
}


// =====================================================================
// =====================================================================


void SimulationDrivenWare::linkToSimulation(const openfluid::base::SimulationStatus* SimStatus)
{
  if (!isLinked()) mp_SimStatus = SimStatus;
}


// =====================================================================
// =====================================================================


openfluid::core::DateTime SimulationDrivenWare::OPENFLUID_GetBeginDate() const
{
  if (mp_SimStatus == NULL)
    throw openfluid::base::FrameworkException(OPENFLUID_CODE_LOCATION,
                                              "Simulation status is not set");

  return mp_SimStatus->getBeginDate();
}


// =====================================================================
// =====================================================================


openfluid::core::DateTime SimulationDrivenWare::OPENFLUID_GetEndDate() const
{
  if (mp_SimStatus == NULL)
    throw openfluid::base::FrameworkException(OPENFLUID_CODE_LOCATION,
                                              "Simulation status is not set");

  return mp_SimStatus->getEndDate();
}


// =====================================================================
// =====================================================================


openfluid::core::DateTime SimulationDrivenWare::OPENFLUID_GetCurrentDate() const
{
  if (mp_SimStatus == NULL)
    throw openfluid::base::FrameworkException(OPENFLUID_CODE_LOCATION,
                                              "Simulation status is not set");

  return mp_SimStatus->getCurrentDate();
}


// =====================================================================
// =====================================================================


openfluid::core::Duration_t SimulationDrivenWare::OPENFLUID_GetSimulationDuration() const
{
  if (mp_SimStatus == NULL)
    throw openfluid::base::FrameworkException(OPENFLUID_CODE_LOCATION,
                                              "Simulation status is not set");

  return mp_SimStatus->getSimulationDuration();
}


// =====================================================================
// =====================================================================


openfluid::core::Duration_t SimulationDrivenWare::OPENFLUID_GetDefaultDeltaT() const
{
  if (mp_SimStatus == NULL)
    throw openfluid::base::FrameworkException(OPENFLUID_CODE_LOCATION,
                                              "Simulation status is not set");

  return mp_SimStatus->getDefaultDeltaT();
}


// =====================================================================
// =====================================================================


openfluid::core::TimeIndex_t SimulationDrivenWare::OPENFLUID_GetCurrentTimeIndex() const
{
  REQUIRE_SIMULATION_STAGE_GE(openfluid::base::SimulationStatus::INITIALIZERUN,
                              OPENFLUID_CODE_LOCATION,
                           "Current run time index cannot be accessed before INITIALIZERUN");

  if (mp_SimStatus == NULL)
    throw openfluid::base::FrameworkException(OPENFLUID_CODE_LOCATION,
                                              "Simulation status is not set");

  return mp_SimStatus->getCurrentTimeIndex();
}


// =====================================================================
// =====================================================================


openfluid::core::TimeIndex_t SimulationDrivenWare::OPENFLUID_GetPreviousRunTimeIndex() const
{
  REQUIRE_SIMULATION_STAGE_GE(openfluid::base::SimulationStatus::RUNSTEP,
                              OPENFLUID_CODE_LOCATION,
                              "Previous run time index cannot be accessed outside RUNSTEP or FINALIZERUN stages");

  if (mp_SimStatus == NULL)
    throw openfluid::base::FrameworkException(OPENFLUID_CODE_LOCATION,
                                              "Simulation status is not set");

  return m_PreviousTimeIndex;
}


// =====================================================================
// =====================================================================


openfluid::base::SimulationStatus::SimulationStage SimulationDrivenWare::OPENFLUID_GetCurrentStage() const
{
  if (mp_SimStatus == NULL)
    throw openfluid::base::FrameworkException(OPENFLUID_CODE_LOCATION,
                                              "Simulation status is not set");

  return mp_SimStatus->getCurrentStage();
}


// =====================================================================
// =====================================================================


openfluid::base::SimulationStatus::SchedulingConstraint SimulationDrivenWare::OPENFLUID_GetSchedulingConstraint() const
{
  if (mp_SimStatus == NULL)
    throw openfluid::base::FrameworkException(OPENFLUID_CODE_LOCATION,
                                              "Simulation status is not set");

  return mp_SimStatus->getSchedulingConstraint();
}


// =====================================================================
// =====================================================================


void SimulationDrivenWare::OPENFLUID_RaiseWarning(const std::string& Msg)
{
  appendToLog(openfluid::tools::FileLogger::LOG_WARNING,Msg);
}


// =====================================================================
// =====================================================================


void SimulationDrivenWare::OPENFLUID_RaiseWarning(const std::string& /*Source*/, const std::string& Msg)
{
  appendToLog(openfluid::tools::FileLogger::LOG_WARNING,Msg);
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
  OPENFLUID_RaiseError(Msg);
}


// =====================================================================
// =====================================================================


void SimulationDrivenWare::initializeWare(const WareID_t& ID)
{
  if (m_Initialized) return;

  PluggableWare::initializeWare(ID);

  m_Initialized = true;
}


// =====================================================================
// =====================================================================


void SimulationDrivenWare::finalizeWare()
{

}



}  }  //namespaces
