/**
  \file
  \brief implements ...

  \author Jean-Christophe FABRE <fabrejc@ensam.inra.fr>
*/

#include "MSeytouxFunc.h"
#include "math.h"
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
  
  
  m_ResError = 0.00005;
  
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


  float ThetaR, ThetaS, ThetaI, Hc, ThetaStar;
  mhydasdk::core::SurfaceUnit* SU;

  BEGIN_SU_ORDERED_LOOP(SU)

    ThetaR = SU->getProperties()->find(wxT("thetares"))->second;
    ThetaS = SU->getProperties()->find(wxT("thetasat"))->second;
    ThetaI = SU->getIniConditions()->find(wxT("thetaisurf"))->second;
    Hc = SU->getProperties()->find(wxT("hc"))->second;
    
    // Computing ThetaStar
    ThetaStar = (ThetaI - ThetaR) / (ThetaS - ThetaR);
    m_SUThetaStar[SU->getID()] = ThetaStar; 

    // Computing Sf
    m_SUSf[SU->getID()] = Hc * (1 - (1 * pow(ThetaStar,6))) * (ThetaS - ThetaI);

    //std::cerr << "SU: " << SU->getID() << " -> " << m_SUThetaStar[SU->getID()] << "  -  " << SU->getIniConditions()->find(wxT("thetaisurf"))->second << std::endl;


  END_LOOP



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
    // adding upstream units output (step n-1) to rain

    if (SimStatus->getCurrentStep() > 0)
    {
      
    }
  
    //
  
    Value = GET_SU_RAINVALUE(SU,SimStatus->getCurrentStep());
    APPEND_SIMVAR_VALUE(SU,"runoff",Value * 0.6);
    APPEND_SIMVAR_VALUE(SU,"infiltration",Value * 0.4);
  END_LOOP



  return true;
}


// =====================================================================
// =====================================================================


bool MorelSeytouxFunc::finalizeRun()
{
  // std::cout << "Momo finalizeRun()" << std::endl;
  return true;
}

