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
 \file ProjectManager.cpp
 \brief Implements ...

 \author Jean-Christophe FABRE <fabrejc@supagro.inra.fr>
 */

#include <openfluid/base/ProjectManager.hpp>
#include <openfluid/config.hpp>
#include <glibmm/keyfile.h>
#include <glibmm/fileutils.h>
#include <fstream>

#include <boost/filesystem/path.hpp>
#include <boost/filesystem/convenience.hpp>
#include <boost/date_time/posix_time/posix_time.hpp>

namespace openfluid {
namespace base {

ProjectManager* ProjectManager::mp_Singleton = NULL;

// =====================================================================
// =====================================================================


ProjectManager::ProjectManager() :
  m_Path(""), m_Name(""), m_Description(""), m_Authors(""), m_CreationDate(""),
  m_LastModDate(""), m_IsIncOutputDir(false), m_IsOpened(false),
  m_KeyFileGroupName("OpenFLUID Project")
{

}

// =====================================================================
// =====================================================================


ProjectManager::~ProjectManager()
{

}

// =====================================================================
// =====================================================================


ProjectManager* ProjectManager::getInstance()
{
  if (mp_Singleton == NULL)
    mp_Singleton = new ProjectManager();
  return mp_Singleton;
}

// =====================================================================
// =====================================================================


bool ProjectManager::open(const Glib::ustring& Path)
{
  try
  {
    m_Path = Path;
    m_InputDir = getInputDirFromProjectPath(Path);
    m_OutputDir = getOuputDirFromProjectPath(Path);

    Glib::KeyFile KFile;
    KFile.load_from_file(getFilePathFromProjectPath(Path));

    m_Name = KFile.get_string(m_KeyFileGroupName, "Name");
    m_Description = KFile.get_string(m_KeyFileGroupName, "Description");
    m_Authors = KFile.get_string(m_KeyFileGroupName, "Authors");
    m_IsIncOutputDir = KFile.get_boolean(m_KeyFileGroupName, "IncOutput");
    m_CreationDate = KFile.get_string(m_KeyFileGroupName, "CreationDate");
    m_LastModDate = KFile.get_string(m_KeyFileGroupName, "LastModDate");

    m_IsOpened = true;

    return true;

  } catch (Glib::FileError e)
  {
    std::cerr << "ProjectManager::open Glib::FileError : " << e.what()
        << std::endl;

    return false;

  } catch (Glib::KeyFileError e)
  {
    std::cerr << "ProjectManager::open Glib::KeyFileError : " << e.what()
        << std::endl;

    return false;
  }
}

// =====================================================================
// =====================================================================


std::string ProjectManager::getFilePathFromProjectPath(
    Glib::ustring ProjectPath)
{
  return Glib::ustring::compose("%1/%2", ProjectPath,
      openfluid::config::PROJECT_FILE);
}

// =====================================================================
// =====================================================================


std::string ProjectManager::getInputDirFromProjectPath(
    Glib::ustring ProjectPath)
{
  return Glib::ustring::compose("%1/%2", ProjectPath,
      openfluid::config::PROJECT_INPUTDIR);
}

// =====================================================================
// =====================================================================


std::string ProjectManager::getOuputDirFromProjectPath(
    Glib::ustring ProjectPath)
{
  return Glib::ustring::compose("%1/%2", ProjectPath,
      openfluid::config::PROJECT_OUTPUTDIRPREFIX);
}

// =====================================================================
// =====================================================================

/* throws boost::filesystem::basic_filesystem_error<boost::filesystem::path> */
bool ProjectManager::create(const Glib::ustring& Path,
    const Glib::ustring& Name, const Glib::ustring& Description,
    const Glib::ustring& Authors, const bool Inc)
{
  if (boost::filesystem::create_directories(getInputDirFromProjectPath(Path)))
  {
    m_Path = Path;
    m_InputDir = getInputDirFromProjectPath(Path);
    m_OutputDir = getOuputDirFromProjectPath(Path);

    m_Name = Name;
    m_Description = Description;
    m_Authors = Authors;
    m_IsIncOutputDir = Inc;
    m_CreationDate = getNow();

    m_IsOpened = true;

    return save();
  }

  return false;
}

// =====================================================================
// =====================================================================


Glib::ustring ProjectManager::getNow()
{
  return boost::posix_time::to_iso_string(
      boost::posix_time::second_clock::local_time());
}

// =====================================================================
// =====================================================================


bool ProjectManager::save()
{

  std::ofstream PrjFile;

  PrjFile.open(getFilePathFromProjectPath(m_Path).c_str());

  if (PrjFile.fail())
    return false;

  m_LastModDate = getNow();

  try
  {
    Glib::KeyFile KFile;

    KFile.set_string(m_KeyFileGroupName, "Name", m_Name);
    KFile.set_string(m_KeyFileGroupName, "Description", m_Description);
    KFile.set_string(m_KeyFileGroupName, "Authors", m_Authors);
    KFile.set_boolean(m_KeyFileGroupName, "IncOutput", m_IsIncOutputDir);
    KFile.set_string(m_KeyFileGroupName, "CreationDate", m_CreationDate);
    KFile.set_string(m_KeyFileGroupName, "LastModDate", m_LastModDate);

    PrjFile << KFile.to_data();

  } catch (Glib::KeyFileError e)
  {
    std::cerr << "ProjectManager::create Glib::KeyFileError : " << e.what()
        << std::endl;

    return false;
  }

  PrjFile.close();

  return true;
}

// =====================================================================
// =====================================================================


void ProjectManager::close()
{
  m_Path = m_InputDir = m_OutputDir = m_Name = m_Description = m_Authors
      = m_CreationDate = m_LastModDate = "";

  m_IsIncOutputDir = m_IsOpened = false;

}

// =====================================================================
// =====================================================================


void ProjectManager::updateOutputDir()
{
  if (m_IsOpened && m_IsIncOutputDir)
  {
    Glib::ustring Now = getNow();
    Now.replace(8,1,"-");

    m_OutputDir = Glib::ustring::compose("%1_%2", getOuputDirFromProjectPath(
        m_Path), Now);
  } else
    m_OutputDir = getOuputDirFromProjectPath(m_Path);
}

// =====================================================================
// =====================================================================


bool ProjectManager::isProject(const Glib::ustring& Path)
{
  if (boost::filesystem::exists(getFilePathFromProjectPath(Path)))
  {
    try
    {
      Glib::KeyFile KFile;
      KFile.load_from_file(getFilePathFromProjectPath(Path));
      return boost::filesystem::exists(getInputDirFromProjectPath(Path));
    } catch (Glib::FileError e)
    {
      return false;
    } catch (Glib::KeyFileError e)
    {
      return false;
    }
  }
  return false;
}

// =====================================================================
// =====================================================================


}
} //namespaces


