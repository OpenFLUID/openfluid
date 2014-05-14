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


#ifndef __UNITDESCRIPTOR_H___
#define __UNITDESCRIPTOR_H___

#include <openfluid/dllexport.hpp>
#include <openfluid/core/TypeDefs.hpp>


namespace openfluid { namespace fluidx {


class DLLEXPORT UnitDescriptor
{
  private:

    openfluid::core::UnitID_t m_UnitID;
    openfluid::core::UnitClass_t m_UnitClass;
    openfluid::core::PcsOrd_t m_PcsOrder;
    std::list<openfluid::core::UnitClassID_t> m_UnitsTos;
    std::list<openfluid::core::UnitClassID_t> m_UnitsParents;

  public:

   UnitDescriptor();

    ~UnitDescriptor();


    inline openfluid::core::UnitID_t& getUnitID() { return m_UnitID; };

    inline const openfluid::core::UnitID_t& getUnitID() const { return m_UnitID; };

    inline openfluid::core::UnitClass_t& getUnitClass() { return m_UnitClass; };

    inline openfluid::core::PcsOrd_t& getProcessOrder() { return m_PcsOrder; };

    inline const openfluid::core::PcsOrd_t& getProcessOrder() const { return m_PcsOrder; };

    inline std::list<openfluid::core::UnitClassID_t>& getUnitsTos() { return m_UnitsTos; };

    inline std::list<openfluid::core::UnitClassID_t>& getUnitsParents() { return m_UnitsParents; };

};


} } // namespaces


#endif /* __UNITDESCRIPTOR_HPP__ */
