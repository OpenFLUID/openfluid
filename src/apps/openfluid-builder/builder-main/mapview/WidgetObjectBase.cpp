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
 \file WidgetObjectBase.cpp
 \brief Implements ...

 \author Damien CHABBERT <dams.vivien@gmail.com>
 */

#include <glibmm/i18n.h>
#include <gtkmm/toolbar.h>
#include <gtkmm/stock.h>
#include <gtkmm/table.h>
#include "BuilderGraphicsHelper.hpp"

#include "WidgetObjectBase.hpp"

WidgetObjectBase::WidgetObjectBase(std::string ClassName, std::string Id) :
  WidgetObject(), m_CurrentState(Gtk::STATE_NORMAL), mp_TypeImage(0)
{
  mp_DisplayLayerCheckBox = Gtk::manage(new Gtk::CheckButton(ClassName));
  mp_DisplayLayerCheckBox->set_tooltip_text(_("Display the layer"));
  mp_DisplayLayerCheckBox->set_active(true);

  mp_TypeImage = Gtk::manage(new Gtk::Image(getPixbufForType(1)));

  Gtk::Table* InfoTable = Gtk::manage(new Gtk::Table());
  InfoTable->set_spacings(5);
  InfoTable->attach(*mp_DisplayLayerCheckBox, 0, 1, 0, 1, Gtk::FILL
      | Gtk::EXPAND, Gtk::SHRINK, 5, 0);
  InfoTable->attach(*mp_TypeImage, 0, 1, 1, 2, Gtk::FILL | Gtk::EXPAND,
      Gtk::SHRINK, 5, 0);
  InfoTable->attach(*Gtk::manage(new Gtk::Label(_("Data source ID:"))), 1, 2,
      1, 2, Gtk::SHRINK, Gtk::SHRINK);
  InfoTable->attach(*Gtk::manage(new Gtk::Label(Id)), 2, 3, 1, 2, Gtk::SHRINK,
      Gtk::SHRINK);

  mp_UpButton = Gtk::manage(new Gtk::Button());
  mp_UpButton->set_image(*Gtk::manage(new Gtk::Image(Gtk::Stock::GO_UP,
      Gtk::ICON_SIZE_MENU)));
  mp_UpButton->set_relief(Gtk::RELIEF_NONE);
  mp_UpButton->set_tooltip_text(_("Move up the layer"));

  mp_DownButton = Gtk::manage(new Gtk::Button());
  mp_DownButton->set_image(*Gtk::manage(new Gtk::Image(Gtk::Stock::GO_DOWN,
      Gtk::ICON_SIZE_MENU)));
  mp_DownButton->set_relief(Gtk::RELIEF_NONE);
  mp_DownButton->set_tooltip_text(_("Move down the layer"));

  mp_RemoveButton = Gtk::manage(new Gtk::Button());
  mp_RemoveButton->set_image(*Gtk::manage(new Gtk::Image(Gtk::Stock::REMOVE,
      Gtk::ICON_SIZE_MENU)));
  mp_RemoveButton->set_relief(Gtk::RELIEF_NONE);
  mp_RemoveButton->set_tooltip_text(_("Remove the layer"));

  Gtk::Table* ActionTable = Gtk::manage(new Gtk::Table());
  ActionTable->attach(*mp_RemoveButton, 0, 1, 0, 2, Gtk::SHRINK, Gtk::SHRINK);
  ActionTable->attach(*mp_UpButton, 1, 2, 0, 1, Gtk::SHRINK, Gtk::SHRINK);
  ActionTable->attach(*mp_DownButton, 1, 2, 1, 2, Gtk::SHRINK, Gtk::SHRINK);

  mp_MainBox->pack_start(*InfoTable, Gtk::PACK_SHRINK);
  mp_MainBox->pack_end(*ActionTable, Gtk::PACK_SHRINK);
  mp_MainBox->show_all_children();

  //******************Signal connexion*********************

  mp_DisplayLayerCheckBox->signal_toggled().connect(sigc::mem_fun(*this,
      &WidgetObjectBase::onIsDisplayButtonChecked));
  mp_UpButton->signal_clicked().connect(sigc::mem_fun(*this,
      &WidgetObjectBase::onUpLayerButtonClicked));
  mp_DownButton->signal_clicked().connect(sigc::mem_fun(*this,
      &WidgetObjectBase::onDownLayerButtonClicked));
  mp_RemoveButton->signal_clicked().connect(sigc::mem_fun(*this,
      &WidgetObjectBase::onRemoveLayerButtonClicked));
  mp_Eventbox->signal_event().connect(sigc::mem_fun(*this,
      &WidgetObjectBase::onEventHappend));
}

