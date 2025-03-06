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
 @file ProjectCentral.cpp

 @author Jean-Christophe Fabre <jean-christophe.fabre@inra.fr>
 @author Armel THÖNI <armel.thoni@inrae.fr>
 */


#include <QDir>
#include <QMessageBox>

#include <openfluid/ui/common/RunSimulationDialog.hpp>
#include <openfluid/ui/common/RunCLISimulationDialog.hpp>
#include <openfluid/machine/SimulatorRegistry.hpp>
#include <openfluid/machine/ObserverRegistry.hpp>
#include <openfluid/machine/GeneratorSignature.hpp>
#include <openfluid/machine/ModelItemInstance.hpp>
#include <openfluid/machine/ObserverInstance.hpp>
#include <openfluid/base/ApplicationException.hpp>
#include <openfluid/base/RunContextManager.hpp>
#include <openfluid/base/IOListener.hpp>
#include <openfluid/tools/Filesystem.hpp>
#include <openfluid/ui/QtHelpers.hpp>

#include "ProjectCentral.hpp"
#include "AppTools.hpp"


ProjectCentral* ProjectCentral::mp_Instance = nullptr;


ProjectCentral::ProjectCentral(const QString& PrjPath)
{
  if (PrjPath.isEmpty())
  {
    setDefaultDescriptors();
  }
  else
  {
    try
    {
      openfluid::base::IOListener Listener;

      openfluid::fluidx::FluidXIO FXIO(&Listener);
      m_FXDesc = FXIO.loadFromDirectory(openfluid::base::RunContextManager::instance()->getInputDir());
      m_LoadingReport = FXIO.getLoadingReport();
    }
    catch (openfluid::base::Exception& E)
    {
      //because we're in a constructor catch, so destructor is not called
      deleteData();
      throw;
    }
  }

  // initialization of models for completion
  mp_AllNamesListModel = new QStringListModel(this);

  openfluid::machine::SimulatorRegistry::instance()->clear();
  openfluid::machine::SimulatorRegistry::instance()->discoverWares();
  openfluid::machine::ObserverRegistry::instance()->clear();
  openfluid::machine::ObserverRegistry::instance()->discoverWares();

  check();
}


// =====================================================================
// =====================================================================


ProjectCentral::~ProjectCentral()
{
  openfluid::machine::SimulatorRegistry::instance()->clear();
  openfluid::machine::ObserverRegistry::instance()->clear();
  deleteData();
}


// =====================================================================
// =====================================================================


ProjectCentral* ProjectCentral::initInstance(const QString& PrjPath)
{
  if (!mp_Instance)
  {
    mp_Instance = new ProjectCentral(PrjPath);
  }

  return mp_Instance;
}


// =====================================================================
// =====================================================================


void ProjectCentral::kill()
{
  if (mp_Instance)
  {
    delete mp_Instance;
  }  
  mp_Instance = nullptr;
}


// =====================================================================
// =====================================================================


ProjectCentral* ProjectCentral::instance()
{
  if (!mp_Instance)
  {
    openfluid::base::ExceptionContext Context =
        openfluid::base::ApplicationException::computeContext("openfluid-builder",
                                                              OPENFLUID_CODE_LOCATION);
    throw openfluid::base::ApplicationException(Context,"Required ProjectCentral instance is null");
  }
  return mp_Instance;
}


// =====================================================================
// =====================================================================


QStringList ProjectCentral::convertUpdatedUnitsClassesToQStringList(
    const std::vector<openfluid::ware::SignatureUnitsClassItem>& UnitsClassesVector)
{
  QStringList QSL;
  
  for (unsigned int i=0; i<UnitsClassesVector.size();++i)
  {
    QSL.append(QString::fromStdString(UnitsClassesVector[i].UnitsClass));
  }

  return QSL;
}


// =====================================================================
// =====================================================================


void ProjectCentral::deleteData()
{
  m_FXDesc.reset();
  m_LoadingReport.clear();
}


// =====================================================================
// =====================================================================


void ProjectCentral::setDefaultDescriptors()
{

}


// =====================================================================
// =====================================================================


void ProjectCentral::run(RunMode Mode)
{
  openfluid::base::RunContextManager* CtxtMan = openfluid::base::RunContextManager::instance();

  CtxtMan->setClearOutputDir(CtxtMan->getProjectContextValue("/builder/runconfig/clear_outdir").get<bool>(false));
  CtxtMan->setProfiling(CtxtMan->getProjectContextValue("/builder/runconfig/profiling").get<bool>(false));
  CtxtMan->setWaresMaxNumThreads(CtxtMan->getProjectContextValue("/builder/runconfig/maxthreads")
                                   .get<int>(openfluid::base::Environment::getIdealThreadCount()));

  if (Mode == RunMode::DEFAULT)
  {
    openfluid::ui::common::RunSimulationDialog RunDlg(QApplication::activeWindow(),&m_FXDesc);
    RunDlg.execute();
  }
  else if (Mode == RunMode::CLI)
  {
    openfluid::ui::common::RunCLISimulationDialog RunDlg(QApplication::activeWindow(),
                                                         QString::fromStdString(CtxtMan->getProjectPath()));
    RunDlg.execute();
  }
  else
  {
    QMessageBox::critical(QApplication::activeWindow(),
                          tr("Run error"),
                          tr("Uncorrect simulation mode."),
                          QMessageBox::Close);
  }

  // Removed to prevent crashes when a parameterization widget is active
  // openfluid::machine::SimulatorSignatureRegistry::instance()->unloadAll();
  // openfluid::machine::ObserverSignatureRegistry::instance()->unloadAll();
}


// =====================================================================
// =====================================================================


