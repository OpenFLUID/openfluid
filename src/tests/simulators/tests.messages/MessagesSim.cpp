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
  @file MessagesSim.cpp

  @author Jean-Christophe FABRE <jean-christophe.fabre@inra.fr>
*/


#include <openfluid/tools/DataHelpers.hpp>

#include "MessagesSim.h"


DEFINE_SIMULATOR_CLASS(MessagesSimulator)


// =====================================================================
// =====================================================================


BEGIN_SIMULATOR_SIGNATURE("tests.messages")

  DECLARE_NAME("test simulator for messages");
  DECLARE_DESCRIPTION("");

  DECLARE_VERSION("1.0");
  DECLARE_SDKVERSION;
  DECLARE_STATUS(openfluid::ware::EXPERIMENTAL);

  DECLARE_DOMAIN("");
  DECLARE_PROCESS("");
  DECLARE_METHOD("");
  DECLARE_AUTHOR("","");


END_SIMULATOR_SIGNATURE


// =====================================================================
// =====================================================================


MessagesSimulator::MessagesSimulator() : PluggableSimulator(),
  m_RepeatMessages(1)
{


}


// =====================================================================
// =====================================================================


MessagesSimulator::~MessagesSimulator()
{


}


// =====================================================================
// =====================================================================


void MessagesSimulator::initParams(const openfluid::ware::WareParams_t& Params)
{
  OPENFLUID_GetSimulatorParameter(Params,"rptmsgs",m_RepeatMessages);
}


// =====================================================================
// =====================================================================


void MessagesSimulator::prepareData()
{

}


// =====================================================================
// =====================================================================


void MessagesSimulator::checkConsistency()
{

}


// =====================================================================
// =====================================================================


openfluid::base::SchedulingRequest MessagesSimulator::initializeRun()
{
  OPENFLUID_LogWarning("Message from tests.messages simulator");

  return DefaultDeltaT();
}


// =====================================================================
// =====================================================================


openfluid::base::SchedulingRequest MessagesSimulator::runStep()
{
  openfluid::core::SpatialUnit* TU;

  int i = 0;

  std::string RptStr, IDStr, TSStr;

  openfluid::tools::convertValue((OPENFLUID_GetCurrentTimeIndex()/OPENFLUID_GetDefaultDeltaT()),&TSStr);


  OPENFLUID_UNITS_ORDERED_LOOP("TestUnits",TU)
  {
    openfluid::tools::convertValue(TU->getID(),&IDStr);

    for (i = 0; i< m_RepeatMessages; i++)
    {
      openfluid::tools::convertValue((i+1),&RptStr);
      OPENFLUID_LogWarning("["+TSStr+"|"+IDStr+"|"+RptStr+"] Message from tests.messages simulator");
    }

  }

  return DefaultDeltaT();
}


// =====================================================================
// =====================================================================


void MessagesSimulator::finalizeRun()
{

  OPENFLUID_LogWarning("Message from tests.messages simulator");
}

