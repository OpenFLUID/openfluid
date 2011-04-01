/*
  This file is part of OpenFLUID-engine software
  Copyright (c) 2007-2010 INRA-Montpellier SupAgro

 == GNU General Public License Usage ==

  OpenFLUID-engine is free software: you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation, either version 3 of the License, or
  (at your option) any later version.

  OpenFLUID-engine is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with OpenFLUID-engine.  If not, see <http://www.gnu.org/licenses/>.

  In addition, as a special exception, INRA gives You the additional right
  to dynamically link the code of OpenFLUID-engine with code not covered 
  under the GNU General Public License ("Non-GPL Code") and to distribute
  linked combinations including the two, subject to the limitations in this
  paragraph. Non-GPL Code permitted under this exception must only link to
  the code of OpenFLUID-engine dynamically through the ofelib library
  interface, and only for building OpenFLUID-engine plugins. The files of
  Non-GPL Code may be link to the ofelib library without causing the
  resulting work to be covered by the GNU General Public License. You must
  obey the GNU General Public License in all respects for all of the
  OpenFLUID-engine code and other code used in conjunction with
  OpenFLUID-engine except the Non-GPL Code covered by this exception. If
  you modify this OpenFLUID-engine, you may extend this exception to your
  version of the file, but you are not obligated to do so. If you do not
  wish to provide this exception without modification, you must delete this
  exception statement from your version and license this OpenFLUID-engine
  solely under the GPL without exception.


 == Other Usage ==

  Other Usage means a use of OpenFLUID-Engine that is inconsistent with
  the GPL license, and requires a written agreement between You and INRA.
  Licensees for Other Usage of OpenFLUID-engine may use this file in
  accordance with the terms contained in the written agreement between
  You and INRA.
 */


/**
  \file main.cpp
  \brief Implements ...

  \author Jean-Christophe FABRE <fabrejc@supagro.inra.fr>
 */

#include <gtkmm.h>

#include "SimulationRunDialog.hpp"


int main(int argc, char *argv[])
{
  openfluid::base::Init();

  openfluid::machine::SimulationBlob SBlob;
  openfluid::base::RuntimeEnvironment* RunEnv;
  RunDialogMachineListener* MachineListen = new RunDialogMachineListener();
  openfluid::io::IOListener* IOListen = new openfluid::io::IOListener();
  openfluid::machine::ModelInstance Model(SBlob,MachineListen);
  openfluid::io::FluidXReader FXReader(IOListen);


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
      throw openfluid::base::OFException("openfluid-minimal","Incorrect number of arguments, should be <inputdir> <outputdir> <pluginsdir>");


    RunEnv = openfluid::base::RuntimeEnvironment::getInstance();
    RunEnv->setInputDir(InputDir);
    RunEnv->setOutputDir(OutputDir);
    RunEnv->addExtraPluginsPaths(PlugsDir);


    FXReader.loadFromDirectory(InputDir);


    openfluid::machine::Factory::buildSimulationBlobFromDescriptors(FXReader.getDomainDescriptor(),
        FXReader.getRunDescriptor(),
        FXReader.getOutputDescriptor(),
        SBlob);

    openfluid::machine::Factory::buildModelInstanceFromDescriptor(FXReader.getModelDescriptor(),
        Model);

  }
  catch (openfluid::base::OFException& E)
  {
    std::cerr << "ERROR: " + std::string(E.what()) << std::endl;
  }
  catch (std::bad_alloc& E)
  {
    std::cerr << "MEMORY ALLOCATION ERROR: " + std::string(E.what()) + ". Possibly not enough memory available" << std::endl;
  }
  catch (std::exception& E)
  {
    std::cerr << "SYSTEM ERROR: " + std::string(E.what()) << std::endl;
  }
  catch (...)
  {
    std::cerr << "UNKNOWN ERROR" << std::endl;
  }


  Gtk::Main kit(argc, argv);
  SimulationRunDialog RunDialog(&SBlob, &Model, MachineListen, IOListen);

  Gtk::Main::run(RunDialog);


  return 0;
}

