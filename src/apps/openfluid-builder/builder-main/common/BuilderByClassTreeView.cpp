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
 \file BuilderByClassTreeView.cpp
 \brief Implements ...

 \author Aline LIBRES <libres@supagro.inra.fr>
 */

#include "BuilderByClassTreeView.hpp"

#include <glibmm/i18n.h>

// =====================================================================
// =====================================================================


void BuilderByClassTreeView::onClassesViewSelectionChanged()
{
  m_signal_ClassSelectionChanged.emit();
}

// =====================================================================
// =====================================================================


void BuilderByClassTreeView::onUnitsViewSelectionChanged()
{
  m_signal_UnitSelectionChanged.emit();
}

// =====================================================================
// =====================================================================


void BuilderByClassTreeView::onRowActivated(
    const Gtk::TreeModel::Path& /*Path*/, Gtk::TreeViewColumn* /*Column*/)
{
  m_signal_Activated.emit();
}

// =====================================================================
// =====================================================================


void BuilderByClassTreeView::setTreeModel(
    Glib::RefPtr<Gtk::TreeModel> TreeModel)
{
  mp_UnitsView->set_model(TreeModel);
}

// =====================================================================
// =====================================================================


BuilderByClassTreeView::BuilderByClassTreeView()
{
  mp_ClassesView = Gtk::manage(new Gtk::TreeView());
  mp_ClassesView->append_column(_("Unit classes"), m_ClassColumns.m_Class);
  mp_ClassesView->set_visible(true);
  mp_ClassesView->get_selection()->signal_changed().connect(sigc::mem_fun(
      *this, &BuilderByClassTreeView::onClassesViewSelectionChanged));

  mp_UnitsView = Gtk::manage(new Gtk::TreeView());
  mp_UnitsView->get_selection()->signal_changed().connect(sigc::mem_fun(*this,
      &BuilderByClassTreeView::onUnitsViewSelectionChanged));
  mp_UnitsView->set_visible(true);
  mp_UnitsView->signal_row_activated().connect(sigc::mem_fun(*this,
      &BuilderByClassTreeView::onRowActivated));

  mp_ClassesWin = Gtk::manage(new Gtk::ScrolledWindow());
  mp_ClassesWin->set_policy(Gtk::POLICY_AUTOMATIC, Gtk::POLICY_AUTOMATIC);
  mp_ClassesWin->set_visible(true);
  mp_ClassesWin->add(*mp_ClassesView);
  mp_ClassesWin->set_shadow_type(Gtk::SHADOW_ETCHED_IN);

  mp_UnitsWin = Gtk::manage(new Gtk::ScrolledWindow());
  mp_UnitsWin->set_policy(Gtk::POLICY_AUTOMATIC, Gtk::POLICY_AUTOMATIC);
  mp_UnitsWin->set_visible(true);
  mp_UnitsWin->add(*mp_UnitsView);
  mp_UnitsWin->set_shadow_type(Gtk::SHADOW_ETCHED_IN);

  mp_MainBox = Gtk::manage(new Gtk::HBox());
  mp_MainBox->pack_start(*mp_ClassesWin, Gtk::PACK_EXPAND_WIDGET, 5);
  mp_MainBox->pack_start(*mp_UnitsWin, Gtk::PACK_EXPAND_WIDGET, 5);
  mp_MainBox->set_visible(true);
}

// =====================================================================
// =====================================================================


sigc::signal<void> BuilderByClassTreeView::signal_ClassSelectionChanged()
{
  return m_signal_ClassSelectionChanged;
}

// =====================================================================
// =====================================================================


sigc::signal<void> BuilderByClassTreeView::signal_UnitSelectionChanged()
{
  return m_signal_UnitSelectionChanged;
}

// =====================================================================
// =====================================================================


sigc::signal<void> BuilderByClassTreeView::signal_Activated()
{
  return m_signal_Activated;
}

// =====================================================================
// =====================================================================


void BuilderByClassTreeView::setClassesTreeModel(
    Glib::RefPtr<Gtk::TreeModel> ClassesModel)
{
  mp_ClassesView->set_model(ClassesModel);
}

// =====================================================================
// =====================================================================


void BuilderByClassTreeView::setUnitsTreeModel(
    Glib::RefPtr<Gtk::TreeModel> UnitsModel)
{
  mp_UnitsView->set_model(UnitsModel);
}

// =====================================================================
// =====================================================================


void BuilderByClassTreeView::requestClassSelection(Gtk::TreeIter Iter)
{
  if (Iter)
  {
    mp_ClassesView->get_selection()->select(Iter);
    mp_ClassesView->scroll_to_row(mp_ClassesView->get_model()->get_path(Iter));
  }
}

// =====================================================================
// =====================================================================


void BuilderByClassTreeView::requestUnitSelection(Gtk::TreeIter Iter)
{
  if (Iter)
  {
    mp_UnitsView->get_selection()->select(Iter);
    mp_UnitsView->scroll_to_row(mp_UnitsView->get_model()->get_path(Iter));
  }
}

// =====================================================================
// =====================================================================


Gtk::TreeIter BuilderByClassTreeView::getSelectedClassIter()
{
  return mp_ClassesView->get_selection()->get_selected();
}

// =====================================================================
// =====================================================================


Gtk::TreeIter BuilderByClassTreeView::getSelectedUnitIter()
{
  return mp_UnitsView->get_selection()->get_selected();
}

// =====================================================================
// =====================================================================


Gtk::Widget* BuilderByClassTreeView::asWidget()
{
  return mp_MainBox;
}

// =====================================================================
// =====================================================================

// =====================================================================
// =====================================================================


int BuilderByClassTreeViewSub::getClassesViewRowCount()
{
  if (mp_ClassesView->get_model())
    return mp_ClassesView->get_model()->children().size();
  return 0;
}

// =====================================================================
// =====================================================================


int BuilderByClassTreeViewSub::getUnitsViewRowCount()
{
  if (mp_UnitsView->get_model())
    return mp_UnitsView->get_model()->children().size();
  return 0;
}

