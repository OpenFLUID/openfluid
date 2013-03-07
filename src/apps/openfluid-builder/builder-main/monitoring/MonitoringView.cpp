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
 \file MonitoringView.cpp
 \brief Implements ...

 \author Aline LIBRES <aline.libres@gmail.com>
 */

#include "MonitoringView.hpp"

#include <glibmm/i18n.h>
#include <gtkmm/stock.h>
#include <gtkmm/label.h>

// =====================================================================
// =====================================================================

MonitoringView::MonitoringView()
{
  mp_AddButton = Gtk::manage(new Gtk::Button());
  mp_AddButton->set_image(
      *Gtk::manage(new Gtk::Image(Gtk::Stock::ADD, Gtk::ICON_SIZE_BUTTON)));
  mp_AddButton->set_tooltip_text(_("Add an observer"));
  mp_AddButton->signal_clicked().connect(
      sigc::mem_fun(*this, &MonitoringView::onAddButtonClicked));

  Gtk::HBox* TopBox = Gtk::manage(new Gtk::HBox());
  TopBox->pack_start(*mp_AddButton, Gtk::PACK_SHRINK, 10);

  mp_ListBox = Gtk::manage(new Gtk::VBox());

  mp_MainBox = Gtk::manage(new Gtk::VBox());
  mp_MainBox->pack_start(*TopBox, Gtk::PACK_SHRINK, 10);
  mp_MainBox->pack_start(*mp_ListBox, Gtk::PACK_EXPAND_WIDGET, 10);
  mp_MainBox->set_visible(true);
  mp_MainBox->show_all_children();
}

// =====================================================================
// =====================================================================

MonitoringView::~MonitoringView()
{

}

// =====================================================================
// =====================================================================

void MonitoringView::update(
    std::list<std::pair<std::string, std::string> > Items)
{
  int ItemsCount = mp_ListBox->children().size();

  for (int i = 0; i < ItemsCount; i++)
    mp_ListBox->remove(*mp_ListBox->children().begin()->get_widget());

  for (std::list<std::pair<std::string, std::string> >::iterator it =
      Items.begin(); it != Items.end(); ++it)
  {
    Gtk::Label* IDLabel = Gtk::manage(new Gtk::Label(it->first));
    IDLabel->set_justify(Gtk::JUSTIFY_LEFT);
    Gtk::Label* NameLabel = Gtk::manage(new Gtk::Label(it->second));
    NameLabel->set_justify(Gtk::JUSTIFY_LEFT);

    Gtk::VBox* InfoBox = Gtk::manage(new Gtk::VBox());
    InfoBox->pack_start(*IDLabel, Gtk::PACK_SHRINK);
    InfoBox->pack_start(*NameLabel, Gtk::PACK_SHRINK);

    Gtk::Button* EditBt = Gtk::manage(new Gtk::Button(_("Edit parameters...")));
    EditBt->signal_clicked().connect(
        sigc::bind<std::string>(
            sigc::mem_fun(*this, &MonitoringView::onEditButtonClicked),
            it->first));

    Gtk::Button* RemoveBt = Gtk::manage(new Gtk::Button());
    RemoveBt->set_image(
        *Gtk::manage(
            new Gtk::Image(Gtk::Stock::REMOVE, Gtk::ICON_SIZE_BUTTON)));
    RemoveBt->set_tooltip_text(_("Remove this observer"));
    RemoveBt->signal_clicked().connect(
        sigc::bind<std::string>(
            sigc::mem_fun(*this, &MonitoringView::onRemoveButtonClicked),
            it->first));

    Gtk::HBox* ObsBox = Gtk::manage(new Gtk::HBox());
    ObsBox->pack_start(*InfoBox, Gtk::PACK_SHRINK);
    ObsBox->pack_end(*RemoveBt, Gtk::PACK_SHRINK, 0, 5);
    ObsBox->pack_end(*EditBt, Gtk::PACK_SHRINK, 0, 5);

    mp_ListBox->pack_start(*ObsBox, Gtk::PACK_SHRINK, 0, 10);
  }

  mp_ListBox->show_all_children();
}

// =====================================================================
// =====================================================================

Gtk::Widget* MonitoringView::asWidget()
{
  return mp_MainBox;
}

// =====================================================================
// =====================================================================

void MonitoringView::onAddButtonClicked()
{
  m_signal_AddObserverAsked.emit();
}

// =====================================================================
// =====================================================================

void MonitoringView::onEditButtonClicked(std::string ObserverID)
{
  m_signal_EditParamsAsked.emit(ObserverID);
}

// =====================================================================
// =====================================================================

void MonitoringView::onRemoveButtonClicked(std::string ObserverID)
{
  m_signal_RemoveObserverAsked(ObserverID);
}

// =====================================================================
// =====================================================================

sigc::signal<void, std::string> MonitoringView::signal_EditParamsAsked()
{
  return m_signal_EditParamsAsked;
}

// =====================================================================
// =====================================================================

sigc::signal<void, std::string> MonitoringView::signal_RemoveObserverAsked()
{
  return m_signal_RemoveObserverAsked;
}

// =====================================================================
// =====================================================================

sigc::signal<void> MonitoringView::signal_AddObserverAsked()
{
  return m_signal_AddObserverAsked;
}

// =====================================================================
// =====================================================================
