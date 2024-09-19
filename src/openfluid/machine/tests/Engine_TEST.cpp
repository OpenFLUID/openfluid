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
  @file Engine_TEST.cpp

  @author Jean-Christophe FABRE <jean-christophe.fabre@inra.fr>
*/


#define BOOST_TEST_MAIN
#define BOOST_AUTO_TEST_MAIN
#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MODULE unittest_engine


#include <boost/test/unit_test.hpp>

#include <openfluid/ware/PluggableSimulator.hpp>
#include <openfluid/machine/ModelInstance.hpp>
#include <openfluid/machine/ModelItemInstance.hpp>
#include <openfluid/machine/MonitoringInstance.hpp>
#include <openfluid/machine/Engine.hpp>
#include <openfluid/machine/SimulationBlob.hpp>
#include <openfluid/machine/MachineListener.hpp>
#include <openfluid/base/RunContextManager.hpp>

#include "tests-config.hpp"


class EmptySimulator : public openfluid::ware::PluggableSimulator
{
  public:

  void initParams(const openfluid::ware::WareParams_t& /*Params*/)
  { };

  void prepareData()
  { };

  void checkConsistency() { };

  openfluid::base::SchedulingRequest initializeRun()
  { return DefaultDeltaT(); };

  openfluid::base::SchedulingRequest runStep()
  {  return 1; };

  void finalizeRun()
  { };
};


// =====================================================================
// =====================================================================


void displayModel(openfluid::machine::ModelInstance& MI)
{
  std::cout << " ---- start" << std::endl;
  if (MI.items().empty())
  {
    std::cout << "(empty model)" << std::endl;
  }
  else
  {
    for (openfluid::machine::ModelItemInstance* MII : MI.items())
    {
      std::cout << MII->Container.signature()->ID << std::endl;
    }
  }
  std::cout << " ---- end" << std::endl;
}


// =====================================================================
// =====================================================================


BOOST_AUTO_TEST_CASE(check_construction)
{
  openfluid::base::RunContextManager::instance()->setOutputDir(CONFIGTESTS_OUTPUT_DATA_DIR+"/OPENFLUID.OUT.Engine");

  openfluid::machine::SimulationBlob SBlob;
  openfluid::machine::MachineListener* MachineListen = new openfluid::machine::MachineListener();
  openfluid::machine::ModelInstance Model(SBlob,MachineListen);
  openfluid::machine::MonitoringInstance Monitoring(SBlob);


  openfluid::machine::Engine Eng(SBlob,Model,Monitoring,MachineListen);

  BOOST_CHECK_THROW(openfluid::ware::SignatureSpatialDataItem("var1[toto]","UA","",""),
                    openfluid::base::FrameworkException);
  BOOST_CHECK_THROW(openfluid::ware::SignatureSpatialDataItem("var1(double)","UA","",""),
                    openfluid::base::FrameworkException);
  BOOST_CHECK_THROW(openfluid::ware::SignatureSpatialDataItem("var1[double","UA","",""),
                    openfluid::base::FrameworkException);

  delete MachineListen;
}


// =====================================================================
// =====================================================================


