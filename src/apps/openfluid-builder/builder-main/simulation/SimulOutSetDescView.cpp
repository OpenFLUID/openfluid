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
 \file SimulOutSetDescView.cpp
 \brief Implements ...

 \author Aline LIBRES <libres@supagro.inra.fr>
 */

#include "SimulOutSetDescView.hpp"

#include <glibmm/i18n.h>

// =====================================================================
// =====================================================================


bool SimulOutSetDescViewImpl::IDsListStoreExistsForCurrentClass()
{
  return m_ByClassIDsListStores.find(getClass())
      != m_ByClassIDsListStores.end();
}

// =====================================================================
// =====================================================================


bool SimulOutSetDescViewImpl::VarsListStoreExistsForCurrentClass()
{
  return m_ByClassVarsListStores.find(getClass())
      != m_ByClassVarsListStores.end();
}

// =====================================================================
// =====================================================================


void SimulOutSetDescViewImpl::selectIDs()
{
  if (IDsListStoreExistsForCurrentClass())
  {
    m_HaveIDsToBeStored = false;

    Glib::RefPtr<Gtk::ListStore> IDListStore =
        m_ByClassIDsListStores[getClass()];

    for (Gtk::TreeModel::Children::iterator it =
        IDListStore->children().begin(); it != IDListStore->children().end(); ++it)
    {
      if (std::find(m_IDs.begin(), m_IDs.end(), it->get_value(
          m_Columns.m_IntValue)) != m_IDs.end())
        mp_IDsTreeView->get_selection()->select(it);
    }

    m_HaveIDsToBeStored = true;
  }
}

// =====================================================================
// =====================================================================


void SimulOutSetDescViewImpl::selectVars()
{
  if (VarsListStoreExistsForCurrentClass())
  {
    m_HaveVarsToBeStored = false;

    Glib::RefPtr<Gtk::ListStore> VarListStore =
        m_ByClassVarsListStores[getClass()];

    for (Gtk::TreeModel::Children::iterator it =
        VarListStore->children().begin(); it != VarListStore->children().end(); ++it)
    {
      if (std::find(m_Vars.begin(), m_Vars.end(), it->get_value(
          m_Columns.m_StringValue)) != m_Vars.end())
        mp_VarsTreeView->get_selection()->select(it);
    }

    m_HaveVarsToBeStored = true;
  }
}

// =====================================================================
// =====================================================================


void SimulOutSetDescViewImpl::onNameChanged()
{
  m_signal_NameChanged.emit();
}

// =====================================================================
// =====================================================================


void SimulOutSetDescViewImpl::onClassChanged()
{
  std::string ClassName = getClass();

  if (IDsListStoreExistsForCurrentClass())
  {
    Glib::RefPtr<Gtk::ListStore> IDListStore =
        m_ByClassIDsListStores[ClassName];
    mp_IDsTreeView->set_model(IDListStore);
    selectIDs();
  }

  if (VarsListStoreExistsForCurrentClass())
  {
    Glib::RefPtr<Gtk::ListStore> VarListStore =
        m_ByClassVarsListStores[ClassName];
    mp_VarsTreeView->set_model(VarListStore);
    selectVars();
  }

  m_signal_ClassChanged.emit();
}

// =====================================================================
// =====================================================================


void SimulOutSetDescViewImpl::selectedIdCallback(
    const Gtk::TreeModel::iterator& Iter)
{
  m_IDs.push_back(Iter->get_value(m_Columns.m_IntValue));
}

// =====================================================================
// =====================================================================


void SimulOutSetDescViewImpl::selectedVarCallback(
    const Gtk::TreeModel::iterator& Iter)
{
  m_Vars.push_back(Iter->get_value(m_Columns.m_StringValue));
}

// =====================================================================
// =====================================================================


