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
  @file GeneratorFromFile_TEST.cpp

  @author Armel THÖNI <armel.thoni@inrae.fr>
  @author Dorian GERARDIN <dorian.gerardin@gmail.com>
*/


#define BOOST_TEST_MAIN
#define BOOST_AUTO_TEST_MAIN
#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MODULE unittest_engine


#include <boost/test/unit_test.hpp>

#include <openfluid/machine/GeneratorSignature.hpp>
#include <openfluid/machine/RandomGenerator.hpp>
#include <openfluid/machine/SimulatorRegistry.hpp>
#include <openfluid/machine/MachineListener.hpp>
#include <openfluid/machine/ModelInstance.hpp>
#include <openfluid/machine/MonitoringInstance.hpp>
#include <openfluid/machine/Engine.hpp>
#include <openfluid/machine/ModelItemInstance.hpp>
#include <openfluid/machine/SimulationBlob.hpp>
#include <openfluid/base/RunContextManager.hpp>
#include <openfluid/fluidx/GeneratorDescriptor.hpp>

#include "TestSimulation.cpp"


void fileTestSetupAndRun(TestSimulation& TS, openfluid::machine::GeneratorSpecs Specs, 
                         openfluid::ware::WareParams_t Params, 
                         openfluid::core::DateTime EndDate=openfluid::core::DateTime(2000,01,01,1,0,0))
{
  TS.defaultSetup();
  openfluid::base::RunContextManager::instance()->setInputDir(
    CONFIGTESTS_INPUT_DATASETS_DIR+"/OPENFLUID.IN.Generators");
  TS.SB.simulationStatus() = openfluid::base::SimulationStatus(openfluid::core::DateTime(2000,01,01,0,0,0), EndDate
                                                              ,60);
  std::map<int, int> UnitsInfos = {{1,1}, {2,2}, {3,1}, {4,3}, {5,1}};
  for (auto& UnitInfos : UnitsInfos)
  {
    TS.SB.spatialGraph().addUnit(openfluid::core::SpatialUnit("TestUnits",UnitInfos.first,UnitInfos.second));
  }
  TS.addGenerator(Specs, Params);
  TS.wholeSimulation();
}


// =====================================================================
// =====================================================================


std::string asString(openfluid::core::IndexedValue IValue)
{
  return IValue.value()->asStringValue().get();
}


// =====================================================================
// =====================================================================


