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

#include <gtkmm/separator.h>

#include <openfluid/core/DateTime.hpp>

#include "BuilderFrame.hpp"


// =====================================================================
// =====================================================================


SimulRunViewImpl::SimulRunViewImpl()
{
  /*
   * Period frame
   */

  mp_BeginEntry = Gtk::manage(new Gtk::Entry());
  mp_BeginEntry->signal_changed().connect(sigc::mem_fun(*this,
      &SimulRunViewImpl::onBeginEntryChanged));

  Gtk::Button* DateBeginButton = Gtk::manage(new Gtk::Button("..."));
  DateBeginButton->signal_clicked().connect(sigc::mem_fun(*this,
      &SimulRunViewImpl::onDateBeginButtonClicked));

  mp_BeginHSpin = Gtk::manage(new Gtk::SpinButton());
  mp_BeginHSpin->set_range(0.0, 23.0);
  mp_BeginHSpin->set_increments(1, 1);
  mp_BeginHSpin->set_numeric(true);
  mp_BeginHSpin->signal_changed().connect(sigc::mem_fun(*this,
      &SimulRunViewImpl::onBeginValueChanged));

  mp_BeginMSpin = Gtk::manage(new Gtk::SpinButton());
  mp_BeginMSpin->set_range(0.0, 59.0);
  mp_BeginMSpin->set_increments(1, 10);
  mp_BeginMSpin->set_numeric(true);
  mp_BeginMSpin->signal_changed().connect(sigc::mem_fun(*this,
      &SimulRunViewImpl::onBeginValueChanged));

  mp_BeginSSpin = Gtk::manage(new Gtk::SpinButton());
  mp_BeginSSpin->set_range(0.0, 59.0);
  mp_BeginSSpin->set_increments(1, 10);
  mp_BeginSSpin->set_numeric(true);
  mp_BeginSSpin->signal_changed().connect(sigc::mem_fun(*this,
      &SimulRunViewImpl::onBeginValueChanged));

  mp_EndEntry = Gtk::manage(new Gtk::Entry());
  mp_EndEntry->signal_changed().connect(sigc::mem_fun(*this,
      &SimulRunViewImpl::onEndEntryChanged));

  Gtk::Button* DateEndButton = Gtk::manage(new Gtk::Button("..."));
  DateEndButton->signal_clicked().connect(sigc::mem_fun(*this,
      &SimulRunViewImpl::onDateEndButtonClicked));

  mp_EndHSpin = Gtk::manage(new Gtk::SpinButton());
  mp_EndHSpin->set_range(0.0, 23.0);
  mp_EndHSpin->set_increments(1, 1);
  mp_EndHSpin->set_numeric(true);
  mp_EndHSpin->signal_changed().connect(sigc::mem_fun(*this,
      &SimulRunViewImpl::onEndValueChanged));

  mp_EndMSpin = Gtk::manage(new Gtk::SpinButton());
  mp_EndMSpin->set_range(0.0, 59.0);
  mp_EndMSpin->set_increments(1, 10);
  mp_EndMSpin->set_numeric(true);
  mp_EndMSpin->signal_changed().connect(sigc::mem_fun(*this,
      &SimulRunViewImpl::onEndValueChanged));

  mp_EndSSpin = Gtk::manage(new Gtk::SpinButton());
  mp_EndSSpin->set_range(0.0, 59.0);
  mp_EndSSpin->set_increments(1, 10);
  mp_EndSSpin->set_numeric(true);
  mp_EndSSpin->signal_changed().connect(sigc::mem_fun(*this,
      &SimulRunViewImpl::onEndValueChanged));


  Gtk::HBox* DateBeginBox = Gtk::manage(new Gtk::HBox());
  DateBeginBox->pack_start(*mp_BeginEntry, Gtk::PACK_SHRINK, 5);
  DateBeginBox->pack_start(*DateBeginButton, Gtk::PACK_SHRINK);

  Gtk::HBox* TimeBeginBox = Gtk::manage(new Gtk::HBox());
  TimeBeginBox->pack_start(*mp_BeginHSpin, Gtk::PACK_SHRINK, 5);
  TimeBeginBox->pack_start(*Gtk::manage(new Gtk::Label(_("h"))),
      Gtk::PACK_SHRINK);
  TimeBeginBox->pack_start(*mp_BeginMSpin, Gtk::PACK_SHRINK, 5);
  TimeBeginBox->pack_start(*Gtk::manage(new Gtk::Label(_("m"))),
      Gtk::PACK_SHRINK);
  TimeBeginBox->pack_start(*mp_BeginSSpin, Gtk::PACK_SHRINK, 5);
  TimeBeginBox->pack_start(*Gtk::manage(new Gtk::Label(_("s"))),
      Gtk::PACK_SHRINK);

  Gtk::HBox* DateEndBox = Gtk::manage(new Gtk::HBox());
  DateEndBox->pack_start(*mp_EndEntry, Gtk::PACK_SHRINK, 5);
  DateEndBox->pack_start(*DateEndButton, Gtk::PACK_SHRINK);

  Gtk::HBox* TimeEndBox = Gtk::manage(new Gtk::HBox());
  TimeEndBox->pack_start(*mp_EndHSpin, Gtk::PACK_SHRINK, 5);
  TimeEndBox->pack_start(*Gtk::manage(new Gtk::Label(_("h"))), Gtk::PACK_SHRINK);
  TimeEndBox->pack_start(*mp_EndMSpin, Gtk::PACK_SHRINK, 5);
  TimeEndBox->pack_start(*Gtk::manage(new Gtk::Label(_("m"))), Gtk::PACK_SHRINK);
  TimeEndBox->pack_start(*mp_EndSSpin, Gtk::PACK_SHRINK, 5);
  TimeEndBox->pack_start(*Gtk::manage(new Gtk::Label(_("s"))), Gtk::PACK_SHRINK);

  Gtk::Table* PeriodTable = Gtk::manage(new Gtk::Table());
  PeriodTable->set_border_width(15);
  PeriodTable->set_spacings(15);

  PeriodTable->attach(*Gtk::manage(new Gtk::Label(_("Period begin"), 0, 0.5)), 0, 1, 0, 1, Gtk::FILL, Gtk::SHRINK);
  PeriodTable->attach(*Gtk::manage(new Gtk::Label(_("Date"), 1, 0.5)), 0, 1, 1, 2, Gtk::FILL, Gtk::SHRINK);
  PeriodTable->attach(*DateBeginBox, 1, 2, 1, 2, Gtk::FILL /*| Gtk::EXPAND*/, Gtk::SHRINK);
  PeriodTable->attach(*Gtk::manage(new Gtk::Label(_("Time"), 1, 0.5)), 0, 1, 2, 3, Gtk::FILL, Gtk::SHRINK);
  PeriodTable->attach(*TimeBeginBox, 1, 2, 2, 3, Gtk::FILL /*| Gtk::EXPAND*/, Gtk::SHRINK);

  PeriodTable->attach(*Gtk::manage(new Gtk::VSeparator()), 3, 4, 0, 3, Gtk::FILL, Gtk::FILL,30);

  PeriodTable->attach(*Gtk::manage(new Gtk::Label(_("Period end"), 0, 0.5)), 4, 5, 0, 1, Gtk::FILL, Gtk::SHRINK);
  PeriodTable->attach(*Gtk::manage(new Gtk::Label(_("Date"), 1, 0.5)), 4, 5, 1, 2, Gtk::FILL, Gtk::SHRINK);
  PeriodTable->attach(*DateEndBox, 5, 6, 1, 2, Gtk::FILL | Gtk::EXPAND, Gtk::SHRINK);
  PeriodTable->attach(*Gtk::manage(new Gtk::Label(_("Time"), 1, 0.5)), 4, 5, 2, 3, Gtk::FILL, Gtk::SHRINK);
  PeriodTable->attach(*TimeEndBox, 5, 6, 2, 3, Gtk::FILL | Gtk::EXPAND, Gtk::SHRINK);


  /*
   * scheduling frame
   */

  mp_DeltaTSpin = Gtk::manage(new Gtk::SpinButton());
  mp_DeltaTSpin->set_range(1.0, 3155760000.0); // set to 100 years max (only for easy checking in GUI, not an OpenFLUID limitation)
  mp_DeltaTSpin->set_increments(1, 3600);
  mp_DeltaTSpin->set_numeric(true);
  mp_DeltaTSpin->signal_changed().connect(sigc::mem_fun(*this,
      &SimulRunViewImpl::onDeltaTChanged));

  mp_ConstraintCombo = Gtk::manage(new Gtk::ComboBox());
  mref_ConstraintTreeModel = Gtk::ListStore::create(m_ConstraintColumns);
  mp_ConstraintCombo->set_model(mref_ConstraintTreeModel);
  mp_ConstraintCombo->signal_changed().connect(sigc::mem_fun(*this,
        &SimulRunViewImpl::onConstraintChanged));

  Gtk::TreeModel::Row DeltaTModeRow;
  DeltaTModeRow = *(mref_ConstraintTreeModel->append());
  DeltaTModeRow[m_ConstraintColumns.m_SchedConstraint] = openfluid::base::SimulationStatus::SCHED_NONE;
  DeltaTModeRow[m_ConstraintColumns.m_Label] = _("None");
  mp_ConstraintCombo->set_active(DeltaTModeRow);

  DeltaTModeRow = *(mref_ConstraintTreeModel->append());
  DeltaTModeRow[m_ConstraintColumns.m_SchedConstraint] = openfluid::base::SimulationStatus::SCHED_DTCHECKED;
  DeltaTModeRow[m_ConstraintColumns.m_Label] = _("Checked - Simulators scheduling requests are checked to be equal to DeltaT");

  DeltaTModeRow = *(mref_ConstraintTreeModel->append());
  DeltaTModeRow[m_ConstraintColumns.m_SchedConstraint] = openfluid::base::SimulationStatus::SCHED_DTFORCED;
  DeltaTModeRow[m_ConstraintColumns.m_Label] = _("Forced - Simulators scheduling requests are forced to DeltaT");

  mp_ConstraintCombo->pack_start(m_ConstraintColumns.m_Label);

  Gtk::HBox* DeltaTBox = Gtk::manage(new Gtk::HBox());
  DeltaTBox->pack_start(*mp_DeltaTSpin, Gtk::PACK_SHRINK, 5);
  DeltaTBox->pack_start(*Gtk::manage(new Gtk::Label(_("seconds"))),
                       Gtk::PACK_SHRINK);


  Gtk::Table* SchedulingTable = Gtk::manage(new Gtk::Table());
  SchedulingTable->set_border_width(15);
  SchedulingTable->set_spacings(15);

  SchedulingTable->attach(*Gtk::manage(new Gtk::Label(_("Delta T"), 1, 0.5)), 0, 1, 0, 1, Gtk::FILL, Gtk::SHRINK);
  SchedulingTable->attach(*DeltaTBox, 1, 2, 0, 1, Gtk::FILL | Gtk::EXPAND, Gtk::SHRINK);

  SchedulingTable->attach(*Gtk::manage(new Gtk::HSeparator()), 0, 2, 1, 2, Gtk::FILL | Gtk::EXPAND, Gtk::SHRINK);

  SchedulingTable->attach(*Gtk::manage(new Gtk::Label(_("Constraint"), 1, 0.5)), 0, 1, 2, 3, Gtk::FILL, Gtk::SHRINK);
  SchedulingTable->attach(*mp_ConstraintCombo, 1, 2, 2, 3, Gtk::FILL | Gtk::EXPAND, Gtk::SHRINK);



  /*
   * Memory frame
   */

  mp_FilesBuffSpin = Gtk::manage(new Gtk::SpinButton());
  mp_FilesBuffSpin->set_range(1.0, 999.0);
  mp_FilesBuffSpin->set_increments(1, 10);
  mp_FilesBuffSpin->set_numeric(true);
  mp_FilesBuffSpin->signal_changed().connect(sigc::mem_fun(*this,
      &SimulRunViewImpl::onFilesBuffChanged));

  mp_ValuesBuffCB = Gtk::manage(new Gtk::CheckButton(_("Values buffer")));
  mp_ValuesBuffCB->set_alignment(0, 0.5);
  mp_ValuesBuffCB->signal_clicked().connect(sigc::mem_fun(*this,
      &SimulRunViewImpl::onValuesBuffToggle));

  mp_ValuesBuffSpin = Gtk::manage(new Gtk::SpinButton());
  mp_ValuesBuffSpin->set_range(1.0, 999.0);
  mp_ValuesBuffSpin->set_increments(1, 10);
  mp_ValuesBuffSpin->set_numeric(true);
  mp_ValuesBuffSpin->signal_changed().connect(sigc::mem_fun(*this,
      &SimulRunViewImpl::onValuesBuffChanged));

  Gtk::HBox* FilesBufferBox = Gtk::manage(new Gtk::HBox());
  FilesBufferBox->pack_start(*mp_FilesBuffSpin, Gtk::PACK_SHRINK, 5);
  FilesBufferBox->pack_start(*Gtk::manage(new Gtk::Label(_("KB"))),
      Gtk::PACK_SHRINK);

  Gtk::HBox* ValuesBufferBox = Gtk::manage(new Gtk::HBox());
  ValuesBufferBox->pack_start(*mp_ValuesBuffSpin, Gtk::PACK_SHRINK, 5);
  ValuesBufferBox->pack_start(*Gtk::manage(new Gtk::Label(_("steps"))),
      Gtk::PACK_SHRINK);

  Gtk::Table* MemoryTable = Gtk::manage(new Gtk::Table());
  MemoryTable->set_border_width(15);
  MemoryTable->set_spacings(15);
  MemoryTable->attach(*Gtk::manage(new Gtk::Label(_("Output files buffer"), 0,
      0.5)), 0, 1, 0, 1, Gtk::FILL, Gtk::SHRINK);
  MemoryTable->attach(*FilesBufferBox, 1, 2, 0, 1, Gtk::FILL | Gtk::EXPAND,
      Gtk::SHRINK);
  MemoryTable->attach(*Gtk::manage(new Gtk::HSeparator()), 0, 2, 1, 2,
      Gtk::FILL | Gtk::EXPAND, Gtk::SHRINK);
  MemoryTable->attach(*mp_ValuesBuffCB, 0, 1, 2, 3, Gtk::FILL, Gtk::SHRINK);
  MemoryTable->attach(*ValuesBufferBox, 1, 2, 2, 3, Gtk::FILL | Gtk::EXPAND,
      Gtk::SHRINK);



  // complete layout

  BuilderFrame* PeriodFrame = Gtk::manage(new BuilderFrame());
  PeriodFrame->setLabelText(_("Simulation period"));
  PeriodFrame->add(*PeriodTable);

  BuilderFrame* SchedulingFrame = Gtk::manage(new BuilderFrame());
  SchedulingFrame->setLabelText(_("Simulation scheduling"));
  SchedulingFrame->add(*SchedulingTable);

  BuilderFrame* MemoryFrame = Gtk::manage(new BuilderFrame());
  MemoryFrame->setLabelText(_("Memory management"));
  MemoryFrame->add(*MemoryTable);

  mp_MainBox = Gtk::manage(new Gtk::VBox());
  mp_MainBox->pack_start(*PeriodFrame, Gtk::PACK_SHRINK, 15);
  mp_MainBox->pack_start(*SchedulingFrame, Gtk::PACK_SHRINK, 15);
  mp_MainBox->pack_start(*MemoryFrame, Gtk::PACK_SHRINK, 15);
  mp_MainBox->set_visible(true);
  mp_MainBox->show_all_children();

  /*
   * see: http://www.kksou.com/php-gtk2/articles/let-user-enter-date-with-a-popup-calendar---Part-2.php
   */
  mp_Calendar = Gtk::manage(new Gtk::Calendar());
  mp_Calendar->signal_day_selected().connect(sigc::bind<bool>(sigc::mem_fun(
      *this, &SimulRunViewImpl::onCalendarDaySelected), false));
  mp_Calendar->signal_month_changed().connect(sigc::bind<bool>(sigc::mem_fun(
      *this, &SimulRunViewImpl::onCalendarDaySelected), true));
  mp_Calendar->signal_button_press_event().connect(sigc::mem_fun(*this,
      &SimulRunViewImpl::onCalendarButtonPressEvent));

  mp_CalendarDialog = new Gtk::Dialog();
  mp_CalendarDialog->get_vbox()->pack_start(*mp_Calendar);
  mp_CalendarDialog->show_all_children();

}

