/**
  \file
  \brief implements ...

  \author Jean-Christophe FABRE <fabrejc@ensam.inra.fr>
*/

#include "MSeytouxFunc.h"
#include "math.h"
#include "setup.h"
#include <iostream>
#include <stdio.h>

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
  
  
  m_ResStep = 0.000005;
  
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

  if (Params.find(wxT("resstep")) != Params.end()) m_ResStep = Params[wxT("resstep")];      
  
  return true;
}


// =====================================================================
// =====================================================================


bool MorelSeytouxFunc::initializeRun(mhydasdk::base::SimulationInfo* SimInfo)
{

  bool IsOK =  true;


  float ThetaR, ThetaS, ThetaI, Hc, ThetaStar;
  mhydasdk::core::SurfaceUnit* SU;
  

  DECLARE_SU_ORDERED_LOOP
  BEGIN_SU_ORDERED_LOOP(SU)


    //ThetaR = SU->getProperties()->find(wxT("thetares"))->second;
    MHYDAS_GetHydroObjectProperty(SU,wxT("thetares"),&ThetaR);
    //ThetaS = SU->getProperties()->find(wxT("thetasat"))->second;    
    MHYDAS_GetHydroObjectProperty(SU,wxT("thetasat"),&ThetaS);
    //ThetaI = SU->getIniConditions()->find(wxT("thetaisurf"))->second;  
    MHYDAS_GetHydroObjectIniCondition(SU,wxT("thetaisurf"),&ThetaI);
    //Hc = SU->getProperties()->find(wxT("hc"))->second;    
    MHYDAS_GetHydroObjectProperty(SU,wxT("hc"),&Hc);
    
            
    
    // Computing ThetaStar
    ThetaStar = (ThetaI - ThetaR) / (ThetaS - ThetaR);
    m_SUThetaStar[SU->getID()] = ThetaStar; 

    // Computing Sf
    m_SUSf[SU->getID()] = Hc * (1 - (1 * pow(ThetaStar,6))) * (ThetaS - ThetaI);

    // initializing saturation state
    m_SUSatState[SU->getID()] = 0;

    // sets whether the upstream output should be used or not.
    // a  revoir
    m_UseUpstreamOutput[SU->getID()] = SIMVAR_EXISTS(SU,"qoutput");
    
    m_CurrentUpstreamInput[SU->getID()] = 0;
    
    m_RainSum[SU->getID()] = 0;
    
    m_PondingSum[SU->getID()] = 0;
    
    m_PreviousDeltaW[SU->getID()] = 0;
    
    m_DeltaW1[SU->getID()] = 0;
    m_DeltaW2[SU->getID()] = 0;
    m_DeltaT1[SU->getID()] = 0;
    m_DeltaT2[SU->getID()] = 0;
    
    

  END_LOOP



  return IsOK;
}


// =====================================================================
// =====================================================================


bool MorelSeytouxFunc::checkConsistency()
{

  return Function::checkConsistency();
}


// =====================================================================
// =====================================================================


