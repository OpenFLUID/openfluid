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
 @file LandRGraph_TEST.cpp

 @author Aline LIBRES <aline.libres@gmail.com>
 */

#define BOOST_TEST_MAIN
#define BOOST_AUTO_TEST_MAIN
#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MODULE unittest_landrgraph
#include <boost/test/unit_test.hpp>
#include <boost/test/auto_unit_test.hpp>
#include <boost/filesystem/path.hpp>
#include <tests-config.hpp>
#include <openfluid/base/FrameworkException.hpp>
#include <openfluid/core/GeoVectorValue.hpp>
#include <openfluid/core/GeoRasterValue.hpp>
#include <openfluid/core/IntegerValue.hpp>
#include <openfluid/core/StringValue.hpp>
#include <openfluid/landr/PolygonGraph.hpp>
#include <openfluid/landr/LineStringGraph.hpp>
#include <openfluid/landr/VectorDataset.hpp>
#include <openfluid/landr/RasterDataset.hpp>
#include <openfluid/scientific/FloatingPoint.hpp>
#include <geos/geom/LineString.h>
#include <geos/geom/CoordinateSequence.h>


// =====================================================================
// =====================================================================


BOOST_AUTO_TEST_CASE(check_addRemoveAttribute)
{
  openfluid::core::GeoVectorValue* Val = new openfluid::core::GeoVectorValue(
      CONFIGTESTS_INPUT_MISCDATA_DIR + "/landr", "RS.shp");

  openfluid::landr::LineStringGraph* Graph =
      openfluid::landr::LineStringGraph::create(*Val);

  openfluid::landr::LineStringEntity* U1 = Graph->entity(1);
  openfluid::landr::LineStringEntity* U2 = Graph->entity(2);

  openfluid::core::IntegerValue IntValue(0);
  openfluid::core::StringValue StrValue("");

  BOOST_CHECK(
      !U1->setAttributeValue("att",new openfluid::core::IntegerValue(123)));
  BOOST_CHECK(
      !U2->setAttributeValue("att",new openfluid::core::StringValue("val")));
  BOOST_CHECK(!U1->getAttributeValue("att",IntValue));
  BOOST_CHECK(!U2->getAttributeValue("att",StrValue));
  BOOST_CHECK_EQUAL(IntValue.get(), 0);
  BOOST_CHECK_EQUAL(StrValue.get(), "");

  Graph->addAttribute("att");

  BOOST_CHECK(
      U1->setAttributeValue("att",new openfluid::core::IntegerValue(123)));
  BOOST_CHECK(
      U2->setAttributeValue("att",new openfluid::core::StringValue("val")));
  BOOST_CHECK(U1->getAttributeValue("att",IntValue));
  BOOST_CHECK(U2->getAttributeValue("att",StrValue));
  BOOST_CHECK_EQUAL(IntValue.get(), 123);
  BOOST_CHECK_EQUAL(StrValue.get(), "val");

  Graph->addAttribute("att");

  IntValue.set(0);
  StrValue.set("");
  BOOST_CHECK(U1->getAttributeValue("att",IntValue));
  BOOST_CHECK(U2->getAttributeValue("att",StrValue));
  BOOST_CHECK_EQUAL(IntValue.get(), 123);
  BOOST_CHECK_EQUAL(StrValue.get(), "val");

  Graph->removeAttribute("att");

  IntValue.set(0);
  StrValue.set("");
  BOOST_CHECK(
      !U1->setAttributeValue("att",new openfluid::core::IntegerValue(123)));
  BOOST_CHECK(
      !U2->setAttributeValue("att",new openfluid::core::StringValue("val")));
  BOOST_CHECK(!U1->getAttributeValue("att",IntValue));
  BOOST_CHECK(!U2->getAttributeValue("att",StrValue));
  BOOST_CHECK_EQUAL(IntValue.get(), 0);
  BOOST_CHECK_EQUAL(StrValue.get(), "");

  delete Graph;
  delete Val;
}


