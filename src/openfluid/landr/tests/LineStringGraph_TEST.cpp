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
 \file LineStringGraph_TEST.cpp
 \brief Implements ...

 \author Aline LIBRES <aline.libres@gmail.com>
 */

#define BOOST_TEST_MAIN
#define BOOST_AUTO_TEST_MAIN
#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MODULE unittest_linestringgraph
#include <boost/test/unit_test.hpp>
#include <boost/test/auto_unit_test.hpp>
#include <boost/filesystem/path.hpp>
#include <tests-config.hpp>
#include <openfluid/base/FrameworkException.hpp>
#include <openfluid/core/GeoVectorValue.hpp>
#include <openfluid/core/GeoRasterValue.hpp>
#include <openfluid/core/IntegerValue.hpp>
#include <openfluid/core/DoubleValue.hpp>
#include <openfluid/core/StringValue.hpp>
#include <openfluid/landr/LineStringGraph.hpp>
#include <openfluid/landr/VectorDataset.hpp>
#include <openfluid/landr/LandRTools.hpp>
#include <openfluid/tools.hpp>
#include <geos/planargraph/DirectedEdge.h>
#include <geos/planargraph/Node.h>
#include <geos/geom/CoordinateSequence.h>
#include <geos/geom/LineString.h>
#include <geos/geom/GeometryFactory.h>
#include <geos/geom/LineSegment.h>
#include <geos/geom/Point.h>
#include <algorithm>





// =====================================================================
// =====================================================================

BOOST_AUTO_TEST_CASE(check_construction_fromGeovectorValue)
{
  openfluid::core::GeoVectorValue* Val = new openfluid::core::GeoVectorValue(
      CONFIGTESTS_INPUT_DATASETS_DIR + "/landr", "RS.shp");

  openfluid::landr::LineStringGraph* Graph =
      openfluid::landr::LineStringGraph::create(*Val);

  BOOST_CHECK_EQUAL(Graph->getSize(), 8);
  BOOST_CHECK_EQUAL(Graph->getEdges()->size(), 8);

  delete Graph;
  delete Val;
}

// =====================================================================
// =====================================================================

BOOST_AUTO_TEST_CASE(check_construction_fromEntityVector)
{
  openfluid::core::GeoVectorValue Val(CONFIGTESTS_INPUT_DATASETS_DIR + "/landr",
                                      "RS.shp");

  openfluid::landr::VectorDataset* Vect = new openfluid::landr::VectorDataset(
      Val);

  openfluid::landr::LandRGraph::Entities_t Entities;

  OGRLayer* Layer0 = Vect->getLayer(0);

  Layer0->ResetReading();

  OGRFeature* Feat;
  while ((Feat = Layer0->GetNextFeature()) != NULL)
  {
    OGRGeometry* OGRGeom = Feat->GetGeometryRef();

    geos::geom::Geometry* GeosGeom =
        (geos::geom::Geometry*) OGRGeom->exportToGEOS();

    openfluid::landr::LandREntity* Entity =
        new openfluid::landr::LineStringEntity(
            dynamic_cast<geos::geom::LineString*>(GeosGeom->clone()),
            Feat->GetFieldAsInteger("OFLD_ID"));

    Entities.push_back(Entity);

    // destroying the feature destroys also the associated OGRGeom
    OGRFeature::DestroyFeature(Feat);
    delete GeosGeom;
  }

  openfluid::landr::LineStringGraph* Graph =
      openfluid::landr::LineStringGraph::create(Entities);

  BOOST_CHECK_EQUAL(Graph->getSize(), 8);
  BOOST_CHECK_EQUAL(Graph->getEdges()->size(), 8);

  delete Graph;
  delete Vect;
}

// =====================================================================
// =====================================================================

