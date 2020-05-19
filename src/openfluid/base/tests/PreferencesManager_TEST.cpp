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
#define BOOST_TEST_MODULE unittest_PreferencesManager


#include <boost/test/unit_test.hpp>

#include <QString>
#include <QStringList>
#include <QFile>

#include <openfluid/base/PreferencesManager.hpp>
#include <openfluid/config.hpp>
#include <openfluid/base/FrameworkException.hpp>
#include <openfluid/base/Environment.hpp>

#include "tests-config.hpp"


// =====================================================================
// =====================================================================


BOOST_AUTO_TEST_CASE(test_SetFileName)
{
  QString CFile = QString(CONFIGTESTS_OUTPUT_DATA_DIR.c_str()) + "/" +
                  QString(openfluid::config::DEFAULT_CONFIGFILE.c_str());
  openfluid::base::PreferencesManager* PrefMgr = openfluid::base::PreferencesManager::instance();

  BOOST_CHECK_THROW(openfluid::base::PreferencesManager::setFileName(CFile),openfluid::base::FrameworkException);

  BOOST_CHECK(PrefMgr->getFileName().toStdString() == openfluid::base::Environment::getConfigFile());

  openfluid::base::PreferencesManager::kill();

  openfluid::base::PreferencesManager::setFileName(CFile);
  PrefMgr = openfluid::base::PreferencesManager::instance();

  BOOST_CHECK(PrefMgr->getFileName() == CFile);

  openfluid::base::PreferencesManager::kill();

}


// =====================================================================
// =====================================================================


BOOST_AUTO_TEST_CASE(test_SetSimpleValues)
{
  QString CFile = QString(CONFIGTESTS_OUTPUT_DATA_DIR.c_str()) + "/" +
                  QString(openfluid::config::DEFAULT_CONFIGFILE.c_str());
  if (QFile::exists(CFile))
  {
    QFile::remove(CFile);
  }

  openfluid::base::PreferencesManager::setFileName(CFile);
  openfluid::base::PreferencesManager* PrefMgr = openfluid::base::PreferencesManager::instance();


  QStringList WksPaths;
  WksPaths.append("aa/bb/cc");
  WksPaths.append("ii/jj/kk");
  WksPaths.append("xx/yy/zz");

  PrefMgr->setLang("oc");
  PrefMgr->setBuilderRecentMax(10);
  PrefMgr->setBuilderWorkspacesPaths(WksPaths);
  PrefMgr->setBuilderDeltaT(777);
  PrefMgr->setBuilderBegin("2222-11-11T00:11:22");
  PrefMgr->setBuilderEnd("2221-12-12=11;22;33");

  BOOST_CHECK_EQUAL(PrefMgr->getBuilderRecentMax(),10);
  BOOST_CHECK(PrefMgr->getLang() == "oc");
  BOOST_CHECK_EQUAL(PrefMgr->getBuilderWorkspacesPaths().count(),3);
  BOOST_CHECK(PrefMgr->getBuilderWorkspacePath() == "aa/bb/cc");
  BOOST_CHECK(PrefMgr->getBuilderWorkspacesPaths()[0] == "aa/bb/cc");
  BOOST_CHECK(PrefMgr->getBuilderWorkspacesPaths()[2] == "xx/yy/zz");
  BOOST_CHECK(PrefMgr->getBuilderProjectsPath() == "aa/bb/cc/projects");
  BOOST_CHECK_EQUAL(PrefMgr->getBuilderDeltaT(),777);
  BOOST_CHECK(PrefMgr->getBuilderBegin() == "2222-11-11T00:11:22");
  BOOST_CHECK(PrefMgr->getBuilderEnd() == "2221-12-12=11;22;33");

  openfluid::base::PreferencesManager::kill();
}


// =====================================================================
// =====================================================================


