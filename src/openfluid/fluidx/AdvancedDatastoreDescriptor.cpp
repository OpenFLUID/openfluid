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
 @file AdvancedDatastoreDescriptor.cpp

 @author Aline LIBRES <aline.libres@gmail.com>
 */

#include "AdvancedDatastoreDescriptor.hpp"

#include <openfluid/fluidx/DatastoreDescriptor.hpp>
#include <openfluid/fluidx/DatastoreItemDescriptor.hpp>
#include <openfluid/base/FrameworkException.hpp>


namespace openfluid { namespace fluidx {


// =====================================================================
// =====================================================================


AdvancedDatastoreDescriptor::AdvancedDatastoreDescriptor(
    openfluid::fluidx::DatastoreDescriptor& DatastoreDesc) :
    mp_DatastoreDesc(&DatastoreDesc)
{

}


// =====================================================================
// =====================================================================


AdvancedDatastoreDescriptor::~AdvancedDatastoreDescriptor()
{

}


// =====================================================================
// =====================================================================


const std::list<openfluid::fluidx::DatastoreItemDescriptor*>& AdvancedDatastoreDescriptor::items() const
{
  return mp_DatastoreDesc->items();
}


// =====================================================================
// =====================================================================


std::list<openfluid::fluidx::DatastoreItemDescriptor*> AdvancedDatastoreDescriptor::getItems(const openfluid::core::UnitClass_t& UnitClass) const
{
  std::list<openfluid::fluidx::DatastoreItemDescriptor*>::iterator it;
  std::list<openfluid::fluidx::DatastoreItemDescriptor*>::iterator itb = mp_DatastoreDesc->items().begin();
  std::list<openfluid::fluidx::DatastoreItemDescriptor*>::iterator ite = mp_DatastoreDesc->items().end();

  std::list<openfluid::fluidx::DatastoreItemDescriptor*> ClassList;

  for (it = itb; it != ite; ++it)
  {
    if ((*it)->getUnitClass() == UnitClass)
      ClassList.push_back((*it));
  }

  return ClassList;
}


// =====================================================================
// =====================================================================


std::list<openfluid::fluidx::DatastoreItemDescriptor*> AdvancedDatastoreDescriptor::getItems(const openfluid::core::UnitClass_t& UnitClass,
                                                                                             openfluid::core::UnstructuredValue::UnstructuredType Type) const
{
  std::list<openfluid::fluidx::DatastoreItemDescriptor*>::iterator it;
  std::list<openfluid::fluidx::DatastoreItemDescriptor*>::iterator itb = mp_DatastoreDesc->items().begin();
  std::list<openfluid::fluidx::DatastoreItemDescriptor*>::iterator ite = mp_DatastoreDesc->items().end();

  std::list<openfluid::fluidx::DatastoreItemDescriptor*> ClassList;

  for (it = itb; it != ite; ++it)
  {
    if ((*it)->getUnitClass() == UnitClass && (*it)->getType() == Type)
      ClassList.push_back((*it));
  }

  return ClassList;
}


// =====================================================================
// =====================================================================


void AdvancedDatastoreDescriptor::moveItem(unsigned int From, unsigned int To)
{
  if (From == To)
    return;

  std::list<openfluid::fluidx::DatastoreItemDescriptor*>& Items =
      mp_DatastoreDesc->items();

  unsigned int Last = Items.size() - 1;

  if (From > Last || To > Last)
    throw openfluid::base::FrameworkException("AdvancedDatastoreDescriptor::moveItem",
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
      mp_DatastoreDesc->items();

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
    throw openfluid::base::FrameworkException("AdvancedDatastoreDescriptor::insertItem()",
                                              "Bad index of item to insert");
}


// =====================================================================
// =====================================================================


void AdvancedDatastoreDescriptor::removeItem(unsigned int Position)
{
  std::list<openfluid::fluidx::DatastoreItemDescriptor*>& Items =
      mp_DatastoreDesc->items();

  if (Position < Items.size())
  {
    std::list<openfluid::fluidx::DatastoreItemDescriptor*>::iterator it =
        Items.begin();
    std::advance(it, Position);

    Items.erase(it);
  }
  else
    throw openfluid::base::FrameworkException("AdvancedDatastoreDescriptor::deleteItem()",
                                              "Bad index of item to delete");
}


// =====================================================================
// =====================================================================


std::list<std::string> AdvancedDatastoreDescriptor::getItemsIDs() const
{
  std::list<std::string> IDsList;

  std::list<openfluid::fluidx::DatastoreItemDescriptor*>::iterator it;
  std::list<openfluid::fluidx::DatastoreItemDescriptor*>::iterator itb = mp_DatastoreDesc->items().begin();
  std::list<openfluid::fluidx::DatastoreItemDescriptor*>::iterator ite = mp_DatastoreDesc->items().end();

  for (it = itb; it != ite; ++it)
    IDsList.push_back((*it)->getID());

  return IDsList;
}


// =====================================================================
// =====================================================================


bool AdvancedDatastoreDescriptor::isItemAlreadyExist(std::string ItemID) const
{
  return mp_DatastoreDesc->isItemIDAlreadyExist(ItemID);
}


// =====================================================================
// =====================================================================


openfluid::fluidx::DatastoreItemDescriptor* AdvancedDatastoreDescriptor::item(const std::string& ItemID)
{

  std::list<openfluid::fluidx::DatastoreItemDescriptor*>::iterator it;
  std::list<openfluid::fluidx::DatastoreItemDescriptor*>::iterator itb = mp_DatastoreDesc->items().begin();
  std::list<openfluid::fluidx::DatastoreItemDescriptor*>::iterator ite = mp_DatastoreDesc->items().end();

  for (it = itb; it != ite; ++it)
  {
    if ((*it)->getID() == ItemID)
      return (*it) ;
  }

  return (openfluid::fluidx::DatastoreItemDescriptor*) 0;

}


// =====================================================================
// =====================================================================





} } // namespaces
