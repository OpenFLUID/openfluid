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
#include <openfluid/base/OFException.hpp>
#include <openfluid/landr/LineStringEntity.hpp>
#include <geos/planargraph/DirectedEdge.h>
#include <geos/planargraph/Node.h>
#include <geos/geom/CoordinateSequence.h>
#include <geos/geom/LineString.h>
#include <sstream>

namespace openfluid {
namespace landr {

// =====================================================================
// =====================================================================

LineStringGraph::LineStringGraph()
{

}

// =====================================================================
// =====================================================================

LineStringGraph::LineStringGraph(LineStringGraph& Other)
{
  std::vector<LandREntity*> OtherEntities = Other.getEntities();

  for (std::vector<LandREntity*>::iterator it = OtherEntities.begin();
      it != OtherEntities.end(); ++it)
  {
    addEdge(
        dynamic_cast<geos::geom::LineString*>((dynamic_cast<LineStringEntity*>(*it))->getLine()->clone()),
        (*it)->getFeature()->Clone());
  }
}

// =====================================================================
// =====================================================================

LineStringGraph::LineStringGraph(const openfluid::core::GeoVectorValue& Val)
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

LineStringGraph::LineStringGraph(const std::vector<LineStringEntity*>& Entities)
{
  for (std::vector<LineStringEntity*>::const_iterator it = Entities.begin();
      it != Entities.end(); ++it)
  {
    addEdge(
        dynamic_cast<const geos::geom::LineString*>((*it)->getLine()->clone()),
        (*it)->getFeature()->Clone());
  }
}

// =====================================================================
// =====================================================================

LineStringEntity* LineStringGraph::addEdge(
    const geos::geom::LineString* LineString, OGRFeature* Feat)
{
  if (LineString->isEmpty())
    return (LineStringEntity*) 0;

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

  LineStringEntity* Edge = new LineStringEntity(LineString, Feat);

  Edge->setDirectedEdges(DirectedEdge0, DirectedEdge1);

  add(Edge);

  m_EntitiesBySelfId[Edge->getSelfId()] = Edge;
  m_Entities.push_back(Edge);

  delete Coordinates;

  return Edge;
}

// =====================================================================
// =====================================================================

LineStringGraph::~LineStringGraph()
{
  deleteAll();
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

LineStringEntity* LineStringGraph::getEntity(int SelfId)
{
  return dynamic_cast<LineStringEntity*>(LandRGraph::getEntity(SelfId));
}

// =====================================================================
// =====================================================================

void LineStringGraph::doRemoveEntity(LandREntity* Entity)
{
  remove(dynamic_cast<geos::planargraph::Edge*>(Entity));
}

// =====================================================================
// =====================================================================

void LineStringGraph::doDeleteAll()
{
// nothing to do
}

// =====================================================================
// =====================================================================

}// namespace landr
} /* namespace openfluid */
