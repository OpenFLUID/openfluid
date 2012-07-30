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

#include <openfluid/base/OFException.hpp>
#include <openfluid/base/RuntimeEnv.hpp>
#include <openfluid/core/GeoRasterValue.hpp>
#include <openfluid/core/GeoVectorValue.hpp>
#include <openfluid/landr/PolygonEdge.hpp>
#include <openfluid/landr/PolygonEntity.hpp>
#include <geos/geom/CoordinateSequence.h>
#include <geos/geom/LineString.h>
#include <geos/geom/Polygon.h>
#include <geos/geom/Point.h>
#include <geos/geom/Location.h>
#include <geos/geom/MultiLineString.h>
#include <geos/geom/GeometryFactory.h>
#include <geos/planargraph/DirectedEdge.h>
#include <geos/planargraph/Node.h>
#include <geos/operation/linemerge/LineMerger.h>

namespace openfluid {
namespace landr {

// =====================================================================
// =====================================================================

PolygonGraph::PolygonGraph()
{

}

// =====================================================================
// =====================================================================

PolygonGraph::PolygonGraph(PolygonGraph& Other)
{
  std::vector<LandREntity*> OtherEntities = Other.getEntities();

  for (std::vector<LandREntity*>::iterator it = OtherEntities.begin();
      it != OtherEntities.end(); ++it)
  {
    addPolygon(
        dynamic_cast<geos::geom::Polygon*>((dynamic_cast<PolygonEntity*>(*it))->getPolygon()->clone()),
        (*it)->getFeature()->Clone());
  }

  removeUnusedNodes();
}

// =====================================================================
// =====================================================================

PolygonGraph::PolygonGraph(const openfluid::core::GeoVectorValue& Val)
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

    addPolygon(dynamic_cast<const geos::geom::Polygon*>(GeosGeom->clone()),
               Feat->Clone());

    // destroying the feature destroys also the associated OGRGeom
    OGRFeature::DestroyFeature(Feat);
    delete GeosGeom;
  }

  removeUnusedNodes();

}

// =====================================================================
// =====================================================================

PolygonGraph::PolygonGraph(const std::vector<PolygonEntity*>& Entities)
{
  for (std::vector<PolygonEntity*>::const_iterator it = Entities.begin();
      it != Entities.end(); ++it)
  {
    addPolygon(
        dynamic_cast<const geos::geom::Polygon*>((*it)->getPolygon()->clone()),
        (*it)->getFeature()->Clone());
  }

  removeUnusedNodes();
}

// =====================================================================
// =====================================================================

PolygonGraph::~PolygonGraph()
{
  deleteAll();
}

// =====================================================================
// =====================================================================

PolygonEntity* PolygonGraph::addPolygon(const geos::geom::Polygon* Polygon,
                                        OGRFeature* Feat)
{
  std::vector<geos::geom::Geometry*> SharedGeoms;

  PolygonEntity* NewEntity = new PolygonEntity(Polygon, Feat);

  if (!Polygon->isValid())
  {
    std::ostringstream s;
    s << "Error in Graph creation: Polygon " << NewEntity->getSelfId()
      << " is not valid.";

    throw openfluid::base::OFException("OpenFLUID Framework",
                                       "PolygonGraph::addPolygon", s.str());

    deleteAll();
  }

  try
  {
    for (std::vector<LandREntity*>::iterator it = m_Entities.begin();
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
          getMergedLineStringsFromGeometry(DiffGeom);

      assert(NewLines);

      for (unsigned int i = 0; i < NewLines->size(); i++)
      {
        PolygonEdge* NewEdge = createEdge(*NewLines->at(i));

        if (NewEdge)
          NewEntity->addEdge(*NewEdge);
      }

    }

    m_EntitiesBySelfId[NewEntity->getSelfId()] = NewEntity;
    m_Entities.push_back(NewEntity);

  } catch (openfluid::base::OFException& e)
  {
    std::cerr << e.what() << std::endl;
    throw;
  }

  return NewEntity;
}

// =====================================================================
// =====================================================================

geos::geom::LineString* PolygonGraph::getMergedLineStringFromGeometry(
    geos::geom::Geometry* Geom)
{
  geos::geom::LineString* LS = 0;

  std::vector<geos::geom::LineString*>* Lines =
      getMergedLineStringsFromGeometry(Geom);

  if (!Lines || Lines->size() != 1)
  {
    for (unsigned int i = 0; i < Lines->size(); i++)
      delete Lines->at(i);
  } else
    LS = *Lines->begin();

  delete Lines;

  return LS;
}

// =====================================================================
// =====================================================================

