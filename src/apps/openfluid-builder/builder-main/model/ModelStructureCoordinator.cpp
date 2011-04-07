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

#include "ModelAvailFctModel.hpp"
#include "ModelFctDetailModel.hpp"
#include "ModelStructureModel.hpp"
#include "ModelGlobalParamsModel.hpp"
#include "BuilderListToolBox.hpp"
#include "ModelAddFunctionModule.hpp"


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
  if (m_StructureModel.getCurrentSelectionSignature())
    m_FctDetailModel.setFctToDisplay(
        m_StructureModel.getCurrentSelectionSignature());
  else
    m_FctDetailModel.setFctToDisplay(
        FunctionSignatureRegistry::getEmptyPluggableSignature());

  updateStructureListToolBox();
}

// =====================================================================
// =====================================================================


void ModelStructureCoordinator::whenAddFctAsked()
{
  openfluid::machine::SignatureItemInstance* Signature = mp_AddFctModule->showDialog();

  if (Signature)
  {
    m_StructureModel.appendFunction(*Signature);
    updateStructureListToolBox();
    m_signal_ModelChanged.emit();
  }
}

// =====================================================================
// =====================================================================


void ModelStructureCoordinator::whenRemoveFctAsked()
{
  m_StructureModel.removeFunctionAt(m_StructureModel.getCurrentSelection());
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


ModelStructureCoordinator::ModelStructureCoordinator(
    ModelFctDetailModel& FctDetailModel, ModelStructureModel& StructureModel,
    ModelGlobalParamsModel& GlobalParamsModel,
    BuilderListToolBox& StructureListToolBox) :
  m_FctDetailModel(FctDetailModel), m_StructureModel(StructureModel),
      m_GlobalParamsModel(GlobalParamsModel), m_StructureListToolBox(
          StructureListToolBox)
{
  m_StructureListToolBox.setAddCommandAvailable(true);

  m_StructureModel.signal_FromUserSelectionChanged().connect(sigc::mem_fun(
      *this, &ModelStructureCoordinator::whenStructureFctSelectionChanged));

  m_StructureListToolBox.signal_AddCommandAsked().connect(sigc::mem_fun(*this,
      &ModelStructureCoordinator::whenAddFctAsked));
  m_StructureListToolBox.signal_RemoveCommandAsked().connect(sigc::mem_fun(
      *this, &ModelStructureCoordinator::whenRemoveFctAsked));
  m_StructureListToolBox.signal_UpCommandAsked().connect(sigc::mem_fun(*this,
      &ModelStructureCoordinator::whenMoveTowardTheBeginAsked));
  m_StructureListToolBox.signal_DownCommandAsked().connect(sigc::mem_fun(*this,
      &ModelStructureCoordinator::whenMoveTowardTheEndAsked));

  mp_AddFctModule = new ModelAddFunctionModule();
}

// =====================================================================
// =====================================================================

sigc::signal<void> ModelStructureCoordinator::signal_ModelChanged()
{
  return m_signal_ModelChanged;
}

// =====================================================================
// =====================================================================


void ModelStructureCoordinator::setSignatures(
    FunctionSignatureRegistry& Signatures)
{
  mp_SignatureRegistry = &Signatures;

  m_GlobalParamsModel.setSignatures(Signatures);
}

// =====================================================================
// =====================================================================


void ModelStructureCoordinator::setEngineRequirements(
    openfluid::machine::ModelInstance& ModelInstance,
    openfluid::machine::SimulationBlob& SimBlob)
{
  mp_ModelInstance = &ModelInstance;
  mp_SimBlob = &SimBlob;

  m_StructureModel.setEngineRequirements(ModelInstance);
  m_GlobalParamsModel.setEngineRequirements(ModelInstance);
}

// =====================================================================
// =====================================================================


void ModelStructureCoordinator::setCurrentFunction(std::string FunctionName)
{
  m_StructureModel.requestSelectionByApp(FunctionName);
}


// =====================================================================
// =====================================================================


void ModelStructureCoordinator::update()
{
  m_StructureModel.update();
}
