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
  @file PolygonGraph_TEST.cpp

  @author Aline LIBRES <aline.libres@gmail.com>
  @author Michael RABOTIN <michael.rabotin@supagro.inra.fr>
 */

#define BOOST_TEST_NO_MAIN
#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MODULE unittest_polygongraph
#include <boost/test/unit_test.hpp>
#include <boost/test/auto_unit_test.hpp>
#include <tests-config.hpp>
#include <openfluid/base/FrameworkException.hpp>
#include <openfluid/base/Environment.hpp>
#include <openfluid/core/GeoVectorValue.hpp>
#include <openfluid/core/GeoRasterValue.hpp>
#include <openfluid/core/IntegerValue.hpp>
#include <openfluid/core/StringValue.hpp>
#include <openfluid/core/DoubleValue.hpp>
#include <openfluid/landr/GdalCompat.hpp>
#include <openfluid/landr/PolygonGraph.hpp>
#include <openfluid/landr/PolygonEntity.hpp>
#include <openfluid/landr/PolygonEdge.hpp>
#include <openfluid/landr/LineStringGraph.hpp>
#include <openfluid/landr/LineStringEntity.hpp>
#include <openfluid/landr/VectorDataset.hpp>
#include <openfluid/scientific/FloatingPoint.hpp>
#include <geos/geom/Geometry.h>
#include <geos/geom/LineString.h>
#include <geos/geom/Polygon.h>
#include <geos/geom/CoordinateArraySequenceFactory.h>
#include <geos/geom/GeometryFactory.h>
#include <geos/geom/MultiLineString.h>
#include <geos/planargraph/Node.h>


// =====================================================================
// =====================================================================


BOOST_AUTO_TEST_CASE(check_construction_fromGeovectorValue)
{
  openfluid::core::GeoVectorValue* Val = new openfluid::core::GeoVectorValue(
      CONFIGTESTS_INPUT_MISCDATA_DIR + "/landr", "SU.shp");

  openfluid::landr::PolygonGraph* Graph =
      openfluid::landr::PolygonGraph::create(*Val);

  BOOST_CHECK_EQUAL(Graph->getSize(), 24);

  BOOST_CHECK_EQUAL(Graph->getEdges()->size(), 58);

  openfluid::landr::LandRGraph::Entities_t Entities = Graph->getEntities();
  std::set<openfluid::landr::PolygonEdge*> Edges;
  for (openfluid::landr::LandRGraph::Entities_t::iterator it = Entities.begin();
      it != Entities.end(); ++it)
    Edges.insert(
        (dynamic_cast<openfluid::landr::PolygonEntity*>(*it))->m_PolyEdges.begin(),
        (dynamic_cast<openfluid::landr::PolygonEntity*>(*it))->m_PolyEdges.end());

  BOOST_CHECK_EQUAL(Edges.size(), 58);

  BOOST_CHECK(Graph->isComplete());

  delete Graph;
  delete Val;
}


// =====================================================================
// =====================================================================


BOOST_AUTO_TEST_CASE(check_construction_fromEntityVector)
{
  openfluid::core::GeoVectorValue Val(CONFIGTESTS_INPUT_MISCDATA_DIR + "/landr",
                                      "SU.shp");

  openfluid::landr::VectorDataset* Vect = new openfluid::landr::VectorDataset(
      Val);

  openfluid::landr::LandRGraph::Entities_t Entities;

  OGRLayer* Layer0 = Vect->layer(0);

  Layer0->ResetReading();

  OGRFeature* Feat;
  while ((Feat = Layer0->GetNextFeature()) != nullptr)
  {
    OGRGeometry* OGRGeom = Feat->GetGeometryRef();

    geos::geom::Geometry* GeosGeom =
        (geos::geom::Geometry*) openfluid::landr::convertOGRGeometryToGEOS(OGRGeom);

    openfluid::landr::LandREntity* Entity = new openfluid::landr::PolygonEntity(
        dynamic_cast<geos::geom::Polygon*>(GeosGeom->clone()),
        Feat->GetFieldAsInteger("OFLD_ID"));

    Entities.push_back(Entity);

    // destroying the feature destroys also the associated OGRGeom
    OGRFeature::DestroyFeature(Feat);
    delete GeosGeom;
  }

  openfluid::landr::PolygonGraph* Graph =
      openfluid::landr::PolygonGraph::create(Entities);

  BOOST_CHECK_EQUAL(Graph->getSize(), 24);
  BOOST_CHECK_EQUAL(Graph->getEdges()->size(), 58);

  BOOST_CHECK(Graph->isComplete());

  delete Graph;
  delete Vect;
}


// =====================================================================
// =====================================================================


BOOST_AUTO_TEST_CASE(check_construction_onePolygon)
{
  // * * * * *
  // *       *
  // *   1   *
  // *       *
  // X * * * *

  geos::geom::CoordinateArraySequenceFactory SeqFactory;
  geos::geom::GeometryFactory Factory;
  openfluid::landr::LandRGraph::Entities_t Entities;

  std::vector<geos::geom::Coordinate>* Coos1 = new std::vector<
      geos::geom::Coordinate>();
  Coos1->push_back(geos::geom::Coordinate(0, 0));
  Coos1->push_back(geos::geom::Coordinate(0, 2));
  Coos1->push_back(geos::geom::Coordinate(2, 2));
  Coos1->push_back(geos::geom::Coordinate(2, 0));
  Coos1->push_back(geos::geom::Coordinate(0, 0));
  geos::geom::LinearRing* LR1 = Factory.createLinearRing(
      SeqFactory.create(Coos1));
  geos::geom::Polygon* P1 = Factory.createPolygon(LR1, nullptr);
  openfluid::landr::PolygonEntity Ent1(P1, 1);
  Entities.push_back(&Ent1);

  openfluid::landr::PolygonGraph* Graph =
      openfluid::landr::PolygonGraph::create(Entities);

  BOOST_CHECK_EQUAL(Graph->getSize(), 1);
  BOOST_CHECK_EQUAL(Graph->getEdges()->size(), 1);
  std::vector<geos::planargraph::Node*> Nodes;
  Graph->getNodes(Nodes);
  BOOST_CHECK_EQUAL(Nodes.size(), 1);
  BOOST_CHECK(Graph->isComplete());

  BOOST_CHECK_EQUAL(Graph->entity(1)->neighboursAndEdges()->size(), 0);

  delete Graph;
}


// =====================================================================
// =====================================================================


BOOST_AUTO_TEST_CASE(check_construction_twoSimplePolygons)
{
  // * * * * *
  // *   1   *
  // X * * * X
  // *   2   *
  // * * * * *

  geos::geom::CoordinateArraySequenceFactory SeqFactory;
  geos::geom::GeometryFactory Factory;
  openfluid::landr::LandRGraph::Entities_t Entites;

  std::vector<geos::geom::Coordinate>* Coos1 = new std::vector<
      geos::geom::Coordinate>();
  Coos1->push_back(geos::geom::Coordinate(0, 2));
  Coos1->push_back(geos::geom::Coordinate(2, 2));
  Coos1->push_back(geos::geom::Coordinate(2, 1));
  Coos1->push_back(geos::geom::Coordinate(0, 1));
  Coos1->push_back(geos::geom::Coordinate(0, 2));
  geos::geom::LinearRing* LR1 = Factory.createLinearRing(
      SeqFactory.create(Coos1));
  geos::geom::Polygon* P1 = Factory.createPolygon(LR1, nullptr);
  openfluid::landr::PolygonEntity Ent1(P1, 1);
  Entites.push_back(&Ent1);

  std::vector<geos::geom::Coordinate>* Coos2 = new std::vector<
      geos::geom::Coordinate>();
  Coos2->push_back(geos::geom::Coordinate(0, 0));
  Coos2->push_back(geos::geom::Coordinate(0, 1));
  Coos2->push_back(geos::geom::Coordinate(2, 1));
  Coos2->push_back(geos::geom::Coordinate(2, 0));
  Coos2->push_back(geos::geom::Coordinate(0, 0));
  geos::geom::LinearRing* LR2 = Factory.createLinearRing(
      SeqFactory.create(Coos2));
  geos::geom::Polygon* P2 = Factory.createPolygon(LR2, nullptr);
  openfluid::landr::PolygonEntity Ent2(P2, 2);
  Entites.push_back(&Ent2);

  openfluid::landr::PolygonGraph* Graph =
      openfluid::landr::PolygonGraph::create(Entites);

  BOOST_CHECK_EQUAL(Graph->getSize(), 2);
  BOOST_CHECK_EQUAL(Graph->getEdges()->size(), 3);
  std::vector<geos::planargraph::Node*> Nodes;
  Graph->getNodes(Nodes);
  BOOST_CHECK_EQUAL(Nodes.size(), 2);
  BOOST_CHECK(Graph->isComplete());

  openfluid::landr::PolygonEntity* p_Ent1 = Graph->entity(1);
  openfluid::landr::PolygonEntity* p_Ent2 = Graph->entity(2);

  BOOST_CHECK_EQUAL(p_Ent1->getOrderedNeighbourOfldIds().size(), 1);
  BOOST_CHECK_EQUAL(p_Ent1->getOrderedNeighbourOfldIds()[0], 2);
  BOOST_CHECK_EQUAL(p_Ent2->getOrderedNeighbourOfldIds().size(), 1);
  BOOST_CHECK_EQUAL(p_Ent2->getOrderedNeighbourOfldIds()[0], 1);

  delete Graph;
}


// =====================================================================
// =====================================================================


BOOST_AUTO_TEST_CASE(check_construction_anIsolatedPolygon)
{
  // * * * * *
  // *   1   *
  // X * * * X   * * *
  // *   3   *   * 2 *
  // * * * * *   X * *

  geos::geom::CoordinateArraySequenceFactory SeqFactory;
  geos::geom::GeometryFactory Factory;
  openfluid::landr::LandRGraph::Entities_t Entites;

  std::vector<geos::geom::Coordinate>* Coos1 = new std::vector<
      geos::geom::Coordinate>();
  Coos1->push_back(geos::geom::Coordinate(0, 1));
  Coos1->push_back(geos::geom::Coordinate(0, 2));
  Coos1->push_back(geos::geom::Coordinate(2, 2));
  Coos1->push_back(geos::geom::Coordinate(2, 1));
  Coos1->push_back(geos::geom::Coordinate(0, 1));
  geos::geom::LinearRing* LR1 = Factory.createLinearRing(
      SeqFactory.create(Coos1));
  geos::geom::Polygon* P1 = Factory.createPolygon(LR1, nullptr);
  openfluid::landr::PolygonEntity Ent1(P1, 1);
  Entites.push_back(&Ent1);

  std::vector<geos::geom::Coordinate>* Coos2 = new std::vector<
      geos::geom::Coordinate>();
  Coos2->push_back(geos::geom::Coordinate(3, 0));
  Coos2->push_back(geos::geom::Coordinate(3, 1));
  Coos2->push_back(geos::geom::Coordinate(4, 1));
  Coos2->push_back(geos::geom::Coordinate(4, 0));
  Coos2->push_back(geos::geom::Coordinate(3, 0));
  geos::geom::LinearRing* LR2 = Factory.createLinearRing(
      SeqFactory.create(Coos2));
  geos::geom::Polygon* P2 = Factory.createPolygon(LR2, nullptr);
  openfluid::landr::PolygonEntity Ent2(P2, 2);
  Entites.push_back(&Ent2);

  std::vector<geos::geom::Coordinate>* Coos3 = new std::vector<
      geos::geom::Coordinate>();
  Coos3->push_back(geos::geom::Coordinate(0, 0));
  Coos3->push_back(geos::geom::Coordinate(0, 1));
  Coos3->push_back(geos::geom::Coordinate(2, 1));
  Coos3->push_back(geos::geom::Coordinate(2, 0));
  Coos3->push_back(geos::geom::Coordinate(0, 0));
  geos::geom::LinearRing* LR3 = Factory.createLinearRing(
      SeqFactory.create(Coos3));
  geos::geom::Polygon* P3 = Factory.createPolygon(LR3, nullptr);
  openfluid::landr::PolygonEntity Ent3(P3, 3);
  Entites.push_back(&Ent3);

  openfluid::landr::PolygonGraph* Graph =
      openfluid::landr::PolygonGraph::create(Entites);

  BOOST_CHECK_EQUAL(Graph->getSize(), 3);
  BOOST_CHECK_EQUAL(Graph->getEdges()->size(), 4);
  std::vector<geos::planargraph::Node*> Nodes;
  Graph->getNodes(Nodes);
  BOOST_CHECK_EQUAL(Nodes.size(), 3);
  BOOST_CHECK(Graph->isComplete());

  openfluid::landr::PolygonEntity* p_Ent1 = Graph->entity(1);
  openfluid::landr::PolygonEntity* p_Ent2 = Graph->entity(2);
  openfluid::landr::PolygonEntity* p_Ent3 = Graph->entity(3);

  BOOST_CHECK_EQUAL(p_Ent1->getOrderedNeighbourOfldIds().size(), 1);
  BOOST_CHECK_EQUAL(p_Ent1->getOrderedNeighbourOfldIds()[0], 3);
  BOOST_CHECK_EQUAL(p_Ent3->getOrderedNeighbourOfldIds().size(), 1);
  BOOST_CHECK_EQUAL(p_Ent3->getOrderedNeighbourOfldIds()[0], 1);
  BOOST_CHECK_EQUAL(p_Ent2->getOrderedNeighbourOfldIds().size(), 0);

  delete Graph;
}


// =====================================================================
// =====================================================================


