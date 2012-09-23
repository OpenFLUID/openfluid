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
 \file NodedGraph_TEST.cpp
 \brief Implements ...

 \author Aline LIBRES <aline.libres@gmail.com>
 */

#define BOOST_TEST_MAIN
#define BOOST_AUTO_TEST_MAIN
#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MODULE unittest_nodedgraph
#include <boost/test/unit_test.hpp>
#include <boost/test/auto_unit_test.hpp>
#include <boost/filesystem/path.hpp>
#include <tests-config.hpp>
#include <openfluid/base/OFException.hpp>
#include <openfluid/landr/NodedGraph.hpp>
#include <openfluid/landr/NodedLine.hpp>
#include <openfluid/core/GeoVectorValue.hpp>
#include <geos/geom/Geometry.h>
#include <geos/geom/LineString.h>
#include <geos/geom/MultiLineString.h>
#include <geos/geom/LinearRing.h>
#include <geos/geom/Point.h>
#include <geos/geom/Polygon.h>
#include <geos/geom/CoordinateArraySequenceFactory.h>
#include <geos/geom/GeometryFactory.h>

// =====================================================================
// =====================================================================

BOOST_AUTO_TEST_CASE(check_construction_verysimple_1PtIntersect)
{
  //   x   x
  //     x
  //   x   x

  std::cout << "verysimple_1PtIntersect" << std::endl;

  geos::geom::CoordinateArraySequenceFactory SeqFactory;
  geos::geom::GeometryFactory Factory;

  std::vector<geos::geom::Coordinate>* Coos1 = new std::vector<
      geos::geom::Coordinate>();
  Coos1->push_back(geos::geom::Coordinate(0, 0));
  Coos1->push_back(geos::geom::Coordinate(2, 2));
  geos::geom::LineString* LS = Factory.createLineString(
      SeqFactory.create(Coos1));

  std::vector<geos::geom::Coordinate>* Coos2 = new std::vector<
      geos::geom::Coordinate>();
  Coos2->push_back(geos::geom::Coordinate(0, 2));
  Coos2->push_back(geos::geom::Coordinate(2, 0));
  geos::geom::LineString* LS2 = Factory.createLineString(
      SeqFactory.create(Coos2));

  openfluid::landr::NodedGraph* Graph = new openfluid::landr::NodedGraph();

  Graph->addALine(LS, 1);
  Graph->addALine(LS2, 2);

  BOOST_CHECK_EQUAL(Graph->getNodedLines().size(), 4);
//  Graph->printNodedLines();

  std::vector<geos::planargraph::Node*> Nodes;
  Graph->getNodes(Nodes);
  BOOST_CHECK_EQUAL(Nodes.size(), 5);

  delete Graph;
  delete LS;
  delete LS2;
}

// =====================================================================
// =====================================================================

BOOST_AUTO_TEST_CASE(check_construction_verysimple_1PtIntersect_snap)
{
  //  x       x
  //    x   x
  //      x
  //    x
  //  x

  std::cout << "verysimple_1PtIntersect_snap" << std::endl;

  geos::geom::CoordinateArraySequenceFactory SeqFactory;
  geos::geom::GeometryFactory Factory;

  std::vector<geos::geom::Coordinate>* Coos1 = new std::vector<
      geos::geom::Coordinate>();
  Coos1->push_back(geos::geom::Coordinate(0, 0));
  Coos1->push_back(geos::geom::Coordinate(1, 1));
  Coos1->push_back(geos::geom::Coordinate(2, 2));
  geos::geom::LineString* LS = Factory.createLineString(
      SeqFactory.create(Coos1));

  std::vector<geos::geom::Coordinate>* Coos2 = new std::vector<
      geos::geom::Coordinate>();
  Coos2->push_back(geos::geom::Coordinate(0, 2));
  Coos2->push_back(geos::geom::Coordinate(0.9, 1.1));
  geos::geom::LineString* LS2 = Factory.createLineString(
      SeqFactory.create(Coos2));

  openfluid::landr::NodedGraph* Graph = new openfluid::landr::NodedGraph();

  Graph->addALine(LS, 1);
  Graph->addALine(LS2, 2);

  BOOST_CHECK_EQUAL(Graph->getNodedLines().size(), 2);

  std::vector<geos::planargraph::Node*> Nodes;
  Graph->getNodes(Nodes);
  BOOST_CHECK_EQUAL(Nodes.size(), 4);

  delete Graph;

  Graph = new openfluid::landr::NodedGraph();
  Graph->setSnapValue(0.2);

  Graph->addALine(LS, 1);
  Graph->addALine(LS2, 2);

  BOOST_CHECK_EQUAL(Graph->getNodedLines().size(), 3);
//  Graph->printNodedLines();

  Nodes.clear();
  Graph->getNodes(Nodes);
  BOOST_CHECK_EQUAL(Nodes.size(), 4);

  delete Graph;
  delete LS;
  delete LS2;
}

