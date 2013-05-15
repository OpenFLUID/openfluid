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
 \file PreferencesManager_TEST.cpp
 \brief Implements ...

 \author Aline LIBRES <libres@supagro.inra.fr>
 */

#define BOOST_TEST_MAIN
#define BOOST_AUTO_TEST_MAIN
#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MODULE unittest_PreferencesManager
#include <boost/test/unit_test.hpp>

#include <openfluid/guicommon/PreferencesManager.hpp>

#include <openfluid/config.hpp>
#include <boost/filesystem.hpp>
#include "tests-config.hpp"
#include <openfluid/base/OFException.hpp>
#include <openfluid/base/RuntimeEnv.hpp>

// =====================================================================
// =====================================================================

BOOST_AUTO_TEST_CASE(test_SetFileName)
{
  openfluid::guicommon::PreferencesManager* PrefMgr =
      openfluid::guicommon::PreferencesManager::getInstance();

  boost::filesystem::path ConfigPath = boost::filesystem::path(
      CONFIGTESTS_OUTPUT_DATA_DIR) /= openfluid::config::DEFAULT_CONFIGFILE;

  BOOST_CHECK_THROW(openfluid::guicommon::PreferencesManager::setFileName(ConfigPath.string()),openfluid::base::OFException);

  BOOST_CHECK_EQUAL(PrefMgr->getFileName(),openfluid::base::RuntimeEnvironment::getInstance()->getDefaultConfigFile());

  delete PrefMgr;

  openfluid::guicommon::PreferencesManager::setFileName(ConfigPath.string());
  PrefMgr = openfluid::guicommon::PreferencesManager::getInstance();

  BOOST_CHECK_EQUAL(PrefMgr->getFileName(),ConfigPath.string());

  delete PrefMgr;
}

// =====================================================================
// =====================================================================

BOOST_AUTO_TEST_CASE(test_SetSimpleValues)
{
  boost::filesystem::path ConfigPath = boost::filesystem::path(
      CONFIGTESTS_OUTPUT_DATA_DIR) /= openfluid::config::DEFAULT_CONFIGFILE;

  if (boost::filesystem::exists(ConfigPath))
    boost::filesystem::remove(ConfigPath);

  openfluid::guicommon::PreferencesManager::setFileName(ConfigPath.string());
  openfluid::guicommon::PreferencesManager* PrefMgr =
      openfluid::guicommon::PreferencesManager::getInstance();

  PrefMgr->setLang("oc");
  PrefMgr->setRecentMax(10);
  PrefMgr->setWorkdir("aa/bb/cc");
  PrefMgr->setDeltaT(777);
  PrefMgr->setBegin("2222-11-11T00:11:22");
  PrefMgr->setEnd("2221-12-12=11;22;33");

  BOOST_CHECK_EQUAL(PrefMgr->getRecentMax(),10);
  BOOST_CHECK_EQUAL(PrefMgr->getLang(),"oc");
  BOOST_CHECK_EQUAL(PrefMgr->getWorkdir(),"aa/bb/cc");
  BOOST_CHECK_EQUAL(PrefMgr->getDeltaT(),777);
  BOOST_CHECK_EQUAL(PrefMgr->getBegin(),"2222-11-11T00:11:22");
  BOOST_CHECK_EQUAL(PrefMgr->getEnd(),"2221-12-12=11;22;33");

  delete PrefMgr;
}

// =====================================================================
// =====================================================================

