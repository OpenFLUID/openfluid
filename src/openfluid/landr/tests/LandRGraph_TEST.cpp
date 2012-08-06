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
 \file LandRGraph_TEST.cpp
 \brief Implements ...

 \author Aline LIBRES <aline.libres@gmail.com>
 */

#define BOOST_TEST_MAIN
#define BOOST_AUTO_TEST_MAIN
#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MODULE unittest_landrgraph
#include <boost/test/unit_test.hpp>
#include <boost/test/auto_unit_test.hpp>
#include <boost/filesystem/path.hpp>
#include <tests-config.hpp>
#include <openfluid/base/OFException.hpp>
#include <openfluid/core/GeoVectorValue.hpp>
#include <openfluid/core/GeoRasterValue.hpp>
#include <openfluid/landr/PolygonGraph.hpp>
#include <openfluid/landr/LineStringGraph.hpp>

// =====================================================================
// =====================================================================

BOOST_AUTO_TEST_CASE(check_addRemoveAttribute)
{
  openfluid::core::GeoVectorValue* Val = new openfluid::core::GeoVectorValue(
      CONFIGTESTS_INPUT_DATASETS_DIR + "/landr", "RS.shp");

  openfluid::landr::LineStringGraph* Graph =
      openfluid::landr::LineStringGraph::create(*Val);

  openfluid::landr::LineStringEntity* U1 = Graph->getEntity(1);
  openfluid::landr::LineStringEntity* U2 = Graph->getEntity(2);

  boost::any IntValue = 0;
  boost::any StrValue = std::string("");

  BOOST_CHECK(!U1->setAttributeValue("att",123));
  BOOST_CHECK(!U2->setAttributeValue("att",std::string("val")));
  BOOST_CHECK(!U1->getAttributeValue("att",IntValue));
  BOOST_CHECK(!U2->getAttributeValue("att",StrValue));
  BOOST_CHECK_EQUAL(boost::any_cast<int>(IntValue), 0);
  BOOST_CHECK_EQUAL(boost::any_cast<std::string>(StrValue), "");

  Graph->addAttribute("att");

  BOOST_CHECK(U1->setAttributeValue("att",123));
  BOOST_CHECK(U2->setAttributeValue("att",std::string("val")));
  BOOST_CHECK(U1->getAttributeValue("att",IntValue));
  BOOST_CHECK(U2->getAttributeValue("att",StrValue));
  BOOST_CHECK_EQUAL(boost::any_cast<int>(IntValue), 123);
  BOOST_CHECK_EQUAL(boost::any_cast<std::string>(StrValue), "val");

  Graph->addAttribute("att");

  IntValue = 0;
  StrValue = std::string("");
  BOOST_CHECK(U1->getAttributeValue("att",IntValue));
  BOOST_CHECK(U2->getAttributeValue("att",StrValue));
  BOOST_CHECK_EQUAL(boost::any_cast<int>(IntValue), 123);
  BOOST_CHECK_EQUAL(boost::any_cast<std::string>(StrValue), "val");

  Graph->removeAttribute("att");

  IntValue = 0;
  StrValue = std::string("");
  BOOST_CHECK(!U1->setAttributeValue("att",123));
  BOOST_CHECK(!U2->setAttributeValue("att",std::string("val")));
  BOOST_CHECK(!U1->getAttributeValue("att",IntValue));
  BOOST_CHECK(!U2->getAttributeValue("att",StrValue));
  BOOST_CHECK_EQUAL(boost::any_cast<int>(IntValue), 0);
  BOOST_CHECK_EQUAL(boost::any_cast<std::string>(StrValue), "");

  delete Graph;
  delete Val;
}

// =====================================================================
// =====================================================================

BOOST_AUTO_TEST_CASE(check_getARasterValue_fromPolygonGraph)
{
  openfluid::core::GeoVectorValue* Vector = new openfluid::core::GeoVectorValue(
      CONFIGTESTS_INPUT_DATASETS_DIR + "/landr", "SU.shp");

  openfluid::landr::PolygonGraph* Graph =
      openfluid::landr::PolygonGraph::create(*Vector);

  openfluid::core::GeoRasterValue* Raster = new openfluid::core::GeoRasterValue(
      CONFIGTESTS_INPUT_DATASETS_DIR + "/GeoRasterValue", "dem.jpeg");

  openfluid::landr::LandREntity* U1 = Graph->LandRGraph::getEntity(1);
  openfluid::landr::LandREntity* U20 = Graph->LandRGraph::getEntity(20);

  BOOST_CHECK_THROW(Graph->getRasterValueForEntityCentroid(*U1),
                    openfluid::base::OFException);

  Graph->addAGeoRasterValue(*Raster);

  BOOST_CHECK_EQUAL(*Graph->getRasterValueForEntityCentroid(*U1), 29);
  BOOST_CHECK_EQUAL(*Graph->getRasterValueForEntityCentroid(*U20), 64);

  delete Graph;
  delete Vector;
  delete Raster;
}

