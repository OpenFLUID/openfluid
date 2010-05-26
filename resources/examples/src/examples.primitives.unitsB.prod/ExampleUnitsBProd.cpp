/**
  \file ExampleFuncProd.cpp
*/


#include <openfluid/base.hpp>
#include <openfluid/core.hpp>



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
      openfluid::core::ScalarValue Value5, AuxValue;

      DECLARE_UNITS_ORDERED_LOOP(1);
      DECLARE_UNITS_LIST_LOOP(5);
      DECLARE_UNITS_LIST_LOOP(18);

      BEGIN_UNITS_ORDERED_LOOP(1,"unitsB",B)

        Value5 = 0.0;
        FromAList = NULL;
        FromBList = NULL;

        FromAList = B->getFromUnits("unitsA");

        if (FromAList != NULL)
        {
          BEGIN_UNITS_LIST_LOOP(5,FromAList,FromA)

            if (OPENFLUID_IsScalarVariableExist(FromA,"var2",SimStatus->getCurrentStep()))
            {
              OPENFLUID_GetVariable(FromA,"var2",SimStatus->getCurrentStep(),&AuxValue);
              Value5 = Value5 + AuxValue;
            }
            else OPENFLUID_RaiseWarning("examples.primitives.unitsB.prod",SimStatus->getCurrentStep(),"var2 is not present, ignored");

            if (OPENFLUID_IsScalarVariableExist(FromA,"var3",SimStatus->getCurrentStep()))
            {
              OPENFLUID_GetVariable(FromA,"var3",SimStatus->getCurrentStep(),&AuxValue);
              Value5 = Value5 + AuxValue;
            }
            else OPENFLUID_RaiseWarning("examples.primitives.unitsB.prod",SimStatus->getCurrentStep(),"var3 is not present, ignored");

          END_LOOP
        }

        if (!SimStatus->isFirstStep())
        {
          FromBList = B->getFromUnits("unitsB");

          if (FromBList != NULL)
          {
            BEGIN_UNITS_LIST_LOOP(18,FromBList,FromB)

              OPENFLUID_GetVariable(FromB,"var5",SimStatus->getCurrentStep()-1,&AuxValue);
              Value5 = Value5 + AuxValue;

            END_LOOP
          }
        }

        OPENFLUID_AppendVariable(B,"var5",Value5);

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


DEFINE_FUNCTION_HOOK(ExampleUnitsBProduction);