// =====================================================================
// =====================================================================


void SimulRunViewImpl::onDeltaTChanged()
{
  mp_DeltaTSpin->update();
  m_signal_DeltaTChanged.emit();
}


// =====================================================================
// =====================================================================


void SimulRunViewImpl::onConstraintChanged()
{
  m_signal_ConstraintChanged.emit();
}


// =====================================================================
// =====================================================================


void SimulRunViewImpl::onBeginEntryChanged()
{
  Glib::ustring Begin = mp_BeginEntry->get_text();
  openfluid::core::DateTime DT;

  if (DT.setFromISOString(Begin))
  {
    mp_BeginHSpin->set_value(DT.getHour());
    mp_BeginMSpin->set_value(DT.getMinute());
    mp_BeginSSpin->set_value(DT.getSecond());
  }

  m_signal_BeginChanged.emit();
}

// =====================================================================
// =====================================================================


void SimulRunViewImpl::onDateBeginButtonClicked()
{
  openfluid::core::DateTime DT;

  if (DT.setFromISOString(mp_BeginEntry->get_text()))
  {
    mp_Calendar->select_month(DT.getMonth() - 1, DT.getYear());
    mp_Calendar->select_day(DT.getDay());
  }

  if (mp_CalendarDialog->run() == Gtk::RESPONSE_OK)
  {
    unsigned int Y, M, D;
    mp_Calendar->get_date(Y, M, D);

    openfluid::core::DateTime DT(Y, M + 1, D,
        mp_BeginHSpin->get_value_as_int(), mp_BeginMSpin->get_value_as_int(),
        mp_BeginSSpin->get_value_as_int());

    mp_BeginEntry->set_text(DT.getAsISOString());
  }

  mp_CalendarDialog->hide();
}

