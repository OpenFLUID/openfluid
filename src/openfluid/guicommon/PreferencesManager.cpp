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
 \file PreferencesManager.cpp
 \brief Implements ...

 \author Aline LIBRES <libres@supagro.inra.fr>
 */

#include "PreferencesManager.hpp"

#include <openfluid/base/RuntimeEnv.hpp>
#include <boost/filesystem.hpp>
#include <glibmm/fileutils.h>
#include <fstream>


namespace openfluid { namespace guicommon {


PreferencesManager* PreferencesManager::mp_Instance = 0;

// =====================================================================
// =====================================================================


PreferencesManager* PreferencesManager::getInstance()
{
  if (!mp_Instance)
    mp_Instance = new PreferencesManager();

  return mp_Instance;
}

// =====================================================================
// =====================================================================


PreferencesManager::PreferencesManager()
{
  m_FileName
      = openfluid::base::RuntimeEnvironment::getInstance()->getDefaultConfigFile();

  mp_KFile = new Glib::KeyFile();

  if (!boost::filesystem::exists(m_FileName))
  {
    setDefaultValues();
    save();
  } else
  {
    loadKeyFile();
  }

}

// =====================================================================
// =====================================================================


PreferencesManager::~PreferencesManager()
{
  mp_Instance = 0;
}

// =====================================================================
// =====================================================================

void PreferencesManager::setDefaultValues()
{
  setLang("en");
  setRecentMax(5);
}

// =====================================================================
// =====================================================================


void PreferencesManager::loadKeyFile()
{
  try
  {
    if (!mp_KFile->load_from_file(m_FileName, Glib::KEY_FILE_KEEP_COMMENTS
        | Glib::KEY_FILE_KEEP_TRANSLATIONS))
      std::cerr << "PreferencesManager: Problem loading key file "
          << m_FileName << std::endl;
  } catch (Glib::FileError e)
  {
    std::cerr << "PreferencesManager: FileError with file " << m_FileName
        << " : " << e.what() << std::endl;
  } catch (Glib::KeyFileError e)
  {
    std::cerr << "PreferencesManager: KeyFileError with file " << m_FileName
        << " : " << e.what() << std::endl;
  }
}

// =====================================================================
// =====================================================================

bool PreferencesManager::save()
{
  std::ofstream File(m_FileName.c_str());

  if (File.fail())
  {
    std::cerr << "PreferencesManager: Problem saving file " << m_FileName
        << std::endl;
    return false;
  }

  File << mp_KFile->to_data();

  File.close();

  return true;
}

// =====================================================================
// =====================================================================


std::string PreferencesManager::getFileName()
{
  return m_FileName;
}

// =====================================================================
// =====================================================================

bool PreferencesManager::isValidKey(std::string Group, std::string Key)
{
  return (mp_KFile->has_group(Group) && mp_KFile->has_key(Group, Key));
}
// =====================================================================
// =====================================================================


void PreferencesManager::setLang(std::string Lang)
{
  mp_KFile->set_string("openfluid.builder.interface", "lang", Lang);
}

// =====================================================================
// =====================================================================


std::string PreferencesManager::getLang()
{
  return isValidKey("openfluid.builder.interface", "lang") ? mp_KFile->get_string(
      "openfluid.builder.interface", "lang")
      : "";
}

// =====================================================================
// =====================================================================


void PreferencesManager::setRecentMax(unsigned int RecentMax)
{
  // imposer une limite ?
  mp_KFile->set_integer("openfluid.builder.interface", "recentmax", RecentMax);

  if (mp_KFile->has_group("openfluid.builder.recentprojects"))
  {
    while (mp_KFile->get_keys("openfluid.builder.recentprojects").size()
        > RecentMax)
      mp_KFile->remove_key("openfluid.builder.recentprojects",
          ((std::vector<std::string>) mp_KFile->get_keys(
              "openfluid.builder.recentprojects"))[0]);
  }
}

// =====================================================================
// =====================================================================


int PreferencesManager::getRecentMax()
{
  return isValidKey("openfluid.builder.interface", "recentmax") ? mp_KFile->get_integer(
      "openfluid.builder.interface", "recentmax")
      : -1;
}

// =====================================================================
// =====================================================================

bool PreferencesManager::addRecentProject(std::string ProjectPath,
    std::string ProjectName)
{
  if (ProjectPath.find('=') != std::string::npos)
    return false;

  unsigned int RecentMax = isValidKey("openfluid.builder.interface",
      "recentmax") ? mp_KFile->get_integer("openfluid.builder.interface",
      "recentmax") : 5;

  std::vector<std::string> ProjectPaths;

  if (mp_KFile->has_group("openfluid.builder.recentprojects"))
    ProjectPaths = mp_KFile->get_keys("openfluid.builder.recentprojects");

  if (!ProjectPaths.empty() && ProjectPaths.size() > (RecentMax - 1))
    mp_KFile->remove_key("openfluid.builder.recentprojects", ProjectPaths[0]);

  mp_KFile->set_string("openfluid.builder.recentprojects", ProjectPath,
      ProjectName);

  return true;
}

// =====================================================================
// =====================================================================


void PreferencesManager::clearRecentProjects()
{
  mp_KFile->remove_group("openfluid.builder.recentprojects");
}

// =====================================================================
// =====================================================================

/* get pairs of <ProjectPath,ProjectName>, from older to newer */
std::vector<std::pair<std::string, std::string> > PreferencesManager::getRecentProjects()
{
  std::vector<std::pair<std::string, std::string> > RecentProjects;

  if (mp_KFile->has_group("openfluid.builder.recentprojects"))
  {
    std::vector<std::string> ProjectPaths = mp_KFile->get_keys(
        "openfluid.builder.recentprojects");

    for (unsigned int i = 0; i < ProjectPaths.size(); i++)
    {
      RecentProjects.push_back(std::make_pair(ProjectPaths[i],
          mp_KFile->get_string("openfluid.builder.recentprojects",
              ProjectPaths[i])));
    }
  }

  return RecentProjects;
}

// =====================================================================
// =====================================================================


void PreferencesManager::setWorkdir(std::string Workdir)
{
  mp_KFile->set_string("openfluid.builder.paths", "workdir", Workdir);
}

// =====================================================================
// =====================================================================


std::string PreferencesManager::getWorkdir()
{
  return isValidKey("openfluid.builder.paths", "workdir") ? mp_KFile->get_string(
      "openfluid.builder.paths", "workdir")
      : "";
}

// =====================================================================
// =====================================================================


void PreferencesManager::addExtraPlugPath(std::string Path)
{
  std::vector<std::string> ExtraPlugPaths;

  if (isValidKey("openfluid.builder.paths", "extraplugpaths"))
    ExtraPlugPaths = mp_KFile->get_string_list("openfluid.builder.paths",
        "extraplugpaths");

  ExtraPlugPaths.push_back(Path);

  mp_KFile->set_string_list("openfluid.builder.paths", "extraplugpaths",
      ExtraPlugPaths);
}

// =====================================================================
// =====================================================================


void PreferencesManager::removeExtraPlugPath(std::string Path)
{
  if (isValidKey("openfluid.builder.paths", "extraplugpaths"))
  {
    std::vector<std::string> ExtraPlugPaths = mp_KFile->get_string_list(
        "openfluid.builder.paths", "extraplugpaths");

    std::vector<std::string>::iterator it = std::find(ExtraPlugPaths.begin(),
        ExtraPlugPaths.end(), Path);

    if (it != ExtraPlugPaths.end())
      ExtraPlugPaths.erase(it);

    mp_KFile->set_string_list("openfluid.builder.paths", "extraplugpaths",
        ExtraPlugPaths);
  }
}

// =====================================================================
// =====================================================================


std::vector<std::string> PreferencesManager::getExtraPlugPaths()
{
  std::vector<std::string> ExtraPlugPaths;

  if (isValidKey("openfluid.builder.paths", "extraplugpaths"))
    ExtraPlugPaths = mp_KFile->get_string_list("openfluid.builder.paths",
        "extraplugpaths");

  return ExtraPlugPaths;
}

// =====================================================================
// =====================================================================


void PreferencesManager::setDeltaT(unsigned int DeltaT)
{
  mp_KFile->set_integer("openfluid.builder.runconfig", "deltat", DeltaT);
}

// =====================================================================
// =====================================================================


int PreferencesManager::getDeltaT()
{
  return isValidKey("openfluid.builder.runconfig", "deltat") ? mp_KFile->get_integer(
      "openfluid.builder.runconfig", "deltat")
      : -1;
}

// =====================================================================
// =====================================================================


void PreferencesManager::setBegin(std::string Begin)
{
  mp_KFile->set_string("openfluid.builder.runconfig", "begin", Begin);
}

// =====================================================================
// =====================================================================


std::string PreferencesManager::getBegin()
{
  return isValidKey("openfluid.builder.runconfig", "begin") ? mp_KFile->get_string(
      "openfluid.builder.runconfig", "begin")
      : "";
}

// =====================================================================
// =====================================================================

void PreferencesManager::setEnd(std::string End)
{
  mp_KFile->set_string("openfluid.builder.runconfig", "end", End);
}

// =====================================================================
// =====================================================================


std::string PreferencesManager::getEnd()
{
  return isValidKey("openfluid.builder.runconfig", "end") ? mp_KFile->get_string(
      "openfluid.builder.runconfig", "end")
      : "";
}

// =====================================================================
// =====================================================================


void PreferencesManager::setOutFilesBuffer(unsigned int Buffer)
{
  mp_KFile->set_integer("openfluid.builder.runconfig", "outfilesbuffer", Buffer);
}

// =====================================================================
// =====================================================================


int PreferencesManager::getOutFilesBuffer()
{
  return isValidKey("openfluid.builder.runconfig", "outfilesbuffer") ? mp_KFile->get_integer(
      "openfluid.builder.runconfig", "outfilesbuffer")
      : -1;
}

// =====================================================================
// =====================================================================

bool PreferencesManager::addMarketplace(std::string PlaceName,
    std::string PlaceUrl)
{
  if (PlaceName.find('=') != std::string::npos)
    return false;

  mp_KFile->set_string("openfluid.market.marketplaces", PlaceName, PlaceUrl);
  return true;
}

// =====================================================================
// =====================================================================


void PreferencesManager::removeMarketplace(std::string PlaceName)
{
  if (isValidKey("openfluid.market.marketplaces", PlaceName))
    mp_KFile->remove_key("openfluid.market.marketplaces", PlaceName);
}

// =====================================================================
// =====================================================================


PreferencesManager::MarketPlaces_t PreferencesManager::getMarketplaces()
{
  MarketPlaces_t Places;

  if (mp_KFile->has_group("openfluid.market.marketplaces"))
  {
    std::vector<std::string> PlaceNames = mp_KFile->get_keys(
        "openfluid.market.marketplaces");

    for (unsigned int i = 0; i < PlaceNames.size(); i++)
    {
      Places[PlaceNames[i]] = mp_KFile->get_string(
          "openfluid.market.marketplaces", PlaceNames[i]);
    }
  }

  return Places;
}

// =====================================================================
// =====================================================================


std::string PreferencesManager::getPluginValue(std::string PluginName,
    std::string Key)
{
  std::string GroupName = Glib::ustring::compose(
      "openfluid.builder.plugins.%1", PluginName);

  if (isValidKey(GroupName, Key))
    return mp_KFile->get_string(GroupName, Key);

  return "";
}

// =====================================================================
// =====================================================================


void PreferencesManager::setPluginValue(std::string PluginName,
    std::string Key, std::string Value)
{
  std::string GroupName = Glib::ustring::compose(
      "openfluid.builder.plugins.%1", PluginName);

  mp_KFile->set_string(GroupName, Key, Value);
}


} } //namespaces
