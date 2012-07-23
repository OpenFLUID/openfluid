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
 \file GeoRasterValue.cpp
 \brief Implements ...

 \author Aline LIBRES <libres@supagro.inra.fr>
 */

#include "GeoRasterValue.hpp"

#include <boost/filesystem/path.hpp>

#include <openfluid/base/OFException.hpp>
#include <geos/geom/Coordinate.h>

namespace openfluid {
namespace core {

// =====================================================================
// =====================================================================

GeoRasterValue::GeoRasterValue(std::string FilePath, std::string FileName) :
    GeoValue(FilePath, FileName), mp_Data(0), mp_RasterBand1(0), mp_GeoTransform(
        0)
{
  GDALAllRegister();
}

// =====================================================================
// =====================================================================

GeoRasterValue::~GeoRasterValue()
{
  if (mp_Data)
    GDALClose(mp_Data);

  delete mp_GeoTransform;
}

// =====================================================================
// =====================================================================

openfluid::core::UnstructuredValue::UnstructuredType GeoRasterValue::getType() const
{
  return openfluid::core::UnstructuredValue::GeoRasterValue;
}

// =====================================================================
// =====================================================================

GDALDataset* GeoRasterValue::get(bool UpdateMode)
{
  if (!mp_Data)
    tryToOpenSource(UpdateMode);

  return mp_Data;
}

// =====================================================================
// =====================================================================

void GeoRasterValue::tryToOpenSource(bool /*UpdateMode*/)
{
  mp_Data = static_cast<GDALDataset*>(GDALOpen(m_AbsolutePath.c_str(),
                                               GA_ReadOnly));

  if (!mp_Data)
  {
    throw openfluid::base::OFException(
        "OpenFLUID framework",
        "GeoRasterValue::tryToOpenSource",
        "Error while trying to open file " + m_AbsolutePath + " ("
        + CPLGetLastErrorMsg() + ")");
  }

}

// =====================================================================
// =====================================================================

GDALRasterBand* GeoRasterValue::getRasterBand1()
{
  if (!mp_RasterBand1)
    mp_RasterBand1 = get()->GetRasterBand(1);

  return mp_RasterBand1;
}

// =====================================================================
// =====================================================================

std::pair<int, int> GeoRasterValue::getPixelFromCoordinate(
    geos::geom::Coordinate Coo)
{
  int offsetX = int((Coo.x - getOrigin()->x) / getPixelWidth());
  int offsetY = int((Coo.y - getOrigin()->y) / getPixelHeight());

  return std::make_pair(offsetX, offsetY);
}

// =====================================================================
// =====================================================================

geos::geom::Coordinate* GeoRasterValue::getOrigin()
{
  if (!mp_GeoTransform)
    computeGeoTransform();

  return new geos::geom::Coordinate(mp_GeoTransform[0], mp_GeoTransform[3]);
}

// =====================================================================
// =====================================================================

void GeoRasterValue::computeGeoTransform()
{
  mp_GeoTransform = new double[6];

  if (GDALGetGeoTransform(get(), mp_GeoTransform) != CE_None)
    throw openfluid::base::OFException(
        "OpenFLUID framework", "GeoRasterValue::computeGeoTransform",
        "Error while getting GeoTransform information");
}

// =====================================================================
// =====================================================================

double GeoRasterValue::getPixelWidth()
{
  if (!mp_GeoTransform)
    computeGeoTransform();

  return mp_GeoTransform[1];
}

// =====================================================================
// =====================================================================

double GeoRasterValue::getPixelHeight()
{
  if (!mp_GeoTransform)
    computeGeoTransform();

  return mp_GeoTransform[5];
}

// =====================================================================
// =====================================================================

std::vector<float> GeoRasterValue::getValuesOfLine(int LineIndex)
{
  std::vector<float> Val;

  int ColumnCount = getRasterBand1()->GetXSize();

  float* ScanLine = (float *) CPLMalloc(sizeof(float) * ColumnCount);

  getRasterBand1()->RasterIO(GF_Read, 0, LineIndex, ColumnCount, 1, ScanLine,
                             ColumnCount, 1, GDT_Float32, 0, 0);

  for (int i = 0; i < ColumnCount; i++)
    Val.push_back(ScanLine[i]);

  CPLFree(ScanLine);

  return Val;
}

// =====================================================================
// =====================================================================

std::vector<float> GeoRasterValue::getValuesOfColumn(int ColIndex)
{
  std::vector<float> Val;

  int LineCount = getRasterBand1()->GetYSize();

  float* ScanLine = (float *) CPLMalloc(sizeof(float) * LineCount);

  getRasterBand1()->RasterIO(GF_Read, ColIndex, 0, 1, LineCount, ScanLine, 1,
                             LineCount, GDT_Float32, 0, 0);

  for (int i = 0; i < LineCount; i++)
    Val.push_back(ScanLine[i]);

  CPLFree(ScanLine);

  return Val;
}

// =====================================================================
// =====================================================================

float GeoRasterValue::getValueOfPixel(int ColIndex, int LineIndex)
{
  float Val;

  float* ScanLine = (float *) CPLMalloc(sizeof(float));

  getRasterBand1()->RasterIO(GF_Read, ColIndex, LineIndex, 1, 1, ScanLine, 1, 1,
                             GDT_Float32, 0, 0);

  Val = ScanLine[0];

  CPLFree(ScanLine);

  return Val;
}

// =====================================================================
// =====================================================================

float GeoRasterValue::getValueOfCoordinate(geos::geom::Coordinate Coo)
{
  std::pair<int,int> Pixel = getPixelFromCoordinate(Coo);

  return getValueOfPixel(Pixel.first,Pixel.second);
}

// =====================================================================
// =====================================================================

}
} // namespaces