void SimulOutSetDescViewImpl::onIDsChanged()
{
  if (m_HaveIDsToBeStored)
  {
    m_IDs.clear();
    mp_IDsTreeView->get_selection()->selected_foreach_iter(sigc::mem_fun(*this,
        &SimulOutSetDescViewImpl::selectedIdCallback));
    m_signal_IDsChanged.emit();
  }
}

// =====================================================================
// =====================================================================


void SimulOutSetDescViewImpl::onVarsChanged()
{
  if (m_HaveVarsToBeStored)
  {
    m_Vars.clear();
    mp_VarsTreeView->get_selection()->selected_foreach_iter(sigc::mem_fun(
        *this, &SimulOutSetDescViewImpl::selectedVarCallback));
    m_signal_VarsChanged.emit();
  }
}

// =====================================================================
// =====================================================================


void SimulOutSetDescViewImpl::onFormatNameChanged()
{
  m_signal_FormatNameChanged.emit();
}

// =====================================================================
// =====================================================================


void SimulOutSetDescViewImpl::onPrecisionChanged()
{
  m_signal_PrecisionChanged.emit();
}

// =====================================================================
// =====================================================================


void SimulOutSetDescViewImpl::onOkButtonClicked()
{
  m_signal_SaveAsked.emit();
}

// =====================================================================
// =====================================================================


void SimulOutSetDescViewImpl::closeDialog()
{
  mp_Dialog->hide();
}

// =====================================================================
// =====================================================================