bool ProjectCentral::save()
{
  std::string InputDir = openfluid::base::RunContextManager::instance()->getInputDir();

  openfluid::base::RunContextManager::instance()->saveProject();

  // remove all fluidx files
  QStringList FluidXFileToRemove;
  QDir InputPath(QString::fromStdString(InputDir));  
  FluidXFileToRemove = InputPath.entryList(QStringList("*.fluidx"),QDir::Files | QDir::NoDotAndDotDot);

  for (int i=0;i<FluidXFileToRemove.size();++i)
  {
    InputPath.remove(FluidXFileToRemove[i]);
  }

  m_LoadingReport.clear();

  // save all fluidx files
  openfluid::base::IOListener Listener;
  openfluid::fluidx::FluidXIO FXIO(&Listener);
  FXIO.writeToManyFiles(m_FXDesc,InputDir);

  return true;
}


// =====================================================================
// =====================================================================


bool ProjectCentral::saveAs(const QString& NewPrjName, const QString& NewPrjPath)
{
  openfluid::base::RunContextManager* PrjCtxt = openfluid::base::RunContextManager::instance();

  QString OldPrjPath = QString::fromStdString(PrjCtxt->getProjectPath());

  // close current project
  PrjCtxt->closeProject();

  // create new project dir
  QDir().mkpath(NewPrjPath);

  // copy files and subdirectories from current to renamed project
  openfluid::tools::Filesystem::copyDirectory(OldPrjPath.toStdString(),NewPrjPath.toStdString(),false,true);

  // open renamed project
  if (PrjCtxt->openProject(NewPrjPath.toStdString()))
  {
    // update project metadata
    PrjCtxt->setProjectName(NewPrjName.toStdString());
    PrjCtxt->setProjectCreationDateAsNow();

    // save project and project metadata
    PrjCtxt->saveProject();
    save();

    return true;
  }

  return false;
}


// =====================================================================
// =====================================================================


void ProjectCentral::check()
{
  m_CheckInfos.clear();

  if (!m_FXDesc.model().items().empty())
  {
    checkModel();
  }
  else
  {
    m_CheckInfos.part(ProjectCheckInfos::PartInfo::PART_MODELDEF).updateStatus(ProjectStatusLevel::PRJ_DISABLED);
    m_CheckInfos.part(ProjectCheckInfos::PartInfo::PART_MODELDEF).addMessage(tr("Model is empty"));
  }

  checkSpatialDomain();

  checkDatastore();

  if (!m_FXDesc.monitoring().items().empty())
  {
    checkMonitoring();
  }
  else
  {
    m_CheckInfos.part(ProjectCheckInfos::PartInfo::PART_MONITORING).updateStatus(ProjectStatusLevel::PRJ_WARNING);
    m_CheckInfos.part(ProjectCheckInfos::PartInfo::PART_MONITORING).addMessage(tr("Monitoring is empty"));
  }

  checkRunConfig();

  // update of models for completion
  mp_AllNamesListModel->setStringList(getAllNamesList());
}


// =====================================================================
// =====================================================================


void ProjectCentral::checkGeneratorParam(const std::string& MinParamName,
                                         const std::string& MaxParamName,
                                         const openfluid::fluidx::ModelItemDescriptor* Item,
                                         const std::string& ID)
{
  bool TestCompare = true;

  if (!isParamSet(Item, MinParamName))
  {
    TestCompare = false;
  }
  else if (!isParamIsDouble(Item, MinParamName))
  {
    TestCompare = false;

    m_CheckInfos.part(ProjectCheckInfos::PartInfo::PART_MODELPARAMS).updateStatus(ProjectStatusLevel::PRJ_ERROR);
    m_CheckInfos.part(ProjectCheckInfos::PartInfo::PART_MODELPARAMS)
                .addMessage(tr("Parameter %1 for %2 has to be a double precision value")
                            .arg(QString::fromStdString(MinParamName))
                            .arg(QString::fromStdString(ID)));
  }

  if (!isParamSet(Item, MaxParamName))
  {
    TestCompare = false;
  }
  else if (!isParamIsDouble(Item, MaxParamName))
  {
    TestCompare = false;
    m_CheckInfos.part(ProjectCheckInfos::PartInfo::PART_MODELPARAMS).updateStatus(ProjectStatusLevel::PRJ_ERROR);
    m_CheckInfos.part(ProjectCheckInfos::PartInfo::PART_MODELPARAMS)
                .addMessage(tr("Parameter %1 for %2 has to be a double precision value")
                            .arg(QString::fromStdString(MaxParamName))
                            .arg(QString::fromStdString(ID)));
  }

  if (TestCompare && !(getParamAsDouble(Item, MaxParamName) > getParamAsDouble(Item, MinParamName)))
  {
    m_CheckInfos.part(ProjectCheckInfos::PartInfo::PART_MODELPARAMS).updateStatus(ProjectStatusLevel::PRJ_ERROR);
    m_CheckInfos.part(ProjectCheckInfos::PartInfo::PART_MODELPARAMS)
                .addMessage(tr("Parameter %1 is greater or equal to parameter %2 for %3")
                            .arg(QString::fromStdString(MinParamName))
                            .arg(QString::fromStdString(MaxParamName))
                            .arg(QString::fromStdString(ID)));
  }
}


// =====================================================================
// =====================================================================


bool ProjectCentral::isParamSet(const openfluid::fluidx::ModelItemDescriptor* Item,
                                const std::string& ParamName) const
{
  return (!Item->getParameters()[ParamName].get().empty() ||
          !m_FXDesc.model().getGlobalParameters()[ParamName].get().empty());
}


// =====================================================================
// =====================================================================


