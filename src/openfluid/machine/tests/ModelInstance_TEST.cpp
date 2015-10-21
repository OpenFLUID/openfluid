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
  @file ModelInstance_TEST.cpp

  @author Jean-Christophe FABRE <jean-christophe.fabre@supagro.inra.fr>
 */


#define BOOST_TEST_MAIN
#define BOOST_AUTO_TEST_MAIN
#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MODULE unittest_ModelInstance
#include <boost/test/unit_test.hpp>
#include <boost/test/auto_unit_test.hpp>
#include <openfluid/machine/ModelInstance.hpp>
#include <openfluid/machine/ModelItemInstance.hpp>
#include <openfluid/machine/SimulationBlob.hpp>
#include <openfluid/base/RuntimeEnv.hpp>
#include <tests-config.hpp>


// =====================================================================
// =====================================================================


class SimA : openfluid::ware::PluggableSimulator
{
  public:

    SimA() : openfluid::ware::PluggableSimulator()
    { };

    ~SimA()
    { };

    void initParams(const openfluid::ware::WareParams_t& /*Params*/)
    { }

    void prepareData()
    { }

    void checkConsistency()
    { }

    openfluid::base::SchedulingRequest initializeRun()
    { return DefaultDeltaT(); }

    openfluid::base::SchedulingRequest runStep()
    { std::cout << "sim.a " << OPENFLUID_GetCurrentTimeIndex() << std::endl; return DefaultDeltaT(); }

    void finalizeRun()
    { }

};


class SimB : openfluid::ware::PluggableSimulator
{
  public:

    SimB() : openfluid::ware::PluggableSimulator()
    { };

    ~SimB()
    { };

    void initParams(const openfluid::ware::WareParams_t& /*Params*/)
    { }

    void prepareData()
    { }

    void checkConsistency()
    { }

    openfluid::base::SchedulingRequest initializeRun()
    { return DefaultDeltaT(); }

    openfluid::base::SchedulingRequest runStep()
    { std::cout << "sim.b " << OPENFLUID_GetCurrentTimeIndex() << std::endl; return MultipliedDefaultDeltaT(2); }

    void finalizeRun()
    { }

};


class SimC : openfluid::ware::PluggableSimulator
{
  public:

    SimC() : openfluid::ware::PluggableSimulator()
    { };

    ~SimC()
    { };

    void initParams(const openfluid::ware::WareParams_t& /*Params*/)
    { }

    void prepareData()
    { }

    void checkConsistency()
    { }

    openfluid::base::SchedulingRequest initializeRun()
    { return DefaultDeltaT(); }

    openfluid::base::SchedulingRequest runStep()
    { std::cout << "sim.c " << OPENFLUID_GetCurrentTimeIndex() << std::endl; return MultipliedDefaultDeltaT(0.5); }

    void finalizeRun()
    { }

};


// =====================================================================
// =====================================================================


BOOST_AUTO_TEST_CASE(check_construction)
{
  openfluid::base::RuntimeEnvironment::instance()
  ->setOutputDir(CONFIGTESTS_OUTPUT_DATA_DIR+"/OPENFLUID.OUT.ModelInstance");

  openfluid::machine::SimulationBlob SB;

  SB.simulationStatus() = openfluid::base::SimulationStatus(openfluid::core::DateTime(2012,1,1,0,0,0),
                                                            openfluid::core::DateTime(2012,1,1,14,46,39),60);

  openfluid::machine::ModelInstance MI(SB,nullptr);

  BOOST_REQUIRE(!MI.hasTimePointToProcess());
}

// =====================================================================
// =====================================================================

