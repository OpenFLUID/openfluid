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
 \file ProjectChecker.cpp
 \brief Implements ...

 \author Aline LIBRES <aline.libres@gmail.com>
 */

#include "ProjectChecker.hpp"

#include <boost/filesystem/operations.hpp>
#include <openfluid/fluidx/AdvancedFluidXDescriptor.hpp>
#include <openfluid/fluidx/ModelItemDescriptor.hpp>
#include <openfluid/fluidx/GeneratorDescriptor.hpp>
#include <openfluid/machine/FunctionSignatureRegistry.hpp>
#include <openfluid/base/RuntimeEnv.hpp>
#include <openfluid/machine/ModelItemInstance.hpp>
#include <openfluid/ware/GeneratorSignature.hpp>

// =====================================================================
// =====================================================================

ProjectChecker::ProjectChecker(
    openfluid::fluidx::AdvancedFluidXDescriptor& Desc) :
    mp_Desc(&Desc)
{
  clearAll();
}

// =====================================================================
// =====================================================================

void ProjectChecker::clearAll()
{
  IsProjectOk = false;
  IsModelOk = false;
  IsParamsOk = true;
  IsDomainOk = false;
  IsInputdataOk = false;
  IsGeneratorParamsOk = true;
  IsExtraFilesOk = false;
  IsRunConfigOk = false;
  IsMonitoringOk = false;

  ProjectMsg = "";
  ModelMsg = "";
  ParamsMsg = "";
  DomainMsg = "";
  InputdataMsg = "";
  ExtraFilesMsg = "";
  RunConfigMsg = "";
  MonitoringMsg = "";
}

// =====================================================================
// =====================================================================

ProjectChecker::~ProjectChecker()
{
}

// =====================================================================
// =====================================================================

bool ProjectChecker::check()
{
  clearAll();

  if (mp_Desc->getModel().getItemsCount() == 0)
    ModelMsg += "Model is empty";
  else
  {
    checkModelRequirements();
    checkModelInputdata();
    checkModelVars();
  }

  if (mp_Desc->getDomain().getUnitsByIdByClass().empty())
    DomainMsg += "Spatial domain is empty";

  if (mp_Desc->getRunDescriptor().getBeginDate() >= mp_Desc->getRunDescriptor().getEndDate())
    RunConfigMsg = "End date must be after begin date in run period";

  if (mp_Desc->getMonitoring().getItems().empty())
    MonitoringMsg = "No observer defined";

  IsExtraFilesOk = ExtraFilesMsg.empty();
  IsInputdataOk = InputdataMsg.empty();
  IsModelOk = ModelMsg.empty();
  IsDomainOk = DomainMsg.empty();
  IsRunConfigOk = RunConfigMsg.empty();
  IsProjectOk = ProjectMsg.empty();
  IsMonitoringOk = MonitoringMsg.empty();

  return getGlobalCheckState();
}

// =====================================================================
// =====================================================================

