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
 \file EngineProjectNewDialog.hpp
 \brief Header of ...

 \author Aline LIBRES <libres@supagro.inra.fr>
 */

#ifndef __ENGINEPROJECTNEWDIALOG_HPP__
#define __ENGINEPROJECTNEWDIALOG_HPP__

#include <gtkmm/dialog.h>
#include <gtkmm/filechooserbutton.h>
#include <gtkmm/entry.h>
#include <gtkmm/textview.h>
#include <gtkmm/label.h>
#include <gtkmm/infobar.h>
#include <gtkmm/checkbutton.h>
#include <gtkmm/radiobutton.h>
#include <gtkmm/button.h>
#include <gtkmm/box.h>
#include <gtkmm/treestore.h>
#include <gtkmm/treeview.h>

#include <boost/filesystem.hpp>

class EngineProjectOpenDialog;


class EngineProjectNewDialog
{
  private:

    Gtk::Dialog* mp_Dialog;

    Gtk::FileChooserButton* mp_WorkdirFileChooserButton;

    Gtk::Entry* mp_NameEntry;

    Gtk::TextView* mp_DescriptionTextView;

    Gtk::Entry* mp_AuthorsEntry;

    Gtk::Label* mp_InfoBarLabel;

    Gtk::InfoBar* mp_InfoBar;

    Gtk::CheckButton* mp_ImportCheck;

    Gtk::RadioButton* mp_ImportProjectFilesButton;

    Gtk::Label* mp_ImportProjectNameLabel;

    Gtk::RadioButton* mp_ImportSystemFilesButton;

    Gtk::FileChooserButton* mp_ImportSystemFileChooserButton;

    Gtk::Button* mp_ImportProjectFileChooserButton;

    Gtk::VBox* mp_ImportBox;

    Glib::ustring m_Workdir;

    Glib::ustring m_ProjectName;

    Glib::ustring m_ImportSystemFolder;

    Glib::ustring m_ImportProjectFolder;

    // is m_ImportSystemFolder or m_ImportProjectFolder
    Glib::ustring m_ImportFolder;

    Glib::RefPtr<Gtk::TreeStore> mref_TreeModel;

    Gtk::TreeView* mp_TreeView;

    class ImportColumns: public Gtk::TreeModel::ColumnRecord
    {
      public:
        ImportColumns()
        {
          add(m_IsSelected);
          add(m_FileName);
          add(m_FilePath);
          add(m_AlreadyFilled);
          add(m_Inconsistent);
        }
        Gtk::TreeModelColumn<bool> m_IsSelected;
        Gtk::TreeModelColumn<Glib::ustring> m_FileName;
        Gtk::TreeModelColumn<Glib::ustring> m_FilePath;
        Gtk::TreeModelColumn<bool> m_AlreadyFilled;
        Gtk::TreeModelColumn<bool> m_Inconsistent;
    };

    ImportColumns m_Columns;

    Glib::ustring m_ProjectInputDir;

    bool m_IsValid;

    EngineProjectOpenDialog* mp_ProjectDialog;

    void onProjectFolderSelectionChanged();

    void onImportSystemFolderSelectionChanged();

    void onImportCheckClicked();

    void onImportTypeChanged();

    void onImportProjectButtonClicked();

    void appendDirectoryContent(boost::filesystem::path DirectoryPath,
        Gtk::TreeIter DirectoryIter);

    bool onTestExpandRow(const Gtk::TreeModel::iterator& Iter,
        const Gtk::TreeModel::Path& Path);

    void onCheckToggled(const Glib::ustring TreePath);

    void checkRowSiblings(Gtk::TreeRow Row, bool Checked);

    void setChildrenChecked(Gtk::TreeRow Row, bool Checked = true);

    void setParentsUnconsistent(Gtk::TreeRow Row);

    void copyFilePathAndChildren(Gtk::TreeRow Row);

    void copyOnDisk(std::string FromPath);

    bool isHidden(std::string FileName);

    void checkProject();

    Glib::ustring replaceInvalidChars(Glib::ustring Str);

    void reset();

  public:

    EngineProjectNewDialog();

    ~EngineProjectNewDialog();

    Glib::ustring show();

    Glib::ustring getImportDir();

};

#endif /* __ENGINEPROJECTNEWDIALOG_HPP__ */
