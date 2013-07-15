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
  \file BuilderExtensionsManager.cpp
  \brief Implements ...

  \author Jean-Christophe FABRE <fabrejc@supagro.inra.fr>
 */

#include "BuilderExtensionsManager.hpp"

#include <glibmm/module.h>

#include <openfluid/config.hpp>
#include <openfluid/tools/SwissTools.hpp>
#include <openfluid/base/RuntimeEnv.hpp>
#include <openfluid/base/ApplicationException.hpp>

#include "builderconfig.hpp"


// =====================================================================
// =====================================================================


ExtensionContainer::ExtensionContainer() : ExtProc(0), PrefsProc(0), Extension(0), Preferences(0)
{}

// =====================================================================
// =====================================================================

void ExtensionContainer::setExtProcFunction(openfluid::builderext::GetExtensionProc TheExtProc)
{
  ExtProc = TheExtProc;
};

// =====================================================================
// =====================================================================

void ExtensionContainer::setPrefsProcFunction(openfluid::builderext::GetExtensionPrefsProc ThePrefsProc)
{
  PrefsProc = ThePrefsProc;
}

// =====================================================================
// =====================================================================


bool ExtensionContainer::instantiateExt()
{
  if(!Extension && ExtProc)
  {
    Extension = ExtProc();

    openfluid::builderext::PluggableBuilderExtension::ExtensionType ExtensionType = Extension->getType();

    if(ExtensionType == Infos.Type)
      return true;


    Extension = 0;

    throw openfluid::base::ApplicationException("openfluid-builder","ExtensionContainer::instantiate ",
        Infos.ID + ": Wrong declared extension type ("
        + BuilderExtensionsManager::getExtensionTypeAsString(Infos.Type)
    + " doesn't match Pluggable extension class type "
    + BuilderExtensionsManager::getExtensionTypeAsString(ExtensionType) + ")");
  }

  return false;
};


// =====================================================================
// =====================================================================


void ExtensionContainer::deleteExt()
{
  delete Extension;
  Extension = 0;
}

// =====================================================================
// =====================================================================

bool ExtensionContainer::instantiatePrefs()
{
  if(!Preferences && PrefsProc)
    Preferences = PrefsProc();

  return Preferences;
}

// =====================================================================
// =====================================================================

void ExtensionContainer::deletePrefs()
{
  delete Preferences;
  Preferences = 0;
}

// =====================================================================
// =====================================================================

// =====================================================================
// =====================================================================


BuilderExtensionsManager* BuilderExtensionsManager::mp_Singleton = NULL;


// =====================================================================
// =====================================================================


BuilderExtensionsManager::BuilderExtensionsManager()
  : m_IsRegistrationDone(false), m_RegisteredExtensionsCount(0), m_IsPreferencesInstantiationDone(false)
{
  openfluid::base::RuntimeEnvironment* RunEnv = openfluid::base::RuntimeEnvironment::getInstance();

  m_DefaultSearchPaths.push_back(RunEnv->getUserDataPath(
          BUILDER_EXTSDIR));

  m_DefaultSearchPaths.push_back(Glib::ustring::compose("%1/%2",
          RunEnv->getInstallPrefix(), BUILDEREXT_INSTALL_PATH));
}


// =====================================================================
// =====================================================================


BuilderExtensionsManager* BuilderExtensionsManager::getInstance()
{
  if (mp_Singleton == NULL) mp_Singleton = new BuilderExtensionsManager();
  return mp_Singleton;
}


// =====================================================================
// =====================================================================


void BuilderExtensionsManager::prependExtensionSearchPath(const std::string& Path)
{
  if (m_IsRegistrationDone)
    return;

  m_ExtraSearchPaths.insert(m_ExtraSearchPaths.begin(),1,openfluid::tools::RemoveTrailingSlashes(Path));
}


// =====================================================================
// =====================================================================


void BuilderExtensionsManager::prependExtensionsSearchPaths(const std::string& SemicolonSeparatedPaths)
{
  if (m_IsRegistrationDone)
    return;

  std::vector<std::string> ExtraPaths;
  ExtraPaths = openfluid::tools::SplitString(SemicolonSeparatedPaths,";");

  for (int i = ExtraPaths.size()-1 ; i>=0 ; i--)
    prependExtensionSearchPath(ExtraPaths[i]);
}