bool ProjectCentral::isParamSet(const openfluid::fluidx::ObserverDescriptor* Item,
                                const std::string& ParamName) const
{
  return (!Item->getParameters()[ParamName].get().empty());
}


// =====================================================================
// =====================================================================


bool ProjectCentral::isParamIsDouble(const openfluid::fluidx::ModelItemDescriptor* Item,
                                     const std::string& ParamName) const
{
  if (!isParamSet(Item, ParamName))
  {
    return false;
  }

  double d;

  return (Item->getParameters()[ParamName].toDouble(d) ||
          m_FXDesc.model().getGlobalParameters()[ParamName].toDouble(d));
}


// =====================================================================
// =====================================================================


double ProjectCentral::getParamAsDouble(const openfluid::fluidx::ModelItemDescriptor* Item,
                                        const std::string& ParamName) const
{
  if (!isParamIsDouble(Item, ParamName))
  {
    openfluid::base::ExceptionContext Context =
        openfluid::base::ApplicationException::computeContext("openfluid-builder",
                                                              OPENFLUID_CODE_LOCATION);

    throw openfluid::base::ApplicationException(Context,
                                                "Parameter " + ParamName + " is not set as a double precision value");
  }

  double d;

  if (Item->getParameters()[ParamName].toDouble(d))
  {
    return d;
  }

  m_FXDesc.model().getGlobalParameters()[ParamName].toDouble(d);

  return d;
}


// =====================================================================
// =====================================================================


