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
 \file DomainStructureModule.hpp
 \brief Header of ...

 \author Aline LIBRES <libres@supagro.inra.fr>
 */

#ifndef __DOMAINSTRUCTUREMODULE_HPP__
#define __DOMAINSTRUCTUREMODULE_HPP__

#include <openfluid/guicommon-gtk/ProjectWorkspaceModule.hpp>

#include <gtkmm/box.h>
#include <gtkmm/liststore.h>
#include <gtkmm/treeview.h>
#include <gtkmm/scrolledwindow.h>

namespace openfluid {
namespace fluidx {
class AdvancedFluidXDescriptor;
}
}

class DomainUnitRelationAddDialog;
class DomainUnitAddEditDialog;
class BuilderButtonBox;

class DomainStructureModule: public openfluid::guicommon::ProjectWorkspaceModule
{
  private:

    openfluid::fluidx::AdvancedDomainDescriptor& m_Domain;

    Gtk::Box* mp_MainPanel;

    class ClassColumns: public Gtk::TreeModel::ColumnRecord
    {
      public:
        ClassColumns()
        {
          add(m_Class);
          add(m_UnitSortColumnId);
          add(m_UnitSortType);
        }
        Gtk::TreeModelColumn<Glib::ustring> m_Class;
        Gtk::TreeModelColumn<int> m_UnitSortColumnId;
        Gtk::TreeModelColumn<Gtk::SortType> m_UnitSortType;
    };
    ClassColumns m_ClassColumns;

    class UnitColumns: public Gtk::TreeModel::ColumnRecord
    {
      public:
        UnitColumns()
        {
          add(m_Id);
          add(m_PcsOrder);
        }
        Gtk::TreeModelColumn<int> m_Id;
        Gtk::TreeModelColumn<int> m_PcsOrder;
    };
    UnitColumns m_UnitColumns;

    Glib::RefPtr<Gtk::ListStore> mref_ClassesStore;
    Glib::RefPtr<Gtk::ListStore> mref_UnitsStore;

    Gtk::TreeView* mp_ClassesView;
    Gtk::TreeView* mp_UnitsView;

    Gtk::ScrolledWindow* mp_ClassesWin;
    Gtk::ScrolledWindow* mp_UnitsWin;

    Gtk::Box* mp_MainBox;

    void onClassSelectionChanged();

    void onUnitRowActivated(const Gtk::TreeModel::Path& Path,
                            Gtk::TreeViewColumn* Column);

    void onUnitsSortChanged();

    void onEditAsked();

    void onAddAsked();

    void onRemoveAsked();

    void updateUnits(Gtk::TreeIter& ClassIter);

    void updateClasses(std::string ClassToForceSelection = "");

  protected:

    DomainUnitRelationAddDialog* mp_RelationAddDialog;
    DomainUnitAddEditDialog* mp_AddEditDialog;

    BuilderButtonBox* mp_ToolBox;

    sigc::signal<void> m_signal_DomainStructureChanged;

    void compose();

    Gtk::Widget* asWidget();

    void whenDomainChanged();

  public:

    DomainStructureModule(
        openfluid::fluidx::AdvancedFluidXDescriptor& AdvancedDesc);

    ~DomainStructureModule();

    sigc::signal<void> signal_ModuleChanged();

    void update();
};

#endif /* __DOMAINSTRUCTUREMODULE_HPP__ */
