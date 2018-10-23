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
  @file Datastore.hpp

  @author Aline LIBRES <libres@supagro.inra.fr>
*/


#ifndef __OPENFLUID_CORE_DATASTORE_HPP__
#define __OPENFLUID_CORE_DATASTORE_HPP__


#include <map>
#include <string>

#include <openfluid/dllexport.hpp>


namespace openfluid { namespace core {


class DatastoreItem;

/**
  Container class for holding information about additional resources.
*/
class OPENFLUID_API Datastore
{
  public:

   /**
     Type to store items indexed by their ID
   */
   typedef std::map<std::string, DatastoreItem*> DataItemsById_t;


  private:

    DataItemsById_t m_ItemsById;

  public:

    /**
      Creates an empty datastore.
    */
    Datastore() = default;

    /**
      Destroys all items of the datastore.
    */
    ~Datastore();

    /**
      Gets all items of the datastore.
     @return A map of all items of the datastore.
    */
    DataItemsById_t getItems();

    /**
      Gets all items of the datastore.
      @return A const map of all items of the datastore.
    */
    const DataItemsById_t getItems() const;

    /**
      Gets the item of the datastore matching the given ID.
      @param[in] ItemID ID of the expected item.
      @return The item with the given ID, or 0 if not found.
    */
    DatastoreItem* item(const std::string& ItemID);

    /**
     Adds an item to the datastore.
     If an item already exists with the ID of the given <tt>Item</tt>,it is deleted before adding.

     @param[in] Item The item to add.
    */
    void addItem(const DatastoreItem* Item);
};


} } // namespaces


#endif /* __OPENFLUID_CORE_DATASTORE_HPP__ */
