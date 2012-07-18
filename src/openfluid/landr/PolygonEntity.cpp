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
#include <geos/planargraph/Node.h>
#include <geos/planargraph/DirectedEdge.h>

namespace openfluid {
namespace landr {

// =====================================================================
// =====================================================================

PolygonEntity::PolygonEntity(const geos::geom::Polygon* NewPolygon,
                             OGRFeature* Feat) :
    geos::planargraph::Edge(), mp_Polygon(NewPolygon), mp_Feature(Feat)
{

}

// =====================================================================
// =====================================================================

PolygonEntity::PolygonEntity(const openfluid::landr::PolygonEntity& Other) :
    geos::planargraph::Edge()
{
  mp_Polygon = dynamic_cast<geos::geom::Polygon*>(Other.getPolygon()->clone());
  mp_Feature =
      (const_cast<openfluid::landr::PolygonEntity&>(Other).getFeature())->Clone();
}

// =====================================================================
// =====================================================================

PolygonEntity::~PolygonEntity()
{
  OGRFeature::DestroyFeature(mp_Feature);
  delete mp_Polygon;
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

int PolygonEntity::getSelfId()
{
  if (mp_Feature && mp_Feature->GetFieldIndex("SELF_ID") != -1)
    return mp_Feature->GetFieldAsInteger("SELF_ID");
  else
    throw openfluid::base::OFException("OpenFLUID Framework",
                                       "PolygonEntity::getSelfId",
                                       "Cannot get SELF_ID field.");
}

// =====================================================================
// =====================================================================

geos::geom::LineString* PolygonEntity::getLineIntersectionWith(
    PolygonEntity& Other)
{
  geos::geom::LineString* LS = 0;

  geos::geom::Geometry* SharedGeom = mp_Polygon->intersection(Other.mp_Polygon);

  if (SharedGeom->getDimension() == geos::geom::Dimension::L)
    LS = PolygonGraph::getMergedLineStringFromGeometry(SharedGeom);

  delete SharedGeom;

  return LS;
}

// =====================================================================
// =====================================================================

void PolygonEntity::addEdge(PolygonEdge* Edge)
{
  m_PolyEdges.push_back(Edge);

  Edge->addFace(this);
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

  delete Edge;
}

// =====================================================================
// =====================================================================

PolygonEdge* PolygonEntity::findEdgeIntersecting(
    geos::geom::LineString& Segment)
{
  for (unsigned int i = 0; i < m_PolyEdges.size(); i++)
  {
    PolygonEdge* Edge = m_PolyEdges.at(i);

    geos::geom::Geometry* Inters = Edge->getLine()->intersection(&Segment);
    if (!Inters->isEmpty() && Inters->getDimension() == 1)
      return Edge;
  }

  return (PolygonEdge*) 0;
}

// =====================================================================
// =====================================================================

void PolygonEntity::addNeighbour(PolygonEntity* Neighbour)
{
  m_Neigbours.push_back(Neighbour);
}

// =====================================================================
// =====================================================================

std::vector<openfluid::landr::PolygonEntity*> PolygonEntity::getNeighbours()
{
  return m_Neigbours;
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
  return mp_Polygon->getArea();
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

  if (!LS || !LS->isClosed())
    return false;

  return true;
}

// =====================================================================
// =====================================================================

}// namespace landr
} /* namespace openfluid */
