/**
  \file
  \brief implements ...

  \author Jean-Christophe FABRE <fabrejc@ensam.inra.fr>
*/

#include "MSeytouxFunc.h"
#include <iostream>

MorelSeytouxFunc::MorelSeytouxFunc(mhydasdk::core::CoreRepository *CoreData)
                : Function(CoreData)
{
  SU_VARIABLE_TO_CREATE("runoff");
  SU_VARIABLE_TO_CREATE("infiltration");

  SU_PROPERTY_TO_CHECK("ks");
  SU_PROPERTY_TO_CHECK("thetares");
  SU_PROPERTY_TO_CHECK("thetasat");  
  SU_PROPERTY_TO_CHECK("betaMS");  
  SU_PROPERTY_TO_CHECK("hc");  
  SU_INICOND_TO_CHECK("thetaisurf");
}

// =====================================================================
// =====================================================================

MorelSeytouxFunc::~MorelSeytouxFunc()
{

}



// =====================================================================
// =====================================================================

bool MorelSeytouxFunc::initParams(mhydasdk::core::ParamsMap Params)
{

  //std::cerr << "MorelSeytouxFunc::initParams " << Params.size() << std::endl;
  // std::cout << "Momo initParams()" << std::endl;
  return true;
}


// =====================================================================
// =====================================================================


bool MorelSeytouxFunc::initializeRun()
{

  bool IsOK =  true;


  return IsOK;
}


// =====================================================================
// =====================================================================


bool MorelSeytouxFunc::checkConsistency()
{
  // std::cout << "Momo checkConsistency()" << std::endl;

  return Function::checkConsistency();
}


// =====================================================================
// =====================================================================


bool MorelSeytouxFunc::runStep(mhydasdk::base::SimulationStatus* SimStatus)
{

  double Value;

  mhydasdk::core::SurfaceUnit* SU;

  BEGIN_SU_ORDERED_LOOP(SU)
    Value = GET_SU_RAINVALUE(SU,SimStatus->getCurrentStep());
    APPEND_SIMVAR_VALUE(SU,"runoff",Value * 0.6);
    APPEND_SIMVAR_VALUE(SU,"infiltration",Value * 0.4);
  END_LOOP


  // std::cout << "coucou c'est Momo run " << mp_CoreData->getSpatialData()->getSUsCollection()->size();
  return true;
}


// =====================================================================
// =====================================================================


bool MorelSeytouxFunc::finalizeRun()
{
  // std::cout << "Momo finalize()" << std::endl;
  return true;
}

