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
  @file LineStringGraph_TEST.cpp

  @author Aline LIBRES <aline.libres@gmail.com>
  @author Michael RABOTIN <michael.rabotin@supagro.inra.fr>
 */

#define BOOST_TEST_NO_MAIN
#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MODULE unittest_linestringgraph
#include <boost/test/unit_test.hpp>
#include <boost/test/auto_unit_test.hpp>
#include <tests-config.hpp>
#include <openfluid/base/FrameworkException.hpp>
#include <openfluid/base/Environment.hpp>
#include <openfluid/core/GeoVectorValue.hpp>
#include <openfluid/core/GeoRasterValue.hpp>
#include <openfluid/core/IntegerValue.hpp>
#include <openfluid/core/DoubleValue.hpp>
#include <openfluid/core/StringValue.hpp>
#include <openfluid/landr/GEOSHelpers.hpp>
#include <openfluid/landr/LineStringGraph.hpp>
#include <openfluid/landr/VectorDataset.hpp>
#include <openfluid/landr/LandRTools.hpp>
#include <openfluid/scientific/FloatingPoint.hpp>
#include <geos/planargraph/DirectedEdge.h>
#include <geos/planargraph/Node.h>
#include <geos/geom/CoordinateSequence.h>
#include <geos/geom/LineString.h>
#include <geos/geom/LineSegment.h>
#include <algorithm>


// =====================================================================
// =====================================================================


BOOST_AUTO_TEST_CASE(check_construction_fromGeovectorValue)
{
  openfluid::core::GeoVectorValue* Val =
    new openfluid::core::GeoVectorValue(CONFIGTESTS_INPUT_MISCDATA_DIR + "/landr", "RS.shp");

  openfluid::landr::LineStringGraph* Graph = openfluid::landr::LineStringGraph::create(*Val);

  BOOST_CHECK_EQUAL(Graph->getSize(), 8);
  BOOST_CHECK_EQUAL(Graph->getEdges()->size(), 8);

  delete Graph;
  delete Val;
}


// =====================================================================
// =====================================================================


BOOST_AUTO_TEST_CASE(check_construction_fromEntityVector)
{
  openfluid::core::GeoVectorValue Val(CONFIGTESTS_INPUT_MISCDATA_DIR + "/landr", "RS.shp");

  openfluid::landr::VectorDataset* Vect = new openfluid::landr::VectorDataset(Val);

  openfluid::landr::LandRGraph::Entities_t Entities;

  OGRLayer* Layer0 = Vect->layer(0);

  Layer0->ResetReading();

  OGRFeature* Feat;
  while ((Feat = Layer0->GetNextFeature()) != nullptr)
  {
    OGRGeometry* OGRGeom = Feat->GetGeometryRef();

    geos::geom::Geometry* GeosGeom = (geos::geom::Geometry*) openfluid::landr::convertOGRGeometryToGEOS(OGRGeom);

    openfluid::landr::LandREntity* Entity =
        new openfluid::landr::LineStringEntity(dynamic_cast<geos::geom::LineString*>(GeosGeom->clone()),
                                               Feat->GetFieldAsInteger("OFLD_ID"));

    Entities.push_back(Entity);

    // destroying the feature destroys also the associated OGRGeom
    OGRFeature::DestroyFeature(Feat);
    delete GeosGeom;
  }

  openfluid::landr::LineStringGraph* Graph = openfluid::landr::LineStringGraph::create(Entities);

  BOOST_CHECK_EQUAL(Graph->getSize(), 8);
  BOOST_CHECK_EQUAL(Graph->getEdges()->size(), 8);

  delete Graph;
  delete Vect;
}


// =====================================================================
// =====================================================================


BOOST_AUTO_TEST_CASE(check_cloneFromEntityVector)
{
  openfluid::core::GeoVectorValue Val(CONFIGTESTS_INPUT_MISCDATA_DIR + "/landr","RS.shp");

  openfluid::landr::VectorDataset* Vect = new openfluid::landr::VectorDataset(Val);

  openfluid::landr::LandRGraph::Entities_t Entities;

  OGRLayer* Layer0 = Vect->layer(0);

  Layer0->ResetReading();

  OGRFeature* Feat;
  while ((Feat = Layer0->GetNextFeature()) != nullptr)
  {
    OGRGeometry* OGRGeom = Feat->GetGeometryRef();

    geos::geom::Geometry* GeosGeom = (geos::geom::Geometry*) openfluid::landr::convertOGRGeometryToGEOS(OGRGeom);

    openfluid::landr::LandREntity* Entity =
        new openfluid::landr::LineStringEntity(dynamic_cast<geos::geom::LineString*>(GeosGeom->clone()),
                                               Feat->GetFieldAsInteger("OFLD_ID"));

    Entities.push_back(Entity);

    // destroying the feature destroys also the associated OGRGeom
    OGRFeature::DestroyFeature(Feat);
    delete GeosGeom;
  }

  openfluid::landr::LineStringGraph* Graph = openfluid::landr::LineStringGraph::create(Entities);

  //  openfluid::landr::LineStringGraph* Copy = Graph->clone();
  //
  //  BOOST_CHECK_EQUAL(Graph->getSize(), Copy->getSize());
  //  BOOST_CHECK_EQUAL(Graph->getEdges()->size(), Copy->getEdges()->size());
  //  BOOST_CHECK_EQUAL(Graph->getEntities().size(), Copy->getEntities().size());

  delete Graph;
  //  delete Copy;
  delete Vect;
}


