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
 \file DomainIDataView.cpp
 \brief Implements ...

 \author Aline LIBRES <libres@supagro.inra.fr>
 */

#include "DomainIDataView.hpp"

#include "DomainIDataColumns.hpp"
#include <openfluid/guicommon/DialogBoxFactory.hpp>

// =====================================================================
// =====================================================================


void DomainIDataViewImpl::onDataEditingStarted(Gtk::CellEditable* /*CellEditable*/,
    const Glib::ustring& Path, std::string DataName, int ColIndex)
{

  Gtk::TreePath path(Path);

  int Id;
  mp_TreeView->get_model()->get_iter(path)->get_value(0, Id);

  std::string Val;
  mp_TreeView->get_model()->get_iter(path)->get_value(ColIndex, Val);

  std::string NewVal =
      openfluid::guicommon::DialogBoxFactory::showDomainIDataEditDialog(Id,
          DataName, Val);

  m_signal_DataEdited.emit(Path, NewVal, DataName, ColIndex);
}

// =====================================================================
// =====================================================================


DomainIDataViewImpl::DomainIDataViewImpl()
{
  mp_TreeView = Gtk::manage(new Gtk::TreeView());

  mp_TreeView->set_visible(true);
}

// =====================================================================
// =====================================================================


void DomainIDataViewImpl::setTreeModel(Glib::RefPtr<Gtk::TreeModel> TreeModel,
    DomainIDataColumns* Columns)
{
  mp_TreeView->remove_all_columns();
  mp_TreeView->set_model(TreeModel);

  if (TreeModel->get_n_columns() < 2)
    return;

  mp_TreeView->append_column("Unit ID", *Columns->getIdColumn());
  mp_TreeView->get_column(0)->set_sort_column(*Columns->getIdColumn());

  std::map<std::string, Gtk::TreeModelColumn<std::string>*> ColumnsByTitle =
      Columns->getByTitleColumns();

  for (std::map<std::string, Gtk::TreeModelColumn<std::string>*>::iterator it =
      ColumnsByTitle.begin(); it != ColumnsByTitle.end(); ++it)
  {
    int ColIndex = mp_TreeView->append_column_editable(it->first, *it->second)
        - 1;

    mp_TreeView->get_column(ColIndex)->set_sort_column(*it->second);

    Gtk::CellRendererText* CellRend =
        (Gtk::CellRendererText *) mp_TreeView->get_column_cell_renderer(
            ColIndex);

    CellRend->signal_editing_started().connect(sigc::bind<std::string, int>(
        sigc::mem_fun(*this, &DomainIDataViewImpl::onDataEditingStarted),
        it->first, ColIndex));
  }

}

// =====================================================================
// =====================================================================


sigc::signal<void, const Glib::ustring, const std::string, std::string, int> DomainIDataViewImpl::signal_DataEdited()
{
  return m_signal_DataEdited;
}

// =====================================================================
// =====================================================================


Gtk::Widget* DomainIDataViewImpl::asWidget()
{
  return mp_TreeView;
}

