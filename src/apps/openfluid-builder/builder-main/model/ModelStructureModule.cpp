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
 \file ModelStructureModule.cpp
 \brief Implements ...

 \author Aline LIBRES <libres@supagro.inra.fr>
 */

#include "ModelStructureModule.hpp"

#include "ModelFctDetailComponent.hpp"
#include "ModelStructureComponent.hpp"
#include "ModelGlobalParamsComponent.hpp"
#include "ModelParamsPanel.hpp"

#include "ModelStructureCoordinator.hpp"
#include "FunctionSignatureRegistry.hpp"
#include "BuilderListToolBoxFactory.hpp"
#include "BuilderListToolBox.hpp"

#include "BuilderFrame.hpp"

// =====================================================================
// =====================================================================


ModelStructureModule::ModelStructureModule(openfluid::guicommon::BuilderDescriptor& BuilderDesc):
ProjectWorkspaceModule(BuilderDesc)
{
  mp_MainPanel = 0;

  mp_ModelFctDetailMVP = new ModelFctDetailComponent();
  mp_ModelStructureMVP = new ModelStructureComponent();

  mp_ModelGlobalParamsMVP = new ModelGlobalParamsComponent();
  mp_ModelParamsPanel = new ModelParamsPanel();
  mp_ModelParamsPanel->addAStaticPage(mp_ModelGlobalParamsMVP->asWidget(),
      _("Global parameters"), 0);

  mp_StructureListToolBox
      = BuilderListToolBoxFactory::createModelStructureToolBox();

  mp_Coordinator = new ModelStructureCoordinator(
      *mp_ModelFctDetailMVP->getModel(), *mp_ModelStructureMVP->getModel(),
      *mp_ModelGlobalParamsMVP->getModel(), *mp_ModelParamsPanel,
      *mp_StructureListToolBox);

  mp_Coordinator->signal_ModelChanged().connect(sigc::mem_fun(*this,
      &ModelStructureModule::whenModelChanged));
}

// =====================================================================
// =====================================================================


ModelStructureModule::~ModelStructureModule()
{
  delete mp_Coordinator;
  delete mp_ModelFctDetailMVP;
  delete mp_ModelStructureMVP;
  delete mp_ModelGlobalParamsMVP;
  delete mp_StructureListToolBox;
  delete mp_ModelParamsPanel;
}

// =====================================================================
// =====================================================================


void ModelStructureModule::compose()
{
  mp_MainPanel = Gtk::manage(new Gtk::VPaned());

  Gtk::VBox* ButtonsPanel = Gtk::manage(new Gtk::VBox());
  ButtonsPanel->pack_start(*mp_StructureListToolBox->asWidget(),Gtk::PACK_SHRINK);
  ButtonsPanel->set_visible(true);

  Gtk::HBox* TopPanel = Gtk::manage(new Gtk::HBox());
  TopPanel->set_border_width(5);
  TopPanel->pack_start(*mp_ModelStructureMVP->asWidget(),Gtk::PACK_EXPAND_WIDGET,5);
  TopPanel->pack_start(*ButtonsPanel, Gtk::PACK_SHRINK,5);
  TopPanel->pack_start(*mp_ModelFctDetailMVP->asWidget(),Gtk::PACK_SHRINK,5);
  TopPanel->set_visible(true);
  TopPanel->set_border_width(6);

  Gtk::HBox* BottomPanel = Gtk::manage(new Gtk::HBox());
  BottomPanel->set_border_width(5);
  BottomPanel->pack_start(*mp_ModelParamsPanel->asWidget());
  BottomPanel->set_visible(true);
  BottomPanel->set_border_width(6);

  BuilderFrame* TopFrame = Gtk::manage(new BuilderFrame());
  TopFrame->setLabelText(_("Model definition"));
  TopFrame->set_visible(true);
  TopFrame->add(*TopPanel);

  BuilderFrame* BottomFrame = Gtk::manage(new BuilderFrame());
  BottomFrame->setLabelText(_("Model parameters"));
  BottomFrame->set_visible(true);
  BottomFrame->add(*BottomPanel);

  mp_MainPanel->pack1(*TopFrame, true,false);
  mp_MainPanel->pack2(*BottomFrame, true,false);
  mp_MainPanel->set_visible(true);
}

// =====================================================================
// =====================================================================


Gtk::Widget* ModelStructureModule::asWidget()
{
  if (mp_MainPanel)
    return mp_MainPanel;
  throw std::logic_error(
      "ModelStructureModule : you try to get a widget from a non yet composed module.");
}

// =====================================================================
// =====================================================================


sigc::signal<void> ModelStructureModule::signal_ModuleChanged()
{
  return m_signal_ModelStructureChanged;
}

// =====================================================================
// =====================================================================


void ModelStructureModule::setEngineRequirements(
    openfluid::machine::ModelInstance& ModelInstance,
    openfluid::machine::SimulationBlob& SimBlob,
    openfluid::guicommon::BuilderDescriptor& BuilderDesc)
{
  mp_ModelInstance = &ModelInstance;
  mp_SimBlob = &SimBlob;

  mp_Coordinator->setEngineRequirements(ModelInstance, SimBlob);
}

// =====================================================================
// =====================================================================


void ModelStructureModule::setCurrentFunction(std::string FunctionName)
{
  mp_Coordinator->setCurrentFunction(FunctionName);
}

// =====================================================================
// =====================================================================


void ModelStructureModule::whenModelChanged()
{
  m_signal_ModelStructureChanged.emit();
}

// =====================================================================
// =====================================================================


void ModelStructureModule::update()
{
  mp_Coordinator->update();
}

// =====================================================================
// =====================================================================


void ModelStructureModule::updateWithFctParamsComponents()
{
  mp_Coordinator->updateWithFctParamsComponents();
}
