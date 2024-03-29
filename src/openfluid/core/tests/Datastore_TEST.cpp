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
  @file Datastore_TEST.cpp

  @author Aline LIBRES <libres@supagro.inra.fr>
 */


#define BOOST_TEST_MAIN
#define BOOST_AUTO_TEST_MAIN
#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MODULE unittest_datastore


#include <iostream>

#include <boost/test/unit_test.hpp>

#include <openfluid/core/Datastore.hpp>
#include <openfluid/core/DatastoreItem.hpp>
#include <openfluid/base/FrameworkException.hpp>


// =====================================================================
// =====================================================================


BOOST_AUTO_TEST_CASE(check_construction)
{
  openfluid::core::Datastore Store;

  BOOST_CHECK_EQUAL(Store.getItems().size(),0);
}


// =====================================================================
// =====================================================================


BOOST_AUTO_TEST_CASE(check_addItem_regular)
{
  openfluid::core::Datastore Store;

  Store.addItem(openfluid::core::DatastoreItem("mymap","path1","datastore/testvect",
                  openfluid::core::UnstructuredValue::UnstructuredType::VECTOR, "SU"));
  Store.addItem(openfluid::core::DatastoreItem("mymap2","path2","datastore/testvect.shp",
                  openfluid::core::UnstructuredValue::UnstructuredType::VECTOR));
  Store.addItem(openfluid::core::DatastoreItem("myrast","path3","datastore/testrast.tif",
                  openfluid::core::UnstructuredValue::UnstructuredType::RASTER));

  BOOST_CHECK_EQUAL(Store.getItems().size(),3);

  BOOST_CHECK_EQUAL(Store.getItems().at("mymap").getID(),"mymap");
  BOOST_CHECK_EQUAL(Store.getItems().find("mymap")->second.getID(),"mymap");

  openfluid::core::Datastore::DataItemsById_t::iterator it;

  it = Store.getItems().find("mymap");
  BOOST_REQUIRE(it != Store.getItems().end());
  auto Item = (*it).second;
  BOOST_CHECK_EQUAL(Item.getID(),"mymap");
  BOOST_CHECK_EQUAL(Item.getPrefixPath(),"path1");
  BOOST_CHECK_EQUAL(Item.getRelativePath(),"datastore/testvect");
  BOOST_CHECK_EQUAL(Item.getUnitsClass(),"SU");
  BOOST_REQUIRE(Item.value() != nullptr);
  BOOST_CHECK(Item.value()->getType() == openfluid::core::UnstructuredValue::UnstructuredType::VECTOR);
                    
  it = Store.getItems().find("mymap2");
  Item = (*it).second;
  BOOST_CHECK_EQUAL(Item.getID(),"mymap2");
  BOOST_CHECK_EQUAL(Item.getPrefixPath(),"path2");
  BOOST_CHECK_EQUAL(Item.getRelativePath(),"datastore/testvect.shp");
  BOOST_CHECK_EQUAL(Item.getUnitsClass(),"");
  BOOST_REQUIRE(Item.value() != nullptr);
  BOOST_CHECK(Item.value()->getType() == openfluid::core::UnstructuredValue::UnstructuredType::VECTOR);

  it = Store.getItems().find("myrast");
  Item = (*it).second;
  BOOST_CHECK_EQUAL(Item.getID(),"myrast");
  BOOST_CHECK_EQUAL(Item.getPrefixPath(),"path3");
  BOOST_CHECK_EQUAL(Item.getRelativePath(),"datastore/testrast.tif");
  BOOST_CHECK_EQUAL(Item.getUnitsClass(),"");
  BOOST_REQUIRE(Item.value() != nullptr);
  BOOST_CHECK(Item.value()->getType() == openfluid::core::UnstructuredValue::UnstructuredType::RASTER);
}


// =====================================================================
// =====================================================================


BOOST_AUTO_TEST_CASE(check_getItem)
{
  openfluid::core::Datastore Store;

  Store.addItem(openfluid::core::DatastoreItem("mymap","","datastore/testvect",
                  openfluid::core::UnstructuredValue::UnstructuredType::VECTOR, "SU"));
  Store.addItem(openfluid::core::DatastoreItem("mymap2","","datastore/testvect.shp",
                  openfluid::core::UnstructuredValue::UnstructuredType::VECTOR));

  BOOST_CHECK_EQUAL(Store.item("mymap")->getID(),"mymap");
  BOOST_CHECK(!Store.item("wrongId"));
  BOOST_CHECK_EQUAL(Store.item("mymap2")->getID(),"mymap2");
}

