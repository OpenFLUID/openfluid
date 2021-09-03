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
  @file PreferencesManager_TEST.cpp

  @author Aline LIBRES <libres@supagro.inra.fr>
*/


#define BOOST_TEST_MAIN
#define BOOST_AUTO_TEST_MAIN
#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MODULE unittest_preferencesmanager


#include <boost/test/unit_test.hpp>

#include <openfluid/base/PreferencesManager.hpp>
#include <openfluid/config.hpp>
#include <openfluid/base/FrameworkException.hpp>
#include <openfluid/base/Environment.hpp>
#include <openfluid/tools/Filesystem.hpp>

#include "tests-config.hpp"


// =====================================================================
// =====================================================================


const std::string PrefsPath =
  openfluid::tools::Filesystem::joinPath({CONFIGTESTS_OUTPUT_DATA_DIR,"PreferencesManager"});
const std::string ConversionPrefsPath =
  openfluid::tools::Filesystem::joinPath({CONFIGTESTS_OUTPUT_DATA_DIR,"PreferencesManager","conversion"});  
const std::string DefaultPrefsFile = openfluid::tools::Filesystem::joinPath({PrefsPath,"default.json"});
const std::string CreatedPrefsFile = openfluid::tools::Filesystem::joinPath({PrefsPath,"created.json"});
const std::string ConvertedPrefsFile =  openfluid::tools::Filesystem::joinPath({ConversionPrefsPath,"converted.json"});

const std::vector<std::string> WorkspacesPaths = {"/path/to/workspace",
                                                  "/path/to/workspace-test",
                                                  "/path/to/workspace-test-2"};

const std::vector<std::string> ExtraSimsPaths = {"/path/to/sims-lib1",
                                                 "/path/to/sims-lib2",
                                                 "/path/to/sims-lib3"};

const std::vector<std::string> ExtraObssPaths = {"/path/to/obss-lib1"};

const std::vector<std::string> ExtraBextsPaths = {};

const std::list<openfluid::base::PreferencesManager::ExternalTool_t> ExternalTools = {
  { "My Tool", 
    { 
      {openfluid::base::PreferencesManager::ExternalToolContext::WORKSPACE, "/usr/local/bin/mytool --workspace=%%W%%"},
      {openfluid::base::PreferencesManager::ExternalToolContext::WARE, "/usr/local/bin/mytool --ware=%%S%%"},
      {openfluid::base::PreferencesManager::ExternalToolContext::FILE, "/usr/local/bin/mytool --file=%%C%%"} 
    } 
  },
  { "Another Tool", 
    { 
      {openfluid::base::PreferencesManager::ExternalToolContext::WORKSPACE, 
       "C:\\\\Program Files\\\\AnotherTool-18.5\\\\anottool.exe %%W%%"}
    }
  },
  { "The Amazing App", 
    { 
      {openfluid::base::PreferencesManager::ExternalToolContext::WARE, "/Applications/TheAmazingApp.app %%S%%"},
      {openfluid::base::PreferencesManager::ExternalToolContext::FILE, "/Applications/TheAmazingApp.app %%C%%"} 
    }  
  }
};


// =====================================================================
// =====================================================================


