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

#include <openfluid/base/ProjectManager.hpp>
#include <openfluid/core/DateTime.hpp>

#include <boost/filesystem.hpp>

// =====================================================================
// =====================================================================


EngineProject::EngineProject(std::string FolderIn, bool WithProjectManager) :
  m_WithProjectManager(WithProjectManager), m_DefaultBeginDT(
      openfluid::core::DateTime(2000, 01, 01, 00, 00, 00)), m_DefaultEndDT(
      openfluid::core::DateTime(2000, 01, 01, 07, 00, 00)),
      m_DefaultDeltaT(120)
{
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
    openfluid::base::RunDescriptor RunDesc(m_DefaultDeltaT, m_DefaultBeginDT,
        m_DefaultEndDT);
    RunDesc.setFilled(true);

    getRunDescriptor() = RunDesc;
  } else
  {
    openfluid::io::FluidXReader FXReader(mp_IOListener);

    try
    {
      FXReader.loadFromDirectory(
          WithProjectManager ? openfluid::base::ProjectManager::getInstance()->getInputDir()
              : FolderIn);
    } catch (openfluid::base::OFException e)
    {
      openfluid::guicommon::DialogBoxFactory::showSimpleWarningMessage(e.what());
    }

    openfluid::base::RunDescriptor RunDesc = FXReader.getRunDescriptor();

    checkAndSetDefaultRunValues(RunDesc);

    try
    {
      openfluid::machine::Factory::buildSimulationBlobFromDescriptors(
          FXReader.getDomainDescriptor(), RunDesc,
          FXReader.getOutputDescriptor(), *mp_SimBlob);

      openfluid::machine::Factory::buildModelInstanceFromDescriptor(
          FXReader.getModelDescriptor(), *mp_ModelInstance);
    } catch (openfluid::base::OFException e)
    {
      openfluid::guicommon::DialogBoxFactory::showSimpleWarningMessage(e.what());
    }
  }
}

// =====================================================================
// =====================================================================


void EngineProject::checkAndSetDefaultRunValues(
    openfluid::base::RunDescriptor& RunDesc)
{
  openfluid::core::DateTime DT;

  std::string Msg = "";

  if (!DT.setFromISOString(RunDesc.getBeginDate().getAsISOString()))
  {
    Msg.append(
        Glib::ustring::compose(
            _("Wrong or undefined simulation begin date, it has been replaced by the default one:\n%1\n\n"),
            m_DefaultBeginDT.getAsISOString()));

    RunDesc.setBeginDate(m_DefaultBeginDT);
  }
  if (!DT.setFromISOString(RunDesc.getEndDate().getAsISOString()))
  {
    Msg.append(
        Glib::ustring::compose(
            _("Wrong or undefined simulation end date, it has been replaced by the default one:\n%1\n\n"),
            m_DefaultEndDT.getAsISOString()));

    RunDesc.setEndDate(m_DefaultEndDT);
  }
  if (RunDesc.getDeltaT() < 1)
  {
    Msg.append(
        Glib::ustring::compose(
            _("Wrong or undefined simulation deltat T, it has been replaced by the default one:\n%1\n"),
            m_DefaultDeltaT));

    RunDesc.setDeltaT(m_DefaultDeltaT);
  }

  if (Msg != "")
  {
    openfluid::guicommon::DialogBoxFactory::showSimpleWarningMessage(Msg);
    RunDesc.setFilled(true);
  }
}

// =====================================================================
// =====================================================================


void EngineProject::run()
{
  openfluid::base::ProjectManager::getInstance()->updateOutputDir();

  mp_SimBlob->clearSimulationGarbage();

  mp_ModelInstance->initialize();

  openfluid::machine::Factory::fillRunEnvironmentFromDescriptor(
      getRunDescriptor());

  openfluid::guicommon::SimulationRunDialog RunDialog(mp_Engine);

  RunDialog.set_modal(true);
  RunDialog.set_title(_("Simulation"));
  Gtk::Main::run(RunDialog);

  mp_ModelInstance->resetInitialized();
}

// =====================================================================
// =====================================================================


void EngineProject::save()
{
  openfluid::base::ProjectManager::getInstance()->save();

  openfluid::io::FluidXWriter Writer(getIOListener());

  Writer.setDomainToWrite(getCoreRepository());
  Writer.setModelToWrite(*getModelInstance());
  Writer.setRunConfigurationToWrite(getRunDescriptor());
  Writer.setOutputConfigurationToWrite(getOutputDescriptor());

  Writer.WriteToManyFiles(getRunEnv()->getInputDir());
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
}

