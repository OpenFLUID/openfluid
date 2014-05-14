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
 \file PreferencesManager_TEST.cpp
 \brief Implements ...

 \author Aline LIBRES <libres@supagro.inra.fr>
 */

#define BOOST_TEST_MAIN
#define BOOST_AUTO_TEST_MAIN
#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MODULE unittest_PreferencesManager
#include <boost/test/unit_test.hpp>
#include <QString>
#include <QStringList>
#include <QFile>

#include <openfluid/guicommon/PreferencesManager.hpp>

#include <openfluid/config.hpp>
#include <boost/filesystem.hpp>
#include "tests-config.hpp"
#include <openfluid/base/FrameworkException.hpp>
#include <openfluid/base/RuntimeEnv.hpp>

// =====================================================================
// =====================================================================

BOOST_AUTO_TEST_CASE(test_SetFileName)
{
  QString CFile = QString(CONFIGTESTS_OUTPUT_DATA_DIR.c_str()) + "/" + QString(openfluid::config::DEFAULT_CONFIGFILE.c_str());
  openfluid::guicommon::PreferencesManager* PrefMgr = openfluid::guicommon::PreferencesManager::getInstance();

  BOOST_CHECK_THROW(openfluid::guicommon::PreferencesManager::setFileName(CFile),openfluid::base::FrameworkException);

  // TODO to reactivate once the qt suffix will be removed form file name
//  BOOST_CHECK(PrefMgr->getFileName().toStdString() == openfluid::base::RuntimeEnvironment::getInstance()->getDefaultConfigFile());

  delete PrefMgr;

  openfluid::guicommon::PreferencesManager::setFileName(CFile);
  PrefMgr = openfluid::guicommon::PreferencesManager::getInstance();

  BOOST_CHECK(PrefMgr->getFileName() == CFile);

  delete PrefMgr;

}

// =====================================================================
// =====================================================================

BOOST_AUTO_TEST_CASE(test_SetSimpleValues)
{
  QString CFile = QString(CONFIGTESTS_OUTPUT_DATA_DIR.c_str()) + "/" + QString(openfluid::config::DEFAULT_CONFIGFILE.c_str());
  if (QFile::exists(CFile)) QFile::remove(CFile);

  openfluid::guicommon::PreferencesManager::setFileName(CFile);
  openfluid::guicommon::PreferencesManager* PrefMgr = openfluid::guicommon::PreferencesManager::getInstance();


  PrefMgr->setLang("oc");
  PrefMgr->setRecentMax(10);
  PrefMgr->setWorkdir("aa/bb/cc");
  PrefMgr->setDeltaT(777);
  PrefMgr->setBegin("2222-11-11T00:11:22");
  PrefMgr->setEnd("2221-12-12=11;22;33");

  BOOST_CHECK_EQUAL(PrefMgr->getRecentMax(),10);
  BOOST_CHECK(PrefMgr->getLang() == "oc");
  BOOST_CHECK(PrefMgr->getWorkdir() == "aa/bb/cc");
  BOOST_CHECK_EQUAL(PrefMgr->getDeltaT(),777);
  BOOST_CHECK(PrefMgr->getBegin() == "2222-11-11T00:11:22");
  BOOST_CHECK(PrefMgr->getEnd() == "2221-12-12=11;22;33");

  delete PrefMgr;
}

// =====================================================================
// =====================================================================

