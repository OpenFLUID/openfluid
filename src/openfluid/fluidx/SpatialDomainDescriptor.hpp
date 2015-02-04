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
  @file SpatialDomainDescriptor.hpp

  @author Jean-Christophe FABRE <jean-christophe.fabre@supagro.inra.fr>
 */


#ifndef __OPENFLUID_FLUIDX_SPATIALDOMAINDESCRIPTOR_HPP__
#define __OPENFLUID_FLUIDX_SPATIALDOMAINDESCRIPTOR_HPP__

#include <openfluid/dllexport.hpp>
#include <openfluid/fluidx/AttributesDescriptor.hpp>
#include <openfluid/fluidx/EventDescriptor.hpp>
#include <openfluid/fluidx/SpatialUnitDescriptor.hpp>


namespace openfluid { namespace fluidx {


class OPENFLUID_API SpatialDomainDescriptor
{
  private:

    std::list<SpatialUnitDescriptor> m_DefDescriptor;

    std::list<AttributesDescriptor> m_AttrsDescriptor;

    std::list<EventDescriptor> m_EventsDescriptor;


  public:

    SpatialDomainDescriptor();

    ~SpatialDomainDescriptor();

    inline std::list<SpatialUnitDescriptor>& spatialUnits() { return m_DefDescriptor; };

    inline const std::list<SpatialUnitDescriptor>& spatialUnits() const { return m_DefDescriptor; };

    inline std::list<AttributesDescriptor>& attributes() { return m_AttrsDescriptor; };

    inline std::list<EventDescriptor>& events() { return m_EventsDescriptor; };


};


} } // namespaces

#endif /* __OPENFLUID_FLUIDX_SPATIALDOMAINDESCRIPTOR_HPP__ */
