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
  @file
  @brief Implements ...

  @author Jean-Christophe FABRE <fabrejc@supagro.inra.fr>
 */


#include <openfluid/machine/FixedGenerator.hpp>

namespace openfluid { namespace machine {


FixedGenerator::FixedGenerator() : Generator(), m_VarValue(0), m_DeltaT(0)
{

}


// =====================================================================
// =====================================================================


FixedGenerator::~FixedGenerator()
{


}


// =====================================================================
// =====================================================================


void FixedGenerator::initParams(const openfluid::ware::WareParams_t& Params)
{
  if (!OPENFLUID_GetSimulatorParameter(Params,"fixedvalue",m_VarValue))
    throw openfluid::base::FrameworkException("FixedGenerator::initParams","missing fixed value for generator");

  std::string DeltaTStr;
  if (OPENFLUID_GetSimulatorParameter(Params,"deltat",DeltaTStr) && !openfluid::tools::ConvertString(DeltaTStr,&m_DeltaT))
    throw openfluid::base::FrameworkException("RandomGenerator::initParams","wrong value for deltat");
}


// =====================================================================
// =====================================================================


openfluid::base::SchedulingRequest FixedGenerator::initializeRun()
{
  openfluid::core::SpatialUnit* LU;
  OPENFLUID_UNITS_ORDERED_LOOP(m_UnitClass,LU)
  {
    if (isVectorVariable())
    {
      openfluid::core::VectorValue VV(m_VarSize,0.0);
      OPENFLUID_InitializeVariable(LU,m_VarName,VV);
    }
    else
      OPENFLUID_InitializeVariable(LU,m_VarName,0.0);

  }

  if (m_DeltaT > 0) return Duration(m_DeltaT);
  else return DefaultDeltaT();
}

// =====================================================================
// =====================================================================


openfluid::base::SchedulingRequest FixedGenerator::runStep()
{

  openfluid::core::SpatialUnit* LU;

  OPENFLUID_UNITS_ORDERED_LOOP(m_UnitClass,LU)
  {

    if (isVectorVariable())
    {
      openfluid::core::VectorValue VV(m_VarSize,m_VarValue);
      OPENFLUID_AppendVariable(LU,m_VarName,VV);
    }
    else
    {
      OPENFLUID_AppendVariable(LU,m_VarName,m_VarValue);
    }
  }

  if (m_DeltaT > 0) return Duration(m_DeltaT);
  else return DefaultDeltaT();
}


} } //namespaces



