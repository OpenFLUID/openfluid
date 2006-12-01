/**
  \file HayamiSUFunc.h
  \brief header of HayamiSUFunction

  \author Jean-Christophe FABRE <fabrejc@ensam.inra.fr>
*/


#ifndef __HAYAMISUFUNC_H__
#define __HAYAMISUFUNC_H__


#include "sdk-core.h"
#include "sdk-base.h"

// =====================================================================
// =====================================================================

/**

*/
class HayamiSUFunction : public mhydasdk::base::Function
{
  private:

  public:

    /**
      Constructor
    */
    HayamiSUFunction(mhydasdk::core::CoreRepository *CoreData);

    /**
      Destructor
    */
    ~HayamiSUFunction();

    bool initParams(mhydasdk::core::ParamsMap Params);

    bool initializeRun();

    bool checkConsistency();

    bool runStep(mhydasdk::base::SimulationStatus* SimStatus);

    bool finalizeRun();

};


#endif  // __HAYAMISUFUNC_H__


