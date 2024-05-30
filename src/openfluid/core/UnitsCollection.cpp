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
  @file UnitsCollection.cpp

  @author Jean-Christophe Fabre <jean-christophe.fabre@inra.fr>
*/


#include <openfluid/core/SpatialUnit.hpp>
#include <openfluid/core/UnitsCollection.hpp>


namespace openfluid { namespace core {


struct SortByProcessOrder
{
  bool operator ()(SpatialUnit& U1,SpatialUnit& U2) const
  {
    // warning: does not apply specific order when units have identical order. 
    //   Based on spatial units injection LIFO (reversed): if domain contains SU 1,3,2,4, iterator will give 4,2,3,1
    return (U1.getProcessOrder() <= U2.getProcessOrder());
  }

};


// =====================================================================
// =====================================================================


SpatialUnit* UnitsCollection::spatialUnit(UnitID_t aUnitID)
{
  UnitsList_t::iterator it;

  for (it=m_Data.begin();it!=m_Data.end();++it)
  {
    if (it->getID() == aUnitID)
    {
      return &(*it);
    }
  }

  return nullptr;
}


// =====================================================================
// =====================================================================


const SpatialUnit* UnitsCollection::spatialUnit(UnitID_t aUnitID) const
{
  UnitsList_t::const_iterator it;

  for (it=m_Data.begin();it!=m_Data.end();++it)
  {
    if (it->getID() == aUnitID)
    {
      return &(*it);
    }
  }

  return nullptr;
}


// =====================================================================
// =====================================================================


SpatialUnit* UnitsCollection::addSpatialUnit(const SpatialUnit& aUnit)
{
  if (spatialUnit(aUnit.getID()) == nullptr)
  {
    m_Data.push_back(aUnit);
    return &(m_Data.back());
  }
  else
  {
    return nullptr;
  }
}


// =====================================================================
// =====================================================================


void UnitsCollection::sortByProcessOrder()
{
  m_Data.sort(SortByProcessOrder());
}


} } // namespaces

