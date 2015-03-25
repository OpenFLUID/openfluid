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
  @file EventDescriptor.hpp

  @author Jean-Christophe FABRE <jean-christophe.fabre@supagro.inra.fr>
 */


#ifndef __OPENFLUID_FLUIDX_EVENTDESCRIPTOR_HPP__
#define __OPENFLUID_FLUIDX_EVENTDESCRIPTOR_HPP__

#include <openfluid/dllexport.hpp>
#include <openfluid/core/TypeDefs.hpp>
#include <openfluid/core/Event.hpp>


namespace openfluid { namespace fluidx {

typedef unsigned long long EventID_t;

class OPENFLUID_API EventDescriptor
{
  private:

    EventID_t m_ID;

    openfluid::core::UnitsClass_t m_UnitsClass;

    openfluid::core::UnitID_t m_UnitID;

    openfluid::core::Event m_Event;

    static EventID_t m_NextID;

    static EventID_t assignID();


  public:

    EventDescriptor();

    EventDescriptor(const EventDescriptor& EvDesc);

    ~EventDescriptor();

    inline openfluid::core::UnitID_t getUnitID() const
    { return m_UnitID; };

    inline void setUnitID(openfluid::core::UnitID_t ID)
    { m_UnitID = ID; };

    inline const openfluid::core::UnitsClass_t getUnitsClass() const
    { return m_UnitsClass; };

    /**
      @deprecated Since version 2.1.0. Use openfluid::fluidx::EventDescriptor::getUnitsClass() const instead
    */
    inline const openfluid::core::UnitsClass_t getUnitClass() const OPENFLUID_DEPRECATED
    { return m_UnitsClass; };

    inline void setUnitsClass(const openfluid::core::UnitsClass_t& UClass)
    { m_UnitsClass = UClass; }

    /**
      @deprecated Since version 2.1.0.
      Use openfluid::fluidx::EventDescriptor::setUnitsClass(const openfluid::core::UnitsClass_t&) instead
    */
    inline void setUnitClass(const openfluid::core::UnitsClass_t& UClass) OPENFLUID_DEPRECATED
    { m_UnitsClass = UClass; };

    inline openfluid::core::Event& event()
    { return m_Event; };

    inline EventID_t getID() const
    { return m_ID; };

};


} } // namespaces

#endif /* __OPENFLUID_FLUIDX_EVENTDESCRIPTOR_HPP__ */
