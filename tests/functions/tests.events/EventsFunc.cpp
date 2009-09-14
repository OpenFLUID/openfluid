/***copyright>=========================================================

  OpenFLUID - Software environment for modelling Fluxes In Landscapes

  This file is a part of OpenFLUID software
  http://www.umr-lisah.fr/openfluid
  Copyright (c) INRA
  See the file "copyright" for the full license governing this code.

=========================================================<copyright***/


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

  openfluid::core::Unit* aUnit;
  openfluid::core::EventsCollection EvColl;
  openfluid::core::Event* Event;
  std::string Info;
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
  END_LOOP

  return true;
}

// =====================================================================
// =====================================================================


bool EventsFunction::finalizeRun(const openfluid::base::SimulationInfo* SimInfo)
{


  return true;
}

