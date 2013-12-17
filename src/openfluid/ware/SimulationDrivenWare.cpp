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
  \file SimulationDrivenWare.cpp
  \brief Implements ...

  \author Jean-Christophe FABRE <fabrejc@supagro.inra.fr>
 */


#include <openfluid/ware/SimulationDrivenWare.hpp>
#include <openfluid/config.hpp>

namespace openfluid { namespace ware {



void SimulationDrivenWare::linkToSimulation(const openfluid::base::SimulationStatus* SimStatus)
{
  if (!isLinked()) mp_SimStatus = SimStatus;
}


// =====================================================================
// =====================================================================


openfluid::core::DateTime SimulationDrivenWare::OPENFLUID_GetBeginDate() const
{
  if (mp_SimStatus == NULL)
    throw openfluid::base::FrameworkException("SimulationDrivenWare::OPENFLUID_GetBeginDate()","Simulation status is not set");

  return mp_SimStatus->getBeginDate();
}


// =====================================================================
// =====================================================================


openfluid::core::DateTime SimulationDrivenWare::OPENFLUID_GetEndDate() const
{
  if (mp_SimStatus == NULL)
    throw openfluid::base::FrameworkException("SimulationDrivenWare::OPENFLUID_GetEndDate()","Simulation status is not set");

  return mp_SimStatus->getEndDate();
}


// =====================================================================
// =====================================================================


openfluid::core::DateTime SimulationDrivenWare::OPENFLUID_GetCurrentDate() const
{
  if (mp_SimStatus == NULL)
    throw openfluid::base::FrameworkException("SimulationDrivenWare::OPENFLUID_GetCurrentDate()","Simulation status is not set");

  return mp_SimStatus->getCurrentDate();
}


// =====================================================================
// =====================================================================


openfluid::core::Duration_t SimulationDrivenWare::OPENFLUID_GetSimulationDuration() const
{
  if (mp_SimStatus == NULL)
    throw openfluid::base::FrameworkException("SimulationDrivenWare::OPENFLUID_GetSimulationDuration()","Simulation status is not set");

  return mp_SimStatus->getSimulationDuration();
}


// =====================================================================
// =====================================================================


openfluid::core::Duration_t SimulationDrivenWare::OPENFLUID_GetDefaultDeltaT() const
{
  if (mp_SimStatus == NULL)
    throw openfluid::base::FrameworkException("SimulationDrivenWare::OPENFLUID_GetDefaultDeltaT()","Simulation status is not set");

  return mp_SimStatus->getDefaultDeltaT();
}


// =====================================================================
// =====================================================================


openfluid::core::TimeIndex_t SimulationDrivenWare::OPENFLUID_GetCurrentTimeIndex() const
{
  REQUIRE_SIMULATION_STAGE_GE(openfluid::base::SimulationStatus::INITIALIZERUN,
                           "SimulationDrivenWare::OPENFLUID_GetCurrentRunTimeIndex",
                           "Current run time index cannot be accessed before INITIALIZERUN");

  if (mp_SimStatus == NULL)
    throw openfluid::base::FrameworkException("SimulationDrivenWare::OPENFLUID_GetCurrentTimeIndex()","Simulation status is not set");

  return mp_SimStatus->getCurrentTimeIndex();
}


// =====================================================================
// =====================================================================


openfluid::core::TimeIndex_t SimulationDrivenWare::OPENFLUID_GetPreviousRunTimeIndex() const
{
  REQUIRE_SIMULATION_STAGE_GE(openfluid::base::SimulationStatus::RUNSTEP,
                           "SimulationDrivenWare::OPENFLUID_GetPreviousRunTimeIndex",
                           "Previous run time index cannot be accessed outside RUNSTEP or FINALIZERUN stages");

  if (mp_SimStatus == NULL)
    throw openfluid::base::FrameworkException("SimulationDrivenWare::OPENFLUID_GetPreviousRunTimeIndex()","Simulation status is not set");

  return m_PreviousTimeIndex;
}


// =====================================================================
// =====================================================================


openfluid::base::SimulationStatus::SimulationStage SimulationDrivenWare::OPENFLUID_GetCurrentStage() const
{
  if (mp_SimStatus == NULL)
    throw openfluid::base::FrameworkException("SimulationDrivenWare::OPENFLUID_GetCurrentStage()","Simulation status is not set");

  return mp_SimStatus->getCurrentStage();
}


// =====================================================================
// =====================================================================


openfluid::base::SimulationStatus::SchedulingConstraint SimulationDrivenWare::OPENFLUID_GetSchedulingConstraint() const
{
  if (mp_SimStatus == NULL)
    throw openfluid::base::FrameworkException("SimulationDrivenWare::OPENFLUID_GetSchedulingConstraint()","Simulation status is not set");

  return mp_SimStatus->getSchedulingConstraint();
}


// =====================================================================
// =====================================================================


void SimulationDrivenWare::OPENFLUID_RaiseWarning(const std::string& Msg)
{
  std::string Source;
  WareException::updateSourceInfo(m_WareType,OPENFLUID_GetCurrentStage(),Source);

  if (mp_SimStatus == NULL || mp_SimLogger == NULL)
    throw openfluid::base::FrameworkException("SimulationDrivenWare::OPENFLUID_RaiseWarning()","Simulation status or execution messages not set");

  if (mp_SimStatus->getCurrentStage() == openfluid::base::SimulationStatus::INITIALIZERUN ||
      mp_SimStatus->getCurrentStage() == openfluid::base::SimulationStatus::RUNSTEP)
    mp_SimLogger->addWarning(OPENFLUID_GetWareID(),Source,mp_SimStatus->getCurrentTimeIndex(),Msg);
  else
    mp_SimLogger->addWarning(OPENFLUID_GetWareID(),Source,Msg);
}


// =====================================================================
// =====================================================================


void SimulationDrivenWare::OPENFLUID_RaiseWarning(const std::string& Source, const std::string& Msg)
{
  if (mp_SimStatus == NULL || mp_SimLogger == NULL)
    throw openfluid::base::FrameworkException("SimulationDrivenWare::OPENFLUID_RaiseWarning()","Simulation status or execution messages not set");

  if (mp_SimStatus->getCurrentStage() == openfluid::base::SimulationStatus::INITIALIZERUN ||
      mp_SimStatus->getCurrentStage() == openfluid::base::SimulationStatus::RUNSTEP)
    mp_SimLogger->addWarning(OPENFLUID_GetWareID(),Source,mp_SimStatus->getCurrentTimeIndex(),Msg);
  else
    mp_SimLogger->addWarning(OPENFLUID_GetWareID(),Source,Msg);
}


// =====================================================================
// =====================================================================


void SimulationDrivenWare::OPENFLUID_RaiseError(const std::string& Msg)
{
  if (mp_SimStatus->getCurrentStage() == openfluid::base::SimulationStatus::INITIALIZERUN ||
      mp_SimStatus->getCurrentStage() == openfluid::base::SimulationStatus::RUNSTEP)
    throw WareException(OPENFLUID_GetWareID(),m_WareType,OPENFLUID_GetCurrentStage(),mp_SimStatus->getCurrentTimeIndex(),Msg);
  else
    throw WareException(OPENFLUID_GetWareID(),m_WareType,OPENFLUID_GetCurrentStage(),Msg);
}

// =====================================================================
// =====================================================================


void SimulationDrivenWare::OPENFLUID_RaiseError(const std::string& Source, const std::string& Msg)
{
  if (mp_SimStatus->getCurrentStage() == openfluid::base::SimulationStatus::INITIALIZERUN ||
      mp_SimStatus->getCurrentStage() == openfluid::base::SimulationStatus::RUNSTEP)
    throw WareException(OPENFLUID_GetWareID(),m_WareType,Source,mp_SimStatus->getCurrentTimeIndex(),Msg);
  else
    throw WareException(OPENFLUID_GetWareID(),m_WareType,Source,Msg);
}


// =====================================================================
// =====================================================================


void SimulationDrivenWare::initializeWare(const WareID_t& ID)
{
  if (m_Initialized) return;

  PluggableWare::initializeWare(ID);

  // initialize loggers
  std::string LogFile;
  std::string LogDir;
  std::string LogFileSuffix = "_undefined";

  if (m_WareType == SIMULATOR) LogFileSuffix = openfluid::config::SIMULATORS_PLUGINS_SUFFIX;
  if (m_WareType == OBSERVER) LogFileSuffix = openfluid::config::OBSERVERS_PLUGINS_SUFFIX;

  OPENFLUID_GetRunEnvironment("dir.output",LogDir);
  LogFile = boost::filesystem::path(LogDir + "/" + OPENFLUID_GetWareID() + LogFileSuffix + ".log").string();

  OPENFLUID_Logger.open(LogFile.c_str());

  m_Initialized = true;
}


// =====================================================================
// =====================================================================


void SimulationDrivenWare::finalizeWare()
{
  OPENFLUID_Logger.close();
}



}  }  //namespaces
