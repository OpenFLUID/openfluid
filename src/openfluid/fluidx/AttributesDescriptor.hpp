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


#ifndef __ATTRIBUTESDESCRIPTOR_HPP__
#define __ATTRIBUTESDESCRIPTOR_HPP__

#include <openfluid/dllexport.hpp>
#include <openfluid/core/TypeDefs.hpp>

namespace openfluid { namespace fluidx {


class DLLEXPORT AttributesDescriptor
{
  public:
    typedef std::map<openfluid::core::AttributeName_t,std::string> AttributeNameValue_t;

    typedef std::map<openfluid::core::UnitID_t,AttributeNameValue_t> UnitIDAttribute_t;


  private:

    openfluid::core::UnitClass_t m_UnitsClass;

    std::vector<std::string> m_ColumnsOrder;

    UnitIDAttribute_t m_Data;


  public:

    AttributesDescriptor();

    ~AttributesDescriptor();

    void parseDataBlob(const std::string& Data);

    inline openfluid::core::UnitClass_t& getUnitsClass() { return m_UnitsClass; };

    inline std::vector<std::string>& getColumnsOrder() { return m_ColumnsOrder; };

    inline const UnitIDAttribute_t& getAttributes() const { return m_Data; };

    inline UnitIDAttribute_t& getAttributes() { return m_Data; };

};


} } // namespaces


#endif /* __ATTRIBUTESDESCRIPTOR_HPP__ */