BOOST_AUTO_TEST_CASE(test_RecentProjectsManagement)
{
  QString CFile = QString(CONFIGTESTS_OUTPUT_DATA_DIR.c_str()) + "/" +
                  QString(openfluid::config::DEFAULT_CONFIGFILE.c_str());
  if (QFile::exists(CFile))
  {
    QFile::remove(CFile);
  }

  openfluid::base::PreferencesManager::setFileName(CFile);
  openfluid::base::PreferencesManager* PrefMgr = openfluid::base::PreferencesManager::instance();

  BOOST_CHECK_EQUAL(PrefMgr->getBuilderRecentMax(),5);
  BOOST_CHECK_EQUAL(PrefMgr->getBuilderRecentProjects().size(),0);

  BOOST_REQUIRE(PrefMgr->addBuilderRecentProject("pj1","aa/bb/file1.txt"));
  BOOST_REQUIRE(PrefMgr->addBuilderRecentProject("","aa/bb/file2.txt"));
  BOOST_REQUIRE(PrefMgr->addBuilderRecentProject("pj3","aa/bb/file3.txt"));

  openfluid::base::PreferencesManager::RecentProjectsList_t Recents = PrefMgr->getBuilderRecentProjects();

  BOOST_CHECK_EQUAL(Recents.size(),3);
  BOOST_CHECK(Recents[0].Path == "aa/bb/file3.txt");
  BOOST_CHECK(Recents[0].Name == "pj3");
  BOOST_CHECK(Recents[1].Path == "aa/bb/file2.txt");
  BOOST_CHECK(Recents[1].Name == "");
  BOOST_CHECK(Recents[2].Path == "aa/bb/file1.txt");
  BOOST_CHECK(Recents[2].Name == "pj1");

  PrefMgr->addBuilderRecentProject("","aa/bb/file2.txt");

  Recents = PrefMgr->getBuilderRecentProjects();

  BOOST_CHECK_EQUAL(Recents.size(),3);
  BOOST_CHECK(Recents[0].Path == "aa/bb/file2.txt");
  BOOST_CHECK(Recents[0].Name == "");
  BOOST_CHECK(Recents[1].Path == "aa/bb/file3.txt");
  BOOST_CHECK(Recents[1].Name == "pj3");
  BOOST_CHECK(Recents[2].Path == "aa/bb/file1.txt");
  BOOST_CHECK(Recents[2].Name == "pj1");

  PrefMgr->addBuilderRecentProject("","aa/bb/file20.txt");
  PrefMgr->addBuilderRecentProject("","aa/bb/file21.txt");
  PrefMgr->addBuilderRecentProject("","aa/bb/file22.txt");
  PrefMgr->addBuilderRecentProject("","aa/bb/file23.txt");

  Recents = PrefMgr->getBuilderRecentProjects();

  BOOST_CHECK_EQUAL(PrefMgr->getBuilderRecentMax(),Recents.size());

  PrefMgr->clearBuilderRecentProjects();

  Recents = PrefMgr->getBuilderRecentProjects();

  BOOST_CHECK_EQUAL(Recents.size(),0);

  openfluid::base::PreferencesManager::kill();
}


// =====================================================================
// =====================================================================


