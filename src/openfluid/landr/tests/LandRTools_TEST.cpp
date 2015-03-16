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
  @file LandRTools_TEST.cpp

  @author Aline LIBRES <aline.libres@gmail.com>
  @author Michael RABOTIN <michael.rabotin@supagro.inra.fr>
 */

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
  @file LandRTools_TEST.cpp

  @author Aline LIBRES <aline.libres@gmail.com>
 */

#define BOOST_TEST_MAIN
#define BOOST_AUTO_TEST_MAIN
#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MODULE unittest_landrtools
#include <boost/test/unit_test.hpp>
#include <boost/test/auto_unit_test.hpp>
#include <tests-config.hpp>
#include <openfluid/landr/GeosCompat.hpp>
#include <openfluid/core/GeoVectorValue.hpp>
#include <openfluid/core/GeoRasterValue.hpp>
#include <openfluid/base/FrameworkException.hpp>
#include <openfluid/landr/LandRTools.hpp>
#include <openfluid/landr/VectorDataset.hpp>
#include <openfluid/landr/RasterDataset.hpp>
#include <geos/geom/Geometry.h>
#include <geos/geom/LineString.h>
#include <geos/geom/Polygon.h>
#include <geos/geom/Point.h>
#include <geos/geom/CoordinateArraySequenceFactory.h>
#include <geos/geom/GeometryFactory.h>
#include <geos/geom/MultiLineString.h>
#include <geos/util/TopologyException.h>
#include <geos/util/IllegalArgumentException.h>
#include <geos/planargraph/Node.h>
#include <openfluid/scientific/FloatingPoint.hpp>
#include <geos/operation/overlay/snap/SnapOverlayOp.h>
#include <geos/operation/overlay/snap/GeometrySnapper.h>
#include <geos/operation/distance/DistanceOp.h>


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
      openfluid::landr::LandRTools::computeMergedLineStringsFromGeometry(MLS);

  BOOST_CHECK_EQUAL(MergedLSVect->size(), 1);

  geos::geom::LineString* MergedLS =
      openfluid::landr::LandRTools::computeMergedLineStringFromGeometry(MLS);

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
      openfluid::landr::LandRTools::computeMergedLineStringsFromGeometry(MLS);

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
      openfluid::landr::LandRTools::computeMergedLineStringFromGeometry(MLS);

  BOOST_CHECK(!MergedLS);

  delete MLS;
  delete MergedLSVect;
  delete MergedLS1;
  delete MergedLS2;
}


// =====================================================================
// =====================================================================


BOOST_AUTO_TEST_CASE(check_computeVectorOfExteriorRings)
{
  openfluid::core::GeoVectorValue Val(CONFIGTESTS_INPUT_MISCDATA_DIR + "/landr",
                                      "RS.shp");

  openfluid::landr::VectorDataset* Vect = new openfluid::landr::VectorDataset(
      Val);

  BOOST_CHECK_THROW(
      openfluid::landr::LandRTools::computeVectorOfExteriorRings(*Vect),
      openfluid::base::FrameworkException);

  delete Vect;

  Val = openfluid::core::GeoVectorValue(
      CONFIGTESTS_INPUT_MISCDATA_DIR + "/landr", "SU.shp");

  Vect = new openfluid::landr::VectorDataset(Val);

  std::vector<geos::geom::LineString*> Geoms =
      openfluid::landr::LandRTools::computeVectorOfExteriorRings(*Vect);

  BOOST_CHECK_EQUAL(Geoms.size(), 24);

  delete Vect;
}


// =====================================================================
// =====================================================================


BOOST_AUTO_TEST_CASE(check_computeVectorOfLines)
{
  openfluid::core::GeoVectorValue Val(CONFIGTESTS_INPUT_MISCDATA_DIR + "/landr",
                                      "SU.shp");

  openfluid::landr::VectorDataset* Vect = new openfluid::landr::VectorDataset(
      Val);

  BOOST_CHECK_THROW(openfluid::landr::LandRTools::computeVectorOfLines(*Vect),
                    openfluid::base::FrameworkException);

  delete Vect;

  Val = openfluid::core::GeoVectorValue(
      CONFIGTESTS_INPUT_MISCDATA_DIR + "/landr", "RS.shp");

  Vect = new openfluid::landr::VectorDataset(Val);

  std::vector<geos::geom::LineString*> Geoms =
      openfluid::landr::LandRTools::computeVectorOfLines(*Vect);

  BOOST_CHECK_EQUAL(Geoms.size(), 8);

  delete Vect;
}


// =====================================================================
// =====================================================================