void ProjectCentral::checkModel()
{
  openfluid::fluidx::CoupledModelDescriptor& Model = m_FXDesc.model();
  openfluid::fluidx::SpatialDomainDescriptor& Domain = m_FXDesc.spatialDomain();

  const std::list<openfluid::fluidx::ModelItemDescriptor*>& Items = Model.items();

  openfluid::machine::SimulatorRegistry* Reg = openfluid::machine::SimulatorRegistry::instance();
  openfluid::base::RunContextManager* RunCtxt = openfluid::base::RunContextManager::instance();

  bool AtLeastOneEnabled = false;

  m_SimulatorsIDsList.clear();
  m_SimulatorsParamsLists.clear();
  m_UpdatedUnitsClass.clear();
  m_AttrsUnits.clear();
  m_VarsUnits.clear();
  m_TypedVarsUnits.clear();


  // Create non existing generators in registry
  for (const auto* Item : Model.items())
  {
    const auto ID = Model.getID(Item);

    if (Item->isEnabled() && Item->isType(openfluid::ware::WareType::GENERATOR) && !Reg->hasGenerator(ID))
    {
      const auto* GenDesc = static_cast<const openfluid::fluidx::GeneratorDescriptor*>(Item);
      Reg->addGenerator({GenDesc->getGeneratorMethod(),
                          GenDesc->getVariableTriplets(),
                          GenDesc->getVariableType(),
                          GenDesc->getVariableDimensions()
                         });
    }
  }


  for (const auto* Item : Model.items())
  {
    const auto ID = Model.getID(Item);

    m_SimulatorsIDsList << QString::fromStdString(ID);


    if (Item->isEnabled())
    {
      AtLeastOneEnabled = true;

      const auto& Container = Reg->wareOrGeneratorContainer(ID);

      if (Container.isValid() &&  Container.hasSignature())
      {
        // checking if simulator is a ghost
        if (Container.isGhost())
        {
          m_CheckInfos.part(ProjectCheckInfos::PartInfo::PART_MODELDEF).updateStatus(ProjectStatusLevel::PRJ_DISABLED);
          m_CheckInfos.part(ProjectCheckInfos::PartInfo::PART_MODELDEF)
                          .addMessage(tr("Simulator %1 is a ghost")
                                      .arg(QString::fromStdString(ID)));
        }


        const auto* Sign = Container.signature().get();


        // ========== external constraints

        // check extrafiles

        const auto& ReqFiles = Sign->HandledData.RequiredExtraFiles;

        if (Item->isType(openfluid::ware::WareType::GENERATOR))
        {
          openfluid::fluidx::GeneratorDescriptor::GeneratorMethod Method =
              (static_cast<const openfluid::fluidx::GeneratorDescriptor*>(Item))->getGeneratorMethod();

          if (Method == openfluid::fluidx::GeneratorDescriptor:: GeneratorMethod::INTERP ||
              Method == openfluid::fluidx::GeneratorDescriptor:: GeneratorMethod::INJECT)
          {
            // sources file
            std::string FileNameFromParam = Item->getParameters()["sources"];

            if (!FileNameFromParam.empty() &&
                !QFileInfo(QString::fromStdString(RunCtxt->getInputFullPath(FileNameFromParam))).exists())
            {
              m_CheckInfos.part(ProjectCheckInfos::PartInfo::PART_MODELPARAMS).updateStatus(
                ProjectStatusLevel::PRJ_ERROR);
              m_CheckInfos.part(ProjectCheckInfos::PartInfo::PART_MODELPARAMS)
                              .addMessage(tr("File %1 required by generator %2 does not exist")
                                          .arg(QString::fromStdString(FileNameFromParam))
                                          .arg(QString::fromStdString(ID)));
            }

            // distribution file
            FileNameFromParam = Item->getParameters()["distribution"];

            if (!FileNameFromParam.empty() &&
                !QFileInfo(QString::fromStdString(RunCtxt->getInputFullPath(FileNameFromParam))).exists())
            {
              m_CheckInfos.part(ProjectCheckInfos::PartInfo::PART_MODELPARAMS).updateStatus(
                ProjectStatusLevel::PRJ_ERROR);
              m_CheckInfos.part(ProjectCheckInfos::PartInfo::PART_MODELPARAMS)
                              .addMessage(tr("File %1 required by generator %2 does not exist")
                                          .arg(QString::fromStdString(FileNameFromParam))
                                          .arg(QString::fromStdString(ID)));
            }
          }
          if (Method == openfluid::fluidx::GeneratorDescriptor:: GeneratorMethod::INJECTMULTICOL)
          {
            // data file
            std::string FileNameFromParam = Item->getParameters()["datafile"];

            if (!FileNameFromParam.empty() &&
                !QFileInfo(QString::fromStdString(RunCtxt->getInputFullPath(FileNameFromParam))).exists())
            {
              m_CheckInfos.part(ProjectCheckInfos::PartInfo::PART_MODELPARAMS).updateStatus(
                ProjectStatusLevel::PRJ_ERROR);
              m_CheckInfos.part(ProjectCheckInfos::PartInfo::PART_MODELPARAMS)
                              .addMessage(tr("File %1 required by generator %2 does not exist")
                                          .arg(QString::fromStdString(FileNameFromParam))
                                          .arg(QString::fromStdString(ID)));
            }
          }
        }
        else
        {
          for (auto itFile = ReqFiles.begin(); itFile != ReqFiles.end(); ++itFile)
          {
            if (!QFileInfo(QString::fromStdString(RunCtxt->getInputFullPath(*itFile))).exists())
            {
              m_CheckInfos.part(ProjectCheckInfos::PartInfo::PART_MODELPARAMS).updateStatus(
                ProjectStatusLevel::PRJ_ERROR);
              m_CheckInfos.part(ProjectCheckInfos::PartInfo::PART_MODELPARAMS)
                              .addMessage(tr("File %1 required by generator %2 does not exist")
                                          .arg(QString::fromStdString((*itFile)))
                                          .arg(QString::fromStdString(ID)));
            }
          }
        }


        // check parameters


        // required parameters
        const auto& ReqParams = Sign->HandledData.RequiredParams;

        for (auto itParam = ReqParams.begin();itParam != ReqParams.end(); ++itParam)
        {
          m_SimulatorsParamsLists[QString::fromStdString(ID)] << QString::fromStdString(itParam->Name);

          if (!isParamSet(Item, itParam->Name))
          {
            m_CheckInfos.part(ProjectCheckInfos::PartInfo::PART_MODELPARAMS).updateStatus(
              ProjectStatusLevel::PRJ_ERROR);
            m_CheckInfos.part(ProjectCheckInfos::PartInfo::PART_MODELPARAMS)
                                        .addMessage(tr("Required parameter %1 for simulator %2 is not set")
                                                    .arg(QString::fromStdString(itParam->Name))
                                                    .arg(QString::fromStdString(ID)));
          }
        }


        // used  parameters
        const std::vector<openfluid::ware::SignatureDataItem>& UsParams = Sign->HandledData.UsedParams;

        for (auto itParam = UsParams.begin();itParam != UsParams.end(); ++itParam)
        {
          m_SimulatorsParamsLists[QString::fromStdString(ID)] << QString::fromStdString(itParam->Name);

          if (!isParamSet(Item, itParam->Name))
          {
            m_CheckInfos.part(ProjectCheckInfos::PartInfo::PART_MODELPARAMS).updateStatus(
              ProjectStatusLevel::PRJ_WARNING);
            m_CheckInfos.part(ProjectCheckInfos::PartInfo::PART_MODELPARAMS)
                                        .addMessage(tr("Used parameter %1 for simulator %2 is not set")
                                                    .arg(QString::fromStdString(itParam->Name))
                                                    .arg(QString::fromStdString(ID)));
          }
        }

        // check Generators Params

        bool RandomMinMaxChecked = false;
        bool InterpMinMaxChecked = false;
        bool InjectMinMaxChecked = false;

        if (Item->isType(openfluid::ware::WareType::GENERATOR))
        {
          openfluid::fluidx::GeneratorDescriptor::GeneratorMethod Method =
              (static_cast<const openfluid::machine::GeneratorSignature*>(Sign))->Method;

          if (Method == openfluid::fluidx::GeneratorDescriptor:: GeneratorMethod::RANDOM && !RandomMinMaxChecked)
          {
            checkGeneratorParam("min", "max", Item, ID);
            RandomMinMaxChecked = true;
          }
          else if (Method == openfluid::fluidx::GeneratorDescriptor:: GeneratorMethod::INTERP &&
              !InterpMinMaxChecked)
          {
            checkGeneratorParam("thresholdmin", "thresholdmax", Item, ID);
            InterpMinMaxChecked = true;
          }
          else if (Method == openfluid::fluidx::GeneratorDescriptor:: GeneratorMethod::INJECT &&
              !InjectMinMaxChecked)
          {
            checkGeneratorParam("thresholdmin", "thresholdmax", Item, ID);
            InjectMinMaxChecked = true;
          }
        }


        // populate updated units classes (created, modified)
        m_UpdatedUnitsClass.append(convertUpdatedUnitsClassesToQStringList(
          Sign->HandledUnitsGraph.UpdatedUnitsClasses));


        // check required attributes

        const auto& ReqData = Sign->HandledData.RequiredAttributes;

        for (auto itReqData = ReqData.begin(); itReqData != ReqData.end(); ++itReqData)
        {
          if (!Domain.isClassNameExists(itReqData->UnitsClass) &&
              !m_UpdatedUnitsClass.contains(QString::fromStdString(itReqData->UnitsClass)))
          {
            m_CheckInfos.part(ProjectCheckInfos::PartInfo::PART_SPATIALATTRS).updateStatus(
              ProjectStatusLevel::PRJ_ERROR);
            m_CheckInfos.part(ProjectCheckInfos::PartInfo::PART_SPATIALATTRS)
                                .addMessage(tr("Unit class %1 does not exist for attribute %2 required by %3")
                                            .arg(QString::fromStdString(itReqData->UnitsClass))
                                            .arg(QString::fromStdString(itReqData->Name))
                                            .arg(QString::fromStdString(ID)));
          }
          else if (!(Domain.getAttributesNames(itReqData->UnitsClass).count(itReqData->Name)
              || m_AttrsUnits.count(std::make_pair(itReqData->UnitsClass, itReqData->Name))))
          {
            m_CheckInfos.part(ProjectCheckInfos::PartInfo::PART_SPATIALATTRS).updateStatus(
              ProjectStatusLevel::PRJ_ERROR);
            m_CheckInfos.part(ProjectCheckInfos::PartInfo::PART_SPATIALATTRS)
                                .addMessage(tr("Attribute %1 required on %2 units by %3 does not exist")
                                            .arg(QString::fromStdString(itReqData->Name))
                                            .arg(QString::fromStdString(itReqData->UnitsClass))
                                            .arg(QString::fromStdString(ID)));
          }
        }


        // check produced attributes
        const auto& ProdData = Sign->SimulatorHandledData.ProducedAttributes;

        for (auto itProdData = ProdData.begin();
            itProdData != ProdData.end(); ++itProdData)
        {
          if (!Domain.isClassNameExists(itProdData->UnitsClass) &&
              !m_UpdatedUnitsClass.contains(QString::fromStdString(itProdData->UnitsClass)))
          {
            m_CheckInfos.part(ProjectCheckInfos::PartInfo::PART_SPATIALATTRS).updateStatus(
              ProjectStatusLevel::PRJ_ERROR);
            m_CheckInfos.part(ProjectCheckInfos::PartInfo::PART_SPATIALATTRS)
                                .addMessage(tr("Unit class %1 does not exist for attribute %2 produced by %3")
                                            .arg(QString::fromStdString(itProdData->UnitsClass))
                                            .arg(QString::fromStdString(itProdData->Name))
                                            .arg(QString::fromStdString(ID)));
          }

          if (!m_AttrsUnits.count(std::make_pair(itProdData->UnitsClass, itProdData->Name)))
          {
            m_AttrsUnits.insert(std::make_pair(itProdData->UnitsClass, itProdData->Name));
          }
          else
          {
            m_CheckInfos.part(ProjectCheckInfos::PartInfo::PART_SPATIALATTRS).updateStatus(
              ProjectStatusLevel::PRJ_ERROR);
            m_CheckInfos.part(ProjectCheckInfos::PartInfo::PART_SPATIALATTRS)
                                .addMessage(tr("Attribute %1 produced on %2 units by %3 "
                                    "is already produced by another simulator")
                                            .arg(QString::fromStdString(itProdData->UnitsClass))
                                            .arg(QString::fromStdString(itProdData->Name))
                                            .arg(QString::fromStdString(ID)));
          }
        }
      }
      else
      {
        m_CheckInfos.part(ProjectCheckInfos::PartInfo::PART_MODELDEF).updateStatus(
          ProjectStatusLevel::PRJ_ERROR);
        if (Item->getType() == openfluid::ware::WareType::SIMULATOR)
        {
          m_CheckInfos.part(ProjectCheckInfos::PartInfo::PART_MODELDEF).addMessage(tr("Simulator %1 is not available")
                                                                         .arg(QString::fromStdString(ID)));
        }
        else if (Item->getType() == openfluid::ware::WareType::GENERATOR)
        {
          m_CheckInfos.part(ProjectCheckInfos::PartInfo::PART_MODELDEF).addMessage(tr("Generator %1 is not available")
                                                                         .arg(QString::fromStdString(ID)));
        }
      }
    }
  }


  // ========== internal constraints

  /* Variables processing order is important
   A) first pass
     1) produced vars
     2) updated vars
   B) second pass
     3) required vars
   */

  // pass 1
  for (auto itModelItem = Items.begin(); itModelItem != Items.end(); ++itModelItem)
  {
    if ((*itModelItem)->isEnabled())
    {
      const auto& Container = Reg->wareOrGeneratorContainer((*itModelItem)->getID());

      if (Container.isValid() && Container.hasSignature())
      {
        const auto* Sign = Container.signature().get();
        std::string ID = Model.getID(*itModelItem);

        // check produced Vars
        const auto& ProdVars = Sign->SimulatorHandledData.ProducedVars;

        for (auto itData = ProdVars.begin(); itData != ProdVars.end(); ++itData)
        {
          m_VariablesNamesLists[QString::fromStdString(itData->UnitsClass)] << QString::fromStdString(itData->Name);

          if (!Domain.isClassNameExists(itData->UnitsClass) &&
              !m_UpdatedUnitsClass.contains(QString::fromStdString(itData->UnitsClass)))
          {
            m_CheckInfos.part(ProjectCheckInfos::PartInfo::PART_SPATIALSTRUCT).updateStatus(
              ProjectStatusLevel::PRJ_ERROR);
            m_CheckInfos.part(ProjectCheckInfos::PartInfo::PART_SPATIALSTRUCT)
                            .addMessage(tr("Unit class %1 does not exist for variable %2 produced by %3")
                                        .arg(QString::fromStdString(itData->UnitsClass))
                                        .arg(QString::fromStdString(itData->Name))
                                        .arg(QString::fromStdString(ID)));
          }

          if (!m_VarsUnits.count(std::make_pair(itData->UnitsClass, itData->Name)))
          {
            m_VarsUnits.insert(std::make_pair(itData->UnitsClass, itData->Name));
            m_TypedVarsUnits.insert(std::make_pair(itData->UnitsClass,
                                                 std::make_pair(itData->Name, itData->DataType)));
          }
          else
          {
            m_CheckInfos.part(ProjectCheckInfos::PartInfo::PART_MODELDEF).updateStatus(ProjectStatusLevel::PRJ_ERROR);
            m_CheckInfos.part(ProjectCheckInfos::PartInfo::PART_MODELDEF)
                            .addMessage(tr("Variable %1 on %2 produced by %3 is already produced "
                                           "by another simulator or generator")
                                        .arg(QString::fromStdString(itData->Name))
                                        .arg(QString::fromStdString(itData->UnitsClass))
                                        .arg(QString::fromStdString(ID)));
          }
        }

        // check updated vars
        const auto& UpVars = Sign->SimulatorHandledData.UpdatedVars;
        for (auto itData = UpVars.begin(); itData != UpVars.end(); ++itData)
        {
          m_VariablesNamesLists[QString::fromStdString(itData->UnitsClass)] << QString::fromStdString(itData->Name);

          if (!Domain.isClassNameExists(itData->UnitsClass) &&
              !m_UpdatedUnitsClass.contains(QString::fromStdString(itData->UnitsClass)))
          {
            m_CheckInfos.part(ProjectCheckInfos::PartInfo::PART_SPATIALSTRUCT).updateStatus(
              ProjectStatusLevel::PRJ_ERROR);
            m_CheckInfos.part(ProjectCheckInfos::PartInfo::PART_SPATIALSTRUCT)
                            .addMessage(tr("Unit class %1 does not exist for variable %2 produced by %3")
                                        .arg(QString::fromStdString(itData->UnitsClass))
                                        .arg(QString::fromStdString(itData->Name))
                                        .arg(QString::fromStdString(ID)));
          }

          if (!m_VarsUnits.count(std::make_pair(itData->UnitsClass, itData->Name)))
          {
            m_VarsUnits.insert(std::make_pair(itData->UnitsClass, itData->Name));
            m_TypedVarsUnits.insert(
                std::make_pair(itData->UnitsClass,
                               std::make_pair(itData->Name, itData->DataType)));
          }
        }
      }
    }
  }

  // pass 2
  for (auto itModelItem = Items.begin(); itModelItem != Items.end(); ++itModelItem)
  {
    if ((*itModelItem)->isEnabled())
    {
      std::string ID = Model.getID(*itModelItem);
      const auto& Container = Reg->wareOrGeneratorContainer((*itModelItem)->getID());

      if (Container.isValid() && Container.hasSignature())
      {

        const auto* Sign = Container.signature().get();

        // check required Vars
        const auto& ReqVars = Sign->HandledData.RequiredVars;

        for (auto itData = ReqVars.begin(); itData != ReqVars.end(); ++itData)
        {
          m_VariablesNamesLists[QString::fromStdString(itData->UnitsClass)] << QString::fromStdString(itData->Name);

          if (!Domain.isClassNameExists(itData->UnitsClass) &&
              !m_UpdatedUnitsClass.contains(QString::fromStdString(itData->UnitsClass)))
          {
            m_CheckInfos.part(ProjectCheckInfos::PartInfo::PART_SPATIALSTRUCT).updateStatus(
              ProjectStatusLevel::PRJ_ERROR);
            m_CheckInfos.part(ProjectCheckInfos::PartInfo::PART_SPATIALSTRUCT)
                            .addMessage(tr("Unit class %1 does not exist for variable %2 required by %3")
                                        .arg(QString::fromStdString(itData->UnitsClass))
                                        .arg(QString::fromStdString(itData->Name))
                                        .arg(QString::fromStdString(ID)));
          }

          if ((itData->DataType == openfluid::core::Value::NONE
              && !m_VarsUnits.count(std::make_pair(itData->UnitsClass, itData->Name)))
              || (itData->DataType != openfluid::core::Value::NONE && !m_TypedVarsUnits.count(
                  std::make_pair(itData->UnitsClass,
                                 std::make_pair(itData->Name, itData->DataType)))))
          {
            m_CheckInfos.part(ProjectCheckInfos::PartInfo::PART_MODELDEF).updateStatus(
              ProjectStatusLevel::PRJ_ERROR);
            m_CheckInfos.part(ProjectCheckInfos::PartInfo::PART_MODELDEF)
                            .addMessage(tr("Variable %1 on %2 required by %3 is not produced "
                                           "by another simulator or generator")
                                        .arg(QString::fromStdString(itData->Name))
                                        .arg(QString::fromStdString(itData->UnitsClass))
                                        .arg(QString::fromStdString(ID)));
          }
        }

        // check used Vars
        const auto& UsedVars = Sign->HandledData.UsedVars;

        for (auto itData = UsedVars.begin(); itData != UsedVars.end(); ++itData)
        {
          m_VariablesNamesLists[QString::fromStdString(itData->UnitsClass)] << QString::fromStdString(itData->Name);
        }
      }
    }
  }


  if (!AtLeastOneEnabled)
  {
    m_CheckInfos.part(ProjectCheckInfos::PartInfo::PART_MODELDEF).updateStatus(ProjectStatusLevel::PRJ_DISABLED);
    m_CheckInfos.part(ProjectCheckInfos::PartInfo::PART_MODELDEF).addMessage(tr("No simulator or generator "
                                                                                "is enabled in model"));
  }


  // sorting names lists

  m_SimulatorsIDsList.sort();

  for (auto& PList : m_SimulatorsParamsLists)
  {
    PList.sort();
  }

  for (auto& PList : m_VariablesNamesLists)
  {
    PList.removeDuplicates();
    PList.sort();
  }
}