BOOST_AUTO_TEST_CASE(check_cloneFromEntityVector)
{
  openfluid::core::GeoVectorValue Val(CONFIGTESTS_INPUT_DATASETS_DIR + "/landr",
                                      "RS.shp");

  openfluid::landr::VectorDataset* Vect = new openfluid::landr::VectorDataset(
      Val);

  openfluid::landr::LandRGraph::Entities_t Entities;

  OGRLayer* Layer0 = Vect->getLayer(0);

  Layer0->ResetReading();

  OGRFeature* Feat;
  while ((Feat = Layer0->GetNextFeature()) != NULL)
  {
    OGRGeometry* OGRGeom = Feat->GetGeometryRef();

    geos::geom::Geometry* GeosGeom =
        (geos::geom::Geometry*) OGRGeom->exportToGEOS();

    openfluid::landr::LandREntity* Entity =
        new openfluid::landr::LineStringEntity(
            dynamic_cast<geos::geom::LineString*>(GeosGeom->clone()),
            Feat->GetFieldAsInteger("OFLD_ID"));

    Entities.push_back(Entity);

    // destroying the feature destroys also the associated OGRGeom
    OGRFeature::DestroyFeature(Feat);
    delete GeosGeom;
  }

  openfluid::landr::LineStringGraph* Graph =
      openfluid::landr::LineStringGraph::create(Entities);

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
//      CONFIGTESTS_INPUT_DATASETS_DIR + "/landr", "RS.shp");
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
  openfluid::core::GeoVectorValue* Val = new openfluid::core::GeoVectorValue(
      CONFIGTESTS_INPUT_DATASETS_DIR + "/landr", "RS.shp");

  openfluid::landr::LineStringGraph* Graph =
      openfluid::landr::LineStringGraph::create(*Val);

  std::vector<openfluid::landr::LineStringEntity*> Starts =
      Graph->getStartLineStringEntities();
  BOOST_CHECK_EQUAL(Starts.size(), 4);

  std::vector<openfluid::landr::LineStringEntity*> Ends =
      Graph->getEndLineStringEntities();
  BOOST_CHECK_EQUAL(Ends.size(), 1);

  BOOST_CHECK_EQUAL(Graph->getLastLineStringEntity()->getOfldId(), 1);

  delete Graph;
  delete Val;
}

// =====================================================================
// =====================================================================

BOOST_AUTO_TEST_CASE(check_getEntity)
{
  openfluid::core::GeoVectorValue* Val = new openfluid::core::GeoVectorValue(
      CONFIGTESTS_INPUT_DATASETS_DIR + "/landr", "RS.shp");

  openfluid::landr::LineStringGraph* Graph =
      openfluid::landr::LineStringGraph::create(*Val);

  BOOST_CHECK(Graph->getEntity(1));
  BOOST_CHECK(Graph->getEntity(5));
  BOOST_CHECK(Graph->getEntity(3));
  BOOST_CHECK(!Graph->getEntity(100));

  delete Graph;
  delete Val;
}

// =====================================================================
// =====================================================================

BOOST_AUTO_TEST_CASE(check_getOfldIdOrderedEntities)
{

  openfluid::core::GeoVectorValue* Val = new openfluid::core::GeoVectorValue(
      CONFIGTESTS_INPUT_DATASETS_DIR + "/landr", "RS.shp");

  openfluid::landr::LineStringGraph* Graph =
      openfluid::landr::LineStringGraph::create(*Val);

  openfluid::landr::LandRGraph::Entities_t OrderedEntities =
      Graph->getOfldIdOrderedEntities();

  int i = 0;
  for (openfluid::landr::LandRGraph::Entities_t::iterator it =
      OrderedEntities.begin(); it != OrderedEntities.end(); ++it)
    BOOST_CHECK_EQUAL((*it)->getOfldId(), ++i);

  delete Graph;
  delete Val;
}

// =====================================================================
// =====================================================================

BOOST_AUTO_TEST_CASE(check_loopMacros)
{
  openfluid::landr::LineStringEntity* CurrentEntity;
  int i = 0;

  openfluid::landr::LineStringGraph* NullGraph = 0;

  DECLARE_ENTITIES_GRAPH_LOOP(1);
  BEGIN_ENTITIES_GRAPH_LOOP(1,NullGraph,CurrentEntity)
  i++;
  END_LOOP

  BOOST_CHECK_EQUAL(i, 0);

  openfluid::core::GeoVectorValue* Val = new openfluid::core::GeoVectorValue(
      CONFIGTESTS_INPUT_DATASETS_DIR + "/landr", "RS.shp");

  openfluid::landr::LineStringGraph* Graph =
      openfluid::landr::LineStringGraph::create(*Val);

  i = 0;
  DECLARE_ENTITIES_GRAPH_LOOP(2);
  BEGIN_ENTITIES_GRAPH_LOOP(2,Graph,CurrentEntity)
  i++;
  END_LOOP;

  BOOST_CHECK_EQUAL(i, Graph->getSize());

  i = 1;
  DECLARE_ENTITIES_ORDERED_LOOP(3);
  BEGIN_ENTITIES_ORDERED_LOOP(3,Graph,CurrentEntity)
  BOOST_CHECK_EQUAL(CurrentEntity->getOfldId(),
                    Graph->getEntity(i)->getOfldId());
  i++;
  END_LOOP;

  delete Graph;
  delete Val;
}

// =====================================================================
// =====================================================================

