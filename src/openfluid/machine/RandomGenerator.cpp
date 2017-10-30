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
 */


#include <ctime>

#include <openfluid/machine/RandomGenerator.hpp>
#include <openfluid/tools/DataHelpers.hpp>


namespace openfluid { namespace machine {


RandomGenerator::RandomGenerator() :
  Generator(), m_Min(0.0), m_Max(0.0), m_DeltaT(0)
{
  std::random_device RandomDevice;

  m_RandomEngine.seed(RandomDevice());
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
  if (!OPENFLUID_GetSimulatorParameter(Params,"min",m_Min))
    throw openfluid::base::FrameworkException(OPENFLUID_CODE_LOCATION,"missing min value for generator");

  if (!OPENFLUID_GetSimulatorParameter(Params,"max",m_Max))
    throw openfluid::base::FrameworkException(OPENFLUID_CODE_LOCATION,"missing max value for generator");

  std::string DeltaTStr;
  if (OPENFLUID_GetSimulatorParameter(Params,"deltat",DeltaTStr) &&
      !openfluid::tools::convertString(DeltaTStr,&m_DeltaT))
    throw openfluid::base::FrameworkException(OPENFLUID_CODE_LOCATION,"wrong value for deltat");
};


// =====================================================================
// =====================================================================


void RandomGenerator::checkConsistency()
{
  if ( m_Min > m_Max)
    throw openfluid::base::FrameworkException(OPENFLUID_CODE_LOCATION,
                                              "max value must be greater or equal to min value for generator");
}


// =====================================================================
// =====================================================================


openfluid::base::SchedulingRequest RandomGenerator::initializeRun()
{
  openfluid::core::SpatialUnit* LU;
  OPENFLUID_UNITS_ORDERED_LOOP(m_UnitsClass,LU)
  {
    if (isVectorVariable())
    {
      openfluid::core::VectorValue VV(m_VarSize,openfluid::core::DoubleValue(0.0));
      OPENFLUID_InitializeVariable(LU,m_VarName,VV);
    }
    else
      OPENFLUID_InitializeVariable(LU,m_VarName,openfluid::core::DoubleValue(0.0));
  }

  if (m_DeltaT > 0)
    return Duration(m_DeltaT);
  else
    return DefaultDeltaT();
}


// =====================================================================
// =====================================================================


openfluid::base::SchedulingRequest RandomGenerator::runStep()
{
  openfluid::core::SpatialUnit* LU;
  std::uniform_real_distribution<double> Distribution(m_Min, m_Max);

  OPENFLUID_UNITS_ORDERED_LOOP(m_UnitsClass,LU)
  {
    openfluid::core::DoubleValue Value(Distribution(m_RandomEngine));

    if (isVectorVariable())
    {
      openfluid::core::VectorValue VV(m_VarSize,Value);
      OPENFLUID_AppendVariable(LU,m_VarName,VV);
    }
    else
      OPENFLUID_AppendVariable(LU,m_VarName,Value);

  }

  if (m_DeltaT > 0)
    return Duration(m_DeltaT);
  else
    return DefaultDeltaT();
}


} } //namespaces


