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
 \file PolygonGraph_TEST.cpp
 \brief Implements ...

 \author Aline LIBRES <aline.libres@gmail.com>
 */

#define BOOST_TEST_MAIN
#define BOOST_AUTO_TEST_MAIN
#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MODULE unittest_polygongraph
#include <boost/test/unit_test.hpp>
#include <boost/test/auto_unit_test.hpp>
#include <boost/filesystem/path.hpp>
#include <tests-config.hpp>
#include <openfluid/base/OFException.hpp>
#include <openfluid/core/GeoVectorValue.hpp>
#include <openfluid/core/GeoRasterValue.hpp>
#include <openfluid/core/IntegerValue.hpp>
#include <openfluid/core/StringValue.hpp>
#include <openfluid/landr/PolygonGraph.hpp>
#include <openfluid/landr/PolygonEntity.hpp>
#include <openfluid/landr/LineStringGraph.hpp>
#include <openfluid/landr/LineStringEntity.hpp>
#include <openfluid/landr/VectorDataset.hpp>
#include <openfluid/tools.hpp>
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
      CONFIGTESTS_INPUT_DATASETS_DIR + "/landr", "SU.shp");

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
  openfluid::core::GeoVectorValue Val(CONFIGTESTS_INPUT_DATASETS_DIR + "/landr",
                                      "SU.shp");

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

    openfluid::landr::LandREntity* Entity = new openfluid::landr::PolygonEntity(
        dynamic_cast<geos::geom::Polygon*>(GeosGeom->clone()),
        Feat->GetFieldAsInteger("SELF_ID"));

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
  geos::geom::Polygon* P1 = Factory.createPolygon(LR1, NULL);
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

  BOOST_CHECK_EQUAL(Graph->getEntity(1)->getNeighboursAndEdges()->size(), 0);

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
  geos::geom::Polygon* P1 = Factory.createPolygon(LR1, NULL);
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
  geos::geom::Polygon* P2 = Factory.createPolygon(LR2, NULL);
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

  openfluid::landr::PolygonEntity* p_Ent1 = Graph->getEntity(1);
  openfluid::landr::PolygonEntity* p_Ent2 = Graph->getEntity(2);

  BOOST_CHECK_EQUAL(p_Ent1->getOrderedNeighbourSelfIds().size(), 1);
  BOOST_CHECK_EQUAL(p_Ent1->getOrderedNeighbourSelfIds()[0], 2);
  BOOST_CHECK_EQUAL(p_Ent2->getOrderedNeighbourSelfIds().size(), 1);
  BOOST_CHECK_EQUAL(p_Ent2->getOrderedNeighbourSelfIds()[0], 1);

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
  geos::geom::Polygon* P1 = Factory.createPolygon(LR1, NULL);
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
  geos::geom::Polygon* P2 = Factory.createPolygon(LR2, NULL);
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
  geos::geom::Polygon* P3 = Factory.createPolygon(LR3, NULL);
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

  openfluid::landr::PolygonEntity* p_Ent1 = Graph->getEntity(1);
  openfluid::landr::PolygonEntity* p_Ent2 = Graph->getEntity(2);
  openfluid::landr::PolygonEntity* p_Ent3 = Graph->getEntity(3);

  BOOST_CHECK_EQUAL(p_Ent1->getOrderedNeighbourSelfIds().size(), 1);
  BOOST_CHECK_EQUAL(p_Ent1->getOrderedNeighbourSelfIds()[0], 3);
  BOOST_CHECK_EQUAL(p_Ent3->getOrderedNeighbourSelfIds().size(), 1);
  BOOST_CHECK_EQUAL(p_Ent3->getOrderedNeighbourSelfIds()[0], 1);
  BOOST_CHECK_EQUAL(p_Ent2->getOrderedNeighbourSelfIds().size(), 0);

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
  geos::geom::Polygon* P1 = Factory.createPolygon(LR1, NULL);
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
  geos::geom::Polygon* P2 = Factory.createPolygon(LR2, NULL);
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
  geos::geom::Polygon* P3 = Factory.createPolygon(LR3, NULL);
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
  geos::geom::Polygon* P4 = Factory.createPolygon(LR4, NULL);
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

  BOOST_CHECK_EQUAL(Graph->getEntity(1)->getNeighboursAndEdges()->size(), 3);
  BOOST_CHECK_EQUAL(Graph->getEntity(2)->getNeighboursAndEdges()->size(), 3);
  BOOST_CHECK_EQUAL(Graph->getEntity(3)->getNeighboursAndEdges()->size(), 3);

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
  geos::geom::Polygon* P1 = Factory.createPolygon(LR1, NULL);
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
  geos::geom::Polygon* P2 = Factory.createPolygon(LR2, NULL);
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

  BOOST_CHECK_EQUAL(Graph->getEntity(1)->getNeighboursAndEdges()->size(), 0);
  BOOST_CHECK_EQUAL(Graph->getEntity(2)->getNeighboursAndEdges()->size(), 0);

  delete Graph;
}