BOOST_AUTO_TEST_CASE(check_RemoveEntity)
{
  openfluid::core::GeoVectorValue* Val = new openfluid::core::GeoVectorValue(
      CONFIGTESTS_INPUT_DATASETS_DIR + "/landr", "RS.shp");

  openfluid::landr::LineStringGraph* Graph =
      openfluid::landr::LineStringGraph::create(*Val);

  std::vector<geos::planargraph::Node*> Nodes;

  BOOST_CHECK_EQUAL(Graph->getSize(), 8);
  BOOST_CHECK_EQUAL(Graph->getEdges()->size(), 8);
  Graph->getNodes(Nodes);
  BOOST_CHECK_EQUAL(Nodes.size(), 9);

  Graph->removeEntity(2);

  BOOST_CHECK(!Graph->getEntity(2));
  BOOST_CHECK_EQUAL(Graph->getSize(), 7);
  BOOST_CHECK_EQUAL(Graph->getEdges()->size(), 7);
  Nodes.clear();
  Graph->getNodes(Nodes);
  BOOST_CHECK_EQUAL(Nodes.size(), 9);

  Graph->removeEntity(1);

  BOOST_CHECK(!Graph->getEntity(1));
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
  openfluid::core::GeoVectorValue* Vector = new openfluid::core::GeoVectorValue(
      CONFIGTESTS_INPUT_DATASETS_DIR + "/landr", "RS.shp");

  openfluid::core::GeoRasterValue* Raster = new openfluid::core::GeoRasterValue(
      CONFIGTESTS_INPUT_DATASETS_DIR + "/GeoRasterValue", "dem.jpeg");

  openfluid::landr::LineStringGraph* Graph =
      openfluid::landr::LineStringGraph::create(*Vector);

  Graph->addAGeoRasterValue(*Raster);

  Graph->setAttributeFromRasterValueAtStartNode("test_val");

  openfluid::core::DoubleValue Val;

  Graph->getEntity(2)->getAttributeValue("test_val", Val);
  BOOST_CHECK_EQUAL(Val.get(), 62);

  Graph->getEntity(3)->getAttributeValue("test_val", Val);
  BOOST_CHECK_EQUAL(Val.get(), 72);

  delete Graph;
  delete Vector;
  delete Raster;
}
// =====================================================================
// =====================================================================

BOOST_AUTO_TEST_CASE(check_setAttributeFromRasterValueAtStartNode_float32PixelType)
{
  openfluid::core::GeoVectorValue* Vector = new openfluid::core::GeoVectorValue(
      CONFIGTESTS_INPUT_DATASETS_DIR + "/landr", "RS.shp");

  openfluid::core::GeoRasterValue* Raster = new openfluid::core::GeoRasterValue(
      CONFIGTESTS_INPUT_DATASETS_DIR + "/GeoRasterValue", "dem.asc");

  openfluid::landr::LineStringGraph* Graph =
      openfluid::landr::LineStringGraph::create(*Vector);

  Graph->addAGeoRasterValue(*Raster);

  Graph->setAttributeFromRasterValueAtStartNode("test_val");

  openfluid::core::DoubleValue Val;

  Graph->getEntity(2)->getAttributeValue("test_val", Val);
  BOOST_CHECK( openfluid::tools::IsVeryClose(Val.get(), 59.5801));

  Graph->getEntity(3)->getAttributeValue("test_val", Val);
  BOOST_CHECK( openfluid::tools::IsVeryClose(Val.get(), 72.1662));

  delete Graph;
  delete Vector;
  delete Raster;
}

// =====================================================================
// =====================================================================

BOOST_AUTO_TEST_CASE(check_setAttributeFromRasterValueAtStartNode_float64PixelType)
{
  openfluid::core::GeoVectorValue* Vector = new openfluid::core::GeoVectorValue(
      CONFIGTESTS_INPUT_DATASETS_DIR + "/landr", "RS.shp");

  openfluid::core::GeoRasterValue* Raster = new openfluid::core::GeoRasterValue(
      CONFIGTESTS_INPUT_DATASETS_DIR + "/GeoRasterValue", "dem.Gtiff");

  openfluid::landr::LineStringGraph* Graph =
      openfluid::landr::LineStringGraph::create(*Vector);

  Graph->addAGeoRasterValue(*Raster);

  Graph->setAttributeFromRasterValueAtStartNode("test_val");

  openfluid::core::DoubleValue Val;

  Graph->getEntity(2)->getAttributeValue("test_val", Val);
  BOOST_CHECK( openfluid::tools::IsVeryClose(Val.get(), 59.5801));

  Graph->getEntity(3)->getAttributeValue("test_val", Val);
  BOOST_CHECK( openfluid::tools::IsVeryClose(Val.get(), 72.1662));

  delete Graph;
  delete Vector;
  delete Raster;
}
// =====================================================================
// =====================================================================

