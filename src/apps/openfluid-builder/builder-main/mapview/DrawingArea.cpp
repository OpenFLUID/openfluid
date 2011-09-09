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

#include "DrawingAreaInitialState.hpp"
#include "DrawingAreaMoveState.hpp"
#include "DrawingAreaSelectState.hpp"
#include "DrawingAreaUnzoomCursorState.hpp"
#include "DrawingAreaZoomCursorState.hpp"
#include "DrawingAreaZoomFrameState.hpp"
#include "DrawingAreaState.hpp"

#include "DrawingArea.hpp"

DrawingArea::DrawingArea()
{
  m_XTranslate = 0;
  m_YTranslate = 0;
  m_Scale = 0;

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

  this->signal_event().connect(sigc::mem_fun(*this, &DrawingArea::onAllEvents));

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

// =====================================================================
// =====================================================================

sigc::signal<void> DrawingArea::signal_ExposeEventChanged()
{
  return m_signal_ExposeEventChanged;
}

// =====================================================================
// =====================================================================

void DrawingArea::changeToInitialState()
{
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

//void DrawingArea::modifyScaleTranslate(double x, double y)
//{
////  Gtk::Allocation allocation = get_allocation();
////
////  double Width = allocation.get_width();
////  double Height = allocation.get_height();
//
////TODO a verifier la suite
//
//  double Scale = Scale * 2;
//
//  x = x * Scale;
//  y = y * Scale;
//
//  setScale(Scale);
//  setXTranslate(-m_XTranslate + x);
//  setYTranslate(-m_YTranslate + y);
//
//}

// =====================================================================
// =====================================================================
// =====================================================================
// =====================================================================

void DrawingArea::onMouseEnterNotify(GdkEvent* event)
{
  if (event->type == GDK_ENTER_NOTIFY)
  {
    get_window()->set_cursor(mp_CurrentState->getCursor());
  }
}

// =====================================================================
// =====================================================================

void DrawingArea::onMouseLeaveNotify(GdkEvent* event)
{
  if (event->type == GDK_LEAVE_NOTIFY)
  {
    get_window()->set_cursor(mp_InitialState->getCursor());
  }
}

// =====================================================================
// =====================================================================

void DrawingArea::onMouseMotionNotify(GdkEvent* event)
{
  if (event->type == GDK_MOTION_NOTIFY)
  {

  }
}

// =====================================================================
// =====================================================================

void DrawingArea::onMouseButtonPressed(GdkEvent* event)
{
  if (event->type == GDK_BUTTON_PRESS)
  {
    mp_CurrentState->onMouseButtonPressed(event);
    m_signal_ExposeEventChanged.emit();
  }
}

// =====================================================================
// =====================================================================

void DrawingArea::onMouseButtonReleased(GdkEvent* event)
{
  if (event->type == GDK_BUTTON_RELEASE)
  {
    if (mp_CurrentState->onMouseButtonReleased(event))
      m_signal_ExposeEventChanged.emit();
  }
}

bool DrawingArea::onAllEvents(GdkEvent* event)
{
  if (m_LayerExist != false)
  {
    onMouseButtonPressed(event);
    onMouseButtonReleased(event);
    onMouseEnterNotify(event);
    onMouseLeaveNotify(event);
    onMouseMotionNotify(event);
  }
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
