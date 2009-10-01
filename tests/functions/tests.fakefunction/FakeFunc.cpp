/**
  \file NewFunctionUsingBuddy.cpp
  \brief Implements NewFunctionUsingBuddy

  \author John Doe <doe@foo.bar.org>
 */


#include "openfluid-base.h"
#include "openfluid-core.h"


// =====================================================================
// =====================================================================


DECLARE_PLUGIN_HOOKS


BEGIN_SIGNATURE_HOOK

  DECLARE_SIGNATURE_ID("tests.fakefunction");
  DECLARE_SIGNATURE_NAME("");
  DECLARE_SIGNATURE_DESCRIPTION("");

  DECLARE_SIGNATURE_VERSION("1.0");
  DECLARE_SIGNATURE_SDKVERSION;
  DECLARE_SIGNATURE_STATUS(openfluid::base::EXPERIMENTAL);

  DECLARE_SIGNATURE_DOMAIN("");
  DECLARE_SIGNATURE_PROCESS("");
  DECLARE_SIGNATURE_METHOD("");
  DECLARE_SIGNATURE_AUTHORNAME("");
  DECLARE_SIGNATURE_AUTHOREMAIL("");

END_SIGNATURE_HOOK


// =====================================================================
// =====================================================================


class FakeFunction : public openfluid::base::PluggableFunction
{
  private:

  public:


    FakeFunction() : PluggableFunction()
    {


    }


    // =====================================================================
    // =====================================================================


    ~FakeFunction()
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

DEFINE_FUNCTION_HOOK(FakeFunction)

