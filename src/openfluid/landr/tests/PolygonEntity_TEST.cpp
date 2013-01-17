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
 \file PolygonEntity_TEST.cpp
 \brief Implements ...

 \author Aline LIBRES <aline.libres@gmail.com>
 */

#define BOOST_TEST_MAIN
#define BOOST_AUTO_TEST_MAIN
#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MODULE unittest_polygonentity
#include <boost/test/unit_test.hpp>
#include <boost/test/auto_unit_test.hpp>
#include <boost/filesystem/path.hpp>
#include <tests-config.hpp>
#include <openfluid/base/OFException.hpp>
#include <openfluid/core/GeoVectorValue.hpp>
#include <openfluid/landr/PolygonEntity.hpp>
#include <openfluid/landr/PolygonGraph.hpp>
#include <openfluid/landr/LineStringGraph.hpp>
#include <openfluid/landr/PolygonEdge.hpp>
#include <geos/planargraph/Node.h>
#include <geos/geom/Geometry.h>
#include <geos/geom/LineString.h>
#include <geos/geom/Polygon.h>
#include <geos/geom/Point.h>
#include <geos/geom/CoordinateArraySequenceFactory.h>
#include <geos/geom/GeometryFactory.h>

// =====================================================================
// =====================================================================

BOOST_AUTO_TEST_CASE(check_construction)
{
  openfluid::core::GeoVectorValue* Val = new openfluid::core::GeoVectorValue(
      CONFIGTESTS_INPUT_DATASETS_DIR + "/landr", "SU.shp");

  OGRFeature* FirstFeature = Val->getLayer0()->GetFeature(0);

  OGRGeometry* OGRGeom = FirstFeature->GetGeometryRef();

  geos::geom::Geometry* GeosGeom =
      (geos::geom::Geometry*) OGRGeom->exportToGEOS();

  openfluid::landr::PolygonEntity* Entity = new openfluid::landr::PolygonEntity(
      dynamic_cast<geos::geom::Polygon*>(GeosGeom->clone()),
      FirstFeature->GetFieldAsInteger("SELF_ID"));

  BOOST_CHECK_EQUAL(Val->getType(),
                    openfluid::core::UnstructuredValue::GeoVectorValue);

  BOOST_CHECK(Entity->getPolygon()->equals(GeosGeom));

  BOOST_CHECK_EQUAL(Entity->getSelfId(), 1);

  OGRFeature::DestroyFeature(FirstFeature);
  delete Entity;
  delete Val;
}

// =====================================================================
// =====================================================================

BOOST_AUTO_TEST_CASE(check_clone)
{
  openfluid::core::GeoVectorValue* Val = new openfluid::core::GeoVectorValue(
      CONFIGTESTS_INPUT_DATASETS_DIR + "/landr", "SU.shp");

  OGRFeature* FirstFeature = Val->getLayer0()->GetFeature(0);
  OGRGeometry* OGRGeom = FirstFeature->GetGeometryRef();

  geos::geom::Geometry* GeosGeom =
      (geos::geom::Geometry*) OGRGeom->exportToGEOS();

  openfluid::landr::PolygonEntity* Entity = new openfluid::landr::PolygonEntity(
      dynamic_cast<geos::geom::Polygon*>(GeosGeom->clone()),
      FirstFeature->GetFieldAsInteger("SELF_ID"));

  OGRFeature::DestroyFeature(FirstFeature);
  delete GeosGeom;
  delete Val;

  openfluid::landr::PolygonEntity* CopyEntity = Entity->clone();

  BOOST_CHECK(Entity->getPolygon()->equals(CopyEntity->getPolygon()));
  BOOST_CHECK_EQUAL(Entity->getSelfId(), CopyEntity->getSelfId());

  std::string EntityPolyStr = Entity->getPolygon()->toString();

  delete Entity;

  std::string CopyEntityPolyStr = CopyEntity->getPolygon()->toString();

  BOOST_CHECK_EQUAL(EntityPolyStr, CopyEntityPolyStr);

  delete CopyEntity;
}

// =====================================================================
// =====================================================================

