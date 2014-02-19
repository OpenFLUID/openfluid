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
 \file PolygonEntity.cpp
 \brief Implements ...

 \author Aline LIBRES <aline.libres@gmail.com>
 */

#include "PolygonEntity.hpp"

#include <openfluid/landr/PolygonEdge.hpp>
#include <openfluid/landr/LandRTools.hpp>
#include <openfluid/landr/LineStringEntity.hpp>
#include <openfluid/landr/LineStringGraph.hpp>
#include <openfluid/base/FrameworkException.hpp>
#include <geos/geom/Polygon.h>
#include <geos/geom/LineString.h>
#include <geos/geom/MultiLineString.h>
#include <geos/geom/GeometryFactory.h>
#include <geos/geom/Point.h>
#include <geos/planargraph/Node.h>
#include <geos/planargraph/DirectedEdge.h>
#include <algorithm>

namespace openfluid {
namespace landr {

// =====================================================================
// =====================================================================

PolygonEntity::PolygonEntity(const geos::geom::Geometry* NewPolygon,
                             unsigned int OfldId) :
    LandREntity(NewPolygon, OfldId), mp_NeighboursMap(0), mp_LineStringNeighboursMap(
        0)
{
  if (mp_Geom->getGeometryTypeId() != geos::geom::GEOS_POLYGON)
  {
    delete mp_Centroid;

    throw openfluid::base::FrameworkException(
                                       "PolygonEntity::PolygonEntity",
                                       "Geometry is not a Polygon.");

  }

  mp_Polygon =
      dynamic_cast<geos::geom::Polygon*>(const_cast<geos::geom::Geometry*>(mp_Geom));

  if (!mp_Polygon->isValid())
  {
    delete mp_Centroid;

    throw openfluid::base::FrameworkException(
                                       "PolygonEntity::PolygonEntity",
                                       "Polygon is not valid.");
  }

}

// =====================================================================
// =====================================================================

PolygonEntity::~PolygonEntity()
{
  delete mp_NeighboursMap;
  delete mp_LineStringNeighboursMap;
}

// =====================================================================
// =====================================================================

PolygonEntity* PolygonEntity::clone()
{
  PolygonEntity* Clone = new PolygonEntity(mp_Geom->clone(), m_OfldId);

  Clone->m_PolyEdges = m_PolyEdges;

  return Clone;
}

// =====================================================================
// =====================================================================

const geos::geom::Polygon* PolygonEntity::getPolygon() const
{
  return mp_Polygon;
}

// =====================================================================
// =====================================================================

void PolygonEntity::addEdge(PolygonEdge& Edge)
{
  Edge.addFace(*this);

  m_PolyEdges.push_back(&Edge);

  mp_NeighboursMap = 0;
}

// =====================================================================
// =====================================================================

void PolygonEntity::removeEdge(PolygonEdge* Edge)
{
  std::vector<PolygonEdge*>::iterator itEdge = std::find(m_PolyEdges.begin(),
                                                         m_PolyEdges.end(),
                                                         Edge);

  if (itEdge != m_PolyEdges.end())
    m_PolyEdges.erase(itEdge);
  else
    throw openfluid::base::FrameworkException(
                                       "PolygonEntity::removeEdge",
                                       "Edge doesn't exist in Edge vector.");

  mp_NeighboursMap = 0;

  delete Edge;
}

// =====================================================================
// =====================================================================

std::vector<geos::geom::LineString*> PolygonEntity::getLineIntersectionsWith(
    PolygonEntity& Other)
{
  std::vector<geos::geom::LineString*> Lines;

  if (mp_Polygon->relate(Other.mp_Polygon, "FFT"
                         "F1*"
                         "***"))
  {
    geos::geom::Geometry* SharedGeom = mp_Polygon->intersection(
        Other.mp_Polygon);

    Lines = *LandRTools::getMergedLineStringsFromGeometry(SharedGeom);

    delete SharedGeom;
  }

  return Lines;
}

// =====================================================================
// =====================================================================

PolygonEdge* PolygonEntity::findEdgeLineIntersectingWith(
    geos::geom::LineString& Segment)
{
  std::vector<PolygonEdge*>::iterator it = m_PolyEdges.begin();
  std::vector<PolygonEdge*>::iterator ite = m_PolyEdges.end();
  for (; it != ite; ++it)
  {
    if (Segment.relate((*it)->getLine(), "1**"
                       "***"
                       "***"))
      return *it;
  }

  return (PolygonEdge*) 0;
}

// =====================================================================
// =====================================================================

const PolygonEntity::NeighboursMap_t* PolygonEntity::getNeighboursAndEdges()
{
  if (!mp_NeighboursMap)
    computeNeighbours();

  return mp_NeighboursMap;
}

// =====================================================================
// =====================================================================

std::vector<int> PolygonEntity::getOrderedNeighbourOfldIds()
{
  std::vector<int> Ids;

  if (!mp_NeighboursMap)
    computeNeighbours();

  NeighboursMap_t::iterator it = mp_NeighboursMap->begin();
  NeighboursMap_t::iterator ite = mp_NeighboursMap->end();
  for (; it != ite; ++it)
    Ids.push_back(it->first->getOfldId());

  std::sort(Ids.begin(), Ids.end());

  return Ids;
}

// =====================================================================
// =====================================================================

void PolygonEntity::computeNeighbours()
{
  delete mp_Neighbours;
  mp_Neighbours = new std::set<LandREntity*>;

  delete mp_NeighboursMap;
  mp_NeighboursMap = new NeighboursMap_t();

  unsigned int iEnd=m_PolyEdges.size();
  for (unsigned int i = 0; i < iEnd; i++)
  {
    PolygonEdge* Edge = m_PolyEdges[i];

    PolygonEntity* OtherFace = 0;

    if (Edge->getFaces().size() > 1)
      OtherFace =
          Edge->getFaces()[0] == this ? Edge->getFaces()[1] :
                                        Edge->getFaces()[0];

    if (OtherFace)
    {
      ((*mp_NeighboursMap)[OtherFace]).push_back(Edge);
      mp_Neighbours->insert(dynamic_cast<LandREntity*>(OtherFace));
    }
  }

}

// =====================================================================
// =====================================================================

bool PolygonEntity::isComplete()
{
  std::vector<geos::geom::Geometry*> Geoms;

  unsigned int iEnd=m_PolyEdges.size();
  for (unsigned int i = 0; i < iEnd; i++)
    Geoms.push_back(m_PolyEdges.at(i)->getLine());

  geos::geom::MultiLineString* MLS =
      geos::geom::GeometryFactory::getDefaultInstance()->createMultiLineString(
          Geoms);

  geos::geom::LineString* LS = LandRTools::getMergedLineStringFromGeometry(
      dynamic_cast<geos::geom::Geometry*>(MLS));

  bool Complete = LS && LS->equals(mp_Polygon->getExteriorRing());

  delete MLS;
  delete LS;

  return Complete;
}

// =====================================================================
// =====================================================================

std::vector<PolygonEdge*> PolygonEntity::getCommonEdgesWith(
    PolygonEntity& Other)
{
  std::vector<PolygonEdge*> Edges;

  if (!mp_NeighboursMap)
    computeNeighbours();

  if (mp_NeighboursMap->count(&Other))
    Edges = mp_NeighboursMap->at(&Other);

  return Edges;
}

// =====================================================================
// =====================================================================

geos::geom::Geometry* PolygonEntity::getBufferedBoundary(double BufferDistance)
{
  return mp_Polygon->getBoundary()->buffer(BufferDistance);
}

// =====================================================================
// =====================================================================

void PolygonEntity::computeLineStringNeighbours(
    LineStringGraph& Graph, LandRTools::Relationship Relation,
    double BufferDistance)
{
  if (!mp_NeighboursMap)
    computeNeighbours();

  delete mp_LineStringNeighboursMap;
  mp_LineStringNeighboursMap = new PolygonEntity::LineStringNeighboursMap_t;

  geos::geom::Geometry* PolyBuff = getBufferedBoundary(BufferDistance);

  openfluid::landr::LandRGraph::Entities_t LSs = Graph.getEntities();

  openfluid::landr::LandRGraph::Entities_t::const_iterator it = LSs.begin();
  openfluid::landr::LandRGraph::Entities_t::const_iterator ite = LSs.end();
  for (; it != ite; ++it)
  {
    LineStringEntity* LS = dynamic_cast<LineStringEntity*>(*it);

    if (Relation == LandRTools::CONTAINS && LS->getLine()->within(PolyBuff))
    {
      geos::geom::Geometry* EdgeBuff;
      unsigned int jEnd=m_PolyEdges.size();
      for (unsigned int j = 0; j < jEnd; j++)
      {
        EdgeBuff = m_PolyEdges[j]->getLine()->buffer(BufferDistance);
        if (LS->getLine()->within(EdgeBuff))
        {
          mp_LineStringNeighboursMap->insert(
              std::make_pair(LS, m_PolyEdges[j]));
          mp_Neighbours->insert(*it);
        }
        delete EdgeBuff;
      }

    }

    else if (Relation == LandRTools::INTERSECTS
             && LS->getLine()->intersects(PolyBuff))
    {
      mp_LineStringNeighboursMap->insert(
          std::make_pair(LS, (openfluid::landr::PolygonEdge*) 0));
      mp_Neighbours->insert(*it);
    }
  }

  delete PolyBuff;
}

// =====================================================================
// =====================================================================

PolygonEntity::LineStringNeighboursMap_t* PolygonEntity::getLineStringNeighbours()
{
  return mp_LineStringNeighboursMap;
}

// =====================================================================
// =====================================================================

geos::geom::LineString* PolygonEntity::mergeEdges(PolygonEdge* Edge, PolygonEdge* EdgeToMerge)
{

  //ensure that the two PolygonEdges are coincident
  if (!Edge->isCoincident(EdgeToMerge))
    throw openfluid::base::FrameworkException(
        "PolygonEntity::mergeEdges",
        "The PolygonEdges are not coincident");


  geos::geom::Point *StartPoint=Edge->getLine()->getStartPoint();
  geos::geom::Point *EndPoint=Edge->getLine()->getEndPoint();

  geos::geom::Point *StartPoint2=EdgeToMerge->getLine()->getStartPoint();
  geos::geom::Point *EndPoint2=EdgeToMerge->getLine()->getEndPoint();

  geos::geom::CoordinateSequence *CoordsOne=0;
  geos::geom::CoordinateSequence *CoordsTwo=0;

  if (EndPoint->getCoordinate()->equals(*(StartPoint2->getCoordinate())))
  {
    CoordsOne=(Edge->getLine())->getCoordinates();
    CoordsTwo=(EdgeToMerge->getLine())->getCoordinates();
    CoordsOne->add(CoordsTwo,false,true);
  }
  else if (StartPoint->getCoordinate()->equals(*(EndPoint2->getCoordinate())))
  {
    CoordsOne=(EdgeToMerge->getLine())->getCoordinates();
    CoordsTwo=(Edge->getLine())->getCoordinates();
    CoordsOne->add(CoordsTwo,false,true);
  }
  else if (EndPoint->getCoordinate()->equals(*(EndPoint2->getCoordinate())))
  {
    CoordsOne=(Edge->getLine())->getCoordinates();
    CoordsTwo=(EdgeToMerge->getLine())->getCoordinates();
    CoordsOne->add(CoordsTwo,false,false);
  }
  else if (StartPoint->getCoordinate()->equals(*(StartPoint2->getCoordinate())))
  {
    geos::geom::Geometry * reverseLine=EdgeToMerge->getLine()->reverse();

    CoordsOne=reverseLine->getCoordinates();
    CoordsTwo=(Edge->getLine())->getCoordinates();
    CoordsOne->add(CoordsTwo,false,true);
  }

  geos::geom::LineString * NewLine=geos::geom::GeometryFactory::getDefaultInstance()->createLineString(CoordsOne);

  return NewLine;
}

// =====================================================================
// =====================================================================





}// namespace landr
} /* namespace openfluid */
