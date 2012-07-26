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
#include <openfluid/landr/PolygonGraph.hpp>
#include <openfluid/landr/PolygonEntity.hpp>
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

  openfluid::landr::PolygonGraph* Graph = new openfluid::landr::PolygonGraph(
      *Val);

  BOOST_CHECK_EQUAL(Graph->getSize(), 24);

  BOOST_CHECK_EQUAL(Graph->getEdges()->size(), 58);

  std::vector<openfluid::landr::PolygonEntity*> Entities = Graph->getEntities();
  std::set<openfluid::landr::PolygonEdge*> Edges;
  for (unsigned int i = 0; i < Entities.size(); i++)
    Edges.insert(Entities[i]->m_PolyEdges.begin(),
                 Entities[i]->m_PolyEdges.end());

  BOOST_CHECK_EQUAL(Edges.size(), 58);

  BOOST_CHECK(Graph->isComplete());

  delete Graph;
  delete Val;
}

// =====================================================================
// =====================================================================

BOOST_AUTO_TEST_CASE(check_construction_fromPolygonEntityVector)
{
  openfluid::core::GeoVectorValue* Val = new openfluid::core::GeoVectorValue(
      CONFIGTESTS_INPUT_DATASETS_DIR + "/landr", "SU.shp");

  std::vector<openfluid::landr::PolygonEntity*> Entities;

  OGRLayer* Layer0 = Val->getLayer0();

  Layer0->ResetReading();

  OGRFeature* Feat;
  while ((Feat = Layer0->GetNextFeature()) != NULL)
  {
    OGRGeometry* OGRGeom = Feat->GetGeometryRef();

    geos::geom::Geometry* GeosGeom =
        (geos::geom::Geometry*) OGRGeom->exportToGEOS();

    openfluid::landr::PolygonEntity* Entity =
        new openfluid::landr::PolygonEntity(
            dynamic_cast<geos::geom::Polygon*>(GeosGeom->clone()),
            Feat->Clone());

    Entities.push_back(Entity);

    // destroying the feature destroys also the associated OGRGeom
    OGRFeature::DestroyFeature(Feat);
    delete GeosGeom;
  }

  openfluid::landr::PolygonGraph* Graph = new openfluid::landr::PolygonGraph(
      Entities);

  BOOST_CHECK_EQUAL(Graph->getSize(), 24);
  BOOST_CHECK_EQUAL(Graph->getEdges()->size(), 58);

  BOOST_CHECK(Graph->isComplete());

  delete Graph;
  delete Val;
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

  std::vector<openfluid::landr::PolygonEntity*> Entites;
  OGRFeature* Feat;
  OGRFeatureDefn* Def = new OGRFeatureDefn("");
  Def->AddFieldDefn(new OGRFieldDefn("SELF_ID", OFTInteger));

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
  Feat = new OGRFeature(Def);
  Feat->SetField("SELF_ID", 1);

  openfluid::landr::PolygonEntity Ent1(P1, Feat);
  Entites.push_back(&Ent1);

  openfluid::landr::PolygonGraph Graph(Entites);

  BOOST_CHECK_EQUAL(Graph.getSize(), 1);
  BOOST_CHECK_EQUAL(Graph.getEdges()->size(), 1);
  std::vector<geos::planargraph::Node*> Nodes;
  Graph.getNodes(Nodes);
  BOOST_CHECK_EQUAL(Nodes.size(), 1);
  BOOST_CHECK(Graph.isComplete());

  BOOST_CHECK_EQUAL(Graph.getEntity(1)->getNeighbours()->size(), 0);
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

  std::vector<openfluid::landr::PolygonEntity*> Entites;
  OGRFeature* Feat;
  OGRFeatureDefn* Def = new OGRFeatureDefn("");
  Def->AddFieldDefn(new OGRFieldDefn("SELF_ID", OFTInteger));

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
  Feat = new OGRFeature(Def);
  Feat->SetField("SELF_ID", 1);
  openfluid::landr::PolygonEntity Ent1(P1, Feat);
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
  Feat = new OGRFeature(Def);
  Feat->SetField("SELF_ID", 2);
  openfluid::landr::PolygonEntity Ent2(P2, Feat);
  Entites.push_back(&Ent2);

  openfluid::landr::PolygonGraph Graph(Entites);

  BOOST_CHECK_EQUAL(Graph.getSize(), 2);
  BOOST_CHECK_EQUAL(Graph.getEdges()->size(), 3);
  std::vector<geos::planargraph::Node*> Nodes;
  Graph.getNodes(Nodes);
  BOOST_CHECK_EQUAL(Nodes.size(), 2);
  BOOST_CHECK(Graph.isComplete());

  openfluid::landr::PolygonEntity* p_Ent1 = Graph.getEntity(1);
  openfluid::landr::PolygonEntity* p_Ent2 = Graph.getEntity(2);

  BOOST_CHECK_EQUAL(p_Ent1->getOrderedNeighbourSelfIds().size(), 1);
  BOOST_CHECK_EQUAL(p_Ent1->getOrderedNeighbourSelfIds()[0], 2);
  BOOST_CHECK_EQUAL(p_Ent2->getOrderedNeighbourSelfIds().size(), 1);
  BOOST_CHECK_EQUAL(p_Ent2->getOrderedNeighbourSelfIds()[0], 1);
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

  std::vector<openfluid::landr::PolygonEntity*> Entites;
  OGRFeature* Feat;
  OGRFeatureDefn* Def = new OGRFeatureDefn("");
  Def->AddFieldDefn(new OGRFieldDefn("SELF_ID", OFTInteger));

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
  Feat = new OGRFeature(Def);
  Feat->SetField("SELF_ID", 1);
  openfluid::landr::PolygonEntity Ent1(P1, Feat);
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
  Feat = new OGRFeature(Def);
  Feat->SetField("SELF_ID", 2);
  openfluid::landr::PolygonEntity Ent2(P2, Feat);
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
  Feat = new OGRFeature(Def);
  Feat->SetField("SELF_ID", 3);
  openfluid::landr::PolygonEntity Ent3(P3, Feat);
  Entites.push_back(&Ent3);

  openfluid::landr::PolygonGraph Graph(Entites);

  BOOST_CHECK_EQUAL(Graph.getSize(), 3);
  BOOST_CHECK_EQUAL(Graph.getEdges()->size(), 4);
  std::vector<geos::planargraph::Node*> Nodes;
  Graph.getNodes(Nodes);
  BOOST_CHECK_EQUAL(Nodes.size(), 3);
  BOOST_CHECK(Graph.isComplete());

  openfluid::landr::PolygonEntity* p_Ent1 = Graph.getEntity(1);
  openfluid::landr::PolygonEntity* p_Ent2 = Graph.getEntity(2);
  openfluid::landr::PolygonEntity* p_Ent3 = Graph.getEntity(3);

  BOOST_CHECK_EQUAL(p_Ent1->getOrderedNeighbourSelfIds().size(), 1);
  BOOST_CHECK_EQUAL(p_Ent1->getOrderedNeighbourSelfIds()[0], 3);
  BOOST_CHECK_EQUAL(p_Ent3->getOrderedNeighbourSelfIds().size(), 1);
  BOOST_CHECK_EQUAL(p_Ent3->getOrderedNeighbourSelfIds()[0], 1);
  BOOST_CHECK_EQUAL(p_Ent2->getOrderedNeighbourSelfIds().size(), 0);
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

  std::vector<openfluid::landr::PolygonEntity*> Entites;
  OGRFeature* Feat;
  OGRFeatureDefn* Def = new OGRFeatureDefn("");
  Def->AddFieldDefn(new OGRFieldDefn("SELF_ID", OFTInteger));

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
  Feat = new OGRFeature(Def);
  Feat->SetField("SELF_ID", 1);
  openfluid::landr::PolygonEntity Ent1(P1, Feat);
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
  Feat = new OGRFeature(Def);
  Feat->SetField("SELF_ID", 2);
  openfluid::landr::PolygonEntity Ent2(P2, Feat);
  Entites.push_back(&Ent2);

  openfluid::landr::PolygonGraph TmpGraph(Entites);

  BOOST_CHECK_EQUAL(TmpGraph.getSize(), 2);
  BOOST_CHECK_EQUAL(TmpGraph.getEdges()->size(), 3);
  std::vector<geos::planargraph::Node*> TmpNodes;
  TmpGraph.getNodes(TmpNodes);
  BOOST_CHECK_EQUAL(TmpNodes.size(), 2);
  BOOST_CHECK(TmpGraph.isComplete());

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
  Feat = new OGRFeature(Def);
  Feat->SetField("SELF_ID", 3);
  openfluid::landr::PolygonEntity Ent3(P3, Feat);
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
  Feat = new OGRFeature(Def);
  Feat->SetField("SELF_ID", 4);
  openfluid::landr::PolygonEntity Ent4(P4, Feat);
  Entites.push_back(&Ent4);

  openfluid::landr::PolygonGraph Graph(Entites);

  BOOST_CHECK_EQUAL(Graph.getSize(), 4);
  BOOST_CHECK_EQUAL(Graph.getEdges()->size(), 9);
  std::vector<geos::planargraph::Node*> Nodes;
  Graph.getNodes(Nodes);
  BOOST_CHECK_EQUAL(Nodes.size(), 6);
  BOOST_CHECK(Graph.isComplete());

  BOOST_CHECK_EQUAL(Graph.getEntity(1)->getNeighbours()->size(), 3);
  BOOST_CHECK_EQUAL(Graph.getEntity(2)->getNeighbours()->size(), 3);
  BOOST_CHECK_EQUAL(Graph.getEntity(3)->getNeighbours()->size(), 3);
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

  std::vector<openfluid::landr::PolygonEntity*> Entites;
  OGRFeature* Feat;
  OGRFeatureDefn* Def = new OGRFeatureDefn("");
  Def->AddFieldDefn(new OGRFieldDefn("SELF_ID", OFTInteger));

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
  Feat = new OGRFeature(Def);
  Feat->SetField("SELF_ID", 1);
  openfluid::landr::PolygonEntity Ent1(P1, Feat);
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
  Feat = new OGRFeature(Def);
  Feat->SetField("SELF_ID", 2);
  openfluid::landr::PolygonEntity Ent2(P2, Feat);
  Entites.push_back(&Ent2);

  openfluid::landr::PolygonGraph Graph(Entites);

  BOOST_CHECK_EQUAL(Graph.getSize(), 2);
  BOOST_CHECK_EQUAL(Graph.getEdges()->size(), 2);
  std::vector<geos::planargraph::Node*> Nodes;
  Graph.getNodes(Nodes);
  BOOST_CHECK_EQUAL(Nodes.size(), 2);
  BOOST_CHECK(Graph.isComplete());

  BOOST_CHECK_EQUAL(Graph.getEntity(1)->getNeighbours()->size(), 0);
  BOOST_CHECK_EQUAL(Graph.getEntity(2)->getNeighbours()->size(), 0);
}

