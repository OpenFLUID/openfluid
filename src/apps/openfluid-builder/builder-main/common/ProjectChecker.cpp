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

#include <glibmm/i18n.h>
#include <glibmm/ustring.h>
#include <boost/filesystem/operations.hpp>
#include <openfluid/fluidx/AdvancedFluidXDescriptor.hpp>
#include <openfluid/fluidx/ModelItemDescriptor.hpp>
#include <openfluid/fluidx/GeneratorDescriptor.hpp>
#include <openfluid/machine/SimulatorSignatureRegistry.hpp>
#include <openfluid/base/RuntimeEnv.hpp>
#include <openfluid/base/ApplicationException.hpp>
#include <openfluid/machine/ModelItemInstance.hpp>
#include <openfluid/ware/GeneratorSignature.hpp>
#include <openfluid/tools/SwissTools.hpp>

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
  IsAttributeOk = false;
  IsGeneratorParamsOk = true;
  IsExtraFilesOk = false;
  IsRunConfigOk = false;
  IsMonitoringOk = false;
  IsDatastoreOk = false;

  ProjectMsg = "";
  ModelMsg = "";
  ParamsMsg = "";
  DomainMsg = "";
  AttributeMsg = "";
  ExtraFilesMsg = "";
  RunConfigMsg = "";
  MonitoringMsg = "";
  DatastoreMsg = "";
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
    ModelMsg += _("Model is empty");
  else
  {
    checkModelRequirements();
    checkModelAttributes();
    checkModelVars();
  }

  if (mp_Desc->getDomain().getUnitsByIdByClass().empty())
    DomainMsg += _("Spatial domain is empty");

  if (mp_Desc->getRunDescriptor().getBeginDate() >= mp_Desc->getRunDescriptor().getEndDate())
    RunConfigMsg = _("End date must be after begin date in run period");

  if (mp_Desc->getMonitoring().getItems().empty())
    MonitoringMsg = _("No observer defined");

  checkDatastore();

  IsExtraFilesOk = ExtraFilesMsg.empty();
  IsAttributeOk = AttributeMsg.empty();
  IsModelOk = ModelMsg.empty();
  IsDomainOk = DomainMsg.empty();
  IsRunConfigOk = RunConfigMsg.empty();
  IsProjectOk = ProjectMsg.empty();
  IsMonitoringOk = MonitoringMsg.empty();
  IsDatastoreOk = DatastoreMsg.empty();

  return getGlobalCheckState();
}


// =====================================================================
// =====================================================================