// =====================================================================
// =====================================================================


void ProjectCentral::checkSpatialDomain()
{
  // Build list of units classes
  m_UnitsClassesList = openfluid::ui::toQStringList(m_FXDesc.spatialDomain().getClassNames());


  // Build list of attributes by units class
  m_AttributesLists.clear();
  for (const auto& UClass : m_UnitsClassesList)
  {
    m_AttributesLists[UClass] =
      openfluid::ui::toQStringList(m_FXDesc.spatialDomain().getAttributesNames(UClass.toStdString()));
    m_AttributesLists[UClass].sort();
  }
}


// =====================================================================
// =====================================================================


void ProjectCentral::checkDatastore()
{
  std::set<std::string> Classes = m_FXDesc.spatialDomain().getClassNames();

  const std::list<openfluid::fluidx::DatastoreItemDescriptor*>& Items = m_FXDesc.datastore().items();

  for (std::list<openfluid::fluidx::DatastoreItemDescriptor*>::const_iterator itDatastore = Items.begin();
       itDatastore != Items.end(); ++itDatastore)
  {
    std::string Class = (*itDatastore)->getUnitsClass();

    if (!Class.empty() && !Classes.count(Class))
    {
      m_CheckInfos.part(ProjectCheckInfos::PartInfo::PART_DATASTORE).updateStatus(ProjectStatusLevel::PRJ_WARNING);
      m_CheckInfos.part(ProjectCheckInfos::PartInfo::PART_DATASTORE).addMessage(tr("Unit class %1 does not exist "
                                                                         "for datastore item %2")
                                                                      .arg(QString::fromStdString(Class))
                                                                      .arg(QString::fromStdString((*itDatastore)
                                                                                                       ->getID())));
    }
  }
}


