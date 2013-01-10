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
  \file Engine_TEST.cpp
  \brief Implements ...

  \author Jean-Christophe FABRE <fabrejc@supagro.inra.fr>
*/


#define BOOST_TEST_MAIN
#define BOOST_AUTO_TEST_MAIN
#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MODULE unittest_engine
#include <boost/test/unit_test.hpp>
#include <boost/test/auto_unit_test.hpp>
#include <boost/foreach.hpp>
#include <tests-config.hpp>

#include <openfluid/ware/PluggableFunction.hpp>
#include <openfluid/machine/ModelInstance.hpp>
#include <openfluid/machine/ModelItemInstance.hpp>
#include <openfluid/machine/ObserversListInstance.hpp>
#include <openfluid/machine/Engine.hpp>
#include <openfluid/machine/SimulationBlob.hpp>
#include <openfluid/machine/MachineListener.hpp>
#include <openfluid/io/IOListener.hpp>


class EmptyFunction : public openfluid::ware::PluggableFunction
{
  public:

  void initParams(const openfluid::ware::WareParams_t& /*Params*/) { };

  void prepareData() { };

  void checkConsistency() { };

  openfluid::core::Duration_t initializeRun()  { return DefaultDeltaT(); };

  openfluid::core::Duration_t runStep() {  return 1; };

  void finalizeRun() { };
};


// =====================================================================
// =====================================================================


void displayModel(openfluid::machine::ModelInstance& MI)
{
  std::cout << " ---- " << std::endl;
  BOOST_FOREACH(openfluid::machine::ModelItemInstance* MII,MI.getItems())
  {
    std::cout << MII->Signature->ID << std::endl;
  }
  std::cout << " ---- " << std::endl;
}


// =====================================================================
// =====================================================================


void displayPretestsMsgs(const openfluid::machine::Engine::PretestInfos_t& PInfos)
{
  if (!PInfos.ExtraFilesMsg.empty()) std::cout << PInfos.ExtraFilesMsg << std::endl;
  if (!PInfos.InputdataMsg.empty()) std::cout << PInfos.InputdataMsg << std::endl;
  if (!PInfos.ModelMsg.empty()) std::cout << PInfos.ModelMsg << std::endl;
}


// =====================================================================
// =====================================================================


BOOST_AUTO_TEST_CASE(check_construction)
{
  openfluid::machine::SimulationBlob SBlob;
  openfluid::machine::MachineListener* MachineListen = new openfluid::machine::MachineListener();
  openfluid::io::IOListener* IOListen = new openfluid::io::IOListener();
  openfluid::machine::ModelInstance Model(SBlob,MachineListen);
  openfluid::machine::ObserversListInstance ObsList(SBlob);


  openfluid::machine::Engine Eng(SBlob,Model,ObsList,MachineListen,IOListen);

  BOOST_CHECK_THROW(openfluid::ware::SignatureHandledTypedDataItem("var1[toto]","UA","",""),openfluid::base::OFException);
  BOOST_CHECK_THROW(openfluid::ware::SignatureHandledTypedDataItem("var1(double)","UA","",""),openfluid::base::OFException);
  BOOST_CHECK_THROW(openfluid::ware::SignatureHandledTypedDataItem("var1[double","UA","",""),openfluid::base::OFException);

  delete MachineListen;
  delete IOListen;

}

// =====================================================================
// =====================================================================

