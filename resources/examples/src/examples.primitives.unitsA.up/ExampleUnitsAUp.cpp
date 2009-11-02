/**
  \file ExampleFuncProd.cpp
*/


#include "openfluid-base.h"
#include "openfluid-core.h"


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

  
  
  
  
  

END_SIGNATURE_HOOK


// =====================================================================
// =====================================================================


/**

*/
class ExampleUnitsAUpdate : public openfluid::base::PluggableFunction
{
  private:

  
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

