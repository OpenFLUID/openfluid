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
 \file PreferencesUrlListWidget.cpp
 \brief Implements ...

 \author Aline LIBRES <libres@supagro.inra.fr>
 */

#include "PreferencesPlacesListWidget.hpp"

#include <glibmm/i18n.h>

#include <gtkmm/label.h>
#include <gtkmm/stock.h>

#include "BuilderButtonBox.hpp"
#include <openfluid/guicommon-gtk/PreferencesManager.hpp>

// =====================================================================
// =====================================================================


PreferencesPlacesListWidget::PreferencesPlacesListWidget()
{
  mref_PlacesModel = Gtk::ListStore::create(m_PlacesColumns);

  mp_PlacesTreeView = Gtk::manage(new Gtk::TreeView(mref_PlacesModel));
  mp_PlacesTreeView->append_column(_("Name"), m_PlacesColumns.m_Name);
  mp_PlacesTreeView->append_column("URL", m_PlacesColumns.m_Path);
  mp_PlacesTreeView->set_visible(true);

  mp_ToolBox = new BuilderListButtonBox();
  mp_ToolBox->setAddCommandVisible();
  mp_ToolBox->setRemoveCommandVisible();

  mp_ToolBox->setAddCommandTooltipText(_("Add a marketplace"));
  mp_ToolBox->setRemoveCommandTooltipText(_(
      "Remove the selected marketplace"));

  mp_ToolBox->signal_AddCommandAsked().connect(sigc::mem_fun(*this,
      &PreferencesPlacesListWidget::whenAddPlaceAsked));
  mp_ToolBox->signal_RemoveCommandAsked().connect(sigc::mem_fun(*this,
      &PreferencesPlacesListWidget::whenRemovePlaceAsked));

  Gtk::HBox* MarketplacesBox = Gtk::manage(new Gtk::HBox(false, 5));
  MarketplacesBox->pack_start(*mp_PlacesTreeView);
  MarketplacesBox->pack_start(*mp_ToolBox->asWidget(), Gtk::PACK_SHRINK);
  MarketplacesBox->set_border_width(5);
  MarketplacesBox->set_visible(true);

  mp_MainBox = Gtk::manage(new Gtk::VBox());
  mp_MainBox->pack_start(*MarketplacesBox);
  mp_MainBox->set_visible(true);

  mp_MainFrame = Gtk::manage(new Gtk::Frame());
  mp_MainFrame->set_shadow_type(Gtk::SHADOW_IN);
  mp_MainFrame->add(*mp_MainBox);

  /*
   * Add place Dialog box
   */

  mp_InfoBarLabel = Gtk::manage(new Gtk::Label());
  mp_InfoBar = Gtk::manage(new Gtk::InfoBar());
  mp_InfoBar->set_message_type(Gtk::MESSAGE_WARNING);
  ((Gtk::Container*) mp_InfoBar->get_content_area())->add(*mp_InfoBarLabel);

  mp_NameLabel = Gtk::manage(new Gtk::Label(_("Marketplace Name:"), 0, 0.5));
  mp_NameEntry = Gtk::manage(new Gtk::Entry());
  mp_NameEntry->set_activates_default(true);
  mp_NameEntry->signal_changed().connect(sigc::mem_fun(*this,
      &PreferencesPlacesListWidget::whenEntryChanged));

  mp_UrlLabel = Gtk::manage(new Gtk::Label(_("Marketplace URL:"), 0, 0.5));
  mp_UrlEntry = Gtk::manage(new Gtk::Entry());
  mp_UrlEntry->set_activates_default(true);
  mp_UrlEntry->signal_changed().connect(sigc::mem_fun(*this,
      &PreferencesPlacesListWidget::whenEntryChanged));

  mp_AddTable = Gtk::manage(new Gtk::Table());
  mp_AddTable->attach(*mp_NameLabel, 0, 1, 0, 1, Gtk::FILL, Gtk::SHRINK, 0, 5);
  mp_AddTable->attach(*mp_NameEntry, 1, 2, 0, 1, Gtk::EXPAND, Gtk::SHRINK, 0, 5);
  mp_AddTable->attach(*mp_UrlLabel, 0, 1, 1, 2, Gtk::FILL, Gtk::SHRINK, 0, 5);
  mp_AddTable->attach(*mp_UrlEntry, 1, 2, 1, 2, Gtk::EXPAND, Gtk::SHRINK, 0, 5);

  mp_AddPlaceDialog = new Gtk::Dialog();
  mp_AddPlaceDialog->get_vbox()->pack_start(*mp_InfoBar, Gtk::PACK_SHRINK, 5);
  mp_AddPlaceDialog->get_vbox()->pack_start(*mp_AddTable);

  mp_AddPlaceDialog->add_button(Gtk::Stock::CANCEL, Gtk::RESPONSE_CANCEL);
  mp_AddPlaceDialog->add_button(Gtk::Stock::OK, Gtk::RESPONSE_OK);
  mp_AddPlaceDialog->set_default_response(Gtk::RESPONSE_OK);

  mp_AddPlaceDialog->show_all_children();
}

