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
  @file ObserverDescriptor.hpp

  @author Jean-Christophe FABRE <jean-christophe.fabre@inra.fr>
 */


#ifndef __OPENFLUID_FLUIDX_OBSERVERDESCRIPTOR_HPP__
#define __OPENFLUID_FLUIDX_OBSERVERDESCRIPTOR_HPP__

#include <openfluid/fluidx/WareDescriptor.hpp>


namespace openfluid { namespace fluidx {


class ObserverDescriptor : public WareDescriptor
{
  private:

    std::string m_ID;

  public:

    ObserverDescriptor(const std::string& ID) : WareDescriptor(), m_ID(ID)
    { m_WareType = openfluid::ware::WareType::OBSERVER; }

    std::string getID() const
    { return m_ID; }

};


} } // namespaces


#endif /* __OPENFLUID_FLUIDX_OBSERVERDESCRIPTOR_HPP__ */
