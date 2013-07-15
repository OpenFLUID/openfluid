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
 \file BuilderTreeStore.cpp
 \brief Implements ...

 \author Aline LIBRES <libres@supagro.inra.fr>
 */

#include "BuilderTreeStore.hpp"

#include <gtkmm/treerowreference.h>

#include <openfluid/base/ApplicationException.hpp>

// =====================================================================
// =====================================================================


BuilderTreeStore::BuilderTreeStore(Gtk::TreeModelColumnRecord& Columns)
{
  m_TitleColumnDefined = false;
  set_column_types(Columns);
}

// =====================================================================
// =====================================================================


Glib::RefPtr<BuilderTreeStore> BuilderTreeStore::create(
    Gtk::TreeModelColumnRecord& Columns)
{
  return Glib::RefPtr<BuilderTreeStore>(new BuilderTreeStore(Columns));
}

// =====================================================================
// =====================================================================


void BuilderTreeStore::setTitleColumn(
    Gtk::TreeModelColumn<Glib::ustring> TitleColumn)
{
  m_TitleColumn = TitleColumn;
  m_TitleColumnDefined = true;
}

// =====================================================================
// =====================================================================


Gtk::TreeRowReference* BuilderTreeStore::createRowRefFromPathString(
    std::string PathString)
{
  return new Gtk::TreeRowReference((Glib::RefPtr<Gtk::TreeStore>) this,
      this->get_path(this->get_iter(PathString)));
}

// =====================================================================
// =====================================================================


Gtk::TreeRowReference* BuilderTreeStore::createRowRefFromIter(
    Gtk::TreeIter Iter)
{
  return new Gtk::TreeRowReference((Glib::RefPtr<Gtk::TreeStore>) this,
      this->get_path(Iter));
}

// =====================================================================
// =====================================================================


Gtk::TreeRow BuilderTreeStore::getRowFromRowRef(Gtk::TreeRowReference& RowRef)
{
  return *(this->get_iter(RowRef.get_path()));
}

// =====================================================================
// =====================================================================


Gtk::TreeRow BuilderTreeStore::appendToRowRef(Gtk::TreeRowReference& RowRef)
{
  return *(this->append(getRowFromRowRef(RowRef)->children()));
}

// =====================================================================
// =====================================================================


Gtk::TreeRowReference* BuilderTreeStore::appendATitleRow(Gtk::TreeModelColumn<
    Glib::ustring> Column, std::string Value)
{
  Gtk::TreeRow Row = *(this->append());
  Row[Column] = Value;
  return createRowRefFromIter(*Row);
}

// =====================================================================
// =====================================================================


// throw OFException
Gtk::TreeRowReference* BuilderTreeStore::appendATitleRow(std::string Value)
{
  if (m_TitleColumnDefined)
    return appendATitleRow(m_TitleColumn, Value);
  else
    throw openfluid::base::ApplicationException("openfluid-builder",
        "BuilderTreeStore::appendATitleRow", "No title column defined.");
}

// =====================================================================
// =====================================================================


void BuilderTreeStore::clearChildrenOfRowRef(Gtk::TreeRowReference& RowRef)
{
  Gtk::TreeModel::Children ChildrenRows = getRowFromRowRef(RowRef).children();

  while (!ChildrenRows.empty())
  {
    erase(ChildrenRows.begin());
  }
}