BOOST_AUTO_TEST_CASE(test_ExtraSimPathManagement)
{
  QString CFile = QString(CONFIGTESTS_OUTPUT_DATA_DIR.c_str()) + "/" +
                  QString(openfluid::config::DEFAULT_CONFIGFILE.c_str());
  if (QFile::exists(CFile))
  {
    QFile::remove(CFile);
  }

  openfluid::base::PreferencesManager::setFileName(CFile);
  openfluid::base::PreferencesManager* PrefMgr = openfluid::base::PreferencesManager::instance();


  QStringList ExtraPaths = PrefMgr->getBuilderExtraSimulatorsPaths();

  BOOST_CHECK_EQUAL(ExtraPaths.size(),0);

  PrefMgr->addBuilderExtraSimulatorsPath("aa/bb/dir1");
  PrefMgr->addBuilderExtraSimulatorsPath("aa/bb/dir2");
  PrefMgr->addBuilderExtraSimulatorsPath("aa/bb/dir3");

  ExtraPaths = PrefMgr->getBuilderExtraSimulatorsPaths();

  BOOST_CHECK_EQUAL(ExtraPaths.size(),3);
  BOOST_CHECK(ExtraPaths[0] == "aa/bb/dir1");
  BOOST_CHECK(ExtraPaths[1] == "aa/bb/dir2");
  BOOST_CHECK(ExtraPaths[2] == "aa/bb/dir3");

  PrefMgr->removeBuilderExtraSimulatorsPath("aa/bb/dir2");

  ExtraPaths = PrefMgr->getBuilderExtraSimulatorsPaths();

  BOOST_CHECK_EQUAL(ExtraPaths.size(),2);
  BOOST_CHECK(ExtraPaths[0] == "aa/bb/dir1");
  BOOST_CHECK(ExtraPaths[1] == "aa/bb/dir3");

  PrefMgr->removeBuilderExtraSimulatorsPath("aa/bb/wrongdir");

  ExtraPaths = PrefMgr->getBuilderExtraSimulatorsPaths();

  BOOST_CHECK_EQUAL(ExtraPaths.size(),2);
  BOOST_CHECK(ExtraPaths[0] == "aa/bb/dir1");
  BOOST_CHECK(ExtraPaths[1] == "aa/bb/dir3");

  PrefMgr->removeBuilderExtraSimulatorsPath("aa/bb/dir1");

  ExtraPaths = PrefMgr->getBuilderExtraSimulatorsPaths();

  BOOST_CHECK_EQUAL(ExtraPaths.size(),1);
  BOOST_CHECK(ExtraPaths[0] == "aa/bb/dir3");

  PrefMgr->removeBuilderExtraSimulatorsPath("aa/bb/dir3");

  ExtraPaths = PrefMgr->getBuilderExtraSimulatorsPaths();

  BOOST_CHECK_EQUAL(ExtraPaths.size(),0);

  QStringList Paths;
  Paths.append("aa/bb/dir1");
  Paths.append("aa/bb/dir2");
  Paths.append("aa/bb/dir3");

  PrefMgr->setBuilderExtraSimulatorsPaths(Paths);

  ExtraPaths = PrefMgr->getBuilderExtraSimulatorsPaths();

  BOOST_CHECK_EQUAL(ExtraPaths.size(),3);
  BOOST_CHECK(ExtraPaths[0] == "aa/bb/dir1");
  BOOST_CHECK(ExtraPaths[1] == "aa/bb/dir2");
  BOOST_CHECK(ExtraPaths[2] == "aa/bb/dir3");

  Paths.clear();
  Paths.push_back("cc/dd/dir1");
  Paths.push_back("cc/dd/dir2");

  PrefMgr->setBuilderExtraSimulatorsPaths(Paths);

  ExtraPaths = PrefMgr->getBuilderExtraSimulatorsPaths();

  BOOST_CHECK_EQUAL(ExtraPaths.size(),2);
  BOOST_CHECK(ExtraPaths[0] == "cc/dd/dir1");
  BOOST_CHECK(ExtraPaths[1] == "cc/dd/dir2");

  openfluid::base::PreferencesManager::kill();
}


// =====================================================================
// =====================================================================


