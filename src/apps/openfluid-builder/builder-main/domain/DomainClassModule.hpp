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
 \file DomainClassModule.hpp
 \brief Header of ...

 \author Aline LIBRES <libres@supagro.inra.fr>
 */

#ifndef __DOMAINCLASSMODULE_HPP__
#define __DOMAINCLASSMODULE_HPP__

#include <openfluid/guicommon/ProjectWorkspaceModule.hpp>

#include <gtkmm/box.h>
#include <gtkmm/treeview.h>
#include <gtkmm/liststore.h>
#include <gtkmm/treestore.h>
#include <gtkmm/scrolledwindow.h>

class DomainAttributesAddDialog;
class DomainAttributesRemoveDialog;
class DomainAttributesEditDialog;

class BuilderButtonBox;

class DomainClassModule: public openfluid::guicommon::ProjectWorkspaceModule
{
  private:

    openfluid::fluidx::AdvancedDomainDescriptor& m_Domain;

    std::string m_ClassName;

    Gtk::Box* mp_MainPanel;

    Gtk::TreeView* mp_AttrsTreeView;
    Gtk::TreeView* mp_EventsTreeView;

    Gtk::ScrolledWindow* mp_AttrsWin;
    Gtk::ScrolledWindow* mp_EventsWin;

    class AttrsColumns: public Gtk::TreeModel::ColumnRecord
    {
      private:

        std::map<std::string, Gtk::TreeModelColumn<std::string> > m_Columns;

      public:

        Gtk::TreeModelColumn<int> m_Id;

        AttrsColumns() :
            Gtk::TreeModel::ColumnRecord()
        {
          add(m_Id);
        }
        AttrsColumns(std::set<std::string>& Names) :
            Gtk::TreeModel::ColumnRecord()
        {
          add(m_Id);
          for (std::set<std::string>::iterator it = Names.begin();
              it != Names.end(); ++it)
            addColumn(*it);
        }
        void addColumn(std::string Name)
        {
          Gtk::TreeModelColumn<std::string> Col;
          add(Col);
          m_Columns[Name] = Col;
        }
        Gtk::TreeModelColumn<std::string>& getColumn(std::string Name)
        {
          return m_Columns[Name];
        }
        void removeColumn(std::string Name)
        {
          m_Columns.erase(Name);
        }
        void renameColumn(std::string OldName, std::string NewName)
        {
          Gtk::TreeModelColumn<std::string> Col = m_Columns[OldName];
          m_Columns.erase(OldName);
          add(Col);
          m_Columns[NewName] = Col;
        }
    };
    AttrsColumns* mp_AttrsColumns;

    class EventsColumns: public Gtk::TreeModel::ColumnRecord
    {
      public:
      EventsColumns()
        {
          add(m_Id_Date_Info);
        }

        Gtk::TreeModelColumn<std::string> m_Id_Date_Info;
    };
    EventsColumns m_EventsColumns;

    Glib::RefPtr<Gtk::ListStore> mref_AttrsListStore;
    Glib::RefPtr<Gtk::TreeStore> mref_EventsTreeStore;

    static std::string escapeUnderscores(std::string Str);

    void whenAddAttributeAsked();
    void whenRemoveAttributeAsked();
    void whenEditAttributeAsked();
    void onAttributeEdited(const Glib::ustring& Path, const Glib::ustring& NewText,
                      std::string AttrName);

    void whenAddEventAsked();
    void whenRemoveEventAsked();

  protected:

    DomainAttributesAddDialog* mp_AttrAddDialog;
    DomainAttributesRemoveDialog* mp_AttrRemoveDialog;
    DomainAttributesEditDialog* mp_AttrEditDialog;

    BuilderButtonBox* mp_AttrListToolBox;

    BuilderButtonBox* mp_EventsListToolBox;

    sigc::signal<void> m_signal_DomainClassChanged;

    void compose();

    Gtk::Widget* asWidget();

    void updateAttributes();
    void updateEvents();

  public:

    DomainClassModule(openfluid::fluidx::AdvancedFluidXDescriptor& AdvancedDesc,
                      std::string ClassName);

    ~DomainClassModule();

    sigc::signal<void> signal_ModuleChanged();

    void update();
};

#endif /* __DOMAINCLASSMODULE_HPP__ */