// =====================================================================
// =====================================================================


//BOOST_AUTO_TEST_CASE(check_clone)
//{
//  openfluid::core::GeoVectorValue* Val = new openfluid::core::GeoVectorValue(
//      CONFIGTESTS_INPUT_MISCDATA_DIR + "/landr", "RS.shp");
//
//  openfluid::landr::LineStringGraph* Graph =
//      openfluid::landr::LineStringGraph::create(*Val);
//
//  openfluid::landr::LineStringGraph* Copy = Graph->clone();
//
//  BOOST_CHECK_EQUAL(Graph->getSize(), Copy->getSize());
//  BOOST_CHECK_EQUAL(Graph->getEdges()->size(), Copy->getEdges()->size());
//  BOOST_CHECK_EQUAL(Graph->getEntities().size(), Copy->getEntities().size());
//
//  delete Graph;
//  delete Copy;
//  delete Val;
//}


// =====================================================================
// =====================================================================


BOOST_AUTO_TEST_CASE(check_StartsEnds)
{
  openfluid::core::GeoVectorValue* Val =
    new openfluid::core::GeoVectorValue(CONFIGTESTS_INPUT_MISCDATA_DIR + "/landr", "RS.shp");

  openfluid::landr::LineStringGraph* Graph = openfluid::landr::LineStringGraph::create(*Val);

  std::vector<openfluid::landr::LineStringEntity*> Starts = Graph->getStartLineStringEntities();
  BOOST_CHECK_EQUAL(Starts.size(), 4);

  std::vector<openfluid::landr::LineStringEntity*> Ends = Graph->getEndLineStringEntities();
  BOOST_CHECK_EQUAL(Ends.size(), 1);

  BOOST_CHECK_EQUAL(Graph->lastLineStringEntity()->getOfldId(), 1);

  delete Graph;
  delete Val;
}


// =====================================================================
// =====================================================================


BOOST_AUTO_TEST_CASE(check_getEntity)
{
  openfluid::core::GeoVectorValue* Val =
    new openfluid::core::GeoVectorValue(CONFIGTESTS_INPUT_MISCDATA_DIR + "/landr", "RS.shp");

  openfluid::landr::LineStringGraph* Graph = openfluid::landr::LineStringGraph::create(*Val);

  BOOST_CHECK(Graph->entity(1));
  BOOST_CHECK(Graph->entity(5));
  BOOST_CHECK(Graph->entity(3));
  BOOST_CHECK(!Graph->entity(100));

  delete Graph;
  delete Val;
}


// =====================================================================
// =====================================================================


BOOST_AUTO_TEST_CASE(check_getOfldIdOrderedEntities)
{

  openfluid::core::GeoVectorValue* Val =
    new openfluid::core::GeoVectorValue(CONFIGTESTS_INPUT_MISCDATA_DIR + "/landr", "RS.shp");

  openfluid::landr::LineStringGraph* Graph = openfluid::landr::LineStringGraph::create(*Val);

  openfluid::landr::LandRGraph::Entities_t OrderedEntities = Graph->getOfldIdOrderedEntities();

  int i = 0;
  for (openfluid::landr::LandRGraph::Entities_t::iterator it = OrderedEntities.begin(); it != OrderedEntities.end();
       ++it)
  {
    BOOST_CHECK_EQUAL((*it)->getOfldId(), ++i);
  }

  delete Graph;
  delete Val;
}


// =====================================================================
// =====================================================================


BOOST_AUTO_TEST_CASE(check_loopMacros)
{
  openfluid::landr::LineStringEntity* CurrentEntity;
  int i = 0;

  openfluid::landr::LineStringGraph* NullGraph = nullptr;

  DECLARE_ENTITIES_GRAPH_LOOP(1);
  BEGIN_ENTITIES_GRAPH_LOOP(1,NullGraph,CurrentEntity)
  i++;
  END_LOOP

  BOOST_CHECK_EQUAL(i, 0);

  openfluid::core::GeoVectorValue* Val =
    new openfluid::core::GeoVectorValue(CONFIGTESTS_INPUT_MISCDATA_DIR + "/landr", "RS.shp");

  openfluid::landr::LineStringGraph* Graph = openfluid::landr::LineStringGraph::create(*Val);

  i = 0;
  DECLARE_ENTITIES_GRAPH_LOOP(2);
  BEGIN_ENTITIES_GRAPH_LOOP(2,Graph,CurrentEntity)
  i++;
  END_LOOP;

  BOOST_CHECK_EQUAL(i, Graph->getSize());

  i = 1;
  DECLARE_ENTITIES_ORDERED_LOOP(3);
  BEGIN_ENTITIES_ORDERED_LOOP(3,Graph,CurrentEntity)
  BOOST_CHECK_EQUAL(CurrentEntity->getOfldId(),Graph->entity(i)->getOfldId());
  i++;
  END_LOOP;

  delete Graph;
  delete Val;
}


// =====================================================================
// =====================================================================


