/**
  @file
  @brief Implements LandPrimitivesSimulator

  @author  <>
 */


#include <openfluid/ware/PluggableSimulator.hpp>


// =====================================================================
// =====================================================================


DECLARE_SIMULATOR_PLUGIN


BEGIN_SIMULATOR_SIGNATURE("tests.primitives.land.prod")

  DECLARE_NAME("");
  DECLARE_DESCRIPTION("");

  DECLARE_VERSION("10.07");
  DECLARE_SDKVERSION;
  DECLARE_STATUS(openfluid::ware::EXPERIMENTAL);

  DECLARE_DOMAIN("");
  DECLARE_PROCESS("");
  DECLARE_METHOD("");
  DECLARE_AUTHOR("","");

  DECLARE_PRODUCED_VAR("tests.var1","TU","","");
  DECLARE_PRODUCED_VAR("tests.var2","MU","","");

  DECLARE_PRODUCED_ATTRIBUTE("attr1","TU","","");

END_SIMULATOR_SIGNATURE


// =====================================================================
// =====================================================================


class LandPrimitivesProdSimulator : public openfluid::ware::PluggableSimulator
{
  private:

  public:


    LandPrimitivesProdSimulator() : PluggableSimulator()
    {


    }


    // =====================================================================
    // =====================================================================


    ~LandPrimitivesProdSimulator()
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
    { }


    // =====================================================================
    // =====================================================================


    openfluid::base::SchedulingRequest initializeRun()
    {
      openfluid::core::SpatialUnit* U;

      OPENFLUID_UNITS_ORDERED_LOOP("TU",U)
      {
        OPENFLUID_InitializeVariable(U,"tests.var1",0.0);
      }

      OPENFLUID_UNITS_ORDERED_LOOP("MU",U)
      {
        OPENFLUID_InitializeVariable(U,"tests.var2",0.0);
      }

      return DefaultDeltaT();
    }


    // =====================================================================
    // =====================================================================


    openfluid::base::SchedulingRequest runStep()
    {
      openfluid::core::SpatialUnit* U;

      OPENFLUID_UNITS_ORDERED_LOOP("TU",U)
      {
        OPENFLUID_AppendVariable(U,"tests.var1",0.0);
      }

      OPENFLUID_UNITS_ORDERED_LOOP("MU",U)
      {
        OPENFLUID_AppendVariable(U,"tests.var2",0.0);
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

DEFINE_SIMULATOR_CLASS(LandPrimitivesProdSimulator)

