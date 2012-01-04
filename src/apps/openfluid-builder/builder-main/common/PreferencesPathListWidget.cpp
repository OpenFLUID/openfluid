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
 \file PreferencesPathListWidget.cpp
 \brief Implements ...

 \author Aline LIBRES <libres@supagro.inra.fr>
 */

#include "PreferencesPathListWidget.hpp"

#include <glibmm/i18n.h>

#include <gtkmm/label.h>
#include <gtkmm/filechooserdialog.h>
#include <gtkmm/stock.h>

#include "BuilderListToolBox.hpp"

#include <iostream>

// =====================================================================
// =====================================================================


PreferencesPathListWidget::PreferencesPathListWidget()
{
  Gtk::Label* UserDefinedPathsLabel = Gtk::manage(new Gtk::Label(
      _("User defined paths:")));
  UserDefinedPathsLabel->set_padding(10, 0);
  UserDefinedPathsLabel->set_alignment(0, 0.5);
  UserDefinedPathsLabel->set_visible(true);

  Gtk::Label* PreDefinedPathsLabel = Gtk::manage(new Gtk::Label(
      _("Predefined paths:")));
  PreDefinedPathsLabel->set_padding(10, 0);
  PreDefinedPathsLabel->set_alignment(0, 0.5);
  PreDefinedPathsLabel->set_visible(true);

  mref_UserDefinedModel = Gtk::ListStore::create(m_PathsColumns);
  mref_PreDefinedModel = Gtk::ListStore::create(m_PathsColumns);

  mp_UserDefinedTreeView
      = Gtk::manage(new Gtk::TreeView(mref_UserDefinedModel));
  mp_UserDefinedTreeView->append_column("", m_PathsColumns.m_Path);
  mp_UserDefinedTreeView->set_headers_visible(false);
  mp_UserDefinedTreeView->set_visible(true);

  mp_PreDefinedTreeView = Gtk::manage(new Gtk::TreeView(mref_PreDefinedModel));
  mp_PreDefinedTreeView->append_column("", m_PathsColumns.m_Path);
  mp_PreDefinedTreeView->set_headers_visible(false);
  mp_PreDefinedTreeView->set_visible(true);

  mp_ToolBox = new BuilderListToolBoxImpl();
  mp_ToolBox->setAllCommandVisible();
  mp_ToolBox->setEditCommandVisible(false);

  mp_ToolBox->setAddCommandTooltipText(_("Append a directory"));
  mp_ToolBox->setRemoveCommandTooltipText(_(
      "Remove the selected directory"));
  mp_ToolBox->setUpCommandTooltipText(_(
      "Move up the selected directory"));
  mp_ToolBox->setDownCommandTooltipText(_(
      "Move down the selected directory"));

  mp_ToolBox->signal_AddCommandAsked().connect(sigc::mem_fun(*this,
      &PreferencesPathListWidget::whenAddPathAsked));
  mp_ToolBox->signal_RemoveCommandAsked().connect(sigc::mem_fun(*this,
      &PreferencesPathListWidget::whenRemovePathAsked));
  mp_ToolBox->signal_UpCommandAsked().connect(sigc::mem_fun(*this,
      &PreferencesPathListWidget::whenUpPathAsked));
  mp_ToolBox->signal_DownCommandAsked().connect(sigc::mem_fun(*this,
      &PreferencesPathListWidget::whenDownPathAsked));

  Gtk::HBox* UserDefinedBox = Gtk::manage(new Gtk::HBox());
  UserDefinedBox->pack_start(*mp_UserDefinedTreeView);
  UserDefinedBox->pack_start(*mp_ToolBox->asWidget(), Gtk::PACK_SHRINK);
  UserDefinedBox->set_visible(true);

  mp_MainBox = Gtk::manage(new Gtk::VBox(false, 5));
  mp_MainBox->pack_start(*UserDefinedPathsLabel, Gtk::PACK_SHRINK);
  mp_MainBox->pack_start(*UserDefinedBox, Gtk::PACK_SHRINK);
  mp_MainBox->pack_start(*PreDefinedPathsLabel, Gtk::PACK_SHRINK);
  mp_MainBox->pack_start(*mp_PreDefinedTreeView, Gtk::PACK_SHRINK);
  mp_MainBox->set_visible(true);

  mp_MainFrame = Gtk::manage(new Gtk::Frame());
  mp_MainFrame->set_shadow_type(Gtk::SHADOW_IN);
  mp_MainFrame->add(*mp_MainBox);
}