BOOST_AUTO_TEST_CASE(check_inject)
{
  {
    // REGULAR INJECTION
    std::cout << "Checking float injection" << std::endl;
    openfluid::machine::GeneratorSpecs Specs{openfluid::fluidx::GeneratorDescriptor::GeneratorMethod::INJECT, 
                                            {{"TestUnits","tests.inject"}}};
    openfluid::ware::WareParams_t Params = {{"sources", "sourcesinject.xml"}, 
                                            {"distribution", "distri.dat"}};

    TestSimulation TS;
    fileTestSetupAndRun(TS, Specs, Params);
    BOOST_REQUIRE_CLOSE(asDouble(TS.getLatestValue("TestUnits", 1, "tests.inject")), 60, 0.00001);
    BOOST_REQUIRE_CLOSE(asDouble(TS.getLatestValue("TestUnits", 2, "tests.inject")), 0.0, 0.00001);
  }
  {
    // Missing file
    std::cout << "Checking Missing file" << std::endl;
    openfluid::machine::GeneratorSpecs Specs{openfluid::fluidx::GeneratorDescriptor::GeneratorMethod::INJECT, 
                                            {{"TestUnits","tests.inject"}}};
    openfluid::ware::WareParams_t Params = {{"sources", "sourcesinject_MISSING.xml"}, 
                                            {"distribution", "distri.dat"}};

    TestSimulation TS;
    TS.defaultSetup();
    TS.addGenerator(Specs, Params);
    BOOST_REQUIRE_THROW(TS.wholeSimulation(), openfluid::base::FrameworkException);
  }
  {
    // Inject string
    std::cout << "Checking injection of string" << std::endl;
    openfluid::machine::GeneratorSpecs Specs{openfluid::fluidx::GeneratorDescriptor::GeneratorMethod::INJECT, 
                                            {{"TestUnits","tests.inject"}},
                                             openfluid::core::Value::STRING};
    openfluid::ware::WareParams_t Params = {{"sources", "sourcesinject_checktype.xml"}, 
                                            {"distribution", "distri.dat"}};

    TestSimulation TS;
    fileTestSetupAndRun(TS, Specs, Params, openfluid::core::DateTime(2000,01,01,2,0,0));
    BOOST_REQUIRE_EQUAL(asString(TS.getLatestValue("TestUnits", 1, "tests.inject")), "-25");
    BOOST_REQUIRE_EQUAL(asString(TS.getLatestValue("TestUnits", 2, "tests.inject")), "125");
  }
  {
    // Inject int
    std::cout << "Checking injection of int" << std::endl;
    openfluid::machine::GeneratorSpecs Specs{openfluid::fluidx::GeneratorDescriptor::GeneratorMethod::INJECT, 
                                            {{"TestUnits","tests.inject"}},
                                             openfluid::core::Value::INTEGER};
    openfluid::ware::WareParams_t Params = {{"sources", "sourcesinject_checktype.xml"}, 
                                            {"distribution", "distri.dat"}};

    TestSimulation TS;
    fileTestSetupAndRun(TS, Specs, Params, openfluid::core::DateTime(2000,01,01,2,0,0));
    BOOST_REQUIRE_EQUAL(asInteger(TS.getLatestValue("TestUnits", 1, "tests.inject")), -25);
    BOOST_REQUIRE_EQUAL(asInteger(TS.getLatestValue("TestUnits", 2, "tests.inject")), 125);
  }
  {
    // Inject bool
    std::cout << "Checking injection of bool" << std::endl;
    openfluid::machine::GeneratorSpecs Specs{openfluid::fluidx::GeneratorDescriptor::GeneratorMethod::INJECT, 
                                            {{"TestUnits","tests.inject"}},
                                             openfluid::core::Value::BOOLEAN};
    openfluid::ware::WareParams_t Params = {{"sources", "sourcesinject_BOOL.xml"}, 
                                            {"distribution", "distri.dat"}};

    TestSimulation TS;
    fileTestSetupAndRun(TS, Specs, Params);
    BOOST_REQUIRE_EQUAL(asBoolean(TS.getLatestValue("TestUnits", 1, "tests.inject")), false);
    BOOST_REQUIRE_EQUAL(asBoolean(TS.getLatestValue("TestUnits", 2, "tests.inject")), true);
  }
  {
    // Inject vector
    std::cout << "Checking injection of vector" << std::endl;
    openfluid::machine::GeneratorSpecs Specs{openfluid::fluidx::GeneratorDescriptor::GeneratorMethod::INJECT, 
                                            {{"TestUnits","tests.inject"}},
                                             openfluid::core::Value::VECTOR};
    openfluid::ware::WareParams_t Params = {{"sources", "sourcesinject_VECTOR.xml"}, 
                                            {"distribution", "distri.dat"}};

    TestSimulation TS;
    fileTestSetupAndRun(TS, Specs, Params);
    BOOST_REQUIRE_CLOSE(TS.getLatestValue("TestUnits", 1, "tests.inject").value()->asVectorValue()[2], 6, 0.00001);
    BOOST_REQUIRE_CLOSE(TS.getLatestValue("TestUnits", 2, "tests.inject").value()->asVectorValue()[2], 60, 0.00001);
  }
  {
    // Inject matrix
    std::cout << "Checking injection of matrix" << std::endl;
    openfluid::machine::GeneratorSpecs Specs{openfluid::fluidx::GeneratorDescriptor::GeneratorMethod::INJECT, 
                                            {{"TestUnits","tests.inject"}},
                                             openfluid::core::Value::MATRIX};
    openfluid::ware::WareParams_t Params = {{"sources", "sourcesinject_MATRIX.xml"}, 
                                            {"distribution", "distri.dat"}};

    TestSimulation TS;
    fileTestSetupAndRun(TS, Specs, Params);
    BOOST_REQUIRE_CLOSE(TS.getLatestValue("TestUnits", 1, "tests.inject").value()->asMatrixValue().get(1,1), 8, 
                                          0.00001);
    BOOST_REQUIRE_CLOSE(TS.getLatestValue("TestUnits", 2, "tests.inject").value()->asMatrixValue().get(1,1), 80, 
                                          0.00001);
  }
  {
    // Inject map
    std::cout << "Checking injection of map" << std::endl;
    openfluid::machine::GeneratorSpecs Specs{openfluid::fluidx::GeneratorDescriptor::GeneratorMethod::INJECT, 
                                            {{"TestUnits","tests.inject"}},
                                             openfluid::core::Value::MAP};
    openfluid::ware::WareParams_t Params = {{"sources", "sourcesinject_MAP.xml"}, 
                                            {"distribution", "distri.dat"}};

    TestSimulation TS;
    fileTestSetupAndRun(TS, Specs, Params);
    BOOST_REQUIRE_EQUAL(TS.getLatestValue("TestUnits", 1, "tests.inject").value()->asMapValue().getMapValue("3").\
                             getVectorValue("internal")[1], 3);
    BOOST_REQUIRE_CLOSE(TS.getLatestValue("TestUnits", 2, "tests.inject").value()->asMapValue().getDouble("3"), 18, 
                                          0.00001);
  }
  {
    // Multiple types in same sources
    std::cout << "Checking injection of different types" << std::endl;
    openfluid::machine::GeneratorSpecs Specs{openfluid::fluidx::GeneratorDescriptor::GeneratorMethod::INJECT, 
                                            {{"TestUnits","tests.inject"}},
                                             openfluid::core::Value::BOOLEAN};
    openfluid::ware::WareParams_t Params = {{"sources", "sourcesinject_different_types.xml"}, 
                                            {"distribution", "distri.dat"}};

    TestSimulation TS;
    TS.defaultSetup();
    TS.addGenerator(Specs, Params);
    BOOST_REQUIRE_THROW(TS.wholeSimulation(), openfluid::base::FrameworkException);
  }
  {
    // Wrong type
    std::cout << "Checking injection of wrong type" << std::endl;
    openfluid::machine::GeneratorSpecs Specs{openfluid::fluidx::GeneratorDescriptor::GeneratorMethod::INJECT, 
                                            {{"TestUnits","tests.inject"}},
                                             openfluid::core::Value::BOOLEAN};
    openfluid::ware::WareParams_t Params = {{"sources", "sourcesinject_MAP.xml"}, 
                                            {"distribution", "distri.dat"}};

    TestSimulation TS;
    TS.defaultSetup();
    TS.addGenerator(Specs, Params);
    BOOST_REQUIRE_THROW(TS.wholeSimulation(), openfluid::base::FrameworkException);
  }
}


