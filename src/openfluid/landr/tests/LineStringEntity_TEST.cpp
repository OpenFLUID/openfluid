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
  @file LineStringEntity_TEST.cpp

  @author Aline LIBRES <aline.libres@gmail.com>
  @author Michael RABOTIN <michael.rabotin@supagro.inra.fr>
 */


#define BOOST_TEST_NO_MAIN
#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MODULE unittest_linestringentity
#include <boost/test/unit_test.hpp>
#include <boost/test/auto_unit_test.hpp>
#include <tests-config.hpp>
#include <openfluid/base/FrameworkException.hpp>
#include <openfluid/base/Environment.hpp>
#include <openfluid/core/GeoVectorValue.hpp>
#include <openfluid/landr/GdalCompat.hpp>
#include <openfluid/landr/LineStringEntity.hpp>
#include <openfluid/landr/LineStringGraph.hpp>
#include <openfluid/landr/VectorDataset.hpp>
#include <geos/planargraph/Node.h>
#include <geos/geom/Geometry.h>
#include <geos/geom/LineString.h>


// =====================================================================
// =====================================================================


BOOST_AUTO_TEST_CASE(check_construction)
{
  openfluid::core::GeoVectorValue Val(CONFIGTESTS_INPUT_MISCDATA_DIR + "/landr",
                                      "RS.shp");

  openfluid::landr::VectorDataset* Vect = new openfluid::landr::VectorDataset(
      Val);

  OGRFeature* FirstFeature = Vect->layer(0)->GetFeature(0);

  OGRGeometry* OGRGeom = FirstFeature->GetGeometryRef();

  geos::geom::Geometry* GeosGeom =
      (geos::geom::Geometry*) openfluid::landr::convertOGRGeometryToGEOS(OGRGeom);

  openfluid::landr::LineStringEntity* Entity =
      new openfluid::landr::LineStringEntity(
          dynamic_cast<geos::geom::LineString*>(GeosGeom->clone()),
          FirstFeature->GetFieldAsInteger("OFLD_ID"));

  BOOST_CHECK_EQUAL(Val.getType(),
                    openfluid::core::UnstructuredValue::GeoVectorValue);

  BOOST_CHECK(Entity->line()->equals(GeosGeom));

//  BOOST_CHECK(Entity->getFeature()->Equal(FirstFeature));

  BOOST_CHECK_EQUAL(Entity->getOfldId(), 5);

  OGRFeature::DestroyFeature(FirstFeature);
  delete Entity;
  delete Vect;
}


// =====================================================================
// =====================================================================


BOOST_AUTO_TEST_CASE(check_clone)
{
  openfluid::core::GeoVectorValue Val(CONFIGTESTS_INPUT_MISCDATA_DIR + "/landr",
                                      "RS.shp");

  openfluid::landr::VectorDataset* Vect = new openfluid::landr::VectorDataset(
      Val);

  OGRFeature* FirstFeature = Vect->layer(0)->GetFeature(0);
  OGRGeometry* OGRGeom = FirstFeature->GetGeometryRef();

  geos::geom::Geometry* GeosGeom =
      (geos::geom::Geometry*) openfluid::landr::convertOGRGeometryToGEOS(OGRGeom);

  openfluid::landr::LineStringEntity* Entity =
      new openfluid::landr::LineStringEntity(
          GeosGeom->clone(), FirstFeature->GetFieldAsInteger("OFLD_ID"));

  OGRFeature::DestroyFeature(FirstFeature);
  delete GeosGeom;
  delete Vect;

  openfluid::landr::LineStringEntity* CopyEntity = Entity->clone();

  BOOST_CHECK(Entity->line()->equals(CopyEntity->line()));
  BOOST_CHECK_EQUAL(Entity->getOfldId(), CopyEntity->getOfldId());

  std::string UnitLineStr = Entity->line()->toString();

  delete Entity;

  std::string CopyUnitLineStr = CopyEntity->line()->toString();

  BOOST_CHECK_EQUAL(UnitLineStr, CopyUnitLineStr);

  delete CopyEntity;
}


