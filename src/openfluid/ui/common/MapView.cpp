/*

  This file is part of OpenFLUID software
  Copyright(c) 2007, INRA - Montpellier SupAgro


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
  along with OpenFLUID. If not, see <http://www.gnu.org/licenses/>.


 == Other Usage ==

  Other Usage means a use of OpenFLUID that is inconsistent with the GPL
  license, and requires a written agreement between You and INRA.
  Licensees for Other Usage of OpenFLUID may use this file in accordance
  with the terms contained in the written agreement between You and INRA.
  
*/

/**
  @file MapView.cpp

  @author Jean-Christophe FABRE <jean-christophe.fabre@inra.fr>
*/


#include <QWheelEvent>

#include "MapView.hpp"


openfluid::ui::common::MapView::MapView(QWidget* Parent):
  QGraphicsView(Parent), m_AutomaticViewEnabled(true)
{
  setRenderHints(QPainter::TextAntialiasing | QPainter::Antialiasing);
  scale(1.0,-1.0);
}


// =====================================================================
// =====================================================================


void openfluid::ui::common::MapView::wheelEvent(QWheelEvent* Event)
{
  if (Event->modifiers().testFlag(Qt::ControlModifier)) // zoom only when Ctrl key is pressed
  {
    emit automaticViewEnabled(false);

    // TODO zoom center on mouse cursor
    if (Event->angleDelta().y() < 0)
    {
      scale(0.9,0.9);
    }
    else
    {
      scale(1.1,1.1);
    }
    emit scaling();
  }
  else
  {
    QGraphicsView::wheelEvent(Event);
  }
}


// =====================================================================
// =====================================================================


void openfluid::ui::common::MapView::resizeEvent(QResizeEvent *Event)
{
  if (m_AutomaticViewEnabled)
  {
    fitViewToItems();
  }

  QGraphicsView::resizeEvent(Event);
}


// =====================================================================
// =====================================================================


void openfluid::ui::common::MapView::enableAutomaticView(bool Enabled)
{
  m_AutomaticViewEnabled = Enabled;

  if (m_AutomaticViewEnabled)
  {
    fitViewToItems();
  }
}


// =====================================================================
// =====================================================================


void openfluid::ui::common::MapView::fitViewToItems()
{
  fitInView(scene()->itemsBoundingRect(),Qt::KeepAspectRatio);
  emit scaling();
}
