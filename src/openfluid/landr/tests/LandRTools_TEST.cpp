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
#include <geos/util/TopologyException.h>
#include <geos/util/IllegalArgumentException.h>

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

  std::vector<geos::geom::LineString*> Geoms =
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

  std::vector<geos::geom::LineString*> Geoms =
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

  std::vector<geos::geom::LineString*> SU_lines =
      openfluid::landr::LandRTools::getVectorOfExteriorRings(*ValSU);

  std::vector<geos::geom::LineString*> RS_lines =
      openfluid::landr::LandRTools::getVectorOfLines(*ValRS);

  std::vector<geos::geom::Geometry*> SU_vect;
  std::vector<geos::geom::Geometry*> RS_vect;
  SU_vect.assign(SU_lines.begin(), SU_lines.end());
  RS_vect.assign(RS_lines.begin(), RS_lines.end());

  const geos::geom::GeometryFactory* Factory =
      geos::geom::GeometryFactory::getDefaultInstance();

  geos::geom::Geometry* SU_coll = Factory->buildGeometry(SU_vect);
  geos::geom::Geometry* RS_coll = Factory->buildGeometry(RS_vect);

  std::vector<geos::geom::LineString*>* Noded =
      openfluid::landr::LandRTools::getNodedLines(SU_coll, RS_coll);

  BOOST_CHECK_EQUAL(Noded->size(), 17);
//  for (unsigned int i = 0; i < Noded->size(); i++)
//    std::cout << Noded->at(i)->toString() << std::endl;

  delete ValSU;
  delete ValRS;
}

// =====================================================================
// =====================================================================

