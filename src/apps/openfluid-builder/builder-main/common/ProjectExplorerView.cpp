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
 \file ProjectExplorerView.cpp
 \brief Implements ...

 \author Aline LIBRES <libres@supagro.inra.fr>
 */

#include "ProjectExplorerView.hpp"

#include <glibmm/i18n.h>

// =====================================================================
// =====================================================================


ProjectExplorerViewImpl::ProjectExplorerViewImpl() :
  m_ActivatedPath("0")
{
  mp_TreeView = Gtk::manage(new Gtk::TreeView());

  mp_TreeView->append_column("Project Explorer", m_Columns.m_Display);
  mp_TreeView->set_level_indentation(10);

  mp_TreeView->get_column(0)->add_attribute(
      ((Gtk::CellRendererText*) mp_TreeView->get_column_cell_renderer(0))->property_weight(),
      m_Columns.m_Weight);

  mp_TreeView->get_column(0)->add_attribute(
        ((Gtk::CellRendererText*) mp_TreeView->get_column_cell_renderer(0))->property_foreground(),
        m_Columns.m_Color);

  mp_TreeView->set_visible(true);

  mp_TreeView->signal_row_activated().connect(sigc::mem_fun(*this,
      &ProjectExplorerViewImpl::onRowActivated));

  mp_MainWindow = Gtk::manage(new Gtk::ScrolledWindow());
  mp_MainWindow->set_policy(Gtk::POLICY_NEVER, Gtk::POLICY_AUTOMATIC);
  mp_MainWindow->set_visible(true);
  mp_MainWindow->add(*mp_TreeView);
}

// =====================================================================
// =====================================================================


sigc::signal<void> ProjectExplorerViewImpl::signal_ActivationChanged()
{
  return m_signal_ActivationChanged;
}

// =====================================================================
// =====================================================================


void ProjectExplorerViewImpl::setTreeModel(
    Glib::RefPtr<Gtk::TreeModel> TreeModel)
{
  Gtk::TreePath RunRowPath = TreeModel->get_path(TreeModel->get_iter("3:0"));

  bool RunRowIsExpanded = mp_TreeView->row_expanded(RunRowPath);

  mp_TreeView->set_model(TreeModel);
  mp_TreeView->expand_all();

  if (!RunRowIsExpanded)
    mp_TreeView->collapse_row(RunRowPath);
}

// =====================================================================
// =====================================================================


void ProjectExplorerViewImpl::onRowActivated(const Gtk::TreeModel::Path& Path,
    Gtk::TreeViewColumn* /*Column*/)
{
  m_ActivatedPath = Path;
  m_signal_ActivationChanged.emit();
}

// =====================================================================
// =====================================================================


Gtk::TreePath ProjectExplorerViewImpl::getActivatedPath()
{
  return m_ActivatedPath;
}

// =====================================================================
// =====================================================================


Gtk::Widget* ProjectExplorerViewImpl::asWidget()
{
  return mp_MainWindow;
}

// =====================================================================
// =====================================================================


Gtk::TreeView* ProjectExplorerViewSub::getTreeView()
{
  return mp_TreeView;
}
