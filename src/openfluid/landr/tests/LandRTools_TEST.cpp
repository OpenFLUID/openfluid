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
#include <openfluid/core/GeoVectorValue.hpp>
#include <openfluid/base/OFException.hpp>
#include <openfluid/landr/LandRTools.hpp>
#include <geos/geom/Geometry.h>
#include <geos/geom/LineString.h>
#include <geos/geom/Polygon.h>
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

BOOST_AUTO_TEST_CASE(check_getVectorOfExteriorRings)
{
  openfluid::core::GeoVectorValue* Val = new openfluid::core::GeoVectorValue(
      CONFIGTESTS_INPUT_DATASETS_DIR + "/landr", "RS.shp");

  BOOST_CHECK_THROW(
      openfluid::landr::LandRTools::getVectorOfExteriorRings(*Val),
      openfluid::base::OFException);

  delete Val;

  Val = new openfluid::core::GeoVectorValue(
      CONFIGTESTS_INPUT_DATASETS_DIR + "/landr", "SU.shp");

  std::vector<geos::geom::Geometry*> Geoms =
      openfluid::landr::LandRTools::getVectorOfExteriorRings(*Val);

  BOOST_CHECK_EQUAL(Geoms.size(), 24);

  delete Val;
}

// =====================================================================
// =====================================================================

BOOST_AUTO_TEST_CASE(check_getVectorOfLines)
{
  openfluid::core::GeoVectorValue* Val = new openfluid::core::GeoVectorValue(
      CONFIGTESTS_INPUT_DATASETS_DIR + "/landr", "SU.shp");

  BOOST_CHECK_THROW( openfluid::landr::LandRTools::getVectorOfLines(*Val),
                    openfluid::base::OFException);

  delete Val;

  Val = new openfluid::core::GeoVectorValue(
      CONFIGTESTS_INPUT_DATASETS_DIR + "/landr", "RS.shp");

  std::vector<geos::geom::Geometry*> Geoms =
      openfluid::landr::LandRTools::getVectorOfLines(*Val);

  BOOST_CHECK_EQUAL(Geoms.size(), 8);

  delete Val;
}

// =====================================================================
// =====================================================================

BOOST_AUTO_TEST_CASE(check_getNodedLines_simple)
{
  openfluid::core::GeoVectorValue* ValSU = new openfluid::core::GeoVectorValue(
      CONFIGTESTS_INPUT_DATASETS_DIR + "/landr", "POLY_TEST.shp");

  openfluid::core::GeoVectorValue* ValRS = new openfluid::core::GeoVectorValue(
      CONFIGTESTS_INPUT_DATASETS_DIR + "/landr", "LINE_TEST.shp");

  std::vector<geos::geom::Geometry*> GeomsSU =
      openfluid::landr::LandRTools::getVectorOfExteriorRings(*ValSU);

  geos::geom::Geometry* Noded = openfluid::landr::LandRTools::getNodedLines(
      GeomsSU);

  BOOST_CHECK_EQUAL(Noded ->getNumGeometries(), 8);

  std::vector<geos::geom::Geometry*> GeomsRS =
      openfluid::landr::LandRTools::getVectorOfLines(*ValRS);

  delete Noded;
  Noded = openfluid::landr::LandRTools::getNodedLines(GeomsRS);

  BOOST_CHECK_EQUAL(Noded ->getNumGeometries(), 1);

  std::vector<geos::geom::Geometry*> Geoms = GeomsSU;
  Geoms.insert(Geoms.end(), GeomsRS.begin(), GeomsRS.end());

  delete Noded;
  Noded = openfluid::landr::LandRTools::getNodedLines(Geoms);

  BOOST_CHECK_EQUAL(Noded ->getNumGeometries(), 17);

  delete Noded;
  unsigned int i;
  for (i = 0; i < Geoms.size(); i++)
    delete Geoms[i];
  delete ValSU;
  delete ValRS;
}

// =====================================================================
// =====================================================================

