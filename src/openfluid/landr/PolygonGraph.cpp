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

int PolygonGraph::FileNum = 0;

// =====================================================================
// =====================================================================

PolygonGraph::PolygonGraph() :
    geos::planargraph::PlanarGraph(), mp_Factory(
        geos::geom::GeometryFactory::getDefaultInstance()), mp_Raster(0), mp_RasterPolygonized(
        0), mp_RasterPolygonizedPolys(0)
{

}

// =====================================================================
// =====================================================================

PolygonGraph::PolygonGraph(openfluid::landr::PolygonGraph& Other) :
    geos::planargraph::PlanarGraph(), mp_Factory(
        geos::geom::GeometryFactory::getDefaultInstance()), mp_Raster(0), mp_RasterPolygonized(
        0), mp_RasterPolygonizedPolys(0)
{
  std::vector<openfluid::landr::PolygonEntity*> OtherEntities =
      Other.getEntities();

  for (std::vector<openfluid::landr::PolygonEntity*>::iterator it =
      OtherEntities.begin(); it != OtherEntities.end(); ++it)
  {
    addPolygon(dynamic_cast<geos::geom::Polygon*>((*it)->getPolygon()->clone()),
               (*it)->getFeature()->Clone());
  }

  removeUnusedNodes();
}

// =====================================================================
// =====================================================================

PolygonGraph::PolygonGraph(const openfluid::core::GeoVectorValue& Val) :
    geos::planargraph::PlanarGraph(), mp_Factory(
        geos::geom::GeometryFactory::getDefaultInstance()), mp_Raster(0), mp_RasterPolygonized(
        0), mp_RasterPolygonizedPolys(0)
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

