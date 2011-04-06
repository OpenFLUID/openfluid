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

#include "SimulOutFileDescModel.hpp"

#include "BuilderListToolBox.hpp"
#include "BuilderAppDialogFactory.hpp"

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


void SimulOutCoordinator::whenAddFileFormatAsked()
{
  m_OutFileDescModel.initialize(0, m_OutFilesModel.generateNextFormatName(),
      m_OutFilesModel.getFilesFormatsByNameVect());
  m_OutFileDescModel.showDialog();
  m_OutFilesModel.addFileFormat(m_OutFileDescModel.getFileDesc(),
      m_OutFileDescModel.getFormatName());

  updateOutFilesListToolBox();
  m_signal_SimulOutChanged.emit();
}

// =====================================================================
// =====================================================================


void SimulOutCoordinator::whenAddSetAsked()
{
  m_OutSetDescModel.initialize(0, "",
      m_OutFilesModel.getFilesFormatsByNameVect(),
      m_OutSetsModel.getSetsByName());

  if (!m_OutFilesModel.getFilesFormatsByNameVect().empty()
      && !m_OutSetDescModel.getClasses().empty())
  {
    m_OutSetDescModel.showDialog();
    m_OutSetsModel.addSet(m_OutSetDescModel.getSetDesc(),
        m_OutSetDescModel.getFormatName(), m_OutSetDescModel.getFormatIndex());
    updateOutSetsListToolBox();
    m_signal_SimulOutChanged.emit();
  } else
    BuilderAppDialogFactory::showSimpleErrorMessage(
        "Impossible to create a Set :\nYou have to create at least a Unit Class and a Files Format before.");
}

// =====================================================================
// =====================================================================


void SimulOutCoordinator::whenRemoveFileFormatAsked()
{
  m_OutFilesModel.deleteSelectedFileFormat();
  updateOutFilesListToolBox();
  m_OutSetsModel.updateFileFormats(m_OutFilesModel.getFilesFormatsByNameVect());
  updateOutSetsListToolBox();
  m_signal_SimulOutChanged.emit();
}

// =====================================================================
// =====================================================================


void SimulOutCoordinator::whenRemoveSetAsked()
{
  m_OutSetsModel.deleteSelectedSet();
  updateOutSetsListToolBox();
  m_signal_SimulOutChanged.emit();
}

// =====================================================================
// =====================================================================


void SimulOutCoordinator::whenEditFileFormatAsked()
{
  m_OutFileDescModel.initialize(m_OutFilesModel.getSelectedFileFormat(),
      m_OutFilesModel.getSelectedFileFormatName(),
      m_OutFilesModel.getFilesFormatsByNameVect());
  m_OutFileDescModel.showDialog();
  m_OutFilesModel.updateSelectedFileFormat(m_OutFileDescModel.getFormatName());
  m_signal_SimulOutChanged.emit();
}

// =====================================================================
// =====================================================================


void SimulOutCoordinator::whenEditSetAsked()
{
  m_OutSetDescModel.initialize(m_OutSetsModel.getSelectedSet(),
      m_OutSetsModel.getSelectedSetFormatName(),
      m_OutFilesModel.getFilesFormatsByNameVect(),
      m_OutSetsModel.getSetsByName());
  m_OutSetDescModel.showDialog();
  m_OutSetsModel.updateSelectedSet(m_OutSetDescModel.getFormatName());
  updateOutSetsListToolBox();
  m_signal_SimulOutChanged.emit();
}

// =====================================================================
// =====================================================================


SimulOutCoordinator::SimulOutCoordinator(SimulOutFilesModel& OutFilesModel,
    SimulOutFileDescModel& FileDescModel,
    BuilderListToolBox& OutFilesListToolBox, SimulOutSetsModel& OutSetsModel,
    SimulOutSetDescModel& SetDescModel, BuilderListToolBox& OutSetsListToolBox) :
  m_OutFilesModel(OutFilesModel), m_OutFileDescModel(FileDescModel),
      m_OutFilesListToolBox(OutFilesListToolBox), m_OutSetsModel(OutSetsModel),
      m_OutSetDescModel(SetDescModel), m_OutSetsListToolBox(OutSetsListToolBox)
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
  m_OutFilesModel.setEngineRequirements(SimBlob.getOutputDescriptor());
  m_OutFileDescModel.setEngineRequirements(SimBlob.getOutputDescriptor());

  m_OutSetsModel.setEngineRequirements(SimBlob.getOutputDescriptor());
  m_OutSetDescModel.setEngineRequirements(SimBlob.getOutputDescriptor(), SimBlob.getCoreRepository(), ModelInstance);

  updateOutFilesListToolBox();
  updateOutSetsListToolBox();
}


// =====================================================================
// =====================================================================


sigc::signal<void> SimulOutCoordinator::signal_SimulOutChanged()
{
  return m_signal_SimulOutChanged;
}
