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

#include <boost/foreach.hpp>
#include <glibmm/i18n.h>

#include <openfluid/guicommon/SimulationRunDialog.hpp>
#include <openfluid/guicommon/DialogBoxFactory.hpp>

// =====================================================================
// =====================================================================


EngineProject::EngineProject()
{
  //      openfluid::base::Init();

  mp_SimBlob = new openfluid::machine::SimulationBlob();
  mp_RunEnv = openfluid::base::RuntimeEnvironment::getInstance();
  mp_IOListener = new openfluid::io::IOListener();
  //  mp_Listener = new BuilderMachineListener();
  mp_Listener = new openfluid::guicommon::RunDialogMachineListener();

  mp_ModelInstance = new openfluid::machine::ModelInstance(*mp_SimBlob,
      mp_Listener);

  //  mp_RunDialog = new BuilderRunDialog();

  ///// CREATE ENGINE
  mp_Engine = new openfluid::machine::Engine(*mp_SimBlob, *mp_ModelInstance,
      mp_Listener, mp_IOListener);
  mp_ModelInstance->resetInitialized();
}

// =====================================================================
// =====================================================================


void EngineProject::run()
{
  getCoreRepository().clearAllVariables();

  mp_ModelInstance->initialize();

  openfluid::guicommon::SimulationRunDialog RunDialog(mp_Engine);

  RunDialog.set_modal(true);
  RunDialog.set_title(_("Simulation"));
  Gtk::Main::run(RunDialog);

  mp_ModelInstance->resetInitialized();

  //  if (check())
  //  {
  //    mp_Listener->linkWithTextView(mp_RunDialog->getTextView());
  //    mp_RunDialog->show();
  //
  //    try
  //    {
  //      mp_ModelInstance->initialize();
  //      m_FullStartTime = getNow();
  //
  //      mp_Listener->displayText(generateSimulationIdStr());
  //      mp_Listener->displayText(generateDomainInformationStr());
  //      mp_Listener->displayText(generateSimulationInformationStr());
  //      mp_Listener->displayText(generateBuffersInformationStr());
  //
  //      mp_Listener->displayText(Glib::ustring::compose(
  //          _("%1Running simulation%2\n"), "**** ", " ****"));
  //
  //      m_EffectiveStartTime = getNow();
  //      mp_Engine->run();
  //      m_EffectiveEndTime = getNow();
  //
  //      mp_Listener->displayText(Glib::ustring::compose(
  //          _("%1Simulation completed%2\n\n"), "**** ", " ****"));
  //
  //      getExecutionMessages().resetWarningFlag();
  //      getRunEnv()->setEffectiveSimulationDuration(m_EffectiveEndTime
  //          - m_EffectiveStartTime);
  //
  //      if (getRunEnv()->isWriteSimReport())
  //      {
  //        mp_Listener->displayText(generateSavingReportStr());
  //        mp_Engine->saveReports();
  //        mp_Listener->displayText(generateDoneStr());
  //        getExecutionMessages().resetWarningFlag();
  //      }
  //
  //      m_FullEndTime = getNow();
  //
  //      mp_Listener->displayText(generateWarningsCountStr());
  //      mp_Listener->displayText(generateRunTimeInformationStr());
  //    } catch (openfluid::base::OFException& E)
  //    {
  //      BuilderAppDialogFactory::showSimpleErrorMessage(E.what());
  //      std::cerr << E.what() << std::endl;
  //      stopEngine();
  //    }
  //
  //    mp_ModelInstance->resetInitialized();
  //  }
}

// =====================================================================
// =====================================================================


bool EngineProject::check()
{
  //  bool IsOk = true;
  //
  //  try
  //  {
  //    std::cout << "* Filling Run Environnement Descriptor..." << std::endl;
  //    openfluid::machine::Factory::fillRunEnvironmentFromDescriptor(
  //        getRunDescriptor());
  //    std::cout << "...Run Environnement Descriptor filled" << std::endl;
  //
  //    getCoreRepository().sortUnitsByProcessOrder();
  //
  //    mp_ModelInstance->initialize();
  //
  //    ///// CHECK MODEL
  //    std::cout << "* Initializing parameters... " << std::endl;
  //    mp_Engine->initParams();
  //
  //    //            mp_StatusParamsValues->clearErrorValues();
  //    //
  //    //            bool IsOk = true;
  //    //
  //    //            const std::list<openfluid::machine::ModelItemInstance *> ItemInstances = ModelInstance->getItems();
  //    //
  //    //            std::list<openfluid::machine::ModelItemInstance *>::const_iterator it;
  //    //
  //    //            for(it=ItemInstances.begin() ; it!=ItemInstances.end() ; ++it)
  //    //            {
  //    //              openfluid::machine::ModelItemInstance * ItemInstance = *it;
  //    //
  //    //              try
  //    //              {
  //    //                ItemInstance->Function->initParams(ItemInstance->Params);
  //    //              }
  //    //              catch(openfluid::base::OFException& E)
  //    //              {
  //    //                std::cerr << E.what() << std::endl;
  //    //                mp_StatusParamsValues->appendErrorValue(E.what());
  //    //                IsOk = false;
  //    //                continue;
  //    //              }
  //    //
  //    //            }
  //    //
  //    //            Glib::ustring Status = IsOk ? "(OK)" : "(with errors)";
  //    //
  //    std::cout << "...Parameters initialized " << /*Status <<*/std::endl;
  //    //
  //    //            return IsOk;
  //
  //    ////
  //
  //    mp_Engine->prepareData();
  //
  //    getCoreRepository().clearAllVariables();
  //    mp_Engine->checkConsistency();
  //  } catch (openfluid::base::OFException& E)
  //  {
  //    BuilderAppDialogFactory::showSimpleErrorMessage(E.what());
  //    std::cerr << E.what() << std::endl;
  //    stopEngine();
  //    IsOk = false;
  //  }
  //
  //  mp_ModelInstance->resetInitialized();
  //  return IsOk;
}