BOOST_AUTO_TEST_CASE(check_pretests)
{
  openfluid::base::RunContextManager::instance()->setOutputDir(CONFIGTESTS_OUTPUT_DATA_DIR+"/OPENFLUID.OUT.Engine");

  openfluid::machine::ModelItemInstance* MIInstance;

  openfluid::machine::SimulationBlob SBlob;
  openfluid::machine::MachineListener* MachineListen = new openfluid::machine::MachineListener();
  openfluid::machine::ModelInstance Model(SBlob,MachineListen);
  openfluid::machine::MonitoringInstance Monitoring(SBlob);


  SBlob.spatialGraph().addUnit(openfluid::core::SpatialUnit("UA",1,1));
  SBlob.spatialGraph().addUnit(openfluid::core::SpatialUnit("UB",1,1));

  openfluid::machine::Engine Eng(SBlob,Model,Monitoring,MachineListen);

  displayModel(Model);
  BOOST_CHECK(Model.items().empty());
  BOOST_CHECK_EQUAL(Model.items().size(),0);

  // ---------------------------------------------------------------------

  openfluid::machine::WareContainer<openfluid::ware::SimulatorSignature> Cont1(openfluid::ware::WareType::SIMULATOR);
  auto Sign1 = new openfluid::ware::SimulatorSignature();
  Sign1->ID = "MySim1";
  Sign1->SimulatorHandledData.RequiredVars.push_back(openfluid::ware::SignatureSpatialDataItem("var1","UA","",""));
  Sign1->SimulatorHandledData.UpdatedVars.push_back(openfluid::ware::SignatureSpatialDataItem("var5[]","UB","",""));
  Cont1.setSignature(Sign1);
  Cont1.validate();

  MIInstance = new openfluid::machine::ModelItemInstance(Cont1);
  MIInstance->Body.reset(new EmptySimulator());

  Model.resetInitialized();
  Model.appendItem(MIInstance);

  displayModel(Model);
  BOOST_CHECK(!Model.items().empty());
  BOOST_CHECK_EQUAL(Model.items().size(),1);

  // ---------------------------------------------------------------------

  openfluid::machine::WareContainer<openfluid::ware::SimulatorSignature> Cont2(openfluid::ware::WareType::SIMULATOR);
  auto Sign2 = new openfluid::ware::SimulatorSignature();
  Sign2->ID = "MySim0";
  Sign2->SimulatorHandledData.ProducedVars.push_back(openfluid::ware::SignatureSpatialDataItem("var1","UA","",""));
  Cont2.setSignature(Sign2);
  Cont2.validate();

  MIInstance = new openfluid::machine::ModelItemInstance(Cont2);
  MIInstance->Body.reset(new EmptySimulator());

  Model.resetInitialized();
  Model.insertItem(MIInstance,0);

  displayModel(Model);
  BOOST_CHECK(!Model.items().empty());
  BOOST_CHECK_EQUAL(Model.items().size(),2);

  // ---------------------------------------------------------------------

  openfluid::machine::WareContainer<openfluid::ware::SimulatorSignature> Cont3(openfluid::ware::WareType::SIMULATOR);
  auto Sign3 = new openfluid::ware::SimulatorSignature();
  Sign3->ID = "MySim2";
  Sign3->SimulatorHandledData.RequiredVars.push_back(openfluid::ware::SignatureSpatialDataItem("var5[]","UB","",""));
  Cont3.setSignature(Sign3);
  Cont3.validate();

  MIInstance = new openfluid::machine::ModelItemInstance(Cont3);
  MIInstance->Body.reset(new EmptySimulator());

  Model.resetInitialized();
  Model.appendItem(MIInstance);

  displayModel(Model);
  BOOST_CHECK(!Model.items().empty());
  BOOST_CHECK_EQUAL(Model.items().size(),3);

  // ---------------------------------------------------------------------

  openfluid::machine::WareContainer<openfluid::ware::SimulatorSignature> Cont4(openfluid::ware::WareType::SIMULATOR);
  auto Sign4 = new openfluid::ware::SimulatorSignature();
  Sign4->ID = "MySim0.5";
  Sign4->SimulatorHandledData.RequiredVars.push_back(openfluid::ware::SignatureSpatialDataItem("var1","UA","",""));
  Cont4.setSignature(Sign4);
  Cont4.validate();

  MIInstance = new openfluid::machine::ModelItemInstance(Cont4);
  MIInstance->Body.reset(new EmptySimulator());

  Model.resetInitialized();
  Model.insertItem(MIInstance,1);

  displayModel(Model);
  BOOST_CHECK(!Model.items().empty());
  BOOST_CHECK_EQUAL(Model.items().size(),4);

  // ---------------------------------------------------------------------

  openfluid::machine::WareContainer<openfluid::ware::SimulatorSignature> Cont5(openfluid::ware::WareType::SIMULATOR);
  auto Sign5 = new openfluid::ware::SimulatorSignature();
  Sign5->ID = "MySim3";
  Cont5.setSignature(Sign5);
  Cont5.validate();

  MIInstance = new openfluid::machine::ModelItemInstance(Cont5);
  MIInstance->Body.reset(new EmptySimulator());

  Model.resetInitialized();
  Model.appendItem(MIInstance);

  displayModel(Model);
  BOOST_CHECK(!Model.items().empty());
  BOOST_CHECK_EQUAL(Model.items().size(),5);

  // ---------------------------------------------------------------------

  openfluid::machine::WareContainer<openfluid::ware::SimulatorSignature> Cont6(openfluid::ware::WareType::SIMULATOR);
  auto Sign6 = new openfluid::ware::SimulatorSignature();
  Sign6->ID = "MySim4";
  Sign6->SimulatorHandledData.RequiredVars.push_back(openfluid::ware::SignatureSpatialDataItem("var1","UA","",""));
  Sign6->SimulatorHandledData.ProducedVars.push_back(openfluid::ware::SignatureSpatialDataItem("var7","UC","",""));
  Cont6.setSignature(Sign6);
  Cont6.validate();

  MIInstance = new openfluid::machine::ModelItemInstance(Cont6);
  MIInstance->Body.reset(new EmptySimulator());

  Model.resetInitialized();
  Model.appendItem(MIInstance);

  displayModel(Model);
  BOOST_CHECK(!Model.items().empty());
  BOOST_CHECK_EQUAL(Model.items().size(),6);


  delete MachineListen;
}


