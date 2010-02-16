/***copyright>=========================================================

  OpenFLUID - Software environment for modelling Fluxes In Landscapes

  This file is a part of OpenFLUID software
  http://www.umr-lisah.fr/openfluid
  Copyright (c) INRA
  See the file "copyright" for the full license governing this code.

=========================================================<copyright***/


/**
  \file InterpGenerator.cpp
  \brief Implements ...

  \author Jean-Christophe FABRE <fabrejc@supagro.inra.fr>
 */



#include "InterpGenerator.h"


InterpGenerator::InterpGenerator() : Generator()
{


}


// =====================================================================
// =====================================================================


InterpGenerator::~InterpGenerator()
{


}


// =====================================================================
// =====================================================================


bool InterpGenerator::checkConsistency()
{


  return true;
}

// =====================================================================
// =====================================================================


bool InterpGenerator::initializeRun(const openfluid::base::SimulationInfo* SimInfo)
{


  return true;
}

// =====================================================================
// =====================================================================


bool InterpGenerator::runStep(const openfluid::base::SimulationStatus* SimStatus)
{

  throw openfluid::base::OFException("under construction");
  return true;
}

// =====================================================================
// =====================================================================


bool InterpGenerator::finalizeRun(const openfluid::base::SimulationInfo* SimInfo)
{

  return true;
}