BOOST_AUTO_TEST_CASE(check_getPolygonizedGeometry_simple)
{
  openfluid::core::GeoVectorValue* ValSU = new openfluid::core::GeoVectorValue(
      CONFIGTESTS_INPUT_DATASETS_DIR + "/landr", "POLY_TEST.shp");

  openfluid::core::GeoVectorValue* ValRS = new openfluid::core::GeoVectorValue(
      CONFIGTESTS_INPUT_DATASETS_DIR + "/landr", "LINE_TEST.shp");

  std::vector<geos::geom::LineString*> SU_lines =
      openfluid::landr::LandRTools::getVectorOfExteriorRings(*ValSU);

  std::vector<geos::geom::LineString*> RS_lines =
      openfluid::landr::LandRTools::getVectorOfLines(*ValRS);

  std::vector<geos::geom::Geometry*> SU_vect;
  std::vector<geos::geom::Geometry*> RS_vect;
  SU_vect.assign(SU_lines.begin(), SU_lines.end());
  RS_vect.assign(RS_lines.begin(), RS_lines.end());

  const geos::geom::GeometryFactory* Factory =
      geos::geom::GeometryFactory::getDefaultInstance();

  geos::geom::Geometry* SU_coll = Factory->buildGeometry(SU_vect);
  geos::geom::Geometry* RS_coll = Factory->buildGeometry(RS_vect);

  std::vector<geos::geom::LineString*>* Noded_lines =
      openfluid::landr::LandRTools::getNodedLines(SU_coll, RS_coll);

  std::vector<geos::geom::Geometry*> Noded;
  Noded.assign(Noded_lines->begin(), Noded_lines->end());
  std::vector<geos::geom::Polygon*> NewPolys;
  std::vector<const geos::geom::LineString*> Dangles;
  openfluid::landr::LandRTools::polygonizeGeometry(Noded, NewPolys, Dangles);

//  for (unsigned int i = 0; i < NewPolys.size(); i++)
//    std::cout << NewPolys.at(i)->toString() << std::endl;
//  for (unsigned int i = 0; i < Dangles.size(); i++)
//    std::cout << Dangles.at(i)->toString() << std::endl;

  BOOST_CHECK_EQUAL(NewPolys.size(), 7);
  BOOST_CHECK_EQUAL(Dangles.size(), 2);

  for (unsigned int i = 0; i < NewPolys.size(); i++)
    delete NewPolys.at(i);
  // do not delete Dangles
  delete ValSU;
  delete ValRS;
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

  std::vector<geos::geom::LineString*> SU_lines =
      openfluid::landr::LandRTools::getVectorOfExteriorRings(*ValSU);
  std::vector<geos::geom::LineString*> SU2_lines =
      openfluid::landr::LandRTools::getVectorOfExteriorRings(*ValSU2);
  std::vector<geos::geom::LineString*> RS_lines =
      openfluid::landr::LandRTools::getVectorOfLines(*ValRS);
  std::vector<geos::geom::LineString*> RS2_lines =
      openfluid::landr::LandRTools::getVectorOfLines(*ValRS2);

  const geos::geom::GeometryFactory* Factory =
      geos::geom::GeometryFactory::getDefaultInstance();

  std::vector<geos::geom::Geometry*> SU_vect;
  SU_vect.assign(SU_lines.begin(), SU_lines.end());
  SU_vect.insert(SU_vect.end(), SU2_lines.begin(), SU2_lines.end());
  geos::geom::Geometry* SU_coll = Factory->buildGeometry(SU_vect);

  std::vector<geos::geom::Geometry*> RS_vect;
  RS_vect.assign(RS_lines.begin(), RS_lines.end());
  RS_vect.insert(RS_vect.end(), RS2_lines.begin(), RS2_lines.end());
  geos::geom::Geometry* RS_coll = Factory->buildGeometry(RS_vect);

  BOOST_CHECK_THROW(
      openfluid::landr::LandRTools::getNodedLines(SU_coll, RS_coll),
      geos::util::TopologyException);

  std::vector<geos::geom::LineString*>* Noded_lines =
      openfluid::landr::LandRTools::getNodedLines(SU_coll, RS_coll, 0.000001);

  std::vector<geos::geom::Geometry*> Noded;
  Noded.assign(Noded_lines->begin(), Noded_lines->end());
  std::vector<geos::geom::Polygon*> NewPolys;
  std::vector<const geos::geom::LineString*> Dangles;
  openfluid::landr::LandRTools::polygonizeGeometry(Noded, NewPolys, Dangles);

//  for (unsigned int i = 0; i < NewPolys.size(); i++)
//    std::cout << NewPolys.at(i)->toString() << std::endl;
//  for (unsigned int i = 0; i < Dangles.size(); i++)
//    std::cout << Dangles.at(i)->toString() << std::endl;

  BOOST_CHECK_EQUAL(NewPolys.size(), 18);
  BOOST_CHECK_EQUAL(Dangles.size(), 3);

  for (unsigned int i = 0; i < NewPolys.size(); i++)
    delete NewPolys.at(i);
  // do not delete Dangles
  delete ValSU;
  delete ValSU2;
  delete ValRS;
  delete ValRS2;
}

// =====================================================================
// =====================================================================

