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

#include <libxml2/libxml/xmlerror.h>
#include <boost/date_time.hpp>
#include <boost/filesystem/convenience.hpp>

#include <openfluid/base/RuntimeEnv.hpp>
#include <openfluid/base/ProjectManager.hpp>
#include <openfluid/fluidx/FunctionDescriptor.hpp>
#include <openfluid/fluidx/GeneratorDescriptor.hpp>
#include <openfluid/fluidx/DatastoreDescriptor.hpp>
#include <openfluid/base/IOListener.hpp>
#include <openfluid/fluidx/FluidXDescriptor.hpp>
#include <openfluid/guicommon/RunDialogMachineListener.hpp>
#include <openfluid/guicommon/DialogBoxFactory.hpp>
#include <openfluid/guicommon/PreferencesManager.hpp>
#include <openfluid/guicommon/SimulationRunDialog.hpp>
#include <openfluid/machine/SimulationBlob.hpp>
#include <openfluid/machine/ModelInstance.hpp>
#include <openfluid/machine/ModelItemInstance.hpp>
#include <openfluid/machine/ObserverInstance.hpp>
#include <openfluid/machine/MonitoringInstance.hpp>
#include <openfluid/machine/Factory.hpp>
#include <openfluid/machine/Generator.hpp>

#include <openfluid/guicommon/GeneratorSignature.hpp>
#include "EngineHelper.hpp"
#include <openfluid/guicommon/FunctionSignatureRegistry.hpp>
#include <openfluid/guicommon/BuilderDescriptor.hpp>

// =====================================================================
// =====================================================================

Glib::ustring Msg = "";

void MyErrorHandler(void* /*userData*/, xmlErrorPtr error)
{
  Msg = Glib::ustring::compose(
      _("Xml error:\n%1\n\nduring parse of file:\n%2\n\n"
          "Project can't be opened."),
      error->message, error->file);
}
;

// =====================================================================
// =====================================================================

EngineProject::EngineProject(Glib::ustring FolderIn, bool WithProjectManager) :
    m_WithProjectManager(WithProjectManager), mp_IOListener(0), mp_FXDesc(0)
{
  std::string Now = boost::posix_time::to_iso_extended_string(
      boost::posix_time::microsec_clock::local_time());
  Now[10] = ' ';
  m_DefaultBeginDT.setFromISOString(Now);

  m_DefaultDeltaT = openfluid::core::DateTime::Minutes(5);

  if (m_WithProjectManager)
    openfluid::base::RuntimeEnvironment::getInstance()->linkToProject();

  mp_IOListener = new openfluid::base::IOListener();

  mp_FXDesc = new openfluid::fluidx::FluidXDescriptor(mp_IOListener);

  if (FolderIn == "")
  {
    setDefaultRunDesc();
    // TODO to be removed or replaced by monitoring
    //setDefaultOutDesc();
  }
  else
  {
    //reset the generic error handler
    initGenericErrorDefaultFunc(NULL);
    //supply custom error handler
    xmlSetStructuredErrorFunc(NULL, MyErrorHandler);

    try
    {
      mp_FXDesc->loadFromDirectory(
          WithProjectManager ? openfluid::base::ProjectManager::getInstance()->getInputDir() :
                               FolderIn);
    }
    catch (openfluid::base::OFException e)
    {
      if (Msg.empty())
        Msg = EngineHelper::minimiseInfoString(e.what());

      openfluid::guicommon::DialogBoxFactory::showSimpleErrorMessage(Msg);

      //because we're in a constructor catch, so destructor isn't called
      deleteEngineObjects();
      Msg = "";
      throw;
    }

    checkAndSetDefaultRunValues();

    // TODO to be removed or replaced by monitoring
    //checkAndSetDefaultOutputValues();

    checkModelDesc();

//    try
//    {
//      openfluid::machine::Factory::buildSimulationBlobFromDescriptors(*mp_FXDesc, *mp_SimBlob);
//
//      openfluid::machine::Factory::buildModelInstanceFromDescriptor(ModelDesc,
//          *mp_ModelInstance);
//
//      openfluid::machine::Factory::buildObserversListFromDescriptor(mp_FXDesc->getObserversListDescriptor(),
//                                                                    *mp_ObsListInstance);
//    }
//    catch (openfluid::base::OFException e)
//    {
//      Glib::ustring
//          Msg =
//              Glib::ustring::compose(
//                  _("Warning:\n%1.\n\nSome files may be overwritten.\nDo you want to continue?"),
//                  EngineHelper::minimiseInfoString(e.what()));
//
//      if (!openfluid::guicommon::DialogBoxFactory::showSimpleOkCancelQuestionDialog(
//          Msg))
//      {
//        //because we're in a constructor catch, so destructor isn't called
//        deleteEngineObjects();
//        throw;
//      }
//    }

// no more used (now check made by BuilderDomain)
//    checkInputData();

    addSignatureToGenerators();

//    Glib::ustring OutputConsistencyMessage = checkOutputsConsistency();
//
//    if (!OutputConsistencyMessage.empty())
//      openfluid::guicommon::DialogBoxFactory::showSimpleWarningMessage(
//          Glib::ustring::compose(_(
//              "Check outputs consistency leads OpenFLUID to delete:\n\n%1"),
//              OutputConsistencyMessage));
  }

  mp_BuilderDesc = new openfluid::guicommon::BuilderDescriptor(*mp_FXDesc);
}

