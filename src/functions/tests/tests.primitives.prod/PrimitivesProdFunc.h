/**
  \file PrimitivesProdFunc.h
  \brief Header of ...
*/

#ifndef __PRIMITIVESPRODFUNC_H__
#define __PRIMITIVESPRODFUNC_H__

#include "openfluid-base.h"
#include "openfluid-core.h"


// =====================================================================
// =====================================================================


DECLARE_PLUGIN_HOOKS;


// =====================================================================
// =====================================================================


/**

*/
class PrimitivesProdFunction : public openfluid::base::PluggableFunction
{
  private:

  public:
    /**
      Constructor
    */
    PrimitivesProdFunction();

    /**
      Destructor
    */
    ~PrimitivesProdFunction();

    bool initParams(openfluid::core::FuncParamsMap_t Params);

    bool prepareData();

    bool checkConsistency();

    bool initializeRun(const openfluid::base::SimulationInfo* SimInfo);

    bool runStep(const openfluid::base::SimulationStatus* SimStatus);

    bool finalizeRun(const openfluid::base::SimulationInfo* SimInfo);

};


#endif  // __PRIMITIVESPRODFUNC_H__
