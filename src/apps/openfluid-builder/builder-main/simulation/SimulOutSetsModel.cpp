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
 \file SimulOutSetsModel.cpp
 \brief Implements ...

 \author Aline LIBRES <libres@supagro.inra.fr>
 */

#include "SimulOutSetsModel.hpp"

// =====================================================================
// =====================================================================


SimulOutSetsModelImpl::SimulOutSetsModelImpl() :
  mp_OutDesc(0), mp_CoreRepos(0), mp_ModelInstance(0)
{
}

// =====================================================================
// =====================================================================


sigc::signal<void> SimulOutSetsModelImpl::signal_FromAppDescriptorChanged()
{
  return m_signal_FromAppDescriptorChanged;
}

// =====================================================================
// =====================================================================


sigc::signal<void> SimulOutSetsModelImpl::signal_FromUserSelectionChanged()
{
  return m_signal_FromUserSelectionChanged;
}

// =====================================================================
// =====================================================================


sigc::signal<void> SimulOutSetsModelImpl::signal_Activated()
{
  return m_signal_Activated;
}

// =====================================================================
// =====================================================================


void SimulOutSetsModelImpl::setEngineRequirements(
    openfluid::base::OutputDescriptor& OutDesc,
    openfluid::core::CoreRepository& CoreRepos,
    openfluid::machine::ModelInstance& ModelInstance)
{
  mp_OutDesc = &OutDesc;
  mp_CoreRepos = &CoreRepos;
  mp_ModelInstance = &ModelInstance;

  m_SelectedSetName = "";

  update();
}

// =====================================================================
// =====================================================================


void SimulOutSetsModelImpl::update()
{
  m_signal_FromAppDescriptorChanged.emit();
}

// =====================================================================
// =====================================================================


openfluid::base::OutputDescriptor* SimulOutSetsModelImpl::getOutputDescriptor()
{
  return mp_OutDesc;
}

// =====================================================================
// =====================================================================


void SimulOutSetsModelImpl::setSelectedSetName(std::string SetName)
{
  m_SelectedSetName = SetName;

  m_signal_FromUserSelectionChanged.emit();
}

// =====================================================================
// =====================================================================


openfluid::base::OutputSetDescriptor* SimulOutSetsModelImpl::getSelectedSet()
{
  if (m_SelectedSetName == "")
    return (openfluid::base::OutputSetDescriptor*) 0;

  for (unsigned int i = 0; i < mp_OutDesc->getFileSets().size(); i++)
  {
    for (unsigned int j = 0; j < mp_OutDesc->getFileSets()[i].getSets().size(); j++)
    {
      if (mp_OutDesc->getFileSets()[i].getSets()[j].getName()
          == m_SelectedSetName)
        return &(mp_OutDesc->getFileSets()[i].getSets()[j]);
    }
  }

  return (openfluid::base::OutputSetDescriptor*) 0;
}

// =====================================================================
// =====================================================================


std::string SimulOutSetsModelImpl::getSelectedSetName()
{
  if (!existsSetName(m_SelectedSetName))
    m_SelectedSetName = "";

  return m_SelectedSetName;
}

// =====================================================================
// =====================================================================


void SimulOutSetsModelImpl::deleteSelectedSet()
{
  for (unsigned int i = 0; i < mp_OutDesc->getFileSets().size(); i++)
  {
    for (unsigned int j = 0; j < mp_OutDesc->getFileSets()[i].getSets().size(); j++)
    {
      if (mp_OutDesc->getFileSets()[i].getSets()[j].getName()
          == m_SelectedSetName)
      {
        mp_OutDesc->getFileSets()[i].getSets().erase(
            mp_OutDesc->getFileSets()[i].getSets().begin() + j);

        m_SelectedSetName = "";

        m_signal_FromAppDescriptorChanged.emit();
      }
    }
  }
}

// =====================================================================
// =====================================================================

void SimulOutSetsModelImpl::addSet(
    openfluid::base::OutputSetDescriptor* SetDesc, std::string FormatName)
{
  if (SetDesc)
  {
    std::string SetName = SetDesc->getName();

    for (unsigned int i = 0; i < mp_OutDesc->getFileSets().size(); i++)
    {
      if (mp_OutDesc->getFileSets()[i].getName() == FormatName)
        mp_OutDesc->getFileSets()[i].getSets().push_back(*SetDesc);
    }

    m_SelectedSetName = SetName;

    m_signal_FromAppDescriptorChanged.emit();
  }
}

// =====================================================================
// =====================================================================


void SimulOutSetsModelImpl::updateSelectedSet(
    openfluid::base::OutputSetDescriptor* UpdatedSetDesc,
    std::string FormatName)
{
  if (UpdatedSetDesc)
  {
    deleteSelectedSet();

    addSet(UpdatedSetDesc, FormatName);
  }
}

// =====================================================================
// =====================================================================

openfluid::machine::ModelInstance* SimulOutSetsModelImpl::getModelInstance()
{
  return mp_ModelInstance;
}

// =====================================================================
// =====================================================================


bool SimulOutSetsModelImpl::existsSetName(std::string SetName)
{
  for (unsigned int i = 0; i < mp_OutDesc->getFileSets().size(); i++)
  {
    for (unsigned int j = 0; j < mp_OutDesc->getFileSets()[i].getSets().size(); j++)
    {
      if (mp_OutDesc->getFileSets()[i].getSets()[j].getName() == SetName)
        return true;
    }
  }

  return false;
}
