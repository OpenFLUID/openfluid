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
  @file PunctualGraphics.cpp

  @author Jean-Christophe FABRE <jean-christophe.fabre@inra.fr>
*/


#include <QPen>
#include <QBrush>

#include "PunctualGraphics.hpp"


constexpr unsigned int POINT_OFFSET = 3;


void openfluid::ui::common::PunctualGraphics::drawPoint(QPainterPath& Path, const OGRPoint* OGRPt)
{
  // TODO to be replaced by a real dot (ellipse)
  //  draw a cross
  Path.moveTo(OGRPt->getX()-POINT_OFFSET,OGRPt->getY());
  Path.lineTo(OGRPt->getX()+POINT_OFFSET,OGRPt->getY());
  Path.moveTo(OGRPt->getX(),OGRPt->getY()-POINT_OFFSET);
  Path.lineTo(OGRPt->getX(),OGRPt->getY()+POINT_OFFSET);
}


// =====================================================================
// =====================================================================


QVariant openfluid::ui::common::PunctualGraphics::itemChange(GraphicsItemChange Change, const QVariant& Value)
{
  if (Change == QGraphicsItem::ItemSelectedHasChanged)
  {
    QPen CurrentPen = pen();

    if (isSelected())
    {
      CurrentPen.setColor(m_SelectionColor);
    }
    else
    {
      CurrentPen.setColor(m_MainColor);
    }

    setPen(CurrentPen);
  }
  return MapItemGraphics::itemChange(Change, Value);
}

