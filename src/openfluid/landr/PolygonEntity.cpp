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

#include <openfluid/landr/PolygonGraph.hpp>
#include <openfluid/landr/PolygonEdge.hpp>
#include <openfluid/base/OFException.hpp>
#include <geos/geom/Polygon.h>
#include <geos/geom/LineString.h>
#include <geos/geom/MultiLineString.h>
#include <geos/geom/GeometryCollection.h>
#include <geos/geom/GeometryFactory.h>
#include <geos/geom/Point.h>
#include <geos/planargraph/Node.h>
#include <geos/planargraph/DirectedEdge.h>

namespace openfluid {
namespace landr {

// =====================================================================
// =====================================================================

PolygonEntity::PolygonEntity(const geos::geom::Polygon* NewPolygon,
                             OGRFeature* Feat) :
    geos::planargraph::Edge(), mp_Polygon(NewPolygon), mp_Feature(Feat), mp_SelfId(
        0), mp_Neighbours(0)
{
  m_Area = mp_Polygon->getArea();
  mp_Centroide = mp_Polygon->getCentroid();
}

// =====================================================================
// =====================================================================

PolygonEntity::PolygonEntity(const openfluid::landr::PolygonEntity& Other) :
    geos::planargraph::Edge(), mp_SelfId(0), mp_Neighbours(0)
{
  mp_Polygon = dynamic_cast<geos::geom::Polygon*>(Other.getPolygon()->clone());
  mp_Feature =
      (const_cast<openfluid::landr::PolygonEntity&>(Other).getFeature())->Clone();

  m_Area = mp_Polygon->getArea();
  mp_Centroide = mp_Polygon->getCentroid();
}

// =====================================================================
// =====================================================================

PolygonEntity::~PolygonEntity()
{
  OGRFeature::DestroyFeature(mp_Feature);
  delete mp_Polygon;
  delete mp_Neighbours;
  delete mp_SelfId;
}

// =====================================================================
// =====================================================================

const geos::geom::Polygon* PolygonEntity::getPolygon() const
{
  return mp_Polygon;
}

// =====================================================================
// =====================================================================

OGRFeature* PolygonEntity::getFeature()
{
  return mp_Feature;
}

// =====================================================================
// =====================================================================

unsigned int PolygonEntity::getSelfId()
{
  if (!mp_SelfId)
  {
    if (mp_Feature && mp_Feature->GetFieldIndex("SELF_ID") != -1)
      mp_SelfId = new unsigned int(mp_Feature->GetFieldAsInteger("SELF_ID"));
    else
      throw openfluid::base::OFException("OpenFLUID Framework",
                                         "PolygonEntity::getSelfId",
                                         "Cannot get SELF_ID field.");
  }

  return *mp_SelfId;
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

    Lines = *PolygonGraph::getMergedLineStringsFromGeometry(SharedGeom);

    delete SharedGeom;
  }

  return Lines;
}

// =====================================================================
// =====================================================================

void PolygonEntity::addEdge(PolygonEdge& Edge)
{
  Edge.addFace(*this);

  m_PolyEdges.push_back(&Edge);

  mp_Neighbours = 0;
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

  mp_Neighbours = 0;

  delete Edge;
}

// =====================================================================
// =====================================================================

PolygonEdge* PolygonEntity::findEdgeLineIntersectingWith(geos::geom::LineString& Segment)
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

const PolygonEntity::NeigboursMap_t* PolygonEntity::getNeighbours()
{
  if (!mp_Neighbours)
    computeNeighbours();

  return mp_Neighbours;
}

// =====================================================================
// =====================================================================

std::vector<int> PolygonEntity::getOrderedNeighbourSelfIds()
{
  std::vector<int> Ids;

  if (!mp_Neighbours)
    computeNeighbours();

  for (NeigboursMap_t::iterator it = mp_Neighbours->begin();
      it != mp_Neighbours->end(); ++it)
    Ids.push_back(it->first->getSelfId());

  std::sort(Ids.begin(), Ids.end());

  return Ids;
}

// =====================================================================
// =====================================================================

//std::vector<openfluid::landr::PolygonEntity*> PolygonEntity::getUpNeighbours()
//{
//  std::vector<openfluid::landr::PolygonEntity*> UpNeighbours;
//
//  return UpNeighbours;
//}
//
//// =====================================================================
//// =====================================================================
//
//std::vector<openfluid::landr::PolygonEntity*> PolygonEntity::getDownNeighbours()
//{
//  std::vector<openfluid::landr::PolygonEntity*> DownNeighbours;
//
//  return DownNeighbours;
//}

// =====================================================================
// =====================================================================

bool PolygonEntity::getAttributeValue(std::string AttributeName,
                                      boost::any& Value)
{
  if (m_Attributes.count(AttributeName))
  {
    Value = m_Attributes.find(AttributeName)->second;
    return true;
  }

  return false;
}

// =====================================================================
// =====================================================================

bool PolygonEntity::setAttributeValue(std::string AttributeName,
                                      boost::any Value)
{
  if (m_Attributes.count(AttributeName))
  {
    m_Attributes[AttributeName] = Value;
    return true;
  }

  return false;
}

// =====================================================================
// =====================================================================

double PolygonEntity::getArea()
{
  return m_Area;
}

// =====================================================================
// =====================================================================

geos::geom::Point* PolygonEntity::getCentroide()
{
  return mp_Centroide;
}

// =====================================================================
// =====================================================================

double PolygonEntity::getDistCentroCentro(PolygonEntity& Other)
{
  return mp_Centroide->distance(Other.getCentroide());
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

  geos::geom::LineString* LS = PolygonGraph::getMergedLineStringFromGeometry(
      dynamic_cast<geos::geom::Geometry*>(MLS));

  bool Complete = LS && LS->equals(mp_Polygon->getExteriorRing());

  delete MLS;
  delete LS;

  return Complete;
}

// =====================================================================
// =====================================================================

void PolygonEntity::computeNeighbours()
{
  delete mp_Neighbours;

  mp_Neighbours = new NeigboursMap_t();

  for (unsigned int i = 0; i < m_PolyEdges.size(); i++)
  {
    PolygonEdge* Edge = m_PolyEdges[i];

    PolygonEntity* OtherFace = 0;

    if (Edge->getFaces().size() > 1)
      OtherFace =
          Edge->getFaces()[0] == this ? Edge->getFaces()[1] :
                                        Edge->getFaces()[0];

    if (OtherFace)
      ((*mp_Neighbours)[OtherFace]).push_back(Edge);
  }

}

// =====================================================================
// =====================================================================

std::vector<PolygonEdge*> PolygonEntity::getCommonEdgesWith(
    PolygonEntity& Other)
{
  std::vector<PolygonEdge*> Edges;

  if (!mp_Neighbours)
    computeNeighbours();

  if (mp_Neighbours->count(&Other))
    Edges = mp_Neighbours->at(&Other);

  return Edges;
}

// =====================================================================
// =====================================================================

}// namespace landr
} /* namespace openfluid */