// =====================================================================
// =====================================================================

void WidgetObjectBase::onUpLayerButtonClicked()
{
  m_signal_UpLayerButtonClicked.emit();
}

// =====================================================================
// =====================================================================

void WidgetObjectBase::onDownLayerButtonClicked()
{
  m_signal_DownLayerButtonClicked.emit();
}

// =====================================================================
// =====================================================================

void WidgetObjectBase::onRemoveLayerButtonClicked()
{
  m_signal_RemoveLayerButtonClicked.emit();
}

// =====================================================================
// =====================================================================

void WidgetObjectBase::onIsDisplayButtonChecked()
{
  m_signal_IsDisplayButtonChecked.emit();
}

// =====================================================================
// =====================================================================

bool WidgetObjectBase::onEventHappend(GdkEvent* event)
{
  if (event->type == GDK_ENTER_NOTIFY)
  {
    mp_Eventbox->set_state(Gtk::STATE_ACTIVE);
  }
  else if (event->type == GDK_LEAVE_NOTIFY)
  {
    mp_Eventbox->set_state(m_CurrentState);
  }
  else if (event->type == GDK_BUTTON_PRESS)
  {
    m_signal_IsSelectedLayerClicked.emit();
  }

  return false;
}

// =====================================================================
// =====================================================================

WidgetObjectBase::mtype_SignalWidgetObjectBase WidgetObjectBase::signalUpLayerButtonClicked()
{
  return m_signal_UpLayerButtonClicked;
}

// =====================================================================
// =====================================================================

WidgetObjectBase::mtype_SignalWidgetObjectBase WidgetObjectBase::signalDownLayerButtonClicked()
{
  return m_signal_DownLayerButtonClicked;
}

// =====================================================================
// =====================================================================

WidgetObjectBase::mtype_SignalWidgetObjectBase WidgetObjectBase::signalRemoveLayerButtonClicked()
{
  return m_signal_RemoveLayerButtonClicked;
}

// =====================================================================
// =====================================================================

WidgetObjectBase::mtype_SignalWidgetObjectBase WidgetObjectBase::signalIsDisplayButtonChecked()
{
  return m_signal_IsDisplayButtonChecked;
}

// =====================================================================
// =====================================================================

WidgetObjectBase::mtype_SignalWidgetObjectBase WidgetObjectBase::signalIsSelectedLayerClicked()
{
  return m_signal_IsSelectedLayerClicked;
}

// =====================================================================
// =====================================================================

void WidgetObjectBase::setUpButtonSensitive(bool Sensitiveness)
{
  mp_UpButton->set_sensitive(Sensitiveness);
}

// =====================================================================
// =====================================================================

void WidgetObjectBase::setDownButtonSensitive(bool Sensitiveness)
{
  mp_DownButton->set_sensitive(Sensitiveness);
}

// =====================================================================
// =====================================================================

void WidgetObjectBase::setOGRGeometryType(int ORGeometryType)
{
  mp_TypeImage->set(getPixbufForType(ORGeometryType));
}

// =====================================================================
// =====================================================================

Glib::RefPtr<Gdk::Pixbuf> WidgetObjectBase::getPixbufForType(int ORGeometryType)
{
  Glib::ustring ImageFileName;

  switch (ORGeometryType)
  {
    case 1:
      ImageFileName = "points.png";
      break;
    case 2:
      ImageFileName = "lines.png";
      break;
    case 3:
      ImageFileName = "polygons.png";
      break;
    default:
      ImageFileName = "points.png";
      break;
  }

  return Gdk::Pixbuf::create_from_file(
      BuilderGraphicsHelper::getPathForFileName(ImageFileName), 22, 22);
}

// =====================================================================
// =====================================================================

void WidgetObjectBase::setIsSelected(bool IsSelected)
{
  m_CurrentState = IsSelected ? Gtk::STATE_SELECTED : Gtk::STATE_NORMAL;

  mp_Eventbox->set_state(m_CurrentState);
}

// =====================================================================
// =====================================================================