BOOST_AUTO_TEST_CASE(check_RemoveEntity)
{
  openfluid::core::GeoVectorValue* Val =
    new openfluid::core::GeoVectorValue(CONFIGTESTS_INPUT_MISCDATA_DIR + "/landr", "RS.shp");

  openfluid::landr::LineStringGraph* Graph = openfluid::landr::LineStringGraph::create(*Val);

  std::vector<geos::planargraph::Node*> Nodes;

  BOOST_CHECK_EQUAL(Graph->getSize(), 8);
  BOOST_CHECK_EQUAL(Graph->getEdges()->size(), 8);
  Graph->getNodes(Nodes);
  BOOST_CHECK_EQUAL(Nodes.size(), 9);

  Graph->removeEntity(2);

  BOOST_CHECK(!Graph->entity(2));
  BOOST_CHECK_EQUAL(Graph->getSize(), 7);
  BOOST_CHECK_EQUAL(Graph->getEdges()->size(), 7);
  Nodes.clear();
  Graph->getNodes(Nodes);
  BOOST_CHECK_EQUAL(Nodes.size(), 9);

  Graph->removeEntity(1);

  BOOST_CHECK(!Graph->entity(1));
  BOOST_CHECK_EQUAL(Graph->getSize(), 6);
  BOOST_CHECK_EQUAL(Graph->getEdges()->size(), 6);
  Nodes.clear();
  Graph->getNodes(Nodes);
  BOOST_CHECK_EQUAL(Nodes.size(), 7);

  delete Graph;
  delete Val;
}


// =====================================================================
// =====================================================================


BOOST_AUTO_TEST_CASE(check_setAttributeFromRasterValueAtStartNode_intPixelType)
{
  openfluid::core::GeoVectorValue* Vector =
    new openfluid::core::GeoVectorValue(CONFIGTESTS_INPUT_MISCDATA_DIR + "/landr", "RS.shp");

  openfluid::core::GeoRasterValue* Raster =
    new openfluid::core::GeoRasterValue(CONFIGTESTS_INPUT_MISCDATA_DIR + "/GeoRasterValue", "dem.jpeg");

  openfluid::landr::LineStringGraph* Graph = openfluid::landr::LineStringGraph::create(*Vector);

  Graph->addAGeoRasterValue(*Raster);

  Graph->setAttributeFromRasterValueAtStartNode("test_val");

  openfluid::core::DoubleValue Val;

  Graph->entity(2)->getAttributeValue("test_val", Val);
  BOOST_CHECK_EQUAL(Val.get(), 62);

  Graph->entity(3)->getAttributeValue("test_val", Val);
  BOOST_CHECK_EQUAL(Val.get(), 72);

  delete Graph;
  delete Vector;
  delete Raster;
}


// =====================================================================
// =====================================================================


BOOST_AUTO_TEST_CASE(check_setAttributeFromRasterValueAtStartNode_float32PixelType)
{
  openfluid::core::GeoVectorValue* Vector =
    new openfluid::core::GeoVectorValue(CONFIGTESTS_INPUT_MISCDATA_DIR + "/landr", "RS.shp");

  openfluid::core::GeoRasterValue* Raster =
    new openfluid::core::GeoRasterValue(CONFIGTESTS_INPUT_MISCDATA_DIR + "/GeoRasterValue", "dem.asc");

  openfluid::landr::LineStringGraph* Graph = openfluid::landr::LineStringGraph::create(*Vector);

  Graph->addAGeoRasterValue(*Raster);

  Graph->setAttributeFromRasterValueAtStartNode("test_val");

  openfluid::core::DoubleValue Val;

  Graph->entity(2)->getAttributeValue("test_val", Val);
  BOOST_CHECK( openfluid::scientific::isVeryClose(Val.get(), 59.5801));

  Graph->entity(3)->getAttributeValue("test_val", Val);
  BOOST_CHECK( openfluid::scientific::isVeryClose(Val.get(), 72.1662));

  delete Graph;
  delete Vector;
  delete Raster;
}


// =====================================================================
// =====================================================================


BOOST_AUTO_TEST_CASE(check_setAttributeFromRasterValueAtStartNode_float64PixelType)
{
  openfluid::core::GeoVectorValue* Vector =
    new openfluid::core::GeoVectorValue(CONFIGTESTS_INPUT_MISCDATA_DIR + "/landr", "RS.shp");

  openfluid::core::GeoRasterValue* Raster =
    new openfluid::core::GeoRasterValue(CONFIGTESTS_INPUT_MISCDATA_DIR + "/GeoRasterValue", "dem.Gtiff");

  openfluid::landr::LineStringGraph* Graph = openfluid::landr::LineStringGraph::create(*Vector);

  Graph->addAGeoRasterValue(*Raster);

  Graph->setAttributeFromRasterValueAtStartNode("test_val");

  openfluid::core::DoubleValue Val;

  Graph->entity(2)->getAttributeValue("test_val", Val);
  BOOST_CHECK( openfluid::scientific::isVeryClose(Val.get(), 59.5801));

  Graph->entity(3)->getAttributeValue("test_val", Val);
  BOOST_CHECK( openfluid::scientific::isVeryClose(Val.get(), 72.1662));

  delete Graph;
  delete Vector;
  delete Raster;
}


// =====================================================================
// =====================================================================


BOOST_AUTO_TEST_CASE(check_setAttributeFromRasterValueAtEndNode_intPixelType)
{
  openfluid::core::GeoVectorValue* Vector =
    new openfluid::core::GeoVectorValue(CONFIGTESTS_INPUT_MISCDATA_DIR + "/landr", "RS.shp");

  openfluid::core::GeoRasterValue* Raster =
    new openfluid::core::GeoRasterValue(CONFIGTESTS_INPUT_MISCDATA_DIR + "/GeoRasterValue", "dem.jpeg");

  openfluid::landr::LineStringGraph* Graph =
      openfluid::landr::LineStringGraph::create(*Vector);

  Graph->addAGeoRasterValue(*Raster);

  Graph->setAttributeFromRasterValueAtEndNode("test_val");

  openfluid::core::DoubleValue Val;

  Graph->entity(2)->getAttributeValue("test_val", Val);
  BOOST_CHECK_EQUAL(Val.get(), 46);

  Graph->entity(3)->getAttributeValue("test_val", Val);
  BOOST_CHECK_EQUAL(Val.get(), 62);

  delete Graph;
  delete Vector;
  delete Raster;
}


