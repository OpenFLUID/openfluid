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
#include <openfluid/guicommon/PreferencesManager.hpp>

#include <openfluid/base/ProjectManager.hpp>
#include <openfluid/core/DateTime.hpp>

#include "GeneratorSignature.hpp"

#include <boost/filesystem.hpp>

// =====================================================================
// =====================================================================


EngineProject::EngineProject(Glib::ustring FolderIn, bool WithProjectManager) :
  m_WithProjectManager(WithProjectManager), FXReader(0)
{
  std::string Now = boost::posix_time::to_iso_extended_string(
      boost::posix_time::microsec_clock::local_time());
  Now[10] = ' ';
  m_DefaultBeginDT.setFromISOString(Now);
  m_DefaultDeltaT = openfluid::core::DateTime::Minutes(5);

  mp_SimBlob = new openfluid::machine::SimulationBlob();
  mp_RunEnv = openfluid::base::RuntimeEnvironment::getInstance();
  if (m_WithProjectManager)
    mp_RunEnv->linkToProject();
  mp_IOListener = new openfluid::io::IOListener();
  mp_Listener = new openfluid::guicommon::RunDialogMachineListener();

  mp_ModelInstance = new openfluid::machine::ModelInstance(*mp_SimBlob,
      mp_Listener);

  mp_Engine = new openfluid::machine::Engine(*mp_SimBlob, *mp_ModelInstance,
      mp_Listener, mp_IOListener);

  mp_ModelInstance->resetInitialized();

  if (FolderIn == "")
  {
    setDefaultRunDesc();
    setDefaultOutDesc();
  } else
  {
    try
    {
      FXReader = new openfluid::io::FluidXReader(mp_IOListener);

      FXReader->loadFromDirectory(
          WithProjectManager ? openfluid::base::ProjectManager::getInstance()->getInputDir()
              : FolderIn);

      openfluid::base::RunDescriptor RunDesc = FXReader->getRunDescriptor();
      checkAndSetDefaultRunValues(RunDesc);

      openfluid::base::OutputDescriptor OutDesc =
          FXReader->getOutputDescriptor();
      checkAndSetDefaultOutputValues(OutDesc);

      openfluid::base::ModelDescriptor ModelDesc =
          FXReader->getModelDescriptor();

      std::string MissingFunctions = checkModelDesc(ModelDesc);
      if (MissingFunctions != "")
        openfluid::guicommon::DialogBoxFactory::showSimpleWarningMessage(
            "Unable to find plugin file(s):\n\n" + MissingFunctions);

      openfluid::machine::Factory::buildSimulationBlobFromDescriptors(
          FXReader->getDomainDescriptor(), RunDesc, OutDesc, *mp_SimBlob);

      openfluid::machine::Factory::buildModelInstanceFromDescriptor(ModelDesc,
          *mp_ModelInstance);

      //add specific GeneratorSignature to Generators
      std::list<openfluid::machine::ModelItemInstance*> Items =
          mp_ModelInstance->getItems();
      for (std::list<openfluid::machine::ModelItemInstance*>::iterator it =
          Items.begin(); it != Items.end(); ++it)
      {
        if ((*it)->ItemType == openfluid::base::ModelItemDescriptor::Generator)
        {
          openfluid::base::GeneratorDescriptor::GeneratorMethod
              GeneratorMethod =
                  (static_cast<openfluid::machine::Generator*> ((*it)->Function))->getGeneratorMethod();

          GeneratorSignature* GenSign = new GeneratorSignature(GeneratorMethod);

          GenSign->ID = (*it)->Signature->ID;

          GenSign->HandledData.ProducedVars = (*it)->Signature->HandledData.ProducedVars;

          GenSign->HandledData.RequiredExtraFiles = (*it)->Signature->HandledData.RequiredExtraFiles;

          (*it)->Signature = GenSign;
        }
      }
    } catch (openfluid::base::OFException e)
    {
      openfluid::guicommon::DialogBoxFactory::showSimpleWarningMessage(e.what());
    }
  }

}

// =====================================================================
// =====================================================================


sigc::signal<void> EngineProject::signal_RunHappened()
{
  return m_signal_RunHappened;
}

// =====================================================================
// =====================================================================


sigc::signal<void> EngineProject::signal_SaveHappened()
{
  return m_signal_SaveHappened;
}

// =====================================================================
// =====================================================================


void EngineProject::setDefaultRunDesc()
{
  openfluid::guicommon::PreferencesManager* PrefMgr =
      openfluid::guicommon::PreferencesManager::getInstance();

  openfluid::core::DateTime DefaultBeginDT;
  openfluid::core::DateTime DefaultEndDT;
  int DefaultDeltaT;

  if (PrefMgr->getBegin() != "")
    DefaultBeginDT.setFromISOString(PrefMgr->getBegin());
  else
    DefaultBeginDT = m_DefaultBeginDT;

  if (PrefMgr->getEnd() != "")
    DefaultEndDT.setFromISOString(PrefMgr->getEnd());
  else
    DefaultEndDT = DefaultBeginDT + openfluid::core::DateTime::Day();

  if (PrefMgr->getDeltaT() != -1)
    DefaultDeltaT = PrefMgr->getDeltaT();
  else
    DefaultDeltaT = m_DefaultDeltaT;

  openfluid::base::RunDescriptor RunDesc(DefaultDeltaT, DefaultBeginDT,
      DefaultEndDT);

  if (PrefMgr->getOutFilesBufferInKB() != -1)
    RunDesc.setFilesBufferSizeInKB(PrefMgr->getOutFilesBufferInKB());

  RunDesc.setFilled(true);

  getRunDescriptor() = RunDesc;
}

