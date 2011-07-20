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
 \file DomainStructureAdapter.cpp
 \brief Implements ...

 \author Aline LIBRES <libres@supagro.inra.fr>
 */

#include "DomainStructureAdapter.hpp"

#include "DomainStructureAdapterModel.hpp"
#include "DomainStructureView.hpp"

#include <iostream>

// =====================================================================
// =====================================================================


void DomainStructureAdapter::whenClassSelectionChanged()
{
  if (m_hasClassSelectionToBeStored)
  {
    m_hasIdSelectionToBeStored = false;
    m_Model.setSelectedClass(m_View.getSelectedClassIter());
    m_hasIdSelectionToBeStored = true;
  }

  m_hasIdSelectionToBeStored = false;
  m_View.setUnitsTreeModel(m_Model.getUnitsTreeModel());
  m_hasIdSelectionToBeStored = true;

  m_View.requestUnitSelection(m_Model.getRequestedUnitSelection());
}

// =====================================================================
// =====================================================================


void DomainStructureAdapter::whenUnitSelectionChanged()
{
  if (m_hasIdSelectionToBeStored)
    m_Model.setSelectedUnit(m_View.getSelectedUnitIter());

  m_signal_FromUserSelectionChanged.emit();
}

// =====================================================================
// =====================================================================


void DomainStructureAdapter::whenActivated()
{
  m_signal_Activated.emit();
}

// =====================================================================
// =====================================================================


DomainStructureAdapter::DomainStructureAdapter(
    DomainStructureAdapterModel& Model, DomainStructureView& View) :
  m_Model(Model), m_View(View), m_hasClassSelectionToBeStored(false),
      m_hasIdSelectionToBeStored(false)
{
  m_View.signal_ClassSelectionChanged().connect(sigc::mem_fun(*this,
      &DomainStructureAdapter::whenClassSelectionChanged));

  m_View.signal_UnitSelectionChanged().connect(sigc::mem_fun(*this,
      &DomainStructureAdapter::whenUnitSelectionChanged));

  m_View.signal_Activated().connect(sigc::mem_fun(*this,
      &DomainStructureAdapter::whenActivated));
}

// =====================================================================
// =====================================================================


sigc::signal<void> DomainStructureAdapter::signal_FromUserSelectionChanged()
{
  return m_signal_FromUserSelectionChanged;
}

// =====================================================================
// =====================================================================


sigc::signal<void> DomainStructureAdapter::signal_Activated()
{
  return m_signal_Activated;
}

// =====================================================================
// =====================================================================


void DomainStructureAdapter::setDomainStructure(
    openfluid::core::UnitsListByClassMap_t UnitListByClass)
{
  m_hasClassSelectionToBeStored = false;
  m_Model.setDomainStructure(UnitListByClass);
  m_View.setClassesTreeModel(m_Model.getClassesTreeModel());
  m_hasClassSelectionToBeStored = true;

  m_View.requestClassSelection(m_Model.getRequestedClassSelection());
  m_View.requestUnitSelection(m_Model.getRequestedUnitSelection());

  // to update in the model selections made by the adaptermodel
  m_signal_FromUserSelectionChanged.emit();
}

// =====================================================================
// =====================================================================


std::pair<Gtk::TreeIter, Gtk::TreeIter> DomainStructureAdapter::getSelectedUnitIters()
{
  return std::make_pair(m_View.getSelectedClassIter(),
      m_View.getSelectedUnitIter());
}

// =====================================================================
// =====================================================================


std::pair<std::string, int> DomainStructureAdapter::getSelectedUnitInfos()
{
  return std::make_pair(m_Model.getClassNameFromIter(
      m_View.getSelectedClassIter()), m_Model.getUnitIdFromIter(
      m_View.getSelectedUnitIter()));
}

// =====================================================================
// =====================================================================


void DomainStructureAdapter::deleteCurrentUnit()
{
  m_Model.deleteUnit(getSelectedUnitIters());
  m_View.requestClassSelection(m_Model.getRequestedClassSelection());
}

// =====================================================================
// =====================================================================


void DomainStructureAdapter::addUnit(openfluid::core::Unit& Unit)
{
  m_Model.addUnit(Unit);
  m_View.requestClassSelection(m_Model.getRequestedClassSelection());
  m_View.setUnitsTreeModel(m_Model.getUnitsTreeModel());
  m_View.requestUnitSelection(m_Model.getRequestedUnitSelection());
}

