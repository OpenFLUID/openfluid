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
  @file MultiInjectGenerator.cpp

  @author Armel THÖNI <armel.thoni@inrae.fr>
 */


#include <set>

#include <openfluid/machine/MultiInjectGenerator.hpp>
#include <openfluid/tools/ProgressiveChronFileReader.hpp>
#include <openfluid/tools/Filesystem.hpp>
#include <openfluid/tools/VarHelpers.hpp>


namespace openfluid { namespace machine {


MultiInjectGenerator::MultiInjectGenerator() : Generator(), m_DataFile("")
{
}


// =====================================================================
// =====================================================================


MultiInjectGenerator::~MultiInjectGenerator()
{
  if( m_DistriBindings != nullptr)
  {
    delete m_DistriBindings;
  }
}


// =====================================================================
// =====================================================================


void MultiInjectGenerator::initParams(const openfluid::ware::WareParams_t& Params)
{
  std::string DataFileGivenPath;
  if (OPENFLUID_GetSimulatorParameter(Params,"datafile",DataFileGivenPath))
  {
    if (std::filesystem::path(DataFileGivenPath).is_absolute())
    {
      m_DataFile = DataFileGivenPath;
    }
    else
    {
      std::string InputDir;
      OPENFLUID_GetRunEnvironment("dir.input",InputDir);
      m_DataFile = openfluid::tools::Filesystem::joinPath({InputDir, DataFileGivenPath});
    }
  }
  else
  {
    throw openfluid::base::FrameworkException(OPENFLUID_CODE_LOCATION,
                                              "missing data file value for generator");
  }
}


// =====================================================================
// =====================================================================


void MultiInjectGenerator::prepareData()
{
  try
  {
    m_DistriBindings = new openfluid::tools::MulticolDistributionBindings(m_DataFile, "%Y%m%dT%H%M%S", ";");
  }
  catch (openfluid::base::FrameworkException& E)
  {
    m_DistriBindings = nullptr; // TODO check if everything have been correctly deleted
    throw E;
  }

  for (const auto& Triplet : m_VarTriplets)
  {
    if (m_VarsByUnitsClass.find(Triplet.UnitsClass) == m_VarsByUnitsClass.end())
    {
      m_VarsByUnitsClass[Triplet.UnitsClass] = {};
    }
    m_VarsByUnitsClass[Triplet.UnitsClass].insert(Triplet.VariableName);
  }
}


// =====================================================================
// =====================================================================


void MultiInjectGenerator::checkConsistency()
{
  // checks that every variable in the selection is available in provided file
  openfluid::core::SpatialUnit* LU;
  for (const auto& VarByUnitClass : m_VarsByUnitsClass)
  {
    OPENFLUID_UNITS_ORDERED_LOOP(VarByUnitClass.first,LU)
    {
      for (const auto& Var : VarByUnitClass.second)
      {
        openfluid::tools::MulticolDistributionBindings::TripletLocation_t ColBySelectionTriplet = 
          m_DistriBindings->getColBySelectionTriplet();
        openfluid::tools::MulticolDistributionBindings::TripletLocation_t::iterator It = 
          ColBySelectionTriplet.find({VarByUnitClass.first, Var, std::to_string(LU->getID())});
        if (It == ColBySelectionTriplet.end())
        {
          It = ColBySelectionTriplet.find({VarByUnitClass.first, Var, "*"});
        }
        if (It == ColBySelectionTriplet.end())
        {
          throw openfluid::base::FrameworkException(OPENFLUID_CODE_LOCATION,
              "Variable from selection not found in data file: " + \
              VarByUnitClass.first+"#"+std::to_string(LU->getID())+":"+Var);
        }
      }
    }
  }
}


// =====================================================================
// =====================================================================


openfluid::base::SchedulingRequest MultiInjectGenerator::initializeRun()
{
  m_DistriBindings->advanceToTime(OPENFLUID_GetCurrentDate());
  openfluid::core::DoubleValue Value;
  openfluid::core::SpatialUnit* LU;
  openfluid::core::DateTime CurrentDT(OPENFLUID_GetCurrentDate());

  for (const auto& VarByUnitClass : m_VarsByUnitsClass)
  {
    OPENFLUID_UNITS_ORDERED_LOOP(VarByUnitClass.first,LU)
    {
      for (const auto& Var : VarByUnitClass.second)
      {
        if (!m_DistriBindings->getValue(VarByUnitClass.first, LU->getID(), Var, CurrentDT,Value))
        {
          Value = 0.0;
        }
        // TODO add vector case (cf InjectGenerator implem)
        OPENFLUID_InitializeVariable(LU,Var,Value);
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


// =====================================================================
// =====================================================================


openfluid::base::SchedulingRequest MultiInjectGenerator::runStep()
{
  m_DistriBindings->advanceToTime(OPENFLUID_GetCurrentDate());

  openfluid::core::DoubleValue Value;
  openfluid::core::SpatialUnit* LU;
  openfluid::core::DateTime CurrentDT(OPENFLUID_GetCurrentDate());

  for (const auto& VarByUnitClass : m_VarsByUnitsClass)
  {
    OPENFLUID_UNITS_ORDERED_LOOP(VarByUnitClass.first,LU)
    {
      for (const auto& Var : VarByUnitClass.second)
      {
        int Status = m_DistriBindings->getValue(VarByUnitClass.first, LU->getID(), Var, CurrentDT,Value);
        if (Status == 1)
        {
          OPENFLUID_AppendVariable(LU,Var,Value);
        }
        else if (Status == -1)
        {
          throw openfluid::base::FrameworkException(OPENFLUID_CODE_LOCATION, 
                "Value not injected since issue occured during multi-column injection: " + VarByUnitClass.first + \
                "#" + std::to_string(LU->getID()) + ":" + Var + " at time " + CurrentDT.getAsISOString());
        }
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

