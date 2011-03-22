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
 \file DomainUnitCreationView.cpp
 \brief Implements ...

 \author Aline LIBRES <libres@supagro.inra.fr>
 */

#include "DomainUnitCreationView.hpp"

#include <glibmm/i18n.h>

void DomainUnitCreationViewImpl::onClassChanged()
{
  m_signal_ClassChanged.emit();
}
void DomainUnitCreationViewImpl::onIdChanged()
{
  m_signal_IdChanged.emit();
}
void DomainUnitCreationViewImpl::onOkButtonClicked()
{
  m_signal_SaveAsked.emit();
}

DomainUnitCreationViewImpl::DomainUnitCreationViewImpl()
{
  Gtk::Label* ClassLabel = Gtk::manage(new Gtk::Label(_("Class Unit")));

  mp_ClassComboEntry = Gtk::manage(new Gtk::ComboBoxEntryText());
  mp_ClassComboEntry->get_entry()->signal_changed().connect(sigc::mem_fun(
      *this, &DomainUnitCreationViewImpl::onClassChanged));

  Gtk::Label* IdLabel = Gtk::manage(new Gtk::Label(_("Id")));

  mp_IdSpin = Gtk::manage(new Gtk::SpinButton());
  mp_IdSpin->set_range(1.0, 9999.0);
  mp_IdSpin->set_increments(1, 10);
  mp_IdSpin->set_numeric(true);
  mp_IdSpin->signal_value_changed().connect(sigc::mem_fun(*this,
      &DomainUnitCreationViewImpl::onIdChanged));

  mp_InfoTable = Gtk::manage(new Gtk::Table());
  mp_InfoTable->attach(*ClassLabel, 0, 1, 0, 1);
  mp_InfoTable->attach(*IdLabel, 0, 1, 1, 2);
  mp_InfoTable->attach(*mp_ClassComboEntry, 1, 2, 0, 1);
  mp_InfoTable->attach(*mp_IdSpin, 1, 2, 1, 2);

  mp_Dialog = new Gtk::Dialog();
  mp_Dialog->get_vbox()->pack_start(*mp_InfoTable);

  mp_CancelCreateButton = mp_Dialog->add_button(Gtk::Stock::CANCEL,
      Gtk::RESPONSE_CANCEL);

  mp_OkCreateButton = Gtk::manage(new Gtk::Button(Gtk::Stock::OK));
  mp_OkCreateButton->signal_clicked().connect(sigc::mem_fun(*this,
      &DomainUnitCreationViewImpl::onOkButtonClicked));
  mp_Dialog->get_action_area()->pack_start(*mp_OkCreateButton);

  mp_Dialog->set_title(_("Unit Creation"));

  mp_Dialog->show_all_children();
}
sigc::signal<void> DomainUnitCreationViewImpl::signal_ClassChanged()
{
  return m_signal_ClassChanged;
}
sigc::signal<void> DomainUnitCreationViewImpl::signal_IdChanged()
{
  return m_signal_IdChanged;
}
sigc::signal<void> DomainUnitCreationViewImpl::signal_SaveAsked()
{
  return m_signal_SaveAsked;
}
void DomainUnitCreationViewImpl::setClassNames(
    std::vector<std::string> ClassNames)
{
  mp_ClassComboEntry->clear_items();
  for (unsigned int i = 0; i < ClassNames.size(); i++)
    mp_ClassComboEntry->append_text(ClassNames[i]);
}
void DomainUnitCreationViewImpl::setClass(std::string ClassName)
{
  mp_ClassComboEntry->get_entry()->set_text(ClassName);
}
void DomainUnitCreationViewImpl::setId(int Id)
{
  mp_IdSpin->set_value(Id);
}
std::string DomainUnitCreationViewImpl::getClassName()
{
  return mp_ClassComboEntry->get_entry()->get_text();
}
int DomainUnitCreationViewImpl::getId()
{
  return mp_IdSpin->get_value_as_int();
}
void DomainUnitCreationViewImpl::closeDialog()
{
  mp_Dialog->hide();
}
void DomainUnitCreationViewImpl::showDialog()
{
  if (mp_Dialog->run() == Gtk::RESPONSE_CANCEL)
    closeDialog();
}
Gtk::Widget* DomainUnitCreationViewImpl::asWidget()
{
  return mp_InfoTable;
}
void DomainUnitCreationViewImpl::showErrorMessageDialog(std::string MessageText)
{
  Gtk::MessageDialog Dialog(MessageText, false, Gtk::MESSAGE_ERROR,
      Gtk::BUTTONS_OK);
  if (Dialog.run())
    Dialog.hide();
}
