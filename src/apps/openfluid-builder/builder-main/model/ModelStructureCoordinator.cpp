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
 \file ModelStructureCoordinator.cpp
 \brief Implements ...

 \author Aline LIBRES <libres@supagro.inra.fr>
 */

#include "ModelStructureCoordinator.hpp"

#include <glibmm/i18n.h>
#include <algorithm>

#include <openfluid/machine/ModelItemInstance.hpp>
#include <openfluid/fluidx/AdvancedFluidXDescriptor.hpp>
#include <openfluid/fluidx/FunctionDescriptor.hpp>
#include <openfluid/guicommon/DialogBoxFactory.hpp>
#include <openfluid/ware/GeneratorSignature.hpp>
#include <openfluid/config.hpp>

#include "ModelAvailFctModel.hpp"
#include "ModelFctDetailModel.hpp"
#include "ModelStructureModel.hpp"
#include "ModelGlobalParamsModel.hpp"
#include "ModelParamsPanel.hpp"
#include "ModelFctParamsComponent.hpp"
#include "ModelFctParamsModel.hpp"
#include "BuilderListToolBox.hpp"
#include "ModelAddFunctionModule.hpp"
#include "ModelItemInstanceFactory.hpp"
#include "ModelGeneratorCreationDialog.hpp"

// =====================================================================
// =====================================================================

void ModelStructureCoordinator::updateStructureListToolBox()
{
  bool AtLeast1Fct = m_StructureModel.getFctCount() > 0;
  bool AtLeast2Fct = m_StructureModel.getFctCount() > 1;
  m_StructureListToolBox.setRemoveCommandAvailable(AtLeast1Fct);
  m_StructureListToolBox.setUpCommandAvailable(AtLeast2Fct);
  m_StructureListToolBox.setDownCommandAvailable(AtLeast2Fct);
}

// =====================================================================
// =====================================================================

void ModelStructureCoordinator::whenStructureFctSelectionChanged()
{
  int CurrentSelection = m_StructureModel.getCurrentSelection();

  if (CurrentSelection < 0)
    m_FctDetailModel.setFctToDisplay(
        openfluid::ware::FunctionSignatureRegistry::getEmptyPluggableSignature());
  else
    m_FctDetailModel.setFctToDisplay(
        openfluid::ware::FunctionSignatureRegistry::getInstance()->getSignatureItemInstance(
            mp_AdvancedDesc->getModel().getItemAt(CurrentSelection)));

  updateStructureListToolBox();
}

// =====================================================================
// =====================================================================

void ModelStructureCoordinator::whenAddFctAsked()
{
  openfluid::machine::ModelItemSignatureInstance* Signature =
      mp_AddFctModule->showDialog();

  if (!Signature)
    return;

  openfluid::fluidx::ModelItemDescriptor* Item;

  ModelGeneratorCreationDialog Dialog(*mp_AdvancedDesc);

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
      std::cerr
          << "ModelStructureCoordinator::whenAddFctAsked : bad ModelItemDescriptor type"
          << std::endl;
      break;
  }

  if (Item)
  {
    m_StructureModel.appendFunction(Item);

    updateStructureListToolBox();

    createModelFctParamsComponent(Item);

    m_signal_ModelChanged.emit();
  }

}

// =====================================================================
// =====================================================================

void ModelStructureCoordinator::whenRemoveFctAsked()
{
  int Index = m_StructureModel.getCurrentSelection();
  if (Index < 0)
    return;

  eraseModelFctParamsComponent(
      mp_AdvancedDesc->getModel().getOrderedIDs()[Index]);

  m_StructureModel.removeFunctionAt(Index);
  updateStructureListToolBox();

  m_signal_ModelChanged.emit();
}

// =====================================================================
// =====================================================================

void ModelStructureCoordinator::whenMoveTowardTheBeginAsked()
{
  m_StructureModel.moveTowardTheBegin();
  m_signal_ModelChanged.emit();
}

// =====================================================================
// =====================================================================

void ModelStructureCoordinator::whenMoveTowardTheEndAsked()
{
  m_StructureModel.moveTowardTheEnd();
  m_signal_ModelChanged.emit();
}

// =====================================================================
// =====================================================================

void ModelStructureCoordinator::whenGlobalValueChanged()
{
  for (std::map<std::string, ModelFctParamsComponent*>::iterator it =
      m_ByNameFctParamsComponents.begin();
      it != m_ByNameFctParamsComponents.end(); ++it)
  {
    it->second->updateGlobalValues();
  }

  m_HasToUpdate = false;
  m_signal_ModelChanged.emit();
  m_HasToUpdate = true;
}

// =====================================================================
// =====================================================================