SimulOutSetDescViewImpl::SimulOutSetDescViewImpl()
{
  Gtk::Label* SetNameLabel = Gtk::manage(new Gtk::Label(_("Set name")));
  Gtk::Label* ClassLabel = Gtk::manage(new Gtk::Label(_("Unit class")));
  Gtk::Label* IDsLabel = Gtk::manage(new Gtk::Label(_("IDs")));
  Gtk::Label* VarsLabel = Gtk::manage(new Gtk::Label(_("Variables")));
  Gtk::Label* FormatNameLabel = Gtk::manage(new Gtk::Label(_("Format name")));
  Gtk::Label* PrecisionLabel = Gtk::manage(new Gtk::Label(_("Precision")));

  mp_SetNameEntry = Gtk::manage(new Gtk::Entry());
  mp_SetNameEntry->signal_changed().connect(sigc::mem_fun(*this,
      &SimulOutSetDescViewImpl::onNameChanged));

  mp_SetNameLabel = Gtk::manage(new Gtk::Label());

  mp_ClassCombo = Gtk::manage(new Gtk::ComboBoxText());
  mp_ClassCombo->signal_changed().connect(sigc::mem_fun(*this,
      &SimulOutSetDescViewImpl::onClassChanged));

  mp_FormatNameCombo = Gtk::manage(new Gtk::ComboBoxText());
  mp_FormatNameCombo->signal_changed().connect(sigc::mem_fun(*this,
      &SimulOutSetDescViewImpl::onFormatNameChanged));

  mp_PrecisionSpin = Gtk::manage(new Gtk::SpinButton());
  mp_PrecisionSpin->set_range(0.0, 20.0);
  mp_PrecisionSpin->set_increments(1, 5);
  mp_PrecisionSpin->set_numeric(true);
  mp_PrecisionSpin->signal_value_changed().connect(sigc::mem_fun(*this,
      &SimulOutSetDescViewImpl::onPrecisionChanged));

  mp_IDsTreeView = Gtk::manage(new Gtk::TreeView());
  mp_IDsTreeView->append_column("", m_Columns.m_IntValue);
  mp_IDsTreeView->set_headers_visible(false);
  mp_IDsTreeView->get_selection()->set_mode(Gtk::SELECTION_MULTIPLE);
  mp_IDsTreeView->get_selection()->signal_changed().connect(sigc::mem_fun(
      *this, &SimulOutSetDescViewImpl::onIDsChanged));

  Gtk::ScrolledWindow* WinIDs = Gtk::manage(new Gtk::ScrolledWindow());
  WinIDs->set_policy(Gtk::POLICY_AUTOMATIC, Gtk::POLICY_AUTOMATIC);
  WinIDs->set_visible(true);
  WinIDs->add(*mp_IDsTreeView);

  mp_VarsTreeView = Gtk::manage(new Gtk::TreeView());
  mp_VarsTreeView->append_column("", m_Columns.m_StringValue);
//  mp_VarsTreeView->set_headers_visible(false);
  mp_VarsTreeView->get_selection()->set_mode(Gtk::SELECTION_MULTIPLE);
  mp_VarsTreeView->get_selection()->signal_changed().connect(sigc::mem_fun(
      *this, &SimulOutSetDescViewImpl::onVarsChanged));

  Gtk::ScrolledWindow* WinVars = Gtk::manage(new Gtk::ScrolledWindow());
  WinVars->set_policy(Gtk::POLICY_AUTOMATIC, Gtk::POLICY_AUTOMATIC);
  WinVars->set_visible(true);
  WinVars->add(*mp_VarsTreeView);

  mp_Table = Gtk::manage(new Gtk::Table());
  mp_Table->attach(*SetNameLabel, 0, 1, 0, 1,Gtk::EXPAND|Gtk::FILL,Gtk::SHRINK);
  mp_Table->attach(*ClassLabel, 0, 1, 1, 2,Gtk::EXPAND|Gtk::FILL,Gtk::SHRINK);
  mp_Table->attach(*IDsLabel, 0, 1, 2, 3,Gtk::EXPAND|Gtk::FILL,Gtk::SHRINK);
  mp_Table->attach(*VarsLabel, 0, 1, 3, 4,Gtk::EXPAND|Gtk::FILL,Gtk::SHRINK);
  mp_Table->attach(*FormatNameLabel, 0, 1, 4, 5,Gtk::EXPAND|Gtk::FILL,Gtk::SHRINK);
  mp_Table->attach(*PrecisionLabel, 0, 1, 5, 6,Gtk::EXPAND|Gtk::FILL,Gtk::SHRINK);
  mp_Table->attach(*mp_SetNameEntry, 1, 2, 0, 1,Gtk::EXPAND|Gtk::FILL,Gtk::SHRINK);
  mp_Table->attach(*mp_SetNameLabel, 1, 2, 0, 1,Gtk::EXPAND|Gtk::FILL,Gtk::SHRINK);
  mp_Table->attach(*mp_ClassCombo, 1, 2, 1, 2,Gtk::EXPAND|Gtk::FILL,Gtk::SHRINK);
  mp_Table->attach(*WinIDs, 1, 2, 2, 3, Gtk::EXPAND|Gtk::FILL, Gtk::EXPAND|Gtk::FILL);
  mp_Table->attach(*WinVars, 1, 2, 3, 4,Gtk::EXPAND|Gtk::FILL,Gtk::SHRINK);
  mp_Table->attach(*mp_FormatNameCombo, 1, 2, 4, 5,Gtk::EXPAND|Gtk::FILL,Gtk::SHRINK);
  mp_Table->attach(*mp_PrecisionSpin, 1, 2, 5, 6,Gtk::EXPAND|Gtk::FILL,Gtk::SHRINK);

  mp_Dialog = new Gtk::Dialog();
  mp_Dialog->get_vbox()->pack_start(*mp_Table);

  mp_CancelCreateButton = mp_Dialog->add_button(Gtk::Stock::CANCEL,
      Gtk::RESPONSE_CANCEL);

  mp_OkCreateButton = Gtk::manage(new Gtk::Button(Gtk::Stock::OK));
  mp_OkCreateButton->signal_clicked().connect(sigc::mem_fun(*this,
      &SimulOutSetDescViewImpl::onOkButtonClicked));
  mp_Dialog->get_action_area()->pack_start(*mp_OkCreateButton);

  mp_CloseEditButton = mp_Dialog->add_button(Gtk::Stock::CLOSE,
      Gtk::RESPONSE_CLOSE);

  mp_Dialog->show_all_children();
}

