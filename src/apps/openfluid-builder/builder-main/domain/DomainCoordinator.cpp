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
 \file DomainCoordinator.cpp
 \brief Implements ...

 \author Aline LIBRES <libres@supagro.inra.fr>
 */

#include "DomainCoordinator.hpp"

#include <glibmm/i18n.h>

#include "DomainStructureModel.hpp"
#include "DomainUnitEditionModel.hpp"
#include "DomainUnitCreationModel.hpp"
#include "DomainIDataModel.hpp"
#include "BuilderListToolBox.hpp"
#include <openfluid/guicommon/DialogBoxFactory.hpp>


void DomainCoordinator::updateStructureListToolBox()
{
  m_StructureListToolBox.setRemoveCommandAvailable(
      m_StructureModel.getSelectedUnit());
  m_StructureListToolBox.setEditCommandAvailable(
      m_StructureModel.getSelectedUnit());
}
void DomainCoordinator::updateIDataListToolBox()
{
  m_IDataListToolBox.setAddCommandAvailable(m_IDataModel.getSelectedClass()
      != "");
  m_IDataListToolBox.setRemoveCommandAvailable(m_IDataModel.getSelectedClass()
      != "");
}
void DomainCoordinator::whenAddUnitAsked()
{
  m_UnitCreationModel.initialize(m_StructureModel.getSelectedClass());
  m_UnitCreationModel.showDialog();
  m_StructureModel.addUnit(m_UnitCreationModel.getUnit());
  delete m_UnitCreationModel.getUnit(); // Corerepos.addUnit create new Unit ptr. //TODO: use smart ptr
  updateStructureListToolBox();
  m_IDataModel.update();
  updateIDataListToolBox();
}
void DomainCoordinator::whenRemoveUnitAsked()
{
  m_StructureModel.deleteSelectedUnit();
  updateStructureListToolBox();
  m_IDataModel.update();
  updateIDataListToolBox();
}
void DomainCoordinator::whenEditUnitAsked()
{
  m_UnitEditionModel.initialize(*m_StructureModel.getSelectedUnit());
  m_UnitEditionModel.showDialog();
  m_StructureModel.alterSelectedUnit();
}
void DomainCoordinator::whenStructureSelectionChanged()
{
  updateStructureListToolBox();
}
void DomainCoordinator::whenAddIDataAsked()
{
  std::string DataName = openfluid::guicommon::DialogBoxFactory::showTextEntryDialog(
      _("Adding data to the selected class"), _("Data Name : "));
  m_IDataModel.addData(DataName);
}
void DomainCoordinator::whenRemoveIDataAsked()
{
  std::string
      DataName =
          openfluid::guicommon::DialogBoxFactory::showTextEntryDialog(
              _("Removing data from the selected class\nAll values of this data will be destroyed"),
              _("Data Name : "));
  m_IDataModel.removeData(DataName);
}

DomainCoordinator::DomainCoordinator(DomainStructureModel& StructureModel,
    DomainUnitEditionModel& UnitEditionModel,
    DomainUnitCreationModel& UnitCreationModel,
    BuilderListToolBox& StructureListToolBox, DomainIDataModel& IDataModel,
    BuilderListToolBox& IDataListToolBox) :
  m_StructureModel(StructureModel), m_UnitEditionModel(UnitEditionModel),
      m_UnitCreationModel(UnitCreationModel), m_StructureListToolBox(
          StructureListToolBox), m_IDataModel(IDataModel), m_IDataListToolBox(
          IDataListToolBox)
{
  m_StructureListToolBox.signal_AddCommandAsked().connect(sigc::mem_fun(*this,
      &DomainCoordinator::whenAddUnitAsked));
  m_StructureListToolBox.signal_RemoveCommandAsked().connect(sigc::mem_fun(
      *this, &DomainCoordinator::whenRemoveUnitAsked));
  m_StructureListToolBox.signal_EditCommandAsked().connect(sigc::mem_fun(*this,
      &DomainCoordinator::whenEditUnitAsked));

  m_StructureModel.signal_FromUserSelectionChanged().connect(sigc::mem_fun(
      *this, &DomainCoordinator::whenStructureSelectionChanged));

  m_IDataListToolBox.signal_AddCommandAsked().connect(sigc::mem_fun(*this,
      &DomainCoordinator::whenAddIDataAsked));
  m_IDataListToolBox.signal_RemoveCommandAsked().connect(sigc::mem_fun(*this,
      &DomainCoordinator::whenRemoveIDataAsked));
}
void DomainCoordinator::setEngineRequirements(
    openfluid::core::CoreRepository& CoreRepos)
{
  m_StructureModel.setEngineRequirements(CoreRepos);
  m_UnitEditionModel.setEngineRequirements(CoreRepos);
  m_UnitCreationModel.setEngineRequirements(CoreRepos);
  m_IDataModel.setEngineRequirements(CoreRepos);
  updateStructureListToolBox();
  updateIDataListToolBox();
}
