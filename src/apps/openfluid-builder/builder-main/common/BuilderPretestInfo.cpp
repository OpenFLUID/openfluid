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
 \file BuilderPretestInfo.cpp
 \brief Implements ...

 \author Aline LIBRES <libres@supagro.inra.fr>
 */

#include "BuilderPretestInfo.hpp"

#include <glibmm/i18n.h>
#include <glibmm/ustring.h>
#include <boost/foreach.hpp>

#include <openfluid/machine/ModelItemInstance.hpp>
#include <openfluid/machine/ModelInstance.hpp>
#include <openfluid/machine/SimulationBlob.hpp>
#include <openfluid/ware/FunctionSignature.hpp>

#include "GeneratorSignature.hpp"

// =====================================================================
// =====================================================================


BuilderPretestInfo::BuilderPretestInfo() :
  Domain(true), DomainMsg(""), Params(true), GeneratorParams(true), ParamsMsg(
      ""), Project(true), ProjectMsg(""), Outputs(true), OutputsMsg(""),
      RunConfig(true), RunConfigMsg("")
{
}

// =====================================================================
// =====================================================================


void BuilderPretestInfo::addBuilderInfo(
    openfluid::machine::ModelInstance* ModelInstance,
    openfluid::machine::SimulationBlob* SimBlob,
    openfluid::base::RunDescriptor& RunDesc)
{
  mp_ModelInstance = ModelInstance;
  mp_SimBlob = SimBlob;
  mp_RunDesc = &RunDesc;

  m_GlobalParams = ModelInstance->getGlobalParameters();

  checkModelFilled();

  checkDomainFilled();

  checkOutputsFilled();

  checkRunDate();

  BOOST_FOREACH(openfluid::machine::ModelItemInstance* Item,ModelInstance->getItems())
{  checkModelItemParams(Item);

  checkModelItemVars(Item);
}

}

// =====================================================================
// =====================================================================

void BuilderPretestInfo::checkModelFilled()
{
  if (mp_ModelInstance->getItemsCount() == 0)
  {
    Model = false;
    ModelMsg = _("Model is empty");
  }
}

// =====================================================================
// =====================================================================


void BuilderPretestInfo::checkDomainFilled()
{
  if (mp_SimBlob->getCoreRepository().getUnitsGlobally()->empty())
  {
    Domain = false;
    DomainMsg = _("Spatial domain is empty");
  }
}

// =====================================================================
// =====================================================================


void BuilderPretestInfo::checkOutputsFilled()
{
  if (mp_SimBlob->getOutputDescriptor().getFileSets().empty())
  {
    Outputs = false;
    OutputsMsg = _("No file format defined");
    return;
  }

  for (unsigned int i = 0; i
      < mp_SimBlob->getOutputDescriptor().getFileSets().size(); i++)
  {
    if (!mp_SimBlob->getOutputDescriptor().getFileSets()[i].getSets().empty())
      return;
  }

  Outputs = false;
  OutputsMsg = _("No set defined");
}

// =====================================================================
// =====================================================================


void BuilderPretestInfo::checkRunDate()
{
  if (mp_RunDesc->getBeginDate() >= mp_RunDesc->getEndDate())
  {
    RunConfig = false;
    RunConfigMsg = _("End date must be after begin date in run period");
  }
}

// =====================================================================
// =====================================================================


void BuilderPretestInfo::checkModelItemParams(
    openfluid::machine::ModelItemInstance* Item)
{
  m_RandomMinMaxChecked = false;
  m_InterpMinMaxChecked = false;
  m_InjectMinMaxChecked = false;

  BOOST_FOREACH(openfluid::ware::SignatureHandledDataItem Param, Item->Signature->HandledData.FunctionParams)
{  checkParamFilled(Item, Param.DataName);

  if(Item->ItemType == openfluid::base::ModelItemDescriptor::Generator)
  {
    checkGeneratorParamsConsistency(Item);
  }
}
}

// =====================================================================
// =====================================================================


void BuilderPretestInfo::checkParamFilled(
    openfluid::machine::ModelItemInstance* Item, std::string ParamName)
{
  if (!localParamIsSet(Item, ParamName) & !globalParamIsSet(ParamName))
  {
    if (Item->ItemType == openfluid::base::ModelItemDescriptor::Generator)
      GeneratorParams = false;
    else
      Params = false;

    ParamsMsg.append(Glib::ustring::compose(_("%1 is missing in %2\n"),
        ParamName, Item->Signature->ID));
  }
}

// =====================================================================
// =====================================================================


void BuilderPretestInfo::checkGeneratorParamsConsistency(
    openfluid::machine::ModelItemInstance* Item)
{
  openfluid::base::GeneratorDescriptor::GeneratorMethod Method =
      (static_cast<GeneratorSignature*> (Item->Signature))->m_GeneratorMethod;

  if (Method == openfluid::base::GeneratorDescriptor::Random
      && !m_RandomMinMaxChecked && (localParamIsSet(Item, "min")
      || globalParamIsSet("min")) && (localParamIsSet(Item, "max")
      || globalParamIsSet("max")))
  {
    checkRandomMinMax(Item);

    m_RandomMinMaxChecked = true;
  }
  else if (Method == openfluid::base::GeneratorDescriptor::Interp
      && !m_InterpMinMaxChecked && (localParamIsSet(Item, "thresholdmin")
      || globalParamIsSet("thresholdmin")) && (localParamIsSet(Item,
      "thresholdmax") || globalParamIsSet("thresholdmax")))
  {
    checkInterpMinMax(Item);

    m_InterpMinMaxChecked = true;
  }
  else if (Method == openfluid::base::GeneratorDescriptor::Inject
      && !m_InjectMinMaxChecked && (localParamIsSet(Item, "thresholdmin")
      || globalParamIsSet("thresholdmin")) && (localParamIsSet(Item,
      "thresholdmax") || globalParamIsSet("thresholdmax")))
  {
    checkInterpMinMax(Item);

    m_InjectMinMaxChecked = true;
  }
}

