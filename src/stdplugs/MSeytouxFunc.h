/**
  \file
  \brief header of ...

  \author Jean-Christophe FABRE <fabrejc@ensam.inra.fr>
*/

#ifndef __MSEYTOUXFUNC_H__
#define __MSEYTOUXFUNC_H__


#include "mhydasdk-base.h"
#include "mhydasdk-core.h"

/**

*/
class MorelSeytouxFunc : public mhydasdk::base::PluggableFunction
{
  private:

    /** 
      Resolution step
    */  
    double m_ResStep;
    
    /**
      Theta star on each SU
    */
    mhydasdk::base::IDFloatValueMap m_SUThetaStar;
    
    /**
      Sf on each SU
    */
    mhydasdk::base::IDFloatValueMap m_SUSf;

    /**
      Saturation state on each SU
    */
    mhydasdk::base::IDIntValueMap m_SUSatState;

    /**
      Use of upstream output as input on each SU
    */
    mhydasdk::base::IDBoolValueMap m_UseUpstreamOutput;

    /**
      Current upstream input value for each SU
    */
    mhydasdk::base::IDFloatValueMap m_CurrentUpstreamInput;
    
    /**
      Rain sum for each SU
    */
    mhydasdk::base::IDFloatValueMap m_RainSum;    

    /**
      Previous DeltaW for each SU
    */
    mhydasdk::base::IDFloatValueMap m_PreviousDeltaW;    

    mhydasdk::base::IDFloatValueMap m_PondingTime; // tp

    mhydasdk::base::IDFloatValueMap m_PondingSum;  // wp

    mhydasdk::base::IDIntValueMap m_PondingStep; // ip

    mhydasdk::base::IDFloatValueMap m_PondingRainIntensity; // rp


    mhydasdk::base::IDFloatValueMap m_DeltaW1;
    mhydasdk::base::IDFloatValueMap m_DeltaW2;
    mhydasdk::base::IDFloatValueMap m_DeltaT1;
    mhydasdk::base::IDFloatValueMap m_DeltaT2;




  public:
    /**
      Constructor
    */
    MorelSeytouxFunc();

    /**
      Destructor
    */
    ~MorelSeytouxFunc();


    bool initParams(mhydasdk::core::ParamsMap Params);

    bool checkConsistency();
    
    bool prepareData();

    bool initializeRun(mhydasdk::base::SimulationInfo* SimInfo);    

    bool runStep(mhydasdk::base::SimulationStatus* SimStatus);

    bool finalizeRun(mhydasdk::base::SimulationInfo* SimInfo);


};

extern "C"
{
  DLLIMPORT mhydasdk::base::PluggableFunction* GetMHYDASPluggableFunction();
};




#endif