// =====================================================================
// =====================================================================

BOOST_AUTO_TEST_CASE(check_construction_horseshoeShapedPolygons_linesContact)
{
  openfluid::core::GeoVectorValue* Val = new openfluid::core::GeoVectorValue(
      CONFIGTESTS_INPUT_DATASETS_DIR + "/landr", "SU_horseshoe_lines.shp");

  openfluid::landr::PolygonGraph Graph(*Val);

  BOOST_CHECK_EQUAL(Graph.getSize(), 3);
  BOOST_CHECK_EQUAL(Graph.getEdges()->size(), 6);
  std::vector<geos::planargraph::Node*> Nodes;
  Graph.getNodes(Nodes);
  BOOST_CHECK_EQUAL(Nodes.size(), 4);
  BOOST_CHECK(Graph.isComplete());

  BOOST_CHECK_EQUAL(Graph.getEntity(1)->getNeighbours()->size(), 2);
  BOOST_CHECK_EQUAL(Graph.getEntity(2)->getNeighbours()->size(), 2);
  BOOST_CHECK_EQUAL(Graph.getEntity(3)->getNeighbours()->size(), 2);

  delete Val;
}

// =====================================================================
// =====================================================================

BOOST_AUTO_TEST_CASE(check_construction_horseshoeShapedPolygons_pointContact)
{
  openfluid::core::GeoVectorValue* Val = new openfluid::core::GeoVectorValue(
      CONFIGTESTS_INPUT_DATASETS_DIR + "/landr", "SU_horseshoe_point.shp");

  openfluid::landr::PolygonGraph Graph(*Val);

  BOOST_CHECK_EQUAL(Graph.getSize(), 3);
  BOOST_CHECK_EQUAL(Graph.getEdges()->size(), 5);
  std::vector<geos::planargraph::Node*> Nodes;
  Graph.getNodes(Nodes);
  BOOST_CHECK_EQUAL(Nodes.size(), 3);
  BOOST_CHECK(Graph.isComplete());

  BOOST_CHECK_EQUAL(Graph.getEntity(1)->getNeighbours()->size(), 2);
  BOOST_CHECK_EQUAL(Graph.getEntity(2)->getNeighbours()->size(), 2);
  BOOST_CHECK_EQUAL(Graph.getEntity(3)->getNeighbours()->size(), 2);

  delete Val;
}

