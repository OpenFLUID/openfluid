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
 \file EngineProject.cpp
 \brief Implements ...

 \author Aline LIBRES <libres@supagro.inra.fr>
 */

#include "EngineProject.hpp"

#include <openfluid/guicommon/SimulationRunDialog.hpp>
#include <openfluid/guicommon/DialogBoxFactory.hpp>

// =====================================================================
// =====================================================================


EngineProject::EngineProject()
{
  mp_SimBlob = new openfluid::machine::SimulationBlob();
  mp_RunEnv = openfluid::base::RuntimeEnvironment::getInstance();
  mp_IOListener = new openfluid::io::IOListener();
  mp_Listener = new openfluid::guicommon::RunDialogMachineListener();

  mp_ModelInstance = new openfluid::machine::ModelInstance(*mp_SimBlob,
      mp_Listener);

  mp_Engine = new openfluid::machine::Engine(*mp_SimBlob, *mp_ModelInstance,
      mp_Listener, mp_IOListener);

  mp_ModelInstance->resetInitialized();
}

// =====================================================================
// =====================================================================


void EngineProject::run()
{
  mp_SimBlob->clearSimulationGarbage();

  mp_ModelInstance->initialize();

  openfluid::guicommon::SimulationRunDialog RunDialog(mp_Engine);

  RunDialog.set_modal(true);
  RunDialog.set_title(_("Simulation"));
  Gtk::Main::run(RunDialog);

  mp_ModelInstance->resetInitialized();
}

// =====================================================================
// =====================================================================


openfluid::machine::SimulationBlob* EngineProject::getSimBlob()
{
  return mp_SimBlob;
}

// =====================================================================
// =====================================================================


openfluid::base::RuntimeEnvironment* EngineProject::getRunEnv()
{
  return mp_RunEnv;
}

// =====================================================================
// =====================================================================


openfluid::io::IOListener* EngineProject::getIOListener()
{
  return mp_IOListener;
}

// =====================================================================
// =====================================================================


openfluid::machine::MachineListener* EngineProject::getMachineListener()
{
  return mp_Listener;
}

// =====================================================================
// =====================================================================


openfluid::machine::ModelInstance* EngineProject::getModelInstance()
{
  return mp_ModelInstance;
}

// =====================================================================
// =====================================================================


openfluid::core::CoreRepository& EngineProject::getCoreRepository()
{
  return mp_SimBlob->getCoreRepository();
}

// =====================================================================
// =====================================================================


openfluid::base::ExecutionMessages& EngineProject::getExecutionMessages()
{
  return mp_SimBlob->getExecutionMessages();
}

// =====================================================================
// =====================================================================


openfluid::base::RunDescriptor& EngineProject::getRunDescriptor()
{
  return mp_SimBlob->getRunDescriptor();
}

// =====================================================================
// =====================================================================


openfluid::base::OutputDescriptor& EngineProject::getOutputDescriptor()
{
  return mp_SimBlob->getOutputDescriptor();
}

// =====================================================================
// =====================================================================


EngineProject::~EngineProject()
{
  //mp_ModelInstance->clear() crashes
  for (unsigned int i = 0; i < mp_ModelInstance->getItemsCount(); i++)
    mp_ModelInstance->deleteItem(0);
  delete mp_ModelInstance;
  delete mp_Listener;
  delete mp_IOListener;
  delete mp_SimBlob;
  //do not delete mp_RunEnv, which is a singleton
  delete mp_Engine;
}

// =====================================================================
// =====================================================================

// =====================================================================
// =====================================================================


EngineProjectEmpty::EngineProjectEmpty()
{
  openfluid::base::RunDescriptor RunDesc(120, openfluid::core::DateTime(2000,
      01, 01, 00, 00, 00), openfluid::core::DateTime(2000, 01, 01, 07, 00, 00));
  RunDesc.setFilled(true);

  getRunDescriptor() = RunDesc;
}

// =====================================================================
// =====================================================================

// =====================================================================
// =====================================================================


EngineProjectFromFolder::EngineProjectFromFolder(std::string FolderIn)
{
  getRunEnv()->setInputDir(FolderIn);
  //TODO: set OUT folder from Preferences
  //  getRunEnv()->setOutputDir(Glib::ustring::compose("%1/OUT", FolderIn));
  try
  {

    //      std::cout << "* Loading data... " << std::endl;
    openfluid::io::FluidXReader FXReader(mp_IOListener);
    FXReader.loadFromDirectory(FolderIn);

    //      std::cout << "* Building  Blob... " << std::endl;
    openfluid::machine::Factory::buildSimulationBlobFromDescriptors(
        FXReader.getDomainDescriptor(), FXReader.getRunDescriptor(),
        FXReader.getOutputDescriptor(), *mp_SimBlob);

    //      std::cout << "* Building Model... " << std::endl;
    openfluid::machine::Factory::buildModelInstanceFromDescriptor(
        FXReader.getModelDescriptor(), *mp_ModelInstance);
  } catch (openfluid::base::OFException e)
  {
    openfluid::guicommon::DialogBoxFactory::showSimpleWarningMessage(e.what());
  }
}
