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
  @file SpatialGraph.hpp

  @author Jean-Christophe Fabre <jean-christophe.fabre@supagro.inra.fr>
*/

#ifndef __OPENFLUID_CORE_SPATIALGRAPH_HPP__
#define __OPENFLUID_CORE_SPATIALGRAPH_HPP__


#include <openfluid/core/SpatialUnit.hpp>
#include <openfluid/dllexport.hpp>


namespace openfluid { namespace core {

class SpatialUnit;
class UnitsCollection;


class OPENFLUID_API SpatialGraph
{
  private:

    UnitsListByClassMap_t m_PcsOrderedUnitsByClass;

    UnitsPtrList_t m_PcsOrderedUnitsGlobal;

    static bool removeUnitFromList(UnitsPtrList_t* UnitsList,
                                   const UnitID_t& UnitID);

  public:

    SpatialGraph();

    bool addUnit(const SpatialUnit& aUnit);

    bool deleteUnit(SpatialUnit* aUnit);

    bool removeFromToConnection(SpatialUnit* FromUnit,
                                SpatialUnit* ToUnit);

    bool removeChildParentConnection(SpatialUnit* ChildUnit,
                                     SpatialUnit* ParentUnit);

    bool sortUnitsByProcessOrder();

    SpatialUnit* spatialUnit(const UnitsClass_t& UnitsClass, UnitID_t UnitID);

    const SpatialUnit* spatialUnit(const UnitsClass_t& UnitsClass, UnitID_t UnitID) const;

    UnitsCollection* spatialUnits(const UnitsClass_t& UnitsClass);

    const UnitsCollection* spatialUnits(const UnitsClass_t& UnitsClass) const;

    inline const UnitsListByClassMap_t* allSpatialUnitsByClass() const
    { return &m_PcsOrderedUnitsByClass; };

    inline const UnitsPtrList_t* allSpatialUnits() const
    { return &m_PcsOrderedUnitsGlobal; };

    inline UnitsPtrList_t* allSpatialUnits()
    { return &m_PcsOrderedUnitsGlobal; };

    bool isUnitsClassExist(const UnitsClass_t& UnitsClass) const;

    void streamContents(std::ostream& OStream);

    void clearAllVariables();

    void clearAllAttributes();

    void clearAllEvents();

    void clearAllData();

    void clearUnits();

};


} } // namespaces


#endif /* __OPENFLUID_CORE_SPATIALGRAPH_HPP__ */

