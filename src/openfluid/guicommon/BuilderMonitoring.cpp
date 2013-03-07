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
 \file BuilderMonitoring.cpp
 \brief Implements ...

 \author Aline LIBRES <aline.libres@gmail.com>
 */

#include "BuilderMonitoring.hpp"

#include <openfluid/machine/ObserverPluginsManager.hpp>
#include <openfluid/machine/ObserverInstance.hpp>
#include <openfluid/fluidx/ObserverDescriptor.hpp>

namespace openfluid {
namespace guicommon {

// =====================================================================
// =====================================================================

BuilderMonitoring::BuilderMonitoring(
    openfluid::fluidx::MonitoringDescriptor& MonitoringDesc) :
    mp_MonitoringDesc(&MonitoringDesc)
{
  mp_Manager = openfluid::machine::ObserverPluginsManager::getInstance();
}

// =====================================================================
// =====================================================================

BuilderMonitoring::~BuilderMonitoring()
{

}

// =====================================================================
// =====================================================================

std::string BuilderMonitoring::checkAndAdaptMonitoring()
{
  mp_Manager->unloadAllWares();
  m_AvailableSignatures = mp_Manager->getAvailableWaresSignatures();

  std::list<openfluid::fluidx::ObserverDescriptor*>& Observers =
      mp_MonitoringDesc->getItems();

  std::string MissingObservers = "";

  std::list<openfluid::fluidx::ObserverDescriptor*>::iterator it =
      Observers.begin();

  while (it != Observers.end())
  {
    try
    {
      getSignature((*it)->getID());
      ++it;
    }
    catch (openfluid::base::OFException& e)
    {
      MissingObservers.append("- " + (*it)->getID() + "\n");

      it = Observers.erase(it);
    }
  }

  return MissingObservers;
}

// =====================================================================
// =====================================================================

const std::list<openfluid::fluidx::ObserverDescriptor*>& BuilderMonitoring::getItems()
{
  return mp_MonitoringDesc->getItems();
}
// =====================================================================
// =====================================================================

const openfluid::machine::ObserverSignatureInstance& BuilderMonitoring::getSignature(
    std::string ObserverID)
{
  for (std::vector<openfluid::machine::ObserverSignatureInstance*>::iterator it =
      m_AvailableSignatures.begin(); it != m_AvailableSignatures.end(); ++it)
  {
    if ((*it)->Signature->ID == ObserverID)
      return **it;
  }

  throw openfluid::base::OFException(
      "OpenFLUID Framework", "BuilderMonitoring::getSignature",
      "Observer " + ObserverID + " is not available");
}

// =====================================================================
// =====================================================================

openfluid::fluidx::ObserverDescriptor& BuilderMonitoring::getDescriptor(
    std::string ObserverID)
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
      "OpenFLUID Framework", "BuilderMonitoring::getDescriptor",
      "Observer " + ObserverID + " is not in Observer list");
}

// =====================================================================
// =====================================================================

void BuilderMonitoring::addToObserverList(std::string ObserverID)
{
  try
  {
    getSignature(ObserverID);
    openfluid::fluidx::ObserverDescriptor* Obs =
        new openfluid::fluidx::ObserverDescriptor(ObserverID);
    mp_MonitoringDesc->appendItem(Obs);
  }
  catch (openfluid::base::OFException& e)
  {
    throw openfluid::base::OFException(
        "OpenFLUID Framework",
        "BuilderMonitoring::addToObserverList",
        "Unable to add Observer " + ObserverID
        + " because it is not available");
  }

}

// =====================================================================
// =====================================================================

void BuilderMonitoring::removeFromObserverList(std::string ObserverID)
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

std::vector<openfluid::machine::ObserverSignatureInstance*> BuilderMonitoring::getUnusedAvailableSignatures()
{
  std::vector<openfluid::machine::ObserverSignatureInstance*> Signs;

  for (std::vector<openfluid::machine::ObserverSignatureInstance*>::iterator it =
      m_AvailableSignatures.begin(); it != m_AvailableSignatures.end(); ++it)
  {
    try
    {
      getDescriptor((*it)->Signature->ID);
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

}
} //namespaces