// =====================================================================
// =====================================================================


BOOST_AUTO_TEST_CASE(check_nodes)
{
  openfluid::core::GeoVectorValue* Val = new openfluid::core::GeoVectorValue(
      CONFIGTESTS_INPUT_MISCDATA_DIR + "/landr", "RS.shp");

  openfluid::landr::LineStringGraph* Graph =
      openfluid::landr::LineStringGraph::create(*Val);

  openfluid::landr::LineStringEntity* U1 = Graph->entity(1);
  openfluid::landr::LineStringEntity* U2 = Graph->entity(2);
  openfluid::landr::LineStringEntity* U3 = Graph->entity(3);
  openfluid::landr::LineStringEntity* U8 = Graph->entity(8);
  openfluid::landr::LineStringEntity* U7 = Graph->entity(7);

  BOOST_CHECK(
      U1->startNode()->getCoordinate().equals(U2->endNode()->getCoordinate()));
  BOOST_CHECK(
      U2->startNode()->getCoordinate().equals(U3->endNode()->getCoordinate()));
  BOOST_CHECK(
      U2->startNode()->getCoordinate().equals(U8->endNode()->getCoordinate()));
  BOOST_CHECK(
      U2->startNode()->getCoordinate().equals(U7->endNode()->getCoordinate()));

  delete Graph;
  delete Val;
}


// =====================================================================
// =====================================================================


BOOST_AUTO_TEST_CASE(check_LineOrientNeighbours)
{
  openfluid::core::GeoVectorValue* Val = new openfluid::core::GeoVectorValue(
      CONFIGTESTS_INPUT_MISCDATA_DIR + "/landr", "RS.shp");

  openfluid::landr::LineStringGraph* Graph =
      openfluid::landr::LineStringGraph::create(*Val);

  openfluid::landr::LineStringEntity* U1 = Graph->entity(1);
  openfluid::landr::LineStringEntity* U2 = Graph->entity(2);
  openfluid::landr::LineStringEntity* U3 = Graph->entity(3);
  openfluid::landr::LineStringEntity* U8 = Graph->entity(8);
  openfluid::landr::LineStringEntity* U4 = Graph->entity(4);

  std::vector<openfluid::landr::LineStringEntity*> U1Down =
      U1->getLineOrientDownNeighbours();
  std::vector<openfluid::landr::LineStringEntity*> U2Down =
      U2->getLineOrientDownNeighbours();
  std::vector<openfluid::landr::LineStringEntity*> U3Down =
      U3->getLineOrientDownNeighbours();
  std::vector<openfluid::landr::LineStringEntity*> U8Down =
      U8->getLineOrientDownNeighbours();
  std::vector<openfluid::landr::LineStringEntity*> U4Down =
      U4->getLineOrientDownNeighbours();

  std::vector<openfluid::landr::LineStringEntity*> U1Up =
      U1->getLineOrientUpNeighbours();
  std::vector<openfluid::landr::LineStringEntity*> U2Up =
      U2->getLineOrientUpNeighbours();
  std::vector<openfluid::landr::LineStringEntity*> U3Up =
      U3->getLineOrientUpNeighbours();
  std::vector<openfluid::landr::LineStringEntity*> U8Up =
      U8->getLineOrientUpNeighbours();
  std::vector<openfluid::landr::LineStringEntity*> U4Up =
      U4->getLineOrientUpNeighbours();

  BOOST_CHECK_EQUAL(U1Down.size(), 0);
  BOOST_CHECK_EQUAL(U2Down.size(), 1);
  BOOST_CHECK_EQUAL((*U2Down.begin())->getOfldId(), 1);
  BOOST_CHECK_EQUAL(U3Down.size(), 1);
  BOOST_CHECK_EQUAL((*U3Down.begin())->getOfldId(), 2);
  BOOST_CHECK_EQUAL(U8Down.size(), 1);
  BOOST_CHECK_EQUAL((*U8Down.begin())->getOfldId(), 2);
  BOOST_CHECK_EQUAL(U4Down.size(), 1);
  BOOST_CHECK_EQUAL((*U4Down.begin())->getOfldId(), 3);

  BOOST_CHECK_EQUAL(U1Up.size(), 1);
  BOOST_CHECK_EQUAL((*U1Up.begin())->getOfldId(), 2);
  BOOST_CHECK_EQUAL(U2Up.size(), 3);
  BOOST_CHECK_EQUAL(U3Up.size(), 2);
  BOOST_CHECK_EQUAL(U8Up.size(), 0);
  BOOST_CHECK_EQUAL(U4Up.size(), 1);
  BOOST_CHECK_EQUAL((*U4Up.begin())->getOfldId(), 5);

  delete Graph;
  delete Val;
}


