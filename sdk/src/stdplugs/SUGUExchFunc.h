/**
  \file SUGUExchFunc.h
  \brief header of SUGUExchangeFunction

  \author Jean-Christophe FABRE <fabrejc@ensam.inra.fr>
*/


#ifndef __SUGUEXCHFUNC_H__
#define __SUGUEXCHFUNC_H__


#include "sdk-core.h"
#include "sdk-base.h"

// =====================================================================
// =====================================================================

/**

*/
class SUGUExchangeFunction : public mhydasdk::base::Function
{
  private:

  public:

    /**
      Constructor
    */
    SUGUExchangeFunction(mhydasdk::core::CoreRepository *CoreData);

    /**
      Destructor
    */
    ~SUGUExchangeFunction();

    bool initParams(mhydasdk::core::ParamsMap Params);

    bool initializeRun(mhydasdk::base::SimulationInfo* SimInfo);

    bool checkConsistency();

    bool runStep(mhydasdk::base::SimulationStatus* SimStatus);

    bool finalizeRun(mhydasdk::base::SimulationInfo* SimInfo);

};


#endif  // __SUGUEXCHFUNC_H__


