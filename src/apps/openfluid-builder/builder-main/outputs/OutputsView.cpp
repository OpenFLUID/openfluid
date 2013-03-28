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
 \file OutputsView.cpp
 \brief Implements ...

 \author Aline LIBRES <aline.libres@gmail.com>
 */

#include "OutputsView.hpp"

#include <glibmm/i18n.h>
#include <gtkmm/label.h>
#include <gtkmm/button.h>
#include <gtkmm/stock.h>
#include <gtkmm/separator.h>
#include <openfluid/base/ProjectManager.hpp>
#include <gtkmm/scrolledwindow.h>
#include <gtkmm/icontheme.h>
#include <gtkmm/iconinfo.h>
#include <gtkmm/alignment.h>
#include <gtkmm/treerowreference.h>
#include <openfluid/guicommon/DialogBoxFactory.hpp>

// =====================================================================
// =====================================================================

OutputsView::OutputsView() :
    mp_AppChooserDialog(0)
{
  std::string RootDir =
      openfluid::base::ProjectManager::getInstance()->getOutputDir();

  mref_Root = Gio::File::create_for_path(RootDir);

  if(!mref_Root->query_exists())
    mref_Root->make_directory();


  Gtk::Label* OutputDirLabel = Gtk::manage(new Gtk::Label());
  OutputDirLabel->set_markup(
      Glib::ustring::compose(_("Output directory: %1%2%3"), "<b>", RootDir,
                             "</b>"));
  OutputDirLabel->set_alignment(Gtk::ALIGN_LEFT);

  Gtk::HBox* TopBox = Gtk::manage(new Gtk::HBox());
  TopBox->pack_start(*OutputDirLabel, Gtk::PACK_SHRINK);

  mp_NavBox = Gtk::manage(new Gtk::HBox(false, 5));
  mp_NavBox->set_visible(true);

  mref_ListStore = Gtk::ListStore::create(m_Columns);

  mp_TreeView = Gtk::manage(new Gtk::TreeView());

  Gtk::TreeView::Column* NameColumn = Gtk::manage(
      new Gtk::TreeView::Column(_("Name")));
  NameColumn->pack_start(m_Columns.m_Icon, false);
  NameColumn->pack_start(m_Columns.m_DisplayName);
  NameColumn->set_sort_indicator(true);
  NameColumn->set_sort_column_id(m_Columns.m_DisplayName);
  mp_TreeView->append_column(*NameColumn);

  mp_TreeView->set_rules_hint(true);
  mp_TreeView->set_headers_clickable(true);
  mp_TreeView->get_selection()->set_mode(Gtk::SELECTION_MULTIPLE);

  mref_ListStore->set_sort_func(
      m_Columns.m_DisplayName,
      sigc::mem_fun(*this, &OutputsView::onSortCompare));
  mref_ListStore->set_sort_column(m_Columns.m_DisplayName, Gtk::SORT_ASCENDING);

  mp_TreeView->set_visible(true);

  mp_TreeView->signal_row_activated().connect(
      sigc::mem_fun(*this, &OutputsView::onRowActivated));

  mp_TreeView->signal_button_press_event().connect(
      sigc::mem_fun(*this, &OutputsView::onBtPressEvent), false);

  mp_TreeView->signal_key_press_event().connect(
      sigc::mem_fun(*this, &OutputsView::onKeyPressEvent), false);

  Gtk::ScrolledWindow* mp_ModelWin = Gtk::manage(new Gtk::ScrolledWindow());
  mp_ModelWin->set_policy(Gtk::POLICY_AUTOMATIC, Gtk::POLICY_AUTOMATIC);
  mp_ModelWin->set_visible(true);
  mp_ModelWin->add(*mp_TreeView);
  mp_ModelWin->set_shadow_type(Gtk::SHADOW_ETCHED_IN);

  mp_TreeView->set_model(mref_ListStore);

  mp_MainBox = Gtk::manage(new Gtk::VBox());
  mp_MainBox->pack_start(*TopBox, Gtk::PACK_SHRINK, 5);
  mp_MainBox->pack_start(*Gtk::manage(new Gtk::HSeparator()), Gtk::PACK_SHRINK,
                         5);
  mp_MainBox->pack_start(*mp_NavBox, Gtk::PACK_SHRINK, 5);
  mp_MainBox->pack_start(*mp_ModelWin, Gtk::PACK_EXPAND_WIDGET, 5);
  mp_MainBox->set_visible(true);
  mp_MainBox->show_all_children();

  addNavButton(mref_Root);
  setCurrentDir(mref_Root);

  Gtk::MenuItem* Item = Gtk::manage(new Gtk::MenuItem(_("Open with...")));
  Item->signal_activate().connect(
      sigc::mem_fun(*this, &OutputsView::onMenuPopupOpenActivated));
  m_MenuPopup.append(*Item);

  Item = Gtk::manage(new Gtk::MenuItem(_("Delete")));
  Item->signal_activate().connect(
      sigc::mem_fun(*this, &OutputsView::onMenuPopupDeleteActivated));
  m_MenuPopup.append(*Item);

  m_MenuPopup.show_all();

  mp_AppChooserDialog = new AppChooserDialog();
}

