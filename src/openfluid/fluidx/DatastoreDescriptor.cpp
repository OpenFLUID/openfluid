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
  @file DatastoreDescriptor.cpp

  @author Jean-Christophe FABRE <jean-christophe.fabre@inra.fr>
  @author Aline LIBRES <aline.libres@gmail.com>
 */


#include "DatastoreDescriptor.hpp"


namespace openfluid { namespace fluidx {


DatastoreDescriptor::DatastoreDescription_t& DatastoreDescriptor::items()
{
  return m_DatastoreDescription;
}


// =====================================================================
// =====================================================================


const DatastoreDescriptor::DatastoreDescription_t& DatastoreDescriptor::items() const
{
  return m_DatastoreDescription;
}


// =====================================================================
// =====================================================================


bool DatastoreDescriptor::appendItem(DatastoreItemDescriptor* Item)
{
  if (isItemExist(Item->getID()))
  {
    return false;
  }

  m_DatastoreDescription.push_back(Item);
  return true;
}


// =====================================================================
// =====================================================================


bool DatastoreDescriptor::isItemExist(const std::string& ItemID) const
{
  for (DatastoreDescription_t::const_iterator it = m_DatastoreDescription.begin();
       it != m_DatastoreDescription.end(); ++it)
  {
    if ((*it)->getID() == ItemID)
    {
      return true;
    }
  }

  return false;
}


// =====================================================================
// =====================================================================


DatastoreDescriptor::DatastoreDescription_t DatastoreDescriptor::getItems(
  const openfluid::core::UnitsClass_t& UnitClass) const
{
  DatastoreDescription_t::const_iterator it;
  DatastoreDescription_t::const_iterator itb = items().begin();
  DatastoreDescription_t::const_iterator ite = items().end();

  DatastoreDescription_t ClassList;

  for (it = itb; it != ite; ++it)
  {
    if ((*it)->getUnitsClass() == UnitClass)
    {
      ClassList.push_back((*it));
    }
  }

  return ClassList;
}


// =====================================================================
// =====================================================================


DatastoreDescriptor::DatastoreDescription_t DatastoreDescriptor::getItems(
    const openfluid::core::UnitsClass_t& UnitClass,
    openfluid::core::UnstructuredValue::UnstructuredType Type) const
{
  DatastoreDescription_t::const_iterator it;
  DatastoreDescription_t::const_iterator itb = items().begin();
  DatastoreDescription_t::const_iterator ite = items().end();

  DatastoreDescription_t ClassList;

  for (it = itb; it != ite; ++it)
  {
    if ((*it)->getUnitsClass() == UnitClass && (*it)->getType() == Type)
    {
      ClassList.push_back((*it));
    }
  }

  return ClassList;
}


// =====================================================================
// =====================================================================


void DatastoreDescriptor::moveItem(unsigned int From, unsigned int To)
{
  if (From == To)
  {
    return;
  }

  std::list<openfluid::fluidx::DatastoreItemDescriptor*>& Items = items();

  unsigned int Last = Items.size() - 1;

  if (From > Last || To > Last)
  {
    throw openfluid::base::FrameworkException(OPENFLUID_CODE_LOCATION,"Bad indexes of items to move");
  }

  DatastoreDescription_t::const_iterator itFrom = Items.begin();
  std::advance(itFrom, From);

  openfluid::fluidx::DatastoreItemDescriptor* Item = *itFrom;

  removeItem(From);

  if (To == Last)
  {
    appendItem(Item);
  }
  else
  {
    insertItem(Item, To);
  }
}


// =====================================================================
// =====================================================================


void DatastoreDescriptor::insertItem(openfluid::fluidx::DatastoreItemDescriptor* Item, unsigned int Position)
{
  DatastoreDescription_t& Items = items();

  if (Position == 0)
  {
    Items.insert(Items.begin(), Item);
  }
  else if (Position < Items.size())
  {
    std::list<openfluid::fluidx::DatastoreItemDescriptor*>::iterator it =
        Items.begin();
    std::advance(it, Position);

    Items.insert(it, Item);
  }
  else
  {
    throw openfluid::base::FrameworkException(OPENFLUID_CODE_LOCATION, "Bad index of item to insert");
  }
}


// =====================================================================
// =====================================================================


void DatastoreDescriptor::removeItem(unsigned int Position)
{
  DatastoreDescription_t& Items = items();

  if (Position < Items.size())
  {
    std::list<openfluid::fluidx::DatastoreItemDescriptor*>::iterator it = Items.begin();
    std::advance(it, Position);

    Items.erase(it);
  }
  else
  {
    throw openfluid::base::FrameworkException(OPENFLUID_CODE_LOCATION, "Bad index of item to delete");
  }
}


// =====================================================================
// =====================================================================


std::list<std::string> DatastoreDescriptor::getItemsIDs() const
{
  std::list<std::string> IDsList;

  std::list<openfluid::fluidx::DatastoreItemDescriptor*>::const_iterator it;
  std::list<openfluid::fluidx::DatastoreItemDescriptor*>::const_iterator itb = items().begin();
  std::list<openfluid::fluidx::DatastoreItemDescriptor*>::const_iterator ite = items().end();

  for (it = itb; it != ite; ++it)
  {
    IDsList.push_back((*it)->getID());
  }

  return IDsList;
}


// =====================================================================
// =====================================================================


openfluid::fluidx::DatastoreItemDescriptor* DatastoreDescriptor::item(const std::string& ItemID)
{
  std::list<openfluid::fluidx::DatastoreItemDescriptor*>::iterator it;
  std::list<openfluid::fluidx::DatastoreItemDescriptor*>::iterator itb = items().begin();
  std::list<openfluid::fluidx::DatastoreItemDescriptor*>::iterator ite = items().end();

  for (it = itb; it != ite; ++it)
  {
    if ((*it)->getID() == ItemID)
    {
      return (*it);
    }
  }

  return nullptr;
}


}  } // namespaces