BOOST_AUTO_TEST_CASE(check_neighbours)
{
  openfluid::core::GeoVectorValue* Val = new openfluid::core::GeoVectorValue(
      CONFIGTESTS_INPUT_DATASETS_DIR + "/landr", "SU.shp");

  openfluid::landr::PolygonGraph* Graph =
      openfluid::landr::PolygonGraph::create(*Val);

  openfluid::landr::PolygonEntity* U2 = Graph->getEntity(2);
  openfluid::landr::PolygonEntity* U18 = Graph->getEntity(18);
  openfluid::landr::PolygonEntity* U24 = Graph->getEntity(24);

  std::vector<int> NeighbourIds;
  std::vector<int>::iterator it;

  NeighbourIds = U2->getOrderedNeighbourSelfIds();
  it = NeighbourIds.begin();

  BOOST_CHECK_EQUAL(NeighbourIds.size(), 5);
  BOOST_CHECK_EQUAL(*it, 1);
  BOOST_CHECK_EQUAL(*(++it), 3);
  BOOST_CHECK_EQUAL(*(++it), 4);
  BOOST_CHECK_EQUAL(*(++it), 5);
  BOOST_CHECK_EQUAL(*(++it), 23);

  NeighbourIds = U18->getOrderedNeighbourSelfIds();
  it = NeighbourIds.begin();

  BOOST_CHECK_EQUAL(NeighbourIds.size(), 4);
  BOOST_CHECK_EQUAL(*it, 4);
  BOOST_CHECK_EQUAL(*(++it), 17);
  BOOST_CHECK_EQUAL(*(++it), 19);
  BOOST_CHECK_EQUAL(*(++it), 20);

  NeighbourIds = U24->getOrderedNeighbourSelfIds();
  it = NeighbourIds.begin();

  BOOST_CHECK_EQUAL(NeighbourIds.size(), 3);
  BOOST_CHECK_EQUAL(*it, 1);
  BOOST_CHECK_EQUAL(*(++it), 22);
  BOOST_CHECK_EQUAL(*(++it), 23);

  delete Graph;
  delete Val;
}

// =====================================================================
// =====================================================================

BOOST_AUTO_TEST_CASE(check_OneLineIntersection)
{
  // * * * * *
  // *       *
  // *   1   * * *
  // *       * 2 *
  // * * * * * * *

  geos::geom::CoordinateArraySequenceFactory SeqFactory;
  geos::geom::GeometryFactory Factory;

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
  openfluid::landr::PolygonEntity Ent1(P1, 0);

  std::vector<geos::geom::Coordinate>* Coos2 = new std::vector<
      geos::geom::Coordinate>();
  Coos2->push_back(geos::geom::Coordinate(2, 0));
  Coos2->push_back(geos::geom::Coordinate(2, 1));
  Coos2->push_back(geos::geom::Coordinate(3, 1));
  Coos2->push_back(geos::geom::Coordinate(3, 0));
  Coos2->push_back(geos::geom::Coordinate(2, 0));
  geos::geom::LinearRing* LR2 = Factory.createLinearRing(
      SeqFactory.create(Coos2));
  geos::geom::Polygon* P2 = Factory.createPolygon(LR2, NULL);
  openfluid::landr::PolygonEntity Ent2(P2, 0);

  geos::geom::LineString* Shared = *Ent1.getLineIntersectionsWith(Ent2).begin();

  BOOST_CHECK(Shared->getCoordinateN(0).equals(geos::geom::Coordinate(2, 1)));
  BOOST_CHECK(Shared->getCoordinateN(1).equals(geos::geom::Coordinate(2, 0)));

  delete Shared;
}

// =====================================================================
// =====================================================================

BOOST_AUTO_TEST_CASE(check_NoLineIntersection)
{
  //         * * *
  //         * 2 *
  // * * * * * * *
  // *       *
  // *   1   *
  // *       *
  // * * * * *

  geos::geom::CoordinateArraySequenceFactory SeqFactory;
  geos::geom::GeometryFactory Factory;

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
  openfluid::landr::PolygonEntity Ent1(P1, 0);

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
  openfluid::landr::PolygonEntity Ent2(P2, 0);

  BOOST_CHECK_EQUAL(Ent1.getLineIntersectionsWith(Ent2).size(), 0);
}

// =====================================================================
// =====================================================================

