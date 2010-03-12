/***copyright>=========================================================

  OpenFLUID - Software environment for modelling Fluxes In Landscapes

  This file is a part of OpenFLUID software
  http://www.umr-lisah.fr/openfluid
  Copyright (c) INRA
  See the file "copyright" for the full license governing this code.

=========================================================<copyright***/


/**
  \file GlobalParams.cpp
  \brief Implements ...

  \author Jean-Christophe FABRE <fabrejc@supagro.inra.fr>
 */


#include "openfluid-base.h"
#include "openfluid-core.h"
#include "openfluid-tools.h"


// =====================================================================
// =====================================================================


DECLARE_PLUGIN_HOOKS

// =====================================================================
// =====================================================================


BEGIN_SIGNATURE_HOOK
  DECLARE_SIGNATURE_ID(("tests.globalparams"));
  DECLARE_SIGNATURE_NAME(("test function for global parameters of models"));
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

/**

*/
class GlobalParamsFunction : public openfluid::base::PluggableFunction
{
  private:

  public:


  GlobalParamsFunction() : PluggableFunction()
  {


  }


  // =====================================================================
  // =====================================================================


  ~GlobalParamsFunction()
  {


  }


  // =====================================================================
  // =====================================================================


  bool initParams(openfluid::core::FuncParamsMap_t Params)
  {
    std::string StrParam;
    long LongParam;
    double DoubleParam;


    if (!OPENFLUID_GetFunctionParameter(Params,"gparam1",&LongParam))
      OPENFLUID_RaiseError("tests.globalparams","gparam1 not found");

    if (LongParam != 1)
      OPENFLUID_RaiseError("tests.globalparams","wrong value for gparam1");


    if (!OPENFLUID_GetFunctionParameter(Params,"lparam1",&LongParam))
      OPENFLUID_RaiseError("tests.globalparams","lparam1 not found");

    if (LongParam != 100)
      OPENFLUID_RaiseError("tests.globalparams","wrong value for lparam1");


    if (!OPENFLUID_GetFunctionParameter(Params,"gparam2",&StrParam))
      OPENFLUID_RaiseError("tests.globalparams","gparam2 not found");

    if (StrParam != "nineteen")
      OPENFLUID_RaiseError("tests.globalparams","wrong value for gparam2");


    if (!OPENFLUID_GetFunctionParameter(Params,"gparam3",&DoubleParam))
      OPENFLUID_RaiseError("tests.globalparams","gparam3 not found");

    if (!openfluid::tools::IsCloseEnough(DoubleParam,0.1,0.00001))
      OPENFLUID_RaiseError("tests.globalparams","wrong value for gparam3");


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

DEFINE_FUNCTION_HOOK(GlobalParamsFunction)

