/**
  \file ExampleFuncProd.cpp
*/


#include <openfluid/base/PlugFunction.hpp>



// =====================================================================
// =====================================================================


DECLARE_PLUGIN_HOOKS;


// =====================================================================
// =====================================================================


BEGIN_SIGNATURE_HOOK
  DECLARE_SIGNATURE_ID(("examples.primitives.unitsA.up"));
  DECLARE_SIGNATURE_NAME(("Example of data update on units of class A"));
  DECLARE_SIGNATURE_DESCRIPTION((""));

  DECLARE_SIGNATURE_VERSION(("1.0"));
  DECLARE_SIGNATURE_SDKVERSION;
  DECLARE_SIGNATURE_STATUS(openfluid::base::EXPERIMENTAL);

  DECLARE_SIGNATURE_DOMAIN(("examples"));
  DECLARE_SIGNATURE_PROCESS((""));
  DECLARE_SIGNATURE_METHOD((""));
  DECLARE_SIGNATURE_AUTHORNAME(("Jean-Christophe Fabre"));
  DECLARE_SIGNATURE_AUTHOREMAIL(("fabrejc@supagro.inra.fr"));

  DECLARE_REQUIRED_VAR("var1","unitsA","the variable 1","");
  
  DECLARE_PRODUCED_VAR("var3","unitsA","the variable 3","");
  
  DECLARE_UPDATED_VAR("var2","unitsA","the variable 2","");

END_SIGNATURE_HOOK


// =====================================================================
// =====================================================================


/**

*/
class ExampleUnitsAUpdate : public openfluid::base::PluggableFunction
{
  private:
    double m_Mult;
  
  public:

  
    ExampleUnitsAUpdate() : PluggableFunction()
    {
  
  
    }
  
  
    // =====================================================================
    // =====================================================================
  
  
    ~ExampleUnitsAUpdate()
    {
  
  
    }
  
  
    // =====================================================================
    // =====================================================================
  
  
    bool initParams(openfluid::core::FuncParamsMap_t Params)
    {
  
      m_Mult = 1.0;
      OPENFLUID_GetFunctionParameter(Params,"gmult",&m_Mult);
      return true;
    }
  
    // =====================================================================
    // =====================================================================
  
  
    bool prepareData()
    {
  
  
      return true;
    }
  
  
    // =====================================================================
    // =====================================================================
  
  
    bool checkConsistency()
    {
  
  
      return true;
    }
  
  
    // =====================================================================
    // =====================================================================
  
  
    bool initializeRun(const openfluid::base::SimulationInfo* SimInfo)
    {
  
  
      return true;
    }
  
    // =====================================================================
    // =====================================================================
  
  
    bool runStep(const openfluid::base::SimulationStatus* SimStatus)
    {
      openfluid::core::Unit* A;
      openfluid::core::DoubleValue Value1, Value2;

      DECLARE_UNITS_ORDERED_LOOP(17);


      BEGIN_UNITS_ORDERED_LOOP(17,"unitsA",A)

        OPENFLUID_GetVariable(A,"var1",SimStatus->getCurrentStep(),Value1);


        if (OPENFLUID_IsVariableExist(A,"var2",SimStatus->getCurrentStep(),openfluid::core::Value::DOUBLE))
        {
          OPENFLUID_GetVariable(A,"var2",SimStatus->getCurrentStep(),Value2);
          Value2 = Value2 * m_Mult;
          OPENFLUID_SetVariable(A,"var2",SimStatus->getCurrentStep(),Value2);
        }
        else
        {
          OPENFLUID_RaiseWarning("examples.primitives.unitsA.up",SimStatus->getCurrentStep(),"var2 not present, init to value 1.0");
          Value2 = 1.0;
          OPENFLUID_AppendVariable(A,"var2",Value2);
        }

        OPENFLUID_AppendVariable(A,"var3",Value1+0.3);

      END_LOOP
  
      return true;
    }
  
    // =====================================================================
    // =====================================================================
  
  
    bool finalizeRun(const openfluid::base::SimulationInfo* SimInfo)
    {
  
  
      return true;
    }

};


// =====================================================================
// =====================================================================


DEFINE_FUNCTION_HOOK(ExampleUnitsAUpdate);