BOOST_AUTO_TEST_CASE(check_addRemoveEdge_isComplete)
{
  // X*******X
  // *   1   *
  // * 3   2 *
  // *       *
  // X********

  geos::geom::CoordinateArraySequenceFactory SeqFactory;
  geos::geom::GeometryFactory Factory;

  std::vector<geos::geom::Coordinate>* CoosLR = new std::vector<
      geos::geom::Coordinate>();
  CoosLR->push_back(geos::geom::Coordinate(0, 0));
  CoosLR->push_back(geos::geom::Coordinate(0, 2));
  CoosLR->push_back(geos::geom::Coordinate(2, 2));
  CoosLR->push_back(geos::geom::Coordinate(2, 0));
  CoosLR->push_back(geos::geom::Coordinate(0, 0));
  geos::geom::LinearRing* LR = Factory.createLinearRing(
      SeqFactory.create(CoosLR));
  geos::geom::Polygon* P = Factory.createPolygon(LR, NULL);
  openfluid::landr::PolygonEntity Entity(P, 0);

  std::vector<geos::geom::Coordinate>* Coos1 = new std::vector<
      geos::geom::Coordinate>();
  Coos1->push_back(geos::geom::Coordinate(0, 2));
  Coos1->push_back(geos::geom::Coordinate(2, 2));
  geos::geom::LineString* LS1 = Factory.createLineString(
      SeqFactory.create(Coos1));
  openfluid::landr::PolygonEdge* E1 = new openfluid::landr::PolygonEdge(*LS1);
  Entity.addEdge(*E1);

  std::vector<geos::geom::Coordinate>* Coos2 = new std::vector<
      geos::geom::Coordinate>();
  Coos2->push_back(geos::geom::Coordinate(0, 0));
  Coos2->push_back(geos::geom::Coordinate(2, 0));
  Coos2->push_back(geos::geom::Coordinate(2, 2));
  geos::geom::LineString* LS2 = Factory.createLineString(
      SeqFactory.create(Coos2));
  openfluid::landr::PolygonEdge* E2 = new openfluid::landr::PolygonEdge(*LS2);
  Entity.addEdge(*E2);

  std::vector<geos::geom::Coordinate>* Coos3 = new std::vector<
      geos::geom::Coordinate>();
  Coos3->push_back(geos::geom::Coordinate(0, 0));
  Coos3->push_back(geos::geom::Coordinate(0, 2));
  geos::geom::LineString* LS3 = Factory.createLineString(
      SeqFactory.create(Coos3));
  openfluid::landr::PolygonEdge* E3 = new openfluid::landr::PolygonEdge(*LS3);
  Entity.addEdge(*E3);

  BOOST_CHECK_EQUAL(Entity.m_PolyEdges.size(), 3);
  BOOST_CHECK(Entity.isComplete());

  Entity.removeEdge(E3);

  BOOST_CHECK_EQUAL(Entity.m_PolyEdges.size(), 2);
  BOOST_CHECK(!Entity.isComplete());
}

// =====================================================================
// =====================================================================

BOOST_AUTO_TEST_CASE(check_findEdgeIntersecting)
{
  // X*******X
  // *   1   *
  // * 3   2 **
  // *       **
  // X*********

  geos::geom::CoordinateArraySequenceFactory SeqFactory;
  geos::geom::GeometryFactory Factory;

  std::vector<geos::geom::Coordinate>* CoosLR = new std::vector<
      geos::geom::Coordinate>();
  CoosLR->push_back(geos::geom::Coordinate(0, 0));
  CoosLR->push_back(geos::geom::Coordinate(0, 2));
  CoosLR->push_back(geos::geom::Coordinate(2, 2));
  CoosLR->push_back(geos::geom::Coordinate(2, 0));
  CoosLR->push_back(geos::geom::Coordinate(0, 0));
  geos::geom::LinearRing* LR = Factory.createLinearRing(
      SeqFactory.create(CoosLR));
  geos::geom::Polygon* P = Factory.createPolygon(LR, NULL);
  openfluid::landr::PolygonEntity Entity(P, 0);

  std::vector<geos::geom::Coordinate>* Coos1 = new std::vector<
      geos::geom::Coordinate>();
  Coos1->push_back(geos::geom::Coordinate(0, 2));
  Coos1->push_back(geos::geom::Coordinate(2, 2));
  geos::geom::LineString* LS1 = Factory.createLineString(
      SeqFactory.create(Coos1));
  openfluid::landr::PolygonEdge* E1 = new openfluid::landr::PolygonEdge(*LS1);
  Entity.addEdge(*E1);

  std::vector<geos::geom::Coordinate>* Coos2 = new std::vector<
      geos::geom::Coordinate>();
  Coos2->push_back(geos::geom::Coordinate(0, 0));
  Coos2->push_back(geos::geom::Coordinate(2, 0));
  Coos2->push_back(geos::geom::Coordinate(2, 2));
  geos::geom::LineString* LS2 = Factory.createLineString(
      SeqFactory.create(Coos2));
  openfluid::landr::PolygonEdge* E2 = new openfluid::landr::PolygonEdge(*LS2);
  Entity.addEdge(*E2);

  std::vector<geos::geom::Coordinate>* Coos3 = new std::vector<
      geos::geom::Coordinate>();
  Coos3->push_back(geos::geom::Coordinate(0, 0));
  Coos3->push_back(geos::geom::Coordinate(0, 2));
  geos::geom::LineString* LS3 = Factory.createLineString(
      SeqFactory.create(Coos3));
  openfluid::landr::PolygonEdge* E3 = new openfluid::landr::PolygonEdge(*LS3);
  Entity.addEdge(*E3);

  std::vector<geos::geom::Coordinate>* CooLS = new std::vector<
      geos::geom::Coordinate>();
  CooLS->push_back(geos::geom::Coordinate(2, 0));
  CooLS->push_back(geos::geom::Coordinate(2, 1));
  geos::geom::LineString* LS = Factory.createLineString(
      SeqFactory.create(CooLS));

  BOOST_CHECK(Entity.findEdgeLineIntersectingWith(*LS) == E2);

  std::vector<geos::geom::Coordinate>* CooWrongLS = new std::vector<
      geos::geom::Coordinate>();
  CooWrongLS->push_back(geos::geom::Coordinate(2, 1));
  CooWrongLS->push_back(geos::geom::Coordinate(3, 1));
  geos::geom::LineString* WrongLS = Factory.createLineString(
      SeqFactory.create(CooWrongLS));

  BOOST_CHECK(!Entity.findEdgeLineIntersectingWith(*WrongLS));

  delete LS;
  delete WrongLS;
}

