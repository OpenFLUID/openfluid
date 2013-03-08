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
 \file PreferencesPanelImpl.cpp
 \brief Implements ...

 \author Aline LIBRES <libres@supagro.inra.fr>
 */

#include "PreferencesPanelImpl.hpp"

#include <glibmm/i18n.h>

#include <gtkmm/separator.h>
#include <gtkmm/filechooserdialog.h>
#include <gtkmm/stock.h>
#include <gtkmm/table.h>
#include <gtkmm/expander.h>

#include <openfluid/base/RuntimeEnv.hpp>
#include <openfluid/guicommon/PreferencesManager.hpp>
#include "i18nManager.hpp"
#include "PreferencesPathListWidget.hpp"
#include "PreferencesPlacesListWidget.hpp"
#include "BuilderExtensionsManager.hpp"

#include <iostream>

// =====================================================================
// =====================================================================


PreferencesInterfacePanel::PreferencesInterfacePanel() :
openfluid::guicommon::PreferencesPanel(_("Interface"))
{
  /*
   * Languages
   */

  Gtk::Label* LangLabel = Gtk::manage(new Gtk::Label(
      _("Interface Language (requires restart):")));

  mref_LanguagesModel = Gtk::ListStore::create(m_LanguagesColumns);

  Gtk::TreeRow Row = *mref_LanguagesModel->append();
  Row[m_LanguagesColumns.m_Code] = "";
  Row[m_LanguagesColumns.m_Name] = _("System default");

  mp_LanguageCombo = Gtk::manage(new Gtk::ComboBox(static_cast<Glib::RefPtr<Gtk::TreeModel> >(mref_LanguagesModel)));
  mp_LanguageCombo->pack_start(m_LanguagesColumns.m_Name);
  m_LanguageConnection = mp_LanguageCombo->signal_changed().connect(
      sigc::mem_fun(*this, &PreferencesInterfacePanel::onLanguageChanged));

  if (i18nManager::getInstance()->isNLSenable())
  {
    std::map<Glib::ustring, Glib::ustring> AvailLanguages =
        i18nManager::getInstance()->getAvailableLanguages();

    for (std::map<Glib::ustring, Glib::ustring>::iterator it =
        AvailLanguages.begin(); it != AvailLanguages.end(); ++it)
    {
      Gtk::TreeRow Row = *mref_LanguagesModel->append();
      Row[m_LanguagesColumns.m_Code] = it->first;
      Row[m_LanguagesColumns.m_Name] = it->second;
    }
  } else
    mp_LanguageCombo->set_sensitive(false);

  Gtk::HBox* LanguageBox = Gtk::manage(new Gtk::HBox(false, 5));
  LanguageBox->pack_start(*LangLabel, Gtk::PACK_SHRINK);
  LanguageBox->pack_start(*mp_LanguageCombo, Gtk::PACK_SHRINK);

  /*
   * Recents
   */

  Gtk::Label* RecentsLabel = Gtk::manage(new Gtk::Label(
      _("Maximum number of recent projects:")));

  mp_RecentsMaxSpinButton = Gtk::manage(new Gtk::SpinButton());
  mp_RecentsMaxSpinButton->set_numeric(true);
  mp_RecentsMaxSpinButton->set_range(0, 12);
  mp_RecentsMaxSpinButton->set_increments(1, 1);
  mp_RecentsMaxSpinButton->signal_changed().connect(sigc::mem_fun(*this,
      &PreferencesInterfacePanel::onRecentsMaxChanged));

  Gtk::Button* ClearRecentsButton = Gtk::manage(
      new Gtk::Button(_("Clear list")));
  ClearRecentsButton->signal_clicked().connect(sigc::mem_fun(*this,
      &PreferencesInterfacePanel::onClearRecentsClicked));

  Gtk::HBox* RecentsBox = Gtk::manage(new Gtk::HBox(false, 5));
  RecentsBox->pack_start(*RecentsLabel, Gtk::PACK_SHRINK);
  RecentsBox->pack_start(*mp_RecentsMaxSpinButton, Gtk::PACK_SHRINK);
  RecentsBox->pack_start(*ClearRecentsButton, Gtk::PACK_SHRINK);

  /*
   * Main panel
   */

  Gtk::Box* PanelBox = createPanelBox();
  PanelBox->pack_start(*createSubTitle(_("Interface Language")),
      Gtk::PACK_SHRINK);
  PanelBox->pack_start(*createSubBoxAlignement(LanguageBox), Gtk::PACK_SHRINK);
  PanelBox->pack_start(*createSubTitle(_("Recent projects")), Gtk::PACK_SHRINK);
  PanelBox->pack_start(*createSubBoxAlignement(RecentsBox), Gtk::PACK_SHRINK);
  PanelBox->show_all_children();

  mp_ContentWindow->add(*PanelBox);
}

