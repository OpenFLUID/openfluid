/*
  This file is part of OpenFLUID-engine software
  Copyright (c) 2007-2010 INRA-Montpellier SupAgro

 == GNU General Public License Usage ==

  OpenFLUID-engine is free software: you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation, either version 3 of the License, or
  (at your option) any later version.

  OpenFLUID-engine is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with OpenFLUID-engine.  If not, see <http://www.gnu.org/licenses/>.

  In addition, as a special exception, INRA gives You the additional right
  to dynamically link the code of OpenFLUID-engine with code not covered 
  under the GNU General Public License ("Non-GPL Code") and to distribute
  linked combinations including the two, subject to the limitations in this
  paragraph. Non-GPL Code permitted under this exception must only link to
  the code of OpenFLUID-engine dynamically through the ofelib library
  interface, and only for building OpenFLUID-engine plugins. The files of
  Non-GPL Code may be link to the ofelib library without causing the
  resulting work to be covered by the GNU General Public License. You must
  obey the GNU General Public License in all respects for all of the
  OpenFLUID-engine code and other code used in conjunction with
  OpenFLUID-engine except the Non-GPL Code covered by this exception. If
  you modify this OpenFLUID-engine, you may extend this exception to your
  version of the file, but you are not obligated to do so. If you do not
  wish to provide this exception without modification, you must delete this
  exception statement from your version and license this OpenFLUID-engine
  solely under the GPL without exception.


 == Other Usage ==

  Other Usage means a use of OpenFLUID-Engine that is inconsistent with
  the GPL license, and requires a written agreement between You and INRA.
  Licensees for Other Usage of OpenFLUID-engine may use this file in
  accordance with the terms contained in the written agreement between
  You and INRA.
*/


/**
  \file BuilderExtensionsManager.cpp
  \brief Implements ...

  \author Jean-Christophe FABRE <fabrejc@supagro.inra.fr>
 */

#include "BuilderExtensionsManager.hpp"
#include "builderconfig.hpp"

#include <openfluid/debug.hpp>
#include <openfluid/machine.hpp>
#include <openfluid/config.hpp>


BuilderExtensionsManager* BuilderExtensionsManager::mp_Singleton = NULL;


// =====================================================================
// =====================================================================


BuilderExtensionsManager::BuilderExtensionsManager()
  : m_IsRegistrationDone(false), m_RegisteredExtensionsCount(0)
{

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


void BuilderExtensionsManager::prependExtensionsSearchPaths(const std::string& SemicolonSeparatedPaths)
{
  if (m_IsRegistrationDone) return;

  std::vector<std::string> ExtraPaths;
  ExtraPaths = openfluid::tools::SplitString(SemicolonSeparatedPaths,";");


  for (int i = ExtraPaths.size()-1 ; i>=0 ; i--) m_SearchPaths.insert(m_SearchPaths.begin(),1,openfluid::tools::RemoveTrailingSlashes(ExtraPaths[i]));

}


// =====================================================================
// =====================================================================


void BuilderExtensionsManager::registerExtensions()
{

  if (m_IsRegistrationDone || m_SearchPaths.empty()) return;

  std::list<std::string>::iterator PathsIt;

  std::list<std::string> FoundFiles;
  std::vector<std::string> TmpFoundFiles;


  for (PathsIt = m_SearchPaths.begin(); PathsIt != m_SearchPaths.end(); ++PathsIt)
  {
    TmpFoundFiles = openfluid::tools::GetFilesByExt(*PathsIt,BUILDEREXTENSION_BINARY_EXTENSION,true,true);

    FoundFiles.insert(FoundFiles.end(),TmpFoundFiles.begin(),TmpFoundFiles.end());
  }

  for (PathsIt = FoundFiles.begin(); PathsIt != FoundFiles.end(); ++PathsIt)
  {
    openfluid::machine::DynamicLib* ExtLib = new openfluid::machine::DynamicLib(*PathsIt);

    if (ExtLib->load())
    {
      ExtensionContainer ExtContainer;
      ExtContainer.Filename = *PathsIt;

      if (ExtLib->hasSymbol(EXTSDKVERSION_PROC_NAME))
      {
        openfluid::builderext::GetExtensionSDKVersionProc SDKProc;
        SDKProc = (openfluid::builderext::GetExtensionSDKVersionProc)ExtLib->getSymbol(EXTSDKVERSION_PROC_NAME);

        if (openfluid::tools::CompareVersions(openfluid::config::FULL_VERSION,SDKProc(),false) == 0)
        {
          if (ExtLib->hasSymbol(EXTINFOS_PROC_NAME) && ExtLib->hasSymbol(EXTENSION_PROC_NAME))
          {

            openfluid::builderext::GetExtensionInfosProc InfosProc = (openfluid::builderext::GetExtensionInfosProc)ExtLib->getSymbol(EXTINFOS_PROC_NAME);

            if (InfosProc != NULL)
            {
              ExtContainer.Infos = InfosProc();
              std::string TmpID = ExtContainer.Infos.ID;

              if (!TmpID.empty())
              {
                openfluid::builderext::GetExtensionProc ExtProc = (openfluid::builderext::GetExtensionProc)ExtLib->getSymbol(EXTENSION_PROC_NAME);

                if (ExtProc != NULL)
                {
                  ExtContainer.Extension = ExtProc();
                  m_RegisteredExtensions[ExtContainer.Extension->getType()][TmpID] = ExtContainer;
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


void BuilderExtensionsManager::linkRegisteredExtensionsAndSimulationBlob(openfluid::machine::SimulationBlob* Blob)
{
  CollectionOfExtensions_t::iterator COEit;
  ExtensionContainerMap_t::iterator ECMit;

  for (COEit = m_RegisteredExtensions.begin(); COEit!= m_RegisteredExtensions.end(); ++COEit)
  {
    for (ECMit = (*COEit).second.begin(); ECMit!= (*COEit).second.end(); ++ECMit)
    {
      (*ECMit).second.Extension->setSimulationBlob(Blob);
    }
  }

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
    case openfluid::builderext::PluggableBuilderExtension::InputdataImporter : return "InputdataImporter"; break;
    case openfluid::builderext::PluggableBuilderExtension::EventsImporter : return "EventsImporter"; break;
    case openfluid::builderext::PluggableBuilderExtension::ExtraImporter : return "ExtraImporter"; break;
    case openfluid::builderext::PluggableBuilderExtension::MixedImporter : return "MixedImporter"; break;
    case openfluid::builderext::PluggableBuilderExtension::SimulationListener : return "SimulationListener"; break;
    case openfluid::builderext::PluggableBuilderExtension::HomeLauncher : return "HomeLauncher"; break;

    default : return "";
  }
  return "";
}