// =====================================================================
// =====================================================================

// If a and b are same type, we sort by alphabetical order, else directories come first
int OutputsView::onSortCompare(const Gtk::TreeModel::iterator& a,
                               const Gtk::TreeModel::iterator& b)
{
  Glib::ustring aName = a->get_value(m_Columns.m_DisplayName);
  Glib::ustring bName = b->get_value(m_Columns.m_DisplayName);

  // the model is not fully set, don't proceed in sort
  if (aName.empty() || bName.empty())
    return 0;

  Gio::FileType aType = a->get_value(m_Columns.m_File)->query_file_type();
  Gio::FileType bType = b->get_value(m_Columns.m_File)->query_file_type();

  if (aType == bType)
  {
    return aName.lowercase().compare(bName.lowercase());
  }

  return (bType == Gio::FILE_TYPE_DIRECTORY);
}

// =====================================================================
// =====================================================================

void OutputsView::update()
{

}

// =====================================================================
// =====================================================================

Gtk::Widget* OutputsView::asWidget()
{
  return mp_MainBox;
}

// =====================================================================
// =====================================================================

void OutputsView::setCurrentDir(Glib::RefPtr<Gio::File> Asked)
{
  m_CurrentDir = Asked;

  m_CurrentMonitor = Asked->monitor_directory();
  m_CurrentMonitor->signal_changed().connect(
      sigc::mem_fun(*this, &OutputsView::onDirMonitoringChanged));

  updateBrowser();
}

// =====================================================================
// =====================================================================

void OutputsView::updateBrowser()
{
  mref_ListStore->clear();

  Glib::RefPtr<Gtk::IconTheme> Theme = Gtk::IconTheme::get_default();

  Glib::RefPtr<Gio::FileEnumerator> Children =
      m_CurrentDir->enumerate_children();

  for (Glib::RefPtr<Gio::FileInfo> Child = Children->next_file(); Child != 0;
      Child = Children->next_file())
  {
    if (!Child->is_backup() && !Child->is_hidden() && !Child->is_symlink())
    {
      Gtk::TreeRow Row = *(mref_ListStore->append());

      // set File first, because sort slot need it
      Row[m_Columns.m_File] = m_CurrentDir->get_child(Child->get_name());

      Row[m_Columns.m_DisplayName] = Child->get_display_name();

      Gtk::IconInfo IconInfo = Theme->lookup_icon(
          Child->get_icon(), 16, Gtk::ICON_LOOKUP_GENERIC_FALLBACK);
      if (IconInfo)
        Row[m_Columns.m_Icon] = IconInfo.load_icon();
      else
        Row[m_Columns.m_Icon] = Theme->load_icon(
            "text-x-generic", 16, Gtk::ICON_LOOKUP_GENERIC_FALLBACK);
    }

  }

  Children->close();

  mref_ListStore->set_sort_column(m_Columns.m_DisplayName, Gtk::SORT_ASCENDING);
}

// =====================================================================
// =====================================================================

void OutputsView::addNavButton(Glib::RefPtr<Gio::File> File)
{
  Gtk::RadioButton* NavBt = Gtk::manage(new Gtk::RadioButton(m_NavGroup));

  if (File->get_path() == mref_Root->get_path())
    NavBt->set_label(_("Output directory"));
  else
    NavBt->set_label(
        File->query_info("standard::display-name")->get_display_name());

  NavBt->set_tooltip_text(File->get_path());

  NavBt->set_visible(true);
  NavBt->set_mode(false); // to look like a standard button

  NavBt->signal_toggled().connect(
      sigc::bind<Glib::RefPtr<Gio::File> >(
          sigc::mem_fun(*this, &OutputsView::setCurrentDir), File));

  mp_NavBox->pack_start(*NavBt, Gtk::PACK_SHRINK);
  mp_NavBox->reorder_child(*NavBt, 0);

  m_NavButtons[File->get_path()] = NavBt;
}

// =====================================================================
// =====================================================================

void OutputsView::onRowActivated(const Gtk::TreeModel::Path& Path,
                                 Gtk::TreeViewColumn* /*Column*/)
{
  Gtk::TreeIter it = mref_ListStore->get_iter(Path);

  if (!it)
    return;

  Glib::RefPtr<Gio::File> Asked = it->get_value(m_Columns.m_File);

  if (Asked->query_file_type() == Gio::FILE_TYPE_DIRECTORY)
  {
    NavBtList_t::iterator Found = m_NavButtons.find(Asked->get_path());

    if (Found != m_NavButtons.end())
    {
      Found->second->set_active();
    }
    else
    {
      setNavigationBar(Asked);
    }
  }
  else
  {
    try
    {
      Gio::AppInfo::launch_default_for_uri(Asked->get_uri());
    }
    catch (...)
    {
      mp_AppChooserDialog->show(Asked);
    }
  }

}