// =====================================================================
// =====================================================================

void PreferencesInterfacePanel::onLanguageChanged()
{
  Gtk::TreeIter it = mp_LanguageCombo->get_active();

  if (it)
  {
    openfluid::guicommon::PreferencesManager::getInstance()->setLang(
        it->get_value(m_LanguagesColumns.m_Code));
  }

}

// =====================================================================
// =====================================================================

void PreferencesInterfacePanel::onRecentsMaxChanged()
{
  /*
   * Forces spinbutton to update for taking in account manual keyboard editing
   * (use with signal_changed and not signal_value_changed)
   */
  mp_RecentsMaxSpinButton->update();
  openfluid::guicommon::PreferencesManager::getInstance()->setRecentMax(
      mp_RecentsMaxSpinButton->get_value_as_int());
}

// =====================================================================
// =====================================================================

void PreferencesInterfacePanel::onClearRecentsClicked()
{
  openfluid::guicommon::PreferencesManager::getInstance()->clearRecentProjects();
}

// =====================================================================
// =====================================================================

void PreferencesInterfacePanel::init()
{
  if (i18nManager::getInstance()->isNLSenable())
  {
    m_LanguageConnection.block();

    Glib::ustring Lang =
        openfluid::guicommon::PreferencesManager::getInstance()->getLang();

    if (Lang.empty())
      mp_LanguageCombo->set_active(0);
    else
    {
      Gtk::TreeModel::Children Languages = mref_LanguagesModel->children();
      for (Gtk::TreeIter it = Languages.begin(); it != Languages.end(); ++it)
      {
        if (it->get_value(m_LanguagesColumns.m_Code) == Lang)
        {
          mp_LanguageCombo->set_active(it);
          break;
        }
      }
    }
    m_LanguageConnection.unblock();
  }

  mp_RecentsMaxSpinButton->set_value(
      openfluid::guicommon::PreferencesManager::getInstance()->getRecentMax());
}

// =====================================================================
// =====================================================================

// =====================================================================
// =====================================================================


