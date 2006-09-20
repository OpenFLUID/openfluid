/**
  \file
  \brief implements ...

  \author Jean-Christophe FABRE <fabrejc@ensam.inra.fr>
*/


#include "HayamiSUFunc.h"




HayamiSUFunc::HayamiSUFunc(mhydasdk::core::CoreRepository *CoreData)
                : Function(CoreData)
{

  m_SUVarsToCheck.Add(wxT("runoff"));
  m_SUVarsToAdd.Add(wxT("qoutput"));

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


bool HayamiSUFunc::initialize()
{

  bool IsOK =  true;

  IsOK = mhydasdk::base::Function::initialize();

  return IsOK;
}


// =====================================================================
// =====================================================================


bool HayamiSUFunc::checkConsistency()
{

  //std::cout << "Yaya checkConsistency()" << std::endl;

  return true;
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


bool HayamiSUFunc::finalize()
{
  // std::cout << "HayamiSU finalize()" << std::endl;
  return true;
}