BOOST_AUTO_TEST_CASE(check_polygonize_medium2Polys)
{
  openfluid::core::GeoVectorValue* ValSU = new openfluid::core::GeoVectorValue(
      CONFIGTESTS_INPUT_DATASETS_DIR + "/landr", "field.shp");
  openfluid::core::GeoVectorValue* ValSU2 = new openfluid::core::GeoVectorValue(
      CONFIGTESTS_INPUT_DATASETS_DIR + "/landr", "soil.shp");

  std::vector<geos::geom::LineString*> SU_lines =
      openfluid::landr::LandRTools::getVectorOfExteriorRings(*ValSU);
  std::vector<geos::geom::LineString*> SU2_lines =
      openfluid::landr::LandRTools::getVectorOfExteriorRings(*ValSU2);

  const geos::geom::GeometryFactory* Factory =
      geos::geom::GeometryFactory::getDefaultInstance();

  std::vector<geos::geom::Geometry*> SU_vect;
  SU_vect.assign(SU_lines.begin(), SU_lines.end());
  geos::geom::Geometry* SU_coll = Factory->buildGeometry(SU_vect);

  std::vector<geos::geom::Geometry*> SU2_vect;
  SU2_vect.assign(SU2_lines.begin(), SU2_lines.end());
  geos::geom::Geometry* SU2_coll = Factory->buildGeometry(SU2_vect);

  std::vector<geos::geom::LineString*>* Noded_lines =
      openfluid::landr::LandRTools::getNodedLines(SU_coll, SU2_coll);

  BOOST_CHECK_EQUAL(Noded_lines->size(), 51);
//  for (unsigned int i = 0; i < Noded_lines->size(); i++)
//    std::cout << Noded_lines->at(i)->toString() << std::endl;

  std::vector<geos::geom::Geometry*> Noded;
  Noded.assign(Noded_lines->begin(), Noded_lines->end());

  std::vector<const geos::geom::LineString*> TheDangles;
  std::vector<geos::geom::Polygon*> ThePolygons;

  openfluid::landr::LandRTools::polygonizeGeometry(Noded, ThePolygons,
                                                   TheDangles);

  BOOST_CHECK_EQUAL(TheDangles.size(), 0);

  BOOST_CHECK_EQUAL(ThePolygons.size(), 24);

//  for (unsigned int i = 0; i < ThePolygons.size(); i++)
//    std::cout << ThePolygons.at(i)->toString() << std::endl;
//  for (unsigned int i = 0; i < TheDangles.size(); i++)
//    std::cout << TheDangles.at(i)->toString() << std::endl;

  for (unsigned int i = 0; i < ThePolygons.size(); i++)
    delete ThePolygons.at(i);
  delete ValSU;
  delete ValSU2;
}

// =====================================================================
// =====================================================================

BOOST_AUTO_TEST_CASE(check_polygonize_medium2Polys1Line)
{
  openfluid::core::GeoVectorValue* Val = new openfluid::core::GeoVectorValue(
      CONFIGTESTS_INPUT_DATASETS_DIR + "/landr", "field.shp");
  openfluid::core::GeoVectorValue* Val2 = new openfluid::core::GeoVectorValue(
      CONFIGTESTS_INPUT_DATASETS_DIR + "/landr", "soil.shp");
  openfluid::core::GeoVectorValue* Val3 = new openfluid::core::GeoVectorValue(
      CONFIGTESTS_INPUT_DATASETS_DIR + "/landr", "reach.shp");

  std::vector<geos::geom::LineString*> Field_lines =
      openfluid::landr::LandRTools::getVectorOfExteriorRings(*Val);
  std::vector<geos::geom::LineString*> Soil_lines =
      openfluid::landr::LandRTools::getVectorOfExteriorRings(*Val2);

  std::vector<geos::geom::Geometry*> Fields;
  std::vector<geos::geom::Geometry*> Soils;
  Fields.assign(Field_lines.begin(), Field_lines.end());
  Soils.assign(Soil_lines.begin(), Soil_lines.end());

  const geos::geom::GeometryFactory* Factory =
      geos::geom::GeometryFactory::getDefaultInstance();

  geos::geom::Geometry* Field_coll = Factory->buildGeometry(Fields);
  geos::geom::Geometry* Soil_coll = Factory->buildGeometry(Soils);

  std::vector<geos::geom::LineString*>* Poly_lines =
      openfluid::landr::LandRTools::getNodedLines(Field_coll, Soil_coll);

  BOOST_CHECK_EQUAL(Poly_lines->size(), 51);

  std::vector<geos::geom::Geometry*> Poly_vect;
  Poly_vect.assign(Poly_lines->begin(), Poly_lines->end());
  geos::geom::Geometry* Poly_coll = Factory->buildGeometry(Poly_vect);

  BOOST_CHECK_THROW(
      openfluid::landr::LandRTools::getNodedLines(Poly_coll, Val3->getGeometries()),
      geos::util::IllegalArgumentException);

  std::vector<geos::geom::LineString*>* All_lines =
      openfluid::landr::LandRTools::getNodedLines(Poly_coll,
                                                  Val3->getGeometries(),
                                                  0.000001);

  BOOST_CHECK_EQUAL(All_lines->size(), 65);

  All_lines->clear();
  All_lines = openfluid::landr::LandRTools::getNodedLines(Poly_coll,
                                                          Val3->getGeometries(),
                                                          0.00001);

  BOOST_CHECK_EQUAL(All_lines->size(), 63);

  std::vector<geos::geom::Geometry*> Noded;
  Noded.assign(All_lines->begin(), All_lines->end());

  std::vector<const geos::geom::LineString*> TheDangles;
  std::vector<geos::geom::Polygon*> ThePolygons;

  openfluid::landr::LandRTools::polygonizeGeometry(Noded, ThePolygons,
                                                   TheDangles);

  BOOST_CHECK_EQUAL(TheDangles.size(), 2);

  BOOST_CHECK_EQUAL(ThePolygons.size(), 29);

//  for (unsigned int i = 0; i < ThePolygons.size(); i++)
//    std::cout << ThePolygons.at(i)->toString() << std::endl;
//  for (unsigned int i = 0; i < TheDangles.size(); i++)
//    std::cout << TheDangles.at(i)->toString() << std::endl;

  for (unsigned int i = 0; i < ThePolygons.size(); i++)
    delete ThePolygons.at(i);

  delete Val;
  delete Val2;
  delete Val3;
}