PolygonGraph::PolygonGraph(
    const std::vector<openfluid::landr::PolygonEntity*>& Entities) :
    geos::planargraph::PlanarGraph(), mp_Factory(
        geos::geom::GeometryFactory::getDefaultInstance()), mp_Raster(0), mp_RasterPolygonized(
        0), mp_RasterPolygonizedPolys(0)
{
  for (std::vector<openfluid::landr::PolygonEntity*>::const_iterator it =
      Entities.begin(); it != Entities.end(); ++it)
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

void PolygonGraph::deleteAll()
{
  unsigned int i;
  for (i = 0; i < m_NewNodes.size(); i++)
    delete m_NewNodes[i];
  for (i = 0; i < m_Entities.size(); i++)
    delete m_Entities[i];
  for (i = 0; i < m_NewDirEdges.size(); i++)
    delete m_NewDirEdges[i];

  if (mp_RasterPolygonized)
  {
    mp_RasterPolygonized->deleteShpOnDisk();
    delete mp_RasterPolygonized;
  }

  if (mp_RasterPolygonizedPolys)
  {
    for (i = 0; i < mp_RasterPolygonizedPolys->size(); i++)
      delete mp_RasterPolygonizedPolys->at(i);

    delete mp_RasterPolygonizedPolys;
  }
}

// =====================================================================
// =====================================================================

openfluid::landr::PolygonEntity* PolygonGraph::addPolygon(
    const geos::geom::Polygon* Polygon, OGRFeature* Feat)
{
  std::vector<geos::geom::Geometry*> SharedGeoms;

  openfluid::landr::PolygonEntity* NewEntity = new PolygonEntity(Polygon, Feat);

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
    for (std::vector<PolygonEntity*>::iterator it = m_Entities.begin();
        it != m_Entities.end(); ++it)
    {
      std::vector<geos::geom::LineString*> SharedLines =
          NewEntity->getLineIntersectionsWith(**it);

      for (unsigned int i = 0; i < SharedLines.size(); i++)
      {
        geos::geom::LineString* SharedLine = SharedLines[i];

        PolygonEdge* SharedEdge = createEdge(*SharedLine);

        NewEntity->addEdge(*SharedEdge);
        (*it)->addEdge(*SharedEdge);

        removeSegment(*it, SharedLine);

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
  m_NewDirEdges.push_back(DirectedEdge0);

  geos::planargraph::DirectedEdge* DirectedEdge1 =
      new geos::planargraph::DirectedEdge(
          EndNode, StartNode, Coordinates->getAt(Coordinates->getSize() - 2),
          false);
  m_NewDirEdges.push_back(DirectedEdge1);

  PolygonEdge* NewEdge = new PolygonEdge(LineString);

  NewEdge->setDirectedEdges(DirectedEdge0, DirectedEdge1);

  add(NewEdge);

  delete Coordinates;

  return NewEdge;
}

// =====================================================================
// =====================================================================

geos::planargraph::Node* PolygonGraph::getNode(
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

unsigned int PolygonGraph::getSize()
{
  return m_Entities.size();
}

// =====================================================================
// =====================================================================

openfluid::landr::PolygonEntity* PolygonGraph::getEntity(int SelfId)
{
  if (m_EntitiesBySelfId.count(SelfId))
    return m_EntitiesBySelfId.find(SelfId)->second;

  return (openfluid::landr::PolygonEntity*) 0;
}

// =====================================================================
// =====================================================================

std::vector<openfluid::landr::PolygonEntity*> PolygonGraph::getEntities()
{
  return m_Entities;
}

// =====================================================================
// =====================================================================

std::vector<openfluid::landr::PolygonEntity*> PolygonGraph::getSelfIdOrderedEntities()
{
  std::vector<PolygonEntity*> Entities;

  for (std::map<int, PolygonEntity*>::iterator it = m_EntitiesBySelfId.begin();
      it != m_EntitiesBySelfId.end(); ++it)
    Entities.push_back(it->second);

  return Entities;
}

// =====================================================================
// =====================================================================

std::map<int, openfluid::landr::PolygonEntity*> PolygonGraph::getEntitiesBySelfId()
{
  return m_EntitiesBySelfId;
}

// =====================================================================
// =====================================================================

void PolygonGraph::addAttribute(std::string AttributeName)
{
  for (std::vector<PolygonEntity*>::iterator it = m_Entities.begin();
      it != m_Entities.end(); ++it)
    (*it)->m_Attributes[AttributeName];
}

// =====================================================================
// =====================================================================

void PolygonGraph::removeAttribute(std::string AttributeName)
{
  for (std::vector<PolygonEntity*>::iterator it = m_Entities.begin();
      it != m_Entities.end(); ++it)
    (*it)->m_Attributes.erase(AttributeName);
}

// =====================================================================
// =====================================================================

bool PolygonGraph::isComplete()
{
  for (std::vector<PolygonEntity*>::iterator it = m_Entities.begin();
      it != m_Entities.end(); ++it)
  {
    if (!(*it)->isComplete())
      return false;
  }

  return true;
}

// =====================================================================
// =====================================================================

void PolygonGraph::removeUnusedNodes()
{
  std::vector<geos::planargraph::Node*>* Unused = findNodesOfDegree(0);

  for (unsigned int i = 0; i < Unused->size(); i++)
    remove(Unused->at(i));

  delete Unused;
}

// =====================================================================
// =====================================================================

void PolygonGraph::addAGeoRasterValue(openfluid::core::GeoRasterValue& Raster)
{
  mp_Raster = &Raster;
  mp_RasterPolygonized = 0;
  mp_RasterPolygonizedPolys = 0;
}

// =====================================================================
// =====================================================================

float* PolygonGraph::getRasterValueForEntityCentroid(PolygonEntity& Entity)
{
  float* Val = 0;

  if (!mp_Raster)
    throw openfluid::base::OFException(
        "OpenFLUID Framework", "PolygonGraph::getRasterValueForEntityCentroid",
        "No raster associated to the PolygonGraph");
  else
  {
    Val = new float(
        mp_Raster->getValueOfCoordinate(
            *Entity.getCentroide()->getCoordinate()));
  }

  return Val;
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

openfluid::core::GeoVectorValue* PolygonGraph::getRasterPolygonized()
{
  if (!mp_RasterPolygonized)
  {
    if (!mp_Raster)
      throw openfluid::base::OFException(
          "OpenFLUID Framework", "PolygonGraph::getRasterPolygonized",
          "No raster associated to the PolygonGraph");
    else
    {
      std::ostringstream FileName;
      FileName << "Polygonized_" << FileNum++ << ".shp";

      mp_RasterPolygonized = mp_Raster->polygonize(mp_Raster->getFilePath(),
                                                   FileName.str());

      mp_RasterPolygonizedPolys = 0;
    }
  }

  return mp_RasterPolygonized;
}

// =====================================================================
// =====================================================================

std::vector<geos::geom::Polygon*>* PolygonGraph::getRasterPolygonizedPolys()
{
  if (!mp_RasterPolygonizedPolys)
  {
    openfluid::core::GeoVectorValue* Polygonized = getRasterPolygonized();

    if (!Polygonized)
      throw openfluid::base::OFException(
          "OpenFLUID Framework", "PolygonGraph::getRasterPolygonizedMultiPoly",
          "No RasterPolygonized associated to the PolygonGraph");
    else
    {
      mp_RasterPolygonizedPolys = new std::vector<geos::geom::Polygon*>();

      // TODO move?
      setlocale(LC_NUMERIC, "C");

      OGRLayer* Layer0 = Polygonized->getLayer0();

      int PixelValFieldIndex = Polygonized->getFieldIndex(
          openfluid::core::GeoRasterValue::getDefaultPolygonizedFieldName());

      Layer0->ResetReading();

      OGRFeature* Feat;
      while ((Feat = Layer0->GetNextFeature()) != NULL)
      {
        OGRGeometry* OGRGeom = Feat->GetGeometryRef();

        // c++ cast doesn't work (have to use the C API instead)
        geos::geom::Geometry* GeosGeom =
            (geos::geom::Geometry*) OGRGeom->exportToGEOS();

        geos::geom::Polygon* Clone =
            dynamic_cast<geos::geom::Polygon*>(GeosGeom->clone());

        Clone->setUserData(
            new int(Feat->GetFieldAsInteger(PixelValFieldIndex)));

        mp_RasterPolygonizedPolys->push_back(Clone);

        // destroying the feature destroys also the associated OGRGeom
        OGRFeature::DestroyFeature(Feat);
        delete GeosGeom;
      }

    }
  }

  return mp_RasterPolygonizedPolys;
}

// =====================================================================
// =====================================================================

}// namespace landr
} /* namespace openfluid */
