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



#include <openfluid/core/SpatialUnit.hpp>
#include <openfluid/core/UnitsCollection.hpp>



namespace openfluid { namespace core {



struct SortByProcessOrder
{
  bool operator ()(SpatialUnit& U1,SpatialUnit& U2) const
  {
    return (U1.getProcessOrder() <= U2.getProcessOrder());
  }

};


// =====================================================================
// =====================================================================


UnitsCollection::UnitsCollection()
{

}


// =====================================================================
// =====================================================================


UnitsCollection::~UnitsCollection()
{

}

// =====================================================================
// =====================================================================


SpatialUnit* UnitsCollection::spatialUnit(UnitID_t aUnitID)
{
  UnitsList_t::iterator it;

  for (it=m_Data.begin();it!=m_Data.end();++it)
  {
    if (it->getID() == aUnitID) return &(*it);
  }

  return NULL;
}


// =====================================================================
// =====================================================================



SpatialUnit* UnitsCollection::addSpatialUnit(SpatialUnit aUnit)
{

  if (spatialUnit(aUnit.getID()) == NULL)
  {
    m_Data.push_back(aUnit);
    return &(m_Data.back());
  }
  else return NULL;
}

// =====================================================================
// =====================================================================


void UnitsCollection::sortByProcessOrder()
{
  m_Data.sort(SortByProcessOrder());
}





} } // namespaces

