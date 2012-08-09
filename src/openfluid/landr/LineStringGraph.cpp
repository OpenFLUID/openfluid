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

#include <openfluid/landr/LineStringEntity.hpp>
#include <openfluid/core/GeoVectorValue.hpp>
#include <openfluid/core/GeoRasterValue.hpp>
#include <openfluid/base/OFException.hpp>
#include <geos/planargraph/DirectedEdge.h>
#include <geos/planargraph/Node.h>
#include <geos/geom/CoordinateSequence.h>
#include <geos/geom/LineString.h>

namespace openfluid {
namespace landr {

// =====================================================================
// =====================================================================

LineStringGraph::LineStringGraph() :
    LandRGraph()
{

}

// =====================================================================
// =====================================================================

LineStringGraph::LineStringGraph(openfluid::core::GeoVectorValue& Val) :
    LandRGraph(Val)
{

}

// =====================================================================
// =====================================================================

LineStringGraph::~LineStringGraph()
{

}

// =====================================================================
// =====================================================================

LineStringGraph* LineStringGraph::create(openfluid::core::GeoVectorValue& Val)
{
  LineStringGraph* Graph = new LineStringGraph(Val);
  Graph->addEntitiesFromGeoVector();

  return Graph;
}

// =====================================================================
// =====================================================================

LineStringGraph* LineStringGraph::create(
    const std::vector<LandREntity*>& Entities)
{
  LineStringGraph* Graph = new LineStringGraph();
  Graph->addEntitiesFromEntityList(Entities);

  return Graph;
}

// =====================================================================
// =====================================================================

LineStringGraph* LineStringGraph::clone()
{
  if (mp_Vector)
    return LineStringGraph::create(*mp_Vector);
  else
    return LineStringGraph::create(getEntities());
}

// =====================================================================
// =====================================================================

LandRGraph::GraphType LineStringGraph::getType()
{
  return LINESTRING;
}

// =====================================================================
// =====================================================================

void LineStringGraph::addEntity(LandREntity* Entity)
{
  LineStringEntity* Edge = dynamic_cast<LineStringEntity*>(Entity);

  const geos::geom::LineString* LineString = Edge->getLine();

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

  geos::planargraph::DirectedEdge* DirectedEdge1 =
      new geos::planargraph::DirectedEdge(
          EndNode, StartNode, Coordinates->getAt(Coordinates->getSize() - 2),
          false);

  Edge->setDirectedEdges(DirectedEdge0, DirectedEdge1);

  add(Edge);

  m_EntitiesBySelfId[Edge->getSelfId()] = Edge;
  m_Entities.push_back(Edge);

  delete Coordinates;
}

// =====================================================================
// =====================================================================

LandREntity* LineStringGraph::getNewEntity(const geos::geom::Geometry* Geom,
                                           unsigned int SelfId)
{
  return new LineStringEntity(Geom, SelfId);
}

// =====================================================================
// =====================================================================

void LineStringGraph::removeEntity(int SelfId)
{
  LineStringEntity* Ent = getEntity(SelfId);

  if (!Ent)
  {
    std::ostringstream s;
    s << "No entity with id " << SelfId;
    throw openfluid::base::OFException("OpenFLUID Framework",
                                       "LineStringGraph::removeEntity",
                                       s.str());
    return;
  }

  remove(dynamic_cast<geos::planargraph::Edge*>(Ent));

  m_Entities.erase(std::find(m_Entities.begin(), m_Entities.end(), Ent));
  m_EntitiesBySelfId.erase(SelfId);

  delete Ent;

  removeUnusedNodes();
}

// =====================================================================
// =====================================================================

LineStringEntity* LineStringGraph::getEntity(int SelfId)
{
  return dynamic_cast<LineStringEntity*>(LandRGraph::getEntity(SelfId));
}

// =====================================================================
// =====================================================================

LineStringEntity* LineStringGraph::getLastLineStringEntity()
{
  std::vector<LineStringEntity*> EndEntities = getEndLineStringEntities();

  if (EndEntities.size() != 1)
    return (LineStringEntity*) 0;

  return *EndEntities.begin();
}

// =====================================================================
// =====================================================================

std::vector<LineStringEntity*> LineStringGraph::getEndLineStringEntities()
{
  std::vector<LineStringEntity*> EndEntities;

  LineStringEntity* CurrentEntity = 0;

  for (unsigned int i = 0; i < getEdges()->size(); i++)
  {
    CurrentEntity = dynamic_cast<LineStringEntity*>(getEdges()->at(i));

    if (CurrentEntity->getEndNode()->getDegree() == 1)
    {
      EndEntities.push_back(CurrentEntity);
    }
  }

  return EndEntities;
}

// =====================================================================
// =====================================================================

std::vector<LineStringEntity*> LineStringGraph::getStartLineStringEntities()
{
  std::vector<LineStringEntity*> StartEntities;

  LineStringEntity* CurrentEntity = 0;

  for (unsigned int i = 0; i < getEdges()->size(); i++)
  {
    CurrentEntity = dynamic_cast<LineStringEntity*>(getEdges()->at(i));

    if (CurrentEntity->getStartNode()->getDegree() == 1)
    {
      StartEntities.push_back(CurrentEntity);
    }
  }

  return StartEntities;
}

// =====================================================================
// =====================================================================

float* LineStringGraph::getRasterValueForEntityStartNode(
    LineStringEntity& Entity)
{
  float* Val = 0;

  if (!mp_Raster)
    throw openfluid::base::OFException(
        "OpenFLUID Framework",
        "LineStringGraph::getRasterValueForEntityStartNode",
        "No raster associated to the LineStringGraph");
  else
  {
    Val = new float(
        mp_Raster->getValueOfCoordinate(
            Entity.getStartNode()->getCoordinate()));
  }

  return Val;
}

// =====================================================================
// =====================================================================

float* LineStringGraph::getRasterValueForEntityEndNode(LineStringEntity& Entity)
{
  float* Val = 0;

  if (!mp_Raster)
    throw openfluid::base::OFException(
        "OpenFLUID Framework",
        "LineStringGraph::getRasterValueForEntityEndNode",
        "No raster associated to the LineStringGraph");
  else
  {
    Val = new float(
        mp_Raster->getValueOfCoordinate(Entity.getEndNode()->getCoordinate()));
  }

  return Val;
}

// =====================================================================
// =====================================================================

void LineStringGraph::setAttributeFromRasterValueAtStartNode(
    std::string AttributeName)
{
  addAttribute(AttributeName);
  for (std::vector<LandREntity*>::iterator it = m_Entities.begin();
      it != m_Entities.end(); ++it)
  {
    float* Val = getRasterValueForEntityStartNode(
        *dynamic_cast<LineStringEntity*>(*it));

    if (!Val)
    {
      std::ostringstream s;
      s << "No raster value for entity " << (*it)->getSelfId() << " StartNode.";

      throw openfluid::base::OFException(
          "OpenFLUID Framework",
          "LineStringGraph::setAttributeFromRasterValueAtStartNode", s.str());
      return;
    }

    (*it)->setAttributeValue(AttributeName, *Val);
  }

}

// =====================================================================
// =====================================================================

void LineStringGraph::setAttributeFromRasterValueAtEndNode(
    std::string AttributeName)
{
  addAttribute(AttributeName);
  for (std::vector<LandREntity*>::iterator it = m_Entities.begin();
      it != m_Entities.end(); ++it)
  {
    float* Val = getRasterValueForEntityEndNode(
        *dynamic_cast<LineStringEntity*>(*it));

    if (!Val)
    {
      std::ostringstream s;
      s << "No raster value for entity " << (*it)->getSelfId() << " EndNode.";

      throw openfluid::base::OFException(
          "OpenFLUID Framework",
          "LineStringGraph::setAttributeFromRasterValueAtEndNode", s.str());
      return;
    }

    (*it)->setAttributeValue(AttributeName, *Val);
  }

}

} // namespace landr
} /* namespace openfluid */