// =====================================================================
// =====================================================================

BOOST_AUTO_TEST_CASE(check_copy)
{
  openfluid::core::GeoVectorValue* Val = new openfluid::core::GeoVectorValue(
      CONFIGTESTS_INPUT_DATASETS_DIR + "/landr", "SU.shp");

  openfluid::landr::PolygonGraph* Graph = new openfluid::landr::PolygonGraph(
      *Val);

  openfluid::landr::PolygonGraph* Copy = new openfluid::landr::PolygonGraph(
      *Graph);

  BOOST_CHECK_EQUAL(Graph->getSize(), Copy->getSize());
  BOOST_CHECK_EQUAL(Graph->getEdges()->size(), Copy->getEdges()->size());
  BOOST_CHECK_EQUAL(Graph->getEntities().size(), Copy->getEntities().size());

  BOOST_CHECK(Graph->isComplete());

  delete Graph;
  delete Copy;
  delete Val;
}

// =====================================================================
// =====================================================================

BOOST_AUTO_TEST_CASE(check_getEntity)
{
  openfluid::core::GeoVectorValue* Val = new openfluid::core::GeoVectorValue(
      CONFIGTESTS_INPUT_DATASETS_DIR + "/landr", "SU.shp");

  openfluid::landr::PolygonGraph* Graph = new openfluid::landr::PolygonGraph(
      *Val);

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

  openfluid::landr::PolygonGraph* Graph = new openfluid::landr::PolygonGraph(
      *Val);

  openfluid::landr::PolygonEntity* U1 = Graph->getEntity(1);
  openfluid::landr::PolygonEntity* U2 = Graph->getEntity(2);

  boost::any IntValue = 0;
  boost::any StrValue = std::string("");

  BOOST_CHECK(!U1->setAttributeValue("att",123));
  BOOST_CHECK(!U2->setAttributeValue("att",std::string("val")));
  BOOST_CHECK(!U1->getAttributeValue("att",IntValue));
  BOOST_CHECK(!U2->getAttributeValue("att",StrValue));
  BOOST_CHECK_EQUAL(boost::any_cast<int>(IntValue), 0);
  BOOST_CHECK_EQUAL(boost::any_cast<std::string>(StrValue), "");

  Graph->addAttribute("att");

  BOOST_CHECK(U1->setAttributeValue("att",123));
  BOOST_CHECK(U2->setAttributeValue("att",std::string("val")));
  BOOST_CHECK(U1->getAttributeValue("att",IntValue));
  BOOST_CHECK(U2->getAttributeValue("att",StrValue));
  BOOST_CHECK_EQUAL(boost::any_cast<int>(IntValue), 123);
  BOOST_CHECK_EQUAL(boost::any_cast<std::string>(StrValue), "val");

  Graph->addAttribute("att");

  IntValue = 0;
  StrValue = std::string("");
  BOOST_CHECK(U1->getAttributeValue("att",IntValue));
  BOOST_CHECK(U2->getAttributeValue("att",StrValue));
  BOOST_CHECK_EQUAL(boost::any_cast<int>(IntValue), 123);
  BOOST_CHECK_EQUAL(boost::any_cast<std::string>(StrValue), "val");

  Graph->removeAttribute("att");

  IntValue = 0;
  StrValue = std::string("");
  BOOST_CHECK(!U1->setAttributeValue("att",123));
  BOOST_CHECK(!U2->setAttributeValue("att",std::string("val")));
  BOOST_CHECK(!U1->getAttributeValue("att",IntValue));
  BOOST_CHECK(!U2->getAttributeValue("att",StrValue));
  BOOST_CHECK_EQUAL(boost::any_cast<int>(IntValue), 0);
  BOOST_CHECK_EQUAL(boost::any_cast<std::string>(StrValue), "");

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

  openfluid::landr::PolygonGraph* Graph = new openfluid::landr::PolygonGraph(
      *Val);

  std::vector<openfluid::landr::PolygonEntity*> OrderedEntities =
      Graph->getSelfIdOrderedEntities();

  for (unsigned int i = 0; i < OrderedEntities.size(); i++)
    BOOST_CHECK_EQUAL(OrderedEntities.at(i)->getSelfId(), i+1);

  delete Graph;
  delete Val;
}