// =====================================================================
// =====================================================================


BOOST_AUTO_TEST_CASE(check_getARasterValue_fromPolygonGraph)
{
  openfluid::core::GeoVectorValue* Vector = new openfluid::core::GeoVectorValue(
      CONFIGTESTS_INPUT_MISCDATA_DIR + "/landr", "SU.shp");

  openfluid::landr::PolygonGraph* Graph =
      openfluid::landr::PolygonGraph::create(*Vector);

  openfluid::core::GeoRasterValue* Raster = new openfluid::core::GeoRasterValue(
      CONFIGTESTS_INPUT_MISCDATA_DIR + "/GeoRasterValue", "dem.jpeg");

  openfluid::landr::LandREntity* U1 = Graph->LandRGraph::entity(1);
  openfluid::landr::LandREntity* U20 = Graph->LandRGraph::entity(20);

  BOOST_CHECK_THROW(Graph->getRasterValueForEntityCentroid(*U1),
                    openfluid::base::FrameworkException);

  Graph->addAGeoRasterValue(*Raster);

  BOOST_CHECK_EQUAL(Graph->getRasterValueForEntityCentroid(*U1), 29);
  BOOST_CHECK_EQUAL(Graph->getRasterValueForEntityCentroid(*U20), 64);

  delete Graph;
  delete Vector;
  delete Raster;
}


// =====================================================================
// =====================================================================


BOOST_AUTO_TEST_CASE(check_getARasterValue_fromLineStringGraph)
{
  openfluid::core::GeoVectorValue* Vector = new openfluid::core::GeoVectorValue(
      CONFIGTESTS_INPUT_MISCDATA_DIR + "/landr", "RS.shp");

  openfluid::landr::LineStringGraph* Graph =
      openfluid::landr::LineStringGraph::create(*Vector);

  openfluid::core::GeoRasterValue* Raster = new openfluid::core::GeoRasterValue(
      CONFIGTESTS_INPUT_MISCDATA_DIR + "/GeoRasterValue", "dem.jpeg");

  openfluid::landr::LandREntity* U1 = Graph->LandRGraph::entity(1);
  openfluid::landr::LandREntity* U3 = Graph->LandRGraph::entity(3);

  BOOST_CHECK_THROW(Graph->getRasterValueForEntityCentroid(*U1),
                    openfluid::base::FrameworkException);

  Graph->addAGeoRasterValue(*Raster);

  BOOST_CHECK_EQUAL(Graph->getRasterValueForEntityCentroid(*U1), 36);
  BOOST_CHECK_EQUAL(Graph->getRasterValueForEntityCentroid(*U3), 69);

  delete Graph;
  delete Vector;
  delete Raster;
}


// =====================================================================
// =====================================================================


BOOST_AUTO_TEST_CASE(check_getRasterPolygonized)
{
  openfluid::core::GeoVectorValue* Vector = new openfluid::core::GeoVectorValue(
      CONFIGTESTS_INPUT_MISCDATA_DIR + "/landr", "SU.shp");

  openfluid::core::GeoRasterValue* Raster = new openfluid::core::GeoRasterValue(
      CONFIGTESTS_INPUT_MISCDATA_DIR + "/GeoRasterValue", "dem.jpeg");

  openfluid::landr::PolygonGraph* Graph =
      openfluid::landr::PolygonGraph::create(*Vector);

  BOOST_CHECK_THROW(Graph->rasterPolygonized(),
                    openfluid::base::FrameworkException);

  Graph->addAGeoRasterValue(*Raster);

  openfluid::landr::VectorDataset* Polygonized = Graph->rasterPolygonized();

  BOOST_CHECK(Polygonized);
  BOOST_CHECK_EQUAL(Polygonized->layer(0)->GetFeatureCount(), 234);

  BOOST_CHECK_EQUAL(
      Polygonized->layer(0)->GetFeature(0)->GetFieldAsInteger(openfluid::landr::RasterDataset::getDefaultPolygonizedFieldName().c_str()),
      96);
  BOOST_CHECK_EQUAL(
      Polygonized->layer(0)->GetFeature(76)->GetFieldAsInteger(openfluid::landr::RasterDataset::getDefaultPolygonizedFieldName().c_str()),
      83);

  delete Graph;
  delete Vector;
  delete Raster;
}


