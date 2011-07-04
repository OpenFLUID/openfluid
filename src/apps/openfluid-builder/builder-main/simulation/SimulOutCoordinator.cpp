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
 \file SimulOutCoordinator.cpp
 \brief Implements ...

 \author Aline LIBRES <libres@supagro.inra.fr>
 */

#include "SimulOutCoordinator.hpp"

#include <glibmm/i18n.h>

#include "SimulOutFilesAddEditDialog.hpp"
#include "SimulOutSetsAddEditDialog.hpp"
#include "BuilderListToolBox.hpp"
#include "EngineHelper.hpp"

#include <openfluid/guicommon/DialogBoxFactory.hpp>

// =====================================================================
// =====================================================================


SimulOutCoordinator::SimulOutCoordinator(SimulOutFilesModel& OutFilesModel,
    SimulOutFilesAddEditDialog& OutFilesDialog,
    BuilderListToolBox& OutFilesListToolBox, SimulOutSetsModel& OutSetsModel,
    SimulOutSetsAddEditDialog& OutSetsDialog,
    BuilderListToolBox& OutSetsListToolBox) :
  m_OutFilesModel(OutFilesModel), m_OutFilesDialog(OutFilesDialog),
      m_OutFilesListToolBox(OutFilesListToolBox), m_OutSetsModel(OutSetsModel),
      m_OutSetsDialog(OutSetsDialog), m_OutSetsListToolBox(OutSetsListToolBox),
      mp_CoreRepos(0), mp_ModelInstance(0), m_HasToUpdate(true)
{
  m_OutFilesModel.signal_FromUserSelectionChanged().connect(sigc::mem_fun(
      *this, &SimulOutCoordinator::whenFromUserFileFormatSelectionChanged));
  m_OutFilesListToolBox.signal_AddCommandAsked().connect(sigc::mem_fun(*this,
      &SimulOutCoordinator::whenAddFileFormatAsked));
  m_OutFilesListToolBox.signal_RemoveCommandAsked().connect(sigc::mem_fun(
      *this, &SimulOutCoordinator::whenRemoveFileFormatAsked));
  m_OutFilesListToolBox.signal_EditCommandAsked().connect(sigc::mem_fun(*this,
      &SimulOutCoordinator::whenEditFileFormatAsked));

  m_OutSetsModel.signal_FromUserSelectionChanged().connect(sigc::mem_fun(*this,
      &SimulOutCoordinator::whenFromUserSetSelectionChanged));
  m_OutSetsListToolBox.signal_AddCommandAsked().connect(sigc::mem_fun(*this,
      &SimulOutCoordinator::whenAddSetAsked));
  m_OutSetsListToolBox.signal_RemoveCommandAsked().connect(sigc::mem_fun(*this,
      &SimulOutCoordinator::whenRemoveSetAsked));
  m_OutSetsListToolBox.signal_EditCommandAsked().connect(sigc::mem_fun(*this,
      &SimulOutCoordinator::whenEditSetAsked));
}

// =====================================================================
// =====================================================================


void SimulOutCoordinator::setEngineRequirements(
    openfluid::machine::ModelInstance& ModelInstance,
    openfluid::machine::SimulationBlob& SimBlob)
{
  mp_CoreRepos = &SimBlob.getCoreRepository();
  mp_ModelInstance = &ModelInstance;

  m_OutFilesModel.setEngineRequirements(SimBlob.getOutputDescriptor());
  m_OutFilesDialog.setEngineRequirements(SimBlob.getOutputDescriptor());

  m_OutSetsModel.setEngineRequirements(SimBlob.getOutputDescriptor(),
      SimBlob.getCoreRepository(), ModelInstance);
  m_OutSetsDialog.setEngineRequirements(SimBlob.getOutputDescriptor(),
      SimBlob.getCoreRepository(), ModelInstance);

  update();
}

// =====================================================================
// =====================================================================


void SimulOutCoordinator::update()
{
  if (m_HasToUpdate)
  {
    m_OutFilesModel.update();

    m_OutSetsModel.update();
    m_OutSetsDialog.update();

    updateOutFilesListToolBox();
    updateOutSetsListToolBox();
  }
}

// =====================================================================
// =====================================================================


void SimulOutCoordinator::whenAddFileFormatAsked()
{
  openfluid::base::OutputFilesDescriptor* FilesDesc = m_OutFilesDialog.show();

  if (FilesDesc)
  {
    m_OutFilesModel.addFileFormat(FilesDesc);

    update();

    m_HasToUpdate = false;
    m_signal_SimulOutChanged.emit();
    m_HasToUpdate = true;
  }
}