BOOST_AUTO_TEST_CASE(check_construction_aFullEnclosedPolygon)
{
  // * * * * X * *
  // * 1     *   *
  // *   * * X   *
  // *   * 2 *   *
  // *   X * X   *
  // *   * 4 * 3 *
  // * * X * X * *

  geos::geom::CoordinateArraySequenceFactory SeqFactory;
  geos::geom::GeometryFactory Factory;
  openfluid::landr::LandRGraph::Entities_t Entites;

  std::vector<geos::geom::Coordinate>* Coos1 = new std::vector<
      geos::geom::Coordinate>();
  Coos1->push_back(geos::geom::Coordinate(0, 0));
  Coos1->push_back(geos::geom::Coordinate(0, 3));
  Coos1->push_back(geos::geom::Coordinate(2, 3));
  Coos1->push_back(geos::geom::Coordinate(2, 2));
  Coos1->push_back(geos::geom::Coordinate(1, 2));
  Coos1->push_back(geos::geom::Coordinate(1, 0));
  Coos1->push_back(geos::geom::Coordinate(0, 0));
  geos::geom::LinearRing* LR1 = Factory.createLinearRing(
      SeqFactory.create(Coos1));
  geos::geom::Polygon* P1 = Factory.createPolygon(LR1, nullptr);
  openfluid::landr::PolygonEntity Ent1(P1, 1);
  Entites.push_back(&Ent1);

  std::vector<geos::geom::Coordinate>* Coos2 = new std::vector<
      geos::geom::Coordinate>();
  Coos2->push_back(geos::geom::Coordinate(1, 1));
  Coos2->push_back(geos::geom::Coordinate(1, 2));
  Coos2->push_back(geos::geom::Coordinate(2, 2));
  Coos2->push_back(geos::geom::Coordinate(2, 1));
  Coos2->push_back(geos::geom::Coordinate(1, 1));
  geos::geom::LinearRing* LR2 = Factory.createLinearRing(
      SeqFactory.create(Coos2));
  geos::geom::Polygon* P2 = Factory.createPolygon(LR2, nullptr);
  openfluid::landr::PolygonEntity Ent2(P2, 2);
  Entites.push_back(&Ent2);

  openfluid::landr::PolygonGraph* TmpGraph =
      openfluid::landr::PolygonGraph::create(Entites);

  BOOST_CHECK_EQUAL(TmpGraph->getSize(), 2);
  BOOST_CHECK_EQUAL(TmpGraph->getEdges()->size(), 3);
  std::vector<geos::planargraph::Node*> TmpNodes;
  TmpGraph->getNodes(TmpNodes);
  BOOST_CHECK_EQUAL(TmpNodes.size(), 2);
  BOOST_CHECK(TmpGraph->isComplete());

  std::vector<geos::geom::Coordinate>* Coos3 = new std::vector<
      geos::geom::Coordinate>();
  Coos3->push_back(geos::geom::Coordinate(3, 0));
  Coos3->push_back(geos::geom::Coordinate(3, 3));
  Coos3->push_back(geos::geom::Coordinate(2, 3));
  Coos3->push_back(geos::geom::Coordinate(2, 0));
  Coos3->push_back(geos::geom::Coordinate(3, 0));
  geos::geom::LinearRing* LR3 = Factory.createLinearRing(
      SeqFactory.create(Coos3));
  geos::geom::Polygon* P3 = Factory.createPolygon(LR3, nullptr);
  openfluid::landr::PolygonEntity Ent3(P3, 3);
  Entites.push_back(&Ent3);

  std::vector<geos::geom::Coordinate>* Coos4 = new std::vector<
      geos::geom::Coordinate>();
  Coos4->push_back(geos::geom::Coordinate(2, 0));
  Coos4->push_back(geos::geom::Coordinate(2, 1));
  Coos4->push_back(geos::geom::Coordinate(1, 1));
  Coos4->push_back(geos::geom::Coordinate(1, 0));
  Coos4->push_back(geos::geom::Coordinate(2, 0));
  geos::geom::LinearRing* LR4 = Factory.createLinearRing(
      SeqFactory.create(Coos4));
  geos::geom::Polygon* P4 = Factory.createPolygon(LR4, nullptr);
  openfluid::landr::PolygonEntity Ent4(P4, 4);
  Entites.push_back(&Ent4);

  openfluid::landr::PolygonGraph* Graph =
      openfluid::landr::PolygonGraph::create(Entites);

  BOOST_CHECK_EQUAL(Graph->getSize(), 4);
  BOOST_CHECK_EQUAL(Graph->getEdges()->size(), 9);
  std::vector<geos::planargraph::Node*> Nodes;
  Graph->getNodes(Nodes);
  BOOST_CHECK_EQUAL(Nodes.size(), 6);
  BOOST_CHECK(Graph->isComplete());

  BOOST_CHECK_EQUAL(Graph->entity(1)->neighboursAndEdges()->size(), 3);
  BOOST_CHECK_EQUAL(Graph->entity(2)->neighboursAndEdges()->size(), 3);
  BOOST_CHECK_EQUAL(Graph->entity(3)->neighboursAndEdges()->size(), 3);

  delete Graph;
  delete TmpGraph;
}


// =====================================================================
// =====================================================================


BOOST_AUTO_TEST_CASE(check_construction_twoNonIntersectingPolygons)
{
  //         X * *
  //         * 2 *
  // * * * * * * *
  // *       *
  // *   1   *
  // *       *
  // X * * * *

  geos::geom::CoordinateArraySequenceFactory SeqFactory;
  geos::geom::GeometryFactory Factory;
  openfluid::landr::LandRGraph::Entities_t Entites;

  std::vector<geos::geom::Coordinate>* Coos1 = new std::vector<
      geos::geom::Coordinate>();
  Coos1->push_back(geos::geom::Coordinate(0, 0));
  Coos1->push_back(geos::geom::Coordinate(0, 2));
  Coos1->push_back(geos::geom::Coordinate(2, 2));
  Coos1->push_back(geos::geom::Coordinate(2, 0));
  Coos1->push_back(geos::geom::Coordinate(0, 0));
  geos::geom::LinearRing* LR1 = Factory.createLinearRing(
      SeqFactory.create(Coos1));
  geos::geom::Polygon* P1 = Factory.createPolygon(LR1, nullptr);
  openfluid::landr::PolygonEntity Ent1(P1, 1);
  Entites.push_back(&Ent1);

  std::vector<geos::geom::Coordinate>* Coos2 = new std::vector<
      geos::geom::Coordinate>();
  Coos2->push_back(geos::geom::Coordinate(2, 3));
  Coos2->push_back(geos::geom::Coordinate(3, 3));
  Coos2->push_back(geos::geom::Coordinate(3, 2));
  Coos2->push_back(geos::geom::Coordinate(2, 2));
  Coos2->push_back(geos::geom::Coordinate(2, 3));
  geos::geom::LinearRing* LR2 = Factory.createLinearRing(
      SeqFactory.create(Coos2));
  geos::geom::Polygon* P2 = Factory.createPolygon(LR2, nullptr);
  openfluid::landr::PolygonEntity Ent2(P2, 2);
  Entites.push_back(&Ent2);

  openfluid::landr::PolygonGraph* Graph =
      openfluid::landr::PolygonGraph::create(Entites);

  BOOST_CHECK_EQUAL(Graph->getSize(), 2);
  BOOST_CHECK_EQUAL(Graph->getEdges()->size(), 2);
  std::vector<geos::planargraph::Node*> Nodes;
  Graph->getNodes(Nodes);
  BOOST_CHECK_EQUAL(Nodes.size(), 2);
  BOOST_CHECK(Graph->isComplete());

  BOOST_CHECK_EQUAL(Graph->entity(1)->neighboursAndEdges()->size(), 0);
  BOOST_CHECK_EQUAL(Graph->entity(2)->neighboursAndEdges()->size(), 0);

  delete Graph;
}


// =====================================================================
// =====================================================================


BOOST_AUTO_TEST_CASE(check_construction_horseshoeShapedPolygons_linesContact)
{
  openfluid::core::GeoVectorValue* Val = new openfluid::core::GeoVectorValue(
      CONFIGTESTS_INPUT_MISCDATA_DIR + "/landr", "SU_horseshoe_lines.shp");

  openfluid::landr::PolygonGraph* Graph =
      openfluid::landr::PolygonGraph::create(*Val);

  BOOST_CHECK_EQUAL(Graph->getSize(), 3);
  BOOST_CHECK_EQUAL(Graph->getEdges()->size(), 6);
  std::vector<geos::planargraph::Node*> Nodes;
  Graph->getNodes(Nodes);
  BOOST_CHECK_EQUAL(Nodes.size(), 4);
  BOOST_CHECK(Graph->isComplete());

  BOOST_CHECK_EQUAL(Graph->entity(1)->neighboursAndEdges()->size(), 2);
  BOOST_CHECK_EQUAL(Graph->entity(2)->neighboursAndEdges()->size(), 2);
  BOOST_CHECK_EQUAL(Graph->entity(3)->neighboursAndEdges()->size(), 2);

  delete Graph;
  delete Val;
}


// =====================================================================
// =====================================================================


BOOST_AUTO_TEST_CASE(check_construction_horseshoeShapedPolygons_pointContact)
{
  openfluid::core::GeoVectorValue* Val = new openfluid::core::GeoVectorValue(
      CONFIGTESTS_INPUT_MISCDATA_DIR + "/landr", "SU_horseshoe_point.shp");

  openfluid::landr::PolygonGraph* Graph =
      openfluid::landr::PolygonGraph::create(*Val);

  BOOST_CHECK_EQUAL(Graph->getSize(), 3);
  BOOST_CHECK_EQUAL(Graph->getEdges()->size(), 5);
  std::vector<geos::planargraph::Node*> Nodes;
  Graph->getNodes(Nodes);
  BOOST_CHECK_EQUAL(Nodes.size(), 3);
  BOOST_CHECK(Graph->isComplete());

  BOOST_CHECK_EQUAL(Graph->entity(1)->neighboursAndEdges()->size(), 2);
  BOOST_CHECK_EQUAL(Graph->entity(2)->neighboursAndEdges()->size(), 2);
  BOOST_CHECK_EQUAL(Graph->entity(3)->neighboursAndEdges()->size(), 2);

  delete Graph;
  delete Val;
}


// =====================================================================
// =====================================================================


BOOST_AUTO_TEST_CASE(check_getEntity)
{
  openfluid::core::GeoVectorValue* Val = new openfluid::core::GeoVectorValue(
      CONFIGTESTS_INPUT_MISCDATA_DIR + "/landr", "SU.shp");

  openfluid::landr::PolygonGraph* Graph =
      openfluid::landr::PolygonGraph::create(*Val);

  BOOST_CHECK(Graph->entity(1));
  BOOST_CHECK(Graph->entity(5));
  BOOST_CHECK(Graph->entity(3));
  BOOST_CHECK(!Graph->entity(100));

  delete Graph;
  delete Val;
}


// =====================================================================
// =====================================================================


BOOST_AUTO_TEST_CASE(check_addRemoveAttribute)
{
  openfluid::core::GeoVectorValue* Val = new openfluid::core::GeoVectorValue(
      CONFIGTESTS_INPUT_MISCDATA_DIR + "/landr", "SU.shp");

  openfluid::landr::PolygonGraph* Graph =
      openfluid::landr::PolygonGraph::create(*Val);

  openfluid::landr::PolygonEntity* U1 = Graph->entity(1);
  openfluid::landr::PolygonEntity* U2 = Graph->entity(2);

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

  IntValue = 0;
  StrValue = std::string("");
  BOOST_CHECK(
      !U1->setAttributeValue("att",new openfluid::core::IntegerValue(123)));
  BOOST_CHECK(
      !U2->setAttributeValue("att",new openfluid::core::StringValue("val")));
  BOOST_CHECK(!U1->getAttributeValue("att",IntValue));
  BOOST_CHECK(!U2->getAttributeValue("att",StrValue));
  BOOST_CHECK_EQUAL(IntValue.get(), 0);
  BOOST_CHECK_EQUAL(StrValue.get(), "");

  Graph->removeAttribute("att");

  delete Graph;
  delete Val;
}


// =====================================================================
// =====================================================================


