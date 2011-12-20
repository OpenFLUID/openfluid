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
 \file DomainIDataView.hpp
 \brief Header of ...

 \author Aline LIBRES <libres@supagro.inra.fr>
 */

#ifndef __DOMAINIDATAVIEW_HPP__
#define __DOMAINIDATAVIEW_HPP__

#include <sigc++/sigc++.h>

#include <gtkmm/treeview.h>
#include <gtkmm/scrolledwindow.h>

#include <glibmm/i18n.h>

class DomainIDataColumns;

// =====================================================================
// =====================================================================


class DomainIDataView
{
  public:

    virtual void requestUnitSelection(Gtk::TreeIter Iter) = 0;

    virtual sigc::signal<void, const Glib::ustring&, std::string>
    signal_DataEdited() = 0;

    virtual sigc::signal<void> signal_UnitSelectionChanged() = 0;

    virtual Gtk::TreeIter getSelectedUnitIter() = 0;

    virtual Gtk::Widget* asWidget() = 0;

    virtual void setTreeModel(Glib::RefPtr<Gtk::TreeModel> TreeModel,
        DomainIDataColumns* Columns) = 0;
};

// =====================================================================
// =====================================================================


class DomainIDataViewImpl: public DomainIDataView
{
  private:
    sigc::signal<void, const Glib::ustring&, std::string> m_signal_DataEdited;

    sigc::signal<void> m_signal_UnitSelectionChanged;

    void onDataEdited(const Glib::ustring& Path, const Glib::ustring& NewText,
        std::string DataName);

    void onUnitSelectionChanged();

  protected:

    Gtk::TreeView* mp_TreeView;

    Gtk::ScrolledWindow* mp_MainWin;

  public:

    DomainIDataViewImpl();

    void requestUnitSelection(Gtk::TreeIter Iter);

    sigc::signal<void, const Glib::ustring&, std::string> signal_DataEdited();

    sigc::signal<void> signal_UnitSelectionChanged();

    Gtk::TreeIter getSelectedUnitIter();

    Gtk::Widget* asWidget();

    void setTreeModel(Glib::RefPtr<Gtk::TreeModel> TreeModel,
        DomainIDataColumns* Columns);
};

// =====================================================================
// =====================================================================


class DomainIDataViewSub: public DomainIDataViewImpl
{
  public:

    Gtk::TreeView* getTreeView()
    {
      return mp_TreeView;
    }

};

#endif /* __DOMAINIDATAVIEW_HPP__ */
