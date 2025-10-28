/*

  This file is part of OpenFLUID software
  Copyright(c) 2021-2026, INRAE


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

  @author Jean-Christophe FABRE <jean-christophe.fabre@inra.fr>
  @author Armel THÖNI <armel.thoni@inrae.fr>
 */


#include <openfluid/machine/InjectGenerator.hpp>


namespace openfluid { namespace machine {

  
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

      if (m_IsMax && Value > m_Max)
      {
        Value = m_Max;
      }
      if (m_IsMin && Value < m_Min)
      {
        Value = m_Min;
      }
    }
    else
    {
      Value = 0.0;
    }

    if (m_VarDimensions.isVector())
    {
      openfluid::core::VectorValue VV(m_VarDimensions.Rows,Value);
      OPENFLUID_InitializeVariable(LU,m_VarName,VV);
    }
    else
    {
      OPENFLUID_InitializeVariable(LU,m_VarName,Value);
    }
  }

  openfluid::core::DateTime NextDT;

  if (m_DistriBindings->advanceToNextTimeAfter(CurrentDT,NextDT))
  {
    return Duration(NextDT.diffInSeconds(CurrentDT));
  }
  else
  {
    return Never();
  }
}


// =====================================================================
// =====================================================================


openfluid::base::SchedulingRequest InjectGenerator::runStep()
{
  // adapted to handle vector of double
  m_DistriBindings->advanceToTime(OPENFLUID_GetCurrentDate());

  openfluid::core::DoubleValue Value;
  openfluid::core::SpatialUnit* LU;
  openfluid::core::DateTime CurrentDT(OPENFLUID_GetCurrentDate());

  OPENFLUID_UNITS_ORDERED_LOOP(m_UnitsClass,LU)
  {
    if (m_DistriBindings->getValue(LU->getID(),CurrentDT,Value))
    {
      if (m_IsMax && Value > m_Max)
      {
        Value = m_Max;
      }
      if (m_IsMin && Value < m_Min)
      {
        Value = m_Min;
      }
      if (m_VarDimensions.isVector())
      {
        openfluid::core::VectorValue VV(m_VarDimensions.Rows,Value);
        OPENFLUID_AppendVariable(LU,m_VarName,VV);
      }
      else
      {
        OPENFLUID_AppendVariable(LU,m_VarName,Value);
      }
    }
  }

  openfluid::core::DateTime NextDT;

  if (m_DistriBindings->advanceToNextTimeAfter(CurrentDT,NextDT))
  {
    return Duration(NextDT.diffInSeconds(CurrentDT));
  }
  else
  {
    return Never();
  }
}


} } //namespaces

