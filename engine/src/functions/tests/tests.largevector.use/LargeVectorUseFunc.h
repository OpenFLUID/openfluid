/**
  \file LargeVectorUseFunc.h
  \brief Header of ...
*/

#ifndef __LARGEVECTORUSEFUNC_H__
#define __LARGEVECTORUSEFUNC_H__

#include "openfluid-base.h"
#include "openfluid-core.h"


// =====================================================================
// =====================================================================


DECLARE_PLUGIN_HOOKS;


// =====================================================================
// =====================================================================


/**

*/
class LargeVectorUseFunction : public openfluid::base::PluggableFunction
{
  private:

  public:
    /**
      Constructor
    */
    LargeVectorUseFunction();

    /**
      Destructor
    */
    ~LargeVectorUseFunction();

    bool initParams(openfluid::core::FuncParamsMap_t Params);

    bool prepareData();

    bool checkConsistency();

    bool initializeRun(const openfluid::base::SimulationInfo* SimInfo);

    bool runStep(const openfluid::base::SimulationStatus* SimStatus);

    bool finalizeRun(const openfluid::base::SimulationInfo* SimInfo);

};


#endif  // __LARGEVECTORUSEFUNC_H__
