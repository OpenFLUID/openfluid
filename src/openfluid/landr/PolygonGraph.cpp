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
 \file PolygonGraph.cpp
 \brief Implements ...

 \author Aline LIBRES <aline.libres@gmail.com>
 */

#include "PolygonGraph.hpp"

#include <openfluid/landr/PolygonEntity.hpp>
#include <openfluid/landr/PolygonEdge.hpp>
#include <openfluid/landr/LandRTools.hpp>
#include <openfluid/landr/VectorDataset.hpp>
#include <openfluid/base/FrameworkException.hpp>
#include <openfluid/core/GeoRasterValue.hpp>
#include <openfluid/core/GeoVectorValue.hpp>
#include <openfluid/core/DoubleValue.hpp>
#include <geos/geom/Polygon.h>
#include <geos/geom/LineString.h>
#include <geos/geom/MultiLineString.h>
#include <geos/geom/GeometryFactory.h>
#include <geos/planargraph/DirectedEdge.h>

namespace openfluid {
namespace landr {

// =====================================================================
// =====================================================================

PolygonGraph::PolygonGraph() :
        LandRGraph()
{

}

// =====================================================================
// =====================================================================

PolygonGraph::PolygonGraph(openfluid::core::GeoVectorValue& Val) :
        LandRGraph(Val)
{

}

// =====================================================================
// =====================================================================

PolygonGraph::PolygonGraph(openfluid::landr::VectorDataset& Vect) :
        LandRGraph(Vect)
{

}

// =====================================================================
// =====================================================================

PolygonGraph* PolygonGraph::create(openfluid::core::GeoVectorValue& Val)
{
  PolygonGraph* Graph = new PolygonGraph(Val);
  Graph->addEntitiesFromGeoVector();

  return Graph;
}

// =====================================================================
// =====================================================================

PolygonGraph* PolygonGraph::create(openfluid::landr::VectorDataset& Vect)
{
  PolygonGraph* Graph = new PolygonGraph(Vect);
  Graph->addEntitiesFromGeoVector();

  return Graph;
}

// =====================================================================
// =====================================================================

PolygonGraph* PolygonGraph::create(const LandRGraph::Entities_t& Entities)
{
  PolygonGraph* Graph = new PolygonGraph();
  Graph->addEntitiesFromEntityList(Entities);

  return Graph;
}

// =====================================================================
// =====================================================================

PolygonGraph::~PolygonGraph()
{
  for (unsigned int i = 0; i < edges.size(); i++)
    delete edges[i];
}

// =====================================================================
// =====================================================================

//PolygonGraph* PolygonGraph::clone()
//{
//  return PolygonGraph::create(*mp_Vector);
//}

// =====================================================================
// =====================================================================

LandRGraph::GraphType PolygonGraph::getType()
{
  return POLYGON;
}

// =====================================================================
// =====================================================================

void PolygonGraph::addEntity(LandREntity* Entity)
{
  PolygonEntity* NewEntity = dynamic_cast<PolygonEntity*>(Entity);

  const geos::geom::Polygon* Polygon = NewEntity->getPolygon();

  std::vector<geos::geom::Geometry*> SharedGeoms;

  try
  {
    for (LandRGraph::Entities_t::iterator it = m_Entities.begin();
        it != m_Entities.end(); ++it)
    {
      PolygonEntity* Poly = dynamic_cast<PolygonEntity*>(*it);
      std::vector<geos::geom::LineString*> SharedLines =
          NewEntity->getLineIntersectionsWith(*Poly);

      for (unsigned int i = 0; i < SharedLines.size(); i++)
      {
        geos::geom::LineString* SharedLine = SharedLines[i];

        PolygonEdge* SharedEdge = createEdge(*SharedLine);

        NewEntity->addEdge(*SharedEdge);
        Poly->addEdge(*SharedEdge);

        removeSegment(Poly, SharedLine);

        SharedGeoms.push_back(SharedLine);
      }

    }

    geos::geom::MultiLineString* NewMultiShared =
        mp_Factory->createMultiLineString(SharedGeoms);

    geos::geom::Geometry* DiffGeom = Polygon->getExteriorRing()->difference(
        NewMultiShared);

    if (!DiffGeom->isEmpty())
    {
      std::vector<geos::geom::LineString*>* NewLines =
          LandRTools::getMergedLineStringsFromGeometry(DiffGeom);

      for (unsigned int i = 0; i < NewLines->size(); i++)
      {
        PolygonEdge* NewEdge = createEdge(*NewLines->at(i));

        if (NewEdge)
          NewEntity->addEdge(*NewEdge);
      }

    }

    m_EntitiesBySelfId[NewEntity->getSelfId()] = NewEntity;
    m_Entities.push_back(NewEntity);

  }
  catch (openfluid::base::FrameworkException& e)
  {
    std::cerr << e.what() << std::endl;
    throw;
  }

}

// =====================================================================
// =====================================================================

LandREntity* PolygonGraph::getNewEntity(const geos::geom::Geometry* Geom,
                                        unsigned int SelfId)
{
  return new PolygonEntity(Geom, SelfId);
}

// =====================================================================
// =====================================================================

PolygonEdge* PolygonGraph::createEdge(geos::geom::LineString& LineString)
{
  if (LineString.isEmpty())
    return (PolygonEdge*) 0;

  geos::geom::CoordinateSequence* Coordinates =
      geos::geom::CoordinateSequence::removeRepeatedPoints(
          LineString.getCoordinatesRO());

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

  PolygonEdge* NewEdge = new PolygonEdge(LineString);

  NewEdge->setDirectedEdges(DirectedEdge0, DirectedEdge1);

  add(NewEdge);

  delete Coordinates;

  return NewEdge;
}

// =====================================================================
// =====================================================================

void PolygonGraph::removeSegment(PolygonEntity* Entity,
                                 geos::geom::LineString* Segment)
{
  PolygonEdge* OldEdge = Entity->findEdgeLineIntersectingWith(*Segment);

  if (!OldEdge)
  {
    delete OldEdge;

    std::ostringstream s;
    s << "Error when removing segment (" << Segment->toString()
          << ") from Polygon " << Entity->getSelfId()
          << ": doesn't find edge intersection.";

    throw openfluid::base::FrameworkException(
        "PolygonGraph::removeSegment", s.str());
  }

  geos::geom::Geometry* DiffGeom = OldEdge->getLine()->difference(Segment);

  if (!DiffGeom->isEmpty())
  {
    if (DiffGeom->getDimension() != geos::geom::Dimension::L)
    {
      delete DiffGeom;
      delete OldEdge;

      std::ostringstream s;
      s << "Error when removing segment (" << Segment->toString()
            << ") from Polygon " << Entity->getSelfId()
            << ": difference geometry is not \"Line\" typed.";

      throw openfluid::base::FrameworkException(
          "PolygonGraph::removeSegment",
          s.str());
    }

    std::vector<geos::geom::LineString*>* DiffGeoms =
        LandRTools::getMergedLineStringsFromGeometry(DiffGeom);
    for (unsigned int i = 0; i < DiffGeoms->size(); i++)
    {
      PolygonEdge* NewEdge = createEdge(
          dynamic_cast<geos::geom::LineString&>(*DiffGeoms->at(i)));

      if (NewEdge)
        Entity->addEdge(*NewEdge);
    }
  }

  remove(OldEdge);

  Entity->removeEdge(OldEdge);
}

// =====================================================================
// =====================================================================

PolygonEntity* PolygonGraph::getEntity(int SelfId)
{
  return dynamic_cast<PolygonEntity*>(LandRGraph::getEntity(SelfId));
}

// =====================================================================
// =====================================================================

bool PolygonGraph::isComplete()
{
  for (LandRGraph::Entities_t::iterator it = m_Entities.begin();
      it != m_Entities.end(); ++it)
  {
    if (!(dynamic_cast<PolygonEntity*>(*it))->isComplete())
      return false;
  }

  return true;
}

// =====================================================================
// =====================================================================

bool PolygonGraph::hasIsland()
{

  for (LandRGraph::Entities_t::iterator it = m_Entities.begin();
      it != m_Entities.end(); ++it)
  {
    PolygonEntity*  Entity=dynamic_cast<PolygonEntity*>(*it);
    const geos::geom::Polygon* RefPoly = Entity->getPolygon();

    if (RefPoly->getNumInteriorRing ()!=0)
    {
      int NumInteriorRing=RefPoly->getNumInteriorRing ();

      for (int i=0;i<NumInteriorRing;i++)
      {
        const geos::geom::LineString *InnerRing=RefPoly->getInteriorRingN(i);
        for (LandRGraph::Entities_t::iterator it2 = m_Entities.begin();
            it2 != m_Entities.end(); ++it2)
        {
          PolygonEntity*  IslandEntity=dynamic_cast<PolygonEntity*>(*it2);
          if(Entity->getSelfId()!=IslandEntity->getSelfId())
          {
            const geos::geom::Polygon* IslandPoly = IslandEntity->getPolygon();
            const geos::geom::LineString *OuterRing=IslandPoly->getExteriorRing();
            if(InnerRing->within(OuterRing))
              return true;
          }
        }
      }

    }
  }
  return false;
}
// =====================================================================
// =====================================================================

PolygonGraph::RastValByRastPoly_t PolygonGraph::getRasterPolyOverlapping(
    PolygonEntity& Entity)
{
  const geos::geom::Polygon* RefPoly = Entity.getPolygon();

  RastValByRastPoly_t IntersectPolys;

  std::vector<geos::geom::Polygon*>* RasterPolys = getRasterPolygonizedPolys();

  if (!RasterPolys)
    throw openfluid::base::FrameworkException(
        "PolygonGraph::getRasterPolyOverlapping",
        "No RasterPolygonizedMultiPolygon associated to the PolygonGraph");

  for (std::vector<geos::geom::Polygon*>::iterator it = RasterPolys->begin();
      it != RasterPolys->end(); ++it)
  {
    if (RefPoly->relate(*it, "21*******"))
    {
      geos::geom::Geometry* Inter = RefPoly->intersection(*it);

      for (unsigned int i = 0; i < Inter->getNumGeometries(); i++)
      {
        geos::geom::Polygon* Poly =
            dynamic_cast<geos::geom::Polygon*>(const_cast<geos::geom::Geometry*>(Inter->getGeometryN(
                i)));

        // !! copy doesn't keep UserData !
        Poly->setUserData((*it)->getUserData());

        IntersectPolys[Poly] = Poly->getArea();
      }
    }
  }

  return IntersectPolys;
}

// =====================================================================
// =====================================================================

void PolygonGraph::setAttributeFromMeanRasterValues(std::string AttributeName)
{
  addAttribute(AttributeName);

  for (LandRGraph::Entities_t::iterator it = m_Entities.begin();
      it != m_Entities.end(); ++it)
  {
    PolygonGraph::RastValByRastPoly_t RastPolys = getRasterPolyOverlapping(
        *dynamic_cast<PolygonEntity*>(*it));

    float PolyArea = (*it)->getArea();

    if (!PolyArea)
      continue;

    float Mean = 0;

    for (PolygonGraph::RastValByRastPoly_t::iterator itPix = RastPolys.begin();
        itPix != RastPolys.end(); ++itPix)
    {
      double* PixelVal = ((double*) itPix->first->getUserData());

      if (!PixelVal)
      {
        std::ostringstream s;
        s << "No raster value for a raster pixel overlapping entity "
            << (*it)->getSelfId() << " .";

        throw openfluid::base::FrameworkException(
            "PolygonGraph::setAttributeFromMeanRasterValues", s.str());
      }

      float OverlappingArea = itPix->second;

      Mean += *PixelVal * (OverlappingArea / PolyArea);
    }

    (*it)->setAttributeValue(AttributeName, new core::DoubleValue(Mean));
  }
}

// =====================================================================
// =====================================================================

void PolygonGraph::createVectorRepresentation(std::string FilePath,
                                              std::string FileName)
{
  openfluid::landr::VectorDataset* OutVector =
      new openfluid::landr::VectorDataset("tmp_" + FileName);

  OutVector->addALayer("", wkbLineString);

  std::vector<geos::planargraph::Edge*>* Edges = getEdges();

  if (!Edges)
  {
    delete OutVector;

    throw openfluid::base::FrameworkException(
        "PolygonGraph::createVectorRepresentation",
        "No edges for this graph.");
  }

  for (std::vector<geos::planargraph::Edge*>::iterator it = Edges->begin();
      it != Edges->end(); ++it)
  {
    OGRFeature* Feat = OGRFeature::CreateFeature(OutVector->getLayerDef());

    geos::geom::Geometry* Geom =
        dynamic_cast<geos::geom::Geometry*>((dynamic_cast<PolygonEdge*>(*it))->getLine());

    OGRGeometry* OGRGeom = OGRGeometryFactory::createFromGEOS((GEOSGeom) Geom);

    if (!OGRGeom)
    {
      delete OutVector;

      throw openfluid::base::FrameworkException(
          "PolygonGraph::createVectorRepresentation",
          "Failed to transform geometry from GEOS to OGR.");
    }

    Feat->SetGeometry(OGRGeom);

    if (OutVector->getLayer(0)->CreateFeature(Feat) != OGRERR_NONE)
    {
      delete OutVector;

      throw openfluid::base::FrameworkException(
          "PolygonGraph::createVectorRepresentation",
          "Failed to create feature in shapefile.");
    }

    OGRFeature::DestroyFeature(Feat);
  }

  OutVector->copyToDisk(FilePath, FileName, true);

  delete OutVector;
}

// =====================================================================
// =====================================================================

void PolygonGraph::computeLineStringNeighbours(
    LineStringGraph& Graph, openfluid::landr::LandRTools::Relationship Relation,
    double BufferDistance)
{
  for (LandRGraph::Entities_t::iterator it = m_Entities.begin();
      it != m_Entities.end(); ++it)
    dynamic_cast<PolygonEntity*>(*it)->computeLineStringNeighbours(
        Graph, Relation, BufferDistance);
}

// =====================================================================
// =====================================================================

}// namespace landr
} /* namespace openfluid */
