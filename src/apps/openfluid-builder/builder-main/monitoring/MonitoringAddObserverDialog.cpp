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
 \file MonitoringAddObserverDialog.cpp
 \brief Implements ...

 \author Aline LIBRES <aline.libres@gmail.com>
 */

#include "MonitoringAddObserverDialog.hpp"

#include <glibmm/i18n.h>
#include <gtkmm/stock.h>
#include <openfluid/machine/ObserverInstance.hpp>

// =====================================================================
// =====================================================================

MonitoringAddObserverDialog::MonitoringAddObserverDialog(
    openfluid::fluidx::AdvancedMonitoringDescriptor& Monit) :
    m_Monit(Monit)
{
  Gtk::Label* InfoBarLabel = Gtk::manage(
      new Gtk::Label(_("No observer available")));

  mp_InfoBar = Gtk::manage(new Gtk::InfoBar());
  mp_InfoBar->set_message_type(Gtk::MESSAGE_WARNING);
  ((Gtk::Container*) mp_InfoBar->get_content_area())->add(*InfoBarLabel);

  mref_ListStore = Gtk::ListStore::create(m_Columns);

  mp_TreeView = Gtk::manage(new Gtk::TreeView());
  mp_TreeView->append_column(_("Observer ID"), m_Columns.m_Id);
  mp_TreeView->append_column(_("Observer short description"), m_Columns.m_Name);
  mp_TreeView->set_model(mref_ListStore);
  mp_TreeView->get_selection()->set_mode(Gtk::SELECTION_MULTIPLE);

  Gtk::ScrolledWindow* ModelWin = Gtk::manage(new Gtk::ScrolledWindow());
  ModelWin->set_policy(Gtk::POLICY_AUTOMATIC, Gtk::POLICY_AUTOMATIC);
  ModelWin->add(*mp_TreeView);
  ModelWin->set_shadow_type(Gtk::SHADOW_ETCHED_IN);

  mp_Dialog = new Gtk::Dialog(_("Add of observers"));
  mp_Dialog->get_vbox()->pack_start(*mp_InfoBar, Gtk::PACK_SHRINK);
  mp_Dialog->get_vbox()->pack_start(*ModelWin);
  mp_Dialog->set_default_size(700, 200);

  mp_Dialog->add_button(Gtk::Stock::CANCEL, Gtk::RESPONSE_CANCEL);
  mp_Dialog->add_button(Gtk::Stock::OK, Gtk::RESPONSE_OK);

  mp_Dialog->show_all_children();
}

// =====================================================================
// =====================================================================

MonitoringAddObserverDialog::~MonitoringAddObserverDialog()
{

}

// =====================================================================
// =====================================================================

std::set<std::string> MonitoringAddObserverDialog::show()
{
  init();

  if (mp_Dialog->run() == Gtk::RESPONSE_OK)
  {
    mp_TreeView->get_selection()->selected_foreach_iter(
        sigc::mem_fun(*this,
                      &MonitoringAddObserverDialog::selected_row_callback));
  }

  mp_Dialog->hide();

  return m_SelectedIDs;
}

// =====================================================================
// =====================================================================

void MonitoringAddObserverDialog::init()
{
  m_SelectedIDs.clear();

  mref_ListStore->clear();

  std::vector<openfluid::machine::ObserverSignatureInstance*> Unused =
      m_Monit.getUnusedAvailableSignatures();

  if (Unused.empty())
  {
    mp_InfoBar->set_visible(true);
    mp_Dialog->set_response_sensitive(Gtk::RESPONSE_OK, false);
    mp_Dialog->set_default_response(Gtk::RESPONSE_CANCEL);
  }
  else
  {
    mp_InfoBar->set_visible(false);
    mp_Dialog->set_response_sensitive(Gtk::RESPONSE_OK, true);
    mp_Dialog->set_default_response(Gtk::RESPONSE_OK);

    for (unsigned int i = 0; i < Unused.size(); i++)
    {
      Gtk::TreeRow Row = *(mref_ListStore->append());
      Row[m_Columns.m_Id] = Unused[i]->Signature->ID;
      Row[m_Columns.m_Name] = Unused[i]->Signature->Name;
    }

    mp_TreeView->get_selection()->select(mref_ListStore->children().begin());
  }

}

// =====================================================================
// =====================================================================

void MonitoringAddObserverDialog::selected_row_callback(
    const Gtk::TreeModel::iterator& Iter)
{
  Gtk::TreeModel::Row Row = *Iter;
  m_SelectedIDs.insert(Row[m_Columns.m_Id]);
}

// =====================================================================
// =====================================================================

