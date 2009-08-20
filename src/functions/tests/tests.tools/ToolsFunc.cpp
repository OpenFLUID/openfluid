/**
  \file ToolsFunc.cpp
  \brief Implements ...
*/


#include "openfluid-base.h"
#include "openfluid-core.h"


// =====================================================================
// =====================================================================


DECLARE_PLUGIN_HOOKS


// =====================================================================
// =====================================================================


/**

*/
class ToolsFunction : public openfluid::base::PluggableFunction
{
  private:


  public:
    /**
      Constructor
    */
    ToolsFunction();

    /**
      Destructor
    */
    ~ToolsFunction();

    bool initParams(openfluid::core::FuncParamsMap_t Params);

    bool prepareData();

    bool checkConsistency();

    bool initializeRun(const openfluid::base::SimulationInfo* SimInfo);

    bool runStep(const openfluid::base::SimulationStatus* SimStatus);

    bool finalizeRun(const openfluid::base::SimulationInfo* SimInfo);

};



// =====================================================================
// =====================================================================


DEFINE_FUNCTION_HOOK(ToolsFunction)


// =====================================================================
// =====================================================================


BEGIN_SIGNATURE_HOOK
  DECLARE_SIGNATURE_ID(("tests.tools"));
  DECLARE_SIGNATURE_NAME(("test function for openfluid::tools"));
  DECLARE_SIGNATURE_DESCRIPTION((""));

  DECLARE_SIGNATURE_VERSION(("1.0"));
  DECLARE_SIGNATURE_SDKVERSION;
  DECLARE_SIGNATURE_STATUS(openfluid::base::EXPERIMENTAL);

  DECLARE_SIGNATURE_DOMAIN((""));
  DECLARE_SIGNATURE_PROCESS((""));
  DECLARE_SIGNATURE_METHOD((""));
  DECLARE_SIGNATURE_AUTHORNAME((""));
  DECLARE_SIGNATURE_AUTHOREMAIL((""));

END_SIGNATURE_HOOK


// =====================================================================
// =====================================================================


ToolsFunction::ToolsFunction()
                : PluggableFunction()
{


}


// =====================================================================
// =====================================================================


ToolsFunction::~ToolsFunction()
{


}


// =====================================================================
// =====================================================================


bool ToolsFunction::initParams(openfluid::core::FuncParamsMap_t Params)
{

  return true;
}

// =====================================================================
// =====================================================================


bool ToolsFunction::prepareData()
{


  return true;
}


// =====================================================================
// =====================================================================


bool ToolsFunction::checkConsistency()
{


  return true;
}


// =====================================================================
// =====================================================================


bool ToolsFunction::initializeRun(const openfluid::base::SimulationInfo* SimInfo)
{

  return true;
}

// =====================================================================
// =====================================================================


bool ToolsFunction::runStep(const openfluid::base::SimulationStatus* SimStatus)
{

  return true;
}

// =====================================================================
// =====================================================================


bool ToolsFunction::finalizeRun(const openfluid::base::SimulationInfo* SimInfo)
{

  return true;
}

