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
  DECLARE_SIGNATURE_ID(("examples.primitives.unitsB.prod"));
  DECLARE_SIGNATURE_NAME(("Example of data production on units of class B"));
  DECLARE_SIGNATURE_DESCRIPTION((""));

  DECLARE_SIGNATURE_VERSION(("1.0"));
  DECLARE_SIGNATURE_SDKVERSION;
  DECLARE_SIGNATURE_STATUS(openfluid::base::EXPERIMENTAL);

  DECLARE_SIGNATURE_DOMAIN(("examples"));
  DECLARE_SIGNATURE_PROCESS((""));
  DECLARE_SIGNATURE_METHOD((""));
  DECLARE_SIGNATURE_AUTHORNAME(("Jean-Christophe Fabre"));
  DECLARE_SIGNATURE_AUTHOREMAIL(("fabrejc@supagro.inra.fr"));

  DECLARE_USED_VAR("var2","unitsA","the variable 2","");
  DECLARE_USED_VAR("var3","unitsA","the variable 3","");
  
  DECLARE_PRODUCED_VAR("var5","unitsB","the variable 5","");
  

END_SIGNATURE_HOOK


// =====================================================================
// =====================================================================


/**

*/
class ExampleUnitsBProduction : public openfluid::base::PluggableFunction
{
  private:

  
  public:

  
    ExampleUnitsBProduction() : PluggableFunction()
    {
  
  
    }
  
  
    // =====================================================================
    // =====================================================================
  
  
    ~ExampleUnitsBProduction()
    {
  
  
    }
  
  
    // =====================================================================
    // =====================================================================
  
  
    bool initParams(openfluid::core::FuncParamsMap_t Params)
    {
  
  
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

      openfluid::core::Unit *FromA, *FromB, *B;
      openfluid::core::UnitsPtrList_t *FromAList, *FromBList;
      openfluid::core::DoubleValue Value5, AuxValue;


      OPENFLUID_UNITS_ORDERED_LOOP("unitsB",B)
      {

        Value5 = 0.0;
        FromAList = NULL;
        FromBList = NULL;

        FromAList = B->getFromUnits("unitsA");

        if (FromAList != NULL)
        {
          OPENFLUID_UNITSLIST_LOOP(FromAList,FromA)
          {

            if (OPENFLUID_IsVariableExist(FromA,"var2",SimStatus->getCurrentStep()))
            {
              OPENFLUID_GetVariable(FromA,"var2",SimStatus->getCurrentStep(),AuxValue);
              Value5 = Value5 + AuxValue;
            }
            else OPENFLUID_RaiseWarning("examples.primitives.unitsB.prod",SimStatus->getCurrentStep(),"var2 is not present, ignored");

            if (OPENFLUID_IsVariableExist(FromA,"var3",SimStatus->getCurrentStep()))
            {
              OPENFLUID_GetVariable(FromA,"var3",SimStatus->getCurrentStep(),AuxValue);
              Value5 = Value5 + AuxValue;
            }
            else OPENFLUID_RaiseWarning("examples.primitives.unitsB.prod",SimStatus->getCurrentStep(),"var3 is not present, ignored");

          }
        }

        if (!SimStatus->isFirstStep())
        {
          FromBList = B->getFromUnits("unitsB");

          if (FromBList != NULL)
          {
            OPENFLUID_UNITSLIST_LOOP(FromBList,FromB)
            {
              OPENFLUID_GetVariable(FromB,"var5",SimStatus->getCurrentStep()-1,AuxValue);
              Value5 = Value5 + AuxValue;

            }
          }
        }

        OPENFLUID_AppendVariable(B,"var5",Value5);

      }


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


DEFINE_FUNCTION_HOOK(ExampleUnitsBProduction);