BOOST_AUTO_TEST_CASE(check_construction)
{
  openfluid::tools::Filesystem::removeFile(DefaultPrefsFile);


  openfluid::base::PreferencesManager* PrefsMgr = openfluid::base::PreferencesManager::instance();

  BOOST_CHECK_THROW(openfluid::base::PreferencesManager::setSettingsFile(PrefsPath+"/wrong.json"),
                    openfluid::base::FrameworkException);

  BOOST_CHECK(PrefsMgr->getSettingsFile() == openfluid::base::Environment::getSettingsFile());

  openfluid::base::PreferencesManager::kill();


  openfluid::base::PreferencesManager::setSettingsFile(DefaultPrefsFile);
  PrefsMgr = openfluid::base::PreferencesManager::instance();

  BOOST_CHECK(PrefsMgr->getSettingsFile() == DefaultPrefsFile);

  auto BuilderGeometry = PrefsMgr->getAppWindowGeometry("builder");
  auto DevStudioGeometry = PrefsMgr->getAppWindowGeometry("devstudio");
  BOOST_CHECK(!BuilderGeometry.ValidPosition);
  BOOST_CHECK(!BuilderGeometry.ValidSize);
  BOOST_CHECK(!DevStudioGeometry.ValidPosition);
  BOOST_CHECK(!DevStudioGeometry.ValidSize);

  BOOST_CHECK_EQUAL(PrefsMgr->getBuilderDeltaT(),3600);
  BOOST_CHECK(!PrefsMgr->getBuilderBeginDate().empty());
  BOOST_CHECK(!PrefsMgr->getBuilderEndDate().empty());
  
  BOOST_CHECK(PrefsMgr->isWaresdevSyntaxHighlightingEnabled());
  BOOST_CHECK_EQUAL(PrefsMgr->getWaresdevSyntaxHighlightingRules().size(),10);
  BOOST_CHECK(PrefsMgr->isWaresdevCurrentlineHighlightingEnabled());
  BOOST_CHECK_EQUAL(PrefsMgr->getWaresdevCurrentlineColor(),"#eff6ff");
  BOOST_CHECK_EQUAL(PrefsMgr->getWaresdevIndentSpaceNb(),2);
  BOOST_CHECK(!PrefsMgr->isWaresdevSpaceTabDisplayEnabled());
  BOOST_CHECK(!PrefsMgr->isWaresdevCarriageReturnDisplayEnabled());
  BOOST_CHECK(!PrefsMgr->isWaresdevLineWrappingEnabled());

  BOOST_CHECK(PrefsMgr->getWaresdevConfigureOptions().empty());
  BOOST_CHECK(PrefsMgr->getWaresdevConfigureEnv("PATH").empty());
  BOOST_CHECK(PrefsMgr->getWaresdevConfigureEnv("FAKEC").empty());
  BOOST_CHECK(PrefsMgr->getWaresdevBuildEnv("PATH").empty());
  BOOST_CHECK(PrefsMgr->getWaresdevBuildEnv("FAKEB").empty());

  BOOST_CHECK(!PrefsMgr->isWaresdevShowCommandEnv("PATH"));

  BOOST_CHECK(PrefsMgr->getWaresdevImportHubUrl().empty());
  BOOST_CHECK(PrefsMgr->getWaresdevImportHubUsername().empty());

  BOOST_CHECK(!PrefsMgr->isWaresdevGitSslNoVerify());

  openfluid::base::PreferencesManager::kill();
}


// =====================================================================
// =====================================================================


