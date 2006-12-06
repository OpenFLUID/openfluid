/**
  \file HayamiRSFunc.h
  \brief header of HayamiRSFunction

  \author Jean-Christophe FABRE <fabrejc@ensam.inra.fr>
*/


#ifndef __HAYAMIRSFUNC_H__
#define __HAYAMIRSFUNC_H__


#include "sdk-core.h"
#include "sdk-base.h"
#include "HayamiTools.h"

// =====================================================================
// =====================================================================

/**

*/
class HayamiRSFunction : public mhydasdk::base::Function
{
  private:

    int m_MaxSteps;
    
    float m_MeanCelerity;

    float m_MeanSigma;    
    
    float m_MeanSlope;
    
    float m_MeanManning;        

    IDKernelMap m_RSKernel;
    
    bool m_UseUpSUOutput;
    
    mhydasdk::base::IDVectOfDoubleMap m_Input;
    
    mhydasdk::base::IDFloatValueMap m_CurrentInputSum;
        

  public:

    /**
      Constructor
    */
    HayamiRSFunction(mhydasdk::core::CoreRepository *CoreData);

    /**
      Destructor
    */
    ~HayamiRSFunction();

    bool initParams(mhydasdk::core::ParamsMap Params);

    bool initializeRun(mhydasdk::base::SimulationInfo* SimInfo);

    bool checkConsistency();

    bool runStep(mhydasdk::base::SimulationStatus* SimStatus);

    bool finalizeRun(mhydasdk::base::SimulationInfo* SimInfo);

};


#endif  // __HAYAMIRSFUNC_H__


