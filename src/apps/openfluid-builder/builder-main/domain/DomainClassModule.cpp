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
 \file DomainClassModule.cpp
 \brief Implements ...

 \author Aline LIBRES <libres@supagro.inra.fr>
 */

#include <gtkmm/notebook.h>

#include "DomainClassModule.hpp"

#include "DomainIDataComponent.hpp"
#include "DomainIDataAddDialog.hpp"
#include "DomainIDataRemoveDialog.hpp"
#include "DomainIDataEditDialog.hpp"
#include "DomainEventsComponent.hpp"

#include "DomainClassCoordinator.hpp"
#include "BuilderListToolBoxFactory.hpp"
#include "BuilderListToolBox.hpp"

#include "BuilderFrame.hpp"


// =====================================================================
// =====================================================================


DomainClassModule::DomainClassModule(openfluid::guicommon::BuilderDescriptor& BuilderDesc):
ProjectWorkspaceModule(BuilderDesc)
{
  mp_MainPanel = 0;

  mp_DomainIDataMVP = new DomainIDataComponent(BuilderDesc.getDomain());
  mp_IDataListToolBox = BuilderListToolBoxFactory::createDomainIDataToolBox();
  mp_IDataAddDialog = new DomainIDataAddDialog(BuilderDesc.getDomain());
  mp_IDataRemoveDialog = new DomainIDataRemoveDialog(BuilderDesc.getDomain());
  mp_IDataEditDialog = new DomainIDataEditDialog(BuilderDesc.getDomain());

  mp_DomainEventsMVP = new DomainEventsComponent();
  mp_EventsListToolBox = BuilderListToolBoxFactory::createDomainEventsToolBox();

  mp_Coordinator = new DomainClassCoordinator(*mp_DomainIDataMVP->getModel(),
      *mp_IDataListToolBox, *mp_IDataAddDialog, *mp_IDataRemoveDialog,
      *mp_IDataEditDialog, *mp_DomainEventsMVP->getModel(),
      *mp_EventsListToolBox);

  mp_Coordinator->signal_DomainClassChanged().connect(sigc::mem_fun(*this,
      &DomainClassModule::whenClassChanged));
}

// =====================================================================
// =====================================================================


DomainClassModule::~DomainClassModule()
{
  delete mp_Coordinator;
  delete mp_DomainIDataMVP;
  delete mp_IDataAddDialog;
  delete mp_IDataRemoveDialog;
  delete mp_IDataEditDialog;
  delete mp_IDataListToolBox;
}

// =====================================================================
// =====================================================================


void DomainClassModule::compose()
{
  mp_MainPanel = Gtk::manage(new Gtk::VBox());

  // input data

  Gtk::VBox* IDataButtonsPanel = Gtk::manage(new Gtk::VBox());
  IDataButtonsPanel->pack_start(*mp_IDataListToolBox->asWidget(),
      Gtk::PACK_SHRINK);
  IDataButtonsPanel->set_visible(true);

  Gtk::HBox* FirstPanel = Gtk::manage(new Gtk::HBox());
  FirstPanel->set_border_width(5);
  FirstPanel->pack_start(*mp_DomainIDataMVP->asWidget());
  FirstPanel->pack_start(*IDataButtonsPanel, Gtk::PACK_SHRINK, 5);
  FirstPanel->set_visible(true);

  // events

  Gtk::VBox* EventsButtonsPanel = Gtk::manage(new Gtk::VBox());
  EventsButtonsPanel->pack_start(*mp_EventsListToolBox->asWidget(),
      Gtk::PACK_SHRINK);
  EventsButtonsPanel->set_visible(true);

  Gtk::HBox* SecondPanel = Gtk::manage(new Gtk::HBox());
  SecondPanel->set_border_width(5);
  SecondPanel->pack_start(*mp_DomainEventsMVP->asWidget());
  SecondPanel->pack_start(*EventsButtonsPanel, Gtk::PACK_SHRINK, 5);
  SecondPanel->set_visible(true);

  Gtk::Notebook* Notebook = Gtk::manage(new Gtk::Notebook());
  Notebook->append_page(*FirstPanel, _("Inputdata"));
  Notebook->append_page(*SecondPanel, _("Events"));
  Notebook->set_border_width(6);
  Notebook->set_visible(true);

  mp_MainPanel->pack_start(*Notebook);

  mp_MainPanel->set_visible(true);
}

// =====================================================================
// =====================================================================


Gtk::Widget* DomainClassModule::asWidget()
{
  if (mp_MainPanel)
    return mp_MainPanel;
  throw std::logic_error(
      "DomainClassModule : you try to get a widget from a non yet composed module.");
}

// =====================================================================
// =====================================================================


sigc::signal<void> DomainClassModule::signal_ModuleChanged()
{
  return m_signal_DomainClassChanged;
}

// =====================================================================
// =====================================================================


void DomainClassModule::setEngineRequirements(
    openfluid::machine::ModelInstance& ModelInstance,
    openfluid::machine::SimulationBlob& SimBlob,
    openfluid::guicommon::BuilderDescriptor& /*BuilderDesc*/)
{
  mp_ModelInstance = &ModelInstance;
  mp_SimBlob = &SimBlob;

  mp_Coordinator->setEngineRequirements(ModelInstance, SimBlob);
}

// =====================================================================
// =====================================================================


void DomainClassModule::whenClassChanged()
{
  m_signal_DomainClassChanged.emit();
}

// =====================================================================
// =====================================================================


void DomainClassModule::setSelectedClassFromApp(std::string ClassName)
{
  mp_Coordinator->setSelectedClassFromApp(ClassName);
}

// =====================================================================
// =====================================================================


void DomainClassModule::update()
{
  mp_Coordinator->update();
}
