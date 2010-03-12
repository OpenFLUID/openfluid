/***copyright>=========================================================

  OpenFLUID - Software environment for modelling Fluxes In Landscapes

  This file is a part of OpenFLUID software
  http://www.umr-lisah.fr/openfluid
  Copyright (c) INRA
  See the file "copyright" for the full license governing this code.

=========================================================<copyright***/


/**
  \file WrongProdFunc.cpp
  \brief Implements ...

  \author Jean-Christophe FABRE <fabrejc@supagro.inra.fr>
 */


#include "openfluid-base.h"
#include "openfluid-core.h"


// =====================================================================
// =====================================================================


DECLARE_PLUGIN_HOOKS

// =====================================================================
// =====================================================================


BEGIN_SIGNATURE_HOOK
  DECLARE_SIGNATURE_ID(("tests.wrongprod"));
  DECLARE_SIGNATURE_NAME(("test function for wrong production"));
  DECLARE_SIGNATURE_DESCRIPTION((""));

  DECLARE_SIGNATURE_VERSION(("1.0"));
  DECLARE_SIGNATURE_SDKVERSION;
  DECLARE_SIGNATURE_STATUS(openfluid::base::EXPERIMENTAL);

  DECLARE_SIGNATURE_DOMAIN((""));
  DECLARE_SIGNATURE_PROCESS((""));
  DECLARE_SIGNATURE_METHOD((""));
  DECLARE_SIGNATURE_AUTHORNAME((""));
  DECLARE_SIGNATURE_AUTHOREMAIL((""));

  DECLARE_PRODUCED_VAR("tests.wrongvector[]","TestUnits","wrong vector for tests","");
  DECLARE_PRODUCED_VAR("tests.wrongscalar","TestUnits","wrong scalar for tests","");
  DECLARE_PRODUCED_VAR("tests.scalarok","TestUnits","scalar OK for tests","");

END_SIGNATURE_HOOK

/**

*/
class WrongProductionFunction : public openfluid::base::PluggableFunction
{
  private:

  public:


  WrongProductionFunction() : PluggableFunction()
  {


  }


  // =====================================================================
  // =====================================================================


  ~WrongProductionFunction()
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

    BEGIN_UNITS_ORDERED_LOOP(1,"TestUnits",TU)

      OPENFLUID_AppendVariable(TU,"tests.scalarok",double(TU->getID()));

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

DEFINE_FUNCTION_HOOK(WrongProductionFunction)

