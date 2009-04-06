/**
  \file FortranFunc.h
  \brief Header of ...
*/

#ifndef __FORTRANFUNC_H__
#define __FORTRANFUNC_H__

#include "openfluid-base.h"
#include "openfluid-core.h"


// =====================================================================
// =====================================================================


DECLARE_PLUGIN_HOOKS;


// =====================================================================
// =====================================================================


/**

*/
class FortranFunction : public openfluid::base::PluggableFunction
{
  private:

  public:
    /**
      Constructor
    */
    FortranFunction();

    /**
      Destructor
    */
    ~FortranFunction();

    bool initParams(openfluid::core::FuncParamsMap_t Params);

    bool prepareData();

    bool checkConsistency();

    bool initializeRun(const openfluid::base::SimulationInfo* SimInfo);

    bool runStep(const openfluid::base::SimulationStatus* SimStatus);

    bool finalizeRun(const openfluid::base::SimulationInfo* SimInfo);

};


#endif  // __FORTRANFUNC_H__