BOOST_AUTO_TEST_CASE(test_RecentProjectsManagement)
{
  boost::filesystem::path ConfigPath = boost::filesystem::path(
      CONFIGTESTS_OUTPUT_DATA_DIR) /= openfluid::config::DEFAULT_CONFIGFILE;

  if (boost::filesystem::exists(ConfigPath))
    boost::filesystem::remove(ConfigPath);

  openfluid::guicommon::PreferencesManager::setFileName(ConfigPath.string());
  openfluid::guicommon::PreferencesManager* PrefMgr =
      openfluid::guicommon::PreferencesManager::getInstance();

  BOOST_CHECK_EQUAL(PrefMgr->getRecentMax(),5);
  BOOST_CHECK_EQUAL(PrefMgr->getRecentProjects().size(),0);

  PrefMgr->addRecentProject("aa/bb/file1.txt", "pj1");
  PrefMgr->addRecentProject("aa/bb/file2.txt", "");
  PrefMgr->addRecentProject("aa/bb/file3.txt", "pj3");

  std::vector<std::pair<std::string, std::string> > Recents =
      PrefMgr->getRecentProjects();

  BOOST_CHECK_EQUAL(Recents.size(),3);
  BOOST_CHECK_EQUAL(Recents[0].first,"aa/bb/file3.txt");
  BOOST_CHECK_EQUAL(Recents[0].second,"pj3");
  BOOST_CHECK_EQUAL(Recents[1].first,"aa/bb/file2.txt");
  BOOST_CHECK_EQUAL(Recents[1].second,"");
  BOOST_CHECK_EQUAL(Recents[2].first,"aa/bb/file1.txt");
  BOOST_CHECK_EQUAL(Recents[2].second,"pj1");

  PrefMgr->addRecentProject("aa/bb/file2.txt", "");

  Recents = PrefMgr->getRecentProjects();

  BOOST_CHECK_EQUAL(Recents.size(),3);
  BOOST_CHECK_EQUAL(Recents[0].first,"aa/bb/file2.txt");
  BOOST_CHECK_EQUAL(Recents[0].second,"");
  BOOST_CHECK_EQUAL(Recents[1].first,"aa/bb/file3.txt");
  BOOST_CHECK_EQUAL(Recents[1].second,"pj3");
  BOOST_CHECK_EQUAL(Recents[2].first,"aa/bb/file1.txt");
  BOOST_CHECK_EQUAL(Recents[2].second,"pj1");

  PrefMgr->clearRecentProjects();

  Recents = PrefMgr->getRecentProjects();

  BOOST_CHECK_EQUAL(Recents.size(),0);

  delete PrefMgr;
}

// =====================================================================
// =====================================================================

