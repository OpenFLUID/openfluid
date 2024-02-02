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
  @file CoupledModelDescriptor_TEST.cpp

  @author Jean-Christophe FABRE <jean-christophe.fabre@inra.fr>
  @author Armel THÖNI <armel.thoni@inrae.fr>
*/


#define BOOST_TEST_MAIN
#define BOOST_AUTO_TEST_MAIN
#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MODULE unittest_coupledmodeldescriptor


#include <boost/test/unit_test.hpp>

#include <openfluid/core/Dimensions.hpp>
#include <openfluid/fluidx/SimulatorDescriptor.hpp>
#include <openfluid/fluidx/GeneratorDescriptor.hpp>
#include <openfluid/fluidx/CoupledModelDescriptor.hpp>
#include <openfluid/fluidx/FluidXIO.hpp>
#include <openfluid/base/IOListener.hpp>

#include "tests-config.hpp"


// =====================================================================
// =====================================================================


BOOST_AUTO_TEST_CASE(check_construction)
{
  openfluid::fluidx::SimulatorDescriptor SimDesc("test.id");

  BOOST_REQUIRE_EQUAL(SimDesc.getID(),"test.id");
  BOOST_REQUIRE_EQUAL(SimDesc.isType(openfluid::ware::WareType::SIMULATOR),true);
  BOOST_REQUIRE_EQUAL(SimDesc.getParameters().size(),0);

  openfluid::fluidx::GeneratorDescriptor GenDesc1({{"test.unitclass","test.var"}},
                                                  openfluid::fluidx::GeneratorDescriptor::GeneratorMethod::FIXED);

  BOOST_REQUIRE_EQUAL(GenDesc1.getVariableName(),"test.var");
  BOOST_REQUIRE_EQUAL(GenDesc1.getUnitsClass(),"test.unitclass");
  BOOST_REQUIRE_EQUAL(static_cast<int>(GenDesc1.getGeneratorMethod()), 
                      static_cast<int>(openfluid::fluidx::GeneratorDescriptor::GeneratorMethod::FIXED));
  BOOST_REQUIRE_EQUAL(GenDesc1.getVariableSize(),1);
  BOOST_REQUIRE_EQUAL(GenDesc1.getParameters().size(),0);

  openfluid::fluidx::GeneratorDescriptor GenDesc2({{"test.unitclass2","test.var2"}},
                                                  openfluid::fluidx::GeneratorDescriptor::GeneratorMethod::INTERP,
                                                  openfluid::core::Value::Type::DOUBLE,
                                                  openfluid::core::Dimensions(13));

  BOOST_REQUIRE_EQUAL(GenDesc2.getVariableName(),"test.var2");
  BOOST_REQUIRE_EQUAL(GenDesc2.getUnitsClass(),"test.unitclass2");
  BOOST_REQUIRE_EQUAL(static_cast<int>(GenDesc2.getGeneratorMethod()), 
                      static_cast<int>(openfluid::fluidx::GeneratorDescriptor:: GeneratorMethod::INTERP));
  BOOST_REQUIRE_EQUAL(GenDesc2.getVariableSize(),13);
  BOOST_REQUIRE_EQUAL(GenDesc2.getParameters().size(),0);

  openfluid::fluidx::CoupledModelDescriptor ModelDesc;

  BOOST_REQUIRE_EQUAL(ModelDesc.items().size(),0);

}


// =====================================================================
// =====================================================================


