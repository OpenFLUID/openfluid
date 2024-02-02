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
  @file DistributionBindings.cpp

  @author Jean-Christophe FABRE <jean-christophe.fabre@inra.fr>
  @author Armel THÖNI <armel.thoni@inrae.fr>
 */


#include <openfluid/tools/DistributionBindings.hpp>


namespace openfluid { namespace tools {


DistributionBindings::DistributionBindings(const DistributionTables& DistriTables) : GenericDistributionBindings()
{
  {
    DistributionTables::SourceIDFile_t::const_iterator itb = DistriTables.SourcesTable.begin();
    DistributionTables::SourceIDFile_t::const_iterator ite = DistriTables.SourcesTable.end();

    for (DistributionTables::SourceIDFile_t::const_iterator it = itb; it != ite; ++it)
    {
      ReaderNextValue<double> RNV;
      RNV.Reader = new ProgressiveChronFileReader<double>((*it).second);
      m_ReadersNextValues.push_back(RNV);

      DistributionTables::UnitIDSourceID_t::const_iterator itub = DistriTables.UnitsTable.begin();
      DistributionTables::UnitIDSourceID_t::const_iterator itue = DistriTables.UnitsTable.end();

      for (DistributionTables::UnitIDSourceID_t::const_iterator itu = itub; itu != itue; ++itu)
      {
        if ((*itu).second == (*it).first)
        {
          m_UnitIDReaders[(*itu).first] = &m_ReadersNextValues.back();
        }
      }
    }
  }
}


// =====================================================================
// =====================================================================


bool DistributionBindings::getValue(const openfluid::core::UnitID_t& UnitID,
                                    const openfluid::core::DateTime& DT,
                                    openfluid::core::DoubleValue& Value)
{
  // if a value is available for DT : passes the value to caller, and returns true
  // else returns false
  typename UnitIDReader_t::iterator it = m_UnitIDReaders.find(UnitID);

  if (it != m_UnitIDReaders.end() && (*it).second->isAvailable && (*it).second->NextValue.first == DT)
  {
    Value = (*it).second->NextValue.second;
    return true;
  }

  return false;
}


// =====================================================================
// =====================================================================


void DistributionBindings::displayBindings()
{
  for (auto& IDReader : m_UnitIDReaders)
  {
    std::cout << IDReader.first << " -> " << IDReader.second->Reader->getFileName() << std::endl;
  }
}


// =====================================================================
// =====================================================================


MulticolDistributionBindings::MulticolDistributionBindings(const std::string& DataFile, 
                      const std::string& DateFormat,
                      const std::string& ColSeparators) : 
    GenericDistributionBindings()
{
  // check if file exists
  ReaderNextValue<std::vector<std::string>> RNV;
  try
  {
    RNV.Reader = new ProgressiveChronFileReader<std::vector<std::string>>(DataFile, DateFormat, ColSeparators);
  }
  catch (openfluid::base::FrameworkException& E)
  {
    RNV.Reader = nullptr;//TODO check if object correctly deleted
    throw E;
  }
  m_ReadersNextValues.push_back(RNV);

  std::string Header;
  RNV.Reader->getNextLine(Header);
  const auto& SelectionTriplets = stringSelectionToClassIDVarList(Header, true);
  unsigned int Col = 0;
  for (const auto& Triplet : SelectionTriplets)
  {
    m_ColBySelectionTriplets[{Triplet.UnitsClassesStr, Triplet.VariablesStr, Triplet.UnitsIDsStr}] = Col;
    Col++;
  }
}


// =====================================================================
// =====================================================================


bool MulticolDistributionBindings::getValue(const openfluid::core::UnitsClass_t& UnitsClass, 
                                    const openfluid::core::UnitID_t& UnitID, 
                                    const openfluid::core::VariableName_t& VariableName, 
                                    const openfluid::core::DateTime& DT,
                                    openfluid::core::DoubleValue& Value)
{
  // if a value is available for DT : passes the value to caller, and returns true
  // else returns false
  const auto& RNV = m_ReadersNextValues.front();

  if (RNV.isAvailable && RNV.NextValue.first == DT)
  {
    std::map<std::vector<std::string>, unsigned int>::iterator It = 
      m_ColBySelectionTriplets.find({UnitsClass, VariableName, std::to_string(UnitID)});
    if (It == m_ColBySelectionTriplets.end())
    {
      It = m_ColBySelectionTriplets.find({UnitsClass, VariableName, "*"});
    }
    if (It != m_ColBySelectionTriplets.end())
    {
      openfluid::core::StringValue(RNV.NextValue.second[(*It).second]).toDoubleValue(Value);
      return true;
    }
  }
  return false;
}


} }

