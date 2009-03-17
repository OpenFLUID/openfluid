/**
  \file HayamiSUFunc.h
  \brief header of HayamiSUFunction

  \author Jean-Christophe FABRE <fabrejc@supagro.inra.fr>
*/


#include "HayamiSUFunc.h"
#include <iostream>
#include "math.h"

#include "HayamiTools.h"

// =====================================================================
// =====================================================================

DEFINE_FUNCTION_HOOK(HayamiSUFunction);



// =====================================================================
// =====================================================================

BEGIN_SIGNATURE_HOOK

  DECLARE_SIGNATURE_ID(("water.surf.transfer-su.hayami"));
  DECLARE_SIGNATURE_NAME(("water transfer on surface units using hayami propagation method"));
  DECLARE_SIGNATURE_DESCRIPTION((""));
  DECLARE_SIGNATURE_DOMAIN(("hydrology"));

  DECLARE_SIGNATURE_STATUS(openfluid::base::BETA);

  DECLARE_SIGNATURE_SDKVERSION;

  DECLARE_SIGNATURE_AUTHORNAME(("Moussa R., Fabre J.-C."));
  DECLARE_SIGNATURE_AUTHOREMAIL(("moussa@supagro.inra.fr, fabrejc@supagro.inra.fr"));

  DECLARE_SU_PRODUCED_VAR("water.surf.Q.downstream-su",("output volume at the outlet of the SUs"),("m3/s"));

  DECLARE_SU_REQUIRED_VAR("water.surf.H.runoff",("runoff on the surface of the unit"),("m/s"));

  DECLARE_SU_REQUIRED_PROPERTY("nmanning",("Manning roughness coefficient"),("s/m^(-1/3)"));


  DECLARE_FUNCTION_PARAM("maxsteps",("maximum hayami kernel steps"),(""));
  DECLARE_FUNCTION_PARAM("meancel",("wave mean celerity"),("m/s"));
  DECLARE_FUNCTION_PARAM("meansigma",("mean diffusivity on SUs"),("m2/s"));

END_SIGNATURE_HOOK

// =====================================================================
// =====================================================================



HayamiSUFunction::HayamiSUFunction()
                : PluggableFunction()
{


  // default values
  m_MaxSteps = 100;
  m_MeanCelerity = 0.045;
  m_MeanSigma = 500;
  m_MeanSlope = 0;
  m_MeanManning = 0;

}


// =====================================================================
// =====================================================================


HayamiSUFunction::~HayamiSUFunction()
{

}


// =====================================================================
// =====================================================================


bool HayamiSUFunction::initParams(openfluid::core::FuncParamsMap_t Params)
{

  OPENFLUID_GetFunctionParameter(Params,("maxsteps"),&m_MaxSteps);
  OPENFLUID_GetFunctionParameter(Params,("meancel"),&m_MeanCelerity);
  OPENFLUID_GetFunctionParameter(Params,("meansigma"),&m_MeanSigma);

  return true;
}


// =====================================================================
// =====================================================================

bool HayamiSUFunction::prepareData()
{

  return true;
}


// =====================================================================
// =====================================================================



bool HayamiSUFunction::checkConsistency()
{

  return true;
}

// =====================================================================
// =====================================================================


bool HayamiSUFunction::initializeRun(const openfluid::base::SimulationInfo* SimInfo)
{
  openfluid::core::Unit* SU;
  float Cel, Sigma;
  openfluid::core::UnitID_t ID;
  openfluid::core::ScalarValue TmpValue;
  openfluid::core::ScalarValue TmpValue2;
  unsigned int SUCount = 0;

  OPENFLUID_GetUnitsCount("SU",&SUCount);

  DECLARE_SU_ORDERED_LOOP;


  BEGIN_SU_ORDERED_LOOP(SU)
    ID = SU->getID();

    m_Input[ID] = new openfluid::core::SerieOfScalarValue();
    m_CurrentInputSum[ID] = 0;

    OPENFLUID_GetInputData(SU,("slope"),&TmpValue);
    m_MeanSlope = m_MeanSlope + TmpValue;
    OPENFLUID_GetInputData(SU,("nmanning"),&TmpValue);
    m_MeanManning = m_MeanManning + TmpValue;
  END_LOOP

  m_MeanSlope = m_MeanSlope / SUCount;
  m_MeanManning = m_MeanManning / SUCount;

  BEGIN_SU_ORDERED_LOOP(SU)
    OPENFLUID_GetInputData(SU,("nmanning"),&TmpValue);
    OPENFLUID_GetInputData(SU,("slope"),&TmpValue2);
    Cel = m_MeanCelerity * (m_MeanManning / TmpValue) * (sqrt((TmpValue2 / m_MeanSlope)));
    Sigma = m_MeanSigma * (TmpValue / m_MeanManning) * (m_MeanSlope / TmpValue2);

    m_SUKernel[SU->getID()] = t_HayamiKernel();

    OPENFLUID_GetInputData(SU,("flowdist"),&TmpValue);
    ComputeHayamiKernel(Cel, Sigma,TmpValue,m_MaxSteps,SimInfo->getTimeStep(), &m_SUKernel[SU->getID()]);

  END_LOOP


  return true;
}



// =====================================================================
// =====================================================================


bool HayamiSUFunction::runStep(const openfluid::base::SimulationStatus* SimStatus)
{

  int ID;
  int CurrentStep;
  int TimeStep;
  openfluid::core::ScalarValue QOutput;
  openfluid::core::ScalarValue QInput;
  openfluid::core::ScalarValue TmpValue, TmpValue2;

  openfluid::core::Unit* SU;


  TimeStep = SimStatus->getTimeStep();
  CurrentStep = SimStatus->getCurrentStep();

  DECLARE_SU_ORDERED_LOOP;
  BEGIN_SU_ORDERED_LOOP(SU)

    ID = SU->getID();

    OPENFLUID_GetVariable(SU,("water.surf.H.runoff"),CurrentStep,&TmpValue);

    OPENFLUID_GetInputData(SU,("area"),&TmpValue2);
    QInput = TmpValue * TmpValue2 / TimeStep;
    m_CurrentInputSum[ID] = m_CurrentInputSum[ID] + QInput;
    m_Input[ID]->push_back(QInput);

    QOutput = 0;
    if (m_CurrentInputSum[ID] > 0)
    {
      QOutput = DoHayamiPropagation(m_SUKernel[ID], CurrentStep, m_Input[ID], m_MaxSteps, TimeStep);
    }


    OPENFLUID_AppendVariable(SU,("water.surf.Q.downstream-su"),QOutput);

  END_LOOP



  return true;

}


// =====================================================================
// =====================================================================


bool HayamiSUFunction::finalizeRun(const openfluid::base::SimulationInfo* SimInfo)
{

  return true;
}


