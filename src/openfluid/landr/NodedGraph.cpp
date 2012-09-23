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
 \file NodedGraph.cpp
 \brief Implements ...

 \author Aline LIBRES <aline.libres@gmail.com>
 */

#include "NodedGraph.hpp"

#include <openfluid/landr/NodedLine.hpp>
#include <openfluid/landr/LandRTools.hpp>
#include <openfluid/core/GeoVectorValue.hpp>
#include <openfluid/base/OFException.hpp>
#include <geos/geom/GeometryFactory.h>
#include <geos/geom/LineString.h>
#include <geos/operation/linemerge/LineMerger.h>
#include <geos/operation/overlay/snap/SnapOverlayOp.h>
#include <geos/operation/overlay/snap/GeometrySnapper.h>
#include <geos/planargraph/DirectedEdge.h>
#include <geos/planargraph/Node.h>
#include <geos/noding/NodedSegmentString.h>
#include <geos/noding/SimpleNoder.h>
#include <geos/noding/IntersectionAdder.h>
#include <iostream>
#include <cassert>

namespace openfluid {
namespace landr {

// =====================================================================
// =====================================================================

NodedGraph::NodedGraph() :
    m_snapTolerance(0), mp_Factory(
        geos::geom::GeometryFactory::getDefaultInstance())
{

}

// =====================================================================
// =====================================================================

NodedGraph::~NodedGraph()
{
  for (std::list<NodedLine*>::iterator it = m_NodedLines.begin();
      it != m_NodedLines.end(); ++it)
    delete *it;
}

// =====================================================================
// =====================================================================

void NodedGraph::setSnapValue(double snapValue)
{
  m_snapTolerance = snapValue;
}

// =====================================================================
// =====================================================================

void NodedGraph::addALayer(openfluid::core::GeoVectorValue& Val,
                           unsigned int Order)
{
  std::vector<geos::geom::LineString*> LinesToAdd = getLinesOfValue(Val);

  for (unsigned int i = 0; i < LinesToAdd.size(); i++)
    addALine(LinesToAdd[i], Order, i);
}

// =====================================================================
// =====================================================================

void NodedGraph::addALine(geos::geom::LineString* LineToAdd, unsigned int Order,
                          int NumLine)
{

  LineToAdd->normalize();

//  std::cout << "PROCESSING " << LineToAdd->toString() << std::endl;

  assert(LineToAdd->isValid());

  std::vector<NodedLine*> NodedLinesToRemove;
  std::list<geos::geom::LineString*> LinesAskedToAdd;

//  std::cout << "defining lines to add" << std::endl;

// graph is empty
  if (m_NodedLines.empty())
  {
//    std::cout << "it's the first" << std::endl;

    LinesAskedToAdd.push_back(LineToAdd);

  } else
  {
    for (std::list<NodedLine*>::iterator it = m_NodedLines.begin();
        it != m_NodedLines.end(); ++it)
    {
      const geos::geom::LineString* ExistingLine = (*it)->getLine();

//      std::cout << "   comparing with " << ExistingLine->toString()
//                << std::endl;

      geos::geom::LineString* SnappedLineToAdd;

      if (m_snapTolerance)
      {
        geos::operation::overlay::snap::GeometrySnapper Snapper(*LineToAdd);
        SnappedLineToAdd = dynamic_cast<geos::geom::LineString*>(Snapper.snapTo(
            *ExistingLine, m_snapTolerance).release());
//        std::cout << "snapped : " << SnappedLineToAdd->toString() << std::endl;
      } else
        SnappedLineToAdd = LineToAdd;

      if (!SnappedLineToAdd->equals(ExistingLine)
          && SnappedLineToAdd->intersects(ExistingLine))
      {
//        std::cout << "   -> intersect :" << std::endl;
//        std::cout << "    "
//                  << SnappedLineToAdd->intersection(ExistingLine)->toString()
//                  << std::endl;

        geos::algorithm::LineIntersector li;
        // slow but more robust ?
        geos::noding::SimpleNoder Noder(
            new geos::noding::IntersectionAdder(li));
        std::vector<geos::noding::SegmentString*>* SegStr = new std::vector<
            geos::noding::SegmentString*>();
        SegStr->push_back(
            new geos::noding::NodedSegmentString(
                SnappedLineToAdd->getCoordinates(), NULL));
        SegStr->push_back(
            new geos::noding::NodedSegmentString(ExistingLine->getCoordinates(),
                                                 NULL));
        Noder.computeNodes(SegStr);
        std::vector<geos::noding::SegmentString*>* NewToAddSegments =
            Noder.getNodedSubstrings();

//        std::vector<geos::geom::Geometry*> checkVector;
        for (unsigned int i = 0; i < NewToAddSegments->size(); i++)
        {
//          std::cout << "  ask to add segment "
//                    << NewToAddSegments->at(i)->getCoordinates()->toString()
//                    << std::endl;

          geos::geom::LineString* NewLine = mp_Factory->createLineString(
              NewToAddSegments->at(i)->getCoordinates());

//          checkVector.push_back(NewLine);
          if (!exists(NewLine, LinesAskedToAdd))
            LinesAskedToAdd.push_back(NewLine);
          else
          {
            delete NewLine;
          }
        }

//        geos::geom::Geometry* checkGeom = mp_Factory->buildGeometry(
//            checkVector);
//        assert(checkGeom->equals(SnappedLineToAdd->Union(ExistingLine)));
        NodedLinesToRemove.push_back(*it);

      } else
      {
//        std::cout << "   -> doesn't intersect, add as is" << std::endl;

        if (!exists(SnappedLineToAdd, LinesAskedToAdd))
          LinesAskedToAdd.push_back(SnappedLineToAdd);
      } // else intersect

    } // for NodedLines

  } // else graph is empty

//  std::cout << "effective removal" << std::endl;

  for (unsigned int i = 0; i < NodedLinesToRemove.size(); i++)
  {
//    std::cout << "   --- effective remove "
//              << NodedLinesToRemove[i]->getLine()->toString() << std::endl;
    remove(NodedLinesToRemove[i]);
    m_NodedLines.remove(NodedLinesToRemove[i]);
  }

  // keeping lines which doesn't already exist in the graph
//  std::cout << "checking already existing lines" << std::endl;
  std::list<geos::geom::LineString*> LinesToAdd;
  for (std::list<geos::geom::LineString*>::iterator it =
      LinesAskedToAdd.begin(); it != LinesAskedToAdd.end(); ++it)
  {
    NodedLine* ExistingLine = alreadyExists(*it);
    if (!ExistingLine)
    {
      LinesToAdd.push_back(*it);
    } else
      ExistingLine->addOriginOrder(Order);
  }

  // cutting
//  std::cout << "cutting lines" << std::endl;
  std::vector<geos::geom::Geometry*> LinesToAddVect;
  LinesToAddVect.assign(LinesToAdd.begin(), LinesToAdd.end());
  geos::geom::Geometry* LinesToAddColl = mp_Factory->buildGeometry(
      LinesToAddVect);
//  std::cout << "LINESTOADD COLL" << std::endl;
//  for(unsigned int i=0; i<LinesToAddColl->getNumGeometries(); i++)
//    std::cout << LinesToAddColl->getGeometryN(i)->toString()<< std::endl;

  // ! intersects "forgets" some segments
  // and Union doesn't dissolve all overlaying segments...
  // ! always use snap version, to avoid some TopolgyException (SnapIfNeeded isn't always sufficient)
  geos::geom::Geometry* LinesToAddCutted =
      geos::operation::overlay::snap::SnapOverlayOp::Union(*LinesToAddColl,
                                                           *LinesToAddColl).release();

  // effectively adding
//  std::cout << "effective adding lines" << std::endl;
  for (unsigned int i = 0; i < LinesToAddCutted->getNumGeometries(); i++)
  {
//    std::cout << LinesToAddCutted->getGeometryN(i)->toString() << std::endl;
    if (LinesToAddCutted->getGeometryN(i)->getDimension() != 1)
      continue;
    {
      geos::geom::LineString* NewLine =
          dynamic_cast<geos::geom::LineString*>(const_cast<geos::geom::Geometry*>(LinesToAddCutted->getGeometryN(
              i)));
      NodedLine* ExistingLine = alreadyExists(NewLine);
      if (!ExistingLine)
      {
//        std::cout << "   --- effective add " << NewLine->toString()
//                  << std::endl;
        // workaround to avoid "IllegalArgumentException: Cannot compute the quadrant for point (0,0)", related to wkt ?
        // cf. http://lists.refractions.net/pipermail/jts-devel/2002-October/000143.html
        NewLine->toText();
        addANodedLine(new NodedLine(NewLine, Order));
      } else
        ExistingLine->addOriginOrder(Order);
    }
  }

  // to repair all unwanted cuts during intersection or union
  mergeAdjacentLines();

}

// =====================================================================
// =====================================================================

void NodedGraph::mergeAdjacentLines()
{
//  std::cout << __FUNCTION__ << std::endl;

  if (nodeMap.getNodeMap().size() > 1 && m_NodedLines.size() > 1)
  {
    std::vector<geos::planargraph::Node*>* Unused = findNodesOfDegree(2);
    while (!Unused->empty())
    {
      //    std::cout << "node to remove " << Unused->at(0)->getCoordinate().toString() << std::endl;
      std::vector<geos::planargraph::DirectedEdge*> DirEdges =
          Unused->at(0)->getOutEdges()->getEdges();
      assert(DirEdges.size() == 2);

      NodedLine* E1 = dynamic_cast<NodedLine*>(DirEdges[0]->getEdge());
      NodedLine* E2 = dynamic_cast<NodedLine*>(DirEdges[1]->getEdge());

      //    std::cout << "... merging " << E1->getLine()->toString() << " and "
      //              << E2->getLine()->toString() << std::endl;

      geos::operation::linemerge::LineMerger Merger;
      Merger.add(E1->getLine());
      Merger.add(E2->getLine());
      geos::geom::LineString* MergedLine = Merger.getMergedLineStrings()->at(0);
      assert(MergedLine);

      int Order =
          E1->getLine()->getLength() > E2->getLine()->getLength() ? E1->getMaxOrder() :
                                                                    E2->getMaxOrder();

      if (!alreadyExists(MergedLine))
      {
//        std::cout << "   --- add a merged line " << MergedLine->toString()
//                  << std::endl;
        addANodedLine(new NodedLine(MergedLine, Order));
      }

//      std::cout << "   --- remove merged segment " << E1->getLine()->toString()
//                << std::endl;
//      std::cout << "   --- remove merged segment " << E2->getLine()->toString()
//                << std::endl;
      m_NodedLines.remove(E1);
      m_NodedLines.remove(E2);
      remove(Unused->at(0));

      Unused = findNodesOfDegree(2);
    }
  }

  // removeUnusedNodes (may be a consequence of a removal as upper)
  std::vector<geos::planargraph::Node*>* Unused = findNodesOfDegree(0);
//  std::cout << "unused " << Unused->size() << std::endl;
  for (unsigned int i = 0; i < Unused->size(); i++)
    remove(Unused->at(i));

  delete Unused;
}

// =====================================================================
// =====================================================================

// TODO use findEdge() instead ?
NodedLine* NodedGraph::alreadyExists(const geos::geom::LineString* Line)
{

  for (std::list<NodedLine*>::iterator it = m_NodedLines.begin();
      it != m_NodedLines.end(); ++it)
  {
    if ((*it)->getLine()->equals(Line))
      return *it;
  }

  return (NodedLine*) 0;
}

// =====================================================================
// =====================================================================

bool NodedGraph::exists(geos::geom::LineString* Line,
                        std::list<geos::geom::LineString*> LinesToCompare)
{
  for (std::list<geos::geom::LineString*>::iterator it = LinesToCompare.begin();
      it != LinesToCompare.end(); ++it)
  {
    if ((*it)->equals(Line))
      return true;
  }

  return false;
}

// =====================================================================
// =====================================================================

void NodedGraph::printNodedLines()
{
  std::cout << "NODED LINES" << std::endl;
  for (std::list<NodedLine*>::iterator it = m_NodedLines.begin();
      it != m_NodedLines.end(); ++it)
  {
    std::cout << (*it)->getLine()->toString() << std::endl;
  }
}

// =====================================================================
// =====================================================================

void NodedGraph::addANodedLine(NodedLine* NodedLine)
{
  geos::geom::CoordinateSequence* Coordinates =
      geos::geom::CoordinateSequence::removeRepeatedPoints(
          NodedLine->getLine()->getCoordinatesRO());

  const geos::geom::Coordinate& StartCoordinate = Coordinates->getAt(0);
  const geos::geom::Coordinate& EndCoordinate = Coordinates->getAt(
      Coordinates->getSize() - 1);

  geos::planargraph::Node* StartNode = getNode(StartCoordinate);
  geos::planargraph::Node* EndNode = getNode(EndCoordinate);

//  std::cout << "creating dir edge " << StartNode->getCoordinate().toString()
//            << " -> " << EndNode->getCoordinate().toString() << std::endl;

  geos::planargraph::DirectedEdge* DirectedEdge0 =
      new geos::planargraph::DirectedEdge(StartNode, EndNode,
                                          Coordinates->getAt(1), true);

  geos::planargraph::DirectedEdge* DirectedEdge1 =
      new geos::planargraph::DirectedEdge(
          EndNode, StartNode, Coordinates->getAt(Coordinates->getSize() - 2),
          false);

  NodedLine->setDirectedEdges(DirectedEdge0, DirectedEdge1);

  add(NodedLine);

  m_NodedLines.push_back(NodedLine);

  delete Coordinates;
}

// =====================================================================
// =====================================================================

geos::planargraph::Node* NodedGraph::getNode(
    const geos::geom::Coordinate& Coordinate)
{
  geos::planargraph::Node* Node = findNode(Coordinate);

  if (Node == NULL)
  {
    Node = new geos::planargraph::Node(Coordinate);
    add(Node);
  }

  return Node;
}

// =====================================================================
// =====================================================================

std::vector<geos::geom::LineString*> NodedGraph::getLinesOfValue(
    openfluid::core::GeoVectorValue& Val)
{
  if (Val.isPolygonType())
    return openfluid::landr::LandRTools::getVectorOfExteriorRings(Val);

  if (Val.isLineType())
    return openfluid::landr::LandRTools::getVectorOfLines(Val);

  throw openfluid::base::OFException("OpenFLUID Framework",
                                     "NodedGraph::getLinesOfGeometries",
                                     "Geometries type is not valid.");
}

// =====================================================================
// =====================================================================

std::list<NodedLine*> NodedGraph::getNodedLines()
{
  return m_NodedLines;
}

// =====================================================================
// =====================================================================

}// namespace landr
} /* namespace openfluid */
