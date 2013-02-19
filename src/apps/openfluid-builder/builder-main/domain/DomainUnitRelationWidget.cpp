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
 \file DomainUnitRelationWidget.cpp
 \brief Implements ...

 \author Aline LIBRES <libres@supagro.inra.fr>
 */

#include "DomainUnitRelationWidget.hpp"

#include <glibmm/i18n.h>

#include "BuilderListToolBox.hpp"
#include "DomainUnitRelationAddDialog.hpp"
#include "BuilderListToolBoxFactory.hpp"

// =====================================================================
// =====================================================================


DomainUnitRelationWidget::DomainUnitRelationWidget(Glib::ustring RelationName,
    DomainUnitRelationAddDialog& AddDialog)
{
  mp_AddDialog = &AddDialog;

  mref_TreeModel = Gtk::ListStore::create(m_Columns);
  mref_TreeModel->set_sort_column(m_Columns.m_Id,Gtk::SORT_ASCENDING);

  mp_TreeView = Gtk::manage(new Gtk::TreeView(mref_TreeModel));

  mp_TreeView->append_column("Class", m_Columns.m_Class);
  mp_TreeView->append_column("ID", m_Columns.m_Id);
  mp_TreeView->get_column(0)->set_sort_column(m_Columns.m_Class);
  mp_TreeView->get_column(1)->set_sort_column(m_Columns.m_Id);
  mp_TreeView->set_visible(true);
  mp_TreeView->expand_all();

  mp_ToolBox = BuilderListToolBoxFactory::createDomainUnitRelationToolBox(
      RelationName);
  mp_ToolBox->signal_RemoveCommandAsked().connect(sigc::mem_fun(*this,
      &DomainUnitRelationWidget::onRemoveClicked));
  mp_ToolBox->signal_AddCommandAsked().connect(sigc::mem_fun(*this,
      &DomainUnitRelationWidget::onAddClicked));

  mp_ScrolledWin = Gtk::manage(new Gtk::ScrolledWindow());
  mp_ScrolledWin->set_policy(Gtk::POLICY_AUTOMATIC, Gtk::POLICY_AUTOMATIC);
  mp_ScrolledWin->add(*mp_TreeView);
  mp_ScrolledWin->set_visible(true);

  Gtk::HBox* MainBox = Gtk::manage(new Gtk::HBox());
  MainBox->pack_start(*mp_ScrolledWin, Gtk::PACK_EXPAND_WIDGET);
  MainBox->pack_start(*mp_ToolBox->asWidget(), Gtk::PACK_SHRINK);
  MainBox->set_visible(true);

  mp_MainExpander = Gtk::manage(new Gtk::Expander(Glib::ustring::compose(
      _("\"%1\" units"), RelationName)));
  mp_MainExpander->add(*MainBox);
  mp_MainExpander->set_visible(true);

}

// =====================================================================
// =====================================================================


void DomainUnitRelationWidget::onRemoveClicked()
{
  Gtk::TreeIter SelectedIter = mp_TreeView->get_selection()->get_selected();

  if (SelectedIter)
  {
    mref_TreeModel->erase(SelectedIter);

    updateToolBox();
  }
}

// =====================================================================
// =====================================================================


void DomainUnitRelationWidget::onAddClicked()
{
  std::list<openfluid::core::UnitClassID_t> UnitsToAdd = mp_AddDialog->show();

  addUnits(UnitsToAdd);

  updateToolBox();
}

// =====================================================================
// =====================================================================


void DomainUnitRelationWidget::addUnits(std::list<openfluid::core::UnitClassID_t> UnitsToAdd)
{
  for (std::list<openfluid::core::UnitClassID_t>::iterator it = UnitsToAdd.begin(); it
        != UnitsToAdd.end(); ++it)
    {
      if (!alreadyExistsUnit(*it))
      {
        Gtk::TreeRow Row = *mref_TreeModel->append();
        Row[m_Columns.m_Class] = it->first;
        Row[m_Columns.m_Id] = it->second;
      }
    }
}

// =====================================================================
// =====================================================================


Gtk::Widget* DomainUnitRelationWidget::asWidget()
{
  return mp_MainExpander;
}

// =====================================================================
// =====================================================================


void DomainUnitRelationWidget::clearUnits()
{
  mref_TreeModel->clear();

  mp_ToolBox->setRemoveCommandAvailable(false);
}

// =====================================================================
// =====================================================================


void DomainUnitRelationWidget::appendUnits(
    std::list<openfluid::core::UnitClassID_t> Units)
{
  for (std::list<openfluid::core::UnitClassID_t>::iterator it = Units.begin(); it
      != Units.end(); ++it)
  {
    Gtk::TreeRow Row = *mref_TreeModel->append();

    Row[m_Columns.m_Class] = it->first;
    Row[m_Columns.m_Id] = it->second;
  }

  updateToolBox();
  updateExpander();

  mref_TreeModel->sort_column_changed();
}

// =====================================================================
// =====================================================================


bool DomainUnitRelationWidget::alreadyExistsUnit(openfluid::core::UnitClassID_t Unit)
{
  Gtk::TreeModel::Children Children = mref_TreeModel->children();

  for (Gtk::TreeModel::Children::iterator it = Children.begin(); it
      != Children.end(); ++it)
  {
    if (it->get_value(m_Columns.m_Class) == Unit.first && it->get_value(
        m_Columns.m_Id) == (int) Unit.second)
      return true;
  }

  return false;
}

// =====================================================================
// =====================================================================


void DomainUnitRelationWidget::updateToolBox()
{
  mp_ToolBox->setRemoveCommandAvailable(!mref_TreeModel->children().empty());
}

// =====================================================================
// =====================================================================


void DomainUnitRelationWidget::updateExpander()
{
  if (!mref_TreeModel->children().empty())
  {
    mp_MainExpander->set_expanded();
  }
}

// =====================================================================
// =====================================================================


std::list<openfluid::core::UnitClassID_t> DomainUnitRelationWidget::getUnits()
{
  std::list<openfluid::core::UnitClassID_t> Units;

  Gtk::TreeModel::Children Children = mref_TreeModel->children();

  for (Gtk::TreeModel::Children::iterator it = Children.begin(); it
      != Children.end(); ++it)
  {
    openfluid::core::UnitClassID_t Unit = std::make_pair(it->get_value(m_Columns.m_Class),it->get_value(m_Columns.m_Id));
    Units.push_back(Unit);
  }

  return Units;
}

// =====================================================================
// =====================================================================

