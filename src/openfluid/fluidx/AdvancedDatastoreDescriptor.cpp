/*
 This file is part of OpenFLUID software
 Copyright (c) 2007-2010 INRA-Montpellier SupAgro


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
 along with OpenFLUID.  If not, see <http://www.gnu.org/licenses/>.

 In addition, as a special exception, INRA gives You the additional right
 to dynamically link the code of OpenFLUID with code not covered
 under the GNU General Public License ("Non-GPL Code") and to distribute
 linked combinations including the two, subject to the limitations in this
 paragraph. Non-GPL Code permitted under this exception must only link to
 the code of OpenFLUID dynamically through the OpenFLUID libraries
 interfaces, and only for building OpenFLUID plugins. The files of
 Non-GPL Code may be link to the OpenFLUID libraries without causing the
 resulting work to be covered by the GNU General Public License. You must
 obey the GNU General Public License in all respects for all of the
 OpenFLUID code and other code used in conjunction with OpenFLUID
 except the Non-GPL Code covered by this exception. If you modify
 this OpenFLUID, you may extend this exception to your version of the file,
 but you are not obligated to do so. If you do not wish to provide this
 exception without modification, you must delete this exception statement
 from your version and license this OpenFLUID solely under the GPL without
 exception.


 == Other Usage ==

 Other Usage means a use of OpenFLUID that is inconsistent with the GPL
 license, and requires a written agreement between You and INRA.
 Licensees for Other Usage of OpenFLUID may use this file in accordance
 with the terms contained in the written agreement between You and INRA.
 */

/**
 \file AdvancedDatastoreDescriptor.cpp
 \brief Implements ...

 \author Aline LIBRES <aline.libres@gmail.com>
 */

#include "AdvancedDatastoreDescriptor.hpp"

#include <openfluid/fluidx/DatastoreDescriptor.hpp>
#include <openfluid/fluidx/DatastoreItemDescriptor.hpp>
#include <openfluid/base/OFException.hpp>

namespace openfluid {
namespace fluidx {

// =====================================================================
// =====================================================================

AdvancedDatastoreDescriptor::AdvancedDatastoreDescriptor(
    openfluid::fluidx::DatastoreDescriptor& DatastoreDesc) :
    mp_DatastoreDesc(&DatastoreDesc)
{

  // TODO check if DataStore Items are unique

}

// =====================================================================
// =====================================================================

AdvancedDatastoreDescriptor::~AdvancedDatastoreDescriptor()
{

}

// =====================================================================
// =====================================================================

const std::list<openfluid::fluidx::DatastoreItemDescriptor*>& AdvancedDatastoreDescriptor::getItems() const
{
  return mp_DatastoreDesc->getItems();
}

// =====================================================================
// =====================================================================

void AdvancedDatastoreDescriptor::moveItem(unsigned int From, unsigned int To)
{
  if (From == To)
    return;

  std::list<openfluid::fluidx::DatastoreItemDescriptor*>& Items =
      mp_DatastoreDesc->getItems();

  unsigned int Last = Items.size() - 1;

  if (From > Last || To > Last)
    throw openfluid::base::OFException("OpenFLUID Builder",
                                       "AdvancedDatastoreDescriptor::moveItem",
                                       "Bad indexes of items to move");

  std::list<openfluid::fluidx::DatastoreItemDescriptor*>::const_iterator itFrom =
      Items.begin();
  std::advance(itFrom, From);

  openfluid::fluidx::DatastoreItemDescriptor* Item = *itFrom;

  removeItem(From);

  if (To == Last)
    appendItem(Item);
  else
    insertItem(Item, To);
}

// =====================================================================
// =====================================================================

void AdvancedDatastoreDescriptor::appendItem(
    openfluid::fluidx::DatastoreItemDescriptor* Item)
{
  if (Item)
    mp_DatastoreDesc->appendItem(Item);
}

// =====================================================================
// =====================================================================

void AdvancedDatastoreDescriptor::insertItem(
    openfluid::fluidx::DatastoreItemDescriptor* Item, unsigned int Position)
{
  std::list<openfluid::fluidx::DatastoreItemDescriptor*>& Items =
      mp_DatastoreDesc->getItems();

  if (Position == 0)
    Items.insert(Items.begin(), Item);
  else if (Position < Items.size())
  {
    std::list<openfluid::fluidx::DatastoreItemDescriptor*>::iterator it =
        Items.begin();
    std::advance(it, Position);

    Items.insert(it, Item);
  }
  else
    throw openfluid::base::OFException(
        "OpenFLUID framework", "AdvancedDatastoreDescriptor::insertItem()",
        "Bad index of item to insert");
}

// =====================================================================
// =====================================================================

void AdvancedDatastoreDescriptor::removeItem(unsigned int Position)
{
  std::list<openfluid::fluidx::DatastoreItemDescriptor*>& Items =
      mp_DatastoreDesc->getItems();

  if (Position < Items.size())
  {
    std::list<openfluid::fluidx::DatastoreItemDescriptor*>::iterator it =
        Items.begin();
    std::advance(it, Position);

    Items.erase(it);
  }
  else
    throw openfluid::base::OFException(
        "OpenFLUID framework", "AdvancedDatastoreDescriptor::deleteItem()",
        "Bad index of item to delete");
}

// =====================================================================
// =====================================================================

}
} // namespaces
