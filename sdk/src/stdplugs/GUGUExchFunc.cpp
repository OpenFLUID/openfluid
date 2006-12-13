/**
  \file GUGUExchFunc.h
  \brief header of GUGUExchangeFunction

  \author Jean-Christophe FABRE <fabrejc@ensam.inra.fr>
*/


#include "GUGUExchFunc.h"


GUGUExchangeFunction::GUGUExchangeFunction(mhydasdk::core::CoreRepository *CoreData)
                : Function(CoreData)
{

}


// =====================================================================
// =====================================================================


GUGUExchangeFunction::~GUGUExchangeFunction()
{

}


// =====================================================================
// =====================================================================


bool GUGUExchangeFunction::initParams(mhydasdk::core::ParamsMap Params)
{

  return true;
}


// =====================================================================
// =====================================================================


bool GUGUExchangeFunction::initializeRun(mhydasdk::base::SimulationInfo* SimInfo)
{

  return true;
}


// =====================================================================
// =====================================================================


bool GUGUExchangeFunction::checkConsistency()
{

  return Function::checkConsistency();
}


// =====================================================================
// =====================================================================


bool GUGUExchangeFunction::runStep(mhydasdk::base::SimulationStatus* SimStatus)
{

  return true;
}


// =====================================================================
// =====================================================================


bool GUGUExchangeFunction::finalizeRun(mhydasdk::base::SimulationInfo* SimInfo)
{

  return true;
}