BOOST_AUTO_TEST_CASE(test_RecentProjectsManagement)
{
  QString CFile = QString(CONFIGTESTS_OUTPUT_DATA_DIR.c_str()) + "/" + QString(openfluid::config::DEFAULT_CONFIGFILE.c_str());
  if (QFile::exists(CFile)) QFile::remove(CFile);

  openfluid::guicommon::PreferencesManager::setFileName(CFile);
  openfluid::guicommon::PreferencesManager* PrefMgr = openfluid::guicommon::PreferencesManager::getInstance();

  BOOST_CHECK_EQUAL(PrefMgr->getRecentMax(),5);
  BOOST_CHECK_EQUAL(PrefMgr->getRecentProjects().size(),0);

  BOOST_REQUIRE(PrefMgr->addRecentProject("pj1","aa/bb/file1.txt"));
  BOOST_REQUIRE(PrefMgr->addRecentProject("","aa/bb/file2.txt"));
  BOOST_REQUIRE(PrefMgr->addRecentProject("pj3","aa/bb/file3.txt"));

  openfluid::guicommon::PreferencesManager::RecentProjectsList_t Recents = PrefMgr->getRecentProjects();

  BOOST_CHECK_EQUAL(Recents.size(),3);
  BOOST_CHECK(Recents[0].Path == "aa/bb/file3.txt");
  BOOST_CHECK(Recents[0].Name == "pj3");
  BOOST_CHECK(Recents[1].Path == "aa/bb/file2.txt");
  BOOST_CHECK(Recents[1].Name == "");
  BOOST_CHECK(Recents[2].Path == "aa/bb/file1.txt");
  BOOST_CHECK(Recents[2].Name == "pj1");

  PrefMgr->addRecentProject("","aa/bb/file2.txt");

  Recents = PrefMgr->getRecentProjects();

  BOOST_CHECK_EQUAL(Recents.size(),3);
  BOOST_CHECK(Recents[0].Path == "aa/bb/file2.txt");
  BOOST_CHECK(Recents[0].Name == "");
  BOOST_CHECK(Recents[1].Path == "aa/bb/file3.txt");
  BOOST_CHECK(Recents[1].Name == "pj3");
  BOOST_CHECK(Recents[2].Path == "aa/bb/file1.txt");
  BOOST_CHECK(Recents[2].Name == "pj1");

  PrefMgr->addRecentProject("","aa/bb/file20.txt");
  PrefMgr->addRecentProject("","aa/bb/file21.txt");
  PrefMgr->addRecentProject("","aa/bb/file22.txt");
  PrefMgr->addRecentProject("","aa/bb/file23.txt");

  Recents = PrefMgr->getRecentProjects();

  BOOST_CHECK_EQUAL(PrefMgr->getRecentMax(),Recents.size());

  PrefMgr->clearRecentProjects();

  Recents = PrefMgr->getRecentProjects();

  BOOST_CHECK_EQUAL(Recents.size(),0);

  delete PrefMgr;
}


// =====================================================================
// =====================================================================


