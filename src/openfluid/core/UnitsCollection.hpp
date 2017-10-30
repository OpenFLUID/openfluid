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
  @file UnitsCollection.hpp

  @author Jean-Christophe Fabre <jean-christophe.fabre@inra.fr>
*/


#ifndef __OPENFLUID_CORE_UNITSCOLLECTION_HPP__
#define __OPENFLUID_CORE_UNITSCOLLECTION_HPP__


#include <openfluid/dllexport.hpp>
#include <openfluid/core/TypeDefs.hpp>


namespace openfluid { namespace core {


class /*DLLEXPORT*/ SpatialUnit;

/**
  Type definition for a list of units
*/
typedef std::list<SpatialUnit> UnitsList_t;


class OPENFLUID_API UnitsCollection
{
  private :

    UnitsList_t m_Data;


  public :

    UnitsCollection();

    ~UnitsCollection();

    SpatialUnit* spatialUnit(UnitID_t aUnitID);

    const SpatialUnit* spatialUnit(UnitID_t aUnitID) const;

    SpatialUnit* addSpatialUnit(const SpatialUnit& aUnit);

    void sortByProcessOrder();

    inline const UnitsList_t* list() const
    { return &m_Data; };

    inline UnitsList_t* list()
    { return &m_Data; };

};

} } // namespaces


#endif /* __OPENFLUID_CORE_UNITSCOLLECTION_HPP__ */