// =====================================================================
// =====================================================================

BOOST_AUTO_TEST_CASE(check_getNodedLines_virtual)
{
  openfluid::core::GeoVectorValue* Val = new openfluid::core::GeoVectorValue(
      CONFIGTESTS_INPUT_DATASETS_DIR + "/landr/virtual", "field2.shp");
  openfluid::core::GeoVectorValue* Val2 = new openfluid::core::GeoVectorValue(
      CONFIGTESTS_INPUT_DATASETS_DIR + "/landr/virtual", "soil.shp");
  openfluid::core::GeoVectorValue* Val3 = new openfluid::core::GeoVectorValue(
      CONFIGTESTS_INPUT_DATASETS_DIR + "/landr/virtual", "reach5.shp");

  std::vector<geos::geom::LineString*> Field_lines =
      openfluid::landr::LandRTools::getVectorOfExteriorRings(*Val);

  std::vector<geos::geom::LineString*> Soil_lines =
      openfluid::landr::LandRTools::getVectorOfExteriorRings(*Val2);

  geos::geom::Geometry* Reach_coll = Val3->getGeometries();

  std::vector<geos::geom::Geometry*> Fields;
  std::vector<geos::geom::Geometry*> Soils;
  Fields.assign(Field_lines.begin(), Field_lines.end());
  Soils.assign(Soil_lines.begin(), Soil_lines.end());

  const geos::geom::GeometryFactory* Factory =
      geos::geom::GeometryFactory::getDefaultInstance();

  geos::geom::Geometry* Field_coll = Factory->buildGeometry(Fields);
  geos::geom::Geometry* Soil_coll = Factory->buildGeometry(Soils);

  double snapTolerance = 0.2;

  std::vector<geos::geom::LineString*>* NodedPolys =
      openfluid::landr::LandRTools::getNodedLines(Field_coll, Soil_coll,
                                                  snapTolerance);

  BOOST_CHECK_EQUAL(NodedPolys->size(), 58);
  //  for (unsigned int i = 0; i < NodedPolys->size(); i++)
  //    std::cout << NodedPolys->at(i)->toString() << std::endl;

  std::vector<geos::geom::Geometry*> Polys;
  Polys.assign(NodedPolys->begin(), NodedPolys->end());
  geos::geom::Geometry* Polys_coll = Factory->buildGeometry(Polys);

  std::vector<geos::geom::LineString*>* NodedAll =
      openfluid::landr::LandRTools::getNodedLines(Polys_coll, Reach_coll,
                                                  snapTolerance);

  BOOST_CHECK_EQUAL(NodedAll->size(), 59);
  //  for (unsigned int i = 0; i < NodedAll->size(); i++)
  //    std::cout << NodedAll->at(i)->toString() << std::endl;

  std::vector<geos::geom::Geometry*> All;
  All.assign(NodedAll->begin(), NodedAll->end());

  std::vector<const geos::geom::LineString*> TheDangles;
  std::vector<geos::geom::Polygon*> ThePolygons;

  openfluid::landr::LandRTools::polygonizeGeometry(All, ThePolygons,
                                                   TheDangles);

  BOOST_CHECK_EQUAL(TheDangles.size(), 0);
  //  for (unsigned int i = 0; i < TheDangles->size(); i++)
  //    std::cout << TheDangles->at(i)->toString() << std::endl;

  BOOST_CHECK_EQUAL(ThePolygons.size(), 25);
  //  for (unsigned int i = 0; i < ThePolygons->size(); i++)
  //    std::cout << ThePolygons->at(i)->toString() << std::endl;

  for (unsigned int i = 0; i < ThePolygons.size(); i++)
    delete ThePolygons.at(i);
  delete Val;
  delete Val2;
  delete Val3;
}