BOOST_AUTO_TEST_CASE(test_ExtraSimPathManagement)
{
  QString CFile = QString(CONFIGTESTS_OUTPUT_DATA_DIR.c_str()) + "/" + QString(openfluid::config::DEFAULT_CONFIGFILE.c_str());
  if (QFile::exists(CFile)) QFile::remove(CFile);

  openfluid::guicommon::PreferencesManager::setFileName(CFile);
  openfluid::guicommon::PreferencesManager* PrefMgr = openfluid::guicommon::PreferencesManager::getInstance();


  QStringList ExtraPaths = PrefMgr->getExtraSimulatorsPaths();

  BOOST_CHECK_EQUAL(ExtraPaths.size(),0);

  PrefMgr->addExtraSimulatorsPath("aa/bb/dir1");
  PrefMgr->addExtraSimulatorsPath("aa/bb/dir2");
  PrefMgr->addExtraSimulatorsPath("aa/bb/dir3");

  ExtraPaths = PrefMgr->getExtraSimulatorsPaths();

  BOOST_CHECK_EQUAL(ExtraPaths.size(),3);
  BOOST_CHECK(ExtraPaths[0] == "aa/bb/dir1");
  BOOST_CHECK(ExtraPaths[1] == "aa/bb/dir2");
  BOOST_CHECK(ExtraPaths[2] == "aa/bb/dir3");

  PrefMgr->removeExtraSimulatorsPath("aa/bb/dir2");

  ExtraPaths = PrefMgr->getExtraSimulatorsPaths();

  BOOST_CHECK_EQUAL(ExtraPaths.size(),2);
  BOOST_CHECK(ExtraPaths[0] == "aa/bb/dir1");
  BOOST_CHECK(ExtraPaths[1] == "aa/bb/dir3");

  PrefMgr->removeExtraSimulatorsPath("aa/bb/wrongdir");

  ExtraPaths = PrefMgr->getExtraSimulatorsPaths();

  BOOST_CHECK_EQUAL(ExtraPaths.size(),2);
  BOOST_CHECK(ExtraPaths[0] == "aa/bb/dir1");
  BOOST_CHECK(ExtraPaths[1] == "aa/bb/dir3");

  PrefMgr->removeExtraSimulatorsPath("aa/bb/dir1");

  ExtraPaths = PrefMgr->getExtraSimulatorsPaths();

  BOOST_CHECK_EQUAL(ExtraPaths.size(),1);
  BOOST_CHECK(ExtraPaths[0] == "aa/bb/dir3");

  PrefMgr->removeExtraSimulatorsPath("aa/bb/dir3");

  ExtraPaths = PrefMgr->getExtraSimulatorsPaths();

  BOOST_CHECK_EQUAL(ExtraPaths.size(),0);

  QStringList Paths;
  Paths.append("aa/bb/dir1");
  Paths.append("aa/bb/dir2");
  Paths.append("aa/bb/dir3");

  PrefMgr->setExtraSimulatorsPaths(Paths);

  ExtraPaths = PrefMgr->getExtraSimulatorsPaths();

  BOOST_CHECK_EQUAL(ExtraPaths.size(),3);
  BOOST_CHECK(ExtraPaths[0] == "aa/bb/dir1");
  BOOST_CHECK(ExtraPaths[1] == "aa/bb/dir2");
  BOOST_CHECK(ExtraPaths[2] == "aa/bb/dir3");

  Paths.clear();
  Paths.push_back("cc/dd/dir1");
  Paths.push_back("cc/dd/dir2");

  PrefMgr->setExtraSimulatorsPaths(Paths);

  ExtraPaths = PrefMgr->getExtraSimulatorsPaths();

  BOOST_CHECK_EQUAL(ExtraPaths.size(),2);
  BOOST_CHECK(ExtraPaths[0] == "cc/dd/dir1");
  BOOST_CHECK(ExtraPaths[1] == "cc/dd/dir2");

  delete PrefMgr;
}

// =====================================================================
// =====================================================================