// =====================================================================
// =====================================================================

BOOST_AUTO_TEST_CASE(check_MergesWith1ResultLine)
{
  geos::geom::CoordinateArraySequenceFactory SeqFactory;
  geos::geom::GeometryFactory Factory;

  std::vector<geos::geom::Geometry*> Geoms;

  std::vector<geos::geom::Coordinate> Coos1;
  Coos1.push_back(geos::geom::Coordinate(0, 1));
  Coos1.push_back(geos::geom::Coordinate(0, 2));
  geos::geom::LineString* LS1 = Factory.createLineString(
      SeqFactory.create(&Coos1));
  Geoms.push_back(LS1);

  std::vector<geos::geom::Coordinate> Coos0;
  Coos0.push_back(geos::geom::Coordinate(0, 0));
  Coos0.push_back(geos::geom::Coordinate(0, 1));
  geos::geom::LineString* LS2 = Factory.createLineString(
      SeqFactory.create(&Coos0));
  Geoms.push_back(LS2);

  geos::geom::MultiLineString* MLS = Factory.createMultiLineString(Geoms);

  BOOST_CHECK_EQUAL(MLS->getNumGeometries(), 2);

  std::vector<geos::geom::LineString*>* MergedLSVect =
      openfluid::landr::PolygonGraph::getMergedLineStringsFromGeometry(MLS);

  BOOST_CHECK_EQUAL(MergedLSVect->size(), 1);

  geos::geom::LineString* MergedLS =
      openfluid::landr::PolygonGraph::getMergedLineStringFromGeometry(MLS);

  BOOST_CHECK_EQUAL(MergedLS->getNumPoints(), 3);

  BOOST_CHECK(MergedLS->getCoordinateN(0).equals(geos::geom::Coordinate(0, 0)));
  BOOST_CHECK(MergedLS->getCoordinateN(2).equals(geos::geom::Coordinate(0, 2)));

  delete MLS;
  delete MergedLSVect;
  delete MergedLS;
}

