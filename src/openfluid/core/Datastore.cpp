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
 \file Datastore.cpp
 \brief Implements ...

 \author Aline LIBRES <libres@supagro.inra.fr>
 */

#include "Datastore.hpp"

#include <openfluid/core/DatastoreItem.hpp>

namespace openfluid {
namespace core {

// =====================================================================
// =====================================================================


Datastore::Datastore()
{
}

// =====================================================================
// =====================================================================


Datastore::~Datastore()
{
  for (DataItemsById_t::iterator it = m_ItemsById.begin(); it
      != m_ItemsById.end(); ++it)
    delete it->second;
}

// =====================================================================
// =====================================================================

Datastore::DataItemsById_t Datastore::getItems()
{
  return m_ItemsById;
}

// =====================================================================
// =====================================================================

const Datastore::DataItemsById_t Datastore::getItems() const
{
  return m_ItemsById;
}

// =====================================================================
// =====================================================================

DatastoreItem* Datastore::getItem(const std::string& ItemID)
{
  DataItemsById_t::iterator it = m_ItemsById.find(ItemID);

  if (it != m_ItemsById.end())
    return it->second;

  return (DatastoreItem*) 0;
}

// =====================================================================
// =====================================================================

void Datastore::addItem(const DatastoreItem* Item)
{
  DataItemsById_t::iterator it = m_ItemsById.find(Item->getID());

  if (it != m_ItemsById.end())
    delete it->second;

  m_ItemsById[Item->getID()] = const_cast<DatastoreItem*>(Item);
}

// =====================================================================
// =====================================================================

} } // namespaces
