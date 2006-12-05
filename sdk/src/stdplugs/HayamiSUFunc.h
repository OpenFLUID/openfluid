/**
  \file HayamiSUFunc.h
  \brief header of HayamiSUFunction

  \author Jean-Christophe FABRE <fabrejc@ensam.inra.fr>
*/


#ifndef __HAYAMISUFUNC_H__
#define __HAYAMISUFUNC_H__


#include "sdk-core.h"
#include "sdk-base.h"

#include "HayamiTools.h"

// =====================================================================
// =====================================================================


WX_DECLARE_HASH_MAP(int, t_HayamiKernel, wxIntegerHash, wxIntegerEqual, IDKernelMap);

WX_DECLARE_HASH_MAP(int, mhydasdk::core::VectorOfDouble*, wxIntegerHash, wxIntegerEqual, IDInputMap);


/**

*/
class HayamiSUFunction : public mhydasdk::base::Function
{
  private:

    int m_MaxSteps;
    
    float m_MeanCelerity;

    float m_MeanSigma;    
    
    float m_MeanSlope;
    
    float m_MeanManning;        

    IDKernelMap m_SUKernel;
    
    IDInputMap m_Input;
    
    mhydasdk::base::IDFloatValueMap m_CurrentInputSum;
        

  public:

    /**
      Constructor
    */
    HayamiSUFunction(mhydasdk::core::CoreRepository *CoreData);

    /**
      Destructor
    */
    ~HayamiSUFunction();

    bool initParams(mhydasdk::core::ParamsMap Params);

    bool initializeRun(mhydasdk::base::SimulationInfo* SimInfo);

    bool checkConsistency();

    bool runStep(mhydasdk::base::SimulationStatus* SimStatus);

    bool finalizeRun(mhydasdk::base::SimulationInfo* SimInfo);

};


#endif  // __HAYAMISUFUNC_H__