BOOST_AUTO_TEST_CASE(test_ExtraPlugPathManagement)
{
  boost::filesystem::path ConfigPath = boost::filesystem::path(
      CONFIGTESTS_OUTPUT_DATA_DIR) /= openfluid::config::DEFAULT_CONFIGFILE;

  if (boost::filesystem::exists(ConfigPath))
    boost::filesystem::remove(ConfigPath);

  openfluid::guicommon::PreferencesManager::setFileName(ConfigPath.string());
  openfluid::guicommon::PreferencesManager* PrefMgr =
      openfluid::guicommon::PreferencesManager::getInstance();

  std::vector<std::string> ExtraPlugPaths = PrefMgr->getExtraPlugPaths();

  BOOST_CHECK_EQUAL(ExtraPlugPaths.size(),0);

  PrefMgr->addExtraPlugPath("aa/bb/dir1");
  PrefMgr->addExtraPlugPath("aa/bb/dir2");
  PrefMgr->addExtraPlugPath("aa/bb/dir3");

  ExtraPlugPaths = PrefMgr->getExtraPlugPaths();

  BOOST_CHECK_EQUAL(ExtraPlugPaths.size(),3);
  BOOST_CHECK_EQUAL(ExtraPlugPaths[0],"aa/bb/dir1");
  BOOST_CHECK_EQUAL(ExtraPlugPaths[1],"aa/bb/dir2");
  BOOST_CHECK_EQUAL(ExtraPlugPaths[2],"aa/bb/dir3");

  PrefMgr->removeExtraPlugPath("aa/bb/dir2");

  ExtraPlugPaths = PrefMgr->getExtraPlugPaths();

  BOOST_CHECK_EQUAL(ExtraPlugPaths.size(),2);
  BOOST_CHECK_EQUAL(ExtraPlugPaths[0],"aa/bb/dir1");
  BOOST_CHECK_EQUAL(ExtraPlugPaths[1],"aa/bb/dir3");

  PrefMgr->removeExtraPlugPath("aa/bb/wrongdir");

  ExtraPlugPaths = PrefMgr->getExtraPlugPaths();

  BOOST_CHECK_EQUAL(ExtraPlugPaths.size(),2);
  BOOST_CHECK_EQUAL(ExtraPlugPaths[0],"aa/bb/dir1");
  BOOST_CHECK_EQUAL(ExtraPlugPaths[1],"aa/bb/dir3");

  PrefMgr->removeExtraPlugPath("aa/bb/dir1");

  ExtraPlugPaths = PrefMgr->getExtraPlugPaths();

  BOOST_CHECK_EQUAL(ExtraPlugPaths.size(),1);
  BOOST_CHECK_EQUAL(ExtraPlugPaths[0],"aa/bb/dir3");

  PrefMgr->removeExtraPlugPath("aa/bb/dir3");

  ExtraPlugPaths = PrefMgr->getExtraPlugPaths();

  BOOST_CHECK_EQUAL(ExtraPlugPaths.size(),0);

  std::vector<Glib::ustring> Paths;
  Paths.push_back("aa/bb/dir1");
  Paths.push_back("aa/bb/dir2");
  Paths.push_back("aa/bb/dir3");

  PrefMgr->setExtraPlugPaths(Paths);

  ExtraPlugPaths = PrefMgr->getExtraPlugPaths();

  BOOST_CHECK_EQUAL(ExtraPlugPaths.size(),3);
  BOOST_CHECK_EQUAL(ExtraPlugPaths[0],"aa/bb/dir1");
  BOOST_CHECK_EQUAL(ExtraPlugPaths[1],"aa/bb/dir2");
  BOOST_CHECK_EQUAL(ExtraPlugPaths[2],"aa/bb/dir3");

  Paths.clear();
  Paths.push_back("cc/dd/dir1");
  Paths.push_back("cc/dd/dir2");

  PrefMgr->setExtraPlugPaths(Paths);

  ExtraPlugPaths = PrefMgr->getExtraPlugPaths();

  BOOST_CHECK_EQUAL(ExtraPlugPaths.size(),2);
  BOOST_CHECK_EQUAL(ExtraPlugPaths[0],"cc/dd/dir1");
  BOOST_CHECK_EQUAL(ExtraPlugPaths[1],"cc/dd/dir2");

  delete PrefMgr;
}

// =====================================================================
// =====================================================================

