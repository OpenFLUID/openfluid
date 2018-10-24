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
  @file DatastoreItemDescriptor.hpp

  @author Aline LIBRES <libres@supagro.inra.fr>
  @author Jean-Christophe Fabre <jean-christophe.fabre@inra.fr>
*/


#ifndef __OPENFLUID_FLUIDX_DATASTOREITEMDESCRIPTOR_HPP__
#define __OPENFLUID_FLUIDX_DATASTOREITEMDESCRIPTOR_HPP__


#include <string>

#include <openfluid/dllexport.hpp>
#include <openfluid/core/UnstructuredValue.hpp>
#include <openfluid/core/TypeDefs.hpp>


namespace openfluid { namespace fluidx {


class OPENFLUID_API DatastoreItemDescriptor
{
  private:

    std::string m_ID;

    std::string m_PrefixPath;

    std::string m_RelativePath;

    openfluid::core::UnstructuredValue::UnstructuredType m_Type;

    std::string m_UnitsClass;

  public:

    DatastoreItemDescriptor() = delete;

    DatastoreItemDescriptor(const std::string& ID,
                            const std::string& PrefixPath, const std::string& RelativePath,
                            openfluid::core::UnstructuredValue::UnstructuredType Type);

    void setUnitsClass(const openfluid::core::UnitsClass_t& UnitsClass);

    /**
      @deprecated Since version 2.1.0.
      Use openfluid::fluidx::DatastoreItemDescriptor::setUnitsClass(const std::string&) instead
    */
    [[deprecated]] void setUnitClass(const std::string& UnitsClass)
    {
      setUnitsClass(UnitsClass);
    };

    openfluid::core::UnitsClass_t getUnitsClass() const;

    /**
      @deprecated Since version 2.1.0. Use openfluid::fluidx::DatastoreItemDescriptor::getUnitsClass() const instead
    */
    [[deprecated]] openfluid::core::UnitsClass_t getUnitClass() const
    {
      return getUnitsClass();
    };

    std::string getID() const;

    std::string getPrefixPath() const;

    std::string getRelativePath() const;

    openfluid::core::UnstructuredValue::UnstructuredType getType() const;
};


} } // namespaces


#endif /* __OPENFLUID_FLUIDX_DATASTOREITEMDESCRIPTOR_HPP__ */