BOOST_AUTO_TEST_CASE(check_pretests)
{
  openfluid::machine::ModelItemInstance* MIInstance;

  openfluid::machine::SimulationBlob SBlob;
  openfluid::machine::MachineListener* MachineListen = new openfluid::machine::MachineListener();
  openfluid::io::IOListener* IOListen = new openfluid::io::IOListener();
  openfluid::machine::ModelInstance Model(SBlob,MachineListen);
  openfluid::machine::ObserversListInstance ObsList(SBlob);


  SBlob.getCoreRepository().addUnit(openfluid::core::Unit("UA",1,1,openfluid::core::Unit::UNKNOWN));
  SBlob.getCoreRepository().addUnit(openfluid::core::Unit("UB",1,1,openfluid::core::Unit::UNKNOWN));

  openfluid::machine::Engine Eng(SBlob,Model,ObsList,MachineListen,IOListen);

  openfluid::machine::Engine::PretestInfos_t PInfos;

  PInfos.ExtraFiles = true;
  PInfos.Inputdata = true;
  PInfos.Model = true;

  displayModel(Model);
  Eng.pretestConsistency(PInfos);
  displayPretestsMsgs(PInfos);
  BOOST_REQUIRE_EQUAL(PInfos.ExtraFiles,true);
  BOOST_REQUIRE_EQUAL(PInfos.Inputdata,true);
  BOOST_REQUIRE_EQUAL(PInfos.Model,true);

  // =====================================================================

  MIInstance = new openfluid::machine::ModelItemInstance();
  MIInstance->Body = new EmptyFunction();
  MIInstance->SDKCompatible = true;
  MIInstance->Signature = new openfluid::ware::FunctionSignature();
  MIInstance->Signature->ID = "MyFunc1";
  MIInstance->Signature->HandledData.RequiredVars.push_back(openfluid::ware::SignatureHandledTypedDataItem("var1","UA","",""));
  MIInstance->Signature->HandledData.UpdatedVars.push_back(openfluid::ware::SignatureHandledTypedDataItem("var5[]","UB","",""));



  Model.resetInitialized();
  Model.appendItem(MIInstance);

  displayModel(Model);
  Eng.pretestConsistency(PInfos);
  displayPretestsMsgs(PInfos);
  BOOST_REQUIRE_EQUAL(PInfos.ExtraFiles,true);
  BOOST_REQUIRE_EQUAL(PInfos.Inputdata,true);
  BOOST_REQUIRE_EQUAL(PInfos.Model,false);
  PInfos = openfluid::machine::Engine::PretestInfos_t();

  // =====================================================================


  MIInstance = new openfluid::machine::ModelItemInstance();
  MIInstance->Body = new EmptyFunction();
  MIInstance->SDKCompatible = true;
  MIInstance->Signature = new openfluid::ware::FunctionSignature();
  MIInstance->Signature->ID = "MyFunc0";
  MIInstance->Signature->HandledData.ProducedVars.push_back(openfluid::ware::SignatureHandledTypedDataItem("var1","UA","",""));

  Model.resetInitialized();
  Model.insertItem(MIInstance,0);

  displayModel(Model);
  Eng.pretestConsistency(PInfos);
  displayPretestsMsgs(PInfos);
  BOOST_REQUIRE_EQUAL(PInfos.ExtraFiles,true);
  BOOST_REQUIRE_EQUAL(PInfos.Inputdata,true);
  BOOST_REQUIRE_EQUAL(PInfos.Model,true);
  PInfos = openfluid::machine::Engine::PretestInfos_t();

  // =====================================================================

  MIInstance = new openfluid::machine::ModelItemInstance();
  MIInstance->Body = new EmptyFunction();
  MIInstance->SDKCompatible = true;
  MIInstance->Signature = new openfluid::ware::FunctionSignature();
  MIInstance->Signature->ID = "MyFunc2";
  MIInstance->Signature->HandledData.RequiredVars.push_back(openfluid::ware::SignatureHandledTypedDataItem("var5[]","UB","",""));

  Model.resetInitialized();
  Model.appendItem(MIInstance);

  displayModel(Model);
  Eng.pretestConsistency(PInfos);
  displayPretestsMsgs(PInfos);
  BOOST_REQUIRE_EQUAL(PInfos.ExtraFiles,true);
  BOOST_REQUIRE_EQUAL(PInfos.Inputdata,true);
  BOOST_REQUIRE_EQUAL(PInfos.Model,true);
  PInfos = openfluid::machine::Engine::PretestInfos_t();


  // =====================================================================

  MIInstance = new openfluid::machine::ModelItemInstance();
  MIInstance->Body = new EmptyFunction();
  MIInstance->SDKCompatible = true;
  MIInstance->Signature = new openfluid::ware::FunctionSignature();
  MIInstance->Signature->ID = "MyFunc0.5";
  MIInstance->Signature->HandledData.RequiredVars.push_back(openfluid::ware::SignatureHandledTypedDataItem("var1","UA","",""));
  MIInstance->Signature->HandledData.RequiredPrevVars.push_back(openfluid::ware::SignatureHandledTypedDataItem("var5[]","UB","",""));

  Model.resetInitialized();
  Model.insertItem(MIInstance,1);

  displayModel(Model);
  Eng.pretestConsistency(PInfos);
  displayPretestsMsgs(PInfos);
  BOOST_REQUIRE_EQUAL(PInfos.ExtraFiles,true);
  BOOST_REQUIRE_EQUAL(PInfos.Inputdata,true);
  BOOST_REQUIRE_EQUAL(PInfos.Model,true);
  PInfos = openfluid::machine::Engine::PretestInfos_t();


  // =====================================================================

  MIInstance = new openfluid::machine::ModelItemInstance();
  MIInstance->Body = new EmptyFunction();
  MIInstance->SDKCompatible = true;
  MIInstance->Signature = new openfluid::ware::FunctionSignature();
  MIInstance->Signature->ID = "MyFunc3";
  MIInstance->Signature->HandledData.RequiredPrevVars.push_back(openfluid::ware::SignatureHandledTypedDataItem("var7","UC","",""));

  Model.resetInitialized();
  Model.appendItem(MIInstance);

  displayModel(Model);
  Eng.pretestConsistency(PInfos);
  displayPretestsMsgs(PInfos);
  BOOST_REQUIRE_EQUAL(PInfos.ExtraFiles,true);
  BOOST_REQUIRE_EQUAL(PInfos.Inputdata,true);
  BOOST_REQUIRE_EQUAL(PInfos.Model,false);
  PInfos = openfluid::machine::Engine::PretestInfos_t();

  // =====================================================================

  MIInstance = new openfluid::machine::ModelItemInstance();
  MIInstance->Body = new EmptyFunction();
  MIInstance->SDKCompatible = true;
  MIInstance->Signature = new openfluid::ware::FunctionSignature();
  MIInstance->Signature->ID = "MyFunc4";
  MIInstance->Signature->HandledData.RequiredVars.push_back(openfluid::ware::SignatureHandledTypedDataItem("var1","UA","",""));
  MIInstance->Signature->HandledData.ProducedVars.push_back(openfluid::ware::SignatureHandledTypedDataItem("var7","UC","",""));


  Model.resetInitialized();
  Model.appendItem(MIInstance);

  displayModel(Model);
  Eng.pretestConsistency(PInfos);
  displayPretestsMsgs(PInfos);
  BOOST_REQUIRE_EQUAL(PInfos.ExtraFiles,true);
  BOOST_REQUIRE_EQUAL(PInfos.Inputdata,true);
  BOOST_REQUIRE_EQUAL(PInfos.Model,true);
  PInfos = openfluid::machine::Engine::PretestInfos_t();



  delete MachineListen;
  delete IOListen;

}

