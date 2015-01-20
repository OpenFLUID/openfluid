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
 @file RasterDataset_TEST.cpp

 @author Aline LIBRES <aline.libres@gmail.com>
 */

#define BOOST_TEST_MAIN
#define BOOST_AUTO_TEST_MAIN
#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MODULE unittest_rasterdataset
#include <boost/test/unit_test.hpp>
#include <boost/test/auto_unit_test.hpp>
#include <boost/filesystem/path.hpp>
#include <boost/filesystem/operations.hpp>
#include <tests-config.hpp>
#include <openfluid/base/FrameworkException.hpp>
#include <openfluid/landr/RasterDataset.hpp>
#include <openfluid/landr/VectorDataset.hpp>
#include <openfluid/core/GeoRasterValue.hpp>
#include <openfluid/core/GeoVectorValue.hpp>
#include <openfluid/core/DoubleValue.hpp>
#include <openfluid/base/RuntimeEnv.hpp>
#include <openfluid/scientific/FloatingPoint.hpp>
#include <geos/geom/Coordinate.h>


// =====================================================================
// =====================================================================


BOOST_AUTO_TEST_CASE(check_constructor)
{
  openfluid::core::GeoRasterValue Value(CONFIGTESTS_INPUT_MISCDATA_DIR,
                                        "landr/dem.jpeg");

  openfluid::landr::RasterDataset* Rast = new openfluid::landr::RasterDataset(
      Value);

  GDALDataset* DS = Rast->source();

  BOOST_CHECK(DS);

  delete Rast;
}


// =====================================================================
// =====================================================================


BOOST_AUTO_TEST_CASE(check_Properties)
{
  openfluid::core::GeoRasterValue Val(
      CONFIGTESTS_INPUT_MISCDATA_DIR + "/GeoRasterValue", "dem.jpeg");

  openfluid::landr::RasterDataset* Rast = new openfluid::landr::RasterDataset(
      Val);

  BOOST_CHECK_EQUAL(Rast->source()->GetRasterXSize(), 20);
  BOOST_CHECK_EQUAL(Rast->source()->GetRasterYSize(), 20);

  BOOST_CHECK_EQUAL(Rast->rasterBand(1)->GetXSize(), 20);
  BOOST_CHECK_EQUAL(Rast->rasterBand(1)->GetYSize(), 20);

  double adfGeoTransform[6];
  BOOST_REQUIRE(
      GDALGetGeoTransform(Rast->source(), adfGeoTransform) == CE_None);

  BOOST_CHECK_EQUAL(Rast->computeOrigin()->x, adfGeoTransform[0]);
  BOOST_CHECK_EQUAL(Rast->computeOrigin()->y, adfGeoTransform[3]);

  BOOST_CHECK_EQUAL(Rast->getPixelWidth(), adfGeoTransform[1]);
  BOOST_CHECK_EQUAL(Rast->getPixelHeight(), adfGeoTransform[5]);

  delete Rast;
}


// =====================================================================
// =====================================================================


BOOST_AUTO_TEST_CASE(check_CoordinateToPixel)
{
  openfluid::core::GeoRasterValue Val(
      CONFIGTESTS_INPUT_MISCDATA_DIR + "/GeoRasterValue", "dem.jpeg");

  openfluid::landr::RasterDataset* Rast = new openfluid::landr::RasterDataset(
      Val);

  std::pair<int, int> Pixel = Rast->getPixelFromCoordinate(*Rast->computeOrigin());

  BOOST_CHECK_EQUAL(Pixel.first, 0);
  BOOST_CHECK_EQUAL(Pixel.second, 0);

  double x = 679288.64458;
  double y = 132607.541088;

  Pixel = Rast->getPixelFromCoordinate(geos::geom::Coordinate(x, y));

  BOOST_CHECK_EQUAL(Pixel.first, 5);
  BOOST_CHECK_EQUAL(Pixel.second, 6);

  delete Rast;
}


// =====================================================================
// =====================================================================


