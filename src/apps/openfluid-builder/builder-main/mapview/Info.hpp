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
 \file Info.hpp
 \brief Header of ...

 \author Damien CHABBERT <dams.vivien@gmail.com>
 */

#ifndef __INFO_HPP__
#define __INFO_HPP__

#include <gtkmm/dialog.h>
#include <gtkmm/table.h>
#include <gtkmm/expander.h>
#include <gtkmm/label.h>
#include <gtkmm/liststore.h>
#include <gtkmm/treeview.h>
#include <gtkmm/scrolledwindow.h>
#include <set>

#include <openfluid/core/CoreRepository.hpp>
#include <openfluid/core/Unit.hpp>

class Info
{

  private:

    Gtk::Dialog* mp_Dialog;

    Gtk::Label* mp_NameClassLabel;
    Gtk::Label* mp_IDLabel;
    Gtk::Label* mp_ProcessOrderLabel;
    Gtk::Label m_IDLabel;
    Gtk::Label m_ProcessOrderLabel;

    Gtk::ScrolledWindow* mp_InfoIntputEventsScrolledWindow;
    Gtk::ScrolledWindow* mp_IDScrolledWindow;

    Gtk::ScrolledWindow* mp_FromScrolledWindow;
    Gtk::ScrolledWindow* mp_ToScrolledWindow;
    Gtk::ScrolledWindow* mp_ParentScrolledWindow;
    Gtk::ScrolledWindow* mp_ChildrenScrolledWindow;

    Gtk::HBox* mp_MainHBox;

    Gtk::VBox* mp_InfoIntputEventsVBox;
    Gtk::VBox* mp_ButtonInfoIntputEventsVBox;

    Gtk::Table* mp_InfoTable;
    Gtk::Table* mp_InputDataTable;
    Gtk::Table* mp_EventTable;

    Gtk::Expander* mp_InfoExpander;
    Gtk::Expander* mp_InputDataExpander;
    Gtk::Expander* mp_EventExpander;

    Gtk::Expander* mp_FromExpander;
    Gtk::Expander* mp_ToExpander;
    Gtk::Expander* mp_ParentExpander;
    Gtk::Expander* mp_ChildrenExpander;

    Gtk::Button* mp_RestoreDefaultButton;
    Gtk::Button* mp_OkButton;

    std::map<int, std::map<std::string, std::string> > m_ChangeValue;
    std::map<Gtk::Label*, std::pair<Gtk::Entry*, Gtk::Button*> >
        m_InputDataLineTable;
    std::vector<std::string> m_InputDataNames;
    std::set<int> m_SelectedUnitId;

    openfluid::core::CoreRepository* mp_CoreRepos;

    class ModelColumnsIDs: public Gtk::TreeModelColumnRecord
    {
      public:

        ModelColumnsIDs()
        {
          add(m_ID);
        }

        Gtk::TreeModelColumn<int> m_ID;
    };

    ModelColumnsIDs m_ModelColumnIDs;

    class ModelColumnsNameClassIDs: public Gtk::TreeModelColumnRecord
    {
      public:

        ModelColumnsNameClassIDs()
        {
          add(m_ID);
          add(m_NameClass);
        }

        Gtk::TreeModelColumn<int> m_ID;
        Gtk::TreeModelColumn<std::string> m_NameClass;
    };

    ModelColumnsNameClassIDs m_ModelColumnsNameClassIDsFrom;
    ModelColumnsNameClassIDs m_ModelColumnsNameClassIDsTo;
    ModelColumnsNameClassIDs m_ModelColumnsNameClassIDsParent;
    ModelColumnsNameClassIDs m_ModelColumnsNameClassIDsChildren;

    Glib::RefPtr<Gtk::ListStore> mref_ListStoreIDs;
    Glib::RefPtr<Gtk::ListStore> mref_ListStoreFrom;
    Glib::RefPtr<Gtk::ListStore> mref_ListStoreTo;
    Glib::RefPtr<Gtk::ListStore> mref_ListStoreParent;
    Glib::RefPtr<Gtk::ListStore> mref_ListStoreChildren;

    Gtk::TreeView* mp_TreeViewIDs;
    Gtk::TreeView* mp_TreeViewFrom;
    Gtk::TreeView* mp_TreeViewTo;
    Gtk::TreeView* mp_TreeViewParent;
    Gtk::TreeView* mp_TreeViewChildren;

    void onIDViewSelectionChanged();
    void loadSelectedID(const Gtk::TreeModel::iterator&);
    void loadInfo(int, int);
    void loadInputData(int, int);
    void loadEvent(int, int);
    void fillNameClassIDListStore(Glib::RefPtr<Gtk::ListStore>&,
        ModelColumnsNameClassIDs&, const openfluid::core::UnitsPtrList_t*,
        std::string);
    void onEntryInputDataChanged(std::string);
    bool on_focus_out_event(GdkEventFocus*, std::string);
  public:

    Info(Gtk::Window&, const Glib::ustring&,
        openfluid::core::CoreRepository& CoreRepos);
    void show(std::string, std::set<int>);
};

#endif /* __INFO_HPP__ */