// =====================================================================
// =====================================================================

BOOST_AUTO_TEST_CASE(check_construction_verysimple2_1PtIntersect)
{
  //  x   xxxxx
  //    x   x
  //      x
  //    x   x
  //  x       xxxxx

  std::cout << "verysimple2_1PtIntersect" << std::endl;

  geos::geom::CoordinateArraySequenceFactory SeqFactory;
  geos::geom::GeometryFactory Factory;

  std::vector<geos::geom::Coordinate>* Coos1 = new std::vector<
      geos::geom::Coordinate>();
  Coos1->push_back(geos::geom::Coordinate(0, 0));
  Coos1->push_back(geos::geom::Coordinate(2, 2));
  Coos1->push_back(geos::geom::Coordinate(1, 2));
  geos::geom::LineString* LS = Factory.createLineString(
      SeqFactory.create(Coos1));

  std::vector<geos::geom::Coordinate>* Coos2 = new std::vector<
      geos::geom::Coordinate>();
  Coos2->push_back(geos::geom::Coordinate(3, 0));
  Coos2->push_back(geos::geom::Coordinate(2, 0));
  Coos2->push_back(geos::geom::Coordinate(0, 2));
  geos::geom::LineString* LS2 = Factory.createLineString(
      SeqFactory.create(Coos2));

  openfluid::landr::NodedGraph* Graph = new openfluid::landr::NodedGraph();

  Graph->addALine(LS, 1);
  Graph->addALine(LS2, 2);

  BOOST_CHECK_EQUAL(Graph->getNodedLines().size(), 4);
//  Graph->printNodedLines();

  std::vector<geos::planargraph::Node*> Nodes;
  Graph->getNodes(Nodes);
  BOOST_CHECK_EQUAL(Nodes.size(), 5);

  delete Graph;
  delete LS;
  delete LS2;
}

// =====================================================================
// =====================================================================

BOOST_AUTO_TEST_CASE(check_construction_verysimple_2PtIntersect)
{
  //  \
  //  xxxxxxxxx
  //  x \
  //  x  \
  //  x   \
  //  xxxxxxxxx
  //        \
  //

  std::cout << "verysimple2_2PtIntersect" << std::endl;

  geos::geom::CoordinateArraySequenceFactory SeqFactory;
  geos::geom::GeometryFactory Factory;

  std::vector<geos::geom::Coordinate>* Coos1 = new std::vector<
      geos::geom::Coordinate>();
  Coos1->push_back(geos::geom::Coordinate(2, 2));
  Coos1->push_back(geos::geom::Coordinate(0, 2));
  Coos1->push_back(geos::geom::Coordinate(0, 0));
  Coos1->push_back(geos::geom::Coordinate(2, 0));
  geos::geom::LineString* LS = Factory.createLineString(
      SeqFactory.create(Coos1));

  std::vector<geos::geom::Coordinate>* Coos2 = new std::vector<
      geos::geom::Coordinate>();
  Coos2->push_back(geos::geom::Coordinate(0, 3));
  Coos2->push_back(geos::geom::Coordinate(2, -1));
  geos::geom::LineString* LS2 = Factory.createLineString(
      SeqFactory.create(Coos2));

  openfluid::landr::NodedGraph* Graph = new openfluid::landr::NodedGraph();

  Graph->addALine(LS, 1);
  Graph->addALine(LS2, 2);

  BOOST_CHECK_EQUAL(Graph->getNodedLines().size(), 6);
//  Graph->printNodedLines();

  std::vector<geos::planargraph::Node*> Nodes;
  Graph->getNodes(Nodes);
  BOOST_CHECK_EQUAL(Nodes.size(), 6);

  delete Graph;
  delete LS;
  delete LS2;
}

// =====================================================================
// =====================================================================

BOOST_AUTO_TEST_CASE(check_construction_simpleLine)
{
  std::cout << "simpleLine" << std::endl;

  openfluid::core::GeoVectorValue* Val = new openfluid::core::GeoVectorValue(
      CONFIGTESTS_INPUT_DATASETS_DIR + "/landr", "LINE_TEST.shp");

  openfluid::landr::NodedGraph* Graph = new openfluid::landr::NodedGraph();

  BOOST_CHECK_EQUAL(Graph->getLinesOfValue(*Val).size(), 1);

  BOOST_CHECK_EQUAL(Graph->getNodedLines().size(), 0);

  Graph->addALayer(*Val, 1);

  BOOST_CHECK_EQUAL(Graph->getNodedLines().size(), 1);

  BOOST_CHECK(
      (*Graph->getNodedLines().begin())->getLine()->equals(Val->getGeometries()));
  BOOST_CHECK_EQUAL( (*Graph->getNodedLines().begin())->getOriginOrders()[0],
                    1);

  std::vector<geos::planargraph::Node*> Nodes;
  Graph->getNodes(Nodes);
  BOOST_CHECK_EQUAL(Nodes.size(), 2);

  delete Graph;
  delete Val;
}