BOOST_AUTO_TEST_CASE(check_setAttributeFromRasterValueAtEndNode_intPixelType)
{
  openfluid::core::GeoVectorValue* Vector = new openfluid::core::GeoVectorValue(
      CONFIGTESTS_INPUT_DATASETS_DIR + "/landr", "RS.shp");

  openfluid::core::GeoRasterValue* Raster = new openfluid::core::GeoRasterValue(
      CONFIGTESTS_INPUT_DATASETS_DIR + "/GeoRasterValue", "dem.jpeg");

  openfluid::landr::LineStringGraph* Graph =
      openfluid::landr::LineStringGraph::create(*Vector);

  Graph->addAGeoRasterValue(*Raster);

  Graph->setAttributeFromRasterValueAtEndNode("test_val");

  openfluid::core::DoubleValue Val;

  Graph->getEntity(2)->getAttributeValue("test_val", Val);
  BOOST_CHECK_EQUAL(Val.get(), 46);

  Graph->getEntity(3)->getAttributeValue("test_val", Val);
  BOOST_CHECK_EQUAL(Val.get(), 62);

  delete Graph;
  delete Vector;
  delete Raster;
}
// =====================================================================
// =====================================================================

BOOST_AUTO_TEST_CASE(check_setAttributeFromRasterValueAtEndNode_float32PixelType)
{
  openfluid::core::GeoVectorValue* Vector = new openfluid::core::GeoVectorValue(
      CONFIGTESTS_INPUT_DATASETS_DIR + "/landr", "RS.shp");

  openfluid::core::GeoRasterValue* Raster = new openfluid::core::GeoRasterValue(
      CONFIGTESTS_INPUT_DATASETS_DIR + "/GeoRasterValue", "dem.asc");

  openfluid::landr::LineStringGraph* Graph =
      openfluid::landr::LineStringGraph::create(*Vector);

  Graph->addAGeoRasterValue(*Raster);

  Graph->setAttributeFromRasterValueAtEndNode("test_val");

  openfluid::core::DoubleValue Val;

  Graph->getEntity(2)->getAttributeValue("test_val", Val);
  BOOST_CHECK( openfluid::tools::IsVeryClose(Val.get(), 42.3272));

  Graph->getEntity(3)->getAttributeValue("test_val", Val);
  BOOST_CHECK( openfluid::tools::IsVeryClose(Val.get(), 59.5801));

  delete Graph;
  delete Vector;
  delete Raster;
}

// =====================================================================
// =====================================================================

BOOST_AUTO_TEST_CASE(check_setAttributeFromRasterValueAtEndNode_float64PixelType)
{
  openfluid::core::GeoVectorValue* Vector = new openfluid::core::GeoVectorValue(
      CONFIGTESTS_INPUT_DATASETS_DIR + "/landr", "RS.shp");

  openfluid::core::GeoRasterValue* Raster = new openfluid::core::GeoRasterValue(
      CONFIGTESTS_INPUT_DATASETS_DIR + "/GeoRasterValue", "dem.Gtiff");

  openfluid::landr::LineStringGraph* Graph =
      openfluid::landr::LineStringGraph::create(*Vector);

  Graph->addAGeoRasterValue(*Raster);

  Graph->setAttributeFromRasterValueAtEndNode("test_val");

  openfluid::core::DoubleValue Val;

  Graph->getEntity(2)->getAttributeValue("test_val", Val);
  BOOST_CHECK( openfluid::tools::IsVeryClose(Val.get(), 42.3272));

  Graph->getEntity(3)->getAttributeValue("test_val", Val);
  BOOST_CHECK( openfluid::tools::IsVeryClose(Val.get(), 59.5801));

  delete Graph;
  delete Vector;
  delete Raster;
}
// =====================================================================
// =====================================================================

BOOST_AUTO_TEST_CASE(check_setAttributeFromRasterValueAtCentroid_intPixelType)
{
  openfluid::core::GeoVectorValue* Vector = new openfluid::core::GeoVectorValue(
      CONFIGTESTS_INPUT_DATASETS_DIR + "/landr", "RS.shp");

  openfluid::core::GeoRasterValue* Raster = new openfluid::core::GeoRasterValue(
      CONFIGTESTS_INPUT_DATASETS_DIR + "/GeoRasterValue", "dem.jpeg");

  openfluid::landr::LineStringGraph* Graph =
      openfluid::landr::LineStringGraph::create(*Vector);

  Graph->addAGeoRasterValue(*Raster);

  Graph->setAttributeFromRasterValueAtCentroid("test_val");

  openfluid::core::DoubleValue Val;

  Graph->getEntity(1)->getAttributeValue("test_val", Val);
  BOOST_CHECK_EQUAL(Val.get(), 36);

  Graph->getEntity(2)->getAttributeValue("test_val", Val);
  BOOST_CHECK_EQUAL(Val.get(), 53);

  delete Graph;
  delete Vector;
  delete Raster;
}

