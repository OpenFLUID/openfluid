/*

  This file is part of OpenFLUID software
  Copyright(c) 2007, INRA - Montpellier SupAgro


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
  along with OpenFLUID. If not, see <http://www.gnu.org/licenses/>.


 == Other Usage ==

  Other Usage means a use of OpenFLUID that is inconsistent with the GPL
  license, and requires a written agreement between You and INRA.
  Licensees for Other Usage of OpenFLUID may use this file in accordance
  with the terms contained in the written agreement between You and INRA.
  
*/



/**
  @file ExtensionPluginsManager.cpp

  @author Jean-Christophe FABRE <jean-christophe.fabre@supagro.inra.fr>
 */


#include "ExtensionPluginsManager.hpp"


OPENFLUID_SINGLETON_INITIALIZATION(ExtensionPluginsManager)


ExtensionPluginsManager::ExtensionPluginsManager(const QStringList& ExtraSearchPaths):
  openfluid::machine::WarePluginsManager<ExtensionContainer,ExtensionContainer,
                                         openfluid::builderext::GetPluggableBuilderExtensionSignatureProc,
                                         openfluid::builderext::GetPluggableBuilderExtensionBodyProc>()
{
  for (int i=0; i <ExtraSearchPaths.size();i++)
    m_SearchPaths.push_back(ExtraSearchPaths[i].toStdString());

  m_SearchPaths.push_back(openfluid::base::RuntimeEnvironment::instance()
  ->getUserDataPath(openfluid::config::BUILDEREXTS_WARESBIN_USR_PATH));
  m_SearchPaths.push_back(openfluid::base::RuntimeEnvironment::instance()->getMarketBagBuildVersionDir()+
                          "/"+
                          openfluid::base::RuntimeEnvironment::instance()->getMarketBagBinSubDir());
  m_SearchPaths.push_back(openfluid::base::RuntimeEnvironment::instance()->getInstallPrefix()+
                          "/"+
                          BUILDEREXTS_INSTALL_PATH);

  m_StandardSearchPaths.push_back(openfluid::base::RuntimeEnvironment::instance()
  ->getUserDataPath(openfluid::config::BUILDEREXTS_WARESBIN_USR_PATH));
  m_StandardSearchPaths.push_back(openfluid::base::RuntimeEnvironment::instance()->getMarketBagBuildVersionDir()+
                                  "/"+
                                  openfluid::base::RuntimeEnvironment::instance()->getMarketBagBinSubDir());
  m_StandardSearchPaths.push_back(openfluid::base::RuntimeEnvironment::instance()->getInstallPrefix()+
                                  "/"+
                                  BUILDEREXTS_INSTALL_PATH);
}


// =====================================================================
// =====================================================================


ExtensionPluginsManager::~ExtensionPluginsManager()
{

}


// =====================================================================
// =====================================================================


ExtensionPluginsManager* ExtensionPluginsManager::instance(const QStringList& ExtraSearchPaths)
{
  if (mp_Instance == nullptr)
    mp_Instance = new ExtensionPluginsManager(ExtraSearchPaths);

  return mp_Instance;
}


// =====================================================================
// =====================================================================


std::string ExtensionPluginsManager::getPluginFullPath(const std::string& Filename) const
{
  std::string PlugFullPath = "";

  unsigned int i = 0;

  while ((PlugFullPath.length() == 0) && (i < m_SearchPaths.size()))
  {
    QFileInfo TmpPath(QString(m_SearchPaths[i].c_str()) + "/" + QString(Filename.c_str()));

    if (TmpPath.exists())
      PlugFullPath = TmpPath.absoluteFilePath().toStdString();

    i++;
  }

  return PlugFullPath;
}

