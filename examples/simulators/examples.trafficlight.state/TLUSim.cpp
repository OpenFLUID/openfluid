/**
  \file TLUSim.cpp
 */


#include <openfluid/ware/PluggableSimulator.hpp>


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
DECLARE_REQUIRED_INPUTDATA("state","TLU","","-");
DECLARE_REQUIRED_INPUTDATA("duration","TLU","","-");

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

    }


    // =====================================================================
    // =====================================================================


    openfluid::base::SchedulingRequest initializeRun()
    {

      bool StateBool;
      openfluid::core::Unit * pTLU;
      openfluid::core::StringValue PeriodString;
      openfluid::core::IntegerValue PeriodInt;
      openfluid::core::StringValue State;

      OPENFLUID_UNITS_ORDERED_LOOP("TLU",pTLU)
      {
        OPENFLUID_GetInputData(pTLU,"duration",PeriodString);
        PeriodString.toIntegerValue(PeriodInt);
        OPENFLUID_GetInputData(pTLU,"state",State);
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
      openfluid::core::Unit * pTLU;
      openfluid::core::StringValue PeriodString;
      openfluid::core::IntegerValue PeriodInt;
      openfluid::core::IntegerValue NextPeriod;

      openfluid::core::StringValue State;
      bool StateBool;

      unsigned int CurrentTimeIndex = (OPENFLUID_GetCurrentTimeIndex());

      OPENFLUID_UNITS_ORDERED_LOOP("TLU",pTLU)
      {
        OPENFLUID_GetInputData(pTLU,"duration",PeriodString);
        PeriodString.toIntegerValue(PeriodInt);

        OPENFLUID_GetInputData(pTLU,"state",State);
        State.toBoolean(StateBool);

        OPENFLUID_GetVariable(pTLU,"examples.TLU.T.changeTime",CurrentTimeIndex-OPENFLUID_GetDefaultDeltaT(),NextPeriod);
        OPENFLUID_GetVariable(pTLU,"examples.TLU.S.state",CurrentTimeIndex-OPENFLUID_GetDefaultDeltaT(),StateBool);


        if(NextPeriod==OPENFLUID_GetCurrentTimeIndex())
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














