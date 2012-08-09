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
 \file LandRGraph.cpp
 \brief Implements ...

 \author Aline LIBRES <aline.libres@gmail.com>
 */

#include "LandRGraph.hpp"

#include <openfluid/landr/LandREntity.hpp>
#include <openfluid/core/GeoVectorValue.hpp>
#include <openfluid/core/GeoRasterValue.hpp>
#include <openfluid/base/OFException.hpp>
#include <geos/planargraph/Node.h>
#include <geos/geom/Polygon.h>
#include <geos/geom/Point.h>
#include <geos/geom/GeometryFactory.h>

namespace openfluid {
namespace landr {

int LandRGraph::FileNum = 0;

// =====================================================================
// =====================================================================

LandRGraph::LandRGraph() :
    geos::planargraph::PlanarGraph(), mp_Vector(0), mp_Factory(
        geos::geom::GeometryFactory::getDefaultInstance()), mp_Raster(0), mp_RasterPolygonized(
        0), mp_RasterPolygonizedPolys(0)
{

}

// =====================================================================
// =====================================================================

LandRGraph::LandRGraph(openfluid::core::GeoVectorValue& Val) :
    geos::planargraph::PlanarGraph(), mp_Vector(&Val), mp_Factory(
        geos::geom::GeometryFactory::getDefaultInstance()), mp_Raster(0), mp_RasterPolygonized(
        0), mp_RasterPolygonizedPolys(0)
{
  if (!mp_Vector)
    throw openfluid::base::OFException("OpenFLUID Framework",
                                       "LandRGraph::LandRGraph",
                                       "No GeoVectorValue.");

  if (!mp_Vector->containsField("SELF_ID"))
    throw openfluid::base::OFException(
        "OpenFLUID Framework", "LandRGraph::LandRGraph",
        "GeoVector file must contain a \"SELF_ID\" field.");
}

// =====================================================================
// =====================================================================

LandRGraph::~LandRGraph()
{
  for (geos::planargraph::NodeMap::container::iterator it = nodeBegin();
      it != nodeEnd(); ++it)
    delete it->second;

  for (unsigned int i = 0; i < m_Entities.size(); i++)
    delete m_Entities[i];

  if (mp_RasterPolygonized)
  {
    mp_RasterPolygonized->deleteShpOnDisk();
    delete mp_RasterPolygonized;
  }

  if (mp_RasterPolygonizedPolys)
  {
    for (unsigned int i = 0; i < mp_RasterPolygonizedPolys->size(); i++)
      delete mp_RasterPolygonizedPolys->at(i);

    delete mp_RasterPolygonizedPolys;
  }

}

// =====================================================================
// =====================================================================

void LandRGraph::addEntitiesFromGeoVector()
{
  if (!mp_Vector)
    throw openfluid::base::OFException("OpenFLUID Framework",
                                       "LandRGraph::addEntitiesFromGeoVector",
                                       "No GeoVectorValue.");

  // TODO move to... ?
  setlocale(LC_NUMERIC, "C");

  OGRLayer* Layer0 =
      (const_cast<openfluid::core::GeoVectorValue*>(mp_Vector))->getLayer0();

  Layer0->ResetReading();

  OGRFeature* Feat;
  while ((Feat = Layer0->GetNextFeature()) != NULL)
  {
    OGRGeometry* OGRGeom = Feat->GetGeometryRef();

    // c++ cast doesn't work (have to use the C API instead)
    geos::geom::Geometry* GeosGeom =
        (geos::geom::Geometry*) OGRGeom->exportToGEOS();

    addEntity(
        getNewEntity(GeosGeom->clone(), Feat->GetFieldAsInteger("SELF_ID")));

    // destroying the feature destroys also the associated OGRGeom
    OGRFeature::DestroyFeature(Feat);
    delete GeosGeom;
  }

  removeUnusedNodes();
}

// =====================================================================
// =====================================================================

void LandRGraph::addEntitiesFromEntityList(
    const std::vector<LandREntity*>& Entities)
{
  for (std::vector<LandREntity*>::const_iterator it = Entities.begin();
      it != Entities.end(); ++it)
  {
    addEntity(getNewEntity((*it)->getGeometry()->clone(), (*it)->getSelfId()));
  }

  removeUnusedNodes();
}

// =====================================================================
// =====================================================================

geos::planargraph::Node* LandRGraph::getNode(
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

void LandRGraph::removeUnusedNodes()
{
  std::vector<geos::planargraph::Node*>* Unused = findNodesOfDegree(0);

  for (unsigned int i = 0; i < Unused->size(); i++)
    remove(Unused->at(i));

  delete Unused;
}

// =====================================================================
// =====================================================================

LandREntity* LandRGraph::getEntity(int SelfId)
{
  if (m_EntitiesBySelfId.count(SelfId))
    return m_EntitiesBySelfId.find(SelfId)->second;

  return (LandREntity*) 0;
}

// =====================================================================
// =====================================================================

std::vector<LandREntity*> LandRGraph::getEntities()
{
  return m_Entities;
}

// =====================================================================
// =====================================================================

std::vector<LandREntity*> LandRGraph::getSelfIdOrderedEntities()
{
  std::vector<LandREntity*> Entities;

  for (std::map<int, LandREntity*>::iterator it = m_EntitiesBySelfId.begin();
      it != m_EntitiesBySelfId.end(); ++it)
    Entities.push_back(it->second);

  return Entities;
}

// =====================================================================
// =====================================================================

std::map<int, LandREntity*> LandRGraph::getEntitiesBySelfId()
{
  return m_EntitiesBySelfId;
}

// =====================================================================
// =====================================================================

unsigned int LandRGraph::getSize()
{
  return m_Entities.size();
}

// =====================================================================
// =====================================================================

void LandRGraph::addAttribute(std::string AttributeName)
{
  for (std::vector<LandREntity*>::iterator it = m_Entities.begin();
      it != m_Entities.end(); ++it)
    (*it)->m_Attributes[AttributeName];
}

// =====================================================================
// =====================================================================

void LandRGraph::removeAttribute(std::string AttributeName)
{
  for (std::vector<LandREntity*>::iterator it = m_Entities.begin();
      it != m_Entities.end(); ++it)
    (*it)->m_Attributes.erase(AttributeName);
}

// =====================================================================
// =====================================================================

std::vector<std::string> LandRGraph::getAttributeNames()
{
  std::vector<std::string> Names;

  if (getSize() > 0)
  {
    std::map<std::string, boost::any> Attr = (*m_Entities.begin())->m_Attributes;

    for (std::map<std::string, boost::any>::iterator it = Attr.begin();
        it != Attr.end(); ++it)
      Names.push_back(it->first);
  }

  return Names;
}

// =====================================================================
// =====================================================================

void LandRGraph::addAGeoRasterValue(openfluid::core::GeoRasterValue& Raster)
{
  mp_Raster = &Raster;
  mp_RasterPolygonized = 0;
  mp_RasterPolygonizedPolys = 0;
}

// =====================================================================
// =====================================================================

bool LandRGraph::hasAnAssociatedRaster()
{
  return (mp_Raster != 0);
}

// =====================================================================
// =====================================================================

openfluid::core::GeoVectorValue* LandRGraph::getRasterPolygonized()
{
  if (!mp_RasterPolygonized)
  {
    if (!mp_Raster)
      throw openfluid::base::OFException(
          "OpenFLUID Framework", "PolygonGraph::getRasterPolygonized",
          "No raster associated to the PolygonGraph");

    std::ostringstream FileName;
    FileName << "Polygonized_" << FileNum++ << ".shp";

    mp_RasterPolygonized = mp_Raster->polygonize(mp_Raster->getFilePath(),
                                                 FileName.str());

    mp_RasterPolygonizedPolys = 0;
  }

  return mp_RasterPolygonized;
}

// =====================================================================
// =====================================================================

std::vector<geos::geom::Polygon*>* LandRGraph::getRasterPolygonizedPolys()
{
  if (!mp_RasterPolygonizedPolys)
  {
    openfluid::core::GeoVectorValue* Polygonized = getRasterPolygonized();

    if (!Polygonized)
      throw openfluid::base::OFException(
          "OpenFLUID Framework", "PolygonGraph::getRasterPolygonizedMultiPoly",
          "No RasterPolygonized associated to the PolygonGraph");

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

      Clone->setUserData(new int(Feat->GetFieldAsInteger(PixelValFieldIndex)));

      mp_RasterPolygonizedPolys->push_back(Clone);

      // destroying the feature destroys also the associated OGRGeom
      OGRFeature::DestroyFeature(Feat);
      delete GeosGeom;
    }

  }

  return mp_RasterPolygonizedPolys;
}

// =====================================================================
// =====================================================================

float* LandRGraph::getRasterValueForEntityCentroid(const LandREntity& Entity)
{
  float* Val = 0;

  if (!mp_Raster)
    throw openfluid::base::OFException(
        "OpenFLUID Framework", "PolygonGraph::getRasterValueForEntityCentroid",
        "No raster associated to the PolygonGraph");

  Val = new float(
      mp_Raster->getValueOfCoordinate(*Entity.getCentroide()->getCoordinate()));

  return Val;
}

// =====================================================================
// =====================================================================

void LandRGraph::setAttributeFromRasterValueAtCentroid(
    std::string AttributeName)
{
  addAttribute(AttributeName);

  for (std::vector<LandREntity*>::iterator it = m_Entities.begin();
      it != m_Entities.end(); ++it)
  {
    float* Val = getRasterValueForEntityCentroid(
        *const_cast<LandREntity*>(*it));

    if (!Val)
    {
      std::ostringstream s;
      s << "No raster value for entity " << (*it)->getSelfId() << " centroid.";

      throw openfluid::base::OFException(
          "OpenFLUID Framework",
          "PolygonGraph::setAttributeFromRasterValueAtCentroid", s.str());
    }

    (*it)->setAttributeValue(AttributeName, *Val);
  }

}

// =====================================================================
// =====================================================================

}// namespace landr
} /* namespace openfluid */
