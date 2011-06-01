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
 \file SimulOutSetDescModel.cpp
 \brief Implements ...

 \author Aline LIBRES <libres@supagro.inra.fr>
 */

#include "SimulOutSetDescModel.hpp"

#include <glibmm/i18n.h>

#include <boost/foreach.hpp>

// =====================================================================
// =====================================================================


bool SimulOutSetDescModelImpl::checkSetNameFormat()
{
  if (getName() == "")
  {
    m_signal_ErrorDialogAsked.emit(_("Set name cannot be empty"));
    return false;
  } else if (m_SetsByName.find(getName()) != m_SetsByName.end())
  {
    m_signal_ErrorDialogAsked.emit(_("This set name already exists"));
    return false;
  }
  return true;
}

// =====================================================================
// =====================================================================


bool SimulOutSetDescModelImpl::checkIdAtLeastOne()
{
  if (getIDs().empty())
  {
    m_signal_ErrorDialogAsked.emit(_("IDs cannot be empty"));
    return false;
  }

  return true;
}

// =====================================================================
// =====================================================================

bool SimulOutSetDescModelImpl::checkVarAtLeastOne()
{
  if (getVars().empty())
  {
    m_signal_ErrorDialogAsked.emit(_("Variables cannot be empty"));
    return false;
  }

  return true;
}

// =====================================================================
// =====================================================================


void SimulOutSetDescModelImpl::extractClassesAndIDs()
{
  m_Classes.clear();
  m_ByClassIDs.clear();

  const openfluid::core::UnitsListByClassMap_t* UnitsByClass =
      mp_CoreRepos->getUnitsByClass();
  for (openfluid::core::UnitsListByClassMap_t::const_iterator it =
      UnitsByClass->begin(); it != UnitsByClass->end(); ++it)
  {
    std::string ClassName = it->first;
    m_Classes.push_back(ClassName);
    BOOST_FOREACH(const openfluid::core::Unit Unit,*it->second.getList())
{    m_ByClassIDs[ClassName].push_back(Unit.getID());
  }
}
}

// =====================================================================
// =====================================================================


void SimulOutSetDescModelImpl::extractVars()
{
  m_ByClassVars.clear();
  m_ByClassScalarVars.clear();
  m_ByClassVectorVars.clear();

  BOOST_FOREACH(openfluid::machine::ModelItemInstance* Item,mp_ModelInstance->getItems())
{  BOOST_FOREACH(openfluid::base::SignatureHandledDataItem Var,Item->Signature->HandledData.ProducedVars)
  {
    m_ByClassVars[Var.UnitClass].push_back(Var.DataName);
    if (openfluid::tools::IsVectorNamedVariable(Var.DataName))
    m_ByClassVectorVars[Var.UnitClass].push_back(
        Var.DataName);
    else
    m_ByClassScalarVars[Var.UnitClass].push_back(
        Var.DataName);
  }
  BOOST_FOREACH(openfluid::base::SignatureHandledDataItem Var,Item->Signature->HandledData.UpdatedVars)
  {
    m_ByClassVars[Var.UnitClass].push_back(Var.DataName);
    if (openfluid::tools::IsVectorNamedVariable(Var.DataName))
    m_ByClassVectorVars[Var.UnitClass].push_back(
        Var.DataName);
    else
    m_ByClassScalarVars[Var.UnitClass].push_back(
        Var.DataName);
  }
}
}

// =====================================================================
// =====================================================================


void SimulOutSetDescModelImpl::extractFormatNames()
{
  m_FormatNames.clear();

  for (unsigned int i = 0; i < m_FilesFormatsByNameVect.size(); i++)
    m_FormatNames.push_back(m_FilesFormatsByNameVect[i].first);
}

// =====================================================================
// =====================================================================


SimulOutSetDescModelImpl::SimulOutSetDescModelImpl() :
  mp_OutDesc(0), mp_CoreRepos(0), mp_ModelInstance(0), m_FormatName(""),
      mp_SetDesc(0)
{
}

// =====================================================================
// =====================================================================


sigc::signal<void> SimulOutSetDescModelImpl::signal_InitForCreationAsked()
{
  return m_signal_InitForCreationAsked;
}

// =====================================================================
// =====================================================================


sigc::signal<void> SimulOutSetDescModelImpl::signal_InitForEditionAsked()
{
  return m_signal_InitForEditionAsked;
}

// =====================================================================
// =====================================================================


