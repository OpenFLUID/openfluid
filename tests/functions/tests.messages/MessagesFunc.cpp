/***copyright>=========================================================

  OpenFLUID - Software environment for modelling Fluxes In Landscapes

  This file is a part of OpenFLUID software
  http://www.umr-lisah.fr/openfluid
  Copyright (c) INRA
  See the file "copyright" for the full license governing this code.

=========================================================<copyright***/


/**
  \file PrimitivesProdFunc.cpp
  \brief Implements ...
*/


#include "MessagesFunc.h"


// =====================================================================
// =====================================================================


DEFINE_FUNCTION_HOOK(MessagesFunction)


// =====================================================================
// =====================================================================


BEGIN_SIGNATURE_HOOK
  DECLARE_SIGNATURE_ID(("tests.messages"));
  DECLARE_SIGNATURE_NAME(("test function for messages"));
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


MessagesFunction::MessagesFunction()
                : PluggableFunction()
{


}


// =====================================================================
// =====================================================================


MessagesFunction::~MessagesFunction()
{


}


// =====================================================================
// =====================================================================


bool MessagesFunction::initParams(openfluid::core::FuncParamsMap_t Params)
{

  m_RepeatMessages = 1;

  OPENFLUID_GetFunctionParameter(Params,"rptmsgs",&m_RepeatMessages);

  return true;
}

// =====================================================================
// =====================================================================


bool MessagesFunction::prepareData()
{


  return true;
}


// =====================================================================
// =====================================================================


bool MessagesFunction::checkConsistency()
{


  return true;
}


// =====================================================================
// =====================================================================


bool MessagesFunction::initializeRun(const openfluid::base::SimulationInfo* SimInfo)
{

  OPENFLUID_RaiseWarning("tests.messages","initializeRun()","Message from tests.messages function");

  return true;
}

// =====================================================================
// =====================================================================


bool MessagesFunction::runStep(const openfluid::base::SimulationStatus* SimStatus)
{
  openfluid::core::Unit* TU;

  int i = 0;

  std::string RptStr, IDStr, TSStr;

  openfluid::tools::ConvertValue(SimStatus->getCurrentStep(),&TSStr);

  DECLARE_UNITS_ORDERED_LOOP(1);

  BEGIN_UNITS_ORDERED_LOOP(1,"TestUnits",TU)

    openfluid::tools::ConvertValue(TU->getID(),&IDStr);

    for (i = 0; i< m_RepeatMessages; i++)
    {
      openfluid::tools::ConvertValue((i+1),&RptStr);
      OPENFLUID_RaiseWarning("tests.messages","runStep()",SimStatus->getCurrentStep(),"["+TSStr+"|"+IDStr+"|"+RptStr+"] Message from tests.messages function");
    }

  END_LOOP


  return true;
}

// =====================================================================
// =====================================================================


bool MessagesFunction::finalizeRun(const openfluid::base::SimulationInfo* SimInfo)
{

  OPENFLUID_RaiseWarning("tests.messages","finalizeRun()","Message from tests.messages function");

  return true;
}

