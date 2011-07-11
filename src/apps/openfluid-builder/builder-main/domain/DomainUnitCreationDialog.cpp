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
 \file DomainUnitCreationDialog.cpp
 \brief Implements ...

 \author Aline LIBRES <libres@supagro.inra.fr>
 */

#include "DomainUnitCreationDialog.hpp"

#include <glibmm/i18n.h>
#include <gtkmm/stock.h>

#include "EngineHelper.hpp"

// =====================================================================
// =====================================================================


DomainUnitCreationDialog::DomainUnitCreationDialog() :
  mp_CoreRepos(0)
{
  mp_InfoBarLabel = Gtk::manage(new Gtk::Label());

  mp_InfoBar = Gtk::manage(new Gtk::InfoBar());
  mp_InfoBar->set_message_type(Gtk::MESSAGE_WARNING);
  ((Gtk::Container*) mp_InfoBar->get_content_area())->add(*mp_InfoBarLabel);

  mp_ClassComboEntryText = Gtk::manage(new Gtk::ComboBoxEntryText());
  mp_ClassComboEntryText->get_entry()->signal_changed().connect(sigc::mem_fun(
      *this, &DomainUnitCreationDialog::onClassChanged));
  mp_ClassComboEntryText->get_entry()->set_activates_default(true);

  mp_IdSpin = Gtk::manage(new Gtk::SpinButton());
  mp_IdSpin->set_range(1.0, 9999.0);
  mp_IdSpin->set_increments(1, 10);
  mp_IdSpin->set_numeric(true);
  mp_IdSpin->signal_changed().connect(sigc::mem_fun(*this,
      &DomainUnitCreationDialog::onIdChanged));

  mp_PcsOrderSpin = Gtk::manage(new Gtk::SpinButton());
  mp_PcsOrderSpin->set_range(1.0, 9999.0);
  mp_PcsOrderSpin->set_increments(1, 10);
  mp_PcsOrderSpin->set_numeric(true);

  mp_InfoTable = Gtk::manage(new Gtk::Table());
  mp_InfoTable->attach(*Gtk::manage(new Gtk::Label(_("Unit class:"), 1, 0.5)),
      0, 1, 0, 1, Gtk::FILL | Gtk::SHRINK, Gtk::SHRINK);
  mp_InfoTable->attach(*Gtk::manage(new Gtk::Label(_("ID:"), 1, 0.5)), 0, 1, 1,
      2, Gtk::FILL | Gtk::SHRINK, Gtk::SHRINK);
  mp_InfoTable->attach(
      *Gtk::manage(new Gtk::Label(_("Process order:"), 1, 0.5)), 0, 1, 2, 3,
      Gtk::FILL | Gtk::SHRINK, Gtk::SHRINK);
  mp_InfoTable->attach(*mp_ClassComboEntryText, 1, 2, 0, 1, Gtk::EXPAND,
      Gtk::SHRINK);
  mp_InfoTable->attach(*mp_IdSpin, 1, 2, 1, 2, Gtk::FILL | Gtk::SHRINK,
      Gtk::SHRINK);
  mp_InfoTable->attach(*mp_PcsOrderSpin, 1, 2, 2, 3, Gtk::FILL | Gtk::SHRINK,
      Gtk::SHRINK);
  mp_InfoTable->set_col_spacings(3);
  mp_InfoTable->set_row_spacings(5);
  mp_InfoTable->set_border_width(5);

  mp_Dialog = new Gtk::Dialog(_("Unit creation"));

  mp_Dialog->get_vbox()->pack_start(*mp_InfoBar);
  mp_Dialog->get_vbox()->pack_start(*mp_InfoTable);

  mp_Dialog->set_default_size(10, 10);

  mp_Dialog->set_default_response(Gtk::RESPONSE_OK);

  mp_Dialog->add_button(Gtk::Stock::CANCEL, Gtk::RESPONSE_CANCEL);
  mp_Dialog->add_button(Gtk::Stock::OK, Gtk::RESPONSE_OK);

  mp_Dialog->show_all_children();
}

// =====================================================================
// =====================================================================


void DomainUnitCreationDialog::onClassChanged()
{
  std::string ClassName = mp_ClassComboEntryText->get_entry()->get_text();

  bool IsValid = true;

  if (EngineHelper::isEmptyString(ClassName))
  {
    mp_InfoBarLabel->set_text(_("ClassName cannot be empty"));
    IsValid = false;
  }

  mp_InfoBar->set_visible(!IsValid);
  mp_Dialog->set_response_sensitive(Gtk::RESPONSE_OK, IsValid);
  mp_Dialog->resize(10, 10);

  if (IsValid)
  {
    m_IDs = EngineHelper::getIDs(mp_CoreRepos, ClassName);

    if (m_IDs.empty())
      mp_IdSpin->set_value(1);
    else
      mp_IdSpin->set_value((*std::max_element(m_IDs.begin(), m_IDs.end())) + 1);
  }
}

// =====================================================================
// =====================================================================


void DomainUnitCreationDialog::onIdChanged()
{
  mp_IdSpin->update();

  bool IsValid = true;

  if (m_IDs.count(mp_IdSpin->get_value_as_int()))
  {
    mp_InfoBarLabel->set_text(_("This unit already exists"));
    IsValid = false;
  }

  mp_InfoBar->set_visible(!IsValid);
  mp_Dialog->set_response_sensitive(Gtk::RESPONSE_OK, IsValid);
  mp_Dialog->resize(10, 10);
}

// =====================================================================
// =====================================================================


void DomainUnitCreationDialog::setEngineRequirements(
    openfluid::core::CoreRepository& CoreRepos)
{
  mp_CoreRepos = &CoreRepos;

  update();
}

// =====================================================================
// =====================================================================


void DomainUnitCreationDialog::update()
{
  mp_ClassComboEntryText->clear_items();

  std::set<std::string> Classes = EngineHelper::getClassNames(mp_CoreRepos);

  for (std::set<std::string>::iterator it = Classes.begin(); it
      != Classes.end(); ++it)
  {
    mp_ClassComboEntryText->append_text(*it);
  }

  onClassChanged();
}

// =====================================================================
// =====================================================================


openfluid::core::Unit* DomainUnitCreationDialog::show(std::string SelectedClass)
{
  mp_ClassComboEntryText->get_entry()->set_text(SelectedClass);

  mp_PcsOrderSpin->set_value(1);

  openfluid::core::Unit* NewUnit = 0;

  if (mp_Dialog->run() == Gtk::RESPONSE_OK)
  {
    mp_PcsOrderSpin->update();

    NewUnit = new openfluid::core::Unit(
        mp_ClassComboEntryText->get_entry()->get_text(),
        mp_IdSpin->get_value_as_int(),
        static_cast<unsigned int> (mp_PcsOrderSpin->get_value_as_int()),
        openfluid::core::Unit::DESCRIPTOR);
  }

  mp_Dialog->hide();

  return NewUnit;
}
