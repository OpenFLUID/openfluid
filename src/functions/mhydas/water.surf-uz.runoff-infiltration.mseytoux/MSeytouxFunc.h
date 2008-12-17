/**
  \file
  \brief header of ...

  \author Jean-Christophe FABRE <fabrejc@supagro.inra.fr>
*/

#ifndef __MSEYTOUXFUNC_H__
#define __MSEYTOUXFUNC_H__


#include "openfluid-base.h"
#include "openfluid-core.h"

/**

*/
class MorelSeytouxFunc : public openfluid::base::PluggableFunction
{
  private:

    /** 
      Resolution step
    */  
    double m_ResStep;
    
    /**
      Theta star on each SU
    */
    openfluid::core::IDDoubleMap m_SUThetaStar;
    
    /**
      Sf on each SU
    */
    openfluid::core::IDDoubleMap m_SUSf;

    /**
      Saturation state on each SU
    */
    openfluid::core::IDIntMap m_SUSatState;

    /**
      Use of upstream output as input on each SU
    */
    openfluid::core::IDBoolMap m_UseUpstreamOutput;

    /**
      Current upstream input value for each SU
    */
    openfluid::core::IDDoubleMap m_CurrentUpstreamInput;
    
    /**
      Rain sum for each SU
    */
    openfluid::core::IDFloatMap m_RainSum;    

    /**
      Previous DeltaW for each SU
    */
    openfluid::core::IDFloatMap m_PreviousDeltaW;    

    openfluid::core::IDFloatMap m_PondingTime; // tp

    openfluid::core::IDFloatMap m_PondingSum;  // wp

    openfluid::core::IDIntMap m_PondingStep; // ip

    openfluid::core::IDFloatMap m_PondingRainIntensity; // rp


    openfluid::core::IDFloatMap m_DeltaW1;
    openfluid::core::IDFloatMap m_DeltaW2;
    openfluid::core::IDFloatMap m_DeltaT1;
    openfluid::core::IDFloatMap m_DeltaT2;




  public:
    /**
      Constructor
    */
    MorelSeytouxFunc();

    /**
      Destructor
    */
    ~MorelSeytouxFunc();


    bool initParams(openfluid::core::ParamsMap Params);

    bool checkConsistency();
    
    bool prepareData();

    bool initializeRun(const openfluid::base::SimulationInfo* SimInfo);    

    bool runStep(const openfluid::base::SimulationStatus* SimStatus);

    bool finalizeRun(const openfluid::base::SimulationInfo* SimInfo);


};

DECLARE_PLUGIN_HOOKS;



#endif