BOOST_AUTO_TEST_CASE(test_ExtraExtensionPathManagement)
{
  QString CFile = QString(CONFIGTESTS_OUTPUT_DATA_DIR.c_str()) + "/" + QString(openfluid::config::DEFAULT_CONFIGFILE.c_str());
  if (QFile::exists(CFile)) QFile::remove(CFile);

  openfluid::guicommon::PreferencesManager::setFileName(CFile);
  openfluid::guicommon::PreferencesManager* PrefMgr = openfluid::guicommon::PreferencesManager::getInstance();


  QStringList ExtraPaths = PrefMgr->getExtraExtensionsPaths();

  BOOST_CHECK_EQUAL(ExtraPaths.size(),0);

  PrefMgr->addExtraExtensionsPath("aa/bb/dir1");
  PrefMgr->addExtraExtensionsPath("aa/bb/dir2");
  PrefMgr->addExtraExtensionsPath("aa/bb/dir3");

  ExtraPaths = PrefMgr->getExtraExtensionsPaths();

  BOOST_CHECK_EQUAL(ExtraPaths.size(),3);
  BOOST_CHECK(ExtraPaths[0] == "aa/bb/dir1");
  BOOST_CHECK(ExtraPaths[1] == "aa/bb/dir2");
  BOOST_CHECK(ExtraPaths[2] == "aa/bb/dir3");

  PrefMgr->removeExtraExtensionsPath("aa/bb/dir2");

  ExtraPaths = PrefMgr->getExtraExtensionsPaths();

  BOOST_CHECK_EQUAL(ExtraPaths.size(),2);
  BOOST_CHECK(ExtraPaths[0] == "aa/bb/dir1");
  BOOST_CHECK(ExtraPaths[1] == "aa/bb/dir3");

  PrefMgr->removeExtraExtensionsPath("aa/bb/wrongdir");

  ExtraPaths = PrefMgr->getExtraExtensionsPaths();

  BOOST_CHECK_EQUAL(ExtraPaths.size(),2);
  BOOST_CHECK(ExtraPaths[0] == "aa/bb/dir1");
  BOOST_CHECK(ExtraPaths[1] == "aa/bb/dir3");

  PrefMgr->removeExtraExtensionsPath("aa/bb/dir1");

  ExtraPaths = PrefMgr->getExtraExtensionsPaths();

  BOOST_CHECK_EQUAL(ExtraPaths.size(),1);
  BOOST_CHECK(ExtraPaths[0] == "aa/bb/dir3");

  PrefMgr->removeExtraExtensionsPath("aa/bb/dir3");

  ExtraPaths = PrefMgr->getExtraExtensionsPaths();

  BOOST_CHECK_EQUAL(ExtraPaths.size(),0);

  QStringList Paths;
  Paths.append("aa/bb/dir1");
  Paths.append("aa/bb/dir2");
  Paths.append("aa/bb/dir3");

  PrefMgr->setExtraExtensionsPaths(Paths);

  ExtraPaths = PrefMgr->getExtraExtensionsPaths();

  BOOST_CHECK_EQUAL(ExtraPaths.size(),3);
  BOOST_CHECK(ExtraPaths[0] == "aa/bb/dir1");
  BOOST_CHECK(ExtraPaths[1] == "aa/bb/dir2");
  BOOST_CHECK(ExtraPaths[2] == "aa/bb/dir3");

  Paths.clear();
  Paths.push_back("cc/dd/dir1");
  Paths.push_back("cc/dd/dir2");

  PrefMgr->setExtraExtensionsPaths(Paths);

  ExtraPaths = PrefMgr->getExtraExtensionsPaths();

  BOOST_CHECK_EQUAL(ExtraPaths.size(),2);
  BOOST_CHECK(ExtraPaths[0] == "cc/dd/dir1");
  BOOST_CHECK(ExtraPaths[1] == "cc/dd/dir2");

  delete PrefMgr;
}


// =====================================================================
// =====================================================================


BOOST_AUTO_TEST_CASE(test_ExtraObserverPathManagement)
{
  QString CFile = QString(CONFIGTESTS_OUTPUT_DATA_DIR.c_str()) + "/" + QString(openfluid::config::DEFAULT_CONFIGFILE.c_str());
  if (QFile::exists(CFile)) QFile::remove(CFile);

  openfluid::guicommon::PreferencesManager::setFileName(CFile);
  openfluid::guicommon::PreferencesManager* PrefMgr = openfluid::guicommon::PreferencesManager::getInstance();


  QStringList ExtraPaths = PrefMgr->getExtraObserversPaths();

  BOOST_CHECK_EQUAL(ExtraPaths.size(),0);

  PrefMgr->addExtraObserversPath("aa/bb/dir1");
  PrefMgr->addExtraObserversPath("aa/bb/dir2");
  PrefMgr->addExtraObserversPath("aa/bb/dir3");

  ExtraPaths = PrefMgr->getExtraObserversPaths();

  BOOST_CHECK_EQUAL(ExtraPaths.size(),3);
  BOOST_CHECK(ExtraPaths[0] == "aa/bb/dir1");
  BOOST_CHECK(ExtraPaths[1] == "aa/bb/dir2");
  BOOST_CHECK(ExtraPaths[2] == "aa/bb/dir3");

  PrefMgr->removeExtraObserversPath("aa/bb/dir2");

  ExtraPaths = PrefMgr->getExtraObserversPaths();

  BOOST_CHECK_EQUAL(ExtraPaths.size(),2);
  BOOST_CHECK(ExtraPaths[0] == "aa/bb/dir1");
  BOOST_CHECK(ExtraPaths[1] == "aa/bb/dir3");

  PrefMgr->removeExtraObserversPath("aa/bb/wrongdir");

  ExtraPaths = PrefMgr->getExtraObserversPaths();

  BOOST_CHECK_EQUAL(ExtraPaths.size(),2);
  BOOST_CHECK(ExtraPaths[0] == "aa/bb/dir1");
  BOOST_CHECK(ExtraPaths[1] == "aa/bb/dir3");

  PrefMgr->removeExtraObserversPath("aa/bb/dir1");

  ExtraPaths = PrefMgr->getExtraObserversPaths();

  BOOST_CHECK_EQUAL(ExtraPaths.size(),1);
  BOOST_CHECK(ExtraPaths[0] == "aa/bb/dir3");

  PrefMgr->removeExtraObserversPath("aa/bb/dir3");

  ExtraPaths = PrefMgr->getExtraObserversPaths();

  BOOST_CHECK_EQUAL(ExtraPaths.size(),0);

  QStringList Paths;
  Paths.append("aa/bb/dir1");
  Paths.append("aa/bb/dir2");
  Paths.append("aa/bb/dir3");

  PrefMgr->setExtraObserversPaths(Paths);

  ExtraPaths = PrefMgr->getExtraObserversPaths();

  BOOST_CHECK_EQUAL(ExtraPaths.size(),3);
  BOOST_CHECK(ExtraPaths[0] == "aa/bb/dir1");
  BOOST_CHECK(ExtraPaths[1] == "aa/bb/dir2");
  BOOST_CHECK(ExtraPaths[2] == "aa/bb/dir3");

  Paths.clear();
  Paths.push_back("cc/dd/dir1");
  Paths.push_back("cc/dd/dir2");

  PrefMgr->setExtraObserversPaths(Paths);

  ExtraPaths = PrefMgr->getExtraObserversPaths();

  BOOST_CHECK_EQUAL(ExtraPaths.size(),2);
  BOOST_CHECK(ExtraPaths[0] == "cc/dd/dir1");
  BOOST_CHECK(ExtraPaths[1] == "cc/dd/dir2");

  delete PrefMgr;
}


