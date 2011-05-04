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

#include <boost/foreach.hpp>

// =====================================================================
// =====================================================================


DomainIDataAddDialog::DomainIDataAddDialog() :
  mp_CoreRepos(0), m_ClassName(""), m_NameValid(false), m_ValueValid(false)
{
  mp_Dialog = new Gtk::Dialog("Adding data to the selected class");

  mp_NameMsgLabel = Gtk::manage(new Gtk::Label());
  mp_ValueMsgLabel = Gtk::manage(new Gtk::Label());

  Gtk::Label* NameLabel = Gtk::manage(new Gtk::Label(_("Data Name :"),
      Gtk::ALIGN_LEFT, Gtk::ALIGN_CENTER));
  Gtk::Label* DefaultValueLabel = Gtk::manage(new Gtk::Label(
      _("Data default value :"), Gtk::ALIGN_LEFT, Gtk::ALIGN_CENTER));

  mp_NameEntry = Gtk::manage(new Gtk::Entry());
  mp_NameEntry->signal_changed().connect(sigc::mem_fun(*this,
      &DomainIDataAddDialog::onNameChanged));
  mp_DefaultValueEntry = Gtk::manage(new Gtk::Entry());
  mp_DefaultValueEntry->signal_changed().connect(sigc::mem_fun(*this,
      &DomainIDataAddDialog::onValueChanged));

  onNameChanged();
  onValueChanged();

  Gtk::Table* Table = Gtk::manage(new Gtk::Table());
  Table->attach(*NameLabel, 0, 1, 0, 1);
  Table->attach(*mp_NameEntry, 1, 2, 0, 1);
  Table->attach(*mp_NameMsgLabel, 0, 2, 1, 2);
  Table->attach(*DefaultValueLabel, 0, 1, 2, 3);
  Table->attach(*mp_DefaultValueEntry, 1, 2, 2, 3);
  Table->attach(*mp_ValueMsgLabel, 0, 2, 3, 4);

  mp_Dialog->get_vbox()->pack_start(*Table, true, true, 10);

  mp_Dialog->add_button(Gtk::Stock::CANCEL, Gtk::RESPONSE_CANCEL);
  mp_Dialog->add_button(Gtk::Stock::OK, Gtk::RESPONSE_OK);

  mp_Dialog->set_response_sensitive(Gtk::RESPONSE_OK, false);

  mp_Dialog->show_all_children();
}

// =====================================================================
// =====================================================================


void DomainIDataAddDialog::onNameChanged()
{
  std::string Name = mp_NameEntry->get_text();

  if (Name == "" || Name == " ")
  {
    mp_NameMsgLabel->set_markup(
        _("<span color='red'>Name can not be empty</span>"));
    m_NameValid = false;
  } else if (std::find(m_IDataNames.begin(), m_IDataNames.end(), Name)
      != m_IDataNames.end())
  {
    mp_NameMsgLabel->set_markup(
        _("<span color='red'>This Data Name already exists</span>"));
    m_NameValid = false;
  } else
  {
    mp_NameMsgLabel->set_text("");
    m_NameValid = true;
  }

  mp_Dialog->set_response_sensitive(Gtk::RESPONSE_OK, m_NameValid
      && m_ValueValid);
}

// =====================================================================
// =====================================================================


void DomainIDataAddDialog::onValueChanged()
{
  std::string Val = mp_DefaultValueEntry->get_text();

  if (Val == "" || Val == " ")
  {
    mp_ValueMsgLabel->set_markup(
        _("<span color='red'>Value can not be empty</span>"));
    m_ValueValid = false;
  } else
  {
    mp_ValueMsgLabel->set_text("");
    m_ValueValid = true;
  }

  mp_Dialog->set_response_sensitive(Gtk::RESPONSE_OK, m_NameValid
      && m_ValueValid);
}

// =====================================================================
// =====================================================================


void DomainIDataAddDialog::setEngineRequirements(
    openfluid::core::CoreRepository& CoreRepos)
{
  mp_CoreRepos = &CoreRepos;
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
  m_IDataNames.clear();

  if (mp_CoreRepos->getUnits(m_ClassName))
  {
    BOOST_FOREACH(openfluid::core::Unit Unit,*(mp_CoreRepos->getUnits(m_ClassName)->getList()))
{    BOOST_FOREACH(std::string DataName, Unit.getInputData()->getInputDataNames())
    {
      m_IDataNames.push_back(DataName);
    }
  }
}
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
