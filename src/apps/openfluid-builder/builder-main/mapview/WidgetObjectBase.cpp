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
#include "BuilderGraphicsHelper.hpp"

#include "WidgetObjectBase.hpp"

WidgetObjectBase::WidgetObjectBase(std::string ClassName, std::string Id) :
  WidgetObject()
{
  mp_CheckButton = Gtk::manage(new Gtk::CheckButton());
  mp_CheckButton->set_active(true);

  Glib::RefPtr<Gdk::Pixbuf> im;

  m_OGRGeometryType = 1;
  m_IsSelected = false;

  im = Gdk::Pixbuf::create_from_file(
      BuilderGraphicsHelper::getPathForFileName("points.png"), 22, 22, true);

  mp_ImageOGRGeometryType = Gtk::manage(new Gtk::Image(im));

  Gtk::Toolbar* tup = Gtk::manage(new Gtk::Toolbar());
  Gtk::Toolbar* tdown = Gtk::manage(new Gtk::Toolbar());

  tup->set_icon_size(Gtk::ICON_SIZE_MENU);
  tdown->set_icon_size(Gtk::ICON_SIZE_MENU);
  tup->set_show_arrow(false);
  tdown->set_show_arrow(false);

  mref_UpLayer = Gtk::Action::create("UpLayer", Gtk::Stock::GO_UP,
      _("Up Layer"), _("Go up the layer"));
  mref_DownLayer = Gtk::Action::create("DownLayer", Gtk::Stock::GO_DOWN,
      _("Down Layer"), _("Go down the layer"));
  mref_RemoveLayer = Gtk::Action::create("RemoveLayer", Gtk::Stock::REMOVE,
      _("Remove Layer"), _("Remove the layer"));

  mp_ButtonUp = Gtk::manage(new Gtk::ToolItem());
  mp_ButtonDown = Gtk::manage(new Gtk::ToolItem());
  mp_ButtonRemove = Gtk::manage(new Gtk::ToolItem());

  mp_ButtonUp = mref_UpLayer->create_tool_item();
  mp_ButtonDown = mref_DownLayer->create_tool_item();
  mp_ButtonRemove = mref_RemoveLayer->create_tool_item();
  m_TitleLayerLabel.set_label(ClassName);
  m_LayerIdLabel.set_label(Id);
  m_TitleLayerLabel.set_alignment(0, 0);
  m_LayerIdLabel.set_alignment(0, 0);

  tup->append(*mp_ButtonUp);
  tdown->append(*mp_ButtonDown);

  mp_MainTable->attach(*mp_CheckButton, 0, 1, 0, 2, Gtk::SHRINK, Gtk::SHRINK);
  mp_MainTable->attach(m_TitleLayerLabel, 1, 2, 0, 1, Gtk::FILL | Gtk::EXPAND,
      Gtk::SHRINK);
  mp_MainTable->attach(m_LayerIdLabel, 1, 2, 1, 2,
      Gtk::FILL | Gtk::EXPAND, Gtk::SHRINK);
  mp_MainTable->attach(*mp_ImageOGRGeometryType, 2, 3, 0, 2, Gtk::SHRINK,
      Gtk::SHRINK);
  mp_MainTable->attach(*mp_ButtonRemove, 3, 4, 0, 2, Gtk::SHRINK, Gtk::SHRINK);
  mp_MainTable->attach(*tup, 4, 5, 0, 1, Gtk::SHRINK, Gtk::SHRINK);
  mp_MainTable->attach(*tdown, 4, 5, 1, 2, Gtk::SHRINK, Gtk::SHRINK);
  mp_MainTable->show_all_children(true);

  //******************Signal connexion*********************

  mp_CheckButton->signal_toggled().connect(
      sigc::mem_fun(*this, &WidgetObjectBase::onIsDisplayButtonChecked));
  mref_UpLayer->signal_activate().connect(
      sigc::mem_fun(*this, &WidgetObjectBase::onUpLayerButtonClicked));
  mref_DownLayer->signal_activate().connect(
      sigc::mem_fun(*this, &WidgetObjectBase::onDownLayerButtonClicked));
  mref_RemoveLayer->signal_activate().connect(
      sigc::mem_fun(*this, &WidgetObjectBase::onRemoveLayerButtonClicked));

  mp_Eventbox->signal_event().connect(
      sigc::mem_fun(*this, &WidgetObjectBase::onEventHappend));
}

// =====================================================================
// =====================================================================
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
    //    double XPress = ((GdkEventMotion *) event)->x;
    //    double YPress = ((GdkEventMotion *) event)->y;

    mp_Eventbox->modify_bg(Gtk::StateType(NULL), Gdk::Color("#66DDFF"));
  }
  if (event->type == GDK_LEAVE_NOTIFY)
  {
    if (!m_IsSelected)
      mp_Eventbox->modify_bg(Gtk::StateType(NULL), Gdk::Color("#F1F2F3"));
    else
      mp_Eventbox->modify_bg(Gtk::StateType(NULL), Gdk::Color("#00FF00"));
  }
  if (event->type == GDK_BUTTON_PRESS)
  {
    m_IsSelected = !m_IsSelected;
    m_signal_IsSelectedLayerClicked.emit();
  }
  return false;
}

// =====================================================================
// =====================================================================
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
// =====================================================================
// =====================================================================


Gtk::ToolItem* WidgetObjectBase::getButtonUp()
{
  return mp_ButtonUp;
}

// =====================================================================
// =====================================================================

Gtk::ToolItem* WidgetObjectBase::getButtonDown()
{
  return mp_ButtonDown;
}

// =====================================================================
// =====================================================================

void WidgetObjectBase::setOGRGeometryType(int ORGeometryType)
{
  m_OGRGeometryType = ORGeometryType;
  setImageOGRGeometryType(m_OGRGeometryType);
}

// =====================================================================
// =====================================================================

void WidgetObjectBase::setImageOGRGeometryType(int ORGeometryType)
{
  Glib::RefPtr<Gdk::Pixbuf> im;
  switch (ORGeometryType)
  {
    case 1:
      im
          = Gdk::Pixbuf::create_from_file(
              BuilderGraphicsHelper::getPathForFileName("points.png"), 22, 22,
              true);
      break;
    case 2:
      im = Gdk::Pixbuf::create_from_file(
          BuilderGraphicsHelper::getPathForFileName("lines.png"), 22, 22, true);
      break;
    case 3:
      im = Gdk::Pixbuf::create_from_file(
          BuilderGraphicsHelper::getPathForFileName("polygons.png"), 22, 22,
          true);
      break;
    default:
      break;
  }
  mp_ImageOGRGeometryType->set(im);
}

// =====================================================================
// =====================================================================

void WidgetObjectBase::setIsSelected(bool IsSelected)
{
  m_IsSelected = IsSelected;
  if (!m_IsSelected)
    mp_Eventbox->modify_bg(Gtk::StateType(NULL), Gdk::Color("#F1F2F3"));
  else
    mp_Eventbox->modify_bg(Gtk::StateType(NULL), Gdk::Color("#00FF00"));
}

// =====================================================================
// =====================================================================