// =====================================================================
// =====================================================================


BOOST_AUTO_TEST_CASE(check_setAttributeFromRasterValueAtEndNode_float32PixelType)
{
  openfluid::core::GeoVectorValue* Vector =
    new openfluid::core::GeoVectorValue(CONFIGTESTS_INPUT_MISCDATA_DIR + "/landr", "RS.shp");

  openfluid::core::GeoRasterValue* Raster =
    new openfluid::core::GeoRasterValue(CONFIGTESTS_INPUT_MISCDATA_DIR + "/GeoRasterValue", "dem.asc");

  openfluid::landr::LineStringGraph* Graph = openfluid::landr::LineStringGraph::create(*Vector);

  Graph->addAGeoRasterValue(*Raster);

  Graph->setAttributeFromRasterValueAtEndNode("test_val");

  openfluid::core::DoubleValue Val;

  Graph->entity(2)->getAttributeValue("test_val", Val);
  BOOST_CHECK( openfluid::scientific::isVeryClose(Val.get(), 42.3272));

  Graph->entity(3)->getAttributeValue("test_val", Val);
  BOOST_CHECK( openfluid::scientific::isVeryClose(Val.get(), 59.5801));

  delete Graph;
  delete Vector;
  delete Raster;
}


// =====================================================================
// =====================================================================


BOOST_AUTO_TEST_CASE(check_setAttributeFromRasterValueAtEndNode_float64PixelType)
{
  openfluid::core::GeoVectorValue* Vector =
    new openfluid::core::GeoVectorValue(CONFIGTESTS_INPUT_MISCDATA_DIR + "/landr", "RS.shp");

  openfluid::core::GeoRasterValue* Raster =
    new openfluid::core::GeoRasterValue(CONFIGTESTS_INPUT_MISCDATA_DIR + "/GeoRasterValue", "dem.Gtiff");

  openfluid::landr::LineStringGraph* Graph = openfluid::landr::LineStringGraph::create(*Vector);

  Graph->addAGeoRasterValue(*Raster);

  Graph->setAttributeFromRasterValueAtEndNode("test_val");

  openfluid::core::DoubleValue Val;

  Graph->entity(2)->getAttributeValue("test_val", Val);
  BOOST_CHECK( openfluid::scientific::isVeryClose(Val.get(), 42.3272));

  Graph->entity(3)->getAttributeValue("test_val", Val);
  BOOST_CHECK( openfluid::scientific::isVeryClose(Val.get(), 59.5801));

  delete Graph;
  delete Vector;
  delete Raster;
}


// =====================================================================
// =====================================================================


BOOST_AUTO_TEST_CASE(check_setAttributeFromRasterValueAtCentroid_intPixelType)
{
  openfluid::core::GeoVectorValue* Vector =
    new openfluid::core::GeoVectorValue(CONFIGTESTS_INPUT_MISCDATA_DIR + "/landr", "RS.shp");

  openfluid::core::GeoRasterValue* Raster =
    new openfluid::core::GeoRasterValue(CONFIGTESTS_INPUT_MISCDATA_DIR + "/GeoRasterValue", "dem.jpeg");

  openfluid::landr::LineStringGraph* Graph = openfluid::landr::LineStringGraph::create(*Vector);

  Graph->addAGeoRasterValue(*Raster);

  Graph->setAttributeFromRasterValueAtCentroid("test_val");

  openfluid::core::DoubleValue Val;

  Graph->entity(1)->getAttributeValue("test_val", Val);
  BOOST_CHECK_EQUAL(Val.get(), 36);

  Graph->entity(2)->getAttributeValue("test_val", Val);
  BOOST_CHECK_EQUAL(Val.get(), 53);

  delete Graph;
  delete Vector;
  delete Raster;
}


// =====================================================================
// =====================================================================


BOOST_AUTO_TEST_CASE(check_setAttributeFromRasterValueAtCentroid_float32PixelType)
{
  openfluid::core::GeoVectorValue* Vector =
    new openfluid::core::GeoVectorValue(CONFIGTESTS_INPUT_MISCDATA_DIR + "/landr", "RS.shp");

  openfluid::core::GeoRasterValue* Raster =
    new openfluid::core::GeoRasterValue(CONFIGTESTS_INPUT_MISCDATA_DIR + "/GeoRasterValue", "dem.asc");

  openfluid::landr::LineStringGraph* Graph = openfluid::landr::LineStringGraph::create(*Vector);

  Graph->addAGeoRasterValue(*Raster);

  Graph->setAttributeFromRasterValueAtCentroid("test_val");

  openfluid::core::DoubleValue Val;

  Graph->entity(1)->getAttributeValue("test_val", Val);
  BOOST_CHECK( openfluid::scientific::isVeryClose(Val.get(), 35.324));

  Graph->entity(2)->getAttributeValue("test_val", Val);
  BOOST_CHECK( openfluid::scientific::isVeryClose(Val.get(), 51.0379));

  delete Graph;
  delete Vector;
  delete Raster;
}


// =====================================================================
// =====================================================================


