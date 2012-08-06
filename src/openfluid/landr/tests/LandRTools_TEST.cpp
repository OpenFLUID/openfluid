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
 \file LandRTools_TEST.cpp
 \brief Implements ...

 \author Aline LIBRES <aline.libres@gmail.com>
 */

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
 \file LandRTools_TEST.cpp
 \brief Implements ...

 \author Aline LIBRES <aline.libres@gmail.com>
 */

#define BOOST_TEST_MAIN
#define BOOST_AUTO_TEST_MAIN
#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MODULE unittest_landrtools
#include <boost/test/unit_test.hpp>
#include <boost/test/auto_unit_test.hpp>
#include <boost/filesystem/path.hpp>
#include <tests-config.hpp>
#include <openfluid/landr/LandRTools.hpp>
#include <geos/geom/Geometry.h>
#include <geos/geom/LineString.h>
#include <geos/geom/CoordinateArraySequenceFactory.h>
#include <geos/geom/GeometryFactory.h>
#include <geos/geom/MultiLineString.h>

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
      openfluid::landr::LandRTools::getMergedLineStringsFromGeometry(MLS);

  BOOST_CHECK_EQUAL(MergedLSVect->size(), 1);

  geos::geom::LineString* MergedLS =
      openfluid::landr::LandRTools::getMergedLineStringFromGeometry(MLS);

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
      openfluid::landr::LandRTools::getMergedLineStringsFromGeometry(MLS);

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
      openfluid::landr::LandRTools::getMergedLineStringFromGeometry(MLS);

  BOOST_CHECK(!MergedLS);

  delete MLS;
  delete MergedLSVect;
  delete MergedLS1;
  delete MergedLS2;
}

// =====================================================================
// =====================================================================
