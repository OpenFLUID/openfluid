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
 \file AdvancedModelDescriptor.cpp
 \brief Implements ...

 \author Aline LIBRES <aline.libres@gmail.com>
 */

#include <openfluid/fluidx/AdvancedModelDescriptor.hpp>

#include <openfluid/fluidx/WareSetDescriptor.hpp>
#include <openfluid/fluidx/FunctionDescriptor.hpp>
#include <openfluid/fluidx/GeneratorDescriptor.hpp>s

namespace openfluid {
namespace fluidx {

// =====================================================================
// =====================================================================

AdvancedModelDescriptor::AdvancedModelDescriptor(
    openfluid::fluidx::CoupledModelDescriptor& ModelDesc) :
    mp_ModelDesc(&ModelDesc)
{

}

// =====================================================================
// =====================================================================

AdvancedModelDescriptor::~AdvancedModelDescriptor()
{
}

// =====================================================================
// =====================================================================

const std::list<openfluid::fluidx::ModelItemDescriptor*>& AdvancedModelDescriptor::getItems() const
{
  return mp_ModelDesc->getItems();
}

// =====================================================================
// =====================================================================

openfluid::fluidx::ModelItemDescriptor* AdvancedModelDescriptor::getItemAt(
    unsigned int Index)
{
  std::list<openfluid::fluidx::ModelItemDescriptor*>& Items =
      mp_ModelDesc->getItems();

  if (Index < Items.size())
  {
    std::list<openfluid::fluidx::ModelItemDescriptor*>::iterator it =
        Items.begin();
    std::advance(it, Index);

    return *it;
  }
  else
    throw openfluid::base::OFException("OpenFLUID framework",
                                       "AdvancedModelDescriptor::getItemAt()",
                                       "Bad index of item to get");
}

// =====================================================================
// =====================================================================

int AdvancedModelDescriptor::getFirstItemIndex(std::string ItemID)
{
  std::list<openfluid::fluidx::ModelItemDescriptor*>& Items =
      mp_ModelDesc->getItems();

  for (std::list<openfluid::fluidx::ModelItemDescriptor*>::iterator it =
      Items.begin(); it != Items.end(); ++it)
  {
    if (getID(*it) == ItemID)
      return std::distance(Items.begin(), it);
  }

  return -1;
}

// =====================================================================
// =====================================================================

int AdvancedModelDescriptor::getFirstItemIndex(
    openfluid::fluidx::ModelItemDescriptor* Item)
{
  std::list<openfluid::fluidx::ModelItemDescriptor*>& Items =
      mp_ModelDesc->getItems();

  std::list<openfluid::fluidx::ModelItemDescriptor*>::iterator it = std::find(
      Items.begin(), Items.end(), Item);

  if (it != Items.end())
    return std::distance(Items.begin(), it);

  return -1;
}

// =====================================================================
// =====================================================================

std::vector<std::string> AdvancedModelDescriptor::getOrderedIDs()
{
  std::vector<std::string> IDs;

  std::list<openfluid::fluidx::ModelItemDescriptor*>& Items =
      mp_ModelDesc->getItems();

  for (std::list<openfluid::fluidx::ModelItemDescriptor*>::iterator it =
      Items.begin(); it != Items.end(); ++it)
    IDs.push_back(getID(*it));

  return IDs;
}

// =====================================================================
// =====================================================================

std::string AdvancedModelDescriptor::getID(
    openfluid::fluidx::ModelItemDescriptor* Item)
{
  if (Item->isType(openfluid::fluidx::WareDescriptor::PluggedFunction))
    return (dynamic_cast<openfluid::fluidx::FunctionDescriptor*>(Item))->getFileID();

  if (Item->isType(openfluid::fluidx::WareDescriptor::Generator))
    return (dynamic_cast<openfluid::fluidx::GeneratorDescriptor*>(Item))->getGeneratedID();
}

// =====================================================================
// =====================================================================

void AdvancedModelDescriptor::appendItem(
    openfluid::fluidx::ModelItemDescriptor* Item)
{
  if (Item)
    mp_ModelDesc->appendItem(Item);
}

// =====================================================================
// =====================================================================

void AdvancedModelDescriptor::insertItem(
    openfluid::fluidx::ModelItemDescriptor* Item, unsigned int Position)
{
  std::list<openfluid::fluidx::ModelItemDescriptor*>& Items =
      mp_ModelDesc->getItems();

  if (Position == 0)
    Items.insert(Items.begin(), Item);
  else if (Position < Items.size())
  {
    std::list<openfluid::fluidx::ModelItemDescriptor*>::iterator it =
        Items.begin();
    std::advance(it, Position);

    Items.insert(it, Item);
  }
  else
    throw openfluid::base::OFException("OpenFLUID framework",
                                       "AdvancedModelDescriptor::insertItem()",
                                       "Bad index of item to insert");
}

// =====================================================================
// =====================================================================

void AdvancedModelDescriptor::setItems(
    std::list<openfluid::fluidx::ModelItemDescriptor*> FunctionsList)
{
  mp_ModelDesc->getItems() = FunctionsList;
}

// =====================================================================
// =====================================================================

void AdvancedModelDescriptor::removeItem(unsigned int Position)
{
  std::list<openfluid::fluidx::ModelItemDescriptor*>& Items =
      mp_ModelDesc->getItems();

  if (Position < Items.size())
  {
    std::list<openfluid::fluidx::ModelItemDescriptor*>::iterator it =
        Items.begin();
    std::advance(it, Position);

    Items.erase(it);
  }
  else
    throw openfluid::base::OFException("OpenFLUID framework",
                                       "AdvancedModelDescriptor::deleteItem()",
                                       "Bad index of item to delete");
}

// =====================================================================
// =====================================================================

void AdvancedModelDescriptor::moveItem(unsigned int From, unsigned int To)
{
  if (From == To)
    return;

  std::list<openfluid::fluidx::ModelItemDescriptor*>& Items =
      mp_ModelDesc->getItems();

  unsigned int Last = Items.size() - 1;

  if (From > Last || To > Last)
    throw openfluid::base::OFException("OpenFLUID Builder",
                                       "AdvancedModelDescriptor::moveItem",
                                       "Bad indexes of items to move");

  std::list<openfluid::fluidx::ModelItemDescriptor*>::const_iterator itFrom =
      Items.begin();
  std::advance(itFrom, From);

  openfluid::fluidx::ModelItemDescriptor* Item = *itFrom;

  removeItem(From);

  if (To == Last)
    appendItem(Item);
  else
    insertItem(Item, To);
}

// =====================================================================
// =====================================================================

unsigned int AdvancedModelDescriptor::getItemsCount() const
{
  return mp_ModelDesc->getItems().size();
}

// =====================================================================
// =====================================================================

void AdvancedModelDescriptor::setGlobalParameter(
    const openfluid::ware::WareParamKey_t& Key,
    const openfluid::ware::WareParamKey_t& Value)
{
  mp_ModelDesc->setGlobalParameter(Key, Value);
}

// =====================================================================
// =====================================================================

void AdvancedModelDescriptor::setGlobalParameters(
    const openfluid::ware::WareParams_t& Params)
{
  mp_ModelDesc->setGlobalParameters(Params);
}

// =====================================================================
// =====================================================================

openfluid::ware::WareParams_t AdvancedModelDescriptor::getGlobalParameters()
{
  return mp_ModelDesc->getGlobalParameters();
}

// =====================================================================
// =====================================================================

void AdvancedModelDescriptor::eraseGlobalParameter(
    const openfluid::ware::WareParamKey_t& Key)
{
  mp_ModelDesc->eraseGlobalParameter(Key);
}

// =====================================================================
// =====================================================================

}
} // namespaces)
