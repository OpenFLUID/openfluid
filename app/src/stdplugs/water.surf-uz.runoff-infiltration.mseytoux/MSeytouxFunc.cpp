/**
  \file
  \brief implements ...

  \author Jean-Christophe FABRE <fabrejc@ensam.inra.fr>
*/

#include "MSeytouxFunc.h"
#include "math.h"
#include <iostream>
#include <stdio.h>



// =====================================================================
// =====================================================================


DEFINE_FUNCTION_HOOK(MorelSeytouxFunc);

// =====================================================================
// =====================================================================


BEGIN_SIGNATURE_HOOK
  
  DECLARE_SIGNATURE_ID(wxT("water.surf-uz.runoff-infiltration.mseytoux"));  
  DECLARE_SIGNATURE_NAME(wxT("Morel-Seytoux production on surface units"));
  DECLARE_SIGNATURE_DESCRIPTION(wxT("Production function computing infiltration and runoff at the surface of a unit using the Morel-Seytoux method, based on the Green and Ampt method."));  
  DECLARE_SIGNATURE_DOMAIN(wxT("hydrology"));
  DECLARE_SIGNATURE_STATUS(mhydasdk::base::BETA);
  
  DECLARE_SIGNATURE_SDKVERSION;
  
  DECLARE_SIGNATURE_AUTHORNAME(wxT("Moussa R., Fabre J.-C."));
  DECLARE_SIGNATURE_AUTHOREMAIL(wxT("moussa@supagro.inra.fr, fabrejc@supagro.inra.fr"));  

  DECLARE_SU_REQUIRED_VAR("water.atm-surf.H.rain",wxT("rainfall height on SU"),wxT("m"));  
  
  DECLARE_SU_PRODUCED_VAR("water.surf.H.runoff",wxT("runoff on the surface of the unit"),wxT("m"));
  DECLARE_SU_PRODUCED_VAR("water.surf.H.infiltration",wxT("infiltration through the surface of the unit"),wxT("m"));

  DECLARE_SU_USED_PREVVAR("water.surf.Q.downstream-su",wxT("output volume at the outlet of the upstream SUs"),wxT("m3/s"));  
  
  DECLARE_SU_REQUIRED_PROPERTY("ks",wxT("hydraulic conductivity when saturated"),wxT("m/s"));
  DECLARE_SU_REQUIRED_PROPERTY("thetares",wxT(""),wxT("m3/m3"));   
  DECLARE_SU_REQUIRED_PROPERTY("thetasat",wxT(""),wxT("m3/m3"));  
  DECLARE_SU_REQUIRED_PROPERTY("betaMS",wxT(""),wxT(""));    
  DECLARE_SU_REQUIRED_PROPERTY("hc",wxT(""),wxT("m"));

  DECLARE_SU_REQUIRED_INICOND("thetaisurf",wxT(""),wxT("m3/m3"));
  
  DECLARE_FUNCTION_PARAM("resstep",wxT("numerical resolution step for ponding time"),wxT(""));
 
END_SIGNATURE_HOOK

// =====================================================================
// =====================================================================




MorelSeytouxFunc::MorelSeytouxFunc()
                : PluggableFunction()
{
  
    
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

  MHYDAS_GetFunctionParam(Params,wxT("resstep"),&m_ResStep);
    
  return true;
}


// =====================================================================
// =====================================================================


bool MorelSeytouxFunc::prepareData()
{
  
  return true;
}

// =====================================================================
// =====================================================================


bool MorelSeytouxFunc::checkConsistency()
{
  return true;
}



// =====================================================================
// =====================================================================


