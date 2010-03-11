/***copyright>=========================================================

  OpenFLUID - Software environment for modelling Fluxes In Landscapes

  This file is a part of OpenFLUID software
  http://www.umr-lisah.fr/openfluid
  Copyright (c) INRA
  See the file "copyright" for the full license governing this code.

=========================================================<copyright***/


/**
  \file OutputsFunc.cpp
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
  DECLARE_SIGNATURE_ID("tests.outputs");
  DECLARE_SIGNATURE_NAME("test function for outputs");
  DECLARE_SIGNATURE_SDKVERSION;

  DECLARE_SIGNATURE_STATUS(openfluid::base::EXPERIMENTAL);

  DECLARE_PRODUCED_VAR("tests.vector[]","UnitsA","vector for tests","");
  DECLARE_PRODUCED_VAR("tests.scalar","UnitsA","scalar for tests","");
  DECLARE_PRODUCED_VAR("tests.vector[]","UnitsB","vector for tests","");
  DECLARE_PRODUCED_VAR("tests.scalar","UnitsB","scalar for tests","");

END_SIGNATURE_HOOK


class OutputsFunction : public openfluid::base::PluggableFunction
{
  private:

  public:


  OutputsFunction() : PluggableFunction()
  {


  }


  // =====================================================================
  // =====================================================================


  ~OutputsFunction()
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
    openfluid::core::Unit *TUA, *TUB;
    long VectorSize = SimStatus->getCurrentStep() % 40;
    openfluid::core::VectorValue TheVector;
    std::string IDStr, TSStr;

    openfluid::tools::ConvertValue(SimStatus->getCurrentStep(),&TSStr);

    DECLARE_UNITS_ORDERED_LOOP(1);
    DECLARE_UNITS_ORDERED_LOOP(5);

    BEGIN_UNITS_ORDERED_LOOP(1,"UnitsA",TUA)

      TheVector = openfluid::core::VectorValue(VectorSize,double(TUA->getID()));
      OPENFLUID_AppendVariable(TUA,"tests.vector",TheVector);
      OPENFLUID_AppendVariable(TUA,"tests.scalar",double(TUA->getID()));

      openfluid::tools::ConvertValue(TUA->getID(),&IDStr);
      OPENFLUID_RaiseWarning("tests.messages",SimStatus->getCurrentStep(),"["+TUA->getClass()+"|"+IDStr+"|"+TSStr+"] Message from tests.outputs function");

    END_LOOP

    BEGIN_UNITS_ORDERED_LOOP(5,"UnitsB",TUB)

      TheVector = openfluid::core::VectorValue(VectorSize,double(TUB->getID()));
      OPENFLUID_AppendVariable(TUB,"tests.vector",TheVector);
      OPENFLUID_AppendVariable(TUB,"tests.scalar",double(TUB->getID()));

      openfluid::tools::ConvertValue(TUB->getID(),&IDStr);
      OPENFLUID_RaiseWarning("tests.messages",SimStatus->getCurrentStep(),"["+TUB->getClass()+"|"+IDStr+"|"+TSStr+"] Message from tests.outputs function");

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

DEFINE_FUNCTION_HOOK(OutputsFunction)

