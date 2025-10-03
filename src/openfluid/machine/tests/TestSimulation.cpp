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
  @file TestSimulation.cpp

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
                                                                openfluid::core::DateTime(2012,1,1,6,0,0),3660);

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


int asInteger(openfluid::core::IndexedValue IValue)
{
  return IValue.value()->asIntegerValue().get();
}


// =====================================================================
// =====================================================================


bool asBoolean(openfluid::core::IndexedValue IValue)
{
  return IValue.value()->asBooleanValue().get();
}
