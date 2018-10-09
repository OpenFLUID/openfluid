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
  @file AttributesTableDescriptor.hpp

  @author Jean-Christophe FABRE <jean-christophe.fabre@inra.fr>
 */


#ifndef __OPENFLUID_FLUIDX_ATTRIBUTESTABLEDESCRIPTOR_HPP__
#define __OPENFLUID_FLUIDX_ATTRIBUTESTABLEDESCRIPTOR_HPP__


#include <openfluid/dllexport.hpp>
#include <openfluid/core/TypeDefs.hpp>


namespace openfluid { namespace fluidx {


class OPENFLUID_API AttributesTableDescriptor
{
  public:
    typedef std::map<openfluid::core::AttributeName_t,std::string> AttributeNameValue_t;

    typedef std::map<openfluid::core::UnitID_t,AttributeNameValue_t> UnitIDAttribute_t;


  private:

    openfluid::core::UnitsClass_t m_UnitsClass;

    std::vector<std::string> m_ColumnsOrder;

    UnitIDAttribute_t m_Data;


  public:

    AttributesTableDescriptor();

    ~AttributesTableDescriptor();

    void parseDataBlob(const std::string& Data);

    inline const openfluid::core::UnitsClass_t getUnitsClass() const
    {
      return m_UnitsClass;
    };

    inline void setUnitsClass(const openfluid::core::UnitsClass_t& UClass)
    {
      m_UnitsClass = UClass;
    };

    inline const std::vector<std::string>& columnsOrder() const
    {
      return m_ColumnsOrder;
    };

    inline void setColumnsOrder(const std::vector<std::string>& Cols)
    {
      m_ColumnsOrder = Cols;
    };

    inline const UnitIDAttribute_t& attributes() const
    {
      return m_Data;
    };

    inline UnitIDAttribute_t& attributes()
    {
      return m_Data;
    };

};


} } // namespaces


#endif /* __OPENFLUID_FLUIDX_ATTRIBUTESTABLEDESCRIPTOR_HPP__ */