// =====================================================================
// =====================================================================


void ProjectCentral::checkMonitoring()
{
  openfluid::fluidx::MonitoringDescriptor& Monitoring = m_FXDesc.monitoring();
  const std::list<openfluid::fluidx::ObserverDescriptor*>& Items = Monitoring.items();

  auto Reg = openfluid::machine::ObserverRegistry::instance();

  bool AtLeastOneEnabled = false;

  for (const auto* Item : Items)
  {
    if (Item->isEnabled())
    {
      AtLeastOneEnabled = true;

      const std::string ID = Item->getID();
      const auto& Container = Reg->wareContainer(ID);
      
      if (Container.isValid() && Container.hasSignature())
      {

        // Factorizing parameters with model check not efficient
        const auto* Sign = Container.signature().get();

        // required parameters

        for (const auto& Param : Sign->HandledData.RequiredParams)
        {
          if (!isParamSet(Item, Param.Name))
          {
            m_CheckInfos.part(ProjectCheckInfos::PartInfo::PART_MONITORING).updateStatus(
              ProjectStatusLevel::PRJ_ERROR);
            m_CheckInfos.part(ProjectCheckInfos::PartInfo::PART_MONITORING)
                                        .addMessage(tr("Required parameter %1 for observer %2 is not set")
                                                    .arg(QString::fromStdString(Param.Name))
                                                    .arg(QString::fromStdString(ID)));
          }
        }

        // used  parameters
        for (const auto& Param : Sign->HandledData.UsedParams)
        {
          if (!isParamSet(Item, Param.Name))
          {
            m_CheckInfos.part(ProjectCheckInfos::PartInfo::PART_MONITORING).updateStatus(
              ProjectStatusLevel::PRJ_WARNING);
            m_CheckInfos.part(ProjectCheckInfos::PartInfo::PART_MONITORING)
                                        .addMessage(tr("Used parameter %1 for observer %2 is not set")
                                                    .arg(QString::fromStdString(Param.Name))
                                                    .arg(QString::fromStdString(ID)));
          }
        }

        // required attributes
        openfluid::fluidx::CoupledModelDescriptor& Model = m_FXDesc.model();
        openfluid::fluidx::SpatialDomainDescriptor& Domain = m_FXDesc.spatialDomain();

        for (const auto& Param : Sign->HandledData.RequiredAttributes)
        {
            if (!Domain.isClassNameExists(Param.UnitsClass) &&
                !m_UpdatedUnitsClass.contains(QString::fromStdString(Param.UnitsClass)))
            {
              m_CheckInfos.part(ProjectCheckInfos::PartInfo::PART_SPATIALATTRS).updateStatus(
                ProjectStatusLevel::PRJ_ERROR);
              m_CheckInfos.part(ProjectCheckInfos::PartInfo::PART_SPATIALATTRS)
                                  .addMessage(tr("Unit class %1 does not exist for attribute %2 required by %3")
                                              .arg(QString::fromStdString(Param.UnitsClass))
                                              .arg(QString::fromStdString(Param.Name))
                                              .arg(QString::fromStdString(ID)));
            }
          if (!(Domain.getAttributesNames(Param.UnitsClass).count(Param.Name)
              || m_AttrsUnits.count(std::make_pair(Param.UnitsClass, Param.Name))))
          {
            m_CheckInfos.part(ProjectCheckInfos::PartInfo::PART_MONITORING).updateStatus(
              ProjectStatusLevel::PRJ_ERROR);
            m_CheckInfos.part(ProjectCheckInfos::PartInfo::PART_MONITORING)
                                .addMessage(tr("Attribute %1 required on %2 units by %3 does not exist")
                                            .arg(QString::fromStdString(Param.Name))
                                            .arg(QString::fromStdString(Param.UnitsClass))
                                            .arg(QString::fromStdString(ID)));
          }
        }

        // required vars
        for (const auto& Param : Sign->HandledData.RequiredVars)
        {
          m_VariablesNamesLists[QString::fromStdString(Param.UnitsClass)] << QString::fromStdString(Param.Name);

            if (!Domain.isClassNameExists(Param.UnitsClass) &&
                !m_UpdatedUnitsClass.contains(QString::fromStdString(Param.UnitsClass)))
            {
              m_CheckInfos.part(ProjectCheckInfos::PartInfo::PART_MONITORING).updateStatus(
                ProjectStatusLevel::PRJ_ERROR);
              m_CheckInfos.part(ProjectCheckInfos::PartInfo::PART_MONITORING)
                              .addMessage(tr("Unit class %1 does not exist for variable %2 required by %3")
                                          .arg(QString::fromStdString(Param.UnitsClass))
                                          .arg(QString::fromStdString(Param.Name))
                                          .arg(QString::fromStdString(ID)));
            }

            if ((Param.DataType == openfluid::core::Value::NONE
                && !m_VarsUnits.count(std::make_pair(Param.UnitsClass, Param.Name)))
                || (Param.DataType != openfluid::core::Value::NONE && !m_TypedVarsUnits.count(
                    std::make_pair(Param.UnitsClass,
                                  std::make_pair(Param.Name, Param.DataType)))))
            {
              m_CheckInfos.part(ProjectCheckInfos::PartInfo::PART_MONITORING).updateStatus(
                ProjectStatusLevel::PRJ_ERROR);
              m_CheckInfos.part(ProjectCheckInfos::PartInfo::PART_MONITORING)
                              .addMessage(tr("Variable %1 on %2 required by %3 is not produced "
                                            "by any simulator or generator")
                                          .arg(QString::fromStdString(Param.Name))
                                          .arg(QString::fromStdString(Param.UnitsClass))
                                          .arg(QString::fromStdString(ID)));
            }
        }

        // required extra files
        openfluid::base::RunContextManager* RunCtxt = openfluid::base::RunContextManager::instance();
        for (const auto& File : Sign->HandledData.RequiredExtraFiles)
        {
          if (!QFileInfo(QString::fromStdString(RunCtxt->getInputFullPath(File))).exists())
          {
            m_CheckInfos.part(ProjectCheckInfos::PartInfo::PART_MODELPARAMS).updateStatus(
              ProjectStatusLevel::PRJ_ERROR);
            m_CheckInfos.part(ProjectCheckInfos::PartInfo::PART_MODELPARAMS)
                            .addMessage(tr("File %1 required by simulator %2 does not exist")
                                        .arg(QString::fromStdString(File))
                                        .arg(QString::fromStdString(ID)));
          }
        }
      }
      else
      {
        m_CheckInfos.part(ProjectCheckInfos::PartInfo::PART_MONITORING).updateStatus(ProjectStatusLevel::PRJ_ERROR);
        m_CheckInfos.part(ProjectCheckInfos::PartInfo::PART_MONITORING).addMessage(tr("Observer %1 is not available")
                                                                         .arg(QString::fromStdString(ID)));
      }

      // used data not displayed even as warning to avoid information overflow
    }
  }


  if (!AtLeastOneEnabled)
  {
    m_CheckInfos.part(ProjectCheckInfos::PartInfo::PART_MONITORING).updateStatus(ProjectStatusLevel::PRJ_WARNING);
    m_CheckInfos.part(ProjectCheckInfos::PartInfo::PART_MONITORING).addMessage(tr("No observer is enabled "
                                                                                  "in monitoring"));
  }
}