// =====================================================================
// =====================================================================


void EngineProject::stopEngine()
{
  //  std::cout << generateWarningsCountStr() << std::endl;
  //
  //  if (mp_Engine)
  //    mp_Engine->closeOutputs();
  //
  //  if (mp_Engine && getRunEnv()->isWriteSimReport())
  //  {
  //    std::cout << _("* Closing outputs... ") << std::endl;
  //    mp_Engine->closeOutputs();
  //    std::cout << generateDoneStr() << std::endl;
  //
  //    std::cout << generateSavingReportStr() << std::endl;
  //    mp_Engine->saveReports();
  //    std::cout << generateDoneStr() << std::endl;
  //
  //    getExecutionMessages().resetWarningFlag();
  //  }
}

// =====================================================================
// =====================================================================

//
//boost::posix_time::ptime EngineProject::getNow()
//{
//  return boost::posix_time::microsec_clock::local_time();
//}
//
//// =====================================================================
//// =====================================================================
//
//
//std::string EngineProject::generateSimulationIdStr()
//{
//  return Glib::ustring::compose(_("\nSimulation ID: %1\n\n"),
//      getRunEnv()->getSimulationID());
//}
//
//// =====================================================================
//// =====================================================================
//
//
//std::string EngineProject::generateDomainInformationStr()
//{
//  Glib::ustring Str = "";
//  unsigned int UnitsCount = 0;
//
//  for (openfluid::core::UnitsListByClassMap_t::const_iterator it =
//      getCoreRepository().getUnitsByClass()->begin(); it
//      != getCoreRepository().getUnitsByClass()->end(); ++it)
//  {
//    Str.append(Glib::ustring::compose("  - %1, %2 units\n", it->first,
//        it->second.getList()->size()));
//    UnitsCount++;
//  }
//
//  return Glib::ustring::compose(_("Spatial domain, %1 units :\n%2\n"),
//      UnitsCount, Str);
//}
//
//// =====================================================================
//// =====================================================================
//
//
//std::string EngineProject::generateWarningsCountStr()
//{
//  return Glib::ustring::compose(_("\n%1 warning(s)\n\n"),
//      getExecutionMessages().getWarningsCount());
//}
//
//// =====================================================================
//// =====================================================================
//
//
//std::string EngineProject::generateSimulationInformationStr()
//{
//  openfluid::base::SimulationInfo* SimInfo = mp_Engine->getSimulationInfo();
//
//  return Glib::ustring::compose(
//      _("Simulation from %1 to %2\n         -> %3 time steps of %4 seconds\n\n"),
//      SimInfo->getStartTime().getAsISOString(),
//      SimInfo->getEndTime().getAsISOString(), SimInfo->getStepsCount(),
//      SimInfo->getTimeStep());
//}
//
//// =====================================================================
//// =====================================================================
//
//
//std::string EngineProject::generateBuffersInformationStr()
//{
//  Glib::ustring Str = "";
//
//  if (getRunEnv()->isUserValuesBufferSize())
//    Str = Glib::ustring::compose(
//        _("Buffers for variables set to %1 time steps\n"),
//        getRunEnv()->getValuesBufferSize());
//  else
//    Str = _("Buffers for variables set to full simulation\n");
//
//  Str.append(Glib::ustring::compose(
//      _("Buffers for output files set to %1 bytes\n\n"),
//      getRunEnv()->getFilesBufferSize()));
//
//  return Str;
//}
//
//// =====================================================================
//// =====================================================================
//
//
//std::string EngineProject::generateRunTimeInformationStr()
//{
//  boost::posix_time::time_duration FullSimDuration = m_FullEndTime
//      - m_FullStartTime;
//
//  return Glib::ustring::compose(
//      _("Simulation run time: %1\n     Total run time: %2\n\n"),
//      boost::posix_time::to_simple_string(
//          getRunEnv()->getEffectiveSimulationDuration()),
//      boost::posix_time::to_simple_string(FullSimDuration));
//}
//
//// =====================================================================
//// =====================================================================
//
//
//std::string EngineProject::generateSavingReportStr()
//{
//  return Glib::ustring::compose(_("%1Saving simulation report%2\n"), "* ",
//      "... ");
//}
//
//// =====================================================================
//// =====================================================================
//
//
//std::string EngineProject::generateDoneStr()
//{
//  return Glib::ustring::compose(_("%1Done%2\n"), "[", "]");
//}


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
  //  delete mp_RunDialog;
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

