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
 \file DomainUnitEditionModel.cpp
 \brief Implements ...

 \author Aline LIBRES <libres@supagro.inra.fr>
 */

#include "DomainUnitEditionModel.hpp"

#include <glibmm/i18n.h>

#include "BuilderAppDialogFactory.hpp"

void DomainUnitEditionModelImpl::initialize()
{
  mp_Unit = 0;

  m_ClassNames.clear();

  m_ProcessOrder = 1;

  m_RelationsByNameMap.clear();
  m_RelationsByNameMap["Froms"];
  m_RelationsByNameMap["Tos"];
  m_RelationsByNameMap["Parents"];
  m_RelationsByNameMap["Children"];

  m_SelectedRelation = std::make_pair("", std::make_pair("", 0));
}
void DomainUnitEditionModelImpl::extractClassNames()
{
  m_ClassNames.clear();

  const openfluid::core::UnitsListByClassMap_t* UnitsByClass =
      mp_CoreRepos->getUnitsByClass();
  for (openfluid::core::UnitsListByClassMap_t::const_iterator it =
      UnitsByClass->begin(); it != UnitsByClass->end(); ++it)
  {
    m_ClassNames.push_back(it->first);
  }
}
void DomainUnitEditionModelImpl::extractRelation(std::string RelationName,
    openfluid::core::UnitsPtrList_t* UnitsPtrList)
{
  if (UnitsPtrList)
    m_RelationsByNameMap[RelationName].insert(
        m_RelationsByNameMap[RelationName].end(), UnitsPtrList->begin(),
        UnitsPtrList->end());
}
void DomainUnitEditionModelImpl::extractUnitAttributes()
{
  m_ProcessOrder = mp_Unit->getProcessOrder();

  for (unsigned int i = 0; i < m_ClassNames.size(); i++)
  {
    std::string ClassName = m_ClassNames[i];

    openfluid::core::UnitsPtrList_t* Froms = mp_Unit->getFromUnits(ClassName);
    openfluid::core::UnitsPtrList_t* Tos = mp_Unit->getToUnits(ClassName);
    openfluid::core::UnitsPtrList_t* Parents = mp_Unit->getParentUnits(
        ClassName);
    openfluid::core::UnitsPtrList_t* Children = mp_Unit->getChildrenUnits(
        ClassName);

    extractRelation("Froms", Froms);
    extractRelation("Tos", Tos);
    extractRelation("Parents", Parents);
    extractRelation("Children", Children);
  }
}
DomainUnitEditionModelImpl::DomainUnitEditionModelImpl()
{
  initialize();
}
sigc::signal<void> DomainUnitEditionModelImpl::signal_InitAsked()
{
  return m_signal_InitAsked;
}
sigc::signal<void> DomainUnitEditionModelImpl::signal_ShowDialogAsked()
{
  return m_signal_ShowDialogAsked;
}
sigc::signal<void> DomainUnitEditionModelImpl::signal_SelectedRelationRemoved()
{
  return m_signal_SelectedRelationRemoved;
}
sigc::signal<void> DomainUnitEditionModelImpl::signal_RelationAdded()
{
  return m_signal_RelationAdded;
}
void DomainUnitEditionModelImpl::setEngineRequirements(
    openfluid::core::CoreRepository& CoreRepos)
{
  mp_CoreRepos = &CoreRepos;
}
void DomainUnitEditionModelImpl::initialize(openfluid::core::Unit& SelectedUnit)
{
  initialize();
  extractClassNames();

  mp_Unit = &SelectedUnit;
  extractUnitAttributes();

  m_signal_InitAsked.emit();
}
void DomainUnitEditionModelImpl::showDialog()
{
  m_signal_ShowDialogAsked.emit();
}
void DomainUnitEditionModelImpl::setProcessOrder(int PcsOrder)
{
  m_ProcessOrder = PcsOrder;
  mp_Unit->setProcessOrder(m_ProcessOrder);
}
std::string DomainUnitEditionModelImpl::getClassName()
{
  return mp_Unit->getClass();
}
int DomainUnitEditionModelImpl::getId()
{
  return mp_Unit->getID();
}
int DomainUnitEditionModelImpl::getProcessOrder()
{
  return m_ProcessOrder;
}
std::map<std::string, openfluid::core::UnitsPtrList_t> DomainUnitEditionModelImpl::getRelationsByName()
{
  return m_RelationsByNameMap;
}
void DomainUnitEditionModelImpl::setSelectedRelation(std::pair<std::string,
    std::pair<std::string, int> > SelectedRelation)
{
  m_SelectedRelation = SelectedRelation;
}
void DomainUnitEditionModelImpl::removeSelectedRelation()
{
  openfluid::core::Unit* SelectedUnit = mp_CoreRepos->getUnit(
      m_SelectedRelation.second.first, m_SelectedRelation.second.second);

  if (SelectedUnit)
  {
    bool Removed = false;
    if (m_SelectedRelation.first == "Froms")
    {
      mp_CoreRepos ->removeFromToConnection(SelectedUnit, mp_Unit);
      Removed = true;
    } else if (m_SelectedRelation.first == "Tos")
    {
      mp_CoreRepos ->removeFromToConnection(mp_Unit, SelectedUnit);
      Removed = true;
    } else if (m_SelectedRelation.first == "Parents")
    {
      mp_CoreRepos ->removeChildParentConnection(mp_Unit, SelectedUnit);
      Removed = true;
    } else if (m_SelectedRelation.first == "Children")
    {
      mp_CoreRepos ->removeChildParentConnection(SelectedUnit, mp_Unit);
      Removed = true;
    }

    if (Removed == true)
    {
      m_RelationsByNameMap[m_SelectedRelation.first].remove(SelectedUnit);
      m_signal_SelectedRelationRemoved.emit();
    }
  }
}
void DomainUnitEditionModelImpl::addRelationAsked(std::string RelationName,
    std::string AddClassName, int AddId)
{
  openfluid::core::Unit* RequiredUnit = mp_CoreRepos->getUnit(AddClassName,
      AddId);

  if (RequiredUnit)
  {
    bool Removed = false;
    if (RelationName == "Froms")
    {
      mp_Unit->addFromUnit(RequiredUnit);
      RequiredUnit->addToUnit(mp_Unit);
      Removed = true;
    } else if (RelationName == "Tos")
    {
      mp_Unit->addToUnit(RequiredUnit);
      RequiredUnit->addFromUnit(mp_Unit);
      Removed = true;
    } else if (RelationName == "Parents")
    {
      mp_Unit->addParentUnit(RequiredUnit);
      RequiredUnit->addChildUnit(mp_Unit);
      Removed = true;
    } else if (RelationName == "Children")
    {
      mp_Unit->addChildUnit(RequiredUnit);
      RequiredUnit->addParentUnit(mp_Unit);
      Removed = true;
    }

    if (Removed == true)
    {
      m_RelationsByNameMap[RelationName].push_back(RequiredUnit);
      m_signal_RelationAdded.emit();
    }
  } else
  {
    BuilderAppDialogFactory::showSimpleErrorMessage(_(
        "This Unit doesn't exist."));
  }
}

std::pair<std::string, std::pair<std::string, int> > DomainUnitEditionModelSub::getSelectedRelation()
{
  return m_SelectedRelation;
}
