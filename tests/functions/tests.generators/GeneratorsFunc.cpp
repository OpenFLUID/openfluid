/***copyright>=========================================================

  OpenFLUID - Software environment for modelling Fluxes In Landscapes

  This file is a part of OpenFLUID software
  http://www.umr-lisah.fr/openfluid
  Copyright (c) INRA
  See the file "copyright" for the full license governing this code.

=========================================================<copyright***/


/**
  \file GeneratorsFunc.cpp
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
  DECLARE_SIGNATURE_ID(("tests.generators"));
  DECLARE_SIGNATURE_NAME(("test function for generators"));
  DECLARE_SIGNATURE_DESCRIPTION((""));

  DECLARE_SIGNATURE_VERSION(("1.0"));
  DECLARE_SIGNATURE_SDKVERSION;
  DECLARE_SIGNATURE_STATUS(openfluid::base::EXPERIMENTAL);

  DECLARE_SIGNATURE_DOMAIN((""));
  DECLARE_SIGNATURE_PROCESS((""));
  DECLARE_SIGNATURE_METHOD((""));
  DECLARE_SIGNATURE_AUTHORNAME((""));
  DECLARE_SIGNATURE_AUTHOREMAIL((""));

  DECLARE_REQUIRED_VAR("tests.fixed","TestUnits","fixed value from generators for tests","");
  DECLARE_REQUIRED_VAR("tests.random","TestUnits","random value from generators for tests","");
  DECLARE_REQUIRED_VAR("tests.interp","TestUnits","interpolated value from generators for tests","");

  DECLARE_REQUIRED_PREVVAR("tests.fixedprev[]","TestUnits","fixed value from generators at a previous time step for tests","");

END_SIGNATURE_HOOK

/**

*/
class GeneratorsFunction : public openfluid::base::PluggableFunction
{
  private:

  public:


  GeneratorsFunction() : PluggableFunction()
  {


  }


  // =====================================================================
  // =====================================================================


  ~GeneratorsFunction()
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
    openfluid::core::Unit* TU;
    DECLARE_UNITS_ORDERED_LOOP(1);

    openfluid::core::ScalarValue SValue;
    openfluid::core::VectorValue VValue;


    BEGIN_UNITS_ORDERED_LOOP(1,"TestUnits",TU)

      OPENFLUID_GetVariable(TU,"tests.fixed",SimStatus->getCurrentStep(),&SValue);
      if (!openfluid::tools::IsCloseEnough(SValue,12.7))
        OPENFLUID_RaiseError("tests.generators","incorrect value for tests.fixed variable");


      OPENFLUID_GetVariable(TU,"tests.random",SimStatus->getCurrentStep(),&SValue);
      if (!(SValue >= 20.53 && SValue<= 50.0))
        OPENFLUID_RaiseError("tests.generators","incorrect value for tests.random variable");


      OPENFLUID_GetVariable(TU,"tests.interp",SimStatus->getCurrentStep(),&SValue);

      if (TU->getID() % 2 != 0)
      {
        if (!(SValue >= -15.0 && SValue <= -1.0))
          OPENFLUID_RaiseError("tests.generators","incorrect value for tests.interp variable (source1.dat)");
      }
      else
      {
        if (!(SValue >= 101.0 && SValue<= 115.0))
          OPENFLUID_RaiseError("tests.generators","incorrect value for tests.interp variable (source2.dat)");
      }


      OPENFLUID_GetVariable(TU,"tests.interpmin",SimStatus->getCurrentStep(),&SValue);

      if (TU->getID() % 2 != 0)
      {
        if (!(SValue >= 0.0))
          OPENFLUID_RaiseError("tests.generators","incorrect value for tests.interpmin variable (source1.dat)");
      }
      else
      {
        if (!(SValue >= 101.0 && SValue<= 115.0))
          OPENFLUID_RaiseError("tests.generators","incorrect value for tests.interpmin variable (source2.dat)");
      }


      OPENFLUID_GetVariable(TU,"tests.interpminmax",SimStatus->getCurrentStep(),&SValue);

      if (!(SValue >= 0.0 && SValue<= 20.0))
          OPENFLUID_RaiseError("tests.generators","incorrect value for tests.interpminmax variable");


      if (SimStatus->getCurrentStep()>0)
      {
        OPENFLUID_GetVariable(TU,"tests.fixedprev",SimStatus->getCurrentStep()-1,&VValue);
        if (!openfluid::tools::IsCloseEnough(VValue[0],5.3))
          OPENFLUID_RaiseError("tests.generators","incorrect value for tests.fixedprev variable");
      }


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

DEFINE_FUNCTION_HOOK(GeneratorsFunction)