std::vector<geos::geom::LineString*>* PolygonGraph::getMergedLineStringsFromGeometry(
    geos::geom::Geometry* Geom)
{
  std::vector<geos::geom::LineString*>* LS = 0;

  geos::operation::linemerge::LineMerger Merger;

  switch (Geom->getGeometryTypeId())
  {
    case geos::geom::GEOS_LINESTRING:
      LS = new std::vector<geos::geom::LineString*>();
      LS->push_back(dynamic_cast<geos::geom::LineString*>(Geom->clone()));
      break;
    case geos::geom::GEOS_MULTILINESTRING:
    case geos::geom::GEOS_LINEARRING:
    case geos::geom::GEOS_GEOMETRYCOLLECTION:
      Merger.add(Geom);
      LS = Merger.getMergedLineStrings();
      break;
    default:
      break;
  }

  return LS;
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
    std::ostringstream s;
    s << "Error when removing segment (" << Segment->toString()
      << ") from Polygon " << Entity->getSelfId()
      << ": doesn't find edge intersection.";

    throw openfluid::base::OFException("OpenFLUID Framework",
                                       "PolygonGraph::removeSegment", s.str());

    delete OldEdge;
    deleteAll();

    return;
  }

  geos::geom::Geometry* DiffGeom = OldEdge->getLine()->difference(Segment);

  if (!DiffGeom->isEmpty())
  {
    if (DiffGeom->getDimension() != geos::geom::Dimension::L)
    {
      std::ostringstream s;
      s << "Error when removing segment (" << Segment->toString()
        << ") from Polygon " << Entity->getSelfId()
        << ": difference geometry is not \"Line\" typed.";

      throw openfluid::base::OFException("OpenFLUID Framework",
                                         "PolygonGraph::removeSegment",
                                         s.str());

      delete DiffGeom;
      delete OldEdge;
      deleteAll();

      return;
    }

    std::vector<geos::geom::LineString*>* DiffGeoms =
        getMergedLineStringsFromGeometry(DiffGeom);
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

void PolygonGraph::doRemoveEntity(LandREntity* Entity)
{
  //TODO
}

// =====================================================================
// =====================================================================

void PolygonGraph::doDeleteAll()
{
  for (unsigned int i = 0; i < edges.size(); i++)
    delete edges[i];
}

// =====================================================================
// =====================================================================

bool PolygonGraph::isComplete()
{
  for (std::vector<LandREntity*>::iterator it = m_Entities.begin();
      it != m_Entities.end(); ++it)
  {
    if (!(dynamic_cast<PolygonEntity*>(*it))->isComplete())
      return false;
  }

  return true;
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
    throw openfluid::base::OFException(
        "OpenFLUID Framework", "PolygonGraph::getRasterPolyOverlaying",
        "No RasterPolygonizedMultiPolygon associated to the PolygonGraph");
  else
  {
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

  }

  return IntersectPolys;
}

// =====================================================================
// =====================================================================

void PolygonGraph::setAttributeFromMeanRasterValues(std::string AttributeName)
{
  addAttribute(AttributeName);

  for (std::vector<LandREntity*>::iterator it = m_Entities.begin();
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
      // get as integer because GeoRasterValue::polygonize function currently only deal with integer values
      int* PixelVal = ((int*) itPix->first->getUserData());

      if (!PixelVal)
      {
        std::ostringstream s;
        s << "No raster value for a raster pixel overlapping entity "
          << (*it)->getSelfId() << " .";

        throw openfluid::base::OFException(
            "OpenFLUID Framework",
            "PolygonGraph::setAttributeFromMeanRasterValues", s.str());
        return;
      }

      float OverlappingArea = itPix->second;

      Mean += *PixelVal * (OverlappingArea / PolyArea);
    }

    (*it)->setAttributeValue(AttributeName, Mean);
  }

}

// =====================================================================
// =====================================================================

void PolygonGraph::createVectorRepresentation(std::string FilePath,
                                              std::string FileName)
{
  openfluid::core::GeoVectorValue* OutVector =
      new openfluid::core::GeoVectorValue(FilePath, FileName);

  OutVector->createShp(wkbLineString);

  std::vector<geos::planargraph::Edge*>* Edges = getEdges();

  if (!Edges)
  {
    throw openfluid::base::OFException(
        "OpenFLUID Framework", "PolygonGraph::createVectorRepresentation",
        "No edges for this graph.");

    OutVector->deleteShpOnDisk();
    delete OutVector;

    return;
  }

  for (std::vector<geos::planargraph::Edge*>::iterator it = Edges->begin();
      it != Edges->end(); ++it)
  {
    OGRFeature* Feat = OGRFeature::CreateFeature(OutVector->getLayerDef());

    geos::geom::Geometry* Geom =
        dynamic_cast<geos::geom::Geometry*>((dynamic_cast<PolygonEdge*>(*it))->getLine());

    OGRGeometry* OGRGeom = OGRGeometryFactory::createFromGEOS((GEOSGeom) Geom);

    if (!OGRGeom)
      if (!Edges)
      {
        throw openfluid::base::OFException(
            "OpenFLUID Framework", "PolygonGraph::createVectorRepresentation",
            "Failed to transform geometry from GEOS to OGR.");

        OutVector->deleteShpOnDisk();
        delete OutVector;

        return;
      }

    Feat->SetGeometry(OGRGeom);

    if (OutVector->getLayer0()->CreateFeature(Feat) != OGRERR_NONE)
    {
      throw openfluid::base::OFException(
          "OpenFLUID Framework", "PolygonGraph::createVectorRepresentation",
          "Failed to create feature in shapefile.");

      OutVector->deleteShpOnDisk();
      delete OutVector;

      return;
    }

    OGRFeature::DestroyFeature(Feat);
  }

  delete OutVector;
}

// =====================================================================
// =====================================================================

}// namespace landr
} /* namespace openfluid */
