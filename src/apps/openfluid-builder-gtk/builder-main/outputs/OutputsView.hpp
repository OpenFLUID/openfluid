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
 \file OutputsView.hpp
 \brief Header of ...

 \author Aline LIBRES <aline.libres@gmail.com>
 */

#ifndef OUTPUTSVIEW_HPP_
#define OUTPUTSVIEW_HPP_

#include <gtkmm/box.h>
#include <gtkmm/treemodel.h>
#include <gtkmm/liststore.h>
#include <gtkmm/treeview.h>
#include <gtkmm/radiobuttongroup.h>
#include <gtkmm/menu.h>
#include <gdkmm/pixbuf.h>
#include <giomm/file.h>
#include <giomm/filemonitor.h>
#include "AppChooserDialog.hpp"

class OutputsView
{
  private:

    Gtk::Box* mp_MainBox;

    Gtk::HBox* mp_NavBox;

    Gtk::RadioButtonGroup m_NavGroup;

    typedef std::map<std::string, Gtk::RadioButton*> NavBtList_t;
    NavBtList_t m_NavButtons;

    Glib::RefPtr<Gio::File> mref_Root;

    Glib::RefPtr<Gtk::ListStore> mref_ListStore;

    class BrowserColumns: public Gtk::TreeModel::ColumnRecord
    {
      public:
        BrowserColumns()
        {
          add(m_DisplayName);
          add(m_Icon);
          add(m_File);
        }
        Gtk::TreeModelColumn<std::string> m_DisplayName;
        Gtk::TreeModelColumn<Glib::RefPtr<Gdk::Pixbuf> > m_Icon;
        Gtk::TreeModelColumn<Glib::RefPtr<Gio::File> > m_File;
    };

    BrowserColumns m_Columns;

    Gtk::TreeView* mp_TreeView;

    AppChooserDialog* mp_AppChooserDialog;

    Gtk::Menu m_MenuPopup;

    Glib::RefPtr<Gio::FileMonitor> m_CurrentMonitor;

    Glib::RefPtr<Gio::File> m_CurrentDir;

    sigc::connection m_TimeoutConn;

    void onRowActivated(const Gtk::TreeModel::Path& Path,
                        Gtk::TreeViewColumn* Column);

    void setNavigationBar(Glib::RefPtr<Gio::File> Asked);

    bool onBtPressEvent(GdkEventButton* Event);

    bool onKeyPressEvent(GdkEventKey* Event);

    void setCurrentDir(Glib::RefPtr<Gio::File> Asked);

    void updateBrowser();

    void addNavButton(Glib::RefPtr<Gio::File> File);

    void onMenuPopupOpenActivated();

    void onMenuPopupDeleteActivated();

    void showAppChooser(const Gtk::TreeModel::iterator& Iter);

    void deleteFile(Glib::RefPtr<Gio::File> File);

    void onDirMonitoringChanged(const Glib::RefPtr<Gio::File>& File,
                                const Glib::RefPtr<Gio::File>& OtherFile,
                                Gio::FileMonitorEvent EventType);

    bool onTimout_applyPendingChanges();

    int onSortCompare(const Gtk::TreeModel::iterator& a,
                      const Gtk::TreeModel::iterator& b);

  public:

    OutputsView();

    void update();

    Gtk::Widget* asWidget();
};

#endif /* OUTPUTSVIEW_HPP_ */