BOOST_AUTO_TEST_CASE(check_computeNodedLines_simple)
{
  openfluid::core::GeoVectorValue ValSU(
      CONFIGTESTS_INPUT_MISCDATA_DIR + "/landr", "POLY_TEST.shp");

  openfluid::core::GeoVectorValue ValRS(
      CONFIGTESTS_INPUT_MISCDATA_DIR + "/landr", "LINE_TEST.shp");

  openfluid::landr::VectorDataset* VectSU = new openfluid::landr::VectorDataset(
      ValSU);

  openfluid::landr::VectorDataset* VectRS = new openfluid::landr::VectorDataset(
      ValRS);

  std::vector<geos::geom::LineString*> SU_lines =
      openfluid::landr::LandRTools::computeVectorOfExteriorRings(*VectSU);

  std::vector<geos::geom::LineString*> RS_lines =
      openfluid::landr::LandRTools::computeVectorOfLines(*VectRS);

  std::vector<geos::geom::Geometry*> SU_vect;
  std::vector<geos::geom::Geometry*> RS_vect;
  SU_vect.assign(SU_lines.begin(), SU_lines.end());
  RS_vect.assign(RS_lines.begin(), RS_lines.end());

  const geos::geom::GeometryFactory* Factory =
      geos::geom::GeometryFactory::getDefaultInstance();

  geos::geom::Geometry* SU_coll = Factory->buildGeometry(SU_vect);
  geos::geom::Geometry* RS_coll = Factory->buildGeometry(RS_vect);

  std::vector<geos::geom::LineString*>* Noded =
      openfluid::landr::LandRTools::computeNodedLines(SU_coll, RS_coll);

  BOOST_CHECK_EQUAL(Noded->size(), 17);
//  for (unsigned int i = 0; i < Noded->size(); i++)
//    std::cout << Noded->at(i)->toString() << std::endl;

  delete VectSU;
  delete VectRS;
}


// =====================================================================
// =====================================================================


BOOST_AUTO_TEST_CASE(check_getPolygonizedGeometry_simple)
{
  openfluid::core::GeoVectorValue ValSU(
      CONFIGTESTS_INPUT_MISCDATA_DIR + "/landr", "POLY_TEST.shp");

  openfluid::core::GeoVectorValue ValRS(
      CONFIGTESTS_INPUT_MISCDATA_DIR + "/landr", "LINE_TEST.shp");

  openfluid::landr::VectorDataset* VectSU = new openfluid::landr::VectorDataset(
      ValSU);

  openfluid::landr::VectorDataset* VectRS = new openfluid::landr::VectorDataset(
      ValRS);

  std::vector<geos::geom::LineString*> SU_lines =
      openfluid::landr::LandRTools::computeVectorOfExteriorRings(*VectSU);

  std::vector<geos::geom::LineString*> RS_lines =
      openfluid::landr::LandRTools::computeVectorOfLines(*VectRS);

  std::vector<geos::geom::Geometry*> SU_vect;
  std::vector<geos::geom::Geometry*> RS_vect;
  SU_vect.assign(SU_lines.begin(), SU_lines.end());
  RS_vect.assign(RS_lines.begin(), RS_lines.end());

  const geos::geom::GeometryFactory* Factory =
      geos::geom::GeometryFactory::getDefaultInstance();

  geos::geom::Geometry* SU_coll = Factory->buildGeometry(SU_vect);
  geos::geom::Geometry* RS_coll = Factory->buildGeometry(RS_vect);

  std::vector<geos::geom::LineString*>* Noded_lines =
      openfluid::landr::LandRTools::computeNodedLines(SU_coll, RS_coll);

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
  delete VectSU;
  delete VectRS;
}


// =====================================================================
// =====================================================================


