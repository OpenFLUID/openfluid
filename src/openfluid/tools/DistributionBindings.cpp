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
  \file DistributionBindings.cpp
  \brief Implements ...

  \author Jean-Christophe FABRE <fabrejc@supagro.inra.fr>
 */

#include <openfluid/tools/DistributionBindings.hpp>


namespace openfluid { namespace tools {


DistributionBindings::DistributionBindings(const DistributionTables& DistriTables)
{
  DistributionTables::SourceIDFile_t::const_iterator itb = DistriTables.SourcesTable.begin();
  DistributionTables::SourceIDFile_t::const_iterator ite = DistriTables.SourcesTable.end();

  for (DistributionTables::SourceIDFile_t::const_iterator it = itb; it != ite; ++it)
  {
    ReaderNextValue RNV;
    RNV.Reader = new ProgressiveChronFileReader((*it).second);
    m_ReadersNextValues.push_back(RNV);

    DistributionTables::UnitIDSourceID_t::const_iterator itub = DistriTables.UnitsTable.begin();
    DistributionTables::UnitIDSourceID_t::const_iterator itue = DistriTables.UnitsTable.end();

    for (DistributionTables::UnitIDSourceID_t::const_iterator itu = itub; itu != itue; ++itu)
    {
      if ((*itu).second == (*it).first)
        m_UnitIDReaders[(*itu).first] = &m_ReadersNextValues.back();
    }
  }
}


// =====================================================================
// =====================================================================


DistributionBindings::~DistributionBindings()
{
  // delete readers

}


// =====================================================================
// =====================================================================


void DistributionBindings::advanceToTime(const openfluid::core::DateTime& DT)
{
  // set readers position to the first value equal or greater for each reader
  ReadersNextValues_t::iterator itb = m_ReadersNextValues.begin();
  ReadersNextValues_t::iterator ite = m_ReadersNextValues.end();

  for (ReadersNextValues_t::iterator it = itb; it != ite; ++it)
  {
    bool DataFound = true;
    openfluid::tools::ChronItem_t CI;

    if (((*it).isAvailable && (*it).NextValue.first < DT) ||
        (*it).isAvailable == false)
    {
      (*it).isAvailable = false;

      while (DataFound && !(*it).isAvailable)
      {
        DataFound = (*it).Reader->getNextValue(CI);
        if (DataFound && CI.first >= DT)
        {
          (*it).isAvailable = true;
          (*it).NextValue = CI;
        }
      }
    }
  }
}


// =====================================================================
// =====================================================================


bool DistributionBindings::advanceToNextTimeAfter(const openfluid::core::DateTime& DT, openfluid::core::DateTime& NextDT)
{
  openfluid::core::DateTime DTPlusOne(DT);
  DTPlusOne.addSeconds(1);
  advanceToTime(DTPlusOne);


  ReadersNextValues_t::iterator itb = m_ReadersNextValues.begin();
  ReadersNextValues_t::iterator ite = m_ReadersNextValues.end();

  openfluid::core::DateTime NDT;

  bool AvailableFound = false;

  for (ReadersNextValues_t::iterator it = itb; it != ite; ++it)
  {

    if (!AvailableFound && (*it).isAvailable)
    {
      NDT = (*it).NextValue.first;
      AvailableFound = true;
    }
  }

  if (!AvailableFound)
  {
    return false;
  }

  for (ReadersNextValues_t::iterator it = itb; it != ite; ++it)
  {
    if ((*it).isAvailable && (*it).NextValue.first < NDT)
      NDT = (*it).NextValue.first;
  }

  NextDT = NDT;

  return true;

}


// =====================================================================
// =====================================================================


bool DistributionBindings::getValue(const openfluid::core::UnitID_t& UnitID, const openfluid::core::DateTime& DT, openfluid::core::DoubleValue& Value)
{
  // if a value is available for DT : passes the value to caller, and returns true
  // else returns false
  UnitIDReader_t::iterator it = m_UnitIDReaders.find(UnitID);

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
  UnitIDReader_t::iterator itb = m_UnitIDReaders.begin();
  UnitIDReader_t::iterator ite = m_UnitIDReaders.end();

  for (UnitIDReader_t::iterator it = itb; it != ite; ++it)
  {
    std::cout << (*it).first << " -> " << (*it).second->Reader->getFileName() << std::endl;
  }


}


} }

