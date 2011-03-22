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
 \file SimulOutFilesModel.cpp
 \brief Implements ...

 \author Aline LIBRES <libres@supagro.inra.fr>
 */

#include "SimulOutFilesModel.hpp"

#include <boost/foreach.hpp>

bool SimulOutFilesModelImpl::isSelectedFileFormatIndexValid()
{
  return (m_SelectedFileFormatIndex > -1 && m_SelectedFileFormatIndex
      < (int) m_FilesFormatsByNameVect.size());
}

SimulOutFilesModelImpl::SimulOutFilesModelImpl() :
  m_CurrentFormatNameIndex(0), m_SelectedFileFormatIndex(-1), mp_OutDesc(0)
{
}
sigc::signal<void> SimulOutFilesModelImpl::signal_FromAppDescriptorChanged()
{
  return m_signal_FromAppDescriptorChanged;
}
sigc::signal<void> SimulOutFilesModelImpl::signal_FromUserSelectionChanged()
{
  return m_signal_FromUserSelectionChanged;
}
sigc::signal<void> SimulOutFilesModelImpl::signal_FromAppShowDialogConfirmDeletionAsked()
{
  return m_signal_FromAppShowDialogConfirmDeletionAsked;
}
Glib::ustring SimulOutFilesModelImpl::generateNextFormatName()
{
  m_CurrentFormatNameIndex++;
  return Glib::ustring::compose("Format #%1", m_CurrentFormatNameIndex);
}
void SimulOutFilesModelImpl::setEngineRequirements(
    openfluid::base::OutputDescriptor& OutDesc)
{
  mp_OutDesc = &OutDesc;
  std::vector<openfluid::base::OutputFilesDescriptor> FilesFormats =
      OutDesc.getFileSets();
  for (unsigned int i = 0; i < FilesFormats.size(); i++)
    m_FilesFormatsByNameVect.push_back(std::make_pair(generateNextFormatName(),
        FilesFormats[i]));

  m_SelectedFileFormatIndex = -1;

  m_signal_FromAppDescriptorChanged.emit();
}
std::vector<std::pair<std::string, openfluid::base::OutputFilesDescriptor> > SimulOutFilesModelImpl::getFilesFormatsByNameVect()
{
  return m_FilesFormatsByNameVect;
}
void SimulOutFilesModelImpl::setSelectedFileFormatIndex(int Index)
{
  m_SelectedFileFormatIndex = Index;
  m_signal_FromUserSelectionChanged.emit();
}
openfluid::base::OutputFilesDescriptor* SimulOutFilesModelImpl::getSelectedFileFormat()
{
  if (isSelectedFileFormatIndexValid())
    return &mp_OutDesc->getFileSets()[m_SelectedFileFormatIndex];
  return (openfluid::base::OutputFilesDescriptor*) 0;
}
std::string SimulOutFilesModelImpl::getSelectedFileFormatName()
{
  if (isSelectedFileFormatIndexValid())
    return m_FilesFormatsByNameVect[m_SelectedFileFormatIndex].first;
  return "";
}
void SimulOutFilesModelImpl::deleteSelectedFileFormat()
{
  if (isSelectedFileFormatIndexValid())
  {
    if (!mp_OutDesc->getFileSets()[m_SelectedFileFormatIndex].getSets().empty())
      m_signal_FromAppShowDialogConfirmDeletionAsked.emit();
    else
      deleteSelectedFileFormatConfirmed();
  }
}
void SimulOutFilesModelImpl::deleteSelectedFileFormatConfirmed()
{
  mp_OutDesc->getFileSets().erase(mp_OutDesc->getFileSets().begin()
      + m_SelectedFileFormatIndex);

  m_FilesFormatsByNameVect.erase(m_FilesFormatsByNameVect.begin()
      + m_SelectedFileFormatIndex);

  m_SelectedFileFormatIndex = -1;
  m_signal_FromAppDescriptorChanged.emit();
}
void SimulOutFilesModelImpl::addFileFormat(
    openfluid::base::OutputFilesDescriptor* FileDesc, std::string FormatName)
{
  if (FileDesc)
  {
    mp_OutDesc->getFileSets().push_back(*FileDesc);

    m_FilesFormatsByNameVect.push_back(std::make_pair(FormatName, *FileDesc));

    m_SelectedFileFormatIndex = -1;
    m_signal_FromAppDescriptorChanged.emit();
  }
}
void SimulOutFilesModelImpl::updateSelectedFileFormat(std::string FormatName)
{
  m_FilesFormatsByNameVect[m_SelectedFileFormatIndex].first = FormatName;
  m_FilesFormatsByNameVect[m_SelectedFileFormatIndex].second
      = *getSelectedFileFormat();

  m_signal_FromAppDescriptorChanged.emit();
}

int SimulOutFilesModelSub::getSelectedFileFormatIndex()
{
  return m_SelectedFileFormatIndex;
}