// =====================================================================
// =====================================================================


BOOST_AUTO_TEST_CASE(check_Neighbours)
{
  openfluid::core::GeoVectorValue* Val = new openfluid::core::GeoVectorValue(
      CONFIGTESTS_INPUT_MISCDATA_DIR + "/landr", "RS.shp");

  openfluid::landr::LineStringGraph* Graph =
      openfluid::landr::LineStringGraph::create(*Val);

  BOOST_CHECK_EQUAL(Graph->entity(1)->neighbours()->size(), 1);
  BOOST_CHECK_EQUAL(*Graph->entity(1)->neighbours()->begin(),
                    Graph->entity(2));
  BOOST_CHECK_EQUAL(Graph->entity(2)->neighbours()->size(), 4);
  BOOST_CHECK_EQUAL(Graph->entity(3)->neighbours()->size(), 5);
  BOOST_CHECK_EQUAL(Graph->entity(4)->neighbours()->size(), 3);
  BOOST_CHECK_EQUAL(Graph->entity(5)->neighbours()->size(), 1);
  BOOST_CHECK_EQUAL(*Graph->entity(5)->neighbours()->begin(),
                    Graph->entity(4));
  BOOST_CHECK_EQUAL(Graph->entity(6)->neighbours()->size(), 2);
  BOOST_CHECK_EQUAL(Graph->entity(7)->neighbours()->size(), 3);
  BOOST_CHECK_EQUAL(Graph->entity(8)->neighbours()->size(), 3);

  delete Graph;
  delete Val;
}


// =====================================================================
// =====================================================================


BOOST_AUTO_TEST_CASE(check_getLineNeighboursDegree2)
{
  openfluid::core::GeoVectorValue* Val = new openfluid::core::GeoVectorValue(
      CONFIGTESTS_INPUT_MISCDATA_DIR + "/landr", "LineToMerge.shp");

  openfluid::landr::LineStringGraph* Graph =
      openfluid::landr::LineStringGraph::create(*Val);

  std::vector<openfluid::landr::LineStringEntity*> vNeighbours=Graph->entity(25)->getLineNeighboursDegree2();
  BOOST_CHECK_EQUAL(vNeighbours.size(),0);
  vNeighbours.clear();
  vNeighbours=Graph->entity(23)->getLineNeighboursDegree2();
  BOOST_CHECK_EQUAL(vNeighbours.size(),1);
  vNeighbours.clear();
  vNeighbours=Graph->entity(22)->getLineNeighboursDegree2();
  BOOST_CHECK_EQUAL(vNeighbours.size(),2);


  delete Graph;
  delete Val;
}


// =====================================================================
// =====================================================================


int main(int argc, char *argv[])
{
  openfluid::base::Environment::init();

  return ::boost::unit_test::unit_test_main( &init_unit_test, argc, argv );
}