bool MorelSeytouxFunc::runStep(mhydasdk::base::SimulationStatus* SimStatus)
{

  double Value;
  int i;
  
  float OutputsSum;
  float RainIntensity; //r
  float EfficientRainIntensity; // re
  float CurrentPondingTime; // tp
  float CurrentPondingSum;  // wp
  float EfficientPondingRainIntensity; // rpe
  float CurrentRunoff;
  float CurrentInfiltration; 

  int ID;
  float CurrentRain;
  int CurrentStep;
  int TimeStep;
  float Ks;

  float Beta;
  double DeltaWi;
  bool Criteria;
  float ExtraTime;
  double InfiltrationCapacity;
  float Area;

  float TmpValue;

  mhydasdk::core::SurfaceUnit* SU;
  mhydasdk::core::SurfaceUnit* UpSU;

  list<mhydasdk::core::SurfaceUnit*>::iterator UpSUiter;
  list<mhydasdk::core::SurfaceUnit*>* UpSUsList;

  TimeStep = SimStatus->getTimeStep();
  CurrentStep = SimStatus->getCurrentStep();
 
  DECLARE_SU_ORDERED_LOOP
  BEGIN_SU_ORDERED_LOOP(SU)

    ID = SU->getID();
    //Ks = SU->getProperties()->find(wxT("ks"))->second;
    MHYDAS_GetHydroObjectProperty(SU,wxT("ks"),&Ks);
    //Beta = SU->getProperties()->find(wxT("betaMS"))->second;
    MHYDAS_GetHydroObjectProperty(SU,wxT("betaMS"),&Beta);    
    Area = SU->getUsrArea();
    
    CurrentRunoff = 0;
    CurrentInfiltration = 0;

    
    // ajout des apports des unites amont (sorties des unites amont a  t-1)
    // adding upstream units output (step n-1) to rain    
    if (m_UseUpstreamOutput[ID] && CurrentStep > 0)
    {
      OutputsSum = 0;      
      UpSUsList = SU->getUpstreamSUs();
      
      for(UpSUiter=UpSUsList->begin(); UpSUiter != UpSUsList->end(); UpSUiter++) \
      {                
        UpSU = *UpSUiter;
        //OutputsSum = OutputsSum + GET_SIMVAR_VALUE(UpSU,"qoutput",CurrentStep-1) * TimeStep / Area;
        MHYDAS_GetSimVarValue(UpSU,wxT("qoutput"),CurrentStep-1,&TmpValue);
        OutputsSum = OutputsSum + TmpValue * TimeStep / Area;
      }
      m_CurrentUpstreamInput[ID] = OutputsSum;
    } 
    // transformation de la pluie de m/s en m/pas de temps    
    // CurrentRain = GET_SU_RAINVALUE(SU,CurrentStep) * TimeStep;    
    MHYDAS_GetSurfaceUnitRainValue(SU,CurrentStep,&CurrentRain);
    CurrentRain = CurrentRain * TimeStep;


    // calcul de l'intensite de pluie
    RainIntensity = (CurrentRain / TimeStep);   

    // calcul de la pluie efficace
    EfficientRainIntensity =  RainIntensity / Ks;

    if (Ks == 0)
    {
      // si ks == 0 alors tout ruisselle
      CurrentRunoff = CurrentRain + m_CurrentUpstreamInput[ID];
      CurrentInfiltration = 0;
    }
    else
    {
      // ks est > 0
      
      if (m_SUSatState[ID] == 0)
      {
        // calcul du temps de saturation
        m_RainSum[ID] = m_RainSum[ID] + CurrentRain + m_CurrentUpstreamInput[ID];
        if (EfficientRainIntensity > 1)
        {
          m_PondingTime[ID] = (CurrentStep-1) * TimeStep + (1/(RainIntensity)) * ((m_SUSf[ID] / (EfficientRainIntensity -1)-m_RainSum[ID]));        
  
          if (m_PondingTime[ID] <= (CurrentStep * TimeStep))
          {
            
            if (m_PondingTime[ID] > ((CurrentStep-1) * TimeStep))
            {
              m_PondingRainIntensity[ID] = RainIntensity;
              m_PondingStep[ID] = CurrentStep;
              m_PondingSum[ID] = m_RainSum[ID] + (m_PondingTime[ID]-((CurrentStep-1)*TimeStep))*RainIntensity;
              m_SUSatState[ID] = 2; 
            }
            else
            {
              m_PondingTime[ID] = (CurrentStep-1) * TimeStep;
              m_PondingStep[ID] = CurrentStep - 1;
              m_PondingRainIntensity[ID] = (1+(m_SUSf[ID]/m_RainSum[ID])) * Ks; 
              m_PondingSum[ID] = m_RainSum[ID];
              m_SUSatState[ID] = 3;             
            }          
          }
        }
      }

      //if (m_SUSatState[ID] > 0)             
      else
      {
        EfficientPondingRainIntensity = m_PondingRainIntensity[ID] / Ks;     
        ExtraTime = (CurrentStep * TimeStep) - m_PondingTime[ID];   
        Criteria = true;
        DeltaWi = 0;
        
        // calcul de termes pour optimisation des calculs
        CurrentPondingSum = m_PondingSum[ID];
        CurrentPondingTime = m_PondingTime[ID];
        float PSBEKs = CurrentPondingSum * (Beta * EfficientPondingRainIntensity -1) / Ks;
        float BetaOnKs = Beta / Ks;
        float EPRIPS = EfficientPondingRainIntensity * CurrentPondingSum;
       
        
        m_DeltaT1[ID] = (m_DeltaW1[ID] * BetaOnKs) - (PSBEKs) * log((EPRIPS + m_DeltaW1[ID]) / (EPRIPS));
        m_DeltaW2[ID] = m_DeltaW1[ID] + m_ResStep;
        
        while (Criteria)
        {        
          m_DeltaT2[ID] = (m_DeltaW2[ID] * BetaOnKs) - (PSBEKs) * log((EPRIPS + m_DeltaW2[ID]) / (EPRIPS));                                      
          if (ExtraTime <= m_DeltaT2[ID] && ExtraTime > m_DeltaT1[ID])
          {          
            DeltaWi = m_DeltaW2[ID] - 0.5 * m_ResStep; 
            m_DeltaW1[ID] = m_DeltaW2[ID] - 2 * m_ResStep;
            Criteria = false; 
          }
          m_DeltaW2[ID] = m_DeltaW2[ID] + m_ResStep;
          m_DeltaT1[ID] = m_DeltaT2[ID];        
        }      
        
        
        // calcul de la capacite d'infiltration        
        InfiltrationCapacity = (DeltaWi - m_PreviousDeltaW[ID]) / TimeStep;
        m_PreviousDeltaW[ID] = DeltaWi;                
          
        if (RainIntensity > InfiltrationCapacity)
        { 
          CurrentRunoff = (RainIntensity - InfiltrationCapacity) * TimeStep;
        }  
        
      }
    
      CurrentInfiltration = (m_CurrentUpstreamInput[ID] + CurrentRain) - CurrentRunoff;
    }   

    APPEND_SIMVAR_VALUE(SU,"runoff",CurrentRunoff);
    APPEND_SIMVAR_VALUE(SU,"infiltration",CurrentInfiltration);
  END_LOOP



  return true;
}


// =====================================================================
// =====================================================================


bool MorelSeytouxFunc::finalizeRun(mhydasdk::base::SimulationInfo* SimInfo)
{
  // std::cout << "Momo finalizeRun()" << std::endl;
  return true;
}