sigc::signal<void> SimulOutSetDescModelImpl::signal_ShowDialogAsked()
{
  return m_signal_ShowDialogAsked;
}

// =====================================================================
// =====================================================================


sigc::signal<void> SimulOutSetDescModelImpl::signal_CloseDialogAsked()
{
  return m_signal_CloseDialogAsked;
}

// =====================================================================
// =====================================================================


sigc::signal<void, std::string> SimulOutSetDescModelImpl::signal_ErrorDialogAsked()
{
  return m_signal_ErrorDialogAsked;
}

// =====================================================================
// =====================================================================


void SimulOutSetDescModelImpl::setEngineRequirements(
    openfluid::base::OutputDescriptor& OutDesc,
    openfluid::core::CoreRepository& CoreRepos,
    openfluid::machine::ModelInstance& ModelInstance)
{
  mp_OutDesc = &OutDesc;
  mp_CoreRepos = &CoreRepos;
  mp_ModelInstance = &ModelInstance;
}

// =====================================================================
// =====================================================================


void SimulOutSetDescModelImpl::initialize(
    openfluid::base::OutputSetDescriptor* SetDesc,
    std::string FileFormatName,
    std::vector<std::pair<std::string, openfluid::base::OutputFilesDescriptor> > FilesFormatsByNameVect,
    std::map<std::string, std::pair<std::string,
        openfluid::base::OutputSetDescriptor> > SetsByName)
{
  m_FormatName = FileFormatName;
  m_FilesFormatsByNameVect = FilesFormatsByNameVect;
  m_SetsByName = SetsByName;

  extractClassesAndIDs();
  extractVars();
  extractFormatNames();

  if (!m_Classes.empty() && !m_FormatNames.empty())
  {
    if (SetDesc)
    {
      mp_SetDesc = SetDesc;
      m_signal_InitForEditionAsked.emit();
    } else
    {
      mp_SetDesc = new openfluid::base::OutputSetDescriptor();
      mp_SetDesc->setUnitsClass(m_Classes[0]);
      m_FormatName = m_FormatNames[0];
      m_signal_InitForCreationAsked.emit();
    }
  }
}

// =====================================================================
// =====================================================================


void SimulOutSetDescModelImpl::showDialog()
{
  m_signal_ShowDialogAsked.emit();
}

// =====================================================================
// =====================================================================


openfluid::base::OutputSetDescriptor* SimulOutSetDescModelImpl::getSetDesc()
{
  return mp_SetDesc;
}

// =====================================================================
// =====================================================================


std::vector<std::string> SimulOutSetDescModelImpl::getClasses()
{
  return m_Classes;
}

// =====================================================================
// =====================================================================


std::map<std::string, std::vector<unsigned int> > SimulOutSetDescModelImpl::getByClassIDs()
{
  return m_ByClassIDs;
}

// =====================================================================
// =====================================================================


std::map<std::string, std::vector<std::string> > SimulOutSetDescModelImpl::getByClassVars()
{
  return m_ByClassVars;
}

// =====================================================================
// =====================================================================


std::vector<std::string> SimulOutSetDescModelImpl::getFormatNames()
{
  return m_FormatNames;
}

// =====================================================================
// =====================================================================


void SimulOutSetDescModelImpl::setName(std::string SetName)
{
  mp_SetDesc->setName(SetName);
}

// =====================================================================
// =====================================================================


void SimulOutSetDescModelImpl::setClass(std::string Class)
{
  mp_SetDesc->setUnitsClass(Class);
}

// =====================================================================
// =====================================================================


void SimulOutSetDescModelImpl::setFormatName(std::string FormatName)
{
  m_FormatName = FormatName;
}

// =====================================================================
// =====================================================================


void SimulOutSetDescModelImpl::setPrecision(int Precision)
{
  mp_SetDesc->setPrecision(Precision);
}

// =====================================================================
// =====================================================================


void SimulOutSetDescModelImpl::setIDs(std::vector<unsigned int> IDs)
{
  if (IDs.size() == m_ByClassIDs[mp_SetDesc->getUnitsClass()].size())
    mp_SetDesc->setAllUnits(true);
  else
  {
    mp_SetDesc->setAllUnits(false);
    mp_SetDesc->getUnitsIDs() = IDs;
  }
}

// =====================================================================
// =====================================================================


