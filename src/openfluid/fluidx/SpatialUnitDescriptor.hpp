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
  @file SpatialUnitDescriptor.hpp

  @author Jean-Christophe FABRE <jean-christophe.fabre@supagro.inra.fr>
 */


#ifndef __OPENFLUID_FLUIDX_SPATIALUNITDESCRIPTOR_HPP__
#define __OPENFLUID_FLUIDX_SPATIALUNITDESCRIPTOR_HPP__

#include <openfluid/dllexport.hpp>
#include <openfluid/core/TypeDefs.hpp>


namespace openfluid { namespace fluidx {


class OPENFLUID_API SpatialUnitDescriptor
{
  private:

    openfluid::core::UnitID_t m_UnitID;
    openfluid::core::UnitsClass_t m_UnitsClass;
    openfluid::core::PcsOrd_t m_PcsOrder;
    std::list<openfluid::core::UnitClassID_t> m_ToUnits;
    std::list<openfluid::core::UnitClassID_t> m_ParentUnits;

  public:

   SpatialUnitDescriptor();

    ~SpatialUnitDescriptor();


    inline openfluid::core::UnitID_t getID() const
    { return m_UnitID; };

    inline void setID(openfluid::core::UnitID_t ID)
    { m_UnitID = ID; };

    inline const openfluid::core::UnitsClass_t getUnitsClass() const
    { return m_UnitsClass; };

    inline void setUnitsClass(const openfluid::core::UnitsClass_t& UClass)
    { m_UnitsClass = UClass; };

    inline openfluid::core::PcsOrd_t getProcessOrder() const
    { return m_PcsOrder; };

    inline void setProcessOrder(openfluid::core::PcsOrd_t Order)
    { m_PcsOrder = Order; };

    inline std::list<openfluid::core::UnitClassID_t>& toSpatialUnits()
    { return m_ToUnits; };

    inline const std::list<openfluid::core::UnitClassID_t>& toSpatialUnits() const
    { return m_ToUnits; };

    inline std::list<openfluid::core::UnitClassID_t>& parentSpatialUnits()
    { return m_ParentUnits; };

    inline const std::list<openfluid::core::UnitClassID_t>& parentSpatialUnits() const
    { return m_ParentUnits; };
};


} } // namespaces


#endif /* __OPENFLUID_FLUIDX_SPATIALUNITDESCRIPTOR_HPP__ */