BOOST_AUTO_TEST_CASE(test_ExtraExtensionPathManagement)
{
  QString CFile = QString(CONFIGTESTS_OUTPUT_DATA_DIR.c_str()) + "/" +
                  QString(openfluid::config::DEFAULT_CONFIGFILE.c_str());
  if (QFile::exists(CFile))
  {
    QFile::remove(CFile);
  }

  openfluid::base::PreferencesManager::setFileName(CFile);
  openfluid::base::PreferencesManager* PrefMgr = openfluid::base::PreferencesManager::instance();


  QStringList ExtraPaths = PrefMgr->getBuilderExtraExtensionsPaths();

  BOOST_CHECK_EQUAL(ExtraPaths.size(),0);

  PrefMgr->addBuilderExtraExtensionsPath("aa/bb/dir1");
  PrefMgr->addBuilderExtraExtensionsPath("aa/bb/dir2");
  PrefMgr->addBuilderExtraExtensionsPath("aa/bb/dir3");

  ExtraPaths = PrefMgr->getBuilderExtraExtensionsPaths();

  BOOST_CHECK_EQUAL(ExtraPaths.size(),3);
  BOOST_CHECK(ExtraPaths[0] == "aa/bb/dir1");
  BOOST_CHECK(ExtraPaths[1] == "aa/bb/dir2");
  BOOST_CHECK(ExtraPaths[2] == "aa/bb/dir3");

  PrefMgr->removeBuilderExtraExtensionsPath("aa/bb/dir2");

  ExtraPaths = PrefMgr->getBuilderExtraExtensionsPaths();

  BOOST_CHECK_EQUAL(ExtraPaths.size(),2);
  BOOST_CHECK(ExtraPaths[0] == "aa/bb/dir1");
  BOOST_CHECK(ExtraPaths[1] == "aa/bb/dir3");

  PrefMgr->removeBuilderExtraExtensionsPath("aa/bb/wrongdir");

  ExtraPaths = PrefMgr->getBuilderExtraExtensionsPaths();

  BOOST_CHECK_EQUAL(ExtraPaths.size(),2);
  BOOST_CHECK(ExtraPaths[0] == "aa/bb/dir1");
  BOOST_CHECK(ExtraPaths[1] == "aa/bb/dir3");

  PrefMgr->removeBuilderExtraExtensionsPath("aa/bb/dir1");

  ExtraPaths = PrefMgr->getBuilderExtraExtensionsPaths();

  BOOST_CHECK_EQUAL(ExtraPaths.size(),1);
  BOOST_CHECK(ExtraPaths[0] == "aa/bb/dir3");

  PrefMgr->removeBuilderExtraExtensionsPath("aa/bb/dir3");

  ExtraPaths = PrefMgr->getBuilderExtraExtensionsPaths();

  BOOST_CHECK_EQUAL(ExtraPaths.size(),0);

  QStringList Paths;
  Paths.append("aa/bb/dir1");
  Paths.append("aa/bb/dir2");
  Paths.append("aa/bb/dir3");

  PrefMgr->setBuilderExtraExtensionsPaths(Paths);

  ExtraPaths = PrefMgr->getBuilderExtraExtensionsPaths();

  BOOST_CHECK_EQUAL(ExtraPaths.size(),3);
  BOOST_CHECK(ExtraPaths[0] == "aa/bb/dir1");
  BOOST_CHECK(ExtraPaths[1] == "aa/bb/dir2");
  BOOST_CHECK(ExtraPaths[2] == "aa/bb/dir3");

  Paths.clear();
  Paths.push_back("cc/dd/dir1");
  Paths.push_back("cc/dd/dir2");

  PrefMgr->setBuilderExtraExtensionsPaths(Paths);

  ExtraPaths = PrefMgr->getBuilderExtraExtensionsPaths();

  BOOST_CHECK_EQUAL(ExtraPaths.size(),2);
  BOOST_CHECK(ExtraPaths[0] == "cc/dd/dir1");
  BOOST_CHECK(ExtraPaths[1] == "cc/dd/dir2");

  openfluid::base::PreferencesManager::kill();
}


// =====================================================================
// =====================================================================