BOOST_AUTO_TEST_CASE(check_getNodedLines_simple_overlaps_swapOrder)
{
  openfluid::core::GeoVectorValue* ValSU = new openfluid::core::GeoVectorValue(
      CONFIGTESTS_INPUT_DATASETS_DIR + "/landr", "POLY_TEST.shp");
  openfluid::core::GeoVectorValue* ValRS = new openfluid::core::GeoVectorValue(
      CONFIGTESTS_INPUT_DATASETS_DIR + "/landr", "LINE_TEST2.shp");

  std::vector<geos::geom::Geometry*> GeomsSU =
      openfluid::landr::LandRTools::getVectorOfExteriorRings(*ValSU);
  std::vector<geos::geom::Geometry*> GeomsRS =
      openfluid::landr::LandRTools::getVectorOfLines(*ValRS);

  // Add RS then SU : pass
  std::vector<geos::geom::Geometry*> Geoms = GeomsRS;
  Geoms.insert(Geoms.end(), GeomsSU.begin(), GeomsSU.end());

  geos::geom::Geometry* Noded = openfluid::landr::LandRTools::getNodedLines(
      Geoms);

  BOOST_CHECK_EQUAL(Noded ->getNumGeometries(), 12);

  Geoms.clear();
  delete Noded;

  // Add SU then RS : fails with classic Union, pass with CascadedUnion
  Geoms = GeomsSU;
  Geoms.insert(Geoms.end(), GeomsRS.begin(), GeomsRS.end());

  Noded = openfluid::landr::LandRTools::getNodedLines(Geoms);

  BOOST_CHECK_EQUAL(Noded ->getNumGeometries(), 12);

  delete Noded;
  unsigned int i;
  for (i = 0; i < Geoms.size(); i++)
    delete Geoms[i];
  delete ValSU;
  delete ValRS;
}

// =====================================================================
// =====================================================================

BOOST_AUTO_TEST_CASE(check_getNodedLines_simple_2SU2RS)
{
  openfluid::core::GeoVectorValue* ValSU = new openfluid::core::GeoVectorValue(
      CONFIGTESTS_INPUT_DATASETS_DIR + "/landr", "POLY_TEST.shp");
  openfluid::core::GeoVectorValue* ValSU2 = new openfluid::core::GeoVectorValue(
      CONFIGTESTS_INPUT_DATASETS_DIR + "/landr", "POLY_TEST2.shp");
  openfluid::core::GeoVectorValue* ValRS = new openfluid::core::GeoVectorValue(
      CONFIGTESTS_INPUT_DATASETS_DIR + "/landr", "LINE_TEST.shp");
  openfluid::core::GeoVectorValue* ValRS2 = new openfluid::core::GeoVectorValue(
      CONFIGTESTS_INPUT_DATASETS_DIR + "/landr", "LINE_TEST2.shp");

  std::vector<geos::geom::Geometry*> GeomsSU =
      openfluid::landr::LandRTools::getVectorOfExteriorRings(*ValSU);
  std::vector<geos::geom::Geometry*> GeomsSU2 =
      openfluid::landr::LandRTools::getVectorOfExteriorRings(*ValSU2);
  std::vector<geos::geom::Geometry*> GeomsRS =
      openfluid::landr::LandRTools::getVectorOfLines(*ValRS);
  std::vector<geos::geom::Geometry*> GeomsRS2 =
      openfluid::landr::LandRTools::getVectorOfLines(*ValRS2);

  std::vector<geos::geom::Geometry*> Geoms = GeomsRS2;
  Geoms.insert(Geoms.end(), GeomsRS.begin(), GeomsRS.end());
  Geoms.insert(Geoms.end(), GeomsSU2.begin(), GeomsSU2.end());
  Geoms.insert(Geoms.end(), GeomsSU.begin(), GeomsSU.end());

  geos::geom::Geometry* Noded = openfluid::landr::LandRTools::getNodedLines(
      Geoms);

  BOOST_CHECK_EQUAL(Noded ->getNumGeometries(), 42);

  delete Noded;
  unsigned int i;
  for (i = 0; i < Geoms.size(); i++)
    delete Geoms[i];
  delete ValSU;
  delete ValSU2;
  delete ValRS;
  delete ValRS2;
}

// =====================================================================
// =====================================================================