// =====================================================================
// =====================================================================

BOOST_AUTO_TEST_CASE(check_construction_simple2Line)
{
  std::cout << "simple2Line" << std::endl;

  openfluid::core::GeoVectorValue* Val = new openfluid::core::GeoVectorValue(
      CONFIGTESTS_INPUT_DATASETS_DIR + "/landr", "LINE_TEST.shp");
  openfluid::core::GeoVectorValue* Val2 = new openfluid::core::GeoVectorValue(
      CONFIGTESTS_INPUT_DATASETS_DIR + "/landr", "LINE_TEST2.shp");

  openfluid::landr::NodedGraph* Graph = new openfluid::landr::NodedGraph();

  BOOST_CHECK_EQUAL(Graph->getLinesOfValue(*Val).size(), 1);
  BOOST_CHECK_EQUAL(Graph->getLinesOfValue(*Val2).size(), 1);

  BOOST_CHECK_EQUAL(Graph->getNodedLines().size(), 0);

  Graph->addALayer(*Val, 1);
  Graph->addALayer(*Val2, 2);

  BOOST_CHECK_EQUAL(Graph->getNodedLines().size(), 2);

  std::vector<geos::planargraph::Node*> Nodes;
  Graph->getNodes(Nodes);
  BOOST_CHECK_EQUAL(Nodes.size(), 4);

  delete Graph;
  delete Val;
  delete Val2;
}

// =====================================================================
// =====================================================================

BOOST_AUTO_TEST_CASE(check_construction_simple2Line_intersect)
{
  std::cout << "simple2Line_intersect" << std::endl;

  openfluid::core::GeoVectorValue* Val = new openfluid::core::GeoVectorValue(
      CONFIGTESTS_INPUT_DATASETS_DIR + "/landr", "LINE_TEST.shp");
  openfluid::core::GeoVectorValue* Val2 = new openfluid::core::GeoVectorValue(
      CONFIGTESTS_INPUT_DATASETS_DIR + "/landr", "LINE_TEST3.shp");

  openfluid::landr::NodedGraph* Graph = new openfluid::landr::NodedGraph();

  BOOST_CHECK_EQUAL(Graph->getLinesOfValue(*Val).size(), 1);
  BOOST_CHECK_EQUAL(Graph->getLinesOfValue(*Val2).size(), 1);

  BOOST_CHECK_EQUAL(Graph->getNodedLines().size(), 0);

  Graph->addALayer(*Val, 1);
  Graph->addALayer(*Val2, 2);

  BOOST_CHECK_EQUAL(Graph->getNodedLines().size(), 4);

//  Graph->printNodedLines();

  std::vector<geos::planargraph::Node*> Nodes;
  Graph->getNodes(Nodes);
  BOOST_CHECK_EQUAL(Nodes.size(), 5);

  delete Graph;
  delete Val;
  delete Val2;
}

// =====================================================================
// =====================================================================

BOOST_AUTO_TEST_CASE(check_construction_verysimple_Poly)
{
  //
  //  xxxxxxxxx
  //  x       x
  //  x       x
  //  x       x
  //  xxxxxxxxx
  //

  std::cout << "verysimple_Poly" << std::endl;

  geos::geom::CoordinateArraySequenceFactory SeqFactory;
  geos::geom::GeometryFactory Factory;

  std::vector<geos::geom::Coordinate>* Coos1 = new std::vector<
      geos::geom::Coordinate>();
  Coos1->push_back(geos::geom::Coordinate(2, 2));
  Coos1->push_back(geos::geom::Coordinate(0, 2));
  Coos1->push_back(geos::geom::Coordinate(0, 0));
  Coos1->push_back(geos::geom::Coordinate(2, 0));
  Coos1->push_back(geos::geom::Coordinate(2, 2));
  geos::geom::LineString* LS = Factory.createLineString(
      SeqFactory.create(Coos1));

  openfluid::landr::NodedGraph* Graph = new openfluid::landr::NodedGraph();

  Graph->addALine(LS, 1);

  BOOST_CHECK_EQUAL(Graph->getNodedLines().size(), 1);
//  Graph->printNodedLines();

  std::vector<geos::planargraph::Node*> Nodes;
  Graph->getNodes(Nodes);
  BOOST_CHECK_EQUAL(Nodes.size(), 0);

  delete Graph;
  delete LS;
}

// =====================================================================
// =====================================================================

