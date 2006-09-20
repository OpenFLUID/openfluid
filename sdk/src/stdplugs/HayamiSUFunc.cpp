/**
  \file
  \brief implements ...

  \author Jean-Christophe FABRE <fabrejc@ensam.inra.fr>
*/


#include "HayamiSUFunc.h"




HayamiSUFunc::HayamiSUFunc(mhydasdk::core::CoreRepository *CoreData)
                : Function(CoreData)
{
  SU_VARIABLE_TO_CREATE("qoutput");
  SU_VARIABLE_TO_CHECK("runoff");

}

// =====================================================================
// =====================================================================

HayamiSUFunc::~HayamiSUFunc()
{

}


// =====================================================================
// =====================================================================

bool HayamiSUFunc::initParams(mhydasdk::core::ParamsMap Params)
{

  // std::cout << "Yaya initParams()" << std::endl;
  return true;
}


// =====================================================================
// =====================================================================


bool HayamiSUFunc::initializeRun()
{

  bool IsOK =  true;

  return IsOK;
}


// =====================================================================
// =====================================================================


bool HayamiSUFunc::checkConsistency()
{
  // std::cout << "Yaya checkConsistency()" << std::endl;

  return Function::checkConsistency();
}


// =====================================================================
// =====================================================================


bool HayamiSUFunc::runStep(mhydasdk::base::SimulationStatus* SimStatus)
{

  // std::cout << "coucou c'est Yaya run " << mp_CoreData->getSpatialData()->getSUsCollection()->size();
  return true;
}


// =====================================================================
// =====================================================================


bool HayamiSUFunc::finalizeRun()
{
  // std::cout << "HayamiSU finalize()" << std::endl;
  return true;
}

