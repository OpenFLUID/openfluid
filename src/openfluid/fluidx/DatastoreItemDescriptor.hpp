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
 \file DatastoreItemDescriptor.hpp
 \brief Header of ...

 \author Aline LIBRES <libres@supagro.inra.fr>
 */

#ifndef __DATASTOREITEMDESCRIPTOR_HPP__
#define __DATASTOREITEMDESCRIPTOR_HPP__

#include <openfluid/dllexport.hpp>

#include <string>

#include <openfluid/core/UnstructuredValue.hpp>


namespace openfluid {
namespace fluidx {

class DLLEXPORT DatastoreItemDescriptor
{
  private:

    std::string m_ID;

    std::string m_PrefixPath;

    std::string m_RelativePath;

    openfluid::core::UnstructuredValue::UnstructuredType m_Type;

    std::string m_UnitClass;

  public:

    DatastoreItemDescriptor(const std::string& ID, const std::string& PrefixPath, const std::string& RelativePath,
        openfluid::core::UnstructuredValue::UnstructuredType Type);

    void setUnitClass(const std::string& UnitClass);

    std::string getID() const;

    std::string getPrefixPath() const;

    std::string getRelativePath() const;

    openfluid::core::UnstructuredValue::UnstructuredType getType() const;

    std::string getUnitClass() const;
};

}
} // namespaces

#endif /* __DATASTOREITEMDESCRIPTOR_HPP__ */