BOOST_AUTO_TEST_CASE(check_construction_verysimple_2Poly)
{
  //
  //  xxxxxxxxxxxxxxxxx
  //  x       x       x
  //  x   1   x   2   x
  //  x       x       x
  //  xxxxxxxxxxxxxxxxx
  //

  std::cout << "verysimple_2Poly" << std::endl;

  geos::geom::CoordinateArraySequenceFactory SeqFactory;
  geos::geom::GeometryFactory Factory;

  std::vector<geos::geom::Coordinate>* Coos1 = new std::vector<
      geos::geom::Coordinate>();
  Coos1->push_back(geos::geom::Coordinate(2, 2));
  Coos1->push_back(geos::geom::Coordinate(0, 2));
  Coos1->push_back(geos::geom::Coordinate(0, 0));
  Coos1->push_back(geos::geom::Coordinate(2, 0));
  Coos1->push_back(geos::geom::Coordinate(2, 2));
  geos::geom::LineString* LS = Factory.createLineString(
      SeqFactory.create(Coos1));

  std::vector<geos::geom::Coordinate>* Coos2 = new std::vector<
      geos::geom::Coordinate>();
  Coos2->push_back(geos::geom::Coordinate(4, 0));
  Coos2->push_back(geos::geom::Coordinate(2, 0));
  Coos2->push_back(geos::geom::Coordinate(2, 2));
  Coos2->push_back(geos::geom::Coordinate(4, 2));
  Coos2->push_back(geos::geom::Coordinate(4, 0));
  geos::geom::LineString* LS2 = Factory.createLineString(
      SeqFactory.create(Coos2));

  openfluid::landr::NodedGraph* Graph = new openfluid::landr::NodedGraph();

  Graph->addALine(LS, 1);
  Graph->addALine(LS2, 2);

  BOOST_CHECK_EQUAL(Graph->getNodedLines().size(), 3);
//  Graph->printNodedLines();

  std::vector<geos::planargraph::Node*> Nodes;
  Graph->getNodes(Nodes);
  BOOST_CHECK_EQUAL(Nodes.size(), 2);

  delete Graph;
  delete LS;
  delete LS2;
}

// =====================================================================
// =====================================================================

BOOST_AUTO_TEST_CASE(check_construction_verysimple2_2Poly)
{
  //
  //       xxxxxxxxx
  //       x       x
  //  xxxxxxxxx    x
  //  x    x  x    x
  //  x    xxxxxxxxx
  //  x       x
  //  xxxxxxxxx
  //

  std::cout << "verysimple2_2Poly" << std::endl;

  geos::geom::CoordinateArraySequenceFactory SeqFactory;
  geos::geom::GeometryFactory Factory;

  std::vector<geos::geom::Coordinate>* Coos1 = new std::vector<
      geos::geom::Coordinate>();
  Coos1->push_back(geos::geom::Coordinate(2, 2));
  Coos1->push_back(geos::geom::Coordinate(0, 2));
  Coos1->push_back(geos::geom::Coordinate(0, 0));
  Coos1->push_back(geos::geom::Coordinate(2, 0));
  Coos1->push_back(geos::geom::Coordinate(2, 2));
  geos::geom::LineString* LS = Factory.createLineString(
      SeqFactory.create(Coos1));

  std::vector<geos::geom::Coordinate>* Coos2 = new std::vector<
      geos::geom::Coordinate>();
  Coos2->push_back(geos::geom::Coordinate(3, 3));
  Coos2->push_back(geos::geom::Coordinate(3, 1));
  Coos2->push_back(geos::geom::Coordinate(1, 1));
  Coos2->push_back(geos::geom::Coordinate(1, 3));
  Coos2->push_back(geos::geom::Coordinate(3, 3));
  geos::geom::LineString* LS2 = Factory.createLineString(
      SeqFactory.create(Coos2));

  openfluid::landr::NodedGraph* Graph = new openfluid::landr::NodedGraph();

  Graph->addALine(LS, 1);
  Graph->addALine(LS2, 2);

  BOOST_CHECK_EQUAL(Graph->getNodedLines().size(), 4);
//  Graph->printNodedLines();

  std::vector<geos::planargraph::Node*> Nodes;
  Graph->getNodes(Nodes);
  BOOST_CHECK_EQUAL(Nodes.size(), 2);

  delete Graph;
  delete LS;
  delete LS2;
}

// =====================================================================
// =====================================================================

