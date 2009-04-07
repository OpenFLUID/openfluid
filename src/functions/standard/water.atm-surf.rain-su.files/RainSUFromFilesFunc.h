/**
  \file RainSUFromFilesFunc.h
  \brief Header of ...
*/

#ifndef __RAINSUFROMFILESFUNC_H__
#define __RAINSUFROMFILESFUNC_H__

#include "openfluid-base.h"
#include "openfluid-core.h"
#include "openfluid-tools.h"


// =====================================================================
// =====================================================================


DECLARE_PLUGIN_HOOKS


// =====================================================================
// =====================================================================



/**

*/
class RainSUFromFilesFunction : public openfluid::base::PluggableFunction
{
  private:

    openfluid::tools::DistributeInterpolate m_DataPool;

    double m_Threshold;


  public:
    /**
      Constructor
    */
    RainSUFromFilesFunction();

    /**
      Destructor
    */
    ~RainSUFromFilesFunction();

    bool initParams(openfluid::core::FuncParamsMap_t Params);

    bool prepareData();

    bool checkConsistency();

    bool initializeRun(const openfluid::base::SimulationInfo* SimInfo);

    bool runStep(const openfluid::base::SimulationStatus* SimStatus);

    bool finalizeRun(const openfluid::base::SimulationInfo* SimInfo);

};


#endif  // __RAINSUFROMFILESFUNC_H__
