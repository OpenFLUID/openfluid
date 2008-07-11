/**
  \file RainRSFromFilesFunc.h
  \brief Header of ...
*/

#ifndef __RAINRSFROMFILESFUNC_H__
#define __RAINRSFROMFILESFUNC_H__

#include "openfluid-base.h"
#include "openfluid-core.h"
#include "openfluid-tools.h"


// =====================================================================
// =====================================================================


DECLARE_PLUGIN_HOOKS;


// =====================================================================
// =====================================================================


/**

*/
class RainRSFromFilesFunction : public openfluid::base::PluggableFunction
{
  private:

    openfluid::tools::DistributeInterpolate m_DataPool;
    
    double m_Threshold;

  public:
    /**
      Constructor
    */
    RainRSFromFilesFunction();

    /**
      Destructor
    */
    ~RainRSFromFilesFunction();

    bool initParams(openfluid::core::ParamsMap Params);

    bool prepareData();

    bool checkConsistency();

    bool initializeRun(const openfluid::base::SimulationInfo* SimInfo);

    bool runStep(const openfluid::base::SimulationStatus* SimStatus);

    bool finalizeRun(const openfluid::base::SimulationInfo* SimInfo);

};


#endif  // __RAINRSFROMFILESFUNC_H__