BOOST_AUTO_TEST_CASE(check_creation)
{
  openfluid::tools::Filesystem::removeFile(CreatedPrefsFile);

  openfluid::base::PreferencesManager::setSettingsFile(CreatedPrefsFile);
  auto PrefsMgr = openfluid::base::PreferencesManager::instance();

  PrefsMgr->setWorkspacesPaths(WorkspacesPaths);

  PrefsMgr->setUILanguage("it_IT");

  PrefsMgr->setAppWindowGeometry("builder",{100,150,1200,1024,true,true});
  PrefsMgr->setAppWindowGeometry("devstudio",{200,250,800,600,true,true});

  PrefsMgr->setBuilderExtraSimulatorsPaths(ExtraSimsPaths);
  PrefsMgr->removeBuilderExtraSimulatorsPath("/path/to/sims-lib2");
  PrefsMgr->setBuilderExtraObserversPaths(ExtraObssPaths);
  PrefsMgr->addBuilderExtraObserversPath("/path/to/obss-lib2");
  PrefsMgr->setBuilderExtraExtensionsPaths(ExtraBextsPaths);
    
  PrefsMgr->setBuilderDockArea(1);
  PrefsMgr->setBuilderToolBarArea(2);
  PrefsMgr->setBuilderWaresWatchersActive(false);
  PrefsMgr->setBuilderWareRemovalConfirm(true);
  PrefsMgr->setBuilderParamRemovalConfirm(false);
  PrefsMgr->setBuilderSpatialAttrsRemovalConfirm(false);
  PrefsMgr->setBuilderSpatialConnsRemovalConfirm(true);
  PrefsMgr->setBuilderSpatialUnitsRemovalConfirm(true);

  PrefsMgr->setBuilderDeltaT(300);
  PrefsMgr->setBuilderBeginDate("2008-01-02 11:13:00");
  PrefsMgr->setBuilderEndDate("2011-08-06 16:01:00");

  PrefsMgr->setWaresdevAutomaticSaveBeforeBuild(true);

  auto SHRules = PrefsMgr->getWaresdevSyntaxHighlightingRules();
  SHRules["openfluid-deprecated"] = { "olive", {"italic","underline"} };
  PrefsMgr->setWaresdevSyntaxHighlightingRules(SHRules);
  PrefsMgr->setWaresdevCurrentlineHighlightingEnabled(false);
  PrefsMgr->setWaresdevLineWrappingEnabled(true);
  PrefsMgr->setWaresdevSpaceTabDisplayEnabled(true);
  PrefsMgr->setWaresdevIndentSpaceNb(3);

  PrefsMgr->setWaresdevConfigureOptions("-DOPTION=ON -DTEXT=\"a quoted text\"");
  PrefsMgr->setWaresdevConfigureGenerator("Fake Generator");
  PrefsMgr->setWaresdevConfigureEnv("PATH","$PATH:/path/to/conf/bin");
  PrefsMgr->setWaresdevConfigureEnv("MYCONFENV","MyConfValue");
  PrefsMgr->setWaresdevBuildEnv("PATH","$PATH:/path/to/build/bin");
  PrefsMgr->setWaresdevBuildEnv("MYBUILDENV","MyBuildValue");
  PrefsMgr->setWaresdevShowCommandEnv("PATH",true);

  PrefsMgr->setWaresdevExternalTools(ExternalTools);

  openfluid::base::PreferencesManager::kill();
}


// =====================================================================
// =====================================================================