// =====================================================================
// =====================================================================


BOOST_AUTO_TEST_CASE(check_getRasterPolygonizedMultiPoly)
{
  openfluid::core::GeoVectorValue* Vector = new openfluid::core::GeoVectorValue(
      CONFIGTESTS_INPUT_MISCDATA_DIR + "/landr", "SU.shp");

  openfluid::core::GeoRasterValue* Raster = new openfluid::core::GeoRasterValue(
      CONFIGTESTS_INPUT_MISCDATA_DIR + "/GeoRasterValue", "dem.jpeg");

  openfluid::landr::PolygonGraph* Graph =
      openfluid::landr::PolygonGraph::create(*Vector);

  BOOST_CHECK_THROW(Graph->rasterPolygonizedPolys(),
                    openfluid::base::FrameworkException);

  Graph->addAGeoRasterValue(*Raster);

  BOOST_CHECK_EQUAL(Graph->rasterPolygonizedPolys()->size(), 234);

  delete Graph;
  delete Vector;
  delete Raster;
}


// =====================================================================
// =====================================================================


BOOST_AUTO_TEST_CASE(check_get_AVectorAttribute_from_Id_for_LineStringGraph)
{
  openfluid::core::GeoVectorValue* Vector = new openfluid::core::GeoVectorValue(
      CONFIGTESTS_INPUT_MISCDATA_DIR + "/landr", "RS.shp");

  openfluid::landr::LineStringGraph* Graph =
      openfluid::landr::LineStringGraph::create(*Vector);

  BOOST_CHECK_THROW(Graph->setAttributeFromVectorId("attribut",*Vector, "No_col"),openfluid::base::FrameworkException);

  Graph->setAttributeFromVectorId("attribut",*Vector, "USR_LEN");
  std::vector<std::string> vAttributes=Graph->getAttributeNames();
  BOOST_CHECK_EQUAL(vAttributes.empty(),false);

  openfluid::landr::LineStringEntity* Entity=Graph->entity(1);
  openfluid::core::DoubleValue DoubleValue(0);
  Entity->getAttributeValue("attribut", DoubleValue);
  BOOST_CHECK( openfluid::scientific::isVeryClose(DoubleValue.get(), 191.11));

  Entity=Graph->entity(6);
  Entity->getAttributeValue("attribut", DoubleValue);
  BOOST_CHECK( openfluid::scientific::isVeryClose(DoubleValue.get(), 317.16));

  Entity=Graph->entity(8);
  Entity->getAttributeValue("attribut", DoubleValue);
  BOOST_CHECK( openfluid::scientific::isVeryClose(DoubleValue.get(), 0.0));


  openfluid::core::GeoVectorValue* OtherVector = new openfluid::core::GeoVectorValue(
       CONFIGTESTS_INPUT_MISCDATA_DIR + "/landr", "badRS_misdirected.shp");

  Graph->setAttributeFromVectorId("attribut",*OtherVector, "USR_SLOP");

  Entity=Graph->entity(1);
  Entity->getAttributeValue("attribut", DoubleValue);
  BOOST_CHECK( openfluid::scientific::isVeryClose(DoubleValue.get(), 0.02));

  Entity=Graph->entity(6);
  Entity->getAttributeValue("attribut", DoubleValue);
  BOOST_CHECK( openfluid::scientific::isVeryClose(DoubleValue.get(), 0.06));


  openfluid::core::GeoVectorValue Value(CONFIGTESTS_INPUT_MISCDATA_DIR + "/landr", "badRS_misdirected.shp");

  openfluid::landr::VectorDataset* Vect = new openfluid::landr::VectorDataset(
      Value);


  Graph->setAttributeFromVectorId("attribut",*Vect, "USR_WID");

  Entity=Graph->entity(1);
  Entity->getAttributeValue("attribut", DoubleValue);
  BOOST_CHECK( openfluid::scientific::isVeryClose(DoubleValue.get(),2.0));

  Entity=Graph->entity(6);
  Entity->getAttributeValue("attribut", DoubleValue);
  BOOST_CHECK( openfluid::scientific::isVeryClose(DoubleValue.get(),1.0));


  delete Graph;
  delete Vector;
  delete OtherVector;
  delete Vect;


}


