/**
  \file TransferGUFunc.cpp
  \brief implementation of TransferGUFunction

  \author Jean-Christophe FABRE <fabrejc@supagro.inra.fr>, Xavier LOUCHART <louchart@supagro.inra.fr>
*/


#include "TransferGUFunc.h"
#include <math.h>

#include <iostream>

// =====================================================================
// =====================================================================

mhydasdk::base::PluggableFunction* GetMHYDASPluggableFunction()
{
  return new TransferGUFunction();
}


// =====================================================================
// =====================================================================



TransferGUFunction::TransferGUFunction()
                : PluggableFunction()
{

  mp_Signature->Author = wxT("Jean-Christophe FABRE, Xavier LOUCHART");
  mp_Signature->AuthorEmail = wxT("fabrejc@supagro.inra.fr, louchart@supagro.inra.fr");
  mp_Signature->ID = wxT("transfergu");
  mp_Signature->FunctionType = mhydasdk::base::SIMULATION;
  mp_Signature->Name = wxT("Hydrological transfer between SU-GU, RS-GU and GU-GU");
  mp_Signature->Description = wxT("");



  DECLARE_GU_PRODUCED_VAR("waterlevel",wxT("water level"),wxT("m"));

  DECLARE_SU_REQUIRED_VAR("infiltration",wxT("-"),wxT("-"));
  
  //DECLARE_RS_REQUIRED_VAR("waterlevel",wxT("-"),wxT("-"));

  DECLARE_FUNCTION_PARAM("tankcoeff",wxT("-"),wxT("-"));
  DECLARE_FUNCTION_PARAM("infiltrationcoeff",wxT("-"),wxT("-"));
  DECLARE_FUNCTION_PARAM("drainagecoeff",wxT("-"),wxT("-"));
  
 
  
  



/*  DECLARE_RS_PRODUCED_VAR("qoutput",wxT("Output volume at the outlet of the ditch"),wxT("m3/s"));
  DECLARE_RS_REQUIRED_PROPERTY("nmanning",wxT("-"),wxT("-"));

  DECLARE_FUNCTION_PARAM("maxsteps",wxT("maximum hayami kernel steps"),wxT("-"));
  DECLARE_FUNCTION_PARAM("meancel",wxT("-"),wxT("-"));  
  DECLARE_FUNCTION_PARAM("meansigma",wxT("-"),wxT("-"));  
*/
  // default values
  m_DrainageCoeff = 1;
  m_InfiltrationCoeff = 1;    
  m_TankCoeff = 1;
  
     

}


// =====================================================================
// =====================================================================


TransferGUFunction::~TransferGUFunction()
{

}


// =====================================================================
// =====================================================================


bool TransferGUFunction::initParams(mhydasdk::core::ParamsMap Params)
{

  MHYDAS_GetFunctionParam(Params,wxT("tankcoeff"),&m_TankCoeff);        
  MHYDAS_GetFunctionParam(Params,wxT("infiltrationcoeff"),&m_InfiltrationCoeff);
  MHYDAS_GetFunctionParam(Params,wxT("drainagecoeff"),&m_DrainageCoeff);
  return true;
}


// =====================================================================
// =====================================================================


bool TransferGUFunction::initializeRun(mhydasdk::base::SimulationInfo* SimInfo)
{
  

  return true;
}


// =====================================================================
// =====================================================================


bool TransferGUFunction::checkConsistency()
{
  
  return PluggableFunction::checkConsistency();
}


// =====================================================================
// =====================================================================


bool TransferGUFunction::runStep(mhydasdk::base::SimulationStatus* SimStatus)
{

  float ThetaS, ThetaI, Volume, WL;
  mhydasdk::core::GroundwaterUnit* GU;


  DECLARE_GU_ORDERED_LOOP;
  BEGIN_GU_ORDERED_LOOP(GU)
   ThetaS = 0.3;
   ThetaI = 0.1;
   Volume = 100;
   
   // MHYDAS_GetDistributedProperty(GU,wxT("thetasat"),&ThetaS));
   // MHYDAS_GetDistributedIniCondition(GU,wxT("thetai"),&ThetaI);
   
   WL = (Volume/(GU->getUsrArea()))*(ThetaS - ThetaI);
   MHYDAS_AppendDistributedVarValue(GU,wxT("waterlevel"),GU->getUsrArea());

  END_LOOP



  return true;
}


// =====================================================================
// =====================================================================


bool TransferGUFunction::finalizeRun(mhydasdk::base::SimulationInfo* SimInfo)
{

  return true;
}