PreferencesPathsPanel::PreferencesPathsPanel() :
    openfluid::guicommon::PreferencesPanel(_("Paths"))
{
  /*
   * Workdir
   */

  Gtk::Label* WorkdirLabel = Gtk::manage(new Gtk::Label(
      _("Default working directory:")));

  mp_WorkdirEntry = Gtk::manage(new Gtk::Entry());
  mp_WorkdirEntry->signal_changed().connect(sigc::mem_fun(*this,
      &PreferencesPathsPanel::onWorkdirEntryChanged));

  mp_WorkdirFileButton = Gtk::manage(new Gtk::Button(_("Browse...")));
  mp_WorkdirFileButton->signal_clicked().connect(sigc::mem_fun(*this,
      &PreferencesPathsPanel::onWorkdirFileButtonClicked));

  Gtk::HBox* WorkdirBox = Gtk::manage(new Gtk::HBox(false, 5));
  WorkdirBox->pack_start(*WorkdirLabel, Gtk::PACK_SHRINK);
  WorkdirBox->pack_start(*mp_WorkdirEntry, Gtk::PACK_EXPAND_WIDGET);
  WorkdirBox->pack_start(*mp_WorkdirFileButton, Gtk::PACK_SHRINK);
  WorkdirBox->set_visible(true);
  WorkdirBox->show_all_children();

  /*
   * Functions
   */

  mp_FunctionsPathListWidget = new PreferencesPathListWidget();
  mp_FunctionsPathListWidget->signal_PathListChanged().connect(sigc::mem_fun(
      *this, &PreferencesPathsPanel::onFunctionsPathListChanged));

  Gtk::Widget* FunctionPaths = mp_FunctionsPathListWidget->asWidget();
  FunctionPaths->set_visible(true);

  /*
   * Extensions
   */

  mp_ExtensionsPathListWidget = new PreferencesPathListWidget();
  mp_ExtensionsPathListWidget->signal_PathListChanged().connect(sigc::mem_fun(
      *this, &PreferencesPathsPanel::onExtensionsPathListChanged));

  Gtk::Widget* ExtensionsPaths = mp_ExtensionsPathListWidget->asWidget();
  ExtensionsPaths->set_visible(true);

  /*
   * Observers
   */

  mp_ObserversPathListWidget = new PreferencesPathListWidget();
  mp_ObserversPathListWidget->signal_PathListChanged().connect(sigc::mem_fun(
      *this, &PreferencesPathsPanel::onObserversPathListChanged));

  Gtk::Widget* ObserversPaths = mp_ObserversPathListWidget->asWidget();
  ObserversPaths->set_visible(true);

  /*
   * Main panel
   */

  Gtk::Box* PanelBox = createPanelBox();
  PanelBox->pack_start(*createSubTitle(_("Working directory")),
      Gtk::PACK_SHRINK);
  PanelBox->pack_start(*createSubBoxAlignement(WorkdirBox), Gtk::PACK_SHRINK);

  Gtk::Expander* PlugExpander = Gtk::manage(new Gtk::Expander());
  PlugExpander->set_label_widget(*createSubTitle(_("Search paths for simulation functions")));
  PlugExpander->add(*createSubBoxAlignement(FunctionPaths));
  PlugExpander->set_visible(true);
  PanelBox->pack_start(*PlugExpander, Gtk::PACK_SHRINK);

  Gtk::Expander* ExtExpander = Gtk::manage(new Gtk::Expander());
  ExtExpander->set_label_widget(*createSubTitle(_("Search paths for extensions (restart needed)")));
  ExtExpander->add(*createSubBoxAlignement(ExtensionsPaths));
  ExtExpander->set_visible(true);
  PanelBox->pack_start(*ExtExpander, Gtk::PACK_SHRINK);

  Gtk::Expander* ObsExpander = Gtk::manage(new Gtk::Expander());
  ObsExpander->set_label_widget(*createSubTitle(_("Search paths for observers (restart needed)")));
  ObsExpander->add(*createSubBoxAlignement(ObserversPaths));
  ObsExpander->set_visible(true);
  PanelBox->pack_start(*ObsExpander, Gtk::PACK_SHRINK);

  mp_ContentWindow->add(*PanelBox);
}

// =====================================================================
// =====================================================================

void PreferencesPathsPanel::init()
{
  mp_WorkdirEntry->set_text(
      openfluid::guicommon::PreferencesManager::getInstance()->getWorkdir());

  mp_FunctionsPathListWidget->setPreDefinedPaths(
      openfluid::base::RuntimeEnvironment::getInstance()->getDefaultFunctionsPluginsPaths());

  mp_FunctionsPathListWidget->setUserDefinedPaths(
      openfluid::guicommon::PreferencesManager::getInstance()->getExtraPlugPaths());

  mp_ExtensionsPathListWidget->setPreDefinedPaths(
      BuilderExtensionsManager::getInstance()->getExtensionsDefaultSearchPaths());

  mp_ExtensionsPathListWidget->setUserDefinedPaths(
      BuilderExtensionsManager::getInstance()->getExtensionsExtraSearchPaths());

  mp_ObserversPathListWidget->setPreDefinedPaths(
      openfluid::base::RuntimeEnvironment::getInstance()->getDefaultObserversPluginsPaths());

  mp_ObserversPathListWidget->setUserDefinedPaths(
      openfluid::base::RuntimeEnvironment::getInstance()->getExtraObserversPluginsPaths());
}

// =====================================================================
// =====================================================================


void PreferencesPathsPanel::onWorkdirEntryChanged()
{
  openfluid::guicommon::PreferencesManager::getInstance()->setWorkdir(
      mp_WorkdirEntry->get_text());
}

// =====================================================================
// =====================================================================

void PreferencesPathsPanel::onWorkdirFileButtonClicked()
{
  Gtk::FileChooserDialog Dialog(_("Choose OpenFLUID working directory"),
      Gtk::FILE_CHOOSER_ACTION_SELECT_FOLDER);

  Dialog.add_button(Gtk::Stock::CANCEL, Gtk::RESPONSE_CANCEL);
  Dialog.add_button(Gtk::Stock::OK, Gtk::RESPONSE_OK);

  Dialog.set_current_folder(Glib::get_home_dir());

  if (Dialog.run() == Gtk::RESPONSE_OK)
  {
    Glib::ustring SelectedFolder =
        Glib::filename_to_utf8(Dialog.get_filename());
    mp_WorkdirEntry->set_text(SelectedFolder);
    openfluid::guicommon::PreferencesManager::getInstance()->setWorkdir(
        SelectedFolder);
  }
}

