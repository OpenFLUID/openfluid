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
 \file DomainIDataEditDialog.cpp
 \brief Implements ...

 \author Aline LIBRES <libres@supagro.inra.fr>
 */

#include "DomainIDataEditDialog.hpp"

#include <gtkmm/table.h>
#include <gtkmm/stock.h>

#include <glibmm/i18n.h>

#include <openfluid/fluidx/AdvancedDomainDescriptor.hpp>
#include "EngineHelper.hpp"

// =====================================================================
// =====================================================================

DomainIDataEditDialog::DomainIDataEditDialog(
    openfluid::fluidx::AdvancedDomainDescriptor& Domain) :
    mp_Domain(&Domain), m_ClassName(""), m_IsValid(false)
{
  mp_Dialog = new Gtk::Dialog(_("Changing Inputdata name"));
  mp_Dialog->set_default_size(10, 10);

  mp_InfoBarLabel = Gtk::manage(new Gtk::Label());

  mp_InfoBar = Gtk::manage(new Gtk::InfoBar());
  mp_InfoBar->set_message_type(Gtk::MESSAGE_WARNING);
  ((Gtk::Container*) mp_InfoBar->get_content_area())->add(*mp_InfoBarLabel);

  Gtk::Label* OldNameLabel = Gtk::manage(
      new Gtk::Label(_("Inputdata name:"), Gtk::ALIGN_LEFT, Gtk::ALIGN_CENTER));
  Gtk::Label* NewNameLabel = Gtk::manage(
      new Gtk::Label(_("New Inputdata name:"), Gtk::ALIGN_LEFT,
                     Gtk::ALIGN_CENTER));

  mp_Combo = Gtk::manage(new Gtk::ComboBoxText());

  mp_NewNameEntry = Gtk::manage(new Gtk::Entry());
  mp_NewNameEntry->signal_changed().connect(
      sigc::mem_fun(*this, &DomainIDataEditDialog::onChanged));
  mp_NewNameEntry->set_activates_default(true);

  Gtk::Table* Table = Gtk::manage(new Gtk::Table());
  Table->set_col_spacings(3);
  Table->attach(*OldNameLabel, 0, 1, 0, 1, Gtk::SHRINK | Gtk::FILL);
  Table->attach(*mp_Combo, 1, 2, 0, 1);
  Table->attach(*NewNameLabel, 0, 1, 1, 2, Gtk::SHRINK | Gtk::FILL);
  Table->attach(*mp_NewNameEntry, 1, 2, 1, 2);

  mp_Dialog->get_vbox()->pack_start(*mp_InfoBar, Gtk::PACK_SHRINK, 5);
  mp_Dialog->get_vbox()->pack_start(*Table, Gtk::PACK_SHRINK, 5);

  mp_Dialog->add_button(Gtk::Stock::CANCEL, Gtk::RESPONSE_CANCEL);
  mp_Dialog->add_button(Gtk::Stock::OK, Gtk::RESPONSE_OK);

  mp_Dialog->set_default_response(Gtk::RESPONSE_OK);

  mp_Dialog->show_all_children();

  onChanged();
}

// =====================================================================
// =====================================================================

void DomainIDataEditDialog::onChanged()
{
  std::string NewName = mp_NewNameEntry->get_text();

  m_IsValid = false;

  if (NewName == "" || EngineHelper::isEmptyString(NewName))
  {
    mp_InfoBarLabel->set_text(_("Inputdata name can not be empty"));
  }
  else if (m_IDataNames.find(NewName) != m_IDataNames.end())
  {
    mp_InfoBarLabel->set_text(_("Inputdata name already exists"));
  }
  else
  {
    m_IsValid = true;
  }

  mp_InfoBar->set_visible(!m_IsValid);
  mp_Dialog->set_response_sensitive(Gtk::RESPONSE_OK, m_IsValid);
  mp_Dialog->resize(10, 10);
}

// =====================================================================
// =====================================================================

void DomainIDataEditDialog::setClass(std::string ClassName)
{
  m_ClassName = ClassName;

  update();
}

// =====================================================================
// =====================================================================

void DomainIDataEditDialog::update()
{
  mp_Combo->clear_items();

  std::set<std::string> IDataNames = mp_Domain->getInputDataNames(m_ClassName);

  for (std::set<std::string>::iterator it = IDataNames.begin();
      it != IDataNames.end(); ++it)
    mp_Combo->append_text(*it);

  mp_Combo->set_active(0);
}

// =====================================================================
// =====================================================================

std::pair<std::string, std::string> DomainIDataEditDialog::show()
{
  mp_NewNameEntry->set_text("");

  std::pair<std::string, std::string> Data = std::make_pair("", "");

  if (mp_Dialog->run() == Gtk::RESPONSE_OK)
  {
    Data = std::make_pair(mp_Combo->get_active_text(),
                          mp_NewNameEntry->get_text());
  }

  mp_Dialog->hide();

  return Data;
}
