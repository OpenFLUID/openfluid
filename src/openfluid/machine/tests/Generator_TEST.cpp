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
  @file Generator_TEST.cpp

  @author Armel THÖNI <armel.thoni@inrae.fr>
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

#include "tests-config.hpp"


class TestSimulation
{
  private:

    std::unique_ptr<openfluid::machine::ModelInstance> MInstance;

    std::unique_ptr<openfluid::machine::MonitoringInstance> Monitoring;

    std::unique_ptr<openfluid::machine::MachineListener> Listener;


  public:

    openfluid::machine::SimulationBlob SB;
    

    void defaultSetup()
    {
      openfluid::base::RunContextManager::instance()
        ->setOutputDir(CONFIGTESTS_OUTPUT_DATA_DIR+"/OPENFLUID.OUT.TestSimulation");

      SB.simulationStatus() = openfluid::base::SimulationStatus(openfluid::core::DateTime(2012,1,1,0,0,0),
                                                                openfluid::core::DateTime(2012,1,1,0,3,19),60);

      SB.spatialGraph().addUnit(openfluid::core::SpatialUnit("SU",1,1));
      SB.spatialGraph().addUnit(openfluid::core::SpatialUnit("SU",2,1));

      Monitoring = std::make_unique<openfluid::machine::MonitoringInstance>(SB);

      Listener = std::make_unique<openfluid::machine::MachineListener>();

      MInstance = std::make_unique<openfluid::machine::ModelInstance>(SB,Listener.get());
    }
  

    void addGenerator(const openfluid::machine::GeneratorSpecs& Specs, const openfluid::ware::WareParams_t& Params)
    {
      auto GenID = openfluid::machine::SimulatorRegistry::instance()->addGenerator(Specs);

      if (GenID.empty())
      {
        throw openfluid::base::FrameworkException(OPENFLUID_CODE_LOCATION,"invalid generator definition");
      }

      openfluid::machine::ModelItemInstance* IInstance = nullptr;
      IInstance = new openfluid::machine::ModelItemInstance(
        openfluid::machine::SimulatorRegistry::instance()->generatorContainer(GenID));
      IInstance->Params = Params;

      openfluid::base::RunContextManager::instance()->processWareParams(IInstance->Params);
      IInstance->OriginalPosition = MInstance.get()->getItemsCount()+1;
      MInstance.get()->appendItem(IInstance);
    }


    void wholeSimulation()
    {
      openfluid::machine::Engine Engine(SB,*MInstance,*Monitoring,Listener.get());

      Engine.initialize();
      Engine.initParams();
      Engine.prepareData();
      Engine.checkConsistency();
      Engine.run();
      Engine.finalize();
    }


    openfluid::core::IndexedValue getLatestValue(const std::string& UnitsClass, int UnitId, const std::string& Var)
    {
      openfluid::core::IndexedValue Value;

      openfluid::core::SpatialUnit* A = SB.spatialGraph().spatialUnit(UnitsClass, UnitId);
  
      A->variables()->getLatestIndexedValue(Var, Value);
      return Value;
    }
};


double asDouble(openfluid::core::IndexedValue IValue)
{
  return IValue.value()->asDoubleValue().get();
}


// =====================================================================
// =====================================================================
  

