/***copyright>=========================================================

  OpenFLUID - Software environment for modelling Fluxes In Landscapes

  This file is a part of OpenFLUID software
  http://www.umr-lisah.fr/openfluid
  Copyright (c) INRA
  See the file "copyright" for the full license governing this code.

=========================================================<copyright***/


/**
  \file FixedGenerator.cpp
  \brief Implements ...

  \author Jean-Christophe FABRE <fabrejc@supagro.inra.fr>
 */


#include "FixedGenerator.h"


FixedGenerator::FixedGenerator() : Generator()
{

}


// =====================================================================
// =====================================================================


FixedGenerator::~FixedGenerator()
{


}

// =====================================================================
// =====================================================================


bool FixedGenerator::checkConsistency()
{
  if (m_GenDesc.getParameters().find("fixedvalue") != m_GenDesc.getParameters().end())
  {
    if (!openfluid::tools::ConvertString(m_GenDesc.getParameters()["fixedvalue"],&m_VarValue))
      throw openfluid::base::OFException("kernel","FixedGenerator::checkConsistency","wrong fixedvalue format for generator");
  }
  else
    throw openfluid::base::OFException("kernel","FixedGenerator::checkConsistency","missing value for generator");

  return true;
}


// =====================================================================
// =====================================================================


bool FixedGenerator::initializeRun(const openfluid::base::SimulationInfo* SimInfo)
{

  return true;
}

// =====================================================================
// =====================================================================


bool FixedGenerator::runStep(const openfluid::base::SimulationStatus* SimStatus)
{

  openfluid::core::Unit* LU;

  DECLARE_UNITS_ORDERED_LOOP(1);

  BEGIN_UNITS_ORDERED_LOOP(1,m_GenDesc.getUnitClass(),LU)

    if (m_GenDesc.isVectorVariable())
    {
      openfluid::core::VectorValue VV(m_GenDesc.getVariableSize(),m_VarValue);
      OPENFLUID_AppendVariable(LU,m_GenDesc.getVariableName(),VV);
    }
    else
      OPENFLUID_AppendVariable(LU,m_GenDesc.getVariableName(),m_VarValue);

  END_LOOP

  return true;
}

// =====================================================================
// =====================================================================


bool FixedGenerator::finalizeRun(const openfluid::base::SimulationInfo* SimInfo)
{

  return true;
}