// =====================================================================
// =====================================================================


void SimulOutCoordinator::whenRemoveFileFormatAsked()
{
  if (m_OutFilesModel.selectedFileFormatHasSets()
      && !openfluid::guicommon::DialogBoxFactory::showSimpleOkCancelQuestionDialog(
          _(
              "This will delete the sets associated to this format.\nDo you want to continue ?")))
    return;

  m_OutFilesModel.deleteSelectedFileFormat();

  update();

  m_HasToUpdate = false;
  m_signal_SimulOutChanged.emit();
  m_HasToUpdate = true;
}

// =====================================================================
// =====================================================================


void SimulOutCoordinator::whenEditFileFormatAsked()
{
  if (m_OutFilesDialog.show(m_OutFilesModel.getSelectedFileFormat()))
  {
    update();

    m_HasToUpdate = false;
    m_signal_SimulOutChanged.emit();
    m_HasToUpdate = true;
  }
}

// =====================================================================
// =====================================================================


void SimulOutCoordinator::whenAddSetAsked()
{
  if (m_OutFilesModel.isOutputEmpty())
  {
    openfluid::guicommon::DialogBoxFactory::showSimpleWarningMessage(
        _("Impossible to create a Set :\nno File Format available."));
    return;
  }

  if (mp_CoreRepos->getUnitsGlobally()->empty())
  {
    openfluid::guicommon::DialogBoxFactory::showSimpleWarningMessage(
        _("Impossible to create a Set :\nno Unit available."));
    return;
  }

  if (!EngineHelper::hasAtLeastAProducedVariable(mp_ModelInstance, mp_CoreRepos))
  {
    openfluid::guicommon::DialogBoxFactory::showSimpleWarningMessage(
        _("Impossible to create a Set :\nno Variable available."));
    return;
  }

  std::pair<openfluid::base::OutputSetDescriptor*, std::string> NewSetInfo =
      m_OutSetsDialog.show();

  if (NewSetInfo.first)
  {
    m_OutSetsModel.addSet(NewSetInfo.first, NewSetInfo.second);

    update();

    m_HasToUpdate = false;
    m_signal_SimulOutChanged.emit();
    m_HasToUpdate = true;
  }
}

// =====================================================================
// =====================================================================


void SimulOutCoordinator::whenRemoveSetAsked()
{
  m_OutSetsModel.deleteSelectedSet();

  update();

  m_HasToUpdate = false;
  m_signal_SimulOutChanged.emit();
  m_HasToUpdate = true;
}

// =====================================================================
// =====================================================================


void SimulOutCoordinator::whenEditSetAsked()
{
  std::pair<openfluid::base::OutputSetDescriptor*, std::string> NewSetInfo =
      m_OutSetsDialog.show(m_OutSetsModel.getSelectedSet());

  if (NewSetInfo.first)
  {
    m_OutSetsModel.updateSelectedSet(NewSetInfo.first, NewSetInfo.second);

    update();

    m_HasToUpdate = false;
    m_signal_SimulOutChanged.emit();
    m_HasToUpdate = true;
  }
}

// =====================================================================
// =====================================================================


void SimulOutCoordinator::whenFromUserFileFormatSelectionChanged()
{
  updateOutFilesListToolBox();
}

// =====================================================================
// =====================================================================


void SimulOutCoordinator::whenFromUserSetSelectionChanged()
{
  updateOutSetsListToolBox();
}

// =====================================================================
// =====================================================================


void SimulOutCoordinator::updateOutFilesListToolBox()
{
  m_OutFilesListToolBox.setRemoveCommandAvailable(
      m_OutFilesModel.getSelectedFileFormat());
  m_OutFilesListToolBox.setEditCommandAvailable(
      m_OutFilesModel.getSelectedFileFormat());
}

// =====================================================================
// =====================================================================


void SimulOutCoordinator::updateOutSetsListToolBox()
{
  m_OutSetsListToolBox.setRemoveCommandAvailable(
      m_OutSetsModel.getSelectedSet());
  m_OutSetsListToolBox.setEditCommandAvailable(m_OutSetsModel.getSelectedSet());
}

// =====================================================================
// =====================================================================


sigc::signal<void> SimulOutCoordinator::signal_SimulOutChanged()
{
  return m_signal_SimulOutChanged;
}