// =====================================================================
// =====================================================================

BOOST_AUTO_TEST_CASE(check_MergesWith2ResultLines)
{
  geos::geom::CoordinateArraySequenceFactory SeqFactory;
  geos::geom::GeometryFactory Factory;

  std::vector<geos::geom::Geometry*> Geoms;

  std::vector<geos::geom::Coordinate> Coos1;
  Coos1.push_back(geos::geom::Coordinate(0, 1));
  Coos1.push_back(geos::geom::Coordinate(0, 2));
  geos::geom::LineString* LS1 = Factory.createLineString(
      SeqFactory.create(&Coos1));
  Geoms.push_back(LS1);

  std::vector<geos::geom::Coordinate> Coos4;
  Coos4.push_back(geos::geom::Coordinate(0, 4));
  Coos4.push_back(geos::geom::Coordinate(0, 5));
  geos::geom::LineString* LS4 = Factory.createLineString(
      SeqFactory.create(&Coos4));
  Geoms.push_back(LS4);

  std::vector<geos::geom::Coordinate> Coos0;
  Coos0.push_back(geos::geom::Coordinate(0, 0));
  Coos0.push_back(geos::geom::Coordinate(0, 1));
  geos::geom::LineString* LS2 = Factory.createLineString(
      SeqFactory.create(&Coos0));
  Geoms.push_back(LS2);

  std::vector<geos::geom::Coordinate> Coos3;
  Coos3.push_back(geos::geom::Coordinate(0, 3));
  Coos3.push_back(geos::geom::Coordinate(0, 4));
  geos::geom::LineString* LS3 = Factory.createLineString(
      SeqFactory.create(&Coos3));
  Geoms.push_back(LS3);

  geos::geom::MultiLineString* MLS = Factory.createMultiLineString(Geoms);

  BOOST_CHECK_EQUAL(MLS->getNumGeometries(), 4);

  std::vector<geos::geom::LineString*>* MergedLSVect =
      openfluid::landr::PolygonGraph::getMergedLineStringsFromGeometry(MLS);

  BOOST_CHECK_EQUAL(MergedLSVect->size(), 2);

  geos::geom::LineString* MergedLS1 = MergedLSVect->at(0);
  geos::geom::LineString* MergedLS2 = MergedLSVect->at(1);

  BOOST_CHECK_EQUAL(MergedLS1->getNumPoints(), 3);
  BOOST_CHECK_EQUAL(MergedLS2->getNumPoints(), 3);

  BOOST_CHECK(
      MergedLS1->getCoordinateN(0).equals(geos::geom::Coordinate(0, 0)));
  BOOST_CHECK(
      MergedLS1->getCoordinateN(2).equals(geos::geom::Coordinate(0, 2)));

  BOOST_CHECK(
      MergedLS2->getCoordinateN(0).equals(geos::geom::Coordinate(0, 3)));
  BOOST_CHECK(
      MergedLS2->getCoordinateN(2).equals(geos::geom::Coordinate(0, 5)));

  geos::geom::LineString* MergedLS =
      openfluid::landr::PolygonGraph::getMergedLineStringFromGeometry(MLS);

  BOOST_CHECK(!MergedLS);

  delete MLS;
  delete MergedLSVect;
  delete MergedLS1;
  delete MergedLS2;
}

// =====================================================================
// =====================================================================

BOOST_AUTO_TEST_CASE(check_getARasterValue)
{
  openfluid::core::GeoVectorValue* Vector = new openfluid::core::GeoVectorValue(
      CONFIGTESTS_INPUT_DATASETS_DIR + "/landr", "SU.shp");

  openfluid::landr::PolygonGraph* Graph = new openfluid::landr::PolygonGraph(
      *Vector);

  openfluid::core::GeoRasterValue* Raster = new openfluid::core::GeoRasterValue(
      CONFIGTESTS_INPUT_DATASETS_DIR + "/GeoRasterValue", "dem.jpeg");

  openfluid::landr::PolygonEntity* U1 = Graph->getEntity(1);
  openfluid::landr::PolygonEntity* U20 = Graph->getEntity(20);

  BOOST_CHECK_THROW(Graph->getRasterValueForEntityCentroid(*U1),
                    openfluid::base::OFException);

  Graph->addAGeoRasterValue(*Raster);

  BOOST_CHECK_EQUAL(*Graph->getRasterValueForEntityCentroid(*U1), 29);
  BOOST_CHECK_EQUAL(*Graph->getRasterValueForEntityCentroid(*U20), 64);

  delete Graph;
  delete Vector;
  delete Raster;
}