BOOST_AUTO_TEST_CASE(check_operations)
{
  openfluid::base::RuntimeEnvironment::instance()
  ->setOutputDir(CONFIGTESTS_OUTPUT_DATA_DIR+"/OPENFLUID.OUT.ModelInstance");

  openfluid::machine::SimulationBlob SB;

  /*SB.getSimulationStatus() = openfluid::base::SimulationStatus(openfluid::core::DateTime(2012,1,1,0,0,0),
                                                                 openfluid::core::DateTime(2012,1,1,14,46,39),60);*/

  SB.simulationStatus() = openfluid::base::SimulationStatus(openfluid::core::DateTime(2012,1,1,0,0,0),
                                                            openfluid::core::DateTime(2012,1,1,0,3,19),60);

  openfluid::machine::ModelInstance MI(SB,nullptr);

  openfluid::machine::ModelItemInstance* MII;

  MII = new openfluid::machine::ModelItemInstance();
  MII->Body = (openfluid::ware::PluggableSimulator*)(new SimA());
  MII->Signature = new openfluid::ware::SimulatorSignature();
  MII->Signature->ID = "sim.a";
  MII->OriginalPosition = 1;
  MI.appendItem(MII);

  MII = new openfluid::machine::ModelItemInstance();
  MII->Body = (openfluid::ware::PluggableSimulator*)(new SimB());
  MII->Signature = new openfluid::ware::SimulatorSignature();
  MII->Signature->ID = "sim.b";
  MII->OriginalPosition = 2;
  MI.appendItem(MII);

  MII = new openfluid::machine::ModelItemInstance();
  MII->Body = (openfluid::ware::PluggableSimulator*)(new SimC());
  MII->Signature = new openfluid::ware::SimulatorSignature();
  MII->Signature->ID = "sim.c";
  MII->OriginalPosition = 3;
  MI.appendItem(MII);

  openfluid::base::SimulationLogger* SimLog =
      new openfluid::base::SimulationLogger(CONFIGTESTS_OUTPUT_DATA_DIR+"/checksimlog2.log");

  MI.initialize(SimLog);

  SB.simulationStatus().setCurrentStage(openfluid::base::SimulationStatus::INITPARAMS);
  MI.call_initParams();

  SB.simulationStatus().setCurrentStage(openfluid::base::SimulationStatus::PREPAREDATA);
  MI.call_prepareData();

  SB.simulationStatus().setCurrentStage(openfluid::base::SimulationStatus::CHECKCONSISTENCY);
  MI.call_checkConsistency();


  SB.simulationStatus().setCurrentStage(openfluid::base::SimulationStatus::INITIALIZERUN);
  MI.call_initializeRun();

  BOOST_REQUIRE(MI.hasTimePointToProcess());

  SB.simulationStatus().setCurrentStage(openfluid::base::SimulationStatus::RUNSTEP);
  while (MI.hasTimePointToProcess())
  {
    std::cout << " ******** " << MI.getNextTimePointIndex() << " ********" << std::endl;
    MI.processNextTimePoint();

  }

  BOOST_REQUIRE(!MI.hasTimePointToProcess());

  SB.simulationStatus().setCurrentStage(openfluid::base::SimulationStatus::FINALIZERUN);
  MI.call_finalizeRun();

  MI.finalize();

  delete SimLog;

}

// =====================================================================
// =====================================================================

// =====================================================================
// =====================================================================

class ModelInstanceSub: public openfluid::machine::ModelInstance
{
  public:

    ModelInstanceSub(openfluid::machine::SimulationBlob& SimulationBlob,
                     openfluid::machine::MachineListener* Listener) :
                       ModelInstance(SimulationBlob, Listener)
  {
  }

    openfluid::ware::WareParams_t mergeParamsWithGlobalParams(
        const openfluid::ware::WareParams_t& Params)
    {
      return ModelInstance::mergeParamsWithGlobalParams(Params);
    }

};

