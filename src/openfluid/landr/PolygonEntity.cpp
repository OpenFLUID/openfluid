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
 \file PolygonEntity.cpp
 \brief Implements ...

 \author Aline LIBRES <aline.libres@gmail.com>
 */

#include "PolygonEntity.hpp"

#include <openfluid/landr/PolygonEdge.hpp>
#include <openfluid/landr/LandRTools.hpp>
#include <openfluid/base/OFException.hpp>
#include <geos/geom/Polygon.h>
#include <geos/geom/LineString.h>
#include <geos/geom/MultiLineString.h>
#include <geos/geom/GeometryFactory.h>
#include <geos/geom/Point.h>
#include <geos/planargraph/Node.h>
#include <geos/planargraph/DirectedEdge.h>

namespace openfluid {
namespace landr {

// =====================================================================
// =====================================================================

PolygonEntity::PolygonEntity(const geos::geom::Geometry* NewPolygon,
                             unsigned int SelfId) :
    LandREntity(NewPolygon, SelfId), mp_NeighboursMap(0)
{
  if (mp_Geom->getGeometryTypeId() != geos::geom::GEOS_POLYGON)
  {
    delete mp_Centroide;

    throw openfluid::base::OFException("OpenFLUID Framework",
                                       "PolygonEntity::PolygonEntity",
                                       "Geometry is not a Polygon.");

  }

  mp_Polygon =
      dynamic_cast<geos::geom::Polygon*>(const_cast<geos::geom::Geometry*>(mp_Geom));

  if (!mp_Polygon->isValid())
  {
    delete mp_Centroide;

    throw openfluid::base::OFException("OpenFLUID Framework",
                                       "PolygonEntity::PolygonEntity",
                                       "Polygon is not valid.");
  }

}

// =====================================================================
// =====================================================================

PolygonEntity::~PolygonEntity()
{
  delete mp_NeighboursMap;
}

// =====================================================================
// =====================================================================

PolygonEntity* PolygonEntity::clone()
{
  PolygonEntity* Clone = new PolygonEntity(mp_Geom->clone(), m_SelfId);

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
    throw openfluid::base::OFException("OpenFLUID Framework",
                                       "PolygonEntity::removeEdge",
                                       "Edge doesn't exists in Edge vector.");

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
  for (std::vector<PolygonEdge*>::iterator it = m_PolyEdges.begin();
      it != m_PolyEdges.end(); ++it)
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

const PolygonEntity::NeigboursMap_t* PolygonEntity::getNeighboursAndEdges()
{
  if (!mp_NeighboursMap)
    computeNeighbours();

  return mp_NeighboursMap;
}

// =====================================================================
// =====================================================================

std::vector<int> PolygonEntity::getOrderedNeighbourSelfIds()
{
  std::vector<int> Ids;

  if (!mp_NeighboursMap)
    computeNeighbours();

  for (NeigboursMap_t::iterator it = mp_NeighboursMap->begin();
      it != mp_NeighboursMap->end(); ++it)
    Ids.push_back(it->first->getSelfId());

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
  mp_NeighboursMap = new NeigboursMap_t();

  for (unsigned int i = 0; i < m_PolyEdges.size(); i++)
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

  for (unsigned int i = 0; i < m_PolyEdges.size(); i++)
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

}// namespace landr
} /* namespace openfluid */