// =====================================================================
// =====================================================================

BOOST_AUTO_TEST_CASE(check_getRasterPolygonized)
{
  openfluid::core::GeoVectorValue* Vector = new openfluid::core::GeoVectorValue(
      CONFIGTESTS_INPUT_DATASETS_DIR + "/landr", "SU.shp");

  openfluid::core::GeoRasterValue* Raster = new openfluid::core::GeoRasterValue(
      CONFIGTESTS_INPUT_DATASETS_DIR + "/GeoRasterValue", "dem.jpeg");

  openfluid::landr::PolygonGraph* Graph = new openfluid::landr::PolygonGraph(
      *Vector);

  BOOST_CHECK_THROW(Graph->getRasterPolygonized(),
                    openfluid::base::OFException);

  Graph->addAGeoRasterValue(*Raster);

  openfluid::core::GeoVectorValue* Polygonized = Graph->getRasterPolygonized();

  BOOST_CHECK(Polygonized);
  BOOST_CHECK_EQUAL(Polygonized->getLayer0()->GetFeatureCount(), 234);

  BOOST_CHECK_EQUAL(
      Polygonized->getLayer0()->GetFeature(0)->GetFieldAsInteger(openfluid::core::GeoRasterValue::getDefaultPolygonizedFieldName().c_str()),
      96);
  BOOST_CHECK_EQUAL(
      Polygonized->getLayer0()->GetFeature(76)->GetFieldAsInteger(openfluid::core::GeoRasterValue::getDefaultPolygonizedFieldName().c_str()),
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
      CONFIGTESTS_INPUT_DATASETS_DIR + "/landr", "SU.shp");

  openfluid::core::GeoRasterValue* Raster = new openfluid::core::GeoRasterValue(
      CONFIGTESTS_INPUT_DATASETS_DIR + "/GeoRasterValue", "dem.jpeg");

  openfluid::landr::PolygonGraph* Graph = new openfluid::landr::PolygonGraph(
      *Vector);

  BOOST_CHECK_THROW(Graph->getRasterPolygonizedPolys(),
                    openfluid::base::OFException);

  Graph->addAGeoRasterValue(*Raster);

  BOOST_CHECK_EQUAL(Graph->getRasterPolygonizedPolys()->size(), 234);

  delete Graph;
  delete Vector;
  delete Raster;
}

// =====================================================================
// =====================================================================

BOOST_AUTO_TEST_CASE(check_getRasterPolyOverlapping_gettingPolygonsOnly)
{
  openfluid::core::GeoVectorValue* Vector = new openfluid::core::GeoVectorValue(
      CONFIGTESTS_INPUT_DATASETS_DIR + "/landr", "SU.shp");

  openfluid::core::GeoRasterValue* Raster = new openfluid::core::GeoRasterValue(
      CONFIGTESTS_INPUT_DATASETS_DIR + "/GeoRasterValue", "dem.jpeg");

  openfluid::landr::PolygonGraph* Graph = new openfluid::landr::PolygonGraph(
      *Vector);

  openfluid::landr::PolygonEntity* U1 = Graph->getEntity(1);

  BOOST_CHECK_THROW(Graph->getRasterPolyOverlapping(*U1),
                    openfluid::base::OFException);

  Graph->addAGeoRasterValue(*Raster);

  openfluid::landr::PolygonGraph::RastValByRastPoly_t OverlapsU1 =
      Graph->getRasterPolyOverlapping(*U1);

  BOOST_CHECK_EQUAL(OverlapsU1.size(), 12);

  double Area;

  for (openfluid::landr::PolygonGraph::RastValByRastPoly_t::iterator it =
      OverlapsU1.begin(); it != OverlapsU1.end(); ++it)
  {
    Area += it->second;

    BOOST_CHECK((int*)it->first->getUserData());
  }

  BOOST_CHECK(openfluid::tools::IsVeryClose(Area,U1->getArea()));

  delete Graph;
  delete Vector;
  delete Raster;
}

// =====================================================================
// =====================================================================