BOOST_AUTO_TEST_CASE(check_polygonize_simple_2SU2RS)
{
  openfluid::core::GeoVectorValue ValSU(
      CONFIGTESTS_INPUT_MISCDATA_DIR + "/landr", "POLY_TEST.shp");
  openfluid::core::GeoVectorValue ValSU2(
      CONFIGTESTS_INPUT_MISCDATA_DIR + "/landr", "POLY_TEST2.shp");
  openfluid::core::GeoVectorValue ValRS(
      CONFIGTESTS_INPUT_MISCDATA_DIR + "/landr", "LINE_TEST.shp");
  openfluid::core::GeoVectorValue ValRS2(
      CONFIGTESTS_INPUT_MISCDATA_DIR + "/landr", "LINE_TEST2.shp");

  openfluid::landr::VectorDataset* VectSU = new openfluid::landr::VectorDataset(
      ValSU);

  openfluid::landr::VectorDataset* VectSU2 =
      new openfluid::landr::VectorDataset(ValSU2);

  openfluid::landr::VectorDataset* VectRS = new openfluid::landr::VectorDataset(
      ValRS);

  openfluid::landr::VectorDataset* VectRS2 =
      new openfluid::landr::VectorDataset(ValRS2);

  std::vector<geos::geom::LineString*> SU_lines =
      openfluid::landr::LandRTools::computeVectorOfExteriorRings(*VectSU);
  std::vector<geos::geom::LineString*> SU2_lines =
      openfluid::landr::LandRTools::computeVectorOfExteriorRings(*VectSU2);
  std::vector<geos::geom::LineString*> RS_lines =
      openfluid::landr::LandRTools::computeVectorOfLines(*VectRS);
  std::vector<geos::geom::LineString*> RS2_lines =
      openfluid::landr::LandRTools::computeVectorOfLines(*VectRS2);

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

  // TODO Check why and if this is the correct geos version change
#if !GEOS_VERSION_GREATER_OR_EQUAL_3_3_2
  BOOST_CHECK_THROW(
      openfluid::landr::LandRTools::computeNodedLines(SU_coll, RS_coll),
      openfluid::base::FrameworkException);
#endif

  std::vector<geos::geom::LineString*>* Noded_lines =
      openfluid::landr::LandRTools::computeNodedLines(SU_coll, RS_coll, 0.000001);

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
  delete VectSU;
  delete VectSU2;
  delete VectRS;
  delete VectRS2;
}


// =====================================================================
// =====================================================================


BOOST_AUTO_TEST_CASE(check_polygonize_medium2Polys)
{
  openfluid::core::GeoVectorValue ValSU(
      CONFIGTESTS_INPUT_MISCDATA_DIR + "/landr", "field.shp");
  openfluid::core::GeoVectorValue ValSU2(
      CONFIGTESTS_INPUT_MISCDATA_DIR + "/landr", "soil.shp");

  openfluid::landr::VectorDataset* VectSU = new openfluid::landr::VectorDataset(
      ValSU);

  openfluid::landr::VectorDataset* VectSU2 =
      new openfluid::landr::VectorDataset(ValSU2);

  std::vector<geos::geom::LineString*> SU_lines =
      openfluid::landr::LandRTools::computeVectorOfExteriorRings(*VectSU);
  std::vector<geos::geom::LineString*> SU2_lines =
      openfluid::landr::LandRTools::computeVectorOfExteriorRings(*VectSU2);

  const geos::geom::GeometryFactory* Factory =
      geos::geom::GeometryFactory::getDefaultInstance();

  std::vector<geos::geom::Geometry*> SU_vect;
  SU_vect.assign(SU_lines.begin(), SU_lines.end());
  geos::geom::Geometry* SU_coll = Factory->buildGeometry(SU_vect);

  std::vector<geos::geom::Geometry*> SU2_vect;
  SU2_vect.assign(SU2_lines.begin(), SU2_lines.end());
  geos::geom::Geometry* SU2_coll = Factory->buildGeometry(SU2_vect);

  std::vector<geos::geom::LineString*>* Noded_lines =
      openfluid::landr::LandRTools::computeNodedLines(SU_coll, SU2_coll);

  BOOST_CHECK_EQUAL(Noded_lines->size(), 51);
//  for (unsigned int i = 0; i < Noded_lines->size(); i++)
 //   std::cout << Noded_lines->at(i)->toString() << std::endl;

  std::vector<geos::geom::Geometry*> Noded;
  Noded.assign(Noded_lines->begin(), Noded_lines->end());

  std::vector<const geos::geom::LineString*> TheDangles;
  std::vector<geos::geom::Polygon*> ThePolygons;

  openfluid::landr::LandRTools::polygonizeGeometry(Noded, ThePolygons,
                                                   TheDangles);

  BOOST_CHECK_EQUAL(TheDangles.size(), 0);

  BOOST_CHECK_EQUAL(ThePolygons.size(), 24);

//  for (unsigned int i = 0; i < ThePolygons.size(); i++)
 //   std::cout << ThePolygons.at(i)->toString() << std::endl;
//  for (unsigned int i = 0; i < TheDangles.size(); i++)
//    std::cout << TheDangles.at(i)->toString() << std::endl;

  for (unsigned int i = 0; i < ThePolygons.size(); i++)
    delete ThePolygons.at(i);
  delete VectSU;
  delete VectSU2;
}


// =====================================================================
// =====================================================================


