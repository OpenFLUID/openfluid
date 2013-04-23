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
 \file AppChooserDialog.cpp
 \brief Implements ...

 \author Aline LIBRES <aline.libres@gmail.com>
 */

#include "AppChooserDialog.hpp"

#include <glibmm/i18n.h>
#include <gtkmm/icontheme.h>
#include <gtkmm/scrolledwindow.h>
#include <gtkmm/stock.h>

// =====================================================================
// =====================================================================

AppChooserDialog::AppChooserDialog()
{
  mref_AppListStore = Gtk::ListStore::create(m_AppColumns);

  Glib::RefPtr<Gtk::IconTheme> Theme = Gtk::IconTheme::get_default();

  Glib::ListHandle<Glib::RefPtr<Gio::AppInfo> > AllApps =
      Gio::AppInfo::get_all();

  for (Glib::ListHandle<Glib::RefPtr<Gio::AppInfo> >::const_iterator it =
      AllApps.begin(); it != AllApps.end(); ++it)
  {
    Gtk::TreeRow Row = *(mref_AppListStore->append());

    Row[m_AppColumns.m_DisplayName] = (*it)->get_name();

    Row[m_AppColumns.m_AppInfo] = (*it);

    Glib::RefPtr<Gio::Icon> Icon = (*it)->get_icon();
    if (Icon)
    {
      try
      {
        Gtk::IconInfo IconInfo = Theme->lookup_icon(
            Icon, 16, Gtk::ICON_LOOKUP_GENERIC_FALLBACK);
        Row[m_AppColumns.m_Icon] = IconInfo.load_icon();

      }
      catch (...)
      {
        Row[m_AppColumns.m_Icon] = Theme->load_icon(
            "image-missing", 16, Gtk::ICON_LOOKUP_GENERIC_FALLBACK);
      }
    }
    else
      Row[m_AppColumns.m_Icon] = Theme->load_icon(
          "image-missing", 16, Gtk::ICON_LOOKUP_GENERIC_FALLBACK);
  }

  mp_AppTreeView = Gtk::manage(new Gtk::TreeView());

  Gtk::TreeView::Column* AppNameColumn = Gtk::manage(
      new Gtk::TreeView::Column(_("Name")));
  AppNameColumn->pack_start(m_AppColumns.m_Icon, false);
  AppNameColumn->pack_start(m_AppColumns.m_DisplayName);
  mp_AppTreeView->append_column(*AppNameColumn);

  mref_AppListStore->set_sort_column(m_AppColumns.m_DisplayName,
                                     Gtk::SORT_ASCENDING);

  mp_AppTreeView->signal_row_activated().connect(
      sigc::mem_fun(*this, &AppChooserDialog::onAppRowActivated));

  mp_AppTreeView->set_model(mref_AppListStore);

  mp_AppTreeView->set_visible(true);

  Gtk::ScrolledWindow* mp_AppModelWin = Gtk::manage(new Gtk::ScrolledWindow());
  mp_AppModelWin->set_policy(Gtk::POLICY_AUTOMATIC, Gtk::POLICY_AUTOMATIC);
  mp_AppModelWin->set_visible(true);
  mp_AppModelWin->add(*mp_AppTreeView);
  mp_AppModelWin->set_shadow_type(Gtk::SHADOW_ETCHED_IN);

  mp_AppLabel = Gtk::manage(new Gtk::Label());
  mp_AppLabel->set_visible(true);

  mp_AppChooserDialog = new Gtk::Dialog("");
  mp_AppChooserDialog->get_vbox()->pack_start(*mp_AppLabel, Gtk::PACK_SHRINK,
                                              5);
  mp_AppChooserDialog->get_vbox()->pack_start(*mp_AppModelWin);
  mp_AppChooserDialog->add_button(Gtk::Stock::CANCEL, Gtk::RESPONSE_CANCEL);
  mp_AppChooserDialog->add_button(Gtk::Stock::OK, Gtk::RESPONSE_OK);
  mp_AppChooserDialog->set_default_size(400, 400);
  mp_AppChooserDialog->show_all_children();
}

// =====================================================================
// =====================================================================

void AppChooserDialog::show(Glib::RefPtr<Gio::File> Asked)
{
  mp_AppLabel->set_markup(
      Glib::ustring::compose(_("%1Choose an application to open \" %2 \" %3"),
                             "<b>", Asked->query_info()->get_display_name(),
                             "</b>"));

  if (mp_AppChooserDialog->run() == Gtk::RESPONSE_OK)
  {
    Gtk::TreeIter it = mp_AppTreeView->get_selection()->get_selected();

    if (it)
      it->get_value(m_AppColumns.m_AppInfo)->launch(Asked);
  }

  mp_AppChooserDialog->hide();
}

// =====================================================================
// =====================================================================

void AppChooserDialog::onAppRowActivated(const Gtk::TreeModel::Path& /*Path*/,
                                         Gtk::TreeViewColumn* /*Column*/)
{
  mp_AppChooserDialog->response(Gtk::RESPONSE_OK);
}

// =====================================================================
// =====================================================================