BOOST_AUTO_TEST_CASE(check_getValues)
{
  openfluid::core::GeoRasterValue Val(
      CONFIGTESTS_INPUT_MISCDATA_DIR + "/GeoRasterValue", "dem.jpeg");

  openfluid::landr::RasterDataset* Rast = new openfluid::landr::RasterDataset(
      Val);

  std::vector<float> Line0 = Rast->getValuesOfLine(0);
  std::vector<float> Line4 = Rast->getValuesOfLine(4);
  std::vector<float> Line19 = Rast->getValuesOfLine(19);

  std::vector<float> Col0 = Rast->getValuesOfColumn(0);
  std::vector<float> Col4 = Rast->getValuesOfColumn(4);
  std::vector<float> Col19 = Rast->getValuesOfColumn(19);

  BOOST_CHECK_EQUAL(Line0.size(), Rast->rasterBand(1)->GetXSize());
  BOOST_CHECK_EQUAL(Line4.size(), Rast->rasterBand(1)->GetXSize());
  BOOST_CHECK_EQUAL(Line19.size(), Rast->rasterBand(1)->GetXSize());
  BOOST_CHECK_EQUAL(Col0.size(), Rast->rasterBand(1)->GetYSize());
  BOOST_CHECK_EQUAL(Col4.size(), Rast->rasterBand(1)->GetYSize());
  BOOST_CHECK_EQUAL(Col19.size(), Rast->rasterBand(1)->GetYSize());

  BOOST_CHECK_EQUAL(Line0[0], 96);
  BOOST_CHECK_EQUAL(Line0[0], Col0[0]);
  BOOST_CHECK_EQUAL(Line0[0], Rast->getValueOfPixel(0,0));

  BOOST_CHECK_EQUAL(Line0[4], 100);
  BOOST_CHECK_EQUAL(Line0[4], Col4[0]);
  BOOST_CHECK_EQUAL(Line0[4], Rast->getValueOfPixel(4,0));

  BOOST_CHECK_EQUAL(Line0[19], 99);
  BOOST_CHECK_EQUAL(Line0[19], Col19[0]);
  BOOST_CHECK_EQUAL(Line0[19], Rast->getValueOfPixel(19,0));

  BOOST_CHECK_EQUAL(Line4[0], 80);
  BOOST_CHECK_EQUAL(Line4[0], Col0[4]);
  BOOST_CHECK_EQUAL(Line4[0], Rast->getValueOfPixel(0,4));

  BOOST_CHECK_EQUAL(Line4[4], 86);
  BOOST_CHECK_EQUAL(Line4[4], Col4[4]);
  BOOST_CHECK_EQUAL(Line4[4], Rast->getValueOfPixel(4,4));

  BOOST_CHECK_EQUAL(Line4[19], 82);
  BOOST_CHECK_EQUAL(Line4[19], Col19[4]);
  BOOST_CHECK_EQUAL(Line4[19], Rast->getValueOfPixel(19,4));

  BOOST_CHECK_EQUAL(Line19[0], 21);
  BOOST_CHECK_EQUAL(Line19[0], Col0[19]);
  BOOST_CHECK_EQUAL(Line19[0], Rast->getValueOfPixel(0,19));

  BOOST_CHECK_EQUAL(Line19[4], 21);
  BOOST_CHECK_EQUAL(Line19[4], Col4[19]);
  BOOST_CHECK_EQUAL(Line19[4], Rast->getValueOfPixel(4,19));

  BOOST_CHECK_EQUAL(Line19[19], 21);
  BOOST_CHECK_EQUAL(Line19[19], Col19[19]);
  BOOST_CHECK_EQUAL(Line19[19], Rast->getValueOfPixel(19,19));

  delete Rast;
}


// =====================================================================
// =====================================================================


