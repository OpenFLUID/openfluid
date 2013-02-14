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
  DECLARE_SIGNATURE_ID(("examples.primitives.unitsB.prod"));
  DECLARE_SIGNATURE_NAME(("Example of data production on units of class B"));
  DECLARE_SIGNATURE_DESCRIPTION((""));

  DECLARE_SIGNATURE_VERSION(("1.0"));
  DECLARE_SIGNATURE_SDKVERSION;
  DECLARE_SIGNATURE_STATUS(openfluid::ware::EXPERIMENTAL);

  DECLARE_SIGNATURE_DOMAIN(("examples"));
  DECLARE_SIGNATURE_PROCESS((""));
  DECLARE_SIGNATURE_METHOD((""));
  DECLARE_SIGNATURE_AUTHORNAME(("Jean-Christophe Fabre"));
  DECLARE_SIGNATURE_AUTHOREMAIL(("fabrejc@supagro.inra.fr"));

  DECLARE_USED_VAR("var2","unitsA","the variable 2","");
  DECLARE_USED_VAR("var3","unitsA","the variable 3","");
  
  DECLARE_PRODUCED_VAR("var5","unitsB","the variable 5","");
  

END_FUNCTION_SIGNATURE


// =====================================================================
// =====================================================================


/**

*/
class ExampleUnitsBProduction : public openfluid::ware::PluggableFunction
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
  
  
    void initParams(const openfluid::ware::WareParams_t& Params)
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
      openfluid::core::Unit *B;

      OPENFLUID_UNITS_ORDERED_LOOP("unitsB",B)
      {
        OPENFLUID_InitializeVariable(B,"var5",0.0);
      }

      return DefaultDeltaT();
    }
  
    // =====================================================================
    // =====================================================================
  
  
    openfluid::base::SchedulingRequest runStep()
    {

      openfluid::core::Unit *FromA, *FromB, *B;
      openfluid::core::UnitsPtrList_t *FromAList, *FromBList;
      openfluid::core::DoubleValue Value5, AuxValue;

      unsigned int CurrentStep = (OPENFLUID_GetCurrentTimeIndex());

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

            if (OPENFLUID_IsVariableExist(FromA,"var2",CurrentStep))
            {
              OPENFLUID_GetVariable(FromA,"var2",CurrentStep,AuxValue);
              Value5 = Value5 + AuxValue;
            }
            else OPENFLUID_RaiseWarning("examples.primitives.unitsB.prod","var2 is not present, ignored");

            if (OPENFLUID_IsVariableExist(FromA,"var3",CurrentStep))
            {
              OPENFLUID_GetVariable(FromA,"var3",CurrentStep,AuxValue);
              Value5 = Value5 + AuxValue;
            }
            else OPENFLUID_RaiseWarning("examples.primitives.unitsB.prod","var3 is not present, ignored");

          }
        }

        if (OPENFLUID_GetCurrentTimeIndex()>0)
        {
          FromBList = B->getFromUnits("unitsB");

          if (FromBList != NULL)
          {
            OPENFLUID_UNITSLIST_LOOP(FromBList,FromB)
            {
              OPENFLUID_GetVariable(FromB,"var5",CurrentStep-OPENFLUID_GetDefaultDeltaT(),AuxValue);
              Value5 = Value5 + AuxValue;

            }
          }
        }

        OPENFLUID_AppendVariable(B,"var5",Value5);

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


DEFINE_FUNCTION_CLASS(ExampleUnitsBProduction);