// =====================================================================
// =====================================================================

BOOST_AUTO_TEST_CASE(check_construction_horseshoeShapedPolygons_linesContact)
{
  openfluid::core::GeoVectorValue* Val = new openfluid::core::GeoVectorValue(
      CONFIGTESTS_INPUT_DATASETS_DIR + "/landr", "SU_horseshoe_lines.shp");

  openfluid::landr::PolygonGraph* Graph =
      openfluid::landr::PolygonGraph::create(*Val);

  BOOST_CHECK_EQUAL(Graph->getSize(), 3);
  BOOST_CHECK_EQUAL(Graph->getEdges()->size(), 6);
  std::vector<geos::planargraph::Node*> Nodes;
  Graph->getNodes(Nodes);
  BOOST_CHECK_EQUAL(Nodes.size(), 4);
  BOOST_CHECK(Graph->isComplete());

  BOOST_CHECK_EQUAL(Graph->getEntity(1)->getNeighboursAndEdges()->size(), 2);
  BOOST_CHECK_EQUAL(Graph->getEntity(2)->getNeighboursAndEdges()->size(), 2);
  BOOST_CHECK_EQUAL(Graph->getEntity(3)->getNeighboursAndEdges()->size(), 2);

  delete Graph;
  delete Val;
}

// =====================================================================
// =====================================================================

BOOST_AUTO_TEST_CASE(check_construction_horseshoeShapedPolygons_pointContact)
{
  openfluid::core::GeoVectorValue* Val = new openfluid::core::GeoVectorValue(
      CONFIGTESTS_INPUT_DATASETS_DIR + "/landr", "SU_horseshoe_point.shp");

  openfluid::landr::PolygonGraph* Graph =
      openfluid::landr::PolygonGraph::create(*Val);

  BOOST_CHECK_EQUAL(Graph->getSize(), 3);
  BOOST_CHECK_EQUAL(Graph->getEdges()->size(), 5);
  std::vector<geos::planargraph::Node*> Nodes;
  Graph->getNodes(Nodes);
  BOOST_CHECK_EQUAL(Nodes.size(), 3);
  BOOST_CHECK(Graph->isComplete());

  BOOST_CHECK_EQUAL(Graph->getEntity(1)->getNeighboursAndEdges()->size(), 2);
  BOOST_CHECK_EQUAL(Graph->getEntity(2)->getNeighboursAndEdges()->size(), 2);
  BOOST_CHECK_EQUAL(Graph->getEntity(3)->getNeighboursAndEdges()->size(), 2);

  delete Graph;
  delete Val;
}

// =====================================================================
// =====================================================================

//BOOST_AUTO_TEST_CASE(check_clone)
//{
//  openfluid::core::GeoVectorValue* Val = new openfluid::core::GeoVectorValue(
//      CONFIGTESTS_INPUT_DATASETS_DIR + "/landr", "SU.shp");
//
//  openfluid::landr::PolygonGraph* Graph =
//      openfluid::landr::PolygonGraph::create(*Val);
//
//  openfluid::landr::PolygonGraph* Copy = Graph->clone();
//
//  BOOST_CHECK_EQUAL(Graph->getSize(), Copy->getSize());
//  BOOST_CHECK_EQUAL(Graph->getEdges()->size(), Copy->getEdges()->size());
//  BOOST_CHECK_EQUAL(Graph->getEntities().size(), Copy->getEntities().size());
//
//  BOOST_CHECK(Graph->isComplete());
//
//  delete Graph;
//  delete Copy;
//  delete Val;
//}

