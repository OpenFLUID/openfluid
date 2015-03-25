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
  @file AdvancedDatastoreDescriptor_TEST.cpp

  @author Aline LIBRES <aline.libres@gmail.com>
 */

#define BOOST_TEST_MAIN
#define BOOST_AUTO_TEST_MAIN
#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MODULE unittest_AdvancedDatastoreDescriptor
#include <boost/test/unit_test.hpp>

#include <openfluid/fluidx/AdvancedDatastoreDescriptor.hpp>
#include "tests-config.hpp"
#include <openfluid/fluidx/FluidXDescriptor.hpp>

// =====================================================================
// =====================================================================

BOOST_AUTO_TEST_CASE(check_construction)
{
  openfluid::fluidx::FluidXDescriptor FXDesc(0);
  FXDesc.loadFromDirectory(
      CONFIGTESTS_INPUT_DATASETS_DIR + "/OPENFLUID.IN.AdvancedDescriptors/singlefile");

  openfluid::fluidx::AdvancedDatastoreDescriptor DS(
      FXDesc.datastoreDescriptor());

  BOOST_CHECK_EQUAL(DS.items().size(), 5);

  const std::list<openfluid::fluidx::DatastoreItemDescriptor*> Items =
      DS.items();

  std::list<openfluid::fluidx::DatastoreItemDescriptor*>::const_iterator it =
      Items.begin();

  BOOST_CHECK_EQUAL((*it)->getID(), "units_A");
  BOOST_CHECK_EQUAL((*it)->getType(),
                    openfluid::core::UnstructuredValue::GeoVectorValue);
  BOOST_CHECK_EQUAL((*it)->getRelativePath(), "shapes/units_A.shp");
  BOOST_CHECK_EQUAL((*it)->getUnitsClass(), "unitsA");

  it++;

  BOOST_CHECK_EQUAL((*it)->getID(), "units_B");
  BOOST_CHECK_EQUAL((*it)->getType(),
                    openfluid::core::UnstructuredValue::GeoVectorValue);
  BOOST_CHECK_EQUAL((*it)->getRelativePath(), "units_B.shp");
  BOOST_CHECK_EQUAL((*it)->getUnitsClass(), "unitsB");

  it++;

  BOOST_CHECK_EQUAL((*it)->getID(), "VectWOClass");
  BOOST_CHECK_EQUAL((*it)->getType(),
                    openfluid::core::UnstructuredValue::GeoVectorValue);
  BOOST_CHECK_EQUAL((*it)->getRelativePath(), "shapes/Vect.shp");
  BOOST_CHECK_EQUAL((*it)->getUnitsClass(), "");

  it++;

  BOOST_CHECK_EQUAL((*it)->getID(), "RastWOClass");
  BOOST_CHECK_EQUAL((*it)->getType(),
                    openfluid::core::UnstructuredValue::GeoRasterValue);
  BOOST_CHECK_EQUAL((*it)->getRelativePath(), "shapes/Rast.tif");

  it++;

  BOOST_CHECK_EQUAL((*it)->getID(), "wrong_class");
  BOOST_CHECK_EQUAL((*it)->getType(),
                    openfluid::core::UnstructuredValue::GeoVectorValue);
  BOOST_CHECK_EQUAL((*it)->getRelativePath(), "shapes/wrong_class.shp");
  BOOST_CHECK_EQUAL((*it)->getUnitsClass(), "wrongClass");

  BOOST_CHECK(!DS.isItemAlreadyExist("wrong.item"));
  BOOST_CHECK(DS.isItemAlreadyExist("RastWOClass"));
}

// =====================================================================
// =====================================================================