// =====================================================================
// =====================================================================


void ProjectCentral::checkRunConfig()
{
  // TODO check why this method is empty
}


// =====================================================================
// =====================================================================


QStringList ProjectCentral::getSimulatorsParamsList() const
{
  QStringList Names;

  QMapIterator<QString, QStringList> it(m_SimulatorsParamsLists);
  while (it.hasNext())
  {
    it.next();
    Names << it.value();
  }

  Names.removeDuplicates();
  Names.sort();

  return Names;
}


// =====================================================================
// =====================================================================


QStringList ProjectCentral::getVariablesNamesList() const
{
  QStringList Names;

  QMapIterator<QString, QStringList> it(m_VariablesNamesLists);
  while (it.hasNext())
  {
    it.next();
    Names << it.value();
  }

  Names.removeDuplicates();
  Names.sort();

  return Names;
}


// =====================================================================
// =====================================================================


QStringList ProjectCentral::getAttributesList() const
{
  QStringList Names;

  QMapIterator<QString, QStringList> it(m_AttributesLists);
  while (it.hasNext())
  {
    it.next();
    Names << it.value();
  }

  Names.removeDuplicates();
  Names.sort();

  return Names;
}


// =====================================================================
// =====================================================================


QStringList ProjectCentral::getAllNamesList() const
{
  QStringList Names;

  Names << m_SimulatorsIDsList;
  Names << m_ObserversIDsList;
  Names << getSimulatorsParamsList();
  Names << getVariablesNamesList();
  Names << m_UnitsClassesList;
  Names << getAttributesList();

  Names.removeDuplicates();
  Names.sort();

  return Names;
}


// =====================================================================
// =====================================================================


QString ProjectCentral::getRunModeStr(ProjectCentral::RunMode Mode)
{
  if (Mode == RunMode::DEFAULT)
  {
    return "default";
  }
  else if (Mode == RunMode::CLI)
  {
    return "cli";
  }

  return "";
}


// =====================================================================
// =====================================================================


ProjectCentral::RunMode ProjectCentral::getRunModeValue(const QString& Str)
{
  if (Str == "default" || Str == "")
  {
    return RunMode::DEFAULT;
  }
  else if (Str == "cli")
  {
    return RunMode::CLI;
  }

  return RunMode::UNDEFINED;
}
