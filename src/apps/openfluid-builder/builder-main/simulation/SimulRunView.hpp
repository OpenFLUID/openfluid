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
 \file SimulRunView.hpp
 \brief Header of ...

 \author Aline LIBRES <libres@supagro.inra.fr>
 */

#ifndef __SIMULRUNVIEW_HPP__
#define __SIMULRUNVIEW_HPP__

#include <sigc++/sigc++.h>

#include <gtkmm/box.h>
#include <gtkmm/label.h>
#include <gtkmm/spinbutton.h>
#include <gtkmm/calendar.h>
#include <gtkmm/table.h>
#include <gtkmm/dialog.h>
#include <gtkmm/comboboxtext.h>
#include <gtkmm/liststore.h>

#include <openfluid/base/SimulationStatus.hpp>

class SimulRunView
{
  public:

    virtual sigc::signal<void> signal_DeltaTChanged() = 0;

    virtual sigc::signal<void> signal_ConstraintChanged() = 0;

    virtual sigc::signal<void> signal_BeginChanged() = 0;

    virtual sigc::signal<void> signal_EndChanged() = 0;

    virtual sigc::signal<void> signal_ValuesBuffToggle() = 0;

    virtual sigc::signal<void> signal_ValuesBuffChanged() = 0;

    virtual sigc::signal<void> signal_FilesBuffChanged() = 0;

    virtual void setDeltaT(int Value) = 0;

    virtual void setConstraint(const openfluid::base::SimulationStatus::SchedulingConstraint& SConst) = 0;

    virtual void setBegin(std::string Value) = 0;

    virtual void setEnd(std::string Value) = 0;

    virtual void setValuesBuffIsSet(bool IsSet) = 0;

    virtual void setValuesBuff(int Value) = 0;

    virtual void setFilesBuff(int Value) = 0;

    virtual void setBeginBG(std::string ColorString) = 0;

    virtual void setEndBG(std::string ColorString) = 0;

    virtual int getDeltaT() = 0;

    virtual openfluid::base::SimulationStatus::SchedulingConstraint getConstraint() = 0;

    virtual std::string getBegin() = 0;

    virtual std::string getEnd() = 0;

    virtual bool isValuesBuffSet() = 0;

    virtual int getValuesBuff() = 0;

    virtual int getFilesBuff() = 0;

    virtual Gtk::Widget* asWidget() = 0;
};


// =====================================================================
// =====================================================================


class SimulRunViewImpl: public SimulRunView
{
  private:

    sigc::signal<void> m_signal_DeltaTChanged;

    sigc::signal<void> m_signal_ConstraintChanged;

    sigc::signal<void> m_signal_BeginChanged;

    sigc::signal<void> m_signal_EndChanged;

    sigc::signal<void> m_signal_ValuesBuffToggle;
    sigc::signal<void> m_signal_ValuesBuffChanged;

    sigc::signal<void> m_signal_FilesBuffChanged;

    Gtk::Table* mp_Table;

    Gtk::Dialog* mp_CalendarDialog;
    Gtk::Calendar* mp_Calendar;
    bool m_IsMonthChanged;

    void onDeltaTChanged();
    void onConstraintChanged();

    void onBeginEntryChanged();
    void onDateBeginButtonClicked();
    void onBeginValueChanged();

    void onEndEntryChanged();
    void onDateEndButtonClicked();
    void onEndValueChanged();

    void onCalendarDaySelected(bool IsMonthChange);
    bool onCalendarButtonPressEvent(GdkEventButton* Event);

    void onValuesBuffToggle();
    void onValuesBuffChanged();

    void onFilesBuffChanged();

  protected:

    class ConstraintComboModelColumns : public Gtk::TreeModel::ColumnRecord
    {
      public:

      ConstraintComboModelColumns()
      { add(m_SchedConstraint); add(m_Label); }

      Gtk::TreeModelColumn<openfluid::base::SimulationStatus::SchedulingConstraint> m_SchedConstraint;
      Gtk::TreeModelColumn<std::string> m_Label;
    };

    ConstraintComboModelColumns m_ConstraintColumns;
    Glib::RefPtr<Gtk::ListStore> mref_ConstraintTreeModel;

    Gtk::Box* mp_MainBox;

    Gtk::SpinButton* mp_DeltaTSpin;
    Gtk::ComboBox* mp_ConstraintCombo;

    Gtk::Entry* mp_BeginEntry;
    Gtk::SpinButton* mp_BeginHSpin;
    Gtk::SpinButton* mp_BeginMSpin;
    Gtk::SpinButton* mp_BeginSSpin;

    Gtk::Entry* mp_EndEntry;
    Gtk::SpinButton* mp_EndHSpin;
    Gtk::SpinButton* mp_EndMSpin;
    Gtk::SpinButton* mp_EndSSpin;

    Gtk::CheckButton* mp_ValuesBuffCB;
    Gtk::SpinButton* mp_ValuesBuffSpin;

    Gtk::SpinButton* mp_FilesBuffSpin;

  public:

    SimulRunViewImpl();

    sigc::signal<void> signal_DeltaTChanged();

    sigc::signal<void> signal_ConstraintChanged();

    sigc::signal<void> signal_BeginChanged();

    sigc::signal<void> signal_EndChanged();

    sigc::signal<void> signal_ValuesBuffToggle();

    sigc::signal<void> signal_ValuesBuffChanged();

    sigc::signal<void> signal_FilesBuffChanged();

    void setDeltaT(int Value);

    void setConstraint(const openfluid::base::SimulationStatus::SchedulingConstraint& SConst);

    void setBegin(std::string Value);

    void setEnd(std::string Value);

    void setValuesBuffIsSet(bool IsSet);

    void setValuesBuff(int Value);

    void setFilesBuff(int Value);

    void setBeginBG(std::string ColorString);

    void setEndBG(std::string ColorString);

    int getDeltaT();

    openfluid::base::SimulationStatus::SchedulingConstraint getConstraint();

    std::string getBegin();

    std::string getEnd();

    bool isValuesBuffSet();

    int getValuesBuff();

    int getFilesBuff();

    Gtk::Widget* asWidget();

};


// =====================================================================
// =====================================================================


class SimulRunViewSub: public SimulRunViewImpl
{
  public:
    bool isValuesBuffSpinSensitive();
    std::string getBeginBGColor();
    std::string getEndBGColor();
};

#endif /* __SIMULRUNVIEW_HPP__ */
