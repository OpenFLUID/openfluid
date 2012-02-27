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
 \file SimulOutSetsAddEditDialog.cpp
 \brief Implements ...

 \author Aline LIBRES <libres@supagro.inra.fr>
 */

#include "SimulOutSetsAddEditDialog.hpp"

#include <glibmm/i18n.h>

#include <gtkmm/scrolledwindow.h>
#include <gtkmm/stock.h>

#include <boost/foreach.hpp>

#include "EngineHelper.hpp"

// =====================================================================
// =====================================================================


SimulOutSetsAddEditDialog::SimulOutSetsAddEditDialog() :
  mp_OutDesc(0), mp_CoreRepos(0), mp_ModelInstance(0), mp_OutSetDesc(0),
      m_ClassesHaveToUpdate(true), m_EditionOriginalName("")
{
  mp_InfoBar = Gtk::manage(new Gtk::InfoBar());
  mp_InfoBarLabel = Gtk::manage(new Gtk::Label());
  mp_InfoBar->set_message_type(Gtk::MESSAGE_WARNING);
  ((Gtk::Container*) mp_InfoBar->get_content_area())->add(*mp_InfoBarLabel);

  mp_SetNameEntry = Gtk::manage(new Gtk::Entry());
  mp_SetNameEntry->signal_changed().connect(sigc::mem_fun(*this,
      &SimulOutSetsAddEditDialog::onNameChanged));
  mp_SetNameEntry->set_activates_default(true);

  mp_ClassCombo = Gtk::manage(new Gtk::ComboBoxText());
  mp_ClassCombo->signal_changed().connect(sigc::mem_fun(*this,
      &SimulOutSetsAddEditDialog::onClassChanged));

  mref_IdsModel = Gtk::ListStore::create(m_IdsColumns);
  mref_IdsModel->set_sort_column(0, Gtk::SORT_ASCENDING);

  mp_IdsTreeView = Gtk::manage(new Gtk::TreeView(mref_IdsModel));
  mp_IdsTreeView->append_column("", m_IdsColumns.m_Value);
  mp_IdsTreeView->set_headers_visible(false);
  mp_IdsTreeView->get_selection()->set_mode(Gtk::SELECTION_MULTIPLE);

  mref_VarsModel = Gtk::ListStore::create(m_VarsColumns);
  mref_VarsModel->set_sort_column_id(0, Gtk::SORT_ASCENDING);

  mp_VarsTreeView = Gtk::manage(new Gtk::TreeView(mref_VarsModel));
  mp_VarsTreeView->append_column("", m_VarsColumns.m_Value);
  mp_VarsTreeView->set_headers_visible(false);
  mp_VarsTreeView->get_selection()->set_mode(Gtk::SELECTION_MULTIPLE);

  mp_FormatNameCombo = Gtk::manage(new Gtk::ComboBoxText());

  mp_PrecisionSpin = Gtk::manage(new Gtk::SpinButton());
  mp_PrecisionSpin->set_range(0.0, 20.0);
  mp_PrecisionSpin->set_increments(1, 5);
  mp_PrecisionSpin->set_numeric(true);
  mp_PrecisionSpin->set_activates_default(true);

  Gtk::ScrolledWindow* WinIDs = Gtk::manage(new Gtk::ScrolledWindow());
  WinIDs->set_policy(Gtk::POLICY_AUTOMATIC, Gtk::POLICY_AUTOMATIC);
  WinIDs->set_visible(true);
  WinIDs->add(*mp_IdsTreeView);

  Gtk::ScrolledWindow* WinVars = Gtk::manage(new Gtk::ScrolledWindow());
  WinVars->set_policy(Gtk::POLICY_AUTOMATIC, Gtk::POLICY_AUTOMATIC);
  WinVars->set_visible(true);
  WinVars->add(*mp_VarsTreeView);

  mp_Table = Gtk::manage(new Gtk::Table());
  mp_Table->attach(*Gtk::manage(new Gtk::Label(_("Set name"), 1, 0.5)), 0, 1,
      0, 1, Gtk::EXPAND | Gtk::FILL, Gtk::SHRINK);
  mp_Table->attach(*Gtk::manage(new Gtk::Label(_("Unit class"), 1, 0.5)), 0, 1,
      1, 2, Gtk::EXPAND | Gtk::FILL, Gtk::SHRINK);
  mp_Table->attach(*Gtk::manage(new Gtk::Label(_("IDs"), 1, 0.5)), 0, 1, 2, 3,
      Gtk::EXPAND | Gtk::FILL, Gtk::SHRINK);
  mp_Table->attach(*Gtk::manage(new Gtk::Label(_("Variables"), 1, 0.5)), 0, 1,
      3, 4, Gtk::EXPAND | Gtk::FILL, Gtk::SHRINK);
  mp_Table->attach(*Gtk::manage(new Gtk::Label(_("Format name"), 1, 0.5)), 0,
      1, 4, 5, Gtk::EXPAND | Gtk::FILL, Gtk::SHRINK);
  mp_Table->attach(*Gtk::manage(new Gtk::Label(_("Precision"), 1, 0.5)), 0, 1,
      5, 6, Gtk::EXPAND | Gtk::FILL, Gtk::SHRINK);
  mp_Table->attach(*mp_SetNameEntry, 1, 2, 0, 1, Gtk::EXPAND | Gtk::FILL,
      Gtk::SHRINK);
  mp_Table->attach(*mp_ClassCombo, 1, 2, 1, 2, Gtk::EXPAND | Gtk::FILL,
      Gtk::SHRINK);
  mp_Table->attach(*WinIDs, 1, 2, 2, 3, Gtk::EXPAND | Gtk::FILL, Gtk::EXPAND
      | Gtk::FILL);
  mp_Table->attach(*WinVars, 1, 2, 3, 4, Gtk::EXPAND | Gtk::FILL);
  mp_Table->attach(*mp_FormatNameCombo, 1, 2, 4, 5, Gtk::EXPAND | Gtk::FILL,
      Gtk::SHRINK);
  mp_Table->attach(*mp_PrecisionSpin, 1, 2, 5, 6, Gtk::EXPAND | Gtk::FILL,
      Gtk::SHRINK);
  mp_Table->set_col_spacings(3);
  mp_Table->set_row_spacings(5);
  mp_Table->set_border_width(5);

  mp_Dialog = new Gtk::Dialog();
  mp_Dialog->get_vbox()->pack_start(*mp_InfoBar, Gtk::PACK_SHRINK);
  mp_Dialog->get_vbox()->pack_start(*mp_Table);

  mp_Dialog->add_button(Gtk::Stock::CANCEL, Gtk::RESPONSE_CANCEL);
  mp_Dialog->add_button(Gtk::Stock::OK, Gtk::RESPONSE_OK);

  mp_Dialog->set_default_response(Gtk::RESPONSE_OK);

  mp_Dialog->show_all_children();
}

