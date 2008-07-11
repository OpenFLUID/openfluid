/**
  \file
  \brief header of ...

  \author Jean-Christophe FABRE <fabrejc@ensam.inra.fr>
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
    openfluid::base::IDDoubleMap m_SUThetaStar;
    
    /**
      Sf on each SU
    */
    openfluid::base::IDDoubleMap m_SUSf;

    /**
      Saturation state on each SU
    */
    openfluid::base::IDIntMap m_SUSatState;

    /**
      Use of upstream output as input on each SU
    */
    openfluid::base::IDBoolMap m_UseUpstreamOutput;

    /**
      Current upstream input value for each SU
    */
    openfluid::base::IDDoubleMap m_CurrentUpstreamInput;
    
    /**
      Rain sum for each SU
    */
    openfluid::base::IDFloatMap m_RainSum;    

    /**
      Previous DeltaW for each SU
    */
    openfluid::base::IDFloatMap m_PreviousDeltaW;    

    openfluid::base::IDFloatMap m_PondingTime; // tp

    openfluid::base::IDFloatMap m_PondingSum;  // wp

    openfluid::base::IDIntMap m_PondingStep; // ip

    openfluid::base::IDFloatMap m_PondingRainIntensity; // rp


    openfluid::base::IDFloatMap m_DeltaW1;
    openfluid::base::IDFloatMap m_DeltaW2;
    openfluid::base::IDFloatMap m_DeltaT1;
    openfluid::base::IDFloatMap m_DeltaT2;




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
