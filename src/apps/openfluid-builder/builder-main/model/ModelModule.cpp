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
 \file ModelModule.cpp
 \brief Implements ...

 \author Aline LIBRES <aline.libres@gmail.com>
 */

#include "ModelModule.hpp"

#include <gtkmm/separator.h>
#include <openfluid/fluidx/AdvancedFluidXDescriptor.hpp>
#include <openfluid/fluidx/FunctionDescriptor.hpp>
#include <openfluid/machine/FunctionSignatureRegistry.hpp>
#include <openfluid/machine/ModelItemInstance.hpp>
#include <openfluid/ware/GeneratorSignature.hpp>
#include "ModelAddFunctionDialog.hpp"
#include "ModelGeneratorCreationDialog.hpp"
#include "WareSetWidget.hpp"
#include "FunctionParamWidget.hpp"
#include "FunctionGlobalParamsWidget.hpp"
#include "FunctionAddParamDialog.hpp"
#include "FunctionAddGlobalParamDialog.hpp"

#include "ModelFctDetailComponent.hpp"
#include "ModelFctDetailModel.hpp"

// =====================================================================
// =====================================================================

ModelModule::ModelModule(
    openfluid::fluidx::AdvancedFluidXDescriptor& AdvancedDesc) :
    ProjectWorkspaceModule(AdvancedDesc), m_Model(AdvancedDesc.getModel())
{
  mp_MainPanel = 0;

  mp_AddFctDialog = new ModelAddFunctionDialog(m_Model);

  mp_AddGlobalParamDialog = new FunctionAddGlobalParamDialog(m_Model);
  mp_AddParamDialog = new FunctionAddParamDialog();

  mp_GlobalParamsWidget = Gtk::manage(
      new FunctionGlobalParamsWidget(m_Model, *mp_AddGlobalParamDialog));
  mp_GlobalParamsWidget->signal_changeOccured().connect(
      sigc::mem_fun(*this, &ModelModule::updateGlobalParams));

  mp_ModelWidget = Gtk::manage(new WareSetWidget("Add simulation function"));
  mp_ModelWidget->signal_AddAsked().connect(
      sigc::mem_fun(*this, &ModelModule::whenAddFunctionAsked));

  update();
}

// =====================================================================
// =====================================================================

ModelModule::~ModelModule()
{
  delete mp_AddFctDialog;
  delete mp_AddGlobalParamDialog;
  delete mp_AddParamDialog;
}

// =====================================================================
// =====================================================================

void ModelModule::compose()
{
  mp_MainPanel = Gtk::manage(new Gtk::VBox(false, 5));

  Gtk::HSeparator* Sep = Gtk::manage(new Gtk::HSeparator());
  Sep->set_visible(true);

  mp_MainPanel->set_border_width(5);
  mp_MainPanel->pack_start(*mp_GlobalParamsWidget, Gtk::PACK_SHRINK, 10);
  mp_MainPanel->pack_start(*Sep, Gtk::PACK_SHRINK);
  mp_MainPanel->pack_start(*mp_ModelWidget);

  mp_MainPanel->set_visible(true);
}

// =====================================================================
// =====================================================================

Gtk::Widget* ModelModule::asWidget()
{
  if (mp_MainPanel)
    return mp_MainPanel;
  throw std::logic_error(
      "ModelModule : you try to get a widget from a non yet composed module.");
}

// =====================================================================
// =====================================================================

void ModelModule::whenAddFunctionAsked()
{
  openfluid::machine::ModelItemSignatureInstance* Signature =
      mp_AddFctDialog->show();

  if (!Signature)
    return;

  openfluid::fluidx::ModelItemDescriptor* Item;

  ModelGeneratorCreationDialog Dialog(m_AdvancedDesc);

  switch (Signature->ItemType)
  {
    case openfluid::fluidx::ModelItemDescriptor::PluggedFunction:
      Item = new openfluid::fluidx::FunctionDescriptor(
          Signature->Signature->ID);
      break;
    case openfluid::fluidx::ModelItemDescriptor::Generator:
      Item =
          Dialog.show(
              (dynamic_cast<openfluid::ware::GeneratorSignature*>(Signature->Signature))->m_GeneratorMethod);
      break;
    default:
      return;
      break;
  }

  if (Item)
  {
    m_Model.appendItem(Item);

    update();

    onModelChanged();
  }

}

// =====================================================================
// =====================================================================

void ModelModule::whenRemoveFunctionAsked(std::string ID)
{
  int Position = m_Model.getFirstItemIndex(ID);

  if (Position < 0)
    return;

  m_Model.removeItem(Position);
  update();
  onModelChanged();
}