// =====================================================================
// =====================================================================


BOOST_AUTO_TEST_CASE(check_get_AVectorAttribute_from_Id_for_PolygonGraph)
{
  openfluid::core::GeoVectorValue* Vector = new openfluid::core::GeoVectorValue(
      CONFIGTESTS_INPUT_MISCDATA_DIR + "/landr", "SU.shp");

  openfluid::landr::PolygonGraph* Graph =
      openfluid::landr::PolygonGraph::create(*Vector);

  BOOST_CHECK_THROW(Graph->setAttributeFromVectorId("attribut",*Vector, "No_col"),openfluid::base::FrameworkException);

  Graph->setAttributeFromVectorId("attribut",*Vector, "FLOW_CDE");
  std::vector<std::string> vAttributes=Graph->getAttributeNames();
  BOOST_CHECK_EQUAL(vAttributes.empty(),false);

  openfluid::landr::PolygonEntity* Entity=Graph->entity(1);
  openfluid::core::StringValue StringValue("");
  Entity->getAttributeValue("attribut", StringValue);
  BOOST_CHECK_EQUAL( StringValue.get(), "R");

  Entity=Graph->entity(19);
  Entity->getAttributeValue("attribut", StringValue);
  BOOST_CHECK_EQUAL( StringValue.get(), "S");


  delete Graph;
  delete Vector;

}


// =====================================================================
// =====================================================================


BOOST_AUTO_TEST_CASE(check_snap_LineStringGraph)
{
  openfluid::core::GeoVectorValue* Vector = new openfluid::core::GeoVectorValue(
      CONFIGTESTS_INPUT_MISCDATA_DIR + "/landr", "badRS_non_snapped.shp");

  openfluid::landr::LineStringGraph* Graph =
      openfluid::landr::LineStringGraph::create(*Vector);
  BOOST_CHECK_EQUAL(Graph->isLineStringGraphArborescence(),false);

  Graph->snapVertices(3);
  BOOST_CHECK_EQUAL(Graph->isLineStringGraphArborescence(),true);


  delete Graph;
  delete Vector;

}


// =====================================================================
// =====================================================================


BOOST_AUTO_TEST_CASE(check_snap_PolygonGraph)
{
  openfluid::core::GeoVectorValue* Vector = new openfluid::core::GeoVectorValue(
      CONFIGTESTS_INPUT_MISCDATA_DIR + "/landr", "badSU_non_snapped.shp");


  openfluid::landr::PolygonGraph* Graph =
      openfluid::landr::PolygonGraph::create(*Vector);
  openfluid::landr::PolygonEntity* p_Ent1 = Graph->entity(1);
  openfluid::landr::PolygonEntity* p_Ent9 = Graph->entity(9);
  BOOST_CHECK_EQUAL(p_Ent1->getOrderedNeighbourOfldIds().size(), 0);
  BOOST_CHECK_EQUAL(p_Ent9->getOrderedNeighbourOfldIds().size(), 3);

  Graph->snapVertices(8);

  p_Ent1 = Graph->entity(1);
  BOOST_CHECK_EQUAL(p_Ent1->getOrderedNeighbourOfldIds().size(), 2);
  p_Ent9 = Graph->entity(9);
  BOOST_CHECK_EQUAL(p_Ent9->getOrderedNeighbourOfldIds().size(), 4);

  delete Graph;
  delete Vector;

}