// =====================================================================
// =====================================================================

void OutputsView::setNavigationBar(Glib::RefPtr<Gio::File> Asked)
{
  for (NavBtList_t::iterator it = m_NavButtons.begin();
      it != m_NavButtons.end(); ++it)
    mp_NavBox->remove(*it->second);
  m_NavButtons.clear();

  for (Glib::RefPtr<Gio::File> Current = Asked;
      Current->get_path() != mref_Root->get_parent()->get_path(); Current =
          Current->get_parent())
    addNavButton(Current);

  m_NavButtons.at(Asked->get_path())->set_active();
}

// =====================================================================
// =====================================================================

bool OutputsView::onBtPressEvent(GdkEventButton* Event)
{
  if ((Event->type == GDK_BUTTON_PRESS) && (Event->button == 3))
  {
    m_MenuPopup.popup(Event->button, Event->time);
  }

  return false;
}

// =====================================================================
// =====================================================================

void OutputsView::onMenuPopupOpenActivated()
{
  Glib::RefPtr<Gtk::TreeView::Selection> RefSelection =
      mp_TreeView->get_selection();

  if (RefSelection)
  {
    mp_TreeView->get_selection()->selected_foreach_iter(
        sigc::mem_fun(*this, &OutputsView::showAppChooser));
  }
}

// =====================================================================
// =====================================================================

void OutputsView::showAppChooser(const Gtk::TreeModel::iterator& Iter)
{
  mp_AppChooserDialog->show(Iter->get_value(m_Columns.m_File));
}

// =====================================================================
// =====================================================================

void OutputsView::onMenuPopupDeleteActivated()
{
  Glib::RefPtr<Gtk::TreeView::Selection> RefSelection =
      mp_TreeView->get_selection();

  if (RefSelection)
  {
    std::vector<Gtk::TreePath> SelectionTreePaths =
        RefSelection->get_selected_rows();

    /* don't use selected_foreach_iter nor pass a TreeRowIter, even
     * a TreeRowRef, because deletion invalidate them
     */
    std::vector<Glib::RefPtr<Gio::File> > SelectionFiles;
    for (unsigned int i = 0; i < SelectionTreePaths.size(); i++)
      SelectionFiles.push_back(
          mref_ListStore->get_iter(SelectionTreePaths[i])->get_value(
              m_Columns.m_File));
    for (unsigned int i = 0; i < SelectionFiles.size(); i++)
      deleteFile(SelectionFiles[i]);

    setNavigationBar(m_CurrentDir);
    updateBrowser();
  }
}

// =====================================================================
// =====================================================================

void OutputsView::deleteFile(Glib::RefPtr<Gio::File> File)
{
  if (!File->trash())
    openfluid::guicommon::DialogBoxFactory::showSimpleErrorMessage(
        _("Unable to delete the file"));
}

// =====================================================================
// =====================================================================

bool OutputsView::onKeyPressEvent(GdkEventKey* Event)
{
  if ((Event->type == GDK_KEY_PRESS) && (Event->keyval == GDK_KEY_Delete))
  {
    Glib::RefPtr<Gtk::TreeView::Selection> RefSelection =
        mp_TreeView->get_selection();

    if (RefSelection)
    {
      std::vector<Gtk::TreePath> SelectionTreePaths =
          RefSelection->get_selected_rows();

      /* don't use selected_foreach_iter nor pass an TreeRowIter, even
       * a TreeRowRef, because deletion invalidate them
       */
      std::vector<Glib::RefPtr<Gio::File> > SelectionFiles;
      for (unsigned int i = 0; i < SelectionTreePaths.size(); i++)
        SelectionFiles.push_back(
            mref_ListStore->get_iter(SelectionTreePaths[i])->get_value(
                m_Columns.m_File));
      for (unsigned int i = 0; i < SelectionFiles.size(); i++)
        deleteFile(SelectionFiles[i]);

      setNavigationBar(m_CurrentDir);
      updateBrowser();
    }
  }

  return false;
}

// =====================================================================
// =====================================================================

/* To avoid too many successive updates (and because the monitoring rate limit
 * concerns successive changes on a single file only, not on the monitored directory)
 * we call a timeout function, allowing us to update each second only, and only if needed.
 */
void OutputsView::onDirMonitoringChanged(
    const Glib::RefPtr<Gio::File>& /*File*/,
    const Glib::RefPtr<Gio::File>& /*OtherFile*/,
    Gio::FileMonitorEvent /*EventType*/)
{
  if (!m_TimeoutConn.connected())
    m_TimeoutConn = Glib::signal_timeout().connect_seconds(
        sigc::mem_fun(*this, &OutputsView::onTimout_applyPendingChanges), 1);
}

// =====================================================================
// =====================================================================

bool OutputsView::onTimout_applyPendingChanges()
{
  updateBrowser();
  return false; // disconnect
}
