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
  @file

  @author Jean-Christophe FABRE <fabrejc@supagro.inra.fr>
 */


#ifndef __DOMAINDESCRIPTOR_HPP__
#define __DOMAINDESCRIPTOR_HPP__

#include <openfluid/dllexport.hpp>
#include <openfluid/fluidx/UnitDescriptor.hpp>
#include <openfluid/fluidx/AttributesDescriptor.hpp>
#include <openfluid/fluidx/EventDescriptor.hpp>


namespace openfluid { namespace fluidx {


class DLLEXPORT DomainDescriptor
{
  private:

    std::list<UnitDescriptor> m_DefDescriptor;

    std::list<AttributesDescriptor> m_AttrsDescriptor;

    std::list<EventDescriptor> m_EventsDescriptor;


  public:

    DomainDescriptor();

    ~DomainDescriptor();

    inline std::list<UnitDescriptor>& getUnits() { return m_DefDescriptor; };

    inline const std::list<UnitDescriptor>& getUnits() const { return m_DefDescriptor; };

    inline std::list<AttributesDescriptor>& getAttributes() { return m_AttrsDescriptor; };

    inline std::list<EventDescriptor>& getEvents() { return m_EventsDescriptor; };


};


} } // namespaces

#endif /* __DOMAINDESCRIPTOR_HPP__ */
