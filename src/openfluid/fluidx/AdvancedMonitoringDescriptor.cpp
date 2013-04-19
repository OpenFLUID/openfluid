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
 \file AdvancedMonitoringDescriptor.cpp
 \brief Implements ...

 \author Aline LIBRES <aline.libres@gmail.com>
 */

#include <openfluid/fluidx/AdvancedMonitoringDescriptor.hpp>

#include <openfluid/fluidx/ObserverDescriptor.hpp>

namespace openfluid {
namespace fluidx {

// =====================================================================
// =====================================================================

AdvancedMonitoringDescriptor::AdvancedMonitoringDescriptor(
    openfluid::fluidx::MonitoringDescriptor& MonitoringDesc) :
    mp_MonitoringDesc(&MonitoringDesc)
{
}

// =====================================================================
// =====================================================================

AdvancedMonitoringDescriptor::~AdvancedMonitoringDescriptor()
{

}

// =====================================================================
// =====================================================================

const std::list<openfluid::fluidx::ObserverDescriptor*>& AdvancedMonitoringDescriptor::getItems() const
{
  return mp_MonitoringDesc->getItems();
}
// =====================================================================
// =====================================================================

openfluid::fluidx::ObserverDescriptor& AdvancedMonitoringDescriptor::getDescriptor(
    std::string ObserverID) const
{
  std::list<openfluid::fluidx::ObserverDescriptor*>& Observers =
      mp_MonitoringDesc->getItems();
  for (std::list<openfluid::fluidx::ObserverDescriptor*>::iterator it =
      Observers.begin(); it != Observers.end(); ++it)
  {
    if ((*it)->getID() == ObserverID)
      return **it;
  }

  throw openfluid::base::OFException(
      "OpenFLUID Framework", "AdvancedMonitoringDescriptor::getDescriptor",
      "Observer " + ObserverID + " is not in Observer list");
}

// =====================================================================
// =====================================================================

void AdvancedMonitoringDescriptor::addToObserverList(std::string ObserverID)
{
  openfluid::fluidx::ObserverDescriptor* Obs =
      new openfluid::fluidx::ObserverDescriptor(ObserverID);
  mp_MonitoringDesc->appendItem(Obs);
}

// =====================================================================
// =====================================================================

void AdvancedMonitoringDescriptor::removeFromObserverList(
    std::string ObserverID)
{
  openfluid::fluidx::ObserverDescriptor& Obs = getDescriptor(ObserverID);
  std::list<openfluid::fluidx::ObserverDescriptor*>& Observers =
      mp_MonitoringDesc->getItems();
  std::list<openfluid::fluidx::ObserverDescriptor*>::iterator it = std::find(
      Observers.begin(), Observers.end(), &Obs);
  if (it != Observers.end())
    Observers.erase(it);
}

// =====================================================================
// =====================================================================

void AdvancedMonitoringDescriptor::setItems(
    std::list<openfluid::fluidx::ObserverDescriptor*> ObserversList)
{
  mp_MonitoringDesc->getItems() = ObserversList;
}

// =====================================================================
// =====================================================================

void AdvancedMonitoringDescriptor::moveItemTowardsTheBeginning(
    std::string ObserverID)
{
  std::list<openfluid::fluidx::ObserverDescriptor*>& Observers =
      mp_MonitoringDesc->getItems();

  if (Observers.size() < 2)
    return;

  std::list<openfluid::fluidx::ObserverDescriptor*>::iterator itFrom =
      Observers.begin();

  for (std::list<openfluid::fluidx::ObserverDescriptor*>::iterator it =
      Observers.begin(); it != Observers.end(); ++it)
  {
    if ((*it)->getID() == ObserverID)
    {
      itFrom = it;
      break;
    }
  }

  if (itFrom == Observers.begin())
    return;

  std::list<openfluid::fluidx::ObserverDescriptor*>::iterator itTo = itFrom;
  itTo--;

  std::swap(*itFrom, *itTo);
}

// =====================================================================
// =====================================================================

void AdvancedMonitoringDescriptor::moveItemTowardsTheEnd(std::string ObserverID)
{
  std::list<openfluid::fluidx::ObserverDescriptor*>& Observers =
      mp_MonitoringDesc->getItems();

  if (Observers.size() < 2)
    return;

  std::list<openfluid::fluidx::ObserverDescriptor*>::iterator itFrom =
      Observers.end().operator --();

  for (std::list<openfluid::fluidx::ObserverDescriptor*>::iterator it =
      Observers.begin(); it != Observers.end(); ++it)
  {
    if ((*it)->getID() == ObserverID)
    {
      itFrom = it;
      break;
    }
  }

  if (itFrom == Observers.end().operator --())
    return;

  std::list<openfluid::fluidx::ObserverDescriptor*>::iterator itTo = itFrom;
  itTo++;

  std::swap(*itFrom, *itTo);
}

// =====================================================================
// =====================================================================

}
} //namespaces
