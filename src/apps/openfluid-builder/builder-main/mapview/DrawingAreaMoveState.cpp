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
 \file DrawingAreaMoveState.cpp
 \brief Implements ...

 \author Damien CHABBERT <dams.vivien@gmail.com>
 */

#include "DrawingAreaMoveState.hpp"

#include "BuilderGraphicsHelper.hpp"

DrawingAreaMoveState::DrawingAreaMoveState(DrawingArea& DrawingArea) :
  DrawingAreaState(DrawingArea)
{
  m_DefaultCursor = Gdk::Cursor(mref_DrawingArea.get_display(),Gdk::Pixbuf::create_from_file(
      BuilderGraphicsHelper::getPathForFileName("hand1.png"),24,24),12,12);

  m_MovingCursor = Gdk::Cursor(mref_DrawingArea.get_display(),Gdk::Pixbuf::create_from_file(
      BuilderGraphicsHelper::getPathForFileName("grabbing.png"),24,24),12,12);

  m_Cursor = m_DefaultCursor;
}

// =====================================================================
// =====================================================================

void DrawingAreaMoveState::onMouseButtonPressed(GdkEventButton* event)
{
  m_XPress = event->x / mref_DrawingArea.getScale();
  m_YPress = event->y / mref_DrawingArea.getScale();

  m_Cursor = m_MovingCursor;
  mref_DrawingArea.get_window()->set_cursor(m_Cursor);
}

// =====================================================================
// =====================================================================

bool DrawingAreaMoveState::onMouseButtonReleased(GdkEventButton* event)
{
  double XRelease = event->x / mref_DrawingArea.getScale();
  double YRelease = event->y / mref_DrawingArea.getScale();

  double XPoint = mref_DrawingArea.getXTranslate() + (m_XPress - XRelease);
  double YPoint = mref_DrawingArea.getYTranslate() + (YRelease - m_YPress);

  mref_DrawingArea.setXTranslate(XPoint);
  mref_DrawingArea.setYTranslate(YPoint);

  m_Cursor = m_DefaultCursor;
  mref_DrawingArea.get_window()->set_cursor(m_Cursor);

  return true;
}