// =====================================================================
// =====================================================================

BOOST_AUTO_TEST_CASE(check_typed_pretests)
{
  openfluid::machine::ModelItemInstance* MIInstance;

  openfluid::machine::SimulationBlob SBlob;
  openfluid::machine::MachineListener* MachineListen = new openfluid::machine::MachineListener();
  openfluid::io::IOListener* IOListen = new openfluid::io::IOListener();
  openfluid::machine::ModelInstance Model(SBlob,MachineListen);
  openfluid::machine::ObserversListInstance ObsList(SBlob);


  SBlob.getCoreRepository().addUnit(openfluid::core::Unit("UA",1,1,openfluid::core::Unit::UNKNOWN));
  SBlob.getCoreRepository().addUnit(openfluid::core::Unit("UB",1,1,openfluid::core::Unit::UNKNOWN));

  openfluid::machine::Engine Eng(SBlob,Model,ObsList,MachineListen,IOListen);

  openfluid::machine::Engine::PretestInfos_t PInfos;

  PInfos.ExtraFiles = true;
  PInfos.Inputdata = true;
  PInfos.Model = true;

  displayModel(Model);
  Eng.pretestConsistency(PInfos);
  displayPretestsMsgs(PInfos);
  BOOST_REQUIRE_EQUAL(PInfos.ExtraFiles,true);
  BOOST_REQUIRE_EQUAL(PInfos.Inputdata,true);
  BOOST_REQUIRE_EQUAL(PInfos.Model,true);

  // =====================================================================

  MIInstance = new openfluid::machine::ModelItemInstance();
  MIInstance->Body = new EmptyFunction();
  MIInstance->SDKCompatible = true;
  MIInstance->Signature = new openfluid::ware::FunctionSignature();
  MIInstance->Signature->ID = "MyFunc1";
  MIInstance->Signature->HandledData.RequiredVars.push_back(openfluid::ware::SignatureHandledTypedDataItem("var1[double]","UA","",""));
  MIInstance->Signature->HandledData.UpdatedVars.push_back(openfluid::ware::SignatureHandledTypedDataItem("var5[vector]","UB","",""));



  Model.resetInitialized();
  Model.appendItem(MIInstance);

  displayModel(Model);
  Eng.pretestConsistency(PInfos);
  displayPretestsMsgs(PInfos);
  BOOST_REQUIRE_EQUAL(PInfos.ExtraFiles,true);
  BOOST_REQUIRE_EQUAL(PInfos.Inputdata,true);
  BOOST_REQUIRE_EQUAL(PInfos.Model,false);
  PInfos = openfluid::machine::Engine::PretestInfos_t();

  // =====================================================================


  MIInstance = new openfluid::machine::ModelItemInstance();
  MIInstance->Body = new EmptyFunction();
  MIInstance->SDKCompatible = true;
  MIInstance->Signature = new openfluid::ware::FunctionSignature();
  MIInstance->Signature->ID = "MyFunc0";
  MIInstance->Signature->HandledData.ProducedVars.push_back(openfluid::ware::SignatureHandledTypedDataItem("var1[double]","UA","",""));

  Model.resetInitialized();
  Model.insertItem(MIInstance,0);

  displayModel(Model);
  Eng.pretestConsistency(PInfos);
  displayPretestsMsgs(PInfos);
  BOOST_REQUIRE_EQUAL(PInfos.ExtraFiles,true);
  BOOST_REQUIRE_EQUAL(PInfos.Inputdata,true);
  BOOST_REQUIRE_EQUAL(PInfos.Model,true);
  PInfos = openfluid::machine::Engine::PretestInfos_t();

  // =====================================================================

  MIInstance = new openfluid::machine::ModelItemInstance();
  MIInstance->Body = new EmptyFunction();
  MIInstance->SDKCompatible = true;
  MIInstance->Signature = new openfluid::ware::FunctionSignature();
  MIInstance->Signature->ID = "MyFunc2";
  MIInstance->Signature->HandledData.RequiredVars.push_back(openfluid::ware::SignatureHandledTypedDataItem("var5[vector]","UB","",""));

  Model.resetInitialized();
  Model.appendItem(MIInstance);

  displayModel(Model);
  Eng.pretestConsistency(PInfos);
  displayPretestsMsgs(PInfos);
  BOOST_REQUIRE_EQUAL(PInfos.ExtraFiles,true);
  BOOST_REQUIRE_EQUAL(PInfos.Inputdata,true);
  BOOST_REQUIRE_EQUAL(PInfos.Model,true);
  PInfos = openfluid::machine::Engine::PretestInfos_t();


  // =====================================================================

  MIInstance = new openfluid::machine::ModelItemInstance();
  MIInstance->Body = new EmptyFunction();
  MIInstance->SDKCompatible = true;
  MIInstance->Signature = new openfluid::ware::FunctionSignature();
  MIInstance->Signature->ID = "MyFunc0.5";
  MIInstance->Signature->HandledData.RequiredVars.push_back(openfluid::ware::SignatureHandledTypedDataItem("var1","UA","",""));
  MIInstance->Signature->HandledData.RequiredPrevVars.push_back(openfluid::ware::SignatureHandledTypedDataItem("var5[]","UB","",""));

  Model.resetInitialized();
  Model.insertItem(MIInstance,1);

  displayModel(Model);
  Eng.pretestConsistency(PInfos);
  displayPretestsMsgs(PInfos);
  BOOST_REQUIRE_EQUAL(PInfos.ExtraFiles,true);
  BOOST_REQUIRE_EQUAL(PInfos.Inputdata,true);
  BOOST_REQUIRE_EQUAL(PInfos.Model,true);
  PInfos = openfluid::machine::Engine::PretestInfos_t();


  // =====================================================================

  MIInstance = new openfluid::machine::ModelItemInstance();
  MIInstance->Body = new EmptyFunction();
  MIInstance->SDKCompatible = true;
  MIInstance->Signature = new openfluid::ware::FunctionSignature();
  MIInstance->Signature->ID = "MyFunc3";
  MIInstance->Signature->HandledData.RequiredPrevVars.push_back(openfluid::ware::SignatureHandledTypedDataItem("var7[double]","UC","",""));

  Model.resetInitialized();
  Model.appendItem(MIInstance);

  displayModel(Model);
  Eng.pretestConsistency(PInfos);
  displayPretestsMsgs(PInfos);
  BOOST_REQUIRE_EQUAL(PInfos.ExtraFiles,true);
  BOOST_REQUIRE_EQUAL(PInfos.Inputdata,true);
  BOOST_REQUIRE_EQUAL(PInfos.Model,false);
  PInfos = openfluid::machine::Engine::PretestInfos_t();

  // =====================================================================

  MIInstance = new openfluid::machine::ModelItemInstance();
  MIInstance->Body = new EmptyFunction();
  MIInstance->SDKCompatible = true;
  MIInstance->Signature = new openfluid::ware::FunctionSignature();
  MIInstance->Signature->ID = "MyFunc4";
  MIInstance->Signature->HandledData.RequiredVars.push_back(openfluid::ware::SignatureHandledTypedDataItem("var1","UA","",""));
  MIInstance->Signature->HandledData.ProducedVars.push_back(openfluid::ware::SignatureHandledTypedDataItem("var7","UC","",""));


  Model.resetInitialized();
  Model.appendItem(MIInstance);

  displayModel(Model);
  Eng.pretestConsistency(PInfos);
  displayPretestsMsgs(PInfos);
  BOOST_REQUIRE_EQUAL(PInfos.ExtraFiles,true);
  BOOST_REQUIRE_EQUAL(PInfos.Inputdata,true);
  BOOST_REQUIRE_EQUAL(PInfos.Model,false);
  PInfos = openfluid::machine::Engine::PretestInfos_t();



  delete MachineListen;
  delete IOListen;

}

// =====================================================================
// =====================================================================