BOOST_AUTO_TEST_CASE(check_random_scalar)
{
  // TEST SCALAR RERUN NOT IDENTICAL
  {
    //DOUBLE
    openfluid::machine::GeneratorSpecs Specs{openfluid::fluidx::GeneratorDescriptor::GeneratorMethod::RANDOM, 
                                            "SU", 
                                            "a"};
    openfluid::ware::WareParams_t Params = {{"deltat", "0"}, {"min", "2"}, {"max","20"}};

    TestSimulation TS;
    TS.defaultSetup();
    TS.addGenerator(Specs, Params);
    TS.wholeSimulation();
    double RandomValue = asDouble(TS.getLatestValue("SU", 1, "a"));
    BOOST_ASSERT(RandomValue >= 2 && RandomValue <= 20);

    TestSimulation TS2;
    TS2.defaultSetup();
    TS2.addGenerator(Specs, Params);
    TS2.wholeSimulation();
    BOOST_ASSERT(RandomValue != asDouble(TS2.getLatestValue("SU", 1, "a")));
  }
  {
    //INT
    openfluid::machine::GeneratorSpecs Specs{openfluid::fluidx::GeneratorDescriptor::GeneratorMethod::RANDOM, 
                                             "SU", 
                                             "b", 
                                             openfluid::core::Value::INTEGER};
    openfluid::ware::WareParams_t Params = {{"deltat", "0"}, {"min", "2"}, {"max","20000"}};

    TestSimulation TS;
    TS.defaultSetup();
    TS.addGenerator(Specs, Params);
    TS.wholeSimulation();
    int RandomValue = TS.getLatestValue("SU", 1, "b").value()->asIntegerValue().get();
    BOOST_ASSERT(RandomValue >= 2 && RandomValue <= 20000);

    TestSimulation TS2;
    TS2.defaultSetup();
    TS2.addGenerator(Specs, Params);
    TS2.wholeSimulation();
    BOOST_ASSERT(RandomValue != TS2.getLatestValue("SU", 1, "b").value()->asIntegerValue().get());
  }
  {
    //BOOLEAN
    openfluid::machine::GeneratorSpecs Specs{openfluid::fluidx::GeneratorDescriptor::GeneratorMethod::RANDOM, 
                                             "SU", 
                                             "b", 
                                             openfluid::core::Value::BOOLEAN};
    openfluid::ware::WareParams_t Params = {{"deltat", "0"}};

    TestSimulation TS;
    TS.defaultSetup();
    TS.addGenerator(Specs, Params);
    TS.wholeSimulation();
  }
  {
    //STRING
    openfluid::machine::GeneratorSpecs Specs{openfluid::fluidx::GeneratorDescriptor::GeneratorMethod::RANDOM, 
                                             "SU", 
                                             "b", 
                                             openfluid::core::Value::STRING};
    openfluid::ware::WareParams_t Params = {{"deltat", "0"}};

    TestSimulation TS;
    TS.defaultSetup();
    TS.addGenerator(Specs, Params);
    // Random for str not handled
    BOOST_REQUIRE_THROW(TS.wholeSimulation(), openfluid::base::FrameworkException); 
  }
}


// =====================================================================
// =====================================================================
  
  
BOOST_AUTO_TEST_CASE(check_random_vector)
{
  {
    // TEST VECTOR INTEGER
    openfluid::machine::GeneratorSpecs Specs{openfluid::fluidx::GeneratorDescriptor::GeneratorMethod::RANDOM, 
                                            "SU", 
                                            "a", 
                                             openfluid::core::Value::INTEGER,
                                             openfluid::fluidx::DataDimensions(3)};
    openfluid::ware::WareParams_t Params = {{"deltat", "0"}, {"min", "2"}, {"max","20000"}};

    TestSimulation TS;
    TS.defaultSetup();
    TS.addGenerator(Specs, Params);
    // pair INT / VECTOR not handled
    BOOST_REQUIRE_THROW(TS.wholeSimulation(), openfluid::base::FrameworkException); 
  }
  {
    // TEST VECTOR BOOL
    openfluid::machine::GeneratorSpecs Specs{openfluid::fluidx::GeneratorDescriptor::GeneratorMethod::RANDOM, 
                                            "SU", 
                                            "a", 
                                             openfluid::core::Value::BOOLEAN,
                                             openfluid::fluidx::DataDimensions(3)};
    openfluid::ware::WareParams_t Params = {{"deltat", "0"}};

    TestSimulation TS;
    TS.defaultSetup();
    TS.addGenerator(Specs, Params);
    // pair BOOL / VECTOR not handled
    BOOST_REQUIRE_THROW(TS.wholeSimulation(), openfluid::base::FrameworkException); 
  }

  // VECTOR OF DOUBLE

  openfluid::machine::GeneratorSpecs Specs{openfluid::fluidx::GeneratorDescriptor::GeneratorMethod::RANDOM, 
                                            "SU", 
                                            "a", 
                                             openfluid::core::Value::DOUBLE,
                                             openfluid::fluidx::DataDimensions(3)};
  {
    // TEST VECTOR CELLS NOT ID
    openfluid::ware::WareParams_t Params = {{"deltat", "0"}, {"min", "2"}, {"max","20"}, {"identicalcells", "false"}};

    TestSimulation TS;
    TS.defaultSetup();
    TS.addGenerator(Specs, Params);
    TS.wholeSimulation();
    
    const auto RandomVector = TS.getLatestValue("SU", 1, "a").value()->asVectorValue();
    BOOST_ASSERT(RandomVector.get(0) != RandomVector.get(1));
  }
  {
    // TEST VECTOR CELLS ID
    openfluid::ware::WareParams_t Params = {{"deltat", "0"}, {"min", "2"}, {"max","20"}, {"identicalcells", "true"}};

    TestSimulation TS;
    TS.defaultSetup();
    TS.addGenerator(Specs, Params);
    TS.wholeSimulation();
    
    const auto RandomVector = TS.getLatestValue("SU", 1, "a").value()->asVectorValue();
    BOOST_REQUIRE_CLOSE(RandomVector.get(0), RandomVector.get(1), 0.00001);
  }
}


