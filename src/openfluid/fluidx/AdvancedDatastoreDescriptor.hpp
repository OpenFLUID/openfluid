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
  @file AdvancedDatastoreDescriptor.hpp

  @author Aline LIBRES <aline.libres@gmail.com>
 */

#ifndef __OPENFLUID_FLUIDX_ADVANCEDDATASTOREDESCRIPTOR_HPP__
#define __OPENFLUID_FLUIDX_ADVANCEDDATASTOREDESCRIPTOR_HPP__

#include <list>
#include <string>
#include <openfluid/dllexport.hpp>

#include <openfluid/core/UnstructuredValue.hpp>
#include <openfluid/core/TypeDefs.hpp>


namespace openfluid { namespace fluidx {

class DatastoreDescriptor;
class DatastoreItemDescriptor;

class OPENFLUID_API AdvancedDatastoreDescriptor
{
  private:

    openfluid::fluidx::DatastoreDescriptor* mp_DatastoreDesc;

  public:

    AdvancedDatastoreDescriptor(openfluid::fluidx::DatastoreDescriptor& DatastoreDesc);

    ~AdvancedDatastoreDescriptor();

    const std::list<openfluid::fluidx::DatastoreItemDescriptor*>& items() const;

    std::list<openfluid::fluidx::DatastoreItemDescriptor*>
      getItems(const openfluid::core::UnitClass_t& UnitClass) const;

    std::list<openfluid::fluidx::DatastoreItemDescriptor*>
      getItems(const openfluid::core::UnitClass_t& UnitClass,
               openfluid::core::UnstructuredValue::UnstructuredType Type) const;

    /**
     * Move the Item located at the given From position to the To position (positions starts at index 0)
     * @param From
     * @param To
     * @throw openfluid::base::OFException a position is out of range
     */
    void moveItem(unsigned int From, unsigned int To);

    void appendItem(openfluid::fluidx::DatastoreItemDescriptor* Item);

    /**
     Insert an Item before the given position (positions starts at index 0)
     @param[in] Item the DatastoreItemDescriptor to insert
     @param[in] Position the position, should be between zero and list size - 1.
     To insert an Item at the end of the list, use appendItem instead.
     @throw openfluid::base::OFException if Position is out of range
     */
    void insertItem(openfluid::fluidx::DatastoreItemDescriptor* Item,
                    unsigned int Position);

    /**
     Remove from the list the Item located at the given Position (positions starts at index 0).
     This doesn't delete the DatastoreItemDescriptor pointer.
     @param[in] Position the position
     @throw openfluid::base::OFException if Position is out of range
     */
    void removeItem(unsigned int Position);

    std::list<std::string> getItemsIDs() const;

    bool isItemAlreadyExist(std::string ItemID) const;

    /**
     * @brief Gets the item of the datastore matching the given ID.
     *
     * @param ItemID ID of the expected item.
     * @return The item with the given ID, or 0 if not found.
     */
    openfluid::fluidx::DatastoreItemDescriptor* item(const std::string& ItemID);



};

} } // namespaces

#endif /* __OPENFLUID_FLUIDX_ADVANCEDDATASTOREDESCRIPTOR_HPP__ */