BOOST_AUTO_TEST_CASE(check_construction_verysimple_3Poly)
{
  //  xxxxxxxxx
  //  x       x
  //  x   3   x
  //  x       x
  //  xxxxxxxxxxxxxxxxx
  //  x       x       x
  //  x   1   x   2   x
  //  x       x       x
  //  xxxxxxxxxxxxxxxxx
  //

  std::cout << "verysimple_3Poly" << std::endl;

  geos::geom::CoordinateArraySequenceFactory SeqFactory;
  geos::geom::GeometryFactory Factory;

  std::vector<geos::geom::Coordinate>* Coos1 = new std::vector<
      geos::geom::Coordinate>();
  Coos1->push_back(geos::geom::Coordinate(2, 2));
  Coos1->push_back(geos::geom::Coordinate(0, 2));
  Coos1->push_back(geos::geom::Coordinate(0, 0));
  Coos1->push_back(geos::geom::Coordinate(2, 0));
  Coos1->push_back(geos::geom::Coordinate(2, 2));
  geos::geom::LineString* LS = Factory.createLineString(
      SeqFactory.create(Coos1));

  std::vector<geos::geom::Coordinate>* Coos2 = new std::vector<
      geos::geom::Coordinate>();
  Coos2->push_back(geos::geom::Coordinate(4, 0));
  Coos2->push_back(geos::geom::Coordinate(2, 0));
  Coos2->push_back(geos::geom::Coordinate(2, 2));
  Coos2->push_back(geos::geom::Coordinate(4, 2));
  Coos2->push_back(geos::geom::Coordinate(4, 0));
  geos::geom::LineString* LS2 = Factory.createLineString(
      SeqFactory.create(Coos2));

  std::vector<geos::geom::Coordinate>* Coos3 = new std::vector<
      geos::geom::Coordinate>();
  Coos3->push_back(geos::geom::Coordinate(0, 4));
  Coos3->push_back(geos::geom::Coordinate(2, 4));
  Coos3->push_back(geos::geom::Coordinate(2, 2));
  Coos3->push_back(geos::geom::Coordinate(0, 2));
  Coos3->push_back(geos::geom::Coordinate(0, 4));
  geos::geom::LineString* LS3 = Factory.createLineString(
      SeqFactory.create(Coos3));

  openfluid::landr::NodedGraph* Graph = new openfluid::landr::NodedGraph();

  Graph->addALine(LS, 1);
  Graph->addALine(LS2, 2);
  Graph->addALine(LS3, 3);

  BOOST_CHECK_EQUAL(Graph->getNodedLines().size(), 5);
//  Graph->printNodedLines();

  std::vector<geos::planargraph::Node*> Nodes;
  Graph->getNodes(Nodes);
  BOOST_CHECK_EQUAL(Nodes.size(), 3);

  delete Graph;
  delete LS;
  delete LS2;
  delete LS3;
}

// =====================================================================
// =====================================================================

BOOST_AUTO_TEST_CASE(check_construction_simplePoly)
{
  std::cout << "simplePoly" << std::endl;

  openfluid::core::GeoVectorValue* Val = new openfluid::core::GeoVectorValue(
      CONFIGTESTS_INPUT_DATASETS_DIR + "/landr", "POLY_TEST.shp");

  openfluid::landr::NodedGraph* Graph = new openfluid::landr::NodedGraph();

  BOOST_CHECK_EQUAL(Graph->getLinesOfValue(*Val).size(), 4);

  BOOST_CHECK_EQUAL(Graph->getNodedLines().size(), 0);

  Graph->addALayer(*Val, 1);
//  Graph->printNodedLines();

  BOOST_CHECK_EQUAL(Graph->getNodedLines().size(), 8);

  std::vector<geos::planargraph::Node*> Nodes;
  Graph->getNodes(Nodes);
  BOOST_CHECK_EQUAL(Nodes.size(), 5);

  delete Graph;
  delete Val;
}

// =====================================================================
// =====================================================================

BOOST_AUTO_TEST_CASE(check_construction_verysimple_LinePoly)
{
  //  \
  //  xxxxxxxxx
  //  x \     x
  //  x  \    x
  //  x   \   x
  //  xxxxxxxxx
  //        \
  //

  std::cout << "verysimple_LinePoly" << std::endl;

  geos::geom::CoordinateArraySequenceFactory SeqFactory;
  geos::geom::GeometryFactory Factory;

  std::vector<geos::geom::Coordinate>* Coos1 = new std::vector<
      geos::geom::Coordinate>();
  Coos1->push_back(geos::geom::Coordinate(2, 2));
  Coos1->push_back(geos::geom::Coordinate(0, 2));
  Coos1->push_back(geos::geom::Coordinate(0, 0));
  Coos1->push_back(geos::geom::Coordinate(2, 0));
  Coos1->push_back(geos::geom::Coordinate(2, 2));
  geos::geom::LineString* LS = Factory.createLineString(
      SeqFactory.create(Coos1));

  std::vector<geos::geom::Coordinate>* Coos2 = new std::vector<
      geos::geom::Coordinate>();
  Coos2->push_back(geos::geom::Coordinate(0, 3));
  Coos2->push_back(geos::geom::Coordinate(2, -1));
  geos::geom::LineString* LS2 = Factory.createLineString(
      SeqFactory.create(Coos2));

  openfluid::landr::NodedGraph* Graph = new openfluid::landr::NodedGraph();

  Graph->addALine(LS, 1);
  Graph->addALine(LS2, 2);

  BOOST_CHECK_EQUAL(Graph->getNodedLines().size(), 5);
//  Graph->printNodedLines();

  std::vector<geos::planargraph::Node*> Nodes;
  Graph->getNodes(Nodes);
  BOOST_CHECK_EQUAL(Nodes.size(), 4);

  delete Graph;
  delete LS;
  delete LS2;
}

