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
 \file ResUnitChooserModel.cpp
 \brief Implements ...

 \author Aline LIBRES <libres@supagro.inra.fr>
 */

#include "ResUnitChooserModel.hpp"

#include <boost/foreach.hpp>

#include <openfluid/base/OutputDescriptor.hpp>
#include <openfluid/base/OutputSetDescriptor.hpp>
#include <openfluid/ware/FunctionSignature.hpp>
#include <openfluid/core/Unit.hpp>
#include <openfluid/core/CoreRepository.hpp>
#include <openfluid/machine/ModelItemInstance.hpp>
#include <openfluid/machine/ModelInstance.hpp>


// =====================================================================
// =====================================================================

void ResUnitChooserModelImpl::extractSet()
{
  bool quit = false;

  for (unsigned int i = 0; i < mp_OutDesc->getFileSets().size() && !quit; i++)
  {
    for (unsigned int j = 0; j < mp_OutDesc->getFileSets()[i].getSets().size()
        && !quit; j++)
    {
      if (mp_OutDesc->getFileSets()[i].getSets()[j].getName() == m_SetName)
      {
        mp_SetDesc = &mp_OutDesc->getFileSets()[i].getSets()[j];
        quit = true;
      }
    }
  }
}

// =====================================================================
// =====================================================================


void ResUnitChooserModelImpl::extractIdsInfo()
{
  if (!mp_SetDesc)
    return;

  m_IDs.clear();

  if (!mp_SetDesc->isAllUnits())
    m_IDs = mp_SetDesc->getUnitsIDs();
  else
  {
    BOOST_FOREACH(openfluid::core::Unit Unit, *mp_CoreRepos->getUnits(m_ClassName)->getList())
{    m_IDs.push_back(Unit.getID());
  }
}

std::sort(m_IDs.begin(), m_IDs.end());
}

// =====================================================================
// =====================================================================


void ResUnitChooserModelImpl::extractVarsInfo()
{
  if (!mp_SetDesc)
    return;

  m_Variables.clear();

  if(!mp_SetDesc->isAllVariables())
    m_Variables = mp_SetDesc->getVariables();
  else
  {
    BOOST_FOREACH(openfluid::machine::ModelItemInstance* Item,mp_ModelInstance->getItems())
    {    BOOST_FOREACH(openfluid::ware::SignatureHandledTypedDataItem Var,Item->Signature->HandledData.ProducedVars)
      {
        if(Var.UnitClass == m_ClassName)
          m_Variables.push_back(Var.DataName);
      }
    BOOST_FOREACH(openfluid::ware::SignatureHandledTypedDataItem Var,Item->Signature->HandledData.UpdatedVars)
    {
      if(Var.UnitClass == m_ClassName)
        m_Variables.push_back(Var.DataName);
    }
    }
  }
}

// =====================================================================
// =====================================================================


ResUnitChooserModelImpl::ResUnitChooserModelImpl() :
  mp_OutDesc(0), mp_CoreRepos(0), mp_ModelInstance(0), mp_SetDesc(0),
      m_SetName(""), m_ClassName(""), m_SelectedUnitId(-1), m_ShowFiles(false)
{
}

// =====================================================================
// =====================================================================


sigc::signal<void> ResUnitChooserModelImpl::signal_FromAppInit()
{
  return m_signal_FromAppInit;
}

// =====================================================================
// =====================================================================


sigc::signal<void> ResUnitChooserModelImpl::signal_ViewResultAsked()
{
  return m_signal_ViewResultAsked;
}

// =====================================================================
// =====================================================================


sigc::signal<void, std::string> ResUnitChooserModelImpl::signal_MessageAsked()
{
  return m_signal_MessageAsked;
}

// =====================================================================
// =====================================================================


void ResUnitChooserModelImpl::setEngineRequirements(
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


std::string ResUnitChooserModelImpl::getClassName()
{
  return m_ClassName;
}

// =====================================================================
// =====================================================================


std::vector<unsigned int> ResUnitChooserModelImpl::getIDs()
{
  return m_IDs;
}

// =====================================================================
// =====================================================================

std::vector<std::string> ResUnitChooserModelImpl::getVariables()
{
  return m_Variables;
}

// =====================================================================
// =====================================================================


void ResUnitChooserModelImpl::setSelectedUnit(int UnitId)
{
  m_SelectedUnitId = UnitId;
}

// =====================================================================
// =====================================================================


void ResUnitChooserModelImpl::setSelectedVars(std::vector<std::string> Vars)
{
  m_SelectedVars = Vars;
}

// =====================================================================
// =====================================================================


void ResUnitChooserModelImpl::setShowFiles(bool ShowFiles)
{
  m_ShowFiles = ShowFiles;
}

// =====================================================================
// =====================================================================


void ResUnitChooserModelImpl::setSelectedSetFromApp(std::string SetName)
{
  m_SetName = SetName;
  update();
}

// =====================================================================
// =====================================================================


void ResUnitChooserModelImpl::update()
{
  if (m_SetName != "")
  {
    extractSet();
    if (!mp_SetDesc)
      return;
    m_ClassName = mp_SetDesc->getUnitsClass();
    extractIdsInfo();
    extractVarsInfo();
    m_signal_FromAppInit.emit();
  }
}

// =====================================================================
// =====================================================================


openfluid::core::Unit* ResUnitChooserModelImpl::getSelectedUnit()
{
  if (openfluid::core::Unit* Unit = mp_CoreRepos->getUnit(m_ClassName,m_SelectedUnitId))
    return Unit;
  return (openfluid::core::Unit*) 0;
}

// =====================================================================
// =====================================================================


openfluid::base::OutputSetDescriptor* ResUnitChooserModelImpl::getSelectedSetDescriptor()
{
  return mp_SetDesc;
}


// =====================================================================
// =====================================================================


std::vector<std::string> ResUnitChooserModelImpl::getSelectedVars()
{
  return m_SelectedVars;
}

// =====================================================================
// =====================================================================


bool ResUnitChooserModelImpl::getShowFiles()
{
  return m_ShowFiles;
}

// =====================================================================
// =====================================================================


void ResUnitChooserModelImpl::validate()
{
  m_signal_ViewResultAsked.emit();
}

// =====================================================================
// =====================================================================


void ResUnitChooserModelImpl::setMessage(std::string MessageText)
{
  m_signal_MessageAsked.emit(MessageText);
}

// =====================================================================
// =====================================================================

// =====================================================================
// =====================================================================


void ResUnitChooserModelSub::setSetAndClassName(std::string SetName,
    std::string ClassName)
{
  m_SetName = SetName;
  m_ClassName = ClassName;
}

// =====================================================================
// =====================================================================


void ResUnitChooserModelSub::extractSet()
{
  ResUnitChooserModelImpl::extractSet();
}

// =====================================================================
// =====================================================================


void ResUnitChooserModelSub::extractIdsInfo()
{
  ResUnitChooserModelImpl::extractIdsInfo();
}

// =====================================================================
// =====================================================================


void ResUnitChooserModelSub::extractVarsInfo()
{
  ResUnitChooserModelImpl::extractVarsInfo();
}
