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
 \file RasterDataset_TEST.cpp
 \brief Implements ...

 \author Aline LIBRES <aline.libres@gmail.com>
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
#include <openfluid/base/OFException.hpp>
#include <openfluid/landr/RasterDataset.hpp>
#include <openfluid/landr/VectorDataset.hpp>
#include <openfluid/core/GeoRasterValue.hpp>
#include <openfluid/core/GeoVectorValue.hpp>
#include <openfluid/base/RuntimeEnv.hpp>
#include <openfluid/tools.hpp>
#include <geos/geom/Coordinate.h>

// =====================================================================
// =====================================================================

BOOST_AUTO_TEST_CASE(check_constructor)
{
  openfluid::core::GeoRasterValue Value(CONFIGTESTS_INPUT_DATASETS_DIR,
                                        "landr/dem.jpeg");

  openfluid::landr::RasterDataset* Rast = new openfluid::landr::RasterDataset(
      Value);

  GDALDataset* DS = Rast->getDataset();

  BOOST_CHECK(DS);

  delete Rast;
}

// =====================================================================
// =====================================================================

BOOST_AUTO_TEST_CASE(check_Properties)
{
  openfluid::core::GeoRasterValue Val(
      CONFIGTESTS_INPUT_DATASETS_DIR + "/GeoRasterValue", "dem.jpeg");

  openfluid::landr::RasterDataset* Rast = new openfluid::landr::RasterDataset(
      Val);

  BOOST_CHECK_EQUAL(Rast->getDataset()->GetRasterXSize(), 20);
  BOOST_CHECK_EQUAL(Rast->getDataset()->GetRasterYSize(), 20);

  BOOST_CHECK_EQUAL(Rast->getRasterBand(1)->GetXSize(), 20);
  BOOST_CHECK_EQUAL(Rast->getRasterBand(1)->GetYSize(), 20);

  double adfGeoTransform[6];
  BOOST_REQUIRE(
      GDALGetGeoTransform(Rast->getDataset(), adfGeoTransform) == CE_None);

  BOOST_CHECK_EQUAL(Rast->getOrigin()->x, adfGeoTransform[0]);
  BOOST_CHECK_EQUAL(Rast->getOrigin()->y, adfGeoTransform[3]);

  BOOST_CHECK_EQUAL(Rast->getPixelWidth(), adfGeoTransform[1]);
  BOOST_CHECK_EQUAL(Rast->getPixelHeight(), adfGeoTransform[5]);

  delete Rast;
}

// =====================================================================
// =====================================================================

