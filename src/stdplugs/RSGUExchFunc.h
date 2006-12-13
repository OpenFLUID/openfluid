/**
  \file RSGUExchFunc.h
  \brief header of RSGUExchangeFunction

  \author Jean-Christophe FABRE <fabrejc@ensam.inra.fr>
*/


#ifndef __RSGUEXCHFUNC_H__
#define __RSGUEXCHFUNC_H__


#include "sdk-core.h"
#include "sdk-base.h"

// =====================================================================
// =====================================================================

/**

*/
class RSGUExchangeFunction : public mhydasdk::base::Function
{
  private:

  public:

    /**
      Constructor
    */
    RSGUExchangeFunction(mhydasdk::core::CoreRepository *CoreData);

    /**
      Destructor
    */
    ~RSGUExchangeFunction();

    bool initParams(mhydasdk::core::ParamsMap Params);

    bool initializeRun(mhydasdk::base::SimulationInfo* SimInfo);

    bool checkConsistency();

    bool runStep(mhydasdk::base::SimulationStatus* SimStatus);

    bool finalizeRun(mhydasdk::base::SimulationInfo* SimInfo);

};


#endif  // __RSGUEXCHFUNC_H__


