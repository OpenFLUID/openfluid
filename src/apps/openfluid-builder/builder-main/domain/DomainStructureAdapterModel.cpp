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
 \file DomainStructureAdapterModel.cpp
 \brief Implements ...

 \author Aline LIBRES <libres@supagro.inra.fr>
 */

#include "DomainStructureAdapterModel.hpp"

#include <boost/foreach.hpp>

#include <openfluid/fluidx/UnitDescriptor.hpp>
#include <openfluid/fluidx/AdvancedDomainDescriptor.hpp>

// =====================================================================
// =====================================================================

void DomainStructureAdapterModelImpl::appendUnitRowToUnitListStore(
    Glib::RefPtr<BuilderListStore> ClassListStore, openfluid::fluidx::UnitDescriptor& Unit)
{
  Gtk::TreeRow Row = *(ClassListStore->append());

  Row[m_Columns.m_Id] = Unit.getUnitID();
  Row[m_Columns.m_PcsOrder] = Unit.getProcessOrder();
}

// =====================================================================
// =====================================================================

void DomainStructureAdapterModelImpl::createUnitStoreForClass(
    std::string ClassName)
{
  Glib::RefPtr<BuilderListStore> List = BuilderListStore::create(m_Columns);
  m_ByClassUnitsStores[ClassName] = List;
}

// =====================================================================
// =====================================================================

bool DomainStructureAdapterModelImpl::isClassNameValid(std::string ClassName)
{
  return m_ByClassUnitsStores.find(ClassName) != m_ByClassUnitsStores.end();
}

// =====================================================================
// =====================================================================

bool DomainStructureAdapterModelImpl::isClassNameEmpty(std::string ClassName)
{
  return m_ByClassUnitsStores[ClassName]->children().empty();
}

// =====================================================================
// =====================================================================

void DomainStructureAdapterModelImpl::setFirstClassSelected()
{
  if (!mref_ClassStore->children().empty())
    m_RequestedSelectedClass = mref_ClassStore->getFirstClassName();
}

// =====================================================================
// =====================================================================

void DomainStructureAdapterModelImpl::setLastClassSelected()
{
  if (!mref_ClassStore->children().empty())
  {
    Gtk::TreeIter it = mref_ClassStore->children().end();
    it--;
    m_RequestedSelectedClass = mref_ClassStore->getClassNameFromIter(it);
  }
}

// =====================================================================
// =====================================================================

void DomainStructureAdapterModelImpl::setFirstUnitSelected(
    std::string ClassName)
{
  if (isClassNameValid(ClassName) && !isClassNameEmpty(ClassName))
    m_ByClassSelectedUnits[ClassName] =
        m_ByClassUnitsStores[ClassName]->children().begin()->get_value(
            m_Columns.m_Id);
}

// =====================================================================
// =====================================================================

void DomainStructureAdapterModelImpl::setLastUnitSelected(std::string ClassName)
{
  if (isClassNameValid(ClassName) && !isClassNameEmpty(ClassName))
  {
    Gtk::TreeIter it = m_ByClassUnitsStores[ClassName]->children().end();
    it--;
    m_ByClassSelectedUnits[ClassName] = it->get_value(m_Columns.m_Id);
  }
}

// =====================================================================
// =====================================================================

DomainStructureAdapterModelImpl::DomainStructureAdapterModelImpl(
    DomainStructureColumns& Columns) :
    m_Columns(Columns), mref_ClassStore(BuilderClassListStore::create()), m_RequestedSelectedClass(
        "")
{
}

// =====================================================================
// =====================================================================

void DomainStructureAdapterModelImpl::setDomainStructure(
    const openfluid::fluidx::AdvancedDomainDescriptor::UnitsByIdByClass_t& UnitListByClass)
{
  // store existing sorts
  std::map<std::string, std::pair<int, Gtk::SortType> > ByClassSorts;

  for (std::map<std::string, Glib::RefPtr<BuilderListStore> >::iterator it =
      m_ByClassUnitsStores.begin(); it != m_ByClassUnitsStores.end(); ++it)
  {
    int SortId;
    Gtk::SortType SortType;

    it->second->get_sort_column_id(SortId, SortType);

    ByClassSorts[it->first] = std::make_pair(SortId, SortType);
  }

  m_ByClassUnitsStores.clear();

  mref_ClassStore->clear();

  std::vector<std::string> ClassNames;

  for (openfluid::fluidx::AdvancedDomainDescriptor::UnitsByIdByClass_t::const_iterator it =
      UnitListByClass.begin(); it != UnitListByClass.end(); ++it)
  {
    std::string ClassName = it->first;

    ClassNames.push_back(ClassName);

    createUnitStoreForClass(ClassName);

    const std::map<int, openfluid::fluidx::BuilderUnit>* Units = &(it->second);

    for (std::map<int, openfluid::fluidx::BuilderUnit>::const_iterator itU = Units->begin();
        itU != Units->end(); ++itU)
    {
      appendUnitRowToUnitListStore(m_ByClassUnitsStores[ClassName],
                                   *itU->second.mp_UnitDesc);
    }

    if (ByClassSorts.find(ClassName) != ByClassSorts.end())
      m_ByClassUnitsStores[ClassName]->set_sort_column(
          ByClassSorts[ClassName].first, ByClassSorts[ClassName].second);
    else
      m_ByClassUnitsStores[ClassName]->set_sort_column(m_Columns.m_Id,
                                                       Gtk::SORT_ASCENDING);

    if (m_ByClassSelectedUnits.find(ClassName) == m_ByClassSelectedUnits.end())
      setFirstUnitSelected(ClassName);
  }

  mref_ClassStore->setClasses(ClassNames);

// if no class already selected, select the first one
  if (!isClassNameValid(m_RequestedSelectedClass))
    setFirstClassSelected();

}