BOOST_AUTO_TEST_CASE(check_operations)
{
  openfluid::fluidx::FluidXDescriptor FXDesc(0);
  FXDesc.loadFromDirectory(
      CONFIGTESTS_INPUT_DATASETS_DIR + "/OPENFLUID.IN.AdvancedDescriptors/singlefile");

  openfluid::fluidx::AdvancedDatastoreDescriptor DS(
      FXDesc.datastoreDescriptor());


  openfluid::fluidx::DatastoreItemDescriptor* Item =DS.item("units_A");
  BOOST_CHECK_EQUAL(Item->getID(), "units_A");
  BOOST_CHECK_EQUAL(Item->getType(),
                    openfluid::core::UnstructuredValue::GeoVectorValue);
  BOOST_CHECK_EQUAL(Item->getRelativePath(), "shapes/units_A.shp");



  BOOST_CHECK_EQUAL(DS.items().size(), 5);

  const std::list<openfluid::fluidx::DatastoreItemDescriptor*>* Items =
      &(DS.items());

  // appendItem
  openfluid::fluidx::DatastoreItemDescriptor AppItem(
      "appended.item", "", "shape/appened.shp",
      openfluid::core::UnstructuredValue::GeoVectorValue);
  DS.appendItem(&AppItem);

  BOOST_CHECK_EQUAL(DS.items().size(), 6);

  std::list<openfluid::fluidx::DatastoreItemDescriptor*>::const_iterator it =
      Items->begin();

  std::advance(it, 5);

  BOOST_CHECK_EQUAL((*it)->getID(), "appended.item");
  BOOST_CHECK_EQUAL((*it)->getType(),
                    openfluid::core::UnstructuredValue::GeoVectorValue);
  BOOST_CHECK_EQUAL((*it)->getRelativePath(), "shape/appened.shp");

  //insertItem
  openfluid::fluidx::DatastoreItemDescriptor InsItem(
      "inserted.item", "", "inserted.tif",
      openfluid::core::UnstructuredValue::GeoRasterValue);

  BOOST_CHECK_THROW(DS.insertItem(&InsItem, 6), openfluid::base::FrameworkException);

  DS.insertItem(&InsItem, 3);

  BOOST_CHECK_EQUAL(DS.items().size(), 7);

  it = Items->begin();

  BOOST_CHECK_EQUAL((*it)->getID(), "units_A");
  BOOST_CHECK_EQUAL((*(++it))->getID(), "units_B");
  BOOST_CHECK_EQUAL((*(++it))->getID(), "VectWOClass");
  BOOST_CHECK_EQUAL((*(++it))->getID(), "inserted.item");
  BOOST_CHECK_EQUAL((*it)->getType(),
                    openfluid::core::UnstructuredValue::GeoRasterValue);
  BOOST_CHECK_EQUAL((*it)->getRelativePath(), "inserted.tif");
  BOOST_CHECK_EQUAL((*(++it))->getID(), "RastWOClass");
  BOOST_CHECK_EQUAL((*(++it))->getID(), "wrong_class");
  BOOST_CHECK_EQUAL((*(++it))->getID(), "appended.item");

  // moveItem
  BOOST_CHECK_THROW(DS.moveItem(2, 7), openfluid::base::FrameworkException);
  BOOST_CHECK_THROW(DS.moveItem(7, 2), openfluid::base::FrameworkException);

  DS.moveItem(5, 2);

  it = Items->begin();

  BOOST_CHECK_EQUAL((*it)->getID(), "units_A");
  BOOST_CHECK_EQUAL((*(++it))->getID(), "units_B");
  BOOST_CHECK_EQUAL((*(++it))->getID(), "wrong_class");
  BOOST_CHECK_EQUAL((*(++it))->getID(), "VectWOClass");
  BOOST_CHECK_EQUAL((*(++it))->getID(), "inserted.item");
  BOOST_CHECK_EQUAL((*(++it))->getID(), "RastWOClass");
  BOOST_CHECK_EQUAL((*(++it))->getID(), "appended.item");

  DS.moveItem(3, 0);

  it = Items->begin();

  BOOST_CHECK_EQUAL((*it)->getID(), "VectWOClass");
  BOOST_CHECK_EQUAL((*(++it))->getID(), "units_A");
  BOOST_CHECK_EQUAL((*(++it))->getID(), "units_B");
  BOOST_CHECK_EQUAL((*(++it))->getID(), "wrong_class");
  BOOST_CHECK_EQUAL((*(++it))->getID(), "inserted.item");
  BOOST_CHECK_EQUAL((*(++it))->getID(), "RastWOClass");
  BOOST_CHECK_EQUAL((*(++it))->getID(), "appended.item");

  DS.moveItem(5, 6);

  it = Items->begin();

  BOOST_CHECK_EQUAL((*it)->getID(), "VectWOClass");
  BOOST_CHECK_EQUAL((*(++it))->getID(), "units_A");
  BOOST_CHECK_EQUAL((*(++it))->getID(), "units_B");
  BOOST_CHECK_EQUAL((*(++it))->getID(), "wrong_class");
  BOOST_CHECK_EQUAL((*(++it))->getID(), "inserted.item");
  BOOST_CHECK_EQUAL((*(++it))->getID(), "appended.item");
  BOOST_CHECK_EQUAL((*(++it))->getID(), "RastWOClass");

  //removeItem

  BOOST_CHECK_THROW(DS.removeItem(7), openfluid::base::FrameworkException);

  DS.removeItem(4);

  it = Items->begin();

  BOOST_CHECK_EQUAL((*it)->getID(), "VectWOClass");
  BOOST_CHECK_EQUAL((*(++it))->getID(), "units_A");
  BOOST_CHECK_EQUAL((*(++it))->getID(), "units_B");
  BOOST_CHECK_EQUAL((*(++it))->getID(), "wrong_class");
  BOOST_CHECK_EQUAL((*(++it))->getID(), "appended.item");
  BOOST_CHECK_EQUAL((*(++it))->getID(), "RastWOClass");

}

// =====================================================================
// =====================================================================

