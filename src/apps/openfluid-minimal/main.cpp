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
  @file main.cpp

  @author Jean-Christophe FABRE <jean-christophe.fabre@supagro.inra.fr>
*/

#include <openfluid/base/IOListener.hpp>
#include <openfluid/fluidx/FluidXDescriptor.hpp>
#include <openfluid/base/RuntimeEnv.hpp>
#include <openfluid/base/Init.hpp>
#include <openfluid/base/ApplicationException.hpp>
#include <openfluid/machine/Engine.hpp>
#include <openfluid/machine/SimulationBlob.hpp>
#include <openfluid/machine/MachineListener.hpp>
#include <openfluid/machine/ModelInstance.hpp>
#include <openfluid/machine/MonitoringInstance.hpp>
#include <openfluid/machine/Factory.hpp>



int main(int argc, char **argv)
{
  INIT_OPENFLUID_APPLICATION(argc,argv);

  try
  {
    std::string InputDir = "";
    std::string OutputDir = "";
    std::string PlugsDir = "";

    if (argc == 4)
    {
      InputDir = std::string(argv[1]);
      OutputDir = std::string(argv[2]);
      PlugsDir = std::string(argv[3]);
    }
    else
    {
      openfluid::base::ExceptionContext Context =
          openfluid::base::ApplicationException::computeContext("openfluid-minimal",
                                                                OPENFLUID_CODE_LOCATION);

      throw openfluid::base::ApplicationException(Context,
                                                  "Incorrect number of arguments, "
                                                  "should be <inputdir> <outputdir> <pluginsdirs>");
    }
    openfluid::machine::Engine* Engine;
    openfluid::machine::SimulationBlob SBlob;
    openfluid::base::RuntimeEnvironment* RunEnv;
    openfluid::base::IOListener* IOListen = new openfluid::base::IOListener();
    openfluid::machine::MachineListener* MachineListen = new openfluid::machine::MachineListener();
    openfluid::machine::ModelInstance Model(SBlob,MachineListen);
    openfluid::machine::MonitoringInstance Monitoring(SBlob);
    openfluid::fluidx::FluidXDescriptor FXDesc(IOListen);


    RunEnv = openfluid::base::RuntimeEnvironment::instance();
    RunEnv->setInputDir(InputDir);
    RunEnv->setOutputDir(OutputDir);
    RunEnv->addExtraSimulatorsPluginsPaths(PlugsDir);
    RunEnv->addExtraObserversPluginsPaths(PlugsDir);


    FXDesc.loadFromDirectory(InputDir);


    openfluid::machine::Factory::buildSimulationBlobFromDescriptors(FXDesc,SBlob);

    openfluid::machine::Factory::buildModelInstanceFromDescriptor(FXDesc.modelDescriptor(),
                                                                  Model);

    openfluid::machine::Factory::buildMonitoringInstanceFromDescriptor(FXDesc.monitoringDescriptor(),
                                                                  Monitoring);

    Engine = new openfluid::machine::Engine(SBlob, Model, Monitoring, MachineListen);

    Engine->initialize();

    Engine->initParams();
    Engine->prepareData();
    Engine->checkConsistency();
    Engine->run();

    Engine->finalize();

    delete Engine;

    return 0;
  }
  catch (openfluid::base::Exception& E)
  {
    std::cerr << "ERROR: " + std::string(E.what()) << std::endl;
  }
  catch (std::bad_alloc& E)
  {
    std::cerr << "MEMORY ALLOCATION ERROR: " +
                 std::string(E.what()) + ". Possibly not enough memory available" << std::endl;
  }
  catch (std::exception& E)
  {
    std::cerr << "SYSTEM ERROR: " + std::string(E.what()) << std::endl;
  }
  catch (...)
  {
    std::cerr << "UNKNOWN ERROR" << std::endl;
  }

  return 127;
}

// =====================================================================
// =====================================================================
