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
#include <boost/math/special_functions/fpclassify.hpp>


InterpGenerator::InterpGenerator() : Generator()
{
  m_IsMax = false;
  m_IsMin = false;

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

  if (m_GenDesc.getParameters()["thresholdmin"] != "")
  {
    if (!openfluid::tools::ConvertString(m_GenDesc.getParameters()["thresholdmin"],&m_Min))
      throw openfluid::base::OFException("kernel","InterpGenerator::checkConsistency","wrong format for threshold min value");
    m_IsMin = true;
  }

  if (m_GenDesc.getParameters()["thresholdmax"] != "")
  {
    if (!openfluid::tools::ConvertString(m_GenDesc.getParameters()["thresholdmax"],&m_Max))
      throw openfluid::base::OFException("kernel","InterpGenerator::checkConsistency","wrong format for threshold max value");
    m_IsMax = true;
  }

  if (m_IsMin && m_IsMax && m_Min > m_Max)
    throw openfluid::base::OFException("kernel","InterpGenerator::checkConsistency","threshold max value must be greater or equal to threshold min value for generator");


  return true;
}

// =====================================================================
// =====================================================================


bool InterpGenerator::initializeRun(const openfluid::base::SimulationInfo* SimInfo)
{

  std::string InputDir;

  OPENFLUID_GetRunEnvironment("dir.input",&InputDir);

  m_DataPool.setConfig(InputDir,m_GenDesc.getParameters()["sources"],m_GenDesc.getParameters()["distribution"],
                       //openfluid::tools::SERIEPREPCS_CUMULATE,
                       openfluid::tools::SERIEPREPCS_NONE,
                       SimInfo->getStartTime(),SimInfo->getEndTime(),SimInfo->getTimeStep());

  m_DataPool.loadAndPrepareData();

  return true;
}

// =====================================================================
// =====================================================================


bool InterpGenerator::runStep(const openfluid::base::SimulationStatus* SimStatus)
{

  openfluid::core::Unit* LU;
  openfluid::core::ScalarValue CurrentValue;
  int ID;

  DECLARE_UNITS_ORDERED_LOOP(1);
  BEGIN_UNITS_ORDERED_LOOP(1,m_GenDesc.getUnitClass(),LU);

    ID = LU->getID();

    if (m_DataPool.getValue(ID,SimStatus->getCurrentStep(),&CurrentValue))
    {
      if (boost::math::isnan(CurrentValue))
        throw openfluid::base::OFException("kernel","InterpGenerator::runStep","interpolated value for variable " + m_GenDesc.getVariableName() + " is NaN");

      if (m_IsMax && CurrentValue > m_IsMax) CurrentValue = m_Max;
      if (m_IsMin && CurrentValue < m_IsMin) CurrentValue = m_Min;


      if (m_GenDesc.isVectorVariable())
      {
        openfluid::core::VectorValue VV(m_GenDesc.getVariableSize(),CurrentValue);
        OPENFLUID_AppendVariable(LU,m_GenDesc.getVariableName(),VV);
      }
      else
        OPENFLUID_AppendVariable(LU,m_GenDesc.getVariableName(),CurrentValue);
    }
    else
      throw openfluid::base::OFException("kernel","InterpGenerator::runStep","error interpolating value for variable " + m_GenDesc.getVariableName());


  END_LOOP;


  return true;
}

// =====================================================================
// =====================================================================


bool InterpGenerator::finalizeRun(const openfluid::base::SimulationInfo* SimInfo)
{

  return true;
}