void SimulOutSetDescModelImpl::setVars(std::vector<std::string> Vars)
{
  if (Vars.size() == m_ByClassVars[mp_SetDesc->getUnitsClass()].size())
  {
    mp_SetDesc->setAllScalars(true);
    mp_SetDesc->setAllVectors(true);
  } else
  {
    mp_SetDesc->setAllScalars(false);
    mp_SetDesc->setAllVectors(false);

    mp_SetDesc->getScalars().clear();
    mp_SetDesc->getVectors().clear();

    for (unsigned int i = 0; i < Vars.size(); i++)
    {
      if (openfluid::tools::IsVectorNamedVariable(Vars[i]))
        mp_SetDesc->getVectors().push_back(
            openfluid::tools::GetVectorNamedVariableName(Vars[i]));
      else
        mp_SetDesc->getScalars().push_back(Vars[i]);
    }
  }
}

// =====================================================================
// =====================================================================


std::string SimulOutSetDescModelImpl::getName()
{
  return mp_SetDesc->getName();
}

// =====================================================================
// =====================================================================


std::string SimulOutSetDescModelImpl::getClass()
{
  return mp_SetDesc->getUnitsClass();
}

// =====================================================================
// =====================================================================


std::string SimulOutSetDescModelImpl::getFormatName()
{
  return m_FormatName;
}

// =====================================================================
// =====================================================================


int SimulOutSetDescModelImpl::getFormatIndex()
{
  for (unsigned int i = 0; i < m_FilesFormatsByNameVect.size(); i++)
  {
    if (m_FilesFormatsByNameVect[i].first == m_FormatName)
      return i;
  }
  return -1;
}

// =====================================================================
// =====================================================================


int SimulOutSetDescModelImpl::getPrecision()
{
  return mp_SetDesc->getPrecision();
}

// =====================================================================
// =====================================================================


std::vector<unsigned int> SimulOutSetDescModelImpl::getIDs()
{
  if (mp_SetDesc->isAllUnits())
    return m_ByClassIDs[getClass()];

  return mp_SetDesc->getUnitsIDs();
}

// =====================================================================
// =====================================================================


std::vector<std::string> SimulOutSetDescModelImpl::getVars()
{
  std::vector<std::string> VectorVars;
  std::vector<std::string> ScalarVars;

  if (mp_SetDesc->isAllVectors())
    VectorVars = m_ByClassVectorVars[getClass()];
  else
    VectorVars = mp_SetDesc->getVectors();

  if (mp_SetDesc->isAllScalars())
    ScalarVars = m_ByClassScalarVars[getClass()];
  else
    ScalarVars = mp_SetDesc->getScalars();

  std::vector<std::string> Vars = VectorVars;

  Vars.insert(Vars.begin(), ScalarVars.begin(), ScalarVars.end());
  return Vars;
}

// =====================================================================
// =====================================================================


void SimulOutSetDescModelImpl::save()
{
  if (checkSetNameFormat() && checkIdAtLeastOne() && checkVarAtLeastOne())
    m_signal_CloseDialogAsked.emit();
}

// =====================================================================
// =====================================================================


void SimulOutSetDescModelImpl::cancel()
{
  mp_SetDesc = 0;
  m_FormatName = "";
}

// =====================================================================
// =====================================================================

// =====================================================================
// =====================================================================


void SimulOutSetDescModelSub::extractClassesAndIDs()
{
  SimulOutSetDescModelImpl::extractClassesAndIDs();
}

// =====================================================================
// =====================================================================


void SimulOutSetDescModelSub::extractVars()
{
  SimulOutSetDescModelImpl::extractVars();
}

// =====================================================================
// =====================================================================


void SimulOutSetDescModelSub::extractFormatNames()
{
  SimulOutSetDescModelImpl::extractFormatNames();
}

// =====================================================================
// =====================================================================


void SimulOutSetDescModelSub::setFilesFormatsByNameVect(
    std::vector<std::pair<std::string, openfluid::base::OutputFilesDescriptor> > FilesFormatsByNameVect)
{
  m_FilesFormatsByNameVect = FilesFormatsByNameVect;
}

// =====================================================================
// =====================================================================


std::map<std::string, std::vector<std::string> > SimulOutSetDescModelSub::getByClassScalarVars()
{
  return m_ByClassScalarVars;
}

// =====================================================================
// =====================================================================


std::map<std::string, std::vector<std::string> > SimulOutSetDescModelSub::getByClassVectorVars()
{
  return m_ByClassVectorVars;
}