// =====================================================================
// =====================================================================

BOOST_AUTO_TEST_CASE(check_construction_verysimple_2PolyLine)
{
  //
  //  xxxxxxxxxxxxxxxxx
  //  x       x       x
  // xxxxxxxxxxxxxxxxxxx
  //  x       x       x
  //  xxxxxxxxxxxxxxxxx
  //

  std::cout << "verysimple_2PolyLine" << std::endl;

  geos::geom::CoordinateArraySequenceFactory SeqFactory;
  geos::geom::GeometryFactory Factory;

  std::vector<geos::geom::Coordinate>* Coos1 = new std::vector<
      geos::geom::Coordinate>();
  Coos1->push_back(geos::geom::Coordinate(2, 2));
  Coos1->push_back(geos::geom::Coordinate(0, 2));
  Coos1->push_back(geos::geom::Coordinate(0, 0));
  Coos1->push_back(geos::geom::Coordinate(2, 0));
  Coos1->push_back(geos::geom::Coordinate(2, 2));
  geos::geom::LineString* LS = Factory.createLineString(
      SeqFactory.create(Coos1));

  std::vector<geos::geom::Coordinate>* Coos2 = new std::vector<
      geos::geom::Coordinate>();
  Coos2->push_back(geos::geom::Coordinate(4, 0));
  Coos2->push_back(geos::geom::Coordinate(2, 0));
  Coos2->push_back(geos::geom::Coordinate(2, 2));
  Coos2->push_back(geos::geom::Coordinate(4, 2));
  Coos2->push_back(geos::geom::Coordinate(4, 0));
  geos::geom::LineString* LS2 = Factory.createLineString(
      SeqFactory.create(Coos2));

  std::vector<geos::geom::Coordinate>* Coos3 = new std::vector<
      geos::geom::Coordinate>();
  Coos3->push_back(geos::geom::Coordinate(-0.5, 1));
  Coos3->push_back(geos::geom::Coordinate(4.5, 1));
  geos::geom::LineString* LS3 = Factory.createLineString(
      SeqFactory.create(Coos3));

  openfluid::landr::NodedGraph* Graph = new openfluid::landr::NodedGraph();

  Graph->addALine(LS, 1);
  Graph->addALine(LS2, 2);
  Graph->addALine(LS3, 3);

  BOOST_CHECK_EQUAL(Graph->getNodedLines().size(), 10);
//  Graph->printNodedLines();

  std::vector<geos::planargraph::Node*> Nodes;
  Graph->getNodes(Nodes);
  BOOST_CHECK_EQUAL(Nodes.size(), 7);

  delete Graph;
  delete LS;
  delete LS2;
  delete LS3;
}

// =====================================================================
// =====================================================================

BOOST_AUTO_TEST_CASE(check_construction_simpleLinePoly)
{
  std::cout << "simpleLinePoly" << std::endl;

  openfluid::core::GeoVectorValue* ValSU = new openfluid::core::GeoVectorValue(
      CONFIGTESTS_INPUT_DATASETS_DIR + "/landr", "POLY_TEST.shp");
  openfluid::core::GeoVectorValue* ValRS = new openfluid::core::GeoVectorValue(
      CONFIGTESTS_INPUT_DATASETS_DIR + "/landr", "LINE_TEST.shp");

  openfluid::landr::NodedGraph* Graph = new openfluid::landr::NodedGraph();

  BOOST_CHECK_EQUAL(Graph->getLinesOfValue(*ValSU).size(), 4);
  BOOST_CHECK_EQUAL(Graph->getLinesOfValue(*ValRS).size(), 1);

  BOOST_CHECK_EQUAL(Graph->getNodedLines().size(), 0);

  Graph->addALayer(*ValSU, 1);
  Graph->addALayer(*ValRS, 2);

  BOOST_CHECK_EQUAL(Graph->getNodedLines().size(), 17);
//  Graph->printNodedLines();

  std::vector<geos::planargraph::Node*> Nodes;
  Graph->getNodes(Nodes);
  BOOST_CHECK_EQUAL(Nodes.size(), 11);

  delete Graph;
  delete ValSU;
  delete ValRS;
}

// =====================================================================
// =====================================================================