// =====================================================================
// =====================================================================


Gtk::Widget* PreferencesPlacesListWidget::asWidget()
{
  return mp_MainFrame;
}

// =====================================================================
// =====================================================================


void PreferencesPlacesListWidget::setPlaces(
    std::map<std::string, std::string> Places)
{
  mref_PlacesModel->clear();

  for (std::map<std::string, std::string>::iterator it = Places.begin(); it
      != Places.end(); ++it)
  {
    Gtk::TreeRow Row = *mref_PlacesModel->append();
    Row[m_PlacesColumns.m_Name] = it->first;
    Row[m_PlacesColumns.m_Path] = it->second;
  }
}

// =====================================================================
// =====================================================================

void PreferencesPlacesListWidget::whenAddPlaceAsked()
{
  mp_NameEntry->set_text("");
  mp_UrlEntry->set_text("");
  whenEntryChanged();

  if (mp_AddPlaceDialog->run() == Gtk::RESPONSE_OK)
  {
    Gtk::TreeRow Row = *mref_PlacesModel->append();
    Row[m_PlacesColumns.m_Path] = mp_UrlEntry->get_text();
    Row[m_PlacesColumns.m_Name] = mp_NameEntry->get_text();

    openfluid::guicommon::PreferencesManager::getInstance()->addMarketplace(
        mp_NameEntry->get_text(), mp_UrlEntry->get_text());
  }

  mp_AddPlaceDialog->hide();

}

// =====================================================================
// =====================================================================


void PreferencesPlacesListWidget::whenEntryChanged()
{
  bool m_IsValid = false;

  std::map<std::string, std::string>
      Places =
          openfluid::guicommon::PreferencesManager::getInstance()->getMarketplaces();

  if (mp_NameEntry->get_text().empty())
    mp_InfoBarLabel->set_text(_("The marketplace name cannot be empty"));
  else if (Places.find(mp_NameEntry->get_text()) != Places.end())
    mp_InfoBarLabel->set_text(_("This marketplace name already exists"));
  else if (mp_UrlEntry->get_text().empty())
    mp_InfoBarLabel->set_text(_("The marketplace URL cannot be empty"));
  else
    m_IsValid = true;

  mp_InfoBar->set_visible(!m_IsValid);
  mp_AddPlaceDialog->set_response_sensitive(Gtk::RESPONSE_OK, m_IsValid);
}

// =====================================================================
// =====================================================================


void PreferencesPlacesListWidget::whenRemovePlaceAsked()
{
  Gtk::TreeIter it = mp_PlacesTreeView->get_selection()->get_selected();

  if (it)
  {
    openfluid::guicommon::PreferencesManager::getInstance()->removeMarketplace(
        it->get_value(m_PlacesColumns.m_Name));
    mref_PlacesModel->erase(it);
  }
}

// =====================================================================
// =====================================================================