BOOST_AUTO_TEST_CASE(check_getRasterPolyOverlapping_gettingAlsoMultiPolygon)
{
  openfluid::core::GeoVectorValue* Vector = new openfluid::core::GeoVectorValue(
      CONFIGTESTS_INPUT_DATASETS_DIR + "/landr", "SU.shp");

  openfluid::core::GeoRasterValue* Raster = new openfluid::core::GeoRasterValue(
      CONFIGTESTS_INPUT_DATASETS_DIR + "/GeoRasterValue", "dem.jpeg");

  openfluid::landr::PolygonGraph* Graph = new openfluid::landr::PolygonGraph(
      *Vector);

  Graph->addAGeoRasterValue(*Raster);

  openfluid::landr::PolygonEntity* U2 = Graph->getEntity(2);

  openfluid::landr::PolygonGraph::RastValByRastPoly_t OverlapsU2 =
      Graph->getRasterPolyOverlapping(*U2);

  BOOST_CHECK_EQUAL(OverlapsU2.size(), 20);

  double Area = 0;

  for (openfluid::landr::PolygonGraph::RastValByRastPoly_t::iterator it =
      OverlapsU2.begin(); it != OverlapsU2.end(); ++it)
  {
    Area += it->second;

    BOOST_CHECK((int*)it->first->getUserData());
  }

  BOOST_CHECK(openfluid::tools::IsVeryClose(Area,U2->getArea()));

  delete Graph;
  delete Vector;
  delete Raster;
}

// =====================================================================
// =====================================================================

BOOST_AUTO_TEST_CASE(check_setAttributeFromRasterValueAtCentroid_intPixelType)
{
  openfluid::core::GeoVectorValue* Vector = new openfluid::core::GeoVectorValue(
      CONFIGTESTS_INPUT_DATASETS_DIR + "/landr", "SU.shp");

  openfluid::core::GeoRasterValue* Raster = new openfluid::core::GeoRasterValue(
      CONFIGTESTS_INPUT_DATASETS_DIR + "/GeoRasterValue", "dem.jpeg");

  openfluid::landr::PolygonGraph* Graph = new openfluid::landr::PolygonGraph(
      *Vector);

  Graph->addAGeoRasterValue(*Raster);

  Graph->setAttributeFromRasterValueAtCentroid("test_val");

  boost::any Val;

  Graph->getEntity(1)->getAttributeValue("test_val", Val);
  BOOST_CHECK_EQUAL(boost::any_cast<float>(Val), 29);

  Graph->getEntity(2)->getAttributeValue("test_val", Val);
  BOOST_CHECK_EQUAL(boost::any_cast<float>(Val), 47);

  delete Graph;
  delete Vector;
  delete Raster;
}

// =====================================================================
// =====================================================================

BOOST_AUTO_TEST_CASE(check_setAttributeFromRasterValueAtCentroid_float32PixelType)
{
  openfluid::core::GeoVectorValue* Vector = new openfluid::core::GeoVectorValue(
      CONFIGTESTS_INPUT_DATASETS_DIR + "/landr", "SU.shp");

  openfluid::core::GeoRasterValue* Raster = new openfluid::core::GeoRasterValue(
      CONFIGTESTS_INPUT_DATASETS_DIR + "/GeoRasterValue", "dem.asc");

  openfluid::landr::PolygonGraph* Graph = new openfluid::landr::PolygonGraph(
      *Vector);

  Graph->addAGeoRasterValue(*Raster);

  Graph->setAttributeFromRasterValueAtCentroid("test_val");

  boost::any Val;

  Graph->getEntity(1)->getAttributeValue("test_val", Val);
  BOOST_CHECK(
      openfluid::tools::IsVeryClose(boost::any_cast<float>(Val), 32.9131));

  Graph->getEntity(2)->getAttributeValue("test_val", Val);
  BOOST_CHECK(
      openfluid::tools::IsVeryClose(boost::any_cast<float>(Val), 51.0607));

  delete Graph;
  delete Vector;
  delete Raster;
}

// =====================================================================
// =====================================================================

BOOST_AUTO_TEST_CASE(check_setAttributeFromRasterValueAtCentroid_float64PixelType)
{
  openfluid::core::GeoVectorValue* Vector = new openfluid::core::GeoVectorValue(
      CONFIGTESTS_INPUT_DATASETS_DIR + "/landr", "SU.shp");

  openfluid::core::GeoRasterValue* Raster = new openfluid::core::GeoRasterValue(
      CONFIGTESTS_INPUT_DATASETS_DIR + "/GeoRasterValue", "dem.Gtiff");

  openfluid::landr::PolygonGraph* Graph = new openfluid::landr::PolygonGraph(
      *Vector);

  Graph->addAGeoRasterValue(*Raster);

  Graph->setAttributeFromRasterValueAtCentroid("test_val");

  boost::any Val;

  Graph->getEntity(1)->getAttributeValue("test_val", Val);
  BOOST_CHECK(
      openfluid::tools::IsVeryClose(boost::any_cast<float>(Val), 32.9131));

  Graph->getEntity(2)->getAttributeValue("test_val", Val);
  BOOST_CHECK(
      openfluid::tools::IsVeryClose(boost::any_cast<float>(Val), 51.0607));

  delete Graph;
  delete Vector;
  delete Raster;
}

// =====================================================================
// =====================================================================

