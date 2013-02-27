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
 \file VectorDataset_TEST.cpp
 \brief Implements ...

 \author Aline LIBRES <aline.libres@gmail.com>
 */

#define BOOST_TEST_MAIN
#define BOOST_AUTO_TEST_MAIN
#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MODULE unittest_vectordataset
#include <boost/test/unit_test.hpp>
#include <boost/test/auto_unit_test.hpp>
#include <boost/filesystem/path.hpp>
#include <boost/filesystem/operations.hpp>
#include <tests-config.hpp>
#include <openfluid/base/OFException.hpp>
#include <openfluid/landr/VectorDataset.hpp>
#include <openfluid/core/GeoVectorValue.hpp>
#include <openfluid/base/RuntimeEnv.hpp>
#include <geos/geom/Geometry.h>

// =====================================================================
// =====================================================================

void deleteIfExists(std::string Path)
{
  OGRDataSource* DS = OGRSFDriverRegistrar::Open(Path.c_str(), false);

  if (DS)
  {
    OGRSFDriver* Driver = DS->GetDriver();
    OGRDataSource::DestroyDataSource(DS);
    Driver->DeleteDataSource(Path.c_str());
  }

}

// =====================================================================
// =====================================================================

// TODO Test with other than shapefiles
BOOST_AUTO_TEST_CASE(check_constructor_empty)
{
  BOOST_CHECK_THROW(
      new openfluid::landr::VectorDataset("test.shp","wrong_driver_name"),
      openfluid::base::OFException);

  openfluid::landr::VectorDataset* Vect = new openfluid::landr::VectorDataset(
      "test.shp");

  OGRDataSource* DS = Vect->getDataSource();

  BOOST_CHECK(DS);
  BOOST_CHECK(DS->GetDriver());
  BOOST_CHECK_EQUAL(DS->GetDriver()->GetName(), "ESRI Shapefile");

  delete Vect;

  Vect = new openfluid::landr::VectorDataset(
      "test.shp");

  Vect->addALayer();

  delete Vect;
}

// =====================================================================
// =====================================================================

BOOST_AUTO_TEST_CASE(check_constructor_fromValue)
{
  openfluid::core::GeoVectorValue Value(CONFIGTESTS_INPUT_DATASETS_DIR,
                                        "landr/SU.shp");

  openfluid::landr::VectorDataset* Vect = new openfluid::landr::VectorDataset(
      Value);

  OGRDataSource* DS = Vect->getDataSource();

  BOOST_CHECK(DS);
  BOOST_CHECK(DS->GetDriver());
  BOOST_CHECK_EQUAL(DS->GetDriver()->GetName(), "ESRI Shapefile");

  openfluid::landr::VectorDataset* Vect2 = new openfluid::landr::VectorDataset(
      Value);

  delete Vect;
  delete Vect2;
}

// =====================================================================
// =====================================================================

// TODO Test with other than shapefiles
BOOST_AUTO_TEST_CASE(check_copyToDisk)
{
  std::string NewPath = openfluid::core::GeoValue::computeAbsolutePath(
      CONFIGTESTS_OUTPUT_DATA_DIR, "OPENFLUID.OUT.VectorDataset/new_test.shp");

  deleteIfExists(NewPath);

  if (!boost::filesystem::exists(
      CONFIGTESTS_OUTPUT_DATA_DIR + "/OPENFLUID.OUT.VectorDataset"))
    boost::filesystem::create_directory(
        CONFIGTESTS_OUTPUT_DATA_DIR + "/OPENFLUID.OUT.VectorDataset");

  openfluid::core::GeoVectorValue Value(CONFIGTESTS_INPUT_DATASETS_DIR,
                                        "landr/SU.shp");

  openfluid::landr::VectorDataset* Vect = new openfluid::landr::VectorDataset(
      Value);

  Vect->copyToDisk(CONFIGTESTS_OUTPUT_DATA_DIR,
                   "OPENFLUID.OUT.VectorDataset/new_test.shp", false);

  openfluid::core::GeoVectorValue NewVal(
      CONFIGTESTS_OUTPUT_DATA_DIR, "OPENFLUID.OUT.VectorDataset/new_test.shp");

  BOOST_CHECK_EQUAL(NewVal.get()->GetLayer(0)->GetFeatureCount(), 24);

  BOOST_CHECK_THROW(
      Vect->copyToDisk(CONFIGTESTS_OUTPUT_DATA_DIR, "OPENFLUID.OUT.VectorDataset/new_test.shp", false),
      openfluid::base::OFException);

  Vect->copyToDisk(CONFIGTESTS_OUTPUT_DATA_DIR,
                   "OPENFLUID.OUT.VectorDataset/new_test.shp", true);

  delete Vect;
}