// =====================================================================
// =====================================================================


void SimulRunViewImpl::onBeginValueChanged()
{
  mp_BeginHSpin->update();
  mp_BeginMSpin->update();
  mp_BeginSSpin->update();

  openfluid::core::DateTime DT;

  if (DT.setFromISOString(mp_BeginEntry->get_text()))
  {
    openfluid::core::DateTime NewDT(DT.getYear(), DT.getMonth(), DT.getDay(),
        mp_BeginHSpin->get_value_as_int(), mp_BeginMSpin->get_value_as_int(),
        mp_BeginSSpin->get_value_as_int());

    mp_BeginEntry->set_text(NewDT.getAsISOString());
  }
}

// =====================================================================
// =====================================================================


void SimulRunViewImpl::onEndEntryChanged()
{
  Glib::ustring End = mp_EndEntry->get_text();
  openfluid::core::DateTime DT;

  if (DT.setFromISOString(End))
  {
    mp_EndHSpin->set_value(DT.getHour());
    mp_EndMSpin->set_value(DT.getMinute());
    mp_EndSSpin->set_value(DT.getSecond());
  }

  m_signal_EndChanged.emit();
}

// =====================================================================
// =====================================================================


void SimulRunViewImpl::onDateEndButtonClicked()
{
  openfluid::core::DateTime DT;

  if (DT.setFromISOString(mp_EndEntry->get_text()))
  {
    mp_Calendar->select_month(DT.getMonth() - 1, DT.getYear());
    mp_Calendar->select_day(DT.getDay());
  }

  if (mp_CalendarDialog->run() == Gtk::RESPONSE_OK)
  {
    unsigned int Y, M, D;
    mp_Calendar->get_date(Y, M, D);

    openfluid::core::DateTime DT(Y, M + 1, D, mp_EndHSpin->get_value_as_int(),
        mp_EndMSpin->get_value_as_int(), mp_EndSSpin->get_value_as_int());

    mp_EndEntry->set_text(DT.getAsISOString());
  }

  mp_CalendarDialog->hide();
}