BOOST_AUTO_TEST_CASE(test_ExtraObserverPathManagement)
{
  QString CFile = QString(CONFIGTESTS_OUTPUT_DATA_DIR.c_str()) + "/" +
                  QString(openfluid::config::DEFAULT_CONFIGFILE.c_str());
  if (QFile::exists(CFile))
  {
    QFile::remove(CFile);
  }

  openfluid::base::PreferencesManager::setFileName(CFile);
  openfluid::base::PreferencesManager* PrefMgr = openfluid::base::PreferencesManager::instance();


  QStringList ExtraPaths = PrefMgr->getBuilderExtraObserversPaths();

  BOOST_CHECK_EQUAL(ExtraPaths.size(),0);

  PrefMgr->addBuilderExtraObserversPath("aa/bb/dir1");
  PrefMgr->addBuilderExtraObserversPath("aa/bb/dir2");
  PrefMgr->addBuilderExtraObserversPath("aa/bb/dir3");

  ExtraPaths = PrefMgr->getBuilderExtraObserversPaths();

  BOOST_CHECK_EQUAL(ExtraPaths.size(),3);
  BOOST_CHECK(ExtraPaths[0] == "aa/bb/dir1");
  BOOST_CHECK(ExtraPaths[1] == "aa/bb/dir2");
  BOOST_CHECK(ExtraPaths[2] == "aa/bb/dir3");

  PrefMgr->removeBuilderExtraObserversPath("aa/bb/dir2");

  ExtraPaths = PrefMgr->getBuilderExtraObserversPaths();

  BOOST_CHECK_EQUAL(ExtraPaths.size(),2);
  BOOST_CHECK(ExtraPaths[0] == "aa/bb/dir1");
  BOOST_CHECK(ExtraPaths[1] == "aa/bb/dir3");

  PrefMgr->removeBuilderExtraObserversPath("aa/bb/wrongdir");

  ExtraPaths = PrefMgr->getBuilderExtraObserversPaths();

  BOOST_CHECK_EQUAL(ExtraPaths.size(),2);
  BOOST_CHECK(ExtraPaths[0] == "aa/bb/dir1");
  BOOST_CHECK(ExtraPaths[1] == "aa/bb/dir3");

  PrefMgr->removeBuilderExtraObserversPath("aa/bb/dir1");

  ExtraPaths = PrefMgr->getBuilderExtraObserversPaths();

  BOOST_CHECK_EQUAL(ExtraPaths.size(),1);
  BOOST_CHECK(ExtraPaths[0] == "aa/bb/dir3");

  PrefMgr->removeBuilderExtraObserversPath("aa/bb/dir3");

  ExtraPaths = PrefMgr->getBuilderExtraObserversPaths();

  BOOST_CHECK_EQUAL(ExtraPaths.size(),0);

  QStringList Paths;
  Paths.append("aa/bb/dir1");
  Paths.append("aa/bb/dir2");
  Paths.append("aa/bb/dir3");

  PrefMgr->setBuilderExtraObserversPaths(Paths);

  ExtraPaths = PrefMgr->getBuilderExtraObserversPaths();

  BOOST_CHECK_EQUAL(ExtraPaths.size(),3);
  BOOST_CHECK(ExtraPaths[0] == "aa/bb/dir1");
  BOOST_CHECK(ExtraPaths[1] == "aa/bb/dir2");
  BOOST_CHECK(ExtraPaths[2] == "aa/bb/dir3");

  Paths.clear();
  Paths.push_back("cc/dd/dir1");
  Paths.push_back("cc/dd/dir2");

  PrefMgr->setBuilderExtraObserversPaths(Paths);

  ExtraPaths = PrefMgr->getBuilderExtraObserversPaths();

  BOOST_CHECK_EQUAL(ExtraPaths.size(),2);
  BOOST_CHECK(ExtraPaths[0] == "cc/dd/dir1");
  BOOST_CHECK(ExtraPaths[1] == "cc/dd/dir2");

  openfluid::base::PreferencesManager::kill();
}


// =====================================================================
// =====================================================================


