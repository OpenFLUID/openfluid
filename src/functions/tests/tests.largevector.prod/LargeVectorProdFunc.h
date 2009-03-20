/**
  \file LargeVectorProdFunc.h
  \brief Header of ...
*/

#ifndef __LARGEVECTORPRODFUNC_H__
#define __LARGEVECTORPRODFUNC_H__

#include "openfluid-base.h"
#include "openfluid-core.h"


// =====================================================================
// =====================================================================


DECLARE_PLUGIN_HOOKS;


// =====================================================================
// =====================================================================


/**

*/
class LargeVectorProdFunction : public openfluid::base::PluggableFunction
{
  private:

  public:
    /**
      Constructor
    */
    LargeVectorProdFunction();

    /**
      Destructor
    */
    ~LargeVectorProdFunction();

    bool initParams(openfluid::core::FuncParamsMap_t Params);

    bool prepareData();

    bool checkConsistency();

    bool initializeRun(const openfluid::base::SimulationInfo* SimInfo);

    bool runStep(const openfluid::base::SimulationStatus* SimStatus);

    bool finalizeRun(const openfluid::base::SimulationInfo* SimInfo);

};


#endif  // __LARGEVECTORPRODFUNC_H__