// =====================================================================
// =====================================================================


BOOST_AUTO_TEST_CASE(check_typed_pretests)
{
  openfluid::base::RunContextManager::instance()->setOutputDir(CONFIGTESTS_OUTPUT_DATA_DIR+"/OPENFLUID.OUT.Engine");

  openfluid::machine::ModelItemInstance* MIInstance;

  openfluid::machine::SimulationBlob SBlob;
  openfluid::machine::MachineListener* MachineListen = new openfluid::machine::MachineListener();
  openfluid::machine::ModelInstance Model(SBlob,MachineListen);
  openfluid::machine::MonitoringInstance Monitoring(SBlob);


  SBlob.spatialGraph().addUnit(openfluid::core::SpatialUnit("UA",1,1));
  SBlob.spatialGraph().addUnit(openfluid::core::SpatialUnit("UB",1,1));

  openfluid::machine::Engine Eng(SBlob,Model,Monitoring,MachineListen);


  displayModel(Model);
  BOOST_CHECK(Model.items().empty());
  BOOST_CHECK_EQUAL(Model.items().size(),0);

  // ---------------------------------------------------------------------

  openfluid::machine::WareContainer<openfluid::ware::SimulatorSignature> Cont1(openfluid::ware::WareType::SIMULATOR);
  auto Sign1 = new openfluid::ware::SimulatorSignature();
  Sign1->ID = "MySim1";
  Sign1->SimulatorHandledData.RequiredVars.push_back(openfluid::ware::SignatureSpatialDataItem("var1[double]","UA","",""));
  Sign1->SimulatorHandledData.UpdatedVars.push_back(openfluid::ware::SignatureSpatialDataItem("var5[vector]","UB","",""));
  Cont1.setSignature(Sign1);
  Cont1.validate();

  MIInstance = new openfluid::machine::ModelItemInstance(Cont1);
  MIInstance->Body.reset(new EmptySimulator());

  Model.resetInitialized();
  Model.appendItem(MIInstance);

  displayModel(Model);
  BOOST_CHECK(!Model.items().empty());
  BOOST_CHECK_EQUAL(Model.items().size(),1);

  // ---------------------------------------------------------------------

  openfluid::machine::WareContainer<openfluid::ware::SimulatorSignature> Cont2(openfluid::ware::WareType::SIMULATOR);
  auto Sign2 = new openfluid::ware::SimulatorSignature();
  Sign2->ID = "MySim0";
  Sign2->SimulatorHandledData.ProducedVars.push_back(openfluid::ware::SignatureSpatialDataItem("var1[double]","UA","",""));
  Cont2.setSignature(Sign2);
  Cont2.validate();

  MIInstance = new openfluid::machine::ModelItemInstance(Cont2);
  MIInstance->Body.reset(new EmptySimulator());

  Model.resetInitialized();
  Model.insertItem(MIInstance,0);

  displayModel(Model);
  BOOST_CHECK(!Model.items().empty());
  BOOST_CHECK_EQUAL(Model.items().size(),2);

  // ---------------------------------------------------------------------

  openfluid::machine::WareContainer<openfluid::ware::SimulatorSignature> Cont3(openfluid::ware::WareType::SIMULATOR);
  auto Sign3 = new openfluid::ware::SimulatorSignature();
  Sign3->ID = "MySim2";
  Sign3->SimulatorHandledData.RequiredVars.push_back(openfluid::ware::SignatureSpatialDataItem("var5[vector]","UB","",""));
  Cont3.setSignature(Sign3);
  Cont3.validate();

  MIInstance = new openfluid::machine::ModelItemInstance(Cont3);
  MIInstance->Body.reset(new EmptySimulator());

  Model.resetInitialized();
  Model.appendItem(MIInstance);

  displayModel(Model);
  BOOST_CHECK(!Model.items().empty());
  BOOST_CHECK_EQUAL(Model.items().size(),3);

  // ---------------------------------------------------------------------

  openfluid::machine::WareContainer<openfluid::ware::SimulatorSignature> Cont4(openfluid::ware::WareType::SIMULATOR);
  auto Sign4 = new openfluid::ware::SimulatorSignature();
  Sign4->ID = "MySim0.5";
  Sign4->SimulatorHandledData.RequiredVars.push_back(openfluid::ware::SignatureSpatialDataItem("var1","UA","",""));
  Cont4.setSignature(Sign4);
  Cont4.validate();

  MIInstance = new openfluid::machine::ModelItemInstance(Cont4);
  MIInstance->Body.reset(new EmptySimulator());

  Model.resetInitialized();
  Model.insertItem(MIInstance,1);

  displayModel(Model);
  BOOST_CHECK(!Model.items().empty());
  BOOST_CHECK_EQUAL(Model.items().size(),4);

  // ---------------------------------------------------------------------

  openfluid::machine::WareContainer<openfluid::ware::SimulatorSignature> Cont5(openfluid::ware::WareType::SIMULATOR);
  auto Sign5 = new openfluid::ware::SimulatorSignature();
  Sign5->ID = "MySim3";
  Cont5.setSignature(Sign5);
  Cont5.validate();

  MIInstance = new openfluid::machine::ModelItemInstance(Cont5);
  MIInstance->Body.reset(new EmptySimulator());

  Model.resetInitialized();
  Model.appendItem(MIInstance);

  displayModel(Model);
  BOOST_CHECK(!Model.items().empty());
  BOOST_CHECK_EQUAL(Model.items().size(),5);

  // ---------------------------------------------------------------------

  openfluid::machine::WareContainer<openfluid::ware::SimulatorSignature> Cont6(openfluid::ware::WareType::SIMULATOR);
  auto Sign6 = new openfluid::ware::SimulatorSignature();
  Sign6->ID = "MySim0.5";
  Sign6->SimulatorHandledData.RequiredVars.push_back(openfluid::ware::SignatureSpatialDataItem("var1","UA","",""));
  Sign6->SimulatorHandledData.ProducedVars.push_back(openfluid::ware::SignatureSpatialDataItem("var7","UC","",""));
  Cont6.setSignature(Sign6);
  Cont6.validate();

  MIInstance = new openfluid::machine::ModelItemInstance(Cont6);
  MIInstance->Body.reset(new EmptySimulator());

  Model.resetInitialized();
  Model.appendItem(MIInstance);

  displayModel(Model);
  BOOST_CHECK(!Model.items().empty());
  BOOST_CHECK_EQUAL(Model.items().size(),6);


  delete MachineListen;
}

