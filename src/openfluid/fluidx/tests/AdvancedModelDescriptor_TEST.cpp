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
 \file AdvancedModelDescriptor_TEST.cpp
 \brief Implements ...

 \author Aline LIBRES <aline.libres@gmail.com>
 */

#define BOOST_TEST_MAIN
#define BOOST_AUTO_TEST_MAIN
#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MODULE unittest_AdvancedModelDescriptor
#include <boost/test/unit_test.hpp>

#include <openfluid/fluidx/AdvancedModelDescriptor.hpp>
#include "tests-config.hpp"
#include <openfluid/fluidx/FluidXDescriptor.hpp>
#include <openfluid/fluidx/WareDescriptor.hpp>
#include <openfluid/fluidx/SimulatorDescriptor.hpp>
#include <openfluid/fluidx/GeneratorDescriptor.hpp>
#include <openfluid/fluidx/CoupledModelDescriptor.hpp>

// =====================================================================
// =====================================================================

class AdvancedModelDescriptorSub: public openfluid::fluidx::AdvancedModelDescriptor
{
  public:

    AdvancedModelDescriptorSub(
        openfluid::fluidx::CoupledModelDescriptor& ModelDesc) :
        openfluid::fluidx::AdvancedModelDescriptor()
    {
      mp_ModelDesc = &ModelDesc;
    }
    ;
};

// =====================================================================
// =====================================================================

BOOST_AUTO_TEST_CASE(check_duplicates)
{
  openfluid::fluidx::FluidXDescriptor FXDesc(0);
  FXDesc.loadFromDirectory(
      CONFIGTESTS_INPUT_DATASETS_DIR + "/OPENFLUID.IN.AdvancedDescriptors/duplicates");

  BOOST_CHECK_THROW(
      openfluid::fluidx::AdvancedModelDescriptor(FXDesc.getModelDescriptor()),
      openfluid::base::FrameworkException);
}

// =====================================================================
// =====================================================================

BOOST_AUTO_TEST_CASE(check_construction)
{
  openfluid::fluidx::FluidXDescriptor FXDesc(0);
  FXDesc.loadFromDirectory(
      CONFIGTESTS_INPUT_DATASETS_DIR + "/OPENFLUID.IN.AdvancedDescriptors/singlefile");

  AdvancedModelDescriptorSub Model(FXDesc.getModelDescriptor());

  BOOST_CHECK_EQUAL(Model.getItemsCount(), 5);

  const std::list<openfluid::fluidx::ModelItemDescriptor*> Items =
      Model.getItems();

  std::list<openfluid::fluidx::ModelItemDescriptor*>::const_iterator it =
      Items.begin();

  BOOST_CHECK_EQUAL(
      (dynamic_cast<openfluid::fluidx::GeneratorDescriptor*>(*it))->getGeneratorMethod(),
      openfluid::fluidx::GeneratorDescriptor::Interp);

  BOOST_CHECK_EQUAL(
      (dynamic_cast<openfluid::fluidx::SimulatorDescriptor*>(*(++it)))->getID(),
      "tests.simulatorA");

  BOOST_CHECK_EQUAL(
      (dynamic_cast<openfluid::fluidx::GeneratorDescriptor*>(*(++it)))->getGeneratorMethod(),
      openfluid::fluidx::GeneratorDescriptor::Fixed);

  BOOST_CHECK_EQUAL(
      (dynamic_cast<openfluid::fluidx::GeneratorDescriptor*>(*(++it)))->getGeneratorMethod(),
      openfluid::fluidx::GeneratorDescriptor::Random);

  BOOST_CHECK_EQUAL(
      (dynamic_cast<openfluid::fluidx::SimulatorDescriptor*>(*(++it)))->getID(),
      "tests.simulatorB");

  BOOST_CHECK_EQUAL(
      dynamic_cast<openfluid::fluidx::GeneratorDescriptor*>(Model.getItemAt(0))->getGeneratedID(),
      "tests.generator.interp.TU.genscalar");
  BOOST_CHECK_EQUAL(
      dynamic_cast<openfluid::fluidx::SimulatorDescriptor*>(Model.getItemAt(4))->getID(),
      "tests.simulatorB");
  BOOST_CHECK_THROW(Model.getItemAt(5), openfluid::base::FrameworkException);

  BOOST_CHECK_EQUAL(
      Model.getFirstItemIndex("tests.generator.interp.TU.genscalar"), 0);
  BOOST_CHECK_EQUAL(Model.getFirstItemIndex("tests.simulatorB"), 4);
  BOOST_CHECK_EQUAL(Model.getFirstItemIndex("tests.wrongsimulator"), -1);

  std::vector<std::string> IDs = Model.getOrderedIDs();

  BOOST_CHECK_EQUAL(IDs.size(), 5);
  BOOST_CHECK_EQUAL(IDs.at(0), "tests.generator.interp.TU.genscalar");
  BOOST_CHECK_EQUAL(IDs.at(4), "tests.simulatorB");
}