// =====================================================================
// =====================================================================


BOOST_AUTO_TEST_CASE(check_get_AVectorAttribute_from_Location_for_LineStringGraph_from_LineStringGraph)
{
  openfluid::core::GeoVectorValue* Vector = new openfluid::core::GeoVectorValue(
      CONFIGTESTS_INPUT_MISCDATA_DIR + "/landr", "RS.shp");

  openfluid::landr::LineStringGraph* Graph =
      openfluid::landr::LineStringGraph::create(*Vector);

  openfluid::core::GeoVectorValue* OtherVector = new openfluid::core::GeoVectorValue(
      CONFIGTESTS_INPUT_MISCDATA_DIR + "/landr", "reach2.shp");

  BOOST_CHECK_THROW(Graph->setAttributeFromVectorLocation("attribut",*Vector, "No_col"),openfluid::base::FrameworkException);

  Graph->setAttributeFromVectorLocation("attribut",*OtherVector, "TYPE",5);
  std::vector<std::string> vAttributes=Graph->getAttributeNames();
  BOOST_CHECK_EQUAL(vAttributes.empty(),false);

  openfluid::landr::LineStringEntity* Entity=Graph->entity(1);
  openfluid::core::IntegerValue IntegerValue(0);
  Entity->getAttributeValue("attribut", IntegerValue);
  BOOST_CHECK_EQUAL( IntegerValue.get(), 15);

  Entity=Graph->entity(2);
  Entity->getAttributeValue("attribut", IntegerValue);
  BOOST_CHECK_EQUAL( IntegerValue.get(), 15);

  Entity=Graph->entity(4);
  Entity->getAttributeValue("attribut", IntegerValue);
  BOOST_CHECK_EQUAL( IntegerValue.get(), 14);

  openfluid::core::GeoVectorValue Value(CONFIGTESTS_INPUT_MISCDATA_DIR + "/landr", "reach2.shp");

  openfluid::landr::VectorDataset* Vect = new openfluid::landr::VectorDataset(Value);
  Graph->setAttributeFromVectorLocation("attribut",*Vect, "MYVALUE",8);
  openfluid::core::DoubleValue DoubleValue(0);

  Entity=Graph->entity(7);
  Entity->getAttributeValue("attribut", DoubleValue);
  BOOST_CHECK( openfluid::scientific::isVeryClose(DoubleValue.get(), 154.26));

  Entity=Graph->entity(1);
  Entity->getAttributeValue("attribut", DoubleValue);
  BOOST_CHECK( openfluid::scientific::isVeryClose(DoubleValue.get(), 17.14));


  Graph->setAttributeFromVectorLocation("attribut",*Vect, "comment",8);
  openfluid::core::StringValue StringValue("");

  Entity=Graph->entity(7);
  Entity->getAttributeValue("attribut", StringValue);
  BOOST_CHECK_EQUAL( StringValue.get(), "reach5");

  Entity=Graph->entity(1);
  Entity->getAttributeValue("attribut", StringValue);
  BOOST_CHECK_EQUAL( StringValue.get(), "reach1");



  delete Graph;
  delete Vector;
  delete OtherVector;
  delete Vect;

}


// =====================================================================
// =====================================================================


