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
  @file PolygonGraphics.cpp

  @author Jean-Christophe FABRE <jean-christophe.fabre@inra.fr>
*/


#include <QBrush>

#include "PolygonGraphics.hpp"


namespace openfluid { namespace ui { namespace spatial {


PolygonGraphics::PolygonGraphics(OGRPolygon* OGRPoly, const QPen& Pen, const QBrush& Brush):
  SurfacicGraphics(Brush.color())
{
  OGRLinearRing* LinearRing = OGRPoly->getExteriorRing();

  QPolygonF Poly;

  for (int i=0; i < LinearRing->getNumPoints(); i++)
  {
    Poly << QPointF(LinearRing->getX(i),LinearRing->getY(i));
  }

  QPainterPath Path;
  Path.addPolygon(Poly);

  setPen(Pen);
  setBrush(Brush);

  setPath(Path);
}

} } } // namespaces
