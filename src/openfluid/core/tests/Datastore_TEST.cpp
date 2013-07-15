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
 \file Datastore_TEST.cpp
 \brief Implements ...

 \author Aline LIBRES <libres@supagro.inra.fr>
 */

#define BOOST_TEST_MAIN
#define BOOST_AUTO_TEST_MAIN
#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MODULE unittest_datastore
#include <boost/test/unit_test.hpp>
#include <boost/test/auto_unit_test.hpp>
#include <openfluid/core/Datastore.hpp>
#include <openfluid/core/DatastoreItem.hpp>
#include <openfluid/base/FrameworkException.hpp>

#include <iostream>

// =====================================================================
// =====================================================================


BOOST_AUTO_TEST_CASE(check_construction)
{
  openfluid::core::Datastore* Store = new openfluid::core::Datastore();

  BOOST_CHECK_EQUAL(Store->getItems().size(),0);

  delete Store;
}

// =====================================================================
// =====================================================================

BOOST_AUTO_TEST_CASE(check_addItem_regular)
{
  openfluid::core::DatastoreItem* VectSUItem =
      new openfluid::core::DatastoreItem("mymap","path1","datastore/testvect",
          openfluid::core::UnstructuredValue::GeoVectorValue, "SU");

  openfluid::core::DatastoreItem* VectItem =
      new openfluid::core::DatastoreItem("mymap2","path2","datastore/testvect.shp",
          openfluid::core::UnstructuredValue::GeoVectorValue);

  openfluid::core::DatastoreItem* RastSUItem =
      new openfluid::core::DatastoreItem("myrast","path3","datastore/testrast.tif",
          openfluid::core::UnstructuredValue::GeoRasterValue);

  openfluid::core::Datastore* Store = new openfluid::core::Datastore();

  Store->addItem(VectSUItem);
  Store->addItem(VectItem);
  Store->addItem(RastSUItem);

  BOOST_CHECK_EQUAL(Store->getItems().size(),3);

  BOOST_CHECK_EQUAL(Store->getItems().at("mymap")->getID(),"mymap");
  BOOST_CHECK_EQUAL(Store->getItems().find("mymap")->second->getID(),"mymap");

  openfluid::core::Datastore::DataItemsById_t::iterator i;
  openfluid::core::DatastoreItem* Item;

  i = Store->getItems().find("mymap");
  Item = i->second;
  BOOST_REQUIRE(Item != 0);
  BOOST_CHECK_EQUAL(Item->getID(),"mymap");
  BOOST_CHECK_EQUAL(Item->getPrefixPath(),"path1");
  BOOST_CHECK_EQUAL(Item->getRelativePath(),"datastore/testvect");
  BOOST_CHECK_EQUAL(Item->getUnitClass(),"SU");
  BOOST_REQUIRE(Item->getValue() != 0);
  BOOST_CHECK_EQUAL(Item->getValue()->getType(),openfluid::core::UnstructuredValue::GeoVectorValue);

  i = Store->getItems().find("mymap2");
  Item = i->second;
  BOOST_REQUIRE(Item != 0);
  BOOST_CHECK_EQUAL(Item->getID(),"mymap2");
  BOOST_CHECK_EQUAL(Item->getPrefixPath(),"path2");
  BOOST_CHECK_EQUAL(Item->getRelativePath(),"datastore/testvect.shp");
  BOOST_CHECK_EQUAL(Item->getUnitClass(),"");
  BOOST_REQUIRE(Item->getValue() != 0);
  BOOST_CHECK_EQUAL(Item->getValue()->getType(),openfluid::core::UnstructuredValue::GeoVectorValue);

  i = Store->getItems().find("myrast");
  Item = i->second;
  BOOST_REQUIRE(Item != 0);
  BOOST_CHECK_EQUAL(Item->getID(),"myrast");
  BOOST_CHECK_EQUAL(Item->getPrefixPath(),"path3");
  BOOST_CHECK_EQUAL(Item->getRelativePath(),"datastore/testrast.tif");
  BOOST_CHECK_EQUAL(Item->getUnitClass(),"");
  BOOST_REQUIRE(Item->getValue() != 0);
  BOOST_CHECK_EQUAL(Item->getValue()->getType(),openfluid::core::UnstructuredValue::GeoRasterValue);

  delete Store;
}

// =====================================================================
// =====================================================================

BOOST_AUTO_TEST_CASE(check_getItem)
{
  openfluid::core::DatastoreItem* VectSUItem =
      new openfluid::core::DatastoreItem("mymap","","datastore/testvect",
          openfluid::core::UnstructuredValue::GeoVectorValue, "SU");

  openfluid::core::DatastoreItem* VectItem =
      new openfluid::core::DatastoreItem("mymap2","","datastore/testvect.shp",
          openfluid::core::UnstructuredValue::GeoVectorValue);

  openfluid::core::Datastore* Store = new openfluid::core::Datastore();

  Store->addItem(VectSUItem);
  Store->addItem(VectItem);

  BOOST_CHECK_EQUAL(Store->getItem("mymap")->getID(),"mymap");
  BOOST_CHECK(!Store->getItem("wrongId"));
  BOOST_CHECK_EQUAL(Store->getItem("mymap2")->getID(),"mymap2");

  delete Store;
}

// =====================================================================
// =====================================================================
