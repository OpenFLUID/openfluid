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
 \file MapViewTreeLayerObject.cpp
 \brief Implements ...

 \author Damien CHABBERT <dams.vivien@gmail.com>
 */

#include "MapViewTreeLayerObject.hpp"

MapViewTreeLayerObject::MapViewTreeLayerObject(const Glib::ustring & Label,
    int IdPositionInit)
{
  m_Id = IdPositionInit;
  //  m_Position = IdPositionInit;
  mp_MainVBox = Gtk::manage(new Gtk::VBox());
  mp_MainTable = Gtk::manage(new Gtk::Table(1, 5, false));
  mp_Expander = Gtk::manage(new Gtk::Expander());
  //  mp_VBoxButtonUpDown = Gtk::manage(new Gtk::VBox());
  mp_Eventbox = Gtk::manage(new Gtk::EventBox());
  m_LayerLabel.set_label(Label);
  mp_CheckButton = Gtk::manage(new Gtk::CheckButton());

  mp_ScrolledWindowExpander = Gtk::manage(new Gtk::ScrolledWindow());
  mp_ViewportExpander = Gtk::manage(
      new Gtk::Viewport(*manage(new Gtk::Adjustment(0, 0, 1)),
          *manage(new Gtk::Adjustment(0, 0, 1))));
  mp_TableExpander = Gtk::manage(new Gtk::Table());

  m_IsDisplay = true;
  m_IsSelected = false;

  mp_CheckButton->set_active(m_IsDisplay);

  m_LayerLabel.set_alignment(0, 0);

  mp_Expander->set_label("Layer Options");

  mp_ScrolledWindowExpander->add(*mp_ViewportExpander);
  mp_Expander->add(*mp_ScrolledWindowExpander);

  mp_ScrolledWindowExpander->set_policy(Gtk::POLICY_NEVER, Gtk::POLICY_NEVER);

  mp_MainTable->attach(*mp_CheckButton, 0, 1, 0, 1, Gtk::SHRINK, Gtk::SHRINK);
  mp_MainTable->attach(m_LayerLabel, 1, 2, 0, 1, Gtk::FILL | Gtk::EXPAND,
      Gtk::SHRINK);
  mp_Eventbox->add(*mp_MainTable);
  Gtk::HSeparator* seperator = Gtk::manage(new Gtk::HSeparator());
  mp_MainVBox->pack_start(*mp_Eventbox, Gtk::PACK_SHRINK);
  if (m_Id != -2)
  {
    mp_MainVBox->pack_end(*seperator, Gtk::PACK_SHRINK);
  }
  mp_MainVBox->pack_end(*mp_Expander, Gtk::PACK_SHRINK);

  //  mp_MainVBox->add_events(Gdk::BUTTON_PRESS_MASK);
  //  mp_MainVBox->add_events(Gdk::POINTER_MOTION_MASK);
  //  mp_MainVBox->add_events(Gdk::BUTTON_RELEASE_MASK);
  mp_Eventbox->signal_event().connect(
      sigc::mem_fun(*this, &MapViewTreeLayerObject::on_event_happend));
  mp_Eventbox->modify_bg(Gtk::StateType(NULL), Gdk::Color("#F1F1F1"));

}

// =====================================================================
// =====================================================================

MapViewTreeLayerObject::~MapViewTreeLayerObject()
{

}

// =====================================================================
// =====================================================================

Gtk::Widget* MapViewTreeLayerObject::asWidget()
{
  return mp_MainVBox;
}

// =====================================================================
// =====================================================================

Glib::ustring MapViewTreeLayerObject::getLabel()
{
  return m_LayerLabel.get_label();
}

// =====================================================================
// =====================================================================

void MapViewTreeLayerObject::setLabel(Glib::ustring label)
{
  m_LayerLabel.set_label(label);
}

// =====================================================================
// =====================================================================

int MapViewTreeLayerObject::getId()
{
  return m_Id;
}

// =====================================================================
// =====================================================================

bool MapViewTreeLayerObject::on_event_happend(GdkEvent* event)
{
  if (event->type == GDK_ENTER_NOTIFY)
  {
    double XPress = ((GdkEventMotion *) event)->x;
    double YPress = ((GdkEventMotion *) event)->y;
    std::cout << "motion: X= " << XPress << " Y= " << YPress << std::endl;
    mp_Eventbox->modify_bg(Gtk::StateType(NULL), Gdk::Color("#66DDFF"));

  }
  if (event->type == GDK_LEAVE_NOTIFY)
  {
    //      double XPress = ((GdkEventMotion *) event)->x;
    //      double YPress = ((GdkEventMotion *) event)->y;
    //      std::cout << "motion: X= " << XPress << " Y= " << YPress << std::endl;
    if (!m_IsSelected)
      mp_Eventbox->modify_bg(Gtk::StateType(NULL), Gdk::Color("#F1F1F1"));
    else
      mp_Eventbox->modify_bg(Gtk::StateType(NULL), Gdk::Color("#00FF00"));
  }
  if (event->type == GDK_BUTTON_PRESS)
  {
    onIsSelected();
  }
  //  if (event->type == GDK_BUTTON_RELEASE)
  //  {
  //    double XPress = event->button.x;
  //    double YPress = event->button.y;
  //    std::cout << "Release: X= " << XPress << " Y= " << YPress << std::endl;
  //  }
  return false;
}

// =====================================================================
// =====================================================================

MapViewTreeLayerObject::mtype_SignalSelected MapViewTreeLayerObject::signalSelected()
{
  return msigc_signalSelected;
}

// =====================================================================
// =====================================================================

bool MapViewTreeLayerObject::getIsSelected()
{
  return m_IsSelected;
}

// =====================================================================
// =====================================================================

void MapViewTreeLayerObject::setIsSelected(bool IsSelected)
{
  m_IsSelected = IsSelected;
}

// =====================================================================
// =====================================================================

void MapViewTreeLayerObject::onIsSelected()
{
  msigc_signalSelected.emit(m_Id, m_IsSelected);
  std::cout << "lala" << std::endl;
}

// =====================================================================
// =====================================================================

Gtk::EventBox* MapViewTreeLayerObject::getEventbox()
{
  return mp_Eventbox;
}

// =====================================================================
// =====================================================================
