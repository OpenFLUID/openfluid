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
  @file PolygonEdge_TEST.cpp

  @author Aline LIBRES <aline.libres@gmail.com>
  @author Michael RABOTIN <michael.rabotin@supagro.inra.fr>
 */

#define BOOST_TEST_NO_MAIN
#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MODULE unittest_polygonedge
#include <boost/test/unit_test.hpp>
#include <boost/test/auto_unit_test.hpp>
#include <tests-config.hpp>
#include <openfluid/landr/PolygonEdge.hpp>
#include <openfluid/landr/PolygonEntity.hpp>
#include <geos/geom/LineString.h>
#include <geos/geom/Polygon.h>
#include <geos/geom/CoordinateArraySequenceFactory.h>
#include <geos/geom/GeometryFactory.h>
#include <openfluid/core/IntegerValue.hpp>
#include <openfluid/core/StringValue.hpp>
#include <openfluid/base/Environment.hpp>


// =====================================================================
// =====================================================================


BOOST_AUTO_TEST_CASE(check_isLineInFace)
{
  // *********
  // *       *
  // *       **
  // *       **
  // X*********

  geos::geom::CoordinateArraySequenceFactory SeqFactory;
  geos::geom::GeometryFactory Factory;

  std::vector<geos::geom::Coordinate>* CoosLS = new std::vector<
      geos::geom::Coordinate>();
  CoosLS->push_back(geos::geom::Coordinate(2, 0));
  CoosLS->push_back(geos::geom::Coordinate(2, 1));
  geos::geom::LineString* LS = Factory.createLineString(
      SeqFactory.create(CoosLS));
  openfluid::landr::PolygonEdge Edge(*LS);

  std::vector<geos::geom::Coordinate>* CoosPoly = new std::vector<
      geos::geom::Coordinate>();
  CoosPoly->push_back(geos::geom::Coordinate(0, 0));
  CoosPoly->push_back(geos::geom::Coordinate(0, 2));
  CoosPoly->push_back(geos::geom::Coordinate(2, 2));
  CoosPoly->push_back(geos::geom::Coordinate(2, 0));
  CoosPoly->push_back(geos::geom::Coordinate(0, 0));
  geos::geom::LinearRing* LR = Factory.createLinearRing(
      SeqFactory.create(CoosPoly));
  geos::geom::Polygon* P = Factory.createPolygon(LR, nullptr);
  openfluid::landr::PolygonEntity Entity(P, 0);

  BOOST_CHECK(Edge.isLineInFace(Entity));

  std::vector<geos::geom::Coordinate>* CoosWrongPoly = new std::vector<
      geos::geom::Coordinate>();
  CoosWrongPoly->push_back(geos::geom::Coordinate(0, 0));
  CoosWrongPoly->push_back(geos::geom::Coordinate(0, 2));
  CoosWrongPoly->push_back(geos::geom::Coordinate(1, 2));
  CoosWrongPoly->push_back(geos::geom::Coordinate(1, 0));
  CoosWrongPoly->push_back(geos::geom::Coordinate(0, 0));
  geos::geom::LinearRing* WrongLR = Factory.createLinearRing(
      SeqFactory.create(CoosWrongPoly));
  geos::geom::Polygon* WrongP = Factory.createPolygon(WrongLR, nullptr);
  openfluid::landr::PolygonEntity WrongEntity(WrongP, 0);

  BOOST_CHECK(!Edge.isLineInFace(WrongEntity));
}


// =====================================================================
// =====================================================================


BOOST_AUTO_TEST_CASE(check_addRemoveAttribute)
{
  // *********
  // *       *
  // *       **
  // *       **
  // X*********

  geos::geom::CoordinateArraySequenceFactory SeqFactory;
  geos::geom::GeometryFactory Factory;

  std::vector<geos::geom::Coordinate>* CoosLS = new std::vector<
      geos::geom::Coordinate>();
  CoosLS->push_back(geos::geom::Coordinate(2, 0));
  CoosLS->push_back(geos::geom::Coordinate(2, 1));
  geos::geom::LineString* LS = Factory.createLineString(
      SeqFactory.create(CoosLS));
  openfluid::landr::PolygonEdge Edge(*LS);

  std::vector<geos::geom::Coordinate>* CoosPoly = new std::vector<
      geos::geom::Coordinate>();
  CoosPoly->push_back(geos::geom::Coordinate(0, 0));
  CoosPoly->push_back(geos::geom::Coordinate(0, 2));
  CoosPoly->push_back(geos::geom::Coordinate(2, 2));
  CoosPoly->push_back(geos::geom::Coordinate(2, 0));
  CoosPoly->push_back(geos::geom::Coordinate(0, 0));
  geos::geom::LinearRing* LR = Factory.createLinearRing(
      SeqFactory.create(CoosPoly));
  geos::geom::Polygon* P = Factory.createPolygon(LR, nullptr);
  openfluid::landr::PolygonEntity Entity(P, 0);


  BOOST_CHECK(
      !Edge.setAttributeValue("att",new openfluid::core::IntegerValue(9999)));

  Edge.m_EdgeAttributes["att"]=0;

  Edge.setAttributeValue("att",new openfluid::core::IntegerValue(123));
  BOOST_CHECK(
      Edge.setAttributeValue("att",new openfluid::core::IntegerValue(123)));
  openfluid::core::IntegerValue Val;
  Edge.getAttributeValue("att",Val);
  BOOST_CHECK_EQUAL(Val.get(),123);


  Edge.setAttributeValue("att",new openfluid::core::StringValue("val"));
  BOOST_CHECK(
      Edge.setAttributeValue("att",new openfluid::core::StringValue("val")));
  openfluid::core::StringValue StrVal;
  Edge.getAttributeValue("att",StrVal);
  BOOST_CHECK_EQUAL(StrVal.get(),"val");

  Edge.removeAttribute("att");
  BOOST_CHECK(!Edge.getAttributeValue("att",StrVal));
  BOOST_CHECK((Edge.m_EdgeAttributes).empty());


}


// =====================================================================
// =====================================================================


int main(int argc, char *argv[])
{
  openfluid::base::Environment::init();

  return ::boost::unit_test::unit_test_main( &init_unit_test, argc, argv );
}
