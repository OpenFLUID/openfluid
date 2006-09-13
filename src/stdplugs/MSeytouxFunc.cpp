/**
  \file
  \brief implements ...

  \author Jean-Christophe FABRE <fabrejc@ensam.inra.fr>
*/

#include "MSeytouxFunc.h"


MorelSeytouxFunc::MorelSeytouxFunc(mhydasdk::core::CoreRepository *CoreData)
                : Function(CoreData)
{

  m_SUVarsToAdd.Add(wxT("runoff"));
  m_SUVarsToAdd.Add(wxT("infiltration"));


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

  // std::cout << "Momo initParams()" << std::endl;
  return true;
}


// =====================================================================
// =====================================================================


bool MorelSeytouxFunc::initialize()
{

  // std::cerr << "Momo initialize()" << std::endl;

  bool IsOK =  true;

  IsOK = mhydasdk::base::Function::initialize();

//  std::cerr << "" << std::endl;

  /*
  std::cerr << "ajout " << addSpatialSimulationVar(wxT("runoff"),mp_CoreData->getSpatialData()->getSUsCollection()) << std::endl;

  std::cerr << "check " << checkSpatialSimulationVar(wxT("runoff"),mp_CoreData->getSpatialData()->getSUsCollection()) << std::endl;

  std::cerr << "check " << checkSpatialSimulationVar(wxT("infiltration"),mp_CoreData->getSpatialData()->getSUsCollection()) << std::endl;
 */
  return IsOK;
}


// =====================================================================
// =====================================================================


bool MorelSeytouxFunc::checkConsistency()
{

  //std::cout << "Momo checkConsistency()" << std::endl;

  return true;
}


// =====================================================================
// =====================================================================


bool MorelSeytouxFunc::runStep(mhydasdk::base::SimulationStatus* SimStatus)
{

  // std::cout << "coucou c'est Momo run " << mp_CoreData->getSpatialData()->getSUsCollection()->size();
  return true;
}


// =====================================================================
// =====================================================================


bool MorelSeytouxFunc::finalize()
{
  std::cout << "Momo finalize()" << std::endl;
  return true;
}