BOOST_AUTO_TEST_CASE(check_CoordinateToPixel)
{
  openfluid::core::GeoRasterValue Val(
      CONFIGTESTS_INPUT_DATASETS_DIR + "/GeoRasterValue", "dem.jpeg");

  openfluid::landr::RasterDataset* Rast = new openfluid::landr::RasterDataset(
      Val);

  std::pair<int, int> Pixel = Rast->getPixelFromCoordinate(*Rast->getOrigin());

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
      CONFIGTESTS_INPUT_DATASETS_DIR + "/GeoRasterValue", "dem.jpeg");

  openfluid::landr::RasterDataset* Rast = new openfluid::landr::RasterDataset(
      Val);

  std::vector<float> Line0 = Rast->getValuesOfLine(0);
  std::vector<float> Line4 = Rast->getValuesOfLine(4);
  std::vector<float> Line19 = Rast->getValuesOfLine(19);

  std::vector<float> Col0 = Rast->getValuesOfColumn(0);
  std::vector<float> Col4 = Rast->getValuesOfColumn(4);
  std::vector<float> Col19 = Rast->getValuesOfColumn(19);

  BOOST_CHECK_EQUAL(Line0.size(), Rast->getRasterBand(1)->GetXSize());
  BOOST_CHECK_EQUAL(Line4.size(), Rast->getRasterBand(1)->GetXSize());
  BOOST_CHECK_EQUAL(Line19.size(), Rast->getRasterBand(1)->GetXSize());
  BOOST_CHECK_EQUAL(Col0.size(), Rast->getRasterBand(1)->GetYSize());
  BOOST_CHECK_EQUAL(Col4.size(), Rast->getRasterBand(1)->GetYSize());
  BOOST_CHECK_EQUAL(Col19.size(), Rast->getRasterBand(1)->GetYSize());

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
      CONFIGTESTS_INPUT_DATASETS_DIR + "/GeoRasterValue", "dem.jpeg");

  openfluid::landr::RasterDataset* Rast = new openfluid::landr::RasterDataset(
      Val);

  BOOST_CHECK_EQUAL(Rast->getValueOfCoordinate(*Rast->getOrigin()), 96);

  double x = 679288.64458;
  double y = 132607.541088;

  BOOST_CHECK_EQUAL(Rast->getValueOfCoordinate(geos::geom::Coordinate(x, y)),
                    83);

  delete Rast;

  // float values
  Val = openfluid::core::GeoRasterValue(
      CONFIGTESTS_INPUT_DATASETS_DIR + "/GeoRasterValue", "dem.asc");

  Rast = new openfluid::landr::RasterDataset(Val);

  BOOST_CHECK(
      openfluid::tools::IsVeryClose(Rast->getValueOfCoordinate(*Rast->getOrigin()), 98.9708));

  BOOST_CHECK(
      openfluid::tools::IsVeryClose(Rast->getValueOfCoordinate(geos::geom::Coordinate(x, y)), 84.587));

  delete Rast;
}

// =====================================================================
// =====================================================================

BOOST_AUTO_TEST_CASE(check_Polygonize)
{
  if (!boost::filesystem::exists(
      openfluid::base::RuntimeEnvironment::getInstance()->getTempDir() + "/GeoVectorValue"))
    boost::filesystem::create_directory(
        openfluid::base::RuntimeEnvironment::getInstance()->getTempDir() + "/GeoVectorValue");

  // integer values
  openfluid::core::GeoRasterValue RasterVal(
      CONFIGTESTS_INPUT_DATASETS_DIR + "/GeoRasterValue", "dem.jpeg");

  openfluid::landr::RasterDataset* Rast = new openfluid::landr::RasterDataset(
      RasterVal);

  openfluid::landr::VectorDataset* VectorVal = Rast->polygonize(
      "GeoVectorValue/TestOut.shp", "RasterVal");

  OGRLayer* VectorLayer = VectorVal->getLayer(0);

  BOOST_CHECK_EQUAL(VectorLayer->GetFeatureCount(), 234);
  BOOST_CHECK_EQUAL(VectorLayer->GetFeature(0)->GetFieldAsInteger("RasterVal"),
                    96);
  BOOST_CHECK_EQUAL(VectorLayer->GetFeature(76)->GetFieldAsInteger("RasterVal"),
                    83);

  delete Rast;
  delete VectorVal;

  // float values, rounded to integer values (GDALFPolygonize for floats available since GDAL 1.9.0 only)
  RasterVal = openfluid::core::GeoRasterValue(
      CONFIGTESTS_INPUT_DATASETS_DIR + "/GeoRasterValue", "dem.asc");

  Rast = new openfluid::landr::RasterDataset(RasterVal);

  VectorVal = Rast->polygonize("GeoVectorValue/TestOut.shp");

  VectorLayer = VectorVal->getLayer(0);

  BOOST_CHECK_EQUAL(VectorLayer->GetFeatureCount(), 151);
  BOOST_CHECK_EQUAL(
      VectorLayer->GetFeature(0)->GetFieldAsInteger(openfluid::landr::RasterDataset::getDefaultPolygonizedFieldName().c_str()),
      99);
  BOOST_CHECK_EQUAL(
      VectorLayer->GetFeature(42)->GetFieldAsInteger(openfluid::landr::RasterDataset::getDefaultPolygonizedFieldName().c_str()),
      85);

  delete Rast;
  delete VectorVal;
}

// =====================================================================
// =====================================================================

