/*

  This file is part of OpenFLUID software
  Copyright(c) 2021-2026, INRAE


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


BOOST_AUTO_TEST_CASE(check_seeded_random_generator)
{
  {
    //INVALID SEED VALUE
    std::cout << "Checking invalid seed value for random generator" << std::endl;

    openfluid::machine::GeneratorSpecs Specs{openfluid::fluidx::GeneratorDescriptor::GeneratorMethod::RANDOM, 
                                            {{"SU","a"}}};
    openfluid::machine::GeneratorSpecs Specs2{openfluid::fluidx::GeneratorDescriptor::GeneratorMethod::RANDOM, 
                                            {{"SU","b"}}};
                                            
    openfluid::ware::WareParams_t Params = {{"deltat", "0"}, {"min", "2"}, {"max","20"}, {"seed", "text"}};

    TestSimulation TS;
    TS.defaultSetup();
    TS.addGenerator(Specs, Params);
    BOOST_REQUIRE_THROW(TS.wholeSimulation(), openfluid::base::FrameworkException); 
  }

  {
    //NEGATIVE SEED
    std::cout << "Checking negative seed for random generator" << std::endl;

    openfluid::machine::GeneratorSpecs Specs{openfluid::fluidx::GeneratorDescriptor::GeneratorMethod::RANDOM, 
                                            {{"SU","a"}}};
                                            
    openfluid::ware::WareParams_t Params = {{"deltat", "0"}, {"min", "2"}, {"max","20"}, {"seed", "-10"}};
    openfluid::ware::WareParams_t Params2 = {{"deltat", "0"}, {"min", "2"}, {"max","20"}, {"seed", "1"}};

    TestSimulation TS;
    TS.defaultSetup();
    TS.addGenerator(Specs, Params);
    TS.wholeSimulation();

    TestSimulation TS2;
    TS2.defaultSetup();
    TS2.addGenerator(Specs, Params2);
    TS2.wholeSimulation();

    BOOST_ASSERT(asDouble(TS.getLatestValue("SU", 1, "a")) != asDouble(TS2.getLatestValue("SU", 1, "a")));
  }

  {
    //DIFFERENT SEED
    std::cout << "Checking different seeds for random generator" << std::endl;

    openfluid::machine::GeneratorSpecs Specs{openfluid::fluidx::GeneratorDescriptor::GeneratorMethod::RANDOM, 
                                            {{"SU","a"}}};
                                            
    openfluid::ware::WareParams_t Params = {{"deltat", "0"}, {"min", "2"}, {"max","20"}, {"seed", "25"}};
    openfluid::ware::WareParams_t Params2 = {{"deltat", "0"}, {"min", "2"}, {"max","20"}, {"seed", "50"}};

    TestSimulation TS;
    TS.defaultSetup();
    TS.addGenerator(Specs, Params);
    TS.wholeSimulation();

    TestSimulation TS2;
    TS2.defaultSetup();
    TS2.addGenerator(Specs, Params2);
    TS2.wholeSimulation();

    BOOST_ASSERT(asDouble(TS.getLatestValue("SU", 1, "a")) != asDouble(TS2.getLatestValue("SU", 1, "a")));
  }

  {
    //DOUBLE
    std::cout << "Checking double seeded random generator" << std::endl;

    openfluid::machine::GeneratorSpecs Specs{openfluid::fluidx::GeneratorDescriptor::GeneratorMethod::RANDOM, 
                                            {{"SU","a"}}};
                                            
    openfluid::ware::WareParams_t Params = {{"deltat", "0"}, {"min", "2"}, {"max","20"}, {"seed", "1"}};

    TestSimulation TS;
    TS.defaultSetup();
    TS.addGenerator(Specs, Params);
    TS.wholeSimulation();

    TestSimulation TS2;
    TS2.defaultSetup();
    TS2.addGenerator(Specs, Params);
    TS2.wholeSimulation();

    BOOST_ASSERT(asDouble(TS.getLatestValue("SU", 1, "a")) == asDouble(TS2.getLatestValue("SU", 1, "a")));
  }

  {
    //INT
    std::cout << "Checking integer seeded random generator" << std::endl;

    openfluid::machine::GeneratorSpecs Specs{openfluid::fluidx::GeneratorDescriptor::GeneratorMethod::RANDOM, 
                                            {{"SU","a"}},
                                            openfluid::core::Value::INTEGER};
                                            
    openfluid::ware::WareParams_t Params = {{"deltat", "0"}, {"min", "2"}, {"max","200"}, {"seed", "10"}};

    TestSimulation TS;
    TS.defaultSetup();
    TS.addGenerator(Specs, Params);
    TS.wholeSimulation();

    TestSimulation TS2;
    TS2.defaultSetup();
    TS2.addGenerator(Specs, Params);
    TS2.wholeSimulation();

    BOOST_ASSERT(asInteger(TS.getLatestValue("SU", 1, "a")) == asInteger(TS2.getLatestValue("SU", 1, "a")));
  }

  {
    //BOOLEAN
    std::cout << "Checking boolean seeded random generator" << std::endl;
    
    openfluid::machine::GeneratorSpecs Specs{openfluid::fluidx::GeneratorDescriptor::GeneratorMethod::RANDOM, 
                                             {{"SU","b"}},
                                             openfluid::core::Value::BOOLEAN};
    openfluid::ware::WareParams_t Params = {{"deltat", "0"}, {"seed", "100"}};

    TestSimulation TS;
    TS.defaultSetup();
    TS.addGenerator(Specs, Params);
    TS.wholeSimulation();

    TestSimulation TS2;
    TS2.defaultSetup();
    TS2.addGenerator(Specs, Params);
    TS2.wholeSimulation();

    BOOST_ASSERT(asBoolean(TS.getLatestValue("SU", 1, "b")) == asBoolean(TS2.getLatestValue("SU", 1, "b")));
  }
}


// =====================================================================
// =====================================================================


BOOST_AUTO_TEST_CASE(check_random_scalar)
{
  // TEST SCALAR RERUN NOT IDENTICAL
  {
    //DOUBLE
    std::cout << "Checking double not seeded random generator" << std::endl;
    openfluid::machine::GeneratorSpecs Specs{openfluid::fluidx::GeneratorDescriptor::GeneratorMethod::RANDOM, 
                                            {{"SU","a"}}};
                                            
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

  // TEST 2 GENERATORS RUN NOT IDENTICAL
  {
    //DOUBLE
    std::cout << "Checking double not seeded with 2 random generators" << std::endl;
    openfluid::machine::GeneratorSpecs Specs{openfluid::fluidx::GeneratorDescriptor::GeneratorMethod::RANDOM, 
                                            {{"SU","a"}}};
    openfluid::machine::GeneratorSpecs Specs2{openfluid::fluidx::GeneratorDescriptor::GeneratorMethod::RANDOM, 
                                            {{"SU","b"}}};
                                            
    openfluid::ware::WareParams_t Params = {{"deltat", "0"}, {"min", "2"}, {"max","20"}};

    TestSimulation TS;
    TS.defaultSetup();
    TS.addGenerator(Specs, Params);
    TS.addGenerator(Specs2, Params);
    TS.wholeSimulation();
    double RandomValue = asDouble(TS.getLatestValue("SU", 1, "a"));
    BOOST_ASSERT(RandomValue >= 2 && RandomValue <= 20);
    BOOST_ASSERT(RandomValue != asDouble(TS.getLatestValue("SU", 1, "b")));
  }

  {
    //INT
    std::cout << "Checking integer not seeded random generator" << std::endl;
    openfluid::machine::GeneratorSpecs Specs{openfluid::fluidx::GeneratorDescriptor::GeneratorMethod::RANDOM, 
                                             {{"SU","b"}},
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
    std::cout << "Checking boolean not seeded random generator" << std::endl;
    openfluid::machine::GeneratorSpecs Specs{openfluid::fluidx::GeneratorDescriptor::GeneratorMethod::RANDOM, 
                                             {{"SU","b"}},
                                             openfluid::core::Value::BOOLEAN};
    openfluid::ware::WareParams_t Params = {{"deltat", "0"}};

    TestSimulation TS;
    TS.defaultSetup();
    TS.addGenerator(Specs, Params);
    TS.wholeSimulation();
  }
  {
    //STRING
    std::cout << "Checking string not seeded random generator" << std::endl;
    openfluid::machine::GeneratorSpecs Specs{openfluid::fluidx::GeneratorDescriptor::GeneratorMethod::RANDOM, 
                                             {{"SU","b"}},
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
    std::cout << "Checking VECTOR INTEGER" << std::endl;
    openfluid::machine::GeneratorSpecs Specs{openfluid::fluidx::GeneratorDescriptor::GeneratorMethod::RANDOM, 
                                            {{"SU","a"}}, 
                                             openfluid::core::Value::INTEGER};
    openfluid::ware::WareParams_t Params = {{"deltat", "0"}, {"min", "2"}, {"max","20000"}, {"varsize","3"}};

    TestSimulation TS;
    TS.defaultSetup();
    // pair INT / VECTOR not handled
    TS.addGenerator(Specs, Params); 
    BOOST_REQUIRE_THROW(TS.wholeSimulation(), openfluid::base::FrameworkException); 
    
  }
  {
    // TEST VECTOR BOOL
    std::cout << "Checking VECTOR BOOL" << std::endl;
    openfluid::machine::GeneratorSpecs Specs{openfluid::fluidx::GeneratorDescriptor::GeneratorMethod::RANDOM, 
                                             {{"SU","a"}}, 
                                             openfluid::core::Value::BOOLEAN};
    openfluid::ware::WareParams_t Params = {{"deltat", "0"}, {"varsize","3"}};

    TestSimulation TS;
    TS.defaultSetup();
    // pair BOOL / VECTOR not handled
    TS.addGenerator(Specs, Params); 
    BOOST_REQUIRE_THROW(TS.wholeSimulation(), openfluid::base::FrameworkException); 
  }

  // VECTOR OF DOUBLE

  openfluid::machine::GeneratorSpecs Specs{openfluid::fluidx::GeneratorDescriptor::GeneratorMethod::RANDOM, 
                                            {{"SU","a"}}, 
                                             openfluid::core::Value::VECTOR};
  {
    // TEST VECTOR CELLS NOT ID
    std::cout << "Checking VECTOR CELLS NOT ID" << std::endl;
    openfluid::ware::WareParams_t Params = {{"deltat", "0"}, {"min", "2"}, {"max","20"}, {"identicalcells", "false"}, 
                                            {"varsize","3"}};

    TestSimulation TS;
    TS.defaultSetup();
    TS.addGenerator(Specs, Params);
    TS.wholeSimulation();
    
    const auto RandomVector = TS.getLatestValue("SU", 1, "a").value()->asVectorValue();
    BOOST_ASSERT(RandomVector.get(0) != RandomVector.get(1));
  }
  {
    // TEST VECTOR CELLS ID
    std::cout << "Checking VECTOR CELLS ID" << std::endl;
    openfluid::ware::WareParams_t Params = {{"deltat", "0"}, {"min", "2"}, {"max","20"}, {"identicalcells", "true"}, 
                                            {"varsize","3"}};

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
                                            {{"SU","a"}}};
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
                                             {{"SU","b"}}, 
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
                                             {{"SU","b"}}, 
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
                                             {{"SU","b"}}, 
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
                                             {{"SU","a"}}, 
                                             openfluid::core::Value::INTEGER};
    openfluid::ware::WareParams_t Params = {{"deltat", "0"}, {"fixedvalue", "3"}, {"varsize","3"}};

    TestSimulation TS;
    TS.defaultSetup();
    // pair INT / VECTOR not handled
    TS.addGenerator(Specs, Params);
    BOOST_REQUIRE_THROW(TS.wholeSimulation(), openfluid::base::FrameworkException); 
  }
  {
    // TEST VECTOR BOOL
    openfluid::machine::GeneratorSpecs Specs{openfluid::fluidx::GeneratorDescriptor::GeneratorMethod::FIXED, 
                                             {{"SU","a"}}, 
                                             openfluid::core::Value::BOOLEAN};
    openfluid::ware::WareParams_t Params = {{"deltat", "0"}, {"fixedvalue", "true"}, {"varsize","3"}};

    TestSimulation TS;
    TS.defaultSetup();
    // pair BOOL / VECTOR not handled
    TS.addGenerator(Specs, Params); 
    BOOST_REQUIRE_THROW(TS.wholeSimulation(), openfluid::base::FrameworkException); 
  }
  {
    // TEST VECTOR STRING
    openfluid::machine::GeneratorSpecs Specs{openfluid::fluidx::GeneratorDescriptor::GeneratorMethod::FIXED, 
                                             {{"SU","a"}}, 
                                             openfluid::core::Value::STRING};
    openfluid::ware::WareParams_t Params = {{"deltat", "0"}, {"fixedvalue", "foo"}, {"varsize","3"}};

    TestSimulation TS;
    TS.defaultSetup();
    // pair STRING / VECTOR not handled
    TS.addGenerator(Specs, Params); 
    BOOST_REQUIRE_THROW(TS.wholeSimulation(), openfluid::base::FrameworkException); 
  }
  
  // OPENFLUID VECTOR

  openfluid::machine::GeneratorSpecs SpecsVector{openfluid::fluidx::GeneratorDescriptor::GeneratorMethod::FIXED, 
                                            {{"SU","a"}}, 
                                             openfluid::core::Value::VECTOR};
  {
    // TEST VECTOR CELLS FROM VAL
    std::cout << "checking TEST VECTOR CELLS FROM VAL" << std::endl;
    openfluid::ware::WareParams_t Params = {{"deltat", "0"}, {"fixedvalue", "2.4"}, {"varsize","3"}};

    TestSimulation TS;
    TS.defaultSetup();
    TS.addGenerator(SpecsVector, Params);
    TS.wholeSimulation(); 
    
    const auto Vector = TS.getLatestValue("SU", 1, "a").value()->asVectorValue();
    BOOST_REQUIRE_CLOSE(Vector.get(0), 2.4, 0.00001);
    BOOST_REQUIRE_CLOSE(Vector.get(0), Vector.get(1), 0.00001);
  }
  {
    // TEST VECTOR CELLS FROM VECTOR
    std::cout << "checking TEST VECTOR CELLS FROM VECTOR" << std::endl;
    openfluid::ware::WareParams_t Params = {{"deltat", "0"}, {"fixedvalue", "[1,5e-2,2.3]"}, {"varsize","3"}};

    TestSimulation TS;
    TS.defaultSetup();
    TS.addGenerator(SpecsVector, Params);
    TS.wholeSimulation();
    
    const auto Vector = TS.getLatestValue("SU", 1, "a").value()->asVectorValue();
    BOOST_REQUIRE_CLOSE(Vector.get(0), 1, 0.00001);
    BOOST_REQUIRE_CLOSE(Vector.get(1), 0.05, 0.00001);
  }
  {
    // TEST VECTOR CELLS FROM VECTOR OF WRONG DIM: 3 expected, 2 given
    std::cout << "checking TEST VECTOR CELLS FROM VECTOR OF WRONG DIM: 3 expected, 2 given" << std::endl;
    openfluid::ware::WareParams_t Params = {{"deltat", "0"}, {"fixedvalue", "[1,5e-2]"}, {"varsize","3"}};

    TestSimulation TS;
    TS.defaultSetup();
    TS.addGenerator(SpecsVector, Params);
    BOOST_REQUIRE_THROW(TS.wholeSimulation(), openfluid::base::FrameworkException);
  }
}
