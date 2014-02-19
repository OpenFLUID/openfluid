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
 \file PolygonEdge.cpp
 \brief Implements ...

 \author Aline LIBRES <aline.libres@gmail.com>
 */

#include "PolygonEdge.hpp"

#include <openfluid/core/Value.hpp>
#include <openfluid/landr/PolygonEntity.hpp>
#include <openfluid/base/FrameworkException.hpp>
#include <geos/geom/LineString.h>
#include <geos/geom/Polygon.h>
#include <geos/geom/Point.h>
#include <geos/planargraph/DirectedEdge.h>
#include <algorithm>

namespace openfluid {
namespace landr {

PolygonEdge::PolygonEdge(geos::geom::LineString& Line) :
    geos::planargraph::Edge(), m_Line(Line)
{
}

// =====================================================================
// =====================================================================

PolygonEdge::~PolygonEdge()
{
  unsigned int iEnd=dirEdge.size();
  for (unsigned int i = 0; i < iEnd; i++)
    delete dirEdge[i];
}

// =====================================================================
// =====================================================================

geos::geom::LineString* PolygonEdge::getLine()
{
  return &m_Line;
}

// =====================================================================
// =====================================================================

void PolygonEdge::addFace(PolygonEntity& NewFace)
{
  if (!isLineInFace(NewFace))
  {
    std::ostringstream s;
    s << "Can not add Polygon " << NewFace.getOfldId()
      << " as neighbour of this edge, because it doesn't contain edge line.";

    throw openfluid::base::FrameworkException(
                                       "PolygonEdge::addFace", s.str());

    return;
  }

  if (m_Faces.size() > 1)
  {
    std::ostringstream s;
    s << "Can not add Polygon " << NewFace.getOfldId()
      << " as neighbour of this edge, which has already two neighbours.";

    throw openfluid::base::FrameworkException(
                                       "PolygonEdge::addFace", s.str());
    return;
  }

  m_Faces.push_back(&NewFace);
}

// =====================================================================
// =====================================================================

bool PolygonEdge::isLineInFace(PolygonEntity& Face)
{
  return (m_Line.relate(Face.getPolygon(), "F1F"
                        "F*F"
                        "***"));
}

// =====================================================================
// =====================================================================

const std::vector<PolygonEntity*> PolygonEdge::getFaces()
{
  return m_Faces;
}

// =====================================================================
// =====================================================================

void PolygonEdge::removeFace(PolygonEntity* Face)
{
  std::vector<PolygonEntity*>::iterator it = std::find(m_Faces.begin(),
                                                       m_Faces.end(), Face);

  if (it != m_Faces.end())
    m_Faces.erase(it);
}

// =====================================================================
// =====================================================================


bool PolygonEdge::getAttributeValue(const std::string& AttributeName,
                                    core::Value& Value) const
{
  std::map<std::string, core::Value*>::const_iterator it = m_EdgeAttributes.find(
      AttributeName);

  if (it != m_EdgeAttributes.end() && it->second)
  {
    Value = *it->second;
    return true;
  }

  return false;
}

// =====================================================================
// =====================================================================

bool PolygonEdge::setAttributeValue(const std::string& AttributeName,
                                    const core::Value* Value)
{
  std::map<std::string, core::Value*>::const_iterator it = m_EdgeAttributes.find(
      AttributeName);

  if (it != m_EdgeAttributes.end())
  {
    if (it->second)
      delete it->second;
    m_EdgeAttributes[AttributeName] = const_cast<core::Value*>(Value->clone());
    return true;
  }

  return false;
}

// =====================================================================
// =====================================================================

void PolygonEdge::removeAttribute(const std::string& AttributeName)
{

  std::map<std::string, core::Value*>::iterator it = m_EdgeAttributes.find(
      AttributeName);

  if (it != m_EdgeAttributes.end())
  {
    if ((*it).second)
      delete (*it).second;

    m_EdgeAttributes.erase(it);
  }
}

// =====================================================================
// =====================================================================

bool PolygonEdge::isCoincident(PolygonEdge *Edge)
{

  geos::geom::Point *StartPoint=getLine()->getStartPoint();
  geos::geom::Point *EndPoint=getLine()->getEndPoint();

  geos::geom::Point *StartPoint2=Edge->getLine()->getStartPoint();
  geos::geom::Point *EndPoint2=Edge->getLine()->getEndPoint();


  if ((StartPoint->getCoordinate())->equals(*(StartPoint2->getCoordinate()))||
      (StartPoint->getCoordinate())->equals(*(EndPoint2->getCoordinate()))||
      (EndPoint->getCoordinate())->equals(*(StartPoint2->getCoordinate()))||
      (EndPoint->getCoordinate())->equals(*(EndPoint2->getCoordinate())))
    return true;

  return false;
}

// =====================================================================
// =====================================================================

}// namespace landr
} /* namespace openfluid */
