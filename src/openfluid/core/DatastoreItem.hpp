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
  @file DatastoreItem.hpp

  @author Aline LIBRES <libres@supagro.inra.fr>
 */

#ifndef __OPENFLUID_CORE_DATASTOREITEM_HPP__
#define __OPENFLUID_CORE_DATASTOREITEM_HPP__

#include <openfluid/dllexport.hpp>
#include <openfluid/deprecation.hpp>

#include <string>

#include <openfluid/core/UnstructuredValue.hpp>


namespace openfluid { namespace core {


/**
  Item of a Datastore, giving information about an additional resource.
*/
class OPENFLUID_API DatastoreItem
{
  private:

    std::string m_ID;

    std::string m_PrefixPath;

    std::string m_RelativePath;

    std::string m_UnitsClass;

    UnstructuredValue* m_Value;

  public:

    /**
      @brief Creates a new resource item.

      Tries to create a new resource item and its associated UnstructuredValue,
      depending on the given type.

      @param ID The ID of the item.
      @param PrefixPath The prefix path of the item, usually the IN directory of the project.
      @param RelativePath The relative path of the item from the PrefixPath
      @param Type The type of the item.
      @param UnitsClass (optional) The associated unit class of the item.
      @throw openfluid::base::FrameworkException if the given type is unknown.
    */
    DatastoreItem(const std::string& ID,
                  const std::string& PrefixPath, const std::string& RelativePath,
                  UnstructuredValue::UnstructuredType Type, const std::string& UnitsClass = "");

    /**
      @brief Destroys the resource and its associated value.
    */
    ~DatastoreItem();

    std::string getID() const;

    /**
      @brief Returns the prefix path of the resource
    */
    std::string getPrefixPath() const;

    /**
      @brief Returns the path of the resource, relative to prefix path.
    */
    std::string getRelativePath() const;

    std::string getUnitsClass() const;

    /**
      @deprecated Since version 2.1.0. Use openfluid::core::DatastoreItem::getUnitsClass() instead
    */
    std::string getUnitClass() const OPENFLUID_DEPRECATED
    { return getUnitsClass(); }

    /**
      @brief Gets the associated value of the resource.
    */
    UnstructuredValue* value();

    /**
      @brief Gets the associated value of the resource.
    */
    const UnstructuredValue* value() const;
};


} } // namespaces


#endif /* __OPENFLUID_CORE_DATASTOREITEM_HPP__ */