BOOST_AUTO_TEST_CASE(test_ExtraExtensionPathManagement)
{
  boost::filesystem::path ConfigPath = boost::filesystem::path(
      CONFIGTESTS_OUTPUT_DATA_DIR) /= openfluid::config::DEFAULT_CONFIGFILE;

  if (boost::filesystem::exists(ConfigPath))
    boost::filesystem::remove(ConfigPath);

  openfluid::guicommon::PreferencesManager::setFileName(ConfigPath.string());
  openfluid::guicommon::PreferencesManager* PrefMgr =
      openfluid::guicommon::PreferencesManager::getInstance();

  std::vector<std::string> ExtraExtPaths = PrefMgr->getExtraExtensionPaths();

  BOOST_CHECK_EQUAL(ExtraExtPaths.size(),0);

  PrefMgr->addExtraExtensionPath("aa/bb/dir1");
  PrefMgr->addExtraExtensionPath("aa/bb/dir2");
  PrefMgr->addExtraExtensionPath("aa/bb/dir3");

  ExtraExtPaths = PrefMgr->getExtraExtensionPaths();

  BOOST_CHECK_EQUAL(ExtraExtPaths.size(),3);
  BOOST_CHECK_EQUAL(ExtraExtPaths[0],"aa/bb/dir1");
  BOOST_CHECK_EQUAL(ExtraExtPaths[1],"aa/bb/dir2");
  BOOST_CHECK_EQUAL(ExtraExtPaths[2],"aa/bb/dir3");

  PrefMgr->removeExtraExtensionPath("aa/bb/dir2");

  ExtraExtPaths = PrefMgr->getExtraExtensionPaths();

  BOOST_CHECK_EQUAL(ExtraExtPaths.size(),2);
  BOOST_CHECK_EQUAL(ExtraExtPaths[0],"aa/bb/dir1");
  BOOST_CHECK_EQUAL(ExtraExtPaths[1],"aa/bb/dir3");

  PrefMgr->removeExtraExtensionPath("aa/bb/wrongdir");

  ExtraExtPaths = PrefMgr->getExtraExtensionPaths();

  BOOST_CHECK_EQUAL(ExtraExtPaths.size(),2);
  BOOST_CHECK_EQUAL(ExtraExtPaths[0],"aa/bb/dir1");
  BOOST_CHECK_EQUAL(ExtraExtPaths[1],"aa/bb/dir3");

  PrefMgr->removeExtraExtensionPath("aa/bb/dir1");

  ExtraExtPaths = PrefMgr->getExtraExtensionPaths();

  BOOST_CHECK_EQUAL(ExtraExtPaths.size(),1);
  BOOST_CHECK_EQUAL(ExtraExtPaths[0],"aa/bb/dir3");

  PrefMgr->removeExtraExtensionPath("aa/bb/dir3");

  ExtraExtPaths = PrefMgr->getExtraExtensionPaths();

  BOOST_CHECK_EQUAL(ExtraExtPaths.size(),0);

  std::vector<Glib::ustring> Paths;
  Paths.push_back("aa/bb/dir1");
  Paths.push_back("aa/bb/dir2");
  Paths.push_back("aa/bb/dir3");

  PrefMgr->setExtraExtensionPaths(Paths);

  ExtraExtPaths = PrefMgr->getExtraExtensionPaths();

  BOOST_CHECK_EQUAL(ExtraExtPaths.size(),3);
  BOOST_CHECK_EQUAL(ExtraExtPaths[0],"aa/bb/dir1");
  BOOST_CHECK_EQUAL(ExtraExtPaths[1],"aa/bb/dir2");
  BOOST_CHECK_EQUAL(ExtraExtPaths[2],"aa/bb/dir3");

  Paths.clear();
  Paths.push_back("cc/dd/dir1");
  Paths.push_back("cc/dd/dir2");

  PrefMgr->setExtraExtensionPaths(Paths);

  ExtraExtPaths = PrefMgr->getExtraExtensionPaths();

  BOOST_CHECK_EQUAL(ExtraExtPaths.size(),2);
  BOOST_CHECK_EQUAL(ExtraExtPaths[0],"cc/dd/dir1");
  BOOST_CHECK_EQUAL(ExtraExtPaths[1],"cc/dd/dir2");

  delete PrefMgr;
}

// =====================================================================
// =====================================================================