BOOST_AUTO_TEST_CASE(check_setAttributeFromRasterValueAtCentroid_float64PixelType)
{
  openfluid::core::GeoVectorValue* Vector =
    new openfluid::core::GeoVectorValue(CONFIGTESTS_INPUT_MISCDATA_DIR + "/landr", "RS.shp");

  openfluid::core::GeoRasterValue* Raster =
    new openfluid::core::GeoRasterValue(CONFIGTESTS_INPUT_MISCDATA_DIR + "/GeoRasterValue", "dem.Gtiff");

  openfluid::landr::LineStringGraph* Graph = openfluid::landr::LineStringGraph::create(*Vector);

  Graph->addAGeoRasterValue(*Raster);

  Graph->setAttributeFromRasterValueAtCentroid("test_val");

  openfluid::core::DoubleValue Val;

  Graph->entity(1)->getAttributeValue("test_val", Val);
  BOOST_CHECK( openfluid::scientific::isVeryClose(Val.get(), 35.324));

  Graph->entity(2)->getAttributeValue("test_val", Val);
  BOOST_CHECK( openfluid::scientific::isVeryClose(Val.get(), 51.0379));

  delete Graph;
  delete Vector;
  delete Raster;
}


// =====================================================================
// =====================================================================


BOOST_AUTO_TEST_CASE(check_reverse_orientation_LineStringEntity)
{
  openfluid::core::GeoVectorValue* Val =
    new openfluid::core::GeoVectorValue(CONFIGTESTS_INPUT_MISCDATA_DIR + "/landr", "RS.shp");

  openfluid::landr::LineStringGraph* Graph = openfluid::landr::LineStringGraph::create(*Val);

  BOOST_CHECK_EQUAL(Graph->getSize(), 8);
  BOOST_CHECK_EQUAL(Graph->getEdges()->size(), 8);
  openfluid::landr::LineStringEntity* U1 = Graph->entity(1);
  geos::planargraph::Node *oldStartNode= U1->startNode();
  geos::planargraph::Node *oldEndNode=U1->endNode();

  Graph->reverseLineStringEntity(*U1);
  openfluid::landr::LineStringEntity* U1reverse = Graph->entity(1);
  BOOST_CHECK(U1reverse->startNode()->getCoordinate().equals(oldEndNode->getCoordinate()));
  BOOST_CHECK(U1reverse->endNode()->getCoordinate().equals(oldStartNode->getCoordinate()));

  delete Graph;
  delete Val;
}


// =====================================================================
// =====================================================================


BOOST_AUTO_TEST_CASE(check_isLineStringGraphArborescence)
{

  openfluid::core::GeoVectorValue* ValRS =
    new openfluid::core::GeoVectorValue(CONFIGTESTS_INPUT_MISCDATA_DIR + "/landr", "RS.shp");

  openfluid::landr::LineStringGraph* Graph = openfluid::landr::LineStringGraph::create(*ValRS);

  BOOST_CHECK_EQUAL(Graph->isLineStringGraphArborescence(),true);
  delete Graph;
  delete ValRS;


  openfluid::core::GeoVectorValue* BadRS_1 =
    new openfluid::core::GeoVectorValue(CONFIGTESTS_INPUT_MISCDATA_DIR + "/landr", "badRS_disconnected.shp");

  openfluid::landr::LineStringGraph* BadGraph_1 = openfluid::landr::LineStringGraph::create(*BadRS_1);

  BOOST_CHECK_EQUAL(BadGraph_1->isLineStringGraphArborescence(),false);
  delete BadRS_1;
  delete BadGraph_1;


  openfluid::core::GeoVectorValue* BadRS_2 =
    new openfluid::core::GeoVectorValue(CONFIGTESTS_INPUT_MISCDATA_DIR + "/landr", "badRS_with_loop.shp");

  openfluid::landr::LineStringGraph* BadGraph_2 = openfluid::landr::LineStringGraph::create(*BadRS_2);

  BOOST_CHECK_EQUAL(BadGraph_2->isLineStringGraphArborescence(),false);
  delete BadRS_2;
  delete BadGraph_2;


  openfluid::core::GeoVectorValue* BadRS_3 =
    new openfluid::core::GeoVectorValue(CONFIGTESTS_INPUT_MISCDATA_DIR + "/landr", "badRS_non_connected.shp");

  openfluid::landr::LineStringGraph* BadGraph_3 = openfluid::landr::LineStringGraph::create(*BadRS_3);

  BOOST_CHECK_EQUAL(BadGraph_3->isLineStringGraphArborescence(),false);
  delete BadRS_3;
  delete BadGraph_3;


  openfluid::core::GeoVectorValue* ValRS4 =
    new openfluid::core::GeoVectorValue(CONFIGTESTS_INPUT_MISCDATA_DIR + "/landr", "badRS_misdirected.shp");

   openfluid::landr::LineStringGraph* Graph4 = openfluid::landr::LineStringGraph::create(*ValRS4);

   BOOST_CHECK_EQUAL(Graph4->isLineStringGraphArborescence(),true);
   delete Graph4;
   delete ValRS4;
}


// =====================================================================
// =====================================================================


