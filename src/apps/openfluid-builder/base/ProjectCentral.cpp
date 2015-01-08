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

 @author Jean-Christophe Fabre <jean-christophe.fabre@supagro.inra.fr>
 */



#include <openfluid/base/RuntimeEnv.hpp>
#include <openfluid/base/ProjectManager.hpp>
#include <openfluid/ui/common/RunSimulationDialog.hpp>
#include <openfluid/machine/SimulatorSignatureRegistry.hpp>
#include <openfluid/machine/ObserverSignatureRegistry.hpp>
#include <openfluid/base/RuntimeEnv.hpp>
#include <openfluid/ware/GeneratorSignature.hpp>
#include <openfluid/machine/ModelItemInstance.hpp>
#include <openfluid/machine/ObserverInstance.hpp>
#include <openfluid/base/ApplicationException.hpp>
#include <openfluid/tools/SwissTools.hpp>

#include "ProjectCentral.hpp"
#include "AppTools.hpp"



ProjectCentral::ProjectCentral(QString PrjPath):
  mp_FXDesc(NULL),mp_AdvancedFXDesc(NULL)
{
  openfluid::base::RuntimeEnvironment::instance()->linkToProject();

  mp_FXDesc = new openfluid::fluidx::FluidXDescriptor(&m_IOListener);

  if (PrjPath == "")
  {
    setDefaultDescriptors();
  }
  else
  {
    try
    {
      mp_FXDesc->loadFromDirectory(openfluid::base::ProjectManager::instance()->getInputDir());
    }
    catch (openfluid::base::Exception& E)
    {
      //because we're in a constructor catch, so destructor is not called
      deleteData();
      throw;
    }
  }

  try
  {
    mp_AdvancedFXDesc = new openfluid::fluidx::AdvancedFluidXDescriptor(*mp_FXDesc);
  }
  catch (openfluid::base::Exception& E)
  {
    //because we're in a constructor catch, so destructor is not called
    deleteData();
    throw;
  }

  check();
}


// =====================================================================
// =====================================================================


ProjectCentral::~ProjectCentral()
{
  deleteData();
}


// =====================================================================
// =====================================================================


QStringList ProjectCentral::convertUpdatedUnitsClassesToQStringList(const std::vector<openfluid::ware::SignatureHandledUnitsClassItem>& UnitsClassesVector)
{
  QStringList QSL;
  for (unsigned int i=0; i<UnitsClassesVector.size();++i)
    QSL.append(QString::fromStdString(UnitsClassesVector[i].UnitsClass));

  return QSL;
}


// =====================================================================
// =====================================================================



void ProjectCentral::deleteData()
{
  if (mp_AdvancedFXDesc)
  {
    delete mp_AdvancedFXDesc;
    mp_AdvancedFXDesc = NULL;
  }

  if (mp_FXDesc)
  {
    delete mp_FXDesc;
    mp_FXDesc = NULL;
  }

  openfluid::base::RuntimeEnvironment::instance()->detachFromProject();
}


// =====================================================================
// =====================================================================


void ProjectCentral::setDefaultDescriptors()
{

}


// =====================================================================
// =====================================================================


void ProjectCentral::run()
{
  openfluid::ui::common::RunSimulationDialog RunDlg(QApplication::activeWindow(),mp_FXDesc);

  RunDlg.execute();

  openfluid::machine::SimulatorSignatureRegistry::instance()->unloadAllSimulators();
  openfluid::machine::ObserverSignatureRegistry::instance()->unloadAllObservers();
}


// =====================================================================
// =====================================================================


bool ProjectCentral::save()
{
  QString InputDir = QString(openfluid::base::ProjectManager::instance()->getInputDir().c_str());

  openfluid::base::ProjectManager::instance()->save();

  QDir InputPath(InputDir);

  // remove all fluidx files
  QStringList FluidXFileToRemove;
  FluidXFileToRemove = InputPath.entryList(QStringList("*.fluidx"),QDir::Files | QDir::NoDotAndDotDot);

  for (int i=0;i<FluidXFileToRemove.size();++i)
     InputPath.remove(FluidXFileToRemove[i]);

  // save all fluidx files
  mp_FXDesc->writeToManyFiles(InputDir.toStdString());

  return true;
}