// =====================================================================
// =====================================================================


void SimulRunViewImpl::onEndValueChanged()
{
  mp_EndHSpin->update();
  mp_EndMSpin->update();
  mp_EndSSpin->update();

  openfluid::core::DateTime DT;

  if (DT.setFromISOString(mp_EndEntry->get_text()))
  {
    openfluid::core::DateTime NewDT(DT.getYear(), DT.getMonth(), DT.getDay(),
        mp_EndHSpin->get_value_as_int(), mp_EndMSpin->get_value_as_int(),
        mp_EndSSpin->get_value_as_int());

    mp_EndEntry->set_text(NewDT.getAsISOString());
  }
}

// =====================================================================
// =====================================================================


void SimulRunViewImpl::onCalendarDaySelected(bool IsMonthChange)
{
  m_IsMonthChanged = IsMonthChange;
}

// =====================================================================
// =====================================================================


bool SimulRunViewImpl::onCalendarButtonPressEvent(GdkEventButton* /*Event*/)
{
  if (!m_IsMonthChanged)
    mp_CalendarDialog->response(Gtk::RESPONSE_OK);

  return true;
}

// =====================================================================
// =====================================================================


void SimulRunViewImpl::onValuesBuffToggle()
{
  m_signal_ValuesBuffToggle.emit();
}

