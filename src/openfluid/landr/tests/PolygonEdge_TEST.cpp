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
 \file PolygonEdge_TEST.cpp
 \brief Implements ...

 \author Aline LIBRES <aline.libres@gmail.com>
 */

#define BOOST_TEST_MAIN
#define BOOST_AUTO_TEST_MAIN
#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MODULE unittest_polygonedge
#include <boost/test/unit_test.hpp>
#include <boost/test/auto_unit_test.hpp>
#include <boost/filesystem/path.hpp>
#include <tests-config.hpp>
#include <openfluid/landr/PolygonEdge.hpp>
#include <openfluid/landr/PolygonEntity.hpp>
#include <geos/geom/LineString.h>
#include <geos/geom/Polygon.h>
#include <geos/geom/CoordinateArraySequenceFactory.h>
#include <geos/geom/GeometryFactory.h>

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
  geos::geom::Polygon* P = Factory.createPolygon(LR, NULL);
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
  geos::geom::Polygon* WrongP = Factory.createPolygon(WrongLR, NULL);
  openfluid::landr::PolygonEntity WrongEntity(WrongP, 0);

  BOOST_CHECK(!Edge.isLineInFace(WrongEntity));
}

// =====================================================================
// =====================================================================

