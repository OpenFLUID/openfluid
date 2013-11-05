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
 \file ProjectCentral.cpp

 \author Jean-Christophe Fabre <fabrejc@supagro.inra.fr>
 */



#include <openfluid/base/RuntimeEnv.hpp>
#include <openfluid/base/ProjectManager.hpp>
#include <openfluid/guicommon/RunSimulationDialog.hpp>
#include <openfluid/machine/SimulatorSignatureRegistry.hpp>
#include <openfluid/machine/ObserverSignatureRegistry.hpp>
#include <openfluid/base/RuntimeEnv.hpp>
#include <openfluid/ware/SimulatorSignature.hpp>
#include <openfluid/ware/GeneratorSignature.hpp>
#include <openfluid/machine/ModelItemInstance.hpp>
#include <openfluid/machine/ObserverInstance.hpp>
#include <openfluid/base/ApplicationException.hpp>

#include "ProjectCentral.hpp"



ProjectCentral::ProjectCentral(QString PrjPath):
  mp_FXDesc(NULL),mp_AdvancedFXDesc(NULL)
{
  openfluid::base::RuntimeEnvironment::getInstance()->linkToProject();

  mp_FXDesc = new openfluid::fluidx::FluidXDescriptor(&m_IOListener);

  if (PrjPath == "")
  {
    setDefaultDescriptors();
  }
  else
  {
    try
    {
      mp_FXDesc->loadFromDirectory(openfluid::base::ProjectManager::getInstance()->getInputDir());


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


void ProjectCentral::deleteData()
{
  if (mp_AdvancedFXDesc == NULL)
  {
    delete mp_AdvancedFXDesc;
    mp_AdvancedFXDesc = NULL;
  }

  if (mp_FXDesc == NULL)
  {
    delete mp_FXDesc;
    mp_FXDesc = NULL;
  }

  openfluid::base::RuntimeEnvironment::getInstance()->detachFromProject();
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
  openfluid::guicommon::RunSimulationDialog RunDlg(QApplication::activeWindow(),mp_FXDesc);

  RunDlg.execute();
}


// =====================================================================
// =====================================================================


bool ProjectCentral::save()
{
  QString InputDir = QString(openfluid::base::ProjectManager::getInstance()->getInputDir().c_str());

  openfluid::base::ProjectManager::getInstance()->save();

  QDir InputPath(InputDir);

  QStringList FluidXFileToRemove;
  FluidXFileToRemove = InputPath.entryList(QStringList("*.fluidx"),QDir::Files | QDir::NoDotAndDotDot);

  for (int i=0;i<FluidXFileToRemove.size();++i)
     InputPath.remove(FluidXFileToRemove[i]);

  mp_FXDesc->writeToManyFiles(InputDir.toStdString());

  return true;
}


// =====================================================================
// =====================================================================


void ProjectCentral::check()
{
  m_CheckInfos.clear();

  if (!mp_FXDesc->getModelDescriptor().getItems().empty())
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

  if (!mp_FXDesc->getMonitoringDescriptor().getItems().empty())
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
      || !mp_AdvancedFXDesc->getModel().getGlobalParameters()[ParamName].get().empty());
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
      || mp_AdvancedFXDesc->getModel().getGlobalParameters()[ParamName].toDouble(d));
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

  mp_AdvancedFXDesc->getModel().getGlobalParameters()[ParamName].toDouble(d);

  return d;
}


// =====================================================================
// =====================================================================


void ProjectCentral::checkModel()
{
  openfluid::fluidx::AdvancedModelDescriptor& Model = mp_AdvancedFXDesc->getModel();
  openfluid::fluidx::AdvancedDomainDescriptor& Domain = mp_AdvancedFXDesc->getDomain();

  const std::list<openfluid::fluidx::ModelItemDescriptor*>& Items = Model.getItems();

  openfluid::machine::SimulatorSignatureRegistry* Reg = openfluid::machine::SimulatorSignatureRegistry::getInstance();
  openfluid::base::RuntimeEnvironment* RunEnv = openfluid::base::RuntimeEnvironment::getInstance();

  openfluid::ware::SimulatorSignature* Sign;

  std::set<std::pair<std::string, std::string> > AttrsUnits;


  for (std::list<openfluid::fluidx::ModelItemDescriptor*>::const_iterator it = Items.begin(); it != Items.end(); ++it)
  {
    if ((*it)->isEnabled())
    {

      openfluid::machine::ModelItemSignatureInstance* SignII = Reg->getSignatureItemInstance(*it);
      std::string ID = Model.getID(*it);

      if (SignII != NULL)
      {
        Sign = SignII->Signature;


        // ========== external constraints

        // check extrafiles

        std::vector<std::string>& ReqFiles = Sign->HandledData.RequiredExtraFiles;

        if ((*it)->isType(openfluid::fluidx::WareDescriptor::Generator))
        {
          openfluid::fluidx::GeneratorDescriptor::GeneratorMethod Method =
              (static_cast<openfluid::fluidx::GeneratorDescriptor*>(*it))->getGeneratorMethod();

          if (Method == openfluid::fluidx::GeneratorDescriptor::Interp ||
              Method == openfluid::fluidx::GeneratorDescriptor::Inject)
          {
            // sources file
            std::string FileNameFromParam = (*it)->getParameters()["sources"];

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
            FileNameFromParam = (*it)->getParameters()["distribution"];

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
          for (std::vector<std::string>::iterator itt = ReqFiles.begin(); itt != ReqFiles.end(); ++itt)
          {
            if (!QFileInfo(QString::fromStdString(RunEnv->getInputFullPath(*itt))).exists())
            {
              m_CheckInfos.part(ProjectCheckInfos::PART_MODELPARAMS).setStatus(PRJ_ERROR);
              m_CheckInfos.part(ProjectCheckInfos::PART_MODELPARAMS)
                          .addMessage(tr("File %1 required by generator %2 does not exist")
                                      .arg(QString::fromStdString((*itt)))
                                      .arg(QString::fromStdString(ID)));
            }
          }
        }


        // check parameters

        bool RandomMinMaxChecked = false;
        bool InterpMinMaxChecked = false;
        bool InjectMinMaxChecked = false;

        std::vector<openfluid::ware::SignatureHandledDataItem>& Params = Sign->HandledData.SimulatorParams;

        for (std::vector<openfluid::ware::SignatureHandledDataItem>::iterator itt = Params.begin(); itt != Params.end(); ++itt)
        {
          if (!isParamSet(*it, itt->DataName))
          {
            if ((*it)->isType(openfluid::fluidx::ModelItemDescriptor::Generator) &&
                itt->DataName != "thresholdmin" &&
                itt->DataName != "thresholdmax" &&
                itt->DataName != "deltat")
            {
              m_CheckInfos.part(ProjectCheckInfos::PART_MODELPARAMS).setStatus(PRJ_ERROR);
            }
            else
            {
              m_CheckInfos.part(ProjectCheckInfos::PART_MODELPARAMS).setStatus(PRJ_WARNING);
            }

            m_CheckInfos.part(ProjectCheckInfos::PART_MODELPARAMS)
                        .addMessage(tr("Parameter %1 for generator %2 is not set")
                                    .arg(QString::fromStdString(itt->DataName))
                                    .arg(QString::fromStdString(ID)));
          }

          // check Generators Params
          if ((*it)->isType(openfluid::fluidx::ModelItemDescriptor::Generator))
          {
            openfluid::fluidx::GeneratorDescriptor::GeneratorMethod Method =
                (static_cast<openfluid::ware::GeneratorSignature*>(Sign))->m_GeneratorMethod;

            if (Method == openfluid::fluidx::GeneratorDescriptor::Random && !RandomMinMaxChecked)
            {
              checkGeneratorParam("min", "max", *it, ID);
              RandomMinMaxChecked = true;
            }
            else if (Method == openfluid::fluidx::GeneratorDescriptor::Interp &&
                     !InterpMinMaxChecked)
            {
              checkGeneratorParam("thresholdmin", "thresholdmax", *it, ID);
              InterpMinMaxChecked = true;
            }
            else if (Method == openfluid::fluidx::GeneratorDescriptor::Inject &&
                     !InjectMinMaxChecked)
            {
              checkGeneratorParam("thresholdmin", "thresholdmax", *it, ID);
              InjectMinMaxChecked = true;
            }
          }
        }



        // check required attributes

        std::vector<openfluid::ware::SignatureHandledDataItem>& ReqData = Sign->HandledData.RequiredAttribute;

        for (std::vector<openfluid::ware::SignatureHandledDataItem>::iterator itt = ReqData.begin(); itt != ReqData.end(); ++itt)
        {
          if (!Domain.isClassNameExists(itt->UnitClass))
          {
            m_CheckInfos.part(ProjectCheckInfos::PART_SPATIALATTRS).setStatus(PRJ_ERROR);
            m_CheckInfos.part(ProjectCheckInfos::PART_SPATIALATTRS)
                        .addMessage(tr("Unit class %1 does not exist for attribute %2 required by %3")
                                    .arg(QString::fromStdString(itt->UnitClass))
                                    .arg(QString::fromStdString(itt->DataName))
                                    .arg(QString::fromStdString(ID)));
          }
          else if (!(Domain.getAttributesNames(itt->UnitClass).count(itt->DataName)
                   || AttrsUnits.count(std::make_pair(itt->UnitClass, itt->DataName))))
          {
            m_CheckInfos.part(ProjectCheckInfos::PART_SPATIALATTRS).setStatus(PRJ_ERROR);
            m_CheckInfos.part(ProjectCheckInfos::PART_SPATIALATTRS)
                        .addMessage(tr("Attribute %1 required on %2 units by %3 does not exist")
                                    .arg(QString::fromStdString(itt->DataName))
                                    .arg(QString::fromStdString(itt->UnitClass))
                                    .arg(QString::fromStdString(ID)));
          }
        }


        // check produced attributes
        std::vector<openfluid::ware::SignatureHandledDataItem>& ProdData = Sign->HandledData.ProducedAttribute;

        for (std::vector<openfluid::ware::SignatureHandledDataItem>::iterator itt = ProdData.begin(); itt != ProdData.end(); ++itt)
        {
          if (!Domain.isClassNameExists(itt->UnitClass))
          {
            m_CheckInfos.part(ProjectCheckInfos::PART_SPATIALATTRS).setStatus(PRJ_ERROR);
            m_CheckInfos.part(ProjectCheckInfos::PART_SPATIALATTRS)
                        .addMessage(tr("Unit class %1 does not exist for attribute %2 produced by %3")
                                    .arg(QString::fromStdString(itt->UnitClass))
                                    .arg(QString::fromStdString(itt->DataName))
                                    .arg(QString::fromStdString(ID)));
          }

          if (!AttrsUnits.count(std::make_pair(itt->UnitClass, itt->DataName)))
          {
            AttrsUnits.insert(std::make_pair(itt->UnitClass, itt->DataName));
          }
          else
          {
            m_CheckInfos.part(ProjectCheckInfos::PART_SPATIALATTRS).setStatus(PRJ_ERROR);
            m_CheckInfos.part(ProjectCheckInfos::PART_SPATIALATTRS)
                        .addMessage(tr("Attribute %1 produced on %2 units by %3 is already produced by another simulator")
                                    .arg(QString::fromStdString(itt->UnitClass))
                                    .arg(QString::fromStdString(itt->DataName))
                                    .arg(QString::fromStdString(ID)));
          }
        }
      }
      else
      {
        m_CheckInfos.part(ProjectCheckInfos::PART_MODELDEF).setStatus(PRJ_ERROR);
        if ((*it)->getType() == openfluid::fluidx::ModelItemDescriptor::PluggedSimulator)
          m_CheckInfos.part(ProjectCheckInfos::PART_MODELDEF).addMessage(tr("Simulator %1 is not available").arg(QString::fromStdString(ID)));
      }
    }
  }



  // ========== internal constraints

  std::vector<openfluid::ware::SignatureHandledTypedDataItem>::iterator itt;

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
  for (std::list<openfluid::fluidx::ModelItemDescriptor*>::const_iterator it = Items.begin(); it != Items.end(); ++it)
  {
    if ((*it)->isEnabled())
    {
      openfluid::machine::ModelItemSignatureInstance* SignII = Reg->getSignatureItemInstance(*it);
      std::string ID = Model.getID(*it);

      if (SignII != NULL)
      {
        Sign = SignII->Signature;
        std::string ID = Model.getID(*it);

        // check produced Vars
        std::vector<openfluid::ware::SignatureHandledTypedDataItem>& ProdVars = Sign->HandledData.ProducedVars;

        for (itt = ProdVars.begin(); itt != ProdVars.end(); ++itt)
        {

          if (!Domain.isClassNameExists(itt->UnitClass))
          {
            m_CheckInfos.part(ProjectCheckInfos::PART_SPATIALSTRUCT).setStatus(PRJ_ERROR);
            m_CheckInfos.part(ProjectCheckInfos::PART_SPATIALSTRUCT)
                        .addMessage(tr("Unit class %1 does not exist for variable %2 produced by %3")
                                    .arg(QString::fromStdString(itt->UnitClass))
                                    .arg(QString::fromStdString(itt->DataName))
                                    .arg(QString::fromStdString(ID)));
          }

          if (!VarsUnits.count(std::make_pair(itt->UnitClass, itt->DataName)))
          {
            VarsUnits.insert(std::make_pair(itt->UnitClass, itt->DataName));
            TypedVarsUnits.insert(std::make_pair(itt->UnitClass,
                                                 std::make_pair(itt->DataName, itt->DataType)));
          }
          else
          {
            m_CheckInfos.part(ProjectCheckInfos::PART_MODELDEF).setStatus(PRJ_ERROR);
            m_CheckInfos.part(ProjectCheckInfos::PART_MODELDEF)
                        .addMessage(tr("Variable %1 on %2 produced by %3 cannot be created because it is created by another simulator or generator")
                                    .arg(QString::fromStdString(itt->DataName))
                                    .arg(QString::fromStdString(itt->UnitClass))
                                    .arg(QString::fromStdString(ID)));
          }
        }

        if ((*it)->isType(openfluid::fluidx::WareDescriptor::Generator))
        {
          openfluid::fluidx::GeneratorDescriptor* GenDesc =
              dynamic_cast<openfluid::fluidx::GeneratorDescriptor*>(*it);

          if (!Domain.isClassNameExists(GenDesc->getUnitClass()))
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
        for (itt = UpVars.begin(); itt != UpVars.end(); ++itt)
        {
          if (!Domain.isClassNameExists(itt->UnitClass))
          {
            m_CheckInfos.part(ProjectCheckInfos::PART_SPATIALSTRUCT).setStatus(PRJ_ERROR);
            m_CheckInfos.part(ProjectCheckInfos::PART_SPATIALSTRUCT)
                        .addMessage(tr("Unit class %1 does not exist for variable %2 produced by %3")
                                    .arg(QString::fromStdString(itt->UnitClass))
                                    .arg(QString::fromStdString(itt->DataName))
                                    .arg(QString::fromStdString(ID)));
          }

          if (!VarsUnits.count(std::make_pair(itt->UnitClass, itt->DataName)))
          {
            VarsUnits.insert(std::make_pair(itt->UnitClass, itt->DataName));
            TypedVarsUnits.insert(
                std::make_pair(itt->UnitClass,
                               std::make_pair(itt->DataName, itt->DataType)));
          }
        }
      }
    }
  }

  // pass 2
  for (std::list<openfluid::fluidx::ModelItemDescriptor*>::const_iterator it = Items.begin(); it != Items.end(); ++it)
  {
    if ((*it)->isEnabled())
    {
      openfluid::machine::ModelItemSignatureInstance* SignII = Reg->getSignatureItemInstance(*it);
      std::string ID = Model.getID(*it);

      if (SignII != NULL)
      {

        Sign = SignII->Signature;

        // check required Vars
        std::vector<openfluid::ware::SignatureHandledTypedDataItem>& ReqVars = Sign->HandledData.RequiredVars;

        for (itt = ReqVars.begin(); itt != ReqVars.end(); ++itt)
        {
          if (!Domain.isClassNameExists(itt->UnitClass))
          {
            m_CheckInfos.part(ProjectCheckInfos::PART_SPATIALSTRUCT).setStatus(PRJ_ERROR);
            m_CheckInfos.part(ProjectCheckInfos::PART_SPATIALSTRUCT)
                        .addMessage(tr("Unit class %1 does not exist for variable %2 required by %3")
                                    .arg(QString::fromStdString(itt->UnitClass))
                                    .arg(QString::fromStdString(itt->DataName))
                                    .arg(QString::fromStdString(ID)));
          }

          if ((itt->DataType == openfluid::core::Value::NONE
              && !VarsUnits.count(std::make_pair(itt->UnitClass, itt->DataName)))
              || (itt->DataType != openfluid::core::Value::NONE && !TypedVarsUnits.count(
                  std::make_pair(itt->UnitClass,
                                 std::make_pair(itt->DataName, itt->DataType)))))
          {
            m_CheckInfos.part(ProjectCheckInfos::PART_MODELDEF).setStatus(PRJ_ERROR);
            m_CheckInfos.part(ProjectCheckInfos::PART_MODELDEF)
                        .addMessage(tr("Variable %1 on %2 required by %3 is not produced by another simulator or generator")
                                    .arg(QString::fromStdString(itt->DataName))
                                    .arg(QString::fromStdString(itt->UnitClass))
                                    .arg(QString::fromStdString(ID)));
          }
        }
      }
    }
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
  std::set<std::string> Classes = mp_AdvancedFXDesc->getDomain().getClassNames();

  const std::list<openfluid::fluidx::DatastoreItemDescriptor*>& Items = mp_AdvancedFXDesc->getDatastore().getItems();

  for (std::list<openfluid::fluidx::DatastoreItemDescriptor*>::const_iterator it = Items.begin(); it != Items.end(); ++it)
  {
    std::string Class = (*it)->getUnitClass();

    if (!Class.empty() && !Classes.count(Class))
    {
      m_CheckInfos.part(ProjectCheckInfos::PART_DATASTORE).setStatus(PRJ_WARNING);
      m_CheckInfos.part(ProjectCheckInfos::PART_DATASTORE).addMessage(tr("Unit class %1 does not exist for datastore item %2")
                                                                      .arg(QString::fromStdString(Class))
                                                                      .arg(QString::fromStdString((*it)->getID())));
    }
  }
}


// =====================================================================
// =====================================================================


void ProjectCentral::checkMonitoring()
{
  openfluid::fluidx::AdvancedMonitoringDescriptor& Monitoring = mp_AdvancedFXDesc->getMonitoring();
  const std::list<openfluid::fluidx::ObserverDescriptor*>& Items = Monitoring.getItems();

  openfluid::machine::ObserverSignatureRegistry* Reg = openfluid::machine::ObserverSignatureRegistry::getInstance();

  for (std::list<openfluid::fluidx::ObserverDescriptor*>::const_iterator it = Items.begin(); it != Items.end(); ++it)
  {
    if ((*it)->isEnabled())
    {
      const openfluid::machine::ObserverSignatureInstance* SignII = Reg->getSignature((*it)->getID());

      if (SignII == NULL)
      {
        m_CheckInfos.part(ProjectCheckInfos::PART_MONITORING).setStatus(PRJ_ERROR);
        m_CheckInfos.part(ProjectCheckInfos::PART_MONITORING).addMessage(tr("Observer %1 is not available").arg(QString::fromStdString((*it)->getID())));
      }
    }
  }
}


// =====================================================================
// =====================================================================


void ProjectCentral::checkRunConfig()
{

}
