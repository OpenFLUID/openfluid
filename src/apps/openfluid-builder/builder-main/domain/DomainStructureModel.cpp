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
 \file DomainStructureModel.cpp
 \brief Implements ...

 \author Aline LIBRES <libres@supagro.inra.fr>
 */

#include "DomainStructureModel.hpp"

// =====================================================================
// =====================================================================


bool DomainStructureModelImpl::isCoreRepos()
{
  if (!mp_CoreRepos)
  {
    throw openfluid::base::OFException("OpenFLUID Builder",
        "DomainStructureModel::isCoreRepos",
        "no Core Repository. Operation is impossible.");
    return false;
  }
  return true;
}

// =====================================================================
// =====================================================================


openfluid::core::CoreRepository* DomainStructureModelImpl::getCoreRepos()
{
  if (isCoreRepos())
    return mp_CoreRepos;
  return 0;
}

// =====================================================================
// =====================================================================


void DomainStructureModelImpl::deleteUnit(openfluid::core::Unit* Unit)
{
  mp_CoreRepos->deleteUnit(Unit);
  m_signal_FromAppUnitDeleted.emit();
}

// =====================================================================
// =====================================================================


DomainStructureModelImpl::DomainStructureModelImpl() :
  mp_CoreRepos(0), mp_SelectedUnit(0), m_SelectedClass("")
{
}

// =====================================================================
// =====================================================================


sigc::signal<void> DomainStructureModelImpl::signal_FromAppDomainChanged()
{
  return m_signal_FromAppDomainChanged;
}

// =====================================================================
// =====================================================================


sigc::signal<void> DomainStructureModelImpl::signal_FromAppUnitDeleted()
{
  return m_signal_FromAppUnitDeleted;
}

// =====================================================================
// =====================================================================


sigc::signal<void, openfluid::core::Unit&> DomainStructureModelImpl::signal_FromAppUnitAdded()
{
  return m_signal_FromAppUnitAdded;
}

// =====================================================================
// =====================================================================


sigc::signal<void> DomainStructureModelImpl::signal_FromUserSelectionChanged()
{
  return m_signal_FromUserSelectionChanged;
}

// =====================================================================
// =====================================================================


void DomainStructureModelImpl::setEngineRequirements(
    openfluid::core::CoreRepository& CoreRepos)
{
  mp_CoreRepos = &CoreRepos;
  update();
}

// =====================================================================
// =====================================================================


openfluid::core::UnitsListByClassMap_t DomainStructureModelImpl::getUnitListByClass()
{
  // warn : copies only base info of units, not relations
  openfluid::core::UnitsListByClassMap_t UnitsMapWithNoEmpty;

  if (mp_CoreRepos)
  {
    openfluid::core::UnitsListByClassMap_t UnitsMap =
        *(getCoreRepos()->getUnitsByClass());
    for (openfluid::core::UnitsListByClassMap_t::iterator it = UnitsMap.begin(); it
        != UnitsMap.end(); ++it)
    {
      if (!it->second.getList()->empty())
        UnitsMapWithNoEmpty[it->first] = it->second;
    }
  }
  return UnitsMapWithNoEmpty;
}

// =====================================================================
// =====================================================================


bool DomainStructureModelImpl::isEmpty()
{
  return getUnitListByClass().empty();
}

// =====================================================================
// =====================================================================


void DomainStructureModelImpl::addUnit(openfluid::core::Unit* Unit)
{
  if (Unit)
  {
    mp_CoreRepos->addUnit(*Unit);
    openfluid::core::Unit* createdUnit = mp_CoreRepos->getUnit(
        Unit->getClass(), Unit->getID());
    delete Unit; // Corerepos.addUnit create new Unit ptr
    m_signal_FromAppUnitAdded.emit(*createdUnit);
  }
}

// =====================================================================
// =====================================================================


void DomainStructureModelImpl::deleteSelectedUnit()
{
  if (mp_SelectedUnit)
  {
    deleteUnit(mp_SelectedUnit);
  }
}

// =====================================================================
// =====================================================================


void DomainStructureModelImpl::setCurrentSelectionByUser(std::pair<std::string,
    int> UnitInfos)
{
  m_SelectedClass = UnitInfos.first;

  openfluid::core::Unit* Unit;
  if (isCoreRepos() && (Unit = getCoreRepos()->getUnit(UnitInfos.first,
      UnitInfos.second)))
    mp_SelectedUnit = Unit;
  else
    mp_SelectedUnit = 0;

  m_signal_FromUserSelectionChanged.emit();
}

// =====================================================================
// =====================================================================


openfluid::core::Unit* DomainStructureModelImpl::getSelectedUnit()
{
  return mp_SelectedUnit;
}

// =====================================================================
// =====================================================================


std::string DomainStructureModelImpl::getSelectedClass()
{
  return m_SelectedClass;
}

// =====================================================================
// =====================================================================


void DomainStructureModelImpl::update()
{
  m_signal_FromAppDomainChanged.emit();
}

// =====================================================================
// =====================================================================

// =====================================================================
// =====================================================================


openfluid::core::CoreRepository* DomainStructureModelSub::getCoreRepos()
{
  return DomainStructureModelImpl::getCoreRepos();
}

// =====================================================================
// =====================================================================


void DomainStructureModelSub::deleteUnit(openfluid::core::Unit* Unit)
{
  DomainStructureModelImpl::deleteUnit(Unit);
}

