/**
  \file HayamiSUFunc.h
  \brief header of HayamiSUFunction

  \author Jean-Christophe FABRE <fabrejc@ensam.inra.fr>
*/


#ifndef __HAYAMISUFUNC_H__
#define __HAYAMISUFUNC_H__


#include "mhydasdk-core.h"
#include "mhydasdk-base.h"

#include "HayamiTools.h"

// =====================================================================
// =====================================================================



/**

*/
class HayamiSUFunction : public mhydasdk::base::PluggableFunction
{
  private:

    int m_MaxSteps;
    
    float m_MeanCelerity;

    float m_MeanSigma;    
    
    float m_MeanSlope;
    
    float m_MeanManning;        

    IDKernelMap m_SUKernel;
    
    mhydasdk::base::IDVectOfDoubleMap m_Input;
    
    mhydasdk::base::IDFloatValueMap m_CurrentInputSum;
        

  public:

    /**
      Constructor
    */
    HayamiSUFunction();

    /**
      Destructor
    */
    ~HayamiSUFunction();

    bool initParams(mhydasdk::core::ParamsMap Params);

    bool prepareData();
    
    bool checkConsistency();

    bool initializeRun(mhydasdk::base::SimulationInfo* SimInfo);

    bool runStep(mhydasdk::base::SimulationStatus* SimStatus);

    bool finalizeRun(mhydasdk::base::SimulationInfo* SimInfo);

};

// =====================================================================
// =====================================================================

extern "C"
{
  DLLIMPORT mhydasdk::base::PluggableFunction* GetMHYDASPluggableFunction();
};



#endif  // __HAYAMISUFUNC_H__


