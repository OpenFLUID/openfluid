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
  @file TLUSim.cpp

  @author Michael Rabotin <michael.rabotin@supagro.inra.fr>
*/


#include <openfluid/ware/PluggableSimulator.hpp>
#include <openfluid/tools/DataHelpers.hpp>

// =====================================================================
// =====================================================================


DECLARE_SIMULATOR_PLUGIN;


// =====================================================================
// =====================================================================


BEGIN_SIMULATOR_SIGNATURE("examples.trafficlight.state")

DECLARE_NAME("Traffic Light Unit (TLU) function state");
DECLARE_DESCRIPTION("");

DECLARE_VERSION(openfluid::config::FULL_VERSION);

DECLARE_STATUS(openfluid::ware::EXPERIMENTAL);

DECLARE_DOMAIN("examples");
DECLARE_PROCESS("");
DECLARE_METHOD("");
DECLARE_AUTHOR("Michael Rabotin","rabotin@supagro.inra.fr");



DECLARE_PRODUCED_VAR("examples.TLU.S.state","TLU","traffic light unit state ","");
DECLARE_PRODUCED_VAR("examples.TLU.T.changeTime","TLU","traffic light unit state changing time","");
DECLARE_REQUIRED_ATTRIBUTE("state","TLU","","-");
DECLARE_REQUIRED_ATTRIBUTE("duration","TLU","","-");

// Scheduling
DECLARE_SCHEDULING_DEFAULT;


END_SIMULATOR_SIGNATURE


// =====================================================================
// =====================================================================


/**

 */
class TLUSimulator : public openfluid::ware::PluggableSimulator
{
  private:


  public:


    TLUSimulator() : PluggableSimulator()
  {


  }


    // =====================================================================
    // =====================================================================


    ~TLUSimulator()
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

    }


    // =====================================================================
    // =====================================================================


    void checkConsistency()
    {
      openfluid::core::SpatialUnit * TLU;
      openfluid::core::StringValue Period;
      openfluid::core::IntegerValue PeriodInt;

      unsigned int DeltaT;
      int ID, Modulo;

      DeltaT=OPENFLUID_GetDefaultDeltaT();
      Modulo=0;

      OPENFLUID_UNITS_ORDERED_LOOP("TLU",TLU)
      {
        OPENFLUID_GetAttribute(TLU,"duration",Period);
        Period.toIntegerValue(PeriodInt);
        if (PeriodInt>DeltaT)
          Modulo=PeriodInt%DeltaT;
        else
          Modulo=DeltaT%PeriodInt;

        if (Modulo!=0)
        {
          ID = TLU->getID();
          std::string IDStr;
          openfluid::tools::convertValue(ID,&IDStr);
          OPENFLUID_RaiseError("The Duration coefficient of TLU " + IDStr +
                               " should be compatible with the Simulation Scheduling.");

        }
      }
    }


    // =====================================================================
    // =====================================================================


    openfluid::base::SchedulingRequest initializeRun()
    {

      bool StateBool;
      openfluid::core::SpatialUnit * pTLU;
      openfluid::core::StringValue PeriodString;
      openfluid::core::IntegerValue PeriodInt;
      openfluid::core::StringValue State;

      OPENFLUID_UNITS_ORDERED_LOOP("TLU",pTLU)
      {
        OPENFLUID_GetAttribute(pTLU,"duration",PeriodString);
        PeriodString.toIntegerValue(PeriodInt);
        OPENFLUID_GetAttribute(pTLU,"state",State);
        State.toBoolean(StateBool);
        OPENFLUID_InitializeVariable(pTLU,"examples.TLU.T.changeTime",PeriodInt);
        OPENFLUID_InitializeVariable(pTLU,"examples.TLU.S.state",StateBool);
      }
      return DefaultDeltaT();
    }


    // =====================================================================
    // =====================================================================


    openfluid::base::SchedulingRequest runStep()
    {
      openfluid::core::SpatialUnit * pTLU;
      openfluid::core::StringValue PeriodString;
      openfluid::core::IntegerValue PeriodInt;
      openfluid::core::IntegerValue NextPeriod;

      openfluid::core::StringValue State;
      bool StateBool;

      unsigned int CurrentTimeIndex = (OPENFLUID_GetCurrentTimeIndex());

      OPENFLUID_UNITS_ORDERED_LOOP("TLU",pTLU)
      {
        OPENFLUID_GetAttribute(pTLU,"duration",PeriodString);
        PeriodString.toIntegerValue(PeriodInt);

        OPENFLUID_GetAttribute(pTLU,"state",State);
        State.toBoolean(StateBool);

        OPENFLUID_GetVariable(pTLU,"examples.TLU.T.changeTime",
                              CurrentTimeIndex-OPENFLUID_GetDefaultDeltaT(),NextPeriod);
        OPENFLUID_GetVariable(pTLU,"examples.TLU.S.state",CurrentTimeIndex-OPENFLUID_GetDefaultDeltaT(),StateBool);


        if (unsigned(NextPeriod)==OPENFLUID_GetCurrentTimeIndex())
        {
          StateBool=1-StateBool;
          NextPeriod=NextPeriod+PeriodInt;
        }

        OPENFLUID_AppendVariable(pTLU,"examples.TLU.T.changeTime",NextPeriod);
        OPENFLUID_AppendVariable(pTLU,"examples.TLU.S.state",StateBool);

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


DEFINE_SIMULATOR_CLASS(TLUSimulator);