// =====================================================================
// =====================================================================


bool ProjectCentral::saveAs(const QString& NewPrjName, const QString& NewPrjPath)
{
  openfluid::base::ProjectManager* PrjMan = openfluid::base::ProjectManager::instance();

  QString OldPrjPath = QString::fromStdString(PrjMan->getPath());

  // close current project
  PrjMan->close();

  // create new project dir
  QDir().mkpath(NewPrjPath);

  // copy files and subdirectories from current to renamed project
  openfluid::tools::CopyDirectoryContentsRecursively(OldPrjPath.toStdString(),NewPrjPath.toStdString());

  // open renamed project
  PrjMan->open(NewPrjPath.toStdString());

  openfluid::base::RuntimeEnvironment::instance()->linkToProject();

  // update project metadata
  PrjMan->setName(NewPrjName.toStdString());
  PrjMan->setCreationDateAsNow();

  // save project and project metadata
  PrjMan->save();
  save();

  return true;
}


// =====================================================================
// =====================================================================


void ProjectCentral::check()
{
  m_CheckInfos.clear();

  if (!mp_FXDesc->modelDescriptor().items().empty())
  {
    checkModel();
  }
  else
  {
    m_CheckInfos.part(ProjectCheckInfos::PART_MODELDEF).setStatus(PRJ_ERROR);
    m_CheckInfos.part(ProjectCheckInfos::PART_MODELDEF).addMessage(tr("Model is empty"));
  }

  checkSpatialDomain();

  checkDatastore();

  if (!mp_FXDesc->monitoringDescriptor().items().empty())
  {
    checkMonitoring();
  }
  else
  {
    m_CheckInfos.part(ProjectCheckInfos::PART_MONITORING).setStatus(PRJ_WARNING);
    m_CheckInfos.part(ProjectCheckInfos::PART_MONITORING).addMessage(tr("Monitoring is empty"));
  }

  checkRunConfig();
}


// =====================================================================
// =====================================================================


void ProjectCentral::checkGeneratorParam(const std::string& MinParamName,
                                         const std::string& MaxParamName,
                                         openfluid::fluidx::ModelItemDescriptor* Item,
                                         const std::string& ID)
{
  bool TestCompare = true;

  if (!isParamSet(Item, MinParamName))
    TestCompare = false;
  else if (!isParamIsDouble(Item, MinParamName))
  {
    TestCompare = false;

    m_CheckInfos.part(ProjectCheckInfos::PART_MODELPARAMS).setStatus(PRJ_ERROR);
    m_CheckInfos.part(ProjectCheckInfos::PART_MODELPARAMS)
                .addMessage(tr("Parameter %1 for %2 has to be a double precision value")
                            .arg(QString::fromStdString(MinParamName))
                            .arg(QString::fromStdString(ID)));
  }

  if (!isParamSet(Item, MaxParamName))
    TestCompare = false;
  else if (!isParamIsDouble(Item, MaxParamName))
  {
    TestCompare = false;
    m_CheckInfos.part(ProjectCheckInfos::PART_MODELPARAMS).setStatus(PRJ_ERROR);
    m_CheckInfos.part(ProjectCheckInfos::PART_MODELPARAMS)
                .addMessage(tr("Parameter %1 for %2 has to be a double precision value")
                            .arg(QString::fromStdString(MaxParamName))
                            .arg(QString::fromStdString(ID)));
  }

  if (TestCompare && !(getParamAsDouble(Item, MaxParamName)
      > getParamAsDouble(Item, MinParamName)))
  {
    m_CheckInfos.part(ProjectCheckInfos::PART_MODELPARAMS).setStatus(PRJ_ERROR);
    m_CheckInfos.part(ProjectCheckInfos::PART_MODELPARAMS)
                .addMessage(tr("Parameter %1 is greater or equal to parameter %2 for %3")
                            .arg(QString::fromStdString(MinParamName))
                            .arg(QString::fromStdString(MaxParamName))
                            .arg(QString::fromStdString(ID)));
  }
}