// =====================================================================
// =====================================================================

void PreferencesPathsPanel::onFunctionsPathListChanged()
{
  openfluid::guicommon::PreferencesManager::getInstance()->setExtraPlugPaths(
      mp_FunctionsPathListWidget->getUserDefinedPaths());
}

// =====================================================================
// =====================================================================

void PreferencesPathsPanel::onExtensionsPathListChanged()
{
  openfluid::guicommon::PreferencesManager::getInstance()->setExtraExtensionPaths(
      mp_ExtensionsPathListWidget->getUserDefinedPaths());
}

// =====================================================================
// =====================================================================

void PreferencesPathsPanel::onObserversPathListChanged()
{
  openfluid::guicommon::PreferencesManager::getInstance()->setExtraObserversPaths(
        mp_ObserversPathListWidget->getUserDefinedPaths());
}

// =====================================================================
// =====================================================================

// =====================================================================
// =====================================================================


PreferencesSimPanel::PreferencesSimPanel() :
    openfluid::guicommon::PreferencesPanel(_("Simulations")), m_IsMonthChanged(false)
{
  /*
   * Time
   */

  mp_DeltaSpin = Gtk::manage(new Gtk::SpinButton());
  mp_DeltaSpin->set_range(1.0, 86400.0); //->24h
  mp_DeltaSpin->set_increments(1, 3600);
  mp_DeltaSpin->set_numeric(true);
  mp_DeltaSpin->signal_changed().connect(sigc::mem_fun(*this,
      &PreferencesSimPanel::onDeltaChanged));

  mp_BeginEntry = Gtk::manage(new Gtk::Entry());
  mp_BeginEntry->signal_changed().connect(sigc::mem_fun(*this,
      &PreferencesSimPanel::onBeginEntryChanged));

  Gtk::Button* DateBeginButton = Gtk::manage(new Gtk::Button("..."));
  DateBeginButton->signal_clicked().connect(sigc::mem_fun(*this,
      &PreferencesSimPanel::onDateBeginButtonClicked));

  mp_BeginHSpin = Gtk::manage(new Gtk::SpinButton());
  mp_BeginHSpin->set_range(0.0, 23.0);
  mp_BeginHSpin->set_increments(1, 1);
  mp_BeginHSpin->set_numeric(true);
  mp_BeginHSpin->signal_changed().connect(sigc::mem_fun(*this,
      &PreferencesSimPanel::onBeginValueChanged));

  mp_BeginMSpin = Gtk::manage(new Gtk::SpinButton());
  mp_BeginMSpin->set_range(0.0, 59.0);
  mp_BeginMSpin->set_increments(1, 10);
  mp_BeginMSpin->set_numeric(true);
  mp_BeginMSpin->signal_changed().connect(sigc::mem_fun(*this,
      &PreferencesSimPanel::onBeginValueChanged));

  mp_BeginSSpin = Gtk::manage(new Gtk::SpinButton());
  mp_BeginSSpin->set_range(0.0, 59.0);
  mp_BeginSSpin->set_increments(1, 10);
  mp_BeginSSpin->set_numeric(true);
  mp_BeginSSpin->signal_changed().connect(sigc::mem_fun(*this,
      &PreferencesSimPanel::onBeginValueChanged));

  mp_EndEntry = Gtk::manage(new Gtk::Entry());
  mp_EndEntry->signal_changed().connect(sigc::mem_fun(*this,
      &PreferencesSimPanel::onEndEntryChanged));

  Gtk::Button* DateEndButton = Gtk::manage(new Gtk::Button("..."));
  DateEndButton->signal_clicked().connect(sigc::mem_fun(*this,
      &PreferencesSimPanel::onDateEndButtonClicked));

  mp_EndHSpin = Gtk::manage(new Gtk::SpinButton());
  mp_EndHSpin->set_range(0.0, 23.0);
  mp_EndHSpin->set_increments(1, 1);
  mp_EndHSpin->set_numeric(true);
  mp_EndHSpin->signal_changed().connect(sigc::mem_fun(*this,
      &PreferencesSimPanel::onEndValueChanged));

  mp_EndMSpin = Gtk::manage(new Gtk::SpinButton());
  mp_EndMSpin->set_range(0.0, 59.0);
  mp_EndMSpin->set_increments(1, 10);
  mp_EndMSpin->set_numeric(true);
  mp_EndMSpin->signal_changed().connect(sigc::mem_fun(*this,
      &PreferencesSimPanel::onEndValueChanged));

  mp_EndSSpin = Gtk::manage(new Gtk::SpinButton());
  mp_EndSSpin->set_range(0.0, 59.0);
  mp_EndSSpin->set_increments(1, 10);
  mp_EndSSpin->set_numeric(true);
  mp_EndSSpin->signal_changed().connect(sigc::mem_fun(*this,
      &PreferencesSimPanel::onEndValueChanged));

  Gtk::HBox* BeginDateBox = Gtk::manage(new Gtk::HBox(false, 5));
  BeginDateBox->pack_start(*mp_BeginEntry, Gtk::PACK_SHRINK);
  BeginDateBox->pack_start(*DateBeginButton, Gtk::PACK_SHRINK);

  Gtk::HBox* BeginTimeBox = Gtk::manage(new Gtk::HBox(false, 5));
  BeginTimeBox->pack_start(*mp_BeginHSpin, Gtk::PACK_SHRINK);
  BeginTimeBox->pack_start(*Gtk::manage(new Gtk::Label(_("h"))),
      Gtk::PACK_SHRINK);
  BeginTimeBox->pack_start(*mp_BeginMSpin, Gtk::PACK_SHRINK);
  BeginTimeBox->pack_start(*Gtk::manage(new Gtk::Label(_("m"))),
      Gtk::PACK_SHRINK);
  BeginTimeBox->pack_start(*mp_BeginSSpin, Gtk::PACK_SHRINK);
  BeginTimeBox->pack_start(*Gtk::manage(new Gtk::Label(_("s"))),
      Gtk::PACK_SHRINK);

  Gtk::HBox* EndDateBox = Gtk::manage(new Gtk::HBox(false, 5));
  EndDateBox->pack_start(*mp_EndEntry, Gtk::PACK_SHRINK);
  EndDateBox->pack_start(*DateEndButton, Gtk::PACK_SHRINK);

  Gtk::HBox* EndTimeBox = Gtk::manage(new Gtk::HBox(false, 5));
  EndTimeBox->pack_start(*mp_EndHSpin, Gtk::PACK_SHRINK);
  EndTimeBox->pack_start(*Gtk::manage(new Gtk::Label(_("h"))), Gtk::PACK_SHRINK);
  EndTimeBox->pack_start(*mp_EndMSpin, Gtk::PACK_SHRINK);
  EndTimeBox->pack_start(*Gtk::manage(new Gtk::Label(_("m"))), Gtk::PACK_SHRINK);
  EndTimeBox->pack_start(*mp_EndSSpin, Gtk::PACK_SHRINK);
  EndTimeBox->pack_start(*Gtk::manage(new Gtk::Label(_("s"))), Gtk::PACK_SHRINK);

  Gtk::HBox* DeltaBox = Gtk::manage(new Gtk::HBox(false, 5));
  DeltaBox->pack_start(*mp_DeltaSpin, Gtk::PACK_SHRINK);
  DeltaBox->pack_start(*Gtk::manage(new Gtk::Label(_("seconds"))),
      Gtk::PACK_SHRINK);

  Gtk::VBox* BeginBox = Gtk::manage(new Gtk::VBox(false, 5));
  BeginBox->pack_start(*BeginDateBox, Gtk::PACK_SHRINK);
  BeginBox->pack_start(*BeginTimeBox, Gtk::PACK_SHRINK);

  Gtk::VBox* EndBox = Gtk::manage(new Gtk::VBox(false, 5));
  EndBox->pack_start(*EndDateBox, Gtk::PACK_SHRINK);
  EndBox->pack_start(*EndTimeBox, Gtk::PACK_SHRINK);

  Gtk::Table* TimeTable = Gtk::manage(new Gtk::Table());
  TimeTable->set_spacings(15);
  TimeTable->attach(*Gtk::manage(new Gtk::Label(_("Delta T"), 0, 0.5)), 0, 1,
      0, 1, Gtk::FILL, Gtk::SHRINK, 0, 0);
  TimeTable->attach(*DeltaBox, 1, 2, 0, 1, Gtk::FILL | Gtk::EXPAND,
      Gtk::SHRINK, 0, 0);
  TimeTable->attach(*Gtk::manage(new Gtk::Label(_("Period begin"), 0, 0)), 0,
      1, 1, 2, Gtk::FILL, Gtk::FILL, 0, 0);
  TimeTable->attach(*BeginBox, 1, 2, 1, 2, Gtk::FILL | Gtk::EXPAND,
      Gtk::SHRINK, 0, 0);
  TimeTable->attach(*Gtk::manage(new Gtk::Label(_("Period end"), 0, 0)), 0, 1,
      2, 3, Gtk::FILL, Gtk::FILL, 0, 0);
  TimeTable->attach(*EndBox, 1, 2, 2, 3, Gtk::FILL | Gtk::EXPAND, Gtk::SHRINK,
      0, 0);
  TimeTable->set_visible(true);
  TimeTable->show_all_children();

  /*
   * Outputs
   */

  Gtk::Label* FilesBuffLabel = Gtk::manage(new Gtk::Label(
      _("Output files buffer")));

  mp_FilesBuffSpin = Gtk::manage(new Gtk::SpinButton());
  mp_FilesBuffSpin->set_range(1.0, 999.0);
  mp_FilesBuffSpin->set_increments(1, 10);
  mp_FilesBuffSpin->set_numeric(true);
  mp_FilesBuffSpin->signal_changed().connect(sigc::mem_fun(*this,
      &PreferencesSimPanel::onFilesBuffChanged));

  Gtk::Label* FilesBuffUnit = Gtk::manage(new Gtk::Label(_("KB")));

  Gtk::HBox* FilesBufferBox = Gtk::manage(new Gtk::HBox(false, 5));
  FilesBufferBox->pack_start(*FilesBuffLabel, Gtk::PACK_SHRINK);
  FilesBufferBox->pack_start(*mp_FilesBuffSpin, Gtk::PACK_SHRINK);
  FilesBufferBox->pack_start(*FilesBuffUnit, Gtk::PACK_SHRINK);

  Gtk::VBox* OutputsBox = Gtk::manage(new Gtk::VBox(false, 5));
  OutputsBox->pack_start(*FilesBufferBox, Gtk::PACK_SHRINK);
  OutputsBox->set_visible(true);
  OutputsBox->show_all_children();

  /*
   * Main panel
   */

  Gtk::Box* PanelBox = createPanelBox();
  PanelBox->pack_start(*createSubTitle(_("Default Time settings")), Gtk::PACK_SHRINK);
  PanelBox->pack_start(*createSubBoxAlignement(TimeTable), Gtk::PACK_SHRINK);
  PanelBox->pack_start(*createSubTitle(_("Default Outputs settings")), Gtk::PACK_SHRINK);
  PanelBox->pack_start(*createSubBoxAlignement(OutputsBox), Gtk::PACK_SHRINK);

  mp_ContentWindow->add(*PanelBox);

  /*
   * cf. http://www.kksou.com/php-gtk2/articles/let-user-enter-date-with-a-popup-calendar---Part-2.php
   */
  mp_Calendar = Gtk::manage(new Gtk::Calendar());
  mp_Calendar->signal_day_selected().connect(sigc::bind<bool>(sigc::mem_fun(
      *this, &PreferencesSimPanel::onCalendarDaySelected), false));
  mp_Calendar->signal_month_changed().connect(sigc::bind<bool>(sigc::mem_fun(
      *this, &PreferencesSimPanel::onCalendarDaySelected), true));
  mp_Calendar->signal_button_press_event().connect(sigc::mem_fun(*this,
      &PreferencesSimPanel::onCalendarButtonPressEvent));

  mp_CalendarDialog = new Gtk::Dialog();
  mp_CalendarDialog->get_vbox()->pack_start(*mp_Calendar);
  mp_CalendarDialog->show_all_children();

}