// =====================================================================
// =====================================================================


std::list<std::string> BuilderExtensionsManager::getExtensionsSearchPaths() const
{
  std::list<std::string> ComposedPaths(m_ExtraSearchPaths.begin(),m_ExtraSearchPaths.end());
  ComposedPaths.insert(ComposedPaths.end(), m_DefaultSearchPaths.begin(), m_DefaultSearchPaths.end());
  return ComposedPaths;
}


// =====================================================================
// =====================================================================


void BuilderExtensionsManager::registerExtensions()
{

  std::list<std::string> SearchPaths = getExtensionsSearchPaths();

  if (m_IsRegistrationDone || SearchPaths.empty()) return;

  std::list<std::string>::iterator PathsIt;

  std::list<std::string> FoundFiles;
  std::vector<std::string> TmpFoundFiles;


  for (PathsIt = SearchPaths.begin(); PathsIt != SearchPaths.end(); ++PathsIt)
  {
    TmpFoundFiles = openfluid::tools::GetFilesByExt(*PathsIt,BUILDEREXTENSION_BINARY_EXTENSION,true,true);

    FoundFiles.insert(FoundFiles.end(),TmpFoundFiles.begin(),TmpFoundFiles.end());
  }

  for (PathsIt = FoundFiles.begin(); PathsIt != FoundFiles.end(); ++PathsIt)
  {
    Glib::Module* ExtLib = new Glib::Module(*PathsIt,Glib::MODULE_BIND_LOCAL);

    if(*ExtLib)
    {
      ExtensionContainer ExtContainer;
      ExtContainer.Filename = *PathsIt;

      void* SDKVersionSymbol = 0;

      if(ExtLib->get_symbol(EXTSDKVERSION_PROC_NAME,SDKVersionSymbol))
      {
        openfluid::builderext::GetExtensionSDKVersionProc SDKProc;

        SDKProc = (openfluid::builderext::GetExtensionSDKVersionProc)SDKVersionSymbol;

        if (openfluid::tools::CompareVersions(openfluid::config::FULL_VERSION,SDKProc(),false) == 0)
        {
          void* InfosSymbol = 0;
          void* ExtensionSymbol = 0;

          if(ExtLib->get_symbol(EXTINFOS_PROC_NAME,InfosSymbol)
              && ExtLib->get_symbol(EXTENSION_PROC_NAME,ExtensionSymbol))
          {

            openfluid::builderext::GetExtensionInfosProc InfosProc = (openfluid::builderext::GetExtensionInfosProc)InfosSymbol;

            if (InfosProc != NULL)
            {
              ExtContainer.Infos = InfosProc();
              std::string TmpID = ExtContainer.Infos.ID;

              if (!TmpID.empty())
              {
                openfluid::builderext::GetExtensionProc ExtProc = (openfluid::builderext::GetExtensionProc)ExtensionSymbol;

                if (ExtProc != NULL)
                {
                  ExtContainer.setExtProcFunction(ExtProc);

                  void* PrefsSymbol = 0;

                  if(ExtLib->get_symbol(EXTPREFS_PROC_NAME,PrefsSymbol))
                  {
                    openfluid::builderext::GetExtensionPrefsProc PrefsProc = (openfluid::builderext::GetExtensionPrefsProc)PrefsSymbol;

                    if(PrefsProc != NULL)
                      ExtContainer.setPrefsProcFunction(PrefsProc);
                  }

                  m_RegisteredExtensions[ExtContainer.Infos.Type][TmpID] = ExtContainer;
                  m_RegisteredExtensionsCount++;
                }
              }
            }
          }
        }
      }
    }
  }
  m_IsRegistrationDone = true;
}


// =====================================================================
// =====================================================================


ExtensionContainerMap_t* BuilderExtensionsManager::getRegisteredExtensions(openfluid::builderext::PluggableBuilderExtension::ExtensionType Type)
{
  if (m_RegisteredExtensions.find(Type) != m_RegisteredExtensions.end())
    return &m_RegisteredExtensions[Type];

  return NULL;
}


// =====================================================================
// =====================================================================


