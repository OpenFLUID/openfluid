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
 \file VectorDataset.cpp
 \brief Implements ...

 \author Aline LIBRES <aline.libres@gmail.com>
 */

#include "VectorDataset.hpp"

#include <utility>
#include <boost/filesystem/operations.hpp>
#include <boost/date_time/posix_time/posix_time.hpp>
#include <boost/date_time/posix_time/time_formatters.hpp>
#include <geos/geom/Geometry.h>
#include <geos/geom/GeometryFactory.h>
#include <geos/geom/LineString.h>
#include <geos/geom/GeometryCollection.h>
#include <geos/operation/valid/IsValidOp.h>
#include <openfluid/base/FrameworkException.hpp>
#include <openfluid/core/GeoVectorValue.hpp>
#include <openfluid/base/RuntimeEnv.hpp>


namespace openfluid { namespace landr {


// =====================================================================
// =====================================================================


VectorDataset::VectorDataset(const std::string& FileName)
{
  std::string DefaultDriverName="ESRI Shapefile";
  OGRRegisterAll();

  OGRDataSource  *poDS= OGRSFDriverRegistrar::Open( FileName.c_str(), FALSE );
  if (poDS!=NULL)
  {

    std::string DriverName=poDS->GetDriver()->GetName();
    if (DriverName!=DefaultDriverName)
    {
      OGRDataSource::DestroyDataSource( poDS );
      throw openfluid::base::FrameworkException(
          "VectorDataset::VectorDataset",
          "\"" + DriverName + "\" driver not supported.");
    }
  }
  OGRDataSource::DestroyDataSource( poDS );

  OGRSFDriver* Driver = OGRSFDriverRegistrar::GetRegistrar()->GetDriverByName(
      DefaultDriverName.c_str());

  if (!Driver)
    throw openfluid::base::FrameworkException(
        "VectorDataset::VectorDataset",
        "\"" + DefaultDriverName + "\" driver not available.");

  std::string Path = getTimestampedPath(FileName);

  mp_DataSource = Driver->CreateDataSource(Path.c_str(), NULL);

  if (!mp_DataSource)
    throw openfluid::base::FrameworkException(
        "VectorDataset::VectorDataset",
        "Error while creating " + Path + " : "
        + "Creation of OGRDataSource failed.");

  mp_DataSource->SetDriver(Driver);
}


// =====================================================================
// =====================================================================


VectorDataset::VectorDataset(openfluid::core::GeoVectorValue& Value)
{
  OGRRegisterAll();

  OGRDataSource* DS = Value.data();
  OGRSFDriver* Driver = DS->GetDriver();

  std::string DriverName=Driver->GetName();

  if (DriverName!="ESRI Shapefile")
    throw openfluid::base::FrameworkException(
        "VectorDataset::VectorDataset",
        "\"" + DriverName + "\" driver not supported.");

  std::string Path = getTimestampedPath(
      boost::filesystem::path(DS->GetName()).filename().string());

  mp_DataSource = Driver->CopyDataSource(DS, Path.c_str(), NULL);

  if (!mp_DataSource)
    throw openfluid::base::FrameworkException(
        "VectorDataset::VectorDataset",
        "Error while creating " + Path + " : "
        + "Creation of OGRDataSource failed.");

  mp_DataSource->SetDriver(Driver);

  // necessary to ensure headers are written out in an orderly way and all resources are recovered
  OGRDataSource::DestroyDataSource(mp_DataSource);

  mp_DataSource = OGRSFDriverRegistrar::Open(Path.c_str(), true);

  if (!mp_DataSource)
    throw openfluid::base::FrameworkException(
        "VectorDataset::VectorDataset",
        "Error while opening " + Path + " : "
        + "Creation of OGRDataSource failed.");
}


// =====================================================================
// =====================================================================


VectorDataset::VectorDataset(const VectorDataset& Other)
{
  OGRRegisterAll();

  OGRDataSource* DS = Other.source();
  OGRSFDriver* Driver = DS->GetDriver();

  std::string DriverName=Driver->GetName();

  if (DriverName!="ESRI Shapefile")
    throw openfluid::base::FrameworkException(
        "VectorDataset::VectorDataset",
        "\"" + DriverName + "\" driver not supported.");

  std::string Path = getTimestampedPath(
      boost::filesystem::path(DS->GetName()).filename().string());

  mp_DataSource = Driver->CopyDataSource(DS, Path.c_str(), NULL);

  if (!mp_DataSource)
    throw openfluid::base::FrameworkException(
        "VectorDataset::VectorDataset",
        "Error while creating " + Path + " : "
        + "Creation of OGRDataSource failed.");

  mp_DataSource->SetDriver(Driver);

  // necessary to ensure headers are written out in an orderly way and all resources are recovered
  OGRDataSource::DestroyDataSource(mp_DataSource);

  mp_DataSource = OGRSFDriverRegistrar::Open(Path.c_str(), true);

  if (!mp_DataSource)
    throw openfluid::base::FrameworkException(
        "VectorDataset::VectorDataset",
        "Error while opening " + Path + " : "
        + "Creation of OGRDataSource failed.");
}


// =====================================================================
// =====================================================================


std::string VectorDataset::getTimestampedPath(const std::string& OriginalFileName)
{
  boost::filesystem::path OriginalFile = boost::filesystem::path(
      OriginalFileName);
  std::string FileWOExt = OriginalFile.stem().string();
  std::string Ext = OriginalFile.extension().string();

  return openfluid::core::GeoValue::computeAbsolutePath(
      getInitializedTmpPath(),
      FileWOExt + "_"
      + boost::posix_time::to_iso_string(
          boost::posix_time::microsec_clock::local_time())
      + Ext);
}


// =====================================================================
// =====================================================================


std::string VectorDataset::getInitializedTmpPath()
{
  std::string TmpPath =
      openfluid::base::RuntimeEnvironment::instance()->getTempDir();

  if (!boost::filesystem::exists(TmpPath))
    boost::filesystem::create_directories(TmpPath);

  return TmpPath;
}


// =====================================================================
// =====================================================================


bool VectorDataset::isAlreadyExisting(const std::string& Path)
{
  OGRDataSource* DS = OGRSFDriverRegistrar::Open(Path.c_str(), false);

  if (DS)
  {
    OGRDataSource::DestroyDataSource(DS);
    return true;
  }

  return false;
}


// =====================================================================
// =====================================================================


VectorDataset::~VectorDataset()
{
  OGRSFDriver* Driver = mp_DataSource->GetDriver();
  std::string Path = mp_DataSource->GetName();

  // if the Datasource was not flushed to disk
  if (isAlreadyExisting(Path))
  {
    OGRDataSource::DestroyDataSource(mp_DataSource);
    Driver->DeleteDataSource(Path.c_str());
  }
}


// =====================================================================
// =====================================================================


OGRDataSource* VectorDataset::source()
{
  return mp_DataSource;
}


// =======================================y==============================
// =====================================================================


OGRDataSource* VectorDataset::source() const
{
  return mp_DataSource;
}


// =====================================================================
// =====================================================================


void VectorDataset::copyToDisk(const std::string& FilePath,
                               const std::string& FileName,
                               bool ReplaceIfExists)
{
  OGRSFDriver* Driver = mp_DataSource->GetDriver();

  if (!boost::filesystem::exists(FilePath))
    boost::filesystem::create_directories(FilePath);

  std::string Path = openfluid::core::GeoValue::computeAbsolutePath(FilePath,
                                                                    FileName);

  OGRDataSource* DS = OGRSFDriverRegistrar::Open(Path.c_str(), false);

  if (DS)
  {
    if (ReplaceIfExists)
    {
      OGRSFDriver* Driver = DS->GetDriver();
      OGRDataSource::DestroyDataSource(DS);
      Driver->DeleteDataSource(Path.c_str());
    }
    else
    {
      OGRDataSource::DestroyDataSource(DS);
      throw openfluid::base::FrameworkException(
          "VectorDataset::copyToDisk",
          "Error while creating " + Path + " : "
          + "This VectorDataset already exists.");
    }
  }

  OGRDataSource* NewDS = Driver->CopyDataSource(mp_DataSource, Path.c_str(),
                                                NULL);

  if (!NewDS)
    throw openfluid::base::FrameworkException(
        "VectorDataset::copyToDisk",
        "Error while creating " + Path + " : "
        + "Copying of OGRDataSource failed.");

  // necessary to ensure headers are written out in an orderly way and all resources are recovered
  OGRDataSource::DestroyDataSource(NewDS);
}


// =====================================================================
// =====================================================================


void VectorDataset::addALayer(std::string LayerName,
                              OGRwkbGeometryType LayerType,
                              OGRSpatialReference* SpatialRef)
{
  if (mp_DataSource->GetLayerByName(LayerName.c_str()) != NULL)
    throw openfluid::base::FrameworkException(
        "VectorDataset::addALayer",
        "Error while adding a layer to " + std::string(mp_DataSource->GetName())
        + ": a layer named " + LayerName + " already exists.");

  OGRLayer* Layer = mp_DataSource->CreateLayer(LayerName.c_str(), SpatialRef,
                                               LayerType, NULL);

  if (!Layer)
    throw openfluid::base::FrameworkException(
        "VectorDataset::addALayer",
        "Error while adding a layer to " + std::string(mp_DataSource->GetName())
        + ": creation of layer " + LayerName + " failed.");

  std::string Path = mp_DataSource->GetName();

  // necessary to ensure headers are written out in an orderly way and all resources are recovered
  OGRDataSource::DestroyDataSource(mp_DataSource);

  mp_DataSource = OGRSFDriverRegistrar::Open(Path.c_str(), true);

  if (!mp_DataSource)
    throw openfluid::base::FrameworkException(
        "VectorDataset::addALayer",
        "Error while opening " + Path + " : "
        + "Opening of OGRDataSource failed.");
}


// =====================================================================
// =====================================================================


OGRLayer* VectorDataset::layer(unsigned int LayerIndex)
{
  return mp_DataSource->GetLayer(LayerIndex);
}


// =====================================================================
// =====================================================================


OGRFeatureDefn* VectorDataset::layerDef(unsigned int LayerIndex)
{
  return layer(LayerIndex)->GetLayerDefn();
}


// =====================================================================
// =====================================================================


void VectorDataset::addAField(const std::string& FieldName,
                              OGRFieldType FieldType,
                              unsigned int LayerIndex)
{
  OGRFieldDefn Field(FieldName.c_str(), FieldType);

  if (layer(LayerIndex)->CreateField(&Field) != OGRERR_NONE)
    throw openfluid::base::FrameworkException(
        "VectorDataset::addAField",
        "Creating field \"" + FieldName + "\" failed.");
}


// =====================================================================
// =====================================================================


bool VectorDataset::isLineType(unsigned int LayerIndex)
{
  return layerDef(LayerIndex)->GetGeomType() == wkbLineString;
}


// =====================================================================
// =====================================================================


bool VectorDataset::isPolygonType(unsigned int LayerIndex)
{
  return layerDef(LayerIndex)->GetGeomType() == wkbPolygon;
}


// =====================================================================
// =====================================================================


bool VectorDataset::containsField(const std::string& FieldName,
                                  unsigned int LayerIndex)
{
  return layerDef(LayerIndex)->GetFieldIndex(FieldName.c_str()) != -1;
}


// =====================================================================
// =====================================================================


int VectorDataset::getFieldIndex(const std::string& FieldName,
                                 unsigned int LayerIndex)
{
  return layerDef(LayerIndex)->GetFieldIndex(FieldName.c_str());
}


// =====================================================================
// =====================================================================


bool VectorDataset::isFieldOfType(const std::string& FieldName,
                                  OGRFieldType FieldType,
                                  unsigned int LayerIndex)
{
  if (!containsField(FieldName))
    throw openfluid::base::FrameworkException(
        "VectorDataset::isFieldOfType",
        "Field \"" + FieldName + "\" is not set.");

  return layerDef(LayerIndex)->GetFieldDefn(getFieldIndex(FieldName))->GetType()
      == FieldType;
}


// =====================================================================
// =====================================================================


bool VectorDataset::isIntValueSet(const std::string& FieldName,
                                  int Value,
                                  unsigned int LayerIndex)
{
  if (!isFieldOfType(FieldName, OFTInteger, LayerIndex))
    throw openfluid::base::FrameworkException(
        "VectorDataset::isIntValueSet",
        "Field \"" + FieldName + "\" is not set or is not of type Int.");

  int CatIndex = layerDef(LayerIndex)->GetFieldIndex(FieldName.c_str());

  OGRLayer* Layer = layer(LayerIndex);

  Layer->ResetReading();

  OGRFeature* Feat;
  while ((Feat = Layer->GetNextFeature()) != NULL)
  {
    if (Feat->GetFieldAsInteger(CatIndex) == Value)
    {
      OGRFeature::DestroyFeature(Feat);
      return true;
    }
    OGRFeature::DestroyFeature(Feat);
  }

  return false;
}


// =====================================================================
// =====================================================================


VectorDataset::FeaturesList_t VectorDataset::getFeatures(
    unsigned int LayerIndex)
{
  if (!m_Features.count(LayerIndex))
    parse(LayerIndex);

  return m_Features.at(LayerIndex);
}


// =====================================================================
// =====================================================================


geos::geom::Geometry* VectorDataset::geometries(unsigned int LayerIndex)
{
  if (!m_Geometries.count(LayerIndex))
    parse(LayerIndex);

  return m_Geometries.at(LayerIndex);
}


// =====================================================================
// =====================================================================


// TODO add an option to allow choice of check validity or not (because it's time consuming)
void VectorDataset::parse(unsigned int LayerIndex)
{
  std::vector<geos::geom::Geometry*> Geoms;

  // TODO move to... ?
  setlocale(LC_NUMERIC, "C");

  OGRLayer* Layer = layer(LayerIndex);

  Layer->ResetReading();

  OGRFeature* Feat;

  FeaturesList_t List;
  m_Features.insert(std::make_pair(LayerIndex, List));

  // GetNextFeature returns a copy of the feature
  while ((Feat = Layer->GetNextFeature()) != NULL)
  {
    OGRGeometry* OGRGeom = Feat->GetGeometryRef();

    // c++ cast doesn't work (have to use the C API instead)
    geos::geom::Geometry* GeosGeom =
        (geos::geom::Geometry*) OGRGeom->exportToGEOS();

    geos::operation::valid::IsValidOp ValidOp(GeosGeom);

    if (!ValidOp.isValid())
      throw openfluid::base::FrameworkException(
          "VectorDataset::parse",
          ValidOp.getValidationError()->toString() + " \nwhile parsing "
          + GeosGeom->toString());

    geos::geom::Geometry* GeomClone = GeosGeom->clone();
    OGRFeature* FeatClone = Feat->Clone();

    Geoms.push_back(GeomClone);
    m_Features.at(LayerIndex).push_back(
        std::make_pair<OGRFeature*, geos::geom::Geometry*>(FeatClone,
                                                           GeomClone));

    // destroying the feature destroys also the associated OGRGeom
    OGRFeature::DestroyFeature(Feat);
    delete GeosGeom;
  }

  // ! do not use buildGeometry, because it may build a MultiPolygon if all geometries are Polygons, what may produce an invalid MultiPolygon!
  // (because the boundaries of any two Polygons of a valid MultiPolygon may touch, *but only at a finite number of points*)
  m_Geometries.insert(
      std::make_pair(
          LayerIndex,
          geos::geom::GeometryFactory::getDefaultInstance()->createGeometryCollection(
              Geoms)));

  geos::operation::valid::IsValidOp ValidOpColl(m_Geometries.at(LayerIndex));

  if (!ValidOpColl.isValid())
    throw openfluid::base::FrameworkException(
        "VectorDataset::parse",
        ValidOpColl.getValidationError()->toString() + " \nwhile creating "
        + m_Geometries.at(LayerIndex)->toString());
}


// =====================================================================
// =====================================================================


bool VectorDataset::isPointType(unsigned int LayerIndex)
{
  return layerDef(LayerIndex)->GetGeomType() == wkbPoint;
}


// =====================================================================
// =====================================================================



} } // namespaces openfluid, landr
