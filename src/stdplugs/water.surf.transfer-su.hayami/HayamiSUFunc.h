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
    
    double m_MeanCelerity;

    double m_MeanSigma;    
    
    double m_MeanSlope;
    
    double m_MeanManning;        

    IDKernelMap m_SUKernel;
    
    mhydasdk::base::IDVectOfMHYDASValueMap m_Input;
    
    mhydasdk::base::IDMHYDASValueMap m_CurrentInputSum;
        

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

    bool initializeRun(const mhydasdk::base::SimulationInfo* SimInfo);

    bool runStep(const mhydasdk::base::SimulationStatus* SimStatus);

    bool finalizeRun(const mhydasdk::base::SimulationInfo* SimInfo);

};

// =====================================================================
// =====================================================================


DECLARE_PLUGIN_HOOKS;


#endif  // __HAYAMISUFUNC_H__