BOOST_AUTO_TEST_CASE(check_get_AVectorAttribute_from_Location_for_LineStringGraph_from_PolygonGraph)
{
  openfluid::core::GeoVectorValue* Vector = new openfluid::core::GeoVectorValue(
      CONFIGTESTS_INPUT_MISCDATA_DIR + "/landr", "RS_complex.shp");

  openfluid::landr::LineStringGraph* Graph =
      openfluid::landr::LineStringGraph::create(*Vector);

  openfluid::core::GeoVectorValue* OtherVector = new openfluid::core::GeoVectorValue(
      CONFIGTESTS_INPUT_MISCDATA_DIR + "/landr", "SU.shp");

  BOOST_CHECK_THROW(Graph->setAttributeFromVectorLocation("attribut",*Vector, "No_col"),openfluid::base::FrameworkException);

  Graph->setAttributeFromVectorLocation("attribut",*OtherVector, "OFLD_ID",0.1);
  std::vector<std::string> vAttributes=Graph->getAttributeNames();
  BOOST_CHECK_EQUAL(vAttributes.empty(),false);

  openfluid::landr::LineStringEntity* Entity=Graph->entity(12);
  openfluid::core::IntegerValue IntegerValue(0);
  Entity->getAttributeValue("attribut", IntegerValue);
  BOOST_CHECK_EQUAL( IntegerValue.get(), 4);

  Entity=Graph->entity(14);
  IntegerValue.set(0);
  Entity->getAttributeValue("attribut", IntegerValue);
  BOOST_CHECK_EQUAL( IntegerValue.get(), 0);


  openfluid::core::GeoVectorValue Value(CONFIGTESTS_INPUT_MISCDATA_DIR + "/landr", "SU.shp");

  openfluid::landr::VectorDataset* Vect = new openfluid::landr::VectorDataset(Value);
  Graph->setAttributeFromVectorLocation("attribut",*Vect, "USR_SLOP",0.5);
  openfluid::core::DoubleValue DoubleValue(0);

  Entity=Graph->entity(20);
  Entity->getAttributeValue("attribut", DoubleValue);
  BOOST_CHECK( openfluid::scientific::isVeryClose(DoubleValue.get(), 0.06));

  Entity=Graph->entity(9);
  Entity->getAttributeValue("attribut", DoubleValue);
  BOOST_CHECK( openfluid::scientific::isVeryClose(DoubleValue.get(), 0.05));


  Graph->setAttributeFromVectorLocation("attribut",*Vect, "FLOW_CDE",0.5);
  openfluid::core::StringValue StringValue("");

  Entity=Graph->entity(10);
  Entity->getAttributeValue("attribut", StringValue);
  BOOST_CHECK_EQUAL( StringValue.get(), "R");

  delete Graph;
  delete Vector;
  delete OtherVector;
  delete Vect;

}


// =====================================================================
// =====================================================================


BOOST_AUTO_TEST_CASE(check_get_AVectorAttribute_from_Location_for_PolygonGraph_from_PolygonGraph)
{
  openfluid::core::GeoVectorValue* Vector = new openfluid::core::GeoVectorValue(
      CONFIGTESTS_INPUT_MISCDATA_DIR + "/landr", "soils_extract3.shp");

  openfluid::landr::PolygonGraph* Graph =
      openfluid::landr::PolygonGraph::create(*Vector);

  openfluid::core::GeoVectorValue* OtherVector = new openfluid::core::GeoVectorValue(
      CONFIGTESTS_INPUT_MISCDATA_DIR + "/landr", "fields_extract2.shp");

  BOOST_CHECK_THROW(Graph->setAttributeFromVectorLocation("attribut",*Vector, "No_col"),openfluid::base::FrameworkException);

  Graph->setAttributeFromVectorLocation("attribut",*OtherVector, "OFLD_ID",0.5);
  std::vector<std::string> vAttributes=Graph->getAttributeNames();
  BOOST_CHECK_EQUAL(vAttributes.empty(),false);

  openfluid::landr::PolygonEntity* Entity=Graph->entity(2);
  openfluid::core::IntegerValue IntegerValue(0);
  Entity->getAttributeValue("attribut", IntegerValue);
  BOOST_CHECK_EQUAL( IntegerValue.get(), 5);

  Entity=Graph->entity(10);
  IntegerValue.set(0);
  Entity->getAttributeValue("attribut", IntegerValue);
  BOOST_CHECK_EQUAL( IntegerValue.get(), 9);


  openfluid::core::GeoVectorValue Value(CONFIGTESTS_INPUT_MISCDATA_DIR + "/landr", "fields_extract2.shp");

  openfluid::landr::VectorDataset* Vect = new openfluid::landr::VectorDataset(Value);
  Graph->setAttributeFromVectorLocation("attribut",*Vect, "USR_AREA",0.5);
  openfluid::core::DoubleValue DoubleValue(0);

  Entity=Graph->entity(7);
  Entity->getAttributeValue("attribut", DoubleValue);
  BOOST_CHECK( openfluid::scientific::isVeryClose(DoubleValue.get(), 1610.964));

  Entity=Graph->entity(10);
  Entity->getAttributeValue("attribut", DoubleValue);
  BOOST_CHECK( openfluid::scientific::isVeryClose(DoubleValue.get(), 1806.479));


  Graph->setAttributeFromVectorLocation("attribut",*Vect, "FLOW_CDE",0.5);
  openfluid::core::StringValue StringValue("");

  Entity=Graph->entity(1);
  Entity->getAttributeValue("attribut", StringValue);
  BOOST_CHECK_EQUAL( StringValue.get(), "SU");

  delete Graph;
  delete Vector;
  delete OtherVector;
  delete Vect;

}


