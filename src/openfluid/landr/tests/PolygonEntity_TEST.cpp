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
  @file PolygonEntity_TEST.cpp

  @author Aline LIBRES <aline.libres@gmail.com>
  @author Michael RABOTIN <michael.rabotin@supagro.inra.fr>
*/

#define BOOST_TEST_NO_MAIN
#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MODULE unittest_polygonentity


#include <boost/test/unit_test.hpp>
#include <boost/test/auto_unit_test.hpp>

#include <geos/planargraph/Node.h>
#include <geos/geom/Geometry.h>
#include <geos/geom/LineString.h>
#include <geos/geom/Polygon.h>
#include <geos/geom/CoordinateArraySequenceFactory.h>
#include <geos/geom/GeometryFactory.h>

#include <openfluid/base/FrameworkException.hpp>
#include <openfluid/base/Environment.hpp>
#include <openfluid/core/GeoVectorValue.hpp>
#include <openfluid/scientific/FloatingPoint.hpp>
#include <openfluid/landr/GEOSHelpers.hpp>
#include <openfluid/landr/PolygonEntity.hpp>
#include <openfluid/landr/PolygonGraph.hpp>
#include <openfluid/landr/LineStringGraph.hpp>
#include <openfluid/landr/PolygonEdge.hpp>
#include <openfluid/landr/VectorDataset.hpp>

#include "tests-config.hpp"


// =====================================================================
// =====================================================================


BOOST_AUTO_TEST_CASE(check_construction)
{
  openfluid::core::GeoVectorValue Val(CONFIGTESTS_INPUT_MISCDATA_DIR + "/landr","SU.shp");

  openfluid::landr::VectorDataset* Vect = new openfluid::landr::VectorDataset(Val);

  OGRFeature* FirstFeature = Vect->layer(0)->GetFeature(0);

  OGRGeometry* OGRGeom = FirstFeature->GetGeometryRef();

  geos::geom::Geometry* GeosGeom = (geos::geom::Geometry*) openfluid::landr::convertOGRGeometryToGEOS(OGRGeom);

  openfluid::landr::PolygonEntity* Entity =
    new openfluid::landr::PolygonEntity(dynamic_cast<geos::geom::Polygon*>(GeosGeom->clone()),
                                        FirstFeature->GetFieldAsInteger("OFLD_ID"));

  BOOST_CHECK_EQUAL(Val.getType(),openfluid::core::UnstructuredValue::GeoVectorValue);

  BOOST_CHECK(Entity->polygon()->equals(GeosGeom));

  BOOST_CHECK_EQUAL(Entity->getOfldId(), 1);

  OGRFeature::DestroyFeature(FirstFeature);
  delete Entity;
  delete Vect;
}


// =====================================================================
// =====================================================================