BOOST_AUTO_TEST_CASE(check_polygonize_medium2Polys1Line)
{
  openfluid::core::GeoVectorValue Val(CONFIGTESTS_INPUT_MISCDATA_DIR + "/landr",
                                      "field.shp");
  openfluid::core::GeoVectorValue Val2(
      CONFIGTESTS_INPUT_MISCDATA_DIR + "/landr", "soil.shp");
  openfluid::core::GeoVectorValue Val3(
      CONFIGTESTS_INPUT_MISCDATA_DIR + "/landr", "reach.shp");

  openfluid::landr::VectorDataset* Vect = new openfluid::landr::VectorDataset(
      Val);
  openfluid::landr::VectorDataset* Vect2 = new openfluid::landr::VectorDataset(
      Val2);
  openfluid::landr::VectorDataset* Vect3 = new openfluid::landr::VectorDataset(
      Val3);

  std::vector<geos::geom::LineString*> Field_lines =
      openfluid::landr::LandRTools::computeVectorOfExteriorRings(*Vect);
  std::vector<geos::geom::LineString*> Soil_lines =
      openfluid::landr::LandRTools::computeVectorOfExteriorRings(*Vect2);

  std::vector<geos::geom::Geometry*> Fields;
  std::vector<geos::geom::Geometry*> Soils;
  Fields.assign(Field_lines.begin(), Field_lines.end());
  Soils.assign(Soil_lines.begin(), Soil_lines.end());

  const geos::geom::GeometryFactory* Factory =
      geos::geom::GeometryFactory::getDefaultInstance();

  geos::geom::Geometry* Field_coll = Factory->buildGeometry(Fields);
  geos::geom::Geometry* Soil_coll = Factory->buildGeometry(Soils);

  std::vector<geos::geom::LineString*>* Poly_lines =
      openfluid::landr::LandRTools::computeNodedLines(Field_coll, Soil_coll);

  BOOST_CHECK_EQUAL(Poly_lines->size(), 51);

  std::vector<geos::geom::Geometry*> Poly_vect;
  Poly_vect.assign(Poly_lines->begin(), Poly_lines->end());
  geos::geom::Geometry* Poly_coll = Factory->buildGeometry(Poly_vect);

  // TODO Check why and if this is the correct geos version change
#if !GEOS_VERSION_GREATER_OR_EQUAL_3_3_2
  BOOST_CHECK_THROW(
      openfluid::landr::LandRTools::computeNodedLines(Poly_coll, Vect3->geometries()),
      openfluid::base::FrameworkException);
#endif

  std::vector<geos::geom::LineString*>* All_lines =
      openfluid::landr::LandRTools::computeNodedLines(Poly_coll,
                                                  Vect3->geometries(),
                                                  0.000001);

  BOOST_CHECK_EQUAL(All_lines->size(), 65);

  All_lines->clear();
  All_lines = openfluid::landr::LandRTools::computeNodedLines(
      Poly_coll, Vect3->geometries(), 0.00001);

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

  delete Vect;
  delete Vect2;
  delete Vect3;
}


// =====================================================================
// =====================================================================