// =====================================================================
// =====================================================================


PreferencesSimPanel::~PreferencesSimPanel()
{
  delete mp_CalendarDialog;
}

// =====================================================================
// =====================================================================

void PreferencesSimPanel::init()
{
  openfluid::guicommon::PreferencesManager* PrefMgr =
      openfluid::guicommon::PreferencesManager::getInstance();

  mp_DeltaSpin->set_value(PrefMgr->getDeltaT());
  mp_BeginEntry->set_text(PrefMgr->getBegin());
  mp_EndEntry->set_text(PrefMgr->getEnd());
  mp_FilesBuffSpin->set_value(PrefMgr->getOutFilesBufferInKB());
}

// =====================================================================
// =====================================================================


void PreferencesSimPanel::onDeltaChanged()
{
  mp_DeltaSpin->update();
  openfluid::guicommon::PreferencesManager::getInstance()->setDeltaT(
      mp_DeltaSpin->get_value_as_int());
}

// =====================================================================
// =====================================================================


void PreferencesSimPanel::onCalendarDaySelected(bool IsMonthChange)
{
  m_IsMonthChanged = IsMonthChange;
}

// =====================================================================
// =====================================================================


bool PreferencesSimPanel::onCalendarButtonPressEvent(GdkEventButton* /*Event*/)
{
  if (!m_IsMonthChanged)
    mp_CalendarDialog->response(Gtk::RESPONSE_OK);

  return true;
}

