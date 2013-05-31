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

DECLARE_USED_VAR("examples.TLU.S.state","TLU","traffic light unit state","");
DECLARE_PRODUCED_VAR("examples.RU.S.stock","RU","number of cars stocked on RU","");
DECLARE_USED_INPUTDATA("stockini","RU","","-");
DECLARE_USED_INPUTDATA("capacity","RU","","-");

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


  public:


    RUSimulator() : PluggableSimulator()
  {


  }


    // =====================================================================
    // =====================================================================


    ~RUSimulator()
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

    }


    // =====================================================================
    // =====================================================================


    openfluid::base::SchedulingRequest initializeRun()
    {
      openfluid::core::Unit* RU;
      openfluid::core::DoubleValue StockValue;

      OPENFLUID_UNITS_ORDERED_LOOP("RU",RU)
      {
        OPENFLUID_GetInputData(RU,"stockini",StockValue);
        OPENFLUID_InitializeVariable(RU,"examples.RU.S.stock",StockValue);
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
      openfluid::core::DoubleValue StockValueUp;
      openfluid::core::DoubleValue TmpValue;
      bool TLUState;
      std::list<openfluid::core::Unit*>* UpRUsList;
      std::list<openfluid::core::Unit*>* UpTLUsList;
      std::list<openfluid::core::Unit*>::iterator UpRUiter;

      OPENFLUID_UNITS_ORDERED_LOOP("RU",RU)
      {

        UpTLUsList = RU->getFromUnits("TLU");
        TLUState=1;
        if (UpTLUsList != NULL)
        {
          UpTLU = *UpTLUsList->begin();
          OPENFLUID_GetVariable(UpTLU,("examples.TLU.S.state"),TLUState);
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
            OPENFLUID_GetInputData(RU,"capacity",Capacity);
            if(OPENFLUID_IsVariableExist(RU,"examples.RU.S.stock",OPENFLUID_GetCurrentTimeIndex()))
              OPENFLUID_GetVariable(RU,"examples.RU.S.stock",StockValue);


            else
              OPENFLUID_GetVariable(RU,"examples.RU.S.stock",OPENFLUID_GetCurrentTimeIndex()-OPENFLUID_GetDefaultDeltaT(),StockValue);

            for(UpRUiter=UpRUsList->begin(); UpRUiter != UpRUsList->end(); UpRUiter++)
            {

              UpRU = *UpRUiter;
              OPENFLUID_GetVariable(UpRU,"examples.RU.S.stock",StockValueUp);
              if(StockValueUp>Capacity)
              {
                StockValue=StockValue+Capacity;
                StockValueUp=StockValueUp-Capacity;
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