// =====================================================================
// =====================================================================

void ModelModule::whenUpAsked(std::string ID)
{
  int Position = m_Model.getFirstItemIndex(ID);

  if (Position < 0)
    return;

  int From = Position;
  int To = (From == 0) ? m_Model.getItemsCount() - 1 : From - 1;

  m_Model.moveItem(From, To);
  update();
  onModelChanged();
}

// =====================================================================
// =====================================================================

void ModelModule::whenDownAsked(std::string ID)
{
  int Position = m_Model.getFirstItemIndex(ID);

  if (Position < 0)
    return;

  int From = Position;
  int To = (From == m_Model.getItemsCount() - 1) ? 0 : From + 1;

  m_Model.moveItem(From, To);
  update();
  onModelChanged();
}

// =====================================================================
// =====================================================================

sigc::signal<void> ModelModule::signal_ModuleChanged()
{
  return m_signal_ModelChanged;
}

// =====================================================================
// =====================================================================

void ModelModule::update()
{
  mp_ModelWidget->storeExpanderStates();

  mp_ModelWidget->clearItems();

  m_ParamWidgets.clear();

  openfluid::machine::FunctionSignatureRegistry* Reg =
      openfluid::machine::FunctionSignatureRegistry::getInstance();

  const std::list<openfluid::fluidx::ModelItemDescriptor*>& Items =
      m_Model.getItems();

  std::list<openfluid::fluidx::ModelItemDescriptor*>::const_iterator itLast =
      Items.end().operator --();

  for (std::list<openfluid::fluidx::ModelItemDescriptor*>::const_iterator it =
      Items.begin(); it != Items.end(); ++it)
  {
    std::string ID = m_Model.getID(*it);

    std::string Name;

    openfluid::machine::ModelItemSignatureInstance* Sign =
        Reg->getSignatureItemInstance(*it);
    if (!Sign)
      Name = _("(Signature not available)");
    else
      Name = Sign->Signature->Name;

    FunctionParamWidget* ItemParamWidget = Gtk::manage(
        new FunctionParamWidget(*(*it), Sign, *mp_AddParamDialog));
    ItemParamWidget->signal_changeOccured().connect(
        sigc::mem_fun(*this, &ModelModule::onModelChanged));
    ItemParamWidget->signal_fileChangeOccured().connect(
        sigc::mem_fun(*this, &ModelModule::onFileChanged));

    ModelFctDetailComponent* ItemInfo = new ModelFctDetailComponent();
    ItemInfo->getModel()->setFctToDisplay(Sign);

    WareItemWidget* ItemWidget = Gtk::manage(
        new WareItemWidget(ID, *ItemParamWidget, *ItemInfo->asWidget(), Name,
                           (*it)->getType()));
    ItemWidget->signal_RemoveAsked().connect(
        sigc::mem_fun(*this, &ModelModule::whenRemoveFunctionAsked));
    ItemWidget->signal_UpAsked().connect(
        sigc::mem_fun(*this, &ModelModule::whenUpAsked));
    ItemWidget->signal_DownAsked().connect(
        sigc::mem_fun(*this, &ModelModule::whenDownAsked));

    mp_ModelWidget->addItem(ItemWidget, ID);

    m_ParamWidgets.push_back(ItemParamWidget);

    if (it == Items.begin())
      ItemWidget->setUpButtonSensitive(false);
    if (it == itLast)
      ItemWidget->setDownButtonSensitive(false);
  }

  mp_ModelWidget->applyExpanderStates();

  mp_ModelWidget->show_all_children();

  updateGlobalParams();
}

// =====================================================================
// =====================================================================

void ModelModule::updateGlobalParams()
{
  std::map<std::string, std::string> GlobalParams =
      openfluid::fluidx::WareDescriptor::getParamsAsMap(
          m_Model.getGlobalParameters());

  for (std::list<FunctionParamWidget*>::iterator it = m_ParamWidgets.begin();
      it != m_ParamWidgets.end(); ++it)
  {
    (*it)->updateGlobals(GlobalParams);
  }

  onModelChanged();
}

// =====================================================================
// =====================================================================

void ModelModule::onModelChanged()
{
  m_signal_ModelChanged.emit();
}

// =====================================================================
// =====================================================================

void ModelModule::onFileChanged()
{
  for (std::list<FunctionParamWidget*>::iterator it = m_ParamWidgets.begin();
      it != m_ParamWidgets.end(); ++it)
  {
    (*it)->updateRequiredFilesRows();
  }

  onModelChanged();
}

// =====================================================================
// =====================================================================
