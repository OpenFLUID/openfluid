/**
  \file RSGUExchFunc.h
  \brief header of RSGUExchangeFunction

  \author Jean-Christophe FABRE <fabrejc@ensam.inra.fr>
*/


#include "RSGUExchFunc.h"


RSGUExchangeFunction::RSGUExchangeFunction(mhydasdk::core::CoreRepository *CoreData)
                : Function(CoreData)
{

}


// =====================================================================
// =====================================================================


RSGUExchangeFunction::~RSGUExchangeFunction()
{

}


// =====================================================================
// =====================================================================


bool RSGUExchangeFunction::initParams(mhydasdk::core::ParamsMap Params)
{

  return true;
}


// =====================================================================
// =====================================================================


bool RSGUExchangeFunction::initializeRun(mhydasdk::base::SimulationInfo* SimInfo)
{

  return true;
}


// =====================================================================
// =====================================================================


bool RSGUExchangeFunction::checkConsistency()
{

  return Function::checkConsistency();
}


// =====================================================================
// =====================================================================


bool RSGUExchangeFunction::runStep(mhydasdk::base::SimulationStatus* SimStatus)
{

  return true;
}


// =====================================================================
// =====================================================================


bool RSGUExchangeFunction::finalizeRun(mhydasdk::base::SimulationInfo* SimInfo)
{

  return true;
}