BOOST_AUTO_TEST_CASE(check_polygonize_simple_2SU2RS)
{
  openfluid::core::GeoVectorValue* ValSU = new openfluid::core::GeoVectorValue(
      CONFIGTESTS_INPUT_DATASETS_DIR + "/landr", "POLY_TEST.shp");
  openfluid::core::GeoVectorValue* ValSU2 = new openfluid::core::GeoVectorValue(
      CONFIGTESTS_INPUT_DATASETS_DIR + "/landr", "POLY_TEST2.shp");
  openfluid::core::GeoVectorValue* ValRS = new openfluid::core::GeoVectorValue(
      CONFIGTESTS_INPUT_DATASETS_DIR + "/landr", "LINE_TEST.shp");
  openfluid::core::GeoVectorValue* ValRS2 = new openfluid::core::GeoVectorValue(
      CONFIGTESTS_INPUT_DATASETS_DIR + "/landr", "LINE_TEST2.shp");

  std::vector<geos::geom::Geometry*> GeomsSU =
      openfluid::landr::LandRTools::getVectorOfExteriorRings(*ValSU);
  std::vector<geos::geom::Geometry*> GeomsSU2 =
      openfluid::landr::LandRTools::getVectorOfExteriorRings(*ValSU2);
  std::vector<geos::geom::Geometry*> GeomsRS =
      openfluid::landr::LandRTools::getVectorOfLines(*ValRS);
  std::vector<geos::geom::Geometry*> GeomsRS2 =
      openfluid::landr::LandRTools::getVectorOfLines(*ValRS2);

  std::vector<geos::geom::Geometry*> Geoms = GeomsRS2;
  Geoms.insert(Geoms.end(), GeomsRS.begin(), GeomsRS.end());
  Geoms.insert(Geoms.end(), GeomsSU2.begin(), GeomsSU2.end());
  Geoms.insert(Geoms.end(), GeomsSU.begin(), GeomsSU.end());

  geos::geom::Geometry* Noded = openfluid::landr::LandRTools::getNodedLines(
      Geoms);

  std::vector<geos::geom::Polygon*> NewPolys;
  std::vector<const geos::geom::LineString*> Dangles;
  openfluid::landr::LandRTools::polygonizeGeometry(Noded, NewPolys, Dangles);

  BOOST_CHECK_EQUAL(NewPolys.size(), 18);
  BOOST_CHECK_EQUAL(Dangles.size(), 4);

  delete Noded;
  unsigned int i;
  for (i = 0; i < Geoms.size(); i++)
    delete Geoms[i];
  for (i = 0; i < NewPolys.size(); i++)
    delete NewPolys.at(i);
  // do not delete Dangles
  delete ValSU;
  delete ValSU2;
  delete ValRS;
  delete ValRS2;
}

// =====================================================================
// =====================================================================

BOOST_AUTO_TEST_CASE(check_getPolygonizedGeometry_simple)
{
  openfluid::core::GeoVectorValue* ValSU = new openfluid::core::GeoVectorValue(
      CONFIGTESTS_INPUT_DATASETS_DIR + "/landr", "POLY_TEST.shp");

  openfluid::core::GeoVectorValue* ValRS = new openfluid::core::GeoVectorValue(
      CONFIGTESTS_INPUT_DATASETS_DIR + "/landr", "LINE_TEST.shp");

  std::vector<geos::geom::Geometry*> Geoms =
      openfluid::landr::LandRTools::getVectorOfExteriorRings(*ValSU);

  std::vector<geos::geom::Geometry*> GeomsRS =
      openfluid::landr::LandRTools::getVectorOfLines(*ValRS);

  Geoms.insert(Geoms.end(), GeomsRS.begin(), GeomsRS.end());

  geos::geom::Geometry* Noded = openfluid::landr::LandRTools::getNodedLines(
      Geoms);

  std::vector<geos::geom::Polygon*> NewPolys;
  std::vector<const geos::geom::LineString*> Dangles;
  openfluid::landr::LandRTools::polygonizeGeometry(Noded, NewPolys, Dangles);

  BOOST_CHECK_EQUAL(NewPolys.size(), 7);
  BOOST_CHECK_EQUAL(Dangles.size(), 2);

  delete Noded;
  unsigned int i;
  for (i = 0; i < Geoms.size(); i++)
    delete Geoms[i];
  for (i = 0; i < NewPolys.size(); i++)
    delete NewPolys.at(i);
  // do not delete Dangles
  delete ValSU;
  delete ValRS;
}

// =====================================================================
// =====================================================================

BOOST_AUTO_TEST_CASE(check_getPolygonizedGeometry_simple1)
{
  openfluid::core::GeoVectorValue* ValSU = new openfluid::core::GeoVectorValue(
      "/home/libres", "simplePoly1.shp");

  openfluid::core::GeoVectorValue* ValRS = new openfluid::core::GeoVectorValue(
      "/home/libres", "simpleLine1.shp");

  std::vector<geos::geom::Geometry*> Geoms =
      openfluid::landr::LandRTools::getVectorOfExteriorRings(*ValSU);

  std::vector<geos::geom::Geometry*> GeomsRS =
      openfluid::landr::LandRTools::getVectorOfLines(*ValRS);

  Geoms.insert(Geoms.end(), GeomsRS.begin(), GeomsRS.end());

  geos::geom::Geometry* Noded = openfluid::landr::LandRTools::getNodedLines(
      Geoms);

  std::vector<geos::geom::Polygon*> NewPolys;
  std::vector<const geos::geom::LineString*> Dangles;
  openfluid::landr::LandRTools::polygonizeGeometry(Noded, NewPolys, Dangles);

  BOOST_CHECK_EQUAL(NewPolys.size(), 2);
  BOOST_CHECK_EQUAL(Dangles.size(), 2);

  delete Noded;
  unsigned int i;
  for (i = 0; i < Geoms.size(); i++)
    delete Geoms[i];
  for (i = 0; i < NewPolys.size(); i++)
    delete NewPolys.at(i);
  // do not delete Dangles
  delete ValSU;
  delete ValRS;
}

// =====================================================================
// =====================================================================

