/**
  \file TransferGUFunc.h
  \brief header of TransferGUFunction

  \author Jean-Christophe FABRE <fabrejc@supagro.inra.fr>, Xavier LOUCHART <louchart@supagro.inra.fr>
*/


#ifndef __TRANSFERGUFUNC_H__
#define __TRANSFERGUFUNC_H__


#include "mhydasdk-core.h"
#include "mhydasdk-base.h"


// =====================================================================
// =====================================================================

/**

*/
class TransferGUFunction : public mhydasdk::base::PluggableFunction
{
  private:
    float m_DrainageCoeff;
    float m_InfiltrationCoeff;    
    float m_TankCoeff;

  public:

    /**
      Constructor
    */
    TransferGUFunction();

    /**
      Destructor
    */
    ~TransferGUFunction();

    bool initParams(mhydasdk::core::ParamsMap Params);

    bool initializeRun(mhydasdk::base::SimulationInfo* SimInfo);

    bool checkConsistency();

    bool runStep(mhydasdk::base::SimulationStatus* SimStatus);

    bool finalizeRun(mhydasdk::base::SimulationInfo* SimInfo);

};

// =====================================================================
// =====================================================================

extern "C"
{
  DLLIMPORT mhydasdk::base::PluggableFunction* GetMHYDASPluggableFunction();
};




#endif  // __TRANSFERGUFUNC_H__


