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
 \file SimulationOutModule.cpp
 \brief Implements ...

 \author Aline LIBRES <libres@supagro.inra.fr>
 */

#include "SimulationOutModule.hpp"

#include "SimulOutFilesComponent.hpp"
#include "SimulOutFileDescComponent.hpp"
#include "SimulOutSetsComponent.hpp"
#include "SimulOutSetDescComponent.hpp"
#include "SimulOutCoordinator.hpp"
#include "BuilderListToolBoxFactory.hpp"
#include "BuilderListToolBox.hpp"
#include "BuilderFrame.hpp"

// =====================================================================
// =====================================================================


SimulationOutModule::SimulationOutModule()
{
  mp_MainPanel = 0;

  mp_SimulOutFilesMVP = new SimulOutFilesComponent();
  mp_SimulOutFileDescMVP = new SimulOutFileDescComponent();

  mp_OutFilesListToolBox = BuilderListToolBoxFactory::createSimulOutFilesToolBox();

  mp_SimulOutSetsMVP = new SimulOutSetsComponent();
  mp_SimulOutSetDescMVP = new SimulOutSetDescComponent();

  mp_OutSetsListToolBox = BuilderListToolBoxFactory::createSimulOutSetsToolBox();

  mp_Coordinator = new SimulOutCoordinator(*mp_SimulOutFilesMVP->getModel(),
      *mp_SimulOutFileDescMVP->getModel(), *mp_OutFilesListToolBox,
      *mp_SimulOutSetsMVP->getModel(), *mp_SimulOutSetDescMVP->getModel(),
      *mp_OutSetsListToolBox);

  mp_Coordinator->signal_SimulOutChanged().connect(sigc::mem_fun(*this,
      &SimulationOutModule::whenOutChanged));
}

// =====================================================================
// =====================================================================


SimulationOutModule::~SimulationOutModule()
{
  delete mp_Coordinator;
  delete mp_SimulOutFilesMVP;
  delete mp_SimulOutFileDescMVP;
  delete mp_OutFilesListToolBox;
  delete mp_SimulOutSetsMVP;
  delete mp_SimulOutSetDescMVP;
  delete mp_OutSetsListToolBox;
}

// =====================================================================
// =====================================================================


void SimulationOutModule::compose()
{
  mp_MainPanel = Gtk::manage(new Gtk::VBox());

  Gtk::HBox* MiddlePanel = Gtk::manage(new Gtk::HBox());
  MiddlePanel->set_border_width(5);
  MiddlePanel->pack_start(*mp_SimulOutFilesMVP->asWidget());
  MiddlePanel->pack_start(*mp_OutFilesListToolBox->asWidget(),
      Gtk::PACK_SHRINK, 5);
  MiddlePanel->set_visible(true);

  Gtk::HBox* BottomPanel = Gtk::manage(new Gtk::HBox());
  BottomPanel->set_border_width(5);
  BottomPanel->pack_start(*mp_SimulOutSetsMVP->asWidget());
  BottomPanel->pack_start(*mp_OutSetsListToolBox->asWidget(), Gtk::PACK_SHRINK,
      5);
  BottomPanel->set_visible(true);

  BuilderFrame* MiddleFrame = Gtk::manage(new BuilderFrame());
  MiddleFrame->setLabelText(_("Files Formats"));
  MiddleFrame->set_visible(true);
  MiddleFrame->add(*MiddlePanel);

  BuilderFrame* BottomFrame = Gtk::manage(new BuilderFrame());
  BottomFrame->setLabelText(_("Sets Definition"));
  BottomFrame->set_visible(true);
  BottomFrame->add(*BottomPanel);

  mp_MainPanel->pack_start(*MiddleFrame);
  mp_MainPanel->pack_start(*BottomFrame);
  mp_MainPanel->set_visible(true);
}

// =====================================================================
// =====================================================================


Gtk::Widget* SimulationOutModule::asWidget()
{
  if (mp_MainPanel)
    return mp_MainPanel;
  throw std::logic_error(
      "SimulationOutModule : you try to get a widget from a non yet composed module.");
}

// =====================================================================
// =====================================================================


sigc::signal<void> SimulationOutModule::signal_ModuleChanged()
{
  return m_signal_SimulationOutChanged;
}

// =====================================================================
// =====================================================================


void SimulationOutModule::setEngineRequirements(
    openfluid::machine::ModelInstance& ModelInstance,
    openfluid::machine::SimulationBlob& SimBlob)
{
  mp_ModelInstance = &ModelInstance;
  mp_SimBlob = &SimBlob;

  mp_Coordinator->setEngineRequirements(ModelInstance, SimBlob);
}

// =====================================================================
// =====================================================================


void SimulationOutModule::whenOutChanged()
{
  m_signal_SimulationOutChanged.emit();
}

// =====================================================================
// =====================================================================


void SimulationOutModule::update()
{
  //  mp_SimulRunMVP->getModel()->update();
}