// =====================================================================
// =====================================================================

BOOST_AUTO_TEST_CASE(check_setAttributeFromRasterValueAtCentroid_float32PixelType)
{
  openfluid::core::GeoVectorValue* Vector = new openfluid::core::GeoVectorValue(
      CONFIGTESTS_INPUT_DATASETS_DIR + "/landr", "RS.shp");

  openfluid::core::GeoRasterValue* Raster = new openfluid::core::GeoRasterValue(
      CONFIGTESTS_INPUT_DATASETS_DIR + "/GeoRasterValue", "dem.asc");

  openfluid::landr::LineStringGraph* Graph =
      openfluid::landr::LineStringGraph::create(*Vector);

  Graph->addAGeoRasterValue(*Raster);

  Graph->setAttributeFromRasterValueAtCentroid("test_val");

  openfluid::core::DoubleValue Val;

  Graph->getEntity(1)->getAttributeValue("test_val", Val);
  BOOST_CHECK( openfluid::tools::IsVeryClose(Val.get(), 35.324));

  Graph->getEntity(2)->getAttributeValue("test_val", Val);
  BOOST_CHECK( openfluid::tools::IsVeryClose(Val.get(), 51.0379));

  delete Graph;
  delete Vector;
  delete Raster;
}

// =====================================================================
// =====================================================================

BOOST_AUTO_TEST_CASE(check_setAttributeFromRasterValueAtCentroid_float64PixelType)
{
  openfluid::core::GeoVectorValue* Vector = new openfluid::core::GeoVectorValue(
      CONFIGTESTS_INPUT_DATASETS_DIR + "/landr", "RS.shp");

  openfluid::core::GeoRasterValue* Raster = new openfluid::core::GeoRasterValue(
      CONFIGTESTS_INPUT_DATASETS_DIR + "/GeoRasterValue", "dem.Gtiff");

  openfluid::landr::LineStringGraph* Graph =
      openfluid::landr::LineStringGraph::create(*Vector);

  Graph->addAGeoRasterValue(*Raster);

  Graph->setAttributeFromRasterValueAtCentroid("test_val");

  openfluid::core::DoubleValue Val;

  Graph->getEntity(1)->getAttributeValue("test_val", Val);
  BOOST_CHECK( openfluid::tools::IsVeryClose(Val.get(), 35.324));

  Graph->getEntity(2)->getAttributeValue("test_val", Val);
  BOOST_CHECK( openfluid::tools::IsVeryClose(Val.get(), 51.0379));

  delete Graph;
  delete Vector;
  delete Raster;
}

// =====================================================================
// =====================================================================

BOOST_AUTO_TEST_CASE(check_reverse_orientation_LineStringEntity)
{
  openfluid::core::GeoVectorValue* Val = new openfluid::core::GeoVectorValue(
      CONFIGTESTS_INPUT_DATASETS_DIR + "/landr", "RS.shp");

  openfluid::landr::LineStringGraph* Graph =
      openfluid::landr::LineStringGraph::create(*Val);

  BOOST_CHECK_EQUAL(Graph->getSize(), 8);
  BOOST_CHECK_EQUAL(Graph->getEdges()->size(), 8);
  openfluid::landr::LineStringEntity* U1 = Graph->getEntity(1);
  geos::planargraph::Node *oldStartNode= U1->getStartNode();
  geos::planargraph::Node *oldEndNode=U1->getEndNode();

  Graph->reverseLineStringEntity(*U1);
  openfluid::landr::LineStringEntity* U1reverse = Graph->getEntity(1);
  BOOST_CHECK(U1reverse->getStartNode()->getCoordinate().equals(oldEndNode->getCoordinate()));
  BOOST_CHECK(U1reverse->getEndNode()->getCoordinate().equals(oldStartNode->getCoordinate()));

  delete Graph;
  delete Val;
}

// =====================================================================
// =====================================================================

