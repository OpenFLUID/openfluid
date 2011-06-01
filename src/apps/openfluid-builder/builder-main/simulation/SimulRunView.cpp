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
 \file SimulRunView.cpp
 \brief Implements ...

 \author Aline LIBRES <libres@supagro.inra.fr>
 */

#include "SimulRunView.hpp"

#include <glibmm/i18n.h>

#include "BuilderFrame.hpp"

void SimulRunViewImpl::onDeltaChanged()
{
  m_signal_DeltaChanged.emit();
}
void SimulRunViewImpl::onBeginChanged()
{
  m_signal_BeginChanged.emit();
}
void SimulRunViewImpl::onEndChanged()
{
  m_signal_EndChanged.emit();
}
void SimulRunViewImpl::onValuesBuffToggle()
{
  m_signal_ValuesBuffToggle.emit();
}
void SimulRunViewImpl::onValuesBuffChanged()
{
  m_signal_ValuesBuffChanged.emit();
}
void SimulRunViewImpl::onFilesBuffChanged()
{
  m_signal_FilesBuffChanged.emit();
}

std::string SimulRunViewImpl::getColorForBGValidState(bool IsValid)
{
  if (IsValid)
    return "white";
  return "red";
}

SimulRunViewImpl::SimulRunViewImpl()
{
  Gtk::Label* DeltaLabel = Gtk::manage(new Gtk::Label(_("Delta T")));
  Gtk::Label* DeltaUnit = Gtk::manage(new Gtk::Label(_("seconds")));
  mp_DeltaSpin = Gtk::manage(new Gtk::SpinButton());
  mp_DeltaSpin->set_range(1.0, 86400.0); //->24h
  mp_DeltaSpin->set_increments(1, 3600);
  mp_DeltaSpin->set_numeric(true);
  mp_DeltaSpin->signal_value_changed().connect(sigc::mem_fun(*this,
      &SimulRunViewImpl::onDeltaChanged));

  Gtk::Label* FilesBuffLabel = Gtk::manage(new Gtk::Label(_("Output files buffer")));
  Gtk::Label* FilesBuffUnit = Gtk::manage(new Gtk::Label(_("KB")));
  mp_FilesBuffSpin = Gtk::manage(new Gtk::SpinButton());
  mp_FilesBuffSpin->set_range(1.0, 999.0);
  mp_FilesBuffSpin->set_increments(1, 10);
  mp_FilesBuffSpin->set_numeric(true);
  mp_FilesBuffSpin->signal_value_changed().connect(sigc::mem_fun(*this,
      &SimulRunViewImpl::onFilesBuffChanged));

  Gtk::Label* BeginLabel = Gtk::manage(new Gtk::Label(_("Period begin")));
  mp_BeginEntry = Gtk::manage(new Gtk::Entry());
  mp_BeginEntry->signal_changed().connect(sigc::mem_fun(*this,
      &SimulRunViewImpl::onBeginChanged));

  Gtk::Label* EndLabel = Gtk::manage(new Gtk::Label(_("Period end")));
  mp_EndEntry = Gtk::manage(new Gtk::Entry());
  mp_EndEntry->signal_changed().connect(sigc::mem_fun(*this,
      &SimulRunViewImpl::onEndChanged));

  //  Gtk::Label* ValuesBuffCBLabel = Gtk::manage(
  //      new Gtk::Label(_("Values Buffer")));
  mp_ValuesBuffCB = Gtk::manage(new Gtk::CheckButton(_("Values buffer")));
  mp_ValuesBuffCB->signal_clicked().connect(sigc::mem_fun(*this,
      &SimulRunViewImpl::onValuesBuffToggle));

  Gtk::Label* ValuesBuffValueLabel = Gtk::manage(new Gtk::Label(_("steps")));
  mp_ValuesBuffSpin = Gtk::manage(new Gtk::SpinButton());
  mp_ValuesBuffSpin->set_range(1.0, 999.0);
  mp_ValuesBuffSpin->set_increments(1, 10);
  mp_ValuesBuffSpin->set_numeric(true);
  mp_ValuesBuffSpin->signal_value_changed().connect(sigc::mem_fun(*this,
      &SimulRunViewImpl::onValuesBuffChanged));

  ////////////////
  // New layout //
  ////////////////

  DeltaLabel->set_alignment(0,0.5);
  BeginLabel->set_alignment(0,0.5);
  EndLabel->set_alignment(0,0.5);
  FilesBuffLabel->set_alignment(0,0.5);
  mp_ValuesBuffCB->set_alignment(0,0.5);

  Gtk::HBox* DeltaBox = Gtk::manage(new Gtk::HBox());
  DeltaBox->pack_start(*mp_DeltaSpin, Gtk::PACK_SHRINK, 5);
  DeltaBox->pack_start(*DeltaUnit, Gtk::PACK_SHRINK, 0);

  Gtk::Button* DateBeginButton = Gtk::manage(new Gtk::Button("..."));
  DateBeginButton->set_sensitive(false);

  Gtk::Button* DateEndButton = Gtk::manage(new Gtk::Button("..."));
  DateEndButton->set_sensitive(false);

  Gtk::HBox* DateBeginBox = Gtk::manage(new Gtk::HBox());
  DateBeginBox->pack_start(*mp_BeginEntry, Gtk::PACK_SHRINK, 5);
  DateBeginBox->pack_start(*DateBeginButton, Gtk::PACK_SHRINK, 0);

  Gtk::HBox* DateEndBox = Gtk::manage(new Gtk::HBox());
  DateEndBox->pack_start(*mp_EndEntry, Gtk::PACK_SHRINK, 5);
  DateEndBox->pack_start(*DateEndButton, Gtk::PACK_SHRINK, 0);

  Gtk::SpinButton* BeginHSpin = Gtk::manage(new Gtk::SpinButton());
  BeginHSpin->set_range(0.0, 24.0);
  BeginHSpin->set_numeric(true);
  //  BeginHSpin->signal_value_changed().connect(sigc::mem_fun(*this,
  //        &SimulRunViewImpl::onBeginHValueChanged));

  Gtk::SpinButton* BeginMSpin = Gtk::manage(new Gtk::SpinButton());
  BeginMSpin->set_range(0.0, 60.0);
  BeginMSpin->set_numeric(true);
  //  BeginMSpin->signal_value_changed().connect(sigc::mem_fun(*this,
  //        &SimulRunViewImpl::onBeginMValueChanged));

  Gtk::SpinButton* BeginSSpin = Gtk::manage(new Gtk::SpinButton());
  BeginSSpin->set_range(0.0, 60.0);
  BeginSSpin->set_numeric(true);
  //  BeginSSpin->signal_value_changed().connect(sigc::mem_fun(*this,
  //        &SimulRunViewImpl::onBeginSValueChanged));

  Gtk::SpinButton* EndHSpin = Gtk::manage(new Gtk::SpinButton());
  EndHSpin->set_range(0.0, 24.0);
  EndHSpin->set_numeric(true);
  //  EndHSpin->signal_value_changed().connect(sigc::mem_fun(*this,
  //        &SimulRunViewImpl::onEndHValueChanged));

  Gtk::SpinButton* EndMSpin = Gtk::manage(new Gtk::SpinButton());
  EndMSpin->set_range(0.0, 60.0);
  EndMSpin->set_numeric(true);
  //  EndMSpin->signal_value_changed().connect(sigc::mem_fun(*this,
  //        &SimulRunViewImpl::onEndMValueChanged));

  Gtk::SpinButton* EndSSpin = Gtk::manage(new Gtk::SpinButton());
  EndSSpin->set_range(0.0, 60.0);
  EndSSpin->set_numeric(true);
  //  EndSSpin->signal_value_changed().connect(sigc::mem_fun(*this,
  //        &SimulRunViewImpl::onEndSValueChanged));

  Gtk::HBox* TimeBeginBox = Gtk::manage(new Gtk::HBox());
  TimeBeginBox->pack_start(*BeginHSpin, Gtk::PACK_SHRINK, 5);
  TimeBeginBox->pack_start(*Gtk::manage(new Gtk::Label(_("h"))),
      Gtk::PACK_SHRINK, 0);
  TimeBeginBox->pack_start(*BeginMSpin, Gtk::PACK_SHRINK, 5);
  TimeBeginBox->pack_start(*Gtk::manage(new Gtk::Label(_("m"))),
      Gtk::PACK_SHRINK, 0);
  TimeBeginBox->pack_start(*BeginSSpin, Gtk::PACK_SHRINK, 5);
  TimeBeginBox->pack_start(*Gtk::manage(new Gtk::Label(_("s"))),
      Gtk::PACK_SHRINK, 0);
  TimeBeginBox->set_sensitive(false);

  Gtk::HBox* TimeEndBox = Gtk::manage(new Gtk::HBox());
  TimeEndBox->pack_start(*EndHSpin, Gtk::PACK_SHRINK, 5);
  TimeEndBox->pack_start(*Gtk::manage(new Gtk::Label(_("h"))),
      Gtk::PACK_SHRINK, 0);
  TimeEndBox->pack_start(*EndMSpin, Gtk::PACK_SHRINK, 5);
  TimeEndBox->pack_start(*Gtk::manage(new Gtk::Label(_("m"))),
      Gtk::PACK_SHRINK, 0);
  TimeEndBox->pack_start(*EndSSpin, Gtk::PACK_SHRINK, 5);
  TimeEndBox->pack_start(*Gtk::manage(new Gtk::Label(_("s"))),
      Gtk::PACK_SHRINK, 0);
  TimeEndBox->set_sensitive(false);

  Gtk::Table* TopTable = Gtk::manage(new Gtk::Table());
  TopTable->set_border_width(15);
  TopTable->set_spacings(15);
  TopTable->attach(*DeltaLabel, 0, 1, 0, 1, Gtk::FILL, Gtk::SHRINK, 0,
      0);
  TopTable->attach(*DeltaBox, 1, 2, 0, 1, Gtk::FILL|Gtk::EXPAND, Gtk::SHRINK, 0,
      0);
  TopTable->attach(*Gtk::manage(new Gtk::HSeparator()), 0, 2, 1, 2, Gtk::FILL|Gtk::EXPAND, Gtk::SHRINK, 0,
      0);
  TopTable->attach(*BeginLabel, 0, 1, 2, 3, Gtk::FILL, Gtk::SHRINK, 0,
      0);
  TopTable->attach(*Gtk::manage(new Gtk::Label(_("Date"))), 0, 1, 3, 4, Gtk::FILL, Gtk::SHRINK, 0,
      0);
  TopTable->attach(*DateBeginBox, 1, 2, 3, 4, Gtk::FILL|Gtk::EXPAND, Gtk::SHRINK, 0,
      0);
  TopTable->attach(*Gtk::manage(new Gtk::Label(_("Time"))), 0, 1, 4, 5, Gtk::FILL, Gtk::SHRINK, 0,
      0);
  TopTable->attach(*TimeBeginBox, 1, 2, 4, 5, Gtk::FILL|Gtk::EXPAND, Gtk::SHRINK, 0,
      0);
  TopTable->attach(*Gtk::manage(new Gtk::HSeparator()), 0, 2, 5, 6, Gtk::FILL|Gtk::EXPAND, Gtk::SHRINK, 0,
      0);
  TopTable->attach(*EndLabel, 0, 1, 6, 7, Gtk::FILL, Gtk::SHRINK, 0,
      0);
  TopTable->attach(*Gtk::manage(new Gtk::Label(_("Date"))), 0, 1, 7, 8, Gtk::FILL, Gtk::SHRINK, 0,
      0);
  TopTable->attach(*DateEndBox, 1, 2, 7, 8, Gtk::FILL|Gtk::EXPAND, Gtk::SHRINK, 0,
      0);
  TopTable->attach(*Gtk::manage(new Gtk::Label(_("Time"))), 0, 1, 8, 9, Gtk::FILL, Gtk::SHRINK, 0,
      0);
  TopTable->attach(*TimeEndBox, 1, 2, 8, 9, Gtk::FILL|Gtk::EXPAND, Gtk::SHRINK, 0,
      0);

  Gtk::HBox* FilesBufferBox = Gtk::manage(new Gtk::HBox());
  FilesBufferBox->pack_start(*mp_FilesBuffSpin, Gtk::PACK_SHRINK, 5);
  FilesBufferBox->pack_start(*FilesBuffUnit, Gtk::PACK_SHRINK, 0);

  Gtk::HBox* ValuesBufferBox = Gtk::manage(new Gtk::HBox());
  ValuesBufferBox->pack_start(*mp_ValuesBuffSpin, Gtk::PACK_SHRINK, 5);
  ValuesBufferBox->pack_start(*ValuesBuffValueLabel, Gtk::PACK_SHRINK, 0);

  Gtk::Table* BottomTable = Gtk::manage(new Gtk::Table());
  BottomTable->set_border_width(15);
  BottomTable->set_spacings(15);
  BottomTable->attach(*FilesBuffLabel, 0, 1, 0, 1, Gtk::FILL, Gtk::SHRINK, 0,
      0);
  BottomTable->attach(*FilesBufferBox, 1, 2, 0, 1, Gtk::FILL|Gtk::EXPAND, Gtk::SHRINK, 0,
      0);
  BottomTable->attach(*Gtk::manage(new Gtk::HSeparator()), 0, 2, 1, 2, Gtk::FILL|Gtk::EXPAND, Gtk::SHRINK, 0,
      0);
  BottomTable->attach(*mp_ValuesBuffCB, 0, 1, 2, 3, Gtk::FILL, Gtk::SHRINK,
      0, 0);
  BottomTable->attach(*ValuesBufferBox, 1, 2, 2, 3, Gtk::FILL|Gtk::EXPAND, Gtk::SHRINK,
      0, 0);

  BuilderFrame* TopFrame = Gtk::manage(new BuilderFrame());
  TopFrame->setLabelText(_("Time"));
  TopFrame->add(*TopTable);

  BuilderFrame* BottomFrame = Gtk::manage(new BuilderFrame());
  BottomFrame->setLabelText(_("Memory management"));
  BottomFrame->add(*BottomTable);

  mp_MainBox = Gtk::manage(new Gtk::VBox());
  mp_MainBox->pack_start(*TopFrame,Gtk::PACK_SHRINK,15);
  mp_MainBox->pack_start(*BottomFrame,Gtk::PACK_SHRINK);
  mp_MainBox->set_visible(true);
  mp_MainBox->show_all_children();
//  mp_MainPaned = Gtk::manage(new Gtk::VPaned());
//  mp_MainPaned->pack1(*TopFrame, false, false);
//  mp_MainPaned->pack2(*BottomFrame, false, false);
//  mp_MainPaned->set_visible(true);
//  mp_MainPaned->show_all_children();

  ////////////////
  // Old layout //
  ////////////////

  //  mp_Table = Gtk::manage(new Gtk::Table(2, 9, false));
  //  mp_Table->set_col_spacings(10);
  //  //Top Row
  //  mp_Table->attach(*DeltaLabel, 0, 1, 0, 1, Gtk::SHRINK, Gtk::SHRINK);
  //  mp_Table->attach(*mp_DeltaSpin, 1, 2, 0, 1, Gtk::SHRINK, Gtk::SHRINK);
  //  mp_Table->attach(*DeltaUnit, 2, 3, 0, 1, Gtk::SHRINK, Gtk::SHRINK);
  //  mp_Table->attach(*Gtk::manage(new Gtk::VSeparator()), 3, 4, 0, 1);
  //  mp_Table->attach(*BeginLabel, 4, 5, 0, 1, Gtk::SHRINK, Gtk::SHRINK);
  //  mp_Table->attach(*mp_BeginEntry, 5, 6, 0, 1, Gtk::SHRINK, Gtk::SHRINK);
  //  mp_Table->attach(*Gtk::manage(new Gtk::VSeparator()), 6, 7, 0, 1);
  //  mp_Table->attach(*ValuesBuffCBLabel, 7, 8, 0, 1, Gtk::SHRINK, Gtk::SHRINK);
  //  mp_Table->attach(*mp_ValuesBuffCB, 8, 9, 0, 1, Gtk::SHRINK, Gtk::SHRINK);
  //  //Bottom Row
  //  mp_Table->attach(*FilesBuffLabel, 0, 1, 1, 2, Gtk::SHRINK, Gtk::SHRINK);
  //  mp_Table->attach(*mp_FilesBuffSpin, 1, 2, 1, 2, Gtk::SHRINK, Gtk::SHRINK);
  //  mp_Table->attach(*FilesBuffUnit, 2, 3, 1, 2, Gtk::SHRINK, Gtk::SHRINK);
  //  mp_Table->attach(*Gtk::manage(new Gtk::VSeparator()), 3, 4, 1, 2);
  //  mp_Table->attach(*EndLabel, 4, 5, 1, 2, Gtk::SHRINK, Gtk::SHRINK);
  //  mp_Table->attach(*mp_EndEntry, 5, 6, 1, 2, Gtk::SHRINK, Gtk::SHRINK);
  //  mp_Table->attach(*Gtk::manage(new Gtk::VSeparator()), 6, 7, 1, 2);
  //  mp_Table->attach(*ValuesBuffValueLabel, 7, 8, 1, 2, Gtk::SHRINK, Gtk::SHRINK);
  //  mp_Table->attach(*mp_ValuesBuffSpin, 8, 9, 1, 2, Gtk::SHRINK, Gtk::SHRINK);
  //
  //  mp_Table->set_visible(true);
  //  mp_Table->show_all_children();
}
sigc::signal<void> SimulRunViewImpl::signal_DeltaChanged()
{
  return m_signal_DeltaChanged;
}
sigc::signal<void> SimulRunViewImpl::signal_BeginChanged()
{
  return m_signal_BeginChanged;
}
sigc::signal<void> SimulRunViewImpl::signal_EndChanged()
{
  return m_signal_EndChanged;
}
sigc::signal<void> SimulRunViewImpl::signal_ValuesBuffToggle()
{
  return m_signal_ValuesBuffToggle;
}
sigc::signal<void> SimulRunViewImpl::signal_ValuesBuffChanged()
{
  return m_signal_ValuesBuffChanged;
}
sigc::signal<void> SimulRunViewImpl::signal_FilesBuffChanged()
{
  return m_signal_FilesBuffChanged;
}
void SimulRunViewImpl::setDelta(int Value)
{
  mp_DeltaSpin->set_value(Value);
}
void SimulRunViewImpl::setBegin(std::string Value)
{
  mp_BeginEntry->set_text(Value);
}
void SimulRunViewImpl::setEnd(std::string Value)
{
  mp_EndEntry->set_text(Value);
}
void SimulRunViewImpl::setValuesBuffIsSet(bool IsSet)
{
  mp_ValuesBuffCB->set_active(IsSet);
  mp_ValuesBuffSpin->set_sensitive(IsSet);
}
void SimulRunViewImpl::setValuesBuff(int Value)
{
  mp_ValuesBuffSpin->set_value(Value);
}
void SimulRunViewImpl::setFilesBuff(int Value)
{
  mp_FilesBuffSpin->set_value(Value);
}
void SimulRunViewImpl::setBeginBG(bool IsValid)
{
  mp_BeginEntry->modify_base(mp_BeginEntry->get_state(), Gdk::Color(
      getColorForBGValidState(IsValid)));
}
void SimulRunViewImpl::setEndBG(bool IsValid)
{
  mp_EndEntry->modify_base(mp_EndEntry->get_state(), Gdk::Color(
      getColorForBGValidState(IsValid)));
}
int SimulRunViewImpl::getDelta()
{
  return mp_DeltaSpin->get_value_as_int();
}
std::string SimulRunViewImpl::getBegin()
{
  return mp_BeginEntry->get_text();
}
std::string SimulRunViewImpl::getEnd()
{
  return mp_EndEntry->get_text();
}
bool SimulRunViewImpl::isValuesBuffSet()
{
  return mp_ValuesBuffCB->get_active();
}
int SimulRunViewImpl::getValuesBuff()
{
  return mp_ValuesBuffSpin->get_value_as_int();
}
int SimulRunViewImpl::getFilesBuff()
{
  return mp_FilesBuffSpin->get_value_as_int();
}
Gtk::Widget* SimulRunViewImpl::asWidget()
{
  //  return mp_Table;
//  return mp_MainPaned;
  return mp_MainBox;
}

bool SimulRunViewSub::isValuesBuffSpinSensitive()
{
  return mp_ValuesBuffSpin->get_sensitive();
}
std::string SimulRunViewSub::getBeginBGColor()
{
  return mp_BeginEntry->get_modifier_style()->get_base(
      mp_BeginEntry->get_state()).to_string();
}
std::string SimulRunViewSub::getEndBGColor()
{
  return mp_EndEntry->get_modifier_style()->get_base(mp_EndEntry->get_state()).to_string();
}
std::string SimulRunViewSub::getColorForBGValidState(bool IsValid)
{
  return SimulRunViewImpl::getColorForBGValidState(IsValid);
}
