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
  \file EventsFunc.cpp
  \brief Implements ...
*/


#include "EventsFunc.h"


// =====================================================================
// =====================================================================


DEFINE_FUNCTION_HOOK(EventsFunction)


// =====================================================================
// =====================================================================


BEGIN_SIGNATURE_HOOK
  DECLARE_SIGNATURE_ID(("tests.events"));
  DECLARE_SIGNATURE_NAME(("test function for events"));
  DECLARE_SIGNATURE_DESCRIPTION((""));

  DECLARE_SIGNATURE_VERSION(("1.0"));
  DECLARE_SIGNATURE_SDKVERSION;
  DECLARE_SIGNATURE_STATUS(openfluid::base::EXPERIMENTAL);

  DECLARE_SIGNATURE_DOMAIN((""));
  DECLARE_SIGNATURE_PROCESS((""));
  DECLARE_SIGNATURE_METHOD((""));
  DECLARE_SIGNATURE_AUTHORNAME((""));
  DECLARE_SIGNATURE_AUTHOREMAIL((""));

  DECLARE_USED_EVENTS("TestUnits");

END_SIGNATURE_HOOK


// =====================================================================
// =====================================================================


EventsFunction::EventsFunction()
                : PluggableFunction()
{


}


// =====================================================================
// =====================================================================


EventsFunction::~EventsFunction()
{


}


// =====================================================================
// =====================================================================


bool EventsFunction::initParams(openfluid::core::FuncParamsMap_t Params)
{


  return true;
}

// =====================================================================
// =====================================================================


bool EventsFunction::prepareData()
{


  return true;
}


// =====================================================================
// =====================================================================


bool EventsFunction::checkConsistency()
{


  return true;
}


// =====================================================================
// =====================================================================


bool EventsFunction::initializeRun(const openfluid::base::SimulationInfo* SimInfo)
{
  openfluid::core::Unit* aUnit;
  openfluid::core::EventsCollection EvColl;
  openfluid::core::Event* Event;
  std::string Info;
  DECLARE_UNITS_ORDERED_LOOP(5);
  DECLARE_EVENT_COLLECTION_LOOP;

  openfluid::core::DateTime BeginDate,EndDate;

  BeginDate = SimInfo->getStartTime() - openfluid::core::DateTime::Days(60);

  EndDate = SimInfo->getStartTime() - 1;

//  std::cout << BeginDate.getAsISOString() << " -->" << EndDate.getAsISOString() << std::endl;

  BEGIN_UNITS_ORDERED_LOOP(5,"TestUnits",aUnit)

    EvColl.clear();
    OPENFLUID_GetEvents(aUnit,BeginDate,EndDate,&EvColl);

//    std::cout << aUnit->getID() << " -> " << EvColl.getCount() << std::endl;


    if (aUnit->getID() == 1)
    {
//      std::cout << EvColl.getCount() << std::endl;
      if (EvColl.getCount() != 2)
        OPENFLUID_RaiseError("tests.events","initializeRun()","wrong event number on TestUnit 1");

      BEGIN_EVENT_COLLECTION_LOOP(EvColl.getEventsList(),Event)
        if (!(Event->isInfoEqual("when","before") &&
              Event->isInfoEqual("where",1.0) &&
              Event->isInfoEqual("numeric",1.13) &&
              Event->getInfoAsString("string",&Info) &&
              Info.substr(0,4) == "EADG"))
          OPENFLUID_RaiseError("tests.events","initializeRun()","wrong event info on TestUnit 1");
      END_LOOP

    }
    else
    {
      if (aUnit->getID() == 7)
      {
        if (EvColl.getCount() != 1)
          OPENFLUID_RaiseError("tests.events","initializeRun()","wrong event number on TestUnit 7");
      }
      else
      {
        if (aUnit->getID() == 12)
        {
          if (EvColl.getCount() > 0)
            OPENFLUID_RaiseError("tests.events","initializeRun()","found older event(s) on TestUnits 12");
        }
        else
        {
          if (EvColl.getCount() > 0)
            OPENFLUID_RaiseError("tests.events","initializeRun()","found unknown events on some TestUnits");
        }
      }
    }
  END_LOOP

  return true;
}

// =====================================================================
// =====================================================================


bool EventsFunction::runStep(const openfluid::base::SimulationStatus* SimStatus)
{

  openfluid::core::Unit *aUnit;
  openfluid::core::EventsCollection EvColl;
  openfluid::core::Event *Event, AddedEvent;
  std::string Info, TmpStr;
  DECLARE_UNITS_ORDERED_LOOP(5);
  DECLARE_EVENT_COLLECTION_LOOP;

  openfluid::core::DateTime BeginDate,EndDate;

  BeginDate = SimStatus->getCurrentTime();

  EndDate = SimStatus->getCurrentTime() + SimStatus->getTimeStep() -1;

//  std::cout << BeginDate.getAsISOString() << " -->" << EndDate.getAsISOString() << std::endl;

  BEGIN_UNITS_ORDERED_LOOP(5,"TestUnits",aUnit)

    EvColl.clear();
    OPENFLUID_GetEvents(aUnit,BeginDate,EndDate,&EvColl);

    BEGIN_EVENT_COLLECTION_LOOP(EvColl.getEventsList(),Event)
      if (!(Event->isInfoEqual("when","during") &&
            Event->isInfoEqual("where",double(aUnit->getID())) &&
            Event->isInfoEqual("numeric",1.15) &&
            Event->getInfoAsString("string",&Info) &&
            Info.substr(0,4) == "EADG"))
        OPENFLUID_RaiseError("tests.events","runStep()","wrong event info on some TestUnit");
    END_LOOP



    bool FoundEvent = false;
    AddedEvent = openfluid::core::Event(openfluid::core::DateTime(SimStatus->getCurrentTime()+(SimStatus->getTimeStep()*2)));
    openfluid::tools::ConvertValue(SimStatus->getTimeStep(),&TmpStr);
    AddedEvent.addInfo("addingstep",TmpStr);

    OPENFLUID_AppendEvent(aUnit,AddedEvent);

    EvColl.clear();
    OPENFLUID_GetEvents(aUnit,openfluid::core::DateTime(SimStatus->getCurrentTime()+SimStatus->getTimeStep()),openfluid::core::DateTime(SimStatus->getCurrentTime()+(SimStatus->getTimeStep()*2)),&EvColl);

    BEGIN_EVENT_COLLECTION_LOOP(EvColl.getEventsList(),Event)
      if (Event->isInfoEqual("addingstep",TmpStr)) FoundEvent = true;
    END_LOOP
    if (!FoundEvent) OPENFLUID_RaiseError("tests.events","runStep()","added event not found");

  END_LOOP

  return true;
}

// =====================================================================
// =====================================================================


bool EventsFunction::finalizeRun(const openfluid::base::SimulationInfo* SimInfo)
{


  return true;
}