BOOST_AUTO_TEST_CASE(check_isLineStringGraphArborescence)
{

  openfluid::core::GeoVectorValue* ValRS = new openfluid::core::GeoVectorValue(
      CONFIGTESTS_INPUT_DATASETS_DIR + "/landr", "RS.shp");
  openfluid::landr::LineStringGraph* Graph =
      openfluid::landr::LineStringGraph::create(*ValRS);
  BOOST_CHECK_EQUAL(Graph->isLineStringGraphArborescence(),true);
  delete Graph;
  delete ValRS;

  openfluid::core::GeoVectorValue* BadRS_1 = new openfluid::core::GeoVectorValue(
      CONFIGTESTS_INPUT_DATASETS_DIR + "/landr", "badRS_disconnected.shp");
  openfluid::landr::LineStringGraph* BadGraph_1 =
      openfluid::landr::LineStringGraph::create(*BadRS_1);
  BOOST_CHECK_EQUAL(BadGraph_1->isLineStringGraphArborescence(),false);
  delete BadRS_1;
  delete BadGraph_1;

  openfluid::core::GeoVectorValue* BadRS_2 = new openfluid::core::GeoVectorValue(
      CONFIGTESTS_INPUT_DATASETS_DIR + "/landr", "badRS_with_loop.shp");
  openfluid::landr::LineStringGraph* BadGraph_2 =
      openfluid::landr::LineStringGraph::create(*BadRS_2);
  BOOST_CHECK_EQUAL(BadGraph_2->isLineStringGraphArborescence(),false);
  delete BadRS_2;
  delete BadGraph_2;

  openfluid::core::GeoVectorValue* BadRS_3 = new openfluid::core::GeoVectorValue(
      CONFIGTESTS_INPUT_DATASETS_DIR + "/landr", "badRS_non_connected.shp");
  openfluid::landr::LineStringGraph* BadGraph_3 =
      openfluid::landr::LineStringGraph::create(*BadRS_3);
  BOOST_CHECK_EQUAL(BadGraph_3->isLineStringGraphArborescence(),false);
  delete BadRS_3;
  delete BadGraph_3;


  openfluid::core::GeoVectorValue* ValRS4 = new openfluid::core::GeoVectorValue(
       CONFIGTESTS_INPUT_DATASETS_DIR + "/landr", "badRS_misdirected.shp");
   openfluid::landr::LineStringGraph* Graph4 =
       openfluid::landr::LineStringGraph::create(*ValRS4);
   BOOST_CHECK_EQUAL(Graph4->isLineStringGraphArborescence(),true);
   delete Graph4;
   delete ValRS4;


}

// =====================================================================
// =====================================================================



BOOST_AUTO_TEST_CASE(check_setAttributeFromMeanRasterValues_intPixelType)
{
  openfluid::core::GeoVectorValue* Vector = new openfluid::core::GeoVectorValue(
      CONFIGTESTS_INPUT_DATASETS_DIR + "/landr", "RS.shp");

  openfluid::core::GeoRasterValue* Raster = new openfluid::core::GeoRasterValue(
      CONFIGTESTS_INPUT_DATASETS_DIR + "/GeoRasterValue", "dem.jpeg");

  openfluid::landr::LineStringGraph* Graph =
      openfluid::landr::LineStringGraph::create(*Vector);

  Graph->addAGeoRasterValue(*Raster);

  Graph->setAttributeFromMeanRasterValues("test_val");

  openfluid::core::DoubleValue Val;

  Graph->getEntity(1)->getAttributeValue("test_val", Val);
  BOOST_CHECK( openfluid::tools::IsVeryClose(Val.get(), 39.5));

  Graph->getEntity(2)->getAttributeValue("test_val", Val);
  BOOST_CHECK( openfluid::tools::IsVeryClose(Val.get(), 54));

  delete Graph;
  delete Vector;
  delete Raster;
}

// =====================================================================
// =====================================================================

BOOST_AUTO_TEST_CASE(check_setAttributeFromMeanRasterValues_float32PixelType)
{
  openfluid::core::GeoVectorValue* Vector = new openfluid::core::GeoVectorValue(
      CONFIGTESTS_INPUT_DATASETS_DIR + "/landr", "RS.shp");

  openfluid::core::GeoRasterValue* Raster = new openfluid::core::GeoRasterValue(
      CONFIGTESTS_INPUT_DATASETS_DIR + "/GeoRasterValue", "dem.asc");

  openfluid::landr::LineStringGraph* Graph =
      openfluid::landr::LineStringGraph::create(*Vector);

  Graph->addAGeoRasterValue(*Raster);

  Graph->setAttributeFromMeanRasterValues("test_val");

  openfluid::core::DoubleValue Val;

  Graph->getEntity(1)->getAttributeValue("test_val", Val);
  BOOST_CHECK( openfluid::tools::IsVeryClose(Val.get(),  36.6167));

  Graph->getEntity(2)->getAttributeValue("test_val", Val);
  BOOST_CHECK( openfluid::tools::IsVeryClose(Val.get(), 50.9536));

  delete Graph;
  delete Vector;
  delete Raster;
}

// =====================================================================
// =====================================================================

