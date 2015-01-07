/**
  \file ExampleUnitsBProd.cpp
*/


#include <openfluid/ware/PluggableSimulator.hpp>



// =====================================================================
// =====================================================================


DECLARE_SIMULATOR_PLUGIN;


// =====================================================================
// =====================================================================


BEGIN_SIMULATOR_SIGNATURE("examples.primitives.unitsB.prod")

  DECLARE_NAME("Example of data production on units of class B");
  DECLARE_DESCRIPTION("");

  DECLARE_VERSION(openfluid::config::FULL_VERSION);

  DECLARE_STATUS(openfluid::ware::EXPERIMENTAL);

  DECLARE_DOMAIN("examples");
  DECLARE_PROCESS("");
  DECLARE_METHOD("");
  DECLARE_AUTHOR("Jean-Christophe Fabre","fabrejc@supagro.inra.fr");

  DECLARE_USED_VAR("var2","unitsA","the variable 2","");
  DECLARE_USED_VAR("var3","unitsA","the variable 3","");
  
  DECLARE_PRODUCED_VAR("var5","unitsB","the variable 5","");
  

END_SIMULATOR_SIGNATURE


// =====================================================================
// =====================================================================


/**

*/
class ExampleUnitsBProduction : public openfluid::ware::PluggableSimulator
{
  private:

  
  public:

  
    ExampleUnitsBProduction() : PluggableSimulator()
    {
  
  
    }
  
  
    // =====================================================================
    // =====================================================================
  
  
    ~ExampleUnitsBProduction()
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
      openfluid::core::SpatialUnit *B;

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

      openfluid::core::SpatialUnit *FromA, *FromB, *B;
      openfluid::core::UnitsPtrList_t *FromAList, *FromBList;
      openfluid::core::DoubleValue Value5, AuxValue;

      unsigned int CurrentTimeIndex = OPENFLUID_GetCurrentTimeIndex();

      OPENFLUID_UNITS_ORDERED_LOOP("unitsB",B)
      {

        Value5 = 0.0;
        FromAList = NULL;
        FromBList = NULL;

        FromAList = B->fromSpatialUnits("unitsA");

        if (FromAList != NULL)
        {
          OPENFLUID_UNITSLIST_LOOP(FromAList,FromA)
          {

            if (OPENFLUID_IsVariableExist(FromA,"var2",CurrentTimeIndex))
            {
              OPENFLUID_GetVariable(FromA,"var2",CurrentTimeIndex,AuxValue);
              Value5 = Value5 + AuxValue;
            }
            else OPENFLUID_RaiseWarning("examples.primitives.unitsB.prod","var2 is not present, ignored");

            if (OPENFLUID_IsVariableExist(FromA,"var3",CurrentTimeIndex))
            {
              OPENFLUID_GetVariable(FromA,"var3",CurrentTimeIndex,AuxValue);
              Value5 = Value5 + AuxValue;
            }
            else OPENFLUID_RaiseWarning("examples.primitives.unitsB.prod","var3 is not present, ignored");

          }
        }

        if (OPENFLUID_GetCurrentTimeIndex()>0)
        {
          FromBList = B->fromSpatialUnits("unitsB");

          if (FromBList != NULL)
          {
            OPENFLUID_UNITSLIST_LOOP(FromBList,FromB)
            {
              OPENFLUID_GetVariable(FromB,"var5",CurrentTimeIndex-OPENFLUID_GetDefaultDeltaT(),AuxValue);
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


DEFINE_SIMULATOR_CLASS(ExampleUnitsBProduction);