BOOST_AUTO_TEST_CASE(check_operations)
{
  openfluid::fluidx::SimulatorDescriptor SimDesc("test.id");
  SimDesc.setParameter("param1",std::string("var1"));
  SimDesc.setParameter("param2",std::string("var2"));
  SimDesc.setParameter("param3",std::string("var3"));
  SimDesc.setParameter("param2",std::string("var22"));


  openfluid::fluidx::GeneratorDescriptor GenDesc1({{"test.unitclass","test.var"}},
                                                  openfluid::fluidx::GeneratorDescriptor:: GeneratorMethod::FIXED,
                                                  openfluid::core::Value::Type::DOUBLE,
                                                  openfluid::core::Dimensions(7));
  GenDesc1.setParameter("fixedvalue",std::string("20.5"));

  openfluid::fluidx::GeneratorDescriptor GenDesc2({{"test.unitclass2","test.var2"}},
                                                  openfluid::fluidx::GeneratorDescriptor:: GeneratorMethod::INTERP);
  GenDesc2.setParameter("sources",std::string("datasources.xml"));
  GenDesc2.setParameter("distribution",std::string("distribution.dat"));

  openfluid::fluidx::CoupledModelDescriptor ModelDesc;

  ModelDesc.appendItem(&GenDesc1);
  ModelDesc.appendItem(&SimDesc);
  ModelDesc.appendItem(&GenDesc2);

  BOOST_REQUIRE_EQUAL(ModelDesc.items().size(),3);

  openfluid::fluidx::CoupledModelDescriptor::SetDescription_t ModelItems;

  ModelItems = ModelDesc.items();

  BOOST_REQUIRE_EQUAL(ModelItems.size(),3);

  openfluid::fluidx::CoupledModelDescriptor::SetDescription_t::iterator it;


  it = ModelItems.begin();
  BOOST_REQUIRE_EQUAL((*it)->isType(openfluid::ware::WareType::GENERATOR),true);
  BOOST_REQUIRE_EQUAL(((openfluid::fluidx::GeneratorDescriptor*)(*it))->getVariableName(),"test.var");
  BOOST_REQUIRE_EQUAL(((openfluid::fluidx::GeneratorDescriptor*)(*it))->getUnitsClass(),"test.unitclass");
  BOOST_REQUIRE_EQUAL(static_cast<int>(((openfluid::fluidx::GeneratorDescriptor*)(*it))->getGeneratorMethod()), 
                      static_cast<int>(openfluid::fluidx::GeneratorDescriptor:: GeneratorMethod::FIXED));
  BOOST_REQUIRE_EQUAL(((openfluid::fluidx::GeneratorDescriptor*)(*it))->getVariableSize(),7);
  BOOST_REQUIRE_EQUAL(((openfluid::fluidx::GeneratorDescriptor*)(*it))->getParameters().size(),1);

  it++;
  BOOST_REQUIRE_EQUAL((*it)->isType(openfluid::ware::WareType::SIMULATOR),true);
  BOOST_REQUIRE_EQUAL(((openfluid::fluidx::SimulatorDescriptor*)(*it))->getID(),"test.id");
  BOOST_REQUIRE_EQUAL(((openfluid::fluidx::SimulatorDescriptor*)(*it))->getParameters().size(),3);

  it++;
  BOOST_REQUIRE_EQUAL((*it)->isType(openfluid::ware::WareType::GENERATOR),true);
  BOOST_REQUIRE_EQUAL(((openfluid::fluidx::GeneratorDescriptor*)(*it))->getVariableName(),"test.var2");
  BOOST_REQUIRE_EQUAL(((openfluid::fluidx::GeneratorDescriptor*)(*it))->getUnitsClass(),"test.unitclass2");
  BOOST_REQUIRE_EQUAL(static_cast<int>(((openfluid::fluidx::GeneratorDescriptor*)(*it))->getGeneratorMethod()), 
                      static_cast<int>(openfluid::fluidx::GeneratorDescriptor:: GeneratorMethod::INTERP));
  BOOST_REQUIRE_EQUAL(((openfluid::fluidx::GeneratorDescriptor*)(*it))->getVariableSize(),1);
  BOOST_REQUIRE_EQUAL(((openfluid::fluidx::GeneratorDescriptor*)(*it))->getParameters().size(),2);

}


// =====================================================================
// =====================================================================


BOOST_AUTO_TEST_CASE(check_construction_from_dataset)
{
  std::unique_ptr<openfluid::base::IOListener> Listener = std::make_unique<openfluid::base::IOListener>();
  openfluid::fluidx::FluidXIO FXIO(Listener.get());

  auto FXDesc = FXIO.loadFromDirectory(CONFIGTESTS_INPUT_DATASETS_DIR + "/OPENFLUID.IN.FluidXIO/singlefile0");

  openfluid::fluidx::CoupledModelDescriptor Model(FXDesc.model());

  BOOST_CHECK_EQUAL(Model.getItemsCount(), 5);

  const openfluid::fluidx::CoupledModelDescriptor::SetDescription_t Items =  Model.items();

  std::list<openfluid::fluidx::ModelItemDescriptor*>::const_iterator it =
      Items.begin();

  BOOST_CHECK_EQUAL(static_cast<int>(
                      dynamic_cast<openfluid::fluidx::GeneratorDescriptor*>(*it)->getGeneratorMethod()), 
                    static_cast<int>(openfluid::fluidx::GeneratorDescriptor:: GeneratorMethod::INTERP));

  BOOST_CHECK_EQUAL(
      (dynamic_cast<openfluid::fluidx::SimulatorDescriptor*>(*(++it)))->getID(),
      "tests.simulatorA");

  BOOST_CHECK_EQUAL(static_cast<int>(
                      dynamic_cast<openfluid::fluidx::GeneratorDescriptor*>(*(++it))->getGeneratorMethod()), 
                    static_cast<int>(openfluid::fluidx::GeneratorDescriptor:: GeneratorMethod::FIXED));

  BOOST_CHECK_EQUAL(static_cast<int>(
                      dynamic_cast<openfluid::fluidx::GeneratorDescriptor*>(*(++it))->getGeneratorMethod()), 
                    static_cast<int>(openfluid::fluidx::GeneratorDescriptor:: GeneratorMethod::RANDOM));

  BOOST_CHECK_EQUAL(
      (dynamic_cast<openfluid::fluidx::SimulatorDescriptor*>(*(++it)))->getID(),
      "tests.simulatorB");

  BOOST_CHECK_EQUAL(
      dynamic_cast<openfluid::fluidx::GeneratorDescriptor*>(&Model.itemAt(0))->getID(),
      "tests.generator.interp.TU.genscalar");
  BOOST_CHECK_EQUAL(
      dynamic_cast<openfluid::fluidx::SimulatorDescriptor*>(&Model.itemAt(4))->getID(),
      "tests.simulatorB");
  BOOST_CHECK_THROW(Model.itemAt(5), openfluid::base::FrameworkException);

  BOOST_CHECK_EQUAL(
      Model.findFirstItem("tests.generator.interp.TU.genscalar"), 0);
  BOOST_CHECK_EQUAL(Model.findFirstItem("tests.simulatorB"), 4);
  BOOST_CHECK_EQUAL(Model.findFirstItem("tests.wrongsimulator"), -1);

  std::vector<std::string> IDs = Model.getOrderedIDs();

  BOOST_CHECK_EQUAL(IDs.size(), 5);
  BOOST_CHECK_EQUAL(IDs.at(0), "tests.generator.interp.TU.genscalar");
  BOOST_CHECK_EQUAL(IDs.at(4), "tests.simulatorB");
}


