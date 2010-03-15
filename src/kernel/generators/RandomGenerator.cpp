/***copyright>=========================================================

  OpenFLUID - Software environment for modelling Fluxes In Landscapes

  This file is a part of OpenFLUID software
  http://www.umr-lisah.fr/openfluid
  Copyright (c) INRA
  See the file "copyright" for the full license governing this code.

=========================================================<copyright***/


/**
  \file RandomGenerator.cpp
  \brief Implements ...

  \author Jean-Christophe FABRE <fabrejc@supagro.inra.fr>
 */


#include <boost/random.hpp>

#include "RandomGenerator.h"


RandomGenerator::RandomGenerator() : Generator()
{
  m_Min = 0.0;
  m_Max = 0.0;

}


// =====================================================================
// =====================================================================


RandomGenerator::~RandomGenerator()
{


}


// =====================================================================
// =====================================================================


bool RandomGenerator::checkConsistency()
{
  if (m_GenDesc.getParameters().find("min") != m_GenDesc.getParameters().end())
  {
    if (!openfluid::tools::ConvertString(m_GenDesc.getParameters()["min"],&m_Min))
      throw openfluid::base::OFException("kernel","RandomGenerator::checkConsistency","wrong format for min value");
  }
  else
    throw openfluid::base::OFException("kernel","FixedGenerator::checkConsistency","missing min value for generator");


  if (m_GenDesc.getParameters().find("max") != m_GenDesc.getParameters().end())
  {
    if (!openfluid::tools::ConvertString(m_GenDesc.getParameters()["max"],&m_Max))
      throw openfluid::base::OFException("kernel","RandomGenerator::checkConsistency","wrong format for max value");
  }
  else
    throw openfluid::base::OFException("kernel","FixedGenerator::checkConsistency","missing max value for generator");


  if ( m_Min > m_Max)
    throw openfluid::base::OFException("kernel","FixedGenerator::checkConsistency","max value must be greater or equal to min value for generator");

  return true;
}


// =====================================================================
// =====================================================================


bool RandomGenerator::initializeRun(const openfluid::base::SimulationInfo* SimInfo)
{


  return true;
}

// =====================================================================
// =====================================================================


bool RandomGenerator::runStep(const openfluid::base::SimulationStatus* SimStatus)
{

  openfluid::core::Unit* LU;
  openfluid::core::ScalarValue Value;

  boost::uniform_real<> Distribution(m_Min, m_Max);
  boost::mt19937 Engine;
  boost::variate_generator<boost::mt19937, boost::uniform_real<> > Random (Engine, Distribution);

  DECLARE_UNITS_ORDERED_LOOP(1);

  BEGIN_UNITS_ORDERED_LOOP(1,m_GenDesc.getUnitClass(),LU)

    Value = Random();

    if (m_GenDesc.isVectorVariable())
    {
      openfluid::core::VectorValue VV(m_GenDesc.getVariableSize(),Value);
      OPENFLUID_AppendVariable(LU,m_GenDesc.getVariableName(),VV);
    }
    else
      OPENFLUID_AppendVariable(LU,m_GenDesc.getVariableName(),Value);

  END_LOOP

  return true;
}

// =====================================================================
// =====================================================================


bool RandomGenerator::finalizeRun(const openfluid::base::SimulationInfo* SimInfo)
{

  return true;
}