// =====================================================================
// =====================================================================


void BuilderPretestInfo::checkRandomMinMax(
    openfluid::machine::ModelItemInstance* Item)
{
  double Min, Max;

  openfluid::tools::ConvertString<double>(
      localParamIsSet(Item, "min") ? Item->Params.get<std::string>("min")
          : m_GlobalParams.get<std::string>("min"), &Min);
  openfluid::tools::ConvertString<double>(
      localParamIsSet(Item, "max") ? Item->Params.get<std::string>("max")
          : m_GlobalParams.get<std::string>("max"), &Max);

  if (Min > Max)
  {
    GeneratorParams = false;
    ParamsMsg.append(Glib::ustring::compose(_("min >= max in %1\n"),
        Item->Signature->ID));
  }

}

// =====================================================================
// =====================================================================


void BuilderPretestInfo::checkInterpMinMax(
    openfluid::machine::ModelItemInstance* Item)
{
  double Min, Max;

  openfluid::tools::ConvertString<double>(
      localParamIsSet(Item, "thresholdmin") ? Item->Params.get<std::string>("thresholdmin")
          : m_GlobalParams.get<std::string>("thresholdmin"), &Min);
  openfluid::tools::ConvertString<double>(
      localParamIsSet(Item, "thresholdmax") ? Item->Params.get<std::string>("thresholdmax")
          : m_GlobalParams.get<std::string>("thresholdmax"), &Max);

  if (Min > Max)
  {
    GeneratorParams = false;
    ParamsMsg.append(Glib::ustring::compose(
        _("thresholdmin >= thresholdmax in %1\n"), Item->Signature->ID));
  }

}

// =====================================================================
// =====================================================================


bool BuilderPretestInfo::localParamIsSet(
    openfluid::machine::ModelItemInstance* Item, std::string ParamName)
{
  try
  {
    return (Item->Params.get<std::string>(ParamName) != "");
  }
  catch (...) { }
  return false;
}

// =====================================================================
// =====================================================================


bool BuilderPretestInfo::globalParamIsSet(std::string ParamName)
{
  try
  {
    return (m_GlobalParams.get<std::string>(ParamName) != "");
  }
  catch (...) { }
  return false;
}

// =====================================================================
// =====================================================================


void BuilderPretestInfo::checkModelItemVars(
    openfluid::machine::ModelItemInstance* Item)
{
  BOOST_FOREACH(openfluid::ware::SignatureHandledDataItem Var,Item->Signature->HandledData.ProducedVars)
{  if(!checkVar(Var))
  {
    Project = false;
    ProjectMsg .append(Glib::ustring::compose(_("Unit class %1 doesn't exist for %2 variable produced by %3\n"), Var.UnitClass,
            Var.DataName, Item->Signature->ID));
  }

}

BOOST_FOREACH(openfluid::ware::SignatureHandledDataItem Var,Item->Signature->HandledData.RequiredVars)
{
  if(!checkVar(Var))
  {
    Project = false;
    ProjectMsg .append(Glib::ustring::compose(_("Unit class %1 doesn't exist for %2 variable required by %3\n"), Var.UnitClass,
            Var.DataName, Item->Signature->ID));
  }
}

BOOST_FOREACH(openfluid::ware::SignatureHandledDataItem Var,Item->Signature->HandledData.RequiredPrevVars)
{
  if(!checkVar(Var))
  {
    Project = false;
    ProjectMsg .append(Glib::ustring::compose(_("Unit class %1 doesn't exist for %2 variable previously required by %3\n"), Var.UnitClass,
            Var.DataName, Item->Signature->ID));
  }
}

BOOST_FOREACH(openfluid::ware::SignatureHandledDataItem Var,Item->Signature->HandledData.UpdatedVars)
{
  if(!checkVar(Var))
  {
    Project = false;
    ProjectMsg .append(Glib::ustring::compose(_("Unit class %1 doesn't exist for %2 variable updated by %3\n"), Var.UnitClass,
            Var.DataName, Item->Signature->ID));
  }
}
}

// =====================================================================
// =====================================================================


bool BuilderPretestInfo::checkVar(openfluid::ware::SignatureHandledDataItem Var)
{
  return (mp_SimBlob->getCoreRepository().getUnits(Var.UnitClass) != NULL)
      && (!mp_SimBlob->getCoreRepository().getUnits(Var.UnitClass)->getList()->empty());

}
// =====================================================================
// =====================================================================


bool BuilderPretestInfo::getGlobalCheckState()
{
  return (ExtraFiles && Inputdata && Model /*&& GeneratorParams */&& Project
      && RunConfig);
}
