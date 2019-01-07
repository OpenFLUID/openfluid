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
  @file LineStringEntity.cpp

  @author Aline LIBRES <aline.libres@gmail.com>
  @author Michael RABOTIN <michael.rabotin@supagro.inra.fr>
 */


#include <geos/geom/LineString.h>
#include <geos/geom/Point.h>
#include <geos/planargraph/DirectedEdge.h>
#include <geos/planargraph/Node.h>

#include <openfluid/base/FrameworkException.hpp>
#include <openfluid/landr/LineStringEntity.hpp>


namespace openfluid { namespace landr {


// =====================================================================
// =====================================================================


LineStringEntity::LineStringEntity(const geos::geom::Geometry* NewLine,unsigned int OfldId) :
  LandREntity(NewLine, OfldId), geos::planargraph::Edge(),
  mp_LOUpNeighbours(0), mp_LODownNeighbours(0)
{
  if (mp_Geom->getGeometryTypeId() != geos::geom::GEOS_LINESTRING)
    throw openfluid::base::FrameworkException(OPENFLUID_CODE_LOCATION,"Geometry is not a LineString");

  mp_Line = dynamic_cast<geos::geom::LineString*>(const_cast<geos::geom::Geometry*>(mp_Geom));

  if (mp_Line->isEmpty())
  {
    delete mp_Centroid;

    throw openfluid::base::FrameworkException(OPENFLUID_CODE_LOCATION,"The LineString is empty");
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


const geos::geom::LineString* LineStringEntity::line() const
{
  return mp_Line;
}


// =====================================================================
// =====================================================================


geos::planargraph::Node* LineStringEntity::startNode()
{
  return getDirEdge(0)->getFromNode();
}


// =====================================================================
// =====================================================================


geos::planargraph::Node* LineStringEntity::endNode()
{
  return getDirEdge(0)->getToNode();
}


// =====================================================================
// =====================================================================


void LineStringEntity::computeNeighbours()
{
  delete mp_Neighbours;

  mp_Neighbours = new std::set<LandREntity*>;

  geos::planargraph::DirectedEdgeStar* UpStar = startNode()->getOutEdges();
  geos::planargraph::DirectedEdgeStar* DownStar = endNode()->getOutEdges();

  std::vector<geos::planargraph::DirectedEdge*>::iterator it = UpStar->iterator();
  std::vector<geos::planargraph::DirectedEdge*>::iterator ite = UpStar->end();
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

  geos::planargraph::DirectedEdgeStar* UpStar = startNode()->getOutEdges();

  geos::geom::Coordinate UpNodeCoo = startNode()->getCoordinate();

  std::vector<geos::planargraph::DirectedEdge*>::iterator it = UpStar->iterator();
  std::vector<geos::planargraph::DirectedEdge*>::iterator ite = UpStar->end();

  for (; it != ite; ++it)
  {
    LineStringEntity* Unit = dynamic_cast<LineStringEntity*>((*it)->getEdge());

    if (Unit->endNode()->getCoordinate().equals(UpNodeCoo))
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

  geos::planargraph::DirectedEdgeStar* DownStar = endNode()->getOutEdges();

  geos::geom::Coordinate DownNodeCoo = endNode()->getCoordinate();

  std::vector<geos::planargraph::DirectedEdge*>::iterator it = DownStar->iterator();
  std::vector<geos::planargraph::DirectedEdge*>::iterator ite = DownStar->end();

  for (; it != ite; ++it)
  {
    LineStringEntity* Unit = dynamic_cast<LineStringEntity*>((*it)->getEdge());

    if (Unit->startNode()->getCoordinate().equals(DownNodeCoo))
      mp_LODownNeighbours->push_back(Unit);
  }
}


// =====================================================================
// =====================================================================


std::vector<LineStringEntity*>  LineStringEntity::getLineNeighboursDegree2()
{
  std::vector<LineStringEntity*> vNeighbours;

  int StartDegree = startNode()->getDegree();
  int EndDegree = endNode()->getDegree();

  computeNeighbours();

  if (StartDegree <= 2)
  {
    std::vector<LineStringEntity*> vUpNeighbours = getLineOrientUpNeighbours();
    vNeighbours.insert (vNeighbours.end(),vUpNeighbours.begin(),vUpNeighbours.end());
  }

  if (EndDegree <= 2)
  {
    std::vector<LineStringEntity*> vDownNeighbours;
    vDownNeighbours = getLineOrientDownNeighbours();
    vNeighbours.insert (vNeighbours.end(),vDownNeighbours.begin(),vDownNeighbours.end());
  }

  return vNeighbours;
}


} }  // namespaces
