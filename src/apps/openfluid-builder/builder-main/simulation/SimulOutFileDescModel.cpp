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
 \file SimulOutFileDescModel.cpp
 \brief Implements ...

 \author Aline LIBRES <libres@supagro.inra.fr>
 */

#include "SimulOutFileDescModel.hpp"

#include <glibmm/i18n.h>


bool SimulOutFileDescModelImpl::checkFileFormatName()
{
  if (m_FormatName == "")
  {
    m_signal_ErrorDialogAsked.emit(_("File format name cannot be empty"));
    return false;
  } else
  {
    for (unsigned int i = 0; i < m_FilesFormatsByNameVect.size(); i++)
      if (m_FilesFormatsByNameVect[i].first == m_FormatName)
      {
        m_signal_ErrorDialogAsked.emit(_("This file format name already exists"));
        return false;
      }
  }
  return true;
}

SimulOutFileDescModelImpl::SimulOutFileDescModelImpl() :
  m_FormatName(""), mp_FileDesc(0)
{
  m_Chars.push_back(":");
  m_Chars.push_back("%");
  m_Chars.push_back(";");
  m_Chars.push_back(",");
  m_Chars.push_back("#");
  m_Chars.push_back("*");
  //      m_Chars.push_back("[blank]");
  m_Chars.push_back(" ");

  m_DateFormats.push_back("%Y%m%dT%H%M%S");
  m_DateFormats.push_back("%Y-%m-%dT%H:%M:%S");
  m_DateFormats.push_back("%Y-%m-%d %H:%M:%S");
  m_DateFormats.push_back("%Y %m %d %H %M %S");
  m_DateFormats.push_back("%d/%m/%Y %H:%M:%S");
  m_DateFormats.push_back("%Y-%m-%d %H.%M");
}
sigc::signal<void> SimulOutFileDescModelImpl::signal_InitForCreationAsked()
{
  return m_signal_InitForCreationAsked;
}
sigc::signal<void> SimulOutFileDescModelImpl::signal_InitForEditionAsked()
{
  return m_signal_InitForEditionAsked;
}
sigc::signal<void> SimulOutFileDescModelImpl::signal_ShowDialogAsked()
{
  return m_signal_ShowDialogAsked;
}
sigc::signal<void> SimulOutFileDescModelImpl::signal_CloseDialogAsked()
{
  return m_signal_CloseDialogAsked;
}
sigc::signal<void, std::string> SimulOutFileDescModelImpl::signal_ErrorDialogAsked()
{
  return m_signal_ErrorDialogAsked;
}
void SimulOutFileDescModelImpl::setEngineRequirements(
    openfluid::base::OutputDescriptor& OutDesc)
{
  mp_OutDesc = &OutDesc;
}
void SimulOutFileDescModelImpl::initialize(
    openfluid::base::OutputFilesDescriptor* FileDesc,
    std::string FileFormatName, std::vector<std::pair<std::string,
        openfluid::base::OutputFilesDescriptor> > FilesFormatsByNameVect)
{
  m_FormatName = FileFormatName;
  m_FilesFormatsByNameVect = FilesFormatsByNameVect;

  if (FileDesc)
  {
    mp_FileDesc = FileDesc;
    m_signal_InitForEditionAsked.emit();
  } else
  {
    mp_FileDesc = new openfluid::base::OutputFilesDescriptor();
    m_signal_InitForCreationAsked.emit();
  }
}
void SimulOutFileDescModelImpl::showDialog()
{
  m_signal_ShowDialogAsked.emit();
}
openfluid::base::OutputFilesDescriptor* SimulOutFileDescModelImpl::getFileDesc()
{
  return mp_FileDesc;
}
std::string SimulOutFileDescModelImpl::getFormatName()
{
  return m_FormatName;
}
std::vector<std::string> SimulOutFileDescModelImpl::getChars()
{
  return m_Chars;
}
std::vector<std::string> SimulOutFileDescModelImpl::getDateFormats()
{
  return m_DateFormats;
}
std::string SimulOutFileDescModelImpl::getColSeparator()
{
  return mp_FileDesc->getColSeparator();
}
std::string SimulOutFileDescModelImpl::getDateFormat()
{
  return mp_FileDesc->getDateFormat();
}
std::string SimulOutFileDescModelImpl::getCommentChar()
{
  return mp_FileDesc->getCommentChar();
}
void SimulOutFileDescModelImpl::setColSeparator(std::string Value)
{
  mp_FileDesc->setColSeparator(Value);
}
void SimulOutFileDescModelImpl::setDateFormat(std::string Value)
{
  mp_FileDesc->setDateFormat(Value);
}
void SimulOutFileDescModelImpl::setCommentChar(std::string Value)
{
  mp_FileDesc->setCommentChar(Value);
}
void SimulOutFileDescModelImpl::setFormatName(std::string Value)
{
  m_FormatName = Value;
}
void SimulOutFileDescModelImpl::save()
{
  if (checkFileFormatName())
    m_signal_CloseDialogAsked.emit();
}
void SimulOutFileDescModelImpl::cancel()
{
  mp_FileDesc = 0;
  m_FormatName = "";
}