// =====================================================================
// =====================================================================

Glib::RefPtr<Gtk::TreeModel> DomainStructureAdapterModelImpl::getClassesTreeModel()
{
  return mref_ClassStore;
}

// =====================================================================
// =====================================================================

Glib::RefPtr<Gtk::TreeModel> DomainStructureAdapterModelImpl::getUnitsTreeModel()
{
  if (isClassNameValid(m_RequestedSelectedClass))
    return m_ByClassUnitsStores[m_RequestedSelectedClass];
  return Gtk::ListStore::create(m_Columns);
}

// =====================================================================
// =====================================================================

Gtk::TreeIter DomainStructureAdapterModelImpl::getRequestedClassSelection()
{
  return mref_ClassStore->getIterFromClassName(m_RequestedSelectedClass);
}

// =====================================================================
// =====================================================================

Gtk::TreeIter DomainStructureAdapterModelImpl::getRequestedUnitSelection()
{
  if (isClassNameValid(m_RequestedSelectedClass))
  {
    int SelectedUnit = m_ByClassSelectedUnits[m_RequestedSelectedClass];

    Gtk::TreeModel::Children Children =
        m_ByClassUnitsStores[m_RequestedSelectedClass]->children();

    for (unsigned int i = 0; i < Children.size(); i++)
    {
      if (Children[i]->get_value(m_Columns.m_Id) == SelectedUnit)
        return Children[i];
    }
  }

  return (Gtk::TreeIter) 0;
}

// =====================================================================
// =====================================================================

std::string DomainStructureAdapterModelImpl::getClassNameFromIter(
    Gtk::TreeIter Iter)
{
  return mref_ClassStore->getClassNameFromIter(Iter);
}

// =====================================================================
// =====================================================================

int DomainStructureAdapterModelImpl::getUnitIdFromIter(Gtk::TreeIter Iter)
{
  if (Iter)
    return Iter->get_value(m_Columns.m_Id);

  return -1;
}

// =====================================================================
// =====================================================================

void DomainStructureAdapterModelImpl::deleteUnit(
    std::pair<Gtk::TreeIter, Gtk::TreeIter> UnitIters)
{
  if (UnitIters.first && UnitIters.second)
  {
    std::string ClassName = getClassNameFromIter(UnitIters.first);

    Gtk::TreeIter NextUnitIter = m_ByClassUnitsStores[ClassName]->erase(
        UnitIters.second);

    if (isClassNameEmpty(ClassName))
    {
      deleteClass(ClassName);
    }
    else
    {
      if (NextUnitIter != m_ByClassUnitsStores[ClassName]->children().end())
        m_ByClassSelectedUnits[ClassName] = NextUnitIter->get_value(
            m_Columns.m_Id);
      else
        setLastUnitSelected(ClassName);
    }
  }
}

// =====================================================================
// =====================================================================

void DomainStructureAdapterModelImpl::deleteClass(std::string ClassName)
{
  Gtk::TreeIter NextClassIter = mref_ClassStore->erase(
      mref_ClassStore->getIterFromClassName(ClassName));

  m_ByClassUnitsStores.erase(m_ByClassUnitsStores.find(ClassName));

  if (NextClassIter != mref_ClassStore->children().end())
    m_RequestedSelectedClass = mref_ClassStore->getClassNameFromIter(
        NextClassIter);
  else
    setLastClassSelected();
}

// =====================================================================
// =====================================================================

void DomainStructureAdapterModelImpl::addUnit(openfluid::fluidx::UnitDescriptor& Unit)
{
  std::string ClassName = Unit.getUnitClass();

  if (!isClassNameValid(ClassName))
  {
    mref_ClassStore->appendClass(ClassName);

    createUnitStoreForClass(ClassName);
  }

  appendUnitRowToUnitListStore(m_ByClassUnitsStores[ClassName], Unit);

  m_ByClassSelectedUnits[ClassName] = Unit.getUnitID();

  m_RequestedSelectedClass = ClassName;
}

// =====================================================================
// =====================================================================

void DomainStructureAdapterModelImpl::setSelectedClass(Gtk::TreeIter Iter)
{
  m_RequestedSelectedClass = getClassNameFromIter(Iter);
}

// =====================================================================
// =====================================================================

void DomainStructureAdapterModelImpl::setSelectedUnit(Gtk::TreeIter UnitIter)
{
  if (isClassNameValid(m_RequestedSelectedClass))
    m_ByClassSelectedUnits[m_RequestedSelectedClass] = getUnitIdFromIter(
        UnitIter);
}

// =====================================================================
// =====================================================================

void DomainStructureAdapterModelImpl::setNewPcsOrder(
    std::pair<Gtk::TreeIter, Gtk::TreeIter> UnitIters, int NewProcessOrder)
{
  if (UnitIters.second)
    UnitIters.second->set_value(m_Columns.m_PcsOrder, NewProcessOrder);
}

// =====================================================================
// =====================================================================

// =====================================================================
// =====================================================================

DomainStructureAdapterModelSub::DomainStructureAdapterModelSub(
    DomainStructureColumns& Columns) :
    DomainStructureAdapterModelImpl(Columns)
{
}

// =====================================================================
// =====================================================================

std::map<std::string, Glib::RefPtr<BuilderListStore> > DomainStructureAdapterModelSub::getByClassUnitsStores()
{
  return m_ByClassUnitsStores;
}

// =====================================================================
// =====================================================================

std::string DomainStructureAdapterModelSub::getRequestedSelectedClass()
{
  return m_RequestedSelectedClass;
}

// =====================================================================
// =====================================================================

Glib::RefPtr<BuilderClassListStore> DomainStructureAdapterModelSub::getClassStore()
{
  return mref_ClassStore;
}
