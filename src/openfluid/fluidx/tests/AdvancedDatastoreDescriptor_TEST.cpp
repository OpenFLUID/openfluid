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
 \file AdvancedDatastoreDescriptor_TEST.cpp
 \brief Implements ...

 \author Aline LIBRES <aline.libres@gmail.com>
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
      FXDesc.getDatastoreDescriptor());

  BOOST_CHECK_EQUAL(DS.getItems().size(), 5);

  const std::list<openfluid::fluidx::DatastoreItemDescriptor*> Items =
      DS.getItems();

  std::list<openfluid::fluidx::DatastoreItemDescriptor*>::const_iterator it =
      Items.begin();

  BOOST_CHECK_EQUAL((*it)->getID(), "units_A");
  BOOST_CHECK_EQUAL((*it)->getType(),
                    openfluid::core::UnstructuredValue::GeoVectorValue);
  BOOST_CHECK_EQUAL((*it)->getRelativePath(), "shapes/units_A.shp");
  BOOST_CHECK_EQUAL((*it)->getUnitClass(), "unitsA");

  it++;

  BOOST_CHECK_EQUAL((*it)->getID(), "units_B");
  BOOST_CHECK_EQUAL((*it)->getType(),
                    openfluid::core::UnstructuredValue::GeoVectorValue);
  BOOST_CHECK_EQUAL((*it)->getRelativePath(), "units_B.shp");
  BOOST_CHECK_EQUAL((*it)->getUnitClass(), "unitsB");

  it++;

  BOOST_CHECK_EQUAL((*it)->getID(), "VectWOClass");
  BOOST_CHECK_EQUAL((*it)->getType(),
                    openfluid::core::UnstructuredValue::GeoVectorValue);
  BOOST_CHECK_EQUAL((*it)->getRelativePath(), "shapes/Vect.shp");
  BOOST_CHECK_EQUAL((*it)->getUnitClass(), "");

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
  BOOST_CHECK_EQUAL((*it)->getUnitClass(), "wrongClass");

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
      FXDesc.getDatastoreDescriptor());

  BOOST_CHECK_EQUAL(DS.getItems().size(), 5);

  const std::list<openfluid::fluidx::DatastoreItemDescriptor*>* Items =
      &(DS.getItems());

  // appendItem
  openfluid::fluidx::DatastoreItemDescriptor AppItem(
      "appended.item", "", "shape/appened.shp",
      openfluid::core::UnstructuredValue::GeoVectorValue);
  DS.appendItem(&AppItem);

  BOOST_CHECK_EQUAL(DS.getItems().size(), 6);

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

  BOOST_CHECK_EQUAL(DS.getItems().size(), 7);

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