// =====================================================================
// =====================================================================


bool ProjectCentral::isParamSet(openfluid::fluidx::ModelItemDescriptor* Item,
                                const std::string& ParamName)
{
  return (!Item->getParameters()[ParamName].get().empty()
      || !mp_AdvancedFXDesc->model().getGlobalParameters()[ParamName].get().empty());
}


// =====================================================================
// =====================================================================


bool ProjectCentral::isParamIsDouble(openfluid::fluidx::ModelItemDescriptor* Item,
                                     const std::string& ParamName)
{
  if (!isParamSet(Item, ParamName))
    return false;

  double d;

  return (Item->getParameters()[ParamName].toDouble(d)
      || mp_AdvancedFXDesc->model().getGlobalParameters()[ParamName].toDouble(d));
}


// =====================================================================
// =====================================================================


double ProjectCentral::getParamAsDouble(openfluid::fluidx::ModelItemDescriptor* Item,
                                        const std::string& ParamName)
{
  if (!isParamIsDouble(Item, ParamName))
    throw openfluid::base::ApplicationException("openfluid-builder",
                                                "ProjectCentral::getParamAsDouble",
                                                "Parameter " + ParamName + " is not set as a double precision value");

  double d;

  if (Item->getParameters()[ParamName].toDouble(d))
    return d;

  mp_AdvancedFXDesc->model().getGlobalParameters()[ParamName].toDouble(d);

  return d;
}


// =====================================================================
// =====================================================================