// =====================================================================
// =====================================================================


void EngineProject::setDefaultOutDesc()
{
  openfluid::base::OutputDescriptor OutDesc;
  openfluid::base::OutputFilesDescriptor FileDesc;

  OutDesc.getFileSets().push_back(FileDesc);

  getOutputDescriptor() = OutDesc;
}

// =====================================================================
// =====================================================================


void EngineProject::checkAndSetDefaultRunValues(
    openfluid::base::RunDescriptor& RunDesc)
{
  openfluid::core::DateTime DT;
  openfluid::core::DateTime BeginDT;

  if (!DT.setFromISOString(RunDesc.getBeginDate().getAsISOString()))
  {
    RunDesc.setBeginDate(m_DefaultBeginDT);

    BeginDT = m_DefaultBeginDT;
  } else
    BeginDT = DT;

  if (!DT.setFromISOString(RunDesc.getEndDate().getAsISOString()))
  {
    openfluid::core::DateTime EndDT = BeginDT
        + openfluid::core::DateTime::Day();

    RunDesc.setEndDate(EndDT);
  }

  if (RunDesc.getDeltaT() < 1)
    RunDesc.setDeltaT(m_DefaultDeltaT);

  RunDesc.setFilled(true);
}

// =====================================================================
// =====================================================================


void EngineProject::checkAndSetDefaultOutputValues(
    openfluid::base::OutputDescriptor& OutDesc)
{
  if (OutDesc.getFileSets().empty())
  {
    openfluid::base::OutputFilesDescriptor FileDesc;
    OutDesc.getFileSets().push_back(FileDesc);
  }
}

// =====================================================================
// =====================================================================


std::string EngineProject::checkModelDesc(
    openfluid::base::ModelDescriptor& ModelDesc)
{
  std::string MissingFunctions = "";

  openfluid::base::ModelDescriptor::ModelDescription_t::iterator it =
      ModelDesc.getItems().begin();

  openfluid::machine::ArrayOfSignatureItemInstance Signatures =
      openfluid::machine::PluginManager::getInstance()->getAvailableFunctions();

  while (it != ModelDesc.getItems().end())
  {
    if ((*it)->isType(openfluid::base::ModelItemDescriptor::PluggedFunction)
        && openfluid::machine::PluginManager::getInstance()->getAvailableFunctions(
            ((openfluid::base::FunctionDescriptor*) (*it))->getFileID()).empty())
    {
      MissingFunctions.append(
          ((openfluid::base::FunctionDescriptor*) (*it))->getFileID() + "\n");

      it = ModelDesc.getItems().erase(it);
    } else
      ++it;
  }

  return MissingFunctions;
}

// =====================================================================
// =====================================================================


void EngineProject::run()
{
  openfluid::base::ProjectManager::getInstance()->updateOutputDir();

  if (m_WithProjectManager)
    mp_RunEnv->linkToProject();

  delete mp_Engine;
  mp_Engine = new openfluid::machine::Engine(*mp_SimBlob, *mp_ModelInstance,
      mp_Listener, mp_IOListener);

  openfluid::base::SimulationProfiler::getInstance()->reset();

  mp_SimBlob->clearSimulationGarbage();

  openfluid::machine::Factory::fillRunEnvironmentFromDescriptor(
      getRunDescriptor());

  openfluid::guicommon::SimulationRunDialog RunDialog(mp_Engine);

  RunDialog.set_modal(true);
  RunDialog.set_title(_("Simulation"));
  Gtk::Main::run(RunDialog);

  mp_ModelInstance->resetInitialized();

  if (RunDialog.isSimulationCompleted())
    m_signal_RunHappened.emit();

}

// =====================================================================
// =====================================================================


void EngineProject::save()
{
  std::string InputDir = getRunEnv()->getInputDir();

  openfluid::base::ProjectManager::getInstance()->save();

  openfluid::io::FluidXWriter Writer(getIOListener());

  Writer.setDomainToWrite(getCoreRepository());
  Writer.setModelToWrite(*getModelInstance());
  Writer.setRunConfigurationToWrite(getRunDescriptor());
  Writer.setOutputConfigurationToWrite(getOutputDescriptor());

  boost::filesystem::path InputPath(InputDir);

  boost::filesystem::directory_iterator end_it;
  for (boost::filesystem::directory_iterator it(InputPath); it != end_it; ++it)
  {
    if ((boost::filesystem::extension(it->path()) == ".fluidx"))
      boost::filesystem::remove(it->path());
  }

  Writer.WriteToManyFiles(InputDir);

  m_signal_SaveHappened.emit();
}

// =====================================================================
// =====================================================================


void EngineProject::check(
    openfluid::machine::Engine::PretestInfos_t& PretestInfos)
{
  mp_Engine->pretestConsistency(PretestInfos);
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
  //don't delete mp_RunEnv, which is a singleton
  if (m_WithProjectManager)
    getRunEnv()->detachFromProject();
  delete mp_Engine;
  if (FXReader)
    delete FXReader;
}