bool MorelSeytouxFunc::initializeRun(const mhydasdk::base::SimulationInfo* SimInfo)
{

  bool IsOK =  true;


  mhydasdk::core::PropertyValue ThetaR, ThetaS, ThetaI, Hc, ThetaStar;
  mhydasdk::core::SurfaceUnit* SU;
  

  DECLARE_SU_ORDERED_LOOP
  BEGIN_SU_ORDERED_LOOP(SU)


    // getting distributed properties
    MHYDAS_GetDistributedProperty(SU,wxT("thetares"),&ThetaR);    
    MHYDAS_GetDistributedProperty(SU,wxT("thetasat"),&ThetaS);  
    MHYDAS_GetDistributedIniCondition(SU,wxT("thetaisurf"),&ThetaI);    
    MHYDAS_GetDistributedProperty(SU,wxT("hc"),&Hc);
    
                
    // Computing ThetaStar
    ThetaStar = (ThetaI - ThetaR) / (ThetaS - ThetaR);
    m_SUThetaStar[SU->getID()] = ThetaStar; 

    // Computing Sf
    m_SUSf[SU->getID()] = Hc * (1 - (1 * pow(ThetaStar,6))) * (ThetaS - ThetaI);

    // initializing saturation state
    m_SUSatState[SU->getID()] = 0;

    // sets whether the upstream output should be used or not.
    // a revoir
    m_UseUpstreamOutput[SU->getID()] = MHYDAS_IsDistributedVarExists(SU,wxT("water.surf.Q.downstream-su"));
    
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


bool MorelSeytouxFunc::runStep(const mhydasdk::base::SimulationStatus* SimStatus)
{

 
  float OutputsSum;
  float RainIntensity; //r
  float EfficientRainIntensity; // re
  float CurrentPondingTime; // tp
  float CurrentPondingSum;  // wp
  float EfficientPondingRainIntensity; // rpe
  float CurrentRunoff;
  float CurrentInfiltration; 

  int ID;
  mhydasdk::core::MHYDASScalarValue CurrentRain;
  int CurrentStep;
  int TimeStep;
  mhydasdk::core::PropertyValue Ks;

  mhydasdk::core::PropertyValue Beta;
  double DeltaWi;
  bool Criteria;
  float ExtraTime;
  double InfiltrationCapacity;
  float Area;

  mhydasdk::core::MHYDASScalarValue TmpValue;

  mhydasdk::core::SurfaceUnit* SU;
  mhydasdk::core::SurfaceUnit* UpSU;

  std::list<mhydasdk::core::SurfaceUnit*>::iterator UpSUiter;
  std::list<mhydasdk::core::SurfaceUnit*>* UpSUsList;

  TimeStep = SimStatus->getTimeStep();
  CurrentStep = SimStatus->getCurrentStep();
 
  DECLARE_SU_ORDERED_LOOP
  BEGIN_SU_ORDERED_LOOP(SU)

    ID = SU->getID();

    // Getting distributed properties
    MHYDAS_GetDistributedProperty(SU,wxT("ks"),&Ks);
    MHYDAS_GetDistributedProperty(SU,wxT("betaMS"),&Beta);    
    Area = SU->getUsrArea();
    
    CurrentRunoff = 0;
    CurrentInfiltration = 0;

    OutputsSum = 0;      

    
    // adding upstream SU output (step n-1) to rain       
    if (m_UseUpstreamOutput[ID] && CurrentStep > 0)
    {
      UpSUsList = SU->getUpstreamSUs();
      
      for(UpSUiter=UpSUsList->begin(); UpSUiter != UpSUsList->end(); UpSUiter++) \
      {                
        UpSU = *UpSUiter;
        MHYDAS_GetDistributedVarValue(UpSU,wxT("water.surf.Q.downstream-su"),CurrentStep-1,&TmpValue);
        OutputsSum = OutputsSum + TmpValue * TimeStep / Area;
      }      
    } 
    m_CurrentUpstreamInput[ID] = OutputsSum;
    
    // convert rain from m/s to m/time step       
    MHYDAS_GetDistributedVarValue(SU,wxT("water.atm-surf.H.rain"),CurrentStep,&CurrentRain);    

    CurrentRain = CurrentRain + m_CurrentUpstreamInput[ID];

    // computing rain intensity
    RainIntensity = (CurrentRain / TimeStep);   

    
    
    if (Ks == 0)
    {
      // si ks == 0 alors tout ruisselle
      CurrentRunoff = CurrentRain;
      CurrentInfiltration = 0;
    }
    else
    {
      // ks est > 0

      // calcul de la pluie efficace
      EfficientRainIntensity =  RainIntensity / Ks;

      
      if (m_SUSatState[ID] == 0)
      {
        // calcul du temps de saturation
        m_RainSum[ID] = m_RainSum[ID] + CurrentRain;
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

      if (m_SUSatState[ID] > 0)             
      //else
      {
    	  
        EfficientPondingRainIntensity = m_PondingRainIntensity[ID] / Ks;     
        ExtraTime = (CurrentStep * TimeStep) - m_PondingTime[ID];   
        Criteria = true;
        DeltaWi = 0;
        
        // computing values for optimization
        CurrentPondingSum = m_PondingSum[ID];
        CurrentPondingTime = m_PondingTime[ID];

        float PSBEKs = CurrentPondingSum * (Beta * EfficientPondingRainIntensity -1) / Ks;
        float BetaOnKs = Beta / Ks;
        float EPRIPS = EfficientPondingRainIntensity * CurrentPondingSum;       
        
        m_DeltaT1[ID] = (m_DeltaW1[ID] * BetaOnKs) - (PSBEKs) * log((EPRIPS + m_DeltaW1[ID]) / (EPRIPS));
        m_DeltaW2[ID] = m_DeltaW1[ID] + m_ResStep;

        
        // determine ponding time 
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
        
        
        // computing infiltration capacity        
        InfiltrationCapacity = (DeltaWi - m_PreviousDeltaW[ID]) / TimeStep;
        m_PreviousDeltaW[ID] = DeltaWi;                
          
        if (RainIntensity > InfiltrationCapacity)
        { 
          CurrentRunoff = (RainIntensity - InfiltrationCapacity) * TimeStep;
        }  
        
      }
    
      CurrentInfiltration = CurrentRain - CurrentRunoff;
    }   


    MHYDAS_AppendDistributedVarValue(SU, wxT("water.surf.H.runoff"), CurrentRunoff);
    MHYDAS_AppendDistributedVarValue(SU, wxT("water.surf.H.infiltration"), CurrentInfiltration);

    
  END_LOOP



  return true;
}


// =====================================================================
// =====================================================================


bool MorelSeytouxFunc::finalizeRun(const mhydasdk::base::SimulationInfo* SimInfo)
{
  return true;
}