// =====================================================================
// =====================================================================


Gtk::Widget* PreferencesPathListWidget::asWidget()
{
  return mp_MainFrame;
}

// =====================================================================
// =====================================================================


void PreferencesPathListWidget::setPreDefinedPaths(
    std::vector<std::string> Paths)
{
  mref_PreDefinedModel->clear();

  for (unsigned int i = 0; i < Paths.size(); i++)
  {
    Gtk::TreeRow Row = *mref_PreDefinedModel->append();
    Row[m_PathsColumns.m_Path] = Paths[i];
  }
}

// =====================================================================
// =====================================================================


void PreferencesPathListWidget::setUserDefinedPaths(
    std::vector<std::string> Paths)
{
  mref_UserDefinedModel->clear();

  for (unsigned int i = 0; i < Paths.size(); i++)
  {
    Gtk::TreeRow Row = *mref_UserDefinedModel->append();
    Row[m_PathsColumns.m_Path] = Paths[i];
  }

  updateToolBox();
}

// =====================================================================
// =====================================================================

std::vector<Glib::ustring> PreferencesPathListWidget::getUserDefinedPaths()
{
  std::vector<Glib::ustring> Paths;

  Gtk::TreeModel::Children Children = mref_UserDefinedModel->children();

  for (Gtk::TreeIter it = Children.begin(); it != Children.end(); ++it)
  {
    Paths.push_back(it->get_value(m_PathsColumns.m_Path));
  }

  return Paths;
}

// =====================================================================
// =====================================================================

void PreferencesPathListWidget::whenAddPathAsked()
{
  Gtk::FileChooserDialog Dialog(_("Choose a directory"),
      Gtk::FILE_CHOOSER_ACTION_SELECT_FOLDER);

  Dialog.add_button(Gtk::Stock::CANCEL, Gtk::RESPONSE_CANCEL);
  Dialog.add_button(Gtk::Stock::OK, Gtk::RESPONSE_OK);

  Dialog.set_current_folder(Glib::get_home_dir());

  if (Dialog.run() == Gtk::RESPONSE_OK)
  {
    Glib::ustring SelectedFolder =
        Glib::filename_to_utf8(Dialog.get_filename());

    Gtk::TreeRow Row = *mref_UserDefinedModel->append();
    Row[m_PathsColumns.m_Path] = SelectedFolder;

    m_signal_PathListChanged.emit();
    updateToolBox();
  }
}

// =====================================================================
// =====================================================================

void PreferencesPathListWidget::whenRemovePathAsked()
{
  Gtk::TreeIter it = mp_UserDefinedTreeView->get_selection()->get_selected();

  if (it)
  {
    mref_UserDefinedModel->erase(it);
    m_signal_PathListChanged.emit();
    updateToolBox();
  }
}

// =====================================================================
// =====================================================================

void PreferencesPathListWidget::whenUpPathAsked()
{
  Gtk::TreeIter it = mp_UserDefinedTreeView->get_selection()->get_selected();

  if (it && it != mref_UserDefinedModel->children().begin())
  {
    Gtk::TreeIter dest = it;
    dest--;

    mref_UserDefinedModel->move(it, dest);

    m_signal_PathListChanged.emit();
    updateToolBox();
  }
}

// =====================================================================
// =====================================================================

void PreferencesPathListWidget::whenDownPathAsked()
{
  Gtk::TreeIter it = mp_UserDefinedTreeView->get_selection()->get_selected();

  Gtk::TreeIter last = mref_UserDefinedModel->children().end();

  if (last != mref_UserDefinedModel->children().begin())
  {
    last--;

    if (it && it != last)
    {
      Gtk::TreeIter dest = it;
      std::advance(dest,2);

      mref_UserDefinedModel->move(it, dest);

      m_signal_PathListChanged.emit();
      updateToolBox();
    }
  }
}

// =====================================================================
// =====================================================================


sigc::signal<void> PreferencesPathListWidget::signal_PathListChanged()
{
  return m_signal_PathListChanged;
}

// =====================================================================
// =====================================================================


void PreferencesPathListWidget::updateToolBox()
{
  bool AtLeast1Path = !mref_UserDefinedModel->children().empty();
  bool AtLeast2Path = mref_UserDefinedModel->children().size() > 1;
  mp_ToolBox->setRemoveCommandAvailable(AtLeast1Path);
  mp_ToolBox->setUpCommandAvailable(AtLeast2Path);
  mp_ToolBox->setDownCommandAvailable(AtLeast2Path);
}
