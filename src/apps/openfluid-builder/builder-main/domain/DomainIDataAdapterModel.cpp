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

// =====================================================================
// =====================================================================


DomainIDataAdapterModelImpl::DomainIDataAdapterModelImpl() :
  mp_UnitsColl(0), mp_Columns(new DomainIDataColumns())
{
}

// =====================================================================
// =====================================================================


void DomainIDataAdapterModelImpl::dataInit(
    openfluid::core::UnitsCollection* UnitsColl)
{
  mp_UnitsColl = UnitsColl;

  delete mp_Columns;
  mp_Columns = new DomainIDataColumns();

  // create columns and then liststore with those columns
  BOOST_FOREACH(std::string DataName, mp_UnitsColl->getList()->begin()->getInputData()->getInputDataNames())
  {
    Gtk::TreeModelColumn<std::string> * DataColumn =
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
      Unit.getInputData()->getValue(DataName, &Val);

      Row.set_value( *mp_Columns->getColumnWithTitle(DataName), Val);
    }
  }

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


void DomainIDataAdapterModelImpl::updateData(const Glib::ustring PathString,
    const std::string NewText, std::string DataName, int ColIndex)
{
  Gtk::TreeIter Iter = mref_ListStore->get_iter(PathString);

  Gtk::TreeModelColumn<int>* ColId = mp_Columns->getIdColumn();

  if (Iter && ColId && mp_UnitsColl)
  {

    Iter->set_value(ColIndex, NewText);

    int Id = Iter->get_value(*ColId);
    mp_UnitsColl->getUnit(Id)->getInputData()->replaceValue(DataName, NewText);

    m_signal_DataChanged.emit();
  }
}

// =====================================================================
// =====================================================================


sigc::signal<void> DomainIDataAdapterModelImpl::signal_DataChanged()
{
  return m_signal_DataChanged;
}