BOOST_AUTO_TEST_CASE(test_MarketplacesManagement)
{
  boost::filesystem::path ConfigPath = boost::filesystem::path(
      CONFIGTESTS_OUTPUT_DATA_DIR) /= openfluid::config::DEFAULT_CONFIGFILE;

  if (boost::filesystem::exists(ConfigPath))
    boost::filesystem::remove(ConfigPath);

  openfluid::guicommon::PreferencesManager::setFileName(ConfigPath.string());
  openfluid::guicommon::PreferencesManager* PrefMgr =
      openfluid::guicommon::PreferencesManager::getInstance();

  std::map<std::string, std::string> Places = PrefMgr->getMarketplaces();

  BOOST_CHECK_EQUAL(Places.size(),0);

  PrefMgr->addMarketplace("PlaceA", "http://aa/aa/");
  PrefMgr->addMarketplace("Place with spaces", "http://bb/bb/");
  PrefMgr->addMarketplace("Place C", "http://cc/cc/");

  Places = PrefMgr->getMarketplaces();

  BOOST_CHECK_EQUAL(Places.size(),3);
  BOOST_CHECK_EQUAL(Places["PlaceA"],"http://aa/aa/");
  BOOST_CHECK_EQUAL(Places["Place with spaces"],"http://bb/bb/");
  BOOST_CHECK_EQUAL(Places["Place C"],"http://cc/cc/");

  PrefMgr->removeMarketplace("Place with spaces");

  Places = PrefMgr->getMarketplaces();

  BOOST_CHECK_EQUAL(Places.size(),2);
  BOOST_CHECK_EQUAL(Places["PlaceA"],"http://aa/aa/");
  BOOST_CHECK_EQUAL(Places["Place C"],"http://cc/cc/");

  PrefMgr->removeMarketplace("Wrong place");

  Places = PrefMgr->getMarketplaces();

  BOOST_CHECK_EQUAL(Places.size(),2);
  BOOST_CHECK_EQUAL(Places["PlaceA"],"http://aa/aa/");
  BOOST_CHECK_EQUAL(Places["Place C"],"http://cc/cc/");

  PrefMgr->removeMarketplace("PlaceA");
  PrefMgr->removeMarketplace("PlaceC");

  BOOST_CHECK_EQUAL(Places.size(),2);

  BOOST_CHECK_EQUAL(PrefMgr->addMarketplace("Place=A", "http://aa/aa/"), false);

  PrefMgr->addMarketplace("Place;A", "http://aa/aa/");

  Places = PrefMgr->getMarketplaces();
  BOOST_CHECK_EQUAL(Places["Place;A"],"http://aa/aa/");

  delete PrefMgr;
}

// =====================================================================
// =====================================================================

BOOST_AUTO_TEST_CASE(test_Save)
{
  boost::filesystem::path ConfigPath = boost::filesystem::path(
      CONFIGTESTS_OUTPUT_DATA_DIR) /= openfluid::config::DEFAULT_CONFIGFILE;

  if (boost::filesystem::exists(ConfigPath))
    boost::filesystem::remove(ConfigPath);

  openfluid::guicommon::PreferencesManager::setFileName(ConfigPath.string());
  openfluid::guicommon::PreferencesManager* PrefMgr =
      openfluid::guicommon::PreferencesManager::getInstance();

  BOOST_CHECK_EQUAL(boost::filesystem::exists(ConfigPath),true);

  PrefMgr->setDeltaT(1111);

  std::vector<Glib::ustring> Paths;
  Paths.push_back("aa/bb/dir1");
  Paths.push_back("aa/bb/dir2");
  PrefMgr->setExtraPlugPaths(Paths);

  delete PrefMgr;

  openfluid::guicommon::PreferencesManager::setFileName(ConfigPath.string());
  PrefMgr = openfluid::guicommon::PreferencesManager::getInstance();

  BOOST_CHECK_EQUAL(PrefMgr->getDeltaT()==1111,false);
  BOOST_CHECK_EQUAL(PrefMgr->getExtraPlugPaths().size(),0);

  PrefMgr->setDeltaT(1111);
  PrefMgr->setExtraPlugPaths(Paths);
  PrefMgr->save();

  delete PrefMgr;

  openfluid::guicommon::PreferencesManager::setFileName(ConfigPath.string());
  PrefMgr = openfluid::guicommon::PreferencesManager::getInstance();

  BOOST_CHECK_EQUAL(PrefMgr->getDeltaT(),1111);
  BOOST_CHECK_EQUAL(PrefMgr->getExtraPlugPaths().size(),2);

  delete PrefMgr;
}

// =====================================================================
// =====================================================================

