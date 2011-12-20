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
 \file SimulOutSetsAdapterModel.cpp
 \brief Implements ...

 \author Aline LIBRES <libres@supagro.inra.fr>
 */

#include "SimulOutSetsAdapterModel.hpp"

#include "BuilderListStore.hpp"
#include "EngineHelper.hpp"

// =====================================================================
// =====================================================================


std::string SimulOutSetsAdapterModelImpl::extractIDsString(
    openfluid::base::OutputSetDescriptor OutSetDesc)
{
  if (OutSetDesc.isAllUnits())
    return "*";

  return getStringListFromVect(OutSetDesc.getUnitsIDs());
}

// =====================================================================
// =====================================================================


std::string SimulOutSetsAdapterModelImpl::extractVariablesString(
    openfluid::base::OutputSetDescriptor OutSetDesc,
    openfluid::machine::ModelInstance* ModelInstance)
{
  if(OutSetDesc.isAllVariables())
    return "*";

  return getStringListFromVect(OutSetDesc.getVariables());

}

// =====================================================================
// =====================================================================


SimulOutSetsAdapterModelImpl::SimulOutSetsAdapterModelImpl() :
  m_SelectedRowRef(0)
{
  mref_ListStore = BuilderListStore::create(m_Columns);

  mref_ListStore->set_sort_column(m_Columns.m_Name, Gtk::SORT_ASCENDING);
}

// =====================================================================
// =====================================================================


void SimulOutSetsAdapterModelImpl::setSets(
    openfluid::base::OutputDescriptor* OutDesc,
    openfluid::machine::ModelInstance* ModelInstance)
{
  m_SelectedRowRef = 0;

  mref_ListStore->clear();

  for (unsigned int i = 0; i < OutDesc->getFileSets().size(); i++)
  {
    for (unsigned int j = 0; j < OutDesc->getFileSets()[i].getSets().size(); j++)
    {
      openfluid::base::OutputSetDescriptor SetDesc =
          OutDesc->getFileSets()[i].getSets()[j];

      Gtk::TreeRow Row = *(mref_ListStore->append());

      Row[m_Columns.m_Name] = SetDesc.getName();
      Row[m_Columns.m_UnitsClass] = SetDesc.getUnitsClass();
      Row[m_Columns.m_Precision] = SetDesc.getPrecision();
      Row[m_Columns.m_UnitsIDs] = extractIDsString(SetDesc);
      Row[m_Columns.m_Vars] = extractVariablesString(SetDesc, ModelInstance);
      Row[m_Columns.m_FormatName] = OutDesc->getFileSets()[i].getName();
    }
  }

  mref_ListStore->sort_column_changed();
}

// =====================================================================
// =====================================================================


Glib::RefPtr<Gtk::TreeModel> SimulOutSetsAdapterModelImpl::getTreeModel()
{
  return mref_ListStore;
}

// =====================================================================
// =====================================================================


void SimulOutSetsAdapterModelImpl::setSelectedSet(Gtk::TreeIter SelectedIter)
{
  if (SelectedIter)
    m_SelectedRowRef = mref_ListStore->createRowRefFromIter(SelectedIter);
  else
    m_SelectedRowRef = 0;
}

// =====================================================================
// =====================================================================


std::string SimulOutSetsAdapterModelImpl::getSelectedSetName()
{
  if (m_SelectedRowRef)
    return mref_ListStore->getRowFromRowRef(*m_SelectedRowRef)[m_Columns.m_Name];
  return "";
}

// =====================================================================
// =====================================================================


void SimulOutSetsAdapterModelImpl::setSelectedSetName(std::string SetName)
{
  Gtk::TreeModel::Children Children = mref_ListStore->children();

  for (Gtk::TreeModel::Children::iterator it = Children.begin(); it
      != Children.end(); ++it)
  {
    if (it->get_value(m_Columns.m_Name) == SetName)
    {
      m_SelectedRowRef = mref_ListStore->createRowRefFromIter(it);
      return;
    }
  }
}

// =====================================================================
// =====================================================================


Gtk::TreeRow SimulOutSetsAdapterModelImpl::getSelectedRow()
{
  return mref_ListStore->getRowFromRowRef(*m_SelectedRowRef);
}

// =====================================================================
// =====================================================================


Glib::ustring SimulOutSetsAdapterModelImpl::getStringListFromStringSet(
    std::set<std::string> Vect)
{
  Glib::ustring Str = "";

  if (!Vect.empty())
    Str = *Vect.begin();

  if (Vect.size() > 1)
  {
    std::set<std::string>::iterator it = Vect.begin();
    it++;
    while (it != Vect.end())
    {
      Str += Glib::ustring::compose(";%1", *it);
      ++it;
    }
  }

  return Str;
}

// =====================================================================
// =====================================================================

// =====================================================================
// =====================================================================


Glib::ustring SimulOutSetsAdapterModelSub::getStringListFromStringSet(std::set<
    std::string> Vect)
{
  return SimulOutSetsAdapterModelImpl::getStringListFromStringSet(Vect);
}

// =====================================================================
// =====================================================================


std::string SimulOutSetsAdapterModelSub::extractIDsString(
    openfluid::base::OutputSetDescriptor OutSetDesc)
{
  return SimulOutSetsAdapterModelImpl::extractIDsString(OutSetDesc);
}

// =====================================================================
// =====================================================================


std::string SimulOutSetsAdapterModelSub::extractVariablesString(
    openfluid::base::OutputSetDescriptor OutSetDesc,
    openfluid::machine::ModelInstance* ModelInstance)
{
  return SimulOutSetsAdapterModelImpl::extractVariablesString(OutSetDesc,
      ModelInstance);
}