BOOST_AUTO_TEST_CASE(check_getValueOfCoordinate)
{
  // integer values
  openfluid::core::GeoRasterValue Val(
      CONFIGTESTS_INPUT_MISCDATA_DIR + "/GeoRasterValue", "dem.jpeg");

  openfluid::landr::RasterDataset* Rast = new openfluid::landr::RasterDataset(
      Val);

  BOOST_CHECK_EQUAL(Rast->getValueOfCoordinate(*Rast->computeOrigin()), 96);

  double x = 679288.64458;
  double y = 132607.541088;

  BOOST_CHECK_EQUAL(Rast->getValueOfCoordinate(geos::geom::Coordinate(x, y)),
                    83);

  delete Rast;

  // float values
  Val = openfluid::core::GeoRasterValue(
      CONFIGTESTS_INPUT_MISCDATA_DIR + "/GeoRasterValue", "dem.asc");

  Rast = new openfluid::landr::RasterDataset(Val);

  BOOST_CHECK(
      openfluid::scientific::isVeryClose(double(Rast->getValueOfCoordinate(*Rast->computeOrigin())), 98.9708));

  BOOST_CHECK(
      openfluid::scientific::isVeryClose(double(Rast->getValueOfCoordinate(geos::geom::Coordinate(x, y))), 84.587));

  delete Rast;
}


// =====================================================================
// =====================================================================


BOOST_AUTO_TEST_CASE(check_Polygonize)
{
  // integer values
  openfluid::core::GeoRasterValue RasterVal(
      CONFIGTESTS_INPUT_MISCDATA_DIR + "/GeoRasterValue", "dem.jpeg");

  openfluid::landr::RasterDataset* Rast = new openfluid::landr::RasterDataset(
      RasterVal);

  openfluid::landr::VectorDataset* VectorVal = Rast->polygonize("TestOut.shp",
                                                                "RasterVal");

  OGRLayer* VectorLayer = VectorVal->layer(0);

  BOOST_CHECK_EQUAL(VectorLayer->GetFeatureCount(), 234);
  BOOST_CHECK_EQUAL(VectorLayer->GetFeature(0)->GetFieldAsInteger("RasterVal"),
                    96);
  BOOST_CHECK_EQUAL(VectorLayer->GetFeature(76)->GetFieldAsInteger("RasterVal"),
                    83);

  delete Rast;
  delete VectorVal;

  // float values keeped (GDALFPolygonize for floats available since GDAL 1.9.0 )
  RasterVal = openfluid::core::GeoRasterValue(
      CONFIGTESTS_INPUT_MISCDATA_DIR + "/GeoRasterValue", "dem.asc");

  Rast = new openfluid::landr::RasterDataset(RasterVal);

  VectorVal = Rast->polygonize("TestOut.shp");

  VectorLayer = VectorVal->layer(0);

  BOOST_CHECK_EQUAL(VectorLayer->GetFeatureCount(), 400);

  openfluid::core::DoubleValue Val=VectorLayer->GetFeature(0)->GetFieldAsDouble(openfluid::landr::RasterDataset::getDefaultPolygonizedFieldName().c_str());
  BOOST_CHECK( openfluid::scientific::isVeryClose(Val.get(), 98.9708));

  Val=VectorLayer->GetFeature(1)->GetFieldAsDouble(openfluid::landr::RasterDataset::getDefaultPolygonizedFieldName().c_str());
  BOOST_CHECK( openfluid::scientific::isVeryClose(Val.get(), 99.201));

  Val=VectorLayer->GetFeature(331)->GetFieldAsDouble(openfluid::landr::RasterDataset::getDefaultPolygonizedFieldName().c_str());
  BOOST_CHECK( openfluid::scientific::isVeryClose(Val.get(), 42.327));

  Val=VectorLayer->GetFeature(399)->GetFieldAsDouble(openfluid::landr::RasterDataset::getDefaultPolygonizedFieldName().c_str());
  BOOST_CHECK( openfluid::scientific::isVeryClose(Val.get(), 21.0336));

  delete Rast;
  delete VectorVal;
}


// =====================================================================
// =====================================================================

