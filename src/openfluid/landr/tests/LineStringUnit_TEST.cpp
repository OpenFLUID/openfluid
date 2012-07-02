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
 \file LineStringUnit_TEST.cpp
 \brief Implements ...

 \author Aline LIBRES <aline.libres@gmail.com>
 */

#define BOOST_TEST_MAIN
#define BOOST_AUTO_TEST_MAIN
#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MODULE unittest_linestringunit
#include <boost/test/unit_test.hpp>
#include <boost/test/auto_unit_test.hpp>
#include <boost/filesystem/path.hpp>
#include <tests-config.hpp>
#include <openfluid/base/OFException.hpp>
#include <openfluid/core/GeoVectorValue.hpp>
#include <openfluid/landr/LineStringUnit.hpp>
#include <openfluid/landr/LineStringGraph.hpp>
#include <geos/planargraph/Node.h>
#include <geos/geom/Geometry.h>
#include <geos/geom/LineString.h>
#include <geos/geom/Point.h>

// =====================================================================
// =====================================================================

BOOST_AUTO_TEST_CASE(check_construction)
{
  openfluid::core::GeoVectorValue* Val = new openfluid::core::GeoVectorValue(
      CONFIGTESTS_INPUT_DATASETS_DIR + "/landr", "RS.shp");

  OGRFeature* FirstFeature = Val->getLayer0()->GetFeature(0);

  OGRGeometry* OGRGeom = FirstFeature->GetGeometryRef();

  geos::geom::Geometry* GeosGeom =
      (geos::geom::Geometry*) OGRGeom->exportToGEOS();

  openfluid::landr::LineStringUnit* Unit = new openfluid::landr::LineStringUnit(
      dynamic_cast<geos::geom::LineString*>(GeosGeom->clone()),
      FirstFeature->Clone());

  BOOST_CHECK_EQUAL(Val->getType(),
                    openfluid::core::UnstructuredValue::GeoVectorValue);

  BOOST_CHECK(Unit->getLine()->equals(GeosGeom));

  BOOST_CHECK(Unit->getFeature()->Equal(FirstFeature));

  BOOST_CHECK_EQUAL(Unit->getSelfId(), 5);

  OGRFeature::DestroyFeature(FirstFeature);
  delete Unit;
  delete Val;
}

// =====================================================================
// =====================================================================

BOOST_AUTO_TEST_CASE(check_copy)
{
  openfluid::core::GeoVectorValue* Val = new openfluid::core::GeoVectorValue(
      CONFIGTESTS_INPUT_DATASETS_DIR + "/landr", "RS.shp");

  OGRFeature* FirstFeature = Val->getLayer0()->GetFeature(0);
  OGRGeometry* OGRGeom = FirstFeature->GetGeometryRef();

  geos::geom::Geometry* GeosGeom =
      (geos::geom::Geometry*) OGRGeom->exportToGEOS();

  openfluid::landr::LineStringUnit* Unit = new openfluid::landr::LineStringUnit(
      dynamic_cast<geos::geom::LineString*>(GeosGeom->clone()),
      FirstFeature->Clone());

  OGRFeature::DestroyFeature(FirstFeature);
  delete GeosGeom;
  delete Val;

  openfluid::landr::LineStringUnit* CopyUnit =
      new openfluid::landr::LineStringUnit(*Unit);

  BOOST_CHECK(Unit->getLine()->equals(CopyUnit->getLine()));
  BOOST_CHECK_EQUAL(Unit->getSelfId(), CopyUnit->getSelfId());
  BOOST_CHECK_EQUAL(Unit->getFeature()->GetFieldCount(),
                    CopyUnit->getFeature()->GetFieldCount());

  std::string UnitLineStr = Unit->getLine()->toString();

  delete Unit;

  std::string CopyUnitLineStr = CopyUnit->getLine()->toString();

  BOOST_CHECK_EQUAL(UnitLineStr, CopyUnitLineStr);

  delete CopyUnit;
}

// =====================================================================
// =====================================================================