BOOST_AUTO_TEST_CASE(check_getPolygonizedGeometry_simple2)
{
  openfluid::core::GeoVectorValue* ValSU = new openfluid::core::GeoVectorValue(
      "/home/libres", "simplePoly1.shp");

  openfluid::core::GeoVectorValue* ValRS = new openfluid::core::GeoVectorValue(
      "/home/libres", "simpleLine2.shp");

  std::vector<geos::geom::Geometry*> Geoms =
      openfluid::landr::LandRTools::getVectorOfExteriorRings(*ValSU);

  std::vector<geos::geom::Geometry*> GeomsRS =
      openfluid::landr::LandRTools::getVectorOfLines(*ValRS);

  Geoms.insert(Geoms.end(), GeomsRS.begin(), GeomsRS.end());

  geos::geom::Geometry* Noded = openfluid::landr::LandRTools::getNodedLines(
      Geoms);

  std::vector<geos::geom::Polygon*> NewPolys;
  std::vector<const geos::geom::LineString*> Dangles;
  openfluid::landr::LandRTools::polygonizeGeometry(Noded, NewPolys, Dangles);

  BOOST_CHECK_EQUAL(NewPolys.size(), 1);
  BOOST_CHECK_EQUAL(Dangles.size(), 0);

  delete Noded;
  unsigned int i;
  for (i = 0; i < Geoms.size(); i++)
    delete Geoms[i];
  for (i = 0; i < NewPolys.size(); i++)
    delete NewPolys.at(i);
  // do not delete Dangles
  delete ValSU;
  delete ValRS;
}

// =====================================================================
// =====================================================================

BOOST_AUTO_TEST_CASE(check_getPolygonizedGeometry_simple3)
{
  openfluid::core::GeoVectorValue* ValSU = new openfluid::core::GeoVectorValue(
      "/home/libres", "simplePoly1.shp");

  openfluid::core::GeoVectorValue* ValRS = new openfluid::core::GeoVectorValue(
      "/home/libres", "simpleLine3.shp");

  std::vector<geos::geom::Geometry*> Geoms =
      openfluid::landr::LandRTools::getVectorOfExteriorRings(*ValSU);

  std::vector<geos::geom::Geometry*> GeomsRS =
      openfluid::landr::LandRTools::getVectorOfLines(*ValRS);

  Geoms.insert(Geoms.end(), GeomsRS.begin(), GeomsRS.end());

  geos::geom::Geometry* Noded = openfluid::landr::LandRTools::getNodedLines(
      Geoms);

  std::vector<geos::geom::Polygon*> NewPolys;
  std::vector<const geos::geom::LineString*> Dangles;
  openfluid::landr::LandRTools::polygonizeGeometry(Noded, NewPolys, Dangles);

  BOOST_CHECK_EQUAL(NewPolys.size(), 1);
  BOOST_CHECK_EQUAL(Dangles.size(), 1);

  delete Noded;
  unsigned int i;
  for (i = 0; i < Geoms.size(); i++)
    delete Geoms[i];
  for (i = 0; i < NewPolys.size(); i++)
    delete NewPolys.at(i);
  // do not delete Dangles
  delete ValSU;
  delete ValRS;
}

// =====================================================================
// =====================================================================

BOOST_AUTO_TEST_CASE(check_getPolygonizedGeometry_simple4)
{
  openfluid::core::GeoVectorValue* ValSU = new openfluid::core::GeoVectorValue(
      "/home/libres", "simplePoly1.shp");

  openfluid::core::GeoVectorValue* ValRS = new openfluid::core::GeoVectorValue(
      "/home/libres", "simpleLine4.shp");

  std::vector<geos::geom::Geometry*> Geoms =
      openfluid::landr::LandRTools::getVectorOfExteriorRings(*ValSU);

  std::vector<geos::geom::Geometry*> GeomsRS =
      openfluid::landr::LandRTools::getVectorOfLines(*ValRS);

  Geoms.insert(Geoms.end(), GeomsRS.begin(), GeomsRS.end());

  geos::geom::Geometry* Noded = openfluid::landr::LandRTools::getNodedLines(
      Geoms);

  std::vector<geos::geom::Polygon*> NewPolys;
  std::vector<const geos::geom::LineString*> Dangles;
  openfluid::landr::LandRTools::polygonizeGeometry(Noded, NewPolys, Dangles);

  BOOST_CHECK_EQUAL(NewPolys.size(), 1);
  BOOST_CHECK_EQUAL(Dangles.size(), 0);

  delete Noded;
  unsigned int i;
  for (i = 0; i < Geoms.size(); i++)
    delete Geoms[i];
  for (i = 0; i < NewPolys.size(); i++)
    delete NewPolys.at(i);
  // do not delete Dangles
  delete ValSU;
  delete ValRS;
}

// =====================================================================
// =====================================================================
