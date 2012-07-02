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
#include <openfluid/landr/LineStringUnit.hpp>
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
  std::vector<openfluid::landr::LineStringUnit*> OtherUnits = Other.getUnits();

  for (std::vector<openfluid::landr::LineStringUnit*>::iterator it =
      OtherUnits.begin(); it != OtherUnits.end(); ++it)
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
    const std::vector<openfluid::landr::LineStringUnit*>& Units) :
    geos::planargraph::PlanarGraph()
{
  for (std::vector<openfluid::landr::LineStringUnit*>::const_iterator it =
      Units.begin(); it != Units.end(); ++it)
  {
    addEdge(
        dynamic_cast<const geos::geom::LineString*>((*it)->getLine()->clone()),
        (*it)->getFeature()->Clone());
  }
}

// =====================================================================
// =====================================================================

openfluid::landr::LineStringUnit* LineStringGraph::addEdge(
    const geos::geom::LineString* LineString, OGRFeature* Feat)
{
  if (LineString->isEmpty())
    return (openfluid::landr::LineStringUnit*) 0;

  geos::geom::CoordinateSequence *coordinates =
      geos::geom::CoordinateSequence::removeRepeatedPoints(
          LineString->getCoordinatesRO());

  const geos::geom::Coordinate& startCoordinate = coordinates->getAt(0);
  const geos::geom::Coordinate& endCoordinate = coordinates->getAt(
      coordinates->getSize() - 1);

  geos::planargraph::Node* startNode = getNode(startCoordinate);
  geos::planargraph::Node* endNode = getNode(endCoordinate);

  geos::planargraph::DirectedEdge *directedEdge0 =
      new geos::planargraph::DirectedEdge(startNode, endNode,
                                          coordinates->getAt(1), true);
  m_NewDirEdges.push_back(directedEdge0);

  geos::planargraph::DirectedEdge *directedEdge1 =
      new geos::planargraph::DirectedEdge(
          endNode, startNode, coordinates->getAt(coordinates->getSize() - 2),
          false);
  m_NewDirEdges.push_back(directedEdge1);

  openfluid::landr::LineStringUnit* edge = new LineStringUnit(LineString, Feat);

  m_UnitsBySelfId[edge->getSelfId()] = edge;
  m_Units.push_back(edge);

  edge->setDirectedEdges(directedEdge0, directedEdge1);

  add(edge);

  delete coordinates;

  return edge;
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
  for (i = 0; i < m_Units.size(); i++)
    delete m_Units[i];
  for (i = 0; i < m_NewDirEdges.size(); i++)
    delete m_NewDirEdges[i];
}

// =====================================================================
// =====================================================================

openfluid::landr::LineStringUnit* LineStringGraph::getLastLineStringUnit()
{
  std::vector<openfluid::landr::LineStringUnit*> EndUnits =
      getEndLineStringUnits();

  if (EndUnits.size() != 1)
    return (openfluid::landr::LineStringUnit*) 0;

  return *EndUnits.begin();
}

// =====================================================================
// =====================================================================

std::vector<openfluid::landr::LineStringUnit*> LineStringGraph::getEndLineStringUnits()
{
  std::vector<openfluid::landr::LineStringUnit*> EndUnits;

  openfluid::landr::LineStringUnit* CurrentUnit = 0;

  for (unsigned int i = 0; i < getEdges()->size(); i++)
  {
    CurrentUnit =
        dynamic_cast<openfluid::landr::LineStringUnit*>(getEdges()->at(i));

    if (CurrentUnit->getEndNode()->getDegree() == 1)
    {
      EndUnits.push_back(CurrentUnit);
    }
  }

  return EndUnits;
}

// =====================================================================
// =====================================================================

std::vector<openfluid::landr::LineStringUnit*> LineStringGraph::getStartLineStringUnits()
{
  std::vector<openfluid::landr::LineStringUnit*> StartUnits;

  openfluid::landr::LineStringUnit* CurrentUnit = 0;

  for (unsigned int i = 0; i < getEdges()->size(); i++)
  {
    CurrentUnit =
        dynamic_cast<openfluid::landr::LineStringUnit*>(getEdges()->at(i));

    if (CurrentUnit->getStartNode()->getDegree() == 1)
    {
      StartUnits.push_back(CurrentUnit);
    }
  }

  return StartUnits;
}

// =====================================================================
// =====================================================================

unsigned int LineStringGraph::getSize()
{
  return getEdges()->size();
}

// =====================================================================
// =====================================================================

openfluid::landr::LineStringUnit* LineStringGraph::getUnit(int SelfId)
{
  if (m_UnitsBySelfId.count(SelfId))
    return m_UnitsBySelfId.find(SelfId)->second;

  return (openfluid::landr::LineStringUnit*) 0;
}

// =====================================================================
// =====================================================================

std::vector<openfluid::landr::LineStringUnit*> LineStringGraph::getUnits()
{
  return m_Units;
}

} // namespace landr
} /* namespace openfluid */
