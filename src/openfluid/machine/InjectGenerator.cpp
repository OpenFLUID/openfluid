/*
  This file is part of OpenFLUID software
  Copyright (c) 2007-2010 INRA-Montpellier SupAgro


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
  along with OpenFLUID.  If not, see <http://www.gnu.org/licenses/>.

  In addition, as a special exception, INRA gives You the additional right
  to dynamically link the code of OpenFLUID with code not covered
  under the GNU General Public License ("Non-GPL Code") and to distribute
  linked combinations including the two, subject to the limitations in this
  paragraph. Non-GPL Code permitted under this exception must only link to
  the code of OpenFLUID dynamically through the OpenFLUID libraries
  interfaces, and only for building OpenFLUID plugins. The files of
  Non-GPL Code may be link to the OpenFLUID libraries without causing the
  resulting work to be covered by the GNU General Public License. You must
  obey the GNU General Public License in all respects for all of the
  OpenFLUID code and other code used in conjunction with OpenFLUID
  except the Non-GPL Code covered by this exception. If you modify
  this OpenFLUID, you may extend this exception to your version of the file,
  but you are not obligated to do so. If you do not wish to provide this
  exception without modification, you must delete this exception statement
  from your version and license this OpenFLUID solely under the GPL without
  exception.


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


#include <openfluid/machine/InjectGenerator.hpp>

#include <boost/math/special_functions/fpclassify.hpp>
#include <boost/filesystem/convenience.hpp>


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
    throw openfluid::base::FrameworkException("InjectGenerator::initParams","missing sources value for generator");

  if (!OPENFLUID_GetSimulatorParameter(Params,"distribution",m_DistriFile))
    throw openfluid::base::FrameworkException("InjectGenerator::initParams","missing distribution value for generator");


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
    throw openfluid::base::FrameworkException("InjectGenerator::checkConsistency","threshold max value must be greater or equal to threshold min value for generator");
}


// =====================================================================
// =====================================================================


openfluid::base::SchedulingRequest InjectGenerator::initializeRun()
{
  m_DistriBindings->advanceToTime(OPENFLUID_GetCurrentDate());
  openfluid::core::DoubleValue Value;
  openfluid::core::Unit* LU;
  openfluid::core::DateTime CurrentDT(OPENFLUID_GetCurrentDate());

  OPENFLUID_UNITS_ORDERED_LOOP(m_UnitClass,LU)
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
  openfluid::core::Unit* LU;
  openfluid::core::DateTime CurrentDT(OPENFLUID_GetCurrentDate());

  OPENFLUID_UNITS_ORDERED_LOOP(m_UnitClass,LU)
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


