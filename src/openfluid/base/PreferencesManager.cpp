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
  @file PreferencesManager.cpp

  @author Jean-Christophe Fabre <jean-christophe.fabre@inra.fr>
  @author Aline LIBRES <libres@supagro.inra.fr>
*/


// OpenFLUID:stylecheck:!incs
// OpenFLUID:stylecheck:!inco


#include <algorithm>
#include <regex>
#include <locale>

#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/ini_parser.hpp>

#include <openfluid/base/Environment.hpp>
#include <openfluid/base/FrameworkException.hpp>
#include <openfluid/tools/Filesystem.hpp>
#include <openfluid/tools/FilesystemPath.hpp>
#include <openfluid/tools/DataHelpers.hpp>
#include <openfluid/tools/MiscHelpers.hpp>
#include <openfluid/base/PreferencesManager.hpp>
#include <openfluid/global.hpp>
#include <openfluid/config.hpp>

#if defined(OPENFLUID_OS_WINDOWS)
#include <windows.h>
#include <winnls.h>
#endif


namespace openfluid { namespace base {


OPENFLUID_SINGLETON_INITIALIZATION(PreferencesManager)


std::string PreferencesManager::m_SettingsFile = "";
const std::string PreferencesManager::m_SettingsRole = "openfluid-settings";


// =====================================================================
// =====================================================================


PreferencesManager::PreferencesManager()
{
  openfluid::base::Environment::init();

  if (m_SettingsFile.empty())
  {
    m_SettingsFile = openfluid::base::Environment::getSettingsFile();
  }

  updateSettingsFile(m_SettingsFile);

  loadSettings();
}


// =====================================================================
// =====================================================================


void PreferencesManager::updateSettingsFile(const std::string& FilePath) const
{
#if (OPENFLUID_VERSION_NUMERIC >= OPENFLUID_VERSION_COMPUTE(2,3,0))
# warning "the project file format compatibility is deprecated and should be removed"
#endif


  if (!openfluid::tools::FilesystemPath(FilePath).isFile())
  {
    std::string FormerFilePath = 
      openfluid::tools::Filesystem::joinPath({
                                               openfluid::tools::FilesystemPath(FilePath).dirname(),
                                               "openfluid.conf"
                                             });

    if (openfluid::tools::FilesystemPath(FormerFilePath).isFile())
    {
      auto cleanKeyValue = [](const std::string& Key, const std::string& Value)
      {
         std::string UnquotedVal = Value;
         if (Value.front() == '"' && Value.back() == '"')
         {
           UnquotedVal = Value.substr(1,UnquotedVal.length()-2);
         }

         return std::make_pair(
           openfluid::tools::trim(Key),
           openfluid::tools::trim(UnquotedVal)
         );
      };

      boost::property_tree::ptree INI;
      boost::property_tree::ini_parser::read_ini(FormerFilePath,INI);
      openfluid::tools::SettingsBackend SB(FilePath,m_SettingsRole,false);

      struct ExtToolInfo
      {
        std::string WorkspaceCmd;
        std::string WareCmd;
        std::string FileCmd;
      };

      std::vector<std::string> ExtToolsOrder;
      std::map<std::string, ExtToolInfo> ExtToolsData;

      for(const auto& Section : INI)
      {
        if (Section.first == "openfluid.builder.paths")
        {
          for(const auto& Entry : Section.second)
          {
            auto [Key,Value] = cleanKeyValue(Entry.first,Entry.second.get_value<std::string>());
            std::string LowerKey = openfluid::tools::toLowerCase(Key);

            std::vector<std::string> PathsArray;
            auto Paths = openfluid::tools::splitString(Value,",");

            for (const auto& P : Paths)
            {
              auto TrimP = openfluid::tools::trim(P);
              if (!TrimP.empty())
              {
                PathsArray.push_back(TrimP);
              }
            }

            if (LowerKey == "workspace")
            {
              SB.setValue("/","workspaces",PathsArray);
            }
            else if (LowerKey == "extrasimpaths")
            {
              SB.setValue("/builder/paths","simulators",PathsArray);
            }
            else if (LowerKey == "extraobspaths")
            {
              SB.setValue("/builder/paths","observers",PathsArray);
            }
            else if (LowerKey == "extraextpaths")
            {
              SB.setValue("/builder/paths","builderexts",PathsArray);
            }
          }
        }
        else if (Section.first == "openfluid.builder.interface")
        {
          for(const auto& Entry : Section.second)
          {
            auto [Key,Value] = cleanKeyValue(Entry.first,Entry.second.get_value<std::string>());
            std::string LowerKey = openfluid::tools::toLowerCase(Key);

            if (LowerKey == "lang")
            {
              SB.setValue("/ui","lang",Value);
            }
            else if (LowerKey == "dockpos")
            {
              int Pos = 1;  // set default to left area
              try
              {
                Pos = std::stoi(Value);
              }
              catch (std::invalid_argument&)
              { }
              SB.setValue("/builder/ui/areas","dock",Pos);
            }
            else if (LowerKey == "toolbarpos")
            {
              int Pos = 4; // set default to top area
              try
              {
                Pos = std::stoi(Value);
              }
              catch (std::invalid_argument&)
              { }
              SB.setValue("/builder/ui/areas","toolbar",Pos);
            }
            else if (LowerKey == "itemremovalconfirm")
            {
              SB.setValue("/builder/ui/confirmations","ware_removal",Value == "true");
            }
            else if (LowerKey == "paramremovalconfirm")
            {
              SB.setValue("/builder/ui/confirmations","param_removal",Value == "true");
            }
            else if (LowerKey == "spatialunitsremovalconfirm")
            {
              SB.setValue("/builder/ui/confirmations","spatialunit_removal",Value == "true");
            }
            else if (LowerKey == "spatialattrsremovalconfirm")
            {
              SB.setValue("/builder/ui/confirmations","spatialattr_removal",Value == "true");
            }
            else if (LowerKey == "spatialconnsremovalconfirm")
            {
              SB.setValue("/builder/ui/confirmations","spatialconn_removal",Value == "true");
            }
            else if (LowerKey == "wareswatchers")
            {
              SB.setValue("/builder/ui","wareswatchers",Value == "true");
            }
            else if (LowerKey == "savebeforerun")
            {
              SB.setValue("/builder/ui/autosave","beforerun",Value == "true");
            }
          }
        }
        else if (Section.first == "openfluid.builder.runconfig")
        {
          for(const auto& Entry : Section.second)
          {
            auto [Key,Value] = cleanKeyValue(Entry.first,Entry.second.get_value<std::string>());
            std::string LowerKey = openfluid::tools::toLowerCase(Key);

            if (LowerKey == "begin" || LowerKey == "end")
            {
              SB.setValue("/builder/runconfig",LowerKey+"_date",Value);
            }
            else if (LowerKey == "deltat")
            {
              int Pos = 3600;  // set default to 3600 seconds
              try
              {
                Pos = std::stoi(Value);
              }
              catch (std::invalid_argument&)
              { }
              SB.setValue("/builder/runconfig","deltat",Pos);
            }
          }
        }
        else if (Section.first == "openfluid.waresdev.commands")
        {          
          for(const auto& Entry : Section.second)
          {
            auto [Key,Value] = cleanKeyValue(Entry.first,Entry.second.get_value<std::string>());
            std::string LowerKey = openfluid::tools::toLowerCase(Key);

            if (LowerKey == "config\\generator")
            {
              SB.setValue("/waresdev/commands/configure","generator",Value);
            }
            else if (LowerKey == "config\\options")
            {
              SB.setValue("/waresdev/commands/configure","options",Value);
            }
            else if (LowerKey == "config\\env\\path")
            {
              SB.setValue("/waresdev/commands/configure/env","PATH",Value);
            }
            else if (LowerKey == "build\\env\\path")
            {
              SB.setValue("/waresdev/commands/build/env","PATH",Value);
            }
            else if (LowerKey == "git_sslnoverify")
            {
              SB.setValue("/waresdev/commands/git","ssl_noverify",Value == "true");
            }
            else if (LowerKey == "showenv\\path")
            {
              SB.setValue("/waresdev/commands/common/show_env","PATH",Value == "true");
            }
          }
        }
        else if (Section.first == "openfluid.waresdev.externaltools.commands")
        {
          for(const auto& Entry : Section.second)
          {
            auto [Key,Value] = cleanKeyValue(Entry.first,Entry.second.get_value<std::string>());
            openfluid::tools::stringReplace(Key,"%20"," ");

            auto Commands = openfluid::tools::splitString(Value,",");

            if (Commands.size() == 3)
            {
              ExtToolsData[Key] = { 
                openfluid::tools::trim(Commands[0]), 
                openfluid::tools::trim(Commands[1]), 
                openfluid::tools::trim(Commands[2])
              };
            }
          }
        }
        else if (Section.first == "openfluid.waresdev.externaltools.order")
        {
          for(const auto& Entry : Section.second)
          {
            auto [Key,Value] = cleanKeyValue(Entry.first,Entry.second.get_value<std::string>());

            if (openfluid::tools::toLowerCase(Key) == "list")
            {
              auto Order = openfluid::tools::splitString(Value,",");

              for (auto O: Order)
              {
                openfluid::tools::stringReplace(O,"%20"," ");
                ExtToolsOrder.push_back(openfluid::tools::trim(O));
              }
            }
          }
        }
        else if (Section.first == "openfluid.waresdev.import")
        {
          for(const auto& Entry : Section.second)
          {
            auto [Key,Value] = cleanKeyValue(Entry.first,Entry.second.get_value<std::string>());
            std::string LowerKey = openfluid::tools::toLowerCase(Key);

            if (LowerKey == "last_wareshub_url")
            {
              SB.setValue("/waresdev/ui/import/hub","url",Value);
            }
            else if (LowerKey == "last_wareshub_username")
            {
              SB.setValue("/waresdev/ui/import/hub","username",Value);
            }
          }
        }
        else if (Section.first == "openfluid.waresdev.interface")
        {
          for(const auto& Entry : Section.second)
          {
            auto [Key,Value] = cleanKeyValue(Entry.first,Entry.second.get_value<std::string>());
            std::string LowerKey = openfluid::tools::toLowerCase(Key);

            if (LowerKey == "savebeforebuild")
            {
              SB.setValue("/waresdev/ui/autosave","beforebuild",Value == "true");
            }
          }
        }
        else if (Section.first == "openfluid.waresdev.texteditor")
        {
          std::regex SyntaxHLRegex("syntax_highlighting\\\\(.*)\\\\(.*)");
          std::smatch SHLMatch;

          std::map<std::string,std::pair<std::string,std::vector<std::string>>> SHLData;

          for(const auto& Entry : Section.second)
          {
            auto [Key,Value] = cleanKeyValue(Entry.first,Entry.second.get_value<std::string>());
            std::string LowerKey = openfluid::tools::toLowerCase(Key);

            if (LowerKey == "currentline_highlighting\\color")
            {
              SB.setValue("/waresdev/ui/texteditor/highlighting/currentline","color",
                          openfluid::tools::toLowerCase(Value));
            }
            else if (LowerKey == "currentline_highlighting\\enabled")
            {
              SB.setValue("/waresdev/ui/texteditor/highlighting/currentline","enabled",Value == "true");
            }
            else if (LowerKey == "linewrapping\\enabled")
            {
              SB.setValue("/waresdev/ui/texteditor","linewrap",Value == "true");
            }
            else if (LowerKey == "indent\\spacenumber")
            {
              int Indent = 2;  // set default to 2 spaces
              try
              {
                Indent = std::stoi(Value);
              }
              catch (std::invalid_argument&)
              { }
              SB.setValue("/waresdev/ui/texteditor","indentation",Indent);
            }
            else if (LowerKey == "show_invisible\\enabled")
            {
              SB.setValue("/waresdev/ui/texteditor/invisible","show_spacetab",Value == "true");
            }
            else if (LowerKey == "show_carriage_return\\enabled")
            {
              SB.setValue("/waresdev/ui/texteditor/invisible","show_crlf",Value == "true");
            }
            else if (LowerKey == "fontname")
            {
              SB.setValue("/waresdev/ui/texteditor/font","name",Value);
            }
            else if (LowerKey == "syntax_highlighting\\enabled")
            {
              SB.setValue("/waresdev/ui/texteditor/highlighting/syntax","enabled",Value == "true");
            }
            else if (std::regex_match(Key, SHLMatch, SyntaxHLRegex))
            {
              if (SHLMatch.size() == 3)
              {
                std::string SHLCat = openfluid::tools::trim(SHLMatch.str(1));
                std::string SHLInfo = openfluid::tools::trim(SHLMatch.str(2));

                if (!SHLCat.empty())
                {

                  if (SHLData.find(SHLCat) == SHLData.end())
                  {
                    SHLData[SHLCat] = {"",std::vector<std::string>()};
                  }

                  if (SHLInfo == "color")
                  {
                    SHLData[SHLCat] = {Value,SHLData[SHLCat].second};
                  }
                  if (SHLInfo == "decoration")
                  {
                    auto ExistingDeco = openfluid::tools::splitString(Value,",");
                    std::vector<std::string> Deco;

                    for (const auto& D : ExistingDeco)
                    {
                      auto CleanedD = openfluid::tools::trim(D);

                      if(!CleanedD.empty())
                      { 
                        Deco.push_back(CleanedD);
                      }
                    }

                    SHLData[SHLCat] = {SHLData[SHLCat].first,Deco};
                  }
                }
              }
            }
          }

          int Index = 0;
          for (const auto& SHL : SHLData)
          {
            std::string IndexedRulePointer = "/waresdev/ui/texteditor/highlighting/syntax/rules/"+std::to_string(Index);
            SB.setValue(IndexedRulePointer,"name",SHL.first);
            SB.setValue(IndexedRulePointer,"color",openfluid::tools::toLowerCase(SHL.second.first));
            SB.setValue(IndexedRulePointer,"decoration",SHL.second.second);
            Index++;
          }
        }
      }

      {
        int Index = 0;

        for (const auto& ToolName : ExtToolsOrder)
        {
          auto it = ExtToolsData.find(ToolName);

          if (it != ExtToolsData.end())
          {

            std::string IndexedToolPointer = "/waresdev/ui/externaltools/"+std::to_string(Index);
            SB.setValue(IndexedToolPointer,"name",(*it).first);
            SB.setValue(IndexedToolPointer,"workspace_command",(*it).second.WorkspaceCmd);
            SB.setValue(IndexedToolPointer,"ware_command",(*it).second.WareCmd);
            SB.setValue(IndexedToolPointer,"file_command",(*it).second.FileCmd);
            ExtToolsData.erase(it);
            Index++;
          }
        }

        // remaining tools not present in order list
        for (const auto& Info : ExtToolsData)
        {
          std::string IndexedToolPointer = "/waresdev/ui/externaltools/"+std::to_string(Index);
          SB.setValue(IndexedToolPointer,"name",Info.first);
          SB.setValue(IndexedToolPointer,"workspace_command",Info.second.WorkspaceCmd);
          SB.setValue(IndexedToolPointer,"ware_command",Info.second.WareCmd);
          SB.setValue(IndexedToolPointer,"file_command",Info.second.FileCmd);
          Index++;
        }        
      }


      SB.save();
      
    }
  }
}


// =====================================================================
// =====================================================================


void PreferencesManager::loadSettings()
{
  auto SettingsF = openfluid::tools::FilesystemPath(m_SettingsFile);
  auto SettingsD = openfluid::tools::FilesystemPath(SettingsF.dirname());

  if (!SettingsD.exists() && !SettingsD.makeDirectory())
  {
    throw openfluid::base::FrameworkException(OPENFLUID_CODE_LOCATION,
                                              "Cannot create directory for settings file");
  }

  m_Settings = std::make_unique<openfluid::tools::SettingsBackend>(m_SettingsFile,m_SettingsRole);

  if (!SettingsF.isFile())
  {
    m_Settings->save();
  }

  setWaresdevTextEditorDefaults(false);
}


// =====================================================================
// =====================================================================


void PreferencesManager::setDefaultValues()
{
  setBuilderDeltaT(300);
}


// =====================================================================
// =====================================================================


std::string PreferencesManager::getSettingsFile() const
{
  return m_SettingsFile;
}


// =====================================================================
// =====================================================================


void PreferencesManager::setSettingsFile(const std::string& FilePath)
{
  if (!mp_Instance)
  {
    m_SettingsFile = FilePath;
  }
  else
  {
    throw openfluid::base::FrameworkException(OPENFLUID_CODE_LOCATION,
                                              "FileName can not be changed after PreferencesManager instantiation");
  }
}


// =====================================================================
// =====================================================================


void PreferencesManager::setUILanguage(const std::string& Lang)
{
  m_Settings->setValue("/ui","lang",Lang);
}


// =====================================================================
// =====================================================================


std::string PreferencesManager::getUILanguage() const
{
  return m_Settings->getValue("/ui/lang").get<std::string>(guessLanguage());
}


// =====================================================================
// =====================================================================


std::vector<std::string> PreferencesManager::getAvailableUILanguages()
{
  std::vector<std::string> Langs;
  std::vector<std::string> QMLangFiles = 
    openfluid::tools::Filesystem::findFilesByExtension(openfluid::base::Environment::getTranslationsDir(),"qm");

  for (const auto& F: QMLangFiles)
  {
    auto LangFile = openfluid::tools::FilesystemPath(F).basename();
    if (LangFile.size() > 5)
    {
      Langs.push_back(LangFile.substr(LangFile.size()-5));
    }
  }  

  return Langs;
}


// =====================================================================
// =====================================================================


bool PreferencesManager::isUILanguageAvailable(const std::string& Lang)
{
  auto AvailableLangs = getAvailableUILanguages();

  return (std::find(AvailableLangs.begin(),AvailableLangs.end(),Lang) != AvailableLangs.end());
}


// =====================================================================
// =====================================================================


std::string PreferencesManager::guessLanguage()
{
#if defined(OPENFLUID_OS_UNIX)
  std::regex LCRegex("([a-z]{2}_[A-Z]{2}).*");
  std::smatch LCMatch;

  std::string RawLocaleStr = std::locale("").name();

  if (std::regex_match(RawLocaleStr,LCMatch,LCRegex) && (LCMatch.size() == 2) &&
      isUILanguageAvailable(LCMatch.str(1)))
  {
    return LCMatch.str(1);
  }
#elif defined(OPENFLUID_OS_WINDOWS)
  // see https://docs.microsoft.com/en-us/windows/win32/api/winnls/nf-winnls-getuserdefaultlocalename
  wchar_t WBuffer[LOCALE_NAME_MAX_LENGTH];
  
  int Ret = GetUserDefaultLocaleName(WBuffer,_countof(WBuffer));
  
  if (Ret>=5)
  {
    char Buffer[LOCALE_NAME_MAX_LENGTH];
    char Default = ' ';
    WideCharToMultiByte(CP_ACP,0,WBuffer,-1,Buffer,260,&Default,NULL);
    
    std::string LocaleStr = std::string(Buffer);
    LocaleStr[2] = '_' ;
    
    if (LocaleStr.size() == 5 && isUILanguageAvailable(LocaleStr))
    {
      return LocaleStr;
    }
  }
#endif

  return "default";
}


// =====================================================================
// =====================================================================


void PreferencesManager::setWorkspacesPaths(const std::vector<std::string>& Paths)
{
  m_Settings->setValue("/","workspaces",Paths);
}


// =====================================================================
// =====================================================================


std::vector<std::string> PreferencesManager::getWorkspacesPaths() const
{
  auto Paths = m_Settings->getValue("/workspaces").get<std::vector<std::string>>(std::vector<std::string>());

  if (Paths.empty())
  {
    Paths.push_back(openfluid::base::Environment::getUserDataFullPath(openfluid::config::WORKSPACE_PATH));
  }

  return Paths;
}


// =====================================================================
// =====================================================================


std::string PreferencesManager::getCurrentWorkspacePath() const
{
  std::vector<std::string> Paths = getWorkspacesPaths();

  if (!Paths.empty())
  {
    return Paths[0];
  }
  else
  {
    return "";
  }
}


// =====================================================================
// =====================================================================


void PreferencesManager::setAppWindowGeometry(const std::string& AppSettings, 
                                              const PreferencesManager::WindowGeometry_t& Geometry)
{
  m_Settings->enableAutoSave(false);

  m_Settings->setValue("/"+AppSettings+"/ui/geometry","position",
                       openfluid::tools::toGeometryString("Point",Geometry.X,Geometry.Y));
  m_Settings->setValue("/"+AppSettings+"/ui/geometry","size",
                       openfluid::tools::toGeometryString("Size",Geometry.Width,Geometry.Height));
  
  m_Settings->save();
  m_Settings->enableAutoSave(true);
}


// =====================================================================
// =====================================================================


PreferencesManager::WindowGeometry_t PreferencesManager::getAppWindowGeometry(const std::string& AppSettings) const
{
  PreferencesManager::WindowGeometry_t Geometry;

  if (m_Settings->exists("/"+AppSettings+"/ui/geometry/position"))
  {
    auto Pos = openfluid::tools::fromGeometryString(m_Settings->getValue("/"+AppSettings+"/ui/geometry/position")
                                                    .get<std::string>(""),"Point");
    Geometry.X = Pos.first;
    Geometry.Y = Pos.second; 
    Geometry.ValidPosition = true;
  }

  if (m_Settings->exists("/"+AppSettings+"/ui/geometry/position"))
  {
    auto Size = openfluid::tools::fromGeometryString(m_Settings->getValue("/"+AppSettings+"/ui/geometry/size")
                                                     .get<std::string>(""),"Size");
    Geometry.Width = Size.first;
    Geometry.Height = Size.second; 
    Geometry.ValidSize = true;
  }

  return Geometry;
}


// =====================================================================
// =====================================================================


void PreferencesManager::setBuilderExtraPaths(const std::string& Key, const std::vector<std::string>& Paths)
{
  m_Settings->setValue("/builder/paths",Key,Paths);
}


// =====================================================================
// =====================================================================


void PreferencesManager::addBuilderExtraPath(const std::string& Key, const std::string& Path)
{
  auto ExtraPaths = 
    m_Settings->getValue("/builder/paths/"+Key).get<std::vector<std::string>>(std::vector<std::string>());

  std::remove(ExtraPaths.begin(),ExtraPaths.end(),Path);
  ExtraPaths.push_back(Path);

  m_Settings->setValue("/builder/paths",Key,ExtraPaths);
}


// =====================================================================
// =====================================================================


void PreferencesManager::removeBuilderExtraPath(const std::string& Key, const std::string& Path)
{
  auto ExtraPaths = 
    m_Settings->getValue("/builder/paths/"+Key).get<std::vector<std::string>>(std::vector<std::string>());

  ExtraPaths.erase(std::remove(ExtraPaths.begin(),ExtraPaths.end(),Path),ExtraPaths.end());

  m_Settings->setValue("/builder/paths",Key,ExtraPaths);
}


// =====================================================================
// =====================================================================


std::vector<std::string> PreferencesManager::getBuilderExtraPaths(const std::string& Key) const
{
  return m_Settings->getValue("/builder/paths/"+Key)
           .get<std::vector<std::string>>(std::vector<std::string>());
}


// =====================================================================
// =====================================================================


void PreferencesManager::setBuilderExtraSimulatorsPaths(const std::vector<std::string>& Paths)
{
  setBuilderExtraPaths("simulators",Paths);
}


// =====================================================================
// =====================================================================


void PreferencesManager::addBuilderExtraSimulatorsPath(const std::string& Path)
{
  addBuilderExtraPath("simulators",Path);
}


// =====================================================================
// =====================================================================


void PreferencesManager::removeBuilderExtraSimulatorsPath(const std::string& Path)
{
  removeBuilderExtraPath("simulators",Path);
}


// =====================================================================
// =====================================================================


std::vector<std::string> PreferencesManager::getBuilderExtraSimulatorsPaths() const
{
  return getBuilderExtraPaths("simulators");
}


// =====================================================================
// =====================================================================


void PreferencesManager::setBuilderExtraExtensionsPaths(const std::vector<std::string>& Paths)
{
  setBuilderExtraPaths("builderexts",Paths);
}


// =====================================================================
// =====================================================================


void PreferencesManager::addBuilderExtraExtensionsPath(const std::string& Path)
{
  addBuilderExtraPath("builderexts",Path);
}


// =====================================================================
// =====================================================================


void PreferencesManager::removeBuilderExtraExtensionsPath(const std::string& Path)
{
  removeBuilderExtraPath("builderexts",Path);
}


// =====================================================================
// =====================================================================


std::vector<std::string> PreferencesManager::getBuilderExtraExtensionsPaths() const
{
  return getBuilderExtraPaths("builderexts");
}


// =====================================================================
// =====================================================================


void PreferencesManager::setBuilderExtraObserversPaths(const std::vector<std::string>& Paths)
{
  setBuilderExtraPaths("observers",Paths);
}


// =====================================================================
// =====================================================================


void PreferencesManager::addBuilderExtraObserversPath(const std::string& Path)
{
  addBuilderExtraPath("observers",Path);
}


// =====================================================================
// =====================================================================


void PreferencesManager::removeBuilderExtraObserversPath(const std::string& Path)
{
  removeBuilderExtraPath("observers",Path);
}


// =====================================================================
// =====================================================================


std::vector<std::string> PreferencesManager::getBuilderExtraObserversPaths() const
{
  return getBuilderExtraPaths("observers");
}


// =====================================================================
// =====================================================================


void PreferencesManager::setBuilderDeltaT(openfluid::core::Duration_t DeltaT)
{
  if (DeltaT < 1)
  {
    DeltaT = 1; 
  }

  m_Settings->setValue("/builder/runconfig","deltat",static_cast<int>(DeltaT));
}


// =====================================================================
// =====================================================================


openfluid::core::Duration_t PreferencesManager::getBuilderDeltaT() const
{
  openfluid::core::Duration_t DeltaT = m_Settings->getValue("/builder/runconfig/deltat").get<int>(3600);
  
  if (DeltaT < 1)
  {
    DeltaT = 1; 
  }

  return DeltaT;
}


// =====================================================================
// =====================================================================


void PreferencesManager::setBuilderBeginDate(const std::string& Begin)
{
  m_Settings->setValue("/builder/runconfig","begin_date",Begin);
}


// =====================================================================
// =====================================================================


std::string PreferencesManager::getBuilderBeginDate() const
{
  return m_Settings->getValue("/builder/runconfig/begin_date").get<std::string>("2021-01-01 00:00:00");
}


// =====================================================================
// =====================================================================


void PreferencesManager::setBuilderEndDate(const std::string& End)
{
  m_Settings->setValue("/builder/runconfig","end_date",End);
}


// =====================================================================
// =====================================================================


std::string PreferencesManager::getBuilderEndDate() const
{
  return m_Settings->getValue("/builder/runconfig/end_date").get<std::string>("2021-06-01 00:00:00");
}


// =====================================================================
// =====================================================================


int PreferencesManager::getBuilderDockArea() const
{
  return m_Settings->getValue("/builder/ui/areas/dock").get<int>(1);  // default is 1 = left area
}


// =====================================================================
// =====================================================================


void PreferencesManager::setBuilderDockArea(int Area)
{
  m_Settings->setValue("/builder/ui/areas","dock",Area);
}


// =====================================================================
// =====================================================================


int PreferencesManager::getBuilderToolBarArea() const
{
  return m_Settings->getValue("/builder/ui/areas/toolbar").get<int>(4);  // default is 4 = top area
}


// =====================================================================
// =====================================================================


void PreferencesManager::setBuilderToolBarArea(int Area)
{
  m_Settings->setValue("/builder/ui/areas","toolbar",Area);
}


// =====================================================================
// =====================================================================


bool PreferencesManager::isBuilderWareRemovalConfirm() const
{
  return m_Settings->getValue("/builder/ui/confirmations/ware_removal").get<bool>(true);
}


// =====================================================================
// =====================================================================


void PreferencesManager::setBuilderWareRemovalConfirm(bool Confirm)
{
  m_Settings->setValue("/builder/ui/confirmations","ware_removal",Confirm);
}


// =====================================================================
// =====================================================================


bool PreferencesManager::isBuilderParamRemovalConfirm() const
{
  return m_Settings->getValue("/builder/ui/confirmations/param_removal").get<bool>(true);
}


// =====================================================================
// =====================================================================


void PreferencesManager::setBuilderParamRemovalConfirm(bool Confirm)
{
  m_Settings->setValue("/builder/ui/confirmations","param_removal",Confirm);
}


// =====================================================================
// =====================================================================


bool PreferencesManager::isBuilderSpatialUnitsRemovalConfirm() const
{
  return m_Settings->getValue("/builder/ui/confirmations/spatialunit_removal").get<bool>(true);
}


// =====================================================================
// =====================================================================


void PreferencesManager::setBuilderSpatialUnitsRemovalConfirm(bool Confirm)
{ 
  m_Settings->setValue("/builder/ui/confirmations","spatialunit_removal",Confirm);
}


// =====================================================================
// =====================================================================


bool PreferencesManager::isBuilderSpatialConnsRemovalConfirm() const
{
  return m_Settings->getValue("/builder/ui/confirmations/spatialconn_removal").get<bool>(true);
}


// =====================================================================
// =====================================================================


void PreferencesManager::setBuilderSpatialConnsRemovalConfirm(bool Confirm)
{ 
  m_Settings->setValue("/builder/ui/confirmations","spatialconn_removal",Confirm);
}


// =====================================================================
// =====================================================================


bool PreferencesManager::isBuilderSpatialAttrsRemovalConfirm() const
{
  return m_Settings->getValue("/builder/ui/confirmations/spatialattr_removal").get<bool>(true);
}


// =====================================================================
// =====================================================================


void PreferencesManager::setBuilderSpatialAttrsRemovalConfirm(bool Confirm)
{
  m_Settings->setValue("/builder/ui/confirmations","spatialattr_removal",Confirm);
}


// =====================================================================
// =====================================================================


bool PreferencesManager::isBuilderWaresWatchersActive() const
{
  return m_Settings->getValue("/builder/ui/wareswatchers").get<bool>(true);
}


// =====================================================================
// =====================================================================


void PreferencesManager::setBuilderWaresWatchersActive(bool Active)
{ 
  m_Settings->setValue("/builder/ui","wareswatchers",Active);
}


// =====================================================================
// =====================================================================


bool PreferencesManager::isBuilderAutomaticSaveBeforeRun() const
{ 
  return m_Settings->getValue("/builder/ui/autosave/beforerun").get<bool>(false);
}


// =====================================================================
// =====================================================================


void PreferencesManager::setBuilderAutomaticSaveBeforeRun(bool AutoSave)
{ 
  m_Settings->setValue("/builder/ui/autosave","beforerun",AutoSave);
}


// =====================================================================
// =====================================================================


bool PreferencesManager::isWaresdevAutomaticSaveBeforeBuild() const
{
  return m_Settings->getValue("/waresdev/ui/autosave/beforebuild").get<bool>(false);
}


// =====================================================================
// =====================================================================


void PreferencesManager::setWaresdevAutomaticSaveBeforeBuild(bool AutoSave)
{ 
  m_Settings->setValue("/waresdev/ui/autosave","beforebuild",AutoSave);
}


// =====================================================================
// =====================================================================


std::list<PreferencesManager::ExternalTool_t> PreferencesManager::getWaresdevExternalTools() const
{ 
  std::list<PreferencesManager::ExternalTool_t> ToolsList;

  auto Value = m_Settings->getValue("/waresdev/ui/externaltools");

  if (Value.isNull())
  {
    return ToolsList;
  }

  auto JSONArray = Value.JSONValue();

  if (JSONArray.is_array())
  {
    for (const auto& V : JSONArray)
    {
      if (V.is_object() && V.contains("name"))
      {
        PreferencesManager::ExternalTool_t Tool;

        Tool.Name = V["name"];

        if (V.contains("workspace_command"))
        {
          std::string Cmd = openfluid::tools::trim(std::string(V["workspace_command"]));
          if (!Cmd.empty())
          {
            Tool.Commands[PreferencesManager::ExternalToolContext::WORKSPACE] = Cmd;
          }
        }

        if (V.contains("ware_command"))
        {
          std::string Cmd = openfluid::tools::trim(std::string(V["ware_command"]));
          if (!Cmd.empty())
          {
            Tool.Commands[PreferencesManager::ExternalToolContext::WARE] = Cmd;
          }
        }

        if (V.contains("file_command"))
        {
          std::string Cmd = openfluid::tools::trim(std::string(V["file_command"]));
          if (!Cmd.empty())
          {
            Tool.Commands[PreferencesManager::ExternalToolContext::FILE] = Cmd;
          }
        }

        ToolsList.push_back(Tool);
      }
    }
  }  

  return ToolsList;
}


// =====================================================================
// =====================================================================


std::list<PreferencesManager::ExternalTool_t> 
  PreferencesManager::getWaresdevExternalToolsInContext(const ExternalToolContext Context) const
{
  std::list<PreferencesManager::ExternalTool_t> ToolsList = getWaresdevExternalTools();
  
/*  std::list<PreferencesManager::ExternalTool_t> CtxtToolsList;

  for (const auto& T : ToolsList)
  {
    if (T.Commands.find(Context)!= T.Commands.end())
    {
      CtxtToolsList.push_back(T);
    }
  }

  return CtxtToolsList;*/

  auto it = ToolsList.begin();
  while (it != ToolsList.end()) 
  {
    const auto& Cmds = (*it).Commands;

    if (Cmds.find(Context)!= Cmds.end())
    {
      it++;  // Command exists for given Context: keep it
    }
    else
    {
      it = ToolsList.erase(it); // Command does not exists for given Context: remove it
    }
  }

  return ToolsList;
}


// =====================================================================
// =====================================================================


void PreferencesManager::setWaresdevExternalTools(const std::list<PreferencesManager::ExternalTool_t>& Tools)
{ 
  m_Settings->enableAutoSave(false);

  m_Settings->remove("/waresdev/ui/externaltools");

  unsigned int Index = 0;
  for (const auto& T: Tools)
  {
    std::string WorkspaceCmd;
    std::string WareCmd;
    std::string FileCmd;

    for (const auto& C: T.Commands)
    {
      if (C.first == PreferencesManager::ExternalToolContext::WORKSPACE)
      {
        WorkspaceCmd = C.second;
      }
      else if (C.first == PreferencesManager::ExternalToolContext::WARE)
      {
        WareCmd = C.second;
      }
      else
      {
        FileCmd = C.second;
      }
    }
    
    std::string IndexedToolPointer = "/waresdev/ui/externaltools/"+std::to_string(Index);
    m_Settings->setValue(IndexedToolPointer,"name",T.Name);
    m_Settings->setValue(IndexedToolPointer,"workspace_command",WorkspaceCmd);
    m_Settings->setValue(IndexedToolPointer,"ware_command",WareCmd);
    m_Settings->setValue(IndexedToolPointer,"file_command",FileCmd);
    Index++;
  }

  m_Settings->save();
  m_Settings->enableAutoSave(true);
}


// =====================================================================
// =====================================================================


void PreferencesManager::setWaresdevTextEditorDefaults(bool ForceReset)
{ 
  const SyntaxHighlightingRules_t DefaultRules = 
  {
    { "function", {"system",{"bold"}} },
    { "keyword", {"#7F0055",{"bold"}} },
    { "datatype", {"system",{"bold"}} },
    { "preprocessor", {"#7f0055",{"bold"}} },
    { "openfluid-keyword", {"#546f02",{"bold"}} },
    { "openfluid-deprecated", {"#546f02",{"bold","strike-through"}} },
    { "deprecated", {"#7f0055",{"bold","strike-through"}} },
    { "quoted", {"#2a00ff",{"none"}} },
    { "comment", {"#a7a7a7",{"italic"}} },
    { "invisible", {"#aaaaaa",{"none"}} }
  };

  SyntaxHighlightingRules_t Rules = getWaresdevSyntaxHighlightingRules();

  for (const auto& Category : DefaultRules)
  {
    if (Rules.find(Category.first) == Rules.end() || ForceReset)
    {
      Rules[Category.first] = { Category.second.Color,Category.second.Decoration };
    }
  }


  m_Settings->enableAutoSave(false);


  // syntax highlighting

  if (!m_Settings->exists("/waresdev/ui/texteditor/highlighting/syntax/enabled") || ForceReset)
  {
    m_Settings->setValue("/waresdev/ui/texteditor/highlighting/syntax","enabled",true);
  }

  unsigned int Index = 0;
  for (const auto& Category : Rules)
  {
    std::string IndexedRulePointer = "/waresdev/ui/texteditor/highlighting/syntax/rules/"+std::to_string(Index);
    m_Settings->setValue(IndexedRulePointer,"name",Category.first);
    m_Settings->setValue(IndexedRulePointer,"color",Category.second.Color);
    m_Settings->setValue(IndexedRulePointer,"decoration",Category.second.Decoration);
    Index++;
  }


  // current line highlighting

  if (!m_Settings->exists("/waresdev/ui/texteditor/highlighting/currentline/enabled") || ForceReset)
  {
    m_Settings->setValue("/waresdev/ui/texteditor/highlighting/currentline","enabled",true);
  }

  if (!m_Settings->exists("/waresdev/ui/texteditor/highlighting/currentline/color") || ForceReset)
  {
    m_Settings->setValue("/waresdev/ui/texteditor/highlighting/currentline","color","#eff6ff");
  }


  // font name

  if (!m_Settings->exists("/waresdev/ui/texteditor/font/name") || ForceReset)
  {
    m_Settings->setValue("/waresdev/ui/texteditor/font","name","monospace");
  }


  // indent

  if (!m_Settings->exists("/waresdev/ui/texteditor/indentation") || ForceReset)
  {
    m_Settings->setValue("/waresdev/ui/texteditor","indentation",2);
  }


  // line wrapping

  if (!m_Settings->exists("/waresdev/ui/texteditor/linewrap") || ForceReset)
  {
    m_Settings->setValue("/waresdev/ui/texteditor","linewrap",false);
  }


  // invisible characters

  if (!m_Settings->exists("/waresdev/ui/texteditor/invisible/show_spacetab") || ForceReset)
  {
    m_Settings->setValue("/waresdev/ui/texteditor/invisible","show_spacetab",false);
  }

  if (!m_Settings->exists("/waresdev/ui/texteditor/invisible/show_crlf") || ForceReset)
  {
    m_Settings->setValue("/waresdev/ui/texteditor/invisible","show_crlf",false);
  }


  m_Settings->save();
  m_Settings->enableAutoSave(true);
}


// =====================================================================
// =====================================================================


bool PreferencesManager::isWaresdevSyntaxHighlightingEnabled() const
{   
  return m_Settings->getValue("/waresdev/ui/texteditor/highlighting/syntax/enabled").get<bool>(true);
}


// =====================================================================
// =====================================================================


void PreferencesManager::setWaresdevSyntaxHighlightingEnabled(bool Enabled)
{ 
  m_Settings->setValue("/waresdev/ui/texteditor/highlighting/syntax","enabled",Enabled);
}


// =====================================================================
// =====================================================================


PreferencesManager::SyntaxHighlightingRules_t PreferencesManager::getWaresdevSyntaxHighlightingRules() const
{ 
  SyntaxHighlightingRules_t Rules;

  auto Value = m_Settings->getValue("/waresdev/ui/texteditor/highlighting/syntax/rules");

  if (Value.isNull())
  {
    return Rules;
  }

  auto JSONArray = Value.JSONValue();

  if (JSONArray.is_array())
  {
    for (const auto& V : JSONArray)
    {
      if (V.is_object() && V.contains("name"))
      {
        PreferencesManager::SyntaxHighlightingRule_t Rule;
        
        std::string RuleName = V["name"];

        if (V.contains("color"))
        {
          std::string Color = openfluid::tools::trim(std::string(V["color"]));
          if (!Color.empty())
          {
            Rule.Color = Color;
          }
        }

        if (V.contains("decoration") && V["decoration"].is_array())
        {
          std::vector<std::string> DecoVect;
          for (auto& Deco : V["decoration"])
          {
            if (Deco.is_string())
            {
              DecoVect.push_back(Deco); 
            }
          }
          Rule.Decoration = DecoVect;
        }
        Rules[RuleName] = Rule;
      }
    }
  }

  return Rules;
}


// =====================================================================
// =====================================================================


void PreferencesManager::setWaresdevSyntaxHighlightingRules(const SyntaxHighlightingRules_t& Rules)
{ 
  m_Settings->enableAutoSave(false);

  unsigned int Index = 0;
  for (const auto& Category : Rules)
  {
    std::string IndexedRulePointer = "/waresdev/ui/texteditor/highlighting/syntax/rules/"+std::to_string(Index);
    m_Settings->setValue(IndexedRulePointer,"name",Category.first);
    m_Settings->setValue(IndexedRulePointer,"color",Category.second.Color);
    m_Settings->setValue(IndexedRulePointer,"decoration",Category.second.Decoration);
    Index++;
  }

  m_Settings->save();
  m_Settings->enableAutoSave(true);
}


// =====================================================================
// =====================================================================


bool PreferencesManager::isWaresdevCurrentlineHighlightingEnabled() const
{ 
  return m_Settings->getValue("/waresdev/ui/texteditor/highlighting/currentline/enabled").get<bool>(true);
}


// =====================================================================
// =====================================================================


void PreferencesManager::setWaresdevCurrentlineHighlightingEnabled(bool Enabled)
{ 
  m_Settings->setValue("/waresdev/ui/texteditor/highlighting/currentline","enabled",Enabled);
}


// =====================================================================
// =====================================================================


std::string PreferencesManager::getWaresdevCurrentlineColor() const
{ 
  return m_Settings->getValue("/waresdev/ui/texteditor/highlighting/currentline/color").get<std::string>("#eff6ff");
}


// =====================================================================
// =====================================================================


void PreferencesManager::setWaresdevCurrentlineColor(const std::string& Color)
{ 
  m_Settings->setValue("/waresdev/ui/texteditor/highlighting/currentline","color",Color); 
}


// =====================================================================
// =====================================================================


std::string PreferencesManager::getWaresdevFontName() const
{ 
  return m_Settings->getValue("/waresdev/ui/texteditor/font/name").get<std::string>("monospace");
}


// =====================================================================
// =====================================================================


void PreferencesManager::setWaresdevFontName(const std::string& FontName)
{ 
  m_Settings->setValue("/waresdev/ui/texteditor/font","name", FontName);
}


// =====================================================================
// =====================================================================


bool PreferencesManager::isWaresdevLineWrappingEnabled() const
{ 
  return m_Settings->getValue("/waresdev/ui/texteditor/linewrap").get<bool>(false);
}


// =====================================================================
// =====================================================================


void PreferencesManager::setWaresdevLineWrappingEnabled(bool Enabled)
{ 
  m_Settings->setValue("/waresdev/ui/texteditor","linewrap",Enabled);
}


// =====================================================================
// =====================================================================


int PreferencesManager::getWaresdevIndentSpaceNb() const
{ 
  return m_Settings->getValue("/waresdev/ui/texteditor/indentation").get<int>(2);
}


// =====================================================================
// =====================================================================


void PreferencesManager::setWaresdevIndentSpaceNb(int SpaceNumber)
{
  return m_Settings->setValue("/waresdev/ui/texteditor","indentation",SpaceNumber);
}


// =====================================================================
// =====================================================================


bool PreferencesManager::isWaresdevSpaceTabDisplayEnabled() const
{
  return m_Settings->getValue("/waresdev/ui/texteditor/invisible/show_spacetab").get<bool>(false);
}


// =====================================================================
// =====================================================================


void PreferencesManager::setWaresdevSpaceTabDisplayEnabled(bool Enabled)
{ 
  return m_Settings->setValue("/waresdev/ui/texteditor/invisible","show_spacetab",Enabled);
}


// =====================================================================
// =====================================================================


bool PreferencesManager::isWaresdevCarriageReturnDisplayEnabled() const
{ 
  return m_Settings->getValue("/waresdev/ui/texteditor/invisible/show_crlf").get<bool>(false);
}


// =====================================================================
// =====================================================================


void PreferencesManager::setWaresdevCarriageReturnDisplayEnabled(bool Enabled)
{ 
  return m_Settings->setValue("/waresdev/ui/texteditor/invisible","show_crlf",Enabled);
}


// =====================================================================
// =====================================================================


std::string PreferencesManager::getWaresdevConfigureEnv(const std::string& Name) const
{ 
  return m_Settings->getValue("/waresdev/commands/configure/env/"+Name).get<std::string>("");
}


// =====================================================================
// =====================================================================


void PreferencesManager::setWaresdevConfigureEnv(const std::string& Name, const std::string& Value)
{ 
  m_Settings->setValue("/waresdev/commands/configure/env",Name,Value);
}


// =====================================================================
// =====================================================================


std::string PreferencesManager::getWaresdevConfigureOptions() const
{ 
  return m_Settings->getValue("/waresdev/commands/configure/options").get<std::string>("");
}


// =====================================================================
// =====================================================================


void PreferencesManager::setWaresdevConfigureOptions(const std::string& Options)
{ 
  m_Settings->setValue("/waresdev/commands/configure","options",Options);
}


// =====================================================================
// =====================================================================


std::string PreferencesManager::getWaresdevConfigureGenerator() const
{ 
  std::string DefaultVal;

#if defined(OPENFLUID_OS_WINDOWS)
  DefaultVal = "MinGW Makefiles";
#endif

  return m_Settings->getValue("/waresdev/commands/configure/generator").get<std::string>(DefaultVal);
}


// =====================================================================
// =====================================================================


void PreferencesManager::setWaresdevConfigureGenerator(const std::string& Generator)
{ 
  m_Settings->setValue("/waresdev/commands/configure","generator",Generator);
}


// =====================================================================
// =====================================================================


std::string PreferencesManager::getWaresdevBuildEnv(const std::string& Name) const
{ 
  return m_Settings->getValue("/waresdev/commands/build/env/"+Name).get<std::string>("");
}


// =====================================================================
// =====================================================================


void PreferencesManager::setWaresdevBuildEnv(const std::string& Name, const std::string& Value)
{ 
  m_Settings->setValue("/waresdev/commands/build/env",Name,Value);
}


// =====================================================================
// =====================================================================


bool PreferencesManager::isWaresdevShowCommandEnv(const std::string& Name) const
{
  return m_Settings->getValue("/waresdev/commands/common/show_env/"+Name).get<bool>(false);
}


// =====================================================================
// =====================================================================


void PreferencesManager::setWaresdevShowCommandEnv(const std::string& Name, bool Enabled)
{ 
  m_Settings->setValue("/waresdev/commands/common/show_env",Name,Enabled);
}


// =====================================================================
// =====================================================================


bool PreferencesManager::isWaresdevGitSslNoVerify() const
{ 
  return m_Settings->getValue("/waresdev/commands/git/ssl_noverify").get<bool>(false);
}


// =====================================================================
// =====================================================================


void PreferencesManager::setWaresdevGitSslNoVerify(bool NoVerify)
{ 
  m_Settings->setValue("/waresdev/commands/git","ssl_noverify",NoVerify);
}


// =====================================================================
// =====================================================================


std::string PreferencesManager::getWaresdevImportHubUrl() const
{ 
  return m_Settings->getValue("/waresdev/ui/import/hub/url").get<std::string>("");
}


// =====================================================================
// =====================================================================


void PreferencesManager::setWaresdevImportHubUrl(const std::string& Url)
{ 
  m_Settings->setValue("/waresdev/ui/import/hub","url",Url);
}


// =====================================================================
// =====================================================================


std::string PreferencesManager::getWaresdevImportHubUsername() const
{
  return m_Settings->getValue("/waresdev/ui/import/hub/username").get<std::string>("");
}


// =====================================================================
// =====================================================================


void PreferencesManager::setWaresdevImportHubUsername(const std::string& Username)
{ 
  m_Settings->setValue("/waresdev/ui/import/hub","username",Username);
}


} } //namespaces