// =====================================================================
// =====================================================================

BOOST_AUTO_TEST_CASE(check_getCommonEdgesWith)
{
  openfluid::core::GeoVectorValue* Val = new openfluid::core::GeoVectorValue(
      CONFIGTESTS_INPUT_DATASETS_DIR + "/landr", "SU_horseshoe_lines.shp");

  openfluid::landr::PolygonGraph* Graph =
      openfluid::landr::PolygonGraph::create(*Val);

  openfluid::landr::PolygonEntity* U1 = Graph->getEntity(1);
  openfluid::landr::PolygonEntity* U2 = Graph->getEntity(2);
  openfluid::landr::PolygonEntity* U3 = Graph->getEntity(3);

  BOOST_CHECK_EQUAL(U1->getCommonEdgesWith(*U1).size(), 0);
  BOOST_CHECK_EQUAL(U1->getCommonEdgesWith(*U2).size(), 1);
  BOOST_CHECK_EQUAL(U1->getCommonEdgesWith(*U3).size(), 2);

  BOOST_CHECK_EQUAL(U2->getCommonEdgesWith(*U2).size(), 0);
  BOOST_CHECK_EQUAL(U2->getCommonEdgesWith(*U1).size(), 1);
  BOOST_CHECK_EQUAL(U2->getCommonEdgesWith(*U3).size(), 1);

  BOOST_CHECK_EQUAL(U3->getCommonEdgesWith(*U3).size(), 0);
  BOOST_CHECK_EQUAL(U3->getCommonEdgesWith(*U1).size(), 2);
  BOOST_CHECK_EQUAL(U3->getCommonEdgesWith(*U2).size(), 1);

  BOOST_CHECK(U1->getCommonEdgesWith(*U2)[0] == U2->getCommonEdgesWith(*U1)[0]);
  BOOST_CHECK(U2->getCommonEdgesWith(*U3)[0] == U3->getCommonEdgesWith(*U2)[0]);

  delete Graph;
  delete Val;
}

// =====================================================================
// =====================================================================

BOOST_AUTO_TEST_CASE(check_getNeighbour_MinDistCentroCentro)
{
  openfluid::core::GeoVectorValue* Val = new openfluid::core::GeoVectorValue(
      CONFIGTESTS_INPUT_DATASETS_DIR + "/landr", "SU.shp");

  openfluid::landr::PolygonGraph* Graph =
      openfluid::landr::PolygonGraph::create(*Val);

  BOOST_CHECK_EQUAL(Graph->getEntity(3)->getNeighbour_MinDistCentroCentro(),
                    Graph->getEntity(2));
  BOOST_CHECK_EQUAL(Graph->getEntity(1)->getNeighbour_MinDistCentroCentro(),
                    Graph->getEntity(24));
  BOOST_CHECK_EQUAL(Graph->getEntity(13)->getNeighbour_MinDistCentroCentro(),
                    Graph->getEntity(14));
  BOOST_CHECK_EQUAL(Graph->getEntity(12)->getNeighbour_MinDistCentroCentro(),
                    Graph->getEntity(15));

  delete Graph;
  delete Val;
}