BOOST_AUTO_TEST_CASE(test_ExtraObserverPathManagement)
{
  boost::filesystem::path ConfigPath = boost::filesystem::path(
      CONFIGTESTS_OUTPUT_DATA_DIR) /= openfluid::config::DEFAULT_CONFIGFILE;

  if (boost::filesystem::exists(ConfigPath))
    boost::filesystem::remove(ConfigPath);

  openfluid::guicommon::PreferencesManager::setFileName(ConfigPath.string());
  openfluid::guicommon::PreferencesManager* PrefMgr =
      openfluid::guicommon::PreferencesManager::getInstance();

  std::vector<std::string> ExtraObsPaths = PrefMgr->getExtraObserversPaths();

  BOOST_CHECK_EQUAL(ExtraObsPaths.size(),0);

  PrefMgr->addExtraObserversPath("aa/bb/dir1");
  PrefMgr->addExtraObserversPath("aa/bb/dir2");
  PrefMgr->addExtraObserversPath("aa/bb/dir3");

  ExtraObsPaths = PrefMgr->getExtraObserversPaths();

  BOOST_CHECK_EQUAL(ExtraObsPaths.size(),3);
  BOOST_CHECK_EQUAL(ExtraObsPaths[0],"aa/bb/dir1");
  BOOST_CHECK_EQUAL(ExtraObsPaths[1],"aa/bb/dir2");
  BOOST_CHECK_EQUAL(ExtraObsPaths[2],"aa/bb/dir3");

  PrefMgr->removeExtraObserversPath("aa/bb/dir2");

  ExtraObsPaths = PrefMgr->getExtraObserversPaths();

  BOOST_CHECK_EQUAL(ExtraObsPaths.size(),2);
  BOOST_CHECK_EQUAL(ExtraObsPaths[0],"aa/bb/dir1");
  BOOST_CHECK_EQUAL(ExtraObsPaths[1],"aa/bb/dir3");

  PrefMgr->removeExtraObserversPath("aa/bb/wrongdir");

  ExtraObsPaths = PrefMgr->getExtraObserversPaths();

  BOOST_CHECK_EQUAL(ExtraObsPaths.size(),2);
  BOOST_CHECK_EQUAL(ExtraObsPaths[0],"aa/bb/dir1");
  BOOST_CHECK_EQUAL(ExtraObsPaths[1],"aa/bb/dir3");

  PrefMgr->removeExtraObserversPath("aa/bb/dir1");

  ExtraObsPaths = PrefMgr->getExtraObserversPaths();

  BOOST_CHECK_EQUAL(ExtraObsPaths.size(),1);
  BOOST_CHECK_EQUAL(ExtraObsPaths[0],"aa/bb/dir3");

  PrefMgr->removeExtraObserversPath("aa/bb/dir3");

  ExtraObsPaths = PrefMgr->getExtraObserversPaths();

  BOOST_CHECK_EQUAL(ExtraObsPaths.size(),0);

  std::vector<Glib::ustring> Paths;
  Paths.push_back("aa/bb/dir1");
  Paths.push_back("aa/bb/dir2");
  Paths.push_back("aa/bb/dir3");

  PrefMgr->setExtraObserversPaths(Paths);

  ExtraObsPaths = PrefMgr->getExtraObserversPaths();

  BOOST_CHECK_EQUAL(ExtraObsPaths.size(),3);
  BOOST_CHECK_EQUAL(ExtraObsPaths[0],"aa/bb/dir1");
  BOOST_CHECK_EQUAL(ExtraObsPaths[1],"aa/bb/dir2");
  BOOST_CHECK_EQUAL(ExtraObsPaths[2],"aa/bb/dir3");

  Paths.clear();
  Paths.push_back("cc/dd/dir1");
  Paths.push_back("cc/dd/dir2");

  PrefMgr->setExtraObserversPaths(Paths);

  ExtraObsPaths = PrefMgr->getExtraObserversPaths();

  BOOST_CHECK_EQUAL(ExtraObsPaths.size(),2);
  BOOST_CHECK_EQUAL(ExtraObsPaths[0],"cc/dd/dir1");
  BOOST_CHECK_EQUAL(ExtraObsPaths[1],"cc/dd/dir2");

  delete PrefMgr;
}
