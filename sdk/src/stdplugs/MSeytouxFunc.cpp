/**
  \file
  \brief implements ...

  \author Jean-Christophe FABRE <fabrejc@ensam.inra.fr>
*/

#include "MSeytouxFunc.h"


MorelSeytouxFunc::MorelSeytouxFunc(mhydasdk::core::CoreRepository *CoreData)
                : Function(CoreData)
{

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

  std::cout << "Momo initParams()" << std::endl;
  return true;
}


// =====================================================================
// =====================================================================


bool MorelSeytouxFunc::initialize()
{
  std::cout << "Momo initialize()" << std::endl;
  return true;
}


// =====================================================================
// =====================================================================


bool MorelSeytouxFunc::checkConsistency()
{

  std::cout << "Momo checkConsistency()" << std::endl;

  return true;
}


// =====================================================================
// =====================================================================


bool MorelSeytouxFunc::runStep(mhydasdk::base::SimulationStatus* SimStatus)
{

  std::cout << "coucou c'est Momo run";
  return true;
}


// =====================================================================
// =====================================================================


bool MorelSeytouxFunc::finalize()
{
  std::cout << "Momo finalize()" << std::endl;
  return true;
}

