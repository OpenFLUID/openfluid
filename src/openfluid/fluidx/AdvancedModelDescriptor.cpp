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
 \file AdvancedModelDescriptor.cpp
 \brief Implements ...

 \author Aline LIBRES <aline.libres@gmail.com>
 */

#include <openfluid/fluidx/AdvancedModelDescriptor.hpp>

#include <set>
#include <openfluid/fluidx/WareSetDescriptor.hpp>
#include <openfluid/fluidx/SimulatorDescriptor.hpp>
#include <openfluid/fluidx/GeneratorDescriptor.hpp>

namespace openfluid {
namespace fluidx {

// =====================================================================
// =====================================================================

AdvancedModelDescriptor::AdvancedModelDescriptor(
    openfluid::fluidx::CoupledModelDescriptor& ModelDesc) :
    mp_ModelDesc(&ModelDesc)
{
  checkModel();
}

// =====================================================================
// =====================================================================

AdvancedModelDescriptor::~AdvancedModelDescriptor()
{
}

// =====================================================================
// =====================================================================

void AdvancedModelDescriptor::checkModel() const
{
  std::set<std::string> UniqueIDs;

  std::list<openfluid::fluidx::ModelItemDescriptor*>& Items =
      mp_ModelDesc->getItems();

  for (std::list<openfluid::fluidx::ModelItemDescriptor*>::iterator it =
      Items.begin(); it != Items.end(); ++it)
  {
    std::string ID = getID(*it);
    if (!UniqueIDs.insert(ID).second)
      throw openfluid::base::FrameworkException(
          "AdvancedModelDescriptor::checkModel",
          "The simulator with ID \"" + ID + "\" is duplicate");
  }
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
    unsigned int Index) const
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
    throw openfluid::base::FrameworkException(
                                       "AdvancedModelDescriptor::getItemAt()",
                                       "Bad index of item to get");
}

// =====================================================================
// =====================================================================

int AdvancedModelDescriptor::getFirstItemIndex(std::string ItemID) const
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
    openfluid::fluidx::ModelItemDescriptor* Item) const
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

std::vector<std::string> AdvancedModelDescriptor::getOrderedIDs() const
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
    openfluid::fluidx::ModelItemDescriptor* Item) const
{
  if (Item->isType(openfluid::fluidx::WareDescriptor::PluggedSimulator))
    return (dynamic_cast<openfluid::fluidx::SimulatorDescriptor*>(Item))->getFileID();

  if (Item->isType(openfluid::fluidx::WareDescriptor::Generator))
    return (dynamic_cast<openfluid::fluidx::GeneratorDescriptor*>(Item))->getGeneratedID();

  throw openfluid::base::FrameworkException(
                                         "AdvancedModelDescriptor::getID()",
                                         "Unknown ware type");
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
    throw openfluid::base::FrameworkException(
                                       "AdvancedModelDescriptor::insertItem()",
                                       "Bad index of item to insert");
}

// =====================================================================
// =====================================================================

void AdvancedModelDescriptor::setItems(
    std::list<openfluid::fluidx::ModelItemDescriptor*> SimulatorsList)
{
  mp_ModelDesc->getItems() = SimulatorsList;
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
    throw openfluid::base::FrameworkException(
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
    throw openfluid::base::FrameworkException(
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


void AdvancedModelDescriptor::getItemsCount(unsigned int& SimCount, unsigned int& GenCount) const
{
  SimCount = 0;
  GenCount = 0;

  std::list<openfluid::fluidx::ModelItemDescriptor*>& Items =
      mp_ModelDesc->getItems();

  for (std::list<openfluid::fluidx::ModelItemDescriptor*>::iterator it =
      Items.begin(); it != Items.end(); ++it)
  {
    if ((*it)->getType() == openfluid::fluidx::WareDescriptor::PluggedSimulator)
      SimCount++;

    if ((*it)->getType() == openfluid::fluidx::WareDescriptor::Generator)
          GenCount++;
  }
}


// =====================================================================
// =====================================================================


void AdvancedModelDescriptor::setGlobalParameter(
    const openfluid::ware::WareParamKey_t& Key,
    const openfluid::ware::WareParamValue_t& Value)
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


openfluid::ware::WareParams_t AdvancedModelDescriptor::getGlobalParameters() const
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

}  } // namespaces)
