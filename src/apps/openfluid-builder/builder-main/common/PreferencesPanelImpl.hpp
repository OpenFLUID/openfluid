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
 \file PreferencesPanelImpl.hpp
 \brief Header of ...

 \author Aline LIBRES <libres@supagro.inra.fr>
 */

#ifndef __PREFERENCESPANELIMPL_HPP__
#define __PREFERENCESPANELIMPL_HPP__

#include <gtkmm/label.h>
#include <gtkmm/combobox.h>
#include <gtkmm/liststore.h>
#include <gtkmm/spinbutton.h>
#include <gtkmm/calendar.h>
#include <gtkmm/dialog.h>

#include <openfluid/guicommon/PreferencesPanel.hpp>


class PreferencesPathListWidget;
class PreferencesPlacesListWidget;

// =====================================================================
// =====================================================================

class PreferencesInterfacePanel : public openfluid::guicommon::PreferencesPanel
{
  private:

    class LanguagesColumns: public Gtk::TreeModel::ColumnRecord
    {
      public:
        LanguagesColumns()
        {
          add(m_Code);
          add(m_Name);
        }
        Gtk::TreeModelColumn<Glib::ustring> m_Code;
        Gtk::TreeModelColumn<Glib::ustring> m_Name;
    };

    LanguagesColumns m_LanguagesColumns;

    Glib::RefPtr<Gtk::ListStore> mref_LanguagesModel;

    Gtk::ComboBox* mp_LanguageCombo;

    sigc::connection m_LanguageConnection;

    Gtk::SpinButton* mp_RecentsMaxSpinButton;

    void onLanguageChanged();

    void onRecentsMaxChanged();

    void onClearRecentsClicked();

  public:

    PreferencesInterfacePanel();

    void init();

};

// =====================================================================
// =====================================================================


class PreferencesPathsPanel : public openfluid::guicommon::PreferencesPanel
{
  private:

    Gtk::Entry* mp_WorkdirEntry;

    Gtk::Button* mp_WorkdirFileButton;

    PreferencesPathListWidget* mp_FunctionsPathListWidget;

    PreferencesPathListWidget* mp_ExtensionsPathListWidget;

    PreferencesPathListWidget* mp_ObserversPathListWidget;

    void onWorkdirEntryChanged();

    void onWorkdirFileButtonClicked();

    void onFunctionsPathListChanged();

    void onExtensionsPathListChanged();

    void onObserversPathListChanged();

  public:

    PreferencesPathsPanel();

    void init();

};

// =====================================================================
// =====================================================================

class PreferencesSimPanel : public openfluid::guicommon::PreferencesPanel
{
  private:

    Gtk::SpinButton* mp_DeltaSpin;

    Gtk::Entry* mp_BeginEntry;

    Gtk::SpinButton* mp_BeginHSpin;
    Gtk::SpinButton* mp_BeginMSpin;
    Gtk::SpinButton* mp_BeginSSpin;

    Gtk::Entry* mp_EndEntry;

    Gtk::SpinButton* mp_EndHSpin;
    Gtk::SpinButton* mp_EndMSpin;
    Gtk::SpinButton* mp_EndSSpin;

    Gtk::Dialog* mp_CalendarDialog;
    Gtk::Calendar* mp_Calendar;

    Gtk::SpinButton* mp_FilesBuffSpin;

    bool m_IsMonthChanged;

    void onDeltaChanged();

    void onCalendarDaySelected(bool IsMonthChange);
    bool onCalendarButtonPressEvent(GdkEventButton* Event);

    void onDateBeginButtonClicked();
    void onBeginEntryChanged();
    void onBeginValueChanged();

    void onDateEndButtonClicked();
    void onEndEntryChanged();
    void onEndValueChanged();

    void onFilesBuffChanged();

  public:

    PreferencesSimPanel();

    ~PreferencesSimPanel();

    void init();

};



// =====================================================================
// =====================================================================

class PreferencesMarketPanel : public openfluid::guicommon::PreferencesPanel
{
  private:

    PreferencesPlacesListWidget* mp_PlacesListWidget;

  public:

    PreferencesMarketPanel();

    void init();

};


// =====================================================================
// =====================================================================

#endif /* __PREFERENCESPANELIMPL_HPP__ */