void ProjectChecker::checkModelRequirements()
{
  openfluid::fluidx::AdvancedModelDescriptor& Model = mp_Desc->getModel();
  const std::list<openfluid::fluidx::ModelItemDescriptor*>& Items =
      Model.getItems();
  openfluid::machine::FunctionSignatureRegistry* Reg =
      openfluid::machine::FunctionSignatureRegistry::getInstance();
  openfluid::base::RuntimeEnvironment* RunEnv =
      openfluid::base::RuntimeEnvironment::getInstance();
  openfluid::ware::FunctionSignature* Sign;

  for (std::list<openfluid::fluidx::ModelItemDescriptor*>::const_iterator it =
      Items.begin(); it != Items.end(); ++it)
  {
    Sign = Reg->getSignatureItemInstance(*it)->Signature;
    std::string ID = Model.getID(*it);

    // check ExtraFiles
    std::vector<std::string>& ReqFiles = Sign->HandledData.RequiredExtraFiles;
    if ((*it)->isType(openfluid::fluidx::WareDescriptor::Generator))
    {
      openfluid::fluidx::GeneratorDescriptor::GeneratorMethod Method =
          (static_cast<openfluid::fluidx::GeneratorDescriptor*>(*it))->getGeneratorMethod();

      if (Method == openfluid::fluidx::GeneratorDescriptor::Interp || Method
          == openfluid::fluidx::GeneratorDescriptor::Inject)
      {
        std::string FileNameFromParam = (*it)->getParameters().get("sources",
                                                                   "");
        if (!FileNameFromParam.empty() && !boost::filesystem::exists(
            RunEnv->getInputFullPath(FileNameFromParam)))
          ExtraFilesMsg += "- File " + FileNameFromParam + " required by " + ID
                           + " not found\n";
        FileNameFromParam = (*it)->getParameters().get("distribution", "");
        if (!FileNameFromParam.empty() && !boost::filesystem::exists(
            RunEnv->getInputFullPath(FileNameFromParam)))
          ExtraFilesMsg += "- File " + FileNameFromParam + " required by " + ID
                           + " not found\n";
      }
    }
    else
    {
      for (std::vector<std::string>::iterator itt = ReqFiles.begin();
          itt != ReqFiles.end(); ++itt)
      {
        if (!boost::filesystem::exists(RunEnv->getInputFullPath(*itt)))
          ExtraFilesMsg += "- File " + *itt + " required by " + ID
                           + " not found\n";
      }
    }

    // check Params
    m_RandomMinMaxChecked = false;
    m_InterpMinMaxChecked = false;
    m_InjectMinMaxChecked = false;

    std::vector<openfluid::ware::SignatureHandledDataItem>& Params =
        Sign->HandledData.FunctionParams;
    for (std::vector<openfluid::ware::SignatureHandledDataItem>::iterator itt =
        Params.begin(); itt != Params.end(); ++itt)
    {
      if (!isParamSet(*it, itt->DataName))
      {
        if ((*it)->isType(openfluid::fluidx::ModelItemDescriptor::Generator) && itt->DataName
            != "thresholdmin"
            && itt->DataName != "thresholdmax")
          IsGeneratorParamsOk = false;
        else
          IsParamsOk = false;

        ParamsMsg += "- " + itt->DataName + " is missing in " + ID + "\n";
      }

      // check Generators Params
      if ((*it)->isType(openfluid::fluidx::ModelItemDescriptor::Generator))
      {
        openfluid::fluidx::GeneratorDescriptor::GeneratorMethod Method =
            (static_cast<openfluid::ware::GeneratorSignature*>(Sign))->m_GeneratorMethod;

        if (Method == openfluid::fluidx::GeneratorDescriptor::Random && !m_RandomMinMaxChecked)
        {
          checkGeneratorParam("min", "max", *it, ID);
          m_RandomMinMaxChecked = true;
        }
        else if (Method == openfluid::fluidx::GeneratorDescriptor::Interp
            && !m_InterpMinMaxChecked)
        {
          checkGeneratorParam("thresholdmin", "thresholdmax", *it, ID);
          m_InterpMinMaxChecked = true;
        }
        else if (Method == openfluid::fluidx::GeneratorDescriptor::Inject
            && !m_InjectMinMaxChecked)
        {
          checkGeneratorParam("thresholdmin", "thresholdmax", *it, ID);
          m_InjectMinMaxChecked = true;
        }
      }
    }

  }

}

// =====================================================================
// =====================================================================

void ProjectChecker::checkGeneratorParam(
    std::string MinParamName, std::string MaxParamName,
    openfluid::fluidx::ModelItemDescriptor* Item, std::string ItemId)
{
  bool TestCompare = true;

  if (!isParamSet(Item, MinParamName))
    TestCompare = false;
  else if (!isParamSetAsDouble(Item, MinParamName))
  {
    TestCompare = IsGeneratorParamsOk = false;
    ParamsMsg += "- " + MinParamName + " has to be a double in " + ItemId
                 + "\n";
  }

  if (!isParamSet(Item, MaxParamName))
    TestCompare = false;
  else if (!isParamSetAsDouble(Item, MaxParamName))
  {
    TestCompare = IsGeneratorParamsOk = false;
    ParamsMsg += "- " + MaxParamName + " has to be a double in " + ItemId
                 + "\n";
  }

  if (TestCompare && !(getParamAsDouble(Item, MaxParamName)
      > getParamAsDouble(Item, MinParamName)))
  {
    IsGeneratorParamsOk = false;
    ParamsMsg += "- " + MinParamName + " >= " + MaxParamName + " in " + ItemId
                 + "\n";
  }

}

// =====================================================================
// =====================================================================

bool ProjectChecker::isParamSet(openfluid::fluidx::ModelItemDescriptor* Item,
                                std::string ParamName)
{
  return (!Item->getParameters().get(ParamName, "").empty()
      || !mp_Desc->getModel().getGlobalParameters().get(ParamName, "").empty());
}

// =====================================================================
// =====================================================================

bool ProjectChecker::isParamSetAsDouble(
    openfluid::fluidx::ModelItemDescriptor* Item, std::string ParamName)
{
  return (Item->getParameters().get_optional<double>(ParamName)
      || mp_Desc->getModel().getGlobalParameters().get_optional<double>(
          ParamName));
}

// =====================================================================
// =====================================================================

double ProjectChecker::getParamAsDouble(
    openfluid::fluidx::ModelItemDescriptor* Item, std::string ParamName)
{
  if (Item->getParameters().get_optional<double>(ParamName))
    return Item->getParameters().get<double>(ParamName);
  else if (mp_Desc->getModel().getGlobalParameters().get_optional<double>(
      ParamName))
    return mp_Desc->getModel().getGlobalParameters().get<double>(ParamName);
  else
    throw openfluid::base::OFException(
        "OpenFLUID Framwework", "ProjectChecker::getParamAsDouble",
        "Parameter " + ParamName + " is not set as a double");
}

