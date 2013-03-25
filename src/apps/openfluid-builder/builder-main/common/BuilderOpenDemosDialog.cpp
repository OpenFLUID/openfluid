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
 \file BuilderOpenDemosDialog.cpp
 \brief Implements ...

 \author Aline LIBRES <aline.libres@gmail.com>
 */

#include "BuilderOpenDemosDialog.hpp"

#include <boost/filesystem/path.hpp>
#include <boost/filesystem/operations.hpp>
#include <glibmm/i18n.h>
#include <giomm/file.h>
#include <gtkmm/stock.h>
#include <gtkmm/scrolledwindow.h>
#include <openfluid/buddies/ExamplesBuddy.hpp>
#include <openfluid/base/RuntimeEnv.hpp>
#include <openfluid/base/ProjectManager.hpp>
#include <openfluid/config.hpp>

// =====================================================================
// =====================================================================

BuilderOpenDemosDialog::BuilderOpenDemosDialog()
{
  mp_Buddy = new openfluid::buddies::ExamplesBuddy(0);

  mref_ListStore = Gtk::ListStore::create(m_Columns);

  mp_TreeView = Gtk::manage(new Gtk::TreeView());

  mp_TreeView->append_column("", m_Columns.m_Name);

  mp_TreeView->get_selection()->signal_changed().connect(
      sigc::mem_fun(*this, &BuilderOpenDemosDialog::onSelectionChanged));

  mp_TreeView->signal_row_activated().connect(
      sigc::mem_fun(*this, &BuilderOpenDemosDialog::onRowActivated));

  mp_TreeView->set_model(mref_ListStore);
  mp_TreeView->set_headers_visible(false);

  mp_TreeView->set_visible(true);

  Gtk::ScrolledWindow* ScrolledWin = Gtk::manage(new Gtk::ScrolledWindow());
  ScrolledWin->set_policy(Gtk::POLICY_AUTOMATIC, Gtk::POLICY_AUTOMATIC);
  ScrolledWin->set_visible(true);
  ScrolledWin->add(*mp_TreeView);
  ScrolledWin->set_shadow_type(Gtk::SHADOW_ETCHED_IN);
  ScrolledWin->set_border_width(5);

  mp_PreviewWidget = new EngineProjectPreviewWidget();

  Gtk::HBox* MainBox = Gtk::manage(new Gtk::HBox());
  MainBox->pack_start(*ScrolledWin);
  MainBox->pack_start(*mp_PreviewWidget->asWidget(), Gtk::PACK_EXPAND_WIDGET,
                      5);

  mp_Dialog = new Gtk::Dialog(_("Open Demo project"));
  mp_Dialog->set_has_separator(true);
  mp_Dialog->get_vbox()->pack_start(*MainBox, Gtk::PACK_EXPAND_WIDGET, 5);

  mp_Dialog->add_button(Gtk::Stock::CANCEL, Gtk::RESPONSE_CANCEL);
  mp_Dialog->add_button(Gtk::Stock::OK, Gtk::RESPONSE_OK);

  mp_Dialog->set_default_response(Gtk::RESPONSE_OK);

  mp_Dialog->set_default_size(500, 500);
  mp_Dialog->show_all_children();
}

// =====================================================================
// =====================================================================

BuilderOpenDemosDialog::~BuilderOpenDemosDialog()
{

}

// =====================================================================
// =====================================================================

std::string BuilderOpenDemosDialog::show()
{
  std::string SelectedPath = "";

  openfluid::base::ProjectManager* Manager =
      openfluid::base::ProjectManager::getInstance();

  mref_ListStore->clear();

  mp_Buddy->run();

  std::string DemosPath =
      boost::filesystem::path(
          openfluid::base::RuntimeEnvironment::getInstance()->getUserExamplesDir() + "/"
          + openfluid::config::PROJECTS_SUBDIR).string();

  Glib::RefPtr<Gio::File> DemosDir = Gio::File::create_for_path(DemosPath);

  Glib::RefPtr<Gio::FileEnumerator> Children = DemosDir->enumerate_children();
  for (Glib::RefPtr<Gio::FileInfo> Child = Children->next_file(); Child != 0;
      Child = Children->next_file())
  {
    std::string ChildPath = DemosDir->get_child(Child->get_name())->get_path();

    if (openfluid::base::ProjectManager::isProject(ChildPath))
    {
      Manager->open(ChildPath);

      Gtk::TreeRow Row = *(mref_ListStore->append());

      Row[m_Columns.m_Name] = Manager->getName();
      Row[m_Columns.m_Path] = ChildPath;

      Manager->close();
    }
  }
  Children->close();

  if (!mref_ListStore->children().empty())
    mp_TreeView->get_selection()->select(mref_ListStore->children().begin());

  if (mp_Dialog->run() == Gtk::RESPONSE_OK)
  {
    Glib::RefPtr<Gtk::TreeSelection> Selection = mp_TreeView->get_selection();

    if (Selection)
    {
      Gtk::TreeIter it = Selection->get_selected();

      if (it)
        SelectedPath = it->get_value(m_Columns.m_Path);
    }

  }

  mp_Dialog->hide();

  return SelectedPath;
}

// =====================================================================
// =====================================================================

void BuilderOpenDemosDialog::onSelectionChanged()
{
  Glib::RefPtr<Gtk::TreeSelection> Selection = mp_TreeView->get_selection();

  if (!Selection)
    return;

  Gtk::TreeIter it = Selection->get_selected();

  if (!it)
    return;

  mp_PreviewWidget->update(it->get_value(m_Columns.m_Path));
}

// =====================================================================
// =====================================================================

void BuilderOpenDemosDialog::onRowActivated(
    const Gtk::TreeModel::Path& /*Path*/, Gtk::TreeViewColumn* /*Column*/)
{
  mp_Dialog->response(Gtk::RESPONSE_OK);
}