BOOST_AUTO_TEST_CASE(check_neighbours)
{
  openfluid::core::GeoVectorValue* Val =
    new openfluid::core::GeoVectorValue(CONFIGTESTS_INPUT_MISCDATA_DIR + "/landr", "SU.shp");

  openfluid::landr::PolygonGraph* Graph =  openfluid::landr::PolygonGraph::create(*Val);

  openfluid::landr::PolygonEntity* U2 = Graph->entity(2);
  openfluid::landr::PolygonEntity* U18 = Graph->entity(18);
  openfluid::landr::PolygonEntity* U24 = Graph->entity(24);

  std::vector<int> NeighbourIds;
  std::vector<int>::iterator it;

  NeighbourIds = U2->getOrderedNeighbourOfldIds();
  it = NeighbourIds.begin();

  BOOST_CHECK_EQUAL(NeighbourIds.size(), 5);
  BOOST_CHECK_EQUAL(*it, 1);
  BOOST_CHECK_EQUAL(*(++it), 3);
  BOOST_CHECK_EQUAL(*(++it), 4);
  BOOST_CHECK_EQUAL(*(++it), 5);
  BOOST_CHECK_EQUAL(*(++it), 23);

  NeighbourIds = U18->getOrderedNeighbourOfldIds();
  it = NeighbourIds.begin();

  BOOST_CHECK_EQUAL(NeighbourIds.size(), 4);
  BOOST_CHECK_EQUAL(*it, 4);
  BOOST_CHECK_EQUAL(*(++it), 17);
  BOOST_CHECK_EQUAL(*(++it), 19);
  BOOST_CHECK_EQUAL(*(++it), 20);

  NeighbourIds = U24->getOrderedNeighbourOfldIds();
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
  const geos::geom::GeometryFactory* Factory = geos::geom::GeometryFactory::getDefaultInstance();

  std::vector<geos::geom::Coordinate>* Coos1 = new std::vector<geos::geom::Coordinate>();
  Coos1->push_back(geos::geom::Coordinate(0, 0));
  Coos1->push_back(geos::geom::Coordinate(0, 2));
  Coos1->push_back(geos::geom::Coordinate(2, 2));
  Coos1->push_back(geos::geom::Coordinate(2, 0));
  Coos1->push_back(geos::geom::Coordinate(0, 0));

  geos::geom::LinearRing* LR1 = Factory->createLinearRing(SeqFactory.create(Coos1));
  geos::geom::Polygon* P1 = Factory->createPolygon(LR1, nullptr);
  openfluid::landr::PolygonEntity Ent1(P1, 0);

  std::vector<geos::geom::Coordinate>* Coos2 = new std::vector<geos::geom::Coordinate>();
  Coos2->push_back(geos::geom::Coordinate(2, 0));
  Coos2->push_back(geos::geom::Coordinate(2, 1));
  Coos2->push_back(geos::geom::Coordinate(3, 1));
  Coos2->push_back(geos::geom::Coordinate(3, 0));
  Coos2->push_back(geos::geom::Coordinate(2, 0));

  geos::geom::LinearRing* LR2 = Factory->createLinearRing(SeqFactory.create(Coos2));
  geos::geom::Polygon* P2 = Factory->createPolygon(LR2, nullptr);
  openfluid::landr::PolygonEntity Ent2(P2, 0);

  geos::geom::LineString* Shared = *Ent1.computeLineIntersectionsWith(Ent2).begin();

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
  const geos::geom::GeometryFactory* Factory = geos::geom::GeometryFactory::getDefaultInstance();

  std::vector<geos::geom::Coordinate>* Coos1 = new std::vector<geos::geom::Coordinate>();
  Coos1->push_back(geos::geom::Coordinate(0, 0));
  Coos1->push_back(geos::geom::Coordinate(0, 2));
  Coos1->push_back(geos::geom::Coordinate(2, 2));
  Coos1->push_back(geos::geom::Coordinate(2, 0));
  Coos1->push_back(geos::geom::Coordinate(0, 0));

  geos::geom::LinearRing* LR1 = Factory->createLinearRing(SeqFactory.create(Coos1));
  geos::geom::Polygon* P1 = Factory->createPolygon(LR1, nullptr);
  openfluid::landr::PolygonEntity Ent1(P1, 0);

  std::vector<geos::geom::Coordinate>* Coos2 = new std::vector<geos::geom::Coordinate>();
  Coos2->push_back(geos::geom::Coordinate(2, 3));
  Coos2->push_back(geos::geom::Coordinate(3, 3));
  Coos2->push_back(geos::geom::Coordinate(3, 2));
  Coos2->push_back(geos::geom::Coordinate(2, 2));
  Coos2->push_back(geos::geom::Coordinate(2, 3));
  geos::geom::LinearRing* LR2 = Factory->createLinearRing(SeqFactory.create(Coos2));
  geos::geom::Polygon* P2 = Factory->createPolygon(LR2, nullptr);
  openfluid::landr::PolygonEntity Ent2(P2, 0);

  BOOST_CHECK_EQUAL(Ent1.computeLineIntersectionsWith(Ent2).size(), 0);
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
  const geos::geom::GeometryFactory* Factory = geos::geom::GeometryFactory::getDefaultInstance();

  std::vector<geos::geom::Coordinate>* CoosLR = new std::vector<geos::geom::Coordinate>();
  CoosLR->push_back(geos::geom::Coordinate(0, 0));
  CoosLR->push_back(geos::geom::Coordinate(0, 2));
  CoosLR->push_back(geos::geom::Coordinate(2, 2));
  CoosLR->push_back(geos::geom::Coordinate(2, 0));
  CoosLR->push_back(geos::geom::Coordinate(0, 0));

  geos::geom::LinearRing* LR = Factory->createLinearRing(SeqFactory.create(CoosLR));
  geos::geom::Polygon* P = Factory->createPolygon(LR, nullptr);
  openfluid::landr::PolygonEntity Entity(P, 0);

  std::vector<geos::geom::Coordinate>* Coos1 = new std::vector<geos::geom::Coordinate>();
  Coos1->push_back(geos::geom::Coordinate(0, 2));
  Coos1->push_back(geos::geom::Coordinate(2, 2));
  geos::geom::LineString* LS1 = Factory->createLineString(SeqFactory.create(Coos1));
  openfluid::landr::PolygonEdge* E1 = new openfluid::landr::PolygonEdge(*LS1);
  Entity.addEdge(*E1);

  std::vector<geos::geom::Coordinate>* Coos2 = new std::vector<geos::geom::Coordinate>();
  Coos2->push_back(geos::geom::Coordinate(0, 0));
  Coos2->push_back(geos::geom::Coordinate(2, 0));
  Coos2->push_back(geos::geom::Coordinate(2, 2));
  geos::geom::LineString* LS2 = Factory->createLineString(SeqFactory.create(Coos2));
  openfluid::landr::PolygonEdge* E2 = new openfluid::landr::PolygonEdge(*LS2);
  Entity.addEdge(*E2);

  std::vector<geos::geom::Coordinate>* Coos3 = new std::vector<geos::geom::Coordinate>();
  Coos3->push_back(geos::geom::Coordinate(0, 0));
  Coos3->push_back(geos::geom::Coordinate(0, 2));
  geos::geom::LineString* LS3 = Factory->createLineString(SeqFactory.create(Coos3));
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
  const geos::geom::GeometryFactory* Factory = geos::geom::GeometryFactory::getDefaultInstance();

  std::vector<geos::geom::Coordinate>* CoosLR = new std::vector<geos::geom::Coordinate>();
  CoosLR->push_back(geos::geom::Coordinate(0, 0));
  CoosLR->push_back(geos::geom::Coordinate(0, 2));
  CoosLR->push_back(geos::geom::Coordinate(2, 2));
  CoosLR->push_back(geos::geom::Coordinate(2, 0));
  CoosLR->push_back(geos::geom::Coordinate(0, 0));

  geos::geom::LinearRing* LR = Factory->createLinearRing(SeqFactory.create(CoosLR));
  geos::geom::Polygon* P = Factory->createPolygon(LR, nullptr);
  openfluid::landr::PolygonEntity Entity(P, 0);

  std::vector<geos::geom::Coordinate>* Coos1 = new std::vector<geos::geom::Coordinate>();
  Coos1->push_back(geos::geom::Coordinate(0, 2));
  Coos1->push_back(geos::geom::Coordinate(2, 2));

  geos::geom::LineString* LS1 = Factory->createLineString(SeqFactory.create(Coos1));
  openfluid::landr::PolygonEdge* E1 = new openfluid::landr::PolygonEdge(*LS1);
  Entity.addEdge(*E1);

  std::vector<geos::geom::Coordinate>* Coos2 = new std::vector<geos::geom::Coordinate>();
  Coos2->push_back(geos::geom::Coordinate(0, 0));
  Coos2->push_back(geos::geom::Coordinate(2, 0));
  Coos2->push_back(geos::geom::Coordinate(2, 2));

  geos::geom::LineString* LS2 = Factory->createLineString(SeqFactory.create(Coos2));
  openfluid::landr::PolygonEdge* E2 = new openfluid::landr::PolygonEdge(*LS2);
  Entity.addEdge(*E2);

  std::vector<geos::geom::Coordinate>* Coos3 = new std::vector<geos::geom::Coordinate>();
  Coos3->push_back(geos::geom::Coordinate(0, 0));
  Coos3->push_back(geos::geom::Coordinate(0, 2));
  geos::geom::LineString* LS3 = Factory->createLineString(SeqFactory.create(Coos3));
  openfluid::landr::PolygonEdge* E3 = new openfluid::landr::PolygonEdge(*LS3);
  Entity.addEdge(*E3);

  std::vector<geos::geom::Coordinate>* CooLS = new std::vector<geos::geom::Coordinate>();
  CooLS->push_back(geos::geom::Coordinate(2, 0));
  CooLS->push_back(geos::geom::Coordinate(2, 1));
  geos::geom::LineString* LS = Factory->createLineString(SeqFactory.create(CooLS));

  BOOST_CHECK(Entity.findEdgeLineIntersectingWith(*LS) == E2);

  std::vector<geos::geom::Coordinate>* CooWrongLS = new std::vector<geos::geom::Coordinate>();
  CooWrongLS->push_back(geos::geom::Coordinate(2, 1));
  CooWrongLS->push_back(geos::geom::Coordinate(3, 1));
  geos::geom::LineString* WrongLS = Factory->createLineString(SeqFactory.create(CooWrongLS));

  BOOST_CHECK(!Entity.findEdgeLineIntersectingWith(*WrongLS));

  delete LS;
  delete WrongLS;
}