// =====================================================================
// =====================================================================


BOOST_AUTO_TEST_CASE(check_advanced_operations)
{
  std::unique_ptr<openfluid::base::IOListener> Listener = std::make_unique<openfluid::base::IOListener>();
  openfluid::fluidx::FluidXIO FXIO(Listener.get());

  auto FXDesc = FXIO.loadFromDirectory(CONFIGTESTS_INPUT_DATASETS_DIR + "/OPENFLUID.IN.FluidXIO/singlefile0");

  openfluid::fluidx::CoupledModelDescriptor Model = FXDesc.model();

  BOOST_CHECK_EQUAL(Model.getItemsCount(), 5);

  const openfluid::fluidx::CoupledModelDescriptor::SetDescription_t* Items = &(Model.items());

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

  BOOST_CHECK_EQUAL(static_cast<int>(
                      dynamic_cast<openfluid::fluidx::GeneratorDescriptor*>(*it)->getGeneratorMethod()), 
                    static_cast<int>(openfluid::fluidx::GeneratorDescriptor:: GeneratorMethod::INTERP));

  BOOST_CHECK_EQUAL(
      (dynamic_cast<openfluid::fluidx::SimulatorDescriptor*>(*(++it)))->getID(),
      "tests.simulatorA");

  BOOST_CHECK_EQUAL(static_cast<int>(
                      dynamic_cast<openfluid::fluidx::GeneratorDescriptor*>(*(++it))->getGeneratorMethod()), 
                    static_cast<int>(openfluid::fluidx::GeneratorDescriptor:: GeneratorMethod::FIXED));

  BOOST_CHECK_EQUAL(
      (dynamic_cast<openfluid::fluidx::SimulatorDescriptor*>(*(++it)))->getID(),
      "inserted.item");

  BOOST_CHECK_EQUAL(static_cast<int>(
                      dynamic_cast<openfluid::fluidx::GeneratorDescriptor*>(*(++it))->getGeneratorMethod()), 
                    static_cast<int>(openfluid::fluidx::GeneratorDescriptor:: GeneratorMethod::RANDOM));

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

  BOOST_CHECK_EQUAL(static_cast<int>(dynamic_cast<openfluid::fluidx::GeneratorDescriptor*>(*it)->getGeneratorMethod()), 
                    static_cast<int>(openfluid::fluidx::GeneratorDescriptor:: GeneratorMethod::INTERP));

  BOOST_CHECK_EQUAL(
      (dynamic_cast<openfluid::fluidx::SimulatorDescriptor*>(*(++it)))->getID(),
      "tests.simulatorA");

  BOOST_CHECK_EQUAL(
      (dynamic_cast<openfluid::fluidx::SimulatorDescriptor*>(*(++it)))->getID(),
      "tests.simulatorB");

  BOOST_CHECK_EQUAL(static_cast<int>(
                      dynamic_cast<openfluid::fluidx::GeneratorDescriptor*>(*(++it))->getGeneratorMethod()), 
                    static_cast<int>(openfluid::fluidx::GeneratorDescriptor:: GeneratorMethod::FIXED));

  BOOST_CHECK_EQUAL(
      (dynamic_cast<openfluid::fluidx::SimulatorDescriptor*>(*(++it)))->getID(),
      "inserted.item");

  BOOST_CHECK_EQUAL(static_cast<int>(
                      dynamic_cast<openfluid::fluidx::GeneratorDescriptor*>(*(++it))->getGeneratorMethod()), 
                    static_cast<int>(openfluid::fluidx::GeneratorDescriptor:: GeneratorMethod::RANDOM));

  BOOST_CHECK_EQUAL(
      (dynamic_cast<openfluid::fluidx::SimulatorDescriptor*>(*(++it)))->getID(),
      "appended.item");

  Model.moveItem(3, 0);

  it = Items->begin();

  BOOST_CHECK_EQUAL(static_cast<int>(dynamic_cast<openfluid::fluidx::GeneratorDescriptor*>(*it)->getGeneratorMethod()), 
                    static_cast<int>(openfluid::fluidx::GeneratorDescriptor:: GeneratorMethod::FIXED));

  BOOST_CHECK_EQUAL(static_cast<int>(
                      dynamic_cast<openfluid::fluidx::GeneratorDescriptor*>(*(++it))->getGeneratorMethod()), 
                    static_cast<int>(openfluid::fluidx::GeneratorDescriptor:: GeneratorMethod::INTERP));

  BOOST_CHECK_EQUAL(
      (dynamic_cast<openfluid::fluidx::SimulatorDescriptor*>(*(++it)))->getID(),
      "tests.simulatorA");

  BOOST_CHECK_EQUAL(
      (dynamic_cast<openfluid::fluidx::SimulatorDescriptor*>(*(++it)))->getID(),
      "tests.simulatorB");

  BOOST_CHECK_EQUAL(
      (dynamic_cast<openfluid::fluidx::SimulatorDescriptor*>(*(++it)))->getID(),
      "inserted.item");

  BOOST_CHECK_EQUAL(static_cast<int>(
                      dynamic_cast<openfluid::fluidx::GeneratorDescriptor*>(*(++it))->getGeneratorMethod()), 
                    static_cast<int>(openfluid::fluidx::GeneratorDescriptor:: GeneratorMethod::RANDOM));

  BOOST_CHECK_EQUAL(
      (dynamic_cast<openfluid::fluidx::SimulatorDescriptor*>(*(++it)))->getID(),
      "appended.item");

  Model.moveItem(5, 6);

  it = Items->begin();

  BOOST_CHECK_EQUAL(static_cast<int>(dynamic_cast<openfluid::fluidx::GeneratorDescriptor*>(*it)->getGeneratorMethod()), 
                    static_cast<int>(openfluid::fluidx::GeneratorDescriptor:: GeneratorMethod::FIXED));

  BOOST_CHECK_EQUAL(static_cast<int>(
                      dynamic_cast<openfluid::fluidx::GeneratorDescriptor*>(*(++it))->getGeneratorMethod()), 
                    static_cast<int>(openfluid::fluidx::GeneratorDescriptor:: GeneratorMethod::INTERP));

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

  BOOST_CHECK_EQUAL(static_cast<int>(
                      dynamic_cast<openfluid::fluidx::GeneratorDescriptor*>(*(++it))->getGeneratorMethod()), 
                    static_cast<int>(openfluid::fluidx::GeneratorDescriptor:: GeneratorMethod::RANDOM));

  //removeItem

  BOOST_CHECK_THROW(Model.removeItem(7), openfluid::base::FrameworkException);

  Model.removeItem(4);

  it = Items->begin();

  BOOST_CHECK_EQUAL(static_cast<int>(dynamic_cast<openfluid::fluidx::GeneratorDescriptor*>(*it)->getGeneratorMethod()), 
                    static_cast<int>(openfluid::fluidx::GeneratorDescriptor:: GeneratorMethod::FIXED));

  BOOST_CHECK_EQUAL(static_cast<int>(
                      dynamic_cast<openfluid::fluidx::GeneratorDescriptor*>(*(++it))->getGeneratorMethod()), 
                    static_cast<int>(openfluid::fluidx::GeneratorDescriptor:: GeneratorMethod::INTERP));

  BOOST_CHECK_EQUAL(
      (dynamic_cast<openfluid::fluidx::SimulatorDescriptor*>(*(++it)))->getID(),
      "tests.simulatorA");

  BOOST_CHECK_EQUAL(
      (dynamic_cast<openfluid::fluidx::SimulatorDescriptor*>(*(++it)))->getID(),
      "tests.simulatorB");

  BOOST_CHECK_EQUAL(
      (dynamic_cast<openfluid::fluidx::SimulatorDescriptor*>(*(++it)))->getID(),
      "appended.item");

  BOOST_CHECK_EQUAL(static_cast<int>(
                      dynamic_cast<openfluid::fluidx::GeneratorDescriptor*>(*(++it))->getGeneratorMethod()), 
                    static_cast<int>(openfluid::fluidx::GeneratorDescriptor:: GeneratorMethod::RANDOM));
}

