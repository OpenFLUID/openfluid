/**
  \file HayamiSUFunc.h
  \brief header of HayamiSUFunction

  \author Jean-Christophe FABRE <fabrejc@supagro.inra.fr>
*/


#ifndef __HAYAMISUFUNC_H__
#define __HAYAMISUFUNC_H__


#include "openfluid-core.h"
#include "openfluid-base.h"

#include "HayamiTools.h"

// =====================================================================
// =====================================================================



/**

*/
class HayamiSUFunction : public openfluid::base::PluggableFunction
{
  private:

    int m_MaxSteps;

    double m_MeanCelerity;

    double m_MeanSigma;

    double m_MeanSlope;

    double m_MeanManning;

    IDKernelMap m_SUKernel;

    openfluid::core::IDSerieOfScalarValuePtrMap m_Input;

    openfluid::core::IDScalarValueMap m_CurrentInputSum;


  public:

    /**
      Constructor
    */
    HayamiSUFunction();

    /**
      Destructor
    */
    ~HayamiSUFunction();

    bool initParams(openfluid::core::FuncParamsMap_t Params);

    bool prepareData();

    bool checkConsistency();

    bool initializeRun(const openfluid::base::SimulationInfo* SimInfo);

    bool runStep(const openfluid::base::SimulationStatus* SimStatus);

    bool finalizeRun(const openfluid::base::SimulationInfo* SimInfo);

};

// =====================================================================
// =====================================================================


DECLARE_PLUGIN_HOOKS


#endif  // __HAYAMISUFUNC_H__