// =====================================================================
// =====================================================================

void ProjectChecker::checkModelInputdata()
{
  openfluid::fluidx::AdvancedDomainDescriptor& Domain = mp_Desc->getDomain();
  const std::list<openfluid::fluidx::ModelItemDescriptor*>& Items =
      mp_Desc->getModel().getItems();
  openfluid::machine::FunctionSignatureRegistry* Reg =
      openfluid::machine::FunctionSignatureRegistry::getInstance();
  openfluid::ware::FunctionSignature* Sign;
  std::vector<openfluid::ware::SignatureHandledDataItem>::iterator itt;

  std::set<std::pair<std::string, std::string> > IDataUnits;

  for (std::list<openfluid::fluidx::ModelItemDescriptor*>::const_iterator it =
      Items.begin(); it != Items.end(); ++it)
  {
    Sign = Reg->getSignatureItemInstance(*it)->Signature;
    std::string ID = mp_Desc->getModel().getID(*it);

    // check required Input data
    std::vector<openfluid::ware::SignatureHandledDataItem>& ReqData =
        Sign->HandledData.RequiredInputdata;
    for (itt = ReqData.begin(); itt != ReqData.end(); ++itt)
    {
      if (!Domain.isClassNameExists(itt->UnitClass))
        InputdataMsg += "- Unit " + itt->UnitClass
                        + " class does not exist for " + itt->DataName
                        + " input data required by " + ID + "\n";

      else if (!(Domain.getInputDataNames(itt->UnitClass).count(itt->DataName)
          || IDataUnits.count(std::make_pair(itt->UnitClass, itt->DataName))))
        InputdataMsg += "- " + itt->DataName + " input data on "
                        + itt->UnitClass + " required by " + ID
                        + " is not available\n";
    }

    // check produced Input data
    std::vector<openfluid::ware::SignatureHandledDataItem>& ProdData =
        Sign->HandledData.ProducedInputdata;
    for (itt = ProdData.begin(); itt != ProdData.end(); ++itt)
    {
      if (!Domain.isClassNameExists(itt->UnitClass))
        InputdataMsg += "- Unit " + itt->UnitClass
                        + " class does not exist for " + itt->DataName
                        + " input data produced by " + ID + "\n";

      if (!IDataUnits.count(std::make_pair(itt->UnitClass, itt->DataName)))
      {
        IDataUnits.insert(std::make_pair(itt->UnitClass, itt->DataName));
      }
      else
        InputdataMsg += "- "
            + itt->DataName + " input data on " + itt->UnitClass
            + " produced by " + ID
            + " cannot be created because it is previously created\n";
    }

  }

}

// =====================================================================
// =====================================================================