BOOST_AUTO_TEST_CASE(check_setAttributeFromMeanRasterValues_intPixelType)
{
  openfluid::core::GeoVectorValue* Vector =
    new openfluid::core::GeoVectorValue(CONFIGTESTS_INPUT_MISCDATA_DIR + "/landr", "RS.shp");

  openfluid::core::GeoRasterValue* Raster =
    new openfluid::core::GeoRasterValue(CONFIGTESTS_INPUT_MISCDATA_DIR + "/GeoRasterValue", "dem.jpeg");

  openfluid::landr::LineStringGraph* Graph = openfluid::landr::LineStringGraph::create(*Vector);

  Graph->addAGeoRasterValue(*Raster);

  Graph->setAttributeFromMeanRasterValues("test_val");

  openfluid::core::DoubleValue Val;

  Graph->entity(1)->getAttributeValue("test_val", Val);
  BOOST_CHECK( openfluid::scientific::isVeryClose(Val.get(), 39.5));

  Graph->entity(2)->getAttributeValue("test_val", Val);
  BOOST_CHECK( openfluid::scientific::isVeryClose(Val.get(),54.0));

  delete Graph;
  delete Vector;
  delete Raster;
}


// =====================================================================
// =====================================================================


BOOST_AUTO_TEST_CASE(check_setAttributeFromMeanRasterValues_float32PixelType)
{
  openfluid::core::GeoVectorValue* Vector =
    new openfluid::core::GeoVectorValue(CONFIGTESTS_INPUT_MISCDATA_DIR + "/landr", "RS.shp");

  openfluid::core::GeoRasterValue* Raster =
    new openfluid::core::GeoRasterValue(CONFIGTESTS_INPUT_MISCDATA_DIR + "/GeoRasterValue", "dem.asc");

  openfluid::landr::LineStringGraph* Graph = openfluid::landr::LineStringGraph::create(*Vector);

  Graph->addAGeoRasterValue(*Raster);

  Graph->setAttributeFromMeanRasterValues("test_val");

  openfluid::core::DoubleValue Val;

  Graph->entity(1)->getAttributeValue("test_val", Val);
  BOOST_CHECK( openfluid::scientific::isVeryClose(Val.get(),  36.6167));

  Graph->entity(2)->getAttributeValue("test_val", Val);
  BOOST_CHECK( openfluid::scientific::isVeryClose(Val.get(), 50.9536));

  delete Graph;
  delete Vector;
  delete Raster;
}


// =====================================================================
// =====================================================================


BOOST_AUTO_TEST_CASE(check_setAttributeFromMeanRasterValues_float64PixelType)
{
  openfluid::core::GeoVectorValue* Vector =
    new openfluid::core::GeoVectorValue(CONFIGTESTS_INPUT_MISCDATA_DIR + "/landr", "RS.shp");

  openfluid::core::GeoRasterValue* Raster =
    new openfluid::core::GeoRasterValue(CONFIGTESTS_INPUT_MISCDATA_DIR + "/GeoRasterValue", "dem.Gtiff");

  openfluid::landr::LineStringGraph* Graph = openfluid::landr::LineStringGraph::create(*Vector);

  Graph->addAGeoRasterValue(*Raster);

  Graph->setAttributeFromMeanRasterValues("test_val");

  openfluid::core::DoubleValue Val;

  Graph->entity(1)->getAttributeValue("test_val", Val);
  BOOST_CHECK( openfluid::scientific::isVeryClose(Val.get(), 36.6167));

  Graph->entity(2)->getAttributeValue("test_val", Val);
  BOOST_CHECK( openfluid::scientific::isVeryClose(Val.get(), 50.9536));

  delete Graph;
  delete Vector;
  delete Raster;
}


// =====================================================================
// =====================================================================