BOOST_AUTO_TEST_CASE(check_opening)
{
  openfluid::tools::Filesystem::removeFile(ConvertedPrefsFile);

  openfluid::base::PreferencesManager::setSettingsFile(CreatedPrefsFile);
  auto PrefsMgr = openfluid::base::PreferencesManager::instance();
  
  { // workspaces
    auto Paths = PrefsMgr->getWorkspacesPaths();
    BOOST_CHECK_EQUAL_COLLECTIONS(WorkspacesPaths.begin(),WorkspacesPaths.end(),Paths.begin(),Paths.end());
  }

  { // ui lang
    BOOST_CHECK_EQUAL(PrefsMgr->getUILanguage(),"it_IT");
  }

  { // apps windows geometry 
    auto BuilderGeometry = PrefsMgr->getAppWindowGeometry("builder");
    auto DevStudioGeometry = PrefsMgr->getAppWindowGeometry("devstudio");

    BOOST_CHECK_EQUAL(BuilderGeometry.X,100);
    BOOST_CHECK_EQUAL(BuilderGeometry.Y,150);
    BOOST_CHECK_EQUAL(BuilderGeometry.Width,1200);
    BOOST_CHECK_EQUAL(BuilderGeometry.Height,1024);
    BOOST_CHECK_EQUAL(DevStudioGeometry.X,200);
    BOOST_CHECK_EQUAL(DevStudioGeometry.Y,250);
    BOOST_CHECK_EQUAL(DevStudioGeometry.Width,800);
    BOOST_CHECK_EQUAL(DevStudioGeometry.Height,600);
  }
  
  { // builder wares paths
    auto BuilderSimPaths = PrefsMgr->getBuilderExtraSimulatorsPaths();
    auto BuilderObsPaths = PrefsMgr->getBuilderExtraObserversPaths();
    auto BuilderExtPaths = PrefsMgr->getBuilderExtraExtensionsPaths();

    BOOST_CHECK_EQUAL(BuilderSimPaths.size(),2);
    BOOST_CHECK_EQUAL(BuilderSimPaths[0],ExtraSimsPaths[0]);
    BOOST_CHECK_EQUAL(BuilderSimPaths[1],ExtraSimsPaths[2]);  
    BOOST_CHECK_EQUAL(BuilderObsPaths.size(),2);
    BOOST_CHECK_EQUAL(BuilderObsPaths[0],ExtraObssPaths[0]);
    BOOST_CHECK(!BuilderObsPaths[1].empty());
    BOOST_CHECK(BuilderExtPaths.empty());
  }

  { // builder ui
    BOOST_CHECK_EQUAL(PrefsMgr->getBuilderToolBarArea(),2);
    BOOST_CHECK_EQUAL(PrefsMgr->getBuilderDockArea(),1);
    BOOST_CHECK(!PrefsMgr->isBuilderWaresWatchersActive());
    BOOST_CHECK(PrefsMgr->isBuilderWareRemovalConfirm());
    BOOST_CHECK(!PrefsMgr->isBuilderParamRemovalConfirm());
    BOOST_CHECK(!PrefsMgr->isBuilderSpatialAttrsRemovalConfirm());
    BOOST_CHECK(PrefsMgr->isBuilderSpatialConnsRemovalConfirm());
    BOOST_CHECK(PrefsMgr->isBuilderSpatialUnitsRemovalConfirm());
    BOOST_CHECK(!PrefsMgr->isBuilderAutomaticSaveBeforeRun());
  }

  { // builder runconfig
    BOOST_CHECK_EQUAL(PrefsMgr->getBuilderDeltaT(),300);
    BOOST_CHECK_EQUAL(PrefsMgr->getBuilderBeginDate(),"2008-01-02 11:13:00");
    BOOST_CHECK_EQUAL(PrefsMgr->getBuilderEndDate(),"2011-08-06 16:01:00");
  }

  {  // waresdev text editor
    BOOST_CHECK(PrefsMgr->isWaresdevSyntaxHighlightingEnabled());
    auto SHRules = PrefsMgr->getWaresdevSyntaxHighlightingRules();
    BOOST_CHECK_EQUAL(SHRules.size(),10);
    BOOST_REQUIRE(SHRules.find("openfluid-deprecated") != SHRules.end());
    BOOST_CHECK_EQUAL(SHRules["openfluid-deprecated"].Color,"olive");
    BOOST_CHECK_EQUAL(SHRules["openfluid-deprecated"].Decoration.size(),2);
    BOOST_CHECK_EQUAL(SHRules["openfluid-deprecated"].Decoration[0],"italic");
    BOOST_CHECK_EQUAL(SHRules["openfluid-deprecated"].Decoration[1],"underline");
    BOOST_CHECK(!PrefsMgr->isWaresdevCurrentlineHighlightingEnabled());
    BOOST_CHECK_EQUAL(PrefsMgr->getWaresdevCurrentlineColor(),"#eff6ff");
    BOOST_CHECK_EQUAL(PrefsMgr->getWaresdevFontName(),"monospace");
    BOOST_CHECK_EQUAL(PrefsMgr->getWaresdevIndentSpaceNb(),3);
    BOOST_CHECK(PrefsMgr->isWaresdevLineWrappingEnabled());
    BOOST_CHECK(PrefsMgr->isWaresdevSpaceTabDisplayEnabled());
    BOOST_CHECK(!PrefsMgr->isWaresdevCarriageReturnDisplayEnabled());
  }

  { // waresdev ui 
    BOOST_CHECK(PrefsMgr->isWaresdevAutomaticSaveBeforeBuild());
  }

  { // waresdev commands
    BOOST_CHECK_EQUAL(PrefsMgr->getWaresdevConfigureOptions(),"-DOPTION=ON -DTEXT=\"a quoted text\"");
    BOOST_CHECK_EQUAL(PrefsMgr->getWaresdevConfigureGenerator(),"Fake Generator");
    BOOST_CHECK_EQUAL(PrefsMgr->getWaresdevConfigureEnv("PATH"),"$PATH:/path/to/conf/bin");
    BOOST_CHECK_EQUAL(PrefsMgr->getWaresdevConfigureEnv("MYCONFENV"),"MyConfValue");
    BOOST_CHECK_EQUAL(PrefsMgr->getWaresdevBuildEnv("PATH"),"$PATH:/path/to/build/bin");
    BOOST_CHECK_EQUAL(PrefsMgr->getWaresdevBuildEnv("MYBUILDENV"),"MyBuildValue");
    BOOST_CHECK_EQUAL(PrefsMgr->isWaresdevShowCommandEnv("PATH"),true);
  }

  { // waresdev external tools
    auto ExtTools = PrefsMgr->getWaresdevExternalTools();
    BOOST_REQUIRE_EQUAL(ExtTools.size(),ExternalTools.size());

    auto it1 = ExtTools.begin();
    auto it2 = ExternalTools.begin();
    for (unsigned int i=0; i<ExtTools.size(); i++)
    {
      BOOST_CHECK_EQUAL((*it1).Name,(*it2).Name);
      BOOST_CHECK_EQUAL((*it1).Commands.size(),(*it2).Commands.size());
      for (const auto& Cmd : (*it1).Commands)
      {
        BOOST_CHECK_EQUAL(Cmd.second,(*it2).Commands.at(Cmd.first));
      }

      it1++;
      it2++;
    }
  }

  openfluid::base::PreferencesManager::kill();
}


