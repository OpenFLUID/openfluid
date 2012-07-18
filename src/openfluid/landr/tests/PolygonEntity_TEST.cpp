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
      FirstFeature->Clone());

  BOOST_CHECK_EQUAL(Val->getType(),
                    openfluid::core::UnstructuredValue::GeoVectorValue);

  BOOST_CHECK(Entity->getPolygon()->equals(GeosGeom));

  BOOST_CHECK(Entity->getFeature()->Equal(FirstFeature));

  BOOST_CHECK_EQUAL(Entity->getSelfId(), 1);

  OGRFeature::DestroyFeature(FirstFeature);
  delete Entity;
  delete Val;
}

// =====================================================================
// =====================================================================

BOOST_AUTO_TEST_CASE(check_copy)
{
  openfluid::core::GeoVectorValue* Val = new openfluid::core::GeoVectorValue(
      CONFIGTESTS_INPUT_DATASETS_DIR + "/landr", "SU.shp");

  OGRFeature* FirstFeature = Val->getLayer0()->GetFeature(0);
  OGRGeometry* OGRGeom = FirstFeature->GetGeometryRef();

  geos::geom::Geometry* GeosGeom =
      (geos::geom::Geometry*) OGRGeom->exportToGEOS();

  openfluid::landr::PolygonEntity* Entity = new openfluid::landr::PolygonEntity(
      dynamic_cast<geos::geom::Polygon*>(GeosGeom->clone()),
      FirstFeature->Clone());

  OGRFeature::DestroyFeature(FirstFeature);
  delete GeosGeom;
  delete Val;

  openfluid::landr::PolygonEntity* CopyEntity =
      new openfluid::landr::PolygonEntity(*Entity);

  BOOST_CHECK(Entity->getPolygon()->equals(CopyEntity->getPolygon()));
  BOOST_CHECK_EQUAL(Entity->getSelfId(), CopyEntity->getSelfId());
  BOOST_CHECK_EQUAL(Entity->getFeature()->GetFieldCount(),
                    CopyEntity->getFeature()->GetFieldCount());

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

  openfluid::landr::PolygonGraph* Graph = new openfluid::landr::PolygonGraph(
      *Val);

  openfluid::landr::PolygonEntity* U2 = Graph->getEntity(2);
  openfluid::landr::PolygonEntity* U18 = Graph->getEntity(18);
  openfluid::landr::PolygonEntity* U24 = Graph->getEntity(24);

  std::vector<openfluid::landr::PolygonEntity*> U2N = U2->getNeighbours();
  std::vector<openfluid::landr::PolygonEntity*> U18N = U18->getNeighbours();
  std::vector<openfluid::landr::PolygonEntity*> U24N = U24->getNeighbours();

  BOOST_CHECK_EQUAL(U2N.size(), 5);
  BOOST_CHECK_EQUAL(U18N.size(), 4);
  BOOST_CHECK_EQUAL(U24N.size(), 3);

  std::set<int> NeighbourIds;
  std::set<int>::iterator it;

  for (unsigned int i = 0; i < U2N.size(); i++)
    NeighbourIds.insert(U2N[i]->getSelfId());

  it = NeighbourIds.begin();
  BOOST_CHECK_EQUAL(*it, 1);
  BOOST_CHECK_EQUAL(*(++it), 3);
  BOOST_CHECK_EQUAL(*(++it), 4);
  BOOST_CHECK_EQUAL(*(++it), 5);
  BOOST_CHECK_EQUAL(*(++it), 23);

  NeighbourIds.clear();
  for (unsigned int i = 0; i < U18N.size(); i++)
    NeighbourIds.insert(U18N[i]->getSelfId());

  it = NeighbourIds.begin();
  BOOST_CHECK_EQUAL(*it, 4);
  BOOST_CHECK_EQUAL(*(++it), 17);
  BOOST_CHECK_EQUAL(*(++it), 19);
  BOOST_CHECK_EQUAL(*(++it), 20);

  NeighbourIds.clear();
  for (unsigned int i = 0; i < U24N.size(); i++)
    NeighbourIds.insert(U24N[i]->getSelfId());

  it = NeighbourIds.begin();
  BOOST_CHECK_EQUAL(*it, 1);
  BOOST_CHECK_EQUAL(*(++it), 22);
  BOOST_CHECK_EQUAL(*(++it), 23);

  delete Graph;
  delete Val;
}

// =====================================================================
// =====================================================================

BOOST_AUTO_TEST_CASE(check_lineIntersection)
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
  openfluid::landr::PolygonEntity Ent1(P1, NULL);

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
  openfluid::landr::PolygonEntity Ent2(P2, NULL);

  geos::geom::LineString* Shared = Ent1.getLineIntersectionWith(Ent2);

  BOOST_CHECK(Shared->getCoordinateN(0).equals(geos::geom::Coordinate(2, 1)));
  BOOST_CHECK(Shared->getCoordinateN(1).equals(geos::geom::Coordinate(2, 0)));

  delete Shared;
}