void ProjectChecker::checkModelRequirements()
{
  openfluid::fluidx::AdvancedModelDescriptor& Model = mp_Desc->getModel();
  const std::list<openfluid::fluidx::ModelItemDescriptor*>& Items =
      Model.getItems();
  openfluid::machine::SimulatorSignatureRegistry* Reg =
      openfluid::machine::SimulatorSignatureRegistry::getInstance();
  openfluid::base::RuntimeEnvironment* RunEnv =
      openfluid::base::RuntimeEnvironment::getInstance();
  openfluid::ware::SimulatorSignature* Sign;

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
        std::string FileNameFromParam = (*it)->getParameters()["sources"];
        if (!FileNameFromParam.empty() && !boost::filesystem::exists(
            RunEnv->getInputFullPath(FileNameFromParam)))
          ExtraFilesMsg += Glib::ustring::compose(
              _("- File %1 required by %2 not found\n"), FileNameFromParam,
              ID);

        FileNameFromParam = (*it)->getParameters()["distribution"];
        if (!FileNameFromParam.empty() && !boost::filesystem::exists(
            RunEnv->getInputFullPath(FileNameFromParam)))
          ExtraFilesMsg += Glib::ustring::compose(
              _("- File %1 required by %2 not found\n"), FileNameFromParam,
              ID);
      }
    }
    else
    {
      for (std::vector<std::string>::iterator itt = ReqFiles.begin();
          itt != ReqFiles.end(); ++itt)
      {
        if (!boost::filesystem::exists(RunEnv->getInputFullPath(*itt)))
          ExtraFilesMsg += Glib::ustring::compose(
              _("- File %1 required by %2 not found\n"), *itt, ID);
      }
    }

    // check Params
    m_RandomMinMaxChecked = false;
    m_InterpMinMaxChecked = false;
    m_InjectMinMaxChecked = false;

    std::vector<openfluid::ware::SignatureHandledDataItem>& Params =
        Sign->HandledData.SimulatorParams;
    for (std::vector<openfluid::ware::SignatureHandledDataItem>::iterator itt =
        Params.begin(); itt != Params.end(); ++itt)
    {
      if (!isParamSet(*it, itt->DataName))
      {
        if ((*it)->isType(openfluid::fluidx::ModelItemDescriptor::Generator) &&
            itt->DataName != "thresholdmin" && itt->DataName != "thresholdmax" &&
            itt->DataName != "deltat")
          IsGeneratorParamsOk = false;
        else
          IsParamsOk = false;

        ParamsMsg += Glib::ustring::compose(_("- %1 is missing in %2\n"),
                                            itt->DataName, ID);
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
    ParamsMsg += Glib::ustring::compose(_("- %1 has to be a double in %2\n"),
                                        MinParamName, ItemId);
  }

  if (!isParamSet(Item, MaxParamName))
    TestCompare = false;
  else if (!isParamSetAsDouble(Item, MaxParamName))
  {
    TestCompare = IsGeneratorParamsOk = false;
    ParamsMsg += Glib::ustring::compose(_("- %1 has to be a double in %2\n"),
                                        MaxParamName, ItemId);
  }

  if (TestCompare && !(getParamAsDouble(Item, MaxParamName)
      > getParamAsDouble(Item, MinParamName)))
  {
    IsGeneratorParamsOk = false;
    ParamsMsg += Glib::ustring::compose(_("- %1 >= %2 in %3\n"), MinParamName,
                                        MaxParamName, ItemId);
  }

}


// =====================================================================
// =====================================================================


bool ProjectChecker::isParamSet(openfluid::fluidx::ModelItemDescriptor* Item,
                                std::string ParamName)
{
  return (!Item->getParameters()[ParamName].get().empty()
      || !mp_Desc->getModel().getGlobalParameters()[ParamName].get().empty());
}


// =====================================================================
// =====================================================================


bool ProjectChecker::isParamSetAsDouble(
    openfluid::fluidx::ModelItemDescriptor* Item, std::string ParamName)
{
  if (!isParamSet(Item, ParamName))
    return false;

  double d;

  return (Item->getParameters()[ParamName].toDouble(d)
      || mp_Desc->getModel().getGlobalParameters()[ParamName].toDouble(d));
}


// =====================================================================
// =====================================================================


double ProjectChecker::getParamAsDouble(
    openfluid::fluidx::ModelItemDescriptor* Item, std::string ParamName)
{
  if (!isParamSetAsDouble(Item, ParamName))
    throw openfluid::base::ApplicationException(
        "openfluid-builder", "ProjectChecker::getParamAsDouble",
        "Parameter " + ParamName + " is not set as a double");

  double d;

  if (Item->getParameters()[ParamName].toDouble(d))
    return d;

  mp_Desc->getModel().getGlobalParameters()[ParamName].toDouble(d);
  return d;
}


// =====================================================================
// =====================================================================


