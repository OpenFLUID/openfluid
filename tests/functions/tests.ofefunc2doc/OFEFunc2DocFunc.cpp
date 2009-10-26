/***copyright>=========================================================

  OpenFLUID - Software environment for modelling Fluxes In Landscapes

  This file is a part of OpenFLUID software
  http://www.umr-lisah.fr/openfluid
  Copyright (c) INRA
  See the file "copyright" for the full license governing this code.

=========================================================<copyright***/


/**
  \file PrimitivesProdFunc.cpp
  \brief Implements ...
*/



/*
<func2doc>

\begin{center}
\includegraphics[scale=0.4]{doc/images/openfluid_official.png}
\end{center}


This is the \LaTeX -style documentation of the \funcID\ function (\funcNAME, version \funcVERSION).\\

\input{doc/part1}
\input{doc/part2}

\cite{Moussa2002b}

\bibliographystyle{plain}
\bibliography{doc/bibliography/MHYDAS_OpenFLUID}

</func2doc>

*/


#include "OFEFunc2DocFunc.h"


// =====================================================================
// =====================================================================


DEFINE_FUNCTION_HOOK(OFEFunc2DocFunction)


// =====================================================================
// =====================================================================


BEGIN_SIGNATURE_HOOK
  DECLARE_SIGNATURE_ID(("tests.ofefunc2doc"));
  DECLARE_SIGNATURE_NAME(("test function for ofefunc2doc"));
  DECLARE_SIGNATURE_DESCRIPTION(("This function is a test for the ofefunc2doc doc extraction tool. Do not use it in a model."));

  DECLARE_SIGNATURE_VERSION(("1.0"));
  DECLARE_SIGNATURE_SDKVERSION;
  DECLARE_SIGNATURE_STATUS(openfluid::base::BETA);

  DECLARE_SIGNATURE_DOMAIN(("Test"));
  DECLARE_SIGNATURE_PROCESS(("No process"));
  DECLARE_SIGNATURE_METHOD(("No method"));
  DECLARE_SIGNATURE_AUTHORNAME(("JC Fabre"));
  DECLARE_SIGNATURE_AUTHOREMAIL(("fabrejc@supagro.inra.fr"));

  DECLARE_FUNCTION_PARAM("testparam","a param just for test","kg/m3")
  DECLARE_FUNCTION_PARAM("dummyp","a dummy param just for test","snail/m2")

  DECLARE_PRODUCED_VAR("tests.prodvector[]","TestUnits","produced vector on TestUnits","");
  DECLARE_PRODUCED_VAR("tests.prodscalar","TestUnits","produced scalar on TestUnits","joker/batman");

  DECLARE_UPDATED_VAR("tests.updscalar","TestUnits","updated scalar on TestUnits","");

  DECLARE_REQUIRED_VAR("tests.reqvector[]","TestUnits","required vector on TestUnits","m2");
  DECLARE_REQUIRED_VAR("tests.reqscalar","TestUnits","required scalar on TestUnits","");

  DECLARE_USED_VAR("tests.usedscalar","TestUnits","used scalar on TestUnits","");

  DECLARE_REQUIRED_INPUTDATA("indataA","TestUnits","required input data A on TestUnits","m")
  DECLARE_USED_INPUTDATA("indataB","TestUnits","used input data B on TestUnits","mm/h")

  DECLARE_USED_EVENTS("TestUnits");
  DECLARE_USED_EVENTS("FakeTestUnits");

  DECLARE_USED_EXTRAFILE("test01.txt");
  DECLARE_USED_EXTRAFILE("test02.txt");
  DECLARE_REQUIRED_EXTRAFILE("test03req.txt");

END_SIGNATURE_HOOK


// =====================================================================
// =====================================================================


OFEFunc2DocFunction::OFEFunc2DocFunction()
                : PluggableFunction()
{


}


// =====================================================================
// =====================================================================


OFEFunc2DocFunction::~OFEFunc2DocFunction()
{


}


// =====================================================================
// =====================================================================


bool OFEFunc2DocFunction::initParams(openfluid::core::FuncParamsMap_t Params)
{

  return true;
}

// =====================================================================
// =====================================================================


bool OFEFunc2DocFunction::prepareData()
{


  return true;
}


// =====================================================================
// =====================================================================


bool OFEFunc2DocFunction::checkConsistency()
{


  return true;
}


// =====================================================================
// =====================================================================


bool OFEFunc2DocFunction::initializeRun(const openfluid::base::SimulationInfo* SimInfo)
{


  return true;
}

// =====================================================================
// =====================================================================


bool OFEFunc2DocFunction::runStep(const openfluid::base::SimulationStatus* SimStatus)
{

  return true;
}

// =====================================================================
// =====================================================================


bool OFEFunc2DocFunction::finalizeRun(const openfluid::base::SimulationInfo* SimInfo)
{


  return true;
}

