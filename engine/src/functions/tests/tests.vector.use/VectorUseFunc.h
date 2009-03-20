/**
  \file VectorUseFunc.h
  \brief Header of ...
*/

#ifndef __VECTORUSEFUNC_H__
#define __VECTORUSEFUNC_H__

#include "openfluid-base.h"
#include "openfluid-core.h"


// =====================================================================
// =====================================================================


DECLARE_PLUGIN_HOOKS;


// =====================================================================
// =====================================================================


/**

*/
class VectorUseFunction : public openfluid::base::PluggableFunction
{
  private:

  public:
    /**
      Constructor
    */
    VectorUseFunction();

    /**
      Destructor
    */
    ~VectorUseFunction();

    bool initParams(openfluid::core::FuncParamsMap_t Params);

    bool prepareData();

    bool checkConsistency();

    bool initializeRun(const openfluid::base::SimulationInfo* SimInfo);

    bool runStep(const openfluid::base::SimulationStatus* SimStatus);

    bool finalizeRun(const openfluid::base::SimulationInfo* SimInfo);

};


#endif  // __VECTORUSEFUNC_H__