// =====================================================================
// =====================================================================


BOOST_AUTO_TEST_CASE(check_getCommonEdgesWith)
{
  openfluid::core::GeoVectorValue* Val =
    new openfluid::core::GeoVectorValue(CONFIGTESTS_INPUT_MISCDATA_DIR + "/landr", "SU_horseshoe_lines.shp");

  openfluid::landr::PolygonGraph* Graph = openfluid::landr::PolygonGraph::create(*Val);

  openfluid::landr::PolygonEntity* U1 = Graph->entity(1);
  openfluid::landr::PolygonEntity* U2 = Graph->entity(2);
  openfluid::landr::PolygonEntity* U3 = Graph->entity(3);

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


BOOST_AUTO_TEST_CASE(check_computeNeighbour_MinDistCentroCentro)
{
  openfluid::core::GeoVectorValue* Val =
    new openfluid::core::GeoVectorValue(CONFIGTESTS_INPUT_MISCDATA_DIR + "/landr", "SU.shp");

  openfluid::landr::PolygonGraph* Graph = openfluid::landr::PolygonGraph::create(*Val);

  BOOST_CHECK_EQUAL(Graph->entity(3)->neighbour_MinDistCentroCentro(),
                    Graph->entity(2));
  BOOST_CHECK_EQUAL(Graph->entity(1)->neighbour_MinDistCentroCentro(),
                    Graph->entity(24));
  BOOST_CHECK_EQUAL(Graph->entity(13)->neighbour_MinDistCentroCentro(),
                    Graph->entity(14));
  BOOST_CHECK_EQUAL(Graph->entity(12)->neighbour_MinDistCentroCentro(),
                    Graph->entity(15));

  delete Graph;
  delete Val;
}


// =====================================================================
// =====================================================================


BOOST_AUTO_TEST_CASE(check_computeLineStringNeighbours_Contains)
{
  openfluid::core::GeoVectorValue* ValRS =
    new openfluid::core::GeoVectorValue(CONFIGTESTS_INPUT_MISCDATA_DIR + "/landr", "RS.shp");

  openfluid::core::GeoVectorValue* ValSU =
    new openfluid::core::GeoVectorValue(CONFIGTESTS_INPUT_MISCDATA_DIR + "/landr", "SU.shp");

  openfluid::landr::LineStringGraph* RSGraph = openfluid::landr::LineStringGraph::create(*ValRS);
  openfluid::landr::PolygonGraph* SUGraph = openfluid::landr::PolygonGraph::create(*ValSU);

  openfluid::landr::PolygonEntity* SU1 = SUGraph->entity(1);
  openfluid::landr::PolygonEntity* SU4 = SUGraph->entity(4);
  openfluid::landr::PolygonEntity* SU17 = SUGraph->entity(17);

  BOOST_CHECK(!SU1->lineStringNeighbours());
  SU1->computeLineStringNeighbours(*RSGraph,openfluid::landr::LandRTools::CONTAINS, 0);
  BOOST_CHECK_EQUAL(SU1->lineStringNeighbours()->size(), 0);
  SU1->computeLineStringNeighbours(*RSGraph,openfluid::landr::LandRTools::CONTAINS,0.0001);
  BOOST_CHECK_EQUAL(SU1->lineStringNeighbours()->size(), 1);
  BOOST_CHECK_EQUAL(SU1->lineStringNeighbours()->begin()->first->getOfldId(),1);

  BOOST_CHECK(!SU4->lineStringNeighbours());
  SU4->computeLineStringNeighbours(*RSGraph,openfluid::landr::LandRTools::CONTAINS, 0);
  BOOST_CHECK_EQUAL(SU4->lineStringNeighbours()->size(), 0);
  SU4->computeLineStringNeighbours(*RSGraph,openfluid::landr::LandRTools::CONTAINS,0.0001);
  BOOST_CHECK_EQUAL(SU4->lineStringNeighbours()->size(), 1);
  BOOST_CHECK_EQUAL(SU4->lineStringNeighbours()->begin()->first->getOfldId(),3);

  BOOST_CHECK(!SU17->lineStringNeighbours());
  SU17->computeLineStringNeighbours(*RSGraph,openfluid::landr::LandRTools::CONTAINS, 0);
  BOOST_CHECK_EQUAL(SU17->lineStringNeighbours()->size(), 0);
  SU17->computeLineStringNeighbours(*RSGraph,openfluid::landr::LandRTools::CONTAINS,0.0001);
  BOOST_CHECK_EQUAL(SU17->lineStringNeighbours()->size(), 1);
  BOOST_CHECK_EQUAL(SU17->lineStringNeighbours()->begin()->first->getOfldId(), 6);

  delete RSGraph;
  delete SUGraph;
  delete ValRS;
  delete ValSU;
}


// =====================================================================
// =====================================================================


BOOST_AUTO_TEST_CASE(check_computeLineStringNeighbours_Intersect)
{
  openfluid::core::GeoVectorValue* ValRS =
    new openfluid::core::GeoVectorValue(CONFIGTESTS_INPUT_MISCDATA_DIR + "/landr", "RS.shp");

  openfluid::core::GeoVectorValue* ValSU =
    new openfluid::core::GeoVectorValue(CONFIGTESTS_INPUT_MISCDATA_DIR + "/landr", "SU.shp");

  openfluid::landr::LineStringGraph* RSGraph = openfluid::landr::LineStringGraph::create(*ValRS);
  openfluid::landr::PolygonGraph* SUGraph = openfluid::landr::PolygonGraph::create(*ValSU);

  openfluid::landr::PolygonEntity* SU1 = SUGraph->entity(1);
  openfluid::landr::PolygonEntity* SU4 = SUGraph->entity(4);
  openfluid::landr::PolygonEntity* SU17 = SUGraph->entity(17);

  BOOST_CHECK(!SU1->lineStringNeighbours());
  SU1->computeLineStringNeighbours(*RSGraph,openfluid::landr::LandRTools::INTERSECTS, 0);
  BOOST_CHECK_EQUAL(SU1->lineStringNeighbours()->size(), 0);
  SU1->computeLineStringNeighbours(*RSGraph,openfluid::landr::LandRTools::INTERSECTS,0.0001);
  BOOST_CHECK_EQUAL(SU1->lineStringNeighbours()->size(), 2);

  BOOST_CHECK(!SU4->lineStringNeighbours());
  SU4->computeLineStringNeighbours(*RSGraph,openfluid::landr::LandRTools::INTERSECTS, 0);
  BOOST_CHECK_EQUAL(SU4->lineStringNeighbours()->size(), 0);
  SU4->computeLineStringNeighbours(*RSGraph,openfluid::landr::LandRTools::INTERSECTS,0.0001);
  BOOST_CHECK_EQUAL(SU4->lineStringNeighbours()->size(), 6);

  BOOST_CHECK(!SU17->lineStringNeighbours());
  SU17->computeLineStringNeighbours(*RSGraph,openfluid::landr::LandRTools::INTERSECTS,0);
  BOOST_CHECK_EQUAL(SU17->lineStringNeighbours()->size(), 0);
  SU17->computeLineStringNeighbours(*RSGraph,openfluid::landr::LandRTools::INTERSECTS,0.0001);
  BOOST_CHECK_EQUAL(SU17->lineStringNeighbours()->size(), 3);

  delete RSGraph;
  delete SUGraph;
  delete ValRS;
  delete ValSU;
}


// =====================================================================
// =====================================================================


BOOST_AUTO_TEST_CASE(check_computeLineStringNeighbours_RelationShip_Contains)
{
  openfluid::core::GeoVectorValue* ValRS =
    new openfluid::core::GeoVectorValue(CONFIGTESTS_INPUT_MISCDATA_DIR + "/landr", "LINE_TEST4.shp");

  openfluid::core::GeoVectorValue* ValSU =
    new openfluid::core::GeoVectorValue(CONFIGTESTS_INPUT_MISCDATA_DIR + "/landr", "POLY_TEST.shp");

  openfluid::landr::LineStringGraph* RSGraph = openfluid::landr::LineStringGraph::create(*ValRS);
  openfluid::landr::PolygonGraph* SUGraph = openfluid::landr::PolygonGraph::create(*ValSU);

  openfluid::landr::PolygonEntity* SU1 = SUGraph->entity(1);
  openfluid::landr::PolygonEntity* SU2 = SUGraph->entity(2);
  openfluid::landr::PolygonEntity* SU3 = SUGraph->entity(3);
  openfluid::landr::PolygonEntity* SU4 = SUGraph->entity(4);

  // line Neighbours of SU1
  BOOST_CHECK(!SU1->lineStringNeighbours());
  SU1->computeLineStringNeighbours(*RSGraph,openfluid::landr::LandRTools::CONTAINS, 0);
  BOOST_CHECK_EQUAL(SU1->lineStringNeighbours()->size(), 0);
  SU1->computeLineStringNeighbours(*RSGraph,openfluid::landr::LandRTools::CONTAINS,0.0001);
  BOOST_CHECK_EQUAL(SU1->lineStringNeighbours()->size(), 2);

  std::map<openfluid::landr::LineStringEntity*, openfluid::landr::PolygonEdge*> *lineMap=SU1->lineStringNeighbours();
  std::map<openfluid::landr::LineStringEntity*, openfluid::landr::PolygonEdge*>::iterator it=lineMap->begin();
  std::map<openfluid::landr::LineStringEntity*, openfluid::landr::PolygonEdge*>::iterator ite=lineMap->end();
  std::list<int> lNeighbours;
  for (; it != ite; ++it)
  {
    lNeighbours.push_back((*it).first->getOfldId());
  }

  lNeighbours.sort();
  std::list<int>::iterator jt=lNeighbours.begin();

  BOOST_CHECK_EQUAL((*jt),1);
  ++jt;
  BOOST_CHECK_EQUAL((*jt),2);

  SU1->computeLineStringNeighbours(*RSGraph,openfluid::landr::LandRTools::CONTAINS,0.1);
  BOOST_CHECK_EQUAL(SU1->lineStringNeighbours()->size(), 3);


  lNeighbours.clear();
  lineMap=SU1->lineStringNeighbours();

  it=lineMap->begin();
  ite=lineMap->end();

  for (; it != ite; ++it)
  {
    lNeighbours.push_back((*it).first->getOfldId());
  }


  lNeighbours.sort();
  jt=lNeighbours.begin();

  BOOST_CHECK_EQUAL((*jt),1);
  ++jt;
  BOOST_CHECK_EQUAL((*jt),2);
  ++jt;
  BOOST_CHECK_EQUAL((*jt),12);


  // line Neighbours of SU2
  BOOST_CHECK(!SU2->lineStringNeighbours());
  SU2->computeLineStringNeighbours(*RSGraph,openfluid::landr::LandRTools::CONTAINS, 0);
  BOOST_CHECK_EQUAL(SU2->lineStringNeighbours()->size(), 0);
  SU2->computeLineStringNeighbours(*RSGraph,openfluid::landr::LandRTools::CONTAINS,0.0001);
  BOOST_CHECK_EQUAL(SU2->lineStringNeighbours()->size(), 0);

  // line Neighbours of SU3
  BOOST_CHECK(!SU3->lineStringNeighbours());
  SU3->computeLineStringNeighbours(*RSGraph,openfluid::landr::LandRTools::CONTAINS, 0);
  BOOST_CHECK_EQUAL(SU3->lineStringNeighbours()->size(), 0);
  SU3->computeLineStringNeighbours(*RSGraph,openfluid::landr::LandRTools::CONTAINS,0.0001);
  BOOST_CHECK_EQUAL(SU3->lineStringNeighbours()->size(), 0);

  // line Neighbours of SU4
  BOOST_CHECK(!SU4->lineStringNeighbours());
  SU4->computeLineStringNeighbours(*RSGraph,openfluid::landr::LandRTools::CONTAINS, 0);
  BOOST_CHECK_EQUAL(SU4->lineStringNeighbours()->size(), 0);
  SU4->computeLineStringNeighbours(*RSGraph,openfluid::landr::LandRTools::CONTAINS,0.01);
  BOOST_CHECK_EQUAL(SU4->lineStringNeighbours()->size(), 0);
  SU4->computeLineStringNeighbours(*RSGraph,openfluid::landr::LandRTools::CONTAINS,0.1);
  BOOST_CHECK_EQUAL(SU4->lineStringNeighbours()->size(), 1);
  BOOST_CHECK_EQUAL(SU4->lineStringNeighbours()->begin()->first->getOfldId(), 10);


  delete RSGraph;
  delete SUGraph;
  delete ValRS;
  delete ValSU;

}


// =====================================================================
// =====================================================================


BOOST_AUTO_TEST_CASE(check_computeLineStringNeighbours_RelationShip_Intersects)
{
  openfluid::core::GeoVectorValue* ValRS =
    new openfluid::core::GeoVectorValue(CONFIGTESTS_INPUT_MISCDATA_DIR + "/landr", "LINE_TEST4.shp");

  openfluid::core::GeoVectorValue* ValSU =
    new openfluid::core::GeoVectorValue(CONFIGTESTS_INPUT_MISCDATA_DIR + "/landr", "POLY_TEST.shp");

  openfluid::landr::LineStringGraph* RSGraph = openfluid::landr::LineStringGraph::create(*ValRS);
  openfluid::landr::PolygonGraph* SUGraph = openfluid::landr::PolygonGraph::create(*ValSU);

  openfluid::landr::PolygonEntity* SU1 = SUGraph->entity(1);
  openfluid::landr::PolygonEntity* SU2 = SUGraph->entity(2);
  openfluid::landr::PolygonEntity* SU3 = SUGraph->entity(3);
  openfluid::landr::PolygonEntity* SU4 = SUGraph->entity(4);

  // line Neighbours of SU1
  BOOST_CHECK(!SU1->lineStringNeighbours());
  SU1->computeLineStringNeighbours(*RSGraph,openfluid::landr::LandRTools::INTERSECTS, 0);
  BOOST_CHECK_EQUAL(SU1->lineStringNeighbours()->size(), 0);
  SU1->computeLineStringNeighbours(*RSGraph,openfluid::landr::LandRTools::INTERSECTS,0.0001);
  BOOST_CHECK_EQUAL(SU1->lineStringNeighbours()->size(), 4);

  std::map<openfluid::landr::LineStringEntity*, openfluid::landr::PolygonEdge*> *lineMap=SU1->lineStringNeighbours();
  std::map<openfluid::landr::LineStringEntity*, openfluid::landr::PolygonEdge*>::iterator it=lineMap->begin();
  std::map<openfluid::landr::LineStringEntity*, openfluid::landr::PolygonEdge*>::iterator ite=lineMap->end();

  std::list<int> lNeighbours;
  for (; it != ite; ++it)
  {
    lNeighbours.push_back((*it).first->getOfldId());
  }

  lNeighbours.sort();
  std::list<int>::iterator jt=lNeighbours.begin();

  BOOST_CHECK_EQUAL((*jt),1);
  ++jt;
  BOOST_CHECK_EQUAL((*jt),2);
  ++jt;
  BOOST_CHECK_EQUAL((*jt),3);
  ++jt;
  BOOST_CHECK_EQUAL((*jt),12);

  // line Neighbours of SU2
  BOOST_CHECK(!SU2->lineStringNeighbours());
  SU2->computeLineStringNeighbours(*RSGraph,openfluid::landr::LandRTools::INTERSECTS, 0);
  BOOST_CHECK_EQUAL(SU2->lineStringNeighbours()->size(), 0);
  SU2->computeLineStringNeighbours(*RSGraph,openfluid::landr::LandRTools::INTERSECTS,0.0001);
  BOOST_CHECK_EQUAL(SU2->lineStringNeighbours()->size(), 5);


  lNeighbours.clear();
  lineMap=SU2->lineStringNeighbours();
  it=lineMap->begin();
  ite=lineMap->end();

  for (; it != ite; ++it)
  {
    lNeighbours.push_back((*it).first->getOfldId());
  }

  lNeighbours.sort();
  jt=lNeighbours.begin();

  BOOST_CHECK_EQUAL((*jt),1);
  ++jt;
  BOOST_CHECK_EQUAL((*jt),5);
  ++jt;
  BOOST_CHECK_EQUAL((*jt),6);
  ++jt;
  BOOST_CHECK_EQUAL((*jt),7);
  ++jt;
  BOOST_CHECK_EQUAL((*jt),11);


  // line Neighbours of SU3
  BOOST_CHECK(!SU3->lineStringNeighbours());
  SU3->computeLineStringNeighbours(*RSGraph,openfluid::landr::LandRTools::INTERSECTS, 0);
  BOOST_CHECK_EQUAL(SU3->lineStringNeighbours()->size(), 0);
  SU3->computeLineStringNeighbours(*RSGraph,openfluid::landr::LandRTools::INTERSECTS,0.0001);
  BOOST_CHECK_EQUAL(SU3->lineStringNeighbours()->size(), 3);


  lNeighbours.clear();
  lineMap=SU3->lineStringNeighbours();
  it=lineMap->begin();
  ite=lineMap->end();

  for (; it != ite; ++it)
  {
    lNeighbours.push_back((*it).first->getOfldId());
  }

  lNeighbours.sort();
  jt=lNeighbours.begin();

  BOOST_CHECK_EQUAL((*jt),3);
  ++jt;
  BOOST_CHECK_EQUAL((*jt),4);
  ++jt;
  BOOST_CHECK_EQUAL((*jt),8);


  // line Neighbours of SU4
  BOOST_CHECK(!SU4->lineStringNeighbours());
  SU4->computeLineStringNeighbours(*RSGraph,openfluid::landr::LandRTools::INTERSECTS, 0);
  BOOST_CHECK_EQUAL(SU4->lineStringNeighbours()->size(), 0);
  SU4->computeLineStringNeighbours(*RSGraph,openfluid::landr::LandRTools::INTERSECTS,0.0001);
  BOOST_CHECK_EQUAL(SU4->lineStringNeighbours()->size(), 3);
  SU4->computeLineStringNeighbours(*RSGraph,openfluid::landr::LandRTools::INTERSECTS,0.1);
  BOOST_CHECK_EQUAL(SU4->lineStringNeighbours()->size(), 4);


  lNeighbours.clear();
  lineMap=SU4->lineStringNeighbours();
  it=lineMap->begin();
  ite=lineMap->end();

  for (; it != ite; ++it)
  {
    lNeighbours.push_back((*it).first->getOfldId());
  }

  lNeighbours.sort();
  jt=lNeighbours.begin();


  BOOST_CHECK_EQUAL((*jt),4);
  ++jt;
  BOOST_CHECK_EQUAL((*jt),5);
  ++jt;
  BOOST_CHECK_EQUAL((*jt),9);
  ++jt;
  BOOST_CHECK_EQUAL((*jt),10);

  delete RSGraph;
  delete SUGraph;
  delete ValRS;
  delete ValSU;

}


// =====================================================================
// =====================================================================


BOOST_AUTO_TEST_CASE(check_computeLineStringNeighbours_RelationShip_Touches)
{

  openfluid::core::GeoVectorValue* ValRS =
    new openfluid::core::GeoVectorValue(CONFIGTESTS_INPUT_MISCDATA_DIR + "/landr", "LINE_TEST4.shp");

  openfluid::core::GeoVectorValue* ValSU =
    new openfluid::core::GeoVectorValue(CONFIGTESTS_INPUT_MISCDATA_DIR + "/landr", "POLY_TEST.shp");

  openfluid::landr::LineStringGraph* RSGraph = openfluid::landr::LineStringGraph::create(*ValRS);
  openfluid::landr::PolygonGraph* SUGraph = openfluid::landr::PolygonGraph::create(*ValSU);

  openfluid::landr::PolygonEntity* SU1 = SUGraph->entity(1);
  openfluid::landr::PolygonEntity* SU2 = SUGraph->entity(2);
  openfluid::landr::PolygonEntity* SU3 = SUGraph->entity(3);
  openfluid::landr::PolygonEntity* SU4 = SUGraph->entity(4);

  // line Neighbours of SU1
  BOOST_CHECK(!SU1->lineStringNeighbours());

  BOOST_CHECK_THROW(SU1->computeLineStringNeighbours(*RSGraph,openfluid::landr::LandRTools::TOUCHES, 0),
                    openfluid::base::FrameworkException);

  SU1->computeLineStringNeighbours(*RSGraph,openfluid::landr::LandRTools::TOUCHES, 0,0.1);
  BOOST_CHECK_EQUAL(SU1->lineStringNeighbours()->size(), 0);
  SU1->computeLineStringNeighbours(*RSGraph,openfluid::landr::LandRTools::TOUCHES,0.01,0.1);

  BOOST_CHECK_EQUAL(SU1->lineStringNeighbours()->size(), 4);

  std::list<int> lNeighbours;
  std::map<openfluid::landr::LineStringEntity*, openfluid::landr::PolygonEdge*> *lineMap=SU1->lineStringNeighbours();
  lineMap=SU1->lineStringNeighbours();
  std::map<openfluid::landr::LineStringEntity*, openfluid::landr::PolygonEdge*>::iterator it=lineMap->begin();
  std::map<openfluid::landr::LineStringEntity*, openfluid::landr::PolygonEdge*>::iterator ite=lineMap->end();


  for (; it != ite; ++it)
  {
    lNeighbours.push_back((*it).first->getOfldId());
  }

  lNeighbours.sort();
  std::list<int>::iterator jt=lNeighbours.begin();
  jt=lNeighbours.begin();


  BOOST_CHECK_EQUAL((*jt),1);
  ++jt;
  BOOST_CHECK_EQUAL((*jt),2);
  ++jt;
  BOOST_CHECK_EQUAL((*jt),3);
  ++jt;
  BOOST_CHECK_EQUAL((*jt),12);

  // line Neighbours of SU2
  SU2->computeLineStringNeighbours(*RSGraph,openfluid::landr::LandRTools::TOUCHES,0.01,0.1);

  BOOST_CHECK_EQUAL(SU2->lineStringNeighbours()->size(), 1);
  BOOST_CHECK_EQUAL(SU2->lineStringNeighbours()->begin()->first->getOfldId(), 5);


  // line Neighbours of SU3
  SU3->computeLineStringNeighbours(*RSGraph,openfluid::landr::LandRTools::TOUCHES,0.01,0.1);

  BOOST_CHECK_EQUAL(SU3->lineStringNeighbours()->size(), 2);

  lNeighbours.clear();
  lineMap=SU3->lineStringNeighbours();
  it=lineMap->begin();
  ite=lineMap->end();

  for (; it != ite; ++it)
  {
    lNeighbours.push_back((*it).first->getOfldId());
  }

  lNeighbours.sort();
  jt=lNeighbours.begin();

  BOOST_CHECK_EQUAL((*jt),3);
  ++jt;
  BOOST_CHECK_EQUAL((*jt),4);


  // line Neighbours of SU4
  SU4->computeLineStringNeighbours(*RSGraph,openfluid::landr::LandRTools::TOUCHES,0.1,0.2);

  BOOST_CHECK_EQUAL(SU4->lineStringNeighbours()->size(), 4);

  lNeighbours.clear();
  lineMap=SU4->lineStringNeighbours();
  it=lineMap->begin();
  ite=lineMap->end();

  for (; it != ite; ++it)
  {
    lNeighbours.push_back((*it).first->getOfldId());
  }

  lNeighbours.sort();
  jt=lNeighbours.begin();

  BOOST_CHECK_EQUAL((*jt),4);
  ++jt;
  BOOST_CHECK_EQUAL((*jt),5);
  ++jt;
  BOOST_CHECK_EQUAL((*jt),9);
  ++jt;
  BOOST_CHECK_EQUAL((*jt),10);


  delete RSGraph;
  delete SUGraph;
  delete ValRS;
  delete ValSU;

}


// =====================================================================
// =====================================================================


BOOST_AUTO_TEST_CASE(check_computeNeighbourWithCommonEdge)
{

  openfluid::core::GeoVectorValue* ValSU =
    new openfluid::core::GeoVectorValue(CONFIGTESTS_INPUT_MISCDATA_DIR + "/landr", "POLY_TEST.shp");

  openfluid::landr::PolygonGraph* SUGraph = openfluid::landr::PolygonGraph::create(*ValSU);


  openfluid::landr::PolygonEntity* SU1 = SUGraph->entity(1);
  openfluid::landr::PolygonEntity* SU2 = SUGraph->entity(2);
  openfluid::landr::PolygonEntity* SU3 = SUGraph->entity(3);
  openfluid::landr::PolygonEntity* SU4 = SUGraph->entity(4);


  std::vector<openfluid::landr::PolygonEdge*> vEdgeSU=SU1->getCommonEdgesWith(*SU2);
  BOOST_CHECK_EQUAL(SU1->neighbourWithCommonEdge(vEdgeSU.at(0))->getOfldId(),2);
  vEdgeSU.clear();
  vEdgeSU=SU2->getCommonEdgesWith(*SU1);
  BOOST_CHECK_EQUAL(SU2->neighbourWithCommonEdge(vEdgeSU.at(0))->getOfldId(),1);
  vEdgeSU.clear();
  vEdgeSU=SU1->getCommonEdgesWith(*SU3);
  BOOST_CHECK_EQUAL(SU1->neighbourWithCommonEdge(vEdgeSU.at(0))->getOfldId(),3);
  vEdgeSU.clear();
  vEdgeSU=SU3->getCommonEdgesWith(*SU1);
  BOOST_CHECK_EQUAL(SU3->neighbourWithCommonEdge(vEdgeSU.at(0))->getOfldId(),1);
  vEdgeSU.clear();
  vEdgeSU=SU3->getCommonEdgesWith(*SU4);
  BOOST_CHECK_EQUAL(SU3->neighbourWithCommonEdge(vEdgeSU.at(0))->getOfldId(),4);
  vEdgeSU.clear();
  vEdgeSU=SU4->getCommonEdgesWith(*SU3);
  BOOST_CHECK_EQUAL(SU4->neighbourWithCommonEdge(vEdgeSU.at(0))->getOfldId(),3);

  openfluid::landr::PolygonEntity* SUFalse=SU3->neighbourWithCommonEdge(SU1->m_PolyEdges.at(1));
  BOOST_CHECK(!SUFalse);

  delete SUGraph;
  delete ValSU;

}


// =====================================================================
// =====================================================================


BOOST_AUTO_TEST_CASE(check_computeNeighboursWithBarriers)
{

  openfluid::core::GeoVectorValue* ValSU =
    new openfluid::core::GeoVectorValue(CONFIGTESTS_INPUT_MISCDATA_DIR + "/landr", "POLY_TEST.shp");

  openfluid::landr::PolygonGraph* SUGraph = openfluid::landr::PolygonGraph::create(*ValSU);

  openfluid::core::GeoVectorValue* ValBarriers =
    new openfluid::core::GeoVectorValue(CONFIGTESTS_INPUT_MISCDATA_DIR + "/landr/", "Barriers.shp");

  openfluid::landr::LineStringGraph* BarriersGraph = openfluid::landr::LineStringGraph::create(*ValBarriers);

  openfluid::landr::PolygonEntity* SU1 = SUGraph->entity(1);
  openfluid::landr::PolygonEntity* SU2 = SUGraph->entity(2);
  openfluid::landr::PolygonEntity* SU3 = SUGraph->entity(3);
  openfluid::landr::PolygonEntity* SU4 = SUGraph->entity(4);

  SU1->computeNeighbours();
  BOOST_CHECK_EQUAL(SU1->getOrderedNeighbourOfldIds().size(),2);
  SU2->computeNeighbours();
  BOOST_CHECK_EQUAL(SU2->getOrderedNeighbourOfldIds().size(),2);
  SU3->computeNeighbours();
  BOOST_CHECK_EQUAL(SU3->getOrderedNeighbourOfldIds().size(),2);
  SU4->computeNeighbours();
  BOOST_CHECK_EQUAL(SU4->getOrderedNeighbourOfldIds().size(),2);


  BOOST_CHECK_THROW(SU1->computeNeighboursWithBarriers(*BarriersGraph,openfluid::landr::LandRTools::INTERSECTS,0.01),
                    openfluid::base::FrameworkException);

  SU1->computeNeighboursWithBarriers(*BarriersGraph,openfluid::landr::LandRTools::CONTAINS,0.01);

  BOOST_CHECK_EQUAL(SU1->getOrderedNeighbourOfldIds().size(),1);
  BOOST_CHECK_EQUAL(SU1->getOrderedNeighbourOfldIds().at(0),2);

  SU2->computeNeighboursWithBarriers(*BarriersGraph,openfluid::landr::LandRTools::CONTAINS,0.01);

  BOOST_CHECK_EQUAL(SU2->getOrderedNeighbourOfldIds().size(),2);


  SU3->computeNeighboursWithBarriers(*BarriersGraph,openfluid::landr::LandRTools::CONTAINS,0.01);

  BOOST_CHECK_EQUAL(SU3->getOrderedNeighbourOfldIds().size(),0);

  SU2->computeNeighboursWithBarriers(*BarriersGraph,openfluid::landr::LandRTools::TOUCHES,0.01,0.2);

  BOOST_CHECK_EQUAL(SU2->getOrderedNeighbourOfldIds().size(),1);
  BOOST_CHECK_EQUAL(SU2->getOrderedNeighbourOfldIds().at(0),1);

  SU2->computeNeighboursWithBarriers(*BarriersGraph,openfluid::landr::LandRTools::TOUCHES,0.1,0.2);

  BOOST_CHECK_EQUAL(SU2->getOrderedNeighbourOfldIds().size(),0);

  openfluid::core::GeoVectorValue* ValRS =
    new openfluid::core::GeoVectorValue(CONFIGTESTS_INPUT_MISCDATA_DIR + "/landr/", "LINE_TEST4.shp");

  openfluid::landr::LineStringGraph* RSGraph = openfluid::landr::LineStringGraph::create(*ValRS);

  SU1->computeLineStringNeighbours(*RSGraph,openfluid::landr::LandRTools::INTERSECTS,0.01);

  BOOST_CHECK_EQUAL(SU1->getOrderedNeighbourOfldIds().size(),1);
  BOOST_CHECK_EQUAL(SU1->getOrderedNeighbourOfldIds().at(0),2);
  BOOST_CHECK_EQUAL(SU1->lineStringNeighbours()->size(),4);

  SU2->computeLineStringNeighbours(*RSGraph,openfluid::landr::LandRTools::INTERSECTS,0.01);

  BOOST_CHECK_EQUAL(SU2->getOrderedNeighbourOfldIds().size(),0);
  BOOST_CHECK_EQUAL(SU2->lineStringNeighbours()->size(),5);


  delete ValSU;
  delete SUGraph;
  delete ValBarriers;
  delete BarriersGraph;
  delete ValRS;
  delete RSGraph;
}


// =====================================================================
// =====================================================================


BOOST_AUTO_TEST_CASE(check_getOrderedNeighboursByLengthBoundary)
{

  openfluid::core::GeoVectorValue* ValSU =
    new openfluid::core::GeoVectorValue(CONFIGTESTS_INPUT_MISCDATA_DIR + "/landr", "SU.shp");

  openfluid::landr::PolygonGraph* SUGraph = openfluid::landr::PolygonGraph::create(*ValSU);

  openfluid::landr::PolygonEntity* SU9 = SUGraph->entity(9);

  std::multimap<double, openfluid::landr::PolygonEntity*> mNeighbours;
  mNeighbours = SU9->getOrderedNeighboursByLengthBoundary();

  std::multimap<double, openfluid::landr::PolygonEntity*> ::iterator it = mNeighbours.begin();
  BOOST_CHECK_EQUAL((*it).second->getOfldId(),11);
  ++it;
  BOOST_CHECK_EQUAL((*it).second->getOfldId(),10);
  ++it;
  BOOST_CHECK_EQUAL((*it).second->getOfldId(),12);
  ++it;
  BOOST_CHECK_EQUAL((*it).second->getOfldId(),7);

  delete ValSU;

}


// =====================================================================
// =====================================================================


BOOST_AUTO_TEST_CASE(check_computeNeighbourByLineTopology)
{

  openfluid::core::GeoVectorValue* ValSU =
    new openfluid::core::GeoVectorValue(CONFIGTESTS_INPUT_MISCDATA_DIR + "/landr", "SU.shp");

  openfluid::core::GeoVectorValue ValTopo(CONFIGTESTS_INPUT_MISCDATA_DIR + "/landr","SUTopoLine.shp");

  openfluid::landr::VectorDataset* VectTopo = new openfluid::landr::VectorDataset(ValTopo);

  openfluid::core::GeoVectorValue* ValRS =
    new openfluid::core::GeoVectorValue(CONFIGTESTS_INPUT_MISCDATA_DIR + "/landr", "RS.shp");

  openfluid::landr::PolygonGraph* SUGraph = openfluid::landr::PolygonGraph::create(*ValSU);

  openfluid::landr::PolygonEntity* SU9 = SUGraph->entity(9);

  BOOST_CHECK_EQUAL(SU9->computeNeighbourByLineTopology(*VectTopo).first->getOfldId(),12);
  BOOST_CHECK(openfluid::scientific::isVeryClose(SU9->computeNeighbourByLineTopology(*VectTopo).second,85.50399));

  openfluid::landr::LineStringGraph* RSGraph = openfluid::landr::LineStringGraph::create(*ValRS);
  SU9->computeLineStringNeighbours(*RSGraph,openfluid::landr::LandRTools::INTERSECTS, 0.1);

  BOOST_CHECK_EQUAL(SU9->computeNeighbourByLineTopology(*VectTopo).first->getOfldId(),5);

  BOOST_CHECK(openfluid::scientific::isVeryClose(SU9->computeNeighbourByLineTopology(*VectTopo).second,44.5096));

  openfluid::landr::PolygonEntity* SU10 = SUGraph->entity(10);

  openfluid::landr::LandREntity* NeighbourFalse = SU10->computeNeighbourByLineTopology(*VectTopo).first;
  BOOST_CHECK(!NeighbourFalse);
  BOOST_CHECK(openfluid::scientific::isVeryClose(SU10->computeNeighbourByLineTopology(*VectTopo).second,0.0));
  openfluid::landr::PolygonEntity* SU8 = SUGraph->entity(8);

  openfluid::landr::LandREntity* NoNeighbour = SU8->computeNeighbourByLineTopology(*VectTopo).first;
  BOOST_CHECK(!NoNeighbour);
  BOOST_CHECK(openfluid::scientific::isVeryClose(SU8->computeNeighbourByLineTopology(*VectTopo).second,0.0));

  delete ValSU;
  delete ValRS;
}


// =====================================================================
// =====================================================================


int main(int argc, char *argv[])
{
  openfluid::base::Environment::init();

  return ::boost::unit_test::unit_test_main( &init_unit_test, argc, argv );
}
