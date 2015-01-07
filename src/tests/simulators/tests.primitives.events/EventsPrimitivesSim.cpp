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



#include <openfluid/ware/PluggableSimulator.hpp>


// =====================================================================
// =====================================================================


DECLARE_SIMULATOR_PLUGIN


// =====================================================================
// =====================================================================


#define THIS_SIM_ID "tests.primitives.events"

BEGIN_SIMULATOR_SIGNATURE(THIS_SIM_ID)

  DECLARE_NAME("test simulator for events primitives");
  DECLARE_DESCRIPTION("");

  DECLARE_VERSION("1.0");
  DECLARE_SDKVERSION;
  DECLARE_STATUS(openfluid::ware::EXPERIMENTAL);

  DECLARE_DOMAIN("");
  DECLARE_PROCESS("");
  DECLARE_METHOD("");
  DECLARE_AUTHOR("","");


  DECLARE_USED_EVENTS("TestUnits");

END_SIMULATOR_SIGNATURE


// =====================================================================
// =====================================================================


class EventsPrimitivesUseSimulator : public openfluid::ware::PluggableSimulator
{
  private:

  public:


    EventsPrimitivesUseSimulator() : PluggableSimulator()
    {


    }


    // =====================================================================
    // =====================================================================


    ~EventsPrimitivesUseSimulator()
    {


    }


    // =====================================================================
    // =====================================================================


    void initParams(const openfluid::ware::WareParams_t& /*Params*/)
    {

    }


    // =====================================================================
    // =====================================================================


    void prepareData()
    {
      openfluid::core::SpatialUnit *aUnit;
      openfluid::core::Event AddedEvent;
      openfluid::core::DateTime BeginDate;

      OPENFLUID_UNITS_ORDERED_LOOP("TestUnits",aUnit)
      {
        AddedEvent = openfluid::core::Event(openfluid::core::DateTime(OPENFLUID_GetEndDate()+10*OPENFLUID_GetDefaultDeltaT()));
        AddedEvent.addInfo("unused_event","true");

        OPENFLUID_AppendEvent(aUnit,AddedEvent);
      }
    }


    // =====================================================================
    // =====================================================================


    void checkConsistency() { }


    // =====================================================================
    // =====================================================================


    openfluid::base::SchedulingRequest initializeRun()
    {
      openfluid::core::SpatialUnit* aUnit;
      openfluid::core::EventsCollection EvColl;
      openfluid::core::Event* Event;
      std::string Info;

      openfluid::core::DateTime BeginDate,EndDate;

      BeginDate = OPENFLUID_GetBeginDate() - openfluid::core::DateTime::Days(60);

      EndDate = OPENFLUID_GetBeginDate() - 1;

    //  std::cout << BeginDate.getAsISOString() << " -->" << EndDate.getAsISOString() << std::endl;

      OPENFLUID_UNITS_ORDERED_LOOP("TestUnits",aUnit)
      {

        EvColl.clear();
        OPENFLUID_GetEvents(aUnit,BeginDate,EndDate,EvColl);

    //    std::cout << aUnit->getID() << " -> " << EvColl.getCount() << std::endl;


        if (aUnit->getID() == 1)
        {
    //      std::cout << EvColl.getCount() << std::endl;
          if (EvColl.getCount() != 2)
            OPENFLUID_RaiseError("wrong event number on TestUnit 1");

          OPENFLUID_EVENT_COLLECTION_LOOP(EvColl.eventsList(),Event)
          {
            if (!(Event->isInfoEqual("when","before") &&
                  Event->isInfoEqual("where",1.0) &&
                  Event->isInfoEqual("numeric",1.13) &&
                  Event->getInfoAsString("string",Info) &&
                  Info.substr(0,4) == "EADG"))
              OPENFLUID_RaiseError("wrong event info on TestUnit 1");
          }

        }
        else
        {
          if (aUnit->getID() == 7)
          {
            if (EvColl.getCount() != 1)
              OPENFLUID_RaiseError("wrong event number on TestUnit 7");
          }
          else
          {
            if (aUnit->getID() == 12)
            {
              if (EvColl.getCount() > 0)
                OPENFLUID_RaiseError("found older event(s) on TestUnits 12");
            }
            else
            {
              if (EvColl.getCount() > 0)
                OPENFLUID_RaiseError("found unknown events on some TestUnits");
            }
          }
        }
      }


      BeginDate = OPENFLUID_GetBeginDate();

      EndDate = OPENFLUID_GetEndDate();

    //  std::cout << BeginDate.getAsISOString() << " -->" << EndDate.getAsISOString() << std::endl;

      OPENFLUID_UNITS_ORDERED_LOOP("TestUnits",aUnit)
      {
        EvColl.clear();
        OPENFLUID_GetEvents(aUnit,BeginDate,EndDate,EvColl);

        OPENFLUID_EVENT_COLLECTION_LOOP(EvColl.eventsList(),Event)
        {
          if (Event->isInfoExist("addingstep")) OPENFLUID_RaiseError("unexpected event found");
        }

      }

      return DefaultDeltaT();
    }