// =====================================================================
// =====================================================================


void PreferencesSimPanel::onDateBeginButtonClicked()
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


void PreferencesSimPanel::onBeginEntryChanged()
{
  Glib::ustring Begin = mp_BeginEntry->get_text();
  openfluid::core::DateTime DT;
  Glib::ustring Color;

  if (Begin.empty())
  {
    Color = "white";
    mp_BeginHSpin->set_value(0);
    mp_BeginMSpin->set_value(0);
    mp_BeginSSpin->set_value(0);
  } else if (DT.setFromISOString(Begin))
  {
    Color = "white";
    mp_BeginHSpin->set_value(DT.getHour());
    mp_BeginMSpin->set_value(DT.getMinute());
    mp_BeginSSpin->set_value(DT.getSecond());
  } else
    Color = "red";

  mp_BeginEntry->modify_base(mp_BeginEntry->get_state(), Gdk::Color(Color));

  openfluid::guicommon::PreferencesManager::getInstance()->setBegin(Begin);
}

// =====================================================================
// =====================================================================


void PreferencesSimPanel::onBeginValueChanged()
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


void PreferencesSimPanel::onDateEndButtonClicked()
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


void PreferencesSimPanel::onEndEntryChanged()
{
  Glib::ustring End = mp_EndEntry->get_text();
  openfluid::core::DateTime DT;
  Glib::ustring Color;

  if (End.empty())
  {
    Color = "white";
    mp_EndHSpin->set_value(0);
    mp_EndMSpin->set_value(0);
    mp_EndSSpin->set_value(0);
  } else if (DT.setFromISOString(End))
  {
    Color = "white";
    mp_EndHSpin->set_value(DT.getHour());
    mp_EndMSpin->set_value(DT.getMinute());
    mp_EndSSpin->set_value(DT.getSecond());
  } else
    Color = "red";

  mp_EndEntry->modify_base(mp_EndEntry->get_state(), Gdk::Color(Color));

  openfluid::guicommon::PreferencesManager::getInstance()->setEnd(End);
}

