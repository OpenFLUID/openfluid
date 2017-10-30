/*

  This file is part of OpenFLUID software
  Copyright(c) 2007, INRA - Montpellier SupAgro


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
  along with OpenFLUID. If not, see <http://www.gnu.org/licenses/>.


 == Other Usage ==

  Other Usage means a use of OpenFLUID that is inconsistent with the GPL
  license, and requires a written agreement between You and INRA.
  Licensees for Other Usage of OpenFLUID may use this file in accordance
  with the terms contained in the written agreement between You and INRA.

 */

/**
  @file LandRGraph.cpp

  @author Aline LIBRES <aline.libres@gmail.com>
  @author Michael RABOTIN <michael.rabotin@supagro.inra.fr>
 */


#include <sstream>

#include <geos/planargraph/Node.h>
#include <geos/geom/Polygon.h>
#include <geos/geom/Point.h>
#include <geos/geom/LineString.h>
#include <geos/geom/LineSegment.h>
#include <geos/geom/GeometryFactory.h>
#include <geos/operation/overlay/snap/GeometrySnapper.h>

#include <openfluid/landr/LandRGraph.hpp>
#include <openfluid/landr/GEOSHelpers.hpp>
#include <openfluid/landr/LandREntity.hpp>
#include <openfluid/landr/LineStringEntity.hpp>
#include <openfluid/landr/VectorDataset.hpp>
#include <openfluid/landr/RasterDataset.hpp>
#include <openfluid/core/DoubleValue.hpp>
#include <openfluid/core/IntegerValue.hpp>
#include <openfluid/core/StringValue.hpp>
#include <openfluid/core/GeoVectorValue.hpp>
#include <openfluid/base/FrameworkException.hpp>


