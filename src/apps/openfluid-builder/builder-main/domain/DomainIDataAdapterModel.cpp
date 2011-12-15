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
 \file DomainIDataAdapterModel.cpp
 \brief Implements ...

 \author Aline LIBRES <libres@supagro.inra.fr>
 */

#include "DomainIDataAdapterModel.hpp"

#include <boost/foreach.hpp>

#include <openfluid/core/UnitsColl.hpp>
#include <openfluid/core/Unit.hpp>

#include "DomainIDataColumns.hpp"
#include "BuilderListStore.hpp"

// =====================================================================
// =====================================================================


DomainIDataAdapterModelImpl::DomainIDataAdapterModelImpl() :
  mp_UnitsColl(0), mp_Columns(new DomainIDataColumns()), mref_ListStore(
      BuilderListStore::create(*mp_Columns)), m_SelectedUnit(-1)
{
}

// =====================================================================
// =====================================================================


void DomainIDataAdapterModelImpl::dataInit(
    openfluid::core::UnitsCollection* UnitsColl)
{
  // store existing sort
  int SortColumnId;
  Gtk::SortType SortType;

  mref_ListStore->get_sort_column_id(SortColumnId, SortType);

  std::string SortColumnTitle = mp_Columns->getColumnTitleWithIndex(
      SortColumnId);

  mp_UnitsColl = UnitsColl;

  delete mp_Columns;
  mp_Columns = new DomainIDataColumns();

  // create columns and then liststore with those columns
  BOOST_FOREACH(std::string DataName, mp_UnitsColl->getList()->begin()->getInputData()->getInputDataNames())
{  Gtk::TreeModelColumn<std::string> * DataColumn =
  new Gtk::TreeModelColumn<std::string>();

  mp_Columns->addWithTitle(DataName, *DataColumn);
}

mref_ListStore = BuilderListStore::create(*mp_Columns);

// populate liststore
BOOST_FOREACH(openfluid::core::Unit Unit,*(mp_UnitsColl->getList()))
{
  Gtk::TreeRow Row = *(mref_ListStore->append());

  // Id column
  Row.set_value(*mp_Columns->getIdColumn(),
      (int) Unit.getID());

  // Data columns
  BOOST_FOREACH(std::string DataName, Unit.getInputData()->getInputDataNames())
  {
    std::string Val;
    Unit.getInputData()->getValue(DataName, Val);

    Row.set_value( *mp_Columns->getColumnWithTitle(DataName), Val);
  }
}

// apply stored sort
int NewSortColumnId = mp_Columns->getColumnIndexWithTitle(SortColumnTitle);

if(SortColumnId == 0) /* Id column was sorted, we sort it */
{
  mref_ListStore->set_sort_column(0,SortType);
}
else if(SortColumnId > 0 && NewSortColumnId!= -1) /* an IData column was sorted and still exists, we sort it*/
{
  mref_ListStore->set_sort_column(NewSortColumnId,SortType);
}
else /* no sort existed or sorted IData column doesn't exist no more, we apply the default (Id) sort */
{
  mref_ListStore->set_sort_column(0,Gtk::SORT_ASCENDING);
}

if(! getRequestedUnitSelection())
setFirstUnitSelected();

}

// =====================================================================
// =====================================================================


void DomainIDataAdapterModelImpl::setFirstUnitSelected()
{
  if (!mref_ListStore->children().empty())
    mref_ListStore->children().begin()->get_value(0, m_SelectedUnit);
}

// =====================================================================
// =====================================================================


void DomainIDataAdapterModelImpl::setSelectedUnit(Gtk::TreeIter Iter)
{
  if (Iter)
    Iter->get_value(0, m_SelectedUnit);
  else
    m_SelectedUnit = -1;
}

// =====================================================================
// =====================================================================


Glib::RefPtr<Gtk::TreeModel> DomainIDataAdapterModelImpl::getTreeModel()
{
  return mref_ListStore;
}

// =====================================================================
// =====================================================================


DomainIDataColumns* DomainIDataAdapterModelImpl::getColumns()
{
  return mp_Columns;
}

// =====================================================================
// =====================================================================


Gtk::TreeIter DomainIDataAdapterModelImpl::getRequestedUnitSelection()
{
  Gtk::TreeModel::Children Children = mref_ListStore->children();

  for (unsigned int i = 0; i < Children.size(); i++)
  {
    int IdValue;
    Children[i]->get_value(0, IdValue);

    if (IdValue == m_SelectedUnit)
      return Children[i];
  }

  return (Gtk::TreeIter) 0;
}

// =====================================================================
// =====================================================================


void DomainIDataAdapterModelImpl::updateData(const std::string NewText,
    std::string DataName)
{
  openfluid::core::Unit* Unit = mp_UnitsColl->getUnit(m_SelectedUnit);
  Gtk::TreeIter Iter = getRequestedUnitSelection();

  if (Unit != NULL && Iter)
  {
    if (NewText.empty())
    {
      std::string OldValue;
      Unit->getInputData()->getValue(DataName, OldValue);

      Iter->set_value(*mp_Columns->getColumnWithTitle(DataName), OldValue);
    }
    else
    {
      if (NewText == "-")
        Iter->set_value(*mp_Columns->getColumnWithTitle(DataName), NewText);

      Unit->getInputData()->replaceValue(DataName, NewText);
    }
  }
}

