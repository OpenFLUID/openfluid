/**
  \file RUSim.cpp
 */


#include <openfluid/ware/PluggableSimulator.hpp>


// =====================================================================
// =====================================================================


DECLARE_SIMULATOR_PLUGIN;


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

DECLARE_SIMULATOR_PARAM("MultiCapacity","Multiplying factor for capacity","-");
DECLARE_USED_VAR("examples.TLU.S.state","TLU","traffic light unit state","");
DECLARE_PRODUCED_VAR("examples.RU.S.stock","RU","number of cars stocked on RU","");
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


    RUSimulator() : PluggableSimulator(),m_MultiCapacity(1)
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
        OPENFLUID_RaiseError("examples.road.traffic","The Multiplying factor for capacity (MultiCapacity) should be positive");
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

      openfluid::core::Unit* RU;
      openfluid::core::Unit* TLU;
      openfluid::core::DoubleValue StockValue;

      m_UseTLUStateFromTLUVar=false;

      OPENFLUID_UNITS_ORDERED_LOOP("RU",RU)
      {
        OPENFLUID_GetAttribute(RU,"stockini",StockValue);
        OPENFLUID_InitializeVariable(RU,"examples.RU.S.stock",StockValue);
      }

      // Checking if TLU units exist
      if (OPENFLUID_IsUnitClassExist("TLU"))
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

      openfluid::core::Unit* RU;
      openfluid::core::Unit* UpRU;
      openfluid::core::Unit* UpTLU;
      openfluid::core::DoubleValue StockValue;
      openfluid::core::DoubleValue Capacity;
      openfluid::core::DoubleValue CapacityByDeltaT;
      openfluid::core::DoubleValue StockValueUp;
      openfluid::core::DoubleValue TmpValue;
      bool TLUState;
      std::list<openfluid::core::Unit*>* UpRUsList;
      std::list<openfluid::core::Unit*>* UpTLUsList;
      std::list<openfluid::core::Unit*>::iterator UpRUiter;
      unsigned int DeltaT =  OPENFLUID_GetDefaultDeltaT();

      OPENFLUID_UNITS_ORDERED_LOOP("RU",RU)
      {

        TLUState=1;
        if (m_UseTLUStateFromTLUVar)
        {
          UpTLUsList = RU->getFromUnits("TLU");
          if (UpTLUsList != NULL)
          {
            UpTLU = *UpTLUsList->begin();
            OPENFLUID_GetVariable(UpTLU,("examples.TLU.S.state"),TLUState);
          }
        }
        if (TLUState!=true)
        {
          OPENFLUID_GetVariable(RU,"examples.RU.S.stock",OPENFLUID_GetCurrentTimeIndex()-OPENFLUID_GetDefaultDeltaT(),StockValue);
          OPENFLUID_AppendVariable(RU,"examples.RU.S.stock",StockValue);
        }

        else
        {
          UpRUsList = RU->getFromUnits("RU");

          if (UpRUsList == NULL)
            OPENFLUID_GetVariable(RU,"examples.RU.S.stock",OPENFLUID_GetCurrentTimeIndex()-OPENFLUID_GetDefaultDeltaT(),StockValue);
          else
          {
            OPENFLUID_GetAttribute(RU,"capacity",Capacity);
         //   CapacityByDeltaT=std::ceil(Capacity/60*DeltaT);
            CapacityByDeltaT=(Capacity*m_MultiCapacity)/60*DeltaT;


            if (OPENFLUID_IsVariableExist(RU,"examples.RU.S.stock",OPENFLUID_GetCurrentTimeIndex()))
              OPENFLUID_GetVariable(RU,"examples.RU.S.stock",StockValue);


            else
              OPENFLUID_GetVariable(RU,"examples.RU.S.stock",OPENFLUID_GetCurrentTimeIndex()-OPENFLUID_GetDefaultDeltaT(),StockValue);

            for (UpRUiter=UpRUsList->begin(); UpRUiter != UpRUsList->end(); UpRUiter++)
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














