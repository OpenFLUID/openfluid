/**
  \file ExampleFuncProd.cpp
*/


#include <openfluid/ware/PluggableFunction.hpp>


// =====================================================================
// =====================================================================


DECLARE_FUNCTION_PLUGIN;


// =====================================================================
// =====================================================================


BEGIN_FUNCTION_SIGNATURE
  DECLARE_SIGNATURE_ID(("examples.primitives.unitsA.prod"));
  DECLARE_SIGNATURE_NAME(("Example of data production on units of class A"));
  DECLARE_SIGNATURE_DESCRIPTION((""));

  DECLARE_SIGNATURE_VERSION(("1.0"));
  DECLARE_SIGNATURE_SDKVERSION;
  DECLARE_SIGNATURE_STATUS(openfluid::ware::EXPERIMENTAL);

  DECLARE_SIGNATURE_DOMAIN(("examples"));
  DECLARE_SIGNATURE_PROCESS((""));
  DECLARE_SIGNATURE_METHOD((""));
  DECLARE_SIGNATURE_AUTHORNAME(("Jean-Christophe Fabre"));
  DECLARE_SIGNATURE_AUTHOREMAIL(("fabrejc@supagro.inra.fr"));

  DECLARE_REQUIRED_PREVVAR("var3","unitsA","the variable 3","");
  
  DECLARE_PRODUCED_VAR("var1","unitsA","the variable 1","");
  DECLARE_PRODUCED_VAR("var2","unitsA","the variable 2","");
  
  DECLARE_REQUIRED_INPUTDATA("inivar1","unitsA","the input data inivar1","");

END_FUNCTION_SIGNATURE


// =====================================================================
// =====================================================================


/**

*/
class ExampleUnitsAProduction : public openfluid::ware::PluggableFunction
{
  private:

  
  public:

  
    ExampleUnitsAProduction() : PluggableFunction()
    {
  
  
    }
  
  
    // =====================================================================
    // =====================================================================
  
  
    ~ExampleUnitsAProduction()
    {
  
  
    }
  
  
    // =====================================================================
    // =====================================================================
  
  
    void initParams(const openfluid::core::FuncParamsMap_t& Params)
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
  
  
    openfluid::core::Duration_t initializeRun()
    {
      return DefaultDeltaT();
    }
  
    // =====================================================================
    // =====================================================================
  
  
    openfluid::core::Duration_t runStep()
    {
      openfluid::core::Unit* A;
      openfluid::core::DoubleValue Value1;

      unsigned int CurrentStep = (OPENFLUID_GetCurrentTimeIndex()/OPENFLUID_GetDefaultDeltaT());

      OPENFLUID_UNITS_ORDERED_LOOP("unitsA",A)
      {
        if (OPENFLUID_GetCurrentTimeIndex() == 0)
        {
           OPENFLUID_GetInputData(A,"inivar1",Value1);
        }
        else
        {
          OPENFLUID_GetVariable(A,"var1",CurrentStep-1,Value1);
          Value1 = Value1 + 2.0;
        }

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


DEFINE_FUNCTION_CLASS(ExampleUnitsAProduction);