// =====================================================================
// =====================================================================

BOOST_AUTO_TEST_CASE(check_Properties)
{
  openfluid::core::GeoVectorValue Value(
      CONFIGTESTS_INPUT_DATASETS_DIR + "/landr", "SU.shp");

  openfluid::landr::VectorDataset* Vect = new openfluid::landr::VectorDataset(
      Value);

  BOOST_CHECK(!Vect->isLineType());
  BOOST_CHECK(Vect->isPolygonType());

  BOOST_CHECK(Vect->containsField("SELF_ID"));
  BOOST_CHECK(!Vect->containsField("wrongField"));

  BOOST_CHECK_EQUAL(Vect->getFieldIndex("SELF_ID"), 1);

  delete Vect;
}

// =====================================================================
// =====================================================================

BOOST_AUTO_TEST_CASE(check_addField)
{
  openfluid::core::GeoVectorValue Value(
      CONFIGTESTS_INPUT_DATASETS_DIR + "/landr", "SU.shp");

  openfluid::landr::VectorDataset* Vect = new openfluid::landr::VectorDataset(
      Value);

  Vect->addAField("NewField", OFTInteger);

  BOOST_CHECK(Vect->containsField("NewField"));
  BOOST_CHECK(Vect->isFieldOfType("NewField",OFTInteger));
  BOOST_CHECK(!Vect->isFieldOfType("NewField",OFTString));
  BOOST_CHECK_EQUAL(Vect->getFieldIndex("NewField"), 10);
  BOOST_CHECK(!Vect->isIntValueSet("NewField",123));

  delete Vect;
}

// =====================================================================
// =====================================================================

BOOST_AUTO_TEST_CASE(check_parse)
{
  openfluid::core::GeoVectorValue Value(
      CONFIGTESTS_INPUT_DATASETS_DIR + "/landr", "SU.shp");

  openfluid::landr::VectorDataset* Vect = new openfluid::landr::VectorDataset(
      Value);

  openfluid::landr::VectorDataset::FeaturesList_t Features =
      Vect->getFeatures();

  BOOST_CHECK_EQUAL(Features.size(), 24);

  for (openfluid::landr::VectorDataset::FeaturesList_t::iterator it =
      Features.begin(); it != Features.end(); ++it)
  {
    geos::geom::Geometry* GeosGeom =
        (geos::geom::Geometry*) it->first->GetGeometryRef()->exportToGEOS();

    BOOST_CHECK_EQUAL(GeosGeom->toString(), it->second->toString());
  }

  geos::geom::Geometry* Geom = Vect->getGeometries();

  BOOST_CHECK_EQUAL(Geom->getNumGeometries(), 24);
  BOOST_CHECK_EQUAL(Geom->getDimension(), 2 /* means Polygons */);

  delete Vect;
  delete Geom;

  Value = openfluid::core::GeoVectorValue(
      CONFIGTESTS_INPUT_DATASETS_DIR + "/landr", "RS.shp");

  Vect = new openfluid::landr::VectorDataset(Value);

  Features = Vect->getFeatures();

  BOOST_CHECK_EQUAL(Features.size(), 8);

  for (openfluid::landr::VectorDataset::FeaturesList_t::iterator it =
      Features.begin(); it != Features.end(); ++it)
  {
    geos::geom::Geometry* GeosGeom =
        (geos::geom::Geometry*) it->first->GetGeometryRef()->exportToGEOS();

    BOOST_CHECK_EQUAL(GeosGeom->toString(), it->second->toString());
  }

  Geom = Vect->getGeometries();

  BOOST_CHECK_EQUAL(Geom->getNumGeometries(), 8);
  BOOST_CHECK_EQUAL(Geom->getDimension(), 1 /* means LineStrings */);

  delete Vect;
  delete Geom;
}