ExtensionContainer* BuilderExtensionsManager::getExtensionContainer(openfluid::builderext::PluggableBuilderExtension::ExtensionType Type,
                                                                    std::string ExtID)
{

  ExtensionContainerMap_t* ThisTypeExts = getRegisteredExtensions(Type);

  if (ThisTypeExts != NULL)
  {
    ExtensionContainerMap_t::iterator FoundIt = ThisTypeExts->find(ExtID);

    if (FoundIt != ThisTypeExts->end())
      return &((*FoundIt).second);
  }

  return NULL;
}


// =====================================================================
// =====================================================================


ExtensionContainer* BuilderExtensionsManager::getExtensionContainer(const std::string& ExtID)
{
  for (CollectionOfExtensions_t::const_iterator COEit = m_RegisteredExtensions.begin(); COEit
  != m_RegisteredExtensions.end(); ++COEit)
  {
    ExtensionContainer* ExtCont = getExtensionContainer(COEit->first,ExtID);

    if(ExtCont != NULL)
      return ExtCont;
  }

  return (ExtensionContainer*)0;
}


// =====================================================================
// =====================================================================


std::string BuilderExtensionsManager::getExtensionTypeAsString(openfluid::builderext::PluggableBuilderExtension::ExtensionType Type)
{
  switch(Type)
  {
    case openfluid::builderext::PluggableBuilderExtension::WorkspaceTab : return "WorkspaceTab"; break;
    case openfluid::builderext::PluggableBuilderExtension::ModelessWindow : return "ModelessWindow"; break;
    case openfluid::builderext::PluggableBuilderExtension::ModalWindow : return "ModalWindow"; break;
    case openfluid::builderext::PluggableBuilderExtension::SpatialgraphImporter : return "SpatialgraphImporter"; break;
    case openfluid::builderext::PluggableBuilderExtension::AttributesImporter : return "AttributesImporter"; break;
    case openfluid::builderext::PluggableBuilderExtension::EventsImporter : return "EventsImporter"; break;
    case openfluid::builderext::PluggableBuilderExtension::ExtraImporter : return "ExtraImporter"; break;
    case openfluid::builderext::PluggableBuilderExtension::MixedImporter : return "MixedImporter"; break;
    case openfluid::builderext::PluggableBuilderExtension::HomeLauncher : return "HomeLauncher"; break;

    default : return "";
  }
  return "";
}


// =====================================================================
// =====================================================================


bool BuilderExtensionsManager::instantiatePluggableExtension(std::string ExtID)
{
  ExtensionContainer* ExtCont = getExtensionContainer(ExtID);

  if(ExtCont)
    return ExtCont->instantiateExt();

  return false;
}


// =====================================================================
// =====================================================================


void BuilderExtensionsManager::deletePluggableExtension(std::string ExtID)
{
  ExtensionContainer* ExtCont = getExtensionContainer(ExtID);

  if(ExtCont)
    ExtCont->deleteExt();
}

// =====================================================================
// =====================================================================

void BuilderExtensionsManager::instantiateRegisteredExtensionPreferences()
{
  if(m_IsPreferencesInstantiationDone)
    return;

  CollectionOfExtensions_t::iterator COEit;
  ExtensionContainerMap_t::iterator ECMit;

  for (COEit = m_RegisteredExtensions.begin(); COEit!= m_RegisteredExtensions.end(); ++COEit)
  {
    for (ECMit = COEit->second.begin(); ECMit!= COEit->second.end(); ++ECMit)
    {
      if(ECMit->second.instantiatePrefs())
        m_RegisteredExtensionPreferences[ECMit->second.Infos.ID] = ECMit->second;
    }
  }

  m_IsPreferencesInstantiationDone = true;
}


// =====================================================================
// =====================================================================


void BuilderExtensionsManager::deleteRegisteredExtensionPreferences()
{
  if(!m_IsPreferencesInstantiationDone)
    return;

  for (ExtensionContainerMap_t::iterator it = m_RegisteredExtensionPreferences.begin(); it!= m_RegisteredExtensionPreferences.end(); ++it)
    it->second.deletePrefs();

  m_RegisteredExtensionPreferences.clear();

  m_IsPreferencesInstantiationDone = false;
}