void ProjectChecker::checkModelVars()
{
  openfluid::fluidx::AdvancedDomainDescriptor& Domain = mp_Desc->getDomain();
  const std::list<openfluid::fluidx::ModelItemDescriptor*>& Items =
      mp_Desc->getModel().getItems();
  openfluid::machine::FunctionSignatureRegistry* Reg =
      openfluid::machine::FunctionSignatureRegistry::getInstance();
  openfluid::ware::FunctionSignature* Sign;
  std::vector<openfluid::ware::SignatureHandledTypedDataItem>::iterator itt;

  std::set<std::pair<std::string, std::string> > VarsUnits;
  std::set<
      std::pair<std::string,
          std::pair<std::string, openfluid::core::Value::Type> > > TypedVarsUnits;

  // A) first pass
  // // 1) required vars
  // // 2) produced vars
  // // 3) updated vars
  // B) second pass
  // // 4) required vars at t-1+

  for (std::list<openfluid::fluidx::ModelItemDescriptor*>::const_iterator it =
      Items.begin(); it != Items.end(); ++it)
  {
    Sign = Reg->getSignatureItemInstance(*it)->Signature;
    std::string ID = mp_Desc->getModel().getID(*it);

    // check required Vars
    std::vector<openfluid::ware::SignatureHandledTypedDataItem>& ReqVars =
        Sign->HandledData.RequiredVars;
    for (itt = ReqVars.begin(); itt != ReqVars.end(); ++itt)
    {
      if (!Domain.isClassNameExists(itt->UnitClass))
        ProjectMsg += "- Unit class " + itt->UnitClass + " doesn't exist for "
                      + itt->DataName + " variable required by " + ID + "\n";

      if ((itt->DataType == openfluid::core::Value::NONE
          && !VarsUnits.count(std::make_pair(itt->UnitClass, itt->DataName)))
          || (itt->DataType != openfluid::core::Value::NONE && !TypedVarsUnits.count(
              std::make_pair(itt->UnitClass,
                             std::make_pair(itt->DataName, itt->DataType)))))
        ModelMsg += "- " + itt->DataName + " variable on " + itt->UnitClass
                    + " required by " + ID + " is not previously created\n";
    }

    // check produced Vars
    std::vector<openfluid::ware::SignatureHandledTypedDataItem>& ProdVars =
        Sign->HandledData.ProducedVars;
    for (itt = ProdVars.begin(); itt != ProdVars.end(); ++itt)
    {
      if (!Domain.isClassNameExists(itt->UnitClass))
        ProjectMsg += "- Unit class " + itt->UnitClass + " doesn't exist for "
                      + itt->DataName + " variable produced by " + ID + "\n";

      if (!VarsUnits.count(std::make_pair(itt->UnitClass, itt->DataName)))
      {
        VarsUnits.insert(std::make_pair(itt->UnitClass, itt->DataName));
        TypedVarsUnits.insert(
            std::make_pair(itt->UnitClass,
                           std::make_pair(itt->DataName, itt->DataType)));
      }
      else
        ModelMsg += "- " + itt->DataName + " variable on " + itt->UnitClass
                    + " produced by " + ID
                    + " cannot be created because it is previously created\n";
    }
    if ((*it)->isType(openfluid::fluidx::WareDescriptor::Generator))
    {
      openfluid::fluidx::GeneratorDescriptor* GenDesc =
          dynamic_cast<openfluid::fluidx::GeneratorDescriptor*>(*it);

      if (!Domain.isClassNameExists(GenDesc->getUnitClass()))
        ProjectMsg += "- Unit class " + GenDesc->getUnitClass()
                      + " doesn't exist for " + GenDesc->getVariableName()
                      + " variable generated by " + ID + "\n";

      if (!VarsUnits.count(
          std::make_pair(GenDesc->getUnitClass(), GenDesc->getVariableName())))
      {
        VarsUnits.insert(
            std::make_pair(GenDesc->getUnitClass(),
                           GenDesc->getVariableName()));
      }
      else
        ModelMsg += "- " + GenDesc->getVariableName() + " variable on "
                    + GenDesc->getUnitClass() + " produced by " + ID
                    + " cannot be generated because it is previously created\n";
    }

    // check updated Vars
    std::vector<openfluid::ware::SignatureHandledTypedDataItem>& UpVars =
        Sign->HandledData.UpdatedVars;
    for (itt = UpVars.begin(); itt != UpVars.end(); ++itt)
    {
      if (!Domain.isClassNameExists(itt->UnitClass))
        ProjectMsg += "- Unit class " + itt->UnitClass + " doesn't exist for "
                      + itt->DataName + " variable updated by " + ID + "\n";

      if (!VarsUnits.count(std::make_pair(itt->UnitClass, itt->DataName)))
      {
        VarsUnits.insert(std::make_pair(itt->UnitClass, itt->DataName));
        TypedVarsUnits.insert(
            std::make_pair(itt->UnitClass,
                           std::make_pair(itt->DataName, itt->DataType)));
      }
    }
  }

  for (std::list<openfluid::fluidx::ModelItemDescriptor*>::const_iterator it =
      Items.begin(); it != Items.end(); ++it)
  {
    Sign = Reg->getSignatureItemInstance(*it)->Signature;
    std::string ID = mp_Desc->getModel().getID(*it);

    // checking required Vars at t-1
    std::vector<openfluid::ware::SignatureHandledTypedDataItem>& ReqVars =
        Sign->HandledData.RequiredPrevVars;
    for (itt = ReqVars.begin(); itt != ReqVars.end(); ++itt)
    {
      if (!Domain.isClassNameExists(itt->UnitClass))
        ProjectMsg += "- Unit class " + itt->UnitClass + " doesn't exist for "
                      + itt->DataName + " variable previously required by " + ID
                      + "\n";

      if ((itt->DataType == openfluid::core::Value::NONE
          && !VarsUnits.count(std::make_pair(itt->UnitClass, itt->DataName)))
          || (itt->DataType != openfluid::core::Value::NONE && !TypedVarsUnits.count(
              std::make_pair(itt->UnitClass,
                             std::make_pair(itt->DataName, itt->DataType)))))
        ModelMsg += "- " + itt->DataName + " variable on " + itt->UnitClass
                    + " required by " + ID + " is not previously created\n";
    }
  }

}

// =====================================================================
// =====================================================================

bool ProjectChecker::getGlobalCheckState()
{
  return (IsExtraFilesOk && IsInputdataOk && IsModelOk && IsGeneratorParamsOk
          && IsProjectOk && IsRunConfigOk);
}

// =====================================================================
// =====================================================================