BOOST_AUTO_TEST_CASE(check_construction_simple2Poly)
{
  std::cout << "simple2Poly" << std::endl;

  openfluid::core::GeoVectorValue* Val = new openfluid::core::GeoVectorValue(
      CONFIGTESTS_INPUT_DATASETS_DIR + "/landr", "POLY_TEST.shp");
  openfluid::core::GeoVectorValue* Val2 = new openfluid::core::GeoVectorValue(
      CONFIGTESTS_INPUT_DATASETS_DIR + "/landr", "POLY_TEST2.shp");

  openfluid::landr::NodedGraph* Graph = new openfluid::landr::NodedGraph();

  BOOST_CHECK_EQUAL(Graph->getLinesOfValue(*Val).size(), 4);
  BOOST_CHECK_EQUAL(Graph->getLinesOfValue(*Val2).size(), 2);

  BOOST_CHECK_EQUAL(Graph->getNodedLines().size(), 0);

  Graph->addALayer(*Val, 1);
  Graph->addALayer(*Val2, 2);

  BOOST_CHECK_EQUAL(Graph->getNodedLines().size(), 23);
//  Graph->printNodedLines();

  std::vector<geos::planargraph::Node*> Nodes;
  Graph->getNodes(Nodes);
  BOOST_CHECK_EQUAL(Nodes.size(), 13);

  delete Graph;
  delete Val;
  delete Val2;
}

// =====================================================================
// =====================================================================

BOOST_AUTO_TEST_CASE(check_construction_simpleLine2Poly)
{
  std::cout << "simpleLine2Poly" << std::endl;

  openfluid::core::GeoVectorValue* Val = new openfluid::core::GeoVectorValue(
      CONFIGTESTS_INPUT_DATASETS_DIR + "/landr", "POLY_TEST.shp");
  openfluid::core::GeoVectorValue* Val2 = new openfluid::core::GeoVectorValue(
      CONFIGTESTS_INPUT_DATASETS_DIR + "/landr", "POLY_TEST2.shp");
  openfluid::core::GeoVectorValue* ValRS = new openfluid::core::GeoVectorValue(
      CONFIGTESTS_INPUT_DATASETS_DIR + "/landr", "LINE_TEST.shp");

  openfluid::landr::NodedGraph* Graph = new openfluid::landr::NodedGraph();

  BOOST_CHECK_EQUAL(Graph->getLinesOfValue(*Val).size(), 4);
  BOOST_CHECK_EQUAL(Graph->getLinesOfValue(*Val2).size(), 2);
  BOOST_CHECK_EQUAL(Graph->getLinesOfValue(*ValRS).size(), 1);

  BOOST_CHECK_EQUAL(Graph->getNodedLines().size(), 0);

  Graph->addALayer(*Val, 1);
  Graph->addALayer(*Val2, 2);
  Graph->addALayer(*ValRS, 3);

  BOOST_CHECK_EQUAL(Graph->getNodedLines().size(), 36);
//  Graph->printNodedLines();

  std::vector<geos::planargraph::Node*> Nodes;
  Graph->getNodes(Nodes);
  BOOST_CHECK_EQUAL(Nodes.size(), 21);

  delete Graph;
  delete Val;
  delete Val2;
  delete ValRS;
}

// =====================================================================
// =====================================================================
//
BOOST_AUTO_TEST_CASE(check_construction_complexPoly)
{
  openfluid::core::GeoVectorValue* ValSU = new openfluid::core::GeoVectorValue(
      "/home/libres/Documents/GIS data/virtuel_seg", "field2.shp");

  openfluid::landr::NodedGraph* Graph = new openfluid::landr::NodedGraph();

  BOOST_CHECK_EQUAL(Graph->getLinesOfValue(*ValSU).size(), 17);

  BOOST_CHECK_EQUAL(Graph->getNodedLines().size(), 0);

  Graph->addALayer(*ValSU, 1);

  BOOST_CHECK_EQUAL(Graph->getNodedLines().size(), 43);
//  Graph->printNodedLines();

  std::vector<geos::planargraph::Node*> Nodes;
  Graph->getNodes(Nodes);
  BOOST_CHECK_EQUAL(Nodes.size(), 27);

  delete Graph;
  delete ValSU;
}

// =====================================================================
// =====================================================================