// =====================================================================
// =====================================================================


void SimulRunViewImpl::onValuesBuffChanged()
{
  mp_ValuesBuffSpin->update();
  m_signal_ValuesBuffChanged.emit();
}

// =====================================================================
// =====================================================================


void SimulRunViewImpl::onFilesBuffChanged()
{
  mp_FilesBuffSpin->update();
  m_signal_FilesBuffChanged.emit();
}

// =====================================================================
// =====================================================================


sigc::signal<void> SimulRunViewImpl::signal_DeltaTChanged()
{
  return m_signal_DeltaTChanged;
}


// =====================================================================
// =====================================================================


sigc::signal<void> SimulRunViewImpl::signal_ConstraintChanged()
{
  return m_signal_ConstraintChanged;
}


// =====================================================================
// =====================================================================


sigc::signal<void> SimulRunViewImpl::signal_BeginChanged()
{
  return m_signal_BeginChanged;
}

// =====================================================================
// =====================================================================


sigc::signal<void> SimulRunViewImpl::signal_EndChanged()
{
  return m_signal_EndChanged;
}

// =====================================================================
// =====================================================================


sigc::signal<void> SimulRunViewImpl::signal_ValuesBuffToggle()
{
  return m_signal_ValuesBuffToggle;
}

// =====================================================================
// =====================================================================