// =====================================================================
// =====================================================================


sigc::signal<void> SimulOutSetDescViewImpl::signal_NameChanged()
{
  return m_signal_NameChanged;
}

// =====================================================================
// =====================================================================


sigc::signal<void> SimulOutSetDescViewImpl::signal_ClassChanged()
{
  return m_signal_ClassChanged;
}

// =====================================================================
// =====================================================================


sigc::signal<void> SimulOutSetDescViewImpl::signal_IDsChanged()
{
  return m_signal_IDsChanged;
}

// =====================================================================
// =====================================================================


sigc::signal<void> SimulOutSetDescViewImpl::signal_VarsChanged()
{
  return m_signal_VarsChanged;
}

// =====================================================================
// =====================================================================


sigc::signal<void> SimulOutSetDescViewImpl::signal_FormatNameChanged()
{
  return m_signal_FormatNameChanged;
}

// =====================================================================
// =====================================================================


sigc::signal<void> SimulOutSetDescViewImpl::signal_PrecisionChanged()
{
  return m_signal_PrecisionChanged;
}

// =====================================================================
// =====================================================================


sigc::signal<void> SimulOutSetDescViewImpl::signal_SaveAsked()
{
  return m_signal_SaveAsked;
}

// =====================================================================
// =====================================================================


sigc::signal<void> SimulOutSetDescViewImpl::signal_CancelAsked()
{
  return m_signal_CancelAsked;
}

// =====================================================================
// =====================================================================


void SimulOutSetDescViewImpl::setCreationMode()
{
  m_isInCreationMode = true;

  mp_OkCreateButton->set_visible(true);
  mp_CancelCreateButton->set_visible(true);

  mp_CloseEditButton->set_visible(false);

  mp_Dialog->set_title(_("Set creation"));

  mp_SetNameEntry->set_visible(true);
  mp_SetNameLabel->set_visible(false);
}

// =====================================================================
// =====================================================================


void SimulOutSetDescViewImpl::setEditionMode()
{
  m_isInCreationMode = false;

  mp_CloseEditButton->set_visible(true);

  mp_OkCreateButton->set_visible(false);
  mp_CancelCreateButton->set_visible(false);

  mp_Dialog->set_title(_("Set edition"));

  mp_SetNameLabel->set_visible(true);
  mp_SetNameEntry->set_visible(false);
}

// =====================================================================
// =====================================================================


void SimulOutSetDescViewImpl::setFormatNames(
    std::vector<std::string> FormatNames)
{
  mp_FormatNameCombo->clear_items();

  for (unsigned int i = 0; i < FormatNames.size(); i++)
    mp_FormatNameCombo->append_text(FormatNames[i]);
}

// =====================================================================
// =====================================================================


void SimulOutSetDescViewImpl::setClasses(std::vector<std::string> Classes)
{
  mp_ClassCombo->clear_items();

  for (unsigned int i = 0; i < Classes.size(); i++)
    mp_ClassCombo->append_text(Classes[i]);
}

// =====================================================================
// =====================================================================


void SimulOutSetDescViewImpl::setByClassIDs(std::map<std::string, std::vector<
    unsigned int> > IDsByClass)
{
  m_ByClassIDsListStores.clear();

  for (std::map<std::string, std::vector<unsigned int> >::iterator it =
      IDsByClass.begin(); it != IDsByClass.end(); ++it)
  {
    Glib::RefPtr<Gtk::ListStore> List = Gtk::ListStore::create(m_Columns);
    for (unsigned int i = 0; i < it->second.size(); i++)
    {
      Gtk::TreeRow Row = *(List->append());
      Row[m_Columns.m_IntValue] = it->second[i];
    }
    m_ByClassIDsListStores[it->first] = List;
  }
}

// =====================================================================
// =====================================================================


