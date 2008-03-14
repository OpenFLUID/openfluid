/**
  \file HayamiRSFunc.h
  \brief header of HayamiRSFunction

  \author Jean-Christophe FABRE <fabrejc@ensam.inra.fr>
*/


#ifndef __HAYAMIRSFUNC_H__
#define __HAYAMIRSFUNC_H__


#include "mhydasdk-core.h"
#include "mhydasdk-base.h"
#include "HayamiTools.h"

// =====================================================================
// =====================================================================

/**

*/
class HayamiRSFunction : public mhydasdk::base::PluggableFunction
{
  private:

    int m_MaxSteps;
    
    float m_MeanCelerity;

    float m_MeanSigma;    
    
    float m_MeanSlope;
    
    float m_MeanManning;        
    
    float m_RSBuffer;  

    /**
      Calibration step for height-discharge relation (meters) 
    */ 
    float m_CalibrationStep;

    IDKernelMap m_RSKernel;
    
    bool m_UseUpSUOutput;
    
    mhydasdk::base::IDVectOfMHYDASValueMap m_Input;
    
    mhydasdk::base::IDFloatMap m_CurrentInputSum;

    mhydasdk::base::IDVectOfMHYDASValueMap m_HeightDischarge;
        
    bool computeWaterHeightFromDischarge(int ID, float Discharge, float *Height);

    /**
         for groundwater flow use
        */ 
     float m_CoeffInfiltration;
     float m_CoeffDrainage;
     float m_CoeffGW;

    
     mhydasdk::base::IDFloatMap m_ThetaIni;
     mhydasdk::base::IDFloatMap m_ThetaSat;
     
     
  public:

    /**
      Constructor
    */
    HayamiRSFunction();

    /**
      Destructor
    */
    ~HayamiRSFunction();

    bool initParams(mhydasdk::core::ParamsMap Params);

    bool prepareData();
    
    bool checkConsistency();

    bool initializeRun(mhydasdk::base::SimulationInfo* SimInfo);

    bool runStep(mhydasdk::base::SimulationStatus* SimStatus);

    bool finalizeRun(mhydasdk::base::SimulationInfo* SimInfo);

};

// =====================================================================
// =====================================================================


DECLARE_PLUGIN_HOOKS;




#endif  // __HAYAMIRSFUNC_H__