BOOST_AUTO_TEST_CASE(test_MarketplacesManagement)
{
  QString CFile = QString(CONFIGTESTS_OUTPUT_DATA_DIR.c_str()) + "/" +
                  QString(openfluid::config::DEFAULT_CONFIGFILE.c_str());
  if (QFile::exists(CFile))
  {
    QFile::remove(CFile);
  }

  openfluid::base::PreferencesManager::setFileName(CFile);
  openfluid::base::PreferencesManager* PrefMgr = openfluid::base::PreferencesManager::instance();


  openfluid::base::PreferencesManager::MarketPlaces_t Places = PrefMgr->getMarketplaces();

  BOOST_CHECK_EQUAL(Places.size(),0);

  PrefMgr->addMarketplace("PlaceA", "http://aa/aa/");
  PrefMgr->addMarketplace("Place with spaces", "http://bb/bb/");
  PrefMgr->addMarketplace("Place C", "http://cc/cc/");

  Places = PrefMgr->getMarketplaces();

  BOOST_CHECK_EQUAL(Places.size(),3);
  BOOST_CHECK(Places["PlaceA"] == "http://aa/aa/");
  BOOST_CHECK(Places["Place with spaces"] == "http://bb/bb/");
  BOOST_CHECK(Places["Place C"] == "http://cc/cc/");

  PrefMgr->removeMarketplace("Place with spaces");

  Places = PrefMgr->getMarketplaces();

  BOOST_CHECK_EQUAL(Places.size(),2);
  BOOST_CHECK(Places["PlaceA"] == "http://aa/aa/");
  BOOST_CHECK(Places["Place C"] == "http://cc/cc/");

  PrefMgr->removeMarketplace("Wrong place");

  Places = PrefMgr->getMarketplaces();

  BOOST_CHECK_EQUAL(Places.size(),2);
  BOOST_CHECK(Places["PlaceA"] == "http://aa/aa/");
  BOOST_CHECK(Places["Place C"] == "http://cc/cc/");

  PrefMgr->removeMarketplace("PlaceA");
  PrefMgr->removeMarketplace("PlaceC");

  BOOST_CHECK_EQUAL(Places.size(),2);

  BOOST_CHECK_EQUAL(PrefMgr->addMarketplace("Place=A", "http://aa/aa/"), false);

  PrefMgr->addMarketplace("Place;A", "http://aa/aa/");

  Places = PrefMgr->getMarketplaces();
  BOOST_CHECK(Places["Place;A"] == "http://aa/aa/");

  openfluid::base::PreferencesManager::kill();
}


// =====================================================================
// =====================================================================


BOOST_AUTO_TEST_CASE(test_DockToolbarPositionsManagement)
{
  QString CFile = QString(CONFIGTESTS_OUTPUT_DATA_DIR.c_str()) + "/" +
                  QString(openfluid::config::DEFAULT_CONFIGFILE.c_str());
  if (QFile::exists(CFile))
  {
    QFile::remove(CFile);
  }

  openfluid::base::PreferencesManager::setFileName(CFile);
  openfluid::base::PreferencesManager* PrefMgr = openfluid::base::PreferencesManager::instance();

  PrefMgr->setBuilderToolBarPosition(Qt::LeftToolBarArea);
  PrefMgr->setBuilderDockPosition(Qt::BottomDockWidgetArea);
  BOOST_CHECK_EQUAL(PrefMgr->getBuilderToolBarPosition(),Qt::LeftToolBarArea);
  BOOST_CHECK_EQUAL(PrefMgr->getBuilderDockPosition(),Qt::BottomDockWidgetArea);

  PrefMgr->setBuilderToolBarPosition(Qt::BottomToolBarArea);
  PrefMgr->setBuilderDockPosition(Qt::RightDockWidgetArea);
  BOOST_CHECK_EQUAL(PrefMgr->getBuilderToolBarPosition(),Qt::BottomToolBarArea);
  BOOST_CHECK_EQUAL(PrefMgr->getBuilderDockPosition(),Qt::RightDockWidgetArea);

  openfluid::base::PreferencesManager::kill();
}


// =====================================================================
// =====================================================================