// =====================================================================
// =====================================================================

BOOST_AUTO_TEST_CASE(check_operations)
{
  openfluid::fluidx::FluidXDescriptor FXDesc(0);
  FXDesc.loadFromDirectory(
      CONFIGTESTS_INPUT_DATASETS_DIR + "/OPENFLUID.IN.AdvancedDescriptors/singlefile");

  AdvancedModelDescriptorSub Model(FXDesc.getModelDescriptor());

  BOOST_CHECK_EQUAL(Model.getItemsCount(), 5);

  const std::list<openfluid::fluidx::ModelItemDescriptor*>* Items =
      &(Model.getItems());

  // appendItem
  openfluid::fluidx::SimulatorDescriptor AppItem("appended.item");
  Model.appendItem(&AppItem);

  BOOST_CHECK_EQUAL(Model.getItemsCount(), 6);

  std::list<openfluid::fluidx::ModelItemDescriptor*>::const_iterator it =
      Items->begin();

  std::advance(it, 5);

  BOOST_CHECK_EQUAL(
      (dynamic_cast<openfluid::fluidx::SimulatorDescriptor*>(*it))->getID(),
      "appended.item");

  //insertItem
  openfluid::fluidx::SimulatorDescriptor InsItem("inserted.item");

  BOOST_CHECK_THROW(Model.insertItem(&InsItem, 6),
                    openfluid::base::FrameworkException);

  Model.insertItem(&InsItem, 3);

  BOOST_CHECK_EQUAL(Model.getItemsCount(), 7);

  it = Items->begin();

  BOOST_CHECK_EQUAL(
      (dynamic_cast<openfluid::fluidx::GeneratorDescriptor*>(*it))->getGeneratorMethod(),
      openfluid::fluidx::GeneratorDescriptor::Interp);

  BOOST_CHECK_EQUAL(
      (dynamic_cast<openfluid::fluidx::SimulatorDescriptor*>(*(++it)))->getID(),
      "tests.simulatorA");

  BOOST_CHECK_EQUAL(
      (dynamic_cast<openfluid::fluidx::GeneratorDescriptor*>(*(++it)))->getGeneratorMethod(),
      openfluid::fluidx::GeneratorDescriptor::Fixed);

  BOOST_CHECK_EQUAL(
      (dynamic_cast<openfluid::fluidx::SimulatorDescriptor*>(*(++it)))->getID(),
      "inserted.item");

  BOOST_CHECK_EQUAL(
      (dynamic_cast<openfluid::fluidx::GeneratorDescriptor*>(*(++it)))->getGeneratorMethod(),
      openfluid::fluidx::GeneratorDescriptor::Random);

  BOOST_CHECK_EQUAL(
      (dynamic_cast<openfluid::fluidx::SimulatorDescriptor*>(*(++it)))->getID(),
      "tests.simulatorB");

  BOOST_CHECK_EQUAL(
      (dynamic_cast<openfluid::fluidx::SimulatorDescriptor*>(*(++it)))->getID(),
      "appended.item");

  // moveItem

  BOOST_CHECK_THROW(Model.moveItem(2, 7), openfluid::base::FrameworkException);
  BOOST_CHECK_THROW(Model.moveItem(7, 2), openfluid::base::FrameworkException);

  Model.moveItem(5, 2);

  it = Items->begin();

  BOOST_CHECK_EQUAL(
      (dynamic_cast<openfluid::fluidx::GeneratorDescriptor*>(*it))->getGeneratorMethod(),
      openfluid::fluidx::GeneratorDescriptor::Interp);

  BOOST_CHECK_EQUAL(
      (dynamic_cast<openfluid::fluidx::SimulatorDescriptor*>(*(++it)))->getID(),
      "tests.simulatorA");

  BOOST_CHECK_EQUAL(
      (dynamic_cast<openfluid::fluidx::SimulatorDescriptor*>(*(++it)))->getID(),
      "tests.simulatorB");

  BOOST_CHECK_EQUAL(
      (dynamic_cast<openfluid::fluidx::GeneratorDescriptor*>(*(++it)))->getGeneratorMethod(),
      openfluid::fluidx::GeneratorDescriptor::Fixed);

  BOOST_CHECK_EQUAL(
      (dynamic_cast<openfluid::fluidx::SimulatorDescriptor*>(*(++it)))->getID(),
      "inserted.item");

  BOOST_CHECK_EQUAL(
      (dynamic_cast<openfluid::fluidx::GeneratorDescriptor*>(*(++it)))->getGeneratorMethod(),
      openfluid::fluidx::GeneratorDescriptor::Random);

  BOOST_CHECK_EQUAL(
      (dynamic_cast<openfluid::fluidx::SimulatorDescriptor*>(*(++it)))->getID(),
      "appended.item");

  Model.moveItem(3, 0);

  it = Items->begin();

  BOOST_CHECK_EQUAL(
      (dynamic_cast<openfluid::fluidx::GeneratorDescriptor*>(*it))->getGeneratorMethod(),
      openfluid::fluidx::GeneratorDescriptor::Fixed);

  BOOST_CHECK_EQUAL(
      (dynamic_cast<openfluid::fluidx::GeneratorDescriptor*>(*(++it)))->getGeneratorMethod(),
      openfluid::fluidx::GeneratorDescriptor::Interp);

  BOOST_CHECK_EQUAL(
      (dynamic_cast<openfluid::fluidx::SimulatorDescriptor*>(*(++it)))->getID(),
      "tests.simulatorA");

  BOOST_CHECK_EQUAL(
      (dynamic_cast<openfluid::fluidx::SimulatorDescriptor*>(*(++it)))->getID(),
      "tests.simulatorB");

  BOOST_CHECK_EQUAL(
      (dynamic_cast<openfluid::fluidx::SimulatorDescriptor*>(*(++it)))->getID(),
      "inserted.item");

  BOOST_CHECK_EQUAL(
      (dynamic_cast<openfluid::fluidx::GeneratorDescriptor*>(*(++it)))->getGeneratorMethod(),
      openfluid::fluidx::GeneratorDescriptor::Random);

  BOOST_CHECK_EQUAL(
      (dynamic_cast<openfluid::fluidx::SimulatorDescriptor*>(*(++it)))->getID(),
      "appended.item");

  Model.moveItem(5, 6);

  it = Items->begin();

  BOOST_CHECK_EQUAL(
      (dynamic_cast<openfluid::fluidx::GeneratorDescriptor*>(*it))->getGeneratorMethod(),
      openfluid::fluidx::GeneratorDescriptor::Fixed);

  BOOST_CHECK_EQUAL(
      (dynamic_cast<openfluid::fluidx::GeneratorDescriptor*>(*(++it)))->getGeneratorMethod(),
      openfluid::fluidx::GeneratorDescriptor::Interp);

  BOOST_CHECK_EQUAL(
      (dynamic_cast<openfluid::fluidx::SimulatorDescriptor*>(*(++it)))->getID(),
      "tests.simulatorA");

  BOOST_CHECK_EQUAL(
      (dynamic_cast<openfluid::fluidx::SimulatorDescriptor*>(*(++it)))->getID(),
      "tests.simulatorB");

  BOOST_CHECK_EQUAL(
      (dynamic_cast<openfluid::fluidx::SimulatorDescriptor*>(*(++it)))->getID(),
      "inserted.item");

  BOOST_CHECK_EQUAL(
      (dynamic_cast<openfluid::fluidx::SimulatorDescriptor*>(*(++it)))->getID(),
      "appended.item");

  BOOST_CHECK_EQUAL(
      (dynamic_cast<openfluid::fluidx::GeneratorDescriptor*>(*(++it)))->getGeneratorMethod(),
      openfluid::fluidx::GeneratorDescriptor::Random);

  //removeItem

  BOOST_CHECK_THROW(Model.removeItem(7), openfluid::base::FrameworkException);

  Model.removeItem(4);

  it = Items->begin();

  BOOST_CHECK_EQUAL(
      (dynamic_cast<openfluid::fluidx::GeneratorDescriptor*>(*it))->getGeneratorMethod(),
      openfluid::fluidx::GeneratorDescriptor::Fixed);

  BOOST_CHECK_EQUAL(
      (dynamic_cast<openfluid::fluidx::GeneratorDescriptor*>(*(++it)))->getGeneratorMethod(),
      openfluid::fluidx::GeneratorDescriptor::Interp);

  BOOST_CHECK_EQUAL(
      (dynamic_cast<openfluid::fluidx::SimulatorDescriptor*>(*(++it)))->getID(),
      "tests.simulatorA");

  BOOST_CHECK_EQUAL(
      (dynamic_cast<openfluid::fluidx::SimulatorDescriptor*>(*(++it)))->getID(),
      "tests.simulatorB");

  BOOST_CHECK_EQUAL(
      (dynamic_cast<openfluid::fluidx::SimulatorDescriptor*>(*(++it)))->getID(),
      "appended.item");

  BOOST_CHECK_EQUAL(
      (dynamic_cast<openfluid::fluidx::GeneratorDescriptor*>(*(++it)))->getGeneratorMethod(),
      openfluid::fluidx::GeneratorDescriptor::Random);
}

// =====================================================================
// =====================================================================

