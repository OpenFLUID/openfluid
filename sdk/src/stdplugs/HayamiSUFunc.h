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

    bool initializeRun(mhydasdk::base::SimulationStatus* SimStatus);

    bool checkConsistency();

    bool runStep(mhydasdk::base::SimulationStatus* SimStatus);

    bool finalizeRun(mhydasdk::base::SimulationStatus* SimStatus);

};


#endif  // __HAYAMISUFUNC_H__


