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
 \file DomainIDataAddDialog.cpp
 \brief Implements ...

 \author Aline LIBRES <libres@supagro.inra.fr>
 */

#include "DomainIDataAddDialog.hpp"

#include <gtkmm/table.h>
#include <gtkmm/stock.h>

#include <glibmm/i18n.h>

#include <openfluid/fluidx/AdvancedDomainDescriptor.hpp>
#include "EngineHelper.hpp"

// =====================================================================
// =====================================================================

DomainIDataAddDialog::DomainIDataAddDialog(
    openfluid::fluidx::AdvancedDomainDescriptor& Domain) :
    mp_Domain(&Domain), m_ClassName(""), m_IsValid(false)
{
  mp_Dialog = new Gtk::Dialog(_("Adding Inputdata field"));
  mp_Dialog->set_default_size(10, 10);

  mp_InfoBarLabel = Gtk::manage(new Gtk::Label());

  mp_InfoBar = Gtk::manage(new Gtk::InfoBar());
  mp_InfoBar->set_message_type(Gtk::MESSAGE_WARNING);
  ((Gtk::Container*) mp_InfoBar->get_content_area())->add(*mp_InfoBarLabel);

  Gtk::Label* NameLabel = Gtk::manage(
      new Gtk::Label(_("Inputdata name:"), Gtk::ALIGN_LEFT, Gtk::ALIGN_CENTER));
  Gtk::Label* DefaultValueLabel = Gtk::manage(
      new Gtk::Label(_("Default value:"), Gtk::ALIGN_LEFT, Gtk::ALIGN_CENTER));

  mp_NameEntry = Gtk::manage(new Gtk::Entry());
  mp_NameEntry->signal_changed().connect(
      sigc::mem_fun(*this, &DomainIDataAddDialog::onChanged));
  mp_NameEntry->set_activates_default(true);

  mp_DefaultValueEntry = Gtk::manage(new Gtk::Entry());
  mp_DefaultValueEntry->signal_changed().connect(
      sigc::mem_fun(*this, &DomainIDataAddDialog::onChanged));
  mp_DefaultValueEntry->set_activates_default(true);

  Gtk::Table* Table = Gtk::manage(new Gtk::Table());
  Table->set_col_spacings(3);
  Table->attach(*NameLabel, 0, 1, 0, 1, Gtk::SHRINK | Gtk::FILL);
  Table->attach(*mp_NameEntry, 1, 2, 0, 1);
  Table->attach(*DefaultValueLabel, 0, 1, 1, 2, Gtk::SHRINK | Gtk::FILL);
  Table->attach(*mp_DefaultValueEntry, 1, 2, 1, 2);

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

void DomainIDataAddDialog::onChanged()
{
  std::string Name = mp_NameEntry->get_text();
  std::string Val = mp_DefaultValueEntry->get_text();

  m_IsValid = false;

  if (Name == "" || EngineHelper::isEmptyString(Name))
  {
    mp_InfoBarLabel->set_text(_("Inputdata name can not be empty"));
  }
  else if (m_IDataNames.count(Name))
  {
    mp_InfoBarLabel->set_text(_("Inputdata name already exists"));
  }
  else if (Val == "" || EngineHelper::isEmptyString(Val))
  {
    mp_InfoBarLabel->set_text(_("Default value cannot be empty"));
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

void DomainIDataAddDialog::setClass(std::string ClassName)
{
  m_ClassName = ClassName;

  update();
}

// =====================================================================
// =====================================================================

void DomainIDataAddDialog::update()
{
  m_IDataNames = mp_Domain->getInputDataNames(m_ClassName);
}

// =====================================================================
// =====================================================================

std::pair<std::string, std::string> DomainIDataAddDialog::show()
{
  mp_NameEntry->set_text("");
  mp_DefaultValueEntry->set_text("");

  std::pair<std::string, std::string> Data = std::make_pair("", "");

  if (mp_Dialog->run() == Gtk::RESPONSE_OK)
  {
    Data = std::make_pair(mp_NameEntry->get_text(),
                          mp_DefaultValueEntry->get_text());
  }

  mp_Dialog->hide();

  return Data;
}