void ProjectCentral::checkModel()
{
  openfluid::fluidx::AdvancedModelDescriptor& Model = mp_AdvancedFXDesc->model();
  openfluid::fluidx::AdvancedDomainDescriptor& Domain = mp_AdvancedFXDesc->spatialDomain();

  const std::list<openfluid::fluidx::ModelItemDescriptor*>& Items = Model.items();

  openfluid::machine::SimulatorSignatureRegistry* Reg = openfluid::machine::SimulatorSignatureRegistry::instance();
  openfluid::base::RuntimeEnvironment* RunEnv = openfluid::base::RuntimeEnvironment::instance();

  openfluid::ware::SimulatorSignature* Sign;

  QStringList UpdatedUnitsClass;

  std::set<std::pair<std::string, std::string> > AttrsUnits;

  bool AtLeastOneEnabled = false;


  for (std::list<openfluid::fluidx::ModelItemDescriptor*>::const_iterator itModelItem = Items.begin();
       itModelItem != Items.end(); ++itModelItem)
  {
    if ((*itModelItem)->isEnabled())
    {
      AtLeastOneEnabled = true;

      openfluid::machine::ModelItemSignatureInstance* SignII = Reg->signatureItemInstance(*itModelItem);
      std::string ID = Model.getID(*itModelItem);

      if (SignII != NULL)
      {
        Sign = SignII->Signature;


        // ========== external constraints

        // check extrafiles

        std::vector<std::string>& ReqFiles = Sign->HandledData.RequiredExtraFiles;

        if ((*itModelItem)->isType(openfluid::fluidx::WareDescriptor::Generator))
        {
          openfluid::fluidx::GeneratorDescriptor::GeneratorMethod Method =
              (static_cast<openfluid::fluidx::GeneratorDescriptor*>(*itModelItem))->getGeneratorMethod();

          if (Method == openfluid::fluidx::GeneratorDescriptor::Interp ||
              Method == openfluid::fluidx::GeneratorDescriptor::Inject)
          {
            // sources file
            std::string FileNameFromParam = (*itModelItem)->getParameters()["sources"];

            if (!FileNameFromParam.empty() &&
                !QFileInfo(QString::fromStdString(RunEnv->getInputFullPath(FileNameFromParam))).exists())
            {
              m_CheckInfos.part(ProjectCheckInfos::PART_MODELPARAMS).setStatus(PRJ_ERROR);
              m_CheckInfos.part(ProjectCheckInfos::PART_MODELPARAMS)
                          .addMessage(tr("File %1 required by generator %2 does not exist")
                                      .arg(QString::fromStdString(FileNameFromParam))
                                      .arg(QString::fromStdString(ID)));
            }

            // distribution file
            FileNameFromParam = (*itModelItem)->getParameters()["distribution"];

            if (!FileNameFromParam.empty() &&
                !QFileInfo(QString::fromStdString(RunEnv->getInputFullPath(FileNameFromParam))).exists())
            {
              m_CheckInfos.part(ProjectCheckInfos::PART_MODELPARAMS).setStatus(PRJ_ERROR);
              m_CheckInfos.part(ProjectCheckInfos::PART_MODELPARAMS)
                          .addMessage(tr("File %1 required by generator %2 does not exist")
                                      .arg(QString::fromStdString(FileNameFromParam))
                                      .arg(QString::fromStdString(ID)));
            }
          }
        }
        else
        {
          for (std::vector<std::string>::iterator itFile = ReqFiles.begin(); itFile != ReqFiles.end(); ++itFile)
          {
            if (!QFileInfo(QString::fromStdString(RunEnv->getInputFullPath(*itFile))).exists())
            {
              m_CheckInfos.part(ProjectCheckInfos::PART_MODELPARAMS).setStatus(PRJ_ERROR);
              m_CheckInfos.part(ProjectCheckInfos::PART_MODELPARAMS)
                          .addMessage(tr("File %1 required by generator %2 does not exist")
                                      .arg(QString::fromStdString((*itFile)))
                                      .arg(QString::fromStdString(ID)));
            }
          }
        }


        // check parameters

        bool RandomMinMaxChecked = false;
        bool InterpMinMaxChecked = false;
        bool InjectMinMaxChecked = false;

        std::vector<openfluid::ware::SignatureHandledDataItem>& Params = Sign->HandledData.SimulatorParams;

        for (std::vector<openfluid::ware::SignatureHandledDataItem>::iterator itParam = Params.begin();
             itParam != Params.end(); ++itParam)
        {
          if (!isParamSet(*itModelItem, itParam->DataName))
          {
            if ((*itModelItem)->isType(openfluid::fluidx::ModelItemDescriptor::Generator) &&
                itParam->DataName != "thresholdmin" &&
                itParam->DataName != "thresholdmax" &&
                itParam->DataName != "deltat")
            {
              m_CheckInfos.part(ProjectCheckInfos::PART_MODELPARAMS).setStatus(PRJ_ERROR);
            }
            else
            {
              m_CheckInfos.part(ProjectCheckInfos::PART_MODELPARAMS).setStatus(PRJ_WARNING);
            }

            m_CheckInfos.part(ProjectCheckInfos::PART_MODELPARAMS)
                        .addMessage(tr("Parameter %1 for generator %2 is not set")
                                    .arg(QString::fromStdString(itParam->DataName))
                                    .arg(QString::fromStdString(ID)));
          }

          // check Generators Params
          if ((*itModelItem)->isType(openfluid::fluidx::ModelItemDescriptor::Generator))
          {
            openfluid::fluidx::GeneratorDescriptor::GeneratorMethod Method =
                (static_cast<openfluid::ware::GeneratorSignature*>(Sign))->m_GeneratorMethod;

            if (Method == openfluid::fluidx::GeneratorDescriptor::Random && !RandomMinMaxChecked)
            {
              checkGeneratorParam("min", "max", *itModelItem, ID);
              RandomMinMaxChecked = true;
            }
            else if (Method == openfluid::fluidx::GeneratorDescriptor::Interp &&
                     !InterpMinMaxChecked)
            {
              checkGeneratorParam("thresholdmin", "thresholdmax", *itModelItem, ID);
              InterpMinMaxChecked = true;
            }
            else if (Method == openfluid::fluidx::GeneratorDescriptor::Inject &&
                     !InjectMinMaxChecked)
            {
              checkGeneratorParam("thresholdmin", "thresholdmax", *itModelItem, ID);
              InjectMinMaxChecked = true;
            }
          }
        }


        // populate updated units classes (created, modified)
        UpdatedUnitsClass.append(convertUpdatedUnitsClassesToQStringList(Sign->HandledUnitsGraph.UpdatedUnitsClass));


        // check required attributes

        std::vector<openfluid::ware::SignatureHandledDataItem>& ReqData = Sign->HandledData.RequiredAttribute;

        for (std::vector<openfluid::ware::SignatureHandledDataItem>::iterator itReqData = ReqData.begin();
             itReqData != ReqData.end(); ++itReqData)
        {
          if (!Domain.isClassNameExists(itReqData->UnitClass) &&
              !UpdatedUnitsClass.contains(QString::fromStdString(itReqData->UnitClass)))
          {
            m_CheckInfos.part(ProjectCheckInfos::PART_SPATIALATTRS).setStatus(PRJ_ERROR);
            m_CheckInfos.part(ProjectCheckInfos::PART_SPATIALATTRS)
                        .addMessage(tr("Unit class %1 does not exist for attribute %2 required by %3")
                                    .arg(QString::fromStdString(itReqData->UnitClass))
                                    .arg(QString::fromStdString(itReqData->DataName))
                                    .arg(QString::fromStdString(ID)));
          }
          else if (!(Domain.getAttributesNames(itReqData->UnitClass).count(itReqData->DataName)
                   || AttrsUnits.count(std::make_pair(itReqData->UnitClass, itReqData->DataName))))
          {
            m_CheckInfos.part(ProjectCheckInfos::PART_SPATIALATTRS).setStatus(PRJ_ERROR);
            m_CheckInfos.part(ProjectCheckInfos::PART_SPATIALATTRS)
                        .addMessage(tr("Attribute %1 required on %2 units by %3 does not exist")
                                    .arg(QString::fromStdString(itReqData->DataName))
                                    .arg(QString::fromStdString(itReqData->UnitClass))
                                    .arg(QString::fromStdString(ID)));
          }
        }


        // check produced attributes
        std::vector<openfluid::ware::SignatureHandledDataItem>& ProdData = Sign->HandledData.ProducedAttribute;

        for (std::vector<openfluid::ware::SignatureHandledDataItem>::iterator itProdData = ProdData.begin();
             itProdData != ProdData.end(); ++itProdData)
        {
          if (!Domain.isClassNameExists(itProdData->UnitClass) &&
              !UpdatedUnitsClass.contains(QString::fromStdString(itProdData->UnitClass)))
          {
            m_CheckInfos.part(ProjectCheckInfos::PART_SPATIALATTRS).setStatus(PRJ_ERROR);
            m_CheckInfos.part(ProjectCheckInfos::PART_SPATIALATTRS)
                        .addMessage(tr("Unit class %1 does not exist for attribute %2 produced by %3")
                                    .arg(QString::fromStdString(itProdData->UnitClass))
                                    .arg(QString::fromStdString(itProdData->DataName))
                                    .arg(QString::fromStdString(ID)));
          }

          if (!AttrsUnits.count(std::make_pair(itProdData->UnitClass, itProdData->DataName)))
          {
            AttrsUnits.insert(std::make_pair(itProdData->UnitClass, itProdData->DataName));
          }
          else
          {
            m_CheckInfos.part(ProjectCheckInfos::PART_SPATIALATTRS).setStatus(PRJ_ERROR);
            m_CheckInfos.part(ProjectCheckInfos::PART_SPATIALATTRS)
                        .addMessage(tr("Attribute %1 produced on %2 units by %3 is already produced by another simulator")
                                    .arg(QString::fromStdString(itProdData->UnitClass))
                                    .arg(QString::fromStdString(itProdData->DataName))
                                    .arg(QString::fromStdString(ID)));
          }
        }
      }
      else
      {
        m_CheckInfos.part(ProjectCheckInfos::PART_MODELDEF).setStatus(PRJ_ERROR);
        if ((*itModelItem)->getType() == openfluid::fluidx::ModelItemDescriptor::PluggedSimulator)
          m_CheckInfos.part(ProjectCheckInfos::PART_MODELDEF).addMessage(tr("Simulator %1 is not available")
                                                                         .arg(QString::fromStdString(ID)));
      }
    }
  }



  // ========== internal constraints

  std::vector<openfluid::ware::SignatureHandledTypedDataItem>::iterator itData;

  std::set<std::pair<std::string, std::string> > VarsUnits;
  std::set<std::pair<std::string,std::pair<std::string, openfluid::core::Value::Type> > > TypedVarsUnits;

  /* Variables processing order is important
   A) first pass
     1) produced vars
     2) updated vars
   B) second pass
     3) required vars
   */

  // pass 1
  for (std::list<openfluid::fluidx::ModelItemDescriptor*>::const_iterator itModelItem = Items.begin();
       itModelItem != Items.end(); ++itModelItem)
  {
    if ((*itModelItem)->isEnabled())
    {
      openfluid::machine::ModelItemSignatureInstance* SignII = Reg->signatureItemInstance(*itModelItem);
      std::string ID = Model.getID(*itModelItem);

      if (SignII != NULL)
      {
        Sign = SignII->Signature;
        std::string ID = Model.getID(*itModelItem);

        // check produced Vars
        std::vector<openfluid::ware::SignatureHandledTypedDataItem>& ProdVars = Sign->HandledData.ProducedVars;

        for (itData = ProdVars.begin(); itData != ProdVars.end(); ++itData)
        {

          if (!Domain.isClassNameExists(itData->UnitClass) &&
              !UpdatedUnitsClass.contains(QString::fromStdString(itData->UnitClass)))
          {
            m_CheckInfos.part(ProjectCheckInfos::PART_SPATIALSTRUCT).setStatus(PRJ_ERROR);
            m_CheckInfos.part(ProjectCheckInfos::PART_SPATIALSTRUCT)
                        .addMessage(tr("Unit class %1 does not exist for variable %2 produced by %3")
                                    .arg(QString::fromStdString(itData->UnitClass))
                                    .arg(QString::fromStdString(itData->DataName))
                                    .arg(QString::fromStdString(ID)));
          }

          if (!VarsUnits.count(std::make_pair(itData->UnitClass, itData->DataName)))
          {
            VarsUnits.insert(std::make_pair(itData->UnitClass, itData->DataName));
            TypedVarsUnits.insert(std::make_pair(itData->UnitClass,
                                                 std::make_pair(itData->DataName, itData->DataType)));
          }
          else
          {
            m_CheckInfos.part(ProjectCheckInfos::PART_MODELDEF).setStatus(PRJ_ERROR);
            m_CheckInfos.part(ProjectCheckInfos::PART_MODELDEF)
                        .addMessage(tr("Variable %1 on %2 produced by %3 cannot be created because it is created by another simulator or generator")
                                    .arg(QString::fromStdString(itData->DataName))
                                    .arg(QString::fromStdString(itData->UnitClass))
                                    .arg(QString::fromStdString(ID)));
          }
        }

        if ((*itModelItem)->isType(openfluid::fluidx::WareDescriptor::Generator))
        {
          openfluid::fluidx::GeneratorDescriptor* GenDesc =
              dynamic_cast<openfluid::fluidx::GeneratorDescriptor*>(*itModelItem);

          if (!Domain.isClassNameExists(GenDesc->getUnitClass()) &&
              !UpdatedUnitsClass.contains(QString::fromStdString(GenDesc->getUnitClass())))
          {
            m_CheckInfos.part(ProjectCheckInfos::PART_SPATIALSTRUCT).setStatus(PRJ_ERROR);
            m_CheckInfos.part(ProjectCheckInfos::PART_SPATIALSTRUCT)
                        .addMessage(tr("Unit class %1 does not exist for variable %2 produced by %3")
                                    .arg(QString::fromStdString(GenDesc->getUnitClass()))
                                    .arg(QString::fromStdString(GenDesc->getVariableName()))
                                    .arg(QString::fromStdString(ID)));
          }

          if (!VarsUnits.count(std::make_pair(GenDesc->getUnitClass(), GenDesc->getVariableName())))
          {
            VarsUnits.insert(std::make_pair(GenDesc->getUnitClass(),GenDesc->getVariableName()));
            TypedVarsUnits.insert(std::make_pair(GenDesc->getUnitClass(),
                                                 std::make_pair(GenDesc->getVariableName(),GenDesc->getVariableType())));
          }
          else
          {
            m_CheckInfos.part(ProjectCheckInfos::PART_MODELDEF).setStatus(PRJ_ERROR);
            m_CheckInfos.part(ProjectCheckInfos::PART_MODELDEF)
                        .addMessage(tr("Variable %1 on %2 produced by %3 is already produced by another simulator or generator")
                                    .arg(QString::fromStdString(GenDesc->getVariableName()))
                                    .arg(QString::fromStdString(GenDesc->getUnitClass()))
                                    .arg(QString::fromStdString(ID)));
          }
        }

        // check updated vars
        std::vector<openfluid::ware::SignatureHandledTypedDataItem>& UpVars =
            Sign->HandledData.UpdatedVars;
        for (itData = UpVars.begin(); itData != UpVars.end(); ++itData)
        {
          if (!Domain.isClassNameExists(itData->UnitClass) &&
              !UpdatedUnitsClass.contains(QString::fromStdString(itData->UnitClass)))
          {
            m_CheckInfos.part(ProjectCheckInfos::PART_SPATIALSTRUCT).setStatus(PRJ_ERROR);
            m_CheckInfos.part(ProjectCheckInfos::PART_SPATIALSTRUCT)
                        .addMessage(tr("Unit class %1 does not exist for variable %2 produced by %3")
                                    .arg(QString::fromStdString(itData->UnitClass))
                                    .arg(QString::fromStdString(itData->DataName))
                                    .arg(QString::fromStdString(ID)));
          }

          if (!VarsUnits.count(std::make_pair(itData->UnitClass, itData->DataName)))
          {
            VarsUnits.insert(std::make_pair(itData->UnitClass, itData->DataName));
            TypedVarsUnits.insert(
                std::make_pair(itData->UnitClass,
                               std::make_pair(itData->DataName, itData->DataType)));
          }
        }
      }
    }
  }

  // pass 2
  for (std::list<openfluid::fluidx::ModelItemDescriptor*>::const_iterator itModelItem = Items.begin();
       itModelItem != Items.end(); ++itModelItem)
  {
    if ((*itModelItem)->isEnabled())
    {
      openfluid::machine::ModelItemSignatureInstance* SignII = Reg->signatureItemInstance(*itModelItem);
      std::string ID = Model.getID(*itModelItem);

      if (SignII != NULL)
      {

        Sign = SignII->Signature;

        // check required Vars
        std::vector<openfluid::ware::SignatureHandledTypedDataItem>& ReqVars = Sign->HandledData.RequiredVars;

        for (itData = ReqVars.begin(); itData != ReqVars.end(); ++itData)
        {
          if (!Domain.isClassNameExists(itData->UnitClass) &&
              !UpdatedUnitsClass.contains(QString::fromStdString(itData->UnitClass)))
          {
            m_CheckInfos.part(ProjectCheckInfos::PART_SPATIALSTRUCT).setStatus(PRJ_ERROR);
            m_CheckInfos.part(ProjectCheckInfos::PART_SPATIALSTRUCT)
                        .addMessage(tr("Unit class %1 does not exist for variable %2 required by %3")
                                    .arg(QString::fromStdString(itData->UnitClass))
                                    .arg(QString::fromStdString(itData->DataName))
                                    .arg(QString::fromStdString(ID)));
          }

          if ((itData->DataType == openfluid::core::Value::NONE
              && !VarsUnits.count(std::make_pair(itData->UnitClass, itData->DataName)))
              || (itData->DataType != openfluid::core::Value::NONE && !TypedVarsUnits.count(
                  std::make_pair(itData->UnitClass,
                                 std::make_pair(itData->DataName, itData->DataType)))))
          {
            m_CheckInfos.part(ProjectCheckInfos::PART_MODELDEF).setStatus(PRJ_ERROR);
            m_CheckInfos.part(ProjectCheckInfos::PART_MODELDEF)
                        .addMessage(tr("Variable %1 on %2 required by %3 is not produced by another simulator or generator")
                                    .arg(QString::fromStdString(itData->DataName))
                                    .arg(QString::fromStdString(itData->UnitClass))
                                    .arg(QString::fromStdString(ID)));
          }
        }
      }
    }
  }


  if (!AtLeastOneEnabled)
  {
    m_CheckInfos.part(ProjectCheckInfos::PART_MODELDEF).setStatus(PRJ_ERROR);
    m_CheckInfos.part(ProjectCheckInfos::PART_MODELDEF).addMessage(tr("No simulator or generator is enabled in model"));
  }
}