// =====================================================================
// =====================================================================
  

BOOST_AUTO_TEST_CASE(check_fixed_scalar)
{
  // TEST SCALAR
  {
    //DOUBLE
    openfluid::machine::GeneratorSpecs Specs{openfluid::fluidx::GeneratorDescriptor::GeneratorMethod::FIXED, 
                                            "SU", 
                                            "a"};
    openfluid::ware::WareParams_t Params = {{"deltat", "0"}, {"fixedvalue", "32"}};

    TestSimulation TS;
    TS.defaultSetup();
    TS.addGenerator(Specs, Params);
    TS.wholeSimulation();
    double Value = asDouble(TS.getLatestValue("SU", 1, "a"));
    BOOST_REQUIRE_CLOSE(Value, 32, 0.00001);
  }
  {
    //INTEGER
    openfluid::machine::GeneratorSpecs Specs{openfluid::fluidx::GeneratorDescriptor::GeneratorMethod::FIXED, 
                                             "SU", 
                                             "b", 
                                             openfluid::core::Value::INTEGER};
    openfluid::ware::WareParams_t Params = {{"deltat", "0"}, {"fixedvalue", "2"}};

    TestSimulation TS;
    TS.defaultSetup();
    TS.addGenerator(Specs, Params);
    TS.wholeSimulation();
    int Value = TS.getLatestValue("SU", 1, "b").value()->asIntegerValue().get();
    BOOST_ASSERT(Value == 2);
  }
  {
    //BOOLEAN
    openfluid::machine::GeneratorSpecs Specs{openfluid::fluidx::GeneratorDescriptor::GeneratorMethod::FIXED, 
                                             "SU", 
                                             "b", 
                                             openfluid::core::Value::BOOLEAN};
    openfluid::ware::WareParams_t Params = {{"deltat", "0"}, {"fixedvalue", "true"}};

    TestSimulation TS;
    TS.defaultSetup();
    TS.addGenerator(Specs, Params);
    TS.wholeSimulation();
    BOOST_ASSERT(TS.getLatestValue("SU", 1, "b").value()->asBooleanValue().get() == true);
  }
  {
    //STRING
    openfluid::machine::GeneratorSpecs Specs{openfluid::fluidx::GeneratorDescriptor::GeneratorMethod::FIXED, 
                                             "SU", 
                                             "b", 
                                             openfluid::core::Value::STRING};
    openfluid::ware::WareParams_t Params = {{"deltat", "0"}, {"fixedvalue", "foo bar"}};

    TestSimulation TS;
    TS.defaultSetup();
    TS.addGenerator(Specs, Params);
    TS.wholeSimulation();
    BOOST_ASSERT(TS.getLatestValue("SU", 1, "b").value()->asStringValue().get() == "foo bar");
  }
}


