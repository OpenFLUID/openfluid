/**
  \file SUGUExchFunc.h
  \brief header of SUGUExchangeFunction

  \author Jean-Christophe FABRE <fabrejc@ensam.inra.fr>
*/


#include "SUGUExchFunc.h"


SUGUExchangeFunction::SUGUExchangeFunction(mhydasdk::core::CoreRepository *CoreData)
                : Function(CoreData)
{

}


// =====================================================================
// =====================================================================


SUGUExchangeFunction::~SUGUExchangeFunction()
{

}


// =====================================================================
// =====================================================================


bool SUGUExchangeFunction::initParams(mhydasdk::core::ParamsMap Params)
{

  return true;
}


// =====================================================================
// =====================================================================


bool SUGUExchangeFunction::initializeRun(mhydasdk::base::SimulationInfo* SimInfo)
{

  return true;
}


// =====================================================================
// =====================================================================


bool SUGUExchangeFunction::checkConsistency()
{

  return Function::checkConsistency();
}


// =====================================================================
// =====================================================================


bool SUGUExchangeFunction::runStep(mhydasdk::base::SimulationStatus* SimStatus)
{

  return true;
}


// =====================================================================
// =====================================================================


bool SUGUExchangeFunction::finalizeRun(mhydasdk::base::SimulationInfo* SimInfo)
{

  return true;
}