// =====================================================================
// =====================================================================


void SimulOutSetsAddEditDialog::onNameChanged()
{
  Glib::ustring SetName = mp_SetNameEntry->get_text();

  bool m_IsValid = false;

  if (SetName.empty())
  {
    mp_InfoBarLabel->set_text(_("Set name cannot be empty"));
  }
  else if (SetName != m_EditionOriginalName && m_SetNames.find(SetName)
      != m_SetNames.end())
  {
    mp_InfoBarLabel->set_text(_("This Set name already exists"));
  }
  else
  {
    m_IsValid = true;
  }

  mp_InfoBar->set_visible(!m_IsValid);
  mp_Dialog->set_response_sensitive(Gtk::RESPONSE_OK, m_IsValid);
}

// =====================================================================
// =====================================================================


void SimulOutSetsAddEditDialog::onClassChanged()
{
  if (!m_ClassesHaveToUpdate)
    return;

  std::string ClassName = mp_ClassCombo->get_active_text();

  createIdRows(ClassName);
  selectIDRows();

  createVarRows(ClassName);
  selectVarRows();
}

// =====================================================================
// =====================================================================


void SimulOutSetsAddEditDialog::createIdRows(std::string ClassName)
{
  mref_IdsModel->clear();

  for (openfluid::core::UnitsList_t::const_iterator it =
      mp_CoreRepos->getUnitsByClass()->at(ClassName).getList()->begin(); it
      != mp_CoreRepos->getUnitsByClass()->at(ClassName).getList()->end(); ++it)
  {
    Gtk::TreeRow Row = *mref_IdsModel->append();
    Row[m_IdsColumns.m_Value] = it->getID();
  }

  mref_IdsModel->sort_column_changed();
}

