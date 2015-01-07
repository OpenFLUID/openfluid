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

  @author Jean-Christophe FABRE <jean-christophe.fabre@supagro.inra.fr>
*/


#define BOOST_TEST_MAIN
#define BOOST_AUTO_TEST_MAIN
#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MODULE unittest_engine
#include <boost/test/unit_test.hpp>
#include <boost/test/auto_unit_test.hpp>
#include <boost/foreach.hpp>
#include <tests-config.hpp>

#include <openfluid/ware/PluggableSimulator.hpp>
#include <openfluid/machine/ModelInstance.hpp>
#include <openfluid/machine/ModelItemInstance.hpp>
#include <openfluid/machine/MonitoringInstance.hpp>
#include <openfluid/machine/Engine.hpp>
#include <openfluid/machine/SimulationBlob.hpp>
#include <openfluid/machine/MachineListener.hpp>
#include <openfluid/base/RuntimeEnv.hpp>




class EmptySimulator : public openfluid::ware::PluggableSimulator
{
  public:

  void initParams(const openfluid::ware::WareParams_t& /*Params*/) { };

  void prepareData() { };

  void checkConsistency() { };

  openfluid::base::SchedulingRequest initializeRun()  { return DefaultDeltaT(); };

  openfluid::base::SchedulingRequest runStep() {  return 1; };

  void finalizeRun() { };
};


// =====================================================================
// =====================================================================


void displayModel(openfluid::machine::ModelInstance& MI)
{
  std::cout << " ---- " << std::endl;
  BOOST_FOREACH(openfluid::machine::ModelItemInstance* MII,MI.items())
  {
    std::cout << MII->Signature->ID << std::endl;
  }
  std::cout << " ---- " << std::endl;
}


// =====================================================================
// =====================================================================


BOOST_AUTO_TEST_CASE(check_construction)
{
  openfluid::base::RuntimeEnvironment::instance()->setOutputDir(CONFIGTESTS_OUTPUT_DATA_DIR+"/OPENFLUID.OUT.Engine");

  openfluid::machine::SimulationBlob SBlob;
  openfluid::machine::MachineListener* MachineListen = new openfluid::machine::MachineListener();
  openfluid::machine::ModelInstance Model(SBlob,MachineListen);
  openfluid::machine::MonitoringInstance Monitoring(SBlob);


  openfluid::machine::Engine Eng(SBlob,Model,Monitoring,MachineListen);

  BOOST_CHECK_THROW(openfluid::ware::SignatureHandledTypedDataItem("var1[toto]","UA","",""),openfluid::base::FrameworkException);
  BOOST_CHECK_THROW(openfluid::ware::SignatureHandledTypedDataItem("var1(double)","UA","",""),openfluid::base::FrameworkException);
  BOOST_CHECK_THROW(openfluid::ware::SignatureHandledTypedDataItem("var1[double","UA","",""),openfluid::base::FrameworkException);

  delete MachineListen;
}

// =====================================================================
// =====================================================================

