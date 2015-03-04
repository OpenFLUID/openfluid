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
  @file VectorDataset_TEST.cpp

  @author Aline LIBRES <aline.libres@gmail.com>
 */

#define BOOST_TEST_MAIN
#define BOOST_AUTO_TEST_MAIN
#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MODULE unittest_vectordataset
#include <boost/test/unit_test.hpp>
#include <boost/test/auto_unit_test.hpp>
#include <tests-config.hpp>
#include <openfluid/base/FrameworkException.hpp>
#include <openfluid/scientific/FloatingPoint.hpp>
#include <openfluid/landr/VectorDataset.hpp>
#include <openfluid/core/GeoVectorValue.hpp>
#include <openfluid/base/RuntimeEnv.hpp>
#include <openfluid/tools/Filesystem.hpp>
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


BOOST_AUTO_TEST_CASE(check_constructor_empty)
{

  openfluid::landr::VectorDataset* Vect = new openfluid::landr::VectorDataset(
      "test.shp");

  OGRDataSource* DS = Vect->source();

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


BOOST_AUTO_TEST_CASE(check_constructor_WrongVectorFormat)
{

  BOOST_CHECK_THROW(
      new openfluid::landr::VectorDataset(CONFIGTESTS_INPUT_MISCDATA_DIR +"/GeoVectorValue/SU.gml"),
      openfluid::base::FrameworkException);

  BOOST_CHECK_THROW(
      new openfluid::landr::VectorDataset(CONFIGTESTS_INPUT_MISCDATA_DIR +"/GeoVectorValue/SU.geojson"),
      openfluid::base::FrameworkException);


  openfluid::core::GeoVectorValue Value(CONFIGTESTS_INPUT_MISCDATA_DIR,
                                        "/GeoVectorValue/SU.geojson");

  BOOST_CHECK_THROW( new openfluid::landr::VectorDataset(Value),
                     openfluid::base::FrameworkException);

  openfluid::core::GeoVectorValue Value2(CONFIGTESTS_INPUT_MISCDATA_DIR,
                                         "/GeoVectorValue/SU.gml");

  BOOST_CHECK_THROW( new openfluid::landr::VectorDataset(Value2),
                     openfluid::base::FrameworkException);
}


// =====================================================================
// =====================================================================


BOOST_AUTO_TEST_CASE(check_constructor_fromValue)
{
  openfluid::core::GeoVectorValue Value(CONFIGTESTS_INPUT_MISCDATA_DIR,
                                        "landr/SU.shp");

  openfluid::landr::VectorDataset* Vect = new openfluid::landr::VectorDataset(
      Value);

  OGRDataSource* DS = Vect->source();

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


BOOST_AUTO_TEST_CASE(check_copyToDisk)
{
  std::string NewPath = openfluid::core::GeoValue::computeAbsolutePath(
      CONFIGTESTS_OUTPUT_DATA_DIR, "OPENFLUID.OUT.VectorDataset/new_test.shp");

  deleteIfExists(NewPath);

  if (!openfluid::tools::Filesystem::isDirectory(CONFIGTESTS_OUTPUT_DATA_DIR + "/OPENFLUID.OUT.VectorDataset"))
    openfluid::tools::Filesystem::makeDirectory(CONFIGTESTS_OUTPUT_DATA_DIR + "/OPENFLUID.OUT.VectorDataset");

  openfluid::core::GeoVectorValue Value(CONFIGTESTS_INPUT_MISCDATA_DIR,
                                        "landr/SU.shp");

  openfluid::landr::VectorDataset* Vect = new openfluid::landr::VectorDataset(
      Value);

  Vect->copyToDisk(CONFIGTESTS_OUTPUT_DATA_DIR,
                   "OPENFLUID.OUT.VectorDataset/new_test.shp", false);

  openfluid::core::GeoVectorValue NewVal(
      CONFIGTESTS_OUTPUT_DATA_DIR, "OPENFLUID.OUT.VectorDataset/new_test.shp");

  BOOST_CHECK_EQUAL(NewVal.data()->GetLayer(0)->GetFeatureCount(), 24);

  BOOST_CHECK_THROW(
      Vect->copyToDisk(CONFIGTESTS_OUTPUT_DATA_DIR, "OPENFLUID.OUT.VectorDataset/new_test.shp", false),
      openfluid::base::FrameworkException);

  Vect->copyToDisk(CONFIGTESTS_OUTPUT_DATA_DIR,
                   "OPENFLUID.OUT.VectorDataset/new_test.shp", true);

  delete Vect;
}


// =====================================================================
// =====================================================================


BOOST_AUTO_TEST_CASE(check_Properties)
{
  openfluid::core::GeoVectorValue Value(
      CONFIGTESTS_INPUT_MISCDATA_DIR + "/landr", "SU.shp");

  openfluid::landr::VectorDataset* Vect = new openfluid::landr::VectorDataset(
      Value);

  BOOST_CHECK(!Vect->isLineType());
  BOOST_CHECK(Vect->isPolygonType());

  BOOST_CHECK(Vect->containsField("OFLD_ID"));
  BOOST_CHECK(!Vect->containsField("wrongField"));

  BOOST_CHECK_EQUAL(Vect->getFieldIndex("OFLD_ID"), 9);

  delete Vect;
}


// =====================================================================
// =====================================================================


BOOST_AUTO_TEST_CASE(check_addField)
{
  openfluid::core::GeoVectorValue Value(
      CONFIGTESTS_INPUT_MISCDATA_DIR + "/landr", "SU.shp");

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
      CONFIGTESTS_INPUT_MISCDATA_DIR + "/landr", "SU.shp");

  openfluid::landr::VectorDataset* Vect = new openfluid::landr::VectorDataset(
      Value);

  openfluid::landr::VectorDataset::FeaturesList_t Features =
      Vect->features();

  BOOST_CHECK_EQUAL(Features.size(), 24);

  for (openfluid::landr::VectorDataset::FeaturesList_t::iterator it =
      Features.begin(); it != Features.end(); ++it)
  {
    geos::geom::Geometry* GeosGeom =
        (geos::geom::Geometry*) it->first->GetGeometryRef()->exportToGEOS();

    BOOST_CHECK_EQUAL(GeosGeom->toString(), it->second->toString());
  }

  geos::geom::Geometry* Geom = Vect->geometries();

  BOOST_CHECK_EQUAL(Geom->getNumGeometries(), 24);
  BOOST_CHECK_EQUAL(Geom->getDimension(), 2 /* means Polygons */);

  delete Vect;
  delete Geom;

  Value = openfluid::core::GeoVectorValue(
      CONFIGTESTS_INPUT_MISCDATA_DIR + "/landr", "RS.shp");

  Vect = new openfluid::landr::VectorDataset(Value);

  Features = Vect->features();

  BOOST_CHECK_EQUAL(Features.size(), 8);

  for (openfluid::landr::VectorDataset::FeaturesList_t::iterator it =
      Features.begin(); it != Features.end(); ++it)
  {
    geos::geom::Geometry* GeosGeom =
        (geos::geom::Geometry*) it->first->GetGeometryRef()->exportToGEOS();

    BOOST_CHECK_EQUAL(GeosGeom->toString(), it->second->toString());
    delete GeosGeom;
  }

  Geom = Vect->geometries();

  BOOST_CHECK_EQUAL(Geom->getNumGeometries(), 8);
  BOOST_CHECK_EQUAL(Geom->getDimension(), 1 /* means LineStrings */);

  delete Vect;
  delete Geom;
}


// =====================================================================
// =====================================================================


BOOST_AUTO_TEST_CASE(check_Geometry_Properties)
{
  openfluid::core::GeoVectorValue ValuePolyg(
      CONFIGTESTS_INPUT_MISCDATA_DIR + "/landr", "SU.shp");

  openfluid::landr::VectorDataset* VectPolyg = new openfluid::landr::VectorDataset(
      ValuePolyg);

  BOOST_CHECK(!VectPolyg->isLineType());
  BOOST_CHECK(!VectPolyg->isPointType());
  BOOST_CHECK(VectPolyg->isPolygonType());

  openfluid::core::GeoVectorValue ValueLine(
      CONFIGTESTS_INPUT_MISCDATA_DIR + "/landr", "RS.shp");

  openfluid::landr::VectorDataset* VectLine = new openfluid::landr::VectorDataset(
      ValueLine);

  BOOST_CHECK(VectLine->isLineType());
  BOOST_CHECK(!VectLine->isPointType());
  BOOST_CHECK(!VectLine->isPolygonType());

  delete VectLine;

  openfluid::core::GeoVectorValue ValuePoint(
      CONFIGTESTS_INPUT_MISCDATA_DIR + "/landr", "PU.shp");

  openfluid::landr::VectorDataset* VectPoint = new openfluid::landr::VectorDataset(
      ValuePoint);

  BOOST_CHECK(!VectPoint->isLineType());
  BOOST_CHECK(VectPoint->isPointType());
  BOOST_CHECK(!VectPoint->isPolygonType());

  delete VectPoint;

}


// =====================================================================
// =====================================================================


BOOST_AUTO_TEST_CASE(check_setIndexIntField)
{
  openfluid::core::GeoVectorValue Value(
      CONFIGTESTS_INPUT_MISCDATA_DIR + "/landr", "SU.shp");

  openfluid::landr::VectorDataset* Vect = new openfluid::landr::VectorDataset(
      Value);


  BOOST_CHECK_THROW(Vect->setIndexIntField("WrongField"),
                    openfluid::base::FrameworkException);
  Vect->addAField("Enum",OFTInteger);
  Vect->setIndexIntField("Enum",10);
  BOOST_CHECK_EQUAL(Vect-> isIntValueSet("Enum",10),true);
  BOOST_CHECK_EQUAL(Vect-> isIntValueSet("Enum",11),true);
  BOOST_CHECK_EQUAL(Vect-> isIntValueSet("Enum",12),true);
  BOOST_CHECK_EQUAL(Vect-> isIntValueSet("Enum",33),true);
  BOOST_CHECK_EQUAL(Vect-> isIntValueSet("Enum",34),false);



  delete Vect;
}


// =====================================================================
// =====================================================================


BOOST_AUTO_TEST_CASE(check_envelope)
{
  openfluid::core::GeoVectorValue Value(
      CONFIGTESTS_INPUT_MISCDATA_DIR + "/landr", "SU.shp");

  openfluid::landr::VectorDataset* Vect = new openfluid::landr::VectorDataset(
      Value);

  OGREnvelope envelope=Vect->envelope();

  BOOST_CHECK(openfluid::scientific::isVeryClose(envelope.MinX,679060.43));
  BOOST_CHECK(openfluid::scientific::isVeryClose(envelope.MinY,131916.45));
  BOOST_CHECK(openfluid::scientific::isVeryClose(envelope.MaxX,679837.58));
  BOOST_CHECK(openfluid::scientific::isVeryClose(envelope.MaxY,132960.09));

  delete Vect;

}


// =====================================================================
// =====================================================================


