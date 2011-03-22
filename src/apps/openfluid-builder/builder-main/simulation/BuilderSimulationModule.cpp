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
 \file BuilderSimulationModule.cpp
 \brief Implements ...

 \author Aline LIBRES <libres@supagro.inra.fr>
 */

#include "BuilderSimulationModule.hpp"

#include "SimulRunComponent.hpp"
#include "SimulOutFilesComponent.hpp"
#include "SimulOutFileDescComponent.hpp"
#include "SimulOutSetsComponent.hpp"
#include "SimulOutSetDescComponent.hpp"
#include "SimulCoordinator.hpp"
#include "BuilderListToolBoxFactory.hpp"
#include "BuilderListToolBox.hpp"
#include "BuilderFrame.hpp"

BuilderSimulationModule::BuilderSimulationModule(
    BuilderListToolBoxFactory& ListToolBoxFactory)
{
  mp_MainPanel = 0;

  mp_SimulRunMVP = new SimulRunComponent();

  mp_SimulOutFilesMVP = new SimulOutFilesComponent();
  mp_SimulOutFileDescMVP = new SimulOutFileDescComponent();

  mp_OutFilesListToolBox = ListToolBoxFactory.createSimulOutFilesToolBox();

  mp_SimulOutSetsMVP = new SimulOutSetsComponent();
  mp_SimulOutSetDescMVP = new SimulOutSetDescComponent();

  mp_OutSetsListToolBox = ListToolBoxFactory.createSimulOutSetsToolBox();

  mp_Coordinator = new SimulCoordinator(*mp_SimulRunMVP->getModel(),
      *mp_SimulOutFilesMVP->getModel(), *mp_SimulOutFileDescMVP->getModel(),
      *mp_OutFilesListToolBox, *mp_SimulOutSetsMVP->getModel(),
      *mp_SimulOutSetDescMVP->getModel(), *mp_OutSetsListToolBox);
}
BuilderSimulationModule::~BuilderSimulationModule()
{
  delete mp_Coordinator;
  delete mp_SimulRunMVP;
  delete mp_SimulOutFilesMVP;
  delete mp_SimulOutFileDescMVP;
  delete mp_OutFilesListToolBox;
  delete mp_SimulOutSetsMVP;
  delete mp_SimulOutSetDescMVP;
  delete mp_OutSetsListToolBox;
}
void BuilderSimulationModule::initialize()
{
}
void BuilderSimulationModule::setEngineRequirements(
    openfluid::base::RunDescriptor& RunDesc,
    openfluid::base::OutputDescriptor& OutDesc,
    openfluid::core::CoreRepository& CoreRepos,
    openfluid::machine::ModelInstance& ModelInstance)
{
  mp_Coordinator->setEngineRequirements(RunDesc, OutDesc, CoreRepos,
      ModelInstance);
}
void BuilderSimulationModule::compose()
{
  mp_MainPanel = Gtk::manage(new Gtk::VBox());

  Gtk::HBox* TopPanel = Gtk::manage(new Gtk::HBox());
  TopPanel->set_border_width(5);
  TopPanel->pack_start(*mp_SimulRunMVP->asWidget(), Gtk::PACK_SHRINK);
  TopPanel->set_visible(true);

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

  BuilderFrame* TopFrame = Gtk::manage(new BuilderFrame());
  TopFrame->setLabelText(_("Run Configuration"));
  TopFrame->set_visible(true);
  TopFrame->add(*TopPanel);

  BuilderFrame* MiddleFrame = Gtk::manage(new BuilderFrame());
  MiddleFrame->setLabelText(_("Files Formats"));
  MiddleFrame->set_visible(true);
  MiddleFrame->add(*MiddlePanel);

  BuilderFrame* BottomFrame = Gtk::manage(new BuilderFrame());
  BottomFrame->setLabelText(_("Sets Definition"));
  BottomFrame->set_visible(true);
  BottomFrame->add(*BottomPanel);

  mp_MainPanel->pack_start(*TopFrame);
  mp_MainPanel->pack_start(*MiddleFrame);
  mp_MainPanel->pack_start(*BottomFrame);
  mp_MainPanel->set_visible(true);
}
Gtk::Widget* BuilderSimulationModule::asWidget()
{
  if (mp_MainPanel)
    return mp_MainPanel;
  throw std::logic_error(
      "BuilderSimulationModule : you try to get a widget from a non yet composed module.");
}

BuilderSimulationModuleSub::BuilderSimulationModuleSub(
    BuilderListToolBoxFactory& ListToolBoxFactory) :
  BuilderSimulationModule(ListToolBoxFactory)
{
}
