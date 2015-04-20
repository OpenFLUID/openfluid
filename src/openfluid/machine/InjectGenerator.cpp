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
  @file InjectGenerator.cpp

  @author Jean-Christophe FABRE <jean-christophe.fabre@supagro.inra.fr>
 */


#include <openfluid/machine/InjectGenerator.hpp>


namespace openfluid { namespace machine {


InjectGenerator::InjectGenerator() : Generator(),
  m_IsMin(false), m_IsMax(false), m_Min(0.0), m_Max(0.0),
  m_SourcesFile(""),m_DistriFile(""), m_DistriBindings(NULL)
{

}


// =====================================================================
// =====================================================================


InjectGenerator::~InjectGenerator()
{
  if (m_DistriBindings != NULL) delete m_DistriBindings;

}


// =====================================================================
// =====================================================================


void InjectGenerator::initParams(const openfluid::ware::WareParams_t& Params)
{
  if (!OPENFLUID_GetSimulatorParameter(Params,"sources",m_SourcesFile))
    throw openfluid::base::FrameworkException(OPENFLUID_CODE_LOCATION,
                                              "missing sources value for generator");

  if (!OPENFLUID_GetSimulatorParameter(Params,"distribution",m_DistriFile))
    throw openfluid::base::FrameworkException(OPENFLUID_CODE_LOCATION,
                                              "missing distribution value for generator");


  if (OPENFLUID_GetSimulatorParameter(Params,"thresholdmin",m_Min)) m_IsMin = true;

  if (OPENFLUID_GetSimulatorParameter(Params,"thresholdmax",m_Max)) m_IsMax = true;

};


// =====================================================================
// =====================================================================


void InjectGenerator::prepareData()
{
  openfluid::tools::DistributionTables DistriTables;
  std::string InputDir;

  OPENFLUID_GetRunEnvironment("dir.input",InputDir);

  DistriTables.build(InputDir,m_SourcesFile,m_DistriFile);
  m_DistriBindings = new openfluid::tools::DistributionBindings(DistriTables);
}


// =====================================================================
// =====================================================================


void InjectGenerator::checkConsistency()
{
  if (m_IsMin && m_IsMax && m_Min > m_Max)
    throw openfluid::base::FrameworkException(OPENFLUID_CODE_LOCATION,
                                              "threshold max value must be greater or equal "
                                              "to threshold min value for generator");
}


// =====================================================================
// =====================================================================


openfluid::base::SchedulingRequest InjectGenerator::initializeRun()
{
  m_DistriBindings->advanceToTime(OPENFLUID_GetCurrentDate());
  openfluid::core::DoubleValue Value;
  openfluid::core::SpatialUnit* LU;
  openfluid::core::DateTime CurrentDT(OPENFLUID_GetCurrentDate());

  OPENFLUID_UNITS_ORDERED_LOOP(m_UnitsClass,LU)
  {
    if (m_DistriBindings->getValue(LU->getID(),CurrentDT,Value))
    {

      if (m_IsMax && Value > m_Max) Value = m_Max;
      if (m_IsMin && Value < m_Min) Value = m_Min;
    }
    else
    {
      Value = 0.0;
    }

    if (isVectorVariable())
    {
      openfluid::core::VectorValue VV(m_VarSize,Value);
      OPENFLUID_InitializeVariable(LU,m_VarName,VV);
    }
    else
      OPENFLUID_InitializeVariable(LU,m_VarName,Value);
  }

  openfluid::core::DateTime NextDT;

  if (m_DistriBindings->advanceToNextTimeAfter(CurrentDT,NextDT))
  {
    return Duration(NextDT.diffInSeconds(CurrentDT));
  }
  else
    return Never();
}

// =====================================================================
// =====================================================================


openfluid::base::SchedulingRequest InjectGenerator::runStep()
{
  m_DistriBindings->advanceToTime(OPENFLUID_GetCurrentDate());

  openfluid::core::DoubleValue Value;
  openfluid::core::SpatialUnit* LU;
  openfluid::core::DateTime CurrentDT(OPENFLUID_GetCurrentDate());

  OPENFLUID_UNITS_ORDERED_LOOP(m_UnitsClass,LU)
  {
    if (m_DistriBindings->getValue(LU->getID(),CurrentDT,Value))
    {

      if (m_IsMax && Value > m_Max) Value = m_Max;
      if (m_IsMin && Value < m_Min) Value = m_Min;

      if (isVectorVariable())
      {
        openfluid::core::VectorValue VV(m_VarSize,Value);
        OPENFLUID_InitializeVariable(LU,m_VarName,VV);
      }
      else
        OPENFLUID_AppendVariable(LU,m_VarName,Value);

    }
  }

  openfluid::core::DateTime NextDT;

  if (m_DistriBindings->advanceToNextTimeAfter(CurrentDT,NextDT))
  {
    return Duration(NextDT.diffInSeconds(CurrentDT));
  }
  else
    return Never();
}


} } //namespaces