BOOST_AUTO_TEST_CASE(check_pretests)
{
  openfluid::base::RuntimeEnvironment::instance()->setOutputDir(CONFIGTESTS_OUTPUT_DATA_DIR+"/OPENFLUID.OUT.Engine");

  openfluid::machine::ModelItemInstance* MIInstance;

  openfluid::machine::SimulationBlob SBlob;
  openfluid::machine::MachineListener* MachineListen = new openfluid::machine::MachineListener();
  openfluid::machine::ModelInstance Model(SBlob,MachineListen);
  openfluid::machine::MonitoringInstance Monitoring(SBlob);


  SBlob.spatialGraph().addUnit(openfluid::core::SpatialUnit("UA",1,1));
  SBlob.spatialGraph().addUnit(openfluid::core::SpatialUnit("UB",1,1));

  openfluid::machine::Engine Eng(SBlob,Model,Monitoring,MachineListen);

  displayModel(Model);

  // =====================================================================

  MIInstance = new openfluid::machine::ModelItemInstance();
  MIInstance->Body = new EmptySimulator();
  MIInstance->Verified = true;
  MIInstance->Signature = new openfluid::ware::SimulatorSignature();
  MIInstance->Signature->ID = "MySim1";
  MIInstance->Signature->HandledData.RequiredVars.push_back(openfluid::ware::SignatureHandledTypedDataItem("var1","UA","",""));
  MIInstance->Signature->HandledData.UpdatedVars.push_back(openfluid::ware::SignatureHandledTypedDataItem("var5[]","UB","",""));



  Model.resetInitialized();
  Model.appendItem(MIInstance);

  displayModel(Model);

  // =====================================================================


  MIInstance = new openfluid::machine::ModelItemInstance();
  MIInstance->Body = new EmptySimulator();
  MIInstance->Verified = true;
  MIInstance->Signature = new openfluid::ware::SimulatorSignature();
  MIInstance->Signature->ID = "MySim0";
  MIInstance->Signature->HandledData.ProducedVars.push_back(openfluid::ware::SignatureHandledTypedDataItem("var1","UA","",""));

  Model.resetInitialized();
  Model.insertItem(MIInstance,0);

  displayModel(Model);

  // =====================================================================

  MIInstance = new openfluid::machine::ModelItemInstance();
  MIInstance->Body = new EmptySimulator();
  MIInstance->Verified = true;
  MIInstance->Signature = new openfluid::ware::SimulatorSignature();
  MIInstance->Signature->ID = "MySim2";
  MIInstance->Signature->HandledData.RequiredVars.push_back(openfluid::ware::SignatureHandledTypedDataItem("var5[]","UB","",""));

  Model.resetInitialized();
  Model.appendItem(MIInstance);

  displayModel(Model);


  // =====================================================================

  MIInstance = new openfluid::machine::ModelItemInstance();
  MIInstance->Body = new EmptySimulator();
  MIInstance->Verified = true;
  MIInstance->Signature = new openfluid::ware::SimulatorSignature();
  MIInstance->Signature->ID = "MySim0.5";
  MIInstance->Signature->HandledData.RequiredVars.push_back(openfluid::ware::SignatureHandledTypedDataItem("var1","UA","",""));

  Model.resetInitialized();
  Model.insertItem(MIInstance,1);

  displayModel(Model);


  // =====================================================================

  MIInstance = new openfluid::machine::ModelItemInstance();
  MIInstance->Body = new EmptySimulator();
  MIInstance->Verified = true;
  MIInstance->Signature = new openfluid::ware::SimulatorSignature();
  MIInstance->Signature->ID = "MySim3";

  Model.resetInitialized();
  Model.appendItem(MIInstance);

  displayModel(Model);

  // =====================================================================

  MIInstance = new openfluid::machine::ModelItemInstance();
  MIInstance->Body = new EmptySimulator();
  MIInstance->Verified = true;
  MIInstance->Signature = new openfluid::ware::SimulatorSignature();
  MIInstance->Signature->ID = "MySim4";
  MIInstance->Signature->HandledData.RequiredVars.push_back(openfluid::ware::SignatureHandledTypedDataItem("var1","UA","",""));
  MIInstance->Signature->HandledData.ProducedVars.push_back(openfluid::ware::SignatureHandledTypedDataItem("var7","UC","",""));


  Model.resetInitialized();
  Model.appendItem(MIInstance);

  displayModel(Model);


  delete MachineListen;
}

// =====================================================================
// =====================================================================