// =====================================================================
// =====================================================================


BOOST_AUTO_TEST_CASE(check_conversion)
{
  std::string RefConfFile = 
    openfluid::tools::Filesystem::joinPath({CONFIGTESTS_INPUT_MISCDATA_DIR,"PreferencesManager","openfluid.conf"});
  std::string WorkConfFile = 
    openfluid::tools::Filesystem::joinPath({CONFIGTESTS_OUTPUT_DATA_DIR,"PreferencesManager",
                                           "conversion","openfluid.conf"});

  openfluid::tools::Filesystem::makeDirectory(ConversionPrefsPath);
  openfluid::tools::Filesystem::removeFile(WorkConfFile);
  openfluid::tools::Filesystem::removeFile(ConvertedPrefsFile);
  openfluid::tools::Filesystem::copyFile(RefConfFile,WorkConfFile);

  openfluid::base::PreferencesManager::setSettingsFile(ConvertedPrefsFile);
  auto PrefsMgr = openfluid::base::PreferencesManager::instance();

  { // workspaces path
  auto WksPath = PrefsMgr->getWorkspacesPaths();
  BOOST_REQUIRE_EQUAL(WksPath.size(),3);
  BOOST_CHECK_EQUAL(WksPath[0],"/path/to/workspace1");
  BOOST_CHECK_EQUAL(WksPath[1],"/path/to/workspace2");
  BOOST_CHECK_EQUAL(WksPath[2],"/path/to/workspace3");  
  }

  { // ui langs
    BOOST_CHECK_EQUAL(PrefsMgr->getUILanguage(),"de_DE");
  }

  { // builder simulators paths
    auto WaresPath = PrefsMgr->getBuilderExtraSimulatorsPaths();
    BOOST_REQUIRE_EQUAL(WaresPath.size(),3);
    BOOST_CHECK_EQUAL(WaresPath[0],"/path/to/simulators-lib3");
    BOOST_CHECK_EQUAL(WaresPath[1],"/path/to/simulators-lib2");
    BOOST_CHECK_EQUAL(WaresPath[2],"/path/to/simulators-lib1");
  }
  { // builder observers paths
    auto WaresPath = PrefsMgr->getBuilderExtraObserversPaths();
    BOOST_REQUIRE_EQUAL(WaresPath.size(),2);
    BOOST_CHECK_EQUAL(WaresPath[0],"/path/to/observers-lib1");
    BOOST_CHECK_EQUAL(WaresPath[1],"/path/to/observers-lib2");
  }
  { // builder extensions paths
    auto WaresPath = PrefsMgr->getBuilderExtraExtensionsPaths();
    BOOST_REQUIRE(WaresPath.empty());
  }

  { // builder ui
    BOOST_CHECK_EQUAL(PrefsMgr->getBuilderDockArea(),1);
    BOOST_CHECK_EQUAL(PrefsMgr->getBuilderToolBarArea(),4);
    BOOST_CHECK(PrefsMgr->isBuilderWareRemovalConfirm());
    BOOST_CHECK(PrefsMgr->isBuilderParamRemovalConfirm());
    BOOST_CHECK(PrefsMgr->isBuilderSpatialAttrsRemovalConfirm());
    BOOST_CHECK(!PrefsMgr->isBuilderSpatialConnsRemovalConfirm());
    BOOST_CHECK(PrefsMgr->isBuilderSpatialUnitsRemovalConfirm());
    BOOST_CHECK(!PrefsMgr->isBuilderAutomaticSaveBeforeRun());
    BOOST_CHECK(PrefsMgr->isBuilderWaresWatchersActive());
  }

  { // builder runconfig
    BOOST_CHECK_EQUAL(PrefsMgr->getBuilderBeginDate(),"2018-02-06 00:00:00");
    BOOST_CHECK_EQUAL(PrefsMgr->getBuilderEndDate(),"2018-02-07 00:00:00");
    BOOST_CHECK_EQUAL(PrefsMgr->getBuilderDeltaT(),600);
  }

  { // waresdev ui
    BOOST_CHECK(PrefsMgr->isWaresdevAutomaticSaveBeforeBuild());
  }

  { // waresdev texteditor
    BOOST_CHECK_EQUAL(PrefsMgr->getWaresdevFontName(),"DejaVu Sans Mono");
    BOOST_CHECK_EQUAL(PrefsMgr->getWaresdevIndentSpaceNb(),4);
    BOOST_CHECK(PrefsMgr->isWaresdevLineWrappingEnabled());
    BOOST_CHECK(PrefsMgr->isWaresdevSpaceTabDisplayEnabled());
    BOOST_CHECK(!PrefsMgr->isWaresdevCarriageReturnDisplayEnabled());
    BOOST_CHECK(PrefsMgr->isWaresdevCurrentlineHighlightingEnabled());
    BOOST_CHECK_EQUAL(PrefsMgr->getWaresdevCurrentlineColor(),"#d9e7ff");    
    BOOST_CHECK(PrefsMgr->isWaresdevSyntaxHighlightingEnabled());
    auto Rules = PrefsMgr->getWaresdevSyntaxHighlightingRules();
    BOOST_CHECK_EQUAL(Rules.size(),10);
    BOOST_REQUIRE(Rules.find("openfluid-keyword") != Rules.end());
    BOOST_REQUIRE(Rules.find("invisible") != Rules.end());
    BOOST_CHECK(Rules.find("does-not-exist") == Rules.end());
    BOOST_CHECK_EQUAL(Rules.at("openfluid-keyword").Color,"#546f02");
    BOOST_CHECK_EQUAL(Rules.at("openfluid-keyword").Decoration.size(),1);
    BOOST_CHECK_EQUAL(Rules.at("openfluid-keyword").Decoration[0],"bold");
    BOOST_CHECK_EQUAL(Rules.at("invisible").Color,"#aaaaaa");
  }
  
  { // waresdev commands options
    BOOST_CHECK_EQUAL(PrefsMgr->getWaresdevConfigureGenerator(),"");
    BOOST_CHECK_EQUAL(PrefsMgr->getWaresdevConfigureOptions(),"-DOPTION_FOR_CMAKE=ValueForCMake");
    BOOST_CHECK_EQUAL(PrefsMgr->getWaresdevConfigureEnv("PATH"),"$PATH:/path/to/config/bin");
    BOOST_CHECK_EQUAL(PrefsMgr->getWaresdevConfigureEnv("FAKE"),"");
    BOOST_CHECK_EQUAL(PrefsMgr->getWaresdevBuildEnv("PATH"),"$PATH:/path/to/build/bin");
    BOOST_CHECK_EQUAL(PrefsMgr->getWaresdevBuildEnv("FAKE"),"");
    BOOST_CHECK(PrefsMgr->isWaresdevShowCommandEnv("PATH"));
    BOOST_CHECK(!PrefsMgr->isWaresdevShowCommandEnv("FAKE"));
    BOOST_CHECK(!PrefsMgr->isWaresdevGitSslNoVerify());
  }

  { // waresdev external tools
    auto ExtTools = PrefsMgr->getWaresdevExternalTools();
    BOOST_REQUIRE_EQUAL(ExtTools.size(),3);
    
    for (const auto& T : ExtTools)
    {
      if (T.Name == "My Editor")
      {
        BOOST_CHECK_EQUAL(T.Commands.size(),1);
        BOOST_CHECK_EQUAL(T.Commands.at(openfluid::base::PreferencesManager::ExternalToolContext::FILE),
                          "/usr/local/bin/myeditor %%C%%");
      }
      else if (T.Name == "SecretTool")
      {
        BOOST_CHECK_EQUAL(T.Commands.size(),2);
        BOOST_CHECK_EQUAL(T.Commands.at(openfluid::base::PreferencesManager::ExternalToolContext::WARE),
                          "\\\"C:\\Programs\\tool.exe\\\"");
        BOOST_CHECK_EQUAL(T.Commands.at(openfluid::base::PreferencesManager::ExternalToolContext::FILE),
                          "\\\"C:\\Programs\\tool.exe\\\" %%C%%");
      }
      else if (T.Name == "IDE")
      {
        BOOST_CHECK_EQUAL(T.Commands.size(),1);
        BOOST_CHECK_EQUAL(T.Commands.at(openfluid::base::PreferencesManager::ExternalToolContext::WORKSPACE),
                          "\\\"/Applications/Integrated Editor.app\\\" %%W%%");
      }
      else
      {
        BOOST_FAIL("wrong external tool");
      }
    }

  }

  { // waresdev hub
    BOOST_CHECK_EQUAL(PrefsMgr->getWaresdevImportHubUsername(),"johndoe");
    BOOST_CHECK_EQUAL(PrefsMgr->getWaresdevImportHubUrl(),"https://hub.dummy.org/dummy-wareshub/api");
  }

  openfluid::base::PreferencesManager::kill();
}


// =====================================================================
// =====================================================================


BOOST_AUTO_TEST_CASE(check_other)
{
  { // available langs
    auto Langs = openfluid::base::PreferencesManager::getAvailableUILanguages();
    BOOST_REQUIRE_EQUAL(Langs.size(),1);
    BOOST_CHECK_EQUAL(Langs[0],"fr_FR");

    BOOST_CHECK(openfluid::base::PreferencesManager::isUILanguageAvailable("fr_FR"));
    BOOST_CHECK(!openfluid::base::PreferencesManager::isUILanguageAvailable("es_ES"));
  }
}

