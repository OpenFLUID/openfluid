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
 \file LineStringGraph.cpp
 \brief Implements ...

 \author Aline LIBRES <aline.libres@gmail.com>
 */

#include "LineStringGraph.hpp"

#include <openfluid/core/GeoVectorValue.hpp>
#include <openfluid/landr/LineStringEntity.hpp>
#include <geos/planargraph/DirectedEdge.h>
#include <geos/planargraph/Node.h>
#include <geos/geom/CoordinateSequence.h>
#include <geos/geom/LineString.h>

#include <vector>

namespace openfluid {
namespace landr {

// =====================================================================
// =====================================================================

LineStringGraph::LineStringGraph() :
    geos::planargraph::PlanarGraph()
{

}

// =====================================================================
// =====================================================================

LineStringGraph::LineStringGraph(openfluid::landr::LineStringGraph& Other) :
    geos::planargraph::PlanarGraph()
{
  std::vector<openfluid::landr::LineStringEntity*> OtherEntities = Other.getEntities();

  for (std::vector<openfluid::landr::LineStringEntity*>::iterator it =
      OtherEntities.begin(); it != OtherEntities.end(); ++it)
  {
    addEdge(dynamic_cast<geos::geom::LineString*>((*it)->getLine()->clone()),
            (*it)->getFeature()->Clone());
  }
}

// =====================================================================
// =====================================================================

LineStringGraph::LineStringGraph(const openfluid::core::GeoVectorValue& Val) :
    geos::planargraph::PlanarGraph()
{
// TODO move to... ?
  setlocale(LC_NUMERIC, "C");

  OGRLayer* Layer0 =
      (const_cast<openfluid::core::GeoVectorValue&>(Val)).getLayer0();

  Layer0->ResetReading();

  OGRFeature* Feat;
  while ((Feat = Layer0->GetNextFeature()) != NULL)
  {
    OGRGeometry* OGRGeom = Feat->GetGeometryRef();

    // c++ cast doesn't work (have to use the C API instead)
    geos::geom::Geometry* GeosGeom =
        (geos::geom::Geometry*) OGRGeom->exportToGEOS();

    addEdge(dynamic_cast<const geos::geom::LineString*>(GeosGeom->clone()),
            Feat->Clone());

    // destroying the feature destroys also the associated OGRGeom
    OGRFeature::DestroyFeature(Feat);
    delete GeosGeom;
  }

}

// =====================================================================
// =====================================================================

LineStringGraph::LineStringGraph(
    const std::vector<openfluid::landr::LineStringEntity*>& Entities) :
    geos::planargraph::PlanarGraph()
{
  for (std::vector<openfluid::landr::LineStringEntity*>::const_iterator it =
      Entities.begin(); it != Entities.end(); ++it)
  {
    addEdge(
        dynamic_cast<const geos::geom::LineString*>((*it)->getLine()->clone()),
        (*it)->getFeature()->Clone());
  }
}

// =====================================================================
// =====================================================================

openfluid::landr::LineStringEntity* LineStringGraph::addEdge(
    const geos::geom::LineString* LineString, OGRFeature* Feat)
{
  if (LineString->isEmpty())
    return (openfluid::landr::LineStringEntity*) 0;

  geos::geom::CoordinateSequence* Coordinates =
      geos::geom::CoordinateSequence::removeRepeatedPoints(
          LineString->getCoordinatesRO());

  const geos::geom::Coordinate& StartCoordinate = Coordinates->getAt(0);
  const geos::geom::Coordinate& EndCoordinate = Coordinates->getAt(
      Coordinates->getSize() - 1);

  geos::planargraph::Node* StartNode = getNode(StartCoordinate);
  geos::planargraph::Node* EndNode = getNode(EndCoordinate);

  geos::planargraph::DirectedEdge* DirectedEdge0 =
      new geos::planargraph::DirectedEdge(StartNode, EndNode,
                                          Coordinates->getAt(1), true);
  m_NewDirEdges.push_back(DirectedEdge0);

  geos::planargraph::DirectedEdge* DirectedEdge1 =
      new geos::planargraph::DirectedEdge(
          EndNode, StartNode, Coordinates->getAt(Coordinates->getSize() - 2),
          false);
  m_NewDirEdges.push_back(DirectedEdge1);

  openfluid::landr::LineStringEntity* Edge = new LineStringEntity(LineString, Feat);

  m_EntitiesBySelfId[Edge->getSelfId()] = Edge;
  m_Entities.push_back(Edge);

  Edge->setDirectedEdges(DirectedEdge0, DirectedEdge1);

  add(Edge);

  delete Coordinates;

  return Edge;
}

// =====================================================================
// =====================================================================

geos::planargraph::Node* LineStringGraph::getNode(
    const geos::geom::Coordinate& Coordinate)
{
  geos::planargraph::Node* Node = findNode(Coordinate);
  if (Node == NULL)
  {
    Node = new geos::planargraph::Node(Coordinate);
    m_NewNodes.push_back(Node);
    add(Node);
  }
  return Node;
}

// =====================================================================
// =====================================================================

LineStringGraph::~LineStringGraph()
{
  unsigned int i;
  for (i = 0; i < m_NewNodes.size(); i++)
    delete m_NewNodes[i];
  for (i = 0; i < m_Entities.size(); i++)
    delete m_Entities[i];
  for (i = 0; i < m_NewDirEdges.size(); i++)
    delete m_NewDirEdges[i];
}

// =====================================================================
// =====================================================================

openfluid::landr::LineStringEntity* LineStringGraph::getLastLineStringEntity()
{
  std::vector<openfluid::landr::LineStringEntity*> EndEntities =
      getEndLineStringEntities();

  if (EndEntities.size() != 1)
    return (openfluid::landr::LineStringEntity*) 0;

  return *EndEntities.begin();
}

// =====================================================================
// =====================================================================

std::vector<openfluid::landr::LineStringEntity*> LineStringGraph::getEndLineStringEntities()
{
  std::vector<openfluid::landr::LineStringEntity*> EndEntities;

  openfluid::landr::LineStringEntity* CurrentEntity = 0;

  for (unsigned int i = 0; i < getEdges()->size(); i++)
  {
    CurrentEntity =
        dynamic_cast<openfluid::landr::LineStringEntity*>(getEdges()->at(i));

    if (CurrentEntity->getEndNode()->getDegree() == 1)
    {
      EndEntities.push_back(CurrentEntity);
    }
  }

  return EndEntities;
}

// =====================================================================
// =====================================================================

std::vector<openfluid::landr::LineStringEntity*> LineStringGraph::getStartLineStringEntities()
{
  std::vector<openfluid::landr::LineStringEntity*> StartEntities;

  openfluid::landr::LineStringEntity* CurrentEntity = 0;

  for (unsigned int i = 0; i < getEdges()->size(); i++)
  {
    CurrentEntity =
        dynamic_cast<openfluid::landr::LineStringEntity*>(getEdges()->at(i));

    if (CurrentEntity->getStartNode()->getDegree() == 1)
    {
      StartEntities.push_back(CurrentEntity);
    }
  }

  return StartEntities;
}

// =====================================================================
// =====================================================================

unsigned int LineStringGraph::getSize()
{
  return getEdges()->size();
}

// =====================================================================
// =====================================================================

openfluid::landr::LineStringEntity* LineStringGraph::getEntity(int SelfId)
{
  if (m_EntitiesBySelfId.count(SelfId))
    return m_EntitiesBySelfId.find(SelfId)->second;

  return (openfluid::landr::LineStringEntity*) 0;
}

// =====================================================================
// =====================================================================

std::vector<openfluid::landr::LineStringEntity*> LineStringGraph::getEntities()
{
  return m_Entities;
}

// =====================================================================
// =====================================================================

std::vector<openfluid::landr::LineStringEntity*> LineStringGraph::getSelfIdOrderedEntities()
{
  std::vector<LineStringEntity*> Entities;

  for (std::map<int, LineStringEntity*>::iterator it = m_EntitiesBySelfId.begin();
      it != m_EntitiesBySelfId.end(); ++it)
    Entities.push_back(it->second);

  return Entities;
}

// =====================================================================
// =====================================================================

void LineStringGraph::addAttribute(std::string AttributeName)
{
  for (std::vector<LineStringEntity*>::iterator it = m_Entities.begin();
      it != m_Entities.end(); ++it)
  {
    addAttribute(AttributeName, **it);
  }
}

// =====================================================================
// =====================================================================

// thanks to LineStringGraph is a friend of LineStringEntity
void LineStringGraph::addAttribute(std::string AttributeName,
                                   LineStringEntity& Entity)
{
  Entity.m_Attributes[AttributeName];
}

// =====================================================================
// =====================================================================

void LineStringGraph::removeAttribute(std::string AttributeName)
{
  for (std::vector<LineStringEntity*>::iterator it = m_Entities.begin();
      it != m_Entities.end(); ++it)
  {
    removeAttribute(AttributeName, **it);
  }
}

// =====================================================================
// =====================================================================

// thanks to LineStringGraph is a friend of LineStringEntity
void LineStringGraph::removeAttribute(std::string AttributeName,
                                      LineStringEntity& Entity)
{
  Entity.m_Attributes.erase(AttributeName);
}

// =====================================================================
// =====================================================================

}// namespace landr
} /* namespace openfluid */
