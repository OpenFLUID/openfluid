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
 \file LineStringEntity.cpp
 \brief Implements ...

 \author Aline LIBRES <aline.libres@gmail.com>
 */

#include "LineStringEntity.hpp"

#include <geos/geom/LineString.h>
#include <geos/geom/Point.h>
#include <geos/planargraph/DirectedEdge.h>
#include <geos/planargraph/Node.h>
#include <openfluid/base/FrameworkException.hpp>

namespace openfluid {
namespace landr {

// =====================================================================
// =====================================================================

LineStringEntity::LineStringEntity(const geos::geom::Geometry* NewLine,
                                   unsigned int OfldId) :
    LandREntity(NewLine, OfldId), geos::planargraph::Edge(), mp_LOUpNeighbours(
        0), mp_LODownNeighbours(0)
{
  if (mp_Geom->getGeometryTypeId() != geos::geom::GEOS_LINESTRING)
    throw openfluid::base::FrameworkException(
        "LineStringEntity::LineStringEntity",
        "Geometry is not a LineString.");

  mp_Line =
      dynamic_cast<geos::geom::LineString*>(const_cast<geos::geom::Geometry*>(mp_Geom));

  if (mp_Line->isEmpty())
  {
    delete mp_Centroid;

    throw openfluid::base::FrameworkException(
                                       "LineStringEntity::LineStringEntity",
                                       "The LineString is empty");
  }
}

// =====================================================================
// =====================================================================

LineStringEntity::~LineStringEntity()
{
  unsigned int iEnd=dirEdge.size();
  for (unsigned int i = 0; i < iEnd; i++)
    delete dirEdge[i];

  delete mp_LOUpNeighbours;
  delete mp_LODownNeighbours;
}

// =====================================================================
// =====================================================================

LineStringEntity* LineStringEntity::clone()
{
  return new LineStringEntity(mp_Geom->clone(), m_OfldId);
}

// =====================================================================
// =====================================================================

const geos::geom::LineString* LineStringEntity::getLine() const
{
  return mp_Line;
}

// =====================================================================
// =====================================================================

geos::planargraph::Node* LineStringEntity::getStartNode()
{
  return getDirEdge(0)->getFromNode();
}

// =====================================================================
// =====================================================================

geos::planargraph::Node* LineStringEntity::getEndNode()
{
  return getDirEdge(0)->getToNode();
}

// =====================================================================
// =====================================================================

void LineStringEntity::computeNeighbours()
{
  std::vector<LineStringEntity*> Ups = getLineOrientUpNeighbours();
  std::vector<LineStringEntity*> Downs = getLineOrientDownNeighbours();

  delete mp_Neighbours;

  mp_Neighbours = new std::set<LandREntity*>;

  geos::planargraph::DirectedEdgeStar* UpStar = getStartNode()->getOutEdges();
  geos::planargraph::DirectedEdgeStar* DownStar = getEndNode()->getOutEdges();

  std::vector<geos::planargraph::DirectedEdge*>::iterator it=UpStar->iterator();
  std::vector<geos::planargraph::DirectedEdge*>::iterator ite=UpStar->end();
  for (; it != ite; ++it)
  {
    LandREntity* Ent = dynamic_cast<LandREntity*>((*it)->getEdge());

    if (Ent != this)
      mp_Neighbours->insert(Ent);
  }
  it = DownStar->iterator();
  ite = DownStar->end();
  for (; it != ite; ++it)
  {
    LandREntity* Ent = dynamic_cast<LandREntity*>((*it)->getEdge());
    if (Ent != this)
      mp_Neighbours->insert(Ent);
  }

}

// =====================================================================
// =====================================================================

std::vector<LineStringEntity*> LineStringEntity::getLineOrientUpNeighbours()
{
  if (!mp_LOUpNeighbours)
    computeLineOrientUpNeighbours();

  return *mp_LOUpNeighbours;
}

// =====================================================================
// =====================================================================

void LineStringEntity::computeLineOrientUpNeighbours()
{
  mp_LOUpNeighbours = new std::vector<LineStringEntity*>();

  geos::planargraph::DirectedEdgeStar* UpStar = getStartNode()->getOutEdges();

  geos::geom::Coordinate UpNodeCoo = getStartNode()->getCoordinate();

  std::vector<geos::planargraph::DirectedEdge*>::iterator it =
        UpStar->iterator();
  std::vector<geos::planargraph::DirectedEdge*>::iterator ite =
        UpStar->end();
  for (; it != ite; ++it)
  {
    LineStringEntity* Unit = dynamic_cast<LineStringEntity*>((*it)->getEdge());

    if (Unit->getEndNode()->getCoordinate().equals(UpNodeCoo))
      mp_LOUpNeighbours->push_back(Unit);
  }
}

// =====================================================================
// =====================================================================

std::vector<LineStringEntity*> LineStringEntity::getLineOrientDownNeighbours()
{
  if (!mp_LODownNeighbours)
    computeLineOrientDownNeighbours();

  return *mp_LODownNeighbours;
}

// =====================================================================
// =====================================================================

void LineStringEntity::computeLineOrientDownNeighbours()
{
  mp_LODownNeighbours = new std::vector<LineStringEntity*>();

  geos::planargraph::DirectedEdgeStar* DownStar = getEndNode()->getOutEdges();

  geos::geom::Coordinate DownNodeCoo = getEndNode()->getCoordinate();

  std::vector<geos::planargraph::DirectedEdge*>::iterator it =
        DownStar->iterator();
  std::vector<geos::planargraph::DirectedEdge*>::iterator ite =
        DownStar->end();
  for (; it != ite; ++it)
  {
    LineStringEntity* Unit = dynamic_cast<LineStringEntity*>((*it)->getEdge());

    if (Unit->getStartNode()->getCoordinate().equals(DownNodeCoo))
      mp_LODownNeighbours->push_back(Unit);
  }
}

// =====================================================================
// =====================================================================

}// namespace landr
} /* namespace openfluid */
