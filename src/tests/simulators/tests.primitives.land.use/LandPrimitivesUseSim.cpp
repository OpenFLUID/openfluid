/**
  @file
  @brief Implements LandPrimitivesSimulator

  @author  <>
 */


#include <openfluid/ware/PluggableSimulator.hpp>


// =====================================================================
// =====================================================================


DECLARE_SIMULATOR_PLUGIN


BEGIN_SIMULATOR_SIGNATURE("tests.primitives.land.use")

  DECLARE_NAME("");
  DECLARE_DESCRIPTION("");

  DECLARE_VERSION("10.07");
  DECLARE_SDKVERSION;
  DECLARE_STATUS(openfluid::ware::EXPERIMENTAL);

  DECLARE_DOMAIN("");
  DECLARE_PROCESS("");
  DECLARE_METHOD("");
  DECLARE_AUTHOR("","");

  DECLARE_REQUIRED_VAR("tests.var1","TU","","");
  DECLARE_REQUIRED_VAR("tests.var2","MU","","");

  DECLARE_REQUIRED_ATTRIBUTE("attr1","TU","","");

END_SIMULATOR_SIGNATURE


// =====================================================================
// =====================================================================


class LandPrimitivesUseSimulator : public openfluid::ware::PluggableSimulator
{
  private:

  public:


    LandPrimitivesUseSimulator() : PluggableSimulator()
    {


    }


    // =====================================================================
    // =====================================================================


    ~LandPrimitivesUseSimulator()
    {


    }


    // =====================================================================
    // =====================================================================


    void initParams(const openfluid::ware::WareParams_t& /*Params*/)
    {  }


    // =====================================================================
    // =====================================================================


    void prepareData()
    {  }


    // =====================================================================
    // =====================================================================


    void checkConsistency()
    {  }


    // =====================================================================
    // =====================================================================


    openfluid::base::SchedulingRequest initializeRun()
    {
      openfluid::core::Unit* U;
      openfluid::core::DoubleValue TmpValue;

      OPENFLUID_UNITS_ORDERED_LOOP("TU",U)
      {
        OPENFLUID_GetVariable(U,"tests.var1",TmpValue);
      }

      OPENFLUID_UNITS_ORDERED_LOOP("MU",U)
      {
        OPENFLUID_GetVariable(U,"tests.var2",TmpValue);
      }

      return DefaultDeltaT();
    }


    // =====================================================================
    // =====================================================================


    openfluid::base::SchedulingRequest runStep()
    {
      openfluid::core::Unit* U;
      openfluid::core::DoubleValue TmpValue;

      OPENFLUID_UNITS_ORDERED_LOOP("TU",U)
      {
        OPENFLUID_GetVariable(U,"tests.var1",TmpValue);
      }

      OPENFLUID_UNITS_ORDERED_LOOP("MU",U)
      {
        OPENFLUID_GetVariable(U,"tests.var2",TmpValue);
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


DEFINE_SIMULATOR_CLASS(LandPrimitivesUseSimulator)