// =====================================================================
// =====================================================================

BOOST_AUTO_TEST_CASE(check_NolineIntersection)
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
  openfluid::landr::PolygonEntity Ent1(P1, NULL);

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
  openfluid::landr::PolygonEntity Ent2(P2, NULL);

  geos::geom::LineString* Shared = Ent1.getLineIntersectionWith(Ent2);

  BOOST_CHECK(!Shared);
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
  openfluid::landr::PolygonEntity Entity(P, NULL);

  std::vector<geos::geom::Coordinate>* Coos1 = new std::vector<
      geos::geom::Coordinate>();
  Coos1->push_back(geos::geom::Coordinate(0, 2));
  Coos1->push_back(geos::geom::Coordinate(2, 2));
  geos::geom::LineString* LS1 = Factory.createLineString(
      SeqFactory.create(Coos1));
  openfluid::landr::PolygonEdge* E1 = new openfluid::landr::PolygonEdge(*LS1);
  Entity.addEdge(E1);

  std::vector<geos::geom::Coordinate>* Coos2 = new std::vector<
      geos::geom::Coordinate>();
  Coos2->push_back(geos::geom::Coordinate(0, 0));
  Coos2->push_back(geos::geom::Coordinate(2, 0));
  Coos2->push_back(geos::geom::Coordinate(2, 2));
  geos::geom::LineString* LS2 = Factory.createLineString(
      SeqFactory.create(Coos2));
  openfluid::landr::PolygonEdge* E2 = new openfluid::landr::PolygonEdge(*LS2);
  Entity.addEdge(E2);

  std::vector<geos::geom::Coordinate>* Coos3 = new std::vector<
      geos::geom::Coordinate>();
  Coos3->push_back(geos::geom::Coordinate(0, 0));
  Coos3->push_back(geos::geom::Coordinate(0, 2));
  geos::geom::LineString* LS3 = Factory.createLineString(
      SeqFactory.create(Coos3));
  openfluid::landr::PolygonEdge* E3 = new openfluid::landr::PolygonEdge(*LS3);
  Entity.addEdge(E3);

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
  openfluid::landr::PolygonEntity Entity(P, NULL);

  std::vector<geos::geom::Coordinate>* Coos1 = new std::vector<
      geos::geom::Coordinate>();
  Coos1->push_back(geos::geom::Coordinate(0, 2));
  Coos1->push_back(geos::geom::Coordinate(2, 2));
  geos::geom::LineString* LS1 = Factory.createLineString(
      SeqFactory.create(Coos1));
  openfluid::landr::PolygonEdge* E1 = new openfluid::landr::PolygonEdge(*LS1);
  Entity.addEdge(E1);

  std::vector<geos::geom::Coordinate>* Coos2 = new std::vector<
      geos::geom::Coordinate>();
  Coos2->push_back(geos::geom::Coordinate(0, 0));
  Coos2->push_back(geos::geom::Coordinate(2, 0));
  Coos2->push_back(geos::geom::Coordinate(2, 2));
  geos::geom::LineString* LS2 = Factory.createLineString(
      SeqFactory.create(Coos2));
  openfluid::landr::PolygonEdge* E2 = new openfluid::landr::PolygonEdge(*LS2);
  Entity.addEdge(E2);

  std::vector<geos::geom::Coordinate>* Coos3 = new std::vector<
      geos::geom::Coordinate>();
  Coos3->push_back(geos::geom::Coordinate(0, 0));
  Coos3->push_back(geos::geom::Coordinate(0, 2));
  geos::geom::LineString* LS3 = Factory.createLineString(
      SeqFactory.create(Coos3));
  openfluid::landr::PolygonEdge* E3 = new openfluid::landr::PolygonEdge(*LS3);
  Entity.addEdge(E3);

  std::vector<geos::geom::Coordinate>* CooLS = new std::vector<
      geos::geom::Coordinate>();
  CooLS->push_back(geos::geom::Coordinate(2, 0));
  CooLS->push_back(geos::geom::Coordinate(2, 1));
  geos::geom::LineString* LS = Factory.createLineString(
      SeqFactory.create(CooLS));

  BOOST_CHECK(Entity.findEdgeIntersecting(*LS) == E2);

  std::vector<geos::geom::Coordinate>* CooWrongLS = new std::vector<
      geos::geom::Coordinate>();
  CooWrongLS->push_back(geos::geom::Coordinate(2, 1));
  CooWrongLS->push_back(geos::geom::Coordinate(3, 1));
  geos::geom::LineString* WrongLS = Factory.createLineString(
      SeqFactory.create(CooWrongLS));

  BOOST_CHECK(!Entity.findEdgeIntersecting(*WrongLS));

  delete LS;
  delete WrongLS;
}
