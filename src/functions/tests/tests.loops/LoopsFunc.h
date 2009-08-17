/**
  \file LoopsFunc.h
  \brief Header of ...
*/

#ifndef __LOOPSFUNC_H__
#define __LOOPSFUNC_H__

#include "openfluid-base.h"
#include "openfluid-core.h"


// =====================================================================
// =====================================================================


DECLARE_PLUGIN_HOOKS;


// =====================================================================
// =====================================================================


/**

*/
class LoopsFunction : public openfluid::base::PluggableFunction
{
  private:

  public:
    /**
      Constructor
    */
    LoopsFunction();

    /**
      Destructor
    */
    ~LoopsFunction();

    bool initParams(openfluid::core::FuncParamsMap_t Params);

    bool prepareData();

    bool checkConsistency();

    bool initializeRun(const openfluid::base::SimulationInfo* SimInfo);

    bool runStep(const openfluid::base::SimulationStatus* SimStatus);

    bool finalizeRun(const openfluid::base::SimulationInfo* SimInfo);

};


#endif  // __LOOPSFUNC_H__
