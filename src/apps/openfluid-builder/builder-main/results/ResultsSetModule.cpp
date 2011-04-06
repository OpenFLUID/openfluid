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
 \file ResultsSetModule.cpp
 \brief Implements ...

 \author Aline LIBRES <libres@supagro.inra.fr>
 */

#include "ResultsSetModule.hpp"

#include "ResUnitChooserComponent.hpp"
#include "ResViewerComponent.hpp"
#include "ResultsSetCoordinator.hpp"

#include "BuilderFrame.hpp"

// =====================================================================
// =====================================================================


ResultsSetModule::ResultsSetModule()
{
  mp_MainPanel = 0;

  mp_ResUnitChooserMVP = new ResUnitChooserComponent();
  mp_ResViewerMVP = new ResViewerComponent();

  mp_Coordinator = new ResultsSetCoordinator(*mp_ResUnitChooserMVP->getModel(),
      *mp_ResViewerMVP->getModel());
}

// =====================================================================
// =====================================================================


ResultsSetModule::~ResultsSetModule()
{
  delete mp_Coordinator;
  delete mp_ResUnitChooserMVP;
  delete mp_ResViewerMVP;
}

// =====================================================================
// =====================================================================


void ResultsSetModule::compose()
{
  mp_MainPanel = Gtk::manage(new Gtk::VBox());

  Gtk::HBox* TopPanel = Gtk::manage(new Gtk::HBox());
  TopPanel->set_border_width(5);
  TopPanel->pack_start(*mp_ResUnitChooserMVP->asWidget());
  TopPanel->set_visible(true);

  Gtk::HBox* MiddlePanel = Gtk::manage(new Gtk::HBox());
  MiddlePanel->set_border_width(5);
  MiddlePanel->pack_start(*mp_ResViewerMVP->asWidget());
  MiddlePanel->set_visible(true);

  BuilderFrame* TopFrame = Gtk::manage(new BuilderFrame());
  TopFrame->setLabelText(_("Unit Choice"));
  TopFrame->set_visible(true);
  TopFrame->add(*TopPanel);

  BuilderFrame* MiddleFrame = Gtk::manage(new BuilderFrame());
  MiddleFrame->setLabelText(_("Variables"));
  MiddleFrame->set_visible(true);
  MiddleFrame->add(*MiddlePanel);

  mp_MainPanel->pack_start(*TopFrame, Gtk::PACK_SHRINK, 0);
  mp_MainPanel->pack_start(*MiddleFrame);
  mp_MainPanel->set_visible(true);
  mp_MainPanel->show_all_children();
}

// =====================================================================
// =====================================================================


Gtk::Widget* ResultsSetModule::asWidget()
{
  if (mp_MainPanel)
    return mp_MainPanel;
  throw std::logic_error(
      "ResultsSetModule : you try to get a widget from a non yet composed module.");
}

// =====================================================================
// =====================================================================


void ResultsSetModule::setEngineRequirements(
    openfluid::machine::ModelInstance& ModelInstance,
    openfluid::machine::SimulationBlob& SimBlob)
{
  mp_ModelInstance = &ModelInstance;
  mp_SimBlob = &SimBlob;

  mp_Coordinator->setEngineRequirements(SimBlob.getRunDescriptor(),
      SimBlob.getOutputDescriptor(), SimBlob.getCoreRepository(), ModelInstance);
}

// =====================================================================
// =====================================================================


void ResultsSetModule::setSelectedSetFromApp(std::string SetName)
{
  mp_Coordinator->setSelectedSetFromApp(SetName);
}


// =====================================================================
// =====================================================================


void ResultsSetModule::update()
{
  mp_Coordinator->update();
}

// =====================================================================
// =====================================================================


sigc::signal<void> ResultsSetModule::signal_ModuleChanged()
{
  return m_signal_ResultsSetChanged;
}
