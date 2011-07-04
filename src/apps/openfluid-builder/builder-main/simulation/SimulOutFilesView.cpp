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
 \file SimulOutFilesView.cpp
 \brief Implements ...

 \author Aline LIBRES <libres@supagro.inra.fr>
 */

#include "SimulOutFilesView.hpp"

#include <glibmm/i18n.h>

#include <openfluid/guicommon/DialogBoxFactory.hpp>

// =====================================================================
// =====================================================================


void SimulOutFilesViewImpl::onSelectionChanged()
{
  m_signal_FileSelectionChanged.emit();
}

// =====================================================================
// =====================================================================


SimulOutFilesViewImpl::SimulOutFilesViewImpl()
{
  mp_TreeView = Gtk::manage(new Gtk::TreeView());

  mp_TreeView->append_column(_("Format name"), m_Columns.m_Name);
  mp_TreeView->append_column(_("Column separator"), m_Columns.m_ColSeparator);
  mp_TreeView->append_column(_("Date format"), m_Columns.m_DateFormat);
  mp_TreeView->append_column(_("Comment character"), m_Columns.m_CommentChar);

  mp_TreeView->get_selection()->signal_changed().connect(sigc::mem_fun(*this,
      &SimulOutFilesViewImpl::onSelectionChanged));

  mp_TreeView->set_visible(true);

  mp_MainWin = Gtk::manage(new Gtk::ScrolledWindow());
  mp_MainWin->set_policy(Gtk::POLICY_AUTOMATIC, Gtk::POLICY_AUTOMATIC);
  mp_MainWin->add(*mp_TreeView);
  mp_MainWin->set_shadow_type(Gtk::SHADOW_ETCHED_IN);
  mp_MainWin->set_visible(true);
}

// =====================================================================
// =====================================================================


sigc::signal<void> SimulOutFilesViewImpl::signal_FileSelectionChanged()
{
  return m_signal_FileSelectionChanged;
}

// =====================================================================
// =====================================================================


void SimulOutFilesViewImpl::setModel(Glib::RefPtr<Gtk::TreeModel> Model)
{
  mp_TreeView->set_model(Model);
}

// =====================================================================
// =====================================================================


void SimulOutFilesViewImpl::setSelectedRow(Gtk::TreeRow Row)
{
  mp_TreeView->get_selection()->select(Row);
}

// =====================================================================
// =====================================================================


Gtk::TreeIter SimulOutFilesViewImpl::getSelectedIter()
{
  return mp_TreeView->get_selection()->get_selected();
}

// =====================================================================
// =====================================================================


Gtk::Widget* SimulOutFilesViewImpl::asWidget()
{
  return mp_MainWin;
}

// =====================================================================
// =====================================================================

// =====================================================================
// =====================================================================


void SimulOutFilesViewSub::selectRowWithIndex(int Index)
{
  mp_TreeView->get_selection()->select(
      mp_TreeView->get_model()->children()[Index]);
}

// =====================================================================
// =====================================================================


Gtk::TreeView* SimulOutFilesViewSub::getTreeView()
{
  return mp_TreeView;
}
