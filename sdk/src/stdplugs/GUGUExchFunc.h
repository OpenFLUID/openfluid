/**
  \file GUGUExchFunc.h
  \brief header of GUGUExchangeFunction

  \author Jean-Christophe FABRE <fabrejc@ensam.inra.fr>
*/


#ifndef __GUGUEXCHFUNC_H__
#define __GUGUEXCHFUNC_H__


#include "sdk-core.h"
#include "sdk-base.h"

// =====================================================================
// =====================================================================

/**

*/
class GUGUExchangeFunction : public mhydasdk::base::Function
{
  private:

  public:

    /**
      Constructor
    */
    GUGUExchangeFunction(mhydasdk::core::CoreRepository *CoreData);

    /**
      Destructor
    */
    ~GUGUExchangeFunction();

    bool initParams(mhydasdk::core::ParamsMap Params);

    bool initializeRun(mhydasdk::base::SimulationInfo* SimInfo);

    bool checkConsistency();

    bool runStep(mhydasdk::base::SimulationStatus* SimStatus);

    bool finalizeRun(mhydasdk::base::SimulationInfo* SimInfo);

};


#endif  // __GUGUEXCHFUNC_H__


