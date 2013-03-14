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
 \file MonitoringModel.cpp
 \brief Implements ...

 \author Aline LIBRES <aline.libres@gmail.com>
 */

#include "MonitoringModel.hpp"

#include <openfluid/fluidx/ObserverDescriptor.hpp>
#include <openfluid/machine/ObserverPluginsManager.hpp>
#include <openfluid/machine/ObserverInstance.hpp>
#include <openfluid/ware/PluggableWare.hpp>

// =====================================================================
// =====================================================================

MonitoringModel::MonitoringModel(
    openfluid::fluidx::AdvancedMonitoringDescriptor& Monitoring) :
    mp_Monitoring(&Monitoring)
{

}

// =====================================================================
// =====================================================================

MonitoringModel::~MonitoringModel()
{

}

// =====================================================================
// =====================================================================

sigc::signal<void> MonitoringModel::signal_UpdateAsked()
{
  return m_signal_UpdateAsked;
}

// =====================================================================
// =====================================================================

sigc::signal<void, std::string> MonitoringModel::signal_EditParamsAsked()
{
  return m_signal_EditParamsAsked;
}

// =====================================================================
// =====================================================================

sigc::signal<void> MonitoringModel::signal_AddObserverAsked()
{
  return m_signal_AddObserverAsked;
}

// =====================================================================
// =====================================================================

sigc::signal<void> MonitoringModel::signal_ObserverRemoved()
{
  return m_signal_ObserverRemoved;
}

// =====================================================================
// =====================================================================

void MonitoringModel::update()
{
  m_signal_UpdateAsked.emit();
}

// =====================================================================
// =====================================================================

std::list<std::pair<std::string, std::string> > MonitoringModel::getItems()
{
  std::list<std::pair<std::string, std::string> > Items;

  const std::list<openfluid::fluidx::ObserverDescriptor*>& ItemDesc =
      mp_Monitoring->getItems();

  for (std::list<openfluid::fluidx::ObserverDescriptor*>::const_iterator it =
      ItemDesc.begin(); it != ItemDesc.end(); ++it)
  {
    std::string ID = (*it)->getID();
    std::string Name = mp_Monitoring->getSignature(ID).Signature->Name;
    Items.push_back(std::make_pair(ID, Name));
  }

  return Items;
}

// =====================================================================
// =====================================================================

void MonitoringModel::addObserverAsked()
{
  m_signal_AddObserverAsked.emit();
}

// =====================================================================
// =====================================================================

void MonitoringModel::editParamsAsked(std::string ObserverID)
{
  m_signal_EditParamsAsked.emit(ObserverID);
}

// =====================================================================
// =====================================================================

void MonitoringModel::removeObserver(std::string ObserverID)
{
  mp_Monitoring->removeFromObserverList(ObserverID);
  m_signal_UpdateAsked.emit();
  m_signal_ObserverRemoved.emit();
}

// =====================================================================
// =====================================================================

void MonitoringModel::addObservers(std::set<std::string> ObserverIDs)
{
  for (std::set<std::string>::iterator it = ObserverIDs.begin();
      it != ObserverIDs.end(); ++it)
    mp_Monitoring->addToObserverList(*it);

  if (!ObserverIDs.empty())
    m_signal_UpdateAsked.emit();
}

// =====================================================================
// =====================================================================

void MonitoringModel::setParameters(std::string ObserverID,
                                    openfluid::ware::WareParams_t Params)
{
  openfluid::fluidx::ObserverDescriptor& Obs = mp_Monitoring->getDescriptor(
      ObserverID);

  Obs.clearParameters();

  Obs.setParameters(Params);
}

// =====================================================================
// =====================================================================