ModelStructureCoordinator::ModelStructureCoordinator(
    ModelFctDetailModel& FctDetailModel, ModelStructureModel& StructureModel,
    ModelGlobalParamsModel& GlobalParamsModel, ModelParamsPanel& ParamsPanel,
    BuilderListToolBox& StructureListToolBox,
    openfluid::fluidx::AdvancedFluidXDescriptor& AdvancedDesc) :
    mp_AdvancedDesc(&AdvancedDesc), m_FctDetailModel(FctDetailModel), m_StructureModel(
        StructureModel), m_GlobalParamsModel(GlobalParamsModel), m_ParamsPanel(
        ParamsPanel), m_StructureListToolBox(StructureListToolBox), m_HasToUpdate(
        true)
{
  m_StructureListToolBox.setAddCommandAvailable(true);

  m_StructureModel.signal_FromUserSelectionChanged().connect(
      sigc::mem_fun(
          *this, &ModelStructureCoordinator::whenStructureFctSelectionChanged));

  m_StructureListToolBox.signal_AddCommandAsked().connect(
      sigc::mem_fun(*this, &ModelStructureCoordinator::whenAddFctAsked));
  m_StructureListToolBox.signal_RemoveCommandAsked().connect(
      sigc::mem_fun(*this, &ModelStructureCoordinator::whenRemoveFctAsked));
  m_StructureListToolBox.signal_UpCommandAsked().connect(
      sigc::mem_fun(*this,
                    &ModelStructureCoordinator::whenMoveTowardTheBeginAsked));
  m_StructureListToolBox.signal_DownCommandAsked().connect(
      sigc::mem_fun(*this,
                    &ModelStructureCoordinator::whenMoveTowardTheEndAsked));

  m_GlobalParamsModel.signal_GlobalValueChanged().connect(
      sigc::mem_fun(*this, &ModelStructureCoordinator::whenGlobalValueChanged));

  mp_AddFctModule = new ModelAddFunctionModule(mp_AdvancedDesc->getModel());

  createParamsComponents();
}

// =====================================================================
// =====================================================================

sigc::signal<void> ModelStructureCoordinator::signal_ModelChanged()
{
  return m_signal_ModelChanged;
}

// =====================================================================
// =====================================================================

void ModelStructureCoordinator::setCurrentFunction(std::string FunctionName)
{
  m_StructureModel.requestSelectionByApp(FunctionName);
  m_ParamsPanel.setCurrentPage(FunctionName);
}

// =====================================================================
// =====================================================================

void ModelStructureCoordinator::update()
{
  if (m_HasToUpdate)
  {
    m_StructureModel.update();

    m_GlobalParamsModel.update();

    for (std::map<std::string, ModelFctParamsComponent*>::iterator it =
        m_ByNameFctParamsComponents.begin();
        it != m_ByNameFctParamsComponents.end(); ++it)
      it->second->getModel()->updateParamsValues();
  }
}

// =====================================================================
// =====================================================================

void ModelStructureCoordinator::createParamsComponents()
{
  const std::list<openfluid::fluidx::ModelItemDescriptor*>& Items =
      mp_AdvancedDesc->getModel().getItems();

  for (std::list<openfluid::fluidx::ModelItemDescriptor*>::const_iterator it =
      Items.begin(); it != Items.end(); ++it)
  {
    createModelFctParamsComponent(*it);
  }
}

// =====================================================================
// =====================================================================

void ModelStructureCoordinator::clearParamsComponents()
{
  std::set<std::string> ParamsComponentsToDelete;

  for (std::map<std::string, ModelFctParamsComponent*>::iterator it =
      m_ByNameFctParamsComponents.begin();
      it != m_ByNameFctParamsComponents.end(); ++it)
    ParamsComponentsToDelete.insert(it->first);

  for (std::set<std::string>::iterator it = ParamsComponentsToDelete.begin();
      it != ParamsComponentsToDelete.end(); ++it)
    eraseModelFctParamsComponent(*it);
}

// =====================================================================
// =====================================================================

void ModelStructureCoordinator::updateWithFctParamsComponents()
{
  std::string SelectedPageName = m_ParamsPanel.getCurrentPageName();

  update();

  clearParamsComponents();
  createParamsComponents();

  mp_AddFctModule->setSignatures(
      *openfluid::ware::FunctionSignatureRegistry::getInstance());

  m_ParamsPanel.setCurrentPage(SelectedPageName);
  m_StructureModel.requestSelectionByAppAt(0);
}

// =====================================================================
// =====================================================================

void ModelStructureCoordinator::createModelFctParamsComponent(
    openfluid::fluidx::ModelItemDescriptor* Item)
{
  openfluid::fluidx::AdvancedModelDescriptor* Model = &(mp_AdvancedDesc->getModel());
  ModelFctParamsComponent* FctParams = new ModelFctParamsComponent(Item,
                                                                   *Model);

  std::string FctName = Model->getOrderedIDs()[Model->getFirstItemIndex(Item)];

  m_ParamsPanel.addAFctParamsPage(FctParams->asWidget(), FctName);
  m_ByNameFctParamsComponents[FctName] = FctParams;

  FctParams->signal_RequiredFileChanged().connect(
      sigc::mem_fun(*this,
                    &ModelStructureCoordinator::whenRequiredFileChanged));

  FctParams->signal_ParamsChanged().connect(
      sigc::mem_fun(*this, &ModelStructureCoordinator::whenParamsChanged));

  m_ParamsPanel.setCurrentPage(FctName);
}

// =====================================================================
// =====================================================================

void ModelStructureCoordinator::eraseModelFctParamsComponent(
    std::string FunctionId)
{
  m_ParamsPanel.removeAPage(FunctionId);
  m_ByNameFctParamsComponents.erase(FunctionId);
}

// =====================================================================
// =====================================================================

void ModelStructureCoordinator::whenRequiredFileChanged()
{
  m_signal_ModelChanged.emit();
}

// =====================================================================
// =====================================================================

void ModelStructureCoordinator::whenParamsChanged()
{
  m_HasToUpdate = false;
  m_signal_ModelChanged.emit();
  m_HasToUpdate = true;
}

// =====================================================================
// =====================================================================