sigc::signal<void> SimulRunViewImpl::signal_ValuesBuffChanged()
{
  return m_signal_ValuesBuffChanged;
}

// =====================================================================
// =====================================================================


sigc::signal<void> SimulRunViewImpl::signal_FilesBuffChanged()
{
  return m_signal_FilesBuffChanged;
}

// =====================================================================
// =====================================================================


void SimulRunViewImpl::setDeltaT(int Value)
{
  mp_DeltaTSpin->set_value(Value);
}


// =====================================================================
// =====================================================================


void SimulRunViewImpl::setConstraint(const openfluid::base::SimulationStatus::SchedulingConstraint& SConst)
{
  Gtk::TreeModel::Children Cstraints = mref_ConstraintTreeModel->children();
  for (Gtk::TreeIter it = Cstraints.begin(); it != Cstraints.end(); ++it)
  {
    if (it->get_value(m_ConstraintColumns.m_SchedConstraint) == SConst)
    {
      mp_ConstraintCombo->set_active(it);
      break;
    }
  }
}


// =====================================================================
// =====================================================================


void SimulRunViewImpl::setBegin(std::string Value)
{
  mp_BeginEntry->set_text(Value);
}


// =====================================================================
// =====================================================================


void SimulRunViewImpl::setEnd(std::string Value)
{
  mp_EndEntry->set_text(Value);
}

