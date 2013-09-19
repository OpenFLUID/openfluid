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
  \file ExtensionPluginsManager.hpp
  \brief Header of ...

  \author Jean-Christophe FABRE <fabrejc@supagro.inra.fr>
 */


#ifndef __EXTENSIONPLUGINSMANAGER_HPP__
#define __EXTENSIONPLUGINSMANAGER_HPP__

#include <QFileInfo>
#include <QStringList>

#include <openfluid/base/RuntimeEnv.hpp>
#include <openfluid/machine/WarePluginsManager.hpp>
#include <openfluid/builderext/PluggableBuilderExtension.hpp>

#include "ExtensionContainer.hpp"
#include "builderconfig.hpp"


class ExtensionPluginsManager : public openfluid::machine::WarePluginsManager<ExtensionContainer,ExtensionContainer,
                                                                              openfluid::builderext::GetPluggableBuilderExtensionSignatureProc,
                                                                              openfluid::builderext::GetPluggableBuilderExtensionBodyProc>
{

  private:

    static ExtensionPluginsManager* mp_Singleton;

    std::vector<std::string> m_SearchPaths;

    std::vector<std::string> m_StandardSearchPaths;


    ExtensionPluginsManager(const QStringList& ExtraSearchPaths):
      WarePluginsManager()
    {
      for (int i=0; i <ExtraSearchPaths.size();i++)
        m_SearchPaths.push_back(ExtraSearchPaths[i].toStdString());

      m_SearchPaths.push_back(openfluid::base::RuntimeEnvironment::getInstance()->getUserDataPath(BUILDER_EXTSDIR.toStdString()));
      m_SearchPaths.push_back(openfluid::base::RuntimeEnvironment::getInstance()->getInstallPrefix()+
                              "/"+
                              BUILDEREXT_INSTALL_PATH.toStdString());

      m_StandardSearchPaths.push_back(openfluid::base::RuntimeEnvironment::getInstance()->getUserDataPath(BUILDER_EXTSDIR.toStdString()));
      m_StandardSearchPaths.push_back(openfluid::base::RuntimeEnvironment::getInstance()->getInstallPrefix()+
                                      "/"+
                                      BUILDEREXT_INSTALL_PATH.toStdString());
    };


  public:

    static ExtensionPluginsManager* getInstance(const QStringList& ExtraSearchPaths)
    {
      if (mp_Singleton == NULL)
        mp_Singleton = new ExtensionPluginsManager(ExtraSearchPaths);

      return mp_Singleton;
    }


    // =====================================================================
    // =====================================================================


    static ExtensionPluginsManager* getInstance()
    {
      if (mp_Singleton == NULL)
        mp_Singleton = new ExtensionPluginsManager(QStringList());

      return mp_Singleton;
    }

    // =====================================================================
    // =====================================================================


    std::string getPluginFullPath(const std::string& Filename)
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


    // =====================================================================
    // =====================================================================


    std::vector<std::string> getPluginsSearchPaths()
    {
      return m_SearchPaths;
    }


    // =====================================================================
    // =====================================================================


    std::string getPluginFilenameSuffix()
    {
      return BUILDEREXTS_PLUGINS_SUFFIX;
    }


    // =====================================================================
    // =====================================================================


    std::vector<std::string> getPluginsStandardSearchPaths()
    {
      return m_StandardSearchPaths;
    }
};





#endif /* __EXTENSIONPLUGINSMANAGER_HPP__ */