BOOST_AUTO_TEST_CASE(check_setAttributeFromMeanRasterValues_float64PixelType)
{
  openfluid::core::GeoVectorValue* Vector = new openfluid::core::GeoVectorValue(
      CONFIGTESTS_INPUT_DATASETS_DIR + "/landr", "RS.shp");

  openfluid::core::GeoRasterValue* Raster = new openfluid::core::GeoRasterValue(
      CONFIGTESTS_INPUT_DATASETS_DIR + "/GeoRasterValue", "dem.Gtiff");

  openfluid::landr::LineStringGraph* Graph =
      openfluid::landr::LineStringGraph::create(*Vector);

  Graph->addAGeoRasterValue(*Raster);

  Graph->setAttributeFromMeanRasterValues("test_val");

  openfluid::core::DoubleValue Val;

  Graph->getEntity(1)->getAttributeValue("test_val", Val);
  BOOST_CHECK( openfluid::tools::IsVeryClose(Val.get(), 36.6167));

  Graph->getEntity(2)->getAttributeValue("test_val", Val);
  BOOST_CHECK( openfluid::tools::IsVeryClose(Val.get(), 50.9536));

  delete Graph;
  delete Vector;
  delete Raster;
}

// =====================================================================
// =====================================================================

BOOST_AUTO_TEST_CASE(check_get_AVectorAttribute_from_Location_for_LineStringGraph)
{
  openfluid::core::GeoVectorValue* Vector = new openfluid::core::GeoVectorValue(
      CONFIGTESTS_INPUT_DATASETS_DIR + "/landr", "RS.shp");

  openfluid::landr::LineStringGraph* Graph =
      openfluid::landr::LineStringGraph::create(*Vector);

  openfluid::core::GeoVectorValue* PolygonVector = new openfluid::core::GeoVectorValue(
      CONFIGTESTS_INPUT_DATASETS_DIR + "/landr", "SU.shp");

  BOOST_CHECK_THROW(Graph->setAttributeFromVectorLocation("attribut",*PolygonVector, "OFLD_ID"),openfluid::base::FrameworkException);

  openfluid::core::GeoVectorValue* OtherVector = new openfluid::core::GeoVectorValue(
      CONFIGTESTS_INPUT_DATASETS_DIR + "/landr", "reach2.shp");

  BOOST_CHECK_THROW(Graph->setAttributeFromVectorLocation("attribut",*Vector, "No_col"),openfluid::base::FrameworkException);

  Graph->setAttributeFromVectorLocation("attribut",*OtherVector, "TYPE",0.5);
  std::vector<std::string> vAttributes=Graph->getAttributeNames();
  BOOST_CHECK_EQUAL(vAttributes.empty(),false);

  openfluid::landr::LineStringEntity* Entity=Graph->getEntity(1);
  openfluid::core::IntegerValue IntegerValue(0);
  Entity->getAttributeValue("attribut", IntegerValue);
  BOOST_CHECK_EQUAL( IntegerValue.get(), 15);

  Entity=Graph->getEntity(2);
  Entity->getAttributeValue("attribut", IntegerValue);
  BOOST_CHECK_EQUAL( IntegerValue.get(), 15);

  Entity=Graph->getEntity(4);
  Entity->getAttributeValue("attribut", IntegerValue);
  BOOST_CHECK_EQUAL( IntegerValue.get(), 14);

  openfluid::core::GeoVectorValue Value(CONFIGTESTS_INPUT_DATASETS_DIR + "/landr", "reach2.shp");

  openfluid::landr::VectorDataset* Vect = new openfluid::landr::VectorDataset(Value);
  Graph->setAttributeFromVectorLocation("attribut",*Vect, "myvalue",0.5);
  openfluid::core::DoubleValue DoubleValue(0);

  Entity=Graph->getEntity(7);
  Entity->getAttributeValue("attribut", DoubleValue);
  BOOST_CHECK( openfluid::tools::IsVeryClose(DoubleValue.get(), 154.26));

  Entity=Graph->getEntity(1);
  Entity->getAttributeValue("attribut", DoubleValue);
  BOOST_CHECK( openfluid::tools::IsVeryClose(DoubleValue.get(), 17.14));


  Graph->setAttributeFromVectorLocation("attribut",*Vect, "comment",0.5);
  openfluid::core::StringValue StringValue("");

  Entity=Graph->getEntity(7);
  Entity->getAttributeValue("attribut", StringValue);
  BOOST_CHECK_EQUAL( StringValue.get(), "reach5");

  Entity=Graph->getEntity(1);
  Entity->getAttributeValue("attribut", StringValue);
  BOOST_CHECK_EQUAL( StringValue.get(), "reach1");



  delete Graph;
  delete Vector;
  delete OtherVector;
  delete PolygonVector;
  delete Vect;

}

// =====================================================================
// =====================================================================