// =====================================================================
// =====================================================================


BOOST_AUTO_TEST_CASE(check_get_AVectorAttribute_from_Location_for_PolygonGraph_from_LineStringGraph)
{
  openfluid::core::GeoVectorValue* Vector = new openfluid::core::GeoVectorValue(
      CONFIGTESTS_INPUT_MISCDATA_DIR + "/landr", "SU.shp");

  openfluid::landr::PolygonGraph* Graph =
      openfluid::landr::PolygonGraph::create(*Vector);

  openfluid::core::GeoVectorValue* OtherVector = new openfluid::core::GeoVectorValue(
      CONFIGTESTS_INPUT_MISCDATA_DIR + "/landr", "RS_complex.shp");

  BOOST_CHECK_THROW(Graph->setAttributeFromVectorLocation("attribut",*Vector, "No_col"),openfluid::base::FrameworkException);

  Graph->setAttributeFromVectorLocation("attribut",*OtherVector, "OFLD_ID",10);
  std::vector<std::string> vAttributes=Graph->getAttributeNames();
  BOOST_CHECK_EQUAL(vAttributes.empty(),false);

  openfluid::landr::PolygonEntity* Entity=Graph->entity(16);
  openfluid::core::IntegerValue IntegerValue(0);
  Entity->getAttributeValue("attribut", IntegerValue);
  BOOST_CHECK_EQUAL( IntegerValue.get(), 9);

  Entity=Graph->entity(12);
  IntegerValue.set(0);
  Entity->getAttributeValue("attribut", IntegerValue);
  BOOST_CHECK_EQUAL( IntegerValue.get(), 11);



  openfluid::core::GeoVectorValue Value(CONFIGTESTS_INPUT_MISCDATA_DIR + "/landr", "RS_complex.shp");
  openfluid::landr::VectorDataset* Vect = new openfluid::landr::VectorDataset(Value);
  Graph->setAttributeFromVectorLocation("attribut",*Vect, "USR_HEIG",100);
  openfluid::core::DoubleValue DoubleValue(0);

  Entity=Graph->entity(24);
  Entity->getAttributeValue("attribut", DoubleValue);
  BOOST_CHECK( openfluid::scientific::isVeryClose(DoubleValue.get(), 1.5));
  DoubleValue.set(0);
  Entity=Graph->entity(21);
  Entity->getAttributeValue("attribut", DoubleValue);
  BOOST_CHECK( openfluid::scientific::isVeryClose(DoubleValue.get(),0.0));


  delete Graph;
  delete Vector;
  delete OtherVector;
  delete Vect;

}


// =====================================================================
// =====================================================================


