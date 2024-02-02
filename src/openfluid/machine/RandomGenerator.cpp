/*

  This file is part of OpenFLUID software
  Copyright(c) 2007, INRA - Montpellier SupAgro


 == GNU General Public License Usage ==

  OpenFLUID is free software: you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation, either version 3 of the License, or
  (at your option) any later version.

  OpenFLUID is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with OpenFLUID. If not, see <http://www.gnu.org/licenses/>.


 == Other Usage ==

  Other Usage means a use of OpenFLUID that is inconsistent with the GPL
  license, and requires a written agreement between You and INRA.
  Licensees for Other Usage of OpenFLUID may use this file in accordance
  with the terms contained in the written agreement between You and INRA.
  
*/


/**
  @file RandomGenerator.cpp

  @author Jean-Christophe FABRE <jean-christophe.fabre@inra.fr>
  @author Armel THÖNI <armel.thoni@inrae.fr>
 */


#include <ctime>

#include <openfluid/machine/RandomGenerator.hpp>
#include <openfluid/tools/StringHelpers.hpp>


namespace openfluid { namespace machine {


RandomGenerator::RandomGenerator() :
  MonoGenerator(), m_IdenticalCellValues(true), m_DeltaT(0)
{
  std::random_device RandomDevice;

  m_RandomEngine.seed(RandomDevice());
}


// =====================================================================
// =====================================================================


template<class T>
NumericalRandomGenerator<T>::NumericalRandomGenerator() : RandomGenerator(), m_Min(0.0), m_Max(0.0)
{
}


// =====================================================================
// =====================================================================


DoubleRandomGenerator::DoubleRandomGenerator() :
  NumericalRandomGenerator<openfluid::core::DoubleValue>()
{
}


// =====================================================================
// =====================================================================


IntRandomGenerator::IntRandomGenerator() :
  NumericalRandomGenerator<int>()
{
}


// =====================================================================
// =====================================================================


BooleanRandomGenerator::BooleanRandomGenerator() :
  RandomGenerator(), m_Probability(0.5)
{
}


// =====================================================================
// =====================================================================


RandomGenerator::~RandomGenerator()
{

}


// =====================================================================
// =====================================================================


void RandomGenerator::initParams(const openfluid::ware::WareParams_t& Params)
{
  std::string DeltaTStr;
  if (OPENFLUID_GetSimulatorParameter(Params,"deltat",DeltaTStr) &&
      !openfluid::tools::toNumeric(DeltaTStr,m_DeltaT))
  {
    throw openfluid::base::FrameworkException(OPENFLUID_CODE_LOCATION,"wrong value for deltat");
  }
}


// =====================================================================
// =====================================================================


template<class T>
void NumericalRandomGenerator<T>::initParams(const openfluid::ware::WareParams_t& Params)
{

  if (!OPENFLUID_GetSimulatorParameter(Params,"min",m_Min))
  {
    throw openfluid::base::FrameworkException(OPENFLUID_CODE_LOCATION,"missing min value for generator");
  }

  if (!OPENFLUID_GetSimulatorParameter(Params,"max",m_Max))
  {
    throw openfluid::base::FrameworkException(OPENFLUID_CODE_LOCATION,"missing max value for generator");
  }
  RandomGenerator::initParams(Params);
}


// =====================================================================
// =====================================================================


void DoubleRandomGenerator::initParams(const openfluid::ware::WareParams_t& Params)
{
  std::string IdenticalCellsStr;
  if (OPENFLUID_GetSimulatorParameter(Params,"identicalcells",IdenticalCellsStr))
  {
    m_IdenticalCellValues = (IdenticalCellsStr == "1" || IdenticalCellsStr == "true");
    // convert str to bool 
    //TODO add to openfluid::tools::StringHelpers
  }

  NumericalRandomGenerator::initParams(Params);
}


// =====================================================================
// =====================================================================


void IntRandomGenerator::initParams(const openfluid::ware::WareParams_t& Params)
{
  NumericalRandomGenerator::initParams(Params);
}


// =====================================================================
// =====================================================================


template<class T>
void NumericalRandomGenerator<T>::checkConsistency()
{
  if ( m_Min > m_Max)
  {
    throw openfluid::base::FrameworkException(OPENFLUID_CODE_LOCATION,
                                              "max value must be greater or equal to min value for generator");
  }
}


// =====================================================================
// =====================================================================


void BooleanRandomGenerator::checkConsistency()
{
  if (m_Probability < 0 || m_Probability > 1)
  {
    throw openfluid::base::FrameworkException(OPENFLUID_CODE_LOCATION,
                                              "probability must be in [0,1] for generator");
  }
}


// =====================================================================
// =====================================================================


openfluid::base::SchedulingRequest RandomGenerator::endOfStep()
{
  if (m_DeltaT > 0)
  {
    return Duration(m_DeltaT);
  }
  else
  {
    return DefaultDeltaT();
  }
}


// =====================================================================
// =====================================================================


openfluid::base::SchedulingRequest DoubleRandomGenerator::initializeRun()
{
  openfluid::core::SpatialUnit* LU;
  OPENFLUID_UNITS_ORDERED_LOOP(m_UnitsClass,LU)
  {
    if (m_VarDimensions.isVector())
    {
      openfluid::core::VectorValue VV(m_VarDimensions.Rows,openfluid::core::DoubleValue(0.0));
      OPENFLUID_InitializeVariable(LU,m_VarName,VV);
    }
    else if (m_VarDimensions.isScalar())
    {
      OPENFLUID_InitializeVariable(LU,m_VarName,openfluid::core::DoubleValue(0.0));
    }
    else
    {
      openfluid::core::MatrixValue MV(m_VarDimensions.Cols,m_VarDimensions.Rows,openfluid::core::DoubleValue(0.0));
      OPENFLUID_InitializeVariable(LU,m_VarName,MV);
    }
  }

  return endOfStep();
}


// =====================================================================
// =====================================================================


openfluid::base::SchedulingRequest IntRandomGenerator::initializeRun()
{
  openfluid::core::SpatialUnit* LU;
  OPENFLUID_UNITS_ORDERED_LOOP(m_UnitsClass,LU)
  {
    if (m_VarDimensions.isScalar())
    {
      OPENFLUID_InitializeVariable(LU,m_VarName,openfluid::core::IntegerValue(0));
    }
    else
    {
      throw openfluid::base::FrameworkException(OPENFLUID_CODE_LOCATION,
                                              "int non-scalar var not handled for generator");
    }
  }

  return endOfStep();
}


// =====================================================================
// =====================================================================


openfluid::base::SchedulingRequest DoubleRandomGenerator::runStep()
{
  openfluid::core::SpatialUnit* LU;
  std::uniform_real_distribution<double> Distribution(m_Min, m_Max);

  OPENFLUID_UNITS_ORDERED_LOOP(m_UnitsClass,LU)
  {
    if (m_VarDimensions.isScalar())  // SCALAR CASE
    {
      openfluid::core::DoubleValue Value(Distribution(m_RandomEngine));
      OPENFLUID_AppendVariable(LU,m_VarName,Value);
    }
    else if (m_VarDimensions.isVector())  // VECTOR CASE
    {
      if (m_IdenticalCellValues)
      {
        openfluid::core::DoubleValue Value(Distribution(m_RandomEngine));
        openfluid::core::VectorValue VV(m_VarDimensions.Rows,Value);
        OPENFLUID_AppendVariable(LU,m_VarName,VV);
      }
      else
      {
        openfluid::core::VectorValue VV(m_VarDimensions.Rows,0.0);
        for (auto& Val : VV)
        {
          Val = openfluid::core::DoubleValue(Distribution(m_RandomEngine));
        }
        OPENFLUID_AppendVariable(LU,m_VarName,VV);
      }
    }
    else  // MATRIX CASE
    {
      if (m_IdenticalCellValues)
      {
        openfluid::core::DoubleValue Value(Distribution(m_RandomEngine));
        openfluid::core::MatrixValue MV(m_VarDimensions.Cols,m_VarDimensions.Rows,Value);
        OPENFLUID_AppendVariable(LU,m_VarName,MV);
      }
      else
      {
        openfluid::core::MatrixValue MV(m_VarDimensions.Cols,m_VarDimensions.Rows,0.0);
        for (unsigned int c=0;c<m_VarDimensions.Cols;c++)
        {
          for (unsigned int r=0;r<m_VarDimensions.Rows;r++)
          {
            MV.setElement(c, r, openfluid::core::DoubleValue(Distribution(m_RandomEngine)));
          }
        }
        OPENFLUID_AppendVariable(LU,m_VarName,MV);
      }
    }
  }

  return endOfStep();
}


// =====================================================================
// =====================================================================


openfluid::base::SchedulingRequest IntRandomGenerator::runStep()
{
  openfluid::core::SpatialUnit* LU;
  std::uniform_int_distribution<long> Distribution(m_Min, m_Max);

  OPENFLUID_UNITS_ORDERED_LOOP(m_UnitsClass,LU)
  {
    if (m_VarDimensions.isScalar())
    {
      openfluid::core::IntegerValue Value(Distribution(m_RandomEngine));
      OPENFLUID_AppendVariable(LU,m_VarName,Value);
    }
    else
    {
      throw openfluid::base::FrameworkException(OPENFLUID_CODE_LOCATION,
                                              "int non-scalar var not handled for generator");
    }
  }

  return endOfStep();
}


// =====================================================================
// =====================================================================


openfluid::base::SchedulingRequest BooleanRandomGenerator::initializeRun()
{
  openfluid::core::SpatialUnit* LU;
  OPENFLUID_UNITS_ORDERED_LOOP(m_UnitsClass,LU)
  {
    if (m_VarDimensions.isScalar())
    {
      OPENFLUID_InitializeVariable(LU,m_VarName,openfluid::core::BooleanValue(0));
    }
    else
    {
      throw openfluid::base::FrameworkException(OPENFLUID_CODE_LOCATION,
                                              "bool non-scalar var not handled for generator");
    }
  }

  return endOfStep();
}


// =====================================================================
// =====================================================================


openfluid::base::SchedulingRequest BooleanRandomGenerator::runStep()
{
  openfluid::core::SpatialUnit* LU;
  std::bernoulli_distribution Distribution(m_Probability);

  OPENFLUID_UNITS_ORDERED_LOOP(m_UnitsClass,LU)
  {
    if (m_VarDimensions.isScalar())
    {
      openfluid::core::BooleanValue Value(Distribution(m_RandomEngine));
      OPENFLUID_AppendVariable(LU,m_VarName,Value);
    }
    else
    {
      throw openfluid::base::FrameworkException(OPENFLUID_CODE_LOCATION,
                                              "bool non-scalar var not handled for generator");
    }
  }

  return endOfStep();
}


} } //namespaces

