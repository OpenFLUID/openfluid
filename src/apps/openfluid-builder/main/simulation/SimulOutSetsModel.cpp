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

#include <boost/foreach.hpp>

bool SimulOutSetsModelImpl::isSelectedSetValid()
{
  return (m_SetsByName.find(m_SelectedSetName) != m_SetsByName.end());
}

SimulOutSetsModelImpl::SimulOutSetsModelImpl() :
  mp_OutDesc(0)
{
}
sigc::signal<void> SimulOutSetsModelImpl::signal_FromAppDescriptorChanged()
{
  return m_signal_FromAppDescriptorChanged;
}
sigc::signal<void> SimulOutSetsModelImpl::signal_FromUserSelectionChanged()
{
  return m_signal_FromUserSelectionChanged;
}
Glib::ustring SimulOutSetsModelImpl::generateFormatName(int Index)
{
  return Glib::ustring::compose("Format #%1", Index + 1);
}
void SimulOutSetsModelImpl::setEngineRequirements(
    openfluid::base::OutputDescriptor& OutDesc)
{
  mp_OutDesc = &OutDesc;

  std::vector<openfluid::base::OutputFilesDescriptor> FilesFormats =
      OutDesc.getFileSets();
  for (unsigned int i = 0; i < FilesFormats.size(); i++)
  {
    std::string FormatName = generateFormatName(i);
    BOOST_FOREACH(openfluid::base::OutputSetDescriptor OutSetDesc,FilesFormats[i].getSets())
{    m_SetsByName [OutSetDesc.getName()] = std::make_pair(FormatName,
        OutSetDesc);
  }
}

m_SelectedSetName = "";

m_signal_FromAppDescriptorChanged.emit();
}
std::map<std::string, std::pair<std::string,
    openfluid::base::OutputSetDescriptor> > SimulOutSetsModelImpl::getSetsByName()
{
  return m_SetsByName;
}
void SimulOutSetsModelImpl::setSelectedSetName(std::string SetName)
{
  m_SelectedSetName = SetName;
  m_signal_FromUserSelectionChanged.emit();
}
openfluid::base::OutputSetDescriptor* SimulOutSetsModelImpl::getSelectedSet()
{
  if (isSelectedSetValid())
  {
    for (unsigned int i = 0; i < mp_OutDesc->getFileSets().size(); i++)
    {
      for (unsigned int j = 0; j
          < mp_OutDesc->getFileSets()[i].getSets().size(); j++)
      {
        if (mp_OutDesc->getFileSets()[i].getSets()[j].getName()
            == m_SelectedSetName)
          return &(mp_OutDesc->getFileSets()[i].getSets()[j]);
      }
    }
  }
  return (openfluid::base::OutputSetDescriptor*) 0;
}
std::string SimulOutSetsModelImpl::getSelectedSetFormatName()
{
  if (isSelectedSetValid())
    return m_SetsByName[m_SelectedSetName].first;
  return "";
}
void SimulOutSetsModelImpl::deleteSelectedSet()
{
  if (isSelectedSetValid())
  {
    for (unsigned int i = 0; i < mp_OutDesc->getFileSets().size(); i++)
    {
      for (unsigned int j = 0; j
          < mp_OutDesc->getFileSets()[i].getSets().size(); j++)
      {
        if (mp_OutDesc->getFileSets()[i].getSets()[j].getName()
            == m_SelectedSetName)
        {
          mp_OutDesc->getFileSets()[i].getSets().erase(
              mp_OutDesc->getFileSets()[i].getSets().begin() + j);
          m_SetsByName.erase(m_SelectedSetName);
          m_SelectedSetName = "";

          m_signal_FromAppDescriptorChanged.emit();
        }
      }
    }
  }
}
void SimulOutSetsModelImpl::addSet(
    openfluid::base::OutputSetDescriptor* SetDesc, std::string FormatName,
    int FormatIndex)
{
  if (SetDesc)
  {
    mp_OutDesc->getFileSets()[FormatIndex].getSets().push_back(*SetDesc);
    m_SetsByName[SetDesc->getName()] = std::make_pair(FormatName, *SetDesc);
    m_SelectedSetName = "";
    m_signal_FromAppDescriptorChanged.emit();
  }
}
void SimulOutSetsModelImpl::updateSelectedSet(std::string NewFormatName)
{
  m_SetsByName[m_SelectedSetName] = std::make_pair(NewFormatName,
      *getSelectedSet());

  m_signal_FromAppDescriptorChanged.emit();
}
void SimulOutSetsModelImpl::updateFileFormats(
    std::vector<std::pair<std::string, openfluid::base::OutputFilesDescriptor> > FilesFormatsByNameVect)
{
  m_SetsByName.clear();
  for (unsigned int i = 0; i < FilesFormatsByNameVect.size(); i++)
  {
    std::string FormatName = FilesFormatsByNameVect[i].first;
    openfluid::base::OutputFilesDescriptor Formats =
        FilesFormatsByNameVect[i].second;

    std::cout << "i " << i << "foramtname : " << FormatName << std::endl;
    BOOST_FOREACH(openfluid::base::OutputSetDescriptor OutDesc,FilesFormatsByNameVect[i].second.getSets())
{    std::cout << "--setname " << OutDesc.getName() << std::endl;
    m_SetsByName [OutDesc.getName()] = std::make_pair(FormatName,
        OutDesc);
  }
}

m_SelectedSetName = "";
m_signal_FromAppDescriptorChanged.emit();
}