    // =====================================================================
    // =====================================================================


    openfluid::base::SchedulingRequest runStep()
    {

      openfluid::core::SpatialUnit *aUnit;
      openfluid::core::EventsCollection EvColl;
      openfluid::core::Event *Event, AddedEvent;
      std::string Info, TmpStr;

      openfluid::core::DateTime BeginDate,EndDate;

      BeginDate = OPENFLUID_GetCurrentDate() - OPENFLUID_GetDefaultDeltaT();

      EndDate = OPENFLUID_GetCurrentDate()- 1;


      OPENFLUID_UNITS_ORDERED_LOOP("TestUnits",aUnit)
      {
        EvColl.clear();
        OPENFLUID_GetEvents(aUnit,BeginDate,EndDate,EvColl);

        OPENFLUID_EVENT_COLLECTION_LOOP(EvColl.eventsList(),Event)
        {
    //      std::cout << std::endl << "========== Unit " << aUnit->getID() << " ==========" << std::endl;
          //Event->println();
          if (!((Event->isInfoEqual("when","during") &&
                Event->isInfoEqual("where",double(aUnit->getID())) &&
                Event->isInfoEqual("numeric",1.15) &&
                Event->getInfoAsString("string",Info) &&
                Info.substr(0,4) == "EADG") || (Event->isInfoExist("addingstep"))))
            OPENFLUID_RaiseError("wrong event info on some TestUnit");
        }



        bool FoundEvent = false;
        AddedEvent = openfluid::core::Event(openfluid::core::DateTime(OPENFLUID_GetCurrentDate()+(OPENFLUID_GetDefaultDeltaT()*2)));
        openfluid::tools::ConvertValue(OPENFLUID_GetDefaultDeltaT(),&TmpStr);
        AddedEvent.addInfo("addingstep",TmpStr);

        OPENFLUID_AppendEvent(aUnit,AddedEvent);

        EvColl.clear();
        OPENFLUID_GetEvents(aUnit,openfluid::core::DateTime(OPENFLUID_GetCurrentDate()+OPENFLUID_GetDefaultDeltaT()),
                                  openfluid::core::DateTime(OPENFLUID_GetCurrentDate()+(OPENFLUID_GetDefaultDeltaT()*2)),EvColl);

        OPENFLUID_EVENT_COLLECTION_LOOP(EvColl.eventsList(),Event)
          if (Event->isInfoEqual("addingstep",TmpStr)) FoundEvent = true;

        if (!FoundEvent) OPENFLUID_RaiseError("added event not found");

      }

      return DefaultDeltaT();
    }


    // =====================================================================
    // =====================================================================


    void finalizeRun()
    {

    }

};

// =====================================================================
// =====================================================================

DEFINE_SIMULATOR_CLASS(EventsPrimitivesUseSimulator)

