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
 \file WaresHelper.cpp
 \brief Implements ...

 \author Aline LIBRES <aline.libres@gmail.com>
 */

#include "WaresHelper.hpp"

#include <openfluid/machine/ObserverInstance.hpp>
#include <openfluid/fluidx/FunctionDescriptor.hpp>
#include <openfluid/fluidx/AdvancedMonitoringDescriptor.hpp>
#include <openfluid/fluidx/AdvancedModelDescriptor.hpp>
#include <openfluid/ware/ObserverSignatureRegistry.hpp>
#include <openfluid/ware/FunctionSignatureRegistry.hpp>

// =====================================================================
// =====================================================================

std::list<openfluid::fluidx::ModelItemDescriptor*> WaresHelper::checkAndGetModifiedModel(
    const openfluid::fluidx::AdvancedModelDescriptor& Desc,
    std::list<std::string>& MissingFunctions)
{
  openfluid::ware::FunctionSignatureRegistry* Reg =
      openfluid::ware::FunctionSignatureRegistry::getInstance();
  Reg->updatePluggableSignatures();

  std::list<openfluid::fluidx::ModelItemDescriptor*> Items = Desc.getItems();

  std::list<openfluid::fluidx::ModelItemDescriptor*>::iterator it =
      Items.begin();

  while (it != Items.end())
  {
    if ((*it)->isType(openfluid::fluidx::ModelItemDescriptor::PluggedFunction))
    {
      std::string ID =
          (dynamic_cast<openfluid::fluidx::FunctionDescriptor*>(*it))->getFileID();

      if (!Reg->isPluggableFunctionAvailable(ID))
      {
        MissingFunctions.push_back(ID);

        it = Items.erase(it);
      }
      else
        ++it;
    }
    else
      ++it;
  }

  return Items;
}

// =====================================================================
// =====================================================================

std::list<openfluid::fluidx::ObserverDescriptor*> WaresHelper::checkAndGetModifiedMonitoring(
    const openfluid::fluidx::AdvancedMonitoringDescriptor& Desc,
    std::list<std::string>& MissingObservers)
{
  openfluid::ware::ObserverSignatureRegistry* Reg =
      openfluid::ware::ObserverSignatureRegistry::getInstance();

  Reg->update();

  std::list<openfluid::fluidx::ObserverDescriptor*> Observers = Desc.getItems();

  std::list<openfluid::fluidx::ObserverDescriptor*>::iterator it =
      Observers.begin();

  while (it != Observers.end())
  {
    try
    {
      Reg->getSignature((*it)->getID());
      ++it;
    }
    catch (openfluid::base::OFException& e)
    {
      MissingObservers.push_back((*it)->getID());

      it = Observers.erase(it);
    }
  }

  return Observers;
}

// =====================================================================
// =====================================================================

std::vector<openfluid::machine::ObserverSignatureInstance*> WaresHelper::getUnusedAvailableObserverSignatures(
    const openfluid::fluidx::AdvancedMonitoringDescriptor& Desc)
{
  std::vector<openfluid::machine::ObserverSignatureInstance*> Signs;

  std::vector<openfluid::machine::ObserverSignatureInstance*> AvailableSignatures =
      openfluid::ware::ObserverSignatureRegistry::getInstance()->getAvailableSignatures();

  for (std::vector<openfluid::machine::ObserverSignatureInstance*>::iterator it =
      AvailableSignatures.begin(); it != AvailableSignatures.end(); ++it)
  {
    try
    {
      Desc.getDescriptor((*it)->Signature->ID);
    }
    catch (openfluid::base::OFException& e)
    {
      Signs.push_back(*it);
    }
  }

  return Signs;
}

// =====================================================================
// =====================================================================