// =====================================================================
// =====================================================================


void SimulOutSetsAddEditDialog::selectIDRows()
{
  if (!mp_OutSetDesc)
    mp_IdsTreeView->get_selection()->select_all();
  else
  {
    if (mp_OutSetDesc->isAllUnits())
      mp_IdsTreeView->get_selection()->select_all();
    else
    {
      for (Gtk::TreeModel::Children::iterator it =
          mref_IdsModel->children().begin(); it
          != mref_IdsModel->children().end(); ++it)
      {
        if (std::find(mp_OutSetDesc->getUnitsIDs().begin(),
            mp_OutSetDesc->getUnitsIDs().end(), it->get_value(
                m_IdsColumns.m_Value)) != mp_OutSetDesc->getUnitsIDs().end())
          mp_IdsTreeView->get_selection()->select(it);
      }
    }
  }
}

// =====================================================================
// =====================================================================


void SimulOutSetsAddEditDialog::createVarRows(std::string ClassName)
{
  mref_VarsModel->clear();

  BOOST_FOREACH(std::string VarName, EngineHelper::getProducedVarNames(
      ClassName, mp_ModelInstance))
  {
    Gtk::TreeRow Row = *mref_VarsModel->append();
    Row[m_VarsColumns.m_Value] = VarName;
  }

mref_VarsModel->sort_column_changed();
}

// =====================================================================
// =====================================================================


void SimulOutSetsAddEditDialog::selectVarRows()
{
  if (!mp_OutSetDesc)
    mp_VarsTreeView->get_selection()->select_all();
  else
  {
    if (mp_OutSetDesc->isAllVariables())
    {
      mp_VarsTreeView->get_selection()->select_all();
    }
    else
    {
      for (Gtk::TreeModel::Children::iterator it =
          mref_VarsModel->children().begin(); it
          != mref_VarsModel->children().end(); ++it)
      {
        std::string VarName = it->get_value(m_VarsColumns.m_Value);

        if (std::find(mp_OutSetDesc->getVariables().begin(),
                      mp_OutSetDesc->getVariables().end(), VarName)
                      != mp_OutSetDesc->getVariables().end())
        {
          mp_VarsTreeView->get_selection()->select(it);
        }
      }
    }
  }
}

// =====================================================================
// =====================================================================


void SimulOutSetsAddEditDialog::setEngineRequirements(
    openfluid::base::OutputDescriptor& OutDesc,
    openfluid::core::CoreRepository& CoreRepos,
    openfluid::machine::ModelInstance& ModelInstance)
{
  mp_OutDesc = &OutDesc;
  mp_CoreRepos = &CoreRepos;
  mp_ModelInstance = &ModelInstance;

  update();
}

// =====================================================================
// =====================================================================


void SimulOutSetsAddEditDialog::update()
{
  if (!mp_OutDesc || !mp_CoreRepos || !mp_ModelInstance)
    return;

  m_SetNames.clear();
  BOOST_FOREACH(openfluid::base::OutputFilesDescriptor FileDesc,mp_OutDesc->getFileSets())
{  ;
  BOOST_FOREACH(openfluid::base::OutputSetDescriptor SetDesc,FileDesc.getSets())
  {
    m_SetNames.insert(SetDesc.getName());
  }
}

m_ClassesHaveToUpdate = false;
mp_ClassCombo->clear_items();
BOOST_FOREACH(std::string ClassName,EngineHelper::getClassNames(mp_CoreRepos))
{
  mp_ClassCombo->append_text(ClassName);
}
m_ClassesHaveToUpdate = true;

mp_FormatNameCombo->clear_items();
for(unsigned int i=0; i<mp_OutDesc->getFileSets().size();i++)
mp_FormatNameCombo->append_text(mp_OutDesc->getFileSets()[i].getName());

}

// =====================================================================
// =====================================================================