// =====================================================================
// =====================================================================

BOOST_AUTO_TEST_CASE(check_computeLineStringNeighbours_Contains)
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
  SU1->computeLineStringNeighbours(*RSGraph,
                                   openfluid::landr::LandRTools::CONTAINS, 0);
  BOOST_CHECK_EQUAL(SU1->getLineStringNeighbours()->size(), 0);
  SU1->computeLineStringNeighbours(*RSGraph,
                                   openfluid::landr::LandRTools::CONTAINS,
                                   0.0001);
  BOOST_CHECK_EQUAL(SU1->getLineStringNeighbours()->size(), 1);
  BOOST_CHECK_EQUAL(SU1->getLineStringNeighbours()->begin()->first->getSelfId(),
                    1);

  BOOST_CHECK(!SU4->getLineStringNeighbours());
  SU4->computeLineStringNeighbours(*RSGraph,
                                   openfluid::landr::LandRTools::CONTAINS, 0);
  BOOST_CHECK_EQUAL(SU4->getLineStringNeighbours()->size(), 0);
  SU4->computeLineStringNeighbours(*RSGraph,
                                   openfluid::landr::LandRTools::CONTAINS,
                                   0.0001);
  BOOST_CHECK_EQUAL(SU4->getLineStringNeighbours()->size(), 1);
  BOOST_CHECK_EQUAL(SU4->getLineStringNeighbours()->begin()->first->getSelfId(),
                    3);

  BOOST_CHECK(!SU17->getLineStringNeighbours());
  SU17->computeLineStringNeighbours(*RSGraph,
                                    openfluid::landr::LandRTools::CONTAINS, 0);
  BOOST_CHECK_EQUAL(SU17->getLineStringNeighbours()->size(), 0);
  SU17->computeLineStringNeighbours(*RSGraph,
                                    openfluid::landr::LandRTools::CONTAINS,
                                    0.0001);
  BOOST_CHECK_EQUAL(SU17->getLineStringNeighbours()->size(), 1);
  BOOST_CHECK_EQUAL(
      SU17->getLineStringNeighbours()->begin()->first->getSelfId(), 6);

  delete RSGraph;
  delete SUGraph;
  delete ValRS;
  delete ValSU;
}

// =====================================================================
// =====================================================================

BOOST_AUTO_TEST_CASE(check_computeLineStringNeighbours_Intersect)
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
  SU1->computeLineStringNeighbours(*RSGraph,
                                   openfluid::landr::LandRTools::INTERSECTS, 0);
  BOOST_CHECK_EQUAL(SU1->getLineStringNeighbours()->size(), 0);
  SU1->computeLineStringNeighbours(*RSGraph,
                                   openfluid::landr::LandRTools::INTERSECTS,
                                   0.0001);
  BOOST_CHECK_EQUAL(SU1->getLineStringNeighbours()->size(), 2);

  BOOST_CHECK(!SU4->getLineStringNeighbours());
  SU4->computeLineStringNeighbours(*RSGraph,
                                   openfluid::landr::LandRTools::INTERSECTS, 0);
  BOOST_CHECK_EQUAL(SU4->getLineStringNeighbours()->size(), 0);
  SU4->computeLineStringNeighbours(*RSGraph,
                                   openfluid::landr::LandRTools::INTERSECTS,
                                   0.0001);
  BOOST_CHECK_EQUAL(SU4->getLineStringNeighbours()->size(), 6);

  BOOST_CHECK(!SU17->getLineStringNeighbours());
  SU17->computeLineStringNeighbours(*RSGraph,
                                    openfluid::landr::LandRTools::INTERSECTS,
                                    0);
  BOOST_CHECK_EQUAL(SU17->getLineStringNeighbours()->size(), 0);
  SU17->computeLineStringNeighbours(*RSGraph,
                                    openfluid::landr::LandRTools::INTERSECTS,
                                    0.0001);
  BOOST_CHECK_EQUAL(SU17->getLineStringNeighbours()->size(), 3);

  delete RSGraph;
  delete SUGraph;
  delete ValRS;
  delete ValSU;
}