BOOST_AUTO_TEST_CASE(check_mergedLineStringEntity)
{
  openfluid::core::GeoVectorValue* Val =
    new openfluid::core::GeoVectorValue(CONFIGTESTS_INPUT_MISCDATA_DIR + "/landr", "RS.shp");

  openfluid::landr::LineStringGraph* Graph = openfluid::landr::LineStringGraph::create(*Val);
  std::vector<geos::planargraph::Node*> Nodes;


  BOOST_CHECK_THROW(Graph->mergeLineStringEntities(*(Graph->entity(1)),*(Graph->entity(5))),
                    openfluid::base::FrameworkException);
  Graph->getNodes(Nodes);
  BOOST_CHECK_EQUAL(Nodes.size(), 9);


  // first coincidence :   |----2------>|-----1----->
  // result :              |----2------------------->
  double lengthBefore = Graph->entity(2)->getLength()+Graph->entity(1)->getLength();
  Graph->mergeLineStringEntities(*(Graph->entity(2)),*(Graph->entity(1)));
  double lengthAfter = Graph->entity(2)->getLength();
  BOOST_CHECK( openfluid::scientific::isVeryClose(lengthBefore, lengthAfter));
  BOOST_CHECK_EQUAL(Graph->getSize(), 7);
  BOOST_CHECK(!Graph->entity(1));
  BOOST_CHECK(Graph->entity(2)->clone());
  Nodes.clear();
  Graph->getNodes(Nodes);
  BOOST_CHECK_EQUAL(Nodes.size(), 8);
  delete Graph;

  // second coincidence :   |----2------>|-----1----->
  // result :               |----1------------------->
  Graph = openfluid::landr::LineStringGraph::create(*Val);
  lengthBefore = Graph->entity(2)->getLength()+Graph->entity(1)->getLength();
  Graph->mergeLineStringEntities(*(Graph->entity(1)),*(Graph->entity(2)));
  lengthAfter = Graph->entity(1)->getLength();
  BOOST_CHECK(openfluid::scientific::isVeryClose(lengthBefore, lengthAfter));
  BOOST_CHECK_EQUAL(Graph->getSize(), 7);
  BOOST_CHECK(!Graph->entity(2));
  BOOST_CHECK(Graph->entity(1)->clone());
  Nodes.clear();
  Graph->getNodes(Nodes);
  BOOST_CHECK_EQUAL(Nodes.size(), 8);
  delete Graph;

  // third coincidence :   |----2------>|<----1-----|
  // result :              <----1-------------------|
  Graph = openfluid::landr::LineStringGraph::create(*Val);
  lengthBefore = Graph->entity(2)->getLength()+Graph->entity(1)->getLength();
  Graph->reverseLineStringEntity(*(Graph->entity(1)));
  Graph->mergeLineStringEntities(*(Graph->entity(1)),*(Graph->entity(2)));
  lengthAfter = Graph->entity(1)->getLength();
  BOOST_CHECK(openfluid::scientific::isVeryClose(lengthBefore, lengthAfter));
  BOOST_CHECK_EQUAL(Graph->getSize(), 7);
  BOOST_CHECK(!Graph->entity(2));
  BOOST_CHECK(Graph->entity(1)->clone());
  Nodes.clear();
  Graph->getNodes(Nodes);
  BOOST_CHECK_EQUAL(Nodes.size(), 8);
  delete Graph;
  delete Val;

  // fourth coincidence :   <-----2------|-----1----->
  // result :               |----1------------------->
  Val = new openfluid::core::GeoVectorValue(CONFIGTESTS_INPUT_MISCDATA_DIR + "/landr", "RS.shp");
  Graph = openfluid::landr::LineStringGraph::create(*Val);
  lengthBefore = Graph->entity(2)->getLength()+Graph->entity(1)->getLength();
  Graph->reverseLineStringEntity(*(Graph->entity(2)));
  Graph->mergeLineStringEntities(*(Graph->entity(1)),*(Graph->entity(2)));
  lengthAfter = Graph->entity(1)->getLength();

  BOOST_CHECK(openfluid::scientific::isVeryClose(lengthBefore, lengthAfter));
  BOOST_CHECK_EQUAL(Graph->getSize(), 7);
  BOOST_CHECK(!Graph->entity(2));
  BOOST_CHECK(Graph->entity(1)->clone());
  Nodes.clear();
  Graph->getNodes(Nodes);
  BOOST_CHECK_EQUAL(Nodes.size(), 8);

  delete Graph;
  delete Val;
}


// =====================================================================
// =====================================================================


BOOST_AUTO_TEST_CASE(check_getLineStringEntityByMinLength)
{
  openfluid::core::GeoVectorValue* Val =
    new openfluid::core::GeoVectorValue(CONFIGTESTS_INPUT_MISCDATA_DIR + "/landr", "badRS_misdirected.shp");

  openfluid::landr::LineStringGraph* Graph = openfluid::landr::LineStringGraph::create(*Val);

  std::multimap<double,  openfluid::landr::LineStringEntity*> mEntities;

  BOOST_CHECK_THROW(Graph->getLineStringEntitiesByMinLength(-1),openfluid::base::FrameworkException);
  mEntities=Graph->getLineStringEntitiesByMinLength(100,false,true);
  BOOST_CHECK_EQUAL(mEntities.size(), 0);

  mEntities.clear();
  mEntities=Graph->getLineStringEntitiesByMinLength(100,true,true);
  BOOST_CHECK_EQUAL(mEntities.size(), 1);

  mEntities.clear();
  mEntities=Graph->getLineStringEntitiesByMinLength(175,true,true);
  BOOST_CHECK_EQUAL(mEntities.size(), 3);

  mEntities.clear();
  mEntities=Graph->getLineStringEntitiesByMinLength(175,false,true);
  BOOST_CHECK_EQUAL(mEntities.size(), 2);

  mEntities.clear();
  mEntities=Graph->getLineStringEntitiesByMinLength(175,true,false);
  BOOST_CHECK_EQUAL(mEntities.size(), 4);

  mEntities.clear();
  mEntities=Graph->getLineStringEntitiesByMinLength(175,false,false);
  BOOST_CHECK_EQUAL(mEntities.size(), 3);

  delete Graph;
  delete Val;
}


// =====================================================================
// =====================================================================


BOOST_AUTO_TEST_CASE(check_construction_from_MultiLineString)
{
  openfluid::core::GeoVectorValue* Val =
    new openfluid::core::GeoVectorValue(CONFIGTESTS_INPUT_MISCDATA_DIR + "/landr", "MultiLine.shp");

  BOOST_CHECK_THROW(openfluid::landr::LineStringGraph::create(*Val),openfluid::base::FrameworkException);

  delete Val;
}


// =====================================================================
// =====================================================================


BOOST_AUTO_TEST_CASE(setOrientationByOfldId)
{
  openfluid::core::GeoVectorValue* Val =
    new openfluid::core::GeoVectorValue(CONFIGTESTS_INPUT_MISCDATA_DIR + "/landr", "badRS_misdirected.shp");

  openfluid::landr::LineStringGraph* Graph = openfluid::landr::LineStringGraph::create(*Val);

  Graph->setOrientationByOfldId(1);

  openfluid::landr::LineStringEntity * lineEntity=Graph->entity(1);
  geos::planargraph::Node * firstNode=lineEntity->endNode();
  std::vector<int> vectIdent;

  openfluid::landr::LandRTools::markInvertedLineStringEntityUsingDFS(firstNode,vectIdent);
  BOOST_CHECK_EQUAL(vectIdent.size(),0);

  delete Val;
}