std::pair<openfluid::base::OutputSetDescriptor*, std::string> SimulOutSetsAddEditDialog::show(
    openfluid::base::OutputSetDescriptor* OutSetDesc)
{
  mp_OutSetDesc = OutSetDesc;

  if (mp_OutSetDesc)
    initEditionMode();
  else
    initAdditionMode();

  if (mp_Dialog->run() == Gtk::RESPONSE_OK)
  {
    openfluid::base::OutputSetDescriptor* NewOutSetDesc =
        new openfluid::base::OutputSetDescriptor();

    NewOutSetDesc->setName(mp_SetNameEntry->get_text());

    NewOutSetDesc->setUnitsClass(mp_ClassCombo->get_active_text());

    setNewSetIDs(NewOutSetDesc);

    setNewSetVars(NewOutSetDesc);

    mp_PrecisionSpin->update();
    NewOutSetDesc->setPrecision(mp_PrecisionSpin->get_value_as_int());

    mp_Dialog->hide();

    return std::make_pair(NewOutSetDesc, mp_FormatNameCombo->get_active_text());
  }

  mp_Dialog->hide();

  return std::make_pair((openfluid::base::OutputSetDescriptor*) 0, "");

}

// =====================================================================
// =====================================================================


void SimulOutSetsAddEditDialog::setNewSetIDs(
    openfluid::base::OutputSetDescriptor* NewOutSetDesc)
{
  std::string ClassName = NewOutSetDesc->getUnitsClass();

  Gtk::TreeSelection::ListHandle_Path SelectedIds =
      mp_IdsTreeView->get_selection()->get_selected_rows();

  if (SelectedIds.size()
      == mp_CoreRepos->getUnitsByClass()->at(ClassName).getList()->size())
    NewOutSetDesc->setAllUnits(true);
  else
  {
    NewOutSetDesc->setAllUnits(false);

    for (Gtk::TreeSelection::ListHandle_Path::iterator it = SelectedIds.begin(); it
        != SelectedIds.end(); ++it)
    {
      NewOutSetDesc->getUnitsIDs().push_back(
          mref_IdsModel->get_iter(*it)->get_value(m_IdsColumns.m_Value));
    }
  }
}

// =====================================================================
// =====================================================================


void SimulOutSetsAddEditDialog::setNewSetVars(
    openfluid::base::OutputSetDescriptor* NewOutSetDesc)
{
  std::string ClassName = NewOutSetDesc->getUnitsClass();

  unsigned int VarsCount = EngineHelper::getProducedVarNames(ClassName,mp_ModelInstance).size();

  Gtk::TreeSelection::ListHandle_Path SelectedVars =
      mp_VarsTreeView->get_selection()->get_selected_rows();

  if (SelectedVars.size() == VarsCount)
  {
    NewOutSetDesc->setAllVariables(true);
  }
  else
  {
    NewOutSetDesc->setAllVariables(false);

    for (Gtk::TreeSelection::ListHandle_Path::iterator it =
        SelectedVars.begin(); it != SelectedVars.end(); ++it)
    {
      std::string VarName = mref_VarsModel->get_iter(*it)->get_value(
          m_VarsColumns.m_Value);

      NewOutSetDesc->getVariables().push_back(VarName);
    }
  }
}

// =====================================================================
// =====================================================================


void SimulOutSetsAddEditDialog::initEditionMode()
{
  mp_Dialog->set_title(_("Output set edition"));

  m_EditionOriginalName = mp_OutSetDesc->getName();

  mp_SetNameEntry->set_text(m_EditionOriginalName);

  mp_ClassCombo->set_active_text(mp_OutSetDesc->getUnitsClass());

  for (unsigned int i = 0; i < mp_OutDesc->getFileSets().size(); i++)
  {
    for (unsigned int j = 0; j < mp_OutDesc->getFileSets()[i].getSets().size(); j++)
    {
      if (mp_OutDesc->getFileSets()[i].getSets()[j].getName()
          == mp_OutSetDesc->getName())
        mp_FormatNameCombo->set_active_text(
            mp_OutDesc->getFileSets()[i].getName());
    }
  }

  onClassChanged();

  mp_PrecisionSpin->set_value(mp_OutSetDesc->getPrecision());
}

// =====================================================================
// =====================================================================


void SimulOutSetsAddEditDialog::initAdditionMode()
{
  mp_Dialog->set_title(_("Output set creation"));

  m_EditionOriginalName = "";

  mp_SetNameEntry->set_text("");

  mp_ClassCombo->set_active(0);

  mp_FormatNameCombo->set_active(0);

  mp_PrecisionSpin->set_value(5);

  onClassChanged();

  onNameChanged();
}