BOOST_AUTO_TEST_CASE(check_setAttributeFromMeanRasterValues_intPixelType)
{
  openfluid::core::GeoVectorValue* Vector = new openfluid::core::GeoVectorValue(
      CONFIGTESTS_INPUT_DATASETS_DIR + "/landr", "SU.shp");

  openfluid::core::GeoRasterValue* Raster = new openfluid::core::GeoRasterValue(
      CONFIGTESTS_INPUT_DATASETS_DIR + "/GeoRasterValue", "dem.jpeg");

  openfluid::landr::PolygonGraph* Graph = new openfluid::landr::PolygonGraph(
      *Vector);

  Graph->addAGeoRasterValue(*Raster);

  Graph->setAttributeFromMeanRasterValues("test_val");

  boost::any Val;

  Graph->getEntity(1)->getAttributeValue("test_val", Val);
  BOOST_CHECK(
      openfluid::tools::IsVeryClose(boost::any_cast<float>(Val), 34.0569));

  Graph->getEntity(2)->getAttributeValue("test_val", Val);
  BOOST_CHECK(
      openfluid::tools::IsVeryClose(boost::any_cast<float>(Val), 46.6497));

  delete Graph;
  delete Vector;
  delete Raster;
}

// =====================================================================
// =====================================================================

BOOST_AUTO_TEST_CASE(check_setAttributeFromMeanRasterValues_float32PixelType)
{
  openfluid::core::GeoVectorValue* Vector = new openfluid::core::GeoVectorValue(
      CONFIGTESTS_INPUT_DATASETS_DIR + "/landr", "SU.shp");

  openfluid::core::GeoRasterValue* Raster = new openfluid::core::GeoRasterValue(
      CONFIGTESTS_INPUT_DATASETS_DIR + "/GeoRasterValue", "dem.asc");

  openfluid::landr::PolygonGraph* Graph = new openfluid::landr::PolygonGraph(
      *Vector);

  Graph->addAGeoRasterValue(*Raster);

  Graph->setAttributeFromMeanRasterValues("test_val");

  boost::any Val;

  Graph->getEntity(1)->getAttributeValue("test_val", Val);
  BOOST_CHECK(
      openfluid::tools::IsVeryClose(boost::any_cast<float>(Val), 33.5981));

  Graph->getEntity(2)->getAttributeValue("test_val", Val);
  BOOST_CHECK(
      openfluid::tools::IsVeryClose(boost::any_cast<float>(Val), 46.7352));

  delete Graph;
  delete Vector;
  delete Raster;
}

// =====================================================================
// =====================================================================

BOOST_AUTO_TEST_CASE(check_setAttributeFromMeanRasterValues_float64PixelType)
{
  openfluid::core::GeoVectorValue* Vector = new openfluid::core::GeoVectorValue(
      CONFIGTESTS_INPUT_DATASETS_DIR + "/landr", "SU.shp");

  openfluid::core::GeoRasterValue* Raster = new openfluid::core::GeoRasterValue(
      CONFIGTESTS_INPUT_DATASETS_DIR + "/GeoRasterValue", "dem.Gtiff");

  openfluid::landr::PolygonGraph* Graph = new openfluid::landr::PolygonGraph(
      *Vector);

  Graph->addAGeoRasterValue(*Raster);

  Graph->setAttributeFromMeanRasterValues("test_val");

  boost::any Val;

  Graph->getEntity(1)->getAttributeValue("test_val", Val);
  BOOST_CHECK(
      openfluid::tools::IsVeryClose(boost::any_cast<float>(Val), 33.5981));

  Graph->getEntity(2)->getAttributeValue("test_val", Val);
  BOOST_CHECK(
      openfluid::tools::IsVeryClose(boost::any_cast<float>(Val), 46.7352));

  delete Graph;
  delete Vector;
  delete Raster;
}

// =====================================================================
// =====================================================================

BOOST_AUTO_TEST_CASE(check_createVectorRepresentation)
{
  openfluid::core::GeoVectorValue* Val = new openfluid::core::GeoVectorValue(
      CONFIGTESTS_INPUT_DATASETS_DIR + "/landr", "SU.shp");

  openfluid::landr::PolygonGraph* Graph = new openfluid::landr::PolygonGraph(
      *Val);

  Graph->createVectorRepresentation(CONFIGTESTS_INPUT_DATASETS_DIR + "/landr",
                                    "SUGraph.shp");

  openfluid::core::GeoVectorValue* GraphVal =
      new openfluid::core::GeoVectorValue(
          CONFIGTESTS_INPUT_DATASETS_DIR + "/landr", "SUGraph.shp");

  BOOST_CHECK_EQUAL(Graph->getEdges()->size(), 58);
  BOOST_CHECK_EQUAL(GraphVal->getLayer0()->GetFeatureCount(), 58);

  GraphVal->deleteShpOnDisk();
  delete GraphVal;
  delete Graph;
  delete Val;
}

// =====================================================================
// =====================================================================
