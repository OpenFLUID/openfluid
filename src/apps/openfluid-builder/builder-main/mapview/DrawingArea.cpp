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
 \file DrawingArea.cpp
 \brief Implements ...

 \author Damien CHABBERT <dams.vivien@gmail.com>
 */
#include <iostream>
#include <gtkmm/window.h>

#include "DrawingAreaInitialState.hpp"
#include "DrawingAreaMoveState.hpp"
#include "DrawingAreaSelectState.hpp"
#include "DrawingAreaUnzoomCursorState.hpp"
#include "DrawingAreaZoomCursorState.hpp"
#include "DrawingAreaZoomFrameState.hpp"
#include "DrawingAreaState.hpp"

#include "DrawingArea.hpp"

//#include "BuilderAppWindow.hpp"

DrawingArea::DrawingArea()
{
  //    set_flags(get_flags() | Gtk::CAN_FOCUS);
  //  set_can_focus(true);

  m_XTranslate = 0;
  m_YTranslate = 0;
  m_Scale = 1;

  m_pressMultiSelect = false;
  m_LayerExist = false;

  modify_bg(Gtk::StateType(NULL), Gdk::Color("#FFFFFF"));

  //states
  mp_InitialState = new DrawingAreaInitialState(*this);
  mp_MoveState = new DrawingAreaMoveState(*this);
  mp_SelectState = new DrawingAreaSelectState(*this);
  mp_UnzoomCursorState = new DrawingAreaUnzoomCursorState(*this);
  mp_ZoomCursorState = new DrawingAreaZoomCursorState(*this);
  mp_ZoomFrameState = new DrawingAreaZoomFrameState(*this);

  mp_CurrentState = dynamic_cast<DrawingAreaState*> (mp_InitialState);

  this->add_events(Gdk::POINTER_MOTION_MASK);
  this->add_events(Gdk::LEAVE_NOTIFY_MASK);
  this->add_events(Gdk::ENTER_NOTIFY_MASK);
  this->add_events(Gdk::BUTTON_PRESS_MASK);
  this->add_events(Gdk::BUTTON_RELEASE_MASK);
  //cairomm
}

// =====================================================================
// =====================================================================

//DrawingArea::~DrawingArea()
//{
//
//}

// =====================================================================
// =====================================================================

//TODO arranger l'expose event
bool DrawingArea::on_expose_event(GdkEventExpose* /*e*/)
{
  m_signal_ExposeEventChanged.emit();
  return true;
}

//void DrawingArea::on_realize()
//{
////  Gtk::Window& window = dynamic_cast<Gtk::Window&>(*get_toplevel());
////  window.signal_key_press_event().connect(
////      sigc::mem_fun(*this, &DrawingArea::whenOnKeyPressed));
////  window.signal_key_release_event().connect(
////        sigc::mem_fun(*this, &DrawingArea::whenOnKeyPressed));
//}
// =====================================================================
// =====================================================================

sigc::signal<void> DrawingArea::signal_ExposeEventChanged()
{
  return m_signal_ExposeEventChanged;
}

// =====================================================================
// =====================================================================

sigc::signal<void, double, double> DrawingArea::signal_CoordinateChanged()
{
  return m_signal_CoordinateChanged;
}

// =====================================================================
// =====================================================================

sigc::signal<void, double, double> DrawingArea::signal_CoordinateSelected()
{
  return m_signal_CoordinateSelected;
}

// =====================================================================
// =====================================================================

void DrawingArea::changeToInitialState()
{
  m_KeyPress.disconnect();
  m_KeyRelease.disconnect();
  m_pressMultiSelect = false;
  mp_CurrentState = dynamic_cast<DrawingAreaState*> (mp_InitialState);
}

// =====================================================================
// =====================================================================

void DrawingArea::changeToMoveState()
{
  mp_CurrentState = dynamic_cast<DrawingAreaState*> (mp_MoveState);
}

// =====================================================================
// =====================================================================

void DrawingArea::changeToSelectState()
{
  m_KeyPress =dynamic_cast<Gtk::Window&> (*get_toplevel()).signal_key_press_event().connect(
      sigc::mem_fun(*this, &DrawingArea::whenOnKeyPressed));
  m_KeyRelease = dynamic_cast<Gtk::Window&> (*get_toplevel()).signal_key_release_event().connect(
      sigc::mem_fun(*this, &DrawingArea::whenOnKeyReleased));
  mp_CurrentState = dynamic_cast<DrawingAreaState*> (mp_SelectState);
}

// =====================================================================
// =====================================================================

void DrawingArea::changeToUnzoomCursorState()
{
  mp_CurrentState = dynamic_cast<DrawingAreaState*> (mp_UnzoomCursorState);
}

