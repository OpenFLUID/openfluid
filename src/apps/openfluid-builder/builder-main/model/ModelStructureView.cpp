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
 \file ModelStructureView.cpp
 \brief Implements ...

 \author Aline LIBRES <libres@supagro.inra.fr>
 */

#include "ModelStructureView.hpp"

#include <glibmm/i18n.h>

// =====================================================================
// =====================================================================


void ModelStructureViewImpl::onTreeViewSelectionChanged()
{
  m_signal_StuctureFctSelectionChanged.emit();
}

// =====================================================================
// =====================================================================


ModelStructureViewImpl::ModelStructureViewImpl(ModelStructureColumns& Columns) :
  m_Columns(Columns)
{
  mp_TreeView = Gtk::manage(new Gtk::TreeView());

  mp_TreeView->append_column("", m_Columns.m_Id);
  mp_TreeView->set_headers_visible(false);

  mp_TreeView->get_selection()->signal_changed().connect(sigc::mem_fun(*this,
      &ModelStructureViewImpl::onTreeViewSelectionChanged));

  mp_TreeView->set_visible(true);
}

// =====================================================================
// =====================================================================


sigc::signal<void> ModelStructureViewImpl::signal_StructureFctSelectionChanged()
{
  return m_signal_StuctureFctSelectionChanged;
}

// =====================================================================
// =====================================================================


void ModelStructureViewImpl::setModel(Glib::RefPtr<Gtk::TreeModel> Model)
{
  mp_TreeView->set_model(Model);
}

// =====================================================================
// =====================================================================


int ModelStructureViewImpl::getSelectedRowPosition()
{
  Gtk::TreeIter Iter = mp_TreeView->get_selection()->get_selected();
  if (Iter)
    return *mp_TreeView->get_model()->get_path(Iter).begin();
  return -1;
}

// =====================================================================
// =====================================================================


Gtk::Widget* ModelStructureViewImpl::asWidget()
{
  return mp_TreeView;
}

// =====================================================================
// =====================================================================


void ModelStructureViewImpl::requestSelectionAt(int RowPosition)
{
  if (RowPosition > -1 && RowPosition
      < (int) mp_TreeView->get_model()->children().size())
  {
    Gtk::TreeRow Row = mp_TreeView->get_model()->children()[RowPosition];
    mp_TreeView->get_selection()->select(Row);
  } else
    mp_TreeView->get_selection()->unselect_all();
  signal_StructureFctSelectionChanged().emit();
}

// =====================================================================
// =====================================================================

// =====================================================================
// =====================================================================


ModelStructureViewSub::ModelStructureViewSub(ModelStructureColumns& Columns) :
  ModelStructureViewImpl(Columns)
{
}

// =====================================================================
// =====================================================================


int ModelStructureViewSub::getRowNb()
{
  return mp_TreeView->get_model()->children().size();
}

// =====================================================================
// =====================================================================


std::string ModelStructureViewSub::getRowId(int RowNumber)
{
  Glib::ustring Id =
      mp_TreeView->get_model()->children()[RowNumber][m_Columns.m_Id];
  return Id;
}
