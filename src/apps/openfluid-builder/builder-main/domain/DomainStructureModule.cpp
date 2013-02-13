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
 \file DomainStructureModule.cpp
 \brief Implements ...

 \author Aline LIBRES <libres@supagro.inra.fr>
 */

#include "DomainStructureModule.hpp"

#include "DomainStructureComponent.hpp"
#include "DomainUnitRelationAddDialog.hpp"
#include "DomainUnitAddEditDialog.hpp"

#include "DomainStructureCoordinator.hpp"
#include "BuilderListToolBoxFactory.hpp"
#include "BuilderListToolBox.hpp"

#include "BuilderFrame.hpp"
#include <openfluid/guicommon/BuilderDescriptor.hpp>

// =====================================================================
// =====================================================================


DomainStructureModule::DomainStructureModule(openfluid::guicommon::BuilderDescriptor& BuilderDesc):
ProjectWorkspaceModule(BuilderDesc)
{
  mp_MainPanel = 0;

  mp_DomainStructureMVP = new DomainStructureComponent(BuilderDesc.getDomain());
//  mp_DomainUnitRelationAddDialog = new DomainUnitRelationAddDialog();
  mp_DomainUnitAddEditDialog = new DomainUnitAddEditDialog(*mp_DomainUnitRelationAddDialog, BuilderDesc.getDomain());


  mp_StructureListToolBox = BuilderListToolBoxFactory::createDomainStructureToolBox();

  mp_Coordinator = new DomainStructureCoordinator(*mp_DomainStructureMVP->getModel(),
      *mp_DomainUnitAddEditDialog,
      *mp_StructureListToolBox);

  mp_Coordinator->signal_DomainChanged().connect(sigc::mem_fun(*this,
      &DomainStructureModule::whenDomainChanged));

  mp_Coordinator->update();
}

// =====================================================================
// =====================================================================


DomainStructureModule::~DomainStructureModule()
{
  delete mp_Coordinator;
  delete mp_DomainUnitAddEditDialog;
//  delete mp_DomainUnitRelationAddDialog;
  delete mp_StructureListToolBox;
  delete mp_DomainStructureMVP;
}

// =====================================================================
// =====================================================================


void DomainStructureModule::compose()
{
  mp_MainPanel = Gtk::manage(new Gtk::VBox());

  Gtk::HBox* TopPanel = Gtk::manage(new Gtk::HBox());
  TopPanel->set_border_width(5);
  TopPanel->pack_start(*mp_DomainStructureMVP->asWidget(),Gtk::PACK_EXPAND_WIDGET,5);

  Gtk::VBox* ButtonsPanel = Gtk::manage(new Gtk::VBox());
  ButtonsPanel->pack_start(*mp_StructureListToolBox->asWidget(),Gtk::PACK_SHRINK);
  ButtonsPanel->set_visible(true);

  TopPanel->pack_start(*ButtonsPanel, Gtk::PACK_SHRINK,5);
  TopPanel->set_visible(true);

  mp_MainPanel->pack_start(*TopPanel,Gtk::PACK_EXPAND_WIDGET,12);

  mp_MainPanel->set_visible(true);
}

// =====================================================================
// =====================================================================


Gtk::Widget* DomainStructureModule::asWidget()
{
  if (mp_MainPanel)
    return mp_MainPanel;
  throw std::logic_error(
      "DomainStructureModule : you try to get a widget from a non yet composed module.");
}

// =====================================================================
// =====================================================================


sigc::signal<void> DomainStructureModule::signal_ModuleChanged()
{
  return m_signal_DomainStructureChanged;
}

// =====================================================================
// =====================================================================


void DomainStructureModule::whenDomainChanged()
{
  m_signal_DomainStructureChanged.emit();
}

// =====================================================================
// =====================================================================


void DomainStructureModule::update()
{
  mp_Coordinator->update();
}