// =====================================================================
// =====================================================================

void DrawingArea::changeToZoomCursorState()
{
  mp_CurrentState = dynamic_cast<DrawingAreaState*> (mp_ZoomCursorState);
}

// =====================================================================
// =====================================================================

void DrawingArea::changeToZoomFrameState()
{
  mp_CurrentState = dynamic_cast<DrawingAreaState*> (mp_ZoomFrameState);
}

// =====================================================================
// =====================================================================
// =====================================================================
// =====================================================================

void DrawingArea::modifyScaleTranslate(
    std::pair<std::pair<double, double>, std::pair<double, double> > MinMax)
{
  Gtk::Allocation allocation = get_allocation();
  double Width = allocation.get_width();
  double Height = allocation.get_height();

  double Scale = std::min(Width / (MinMax.second.first - MinMax.first.first),
      Height / (MinMax.second.second - MinMax.first.second));

  setScale(Scale);
  setXTranslate(MinMax.first.first);
  setYTranslate(MinMax.second.second);
}

// =====================================================================
// =====================================================================

// =====================================================================
// =====================================================================

bool DrawingArea::on_button_press_event(GdkEventButton* event)
{
  mp_CurrentState->onMouseButtonPressed(event);
  if (mp_CurrentState == mp_SelectState)
  {
    m_signal_CoordinateSelected.emit(
        (static_cast<DrawingAreaSelectState*> (mp_CurrentState))->getXPress(),
        (static_cast<DrawingAreaSelectState*> (mp_CurrentState))->getYPress());
  } else if (mp_CurrentState == mp_ZoomCursorState || mp_CurrentState
      == mp_UnzoomCursorState)
  {
    m_signal_ExposeEventChanged.emit();
  }
  return false;
}

// =====================================================================
// =====================================================================

bool DrawingArea::whenOnKeyPressed(GdkEventKey* event)
{
  std::cout << "key pressed ok" << std::endl;
  if (event->keyval == 0xFFE3)
    m_pressMultiSelect = true;
  return false;
}

// =====================================================================
// =====================================================================

bool DrawingArea::whenOnKeyReleased(GdkEventKey* event)
{
  std::cout << "key released ok" << std::endl;
  if (event->keyval == 0xFFE3)
    m_pressMultiSelect = false;
  return false;
}

// =====================================================================
// =====================================================================

bool DrawingArea::on_button_release_event(GdkEventButton* event)
{
  mp_CurrentState->onMouseButtonReleased(event);
  if (mp_CurrentState == mp_ZoomFrameState || mp_CurrentState == mp_MoveState)
  {
    m_signal_ExposeEventChanged.emit();
  }
  return false;
}

// =====================================================================
// =====================================================================

bool DrawingArea::on_enter_notify_event(GdkEventCrossing* /*event*/)
{
  get_window()->set_cursor(mp_CurrentState->getCursor());
  return false;
}

// =====================================================================
// =====================================================================


bool DrawingArea::on_leave_notify_event(GdkEventCrossing* /*event*/)
{
  get_window()->set_cursor(mp_InitialState->getCursor());
  return false;
}

// =====================================================================
// =====================================================================

bool DrawingArea::on_motion_notify_event(GdkEventMotion* event)
{
  double XPress = event->x / getScale();
  double YPress = event->y / getScale();

  XPress = getXTranslate() + XPress;
  YPress = getYTranslate() - YPress;
  m_signal_CoordinateChanged.emit(XPress, YPress);
  return false;
}

// =====================================================================
// =====================================================================
// =====================================================================
// =====================================================================

double DrawingArea::getScale()
{
  return m_Scale;
}

// =====================================================================
// =====================================================================

double DrawingArea::getXTranslate()
{
  return m_XTranslate;
}

// =====================================================================
// =====================================================================

double DrawingArea::getYTranslate()
{
  return m_YTranslate;
}

// =====================================================================
// =====================================================================

bool DrawingArea::getPressMultiSelect()
{
  return m_pressMultiSelect;
}

// =====================================================================
// =====================================================================

void DrawingArea::setScale(double Scale)
{
  m_Scale = Scale;
}

// =====================================================================
// =====================================================================

void DrawingArea::setXTranslate(double XTranslate)
{
  m_XTranslate = XTranslate;
}

// =====================================================================
// =====================================================================

void DrawingArea::setYTranslate(double YTranslate)
{
  m_YTranslate = YTranslate;
}

// =====================================================================
// =====================================================================

void DrawingArea::setLayerExist(bool LayerExist)
{
  m_LayerExist = LayerExist;
}