// =====================================================================
// =====================================================================


void ProjectCentral::checkSpatialDomain()
{

}


// =====================================================================
// =====================================================================


void ProjectCentral::checkDatastore()
{
  std::set<std::string> Classes = mp_AdvancedFXDesc->spatialDomain().getClassNames();

  const std::list<openfluid::fluidx::DatastoreItemDescriptor*>& Items = mp_AdvancedFXDesc->datastore().items();

  for (std::list<openfluid::fluidx::DatastoreItemDescriptor*>::const_iterator itDatastore = Items.begin();
       itDatastore != Items.end(); ++itDatastore)
  {
    std::string Class = (*itDatastore)->getUnitClass();

    if (!Class.empty() && !Classes.count(Class))
    {
      m_CheckInfos.part(ProjectCheckInfos::PART_DATASTORE).setStatus(PRJ_WARNING);
      m_CheckInfos.part(ProjectCheckInfos::PART_DATASTORE).addMessage(tr("Unit class %1 does not exist for datastore item %2")
                                                                      .arg(QString::fromStdString(Class))
                                                                      .arg(QString::fromStdString((*itDatastore)->getID())));
    }
  }
}


// =====================================================================
// =====================================================================


void ProjectCentral::checkMonitoring()
{
  openfluid::fluidx::AdvancedMonitoringDescriptor& Monitoring = mp_AdvancedFXDesc->monitoring();
  const std::list<openfluid::fluidx::ObserverDescriptor*>& Items = Monitoring.items();

  openfluid::machine::ObserverSignatureRegistry* Reg = openfluid::machine::ObserverSignatureRegistry::instance();

  bool AtLeastOneEnabled = false;


  for (std::list<openfluid::fluidx::ObserverDescriptor*>::const_iterator itMonitoring = Items.begin();
       itMonitoring != Items.end(); ++itMonitoring)
  {
    if ((*itMonitoring)->isEnabled())
    {
      AtLeastOneEnabled = true;

      const openfluid::machine::ObserverSignatureInstance* SignII = Reg->signature((*itMonitoring)->getID());

      if (SignII == NULL)
      {
        m_CheckInfos.part(ProjectCheckInfos::PART_MONITORING).setStatus(PRJ_ERROR);
        m_CheckInfos.part(ProjectCheckInfos::PART_MONITORING).addMessage(tr("Observer %1 is not available")
                                                                         .arg(QString::fromStdString((*itMonitoring)->getID())));
      }
    }
  }


  if (!AtLeastOneEnabled)
  {
    m_CheckInfos.part(ProjectCheckInfos::PART_MONITORING).setStatus(PRJ_WARNING);
    m_CheckInfos.part(ProjectCheckInfos::PART_MONITORING).addMessage(tr("No observer is enabled in monitoring"));
  }
}


// =====================================================================
// =====================================================================


void ProjectCentral::checkRunConfig()
{

}