// =====================================================================
// =====================================================================


BOOST_AUTO_TEST_CASE(test_MarketplacesManagement)
{
  QString CFile = QString(CONFIGTESTS_OUTPUT_DATA_DIR.c_str()) + "/" + QString(openfluid::config::DEFAULT_CONFIGFILE.c_str());
  if (QFile::exists(CFile)) QFile::remove(CFile);

  openfluid::guicommon::PreferencesManager::setFileName(CFile);
  openfluid::guicommon::PreferencesManager* PrefMgr = openfluid::guicommon::PreferencesManager::getInstance();


  openfluid::guicommon::PreferencesManager::MarketPlaces_t Places = PrefMgr->getMarketplaces();

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

  delete PrefMgr;
}


// =====================================================================
// =====================================================================


BOOST_AUTO_TEST_CASE(test_DockToolbarPositionsManagement)
{
  QString CFile = QString(CONFIGTESTS_OUTPUT_DATA_DIR.c_str()) + "/" + QString(openfluid::config::DEFAULT_CONFIGFILE.c_str());
  if (QFile::exists(CFile)) QFile::remove(CFile);

  openfluid::guicommon::PreferencesManager::setFileName(CFile);
  openfluid::guicommon::PreferencesManager* PrefMgr = openfluid::guicommon::PreferencesManager::getInstance();

  PrefMgr->setToolBarPosition(Qt::LeftToolBarArea);
  PrefMgr->setDockPosition(Qt::BottomDockWidgetArea);
  BOOST_CHECK_EQUAL(PrefMgr->getToolBarPosition(),Qt::LeftToolBarArea);
  BOOST_CHECK_EQUAL(PrefMgr->getDockPosition(),Qt::BottomDockWidgetArea);

  PrefMgr->setToolBarPosition(Qt::BottomToolBarArea);
  PrefMgr->setDockPosition(Qt::RightDockWidgetArea);
  BOOST_CHECK_EQUAL(PrefMgr->getToolBarPosition(),Qt::BottomToolBarArea);
  BOOST_CHECK_EQUAL(PrefMgr->getDockPosition(),Qt::RightDockWidgetArea);

  delete PrefMgr;
}


