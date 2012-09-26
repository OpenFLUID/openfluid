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
 \file GeoVectorValue.cpp
 \brief Implements ...

 \author Aline LIBRES <libres@supagro.inra.fr>
 */

#include "GeoVectorValue.hpp"

#include <openfluid/base/OFException.hpp>
#include <geos/geom/Geometry.h>
#include <geos/geom/GeometryFactory.h>
#include <geos/geom/LineString.h>
#include <utility>

#include <geos/operation/valid/IsValidOp.h>
#include <geos/geom/GeometryCollection.h>

namespace openfluid {
namespace core {

// =====================================================================
// =====================================================================

GeoVectorValue::GeoVectorValue(std::string FilePath, std::string FileName) :
    GeoValue(FilePath, FileName), mp_ShpDriverName("ESRI Shapefile"), mp_ShpDriver(
        0), mp_Data(0), mp_Layer(0), mp_LayerDef(0), mp_Geometries(0)
{
  OGRRegisterAll();
}

// =====================================================================
// =====================================================================

GeoVectorValue::~GeoVectorValue()
{
  destroyDataSource();
}

// =====================================================================
// =====================================================================

void GeoVectorValue::destroyDataSource()
{
  if (mp_Data)
  {
    OGRDataSource::DestroyDataSource(mp_Data);
    mp_Data = 0;
  }
}

// =====================================================================
// =====================================================================

openfluid::core::UnstructuredValue::UnstructuredType GeoVectorValue::getType() const
{
  return openfluid::core::UnstructuredValue::GeoVectorValue;
}

// =====================================================================
// =====================================================================

OGRDataSource* GeoVectorValue::get(bool UpdateMode)
{
  if (!mp_Data)
    tryToOpenSource(UpdateMode);

  return mp_Data;
}

// =====================================================================
// =====================================================================

void GeoVectorValue::tryToOpenSource(bool UpdateMode)
{
  mp_Data = OGRSFDriverRegistrar::Open(m_AbsolutePath.c_str(), UpdateMode);

  if (!mp_Data)
    throw openfluid::base::OFException(
        "OpenFLUID framework", "GeoVectorValue::tryToOpenSource",
        "Error while trying to open file " + m_AbsolutePath);

  mp_Layer = mp_Data->GetLayer(0);

  if (!mp_Layer)
  {
    destroyDataSource();

    throw openfluid::base::OFException(
        "OpenFLUID framework", "GeoVectorValue::tryToOpenSource",
        "Unable to get first layer for " + m_AbsolutePath);
  }

  mp_LayerDef = mp_Layer->GetLayerDefn();
}

// =====================================================================
// =====================================================================

void GeoVectorValue::createShp(OGRwkbGeometryType LayerType,
                               OGRSpatialReference* SpatialRef,
                               bool ReplaceIfExists)
{
  if (isAlreadyExisting())
  {
    if (ReplaceIfExists)
      deleteShpOnDisk();
    else
      throw openfluid::base::OFException(
          "OpenFLUID framework",
          "GeoVectorValue::createShp",
          "Error while trying to create file " + m_AbsolutePath
          + " : file already exists.");
  }

  mp_Data = tryToGetShpDriver()->CreateDataSource(m_AbsolutePath.c_str(), NULL);

  if (!mp_Data)
    throw openfluid::base::OFException(
        "OpenFLUID framework",
        "GeoVectorValue::createShp",
        "Error while creating " + m_AbsolutePath + " : "
        + "Creation of output file failed.");

  mp_Layer = mp_Data->CreateLayer("", SpatialRef, LayerType, NULL);

  if (!mp_Layer)
  {
    destroyDataSource();

    throw openfluid::base::OFException(
        "OpenFLUID framework",
        "GeoVectorValue::createShp",
        "Error while creating " + m_AbsolutePath + " : "
        + "Creation of first layer failed.");
  }

  // necessary to ensure headers are written out in an orderly way and all resources are recovered
  destroyDataSource();

  tryToOpenSource(true);
}

// =====================================================================
// =====================================================================

void GeoVectorValue::copyShp(GeoVectorValue& Source, bool ReplaceIfExists)
{
  if (isAlreadyExisting())
  {
    if (ReplaceIfExists)
      deleteShpOnDisk();
    else
      throw openfluid::base::OFException(
          "OpenFLUID framework",
          "GeoVectorValue::copyShp",
          "Error while trying to create file " + m_AbsolutePath
          + " : file already exists.");
  }

  mp_Data = tryToGetShpDriver()->CreateDataSource(m_AbsolutePath.c_str(), NULL);

  if (!mp_Data)
    throw openfluid::base::OFException(
        "OpenFLUID framework",
        "GeoVectorValue::copyShp",
        "Error while creating " + m_AbsolutePath + " : "
        + "Creation of output file failed.");

  mp_Layer = mp_Data->CopyLayer(Source.getLayer0(), "", NULL);

  if (!mp_Layer)
  {
    destroyDataSource();

    throw openfluid::base::OFException(
        "OpenFLUID framework",
        "GeoVectorValue::copyShp",
        "Error while creating " + m_AbsolutePath + " : "
        + "Copy of first layer failed.");
  }

  // necessary to ensure headers are written out in an orderly way and all resources are recovered
  destroyDataSource();

  tryToOpenSource(true);
}

// =====================================================================
// =====================================================================

OGRSFDriver* GeoVectorValue::tryToGetShpDriver()
{
  if (!mp_ShpDriver)
  {
    mp_ShpDriver = OGRSFDriverRegistrar::GetRegistrar()->GetDriverByName(
        mp_ShpDriverName.c_str());

    if (!mp_ShpDriver)
      throw openfluid::base::OFException(
          "OpenFLUID framework", "GeoVectorValue::tryToGetShpDriver",
          "\"" + mp_ShpDriverName + "\" driver not available.");
  }

  return mp_ShpDriver;
}

// =====================================================================
// =====================================================================

bool GeoVectorValue::isAlreadyExisting()
{
  if (mp_Data)
    return true;

  OGRDataSource* p_Data = OGRSFDriverRegistrar::Open(m_AbsolutePath.c_str(),
                                                     false);

  if (p_Data)
  {
    OGRDataSource::DestroyDataSource(p_Data);
    return true;
  }

  return false;
}

// =====================================================================
// =====================================================================

void GeoVectorValue::deleteShpOnDisk()
{
  if (isAlreadyExisting())
  {
    destroyDataSource();

    tryToGetShpDriver()->DeleteDataSource(m_AbsolutePath.c_str());
  }
}

// =====================================================================
// =====================================================================

OGRLayer* GeoVectorValue::getLayer0()
{
  if (!mp_Data)
    get();

  return mp_Layer;
}

// =====================================================================
// =====================================================================

OGRFeatureDefn* GeoVectorValue::getLayerDef()
{
  return getLayer0()->GetLayerDefn();
}

// =====================================================================
// =====================================================================

void GeoVectorValue::addAField(std::string FieldName, OGRFieldType FieldType)
{
  OGRFieldDefn Field(FieldName.c_str(), FieldType);

  if (getLayer0()->CreateField(&Field) != OGRERR_NONE)
    throw openfluid::base::OFException(
        "OpenFLUID framework", "GeoVectorValue::addAField",
        "Creating field \"" + FieldName + "\" failed.");
}

// =====================================================================
// =====================================================================

bool GeoVectorValue::isLineType()
{
  return getLayerDef()->GetGeomType() == wkbLineString;
}

// =====================================================================
// =====================================================================

bool GeoVectorValue::isPolygonType()
{
  return getLayerDef()->GetGeomType() == wkbPolygon;
}

// =====================================================================
// =====================================================================

bool GeoVectorValue::containsField(std::string FieldName)
{
  return getLayerDef()->GetFieldIndex(FieldName.c_str()) != -1;
}

// =====================================================================
// =====================================================================

int GeoVectorValue::getFieldIndex(std::string FieldName)
{
  return getLayerDef()->GetFieldIndex(FieldName.c_str());
}

// =====================================================================
// =====================================================================

bool GeoVectorValue::isFieldOfType(std::string FieldName,
                                   OGRFieldType FieldType)
{
  if (!containsField(FieldName))
    throw openfluid::base::OFException(
        "OpenFLUID framework", "GeoVectorValue::isFieldOfType",
        "Field \"" + FieldName + "\" is not set.");

  return getLayerDef()->GetFieldDefn(getFieldIndex(FieldName))->GetType()
         == FieldType;
}

// =====================================================================
// =====================================================================

bool GeoVectorValue::isIntValueSet(std::string FieldName, int Value)
{
  if (!isFieldOfType(FieldName, OFTInteger))
    throw openfluid::base::OFException(
        "OpenFLUID framework", "GeoVectorValue::isIntValueSet",
        "Field \"" + FieldName + "\" is not set or is not of type Int.");

  int CatIndex = getLayerDef()->GetFieldIndex(FieldName.c_str());

  mp_Layer->ResetReading();

  OGRFeature* Feat;
  while ((Feat = mp_Layer->GetNextFeature()) != NULL)
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

GeoVectorValue::FeaturesList_t GeoVectorValue::getFeatures()
{
  if (!mp_Geometries)
    parse();

  return m_Features;
}

// =====================================================================
// =====================================================================

geos::geom::Geometry* GeoVectorValue::getGeometries()
{
  if (!mp_Geometries)
    parse();

  return mp_Geometries;
}

// =====================================================================
// =====================================================================

// TODO add an option to allow choice of check validity or not (because it's time consuming)
void GeoVectorValue::parse()
{
  std::vector<geos::geom::Geometry*> Geoms;

  // TODO move to... ?
  setlocale(LC_NUMERIC, "C");

  OGRLayer* Layer0 = getLayer0();

  Layer0->ResetReading();

  OGRFeature* Feat;

  // GetNextFeature returns a copy of the feature
  while ((Feat = Layer0->GetNextFeature()) != NULL)
  {
    OGRGeometry* OGRGeom = Feat->GetGeometryRef();

    // c++ cast doesn't work (have to use the C API instead)
    geos::geom::Geometry* GeosGeom =
        (geos::geom::Geometry*) OGRGeom->exportToGEOS();

    geos::operation::valid::IsValidOp ValidOp(GeosGeom);

    if (!ValidOp.isValid())
      throw openfluid::base::OFException(
          "OpenFLUID framework",
          "GeoVectorValue::parse",
          ValidOp.getValidationError()->toString() + " \nwhile parsing "
          + GeosGeom->toString());

    geos::geom::Geometry* GeomClone = GeosGeom->clone();
    OGRFeature* FeatClone = Feat->Clone();

    Geoms.push_back(GeomClone);
    m_Features.push_back(
        std::make_pair<OGRFeature*, geos::geom::Geometry*>(FeatClone,
                                                           GeomClone));

    // destroying the feature destroys also the associated OGRGeom
    OGRFeature::DestroyFeature(Feat);
    delete GeosGeom;
  }

  // ! do not use buildGeometry, because it may build a MultiPolygon if all geometries are Polygons, what may produce an invalid MultiPolygon!
  // (because the boundaries of any two Polygons of a valid MultiPolygon may touch, *but only at a finite number of points*)
  mp_Geometries =
      geos::geom::GeometryFactory::getDefaultInstance()->createGeometryCollection(
          Geoms);

  geos::operation::valid::IsValidOp ValidOpColl(mp_Geometries);

  if (!ValidOpColl.isValid())
    throw openfluid::base::OFException(
        "OpenFLUID framework",
        "GeoVectorValue::parse",
        ValidOpColl.getValidationError()->toString() + " \nwhile creating "
        + mp_Geometries->toString());
}

// =====================================================================
// =====================================================================

}
} // namespaces