// =====================================================================
// =====================================================================


void PreferencesSimPanel::onEndValueChanged()
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


void PreferencesSimPanel::onFilesBuffChanged()
{
  mp_FilesBuffSpin->update();
  openfluid::guicommon::PreferencesManager::getInstance()->setOutFilesBufferInKB(
      mp_FilesBuffSpin->get_value_as_int());
}



// =====================================================================
// =====================================================================

// =====================================================================
// =====================================================================


PreferencesMarketPanel::PreferencesMarketPanel() :
    openfluid::guicommon::PreferencesPanel(_("Market"))
{
  mp_PlacesListWidget = new PreferencesPlacesListWidget();

  Gtk::Widget* PlacesPaths = mp_PlacesListWidget->asWidget();
  PlacesPaths->set_visible(true);

  Gtk::Box* PanelBox = createPanelBox();
  PanelBox->pack_start(*createSubTitle(_("Marketplaces")),
      Gtk::PACK_SHRINK);
  PanelBox->pack_start(*createSubBoxAlignement(PlacesPaths), Gtk::PACK_SHRINK);

  mp_ContentWindow->add(*PanelBox);
}

// =====================================================================
// =====================================================================

void PreferencesMarketPanel::init()
{
  mp_PlacesListWidget->setPlaces(
      openfluid::guicommon::PreferencesManager::getInstance()->getMarketplaces());
}


// =====================================================================
// =====================================================================
