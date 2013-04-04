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
 \file DatasoreAddItemDialog.hpp
 \brief Header of ...

 \author Aline LIBRES <aline.libres@gmail.com>
 */

#ifndef DATASOREADDITEMDIALOG_HPP_
#define DATASOREADDITEMDIALOG_HPP_

#include <giomm/file.h>
#include <gtkmm/dialog.h>
#include <gtkmm/infobar.h>
#include <gtkmm/entry.h>
#include <gtkmm/checkbutton.h>
#include <gtkmm/comboboxtext.h>
#include <gtkmm/radiobutton.h>
#include <gtkmm/treeview.h>
#include <gtkmm/liststore.h>
#include <gtkmm/expander.h>

namespace openfluid {
namespace fluidx {
class AdvancedDatastoreDescriptor;
class DatastoreItemDescriptor;
}
}

class DatasoreAddItemDialog
{
  private:

    openfluid::fluidx::AdvancedDatastoreDescriptor* mp_Datastore;

    Gtk::Dialog* mp_Dialog;

    Glib::RefPtr<Gio::File> mref_InDirFile;

    Gtk::InfoBar* mp_InfoBar;
    Gtk::Label* mp_InfoBarLabel;

    Gtk::Entry* mp_IDEntry;

    Gtk::ComboBoxText* mp_TypeCombo;

    Gtk::CheckButton* mp_ClassCheck;
    Gtk::Entry* mp_ClassEntry;

    Gtk::RadioButton* mp_FileRadio;
    Gtk::RadioButton* mp_DirRadio;
    Gtk::RadioButton* mp_ResourceRadio;

    Gtk::VBox* mp_FileDetailBox;
    Gtk::VBox* mp_DirDetailBox;

    Gtk::Entry* mp_FilePathEntry;
    Gtk::Entry* mp_DirPathEntry;

    Gtk::Entry* mp_ResourceStringEntry;

    Gtk::RadioButton* mp_NoSubDirRadio;
    Gtk::RadioButton* mp_SubDirRadio;

    Gtk::HBox* mp_SubDirBox;
    Gtk::Entry* mp_SubDirEntry;

    Gtk::Expander* mp_Expander;

    class Columns: public Gtk::TreeModel::ColumnRecord
    {
      public:

        Columns()
        {
          add(m_File);
          add(m_FileName);
          add(m_Selected);
          add(m_Sensitive);
        }

        Gtk::TreeModelColumn<Glib::RefPtr<Gio::File> > m_File;
        Gtk::TreeModelColumn<std::string> m_FileName;
        Gtk::TreeModelColumn<bool> m_Selected;
        Gtk::TreeModelColumn<bool> m_Sensitive;
    };

    Columns m_Columns;
    Gtk::TreeView* mp_TreeView;
    Glib::RefPtr<Gtk::ListStore> mref_TreeModel;

    void init();

    void checkIsValid();

    void onCheckToggled();

    void onSourceToggled();

    void onBrowseFileButtonClicked();
    void onBrowseDirButtonClicked();

    void onSubDirToggled();

    bool isFileInInputDir(Glib::RefPtr<Gio::File> File);

    bool copyFile(Glib::RefPtr<Gio::File> FromFile,
                  Glib::RefPtr<Gio::File>& ToFile);

    openfluid::fluidx::DatastoreItemDescriptor* createItem(
        std::string RelativePath);

    std::string getRoot(std::string FileName);

  public:

    DatasoreAddItemDialog(
        openfluid::fluidx::AdvancedDatastoreDescriptor& Datastore);

    openfluid::fluidx::DatastoreItemDescriptor* show();
};

#endif /* DATASOREADDITEMDIALOG_HPP_ */