// =====================================================================
// =====================================================================

BOOST_AUTO_TEST_CASE(check_getARasterValue_fromLineStringGraph)
{
  openfluid::core::GeoVectorValue* Vector = new openfluid::core::GeoVectorValue(
      CONFIGTESTS_INPUT_DATASETS_DIR + "/landr", "RS.shp");

  openfluid::landr::LineStringGraph* Graph =
      openfluid::landr::LineStringGraph::create(*Vector);

  openfluid::core::GeoRasterValue* Raster = new openfluid::core::GeoRasterValue(
      CONFIGTESTS_INPUT_DATASETS_DIR + "/GeoRasterValue", "dem.jpeg");

  openfluid::landr::LandREntity* U1 = Graph->LandRGraph::getEntity(1);
  openfluid::landr::LandREntity* U3 = Graph->LandRGraph::getEntity(3);

  BOOST_CHECK_THROW(Graph->getRasterValueForEntityCentroid(*U1),
                    openfluid::base::OFException);

  Graph->addAGeoRasterValue(*Raster);

  BOOST_CHECK_EQUAL(*Graph->getRasterValueForEntityCentroid(*U1), 36);
  BOOST_CHECK_EQUAL(*Graph->getRasterValueForEntityCentroid(*U3), 69);

  delete Graph;
  delete Vector;
  delete Raster;
}

// =====================================================================
// =====================================================================

BOOST_AUTO_TEST_CASE(check_getRasterPolygonized)
{
  openfluid::core::GeoVectorValue* Vector = new openfluid::core::GeoVectorValue(
      CONFIGTESTS_INPUT_DATASETS_DIR + "/landr", "SU.shp");

  openfluid::core::GeoRasterValue* Raster = new openfluid::core::GeoRasterValue(
      CONFIGTESTS_INPUT_DATASETS_DIR + "/GeoRasterValue", "dem.jpeg");

  openfluid::landr::PolygonGraph* Graph =
      openfluid::landr::PolygonGraph::create(*Vector);

  BOOST_CHECK_THROW(Graph->getRasterPolygonized(),
                    openfluid::base::OFException);

  Graph->addAGeoRasterValue(*Raster);

  openfluid::core::GeoVectorValue* Polygonized = Graph->getRasterPolygonized();

  BOOST_CHECK(Polygonized);
  BOOST_CHECK_EQUAL(Polygonized->getLayer0()->GetFeatureCount(), 234);

  BOOST_CHECK_EQUAL(
      Polygonized->getLayer0()->GetFeature(0)->GetFieldAsInteger(openfluid::core::GeoRasterValue::getDefaultPolygonizedFieldName().c_str()),
      96);
  BOOST_CHECK_EQUAL(
      Polygonized->getLayer0()->GetFeature(76)->GetFieldAsInteger(openfluid::core::GeoRasterValue::getDefaultPolygonizedFieldName().c_str()),
      83);

  delete Graph;
  delete Vector;
  delete Raster;
}

// =====================================================================
// =====================================================================

BOOST_AUTO_TEST_CASE(check_getRasterPolygonizedMultiPoly)
{
  openfluid::core::GeoVectorValue* Vector = new openfluid::core::GeoVectorValue(
      CONFIGTESTS_INPUT_DATASETS_DIR + "/landr", "SU.shp");

  openfluid::core::GeoRasterValue* Raster = new openfluid::core::GeoRasterValue(
      CONFIGTESTS_INPUT_DATASETS_DIR + "/GeoRasterValue", "dem.jpeg");

  openfluid::landr::PolygonGraph* Graph =
      openfluid::landr::PolygonGraph::create(*Vector);

  BOOST_CHECK_THROW(Graph->getRasterPolygonizedPolys(),
                    openfluid::base::OFException);

  Graph->addAGeoRasterValue(*Raster);

  BOOST_CHECK_EQUAL(Graph->getRasterPolygonizedPolys()->size(), 234);

  delete Graph;
  delete Vector;
  delete Raster;
}