BOOST_AUTO_TEST_CASE(test_textEditorProperties)
{
  QString CFile = QString(CONFIGTESTS_OUTPUT_DATA_DIR.c_str()) + "/" +
                  QString(openfluid::config::DEFAULT_CONFIGFILE.c_str());
  if (QFile::exists(CFile))
  {
    QFile::remove(CFile);
  }

  openfluid::base::PreferencesManager::setFileName(CFile);
  openfluid::base::PreferencesManager* PrefMgr = openfluid::base::PreferencesManager::instance();

  openfluid::base::PreferencesManager::SyntaxHighlightingRules_t Rules;
  openfluid::base::PreferencesManager::SyntaxHighlightingRule_t Rule(QString("pink"),QStringList("none"));
  Rules.insert("datatype",Rule);
  openfluid::base::PreferencesManager::SyntaxHighlightingRule_t ControlRule(QString("controlColor"),
                                                                            QStringList("none"));

  BOOST_CHECK_EQUAL(PrefMgr->isWaresdevSyntaxHighlightingEnabled(),true);
  BOOST_CHECK_EQUAL(
    PrefMgr->getWaresdevSyntaxHighlightingRules().value("datatype",ControlRule).m_Color.toStdString(),"system");

  PrefMgr->setWaresdevSyntaxHighlightingEnabled(false);
  PrefMgr->setWaresdevSyntaxHighlightingRules(Rules);
  BOOST_CHECK_EQUAL(PrefMgr->isWaresdevSyntaxHighlightingEnabled(),false);
  BOOST_CHECK_EQUAL(
    PrefMgr->getWaresdevSyntaxHighlightingRules().value("datatype",ControlRule).m_Color.toStdString(),"pink");

  PrefMgr->setWaresdevTextEditorDefaults(true);
  BOOST_CHECK_EQUAL(PrefMgr->isWaresdevSyntaxHighlightingEnabled(),true);
  BOOST_CHECK_EQUAL(
    PrefMgr->getWaresdevSyntaxHighlightingRules().value("datatype",ControlRule).m_Color.toStdString(),"system");

  openfluid::base::PreferencesManager::kill();
}


// =====================================================================
// =====================================================================


BOOST_AUTO_TEST_CASE(test_externalToolsProperties)
{
  QString CFile = QString(CONFIGTESTS_OUTPUT_DATA_DIR.c_str()) + "/" +
                  QString(openfluid::config::DEFAULT_CONFIGFILE.c_str());
  if (QFile::exists(CFile))
  {
    QFile::remove(CFile);
  }

  openfluid::base::PreferencesManager::setFileName(CFile);
  openfluid::base::PreferencesManager* PrefMgr = openfluid::base::PreferencesManager::instance();

  openfluid::base::PreferencesManager::ExternalToolsCommands_t Tools;
  QStringList ToolA, ToolB, ToolC, ToolD;
  ToolA << "A %%W%%" << "AB %%S%%" << "AC %%C%%";
  Tools.insert("A", ToolA);
  ToolB << "" << "B %%S%%" << "";
  Tools.insert("B", ToolB);
  ToolC << "C %%W%% -i" << "C -i %%S%% -j" << "";
  Tools.insert("C", ToolC);

  PrefMgr->setWaresdevExternalToolsCommands(Tools);

  BOOST_CHECK_EQUAL(
    PrefMgr->getWaresdevExternalToolsCommandsInContext(
      openfluid::base::PreferencesManager::ExternalToolContext::WORKSPACE)["A"].toStdString(),"A %%P%%");

  BOOST_CHECK_EQUAL(
    PrefMgr->getWaresdevExternalToolsCommandsInContext(
      openfluid::base::PreferencesManager::ExternalToolContext::WARE)["A"].toStdString(),"AB %%P%%");

  BOOST_CHECK_EQUAL(
    PrefMgr->getWaresdevExternalToolsCommandsInContext(
      openfluid::base::PreferencesManager::ExternalToolContext::FILE)["A"].toStdString(),"AC %%P%%");

  BOOST_CHECK(
    !PrefMgr->getWaresdevExternalToolsCommandsInContext(
      openfluid::base::PreferencesManager::ExternalToolContext::WORKSPACE).contains("B"));

  BOOST_CHECK_EQUAL(
    PrefMgr->getWaresdevExternalToolsCommandsInContext(
      openfluid::base::PreferencesManager::ExternalToolContext::WARE)["C"].toStdString(),"C -i %%P%% -j");

  openfluid::base::PreferencesManager::kill();
}