void SimulOutSetDescViewImpl::setByClassVars(std::map<std::string, std::vector<
    std::string> > VarsByClass)
{
  m_ByClassVarsListStores.clear();

  for (std::map<std::string, std::vector<std::string> >::iterator it =
      VarsByClass.begin(); it != VarsByClass.end(); ++it)
  {
    Glib::RefPtr<Gtk::ListStore> List = Gtk::ListStore::create(m_Columns);
    for (unsigned int i = 0; i < it->second.size(); i++)
    {
      Gtk::TreeRow Row = *(List->append());
      Row[m_Columns.m_StringValue] = it->second[i];
    }
    m_ByClassVarsListStores[it->first] = List;
  }
}

// =====================================================================
// =====================================================================


void SimulOutSetDescViewImpl::setName(std::string Value)
{
  mp_SetNameEntry->set_text(Value);
  mp_SetNameLabel->set_text(Value);
}

// =====================================================================
// =====================================================================


void SimulOutSetDescViewImpl::setClass(std::string Value)
{
  mp_ClassCombo->set_active_text(Value);
}

// =====================================================================
// =====================================================================


void SimulOutSetDescViewImpl::setFormatName(std::string Value)
{
  mp_FormatNameCombo->set_active_text(Value);
}

// =====================================================================
// =====================================================================


void SimulOutSetDescViewImpl::setPrecision(int Value)
{
  mp_PrecisionSpin->set_value(Value);
}

// =====================================================================
// =====================================================================


void SimulOutSetDescViewImpl::setIDs(std::vector<unsigned int> IDs)
{
  m_IDs = IDs;
  selectIDs();
}

// =====================================================================
// =====================================================================


void SimulOutSetDescViewImpl::setVars(std::vector<std::string> Vars)
{
  m_Vars = Vars;
  selectVars();
}

// =====================================================================
// =====================================================================


std::string SimulOutSetDescViewImpl::getName()
{
  return mp_SetNameEntry->get_text();
}

// =====================================================================
// =====================================================================


std::string SimulOutSetDescViewImpl::getClass()
{
  return mp_ClassCombo->get_active_text();
}

// =====================================================================
// =====================================================================


std::string SimulOutSetDescViewImpl::getFormatName()
{
  return mp_FormatNameCombo->get_active_text();
}

// =====================================================================
// =====================================================================


int SimulOutSetDescViewImpl::getPrecision()
{
  return mp_PrecisionSpin->get_value_as_int();
}

// =====================================================================
// =====================================================================


std::vector<unsigned int> SimulOutSetDescViewImpl::getIDs()
{
  return m_IDs;
}

// =====================================================================
// =====================================================================


std::vector<std::string> SimulOutSetDescViewImpl::getVars()
{
  return m_Vars;
}

// =====================================================================
// =====================================================================


void SimulOutSetDescViewImpl::showDialog()
{
  int Res = mp_Dialog->run();

  if (m_isInCreationMode && (Res == Gtk::RESPONSE_CANCEL || Res
      == Gtk::RESPONSE_DELETE_EVENT))
    m_signal_CancelAsked.emit();

  closeDialog();
}

// =====================================================================
// =====================================================================


Gtk::Widget* SimulOutSetDescViewImpl::asWidget()
{
  return mp_Table;
}

// =====================================================================
// =====================================================================


void SimulOutSetDescViewImpl::showErrorMessageDialog(std::string MessageText)
{
  Gtk::MessageDialog Dialog(MessageText, false, Gtk::MESSAGE_ERROR,
      Gtk::BUTTONS_OK);
  if (Dialog.run())
    Dialog.hide();
}

// =====================================================================
// =====================================================================

// =====================================================================
// =====================================================================


Gtk::TreeView* SimulOutSetDescViewSub::getIDsTreeView()
{
  return mp_IDsTreeView;
}

// =====================================================================
// =====================================================================


Gtk::TreeView* SimulOutSetDescViewSub::getVarsTreeView()
{
  return mp_VarsTreeView;
}