BOOST_AUTO_TEST_CASE(check_computeNodedLines_virtual)
{
  openfluid::core::GeoVectorValue Val(
      CONFIGTESTS_INPUT_MISCDATA_DIR + "/landr/virtual", "field2.shp");
  openfluid::core::GeoVectorValue Val2(
      CONFIGTESTS_INPUT_MISCDATA_DIR + "/landr/virtual", "soil.shp");
  openfluid::core::GeoVectorValue Val3(
      CONFIGTESTS_INPUT_MISCDATA_DIR + "/landr/virtual", "reach5.shp");

  openfluid::landr::VectorDataset* Vect = new openfluid::landr::VectorDataset(
      Val);
  openfluid::landr::VectorDataset* Vect2 = new openfluid::landr::VectorDataset(
      Val2);
  openfluid::landr::VectorDataset* Vect3 = new openfluid::landr::VectorDataset(
      Val3);

  std::vector<geos::geom::LineString*> Field_lines =
      openfluid::landr::LandRTools::computeVectorOfExteriorRings(*Vect);

  std::vector<geos::geom::LineString*> Soil_lines =
      openfluid::landr::LandRTools::computeVectorOfExteriorRings(*Vect2);

  geos::geom::Geometry* Reach_coll = Vect3->geometries();

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
      openfluid::landr::LandRTools::computeNodedLines(Field_coll, Soil_coll,
                                                  snapTolerance);

  BOOST_CHECK_EQUAL(NodedPolys->size(), 58);
 //   for (unsigned int i = 0; i < NodedPolys->size(); i++)
 //     std::cout << NodedPolys->at(i)->toString() << std::endl;

  std::vector<geos::geom::Geometry*> Polys;
  Polys.assign(NodedPolys->begin(), NodedPolys->end());
  geos::geom::Geometry* Polys_coll = Factory->buildGeometry(Polys);

  std::vector<geos::geom::LineString*>* NodedAll =
      openfluid::landr::LandRTools::computeNodedLines(Polys_coll, Reach_coll,
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
  delete Vect;
  delete Vect2;
  delete Vect3;
}


// =====================================================================
// =====================================================================


BOOST_AUTO_TEST_CASE(check_computeNodedLines_virtual_snap)
{
  openfluid::core::GeoVectorValue Val(
      CONFIGTESTS_INPUT_MISCDATA_DIR + "/landr/virtual", "field2.shp");
  openfluid::core::GeoVectorValue Val2(
      CONFIGTESTS_INPUT_MISCDATA_DIR + "/landr/virtual", "soil.shp");

  openfluid::landr::VectorDataset* Vect = new openfluid::landr::VectorDataset(
      Val);
  openfluid::landr::VectorDataset* Vect2 = new openfluid::landr::VectorDataset(
      Val2);

  std::vector<geos::geom::LineString*> Field_lines =
      openfluid::landr::LandRTools::computeVectorOfExteriorRings(*Vect);

  std::vector<geos::geom::LineString*> Soil_lines =
      openfluid::landr::LandRTools::computeVectorOfExteriorRings(*Vect2);

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
      openfluid::landr::LandRTools::computeNodedLines(Field_coll, Soil_coll,
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
  NodedPolys = openfluid::landr::LandRTools::computeNodedLines(Field_coll,
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
  delete Vect;
  delete Vect2;
}


// =====================================================================
// =====================================================================


BOOST_AUTO_TEST_CASE(check_intersect_2Polygons)
{
  openfluid::core::GeoVectorValue Val(
      CONFIGTESTS_INPUT_MISCDATA_DIR + "/landr", "POLY_TEST.shp");
  openfluid::core::GeoVectorValue ValLine(
      CONFIGTESTS_INPUT_MISCDATA_DIR + "/landr", "LINE_TEST.shp");

  openfluid::landr::VectorDataset* Vect = new openfluid::landr::VectorDataset(
      Val);
  openfluid::landr::VectorDataset* VectLine = new openfluid::landr::VectorDataset(
      ValLine);

  geos::geom::Geometry *Geom1=Vect->geometries();
  geos::geom::Geometry *GeomLine=VectLine->geometries();

  BOOST_CHECK_THROW(
       openfluid::landr::LandRTools::computeIntersectPolygons(GeomLine, Geom1),
       openfluid::base::FrameworkException);

  BOOST_CHECK_THROW(
      openfluid::landr::LandRTools::computeIntersectPolygons(Geom1, GeomLine),
      openfluid::base::FrameworkException);

  delete VectLine;

  openfluid::core::GeoVectorValue Val2(
      CONFIGTESTS_INPUT_MISCDATA_DIR + "/landr", "POLY_TEST2.shp");

  openfluid::landr::VectorDataset* Vect2 = new openfluid::landr::VectorDataset(
      Val2);

  geos::geom::Geometry *Geom2=Vect2->geometries();

  std::vector<geos::geom::Polygon*> IntersectPolys  =
      openfluid::landr::LandRTools::computeIntersectPolygons(Geom1, Geom2);
//  for (unsigned int i = 0; i < IntersectPolys.size(); i++)
//    std::cout << IntersectPolys.at(i)->toString() << std::endl;
  BOOST_CHECK_EQUAL(IntersectPolys.size(), 6);

  delete Vect;
  delete Vect2;


}


// =====================================================================
// =====================================================================


BOOST_AUTO_TEST_CASE(check_splitLineStringByPoint)
{
  openfluid::core::GeoVectorValue* Val = new openfluid::core::GeoVectorValue(
      CONFIGTESTS_INPUT_MISCDATA_DIR + "/landr", "RS.shp");

  openfluid::landr::LineStringGraph* Graph =
      openfluid::landr::LineStringGraph::create(*Val);


  std::vector<geos::geom::LineString*> vEntities;

  geos::geom::Coordinate CoorNode;
  CoorNode=Graph->entity(7)->startNode()->getCoordinate();

  geos::geom::Point * PointNode=geos::geom::GeometryFactory::getDefaultInstance()->createPoint(CoorNode);

  vEntities=
  openfluid::landr::LandRTools::splitLineStringByPoint(*const_cast<geos::geom::LineString*>(Graph->entity(7)->line()),
                                                       *PointNode,0.01);
  BOOST_CHECK(vEntities.empty());

  geos::geom::Coordinate Coor;
  Coor.x=679585;
  Coor.y=132354;
  geos::geom::Point * Point=geos::geom::GeometryFactory::getDefaultInstance()->createPoint(Coor);


  BOOST_CHECK_THROW(
  openfluid::landr::LandRTools::splitLineStringByPoint(*const_cast<geos::geom::LineString*>(Graph->entity(7)->line()),
                                                       *Point,0),
                                                       openfluid::base::FrameworkException);


  vEntities=
  openfluid::landr::LandRTools::splitLineStringByPoint(*const_cast<geos::geom::LineString*>(Graph->entity(7)->line()),
                                                       *Point,0.0001);
  BOOST_CHECK(vEntities.empty());

  vEntities=
  openfluid::landr::LandRTools::splitLineStringByPoint(*const_cast<geos::geom::LineString*>(Graph->entity(7)->line()),
                                                       *Point,1);

  BOOST_CHECK_EQUAL(vEntities.size(),2);

  BOOST_CHECK( openfluid::scientific::isVeryClose(Graph->entity(7)->getLength(),
               (vEntities[0]->getLength()+vEntities[1]->getLength())));
  double distance=vEntities[0]->getEndPoint()->getCoordinate()->distance(*(Point->getCoordinate()));
  BOOST_CHECK(distance<1);
  distance=vEntities[1]->getStartPoint()->getCoordinate()->distance(*(Point->getCoordinate()));
  BOOST_CHECK(distance<1);

  delete Val;
  delete Graph;
  delete Point;
  delete PointNode;


}


// =====================================================================
// =====================================================================


BOOST_AUTO_TEST_CASE(check_splitLineStringByPoints)
{
  openfluid::core::GeoVectorValue* Val = new openfluid::core::GeoVectorValue(
      CONFIGTESTS_INPUT_MISCDATA_DIR + "/landr", "RS.shp");

  openfluid::landr::LineStringGraph* Graph =
      openfluid::landr::LineStringGraph::create(*Val);


  std::vector<geos::geom::LineString*> vEntities;
  std::vector<geos::geom::Point*> vPoints;

  geos::geom::Coordinate CoorNode;
  CoorNode=Graph->entity(7)->startNode()->getCoordinate();

  geos::geom::Point * PointStartNode=geos::geom::GeometryFactory::getDefaultInstance()->createPoint(CoorNode);
  vPoints.push_back(PointStartNode);

  CoorNode=Graph->entity(7)->endNode()->getCoordinate();
  geos::geom::Point * PointEndNode=geos::geom::GeometryFactory::getDefaultInstance()->createPoint(CoorNode);
  vPoints.push_back(PointEndNode);

  BOOST_CHECK_THROW(openfluid::landr::LandRTools::splitLineStringByPoints
                    (*const_cast<geos::geom::LineString*>(Graph->entity(7)->line()),
                    vPoints,0,vEntities),
                    openfluid::base::FrameworkException);

  openfluid::landr::LandRTools::splitLineStringByPoints(*const_cast<geos::geom::LineString*>(Graph->entity(7)->line()),
                                                        vPoints,0.01,vEntities);
  BOOST_CHECK_EQUAL(vEntities.size(),1);

  vPoints.clear();

  geos::geom::Coordinate Coor;
  Coor.x=679585;
  Coor.y=132354;
  geos::geom::Point * Point1=geos::geom::GeometryFactory::getDefaultInstance()->createPoint(Coor);
  vPoints.push_back(Point1);

  Coor.x=679535;
  Coor.y=132347;
  geos::geom::Point * Point2=geos::geom::GeometryFactory::getDefaultInstance()->createPoint(Coor);
  vPoints.push_back(Point2);

  vEntities.clear();

  openfluid::landr::LandRTools::splitLineStringByPoints(*const_cast<geos::geom::LineString*>(Graph->entity(7)->line()),
                                                        vPoints,0.0001,vEntities);
  BOOST_CHECK_EQUAL(vEntities.size(),1);

  vEntities.clear();

  openfluid::landr::LandRTools::splitLineStringByPoints(*const_cast<geos::geom::LineString*>(Graph->entity(7)->line()),
                                                        vPoints,1,vEntities);
  BOOST_CHECK_EQUAL(vEntities.size(),3);


  BOOST_CHECK( openfluid::scientific::isVeryClose(Graph->entity(7)->getLength(),
             (vEntities[0]->getLength()+vEntities[1]->getLength()+vEntities[2]->getLength())));

  delete Val;
  delete Graph;
  delete PointStartNode;
  delete PointEndNode;
  delete Point1;
  delete Point2;

}


// =====================================================================
// =====================================================================


BOOST_AUTO_TEST_CASE(check_intersect_horseshoe_with_polygon)
{
  // intersect HorseShoe with simple polygon
  openfluid::core::GeoVectorValue Val(
      CONFIGTESTS_INPUT_MISCDATA_DIR + "/landr", "POLY_TEST.shp");
  openfluid::core::GeoVectorValue ValHorse(
        CONFIGTESTS_INPUT_MISCDATA_DIR + "/landr", "POLY_HORSESHOE.shp");

  openfluid::landr::VectorDataset* Vect = new openfluid::landr::VectorDataset(
      Val);
  openfluid::landr::VectorDataset* VectHorse = new openfluid::landr::VectorDataset(
      ValHorse);

  geos::geom::Geometry *Geom1=Vect->geometries();
  geos::geom::Geometry *GeomHorse=VectHorse->geometries();

  std::vector<geos::geom::Polygon*> IntersectPolys  =
      openfluid::landr::LandRTools::computeIntersectPolygons(Geom1, GeomHorse);
  BOOST_CHECK_EQUAL(IntersectPolys.size(), 3);

  std::vector<geos::geom::Polygon*>::iterator it=IntersectPolys.begin();
  std::vector<geos::geom::Polygon*>::iterator ite=IntersectPolys.end();
  double area=0.0;

  for (;it!=ite;++it)
      area=area+(*it)->getArea();

  BOOST_CHECK( openfluid::scientific::isVeryClose(area,0.12999));

  // intersect HorseShoe with polygon with hole
  IntersectPolys.clear();
  openfluid::core::GeoVectorValue ValHole(
      CONFIGTESTS_INPUT_MISCDATA_DIR + "/landr", "SU-has-hole.shp");

  openfluid::landr::VectorDataset* VectHole = new openfluid::landr::VectorDataset(
      ValHole);

  geos::geom::Geometry *GeomHole=VectHole->geometries();
  IntersectPolys  =  openfluid::landr::LandRTools::computeIntersectPolygons(GeomHole, GeomHorse);
  BOOST_CHECK_EQUAL(IntersectPolys.size(), 3);
  area=0.0;
  it=IntersectPolys.begin();
  ite=IntersectPolys.end();

  for (;it!=ite;++it)
    area=area+(*it)->getArea();

  BOOST_CHECK( openfluid::scientific::isVeryClose(area,0.69474));


  // intersect HorseShoe with polygon with island
  IntersectPolys.clear();
  openfluid::core::GeoVectorValue ValIsland(
      CONFIGTESTS_INPUT_MISCDATA_DIR + "/landr", "SU-has-islands.shp");

  openfluid::landr::VectorDataset* VectIsland = new openfluid::landr::VectorDataset(
      ValIsland);

  geos::geom::Geometry *GeomIsland=VectIsland->geometries();
  IntersectPolys  =  openfluid::landr::LandRTools::computeIntersectPolygons(GeomIsland, GeomHorse);
  BOOST_CHECK_EQUAL(IntersectPolys.size(), 4);
  area=0.0;
  it=IntersectPolys.begin();
  ite=IntersectPolys.end();

  for (;it!=ite;++it)
    area=area+(*it)->getArea();

  BOOST_CHECK( openfluid::scientific::isVeryClose(area,0.743386));

  delete Vect;
  delete VectHorse;
  delete VectHole;
  delete VectIsland;

}


// =====================================================================
// =====================================================================


BOOST_AUTO_TEST_CASE(check_cleanLines_after_Intersect2Polys)
{
  double SnapTolerance=0.001;
  openfluid::core::GeoVectorValue ValSU(
      CONFIGTESTS_INPUT_MISCDATA_DIR + "/landr", "fields_extract2.shp");
  openfluid::core::GeoVectorValue ValSU2(
      CONFIGTESTS_INPUT_MISCDATA_DIR + "/landr", "soils_extract3.shp");

  openfluid::landr::VectorDataset* VectSU = new openfluid::landr::VectorDataset(
      ValSU);

  openfluid::landr::VectorDataset* VectSU2 =
      new openfluid::landr::VectorDataset(ValSU2);

  geos::geom::Geometry *Geom1=VectSU->geometries();
  geos::geom::Geometry *Geom2=VectSU2->geometries();


  std::vector<geos::geom::Polygon*> IntersectPolys  =
      openfluid::landr::LandRTools::computeIntersectPolygons(Geom1, Geom2);

  std::vector<geos::geom::Geometry*> GeomPolygons;
  const geos::geom::GeometryFactory* Factory ;
  geos::geom::Geometry* GeomPolygons_coll=0;

  GeomPolygons.assign(IntersectPolys.begin(), IntersectPolys.end());
  Factory= geos::geom::GeometryFactory::getDefaultInstance();
  GeomPolygons_coll = Factory->buildGeometry(GeomPolygons);


  std::vector<geos::geom::LineString*> PolyRings;
  unsigned int i2End=GeomPolygons_coll->getNumGeometries();
  for (unsigned int i = 0; i < i2End; i++)
    PolyRings.push_back(const_cast<geos::geom::LineString*>(dynamic_cast<geos::geom::Polygon*>
                        (const_cast<geos::geom::Geometry*>(GeomPolygons_coll->getGeometryN(i)))->
                        getExteriorRing()));


  std::vector<geos::geom::LineString*>* cleanLines  =
      openfluid::landr::LandRTools::cleanLineStrings(PolyRings,SnapTolerance);

  BOOST_CHECK_EQUAL(cleanLines->size(), 26);

  delete VectSU;
  delete VectSU2;
  delete Geom1;
  delete Geom2;
  unsigned int iEnd=IntersectPolys.size();
  for (unsigned int i = 0; i < iEnd; i++)
    delete IntersectPolys.at(i);

  delete GeomPolygons_coll;

  iEnd=cleanLines->size();
  for (unsigned int i = 0; i < iEnd; i++)
    delete cleanLines->at(i);
  delete cleanLines;

}


// =====================================================================
// =====================================================================


BOOST_AUTO_TEST_CASE(check_computeNodesFromVectorOfLines)
{

  openfluid::core::GeoVectorValue Val(CONFIGTESTS_INPUT_MISCDATA_DIR + "/landr",
                                      "RS.shp");

  openfluid::landr::VectorDataset* Vect = new openfluid::landr::VectorDataset(
      Val);

  std::vector<geos::geom::LineString*> Geoms =
      openfluid::landr::LandRTools::computeVectorOfLines(*Vect);

  std::vector<geos::geom::Point*> Points=openfluid::landr::LandRTools::computeNodesFromVectorOfLines(Geoms);
  BOOST_CHECK_EQUAL(Points.size(),9);

  delete Vect;
}


// =====================================================================
// =====================================================================


BOOST_AUTO_TEST_CASE(markInvertedLineStringEntityUsingDFS)
{
  openfluid::core::GeoVectorValue* Val = new openfluid::core::GeoVectorValue(
      CONFIGTESTS_INPUT_MISCDATA_DIR + "/landr", "badRS_misdirected.shp");

  openfluid::landr::LineStringGraph* Graph =
      openfluid::landr::LineStringGraph::create(*Val);

  openfluid::landr::LineStringEntity * lineEntity=Graph->entity(1);
  Graph->reverseLineStringEntity(*lineEntity);    // reverse the outlet
  lineEntity=Graph->entity(1);
  geos::planargraph::Node * firstNode=lineEntity->endNode();
  std::vector<int> vectIdent;

  openfluid::landr::LandRTools::markInvertedLineStringEntityUsingDFS(firstNode,vectIdent);
  BOOST_CHECK_EQUAL(vectIdent.size(),3);

  delete Val;

}


// =====================================================================
// =====================================================================


BOOST_AUTO_TEST_CASE(check_envelope)
{
  openfluid::core::GeoRasterValue RasterVal(
      CONFIGTESTS_INPUT_MISCDATA_DIR + "/GeoRasterValue", "dem.jpeg");

  openfluid::landr::RasterDataset* Rast = new openfluid::landr::RasterDataset(
      RasterVal);

  std::vector<OGREnvelope> vEnvelope;
  vEnvelope.push_back(Rast->envelope());


  openfluid::core::GeoVectorValue Value(
      CONFIGTESTS_INPUT_MISCDATA_DIR + "/landr", "SU.shp");

  openfluid::landr::VectorDataset* Vect =
      new openfluid::landr::VectorDataset(Value);
  vEnvelope.push_back(Vect->envelope());

  BOOST_CHECK(openfluid::landr::LandRTools::isExtentsIntersect(vEnvelope));


  vEnvelope.clear();
  vEnvelope.push_back(Rast->envelope());

  openfluid::core::GeoVectorValue Value2(
      CONFIGTESTS_INPUT_MISCDATA_DIR + "/landr", "POLY_TEST.shp");

  Vect = new openfluid::landr::VectorDataset(Value2);
  vEnvelope.push_back(Vect->envelope());

  BOOST_CHECK(! openfluid::landr::LandRTools::isExtentsIntersect(vEnvelope));


  delete Rast;
  delete Vect;

}


// =====================================================================
// =====================================================================