BOOST_AUTO_TEST_CASE(check_getOfldIdOrderedEntities)
{
  openfluid::core::GeoVectorValue* Val = new openfluid::core::GeoVectorValue(
      CONFIGTESTS_INPUT_MISCDATA_DIR + "/landr", "SU.shp");

  openfluid::landr::PolygonGraph* Graph =
      openfluid::landr::PolygonGraph::create(*Val);

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


BOOST_AUTO_TEST_CASE(check_computeRasterPolyOverlapping_gettingPolygonsOnly)
{
  openfluid::core::GeoVectorValue* Vector = new openfluid::core::GeoVectorValue(
      CONFIGTESTS_INPUT_MISCDATA_DIR + "/landr", "SU.shp");

  openfluid::core::GeoRasterValue* Raster = new openfluid::core::GeoRasterValue(
      CONFIGTESTS_INPUT_MISCDATA_DIR + "/GeoRasterValue", "dem.jpeg");

  openfluid::landr::PolygonGraph* Graph =
      openfluid::landr::PolygonGraph::create(*Vector);

  openfluid::landr::PolygonEntity* U1 = Graph->entity(1);

  BOOST_CHECK_THROW(Graph->computeRasterPolyOverlapping(*U1),
                    openfluid::base::FrameworkException);

  Graph->addAGeoRasterValue(*Raster);

  openfluid::landr::PolygonGraph::RastValByRastPoly_t OverlapsU1 =
      Graph->computeRasterPolyOverlapping(*U1);

  BOOST_CHECK_EQUAL(OverlapsU1.size(), 12);

  double Area;

  for (openfluid::landr::PolygonGraph::RastValByRastPoly_t::iterator it =
      OverlapsU1.begin(); it != OverlapsU1.end(); ++it)
  {
    Area += it->second;

    BOOST_CHECK((int*)it->first->getUserData());
  }

  BOOST_CHECK(openfluid::scientific::isVeryClose(Area,U1->getArea()));

  delete Graph;
  delete Vector;
  delete Raster;
}


// =====================================================================
// =====================================================================


BOOST_AUTO_TEST_CASE(check_computeRasterPolyOverlapping_gettingAlsoMultiPolygon)
{
  openfluid::core::GeoVectorValue* Vector = new openfluid::core::GeoVectorValue(
      CONFIGTESTS_INPUT_MISCDATA_DIR + "/landr", "SU.shp");

  openfluid::core::GeoRasterValue* Raster = new openfluid::core::GeoRasterValue(
      CONFIGTESTS_INPUT_MISCDATA_DIR + "/GeoRasterValue", "dem.jpeg");

  openfluid::landr::PolygonGraph* Graph =
      openfluid::landr::PolygonGraph::create(*Vector);

  Graph->addAGeoRasterValue(*Raster);

  openfluid::landr::PolygonEntity* U2 = Graph->entity(2);

  openfluid::landr::PolygonGraph::RastValByRastPoly_t OverlapsU2 =
      Graph->computeRasterPolyOverlapping(*U2);

  BOOST_CHECK_EQUAL(OverlapsU2.size(), 20);

  double Area = 0;

  for (openfluid::landr::PolygonGraph::RastValByRastPoly_t::iterator it =
      OverlapsU2.begin(); it != OverlapsU2.end(); ++it)
  {
    Area += it->second;

    BOOST_CHECK((int*)it->first->getUserData());
  }

  BOOST_CHECK(openfluid::scientific::isVeryClose(Area,U2->getArea()));

  delete Graph;
  delete Vector;
  delete Raster;
}


// =====================================================================
// =====================================================================


BOOST_AUTO_TEST_CASE(check_setAttributeFromRasterValueAtCentroid_intPixelType)
{
  openfluid::core::GeoVectorValue* Vector = new openfluid::core::GeoVectorValue(
      CONFIGTESTS_INPUT_MISCDATA_DIR + "/landr", "SU.shp");

  openfluid::core::GeoRasterValue* Raster = new openfluid::core::GeoRasterValue(
      CONFIGTESTS_INPUT_MISCDATA_DIR + "/GeoRasterValue", "dem.jpeg");

  openfluid::landr::PolygonGraph* Graph =
      openfluid::landr::PolygonGraph::create(*Vector);

  Graph->addAGeoRasterValue(*Raster);

  Graph->setAttributeFromRasterValueAtCentroid("test_val");

  openfluid::core::DoubleValue Val;

  Graph->entity(1)->getAttributeValue("test_val", Val);
  BOOST_CHECK_EQUAL(Val.get(), 29);

  Graph->entity(2)->getAttributeValue("test_val", Val);
  BOOST_CHECK_EQUAL(Val.get(), 47);

  delete Graph;
  delete Vector;
  delete Raster;
}


// =====================================================================
// =====================================================================


BOOST_AUTO_TEST_CASE(check_setAttributeFromRasterValueAtCentroid_float32PixelType)
{
  openfluid::core::GeoVectorValue* Vector = new openfluid::core::GeoVectorValue(
      CONFIGTESTS_INPUT_MISCDATA_DIR + "/landr", "SU.shp");

  openfluid::core::GeoRasterValue* Raster = new openfluid::core::GeoRasterValue(
      CONFIGTESTS_INPUT_MISCDATA_DIR + "/GeoRasterValue", "dem.asc");

  openfluid::landr::PolygonGraph* Graph =
      openfluid::landr::PolygonGraph::create(*Vector);

  Graph->addAGeoRasterValue(*Raster);

  Graph->setAttributeFromRasterValueAtCentroid("test_val");

  openfluid::core::DoubleValue Val;

  Graph->entity(1)->getAttributeValue("test_val", Val);
  BOOST_CHECK( openfluid::scientific::isVeryClose(Val.get(), 32.9131));

  Graph->entity(2)->getAttributeValue("test_val", Val);
  BOOST_CHECK( openfluid::scientific::isVeryClose(Val.get(), 51.0607));

  delete Graph;
  delete Vector;
  delete Raster;
}


// =====================================================================
// =====================================================================


BOOST_AUTO_TEST_CASE(check_setAttributeFromRasterValueAtCentroid_float64PixelType)
{
  openfluid::core::GeoVectorValue* Vector = new openfluid::core::GeoVectorValue(
      CONFIGTESTS_INPUT_MISCDATA_DIR + "/landr", "SU.shp");

  openfluid::core::GeoRasterValue* Raster = new openfluid::core::GeoRasterValue(
      CONFIGTESTS_INPUT_MISCDATA_DIR + "/GeoRasterValue", "dem.Gtiff");

  openfluid::landr::PolygonGraph* Graph =
      openfluid::landr::PolygonGraph::create(*Vector);

  Graph->addAGeoRasterValue(*Raster);

  Graph->setAttributeFromRasterValueAtCentroid("test_val");

  openfluid::core::DoubleValue Val;

  Graph->entity(1)->getAttributeValue("test_val", Val);
  BOOST_CHECK( openfluid::scientific::isVeryClose(Val.get(), 32.9131));

  Graph->entity(2)->getAttributeValue("test_val", Val);
  BOOST_CHECK( openfluid::scientific::isVeryClose(Val.get(), 51.0607));

  delete Graph;
  delete Vector;
  delete Raster;
}


// =====================================================================
// =====================================================================


BOOST_AUTO_TEST_CASE(check_setAttributeFromMeanRasterValues_intPixelType)
{
  openfluid::core::GeoVectorValue* Vector = new openfluid::core::GeoVectorValue(
      CONFIGTESTS_INPUT_MISCDATA_DIR + "/landr", "SU.shp");

  openfluid::core::GeoRasterValue* Raster = new openfluid::core::GeoRasterValue(
      CONFIGTESTS_INPUT_MISCDATA_DIR + "/GeoRasterValue", "dem.jpeg");

  openfluid::landr::PolygonGraph* Graph =
      openfluid::landr::PolygonGraph::create(*Vector);

  Graph->addAGeoRasterValue(*Raster);

  Graph->setAttributeFromMeanRasterValues("test_val");

  openfluid::core::DoubleValue Val;

  Graph->entity(1)->getAttributeValue("test_val", Val);
  BOOST_CHECK( openfluid::scientific::isVeryClose(Val.get(), 34.0569));

  Graph->entity(2)->getAttributeValue("test_val", Val);
  BOOST_CHECK( openfluid::scientific::isVeryClose(Val.get(), 46.6497));

  delete Graph;
  delete Vector;
  delete Raster;
}


// =====================================================================
// =====================================================================


BOOST_AUTO_TEST_CASE(check_setAttributeFromMeanRasterValues_float32PixelType)
{
  openfluid::core::GeoVectorValue* Vector = new openfluid::core::GeoVectorValue(
      CONFIGTESTS_INPUT_MISCDATA_DIR + "/landr", "SU.shp");

  openfluid::core::GeoRasterValue* Raster = new openfluid::core::GeoRasterValue(
      CONFIGTESTS_INPUT_MISCDATA_DIR + "/GeoRasterValue", "dem.asc");

  openfluid::landr::PolygonGraph* Graph =
      openfluid::landr::PolygonGraph::create(*Vector);

  Graph->addAGeoRasterValue(*Raster);

  Graph->setAttributeFromMeanRasterValues("test_val");

  openfluid::core::DoubleValue Val;

  Graph->entity(1)->getAttributeValue("test_val", Val);
  BOOST_CHECK( openfluid::scientific::isVeryClose(Val.get(),  33.6118));

  Graph->entity(2)->getAttributeValue("test_val", Val);
  BOOST_CHECK( openfluid::scientific::isVeryClose(Val.get(), 46.8027));

  delete Graph;
  delete Vector;
  delete Raster;
}


// =====================================================================
// =====================================================================


BOOST_AUTO_TEST_CASE(check_setAttributeFromMeanRasterValues_float64PixelType)
{
  openfluid::core::GeoVectorValue* Vector = new openfluid::core::GeoVectorValue(
      CONFIGTESTS_INPUT_MISCDATA_DIR + "/landr", "SU.shp");

  openfluid::core::GeoRasterValue* Raster = new openfluid::core::GeoRasterValue(
      CONFIGTESTS_INPUT_MISCDATA_DIR + "/GeoRasterValue", "dem.Gtiff");

  openfluid::landr::PolygonGraph* Graph =
      openfluid::landr::PolygonGraph::create(*Vector);

  Graph->addAGeoRasterValue(*Raster);

  Graph->setAttributeFromMeanRasterValues("test_val");

  openfluid::core::DoubleValue Val;

  Graph->entity(1)->getAttributeValue("test_val", Val);
  BOOST_CHECK( openfluid::scientific::isVeryClose(Val.get(), 33.6118));

  Graph->entity(2)->getAttributeValue("test_val", Val);
  BOOST_CHECK( openfluid::scientific::isVeryClose(Val.get(), 46.8027));

  delete Graph;
  delete Vector;
  delete Raster;
}


// =====================================================================
// =====================================================================


BOOST_AUTO_TEST_CASE(check_createVectorRepresentation)
{
  openfluid::core::GeoVectorValue* Val = new openfluid::core::GeoVectorValue(
      CONFIGTESTS_INPUT_MISCDATA_DIR + "/landr", "SU.shp");

  openfluid::landr::PolygonGraph* Graph =
      openfluid::landr::PolygonGraph::create(*Val);

  Graph->createVectorRepresentation(CONFIGTESTS_OUTPUT_DATA_DIR + "/landr",
                                    "SUGraph.shp");

  openfluid::core::GeoVectorValue GraphVal(
      CONFIGTESTS_OUTPUT_DATA_DIR + "/landr", "SUGraph.shp");

  openfluid::landr::VectorDataset* GraphVect =
      new openfluid::landr::VectorDataset(GraphVal);

  BOOST_CHECK_EQUAL(Graph->getEdges()->size(), 58);
  BOOST_CHECK_EQUAL(GraphVect->layer(0)->GetFeatureCount(), 58);

  delete GraphVect;
  delete Graph;
  delete Val;
}


// =====================================================================
// =====================================================================


BOOST_AUTO_TEST_CASE(check_computeLineStringNeighboursOfPolygonGraph_Contains)
{
  openfluid::core::GeoVectorValue* ValRS = new openfluid::core::GeoVectorValue(
      CONFIGTESTS_INPUT_MISCDATA_DIR + "/landr", "RS.shp");

  openfluid::core::GeoVectorValue* ValSU = new openfluid::core::GeoVectorValue(
      CONFIGTESTS_INPUT_MISCDATA_DIR + "/landr", "SU.shp");

  openfluid::landr::LineStringGraph* RSGraph =
      openfluid::landr::LineStringGraph::create(*ValRS);
  openfluid::landr::PolygonGraph* SUGraph =
      openfluid::landr::PolygonGraph::create(*ValSU);

  openfluid::landr::PolygonEntity* SU1 = SUGraph->entity(1);
  openfluid::landr::PolygonEntity* SU4 = SUGraph->entity(4);
  openfluid::landr::PolygonEntity* SU17 = SUGraph->entity(17);

  BOOST_CHECK(!SU1->lineStringNeighbours());
  BOOST_CHECK(!SU4->lineStringNeighbours());
  BOOST_CHECK(!SU17->lineStringNeighbours());

  SUGraph->computeLineStringNeighbours(*RSGraph,
                                       openfluid::landr::LandRTools::CONTAINS,
                                       0);

  BOOST_CHECK_EQUAL(SU1->lineStringNeighbours()->size(), 0);
  BOOST_CHECK_EQUAL(SU4->lineStringNeighbours()->size(), 0);
  BOOST_CHECK_EQUAL(SU17->lineStringNeighbours()->size(), 0);

  BOOST_CHECK_EQUAL(SU1->neighbours()->size(), 2);
  BOOST_CHECK_EQUAL(SU4->neighbours()->size(), 4);
  BOOST_CHECK_EQUAL(SU17->neighbours()->size(), 4);

  SUGraph->computeLineStringNeighbours(*RSGraph,
                                       openfluid::landr::LandRTools::CONTAINS,
                                       0.0001);

  BOOST_CHECK_EQUAL(SU1->lineStringNeighbours()->size(), 1);
  BOOST_CHECK_EQUAL(SU1->lineStringNeighbours()->begin()->first->getOfldId(),
                    1);
  BOOST_CHECK_EQUAL(SU4->lineStringNeighbours()->size(), 1);
  BOOST_CHECK_EQUAL(SU4->lineStringNeighbours()->begin()->first->getOfldId(),
                    3);
  BOOST_CHECK_EQUAL(SU17->lineStringNeighbours()->size(), 1);
  BOOST_CHECK_EQUAL(
      SU17->lineStringNeighbours()->begin()->first->getOfldId(), 6);

  BOOST_CHECK_EQUAL(SU1->neighbours()->size(), 3);
  BOOST_CHECK_EQUAL(SU4->neighbours()->size(), 5);
  BOOST_CHECK_EQUAL(SU17->neighbours()->size(), 5);

  delete RSGraph;
  delete SUGraph;
  delete ValRS;
  delete ValSU;
}


// =====================================================================
// =====================================================================


BOOST_AUTO_TEST_CASE(check_computeLineStringNeighboursOfPolygonGraph_Intersect)
{
  openfluid::core::GeoVectorValue* ValRS = new openfluid::core::GeoVectorValue(
      CONFIGTESTS_INPUT_MISCDATA_DIR + "/landr", "RS.shp");

  openfluid::core::GeoVectorValue* ValSU = new openfluid::core::GeoVectorValue(
      CONFIGTESTS_INPUT_MISCDATA_DIR + "/landr", "SU.shp");

  openfluid::landr::LineStringGraph* RSGraph =
      openfluid::landr::LineStringGraph::create(*ValRS);
  openfluid::landr::PolygonGraph* SUGraph =
      openfluid::landr::PolygonGraph::create(*ValSU);

  openfluid::landr::PolygonEntity* SU1 = SUGraph->entity(1);
  openfluid::landr::PolygonEntity* SU4 = SUGraph->entity(4);
  openfluid::landr::PolygonEntity* SU17 = SUGraph->entity(17);

  BOOST_CHECK(!SU1->lineStringNeighbours());
  BOOST_CHECK(!SU4->lineStringNeighbours());
  BOOST_CHECK(!SU17->lineStringNeighbours());

  SUGraph->computeLineStringNeighbours(*RSGraph,
                                       openfluid::landr::LandRTools::INTERSECTS,
                                       0);

  BOOST_CHECK_EQUAL(SU1->lineStringNeighbours()->size(), 0);
  BOOST_CHECK_EQUAL(SU4->lineStringNeighbours()->size(), 0);
  BOOST_CHECK_EQUAL(SU17->lineStringNeighbours()->size(), 0);

  BOOST_CHECK_EQUAL(SU1->neighbours()->size(), 2);
  BOOST_CHECK_EQUAL(SU4->neighbours()->size(), 4);
  BOOST_CHECK_EQUAL(SU17->neighbours()->size(), 4);

  SUGraph->computeLineStringNeighbours(*RSGraph,
                                       openfluid::landr::LandRTools::INTERSECTS,
                                       0.0001);

  BOOST_CHECK_EQUAL(SU1->lineStringNeighbours()->size(), 2);
  BOOST_CHECK_EQUAL(SU4->lineStringNeighbours()->size(), 6);
  BOOST_CHECK_EQUAL(SU17->lineStringNeighbours()->size(), 3);

  BOOST_CHECK_EQUAL(SU1->neighbours()->size(), 4);
  BOOST_CHECK_EQUAL(SU4->neighbours()->size(), 10);
  BOOST_CHECK_EQUAL(SU17->neighbours()->size(), 7);

  delete RSGraph;
  delete SUGraph;
  delete ValRS;
  delete ValSU;
}


// =====================================================================
// =====================================================================


BOOST_AUTO_TEST_CASE(check_polygon_has_islands)
{
  openfluid::core::GeoVectorValue* Val = new openfluid::core::GeoVectorValue(
      CONFIGTESTS_INPUT_MISCDATA_DIR + "/landr", "SU.shp");

  openfluid::landr::PolygonGraph* Graph =
      openfluid::landr::PolygonGraph::create(*Val);

  openfluid::core::GeoVectorValue* PolHole = new openfluid::core::GeoVectorValue(
      CONFIGTESTS_INPUT_MISCDATA_DIR + "/landr", "SU-has-hole.shp");

  openfluid::landr::PolygonGraph* GraphHole =
      openfluid::landr::PolygonGraph::create(*PolHole);

  openfluid::core::GeoVectorValue* PolIsland = new openfluid::core::GeoVectorValue(
      CONFIGTESTS_INPUT_MISCDATA_DIR + "/landr", "SU-has-islands.shp");

  openfluid::landr::PolygonGraph* GraphIsland =
      openfluid::landr::PolygonGraph::create(*PolIsland);


  BOOST_CHECK_EQUAL(Graph->hasIsland(), false);
  BOOST_CHECK_EQUAL(GraphHole->hasIsland(), false);
  BOOST_CHECK_EQUAL(GraphIsland->hasIsland(), true);

  delete Graph;
  delete Val;
  delete GraphIsland;
  delete PolIsland;
  delete GraphHole;
  delete PolHole;
}


// =====================================================================
// =====================================================================


BOOST_AUTO_TEST_CASE(check_addRemoveEdgeAttribute)
{
  openfluid::core::GeoVectorValue* Val = new openfluid::core::GeoVectorValue(
      CONFIGTESTS_INPUT_MISCDATA_DIR + "/landr", "SU.shp");

  openfluid::landr::PolygonGraph* Graph =
      openfluid::landr::PolygonGraph::create(*Val);

  std::vector<std::string> VEdgeAttributes;
  VEdgeAttributes=Graph->getEdgeAttributeNames();
  BOOST_CHECK(VEdgeAttributes.empty());

  openfluid::core::DoubleValue DoubleVal;

  Graph->createEdgeAttribute("att",DoubleVal);
  VEdgeAttributes=Graph->getEdgeAttributeNames();
  BOOST_CHECK(!VEdgeAttributes.empty());

  Graph->removeEdgeAttribute("att");
  VEdgeAttributes.clear();
  VEdgeAttributes=Graph->getEdgeAttributeNames();
  BOOST_CHECK(VEdgeAttributes.empty());


  openfluid::core::IntegerValue IntVal=123;
  Graph->createEdgeAttribute("att",IntVal);
  openfluid::landr::PolygonEntity* U1 = Graph->entity(1);
  std::vector<openfluid::landr::PolygonEdge*> vPolyEdge=U1->m_PolyEdges;
  openfluid::core::IntegerValue IntVal2;
  (*vPolyEdge.begin())->getAttributeValue("att",IntVal2);
  BOOST_CHECK_EQUAL(IntVal2.get(),123);

  Graph->removeEdgeAttribute("att");

  delete Graph;
  delete Val;
}


// =====================================================================
// =====================================================================


BOOST_AUTO_TEST_CASE(check_remove_PolygonEntity)
{
  openfluid::core::GeoVectorValue* Vector = new openfluid::core::GeoVectorValue(
      CONFIGTESTS_INPUT_MISCDATA_DIR + "/landr", "SU.shp");

  openfluid::landr::PolygonGraph* Graph =
      openfluid::landr::PolygonGraph::create(*Vector);

  BOOST_CHECK_EQUAL(Graph->getSize(),24);

  Graph->computeNeighbours();
  openfluid::landr::PolygonEntity* Ent=Graph->entity(10);

  BOOST_CHECK_EQUAL(Ent->getOrderedNeighbourOfldIds().size(),3);
  BOOST_CHECK_EQUAL(Graph->isComplete(),true);
  BOOST_CHECK_EQUAL(Graph->getEdges()->size(),58);

  Graph->removeEntity(9);

  BOOST_CHECK_EQUAL(Graph->getSize(),23);
  BOOST_CHECK_EQUAL(Graph->isComplete(),true);
  BOOST_CHECK_EQUAL(Graph->getEdges()->size(),58);
  BOOST_CHECK(!Graph->entity(9));

  Ent=Graph->entity(10);
  BOOST_CHECK_EQUAL(Ent->getOrderedNeighbourOfldIds().size(),2);

  Ent=Graph->entity(7);
  BOOST_CHECK_EQUAL(Ent->getOrderedNeighbourOfldIds().size(),3);

  Ent=Graph->entity(12);
  BOOST_CHECK_EQUAL(Ent->getOrderedNeighbourOfldIds().size(),3);

  Ent=Graph->entity(11);
  BOOST_CHECK_EQUAL(Ent->getOrderedNeighbourOfldIds().size(),2);

  Graph->removeEntity(10);

  BOOST_CHECK_EQUAL(Graph->getSize(),22);
  BOOST_CHECK_EQUAL(Graph->isComplete(),true);
  BOOST_CHECK_EQUAL(Graph->getEdges()->size(),53);
  BOOST_CHECK(!Graph->entity(10));

  Ent=Graph->entity(11);
  BOOST_CHECK_EQUAL(Ent->getOrderedNeighbourOfldIds().size(),1);

  delete Graph;
  delete Vector;


}


// =====================================================================
// =====================================================================


BOOST_AUTO_TEST_CASE(check_getPolygonEntityByMinArea)
{
  openfluid::core::GeoVectorValue* Vector = new openfluid::core::GeoVectorValue(
      CONFIGTESTS_INPUT_MISCDATA_DIR + "/landr", "SU.shp");

  openfluid::landr::PolygonGraph* Graph =
      openfluid::landr::PolygonGraph::create(*Vector);


  std::multimap<double,  openfluid::landr::PolygonEntity*> mEntities;

  BOOST_CHECK_THROW(Graph->getPolygonEntitiesByMinArea(-1),
                    openfluid::base::FrameworkException);
  BOOST_CHECK_THROW(Graph->getPolygonEntitiesByMinArea(0),
                    openfluid::base::FrameworkException);
  mEntities=Graph->getPolygonEntitiesByMinArea(1);
  BOOST_CHECK_EQUAL(mEntities.size(), 0);

  mEntities.clear();
  mEntities=Graph->getPolygonEntitiesByMinArea(1,false);
  BOOST_CHECK_EQUAL(mEntities.size(), 0);
  mEntities.clear();
  mEntities=Graph->getPolygonEntitiesByMinArea(20000);
  BOOST_CHECK_EQUAL(mEntities.size(), 10);
  mEntities.clear();
  mEntities=Graph->getPolygonEntitiesByMinArea(20000,false);
  BOOST_CHECK_EQUAL(mEntities.size(), 10);


  delete Graph;
  delete Vector;

  Vector = new openfluid::core::GeoVectorValue(
      CONFIGTESTS_INPUT_MISCDATA_DIR + "/landr", "SU_To_Snap.shp");

  Graph =
      openfluid::landr::PolygonGraph::create(*Vector);

  mEntities=Graph->getPolygonEntitiesByMinArea(3500);
  BOOST_CHECK_EQUAL(mEntities.size(), 0);
  mEntities.clear();
  mEntities=Graph->getPolygonEntitiesByMinArea(3500,false);
  BOOST_CHECK_EQUAL(mEntities.size(), 1);

  delete Graph;
  delete Vector;

}


// =====================================================================
// =====================================================================


BOOST_AUTO_TEST_CASE(check_mergePolygonEntities)
{
  openfluid::core::GeoVectorValue* Vector = new openfluid::core::GeoVectorValue(
      CONFIGTESTS_INPUT_MISCDATA_DIR + "/landr", "SU.shp");

  openfluid::landr::PolygonGraph* Graph =
      openfluid::landr::PolygonGraph::create(*Vector);



  BOOST_CHECK_THROW(Graph->mergePolygonEntities(*(Graph->entity(18)),*(Graph->entity(5))),
                    openfluid::base::FrameworkException);

  double areaBefore=Graph->entity(7)->getArea()+Graph->entity(13)->getArea();
  Graph->mergePolygonEntities(*(Graph->entity(7)),*(Graph->entity(13)));
  BOOST_CHECK_EQUAL(Graph->getSize(), 23);
  BOOST_CHECK(!Graph->entity(13));
  BOOST_CHECK_EQUAL(Graph->isComplete(),true);
  BOOST_CHECK_EQUAL(Graph->getEdges()->size(),57);
  double areaAfter=Graph->entity(7)->getArea();
  BOOST_CHECK( openfluid::scientific::isVeryClose(areaBefore, areaAfter));


  delete Graph;
  delete Vector;

}


// =====================================================================
// =====================================================================


BOOST_AUTO_TEST_CASE(check_getPolygonEntityByCompactness)
{
  openfluid::core::GeoVectorValue* Vector = new openfluid::core::GeoVectorValue(
      CONFIGTESTS_INPUT_MISCDATA_DIR + "/landr", "SU.shp");

  openfluid::landr::PolygonGraph* Graph =
      openfluid::landr::PolygonGraph::create(*Vector);


  std::multimap<double,  openfluid::landr::PolygonEntity*> mEntities;

  BOOST_CHECK_THROW(Graph->getPolygonEntitiesByCompactness(-1),
                    openfluid::base::FrameworkException);
  BOOST_CHECK_THROW(Graph->getPolygonEntitiesByCompactness(0),
                    openfluid::base::FrameworkException);
  mEntities=Graph->getPolygonEntitiesByCompactness(1.70);
  BOOST_CHECK_EQUAL(mEntities.size(), 0);

  mEntities.clear();
  mEntities=Graph->getPolygonEntitiesByCompactness(1.70,false);
  BOOST_CHECK_EQUAL(mEntities.size(), 0);

  mEntities.clear();
  mEntities=Graph->getPolygonEntitiesByCompactness(1.50);
  BOOST_CHECK_EQUAL(mEntities.size(), 3);

  mEntities.clear();
  mEntities=Graph->getPolygonEntitiesByCompactness(1.50,false);
  BOOST_CHECK_EQUAL(mEntities.size(), 3);

  delete Graph;
  delete Vector;

  Vector = new openfluid::core::GeoVectorValue(
      CONFIGTESTS_INPUT_MISCDATA_DIR + "/landr", "SU_To_Snap.shp");

  Graph =
      openfluid::landr::PolygonGraph::create(*Vector);

  mEntities=Graph->getPolygonEntitiesByCompactness(1.3);
  BOOST_CHECK_EQUAL(mEntities.size(), 0);
  mEntities.clear();
  mEntities=Graph->getPolygonEntitiesByCompactness(1.3,false);
  BOOST_CHECK_EQUAL(mEntities.size(), 2);

  delete Graph;
  delete Vector;


}


// =====================================================================
// =====================================================================


BOOST_AUTO_TEST_CASE(check_construction_from_Bad_Geometry)
{
  openfluid::core::GeoVectorValue* Val = new openfluid::core::GeoVectorValue(
      CONFIGTESTS_INPUT_MISCDATA_DIR + "/landr", "BAD_POLYTEST.shp");

  BOOST_CHECK_THROW(
      openfluid::landr::PolygonGraph* Graph = openfluid::landr::PolygonGraph::create(*Val),
      openfluid::base::FrameworkException);


  delete Val;

  openfluid::core::GeoVectorValue Val2(CONFIGTESTS_INPUT_MISCDATA_DIR + "/landr",
                                       "BAD_POLYTEST.shp");

  openfluid::landr::VectorDataset* Vect =
      new openfluid::landr::VectorDataset(Val2);

  BOOST_CHECK_THROW(openfluid::landr::PolygonGraph* Graph = openfluid::landr::PolygonGraph::create(*Vect),
      openfluid::base::FrameworkException);

  delete Vect;
}


// =====================================================================
// =====================================================================


BOOST_AUTO_TEST_CASE(check_computeLineStringNeighboursOfPolygonGraph_Contains_with_Complex_LineString)
{
  openfluid::core::GeoVectorValue* ValRS = new openfluid::core::GeoVectorValue(
      CONFIGTESTS_INPUT_MISCDATA_DIR + "/landr", "RS_complex.shp");

  openfluid::core::GeoVectorValue* ValSU = new openfluid::core::GeoVectorValue(
      CONFIGTESTS_INPUT_MISCDATA_DIR + "/landr", "SU.shp");

  openfluid::landr::LineStringGraph* RSGraph =
      openfluid::landr::LineStringGraph::create(*ValRS);
  openfluid::landr::PolygonGraph* SUGraph =
      openfluid::landr::PolygonGraph::create(*ValSU);

  openfluid::landr::PolygonEntity* SU1 = SUGraph->entity(1);
  openfluid::landr::PolygonEntity* SU2 = SUGraph->entity(2);
  openfluid::landr::PolygonEntity* SU3 = SUGraph->entity(3);
  openfluid::landr::PolygonEntity* SU4 = SUGraph->entity(4);
  openfluid::landr::PolygonEntity* SU5 = SUGraph->entity(5);
  openfluid::landr::PolygonEntity* SU6 = SUGraph->entity(6);
  openfluid::landr::PolygonEntity* SU7 = SUGraph->entity(7);
  openfluid::landr::PolygonEntity* SU8 = SUGraph->entity(8);
  openfluid::landr::PolygonEntity* SU9 = SUGraph->entity(9);
  openfluid::landr::PolygonEntity* SU10 = SUGraph->entity(10);
  openfluid::landr::PolygonEntity* SU11 = SUGraph->entity(11);
  openfluid::landr::PolygonEntity* SU12 = SUGraph->entity(12);
  openfluid::landr::PolygonEntity* SU13 = SUGraph->entity(13);
  openfluid::landr::PolygonEntity* SU14 = SUGraph->entity(14);
  openfluid::landr::PolygonEntity* SU15 = SUGraph->entity(15);
  openfluid::landr::PolygonEntity* SU16 = SUGraph->entity(16);
  openfluid::landr::PolygonEntity* SU17 = SUGraph->entity(17);
  openfluid::landr::PolygonEntity* SU18 = SUGraph->entity(18);
  openfluid::landr::PolygonEntity* SU19 = SUGraph->entity(19);
  openfluid::landr::PolygonEntity* SU20 = SUGraph->entity(20);
  openfluid::landr::PolygonEntity* SU21 = SUGraph->entity(21);
  openfluid::landr::PolygonEntity* SU22 = SUGraph->entity(22);
  openfluid::landr::PolygonEntity* SU23 = SUGraph->entity(23);
  openfluid::landr::PolygonEntity* SU24 = SUGraph->entity(24);



  BOOST_CHECK(!SU1->lineStringNeighbours());
  BOOST_CHECK(!SU4->lineStringNeighbours());
  BOOST_CHECK(!SU17->lineStringNeighbours());

  SUGraph->computeLineStringNeighbours(*RSGraph,
                                       openfluid::landr::LandRTools::CONTAINS,
                                       0);

  BOOST_CHECK_EQUAL(SU1->lineStringNeighbours()->size(), 0);
  BOOST_CHECK_EQUAL(SU2->lineStringNeighbours()->size(), 0);
  BOOST_CHECK_EQUAL(SU3->lineStringNeighbours()->size(), 0);
  BOOST_CHECK_EQUAL(SU4->lineStringNeighbours()->size(), 0);
  BOOST_CHECK_EQUAL(SU5->lineStringNeighbours()->size(), 0);
  BOOST_CHECK_EQUAL(SU6->lineStringNeighbours()->size(), 0);
  BOOST_CHECK_EQUAL(SU7->lineStringNeighbours()->size(), 0);
  BOOST_CHECK_EQUAL(SU8->lineStringNeighbours()->size(), 0);
  BOOST_CHECK_EQUAL(SU9->lineStringNeighbours()->size(), 0);
  BOOST_CHECK_EQUAL(SU10->lineStringNeighbours()->size(), 0);
  BOOST_CHECK_EQUAL(SU11->lineStringNeighbours()->size(), 0);
  BOOST_CHECK_EQUAL(SU12->lineStringNeighbours()->size(), 0);
  BOOST_CHECK_EQUAL(SU13->lineStringNeighbours()->size(), 0);
  BOOST_CHECK_EQUAL(SU14->lineStringNeighbours()->size(), 0);
  BOOST_CHECK_EQUAL(SU15->lineStringNeighbours()->size(), 0);
  BOOST_CHECK_EQUAL(SU16->lineStringNeighbours()->size(), 0);
  BOOST_CHECK_EQUAL(SU17->lineStringNeighbours()->size(), 0);
  BOOST_CHECK_EQUAL(SU18->lineStringNeighbours()->size(), 0);
  BOOST_CHECK_EQUAL(SU19->lineStringNeighbours()->size(), 0);
  BOOST_CHECK_EQUAL(SU20->lineStringNeighbours()->size(), 0);
  BOOST_CHECK_EQUAL(SU21->lineStringNeighbours()->size(), 0);
  BOOST_CHECK_EQUAL(SU22->lineStringNeighbours()->size(), 0);
  BOOST_CHECK_EQUAL(SU23->lineStringNeighbours()->size(), 0);
  BOOST_CHECK_EQUAL(SU24->lineStringNeighbours()->size(), 0);

  SUGraph->computeLineStringNeighbours(*RSGraph,
                                       openfluid::landr::LandRTools::CONTAINS,
                                       5);

  BOOST_CHECK_EQUAL(SU1->lineStringNeighbours()->size(), 0);
  BOOST_CHECK_EQUAL(SU8->lineStringNeighbours()->size(), 0);
  BOOST_CHECK_EQUAL(SU10->lineStringNeighbours()->size(), 0);
  BOOST_CHECK_EQUAL(SU11->lineStringNeighbours()->size(), 0);
  BOOST_CHECK_EQUAL(SU15->lineStringNeighbours()->size(), 0);
  BOOST_CHECK_EQUAL(SU16->lineStringNeighbours()->size(), 0);
  BOOST_CHECK_EQUAL(SU19->lineStringNeighbours()->size(), 0);
  BOOST_CHECK_EQUAL(SU23->lineStringNeighbours()->size(), 0);
  BOOST_CHECK_EQUAL(SU24->lineStringNeighbours()->size(), 0);

  BOOST_CHECK_EQUAL(SU2->lineStringNeighbours()->size(), 1);
  BOOST_CHECK_EQUAL(SU4->lineStringNeighbours()->size(), 1);
  BOOST_CHECK_EQUAL(SU5->lineStringNeighbours()->size(), 1);
  BOOST_CHECK_EQUAL(SU7->lineStringNeighbours()->size(), 1);
  BOOST_CHECK_EQUAL(SU9->lineStringNeighbours()->size(), 1);
  BOOST_CHECK_EQUAL(SU12->lineStringNeighbours()->size(), 1);
  BOOST_CHECK_EQUAL(SU13->lineStringNeighbours()->size(), 1);
  BOOST_CHECK_EQUAL(SU14->lineStringNeighbours()->size(), 1);
  BOOST_CHECK_EQUAL(SU17->lineStringNeighbours()->size(), 1);
  BOOST_CHECK_EQUAL(SU18->lineStringNeighbours()->size(), 1);
  BOOST_CHECK_EQUAL(SU21->lineStringNeighbours()->size(), 1);
  BOOST_CHECK_EQUAL(SU22->lineStringNeighbours()->size(), 1);

  BOOST_CHECK_EQUAL(SU3->lineStringNeighbours()->size(), 2);
  BOOST_CHECK_EQUAL(SU6->lineStringNeighbours()->size(), 2);
  BOOST_CHECK_EQUAL(SU20->lineStringNeighbours()->size(), 2);




  // get the LineString Neighbours of Polygon with just one Line Neighbours
  BOOST_CHECK_EQUAL(SU2->lineStringNeighbours()->begin()->first->getOfldId(),16);
  BOOST_CHECK_EQUAL(SU4->lineStringNeighbours()->begin()->first->getOfldId(),3);
  BOOST_CHECK_EQUAL(SU5->lineStringNeighbours()->begin()->first->getOfldId(),17);
  BOOST_CHECK_EQUAL(SU7->lineStringNeighbours()->begin()->first->getOfldId(),4);
  BOOST_CHECK_EQUAL(SU9->lineStringNeighbours()->begin()->first->getOfldId(),5);
  BOOST_CHECK_EQUAL(SU12->lineStringNeighbours()->begin()->first->getOfldId(),5);
  BOOST_CHECK_EQUAL(SU13->lineStringNeighbours()->begin()->first->getOfldId(),4);
  BOOST_CHECK_EQUAL(SU14->lineStringNeighbours()->begin()->first->getOfldId(),6);
  BOOST_CHECK_EQUAL(SU17->lineStringNeighbours()->begin()->first->getOfldId(),6);
  BOOST_CHECK_EQUAL(SU18->lineStringNeighbours()->begin()->first->getOfldId(),3);
  BOOST_CHECK_EQUAL(SU21->lineStringNeighbours()->begin()->first->getOfldId(),8);
  BOOST_CHECK_EQUAL(SU22->lineStringNeighbours()->begin()->first->getOfldId(),7);

  // get the LineString Neighbours of Polygon with just more than one Line Neighbours

  // Neighbours of SU3
  std::map<openfluid::landr::LineStringEntity*, openfluid::landr::PolygonEdge*> *lineMap=SU3->lineStringNeighbours();
  std::map<openfluid::landr::LineStringEntity*, openfluid::landr::PolygonEdge*>::iterator it=lineMap->begin();
  std::map<openfluid::landr::LineStringEntity*, openfluid::landr::PolygonEdge*>::iterator ite=lineMap->end();
  std::list<int> lNeighbours;
  for (; it != ite; ++it)
    lNeighbours.push_back((*it).first->getOfldId());

  lNeighbours.sort();
  std::list<int>::iterator jt=lNeighbours.begin();

  BOOST_CHECK_EQUAL((*jt),15);
  ++jt;
  BOOST_CHECK_EQUAL((*jt),16);

  // Neighbours of SU6
  lineMap=SU6->lineStringNeighbours();
  it=lineMap->begin();
  ite=lineMap->end();
  lNeighbours.clear();
  for (; it != ite; ++it)
    lNeighbours.push_back((*it).first->getOfldId());

  lNeighbours.sort();
  jt=lNeighbours.begin();

  BOOST_CHECK_EQUAL((*jt),15);
  ++jt;
  BOOST_CHECK_EQUAL((*jt),17);

  // Neighbours of SU20
  lineMap=SU20->lineStringNeighbours();
  it=lineMap->begin();
  ite=lineMap->end();
  lNeighbours.clear();
  for (; it != ite; ++it)
    lNeighbours.push_back((*it).first->getOfldId());

  lNeighbours.sort();
  jt=lNeighbours.begin();

  BOOST_CHECK_EQUAL((*jt),7);
  ++jt;
  BOOST_CHECK_EQUAL((*jt),8);


  delete RSGraph;
  delete SUGraph;
  delete ValRS;
  delete ValSU;

}


// =====================================================================
// =====================================================================


BOOST_AUTO_TEST_CASE(check_computeLineStringNeighboursOfPolygonGraph_Intersects_with_Complex_LineString)
{
  openfluid::core::GeoVectorValue* ValRS = new openfluid::core::GeoVectorValue(
      CONFIGTESTS_INPUT_MISCDATA_DIR + "/landr", "RS_complex.shp");

  openfluid::core::GeoVectorValue* ValSU = new openfluid::core::GeoVectorValue(
      CONFIGTESTS_INPUT_MISCDATA_DIR + "/landr", "SU.shp");

  openfluid::landr::LineStringGraph* RSGraph =
      openfluid::landr::LineStringGraph::create(*ValRS);
  openfluid::landr::PolygonGraph* SUGraph =
      openfluid::landr::PolygonGraph::create(*ValSU);

  openfluid::landr::PolygonEntity* SU1 = SUGraph->entity(1);
  openfluid::landr::PolygonEntity* SU2 = SUGraph->entity(2);
  openfluid::landr::PolygonEntity* SU3 = SUGraph->entity(3);
  openfluid::landr::PolygonEntity* SU4 = SUGraph->entity(4);
  openfluid::landr::PolygonEntity* SU5 = SUGraph->entity(5);
  openfluid::landr::PolygonEntity* SU6 = SUGraph->entity(6);
  openfluid::landr::PolygonEntity* SU7 = SUGraph->entity(7);
  openfluid::landr::PolygonEntity* SU8 = SUGraph->entity(8);
  openfluid::landr::PolygonEntity* SU9 = SUGraph->entity(9);
  openfluid::landr::PolygonEntity* SU10 = SUGraph->entity(10);
  openfluid::landr::PolygonEntity* SU11 = SUGraph->entity(11);
  openfluid::landr::PolygonEntity* SU12 = SUGraph->entity(12);
  openfluid::landr::PolygonEntity* SU13 = SUGraph->entity(13);
  openfluid::landr::PolygonEntity* SU14 = SUGraph->entity(14);
  openfluid::landr::PolygonEntity* SU15 = SUGraph->entity(15);
  openfluid::landr::PolygonEntity* SU16 = SUGraph->entity(16);
  openfluid::landr::PolygonEntity* SU17 = SUGraph->entity(17);
  openfluid::landr::PolygonEntity* SU18 = SUGraph->entity(18);
  openfluid::landr::PolygonEntity* SU19 = SUGraph->entity(19);
  openfluid::landr::PolygonEntity* SU20 = SUGraph->entity(20);
  openfluid::landr::PolygonEntity* SU21 = SUGraph->entity(21);
  openfluid::landr::PolygonEntity* SU22 = SUGraph->entity(22);
  openfluid::landr::PolygonEntity* SU23 = SUGraph->entity(23);
  openfluid::landr::PolygonEntity* SU24 = SUGraph->entity(24);



  BOOST_CHECK(!SU1->lineStringNeighbours());
  BOOST_CHECK(!SU4->lineStringNeighbours());
  BOOST_CHECK(!SU17->lineStringNeighbours());

  SUGraph->computeLineStringNeighbours(*RSGraph,
                                       openfluid::landr::LandRTools::INTERSECTS,
                                       0);

  BOOST_CHECK_EQUAL(SU1->lineStringNeighbours()->size(), 0);
  BOOST_CHECK_EQUAL(SU2->lineStringNeighbours()->size(), 0);
  BOOST_CHECK_EQUAL(SU3->lineStringNeighbours()->size(), 0);
  BOOST_CHECK_EQUAL(SU4->lineStringNeighbours()->size(), 0);
  BOOST_CHECK_EQUAL(SU5->lineStringNeighbours()->size(), 0);
  BOOST_CHECK_EQUAL(SU6->lineStringNeighbours()->size(), 0);
  BOOST_CHECK_EQUAL(SU7->lineStringNeighbours()->size(), 0);
  BOOST_CHECK_EQUAL(SU8->lineStringNeighbours()->size(), 0);
  BOOST_CHECK_EQUAL(SU9->lineStringNeighbours()->size(), 0);
  BOOST_CHECK_EQUAL(SU10->lineStringNeighbours()->size(), 0);
  BOOST_CHECK_EQUAL(SU11->lineStringNeighbours()->size(), 0);
  BOOST_CHECK_EQUAL(SU12->lineStringNeighbours()->size(), 0);
  BOOST_CHECK_EQUAL(SU13->lineStringNeighbours()->size(), 0);
  BOOST_CHECK_EQUAL(SU14->lineStringNeighbours()->size(), 0);
  BOOST_CHECK_EQUAL(SU15->lineStringNeighbours()->size(), 0);
  BOOST_CHECK_EQUAL(SU16->lineStringNeighbours()->size(), 0);
  BOOST_CHECK_EQUAL(SU17->lineStringNeighbours()->size(), 0);
  BOOST_CHECK_EQUAL(SU18->lineStringNeighbours()->size(), 0);
  BOOST_CHECK_EQUAL(SU19->lineStringNeighbours()->size(), 0);
  BOOST_CHECK_EQUAL(SU20->lineStringNeighbours()->size(), 0);
  BOOST_CHECK_EQUAL(SU21->lineStringNeighbours()->size(), 0);
  BOOST_CHECK_EQUAL(SU22->lineStringNeighbours()->size(), 0);
  BOOST_CHECK_EQUAL(SU23->lineStringNeighbours()->size(), 0);
  BOOST_CHECK_EQUAL(SU24->lineStringNeighbours()->size(), 0);

  SUGraph->computeLineStringNeighbours(*RSGraph,
                                       openfluid::landr::LandRTools::INTERSECTS,
                                       5);
  BOOST_CHECK_EQUAL(SU10->lineStringNeighbours()->size(), 0);

  BOOST_CHECK_EQUAL(SU1->lineStringNeighbours()->size(), 1);
  BOOST_CHECK_EQUAL(SU15->lineStringNeighbours()->size(), 1);
  BOOST_CHECK_EQUAL(SU19->lineStringNeighbours()->size(), 1);
  BOOST_CHECK_EQUAL(SU24->lineStringNeighbours()->size(), 1);

  BOOST_CHECK_EQUAL(SU2->lineStringNeighbours()->size(), 5);
  BOOST_CHECK_EQUAL(SU3->lineStringNeighbours()->size(), 2);
  BOOST_CHECK_EQUAL(SU4->lineStringNeighbours()->size(), 7);
  BOOST_CHECK_EQUAL(SU5->lineStringNeighbours()->size(), 4);
  BOOST_CHECK_EQUAL(SU6->lineStringNeighbours()->size(), 4);
  BOOST_CHECK_EQUAL(SU7->lineStringNeighbours()->size(), 6);
  BOOST_CHECK_EQUAL(SU8->lineStringNeighbours()->size(), 2);
  BOOST_CHECK_EQUAL(SU9->lineStringNeighbours()->size(), 5);
  BOOST_CHECK_EQUAL(SU11->lineStringNeighbours()->size(), 3);
  BOOST_CHECK_EQUAL(SU12->lineStringNeighbours()->size(), 6);
  BOOST_CHECK_EQUAL(SU13->lineStringNeighbours()->size(), 7);
  BOOST_CHECK_EQUAL(SU14->lineStringNeighbours()->size(), 8);
  BOOST_CHECK_EQUAL(SU16->lineStringNeighbours()->size(), 5);
  BOOST_CHECK_EQUAL(SU17->lineStringNeighbours()->size(), 7);
  BOOST_CHECK_EQUAL(SU18->lineStringNeighbours()->size(), 7);
  BOOST_CHECK_EQUAL(SU20->lineStringNeighbours()->size(), 6);
  BOOST_CHECK_EQUAL(SU21->lineStringNeighbours()->size(), 2);
  BOOST_CHECK_EQUAL(SU22->lineStringNeighbours()->size(), 5);
  BOOST_CHECK_EQUAL(SU23->lineStringNeighbours()->size(), 4);


  // get the LineString Neighbours of Polygon with just one Line Neighbours
  BOOST_CHECK_EQUAL(SU1->lineStringNeighbours()->begin()->first->getOfldId(),1);
  BOOST_CHECK_EQUAL(SU15->lineStringNeighbours()->begin()->first->getOfldId(),18);
  BOOST_CHECK_EQUAL(SU19->lineStringNeighbours()->begin()->first->getOfldId(),10);
  BOOST_CHECK_EQUAL(SU24->lineStringNeighbours()->begin()->first->getOfldId(),1);

  // get the LineString Neighbours of Polygon with just more than one Line Neighbours

  // Neighbours of SU2
  std::map<openfluid::landr::LineStringEntity*, openfluid::landr::PolygonEdge*> *lineMap=SU2->lineStringNeighbours();
  std::map<openfluid::landr::LineStringEntity*, openfluid::landr::PolygonEdge*>::iterator it=lineMap->begin();
  std::map<openfluid::landr::LineStringEntity*, openfluid::landr::PolygonEdge*>::iterator ite=lineMap->end();
  std::list<int> lNeighbours;
  for (; it != ite; ++it)
    lNeighbours.push_back((*it).first->getOfldId());

  lNeighbours.sort();
  std::list<int>::iterator jt=lNeighbours.begin();

  BOOST_CHECK_EQUAL((*jt),1);
  ++jt;
  BOOST_CHECK_EQUAL((*jt),3);
  ++jt;
  BOOST_CHECK_EQUAL((*jt),7);
  ++jt;
  BOOST_CHECK_EQUAL((*jt),12);
  ++jt;
  BOOST_CHECK_EQUAL((*jt),16);

  // Neighbours of SU3
  lineMap=SU3->lineStringNeighbours();
  it=lineMap->begin();
  ite=lineMap->end();
  lNeighbours.clear();
  for (; it != ite; ++it)
    lNeighbours.push_back((*it).first->getOfldId());

  lNeighbours.sort();
  jt=lNeighbours.begin();

  BOOST_CHECK_EQUAL((*jt),15);
  ++jt;
  BOOST_CHECK_EQUAL((*jt),16);

  // Neighbours of SU4
  lineMap=SU4->lineStringNeighbours();
  it=lineMap->begin();
  ite=lineMap->end();
  lNeighbours.clear();
  for (; it != ite; ++it)
    lNeighbours.push_back((*it).first->getOfldId());

  lNeighbours.sort();
  jt=lNeighbours.begin();

  BOOST_CHECK_EQUAL((*jt),1);
  ++jt;
  BOOST_CHECK_EQUAL((*jt),3);
  ++jt;
  BOOST_CHECK_EQUAL((*jt),4);
  ++jt;
  BOOST_CHECK_EQUAL((*jt),6);
  ++jt;
  BOOST_CHECK_EQUAL((*jt),7);
  ++jt;
  BOOST_CHECK_EQUAL((*jt),12);
  ++jt;
  BOOST_CHECK_EQUAL((*jt),13);

  // Neighbours of SU5
  lineMap=SU5->lineStringNeighbours();
  it=lineMap->begin();
  ite=lineMap->end();
  lNeighbours.clear();
  for (; it != ite; ++it)
    lNeighbours.push_back((*it).first->getOfldId());

  lNeighbours.sort();
  jt=lNeighbours.begin();

  BOOST_CHECK_EQUAL((*jt),13);
  ++jt;
  BOOST_CHECK_EQUAL((*jt),15);
  ++jt;
  BOOST_CHECK_EQUAL((*jt),16);
  ++jt;
  BOOST_CHECK_EQUAL((*jt),17);

  // Neighbours of SU6
  lineMap=SU6->lineStringNeighbours();
  it=lineMap->begin();
  ite=lineMap->end();
  lNeighbours.clear();
  for (; it != ite; ++it)
    lNeighbours.push_back((*it).first->getOfldId());

  lNeighbours.sort();
  jt=lNeighbours.begin();

  BOOST_CHECK_EQUAL((*jt),13);
  ++jt;
  BOOST_CHECK_EQUAL((*jt),14);
  ++jt;
  BOOST_CHECK_EQUAL((*jt),15);
  ++jt;
  BOOST_CHECK_EQUAL((*jt),17);

  // Neighbours of SU7
  lineMap=SU7->lineStringNeighbours();
  it=lineMap->begin();
  ite=lineMap->end();
  lNeighbours.clear();
  for (; it != ite; ++it)
    lNeighbours.push_back((*it).first->getOfldId());

  lNeighbours.sort();
  jt=lNeighbours.begin();

  BOOST_CHECK_EQUAL((*jt),3);
  ++jt;
  BOOST_CHECK_EQUAL((*jt),4);
  ++jt;
  BOOST_CHECK_EQUAL((*jt),5);
  ++jt;
  BOOST_CHECK_EQUAL((*jt),6);
  ++jt;
  BOOST_CHECK_EQUAL((*jt),11);
  ++jt;
  BOOST_CHECK_EQUAL((*jt),13);

  // Neighbours of SU8
  lineMap=SU8->lineStringNeighbours();
  it=lineMap->begin();
  ite=lineMap->end();
  lNeighbours.clear();
  for (; it != ite; ++it)
    lNeighbours.push_back((*it).first->getOfldId());

  lNeighbours.sort();
  jt=lNeighbours.begin();

  BOOST_CHECK_EQUAL((*jt),13);
  ++jt;
  BOOST_CHECK_EQUAL((*jt),14);

  // Neighbours of SU9
  lineMap=SU9->lineStringNeighbours();
  it=lineMap->begin();
  ite=lineMap->end();
  lNeighbours.clear();
  for (; it != ite; ++it)
    lNeighbours.push_back((*it).first->getOfldId());

  lNeighbours.sort();
  jt=lNeighbours.begin();

  BOOST_CHECK_EQUAL((*jt),4);
  ++jt;
  BOOST_CHECK_EQUAL((*jt),5);
  ++jt;
  BOOST_CHECK_EQUAL((*jt),11);
  ++jt;
  BOOST_CHECK_EQUAL((*jt),19);
  ++jt;
  BOOST_CHECK_EQUAL((*jt),20);

  // Neighbours of SU11
  lineMap=SU11->lineStringNeighbours();
  it=lineMap->begin();
  ite=lineMap->end();
  lNeighbours.clear();
  for (; it != ite; ++it)
    lNeighbours.push_back((*it).first->getOfldId());

  lNeighbours.sort();
  jt=lNeighbours.begin();

  BOOST_CHECK_EQUAL((*jt),5);
  ++jt;
  BOOST_CHECK_EQUAL((*jt),19);
  ++jt;
  BOOST_CHECK_EQUAL((*jt),20);

  // Neighbours of SU12
  lineMap=SU12->lineStringNeighbours();
  it=lineMap->begin();
  ite=lineMap->end();
  lNeighbours.clear();
  for (; it != ite; ++it)
    lNeighbours.push_back((*it).first->getOfldId());

  lNeighbours.sort();
  jt=lNeighbours.begin();

  BOOST_CHECK_EQUAL((*jt),4);
  ++jt;
  BOOST_CHECK_EQUAL((*jt),5);
  ++jt;
  BOOST_CHECK_EQUAL((*jt),11);
  ++jt;
  BOOST_CHECK_EQUAL((*jt),18);
  ++jt;
  BOOST_CHECK_EQUAL((*jt),19);

  // Neighbours of SU13
  lineMap=SU13->lineStringNeighbours();
  it=lineMap->begin();
  ite=lineMap->end();
  lNeighbours.clear();
  for (; it != ite; ++it)
    lNeighbours.push_back((*it).first->getOfldId());

  lNeighbours.sort();
  jt=lNeighbours.begin();

  BOOST_CHECK_EQUAL((*jt),3);
  ++jt;
  BOOST_CHECK_EQUAL((*jt),4);
  ++jt;
  BOOST_CHECK_EQUAL((*jt),5);
  ++jt;
  BOOST_CHECK_EQUAL((*jt),6);
  ++jt;
  BOOST_CHECK_EQUAL((*jt),11);
  ++jt;
  BOOST_CHECK_EQUAL((*jt),13);
  ++jt;
  BOOST_CHECK_EQUAL((*jt),18);

  // Neighbours of SU14
  lineMap=SU14->lineStringNeighbours();
  it=lineMap->begin();
  ite=lineMap->end();
  lNeighbours.clear();
  for (; it != ite; ++it)
    lNeighbours.push_back((*it).first->getOfldId());

  lNeighbours.sort();
  jt=lNeighbours.begin();

  BOOST_CHECK_EQUAL((*jt),2);
  ++jt;
  BOOST_CHECK_EQUAL((*jt),3);
  ++jt;
  BOOST_CHECK_EQUAL((*jt),4);
  ++jt;
  BOOST_CHECK_EQUAL((*jt),6);
  ++jt;
  BOOST_CHECK_EQUAL((*jt),9);
  ++jt;
  BOOST_CHECK_EQUAL((*jt),10);
  ++jt;
  BOOST_CHECK_EQUAL((*jt),13);
  ++jt;
  BOOST_CHECK_EQUAL((*jt),18);

  // Neighbours of SU16
  lineMap=SU16->lineStringNeighbours();
  it=lineMap->begin();
  ite=lineMap->end();
  lNeighbours.clear();
  for (; it != ite; ++it)
    lNeighbours.push_back((*it).first->getOfldId());

  lNeighbours.sort();
  jt=lNeighbours.begin();

  BOOST_CHECK_EQUAL((*jt),2);
  ++jt;
  BOOST_CHECK_EQUAL((*jt),6);
  ++jt;
  BOOST_CHECK_EQUAL((*jt),9);
  ++jt;
  BOOST_CHECK_EQUAL((*jt),10);
  ++jt;
  BOOST_CHECK_EQUAL((*jt),18);

  // Neighbours of SU17
  lineMap=SU17->lineStringNeighbours();
  it=lineMap->begin();
  ite=lineMap->end();
  lNeighbours.clear();
  for (; it != ite; ++it)
    lNeighbours.push_back((*it).first->getOfldId());

  lNeighbours.sort();
  jt=lNeighbours.begin();

  BOOST_CHECK_EQUAL((*jt),2);
  ++jt;
  BOOST_CHECK_EQUAL((*jt),3);
  ++jt;
  BOOST_CHECK_EQUAL((*jt),4);
  ++jt;
  BOOST_CHECK_EQUAL((*jt),6);
  ++jt;
  BOOST_CHECK_EQUAL((*jt),9);
  ++jt;
  BOOST_CHECK_EQUAL((*jt),10);
  ++jt;
  BOOST_CHECK_EQUAL((*jt),13);

  // Neighbours of SU18
  lineMap=SU18->lineStringNeighbours();
  it=lineMap->begin();
  ite=lineMap->end();
  lNeighbours.clear();
  for (; it != ite; ++it)
    lNeighbours.push_back((*it).first->getOfldId());

  lNeighbours.sort();
  jt=lNeighbours.begin();

  BOOST_CHECK_EQUAL((*jt),1);
  ++jt;
  BOOST_CHECK_EQUAL((*jt),3);
  ++jt;
  BOOST_CHECK_EQUAL((*jt),4);
  ++jt;
  BOOST_CHECK_EQUAL((*jt),6);
  ++jt;
  BOOST_CHECK_EQUAL((*jt),7);
  ++jt;
  BOOST_CHECK_EQUAL((*jt),12);
  ++jt;
  BOOST_CHECK_EQUAL((*jt),13);

  // Neighbours of SU20
  lineMap=SU20->lineStringNeighbours();
  it=lineMap->begin();
  ite=lineMap->end();
  lNeighbours.clear();
  for (; it != ite; ++it)
    lNeighbours.push_back((*it).first->getOfldId());

  lNeighbours.sort();
  jt=lNeighbours.begin();

  BOOST_CHECK_EQUAL((*jt),1);
  ++jt;
  BOOST_CHECK_EQUAL((*jt),3);
  ++jt;
  BOOST_CHECK_EQUAL((*jt),7);
  ++jt;
  BOOST_CHECK_EQUAL((*jt),8);
  ++jt;
  BOOST_CHECK_EQUAL((*jt),10);
  ++jt;
  BOOST_CHECK_EQUAL((*jt),12);

  // Neighbours of SU21
  lineMap=SU21->lineStringNeighbours();
  it=lineMap->begin();
  ite=lineMap->end();
  lNeighbours.clear();
  for (; it != ite; ++it)
    lNeighbours.push_back((*it).first->getOfldId());

  lNeighbours.sort();
  jt=lNeighbours.begin();

  BOOST_CHECK_EQUAL((*jt),7);
  ++jt;
  BOOST_CHECK_EQUAL((*jt),8);

  // Neighbours of SU22
  lineMap=SU22->lineStringNeighbours();
  it=lineMap->begin();
  ite=lineMap->end();
  lNeighbours.clear();
  for (; it != ite; ++it)
    lNeighbours.push_back((*it).first->getOfldId());

  lNeighbours.sort();
  jt=lNeighbours.begin();

  BOOST_CHECK_EQUAL((*jt),1);
  ++jt;
  BOOST_CHECK_EQUAL((*jt),3);
  ++jt;
  BOOST_CHECK_EQUAL((*jt),7);
  ++jt;
  BOOST_CHECK_EQUAL((*jt),8);
  ++jt;
  BOOST_CHECK_EQUAL((*jt),12);

  // Neighbours of SU23
  lineMap=SU23->lineStringNeighbours();
  it=lineMap->begin();
  ite=lineMap->end();
  lNeighbours.clear();
  for (; it != ite; ++it)
    lNeighbours.push_back((*it).first->getOfldId());

  lNeighbours.sort();
  jt=lNeighbours.begin();

  BOOST_CHECK_EQUAL((*jt),1);
  ++jt;
  BOOST_CHECK_EQUAL((*jt),3);
  ++jt;
  BOOST_CHECK_EQUAL((*jt),7);
  ++jt;
  BOOST_CHECK_EQUAL((*jt),12);


  delete RSGraph;
  delete SUGraph;
  delete ValRS;
  delete ValSU;

}


// =====================================================================
// =====================================================================


BOOST_AUTO_TEST_CASE(check_computeLineStringNeighboursOfPolygonGraph_Touches_with_Complex_LineString)
{
  openfluid::core::GeoVectorValue* ValRS = new openfluid::core::GeoVectorValue(
      CONFIGTESTS_INPUT_MISCDATA_DIR + "/landr", "RS_complex.shp");

  openfluid::core::GeoVectorValue* ValSU = new openfluid::core::GeoVectorValue(
      CONFIGTESTS_INPUT_MISCDATA_DIR + "/landr", "SU.shp");

  openfluid::landr::LineStringGraph* RSGraph =
      openfluid::landr::LineStringGraph::create(*ValRS);
  openfluid::landr::PolygonGraph* SUGraph =
      openfluid::landr::PolygonGraph::create(*ValSU);

  openfluid::landr::PolygonEntity* SU1 = SUGraph->entity(1);
  openfluid::landr::PolygonEntity* SU2 = SUGraph->entity(2);
  openfluid::landr::PolygonEntity* SU3 = SUGraph->entity(3);
  openfluid::landr::PolygonEntity* SU4 = SUGraph->entity(4);
  openfluid::landr::PolygonEntity* SU5 = SUGraph->entity(5);
  openfluid::landr::PolygonEntity* SU6 = SUGraph->entity(6);
  openfluid::landr::PolygonEntity* SU7 = SUGraph->entity(7);
  openfluid::landr::PolygonEntity* SU8 = SUGraph->entity(8);
  openfluid::landr::PolygonEntity* SU9 = SUGraph->entity(9);
  openfluid::landr::PolygonEntity* SU10 = SUGraph->entity(10);
  openfluid::landr::PolygonEntity* SU11 = SUGraph->entity(11);
  openfluid::landr::PolygonEntity* SU12 = SUGraph->entity(12);
  openfluid::landr::PolygonEntity* SU13 = SUGraph->entity(13);
  openfluid::landr::PolygonEntity* SU14 = SUGraph->entity(14);
  openfluid::landr::PolygonEntity* SU15 = SUGraph->entity(15);
  openfluid::landr::PolygonEntity* SU16 = SUGraph->entity(16);
  openfluid::landr::PolygonEntity* SU17 = SUGraph->entity(17);
  openfluid::landr::PolygonEntity* SU18 = SUGraph->entity(18);
  openfluid::landr::PolygonEntity* SU19 = SUGraph->entity(19);
  openfluid::landr::PolygonEntity* SU20 = SUGraph->entity(20);
  openfluid::landr::PolygonEntity* SU21 = SUGraph->entity(21);
  openfluid::landr::PolygonEntity* SU22 = SUGraph->entity(22);
  openfluid::landr::PolygonEntity* SU23 = SUGraph->entity(23);
  openfluid::landr::PolygonEntity* SU24 = SUGraph->entity(24);



  BOOST_CHECK(!SU1->lineStringNeighbours());
  BOOST_CHECK(!SU4->lineStringNeighbours());
  BOOST_CHECK(!SU17->lineStringNeighbours());

  BOOST_CHECK_THROW(SUGraph->computeLineStringNeighbours(*RSGraph,
                                                         openfluid::landr::LandRTools::TOUCHES,
                                                         0),openfluid::base::FrameworkException);

  SUGraph->computeLineStringNeighbours(*RSGraph,openfluid::landr::LandRTools::TOUCHES,0,10);

  BOOST_CHECK_EQUAL(SU1->lineStringNeighbours()->size(), 0);
  BOOST_CHECK_EQUAL(SU2->lineStringNeighbours()->size(), 0);
  BOOST_CHECK_EQUAL(SU3->lineStringNeighbours()->size(), 0);
  BOOST_CHECK_EQUAL(SU4->lineStringNeighbours()->size(), 0);
  BOOST_CHECK_EQUAL(SU5->lineStringNeighbours()->size(), 0);
  BOOST_CHECK_EQUAL(SU6->lineStringNeighbours()->size(), 0);
  BOOST_CHECK_EQUAL(SU7->lineStringNeighbours()->size(), 0);
  BOOST_CHECK_EQUAL(SU8->lineStringNeighbours()->size(), 0);
  BOOST_CHECK_EQUAL(SU9->lineStringNeighbours()->size(), 0);
  BOOST_CHECK_EQUAL(SU10->lineStringNeighbours()->size(), 0);
  BOOST_CHECK_EQUAL(SU11->lineStringNeighbours()->size(), 0);
  BOOST_CHECK_EQUAL(SU12->lineStringNeighbours()->size(), 0);
  BOOST_CHECK_EQUAL(SU13->lineStringNeighbours()->size(), 0);
  BOOST_CHECK_EQUAL(SU14->lineStringNeighbours()->size(), 0);
  BOOST_CHECK_EQUAL(SU15->lineStringNeighbours()->size(), 0);
  BOOST_CHECK_EQUAL(SU16->lineStringNeighbours()->size(), 0);
  BOOST_CHECK_EQUAL(SU17->lineStringNeighbours()->size(), 0);
  BOOST_CHECK_EQUAL(SU18->lineStringNeighbours()->size(), 0);
  BOOST_CHECK_EQUAL(SU19->lineStringNeighbours()->size(), 0);
  BOOST_CHECK_EQUAL(SU20->lineStringNeighbours()->size(), 0);
  BOOST_CHECK_EQUAL(SU21->lineStringNeighbours()->size(), 0);
  BOOST_CHECK_EQUAL(SU22->lineStringNeighbours()->size(), 0);
  BOOST_CHECK_EQUAL(SU23->lineStringNeighbours()->size(), 0);
  BOOST_CHECK_EQUAL(SU24->lineStringNeighbours()->size(), 0);

  SUGraph->computeLineStringNeighbours(*RSGraph,
                                       openfluid::landr::LandRTools::TOUCHES,
                                       5,50);

  BOOST_CHECK_EQUAL(SU10->lineStringNeighbours()->size(), 0);
  BOOST_CHECK_EQUAL(SU19->lineStringNeighbours()->size(), 0);
  BOOST_CHECK_EQUAL(SU21->lineStringNeighbours()->size(), 0);

  BOOST_CHECK_EQUAL(SU1->lineStringNeighbours()->size(), 1);
  BOOST_CHECK_EQUAL(SU8->lineStringNeighbours()->size(), 1);
  BOOST_CHECK_EQUAL(SU9->lineStringNeighbours()->size(), 1);
  BOOST_CHECK_EQUAL(SU11->lineStringNeighbours()->size(), 1);
  BOOST_CHECK_EQUAL(SU15->lineStringNeighbours()->size(), 1);
  BOOST_CHECK_EQUAL(SU18->lineStringNeighbours()->size(), 1);
  BOOST_CHECK_EQUAL(SU20->lineStringNeighbours()->size(), 1);
  BOOST_CHECK_EQUAL(SU22->lineStringNeighbours()->size(), 1);
  BOOST_CHECK_EQUAL(SU23->lineStringNeighbours()->size(), 1);
  BOOST_CHECK_EQUAL(SU24->lineStringNeighbours()->size(), 1);


  BOOST_CHECK_EQUAL(SU2->lineStringNeighbours()->size(), 2);
  BOOST_CHECK_EQUAL(SU3->lineStringNeighbours()->size(), 2);
  BOOST_CHECK_EQUAL(SU4->lineStringNeighbours()->size(), 2);
  BOOST_CHECK_EQUAL(SU5->lineStringNeighbours()->size(), 2);
  BOOST_CHECK_EQUAL(SU7->lineStringNeighbours()->size(), 2);
  BOOST_CHECK_EQUAL(SU12->lineStringNeighbours()->size(), 2);
  BOOST_CHECK_EQUAL(SU13->lineStringNeighbours()->size(), 2);
  BOOST_CHECK_EQUAL(SU14->lineStringNeighbours()->size(), 2);
  BOOST_CHECK_EQUAL(SU16->lineStringNeighbours()->size(), 2);
  BOOST_CHECK_EQUAL(SU17->lineStringNeighbours()->size(), 2);

  BOOST_CHECK_EQUAL(SU6->lineStringNeighbours()->size(), 3);


  // get the LineString Neighbours of Polygon with just one Line Neighbours
  BOOST_CHECK_EQUAL(SU1->lineStringNeighbours()->begin()->first->getOfldId(),1);
  BOOST_CHECK_EQUAL(SU8->lineStringNeighbours()->begin()->first->getOfldId(),13);
  BOOST_CHECK_EQUAL(SU9->lineStringNeighbours()->begin()->first->getOfldId(),5);
  BOOST_CHECK_EQUAL(SU11->lineStringNeighbours()->begin()->first->getOfldId(),19);
  BOOST_CHECK_EQUAL(SU15->lineStringNeighbours()->begin()->first->getOfldId(),18);
  BOOST_CHECK_EQUAL(SU18->lineStringNeighbours()->begin()->first->getOfldId(),3);
  BOOST_CHECK_EQUAL(SU20->lineStringNeighbours()->begin()->first->getOfldId(),7);
  BOOST_CHECK_EQUAL(SU22->lineStringNeighbours()->begin()->first->getOfldId(),7);
  BOOST_CHECK_EQUAL(SU23->lineStringNeighbours()->begin()->first->getOfldId(),1);
  BOOST_CHECK_EQUAL(SU24->lineStringNeighbours()->begin()->first->getOfldId(),1);

  // get the LineString Neighbours of Polygon with just more than one Line Neighbours

  // Neighbours of SU2
  std::map<openfluid::landr::LineStringEntity*, openfluid::landr::PolygonEdge*> *lineMap=SU2->lineStringNeighbours();
  std::map<openfluid::landr::LineStringEntity*, openfluid::landr::PolygonEdge*>::iterator it=lineMap->begin();
  std::map<openfluid::landr::LineStringEntity*, openfluid::landr::PolygonEdge*>::iterator ite=lineMap->end();
  std::list<int> lNeighbours;
  for (; it != ite; ++it)
    lNeighbours.push_back((*it).first->getOfldId());

  lNeighbours.sort();
  std::list<int>::iterator jt=lNeighbours.begin();

  BOOST_CHECK_EQUAL((*jt),1);
  ++jt;
  BOOST_CHECK_EQUAL((*jt),16);

  // Neighbours of SU3
  lineMap=SU3->lineStringNeighbours();
  it=lineMap->begin();
  ite=lineMap->end();
  lNeighbours.clear();
  for (; it != ite; ++it)
    lNeighbours.push_back((*it).first->getOfldId());

  lNeighbours.sort();
  jt=lNeighbours.begin();

  BOOST_CHECK_EQUAL((*jt),15);
  ++jt;
  BOOST_CHECK_EQUAL((*jt),16);

  // Neighbours of SU4
  lineMap=SU4->lineStringNeighbours();
  it=lineMap->begin();
  ite=lineMap->end();
  lNeighbours.clear();
  for (; it != ite; ++it)
    lNeighbours.push_back((*it).first->getOfldId());

  lNeighbours.sort();
  jt=lNeighbours.begin();

  BOOST_CHECK_EQUAL((*jt),3);
  ++jt;
  BOOST_CHECK_EQUAL((*jt),13);

  // Neighbours of SU5
  lineMap=SU5->lineStringNeighbours();
  it=lineMap->begin();
  ite=lineMap->end();
  lNeighbours.clear();
  for (; it != ite; ++it)
    lNeighbours.push_back((*it).first->getOfldId());

  lNeighbours.sort();
  jt=lNeighbours.begin();

  BOOST_CHECK_EQUAL((*jt),13);
  ++jt;
  BOOST_CHECK_EQUAL((*jt),17);

  // Neighbours of SU7
  lineMap=SU7->lineStringNeighbours();
  it=lineMap->begin();
  ite=lineMap->end();
  lNeighbours.clear();
  for (; it != ite; ++it)
    lNeighbours.push_back((*it).first->getOfldId());

  lNeighbours.sort();
  jt=lNeighbours.begin();

  BOOST_CHECK_EQUAL((*jt),4);
  ++jt;
  BOOST_CHECK_EQUAL((*jt),13);


  // Neighbours of SU12
  lineMap=SU12->lineStringNeighbours();
  it=lineMap->begin();
  ite=lineMap->end();
  lNeighbours.clear();
  for (; it != ite; ++it)
    lNeighbours.push_back((*it).first->getOfldId());

  lNeighbours.sort();
  jt=lNeighbours.begin();

  BOOST_CHECK_EQUAL((*jt),5);
  ++jt;
  BOOST_CHECK_EQUAL((*jt),19);

  // Neighbours of SU13
  lineMap=SU13->lineStringNeighbours();
  it=lineMap->begin();
  ite=lineMap->end();
  lNeighbours.clear();
  for (; it != ite; ++it)
    lNeighbours.push_back((*it).first->getOfldId());

  lNeighbours.sort();
  jt=lNeighbours.begin();

  BOOST_CHECK_EQUAL((*jt),4);
  ++jt;
  BOOST_CHECK_EQUAL((*jt),18);

  // Neighbours of SU14
  lineMap=SU14->lineStringNeighbours();
  it=lineMap->begin();
  ite=lineMap->end();
  lNeighbours.clear();
  for (; it != ite; ++it)
    lNeighbours.push_back((*it).first->getOfldId());

  lNeighbours.sort();
  jt=lNeighbours.begin();

  BOOST_CHECK_EQUAL((*jt),6);
  ++jt;
  BOOST_CHECK_EQUAL((*jt),18);

  // Neighbours of SU16
  lineMap=SU16->lineStringNeighbours();
  it=lineMap->begin();
  ite=lineMap->end();
  lNeighbours.clear();
  for (; it != ite; ++it)
    lNeighbours.push_back((*it).first->getOfldId());

  lNeighbours.sort();
  jt=lNeighbours.begin();

  BOOST_CHECK_EQUAL((*jt),2);
  ++jt;
  BOOST_CHECK_EQUAL((*jt),18);

  // Neighbours of SU17
  lineMap=SU17->lineStringNeighbours();
  it=lineMap->begin();
  ite=lineMap->end();
  lNeighbours.clear();
  for (; it != ite; ++it)
    lNeighbours.push_back((*it).first->getOfldId());

  lNeighbours.sort();
  jt=lNeighbours.begin();

  BOOST_CHECK_EQUAL((*jt),2);
  ++jt;
  BOOST_CHECK_EQUAL((*jt),6);


  // Neighbours of SU6
  lineMap=SU6->lineStringNeighbours();
  it=lineMap->begin();
  ite=lineMap->end();
  lNeighbours.clear();
  for (; it != ite; ++it)
    lNeighbours.push_back((*it).first->getOfldId());

  lNeighbours.sort();
  jt=lNeighbours.begin();

  BOOST_CHECK_EQUAL((*jt),13);
  ++jt;
  BOOST_CHECK_EQUAL((*jt),15);
  ++jt;
  BOOST_CHECK_EQUAL((*jt),17);



  delete RSGraph;
  delete SUGraph;
  delete ValRS;
  delete ValSU;

}


// =====================================================================
// =====================================================================


BOOST_AUTO_TEST_CASE(check_construction_from_MultiPolygon)
{
  openfluid::core::GeoVectorValue* Val = new openfluid::core::GeoVectorValue(
      CONFIGTESTS_INPUT_MISCDATA_DIR + "/landr", "MultiPolygon.shp");

  BOOST_CHECK_THROW(
      openfluid::landr::PolygonGraph* Graph = openfluid::landr::PolygonGraph::create(*Val),
      openfluid::base::FrameworkException);

  delete Val;
}


// =====================================================================
// =====================================================================


BOOST_AUTO_TEST_CASE(check_setAttributeFromMeanRasterValues_With_NoData)
{
  openfluid::core::GeoVectorValue* Vector = new openfluid::core::GeoVectorValue(
      CONFIGTESTS_INPUT_MISCDATA_DIR + "/landr", "SU_dem_Nodata.shp");

  openfluid::core::GeoRasterValue* Raster = new openfluid::core::GeoRasterValue(
      CONFIGTESTS_INPUT_MISCDATA_DIR + "/GeoRasterValue", "dem2.Gtiff");

  openfluid::landr::PolygonGraph* Graph =
      openfluid::landr::PolygonGraph::create(*Vector);

  Graph->addAGeoRasterValue(*Raster);

  Graph->setAttributeFromMeanRasterValues("test_val");

  openfluid::core::DoubleValue Val;

  Graph->entity(1)->getAttributeValue("test_val", Val);
  BOOST_CHECK( openfluid::scientific::isVeryClose(Val.get(), 76.7213));

  delete Graph;
  delete Vector;
  delete Raster;
}


// =====================================================================
// =====================================================================


BOOST_AUTO_TEST_CASE(check_computeNeighboursWithBarriers)
{

  openfluid::core::GeoVectorValue* ValSU = new openfluid::core::GeoVectorValue(
      CONFIGTESTS_INPUT_MISCDATA_DIR + "/landr", "POLY_TEST.shp");

  openfluid::landr::PolygonGraph* SUGraph =
      openfluid::landr::PolygonGraph::create(*ValSU);

  openfluid::core::GeoVectorValue* ValBarriers = new openfluid::core::GeoVectorValue(
      CONFIGTESTS_INPUT_MISCDATA_DIR + "/landr/", "Barriers.shp");

  openfluid::landr::LineStringGraph* BarriersGraph =
      openfluid::landr::LineStringGraph::create(*ValBarriers);

  SUGraph->computeNeighbours();


  BOOST_CHECK_EQUAL(SUGraph->entity(1)->getOrderedNeighbourOfldIds().size(),2);
  BOOST_CHECK_EQUAL(SUGraph->entity(2)->getOrderedNeighbourOfldIds().size(),2);
  BOOST_CHECK_EQUAL(SUGraph->entity(3)->getOrderedNeighbourOfldIds().size(),2);
  BOOST_CHECK_EQUAL(SUGraph->entity(4)->getOrderedNeighbourOfldIds().size(),2);



  BOOST_CHECK_THROW(SUGraph->computeNeighboursWithBarriers(*BarriersGraph,
                                     openfluid::landr::LandRTools::INTERSECTS,0.01),
                    openfluid::base::FrameworkException);

  SUGraph->computeNeighboursWithBarriers(*BarriersGraph,
                                       openfluid::landr::LandRTools::CONTAINS,0.01);

  BOOST_CHECK_EQUAL(SUGraph->entity(1)->getOrderedNeighbourOfldIds().size(),1);
  BOOST_CHECK_EQUAL(SUGraph->entity(1)->getOrderedNeighbourOfldIds().at(0),2);
  BOOST_CHECK_EQUAL(SUGraph->entity(2)->getOrderedNeighbourOfldIds().size(),2);
  BOOST_CHECK_EQUAL(SUGraph->entity(4)->getOrderedNeighbourOfldIds().size(),1);



    BOOST_CHECK_EQUAL(SUGraph->entity(2)->getOrderedNeighbourOfldIds().size(),2);



  BOOST_CHECK_EQUAL(SUGraph->entity(3)->getOrderedNeighbourOfldIds().size(),0);

  SUGraph->computeNeighboursWithBarriers(*BarriersGraph,
                                     openfluid::landr::LandRTools::TOUCHES,0.01,0.2);

  BOOST_CHECK_EQUAL(SUGraph->entity(2)->getOrderedNeighbourOfldIds().size(),1);
  BOOST_CHECK_EQUAL(SUGraph->entity(2)->getOrderedNeighbourOfldIds().at(0),1);
  BOOST_CHECK_EQUAL(SUGraph->entity(4)->getOrderedNeighbourOfldIds().size(),0);

  SUGraph->computeNeighboursWithBarriers(*BarriersGraph,
		  openfluid::landr::LandRTools::TOUCHES,0.1,0.2);

  BOOST_CHECK_EQUAL(SUGraph->entity(1)->getOrderedNeighbourOfldIds().size(),0);
  BOOST_CHECK_EQUAL(SUGraph->entity(2)->getOrderedNeighbourOfldIds().size(),0);

  openfluid::core::GeoVectorValue* ValRS = new openfluid::core::GeoVectorValue(
		  CONFIGTESTS_INPUT_MISCDATA_DIR + "/landr/", "LINE_TEST4.shp");

  openfluid::landr::LineStringGraph* RSGraph =
		  openfluid::landr::LineStringGraph::create(*ValRS);

  SUGraph->computeLineStringNeighbours(*RSGraph,
		  openfluid::landr::LandRTools::INTERSECTS,0.01);

  BOOST_CHECK_EQUAL(SUGraph->entity(1)->getOrderedNeighbourOfldIds().size(),0);
  BOOST_CHECK_EQUAL(SUGraph->entity(1)->lineStringNeighbours()->size(),4);
  BOOST_CHECK_EQUAL(SUGraph->entity(2)->getOrderedNeighbourOfldIds().size(),0);
  BOOST_CHECK_EQUAL(SUGraph->entity(2)->lineStringNeighbours()->size(),5);

  delete ValSU;
  delete SUGraph;
  delete ValBarriers;
  delete BarriersGraph;
  delete ValRS;
  delete RSGraph;

}


// =====================================================================
// =====================================================================


BOOST_AUTO_TEST_CASE(check_mergePolygonEntitiesByMinArea)
{
  openfluid::core::GeoVectorValue* ValPoly = new openfluid::core::GeoVectorValue(
      CONFIGTESTS_INPUT_MISCDATA_DIR + "/landr", "SU.shp");

  openfluid::landr::PolygonGraph* PolyGraph =
      openfluid::landr::PolygonGraph::create(*ValPoly);


  BOOST_CHECK_THROW(PolyGraph->mergePolygonEntitiesByMinArea(-1),openfluid::base::FrameworkException);
  BOOST_CHECK_THROW(PolyGraph->mergePolygonEntitiesByMinArea(0),openfluid::base::FrameworkException);
  BOOST_CHECK_THROW(PolyGraph->mergePolygonEntitiesByMinArea(-0.000000001),openfluid::base::FrameworkException);
  PolyGraph->mergePolygonEntitiesByMinArea(12);
  BOOST_CHECK_EQUAL(PolyGraph->getEntities().size(), 24);


  double areaBefore=PolyGraph->entity(9)->getArea()+PolyGraph->entity(7)->getArea();
  PolyGraph->mergePolygonEntitiesByMinArea(12000);
  BOOST_CHECK_EQUAL(PolyGraph->getEntities().size(), 23);
  BOOST_CHECK(!PolyGraph->entity(9));
  double areaAfter=PolyGraph->entity(7)->getArea();
  BOOST_CHECK( openfluid::scientific::isVeryClose(areaBefore, areaAfter));

  PolyGraph = openfluid::landr::PolygonGraph::create(*ValPoly);

  areaBefore=PolyGraph->entity(13)->getArea()+PolyGraph->entity(14)->getArea();
  double areaBefore1=PolyGraph->entity(9)->getArea()+PolyGraph->entity(7)->getArea();
  PolyGraph->mergePolygonEntitiesByMinArea(13000);
  BOOST_CHECK_EQUAL(PolyGraph->getEntities().size(), 22);
  BOOST_CHECK(!PolyGraph->entity(9));
  BOOST_CHECK(!PolyGraph->entity(13));
  BOOST_CHECK(PolyGraph->entity(7));
  BOOST_CHECK(PolyGraph->entity(14));
  areaAfter=PolyGraph->entity(14)->getArea();
  double areaAfter1=PolyGraph->entity(7)->getArea();
  BOOST_CHECK( openfluid::scientific::isVeryClose(areaBefore, areaAfter));
  BOOST_CHECK( openfluid::scientific::isVeryClose(areaBefore1, areaAfter1));

  delete PolyGraph;
  delete ValPoly;

}


// =====================================================================
// =====================================================================


BOOST_AUTO_TEST_CASE(check_mergePolygonEntitiesByCompactness)
{
  openfluid::core::GeoVectorValue* ValPoly = new openfluid::core::GeoVectorValue(
      CONFIGTESTS_INPUT_MISCDATA_DIR + "/landr", "SU.shp");

  openfluid::landr::PolygonGraph* PolyGraph =
      openfluid::landr::PolygonGraph::create(*ValPoly);


  BOOST_CHECK_THROW(PolyGraph->mergePolygonEntitiesByCompactness(-1),openfluid::base::FrameworkException);
  BOOST_CHECK_THROW(PolyGraph->mergePolygonEntitiesByCompactness(0),openfluid::base::FrameworkException);
  BOOST_CHECK_THROW(PolyGraph->mergePolygonEntitiesByCompactness(-0.000000001),openfluid::base::FrameworkException);
  PolyGraph->mergePolygonEntitiesByCompactness(1.7);
  BOOST_CHECK_EQUAL(PolyGraph->getEntities().size(), 24);


  double areaBefore=PolyGraph->entity(13)->getArea()+PolyGraph->entity(14)->getArea();
  PolyGraph->mergePolygonEntitiesByCompactness(1.6);
  BOOST_CHECK_EQUAL(PolyGraph->getEntities().size(), 23);
  BOOST_CHECK(!PolyGraph->entity(13));
  double areaAfter=PolyGraph->entity(14)->getArea();
  BOOST_CHECK( openfluid::scientific::isVeryClose(areaBefore, areaAfter));
  delete PolyGraph;

  PolyGraph = openfluid::landr::PolygonGraph::create(*ValPoly);
  areaBefore=PolyGraph->entity(13)->getArea()+PolyGraph->entity(14)->getArea();
  double areaBefore1=PolyGraph->entity(18)->getArea()+PolyGraph->entity(17)->getArea();
  PolyGraph->mergePolygonEntitiesByCompactness(1.51);
  BOOST_CHECK_EQUAL(PolyGraph->getEntities().size(), 22);
  BOOST_CHECK(!PolyGraph->entity(13));
  BOOST_CHECK(!PolyGraph->entity(18));
  BOOST_CHECK(PolyGraph->entity(17));
  BOOST_CHECK(PolyGraph->entity(14));
  areaAfter=PolyGraph->entity(14)->getArea();
  double areaAfter1=PolyGraph->entity(17)->getArea();
  BOOST_CHECK( openfluid::scientific::isVeryClose(areaBefore, areaAfter));
  BOOST_CHECK( openfluid::scientific::isVeryClose(areaBefore1, areaAfter1));

  delete PolyGraph;
  delete ValPoly;

}


// =====================================================================
// =====================================================================


BOOST_AUTO_TEST_CASE(check_construction_from_Bad_Polygon_Geometry)
{
  openfluid::core::GeoVectorValue* Val = new openfluid::core::GeoVectorValue(
      CONFIGTESTS_INPUT_MISCDATA_DIR + "/landr", "BAD_POLYGEOM.shp");

  openfluid::landr::PolygonGraph* Graph = nullptr;
  BOOST_CHECK_THROW(openfluid::landr::PolygonGraph::create(*Val),openfluid::base::FrameworkException);

  delete Val;
  delete Graph;
}


// =====================================================================
// =====================================================================


BOOST_AUTO_TEST_CASE(check_construction_from_non_PolygonType)
{
  openfluid::core::GeoVectorValue* Val = new openfluid::core::GeoVectorValue(
      CONFIGTESTS_INPUT_MISCDATA_DIR + "/landr", "RS.shp");

  openfluid::landr::PolygonGraph* Graph1 = nullptr;
  BOOST_CHECK_THROW(Graph1 = openfluid::landr::PolygonGraph::create(*Val),openfluid::base::FrameworkException);

  delete Val;
  delete Graph1;


  openfluid::core::GeoVectorValue Val2(CONFIGTESTS_INPUT_MISCDATA_DIR + "/landr",
                                       "RS.shp");

  openfluid::landr::VectorDataset* Vect =
      new openfluid::landr::VectorDataset(Val2);

  openfluid::landr::PolygonGraph* Graph2=nullptr;
  BOOST_CHECK_THROW( Graph2 = openfluid::landr::PolygonGraph::create(*Vect),
                     openfluid::base::FrameworkException);

  delete Vect;
  delete Graph2;

  openfluid::core::GeoVectorValue* Val3 = new openfluid::core::GeoVectorValue(
      CONFIGTESTS_INPUT_MISCDATA_DIR + "/landr", "PU.shp");

  openfluid::landr::PolygonGraph* Graph3 = nullptr;
  BOOST_CHECK_THROW(Graph3 = openfluid::landr::PolygonGraph::create(*Val3),openfluid::base::FrameworkException);

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