BOOST_AUTO_TEST_CASE(check_mergedLineStringEntity)
{
  openfluid::core::GeoVectorValue* Val = new openfluid::core::GeoVectorValue(
      CONFIGTESTS_INPUT_DATASETS_DIR + "/landr", "RS.shp");

  openfluid::landr::LineStringGraph* Graph =
      openfluid::landr::LineStringGraph::create(*Val);
  std::vector<geos::planargraph::Node*> Nodes;


  BOOST_CHECK_THROW(Graph->mergeLineStringEntities(*(Graph->getEntity(1)),*(Graph->getEntity(5))),
                    openfluid::base::FrameworkException);
  Graph->getNodes(Nodes);
  BOOST_CHECK_EQUAL(Nodes.size(), 9);


  // first coincidence :   |----2------>|-----1----->
  // result :              |----2------------------->
  Graph->mergeLineStringEntities(*(Graph->getEntity(2)),*(Graph->getEntity(1)));
  BOOST_CHECK_EQUAL(Graph->getSize(), 7);
  BOOST_CHECK(!Graph->getEntity(1));
  BOOST_CHECK(Graph->getEntity(2)->clone());
  Nodes.clear();
  Graph->getNodes(Nodes);
  BOOST_CHECK_EQUAL(Nodes.size(), 8);
  delete Graph;

  // second coincidence :   |----2------>|-----1----->
  // result :               |----1------------------->
  Graph = openfluid::landr::LineStringGraph::create(*Val);
  Graph->mergeLineStringEntities(*(Graph->getEntity(1)),*(Graph->getEntity(2)));
  BOOST_CHECK_EQUAL(Graph->getSize(), 7);
  BOOST_CHECK(!Graph->getEntity(2));
  BOOST_CHECK(Graph->getEntity(1)->clone());
  Nodes.clear();
  Graph->getNodes(Nodes);
  BOOST_CHECK_EQUAL(Nodes.size(), 8);
  delete Graph;

  // third coincidence :   |----2------>|<----1-----|
  // result :              <----1-------------------|
  Graph = openfluid::landr::LineStringGraph::create(*Val);
  Graph->reverseLineStringEntity(*(Graph->getEntity(1)));
  Graph->mergeLineStringEntities(*(Graph->getEntity(1)),*(Graph->getEntity(2)));
  BOOST_CHECK_EQUAL(Graph->getSize(), 7);
  BOOST_CHECK(!Graph->getEntity(2));
  BOOST_CHECK(Graph->getEntity(1)->clone());
  Nodes.clear();
  Graph->getNodes(Nodes);
  BOOST_CHECK_EQUAL(Nodes.size(), 8);
  delete Graph;

  // fourth coincidence :   <-----2------|-----1----->
  // result :               |----1------------------->
  Graph = openfluid::landr::LineStringGraph::create(*Val);
  Graph->reverseLineStringEntity(*(Graph->getEntity(2)));
  Graph->mergeLineStringEntities(*(Graph->getEntity(1)),*(Graph->getEntity(2)));
  BOOST_CHECK_EQUAL(Graph->getSize(), 7);
  BOOST_CHECK(!Graph->getEntity(2));
  BOOST_CHECK(Graph->getEntity(1)->clone());
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
  openfluid::core::GeoVectorValue* Val = new openfluid::core::GeoVectorValue(
      CONFIGTESTS_INPUT_DATASETS_DIR + "/landr", "badRS_misdirected.shp");

  openfluid::landr::LineStringGraph* Graph =
      openfluid::landr::LineStringGraph::create(*Val);

  std::multimap<double,  openfluid::landr::LineStringEntity*> mEntities;

  BOOST_CHECK_THROW(Graph->getLineStringEntitiesByMinLength(-1),
                    openfluid::base::FrameworkException);
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
  openfluid::core::GeoVectorValue* Val = new openfluid::core::GeoVectorValue(
      CONFIGTESTS_INPUT_DATASETS_DIR + "/landr", "MultiLine.shp");

  openfluid::landr::LineStringGraph* Graph;
  BOOST_CHECK_THROW( Graph =
      openfluid::landr::LineStringGraph::create(*Val),openfluid::base::FrameworkException);

  delete Val;
}

// =====================================================================
// =====================================================================

BOOST_AUTO_TEST_CASE(setOrientationByOfldId)
{
  openfluid::core::GeoVectorValue* Val = new openfluid::core::GeoVectorValue(
      CONFIGTESTS_INPUT_DATASETS_DIR + "/landr", "badRS_misdirected.shp");

  openfluid::landr::LineStringGraph* Graph =
      openfluid::landr::LineStringGraph::create(*Val);

  Graph->setOrientationByOfldId(1);

  openfluid::landr::LineStringEntity * lineEntity=Graph->getEntity(1);
  geos::planargraph::Node * firstNode=lineEntity->getEndNode();
  std::vector<int> vectIdent;

  openfluid::landr::LandRTools::markInvertedLineStringEntityUsingDFS(firstNode,vectIdent);
  BOOST_CHECK_EQUAL(vectIdent.size(),0);

  delete Val;
}

// =====================================================================
// =====================================================================