namespace openfluid { namespace landr {


int LandRGraph::m_FileNum = 0;


// =====================================================================
// =====================================================================


LandRGraph::LandRGraph() :
  geos::planargraph::PlanarGraph(),
  mp_Vector(nullptr), mp_Factory(geos::geom::GeometryFactory::getDefaultInstance()),
  mp_Raster(nullptr), mp_RasterPolygonized(nullptr), mp_RasterPolygonizedPolys(nullptr)
{

}


// =====================================================================
// =====================================================================


LandRGraph::LandRGraph(openfluid::core::GeoVectorValue& Val) :
  geos::planargraph::PlanarGraph(),
  mp_Factory(geos::geom::GeometryFactory::getDefaultInstance()),
  mp_Raster(nullptr), mp_RasterPolygonized(nullptr), mp_RasterPolygonizedPolys(nullptr)
{
  mp_Vector = new VectorDataset(Val);

  if (!mp_Vector)
    throw openfluid::base::FrameworkException(OPENFLUID_CODE_LOCATION,"No GeoVectorValue");

  if (!mp_Vector->containsField("OFLD_ID"))
    throw openfluid::base::FrameworkException(OPENFLUID_CODE_LOCATION,
                                              "GeoVector file must contain a \"OFLD_ID\" field");

  std::list<OGRFeature*>  lDuplicate=mp_Vector->hasDuplicateGeometry();
  if (!lDuplicate.empty())
  {
    std::ostringstream s;
    s << "error : GeoVectorValue has duplicate Geometry - OFLD_ID: ";

    std::list<OGRFeature*>::iterator it=lDuplicate.begin();
    std::list<OGRFeature*>::iterator ite=lDuplicate.end();

    for (;it!=ite;++it)
      s <<(*it)->GetFieldAsInteger("OFLD_ID")<<" ";

    throw openfluid::base::FrameworkException(OPENFLUID_CODE_LOCATION, s.str());
  }
}


// =====================================================================
// =====================================================================


LandRGraph::LandRGraph(const openfluid::landr::VectorDataset& Vect) :
        geos::planargraph::PlanarGraph(), mp_Factory(geos::geom::GeometryFactory::getDefaultInstance()),
        mp_Raster(nullptr), mp_RasterPolygonized(nullptr), mp_RasterPolygonizedPolys(nullptr)
{
  mp_Vector = new openfluid::landr::VectorDataset(Vect);

  if (!mp_Vector)
    throw openfluid::base::FrameworkException(
        OPENFLUID_CODE_LOCATION,
        "No VectorDataset.");

  if (!mp_Vector->containsField("OFLD_ID"))
    throw openfluid::base::FrameworkException(
        OPENFLUID_CODE_LOCATION,
        "VectorDataset file must contain a \"OFLD_ID\" field.");

  std::list<OGRFeature*> lDuplicate=mp_Vector->hasDuplicateGeometry();

  if (!lDuplicate.empty())
  {
    std::ostringstream s;
    s << "error : VectorDataset has duplicate Geometry - OFLD_ID: ";

    std::list<OGRFeature*>::iterator it=lDuplicate.begin();
    std::list<OGRFeature*>::iterator ite=lDuplicate.end();

    for (;it!=ite;++it)
      s <<(*it)->GetFieldAsInteger("OFLD_ID")<<" ";

    throw openfluid::base::FrameworkException(OPENFLUID_CODE_LOCATION, s.str());
  }
}


// =====================================================================
// =====================================================================


LandRGraph::~LandRGraph()
{
  geos::planargraph::NodeMap::container::iterator it = nodeBegin();
  geos::planargraph::NodeMap::container::iterator ite = nodeEnd();
  for (; it != ite; ++it)
    delete it->second;

  LandRGraph::Entities_t::iterator itE = m_Entities.begin();
  LandRGraph::Entities_t::iterator itEe = m_Entities.end();
  for (; itE != itEe; ++itE)
    delete (*itE);

  delete mp_RasterPolygonized;

  if (mp_RasterPolygonizedPolys)
  {
    unsigned int mpSize = mp_RasterPolygonizedPolys->size();

    for (unsigned int i = 0; i < mpSize; i++)
      delete mp_RasterPolygonizedPolys->at(i);

    delete mp_RasterPolygonizedPolys;
  }

  delete mp_Vector;
  delete mp_Raster;
}


// =====================================================================
// =====================================================================


void LandRGraph::addEntitiesFromGeoVector()
{
  if (!mp_Vector)
    throw openfluid::base::FrameworkException(OPENFLUID_CODE_LOCATION,"No GeoVectorValue");

  // TODO move to... ?
  setlocale(LC_NUMERIC, "C");

  OGRLayer* Layer0 = mp_Vector->layer(0);

  Layer0->ResetReading();

  OGRFeature* Feat;
  while ((Feat = Layer0->GetNextFeature()) != nullptr)
  {
    OGRGeometry* OGRGeom = Feat->GetGeometryRef();
    if(!OGRGeom->IsValid())
    {
      std::ostringstream s;
      s << "Error when exporting OGR Geometry into GEOS geometry";

      throw openfluid::base::FrameworkException(OPENFLUID_CODE_LOCATION, s.str());
    }

    // c++ cast doesn't work (have to use C-style casting instead)
    geos::geom::Geometry* GeosGeom = (geos::geom::Geometry*) openfluid::landr::convertOGRGeometryToGEOS(OGRGeom);

    addEntity(createNewEntity(GeosGeom->clone(), Feat->GetFieldAsInteger("OFLD_ID")));

    // destroying the feature destroys also the associated OGRGeom
   delete GeosGeom;
   OGRFeature::DestroyFeature(Feat);
  }

 removeUnusedNodes();
}


// =====================================================================
// =====================================================================


void LandRGraph::addEntitiesFromEntityList(const LandRGraph::Entities_t& Entities)
{
  LandRGraph::Entities_t::const_iterator it = Entities.begin();
  LandRGraph::Entities_t::const_iterator ite = Entities.end();

  for (; it != ite; ++it)
    addEntity(createNewEntity((*it)->geometry()->clone(), (*it)->getOfldId()));

  removeUnusedNodes();
}


// =====================================================================
// =====================================================================


geos::planargraph::Node* LandRGraph::node(const geos::geom::Coordinate& Coordinate)
{
  geos::planargraph::Node* Node = findNode(Coordinate);

  if (Node == nullptr)
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

  unsigned int UnSize=Unused->size();
  for (unsigned int i = 0; i < UnSize; i++)
    remove(Unused->at(i));

  delete Unused;

}


// =====================================================================
// =====================================================================


LandREntity* LandRGraph::entity(int OfldId)
{
  if (m_EntitiesByOfldId.count(OfldId))
    return m_EntitiesByOfldId.find(OfldId)->second;

  return nullptr;
}


// =====================================================================
// =====================================================================


std::list<LandREntity*> LandRGraph::getEntities()
{
  return m_Entities;
}


// =====================================================================
// =====================================================================


LandRGraph::Entities_t LandRGraph::getOfldIdOrderedEntities()
{
  LandRGraph::Entities_t Entities;

  std::map<int, LandREntity*>::iterator it = m_EntitiesByOfldId.begin();
  std::map<int, LandREntity*>::iterator ite = m_EntitiesByOfldId.end();

  for (; it != ite; ++it)
    Entities.push_back(it->second);

  return Entities;
}


// =====================================================================
// =====================================================================


std::map<int, LandREntity*> LandRGraph::getEntitiesByOfldId()
{
  return m_EntitiesByOfldId;
}


// =====================================================================
// =====================================================================


unsigned int LandRGraph::getSize() const
{
  return m_Entities.size();
}


// =====================================================================
// =====================================================================


void LandRGraph::addAttribute(const std::string& AttributeName)
{
  LandRGraph::Entities_t::iterator it = m_Entities.begin();
  LandRGraph::Entities_t::iterator ite = m_Entities.end();

  for (; it != ite; ++it)
  {
    if (!(*it)->m_Attributes.count(AttributeName))
      (*it)->m_Attributes[AttributeName] = 0;
  }
}


// =====================================================================
// =====================================================================


void LandRGraph::removeAttribute(const std::string& AttributeName)
{
  LandRGraph::Entities_t::iterator it = m_Entities.begin();
  LandRGraph::Entities_t::iterator ite = m_Entities.end();

  for (;it != ite; ++it)
  {
    delete (*it)->m_Attributes[AttributeName];
    (*it)->m_Attributes.erase(AttributeName);
  }
}


// =====================================================================
// =====================================================================


std::vector<std::string> LandRGraph::getAttributeNames()
{
  std::vector<std::string> Names;

  if (getSize() > 0)
  {
    std::map<std::string, core::Value*> Attr = (*m_Entities.begin())->m_Attributes;

    std::map<std::string, core::Value*>::iterator it = Attr.begin();
    std::map<std::string, core::Value*>::iterator ite = Attr.end();
    for (; it != ite; ++it)
      Names.push_back(it->first);
  }

  return Names;
}


// =====================================================================
// =====================================================================


void LandRGraph::addAGeoRasterValue(openfluid::core::GeoRasterValue& Raster)
{
  if (mp_Raster)
    delete mp_Raster;

  mp_Raster = new RasterDataset(Raster);

  mp_RasterPolygonized = nullptr;
  mp_RasterPolygonizedPolys = nullptr;
}


// =====================================================================
// =====================================================================


void LandRGraph::addAGeoRasterValue(const openfluid::landr::RasterDataset& Raster)
{
  if (mp_Raster)
    delete mp_Raster;

  mp_Raster = new RasterDataset(Raster);

  mp_RasterPolygonized = nullptr;
  mp_RasterPolygonizedPolys = nullptr;
}


// =====================================================================
// =====================================================================


bool LandRGraph::hasAnAssociatedRaster()
{
  return (mp_Raster != nullptr);
}


// =====================================================================
// =====================================================================


openfluid::landr::VectorDataset* LandRGraph::rasterPolygonized()
{
  if (!mp_RasterPolygonized)
  {
    if (!mp_Raster)
      throw openfluid::base::FrameworkException(OPENFLUID_CODE_LOCATION,"No raster associated to the PolygonGraph");

    std::ostringstream FileName;
    FileName << "Polygonized_" << m_FileNum++ << ".shp";

    mp_RasterPolygonized = mp_Raster->polygonize(FileName.str());
    mp_RasterPolygonizedPolys = nullptr;
  }

  return mp_RasterPolygonized;
}


// =====================================================================
// =====================================================================


std::vector<geos::geom::Polygon*>* LandRGraph::rasterPolygonizedPolys()
{
  if (!mp_RasterPolygonizedPolys)
  {
    openfluid::landr::VectorDataset* Polygonized = rasterPolygonized();

    if (!Polygonized)
      throw openfluid::base::FrameworkException(OPENFLUID_CODE_LOCATION,
                                                "No RasterPolygonized associated to the PolygonGraph");

    mp_RasterPolygonizedPolys = new std::vector<geos::geom::Polygon*>();

    // TODO move?
    setlocale(LC_NUMERIC, "C");

    OGRLayer* Layer0 = Polygonized->layer(0);

    int PixelValFieldIndex = Polygonized->getFieldIndex(
        openfluid::landr::RasterDataset::getDefaultPolygonizedFieldName());

    Layer0->ResetReading();

    OGRFeature* Feat;
    while ((Feat = Layer0->GetNextFeature()) != nullptr)
    {
      OGRGeometry* OGRGeom = Feat->GetGeometryRef();

      // c++ cast doesn't work (have to use C-style casting instead)
      geos::geom::Geometry* GeosGeom = (geos::geom::Geometry*) openfluid::landr::convertOGRGeometryToGEOS(OGRGeom);

      geos::geom::Polygon* Clone = dynamic_cast<geos::geom::Polygon*>(GeosGeom->clone());

      Clone->setUserData(new double(Feat->GetFieldAsDouble(PixelValFieldIndex)));

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


double LandRGraph::getRasterValueForEntityCentroid(const LandREntity& Entity)
{

  if (!mp_Raster)
    throw openfluid::base::FrameworkException(OPENFLUID_CODE_LOCATION,"No raster associated to the PolygonGraph");

  double  Val = (double)(mp_Raster->getValueOfCoordinate(*Entity.centroid()->getCoordinate()));

  return Val;
}


// =====================================================================
// =====================================================================


void LandRGraph::setAttributeFromRasterValueAtCentroid(const std::string& AttributeName)
{
  addAttribute(AttributeName);

  LandRGraph::Entities_t::iterator it = m_Entities.begin();
  LandRGraph::Entities_t::iterator ite = m_Entities.end();
  for (; it != ite; ++it)
  {
    double* Val = new double (getRasterValueForEntityCentroid(*const_cast<LandREntity*>(*it)));

    if (!Val)
    {
      std::ostringstream s;
      s << "No raster value for entity " << (*it)->getOfldId() << " centroid.";

      throw openfluid::base::FrameworkException(OPENFLUID_CODE_LOCATION, s.str());
    }

    (*it)->setAttributeValue(AttributeName, new core::DoubleValue(*Val));
    delete Val;
  }
}


// =====================================================================
// =====================================================================


void LandRGraph::computeNeighbours()
{
  LandRGraph::Entities_t::iterator it = m_Entities.begin();
  LandRGraph::Entities_t::iterator ite = m_Entities.end();

  for (; it != ite; ++it)
    (*it)->computeNeighbours();

}


// =====================================================================
// =====================================================================


void LandRGraph::exportToShp(const std::string& FilePath,const std::string& FileName)
{
  openfluid::landr::VectorDataset* Out = new openfluid::landr::VectorDataset(FileName);

  switch (getType())
  {
    case POLYGON:
      Out->addALayer("", wkbPolygon);
      break;
    case LINESTRING:
      Out->addALayer("", wkbLineString);
      break;
    default:
      delete Out;
      throw openfluid::base::FrameworkException(OPENFLUID_CODE_LOCATION,
                                                "Unable to export graph as a shapefile as it is not POLYGON, "
                                                "nor LINESTRING typed");
      break;
  }

  Out->addAField("OFLD_ID", OFTInteger);

  Entities_t::iterator it = m_Entities.begin();
  Entities_t::iterator ite = m_Entities.end();

  for (; it != ite; ++it)
  {
    OGRFeature *Feat = OGRFeature::CreateFeature(Out->layerDef());

    Feat->SetField("OFLD_ID", (int) (*it)->getOfldId());

    OGRGeometry* OGRGeom = openfluid::landr::convertGEOSGeometryToOGR((GEOSGeom) (*it)->geometry());

    if (!OGRGeom)
    {
      delete Out;
      delete OGRGeom;
      throw openfluid::base::FrameworkException(OPENFLUID_CODE_LOCATION,
                                                "Failed to transform geometry from GEOS to OGR.");
    }

    Feat->SetGeometry(OGRGeom);

    if (Out->layer(0)->CreateFeature(Feat) != OGRERR_NONE)
    {
      delete Out;
      delete OGRGeom;
      throw openfluid::base::FrameworkException(OPENFLUID_CODE_LOCATION,"Failed to create feature in shapefile");
    }

    OGRFeature::DestroyFeature(Feat);
    delete OGRGeom;
  }

  Out->copyToDisk(FilePath, FileName, true);

  delete Out;
}


// =====================================================================
// =====================================================================


void LandRGraph::setAttributeFromVectorId(const std::string& AttributeName,
                                          openfluid::core::GeoVectorValue& Vector,
                                          const std::string& IdColumn,
                                          const std::string& ValueColumn)
{
  if (!Vector.containsField(ValueColumn))
  {
    std::ostringstream s;
    s << "Unable to find the column " << ValueColumn << " in GeoVector.";

    throw openfluid::base::FrameworkException(OPENFLUID_CODE_LOCATION, s.str());
  }

  if (!Vector.containsField(IdColumn))
  {
    std::ostringstream s;
    s << "Unable to find the column " << IdColumn << " in GeoVector.";

    throw openfluid::base::FrameworkException(OPENFLUID_CODE_LOCATION, s.str());
  }

  addAttribute(AttributeName);

  setlocale(LC_NUMERIC, "C");

  OGRLayer* Layer0 = Vector.layer(0);
  Layer0->ResetReading();

  int columnIndex=Vector.getFieldIndex(ValueColumn);
  OGRFeature* Feat;
  while ((Feat = Layer0->GetNextFeature()) != nullptr)
  {
    int OfldId=Feat->GetFieldAsInteger(IdColumn.c_str());
    openfluid::landr::LandREntity* Entity=entity(OfldId);

    if (Entity)
    {
      if (Vector.isFieldOfType(ValueColumn, OFTInteger) || Vector.isFieldOfType(ValueColumn,GDALOFTInteger64_COMPAT))
      {
        int value=Feat->GetFieldAsInteger(columnIndex);
        Entity->setAttributeValue(AttributeName, new openfluid::core::IntegerValue(value));
      }
      else if (Vector.isFieldOfType(ValueColumn, OFTReal))
      {
        double value=Feat->GetFieldAsDouble(columnIndex);
        Entity->setAttributeValue(AttributeName, new openfluid::core::DoubleValue(value));
      }
      else
      {
        std::string value=Feat->GetFieldAsString(columnIndex);
        Entity->setAttributeValue(AttributeName, new openfluid::core::StringValue(value));
      }
    }

    // destroying the feature
    OGRFeature::DestroyFeature(Feat);
  }
}


// =====================================================================
// =====================================================================


void LandRGraph::setAttributeFromVectorId(const std::string& AttributeName,
                                          openfluid::landr::VectorDataset& Vector,
                                          const std::string& IdColumn,
                                          const std::string& ValueColumn)
{
  if (!Vector.containsField(ValueColumn))
  {
    std::ostringstream s;
    s << "Unable to find the column " << ValueColumn << " in VectorDataset.";

    throw openfluid::base::FrameworkException(OPENFLUID_CODE_LOCATION, s.str());
  }

  if (!Vector.containsField(IdColumn))
  {
    std::ostringstream s;
    s << "Unable to find the column " << IdColumn << " in VectorDataset.";
    throw openfluid::base::FrameworkException(OPENFLUID_CODE_LOCATION, s.str());
  }

  addAttribute(AttributeName);

  setlocale(LC_NUMERIC, "C");

  OGRLayer* Layer0 = Vector.layer(0);
  Layer0->ResetReading();

  int columnIndex=Vector.getFieldIndex(ValueColumn);
  OGRFeature* Feat;
  while ((Feat = Layer0->GetNextFeature()) != nullptr)
  {
    int OfldId=Feat->GetFieldAsInteger(IdColumn.c_str());
    openfluid::landr::LandREntity* Entity=entity(OfldId);
    if (Entity)
    {
      if (Vector.isFieldOfType(ValueColumn, OFTInteger) || Vector.isFieldOfType(ValueColumn,GDALOFTInteger64_COMPAT))
      {
        int value=Feat->GetFieldAsInteger(columnIndex);
        Entity->setAttributeValue(AttributeName, new openfluid::core::IntegerValue(value));
      }
      else if (Vector.isFieldOfType(ValueColumn, OFTReal))
      {
        double value=Feat->GetFieldAsDouble(columnIndex);
        Entity->setAttributeValue(AttributeName, new openfluid::core::DoubleValue(value));
      }
      else
      {
        std::string value=Feat->GetFieldAsString(columnIndex);
        Entity->setAttributeValue(AttributeName, new openfluid::core::StringValue(value));
      }
    }

    // destroying the feature
    OGRFeature::DestroyFeature(Feat);
  }
}


// =====================================================================
// =====================================================================


void LandRGraph::snapVertices(double snapTolerance)
{
  LandRGraph::Entities_t::iterator it = m_Entities.begin();
  LandRGraph::Entities_t::iterator ite = m_Entities.end();
  std::list<int> listOfldId;
  for (; it != ite; ++it)
    listOfldId.push_back((*it)->getOfldId());

  std::list<int>::iterator li=listOfldId.begin();
  std::list<int>::iterator lie=listOfldId.end();

  for (; li != lie; ++li)
  {
    std::vector<geos::geom::Geometry*> entitiesGeoms;
    LandRGraph::Entities_t::iterator jt = m_Entities.begin();
    LandRGraph::Entities_t::iterator jte = m_Entities.end();

    for (; jt != jte; ++jt)
    {
      if (static_cast<unsigned>(*li)!=(*jt)->getOfldId())
        entitiesGeoms.push_back(const_cast<geos::geom::Geometry*>((*jt)->geometry()));
    }

    geos::geom::Geometry* entitiesGeom =
      geos::geom::GeometryFactory::getDefaultInstance()->buildGeometry(entitiesGeoms);

    geos::operation::overlay::snap::GeometrySnapper geomSnapper(*(entity(*li))->geometry());
    std::unique_ptr<geos::geom::Geometry> snapEntityGeom=geomSnapper.snapTo(*entitiesGeom,snapTolerance);
    geos::geom::Geometry* snappedEntityGeom=snapEntityGeom.release();

    removeEntity(*(li));
    addEntity(createNewEntity(snappedEntityGeom,(*li)));

    removeUnusedNodes();

    delete entitiesGeom;
  }
}



// =====================================================================
// =====================================================================


void LandRGraph::setAttributeFromVectorLocation(const std::string& AttributeName,
                                                openfluid::core::GeoVectorValue& Vector,
                                                const std::string& Column,
                                                double Thresh)
{
  if (!Vector.isPolygonType()&&!Vector.isLineType())
    throw openfluid::base::FrameworkException(OPENFLUID_CODE_LOCATION,"Vector is not Line nor Polygon type");


  if (!Vector.containsField(Column))
  {
    std::ostringstream s;
    s << "Unable to find the column " << Column << " in GeoVector.";

    throw openfluid::base::FrameworkException(OPENFLUID_CODE_LOCATION, s.str());
  }

  addAttribute(AttributeName);

  setlocale(LC_NUMERIC, "C");

  OGRLayer* Layer0 = Vector.layer(0);
  Layer0->ResetReading();

  int columnIndex=Vector.getFieldIndex(Column);


  LandRGraph::Entities_t::iterator it = m_Entities.begin();
  LandRGraph::Entities_t::iterator ite = m_Entities.end();

  for (; it != ite; ++it)
  {
    geos::geom::Point* IntPoint;
    if((*it)->geometry()->getDimension() == 1)
    {
      const geos::geom::LineString* Line = dynamic_cast<openfluid::landr::LineStringEntity*>(*it)->line();
      const geos::geom::Coordinate FirstCoord = Line->getCoordinateN(0);
      const geos::geom::Coordinate SecondCoord = Line->getCoordinateN(1);
      geos::geom::LineSegment LineSegment(FirstCoord,SecondCoord);
      geos::geom::Coordinate CoordInteriorPoint;
      LineSegment.midPoint(CoordInteriorPoint);
      IntPoint = mp_Factory->createPoint(CoordInteriorPoint);
    }
    else
      IntPoint=(*it)->geometry()->getInteriorPoint();

    OGRFeature* Feat;
    while ((Feat = Layer0->GetNextFeature()) != nullptr)
    {
      OGRGeometry* OGRGeom = Feat->GetGeometryRef();

      // c++ cast doesn't work (have to use C-style casting instead)
      geos::geom::Geometry* GeosGeom =
          (geos::geom::Geometry*)openfluid::landr::convertOGRGeometryToGEOS(OGRGeom);

      if (IntPoint->isWithinDistance(GeosGeom,Thresh))
      {
        if (Vector.isFieldOfType(Column, OFTInteger) || Vector.isFieldOfType(Column,GDALOFTInteger64_COMPAT))
        {
          int value=Feat->GetFieldAsInteger(columnIndex);
          (*it)->setAttributeValue(AttributeName, new openfluid::core::IntegerValue(value));
        }
        else if (Vector.isFieldOfType(Column, OFTReal))
        {
          double value=Feat->GetFieldAsDouble(columnIndex);
          (*it)->setAttributeValue(AttributeName, new openfluid::core::DoubleValue(value));
        }
        else
        {
          std::string value=Feat->GetFieldAsString(columnIndex);
          (*it)->setAttributeValue(AttributeName, new openfluid::core::StringValue(value));
        }
      }
      // destroying the feature destroys also the associated OGRGeom
      OGRFeature::DestroyFeature(Feat);
      delete GeosGeom;
    }

    Layer0->ResetReading();
  }
}


// =====================================================================
// =====================================================================


void LandRGraph::setAttributeFromVectorLocation(const std::string& AttributeName,
                                                openfluid::landr::VectorDataset& Vector,
                                                const std::string& Column,
                                                double Thresh)
{
  if (!Vector.isPolygonType()&&!Vector.isLineType())
    throw openfluid::base::FrameworkException(OPENFLUID_CODE_LOCATION,"Vector is not Line nor Polygon type");


  if (!Vector.containsField(Column))
  {
    std::ostringstream s;
    s << "Unable to find the column " << Column << " in GeoVector.";

    throw openfluid::base::FrameworkException(OPENFLUID_CODE_LOCATION, s.str());
  }

  addAttribute(AttributeName);

  setlocale(LC_NUMERIC, "C");

  OGRLayer* Layer0 = Vector.layer(0);
  Layer0->ResetReading();

  int columnIndex=Vector.getFieldIndex(Column);


  LandRGraph::Entities_t::iterator it = m_Entities.begin();
  LandRGraph::Entities_t::iterator ite = m_Entities.end();


  for (; it != ite; ++it)
  {
    geos::geom::Point* IntPoint;
    if((*it)->geometry()->getDimension() == 1)
    {
      const geos::geom::LineString* Line = dynamic_cast<openfluid::landr::LineStringEntity*>(*it)->line();
      const geos::geom::Coordinate FirstCoord = Line->getCoordinateN(0);
      const geos::geom::Coordinate SecondCoord = Line->getCoordinateN(1);
      geos::geom::LineSegment LineSegment(FirstCoord,SecondCoord);
      geos::geom::Coordinate CoordInteriorPoint;
      LineSegment.midPoint(CoordInteriorPoint);
      IntPoint = mp_Factory->createPoint(CoordInteriorPoint);
    }
    else
      IntPoint=(*it)->geometry()->getInteriorPoint();

    OGRFeature* Feat;
    while ((Feat = Layer0->GetNextFeature()) != nullptr)
    {
      OGRGeometry* OGRGeom = Feat->GetGeometryRef();

      // c++ cast doesn't work (have to use the C API instead)
      geos::geom::Geometry* GeosGeom =
          (geos::geom::Geometry*) openfluid::landr::convertOGRGeometryToGEOS(OGRGeom);


      if(IntPoint->isWithinDistance(GeosGeom,Thresh))
      {
        if (Vector.isFieldOfType(Column, OFTInteger) || Vector.isFieldOfType(Column,GDALOFTInteger64_COMPAT))
        {
          int value=Feat->GetFieldAsInteger(columnIndex);
          (*it)->setAttributeValue(AttributeName, new openfluid::core::IntegerValue(value));
        }
        else if (Vector.isFieldOfType(Column, OFTReal))
        {
          double value=Feat->GetFieldAsDouble(columnIndex);
          (*it)->setAttributeValue(AttributeName, new openfluid::core::DoubleValue(value));
        }
        else
        {
          std::string value=Feat->GetFieldAsString(columnIndex);
          (*it)->setAttributeValue(AttributeName, new openfluid::core::StringValue(value));
        }

      }
      // destroying the feature destroys also the associated OGRGeom
      OGRFeature::DestroyFeature(Feat);
      delete GeosGeom;
    }
    Layer0->ResetReading();
  }
}


} }  // namespaces
