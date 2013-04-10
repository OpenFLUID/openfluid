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
 \file WareItemWidget.cpp
 \brief Implements ...

 \author Aline LIBRES <aline.libres@gmail.com>
 */

#include "WareItemWidget.hpp"

#include <glibmm/i18n.h>
#include <gtkmm/stock.h>
#include <gtkmm/scrolledwindow.h>
#include <gtkmm/eventbox.h>
#include <gtkmm/box.h>
#include <gtkmm/separator.h>
#include <gtkmm/paned.h>

// =====================================================================
// =====================================================================

WareItemWidget::WareItemWidget(std::string ID, Gtk::Widget& ParamWidget,
                               Gtk::Widget& InfoWidget, std::string Description)
{
  mp_IDLabel = Gtk::manage(new Gtk::Label());
  mp_IDLabel->set_justify(Gtk::JUSTIFY_LEFT);
  mp_IDLabel->set_alignment(Gtk::ALIGN_LEFT, Gtk::ALIGN_CENTER);
  mp_IDLabel->set_markup("<b>" + ID + "</b>");

  mp_DescriptionLabel = Gtk::manage(new Gtk::Label(Description));
  mp_DescriptionLabel->set_alignment(Gtk::ALIGN_LEFT, Gtk::ALIGN_CENTER);

  Gtk::Button* RemoveBt = Gtk::manage(new Gtk::Button());
  RemoveBt->set_image(
      *Gtk::manage(new Gtk::Image(Gtk::Stock::REMOVE, Gtk::ICON_SIZE_BUTTON)));
  RemoveBt->set_tooltip_text(Glib::ustring::compose(_("Remove %1"), ID));
  RemoveBt->signal_clicked().connect(
      sigc::bind<std::string>(
          sigc::mem_fun(*this, &WareItemWidget::onRemoveButtonClicked), ID));

  mp_UpBt = Gtk::manage(new Gtk::Button());
  mp_UpBt->set_image(
      *Gtk::manage(new Gtk::Image(Gtk::Stock::GO_UP, Gtk::ICON_SIZE_BUTTON)));
  mp_UpBt->set_tooltip_text(Glib::ustring::compose(_("Move up %1"), ID));
  mp_UpBt->signal_clicked().connect(
      sigc::bind<std::string>(
          sigc::mem_fun(*this, &WareItemWidget::onUpButtonClicked), ID));

  mp_DownBt = Gtk::manage(new Gtk::Button());
  mp_DownBt->set_image(
      *Gtk::manage(new Gtk::Image(Gtk::Stock::GO_DOWN, Gtk::ICON_SIZE_BUTTON)));
  mp_DownBt->set_tooltip_text(Glib::ustring::compose(_("Move down %1"), ID));
  mp_DownBt->signal_clicked().connect(
      sigc::bind<std::string>(
          sigc::mem_fun(*this, &WareItemWidget::onDownButtonClicked), ID));

  Gtk::Frame* ParamFrame = Gtk::manage(new Gtk::Frame());
  ParamFrame->set_shadow_type(Gtk::SHADOW_ETCHED_IN);
  ParamFrame->add(ParamWidget);
  Gtk::HPaned* ExpanderBox = Gtk::manage(new Gtk::HPaned());
  ExpanderBox->pack1(*ParamFrame, true, true);
  ExpanderBox->pack2(InfoWidget, true, true);

  mp_Expander = Gtk::manage(new Gtk::Expander(_("Parameters and information")));
  mp_Expander->add(*ExpanderBox);

  Gtk::VBox* InfoBox = Gtk::manage(new Gtk::VBox(true, 10));
  InfoBox->pack_start(*mp_IDLabel, Gtk::PACK_SHRINK);
  InfoBox->pack_start(*mp_DescriptionLabel, Gtk::PACK_SHRINK);

  Gtk::VBox* ButtonBox = Gtk::manage(new Gtk::VBox(Gtk::BUTTONBOX_START));
  ButtonBox->pack_start(*mp_UpBt, Gtk::PACK_SHRINK);
  ButtonBox->pack_start(*RemoveBt, Gtk::PACK_SHRINK);
  ButtonBox->pack_start(*mp_DownBt, Gtk::PACK_SHRINK);

  // to keep buttons start-pack even when Expander is expanded
  Gtk::VBox* ButtonBoxBox = Gtk::manage(new Gtk::VBox());
  ButtonBoxBox->pack_start(*ButtonBox, Gtk::PACK_SHRINK);

  Gtk::VBox* MainInfoBox = Gtk::manage(new Gtk::VBox(false, 10));
  MainInfoBox->pack_start(*InfoBox, Gtk::PACK_SHRINK);
  MainInfoBox->pack_end(*mp_Expander, Gtk::PACK_EXPAND_WIDGET);

  Gtk::HBox* MainBox = Gtk::manage(new Gtk::HBox());
  MainBox->pack_start(*MainInfoBox, Gtk::PACK_EXPAND_WIDGET);
  MainBox->pack_end(*ButtonBoxBox, Gtk::PACK_SHRINK);
  MainBox->set_border_width(10);

  // for background color
  Gtk::EventBox* MainEventBox = Gtk::manage(new Gtk::EventBox());
  MainEventBox->add(*MainBox);
  MainEventBox->modify_bg(Gtk::STATE_NORMAL, Gdk::Color("#FFFFFF"));

  set_shadow_type(Gtk::SHADOW_ETCHED_IN);
  set_border_width(5);
  add(*MainEventBox);

  show_all_children();
}

// =====================================================================
// =====================================================================

WareItemWidget::~WareItemWidget()
{

}

// =====================================================================
// =====================================================================

void WareItemWidget::setUpButtonSensitive(bool Sensitive)
{
  mp_UpBt->set_sensitive(Sensitive);
}

// =====================================================================
// =====================================================================

void WareItemWidget::setDownButtonSensitive(bool Sensitive)
{
  mp_DownBt->set_sensitive(Sensitive);
}

// =====================================================================
// =====================================================================

bool WareItemWidget::getExpanded()
{
  return mp_Expander->get_expanded();
}

// =====================================================================
// =====================================================================

void WareItemWidget::setExpanded(bool Expanded)
{
  mp_Expander->set_expanded(Expanded);
}

// =====================================================================
// =====================================================================

void WareItemWidget::onRemoveButtonClicked(std::string ID)
{
  m_signal_RemoveAsked.emit(ID);
}

// =====================================================================
// =====================================================================

void WareItemWidget::onUpButtonClicked(std::string ID)
{
  m_signal_UpAsked.emit(ID);
}

// =====================================================================
// =====================================================================

void WareItemWidget::onDownButtonClicked(std::string ID)
{
  m_signal_DownAsked.emit(ID);
}

// =====================================================================
// =====================================================================

sigc::signal<void, std::string> WareItemWidget::signal_RemoveAsked()
{
  return m_signal_RemoveAsked;
}

// =====================================================================
// =====================================================================

sigc::signal<void, std::string> WareItemWidget::signal_UpAsked()
{
  return m_signal_UpAsked;
}

// =====================================================================
// =====================================================================

sigc::signal<void, std::string> WareItemWidget::signal_DownAsked()
{
  return m_signal_DownAsked;
}

// =====================================================================
// =====================================================================

