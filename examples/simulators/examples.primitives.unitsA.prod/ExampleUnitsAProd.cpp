/**
  \file ExampleUnitsAProd.cpp
*/


#include <openfluid/ware/PluggableSimulator.hpp>


// =====================================================================
// =====================================================================


DECLARE_SIMULATOR_PLUGIN;


// =====================================================================
// =====================================================================


BEGIN_SIMULATOR_SIGNATURE("examples.primitives.unitsA.prod")

  DECLARE_NAME("Example of data production on units of class A");
  DECLARE_DESCRIPTION("");

  DECLARE_VERSION(openfluid::config::FULL_VERSION);
  DECLARE_STATUS(openfluid::ware::EXPERIMENTAL);

  DECLARE_DOMAIN("examples");
  DECLARE_PROCESS("");
  DECLARE_METHOD("");
  DECLARE_AUTHOR("Jean-Christophe Fabre","fabrejc@supagro.inra.fr");

  DECLARE_REQUIRED_VAR("var3","unitsA","the variable 3","");
  
  DECLARE_PRODUCED_VAR("var1","unitsA","the variable 1","");
  DECLARE_PRODUCED_VAR("var2","unitsA","the variable 2","");
  
  DECLARE_REQUIRED_INPUTDATA("inivar1","unitsA","the input data inivar1","");

END_SIMULATOR_SIGNATURE


// =====================================================================
// =====================================================================


/**

*/
class ExampleUnitsAProduction : public openfluid::ware::PluggableSimulator
{
  private:

  
  public:

  
    ExampleUnitsAProduction() : PluggableSimulator()
    {
  
  
    }
  
  
    // =====================================================================
    // =====================================================================
  
  
    ~ExampleUnitsAProduction()
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
      openfluid::core::Unit* A;


      OPENFLUID_UNITS_ORDERED_LOOP("unitsA",A)
      {

        OPENFLUID_InitializeVariable(A,"var1",0.0);
        OPENFLUID_InitializeVariable(A,"var2",0.0);
      }


      return DefaultDeltaT();
    }


    // =====================================================================
    // =====================================================================
  
  
    openfluid::base::SchedulingRequest runStep()
    {
      openfluid::core::Unit* A;
      openfluid::core::DoubleValue Value1;

      unsigned int CurrentTimeIndex = (OPENFLUID_GetCurrentTimeIndex());

      OPENFLUID_UNITS_ORDERED_LOOP("unitsA",A)
      {
        OPENFLUID_GetVariable(A,"var1",CurrentTimeIndex-OPENFLUID_GetDefaultDeltaT(),Value1);
        Value1 = Value1 + 2.0;

        OPENFLUID_AppendVariable(A,"var1",Value1);
        OPENFLUID_AppendVariable(A,"var2",1.5);
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


DEFINE_SIMULATOR_CLASS(ExampleUnitsAProduction);