// =====================================================================
// =====================================================================


BOOST_AUTO_TEST_CASE(check_interp)
{
  {
    // REGULAR INTERPOLATION
    std::cout << "Checking REGULAR INTERPOLATION" << std::endl;
    openfluid::machine::GeneratorSpecs Specs{openfluid::fluidx::GeneratorDescriptor::GeneratorMethod::INTERP, 
                                            {{"TestUnits", "tests.interp"}}};
    openfluid::ware::WareParams_t Params = {{"sources", "sourcesinterp.xml"}, 
                                            {"distribution", "distri.dat"}};

    TestSimulation TS;
    fileTestSetupAndRun(TS, Specs, Params);
    BOOST_REQUIRE_CLOSE(asDouble(TS.getLatestValue("TestUnits", 1, "tests.interp")), -9, 0.00001);
    BOOST_REQUIRE_CLOSE(asDouble(TS.getLatestValue("TestUnits", 2, "tests.interp")), 109.94, 0.01);
  }
  {
    // Missing file
    std::cout << "Checking Missing file" << std::endl;
    openfluid::machine::GeneratorSpecs Specs{openfluid::fluidx::GeneratorDescriptor::GeneratorMethod::INTERP, 
                                            {{"TestUnits", "tests.interp"}}};
    openfluid::ware::WareParams_t Params = {{"sources", "sourcesinterp_MISSING.xml"}, 
                                            {"distribution", "distri.dat"}};

    TestSimulation TS;
    TS.defaultSetup();
    TS.addGenerator(Specs, Params);
    BOOST_REQUIRE_THROW(TS.wholeSimulation(), openfluid::base::FrameworkException);
  }
}


// =====================================================================
// =====================================================================
  

