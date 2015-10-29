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
  @file ExtensionPluginsManager.hpp

  @author Jean-Christophe FABRE <jean-christophe.fabre@supagro.inra.fr>
 */


#ifndef __OPENFLUID_BUILDERAPP_EXTENSIONPLUGINSMANAGER_HPP__
#define __OPENFLUID_BUILDERAPP_EXTENSIONPLUGINSMANAGER_HPP__

#include <QFileInfo>
#include <QStringList>

#include <openfluid/base/RuntimeEnv.hpp>
#include <openfluid/machine/WarePluginsManager.hpp>
#include <openfluid/builderext/PluggableBuilderExtension.hpp>
#include <openfluid/utils/KillableSingleton.hpp>

#include "ExtensionContainer.hpp"
#include "builderconfig.hpp"


class ExtensionPluginsManager :
    public openfluid::machine::WarePluginsManager<ExtensionContainer,ExtensionContainer,
                                                  openfluid::builderext::GetPluggableBuilderExtensionSignatureProc,
                                                  openfluid::builderext::GetPluggableBuilderExtensionBodyProc>,
    public openfluid::utils::KillableSingleton<ExtensionPluginsManager>
{

  friend class openfluid::utils::KillableSingleton<ExtensionPluginsManager>;


  private:

    std::vector<std::string> m_SearchPaths;

    std::vector<std::string> m_StandardSearchPaths;


    ExtensionPluginsManager(const QStringList& ExtraSearchPaths = QStringList());

    ~ExtensionPluginsManager();


  public:

    static ExtensionPluginsManager* instance(const QStringList& ExtraSearchPaths = QStringList());

    std::string getPluginFullPath(const std::string& Filename) const;

    std::vector<std::string> getPluginsSearchPaths() const
    { return m_SearchPaths; }

    std::string getPluginFilenameSuffix() const
    { return openfluid::config::BUILDEREXTS_PLUGINS_SUFFIX; }

    std::vector<std::string> getPluginsStandardSearchPaths() const
    { return m_StandardSearchPaths; }
};





#endif /* __OPENFLUID_BUILDERAPP_EXTENSIONPLUGINSMANAGER_HPP__ */
