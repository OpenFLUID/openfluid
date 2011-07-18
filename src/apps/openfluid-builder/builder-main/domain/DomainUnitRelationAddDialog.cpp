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
 \file DomainUnitRelationAddDialog.cpp
 \brief Implements ...

 \author Aline LIBRES <libres@supagro.inra.fr>
 */

#include "DomainUnitRelationAddDialog.hpp"

#include <gtkmm/stock.h>

#include "EngineHelper.hpp"

// =====================================================================
// =====================================================================


DomainUnitRelationAddDialog::DomainUnitRelationAddDialog() :
  mp_CoreRepos(0)
{
  mp_Dialog = new Gtk::Dialog("Choose Units to link to");

  mref_TreeModel = Gtk::TreeStore::create(m_Columns);

  mp_TreeView = Gtk::manage(new Gtk::TreeView(mref_TreeModel));

  mp_TreeView->append_column("Unit", m_Columns.m_Text);

  mp_TreeView->get_column(0)->set_sort_column(m_Columns.m_Id);

  mp_TreeView->get_selection()->set_mode(Gtk::SELECTION_MULTIPLE);

  mp_TreeView->expand_all();

  mp_ScrolledWin = Gtk::manage(new Gtk::ScrolledWindow());
  mp_ScrolledWin->set_policy(Gtk::POLICY_AUTOMATIC, Gtk::POLICY_AUTOMATIC);
  mp_ScrolledWin->add(*mp_TreeView);

  mp_Dialog->get_vbox()->pack_start(*mp_ScrolledWin);

  mp_Dialog->add_button(Gtk::Stock::CANCEL, Gtk::RESPONSE_CANCEL);
  mp_Dialog->add_button(Gtk::Stock::OK, Gtk::RESPONSE_OK);

  mp_Dialog->set_default_response(Gtk::RESPONSE_OK);

  mp_Dialog->set_default_size(150, 150);

  mp_Dialog->show_all_children();
}

// =====================================================================
// =====================================================================


void DomainUnitRelationAddDialog::setEngineRequirements(
    openfluid::core::CoreRepository& CoreRepos)
{
  mp_CoreRepos = &CoreRepos;
}

// =====================================================================
// =====================================================================


void DomainUnitRelationAddDialog::update(std::set<std::string> ClassNames)
{
  mref_TreeModel->clear();

  for (std::set<std::string>::iterator itClass = ClassNames.begin(); itClass
      != ClassNames.end(); ++itClass)
  {
    Gtk::TreeRow ClassRow = *mref_TreeModel->append();
    ClassRow[m_Columns.m_Class] = *itClass;
    ClassRow[m_Columns.m_Text] = *itClass;

    std::set<int> Ids = EngineHelper::getIDs(mp_CoreRepos, *itClass);

    for (std::set<int>::iterator itId = Ids.begin(); itId != Ids.end(); ++itId)
    {
      Gtk::TreeRow Row = *mref_TreeModel->append(ClassRow->children());
      Row[m_Columns.m_Class] = *itClass;
      Row[m_Columns.m_Id] = *itId;
      Row[m_Columns.m_Text] = Glib::ustring::compose("%1", *itId);
    }
  }

  mp_TreeView->expand_all();
}

// =====================================================================
// =====================================================================


std::list<openfluid::core::Unit*> DomainUnitRelationAddDialog::show()
{
  mp_TreeView->get_selection()->unselect_all();

  std::list<openfluid::core::Unit*> SelectedUnits;

  if (mp_Dialog->run() == Gtk::RESPONSE_OK)
  {
    Gtk::TreeSelection::ListHandle_Path Selecteds =
        mp_TreeView->get_selection()->get_selected_rows();

    for (Gtk::TreeSelection::ListHandle_Path::iterator it = Selecteds.begin(); it
        != Selecteds.end(); ++it)
    {
      std::string Class = mref_TreeModel->get_iter(*it)->get_value(
          m_Columns.m_Class);
      int Id = mref_TreeModel->get_iter(*it)->get_value(m_Columns.m_Id);

      if (Id)
        SelectedUnits.push_back(mp_CoreRepos->getUnit(Class, Id));
    }
  }

  mp_Dialog->hide();

  return SelectedUnits;
}
