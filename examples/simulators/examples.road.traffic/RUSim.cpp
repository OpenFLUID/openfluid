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
  @file RUSim.cpp

  @author Michael Rabotin <michael.rabotin@supagro.inra.fr>
*/


#include <openfluid/ware/PluggableSimulator.hpp>


// =====================================================================
// =====================================================================


BEGIN_SIMULATOR_SIGNATURE("examples.road.traffic")

DECLARE_NAME("Road Unit (RU) function transfert and stockage for cars");
DECLARE_DESCRIPTION("");

DECLARE_VERSION(openfluid::config::FULL_VERSION);

DECLARE_STATUS(openfluid::ware::EXPERIMENTAL);

DECLARE_DOMAIN("examples");
DECLARE_PROCESS("");
DECLARE_METHOD("");
DECLARE_AUTHOR("Michael Rabotin","rabotin@supagro.inra.fr");

DECLARE_USED_PARAMETER("MultiCapacity","Multiplying factor for capacity","-");
DECLARE_USED_VARIABLE("examples.TLU.S.state","TLU","traffic light unit state","");
DECLARE_PRODUCED_VARIABLE("examples.RU.S.stock","RU","number of cars stocked on RU","");
DECLARE_USED_ATTRIBUTE("stockini","RU","","-");
DECLARE_USED_ATTRIBUTE("capacity","RU","","car/min");

// Scheduling
DECLARE_SCHEDULING_DEFAULT;

END_SIMULATOR_SIGNATURE


// =====================================================================
// =====================================================================


/**

 */
class RUSimulator : public openfluid::ware::PluggableSimulator
{

  private:
    bool m_UseTLUStateFromTLUVar;
    double m_MultiCapacity;

  public:


    RUSimulator() : PluggableSimulator(),m_UseTLUStateFromTLUVar(false),m_MultiCapacity(1)
  {


  }


    // =====================================================================
    // =====================================================================


    ~RUSimulator()
    {


    }


    // =====================================================================
    // =====================================================================


    void initParams(const openfluid::ware::WareParams_t& Params)
    {

      OPENFLUID_GetSimulatorParameter(Params,"MultiCapacity",m_MultiCapacity);
      if (m_MultiCapacity<0)
        OPENFLUID_RaiseError("The Multiplying factor for capacity (MultiCapacity) should be positive");
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

    }


    // =====================================================================
    // =====================================================================


    openfluid::base::SchedulingRequest initializeRun()
    {

      openfluid::core::SpatialUnit* RU;
      openfluid::core::SpatialUnit* TLU;
      openfluid::core::IntegerValue StockValue;

      m_UseTLUStateFromTLUVar=false;

      OPENFLUID_UNITS_ORDERED_LOOP("RU",RU)
      {
        OPENFLUID_GetAttribute(RU,"stockini",StockValue);
        OPENFLUID_InitializeVariable(RU,"examples.RU.S.stock",StockValue);
      }

      // Checking if TLU units exist
      if (OPENFLUID_IsUnitsClassExist("TLU"))
      {
        // Checking if TLU variables exist
        OPENFLUID_UNITS_ORDERED_LOOP("TLU",TLU)
        {
          m_UseTLUStateFromTLUVar = OPENFLUID_IsVariableExist(TLU,"examples.TLU.S.state");
        }
      }



      return DefaultDeltaT();
    }


    // =====================================================================
    // =====================================================================


    openfluid::base::SchedulingRequest runStep()
    {
      openfluid::core::SpatialUnit* RU;
      openfluid::core::SpatialUnit* UpRU;
      openfluid::core::SpatialUnit* UpTLU;
      openfluid::core::IntegerValue StockValue;
      openfluid::core::IntegerValue Capacity;
      openfluid::core::IntegerValue CapacityByDeltaT;
      openfluid::core::IntegerValue StockValueUp;
      openfluid::core::IntegerValue TmpValue;
      bool TLUState;
      std::list<openfluid::core::SpatialUnit*>* UpRUsList;
      std::list<openfluid::core::SpatialUnit*>* UpTLUsList;
      std::list<openfluid::core::SpatialUnit*>::iterator UpRUiter;
      unsigned int DeltaT =  OPENFLUID_GetDefaultDeltaT();

      OPENFLUID_UNITS_ORDERED_LOOP("RU",RU)
      {

        TLUState=1;
        if (m_UseTLUStateFromTLUVar)
        {
          UpTLUsList = RU->fromSpatialUnits("TLU");
          if (UpTLUsList != NULL)
          {
            UpTLU = *UpTLUsList->begin();
            OPENFLUID_GetVariable(UpTLU,("examples.TLU.S.state"),TLUState);
          }
        }
        if (TLUState!=true)
        {
          OPENFLUID_GetVariable(RU,"examples.RU.S.stock",
                                OPENFLUID_GetCurrentTimeIndex()-OPENFLUID_GetDefaultDeltaT(),StockValue);
          OPENFLUID_AppendVariable(RU,"examples.RU.S.stock",StockValue);
        }
        else
        {
          UpRUsList = RU->fromSpatialUnits("RU");

          if (UpRUsList == NULL)
            OPENFLUID_GetVariable(RU,"examples.RU.S.stock",
                                  OPENFLUID_GetCurrentTimeIndex()-OPENFLUID_GetDefaultDeltaT(),StockValue);
          else
          {
            OPENFLUID_GetAttribute(RU,"capacity",Capacity);
         //   CapacityByDeltaT=std::ceil(Capacity/60*DeltaT);
            CapacityByDeltaT = (Capacity*m_MultiCapacity)/60*DeltaT;


            if (OPENFLUID_IsVariableExist(RU,"examples.RU.S.stock",OPENFLUID_GetCurrentTimeIndex()))
              OPENFLUID_GetVariable(RU,"examples.RU.S.stock",StockValue);
            else
              OPENFLUID_GetVariable(RU,"examples.RU.S.stock",
                                    OPENFLUID_GetCurrentTimeIndex()-OPENFLUID_GetDefaultDeltaT(),StockValue);


            for (UpRUiter=UpRUsList->begin(); UpRUiter != UpRUsList->end(); ++UpRUiter)
            {

              UpRU = *UpRUiter;
              OPENFLUID_GetVariable(UpRU,"examples.RU.S.stock",StockValueUp);
              if (StockValueUp>CapacityByDeltaT)
              {
                StockValue=StockValue+CapacityByDeltaT;
                StockValueUp=StockValueUp-CapacityByDeltaT;
              }
              else
              {
                StockValue=StockValue+StockValueUp;
                StockValueUp=0;
              }

              OPENFLUID_SetVariable(UpRU,"examples.RU.S.stock",StockValueUp);

            }
          }

          OPENFLUID_AppendVariable(RU,"examples.RU.S.stock",StockValue);

        }

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


DEFINE_SIMULATOR_CLASS(RUSimulator);














