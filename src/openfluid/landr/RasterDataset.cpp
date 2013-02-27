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
 \file RasterDataset.cpp
 \brief Implements ...

 \author Aline LIBRES <aline.libres@gmail.com>
 */

#include "RasterDataset.hpp"

#include <string.h>
#include <gdal_alg.h>
#include <boost/filesystem/path.hpp>
#include <geos/geom/Coordinate.h>
#include <openfluid/core/GeoRasterValue.hpp>
#include <openfluid/base/OFException.hpp>
#include <openfluid/landr/VectorDataset.hpp>

namespace openfluid {
namespace landr {

// =====================================================================
// =====================================================================

RasterDataset::RasterDataset(openfluid::core::GeoRasterValue& Value) :
    mp_GeoTransform(0)
{
  GDALAllRegister();

  GDALDataset* DS = Value.get(false);

  // GDAL supports many raster formats for reading, but significantly less formats for writing
  // (see http://www.gdal.org/gdal_vrttut.html)
  GDALDriver* Driver = static_cast<GDALDriver*>(GDALGetDriverByName("VRT"));
  mp_Dataset = Driver->CreateCopy("", DS, true, NULL, NULL, NULL);

  if (!mp_Dataset)
  {
    throw openfluid::base::OFException(
        "OpenFLUID framework",
        "RasterDataset::RasterDataset",
        "Error while creating a virtual copy of " + Value.getAbsolutePath()
        + " (" + CPLGetLastErrorMsg() + ")");
  }
}

// =====================================================================
// =====================================================================

RasterDataset::RasterDataset(const RasterDataset& Other) :
    mp_GeoTransform(0)
{
  GDALAllRegister();

  GDALDataset* DS = Other.getDataset();

  // GDAL supports many raster formats for reading, but significantly less formats for writing
  // (see http://www.gdal.org/gdal_vrttut.html)
  GDALDriver* Driver = static_cast<GDALDriver*>(GDALGetDriverByName("VRT"));
  mp_Dataset = Driver->CreateCopy("", DS, true, NULL, NULL, NULL);

  if (!mp_Dataset)
  {
    throw openfluid::base::OFException(
        "OpenFLUID framework",
        "RasterDataset::RasterDataset",
        "Error while creating a virtual copy (" + std::string(
            CPLGetLastErrorMsg())
        + ")");
  }
}

// =====================================================================
// =====================================================================

RasterDataset::~RasterDataset()
{
  GDALClose(mp_Dataset);
  delete mp_GeoTransform;
}

// =====================================================================
// =====================================================================

GDALDataset* RasterDataset::getDataset()
{
  return mp_Dataset;
}

// =====================================================================
// =====================================================================

GDALDataset* RasterDataset::getDataset() const
{
  return mp_Dataset;
}

// =====================================================================
// =====================================================================

GDALRasterBand* RasterDataset::getRasterBand(unsigned int RasterBandIndex)
{
  return mp_Dataset->GetRasterBand(RasterBandIndex);
}

// =====================================================================
// =====================================================================

std::pair<int, int> RasterDataset::getPixelFromCoordinate(
    geos::geom::Coordinate Coo)
{
  int offsetX = int((Coo.x - getOrigin()->x) / getPixelWidth());
  int offsetY = int((Coo.y - getOrigin()->y) / getPixelHeight());

  return std::make_pair(offsetX, offsetY);
}

// =====================================================================
// =====================================================================

geos::geom::Coordinate* RasterDataset::getOrigin()
{
  if (!mp_GeoTransform)
    computeGeoTransform();

  return new geos::geom::Coordinate(mp_GeoTransform[0], mp_GeoTransform[3]);
}

// =====================================================================
// =====================================================================

void RasterDataset::computeGeoTransform()
{
  mp_GeoTransform = new double[6];

  if (GDALGetGeoTransform(mp_Dataset, mp_GeoTransform) != CE_None)
    throw openfluid::base::OFException(
        "OpenFLUID framework", "RasterDataset::computeGeoTransform",
        "Error while getting GeoTransform information");
}

// =====================================================================
// =====================================================================

double RasterDataset::getPixelWidth()
{
  if (!mp_GeoTransform)
    computeGeoTransform();

  return mp_GeoTransform[1];
}

// =====================================================================
// =====================================================================

double RasterDataset::getPixelHeight()
{
  if (!mp_GeoTransform)
    computeGeoTransform();

  return mp_GeoTransform[5];
}

// =====================================================================
// =====================================================================

std::vector<float> RasterDataset::getValuesOfLine(int LineIndex,
                                                  unsigned int RasterBandIndex)
{
  std::vector<float> Val;

  int ColumnCount = getRasterBand(RasterBandIndex)->GetXSize();

  float* ScanLine = (float *) CPLMalloc(sizeof(float) * ColumnCount);

  getRasterBand(RasterBandIndex)->RasterIO(GF_Read, 0, LineIndex, ColumnCount,
                                           1, ScanLine, ColumnCount, 1,
                                           GDT_Float32, 0, 0);

  for (int i = 0; i < ColumnCount; i++)
    Val.push_back(ScanLine[i]);

  CPLFree(ScanLine);

  return Val;
}

// =====================================================================
// =====================================================================

std::vector<float> RasterDataset::getValuesOfColumn(
    int ColIndex, unsigned int RasterBandIndex)
{
  std::vector<float> Val;

  int LineCount = getRasterBand(RasterBandIndex)->GetYSize();

  float* ScanLine = (float *) CPLMalloc(sizeof(float) * LineCount);

  getRasterBand(RasterBandIndex)->RasterIO(GF_Read, ColIndex, 0, 1, LineCount,
                                           ScanLine, 1, LineCount, GDT_Float32,
                                           0, 0);

  for (int i = 0; i < LineCount; i++)
    Val.push_back(ScanLine[i]);

  CPLFree(ScanLine);

  return Val;
}

// =====================================================================
// =====================================================================

float RasterDataset::getValueOfPixel(int ColIndex, int LineIndex,
                                     unsigned int RasterBandIndex)
{
  float Val;

  float* ScanLine = (float *) CPLMalloc(sizeof(float));

  //  The pixel values will automatically be translated from the GDALRasterBand data type as needed.
  if (getRasterBand(RasterBandIndex)->RasterIO(GF_Read, ColIndex, LineIndex, 1,
                                               1, ScanLine, 1, 1, GDT_Float32,
                                               0, 0)
      != CE_None)
    throw openfluid::base::OFException(
        "OpenFLUID framework", "RasterDataset::getValueOfPixel",
        "Error while getting value from raster.");

  Val = ScanLine[0];

  CPLFree(ScanLine);

  return Val;
}

// =====================================================================
// =====================================================================

float RasterDataset::getValueOfCoordinate(geos::geom::Coordinate Coo,
                                          unsigned int RasterBandIndex)
{
  std::pair<int, int> Pixel = getPixelFromCoordinate(Coo);

  return getValueOfPixel(Pixel.first, Pixel.second, RasterBandIndex);
}

// =====================================================================
// =====================================================================

openfluid::landr::VectorDataset* RasterDataset::polygonize(
    std::string FileName, std::string FieldName, unsigned int RasterBandIndex)
{
  if (!mp_PolygonizedByRasterBandIndex.count(RasterBandIndex))
  {
    FieldName =
        (FieldName == "" ? getDefaultPolygonizedFieldName() : FieldName);

    mp_PolygonizedByRasterBandIndex.insert(
        std::make_pair(RasterBandIndex,
                       new openfluid::landr::VectorDataset(FileName)));

    mp_PolygonizedByRasterBandIndex.at(RasterBandIndex)->addALayer("",
                                                                   wkbPolygon);
    mp_PolygonizedByRasterBandIndex.at(RasterBandIndex)->addAField(FieldName,
                                                                   OFTInteger);
    int FieldIndex =
        mp_PolygonizedByRasterBandIndex.at(RasterBandIndex)->getFieldIndex(
            FieldName);

    OGRLayer* Layer =
        mp_PolygonizedByRasterBandIndex.at(RasterBandIndex)->getLayer(0);

    if (GDALPolygonize(getRasterBand(RasterBandIndex), NULL, Layer, FieldIndex,
                       NULL, NULL, NULL)
        != CE_None)
    {
      throw openfluid::base::OFException("OpenFLUID framework",
                                         "RasterDataset::polygonize",
                                         "Error while polygonizing raster.");
      delete mp_PolygonizedByRasterBandIndex.at(RasterBandIndex);
      mp_PolygonizedByRasterBandIndex.erase(RasterBandIndex);
    }
  }

  return mp_PolygonizedByRasterBandIndex.at(RasterBandIndex);
}

// =====================================================================
// =====================================================================

std::string RasterDataset::getDefaultPolygonizedFieldName()
{
  return "PixelVal";
}

// =====================================================================
// =====================================================================

}
} // namespaces
