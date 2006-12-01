/**
  \file HayamiSUFunc.h
  \brief header of HayamiSUFunction

  \author Jean-Christophe FABRE <fabrejc@ensam.inra.fr>
*/


#include "HayamiSUFunc.h"


HayamiSUFunction::HayamiSUFunction(mhydasdk::core::CoreRepository *CoreData)
                : Function(CoreData)
{

}


// =====================================================================
// =====================================================================


HayamiSUFunction::~HayamiSUFunction()
{

}


// =====================================================================
// =====================================================================


bool HayamiSUFunction::initParams(mhydasdk::core::ParamsMap Params)
{

  return true;
}


// =====================================================================
// =====================================================================


bool HayamiSUFunction::initializeRun()
{

  return true;
}


// =====================================================================
// =====================================================================


bool HayamiSUFunction::checkConsistency()
{

  return Function::checkConsistency();
}


// =====================================================================
// =====================================================================


bool HayamiSUFunction::runStep(mhydasdk::base::SimulationStatus* SimStatus)
{

  return true;
}


// =====================================================================
// =====================================================================


bool HayamiSUFunction::finalizeRun()
{

  return true;
}