// =====================================================================
// =====================================================================

sigc::signal<void> EngineProject::signal_RunStarted()
{
  return m_signal_RunStarted;
}

// =====================================================================
// =====================================================================

sigc::signal<void> EngineProject::signal_RunStopped()
{
  return m_signal_RunStopped;
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

  openfluid::fluidx::RunDescriptor RunDesc(DefaultDeltaT, DefaultBeginDT,
                                           DefaultEndDT);

  if (PrefMgr->getOutFilesBufferInKB() != -1)
    RunDesc.setFilesBufferSizeInKB(PrefMgr->getOutFilesBufferInKB());

  RunDesc.setFilled(true);

  mp_FXDesc->getRunDescriptor() = RunDesc;
}

// =====================================================================
// =====================================================================

// TODO to be removed or replaced by monitoring
/*
void EngineProject::setDefaultOutDesc()
{
  openfluid::base::OutputDescriptor OutDesc;
  openfluid::base::OutputFilesDescriptor FileDesc;

  OutDesc.getFileSets().push_back(FileDesc);

  mp_FXDesc->getOutputDescriptor() = OutDesc;
}
*/

// =====================================================================
// =====================================================================

void EngineProject::checkAndSetDefaultRunValues()
{
  openfluid::fluidx::RunDescriptor& RunDesc = mp_FXDesc->getRunDescriptor();

  openfluid::core::DateTime DT;
  openfluid::core::DateTime BeginDT;

  if (!DT.setFromISOString(RunDesc.getBeginDate().getAsISOString()))
  {
    RunDesc.setBeginDate(m_DefaultBeginDT);

    BeginDT = m_DefaultBeginDT;
  }
  else
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
// TODO to be removed or replaced by monitoring
/*
void EngineProject::checkAndSetDefaultOutputValues()
{
  openfluid::base::OutputDescriptor& OutDesc = mp_FXDesc->getOutputDescriptor();

  if (OutDesc.getFileSets().empty())
  {
    openfluid::base::OutputFilesDescriptor FileDesc;
    OutDesc.getFileSets().push_back(FileDesc);
  }
}
*/


// =====================================================================
// =====================================================================

void EngineProject::checkModelDesc()
{
//  openfluid::fluidx::CoupledModelDescriptor& ModelDesc = mp_FXDesc->getModelDescriptor();
//
//  std::string MissingFunctions = "";
//
//  openfluid::fluidx::CoupledModelDescriptor::SetDescription_t::iterator it =
//      ModelDesc.getItems().begin();
//
//  FunctionSignatureRegistry* SignaturesReg =
//      FunctionSignatureRegistry::getInstance();
//
//  while (it != ModelDesc.getItems().end())
//  {
//    if ((*it)->isType(openfluid::fluidx::ModelItemDescriptor::PluggedFunction)
//        && !SignaturesReg->isPluggableFunctionAvailable(
//            ((openfluid::fluidx::FunctionDescriptor*) (*it))->getFileID()))
//    {
//      MissingFunctions.append("- "
//          + ((openfluid::fluidx::FunctionDescriptor*) (*it))->getFileID() + "\n");
//
//      it = ModelDesc.getItems().erase(it);
//    }
//    else
//      ++it;
//  }
//
//  if (MissingFunctions != "")
//  {
//    Glib::ustring Msg = Glib::ustring::compose(
//        _("Unable to find plugin file(s):\n%1\n\n"
//            "Corresponding simulation functions will be removed from the model.\n"
//            "Do you want to continue?"), MissingFunctions);
//
//    if (!openfluid::guicommon::DialogBoxFactory::showSimpleOkCancelQuestionDialog(
//        Msg))
//    {
//      //because we're in a constructor catch, so destructor isn't called
//      deleteEngineObjects();
//      throw openfluid::base::OFException("");
//    }
//  }

}

// =====================================================================
// =====================================================================

void EngineProject::addSignatureToGenerators()
{
//  std::list<openfluid::machine::ModelItemInstance*> Items =
//      mp_ModelInstance->getItems();
//
//  for (std::list<openfluid::machine::ModelItemInstance*>::iterator it =
//      Items.begin(); it != Items.end(); ++it)
//  {
//    if ((*it)->ItemType == openfluid::fluidx::ModelItemDescriptor::Generator)
//    {
//      openfluid::fluidx::GeneratorDescriptor::GeneratorMethod
//          GeneratorMethod = (*it)->GeneratorInfo->GeneratorMethod;
//
//      GeneratorSignature* GenSign = new GeneratorSignature(GeneratorMethod);
//
//      GenSign->ID = (*it)->Signature->ID;
//      GenSign->HandledData.ProducedVars = (*it)->Signature->HandledData.ProducedVars;
//      GenSign->HandledData.RequiredExtraFiles = (*it)->Signature->HandledData.RequiredExtraFiles;
//
//      (*it)->Signature = GenSign;
//    }
//  }
}

// =====================================================================
// =====================================================================

void EngineProject::run()
{
  openfluid::base::ProjectManager::getInstance()->updateOutputDir();

  if (m_WithProjectManager)
    openfluid::base::RuntimeEnvironment::getInstance()->linkToProject();

  openfluid::machine::SimulationBlob* SimBlob =
      new openfluid::machine::SimulationBlob();

  openfluid::guicommon::RunDialogMachineListener* Listener =
      new openfluid::guicommon::RunDialogMachineListener();

  openfluid::machine::ModelInstance* ModelInstance =
      new openfluid::machine::ModelInstance(*SimBlob, Listener);

  openfluid::machine::MonitoringInstance MonitInstance(*SimBlob);

  openfluid::machine::Engine Engine(*SimBlob, *ModelInstance, MonitInstance,
                                    Listener);

  openfluid::machine::Factory::buildSimulationBlobFromDescriptors(*mp_FXDesc,
                                                                  *SimBlob);

  openfluid::machine::Factory::buildModelInstanceFromDescriptor(
      mp_FXDesc->getModelDescriptor(), *ModelInstance);

  openfluid::machine::Factory::buildMonitoringInstanceFromDescriptor(
      mp_FXDesc->getMonitoringDescriptor(), MonitInstance);

  // no more used, TODO to clean
//  mp_SimBlob->clearSimulationGarbage();

  openfluid::machine::Factory::fillRunEnvironmentFromDescriptor(
      mp_FXDesc->getRunDescriptor());

  SimBlob->getCoreRepository().sortUnitsByProcessOrder();

  openfluid::guicommon::SimulationRunDialog RunDialog(&Engine);

  RunDialog.signal_SimulationStarted().connect(
      sigc::mem_fun(*this, &EngineProject::whenSimulationStarted));
  RunDialog.signal_SimulationStopped().connect(
      sigc::mem_fun(*this, &EngineProject::whenSimulationStopped));

  Gtk::Main::run(RunDialog);
}

// =====================================================================
// =====================================================================

void EngineProject::whenSimulationStarted()
{
  m_signal_RunStarted.emit();
}

// =====================================================================
// =====================================================================

void EngineProject::whenSimulationStopped()
{
  m_signal_RunStopped.emit();
}

// =====================================================================
// =====================================================================

void EngineProject::save()
{
  std::string InputDir = openfluid::base::RuntimeEnvironment::getInstance()->getInputDir();

  openfluid::base::ProjectManager::getInstance()->save();

  boost::filesystem::path InputPath(InputDir);

  boost::filesystem::directory_iterator end_it;
  for (boost::filesystem::directory_iterator it(InputPath); it != end_it; ++it)
  {
    if ((boost::filesystem::extension(it->path()) == ".fluidx"))
      boost::filesystem::remove(it->path());
  }

  mp_FXDesc->writeToManyFiles(InputDir);

  m_signal_SaveHappened.emit();
}

// =====================================================================
// =====================================================================

void EngineProject::check(
    openfluid::machine::Engine::PretestInfos_t& PretestInfos)
{
  // TODO: check descriptors instead
//  mp_Engine->pretestConsistency(PretestInfos);
}

// =====================================================================
// =====================================================================

openfluid::guicommon::BuilderDescriptor& EngineProject::getBuilderDesc()
{
  return *mp_BuilderDesc;
}

// =====================================================================
// =====================================================================

//TODO: delete and replace by direct call to FXDesc
//openfluid::core::Datastore& EngineProject::getDatastore()
//{
//  return mp_SimBlob->getDatastore();
//}

// =====================================================================
// =====================================================================

EngineProject::~EngineProject()
{
  deleteEngineObjects();
}

// =====================================================================
// =====================================================================

void EngineProject::deleteEngineObjects()
{
  delete mp_IOListener;
  //don't delete mp_RunEnv nor mp_FXDesc, which are singletons
  if (m_WithProjectManager)
    openfluid::base::RuntimeEnvironment::getInstance()->detachFromProject();
}

// =====================================================================
// =====================================================================

//Glib::ustring EngineProject::checkOutputsConsistency()
//{
//  Glib::ustring Message = "";
//
//  std::set<std::string> ClassNames = EngineHelper::getClassNames(
//      &getCoreRepository());
//
//  for (unsigned int i = 0; i < getOutputDescriptor().getFileSets().size(); i++)
//  {
//    std::vector<openfluid::base::OutputSetDescriptor>::iterator itSet =
//        getOutputDescriptor().getFileSets()[i].getSets().begin();
//
//    while (itSet != getOutputDescriptor().getFileSets()[i].getSets().end())
//    {
//      std::string ClassName = itSet->getUnitsClass();
//      std::string SetName = itSet->getName();
//
//      std::set<std::string> VarNames = EngineHelper::getProducedVarNames(
//          ClassName, mp_ModelInstance);
//
//      Glib::ustring MsgDetail = "";
//
//      if (ClassNames.find(ClassName) == ClassNames.end())
//      {
//        MsgDetail = Glib::ustring::compose(_("class %1 doesn't exist"),
//            ClassName);
//      }
//      else if (VarNames.empty())
//      {
//        MsgDetail = Glib::ustring::compose(
//            _("no variable produced for class %1"), ClassName);
//      }
//      else if (!itSet->isAllUnits() && itSet->getUnitsIDs().empty())
//      {
//        MsgDetail = _("no unit selected");
//      }
//      else if (!itSet->isAllVariables() && itSet->getVariables().empty())
//      {
//        MsgDetail = _("no variable selected");
//      }
//
//      if (MsgDetail != "")
//      {
//        Message.append(Glib::ustring::compose("- Set %1 (%2)\n", SetName,
//            MsgDetail));
//        getOutputDescriptor().getFileSets()[i].getSets().erase(itSet);
//      }
//      else
//      {
//        // check Ids exist
//
//        std::vector<unsigned int>::iterator itId = itSet->getUnitsIDs().begin();
//
//        while (itId != itSet->getUnitsIDs().end())
//        {
//          if (getCoreRepository().getUnit(ClassName, *itId) == NULL)
//          {
//            Message.append(Glib::ustring::compose(_("- ID %1 of set %2\n"),
//                *itId, SetName));
//            itSet->getUnitsIDs().erase(itId);
//          }
//          else
//            itId++;
//        }
//
//        // check Var names exist
//
//        std::vector<std::string>::iterator itVar;
//
//        itVar = itSet->getVariables().begin();
//
//        while (itVar != itSet->getVariables().end())
//        {
//          if (VarNames.find(*itVar) == VarNames.end())
//          {
//            Message.append(Glib::ustring::compose(_("- Var %1 of set %2\n"),
//                *itVar, SetName));
//            itSet->getVariables().erase(itVar);
//          }
//          else
//            itVar++;
//        }
//
//        // check still at least one Id and one Var in Set
//
//        if ((!itSet->isAllUnits() && itSet->getUnitsIDs().empty())
//            || (!itSet->isAllVariables() && itSet->getVariables().empty()))
//        {
//          Message.append(Glib::ustring::compose(_("=> Set %1\n"), SetName));
//          getOutputDescriptor().getFileSets()[i].getSets().erase(itSet);
//        }
//        else
//          ++itSet;
//      }
//    }
//  }
//
//  return Message;
//}

// =====================================================================
// =====================================================================