// =====================================================================
// =====================================================================
  
  
BOOST_AUTO_TEST_CASE(check_fixed_vector)
{
  {
    // TEST VECTOR INT
    openfluid::machine::GeneratorSpecs Specs{openfluid::fluidx::GeneratorDescriptor::GeneratorMethod::FIXED, 
                                            "SU", 
                                            "a", 
                                             openfluid::core::Value::INTEGER,
                                             openfluid::fluidx::DataDimensions(3)};
    openfluid::ware::WareParams_t Params = {{"deltat", "0"}, {"fixedvalue", "3"}};

    TestSimulation TS;
    TS.defaultSetup();
    TS.addGenerator(Specs, Params);
    // pair INT / VECTOR not handled
    BOOST_REQUIRE_THROW(TS.wholeSimulation(), openfluid::base::FrameworkException);
  }
  {
    // TEST VECTOR BOOL
    openfluid::machine::GeneratorSpecs Specs{openfluid::fluidx::GeneratorDescriptor::GeneratorMethod::FIXED, 
                                            "SU", 
                                            "a", 
                                             openfluid::core::Value::BOOLEAN,
                                             openfluid::fluidx::DataDimensions(3)};
    openfluid::ware::WareParams_t Params = {{"deltat", "0"}, {"fixedvalue", "true"}};

    TestSimulation TS;
    TS.defaultSetup();
    TS.addGenerator(Specs, Params);
    // pair BOOL / VECTOR not handled
    BOOST_REQUIRE_THROW(TS.wholeSimulation(), openfluid::base::FrameworkException); 
  }
  {
    // TEST VECTOR STRING
    openfluid::machine::GeneratorSpecs Specs{openfluid::fluidx::GeneratorDescriptor::GeneratorMethod::FIXED, 
                                            "SU", 
                                            "a", 
                                             openfluid::core::Value::STRING,
                                             openfluid::fluidx::DataDimensions(3)};
    openfluid::ware::WareParams_t Params = {{"deltat", "0"}, {"fixedvalue", "foo"}};

    TestSimulation TS;
    TS.defaultSetup();
    TS.addGenerator(Specs, Params);
    // pair INT / VECTOR not handled
    BOOST_REQUIRE_THROW(TS.wholeSimulation(), openfluid::base::FrameworkException);
  }

  // VECTOR OF DOUBLE

  openfluid::machine::GeneratorSpecs Specs{openfluid::fluidx::GeneratorDescriptor::GeneratorMethod::FIXED, 
                                            "SU", 
                                            "a", 
                                             openfluid::core::Value::DOUBLE,
                                             openfluid::fluidx::DataDimensions(3)};
  {
    // TEST VECTOR CELLS FROM VAL
    openfluid::ware::WareParams_t Params = {{"deltat", "0"}, {"fixedvalue", "2.4"}};

    TestSimulation TS;
    TS.defaultSetup();
    TS.addGenerator(Specs, Params);
    TS.wholeSimulation();
    
    const auto Vector = TS.getLatestValue("SU", 1, "a").value()->asVectorValue();
    BOOST_REQUIRE_CLOSE(Vector.get(0), 2.4, 0.00001);
    BOOST_REQUIRE_CLOSE(Vector.get(0), Vector.get(1), 0.00001);
  }
  {
    // TEST VECTOR CELLS FROM VECTOR
    openfluid::ware::WareParams_t Params = {{"deltat", "0"}, {"fixedvalue", "[1,5e-2,2.3]"}};

    TestSimulation TS;
    TS.defaultSetup();
    TS.addGenerator(Specs, Params);
    TS.wholeSimulation();
    
    const auto Vector = TS.getLatestValue("SU", 1, "a").value()->asVectorValue();
    BOOST_REQUIRE_CLOSE(Vector.get(0), 1, 0.00001);
    BOOST_REQUIRE_CLOSE(Vector.get(1), 0.05, 0.00001);
  }
  {
    // TEST VECTOR CELLS FROM VECTOR OF WRONG DIM: 3 expected, 2 given
    openfluid::ware::WareParams_t Params = {{"deltat", "0"}, {"fixedvalue", "[1,5e-2]"}};

    TestSimulation TS;
    TS.defaultSetup();
    TS.addGenerator(Specs, Params);
    BOOST_REQUIRE_THROW(TS.wholeSimulation(), openfluid::base::FrameworkException);
  }
}
