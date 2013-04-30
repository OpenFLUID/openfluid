/**
  \file TLUFunc.cpp
 */


#include <openfluid/ware/PluggableSimulator.hpp>


// =====================================================================
// =====================================================================


DECLARE_FUNCTION_PLUGIN;


// =====================================================================
// =====================================================================


BEGIN_FUNCTION_SIGNATURE("examples.trafficlight.state")

DECLARE_SIGNATURE_NAME("Traffic Light Unit (TLU) function state");
DECLARE_SIGNATURE_DESCRIPTION("");

DECLARE_SIGNATURE_VERSION("1.0");
DECLARE_SIGNATURE_SDKVERSION;
DECLARE_SIGNATURE_STATUS(openfluid::ware::EXPERIMENTAL);

DECLARE_SIGNATURE_DOMAIN("examples");
DECLARE_SIGNATURE_PROCESS("");
DECLARE_SIGNATURE_METHOD("");
DECLARE_SIGNATURE_AUTHORNAME("Michael Rabotin");
DECLARE_SIGNATURE_AUTHOREMAIL("rabotin@supagro.inra.fr");



DECLARE_PRODUCED_VAR("examples.TLU.S.state","TLU","traffic light unit state ","");
DECLARE_PRODUCED_VAR("examples.TLU.T.changeTime","TLU","traffic light unit state changing time","");
DECLARE_REQUIRED_INPUTDATA("state","TLU","","-");
DECLARE_REQUIRED_INPUTDATA("duration","TLU","","-");

END_FUNCTION_SIGNATURE


// =====================================================================
// =====================================================================


/**

 */
class TLUFunction : public openfluid::ware::PluggableSimulator
{
  private:


  public:


    TLUFunction() : PluggableSimulator()
  {


  }


    // =====================================================================
    // =====================================================================


    ~TLUFunction()
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


DEFINE_FUNCTION_CLASS(TLUFunction);