BOOST_AUTO_TEST_CASE(check_mergeParamsWithGlobalParams)
{
  openfluid::base::RuntimeEnvironment::instance()
  ->setOutputDir(CONFIGTESTS_OUTPUT_DATA_DIR+"/OPENFLUID.OUT.ModelInstance");

  openfluid::machine::SimulationBlob SB;

  ModelInstanceSub MI(SB, nullptr);

  openfluid::machine::ModelItemInstance* MII = new openfluid::machine::ModelItemInstance();
  MII->Body = (openfluid::ware::PluggableSimulator*) (new SimA());
  MII->Signature = new openfluid::ware::SimulatorSignature();
  MII->Signature->HandledData.UsedParams.push_back(*new openfluid::ware::SignatureDataItem("A1","",""));
  MII->Signature->HandledData.UsedParams.push_back(*new openfluid::ware::SignatureDataItem("B1.C1","",""));
  MII->Signature->HandledData.UsedParams.push_back(*new openfluid::ware::SignatureDataItem("D1.E1.F1","",""));

  MI.appendItem(MII);

  openfluid::ware::WareParams_t InParams;

  openfluid::ware::WareParams_t OutParams = MI.mergeParamsWithGlobalParams(InParams);
  BOOST_CHECK(OutParams.empty());

  // local doesn't exist - global set -> take global
  MI.setGlobalParameter("A1",std::string("A1global"));
  MI.setGlobalParameter("B1.C1",std::string("B1.C1global"));
  MI.setGlobalParameter("D1.E1.F1",std::string("D1.E1.F1global"));
  OutParams = MI.mergeParamsWithGlobalParams(InParams);
  BOOST_CHECK_EQUAL(OutParams.size(),3);
  BOOST_CHECK_EQUAL(OutParams["A1"].get(),"A1global");
  BOOST_CHECK_EQUAL(OutParams["B1.C1"].get(),"B1.C1global");
  BOOST_CHECK_EQUAL(OutParams["D1.E1.F1"].get(),"D1.E1.F1global");

  // local set - global set -> take local
  InParams["A1"] = openfluid::ware::WareParamValue_t("A1local");
  InParams["B1.C1"] = openfluid::ware::WareParamValue_t("B1.C1local");
  InParams["D1.E1.F1"] = openfluid::ware::WareParamValue_t("D1.E1.F1local");
  OutParams = MI.mergeParamsWithGlobalParams(InParams);
  BOOST_CHECK_EQUAL(OutParams.size(),3);
  BOOST_CHECK_EQUAL(OutParams["A1"].get(),"A1local");
  BOOST_CHECK_EQUAL(OutParams["B1.C1"].get(),"B1.C1local");
  BOOST_CHECK_EQUAL(OutParams["D1.E1.F1"].get(),"D1.E1.F1local");

  // local set empty - global set -> take global
  InParams["A1"] = openfluid::ware::WareParamValue_t("");
  InParams["B1.C1"] = openfluid::ware::WareParamValue_t("");
  InParams["D1.E1.F1"] = openfluid::ware::WareParamValue_t("");
  OutParams = MI.mergeParamsWithGlobalParams(InParams);
  BOOST_CHECK_EQUAL(OutParams.size(),3);
  BOOST_CHECK_EQUAL(OutParams["A1"].get(),"A1global");
  BOOST_CHECK_EQUAL(OutParams["B1.C1"].get(),"B1.C1global");
  BOOST_CHECK_EQUAL(OutParams["D1.E1.F1"].get(),"D1.E1.F1global");

  openfluid::ware::WareParams_t EmptyParams;

  // local set empty - global doesn't exist -> take local (empty)
  MI.setGlobalParameters(EmptyParams);
  OutParams = MI.mergeParamsWithGlobalParams(InParams);
  BOOST_CHECK_EQUAL(OutParams.size(),3);
  BOOST_CHECK_EQUAL(OutParams["A1"].get(),"");
  BOOST_CHECK_EQUAL(OutParams["B1.C1"].get(),"");
  BOOST_CHECK_EQUAL(OutParams["D1.E1.F1"].get(),"");

  // local set - global doesn't exist -> take local
  InParams["A1"] = openfluid::ware::WareParamValue_t("A1local");
  InParams["B1.C1"] = openfluid::ware::WareParamValue_t("B1.C1local");
  InParams["D1.E1.F1"] = openfluid::ware::WareParamValue_t("D1.E1.F1local");
  OutParams = MI.mergeParamsWithGlobalParams(InParams);
  BOOST_CHECK_EQUAL(OutParams.size(),3);
  BOOST_CHECK_EQUAL(OutParams["A1"].get(),"A1local");
  BOOST_CHECK_EQUAL(OutParams["B1.C1"].get(),"B1.C1local");
  BOOST_CHECK_EQUAL(OutParams["D1.E1.F1"].get(),"D1.E1.F1local");

  // local set - global set -> take local
  MI.setGlobalParameter("A1",std::string("A1global"));
  MI.setGlobalParameter("B1.C1",std::string("B1.C1global"));
  MI.setGlobalParameter("D1.E1.F1",std::string("D1.E1.F1global"));
  BOOST_CHECK_EQUAL(OutParams.size(),3);
  BOOST_CHECK_EQUAL(OutParams["A1"].get(),"A1local");
  BOOST_CHECK_EQUAL(OutParams["B1.C1"].get(),"B1.C1local");
  BOOST_CHECK_EQUAL(OutParams["D1.E1.F1"].get(),"D1.E1.F1local");
}

// =====================================================================
// =====================================================================