BOOST_AUTO_TEST_CASE(check_typed_pretests)
{
  openfluid::base::RuntimeEnvironment::instance()->setOutputDir(CONFIGTESTS_OUTPUT_DATA_DIR+"/OPENFLUID.OUT.Engine");

  openfluid::machine::ModelItemInstance* MIInstance;

  openfluid::machine::SimulationBlob SBlob;
  openfluid::machine::MachineListener* MachineListen = new openfluid::machine::MachineListener();
  openfluid::machine::ModelInstance Model(SBlob,MachineListen);
  openfluid::machine::MonitoringInstance Monitoring(SBlob);


  SBlob.spatialGraph().addUnit(openfluid::core::SpatialUnit("UA",1,1));
  SBlob.spatialGraph().addUnit(openfluid::core::SpatialUnit("UB",1,1));

  openfluid::machine::Engine Eng(SBlob,Model,Monitoring,MachineListen);


  displayModel(Model);

  // =====================================================================

  MIInstance = new openfluid::machine::ModelItemInstance();
  MIInstance->Body = new EmptySimulator();
  MIInstance->Verified = true;
  MIInstance->Signature = new openfluid::ware::SimulatorSignature();
  MIInstance->Signature->ID = "MySim1";
  MIInstance->Signature->HandledData.RequiredVars.push_back(openfluid::ware::SignatureHandledTypedDataItem("var1[double]","UA","",""));
  MIInstance->Signature->HandledData.UpdatedVars.push_back(openfluid::ware::SignatureHandledTypedDataItem("var5[vector]","UB","",""));



  Model.resetInitialized();
  Model.appendItem(MIInstance);

  displayModel(Model);

  // =====================================================================


  MIInstance = new openfluid::machine::ModelItemInstance();
  MIInstance->Body = new EmptySimulator();
  MIInstance->Verified = true;
  MIInstance->Signature = new openfluid::ware::SimulatorSignature();
  MIInstance->Signature->ID = "MySim0";
  MIInstance->Signature->HandledData.ProducedVars.push_back(openfluid::ware::SignatureHandledTypedDataItem("var1[double]","UA","",""));

  Model.resetInitialized();
  Model.insertItem(MIInstance,0);

  displayModel(Model);

  // =====================================================================

  MIInstance = new openfluid::machine::ModelItemInstance();
  MIInstance->Body = new EmptySimulator();
  MIInstance->Verified = true;
  MIInstance->Signature = new openfluid::ware::SimulatorSignature();
  MIInstance->Signature->ID = "MySim2";
  MIInstance->Signature->HandledData.RequiredVars.push_back(openfluid::ware::SignatureHandledTypedDataItem("var5[vector]","UB","",""));

  Model.resetInitialized();
  Model.appendItem(MIInstance);

  displayModel(Model);


  // =====================================================================

  MIInstance = new openfluid::machine::ModelItemInstance();
  MIInstance->Body = new EmptySimulator();
  MIInstance->Verified = true;
  MIInstance->Signature = new openfluid::ware::SimulatorSignature();
  MIInstance->Signature->ID = "MySim0.5";
  MIInstance->Signature->HandledData.RequiredVars.push_back(openfluid::ware::SignatureHandledTypedDataItem("var1","UA","",""));

  Model.resetInitialized();
  Model.insertItem(MIInstance,1);

  displayModel(Model);


  // =====================================================================

  MIInstance = new openfluid::machine::ModelItemInstance();
  MIInstance->Body = new EmptySimulator();
  MIInstance->Verified = true;
  MIInstance->Signature = new openfluid::ware::SimulatorSignature();
  MIInstance->Signature->ID = "MySim3";

  Model.resetInitialized();
  Model.appendItem(MIInstance);

  displayModel(Model);

  // =====================================================================

  MIInstance = new openfluid::machine::ModelItemInstance();
  MIInstance->Body = new EmptySimulator();
  MIInstance->Verified = true;
  MIInstance->Signature = new openfluid::ware::SimulatorSignature();
  MIInstance->Signature->ID = "MySim4";
  MIInstance->Signature->HandledData.RequiredVars.push_back(openfluid::ware::SignatureHandledTypedDataItem("var1","UA","",""));
  MIInstance->Signature->HandledData.ProducedVars.push_back(openfluid::ware::SignatureHandledTypedDataItem("var7","UC","",""));


  Model.resetInitialized();
  Model.appendItem(MIInstance);

  displayModel(Model);



  delete MachineListen;
}

// =====================================================================
// =====================================================================