// =====================================================================
// =====================================================================


BOOST_AUTO_TEST_CASE(check_mergeLineStringEntitiesByMinLength)
{
  openfluid::core::GeoVectorValue* ValLine =
    new openfluid::core::GeoVectorValue(CONFIGTESTS_INPUT_MISCDATA_DIR + "/landr", "LineToMerge.shp");

  openfluid::landr::LineStringGraph* Graph = openfluid::landr::LineStringGraph::create(*ValLine);


  BOOST_CHECK_THROW(Graph->mergeLineStringEntitiesByMinLength(-1),openfluid::base::FrameworkException);
  BOOST_CHECK_THROW(Graph->mergeLineStringEntitiesByMinLength(0),openfluid::base::FrameworkException);
  BOOST_CHECK_THROW(Graph->mergeLineStringEntitiesByMinLength(-0.000000001),openfluid::base::FrameworkException);


  Graph->mergeLineStringEntitiesByMinLength(10);
  BOOST_CHECK_EQUAL(Graph->getEntities().size(), 8);


  Graph->mergeLineStringEntitiesByMinLength(110,false);
  BOOST_CHECK_EQUAL(Graph->getEntities().size(), 6);
  delete Graph;


  Graph = openfluid::landr::LineStringGraph::create(*ValLine);


  geos::geom::Coordinate OldStart20=Graph->entity(20)->startNode()->getCoordinate();
  geos::geom::Coordinate OldEnd20=Graph->entity(20)->endNode()->getCoordinate();
  geos::geom::Coordinate OldEnd25=Graph->entity(25)->endNode()->getCoordinate();
  geos::geom::Coordinate OldStart26=Graph->entity(26)->startNode()->getCoordinate();
  geos::geom::Coordinate OldEnd26=Graph->entity(26)->endNode()->getCoordinate();

  Graph->mergeLineStringEntitiesByMinLength(110,true);
  BOOST_CHECK_EQUAL(Graph->getEntities().size(), 3);
  BOOST_CHECK(!Graph->entity(24));
  BOOST_CHECK(!Graph->entity(25));
  BOOST_CHECK(!Graph->entity(27));
  BOOST_CHECK(!Graph->entity(22));
  BOOST_CHECK(!Graph->entity(21));
  BOOST_CHECK(Graph->entity(20));
  BOOST_CHECK(Graph->entity(23));
  BOOST_CHECK(Graph->entity(26));

  geos::geom::Coordinate NewStart20=Graph->entity(20)->startNode()->getCoordinate();
  geos::geom::Coordinate NewEnd20=Graph->entity(20)->endNode()->getCoordinate();
  geos::geom::Coordinate NewStart23=Graph->entity(23)->startNode()->getCoordinate();
  geos::geom::Coordinate NewEnd23=Graph->entity(23)->endNode()->getCoordinate();
  geos::geom::Coordinate NewStart26=Graph->entity(26)->startNode()->getCoordinate();
  geos::geom::Coordinate NewEnd26=Graph->entity(26)->endNode()->getCoordinate();

  BOOST_CHECK_EQUAL(NewStart20.equals(OldStart20), true);
  BOOST_CHECK_EQUAL(NewEnd20.equals(OldEnd20), true);
  BOOST_CHECK_EQUAL(NewStart23.equals(OldEnd20), true);
  BOOST_CHECK_EQUAL(NewEnd23.equals(OldEnd25), true);
  BOOST_CHECK_EQUAL(NewStart26.equals(OldStart26), true);
  BOOST_CHECK_EQUAL(NewEnd26.equals(OldEnd26), true);


  delete Graph;
  delete ValLine;

}


// =====================================================================
// =====================================================================


BOOST_AUTO_TEST_CASE(check_construction_from_non_LineType)
{
  openfluid::core::GeoVectorValue* Val =
    new openfluid::core::GeoVectorValue(CONFIGTESTS_INPUT_MISCDATA_DIR + "/landr", "SU.shp");

  openfluid::landr::LineStringGraph* Graph1 = nullptr;
  BOOST_CHECK_THROW(Graph1 = openfluid::landr::LineStringGraph::create(*Val),openfluid::base::FrameworkException);

  delete Val;
  delete Graph1;


  openfluid::core::GeoVectorValue Val2(CONFIGTESTS_INPUT_MISCDATA_DIR + "/landr","SU.shp");

  openfluid::landr::VectorDataset* Vect = new openfluid::landr::VectorDataset(Val2);

  openfluid::landr::LineStringGraph* Graph2 = nullptr;
  BOOST_CHECK_THROW(Graph2 = openfluid::landr::LineStringGraph::create(*Vect),openfluid::base::FrameworkException);

  delete Vect;
  delete Graph2;

  openfluid::core::GeoVectorValue* Val3 =
    new openfluid::core::GeoVectorValue(CONFIGTESTS_INPUT_MISCDATA_DIR + "/landr", "PU.shp");

  openfluid::landr::LineStringGraph* Graph3 = nullptr;
  BOOST_CHECK_THROW(Graph3 = openfluid::landr::LineStringGraph::create(*Val3),openfluid::base::FrameworkException);

  delete Val3;
  delete Graph3;

}


// =====================================================================
// =====================================================================


int main(int argc, char *argv[])
{
  openfluid::base::Environment::init();

  return ::boost::unit_test::unit_test_main( &init_unit_test, argc, argv );
}
