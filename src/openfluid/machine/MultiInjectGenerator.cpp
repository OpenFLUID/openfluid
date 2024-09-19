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
  if (OPENFLUID_GetWareParameter(Params,"datafile",DataFileGivenPath))
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
  openfluid::tools::MulticolDistributionBindings::TripletLocation_t ColBySelectionTriplet = 
    m_DistriBindings->getColBySelectionTriplet();
  
  for (const auto& Triplet : m_VarTriplets)
  {
    // Check if identified ID exists for class
    // Check if all units from class in file
    
    // No joker in csv
    if (ColBySelectionTriplet.find({Triplet.UnitsClass, Triplet.VariableName, 
          openfluid::config::CHAR_JOKER}) == ColBySelectionTriplet.end())
    {
      // checks if file contains all required ID for unit class
      std::vector<std::string> MissingUnits;

      bool FoundID = Triplet.UnitsIDsStr == openfluid::config::CHAR_JOKER;

      OPENFLUID_UNITS_ORDERED_LOOP(Triplet.UnitsClass, LU)
      {
        std::string CurrentIDStr = std::to_string(LU->getID());
        if (CurrentIDStr == Triplet.UnitsIDsStr)
        {
          FoundID = true;
        }

        if (ColBySelectionTriplet.find({Triplet.UnitsClass, Triplet.VariableName, 
              CurrentIDStr}) == ColBySelectionTriplet.end())
        {
          MissingUnits.push_back(CurrentIDStr);
        }
      }

      if (!FoundID)
      {
        throw openfluid::base::FrameworkException(OPENFLUID_CODE_LOCATION,
              "Unit ID from selection not found in spatial domain: " + \
              Triplet.UnitsClass + " " + Triplet.UnitsIDsStr);
      }


      if (MissingUnits.size() > 0)
      {
        throw openfluid::base::FrameworkException(OPENFLUID_CODE_LOCATION,
              "Variable from selection not found in data file: " + \
              Triplet.getClassIDVarString() + " for unit(s) " + openfluid::tools::join(MissingUnits, ", "));
      }
    }

    // Check if all units from class in selection
    //   check if joker in selection
    if (std::find(m_VarTriplets.begin(), m_VarTriplets.end(), openfluid::tools::ClassIDVar(Triplet.UnitsClass, 
          openfluid::config::CHAR_JOKER, Triplet.VariableName)) == m_VarTriplets.end())
    {
      std::vector<std::string> MissingUnits;
      // iterate over all units of class
      OPENFLUID_UNITS_ORDERED_LOOP(Triplet.UnitsClass, LU)
      {
        std::string CurrentIDStr = std::to_string(LU->getID());
        if (std::find(m_VarTriplets.begin(), m_VarTriplets.end(), openfluid::tools::ClassIDVar(Triplet.UnitsClass, 
              CurrentIDStr, Triplet.VariableName)) == m_VarTriplets.end())
        {
          MissingUnits.push_back(CurrentIDStr);
        }
      }
      if (MissingUnits.size() > 0)
      {
        throw openfluid::base::FrameworkException(OPENFLUID_CODE_LOCATION,
              "Unit from domain not found in selection for variable " + \
              Triplet.VariableName + " for unit(s) "+ Triplet.UnitsClass + " " + \
              openfluid::tools::join(MissingUnits, ", "));
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

