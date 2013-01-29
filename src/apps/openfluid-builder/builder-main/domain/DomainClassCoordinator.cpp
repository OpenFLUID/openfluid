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
 \file DomainClassCoordinator.cpp
 \brief Implements ...

 \author Aline LIBRES <libres@supagro.inra.fr>
 */

#include "DomainClassCoordinator.hpp"

#include <glibmm/i18n.h>

#include <openfluid/guicommon/DialogBoxFactory.hpp>
#include <openfluid/machine/SimulationBlob.hpp>

#include "DomainIDataModel.hpp"
#include "DomainIDataAddDialog.hpp"
#include "DomainIDataRemoveDialog.hpp"
#include "DomainIDataEditDialog.hpp"
#include "DomainEventsModel.hpp"
#include "BuilderListToolBox.hpp"

// =====================================================================
// =====================================================================


void DomainClassCoordinator::updateIDataListToolBox()
{
  bool HasAtLeastAnIData = !m_IDataModel.isEmptyDataList();
  m_IDataListToolBox.setRemoveCommandAvailable(HasAtLeastAnIData);
  m_IDataListToolBox.setEditCommandAvailable(HasAtLeastAnIData);
}

// =====================================================================
// =====================================================================


void DomainClassCoordinator::whenAddIDataAsked()
{
  std::pair<std::string, std::string> Data = m_IDataAddDialog.show();

  m_IDataModel.addData(Data.first, Data.second);

  updateIDataListToolBox();
}

// =====================================================================
// =====================================================================


void DomainClassCoordinator::whenRemoveIDataAsked()
{
  std::string DataName = m_IDataRemoveDialog.show();
  m_IDataModel.removeData(DataName);

  updateIDataListToolBox();
}

// =====================================================================
// =====================================================================


void DomainClassCoordinator::whenEditIDataAsked()
{
  std::pair<std::string, std::string> Data = m_IDataEditDialog.show();

  m_IDataModel.changeDataName(Data.first, Data.second);
}

// =====================================================================
// =====================================================================


void DomainClassCoordinator::whenIDataChanged()
{
  m_IDataAddDialog.update();
  m_IDataRemoveDialog.update();
  m_IDataEditDialog.update();

  m_signal_DomainClassChanged.emit();
}

// =====================================================================
// =====================================================================


void DomainClassCoordinator::whenAddEventAsked()
{
  openfluid::guicommon::DialogBoxFactory::showDisabledFeatureMessage();
}

// =====================================================================
// =====================================================================


void DomainClassCoordinator::whenRemoveEventAsked()
{
  openfluid::guicommon::DialogBoxFactory::showDisabledFeatureMessage();
}

// =====================================================================
// =====================================================================


DomainClassCoordinator::DomainClassCoordinator(DomainIDataModel& IDataModel,
    BuilderListToolBox& IDataListToolBox, DomainIDataAddDialog& IDataAddDialog,
    DomainIDataRemoveDialog& IDataRemoveDialog,
    DomainIDataEditDialog& IDataEditDialog, DomainEventsModel& EventsModel,
    BuilderListToolBox& EventsListToolBox) :
  m_IDataModel(IDataModel), m_IDataListToolBox(IDataListToolBox),
      m_IDataAddDialog(IDataAddDialog), m_IDataRemoveDialog(IDataRemoveDialog),
      m_IDataEditDialog(IDataEditDialog), m_EventsModel(EventsModel),
      m_EventsListToolBox(EventsListToolBox)
{
  m_IDataListToolBox.signal_AddCommandAsked().connect(sigc::mem_fun(*this,
      &DomainClassCoordinator::whenAddIDataAsked));
  m_IDataListToolBox.signal_RemoveCommandAsked().connect(sigc::mem_fun(*this,
      &DomainClassCoordinator::whenRemoveIDataAsked));
  m_IDataListToolBox.signal_EditCommandAsked().connect(sigc::mem_fun(*this,
      &DomainClassCoordinator::whenEditIDataAsked));

  m_IDataModel.signal_IDataChanged().connect(sigc::mem_fun(*this,
      &DomainClassCoordinator::whenIDataChanged));

  m_EventsListToolBox.signal_AddCommandAsked().connect(sigc::mem_fun(*this,
      &DomainClassCoordinator::whenAddEventAsked));
  m_EventsListToolBox.signal_RemoveCommandAsked().connect(sigc::mem_fun(*this,
      &DomainClassCoordinator::whenRemoveEventAsked));
}

// =====================================================================
// =====================================================================


sigc::signal<void> DomainClassCoordinator::signal_DomainClassChanged()
{
  return m_signal_DomainClassChanged;
}

// =====================================================================
// =====================================================================


void DomainClassCoordinator::setEngineRequirements(
    openfluid::machine::ModelInstance& /*ModelInstance*/,
    openfluid::machine::SimulationBlob& SimBlob)
{
  m_IDataAddDialog.setEngineRequirements(SimBlob.getCoreRepository());
  m_IDataRemoveDialog.setEngineRequirements(SimBlob.getCoreRepository());
  m_IDataEditDialog.setEngineRequirements(SimBlob.getCoreRepository());

  m_EventsModel.setEngineRequirements(SimBlob.getCoreRepository());
}

// =====================================================================
// =====================================================================


void DomainClassCoordinator::setSelectedClassFromApp(std::string ClassName)
{
  m_IDataModel.setClass(ClassName);
  m_IDataAddDialog.setClass(ClassName);
  m_IDataRemoveDialog.setClass(ClassName);
  m_IDataEditDialog.setClass(ClassName);
  m_EventsModel.setClass(ClassName);

  updateIDataListToolBox();
}

// =====================================================================
// =====================================================================


void DomainClassCoordinator::update()
{
  m_IDataModel.update();
  m_IDataAddDialog.update();
  m_IDataRemoveDialog.update();
  m_IDataEditDialog.update();

  updateIDataListToolBox();
}
