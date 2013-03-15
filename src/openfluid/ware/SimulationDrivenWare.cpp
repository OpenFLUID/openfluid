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
  \file SimulationDrivenWare.cpp
  \brief Implements ...

  \author Jean-Christophe FABRE <fabrejc@supagro.inra.fr>
 */


#include <openfluid/ware/SimulationDrivenWare.hpp>

namespace openfluid { namespace ware {


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
    throw openfluid::base::OFException("OpenFLUID framework","SimulationDrivenWare::OPENFLUID_GetBeginDate()","Simulation status is not set");

  return mp_SimStatus->getBeginDate();
}


// =====================================================================
// =====================================================================


openfluid::core::DateTime SimulationDrivenWare::OPENFLUID_GetEndDate() const
{
  if (mp_SimStatus == NULL)
    throw openfluid::base::OFException("OpenFLUID framework","SimulationDrivenWare::OPENFLUID_GetEndDate()","Simulation status is not set");

  return mp_SimStatus->getEndDate();
}


// =====================================================================
// =====================================================================


openfluid::core::DateTime SimulationDrivenWare::OPENFLUID_GetCurrentDate() const
{
  if (mp_SimStatus == NULL)
    throw openfluid::base::OFException("OpenFLUID framework","SimulationDrivenWare::OPENFLUID_GetCurrentDate()","Simulation status is not set");

  return mp_SimStatus->getCurrentDate();
}


// =====================================================================
// =====================================================================


openfluid::core::Duration_t SimulationDrivenWare::OPENFLUID_GetSimulationDuration() const
{
  if (mp_SimStatus == NULL)
    throw openfluid::base::OFException("OpenFLUID framework","SimulationDrivenWare::OPENFLUID_GetSimulationDuration()","Simulation status is not set");

  return mp_SimStatus->getSimulationDuration();
}


// =====================================================================
// =====================================================================


openfluid::core::Duration_t SimulationDrivenWare::OPENFLUID_GetDefaultDeltaT() const
{
  if (mp_SimStatus == NULL)
    throw openfluid::base::OFException("OpenFLUID framework","SimulationDrivenWare::OPENFLUID_GetDefaultDeltaT()","Simulation status is not set");

  return mp_SimStatus->getDefaultDeltaT();
}


// =====================================================================
// =====================================================================


openfluid::core::TimeIndex_t SimulationDrivenWare::OPENFLUID_GetCurrentTimeIndex() const
{
  if (mp_SimStatus == NULL)
    throw openfluid::base::OFException("OpenFLUID framework","SimulationDrivenWare::OPENFLUID_GetCurrentTimeIndex()","Simulation status is not set");

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
    throw openfluid::base::OFException("OpenFLUID framework","SimulationDrivenWare::OPENFLUID_GetPreviousRunTimeIndex()","Simulation status is not set");

  return m_PreviousTimeIndex;
}


// =====================================================================
// =====================================================================


openfluid::base::SimulationStatus::SimulationStage SimulationDrivenWare::OPENFLUID_GetCurrentStage() const
{
  if (mp_SimStatus == NULL)
    throw openfluid::base::OFException("OpenFLUID framework","SimulationDrivenWare::OPENFLUID_GetCurrentStage()","Simulation status is not set");

  return mp_SimStatus->getCurrentStage();
}


// =====================================================================
// =====================================================================


void SimulationDrivenWare::OPENFLUID_RaiseWarning(std::string Sender, std::string Msg)
{
  if (mp_SimStatus == NULL || mp_SimLogger == NULL)
    throw openfluid::base::OFException("OpenFLUID framework","SimulationDrivenWare::OPENFLUID_RaiseWarning()","Simulation status or execution messages not set");

  if (mp_SimStatus->getCurrentStage() == openfluid::base::SimulationStatus::INITIALIZERUN ||
      mp_SimStatus->getCurrentStage() == openfluid::base::SimulationStatus::RUNSTEP)
    mp_SimLogger->addWarning(Sender,mp_SimStatus->getCurrentTimeIndex(),Msg);
  else
    mp_SimLogger->addWarning(Sender,Msg);
}


// =====================================================================
// =====================================================================


void SimulationDrivenWare::OPENFLUID_RaiseWarning(std::string Sender, std::string Source, std::string Msg)
{
  if (mp_SimStatus == NULL || mp_SimLogger == NULL)
    throw openfluid::base::OFException("OpenFLUID framework","SimulationDrivenWare::OPENFLUID_RaiseWarning()","Simulation status or execution messages not set");

  if (mp_SimStatus->getCurrentStage() == openfluid::base::SimulationStatus::INITIALIZERUN ||
      mp_SimStatus->getCurrentStage() == openfluid::base::SimulationStatus::RUNSTEP)
    mp_SimLogger->addWarning(Sender,Source,mp_SimStatus->getCurrentTimeIndex(),Msg);
  else
    mp_SimLogger->addWarning(Sender,Source,Msg);
}

// =====================================================================
// =====================================================================


void SimulationDrivenWare::OPENFLUID_RaiseError(std::string Sender, std::string Msg)
{
  if (mp_SimStatus->getCurrentStage() == openfluid::base::SimulationStatus::INITIALIZERUN ||
      mp_SimStatus->getCurrentStage() == openfluid::base::SimulationStatus::RUNSTEP)
    throw openfluid::base::OFException(Sender,mp_SimStatus->getCurrentTimeIndex(),Msg);
  else
    throw openfluid::base::OFException(Sender,Msg);
}


// =====================================================================
// =====================================================================


void SimulationDrivenWare::OPENFLUID_RaiseError(std::string Sender, std::string Source, std::string Msg)
{
  if (mp_SimStatus->getCurrentStage() == openfluid::base::SimulationStatus::INITIALIZERUN ||
      mp_SimStatus->getCurrentStage() == openfluid::base::SimulationStatus::RUNSTEP)
    throw openfluid::base::OFException(Sender,Source,mp_SimStatus->getCurrentTimeIndex(),Msg);
  else
    throw openfluid::base::OFException(Sender,Source,Msg);
}



}  }  //namespaces