BOOST_AUTO_TEST_CASE(check_inject_multi)
{
  {
    //EXPLICIT COLUMNS
    openfluid::machine::GeneratorSpecs Specs{openfluid::fluidx::GeneratorDescriptor::GeneratorMethod::INJECTMULTICOL, 
      openfluid::tools::deserializeVarTriplets("SU#1:var.a;SU#2:var.a;SU#1:var.b;SU#2:var.b")};
    openfluid::ware::WareParams_t Params = {{"datafile", 
                                             CONFIGTESTS_INPUT_MISCDATA_DIR+"/MultiInjectData/multi_out.csv"}};

    TestSimulation TS;
    TS.defaultSetup();
    TS.addGenerator(Specs, Params);
    TS.wholeSimulation();
    BOOST_REQUIRE_CLOSE(asDouble(TS.getLatestValue("SU", 1, "var.b")), 0.03, 0.00001);
    BOOST_REQUIRE_CLOSE(asDouble(TS.getLatestValue("SU", 2, "var.a")), 3.5823, 0.00001);
  }
  {
    //GENERIC COLUMNS IN SELECTION
    openfluid::machine::GeneratorSpecs Specs{openfluid::fluidx::GeneratorDescriptor::GeneratorMethod::INJECTMULTICOL, 
                                            openfluid::tools::deserializeVarTriplets("SU#*:var.a;SU#*:var.b")};
    openfluid::ware::WareParams_t Params = {{"datafile", 
                                             CONFIGTESTS_INPUT_MISCDATA_DIR+"/MultiInjectData/multi_out.csv"}};

    TestSimulation TS;
    TS.defaultSetup();
    TS.addGenerator(Specs, Params);
    TS.wholeSimulation();
    BOOST_REQUIRE_CLOSE(asDouble(TS.getLatestValue("SU", 1, "var.b")), 0.03, 0.00001);
    BOOST_REQUIRE_CLOSE(asDouble(TS.getLatestValue("SU", 2, "var.a")), 3.5823, 0.00001);
  }
  {
    //GENERIC COLUMNS IN DATA FILE
    openfluid::machine::GeneratorSpecs Specs{openfluid::fluidx::GeneratorDescriptor::GeneratorMethod::INJECTMULTICOL, 
      openfluid::tools::deserializeVarTriplets("SU#1:var.a;SU#2:var.a;SU#1:var.b;SU#2:var.b")};
    openfluid::ware::WareParams_t Params = {{"datafile", 
                                             CONFIGTESTS_INPUT_MISCDATA_DIR+"/MultiInjectData/multi_joker.csv"}};

    TestSimulation TS;
    TS.defaultSetup();
    TS.addGenerator(Specs, Params);
    TS.wholeSimulation();
    BOOST_REQUIRE_CLOSE(asDouble(TS.getLatestValue("SU", 1, "var.b")), 0.03, 0.00001);
    BOOST_REQUIRE_CLOSE(asDouble(TS.getLatestValue("SU", 1, "var.a")), 2.5632, 0.00001);
  }
  {
    //GENERIC COLUMNS IN DATA FILE AND SELECTION
    openfluid::machine::GeneratorSpecs Specs{openfluid::fluidx::GeneratorDescriptor::GeneratorMethod::INJECTMULTICOL, 
                                            openfluid::tools::deserializeVarTriplets("SU#*:var.a;SU#*:var.b")};
    openfluid::ware::WareParams_t Params = {{"datafile", 
                                             CONFIGTESTS_INPUT_MISCDATA_DIR+"/MultiInjectData/multi_joker.csv"}};

    TestSimulation TS;
    TS.defaultSetup();
    TS.addGenerator(Specs, Params);
    TS.wholeSimulation();
    BOOST_REQUIRE_CLOSE(asDouble(TS.getLatestValue("SU", 1, "var.a")), 2.5632, 0.00001);
    BOOST_REQUIRE_CLOSE(asDouble(TS.getLatestValue("SU", 1, "var.b")), 0.03, 0.00001);
    BOOST_REQUIRE_CLOSE(asDouble(TS.getLatestValue("SU", 2, "var.a")), 3.5823, 0.00001);
  }


  // BAD SETTING
  {
    //Missing data file
    openfluid::machine::GeneratorSpecs Specs{openfluid::fluidx::GeneratorDescriptor::GeneratorMethod::INJECTMULTICOL, 
                                            openfluid::tools::deserializeVarTriplets("SU#*:var.a;SU#*:var.b")};
    openfluid::ware::WareParams_t Params = {{"datafile", 
                                             CONFIGTESTS_INPUT_MISCDATA_DIR+"/MultiInjectData/multi_out_MISSING.csv"}};

    TestSimulation TS;
    TS.defaultSetup();
    TS.addGenerator(Specs, Params);
    BOOST_REQUIRE_THROW(TS.wholeSimulation(), openfluid::base::FrameworkException);
  }
  {
    //Missing column
    openfluid::machine::GeneratorSpecs Specs{openfluid::fluidx::GeneratorDescriptor::GeneratorMethod::INJECTMULTICOL, 
                                            openfluid::tools::deserializeVarTriplets("SU#*:var.a;SU#*:var.c")};
    openfluid::ware::WareParams_t Params = {{"datafile", 
                                             CONFIGTESTS_INPUT_MISCDATA_DIR+"/MultiInjectData/multi_out.csv"}};

    TestSimulation TS;
    TS.defaultSetup();
    TS.addGenerator(Specs, Params);
    BOOST_REQUIRE_THROW(TS.wholeSimulation(), openfluid::base::FrameworkException);
  }
  {
    //Missing unit in datafile
    openfluid::machine::GeneratorSpecs Specs{openfluid::fluidx::GeneratorDescriptor::GeneratorMethod::INJECTMULTICOL, 
                                            openfluid::tools::deserializeVarTriplets("SU#*:var.a")};
    openfluid::ware::WareParams_t Params = {{"datafile", 
                                             CONFIGTESTS_INPUT_MISCDATA_DIR+"/MultiInjectData/multi_out.csv"}};

    TestSimulation TS;
    TS.defaultSetup();
    TS.SB.spatialGraph().addUnit(openfluid::core::SpatialUnit("SU",10,2));
    TS.addGenerator(Specs, Params);
    BOOST_REQUIRE_THROW(TS.wholeSimulation(), openfluid::base::FrameworkException);
  }
  {
    //Missing unit in datafile with jokers
    openfluid::machine::GeneratorSpecs Specs{openfluid::fluidx::GeneratorDescriptor::GeneratorMethod::INJECTMULTICOL, 
                                            openfluid::tools::deserializeVarTriplets("SU#*:var.a")};
    openfluid::ware::WareParams_t Params = {{"datafile", 
                                             CONFIGTESTS_INPUT_MISCDATA_DIR+"/MultiInjectData/multi_out.csv"}};

    TestSimulation TS;
    TS.defaultSetup();
    TS.SB.spatialGraph().addUnit(openfluid::core::SpatialUnit("SU",10,2));
    TS.addGenerator(Specs, Params);
    BOOST_REQUIRE_THROW(TS.wholeSimulation(), openfluid::base::FrameworkException);
  }
  {
    // Unit class not present in spatial domain
    openfluid::machine::GeneratorSpecs Specs{openfluid::fluidx::GeneratorDescriptor::GeneratorMethod::INJECTMULTICOL, 
                                          openfluid::tools::deserializeVarTriplets("SU#*:var.a;SU#*:var.b;RS#*:var.a")};
    openfluid::ware::WareParams_t Params = {{"datafile", 
                                          CONFIGTESTS_INPUT_MISCDATA_DIR+"/MultiInjectData/multi_wrong_unitclass.csv"}};

    TestSimulation TS;
    TS.defaultSetup();
    TS.addGenerator(Specs, Params);
    BOOST_REQUIRE_THROW(TS.wholeSimulation(), openfluid::base::FrameworkException);
  }

  // BAD DATA
  {
    // nan
    std::cout << "Checking nan value" << std::endl;
    openfluid::machine::GeneratorSpecs Specs{openfluid::fluidx::GeneratorDescriptor::GeneratorMethod::INJECTMULTICOL, 
                                            openfluid::tools::deserializeVarTriplets("SU#1:var.b;SU#2:var.b")};
    openfluid::ware::WareParams_t Params = {{"datafile", 
                                             CONFIGTESTS_INPUT_MISCDATA_DIR+"/MultiInjectData/multi_na.csv"}};

    TestSimulation TS;
    TS.defaultSetup();
    TS.addGenerator(Specs, Params);
    TS.wholeSimulation();
    // when nan found, latest available value is the one at previous time
    BOOST_REQUIRE_CLOSE(asDouble(TS.getLatestValue("SU", 2, "var.b")), 0.04, 0.00001);
  }
  {
    // missing
    std::cout << "Checking missing value" << std::endl;
    openfluid::machine::GeneratorSpecs Specs{openfluid::fluidx::GeneratorDescriptor::GeneratorMethod::INJECTMULTICOL, 
                                            openfluid::tools::deserializeVarTriplets("SU#1:var.b;SU#2:var.b")};
    openfluid::ware::WareParams_t Params = {{"datafile", 
                                             CONFIGTESTS_INPUT_MISCDATA_DIR+"/MultiInjectData/multi_missing.csv"}};

    TestSimulation TS;
    TS.defaultSetup();
    TS.addGenerator(Specs, Params);
    try
    {
      TS.wholeSimulation();
      BOOST_CHECK_MESSAGE(false, "Simulation not in exception");
    }
    catch (const openfluid::base::FrameworkException& E)
    {
      BOOST_REQUIRE(std::string(E.what()).find("Wrong number of columns") != std::string::npos);
    }
  }
  {
    // wrong type
    std::cout << "Checking wrong value" << std::endl;
    openfluid::machine::GeneratorSpecs Specs{openfluid::fluidx::GeneratorDescriptor::GeneratorMethod::INJECTMULTICOL, 
                                            openfluid::tools::deserializeVarTriplets("SU#1:var.b;SU#2:var.b")};
    openfluid::ware::WareParams_t Params = {{"datafile", 
                                             CONFIGTESTS_INPUT_MISCDATA_DIR+"/MultiInjectData/multi_wrongformat.csv"}};

    TestSimulation TS;
    TS.defaultSetup();
    TS.addGenerator(Specs, Params);
    try
    {
      TS.wholeSimulation();
      BOOST_CHECK_MESSAGE(false, "Simulation not in exception");
    }
    catch (const openfluid::base::FrameworkException& E)
    {
      // Expecting an exception explaining that the value has not been injected
      BOOST_REQUIRE(std::string(E.what()).find("Value not injected") != std::string::npos);
    }
  }
  {
    //Missing spatial unit
    std::cout << "Checking missing spatial unit" << std::endl;
    openfluid::machine::GeneratorSpecs Specs{openfluid::fluidx::GeneratorDescriptor::GeneratorMethod::INJECTMULTICOL, 
                                            openfluid::tools::deserializeVarTriplets("SU#1:var.a")};
    openfluid::ware::WareParams_t Params = {{"datafile", 
                                             CONFIGTESTS_INPUT_MISCDATA_DIR+"/MultiInjectData/multi_out.csv"}};

    TestSimulation TS;
    TS.defaultSetup();
    TS.addGenerator(Specs, Params);
    BOOST_REQUIRE_THROW(TS.wholeSimulation(), openfluid::base::FrameworkException);
  }
  {
    //Missing spatial unit joker
    std::cout << "Checking missing spatial unit joker" << std::endl;
    openfluid::machine::GeneratorSpecs Specs{openfluid::fluidx::GeneratorDescriptor::GeneratorMethod::INJECTMULTICOL, 
                                            openfluid::tools::deserializeVarTriplets("SU#1:var.a")};
    openfluid::ware::WareParams_t Params = {{"datafile", 
                                             CONFIGTESTS_INPUT_MISCDATA_DIR+"/MultiInjectData/multi_joker.csv"}};

    TestSimulation TS;
    TS.defaultSetup();
    TS.addGenerator(Specs, Params);
    BOOST_REQUIRE_THROW(TS.wholeSimulation(), openfluid::base::FrameworkException);
  }
  {
    //wrong SU in selection
    std::cout << "Checking wrong SU in selection" << std::endl;
    openfluid::machine::GeneratorSpecs Specs{openfluid::fluidx::GeneratorDescriptor::GeneratorMethod::INJECTMULTICOL, 
      openfluid::tools::deserializeVarTriplets("SU#1:var.a;SU#2:var.a;SU#1:var.b;SU#2:var.b;SU#3:var.a")};
    openfluid::ware::WareParams_t Params = {{"datafile", 
                                             CONFIGTESTS_INPUT_MISCDATA_DIR+"/MultiInjectData/multi_out.csv"}};

    TestSimulation TS;
    TS.defaultSetup();
    TS.addGenerator(Specs, Params);
    BOOST_REQUIRE_THROW(TS.wholeSimulation(), openfluid::base::FrameworkException);
  }
}
