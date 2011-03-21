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
 \file SimulOutFileDescView.cpp
 \brief Implements ...

 \author Aline LIBRES <libres@supagro.inra.fr>
 */

#include "SimulOutFileDescView.hpp"

#include <glibmm/i18n.h>

void SimulOutFileDescViewImpl::onNameChanged()
{
  m_signal_NameChanged.emit();
}
void SimulOutFileDescViewImpl::onColSepChanged()
{
  m_signal_ColSepChanged.emit();
}
void SimulOutFileDescViewImpl::onDateFormatChanged()
{
  m_signal_DateFormatChanged.emit();
}
void SimulOutFileDescViewImpl::onCommentCharChanged()
{
  m_signal_CommentCharChanged.emit();
}
void SimulOutFileDescViewImpl::onOkButtonClicked()
{
  m_signal_SaveAsked.emit();
}
void SimulOutFileDescViewImpl::closeDialog()
{
  mp_Dialog->hide();
}

SimulOutFileDescViewImpl::SimulOutFileDescViewImpl()
{
  Gtk::Label* FormatNameLabel = Gtk::manage(new Gtk::Label(_("Format Name")));
  Gtk::Label* ColSepLabel = Gtk::manage(new Gtk::Label(_("Column Separator")));
  Gtk::Label* DateFormatLabel = Gtk::manage(new Gtk::Label(_("Date Format")));
  Gtk::Label* CommentCharLabel = Gtk::manage(new Gtk::Label(
      _("Comment Character")));

  mp_FormatNameEntry = Gtk::manage(new Gtk::Entry());
  mp_FormatNameEntry->signal_changed().connect(sigc::mem_fun(*this,
      &SimulOutFileDescViewImpl::onNameChanged));

  mp_FormatNameLabel = Gtk::manage(new Gtk::Label());

  mp_ColSepComboEntry = Gtk::manage(new Gtk::ComboBoxEntryText());
  mp_ColSepComboEntry->get_entry()->signal_changed().connect(sigc::mem_fun(
      *this, &SimulOutFileDescViewImpl::onColSepChanged));

  mp_DateFormatComboEntry = Gtk::manage(new Gtk::ComboBoxEntryText());
  mp_DateFormatComboEntry->get_entry()->signal_changed().connect(sigc::mem_fun(
      *this, &SimulOutFileDescViewImpl::onDateFormatChanged));

  mp_CommentCharComboEntry = Gtk::manage(new Gtk::ComboBoxEntryText());
  mp_CommentCharComboEntry->get_entry()->signal_changed().connect(
      sigc::mem_fun(*this, &SimulOutFileDescViewImpl::onCommentCharChanged));

  mp_Table = Gtk::manage(new Gtk::Table());
  mp_Table->attach(*FormatNameLabel, 0, 1, 0, 1);
  mp_Table->attach(*ColSepLabel, 0, 1, 1, 2);
  mp_Table->attach(*DateFormatLabel, 0, 1, 2, 3);
  mp_Table->attach(*CommentCharLabel, 0, 1, 3, 4);
  mp_Table->attach(*mp_FormatNameEntry, 1, 2, 0, 1);
  mp_Table->attach(*mp_FormatNameLabel, 1, 2, 0, 1);
  mp_Table->attach(*mp_ColSepComboEntry, 1, 2, 1, 2);
  mp_Table->attach(*mp_DateFormatComboEntry, 1, 2, 2, 3);
  mp_Table->attach(*mp_CommentCharComboEntry, 1, 2, 3, 4);

  mp_Dialog = new Gtk::Dialog();
  mp_Dialog->get_vbox()->pack_start(*mp_Table);

  mp_CancelCreateButton = mp_Dialog->add_button(Gtk::Stock::CANCEL,
      Gtk::RESPONSE_CANCEL);

  mp_OkCreateButton = Gtk::manage(new Gtk::Button(Gtk::Stock::OK));
  mp_OkCreateButton->signal_clicked().connect(sigc::mem_fun(*this,
      &SimulOutFileDescViewImpl::onOkButtonClicked));
  mp_Dialog->get_action_area()->pack_start(*mp_OkCreateButton);

  mp_CloseEditButton = mp_Dialog->add_button(Gtk::Stock::CLOSE,
      Gtk::RESPONSE_CLOSE);

  mp_Dialog->show_all_children();
}
sigc::signal<void> SimulOutFileDescViewImpl::signal_NameChanged()
{
  return m_signal_NameChanged;
}
sigc::signal<void> SimulOutFileDescViewImpl::signal_ColSepChanged()
{
  return m_signal_ColSepChanged;
}
sigc::signal<void> SimulOutFileDescViewImpl::signal_DateFormatChanged()
{
  return m_signal_DateFormatChanged;
}
sigc::signal<void> SimulOutFileDescViewImpl::signal_CommentCharChanged()
{
  return m_signal_CommentCharChanged;
}
sigc::signal<void> SimulOutFileDescViewImpl::signal_SaveAsked()
{
  return m_signal_SaveAsked;
}
sigc::signal<void> SimulOutFileDescViewImpl::signal_CancelAsked()
{
  return m_signal_CancelAsked;
}
void SimulOutFileDescViewImpl::setCreationMode()
{
  m_isInCreationMode = true;

  mp_OkCreateButton->set_visible(true);
  mp_CancelCreateButton->set_visible(true);

  mp_CloseEditButton->set_visible(false);

  mp_Dialog->set_title(_("Files Format Creation"));

  mp_FormatNameEntry->set_visible(true);
  mp_FormatNameLabel->set_visible(false);
}
void SimulOutFileDescViewImpl::setEditionMode()
{
  m_isInCreationMode = false;

  mp_CloseEditButton->set_visible(true);

  mp_OkCreateButton->set_visible(false);
  mp_CancelCreateButton->set_visible(false);

  mp_Dialog->set_title(_("Files Format Edition"));

  mp_FormatNameLabel->set_visible(true);
  mp_FormatNameEntry->set_visible(false);
}
void SimulOutFileDescViewImpl::setChars(std::vector<std::string> Chars)
{
  mp_ColSepComboEntry->clear_items();
  mp_CommentCharComboEntry->clear_items();

  for (unsigned int i = 0; i < Chars.size(); i++)
  {
    mp_ColSepComboEntry->append_text(Chars[i]);
    mp_CommentCharComboEntry->append_text(Chars[i]);
  }
}
void SimulOutFileDescViewImpl::setDateFormats(
    std::vector<std::string> DateFormats)
{
  mp_DateFormatComboEntry->clear_items();
  for (unsigned int i = 0; i < DateFormats.size(); i++)
    mp_DateFormatComboEntry->append_text(DateFormats[i]);
}
void SimulOutFileDescViewImpl::setFormatName(std::string Value)
{
  mp_FormatNameEntry->set_text(Value);
  mp_FormatNameLabel->set_text(Value);
}
void SimulOutFileDescViewImpl::setColSep(std::string Value)
{
  mp_ColSepComboEntry->get_entry()->set_text(Value);
}
void SimulOutFileDescViewImpl::setDateFormat(std::string Value)
{
  mp_DateFormatComboEntry->get_entry()->set_text(Value);
}
void SimulOutFileDescViewImpl::setCommentChar(std::string Value)
{
  mp_CommentCharComboEntry->get_entry()->set_text(Value);
}
std::string SimulOutFileDescViewImpl::getFormatName()
{
  return mp_FormatNameEntry->get_text();
}
std::string SimulOutFileDescViewImpl::getColSep()
{
  return mp_ColSepComboEntry->get_entry()->get_text();
}
std::string SimulOutFileDescViewImpl::getDateFormat()
{
  return mp_DateFormatComboEntry->get_entry()->get_text();
}
std::string SimulOutFileDescViewImpl::getCommentChar()
{
  return mp_CommentCharComboEntry->get_entry()->get_text();
}
void SimulOutFileDescViewImpl::showDialog()
{
  int Res = mp_Dialog->run();

  if (m_isInCreationMode && (Res == Gtk::RESPONSE_CANCEL || Res
      == Gtk::RESPONSE_DELETE_EVENT))
    m_signal_CancelAsked.emit();

  closeDialog();
}
Gtk::Widget* SimulOutFileDescViewImpl::asWidget()
{
  return mp_Table;
}
void SimulOutFileDescViewImpl::showErrorMessageDialog(std::string MessageText)
{
  Gtk::MessageDialog Dialog(MessageText, false, Gtk::MESSAGE_ERROR,
      Gtk::BUTTONS_OK);
  if (Dialog.run())
    Dialog.hide();
}