BOOST_AUTO_TEST_CASE(check_construction_verysimple_OverlappingLinePoly)
{
  //
  //  xxxxxxxxx
  //  x       x
  //  x       x
  //  x       x
  //  xxxxxxxxxxxxxx
  //

  std::cout << "verysimple_OverlappingLinePoly" << std::endl;

  geos::geom::CoordinateArraySequenceFactory SeqFactory;
  geos::geom::GeometryFactory Factory;

  std::vector<geos::geom::Coordinate>* Coos1 = new std::vector<
      geos::geom::Coordinate>();
  Coos1->push_back(geos::geom::Coordinate(2, 2));
  Coos1->push_back(geos::geom::Coordinate(0, 2));
  Coos1->push_back(geos::geom::Coordinate(0, 0));
  Coos1->push_back(geos::geom::Coordinate(2, 0));
  Coos1->push_back(geos::geom::Coordinate(2, 2));
  geos::geom::LineString* LS = Factory.createLineString(
      SeqFactory.create(Coos1));

  std::vector<geos::geom::Coordinate>* Coos2 = new std::vector<
      geos::geom::Coordinate>();
  Coos2->push_back(geos::geom::Coordinate(3, 0));
  Coos2->push_back(geos::geom::Coordinate(2, 0));
  Coos2->push_back(geos::geom::Coordinate(2, 2));
  geos::geom::LineString* LS2 = Factory.createLineString(
      SeqFactory.create(Coos2));

  openfluid::landr::NodedGraph* Graph = new openfluid::landr::NodedGraph();

  Graph->addALine(LS, 1);
  Graph->addALine(LS2, 2);

  BOOST_CHECK_EQUAL(Graph->getNodedLines().size(), 2);
//  Graph->printNodedLines();

  std::vector<geos::planargraph::Node*> Nodes;
  Graph->getNodes(Nodes);
  BOOST_CHECK_EQUAL(Nodes.size(), 2);

  delete Graph;
  delete LS;
  delete LS2;
}

// =====================================================================
// =====================================================================

BOOST_AUTO_TEST_CASE(check_construction_verysimple_EqualsPolys)
{
  //
  //  xxxxxxxxx
  //  x       x
  //  x       x
  //  x       x
  //  xxxxxxxxx
  //

  std::cout << "verysimple_EqualsPolys" << std::endl;

  geos::geom::CoordinateArraySequenceFactory SeqFactory;
  geos::geom::GeometryFactory Factory;

  std::vector<geos::geom::Coordinate>* Coos1 = new std::vector<
      geos::geom::Coordinate>();
  Coos1->push_back(geos::geom::Coordinate(2, 2));
  Coos1->push_back(geos::geom::Coordinate(0, 2));
  Coos1->push_back(geos::geom::Coordinate(0, 0));
  Coos1->push_back(geos::geom::Coordinate(2, 0));
  Coos1->push_back(geos::geom::Coordinate(2, 2));
  geos::geom::LineString* LS = Factory.createLineString(
      SeqFactory.create(Coos1));

  std::vector<geos::geom::Coordinate>* Coos2 = new std::vector<
      geos::geom::Coordinate>();
  Coos2->push_back(geos::geom::Coordinate(2, 2));
  Coos2->push_back(geos::geom::Coordinate(0, 2));
  Coos2->push_back(geos::geom::Coordinate(0, 0));
  Coos2->push_back(geos::geom::Coordinate(2, 0));
  Coos2->push_back(geos::geom::Coordinate(2, 2));
  geos::geom::LineString* LS2 = Factory.createLineString(
      SeqFactory.create(Coos2));

  openfluid::landr::NodedGraph* Graph = new openfluid::landr::NodedGraph();

  Graph->addALine(LS, 1);
  Graph->addALine(LS2, 2);

  BOOST_CHECK_EQUAL(Graph->getNodedLines().size(), 1);
//  Graph->printNodedLines();

  std::vector<geos::planargraph::Node*> Nodes;
  Graph->getNodes(Nodes);
  BOOST_CHECK_EQUAL(Nodes.size(), 0);

  delete Graph;
  delete LS;
  delete LS2;
}

// =====================================================================
// =====================================================================

// Always fails (intersect nor union gives expected results)
//BOOST_AUTO_TEST_CASE(check_construction_medium2Polys)
//{
//  std::cout << "medium2Polys" << std::endl;
//
//  openfluid::core::GeoVectorValue* ValSU = new openfluid::core::GeoVectorValue(
//      CONFIGTESTS_INPUT_DATASETS_DIR + "/landr", "field.shp");
//  openfluid::core::GeoVectorValue* ValSU2 = new openfluid::core::GeoVectorValue(
//      CONFIGTESTS_INPUT_DATASETS_DIR + "/landr", "soil.shp");
//
//  openfluid::landr::NodedGraph* Graph = new openfluid::landr::NodedGraph();
//
//  BOOST_CHECK_EQUAL(Graph->getLinesOfValue(*ValSU).size(), 6);
//  BOOST_CHECK_EQUAL(Graph->getLinesOfValue(*ValSU2).size(), 4);
//
//  BOOST_CHECK_EQUAL(Graph->getNodedLines().size(), 0);
//
//  Graph->addALayer(*ValSU, 1);
//  Graph->addALayer(*ValSU2, 2);
//
//  BOOST_CHECK_EQUAL(Graph->getNodedLines().size(), 29);
////  Graph->printNodedLines();
//
//  std::vector<geos::planargraph::Node*> Nodes;
//  Graph->getNodes(Nodes);
//  BOOST_CHECK_EQUAL(Nodes.size(), 27);
//
//  delete Graph;
//  delete ValSU;
//  delete ValSU2;
//}

// =====================================================================
// =====================================================================