// =====================================================================
// =====================================================================

BOOST_AUTO_TEST_CASE(check_getEntity)
{
  openfluid::core::GeoVectorValue* Val = new openfluid::core::GeoVectorValue(
      CONFIGTESTS_INPUT_DATASETS_DIR + "/landr", "SU.shp");

  openfluid::landr::PolygonGraph* Graph =
      openfluid::landr::PolygonGraph::create(*Val);

  BOOST_CHECK(Graph->getEntity(1));
  BOOST_CHECK(Graph->getEntity(5));
  BOOST_CHECK(Graph->getEntity(3));
  BOOST_CHECK(!Graph->getEntity(100));

  delete Graph;
  delete Val;
}

// =====================================================================
// =====================================================================

BOOST_AUTO_TEST_CASE(check_addRemoveAttribute)
{
  openfluid::core::GeoVectorValue* Val = new openfluid::core::GeoVectorValue(
      CONFIGTESTS_INPUT_DATASETS_DIR + "/landr", "SU.shp");

  openfluid::landr::PolygonGraph* Graph =
      openfluid::landr::PolygonGraph::create(*Val);

  openfluid::landr::PolygonEntity* U1 = Graph->getEntity(1);
  openfluid::landr::PolygonEntity* U2 = Graph->getEntity(2);

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

BOOST_AUTO_TEST_CASE(check_getSelfIdOrderedEntities)
{
  openfluid::core::GeoVectorValue* Val = new openfluid::core::GeoVectorValue(
      CONFIGTESTS_INPUT_DATASETS_DIR + "/landr", "SU.shp");

  openfluid::landr::PolygonGraph* Graph =
      openfluid::landr::PolygonGraph::create(*Val);

  openfluid::landr::LandRGraph::Entities_t OrderedEntities =
      Graph->getSelfIdOrderedEntities();

  int i = 0;
  for (openfluid::landr::LandRGraph::Entities_t::iterator it =
      OrderedEntities.begin(); it != OrderedEntities.end(); ++it)
    BOOST_CHECK_EQUAL((*it)->getSelfId(), ++i);

  delete Graph;
  delete Val;
}

// =====================================================================
// =====================================================================

//BOOST_AUTO_TEST_CASE(check_getRasterPolyOverlapping_gettingPolygonsOnly)
//{
//  openfluid::core::GeoVectorValue* Vector = new openfluid::core::GeoVectorValue(
//      CONFIGTESTS_INPUT_DATASETS_DIR + "/landr", "SU.shp");
//
//  openfluid::core::GeoRasterValue* Raster = new openfluid::core::GeoRasterValue(
//      CONFIGTESTS_INPUT_DATASETS_DIR + "/GeoRasterValue", "dem.jpeg");
//
//  openfluid::landr::PolygonGraph* Graph =
//      openfluid::landr::PolygonGraph::create(*Vector);
//
//  openfluid::landr::PolygonEntity* U1 = Graph->getEntity(1);
//
//  BOOST_CHECK_THROW(Graph->getRasterPolyOverlapping(*U1),
//                    openfluid::base::OFException);
//
//  Graph->addAGeoRasterValue(*Raster);
//
//  openfluid::landr::PolygonGraph::RastValByRastPoly_t OverlapsU1 =
//      Graph->getRasterPolyOverlapping(*U1);
//
//  BOOST_CHECK_EQUAL(OverlapsU1.size(), 12);
//
//  double Area;
//
//  for (openfluid::landr::PolygonGraph::RastValByRastPoly_t::iterator it =
//      OverlapsU1.begin(); it != OverlapsU1.end(); ++it)
//  {
//    Area += it->second;
//
//    BOOST_CHECK((int*)it->first->getUserData());
//  }
//
//  BOOST_CHECK(openfluid::tools::IsVeryClose(Area,U1->getArea()));
//
//  delete Graph;
//  delete Vector;
//  delete Raster;
//}
//
//// =====================================================================
//// =====================================================================
//
//BOOST_AUTO_TEST_CASE(check_getRasterPolyOverlapping_gettingAlsoMultiPolygon)
//{
//  openfluid::core::GeoVectorValue* Vector = new openfluid::core::GeoVectorValue(
//      CONFIGTESTS_INPUT_DATASETS_DIR + "/landr", "SU.shp");
//
//  openfluid::core::GeoRasterValue* Raster = new openfluid::core::GeoRasterValue(
//      CONFIGTESTS_INPUT_DATASETS_DIR + "/GeoRasterValue", "dem.jpeg");
//
//  openfluid::landr::PolygonGraph* Graph =
//      openfluid::landr::PolygonGraph::create(*Vector);
//
//  Graph->addAGeoRasterValue(*Raster);
//
//  openfluid::landr::PolygonEntity* U2 = Graph->getEntity(2);
//
//  openfluid::landr::PolygonGraph::RastValByRastPoly_t OverlapsU2 =
//      Graph->getRasterPolyOverlapping(*U2);
//
//  BOOST_CHECK_EQUAL(OverlapsU2.size(), 20);
//
//  double Area = 0;
//
//  for (openfluid::landr::PolygonGraph::RastValByRastPoly_t::iterator it =
//      OverlapsU2.begin(); it != OverlapsU2.end(); ++it)
//  {
//    Area += it->second;
//
//    BOOST_CHECK((int*)it->first->getUserData());
//  }
//
//  BOOST_CHECK(openfluid::tools::IsVeryClose(Area,U2->getArea()));
//
//  delete Graph;
//  delete Vector;
//  delete Raster;
//}
//
//// =====================================================================
//// =====================================================================
//
//BOOST_AUTO_TEST_CASE(check_setAttributeFromRasterValueAtCentroid_intPixelType)
//{
//  openfluid::core::GeoVectorValue* Vector = new openfluid::core::GeoVectorValue(
//      CONFIGTESTS_INPUT_DATASETS_DIR + "/landr", "SU.shp");
//
//  openfluid::core::GeoRasterValue* Raster = new openfluid::core::GeoRasterValue(
//      CONFIGTESTS_INPUT_DATASETS_DIR + "/GeoRasterValue", "dem.jpeg");
//
//  openfluid::landr::PolygonGraph* Graph =
//      openfluid::landr::PolygonGraph::create(*Vector);
//
//  Graph->addAGeoRasterValue(*Raster);
//
//  Graph->setAttributeFromRasterValueAtCentroid("test_val");
//
//  openfluid::core::DoubleValue Val;
//
//  Graph->getEntity(1)->getAttributeValue("test_val", Val);
//  BOOST_CHECK_EQUAL(Val.get(), 29);
//
//  Graph->getEntity(2)->getAttributeValue("test_val", Val);
//  BOOST_CHECK_EQUAL(Val.get(), 47);
//
//  delete Graph;
//  delete Vector;
//  delete Raster;
//}
//
//// =====================================================================
//// =====================================================================
//
//BOOST_AUTO_TEST_CASE(check_setAttributeFromRasterValueAtCentroid_float32PixelType)
//{
//  openfluid::core::GeoVectorValue* Vector = new openfluid::core::GeoVectorValue(
//      CONFIGTESTS_INPUT_DATASETS_DIR + "/landr", "SU.shp");
//
//  openfluid::core::GeoRasterValue* Raster = new openfluid::core::GeoRasterValue(
//      CONFIGTESTS_INPUT_DATASETS_DIR + "/GeoRasterValue", "dem.asc");
//
//  openfluid::landr::PolygonGraph* Graph =
//      openfluid::landr::PolygonGraph::create(*Vector);
//
//  Graph->addAGeoRasterValue(*Raster);
//
//  Graph->setAttributeFromRasterValueAtCentroid("test_val");
//
//  openfluid::core::DoubleValue Val;
//
//  Graph->getEntity(1)->getAttributeValue("test_val", Val);
//  BOOST_CHECK( openfluid::tools::IsVeryClose(Val.get(), 32.9131));
//
//  Graph->getEntity(2)->getAttributeValue("test_val", Val);
//  BOOST_CHECK( openfluid::tools::IsVeryClose(Val.get(), 51.0607));
//
//  delete Graph;
//  delete Vector;
//  delete Raster;
//}
//
//// =====================================================================
//// =====================================================================
//
//BOOST_AUTO_TEST_CASE(check_setAttributeFromRasterValueAtCentroid_float64PixelType)
//{
//  openfluid::core::GeoVectorValue* Vector = new openfluid::core::GeoVectorValue(
//      CONFIGTESTS_INPUT_DATASETS_DIR + "/landr", "SU.shp");
//
//  openfluid::core::GeoRasterValue* Raster = new openfluid::core::GeoRasterValue(
//      CONFIGTESTS_INPUT_DATASETS_DIR + "/GeoRasterValue", "dem.Gtiff");
//
//  openfluid::landr::PolygonGraph* Graph =
//      openfluid::landr::PolygonGraph::create(*Vector);
//
//  Graph->addAGeoRasterValue(*Raster);
//
//  Graph->setAttributeFromRasterValueAtCentroid("test_val");
//
//  openfluid::core::DoubleValue Val;
//
//  Graph->getEntity(1)->getAttributeValue("test_val", Val);
//  BOOST_CHECK( openfluid::tools::IsVeryClose(Val.get(), 32.9131));
//
//  Graph->getEntity(2)->getAttributeValue("test_val", Val);
//  BOOST_CHECK( openfluid::tools::IsVeryClose(Val.get(), 51.0607));
//
//  delete Graph;
//  delete Vector;
//  delete Raster;
//}
//
//// =====================================================================
//// =====================================================================
//
//BOOST_AUTO_TEST_CASE(check_setAttributeFromMeanRasterValues_intPixelType)
//{
//  openfluid::core::GeoVectorValue* Vector = new openfluid::core::GeoVectorValue(
//      CONFIGTESTS_INPUT_DATASETS_DIR + "/landr", "SU.shp");
//
//  openfluid::core::GeoRasterValue* Raster = new openfluid::core::GeoRasterValue(
//      CONFIGTESTS_INPUT_DATASETS_DIR + "/GeoRasterValue", "dem.jpeg");
//
//  openfluid::landr::PolygonGraph* Graph =
//      openfluid::landr::PolygonGraph::create(*Vector);
//
//  Graph->addAGeoRasterValue(*Raster);
//
//  Graph->setAttributeFromMeanRasterValues("test_val");
//
//  openfluid::core::DoubleValue Val;
//
//  Graph->getEntity(1)->getAttributeValue("test_val", Val);
//  BOOST_CHECK( openfluid::tools::IsVeryClose(Val.get(), 34.0569));
//
//  Graph->getEntity(2)->getAttributeValue("test_val", Val);
//  BOOST_CHECK( openfluid::tools::IsVeryClose(Val.get(), 46.6497));
//
//  delete Graph;
//  delete Vector;
//  delete Raster;
//}
//
//// =====================================================================
//// =====================================================================
//
//BOOST_AUTO_TEST_CASE(check_setAttributeFromMeanRasterValues_float32PixelType)
//{
//  openfluid::core::GeoVectorValue* Vector = new openfluid::core::GeoVectorValue(
//      CONFIGTESTS_INPUT_DATASETS_DIR + "/landr", "SU.shp");
//
//  openfluid::core::GeoRasterValue* Raster = new openfluid::core::GeoRasterValue(
//      CONFIGTESTS_INPUT_DATASETS_DIR + "/GeoRasterValue", "dem.asc");
//
//  openfluid::landr::PolygonGraph* Graph =
//      openfluid::landr::PolygonGraph::create(*Vector);
//
//  Graph->addAGeoRasterValue(*Raster);
//
//  Graph->setAttributeFromMeanRasterValues("test_val");
//
//  openfluid::core::DoubleValue Val;
//
//  Graph->getEntity(1)->getAttributeValue("test_val", Val);
//  BOOST_CHECK( openfluid::tools::IsVeryClose(Val.get(), 33.5981));
//
//  Graph->getEntity(2)->getAttributeValue("test_val", Val);
//  BOOST_CHECK( openfluid::tools::IsVeryClose(Val.get(), 46.7352));
//
//  delete Graph;
//  delete Vector;
//  delete Raster;
//}
//
//// =====================================================================
//// =====================================================================
//
//BOOST_AUTO_TEST_CASE(check_setAttributeFromMeanRasterValues_float64PixelType)
//{
//  openfluid::core::GeoVectorValue* Vector = new openfluid::core::GeoVectorValue(
//      CONFIGTESTS_INPUT_DATASETS_DIR + "/landr", "SU.shp");
//
//  openfluid::core::GeoRasterValue* Raster = new openfluid::core::GeoRasterValue(
//      CONFIGTESTS_INPUT_DATASETS_DIR + "/GeoRasterValue", "dem.Gtiff");
//
//  openfluid::landr::PolygonGraph* Graph =
//      openfluid::landr::PolygonGraph::create(*Vector);
//
//  Graph->addAGeoRasterValue(*Raster);
//
//  Graph->setAttributeFromMeanRasterValues("test_val");
//
//  openfluid::core::DoubleValue Val;
//
//  Graph->getEntity(1)->getAttributeValue("test_val", Val);
//  BOOST_CHECK( openfluid::tools::IsVeryClose(Val.get(), 33.5981));
//
//  Graph->getEntity(2)->getAttributeValue("test_val", Val);
//  BOOST_CHECK( openfluid::tools::IsVeryClose(Val.get(), 46.7352));
//
//  delete Graph;
//  delete Vector;
//  delete Raster;
//}

// =====================================================================
// =====================================================================

BOOST_AUTO_TEST_CASE(check_createVectorRepresentation)
{
  openfluid::core::GeoVectorValue* Val = new openfluid::core::GeoVectorValue(
      CONFIGTESTS_INPUT_DATASETS_DIR + "/landr", "SU.shp");

  openfluid::landr::PolygonGraph* Graph =
      openfluid::landr::PolygonGraph::create(*Val);

  Graph->createVectorRepresentation(CONFIGTESTS_OUTPUT_DATA_DIR + "/landr",
                                    "SUGraph.shp");

  openfluid::core::GeoVectorValue GraphVal(
      CONFIGTESTS_OUTPUT_DATA_DIR + "/landr", "SUGraph.shp");

  openfluid::landr::VectorDataset* GraphVect =
      new openfluid::landr::VectorDataset(GraphVal);

  BOOST_CHECK_EQUAL(Graph->getEdges()->size(), 58);
  BOOST_CHECK_EQUAL(GraphVect->getLayer(0)->GetFeatureCount(), 58);

  delete GraphVect;
  delete Graph;
  delete Val;
}

// =====================================================================
// =====================================================================

BOOST_AUTO_TEST_CASE(check_computeLineStringNeighboursOfPolygonGraph_Contains)
{
  openfluid::core::GeoVectorValue* ValRS = new openfluid::core::GeoVectorValue(
      CONFIGTESTS_INPUT_DATASETS_DIR + "/landr", "RS.shp");

  openfluid::core::GeoVectorValue* ValSU = new openfluid::core::GeoVectorValue(
      CONFIGTESTS_INPUT_DATASETS_DIR + "/landr", "SU.shp");

  openfluid::landr::LineStringGraph* RSGraph =
      openfluid::landr::LineStringGraph::create(*ValRS);
  openfluid::landr::PolygonGraph* SUGraph =
      openfluid::landr::PolygonGraph::create(*ValSU);

  openfluid::landr::PolygonEntity* SU1 = SUGraph->getEntity(1);
  openfluid::landr::PolygonEntity* SU4 = SUGraph->getEntity(4);
  openfluid::landr::PolygonEntity* SU17 = SUGraph->getEntity(17);

  BOOST_CHECK(!SU1->getLineStringNeighbours());
  BOOST_CHECK(!SU4->getLineStringNeighbours());
  BOOST_CHECK(!SU17->getLineStringNeighbours());

  SUGraph->computeLineStringNeighbours(*RSGraph,
                                       openfluid::landr::LandRTools::CONTAINS,
                                       0);

  BOOST_CHECK_EQUAL(SU1->getLineStringNeighbours()->size(), 0);
  BOOST_CHECK_EQUAL(SU4->getLineStringNeighbours()->size(), 0);
  BOOST_CHECK_EQUAL(SU17->getLineStringNeighbours()->size(), 0);

  BOOST_CHECK_EQUAL(SU1->getNeighbours()->size(), 2);
  BOOST_CHECK_EQUAL(SU4->getNeighbours()->size(), 4);
  BOOST_CHECK_EQUAL(SU17->getNeighbours()->size(), 4);

  SUGraph->computeLineStringNeighbours(*RSGraph,
                                       openfluid::landr::LandRTools::CONTAINS,
                                       0.0001);

  BOOST_CHECK_EQUAL(SU1->getLineStringNeighbours()->size(), 1);
  BOOST_CHECK_EQUAL(SU1->getLineStringNeighbours()->begin()->first->getSelfId(),
                    1);
  BOOST_CHECK_EQUAL(SU4->getLineStringNeighbours()->size(), 1);
  BOOST_CHECK_EQUAL(SU4->getLineStringNeighbours()->begin()->first->getSelfId(),
                    3);
  BOOST_CHECK_EQUAL(SU17->getLineStringNeighbours()->size(), 1);
  BOOST_CHECK_EQUAL(
      SU17->getLineStringNeighbours()->begin()->first->getSelfId(), 6);

  BOOST_CHECK_EQUAL(SU1->getNeighbours()->size(), 3);
  BOOST_CHECK_EQUAL(SU4->getNeighbours()->size(), 5);
  BOOST_CHECK_EQUAL(SU17->getNeighbours()->size(), 5);

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
      CONFIGTESTS_INPUT_DATASETS_DIR + "/landr", "RS.shp");

  openfluid::core::GeoVectorValue* ValSU = new openfluid::core::GeoVectorValue(
      CONFIGTESTS_INPUT_DATASETS_DIR + "/landr", "SU.shp");

  openfluid::landr::LineStringGraph* RSGraph =
      openfluid::landr::LineStringGraph::create(*ValRS);
  openfluid::landr::PolygonGraph* SUGraph =
      openfluid::landr::PolygonGraph::create(*ValSU);

  openfluid::landr::PolygonEntity* SU1 = SUGraph->getEntity(1);
  openfluid::landr::PolygonEntity* SU4 = SUGraph->getEntity(4);
  openfluid::landr::PolygonEntity* SU17 = SUGraph->getEntity(17);

  BOOST_CHECK(!SU1->getLineStringNeighbours());
  BOOST_CHECK(!SU4->getLineStringNeighbours());
  BOOST_CHECK(!SU17->getLineStringNeighbours());

  SUGraph->computeLineStringNeighbours(*RSGraph,
                                       openfluid::landr::LandRTools::INTERSECTS,
                                       0);

  BOOST_CHECK_EQUAL(SU1->getLineStringNeighbours()->size(), 0);
  BOOST_CHECK_EQUAL(SU4->getLineStringNeighbours()->size(), 0);
  BOOST_CHECK_EQUAL(SU17->getLineStringNeighbours()->size(), 0);

  BOOST_CHECK_EQUAL(SU1->getNeighbours()->size(), 2);
  BOOST_CHECK_EQUAL(SU4->getNeighbours()->size(), 4);
  BOOST_CHECK_EQUAL(SU17->getNeighbours()->size(), 4);

  SUGraph->computeLineStringNeighbours(*RSGraph,
                                       openfluid::landr::LandRTools::INTERSECTS,
                                       0.0001);

  BOOST_CHECK_EQUAL(SU1->getLineStringNeighbours()->size(), 2);
  BOOST_CHECK_EQUAL(SU4->getLineStringNeighbours()->size(), 6);
  BOOST_CHECK_EQUAL(SU17->getLineStringNeighbours()->size(), 3);

  BOOST_CHECK_EQUAL(SU1->getNeighbours()->size(), 4);
  BOOST_CHECK_EQUAL(SU4->getNeighbours()->size(), 10);
  BOOST_CHECK_EQUAL(SU17->getNeighbours()->size(), 7);

  delete RSGraph;
  delete SUGraph;
  delete ValRS;
  delete ValSU;
}

// =====================================================================
// =====================================================================

