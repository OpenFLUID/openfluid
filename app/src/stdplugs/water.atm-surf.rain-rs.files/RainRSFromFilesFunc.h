/**
  \file RainRSFromFilesFunc.h
  \brief Header of ...
*/

#ifndef __RAINRSFROMFILESFUNC_H__
#define __RAINRSFROMFILESFUNC_H__

#include "mhydasdk-base.h"
#include "mhydasdk-core.h"
#include "mhydasdk-tools.h"


// =====================================================================
// =====================================================================


DECLARE_PLUGIN_HOOKS;


// =====================================================================
// =====================================================================


/**

*/
class RainRSFromFilesFunction : public mhydasdk::base::PluggableFunction
{
  private:

    mhydasdk::tools::DistributeInterpolate m_DataPool;
    
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

    bool initParams(mhydasdk::core::ParamsMap Params);

    bool prepareData();

    bool checkConsistency();

    bool initializeRun(const mhydasdk::base::SimulationInfo* SimInfo);

    bool runStep(const mhydasdk::base::SimulationStatus* SimStatus);

    bool finalizeRun(const mhydasdk::base::SimulationInfo* SimInfo);

};


#endif  // __RAINRSFROMFILESFUNC_H__