BOOST_AUTO_TEST_CASE(check_nodes)
{
  openfluid::core::GeoVectorValue* Val = new openfluid::core::GeoVectorValue(
      CONFIGTESTS_INPUT_DATASETS_DIR + "/landr", "RS.shp");

  openfluid::landr::LineStringGraph* Graph =
      new openfluid::landr::LineStringGraph(*Val);

  openfluid::landr::LineStringUnit* U1 = Graph->getUnit(1);
  openfluid::landr::LineStringUnit* U2 = Graph->getUnit(2);
  openfluid::landr::LineStringUnit* U3 = Graph->getUnit(3);
  openfluid::landr::LineStringUnit* U8 = Graph->getUnit(8);
  openfluid::landr::LineStringUnit* U7 = Graph->getUnit(7);

  BOOST_CHECK(
      U1->getStartNode()->getCoordinate().equals(U2->getEndNode()->getCoordinate()));
  BOOST_CHECK(
      U2->getStartNode()->getCoordinate().equals(U3->getEndNode()->getCoordinate()));
  BOOST_CHECK(
      U2->getStartNode()->getCoordinate().equals(U8->getEndNode()->getCoordinate()));
  BOOST_CHECK(
      U2->getStartNode()->getCoordinate().equals(U7->getEndNode()->getCoordinate()));

  delete Graph;
  delete Val;
}

// =====================================================================
// =====================================================================

BOOST_AUTO_TEST_CASE(check_neighbours)
{
  openfluid::core::GeoVectorValue* Val = new openfluid::core::GeoVectorValue(
      CONFIGTESTS_INPUT_DATASETS_DIR + "/landr", "RS.shp");

  openfluid::landr::LineStringGraph* Graph =
      new openfluid::landr::LineStringGraph(*Val);

  openfluid::landr::LineStringUnit* U1 = Graph->getUnit(1);
  openfluid::landr::LineStringUnit* U2 = Graph->getUnit(2);
  openfluid::landr::LineStringUnit* U3 = Graph->getUnit(3);
  openfluid::landr::LineStringUnit* U8 = Graph->getUnit(8);
  openfluid::landr::LineStringUnit* U4 = Graph->getUnit(4);

  std::vector<openfluid::landr::LineStringUnit*> U1Down =
      U1->getDownNeighbours();
  std::vector<openfluid::landr::LineStringUnit*> U2Down =
      U2->getDownNeighbours();
  std::vector<openfluid::landr::LineStringUnit*> U3Down =
      U3->getDownNeighbours();
  std::vector<openfluid::landr::LineStringUnit*> U8Down =
      U8->getDownNeighbours();
  std::vector<openfluid::landr::LineStringUnit*> U4Down =
      U4->getDownNeighbours();

  std::vector<openfluid::landr::LineStringUnit*> U1Up = U1->getUpNeighbours();
  std::vector<openfluid::landr::LineStringUnit*> U2Up = U2->getUpNeighbours();
  std::vector<openfluid::landr::LineStringUnit*> U3Up = U3->getUpNeighbours();
  std::vector<openfluid::landr::LineStringUnit*> U8Up = U8->getUpNeighbours();
  std::vector<openfluid::landr::LineStringUnit*> U4Up = U4->getUpNeighbours();

  BOOST_CHECK_EQUAL(U1Down.size(), 0);
  BOOST_CHECK_EQUAL(U2Down.size(), 1);
  BOOST_CHECK_EQUAL((*U2Down.begin())->getSelfId(), 1);
  BOOST_CHECK_EQUAL(U3Down.size(), 1);
  BOOST_CHECK_EQUAL((*U3Down.begin())->getSelfId(), 2);
  BOOST_CHECK_EQUAL(U8Down.size(), 1);
  BOOST_CHECK_EQUAL((*U8Down.begin())->getSelfId(), 2);
  BOOST_CHECK_EQUAL(U4Down.size(), 1);
  BOOST_CHECK_EQUAL((*U4Down.begin())->getSelfId(), 3);

  BOOST_CHECK_EQUAL(U1Up.size(), 1);
  BOOST_CHECK_EQUAL((*U1Up.begin())->getSelfId(), 2);
  BOOST_CHECK_EQUAL(U2Up.size(), 3);
  BOOST_CHECK_EQUAL(U3Up.size(), 2);
  BOOST_CHECK_EQUAL(U8Up.size(), 0);
  BOOST_CHECK_EQUAL(U4Up.size(), 1);
  BOOST_CHECK_EQUAL((*U4Up.begin())->getSelfId(), 5);

  delete Graph;
  delete Val;
}

// =====================================================================
// =====================================================================

