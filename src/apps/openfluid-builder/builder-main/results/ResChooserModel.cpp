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
 \file ResChooserModel.cpp
 \brief Implements ...

 \author Aline LIBRES <libres@supagro.inra.fr>
 */

#include "ResChooserModel.hpp"

#include <boost/foreach.hpp>

// =====================================================================
// =====================================================================


void ResChooserModelImpl::extractSets()
{
  m_BySetNameClassName.clear();
  m_BySetNameSetDesc.clear();
  m_BySetNameIDs.clear();

  for (unsigned int i = 0; i < mp_OutDesc->getFileSets().size(); i++)
  {
    for (unsigned int j = 0; j < mp_OutDesc->getFileSets()[i].getSets().size(); j++)
    {
      openfluid::base::OutputSetDescriptor* SetDesc =
          &mp_OutDesc->getFileSets()[i].getSets()[j];
      std::string SetName = SetDesc->getName();
      std::string ClassName = SetDesc->getUnitsClass();

      m_BySetNameClassName[SetName] = ClassName;
      m_BySetNameSetDesc[SetName] = SetDesc;

      std::vector<unsigned int> IDs;
      if (!SetDesc->isAllUnits())
        IDs = SetDesc->getUnitsIDs();
      else
      {
        BOOST_FOREACH(openfluid::core::Unit Unit, *mp_CoreRepos->getUnits(ClassName)->getList())
IDs        .push_back(Unit.getID());
      }
      std::sort(IDs.begin(), IDs.end());

      m_BySetNameIDs[SetName] = IDs;
    }
  }
}

// =====================================================================
// =====================================================================


ResChooserModelImpl::ResChooserModelImpl() :
  mp_OutDesc(0), mp_CoreRepos(0), mp_ModelInstance(0), m_SelectedSetName(""),
      m_SelectedUnitInfo(std::make_pair("", -1))
{
}

// =====================================================================
// =====================================================================


sigc::signal<void> ResChooserModelImpl::signal_FromAppInit()
{
  return m_signal_FromAppInit;
}

// =====================================================================
// =====================================================================


sigc::signal<void> ResChooserModelImpl::signal_ViewResultAsked()
{
  return m_signal_ViewResultAsked;
}

// =====================================================================
// =====================================================================


sigc::signal<void, std::string> ResChooserModelImpl::signal_MessageAsked()
{
  return m_signal_MessageAsked;
}

// =====================================================================
// =====================================================================


void ResChooserModelImpl::setEngineRequirements(
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


void ResChooserModelImpl::initialize()
{
  extractSets();

  m_signal_FromAppInit.emit();
}

// =====================================================================
// =====================================================================


std::map<std::string, std::string> ResChooserModelImpl::getBySetNameClassName()
{
  return m_BySetNameClassName;
}

// =====================================================================
// =====================================================================


std::map<std::string, std::vector<unsigned int> > ResChooserModelImpl::getBySetNameIDs()
{
  return m_BySetNameIDs;
}

// =====================================================================
// =====================================================================


void ResChooserModelImpl::setSelectedUnitInfo(
    std::pair<std::string, int> UnitInfo)
{
  if (UnitInfo.first != "" && UnitInfo.second > -1)
    m_SelectedUnitInfo = UnitInfo;
}

// =====================================================================
// =====================================================================


void ResChooserModelImpl::setSelectedSetName(std::string SetName)
{
  m_SelectedSetName = SetName;
}

// =====================================================================
// =====================================================================


openfluid::core::Unit* ResChooserModelImpl::getSelectedUnit()
{
  if (openfluid::core::Unit* Unit = mp_CoreRepos->getUnit(m_SelectedUnitInfo.first,m_SelectedUnitInfo.second))
    return Unit;
  return (openfluid::core::Unit*) 0;
}

// =====================================================================
// =====================================================================


openfluid::base::OutputSetDescriptor* ResChooserModelImpl::getSelectedSetDescriptor()
{
  if (m_BySetNameSetDesc.find(m_SelectedSetName) != m_BySetNameSetDesc.end())
    return m_BySetNameSetDesc[m_SelectedSetName];
  return (openfluid::base::OutputSetDescriptor*) 0;
}

// =====================================================================
// =====================================================================


void ResChooserModelImpl::validate()
{
  m_signal_ViewResultAsked.emit();
}

// =====================================================================
// =====================================================================


void ResChooserModelImpl::update()
{
  initialize();
}

// =====================================================================
// =====================================================================

// =====================================================================
// =====================================================================


void ResChooserModelSub::extractSets()
{
  ResChooserModelImpl::extractSets();
}

// =====================================================================
// =====================================================================


std::string ResChooserModelSub::getSelectedSetName()
{
  return m_SelectedSetName;
}

// =====================================================================
// =====================================================================


std::pair<std::string, unsigned int> ResChooserModelSub::getSelectedUnitInfo()
{
  return m_SelectedUnitInfo;
}