// =====================================================================
// =====================================================================

BOOST_AUTO_TEST_CASE(check_getNodedLines_virtual_snap)
{
  openfluid::core::GeoVectorValue* Val = new openfluid::core::GeoVectorValue(
      CONFIGTESTS_INPUT_DATASETS_DIR + "/landr/virtual", "field2.shp");
  openfluid::core::GeoVectorValue* Val2 = new openfluid::core::GeoVectorValue(
      CONFIGTESTS_INPUT_DATASETS_DIR + "/landr/virtual", "soil.shp");

  std::vector<geos::geom::LineString*> Field_lines =
      openfluid::landr::LandRTools::getVectorOfExteriorRings(*Val);

  std::vector<geos::geom::LineString*> Soil_lines =
      openfluid::landr::LandRTools::getVectorOfExteriorRings(*Val2);

  std::vector<geos::geom::Geometry*> Fields;
  std::vector<geos::geom::Geometry*> Soils;
  Fields.assign(Field_lines.begin(), Field_lines.end());
  Soils.assign(Soil_lines.begin(), Soil_lines.end());

  const geos::geom::GeometryFactory* Factory =
      geos::geom::GeometryFactory::getDefaultInstance();

  geos::geom::Geometry* Field_coll = Factory->buildGeometry(Fields);
  geos::geom::Geometry* Soil_coll = Factory->buildGeometry(Soils);

  double snapTolerance = 0.2;

  std::vector<geos::geom::LineString*>* NodedPolys =
      openfluid::landr::LandRTools::getNodedLines(Field_coll, Soil_coll,
                                                  snapTolerance);

  BOOST_CHECK_EQUAL(NodedPolys->size(), 58);

  std::vector<geos::geom::Geometry*> All;
  All.assign(NodedPolys->begin(), NodedPolys->end());

  std::vector<const geos::geom::LineString*> TheDangles;
  std::vector<geos::geom::Polygon*> ThePolygons;
  openfluid::landr::LandRTools::polygonizeGeometry(All, ThePolygons,
                                                   TheDangles);

  BOOST_CHECK_EQUAL(TheDangles.size(), 0);
  BOOST_CHECK_EQUAL(ThePolygons.size(), 24);

  snapTolerance = 10;

  NodedPolys->clear();
  NodedPolys = openfluid::landr::LandRTools::getNodedLines(Field_coll,
                                                           Soil_coll,
                                                           snapTolerance);

  BOOST_CHECK_EQUAL(NodedPolys->size(), 54);

  All.clear();
  All.assign(NodedPolys->begin(), NodedPolys->end());

  TheDangles.clear();
  ThePolygons.clear();
  openfluid::landr::LandRTools::polygonizeGeometry(All, ThePolygons,
                                                   TheDangles);

  BOOST_CHECK_EQUAL(TheDangles.size(), 0);
  BOOST_CHECK_EQUAL(ThePolygons.size(), 22);

  for (unsigned int i = 0; i < ThePolygons.size(); i++)
    delete ThePolygons.at(i);
  delete Val;
  delete Val2;
}

// =====================================================================
// =====================================================================