void ProjectChecker::checkModelAttributes()
{
  openfluid::fluidx::AdvancedDomainDescriptor& Domain = mp_Desc->getDomain();
  const std::list<openfluid::fluidx::ModelItemDescriptor*>& Items =
      mp_Desc->getModel().getItems();
  openfluid::machine::SimulatorSignatureRegistry* Reg =
      openfluid::machine::SimulatorSignatureRegistry::getInstance();
  openfluid::ware::SimulatorSignature* Sign;
  std::vector<openfluid::ware::SignatureHandledDataItem>::iterator itt;

  std::set<std::pair<std::string, std::string> > AttrsUnits;

  for (std::list<openfluid::fluidx::ModelItemDescriptor*>::const_iterator it =
      Items.begin(); it != Items.end(); ++it)
  {
    Sign = Reg->getSignatureItemInstance(*it)->Signature;
    std::string ID = mp_Desc->getModel().getID(*it);

    // check required attribute
    std::vector<openfluid::ware::SignatureHandledDataItem>& ReqData =
        Sign->HandledData.RequiredAttribute;
    for (itt = ReqData.begin(); itt != ReqData.end(); ++itt)
    {
      if (!Domain.isClassNameExists(itt->UnitClass))
        AttributeMsg +=
            Glib::ustring::compose(
                _("- Unit %1 class does not exist for %2 attribute required by %3\n"),
                itt->UnitClass, itt->DataName, ID);

      else if (!(Domain.getAttributesNames(itt->UnitClass).count(itt->DataName)
          || AttrsUnits.count(std::make_pair(itt->UnitClass, itt->DataName))))
        AttributeMsg += Glib::ustring::compose(
            _("- %1 attribute on %2 required by %3 is not available\n"),
            itt->DataName, itt->UnitClass, ID);
    }

    // check produced attribute
    std::vector<openfluid::ware::SignatureHandledDataItem>& ProdData =
        Sign->HandledData.ProducedAttribute;
    for (itt = ProdData.begin(); itt != ProdData.end(); ++itt)
    {
      if (!Domain.isClassNameExists(itt->UnitClass))
        AttributeMsg +=
            Glib::ustring::compose(
                _("- Unit %1 class does not exist for %2 attribute produced by %3\n"),
                itt->UnitClass, itt->DataName, ID);

      if (!AttrsUnits.count(std::make_pair(itt->UnitClass, itt->DataName)))
      {
        AttrsUnits.insert(std::make_pair(itt->UnitClass, itt->DataName));
      }
      else
        AttributeMsg +=
            Glib::ustring::compose(
                _("- %1 attribute on %2 produced by %3 cannot be created because it is previously created\n"),
                itt->DataName, itt->UnitClass, ID);
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

  openfluid::machine::SimulatorSignatureRegistry* Reg =
      openfluid::machine::SimulatorSignatureRegistry::getInstance();

  openfluid::ware::SimulatorSignature* Sign;

  std::vector<openfluid::ware::SignatureHandledTypedDataItem>::iterator itt;

  std::set<std::pair<std::string, std::string> > VarsUnits;
  std::set<
      std::pair<std::string,
          std::pair<std::string, openfluid::core::Value::Type> > > TypedVarsUnits;

  /* Variables processing order is important
   A) first pass
   1) produced vars
   2) updated vars
   B) second pass
   3) required vars
   */

  // pass 1
  for (std::list<openfluid::fluidx::ModelItemDescriptor*>::const_iterator it =
      Items.begin(); it != Items.end(); ++it)
  {
    Sign = Reg->getSignatureItemInstance(*it)->Signature;
    std::string ID = mp_Desc->getModel().getID(*it);

    // check produced Vars
    std::vector<openfluid::ware::SignatureHandledTypedDataItem>& ProdVars =
        Sign->HandledData.ProducedVars;
    for (itt = ProdVars.begin(); itt != ProdVars.end(); ++itt)
    {

      if (!Domain.isClassNameExists(itt->UnitClass))
        ProjectMsg +=
            Glib::ustring::compose(
                _("- Unit class %1 doesn't exist for %2 variable produced by %3\n"),
                itt->UnitClass, itt->DataName, ID);

      if (!VarsUnits.count(std::make_pair(itt->UnitClass, itt->DataName)))
      {
        VarsUnits.insert(std::make_pair(itt->UnitClass, itt->DataName));
        TypedVarsUnits.insert(std::make_pair(itt->UnitClass,
                                             std::make_pair(itt->DataName, itt->DataType)));
      }
      else
        ModelMsg +=
            Glib::ustring::compose(
                _("- %1 variable on %2 produced by %3 cannot be created because it is previously created\n"),
                itt->DataName, itt->UnitClass, ID);
    }
    if ((*it)->isType(openfluid::fluidx::WareDescriptor::Generator))
    {
      openfluid::fluidx::GeneratorDescriptor* GenDesc =
          dynamic_cast<openfluid::fluidx::GeneratorDescriptor*>(*it);

      if (!Domain.isClassNameExists(GenDesc->getUnitClass()))
        ProjectMsg +=
            Glib::ustring::compose(
                _("- Unit class %1 doesn't exist for %2 variable generated by %3\n"),
                GenDesc->getUnitClass(), GenDesc->getVariableName(), ID);

      if (!VarsUnits.count(std::make_pair(GenDesc->getUnitClass(), GenDesc->getVariableName())))
      {
        VarsUnits.insert(std::make_pair(GenDesc->getUnitClass(),GenDesc->getVariableName()));
        TypedVarsUnits.insert(std::make_pair(GenDesc->getUnitClass(),
                                             std::make_pair(GenDesc->getVariableName(),GenDesc->getVariableType())));
      }
      else
        ModelMsg +=
            Glib::ustring::compose(
                _("- %1 variable on %2 produced by %3 cannot be generated because it is previously created\n"),
                GenDesc->getVariableName(), GenDesc->getUnitClass(), ID);
    }

    // check updated Vars
    std::vector<openfluid::ware::SignatureHandledTypedDataItem>& UpVars =
        Sign->HandledData.UpdatedVars;
    for (itt = UpVars.begin(); itt != UpVars.end(); ++itt)
    {
      if (!Domain.isClassNameExists(itt->UnitClass))
        ProjectMsg +=
            Glib::ustring::compose(
                _("- Unit class %1 doesn't exist for %2 variable updated by %3\n"),
                itt->UnitClass, itt->DataName, ID);

      if (!VarsUnits.count(std::make_pair(itt->UnitClass, itt->DataName)))
      {
        VarsUnits.insert(std::make_pair(itt->UnitClass, itt->DataName));
        TypedVarsUnits.insert(
            std::make_pair(itt->UnitClass,
                           std::make_pair(itt->DataName, itt->DataType)));
      }
    }
  }

  // pass 2
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
        ProjectMsg +=
            Glib::ustring::compose(
                _("- Unit class %1 doesn't exist for %2 variable required by %3\n"),
                itt->UnitClass, itt->DataName, ID);

      if ((itt->DataType == openfluid::core::Value::NONE
          && !VarsUnits.count(std::make_pair(itt->UnitClass, itt->DataName)))
          || (itt->DataType != openfluid::core::Value::NONE && !TypedVarsUnits.count(
              std::make_pair(itt->UnitClass,
                             std::make_pair(itt->DataName, itt->DataType)))))
        ModelMsg += Glib::ustring::compose(
            _("- %1 variable on %2 required by %3 is not produced\n"),
            itt->DataName, itt->UnitClass, ID);
    }

  }
}


// =====================================================================
// =====================================================================


void ProjectChecker::checkDatastore()
{
  std::set<std::string> Classes = mp_Desc->getDomain().getClassNames();

  const std::list<openfluid::fluidx::DatastoreItemDescriptor*>& Items =
      mp_Desc->getDatastore().getItems();
  for (std::list<openfluid::fluidx::DatastoreItemDescriptor*>::const_iterator it =
      Items.begin(); it != Items.end(); ++it)
  {
    std::string Class = (*it)->getUnitClass();
    if (!Class.empty() && !Classes.count(Class))
      DatastoreMsg += Glib::ustring::compose(
          _("Unit class %1 doesn't exist for datastore item %2"), Class,
          (*it)->getID());
  }
}


// =====================================================================
// =====================================================================


bool ProjectChecker::getGlobalCheckState()
{
  return (IsExtraFilesOk && IsAttributeOk && IsModelOk && IsGeneratorParamsOk
          && IsProjectOk && IsRunConfigOk);
}


// =====================================================================
// =====================================================================

