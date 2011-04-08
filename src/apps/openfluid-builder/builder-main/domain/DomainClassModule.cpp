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

#include "DomainClassModule.hpp"

#include "DomainIDataComponent.hpp"

#include "DomainClassCoordinator.hpp"
#include "BuilderListToolBoxFactory.hpp"
#include "BuilderListToolBox.hpp"

#include "BuilderFrame.hpp"

// =====================================================================
// =====================================================================


DomainClassModule::DomainClassModule()
{
  mp_MainPanel = 0;

  mp_DomainIDataMVP = new DomainIDataComponent();
  mp_IDataListToolBox = BuilderListToolBoxFactory::createDomainIDataToolBox();

  mp_Coordinator = new DomainClassCoordinator(*mp_DomainIDataMVP->getModel(),
      *mp_IDataListToolBox);

  mp_Coordinator->signal_DomainClassChanged().connect(sigc::mem_fun(*this,
      &DomainClassModule::whenClassChanged));
}

// =====================================================================
// =====================================================================


DomainClassModule::~DomainClassModule()
{
  delete mp_Coordinator;
  delete mp_DomainIDataMVP;
  delete mp_IDataListToolBox;
}

// =====================================================================
// =====================================================================


void DomainClassModule::compose()
{
  mp_MainPanel = Gtk::manage(new Gtk::VBox());

  Gtk::HBox* FirstPanel = Gtk::manage(new Gtk::HBox());
  FirstPanel->set_border_width(5);
  FirstPanel->pack_start(*mp_DomainIDataMVP->asWidget());
  FirstPanel->pack_start(*mp_IDataListToolBox->asWidget(), Gtk::PACK_SHRINK, 5);
  FirstPanel->set_visible(true);

  Gtk::HBox* SecondPanel = Gtk::manage(new Gtk::HBox());
  SecondPanel->set_border_width(5);
  Gtk::Label* EventsLabel = Gtk::manage(new Gtk::Label("Events"));
  SecondPanel->pack_start(*EventsLabel);
  SecondPanel->set_visible(true);


  Gtk::Notebook* Notebook = Gtk::manage(new Gtk::Notebook());
  Notebook->append_page(*FirstPanel,_("Input Data"));
  Notebook->append_page(*SecondPanel,_("Events"));
  Notebook->set_visible(true);

  BuilderFrame* MainFrame = Gtk::manage(new BuilderFrame());
  MainFrame->setLabelText(_("Class Parameters"));
  MainFrame->set_visible(true);
  MainFrame->add(*Notebook);

  mp_MainPanel->pack_start(*MainFrame);

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
    openfluid::machine::SimulationBlob& SimBlob)
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
