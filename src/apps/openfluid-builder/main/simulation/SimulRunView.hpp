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

#include <gtkmm.h>

class SimulRunView
{
  public:
    virtual sigc::signal<void> signal_DeltaChanged() = 0;
    virtual sigc::signal<void> signal_BeginChanged() = 0;
    virtual sigc::signal<void> signal_EndChanged() = 0;
    virtual sigc::signal<void> signal_ValuesBuffToggle() = 0;
    virtual sigc::signal<void> signal_ValuesBuffChanged() = 0;
    virtual sigc::signal<void> signal_FilesBuffChanged() = 0;
    virtual void setDelta(int Value) = 0;
    virtual void setBegin(std::string Value) = 0;
    virtual void setEnd(std::string Value) = 0;
    virtual void setValuesBuffIsSet(bool IsSet) = 0;
    virtual void setValuesBuff(int Value) = 0;
    virtual void setFilesBuff(int Value) = 0;
    virtual void setBeginBG(bool IsValid) = 0;
    virtual void setEndBG(bool IsValid) = 0;
    virtual int getDelta() = 0;
    virtual std::string getBegin() = 0;
    virtual std::string getEnd() = 0;
    virtual bool isValuesBuffSet() = 0;
    virtual int getValuesBuff() = 0;
    virtual int getFilesBuff() = 0;
    virtual Gtk::Widget* asWidget() = 0;
};

class SimulRunViewImpl: public SimulRunView
{
  private:
    sigc::signal<void> m_signal_DeltaChanged;
    sigc::signal<void> m_signal_BeginChanged;
    sigc::signal<void> m_signal_EndChanged;
    sigc::signal<void> m_signal_ValuesBuffToggle;
    sigc::signal<void> m_signal_ValuesBuffChanged;
    sigc::signal<void> m_signal_FilesBuffChanged;
    Gtk::Table* mp_Table;
    void onDeltaChanged();
    void onBeginChanged();
    void onEndChanged();
    void onValuesBuffToggle();
    void onValuesBuffChanged();
    void onFilesBuffChanged();
  protected:
    Gtk::SpinButton* mp_DeltaSpin;
    Gtk::Entry* mp_BeginEntry;
    Gtk::Entry* mp_EndEntry;
    Gtk::CheckButton* mp_ValuesBuffCB;
    Gtk::SpinButton* mp_ValuesBuffSpin;
    Gtk::SpinButton* mp_FilesBuffSpin;
    std::string getColorForBGValidState(bool IsValid);
  public:
    SimulRunViewImpl();
    sigc::signal<void> signal_DeltaChanged();
    sigc::signal<void> signal_BeginChanged();
    sigc::signal<void> signal_EndChanged();
    sigc::signal<void> signal_ValuesBuffToggle();
    sigc::signal<void> signal_ValuesBuffChanged();
    sigc::signal<void> signal_FilesBuffChanged();
    void setDelta(int Value);
    void setBegin(std::string Value);
    void setEnd(std::string Value);
    void setValuesBuffIsSet(bool IsSet);
    void setValuesBuff(int Value);
    void setFilesBuff(int Value);
    void setBeginBG(bool IsValid);
    void setEndBG(bool IsValid);
    int getDelta();
    std::string getBegin();
    std::string getEnd();
    bool isValuesBuffSet();
    int getValuesBuff();
    int getFilesBuff();
    Gtk::Widget* asWidget();
};

class SimulRunViewSub: public SimulRunViewImpl
{
  public:
    bool isValuesBuffSpinSensitive();
    std::string getBeginBGColor();
    std::string getEndBGColor();
    std::string getColorForBGValidState(bool IsValid);
};

#endif /* __SIMULRUNVIEW_HPP__ */
