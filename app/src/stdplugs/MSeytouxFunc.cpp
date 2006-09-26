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

  list<mhydasdk::core::SurfaceUnit*>::iterator iter;
  list<mhydasdk::core::SurfaceUnit*>* SUsList = mp_CoreData->getSpatialData()->getSUsOrderedList();
  for(iter=SUsList->begin(); iter != SUsList->end(); iter++)
  {
    mhydasdk::core::SurfaceUnit* SU = *iter;

    Value = (SU->getRainSource()->getTimeSerie()->getItemsCollection()->at(SimStatus->getCurrentStep())->getValue());
    (SU->getSimulatedVars()->find(wxT("runoff"))->second)->push_back(Value * 0.6);
    (SU->getSimulatedVars()->find(wxT("infiltration"))->second)->push_back(Value * 0.4);

  }

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

