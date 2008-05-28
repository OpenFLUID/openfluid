/**
  \file RainSUFromFilesFunc.h
  \brief Header of ...
*/

#ifndef __RAINSUFROMFILESFUNC_H__
#define __RAINSUFROMFILESFUNC_H__

#include "mhydasdk-base.h"
#include "mhydasdk-core.h"


// =====================================================================
// =====================================================================


DECLARE_PLUGIN_HOOKS;


// =====================================================================
// =====================================================================


/**

*/
class RainSUFromFilesFunction : public mhydasdk::base::PluggableFunction
{
  private:

  public:
    /**
      Constructor
    */
    RainSUFromFilesFunction();

    /**
      Destructor
    */
    ~RainSUFromFilesFunction();

    bool initParams(mhydasdk::core::ParamsMap Params);

    bool prepareData();

    bool checkConsistency();

    bool initializeRun(const mhydasdk::base::SimulationInfo* SimInfo);

    bool runStep(const mhydasdk::base::SimulationStatus* SimStatus);

    bool finalizeRun(const mhydasdk::base::SimulationInfo* SimInfo);

};


#endif  // __RAINSUFROMFILESFUNC_H__
