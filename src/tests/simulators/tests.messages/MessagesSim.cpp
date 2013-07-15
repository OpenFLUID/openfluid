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
  \file PrimitivesProdSim.cpp
  \brief Implements ...
*/


#include "MessagesSim.h"


// =====================================================================
// =====================================================================


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


MessagesSimulator::MessagesSimulator()
                : PluggableSimulator()
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

  m_RepeatMessages = 1;

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
  OPENFLUID_RaiseWarning("Message from tests.messages simulator");

  return DefaultDeltaT();
}

// =====================================================================
// =====================================================================


openfluid::base::SchedulingRequest MessagesSimulator::runStep()
{
  openfluid::core::Unit* TU;

  int i = 0;

  std::string RptStr, IDStr, TSStr;

  openfluid::tools::ConvertValue((OPENFLUID_GetCurrentTimeIndex()/OPENFLUID_GetDefaultDeltaT()),&TSStr);


  OPENFLUID_UNITS_ORDERED_LOOP("TestUnits",TU)
  {
    openfluid::tools::ConvertValue(TU->getID(),&IDStr);

    for (i = 0; i< m_RepeatMessages; i++)
    {
      openfluid::tools::ConvertValue((i+1),&RptStr);
      OPENFLUID_RaiseWarning("["+TSStr+"|"+IDStr+"|"+RptStr+"] Message from tests.messages simulator");
    }

  }

  return DefaultDeltaT();
}

// =====================================================================
// =====================================================================


void MessagesSimulator::finalizeRun()
{

  OPENFLUID_RaiseWarning("Message from tests.messages simulator");
}