// =====================================================================
// =====================================================================


void SimulRunViewImpl::setValuesBuffIsSet(bool IsSet)
{
  mp_ValuesBuffCB->set_active(IsSet);
  mp_ValuesBuffSpin->set_sensitive(IsSet);
}

// =====================================================================
// =====================================================================


void SimulRunViewImpl::setValuesBuff(int Value)
{
  mp_ValuesBuffSpin->set_value(Value);
}

// =====================================================================
// =====================================================================


void SimulRunViewImpl::setFilesBuff(int Value)
{
  mp_FilesBuffSpin->set_value(Value);
}

// =====================================================================
// =====================================================================


void SimulRunViewImpl::setBeginBG(std::string ColorString)
{
  mp_BeginEntry->modify_base(mp_BeginEntry->get_state(),
      Gdk::Color(ColorString));
}

// =====================================================================
// =====================================================================


void SimulRunViewImpl::setEndBG(std::string ColorString)
{
  mp_EndEntry->modify_base(mp_EndEntry->get_state(), Gdk::Color(ColorString));
}

// =====================================================================
// =====================================================================


int SimulRunViewImpl::getDeltaT()
{
  return mp_DeltaTSpin->get_value_as_int();
}

// =====================================================================
// =====================================================================


openfluid::base::SimulationStatus::SchedulingConstraint SimulRunViewImpl::getConstraint()
{
  Gtk::TreeModel::iterator It = mp_ConstraintCombo->get_active();

  if(It)
  {
    Gtk::TreeModel::Row TmpRow = *It;
    if(TmpRow)
      return TmpRow[m_ConstraintColumns.m_SchedConstraint];
  }

  return openfluid::base::SimulationStatus::SCHED_NONE; // should never happen
}


// =====================================================================
// =====================================================================


std::string SimulRunViewImpl::getBegin()
{
  return mp_BeginEntry->get_text();
}

// =====================================================================
// =====================================================================


std::string SimulRunViewImpl::getEnd()
{
  return mp_EndEntry->get_text();
}

// =====================================================================
// =====================================================================


bool SimulRunViewImpl::isValuesBuffSet()
{
  return mp_ValuesBuffCB->get_active();
}

// =====================================================================
// =====================================================================


int SimulRunViewImpl::getValuesBuff()
{
  return mp_ValuesBuffSpin->get_value_as_int();
}

// =====================================================================
// =====================================================================


int SimulRunViewImpl::getFilesBuff()
{
  return mp_FilesBuffSpin->get_value_as_int();
}

// =====================================================================
// =====================================================================


Gtk::Widget* SimulRunViewImpl::asWidget()
{
  return mp_MainBox;
}

// =====================================================================
// =====================================================================

// =====================================================================
// =====================================================================


bool SimulRunViewSub::isValuesBuffSpinSensitive()
{
  return mp_ValuesBuffSpin->get_sensitive();
}

// =====================================================================
// =====================================================================


std::string SimulRunViewSub::getBeginBGColor()
{
  return mp_BeginEntry->get_modifier_style()->get_base(
      mp_BeginEntry->get_state()).to_string();
}

// =====================================================================
// =====================================================================


std::string SimulRunViewSub::getEndBGColor()
{
  return mp_EndEntry->get_modifier_style()->get_base(mp_EndEntry->get_state()).to_string();
}

