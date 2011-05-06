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
#define BOOST_TEST_MODULE builder_unittest_PreferencesManager
#include <boost/test/unit_test.hpp>

#include <openfluid/guicommon/PreferencesManager.hpp>

#include <openfluid/base/RuntimeEnv.hpp>
#include <boost/filesystem.hpp>

// =====================================================================
// =====================================================================

BOOST_AUTO_TEST_CASE(test_SetSimpleValues)
{
  boost::filesystem::path
      ConfigPath =
          boost::filesystem::path(
              openfluid::base::RuntimeEnvironment::getInstance()->getDefaultConfigFile());

  if (boost::filesystem::exists(ConfigPath))
    boost::filesystem::remove(ConfigPath);

  openfluid::guicommon::PreferencesManager* PrefMgr = openfluid::guicommon::PreferencesManager::getInstance();

  PrefMgr->setLang("oc");
  PrefMgr->setRecentMax(10);
  PrefMgr->setWorkdir("aa/bb/cc");
  PrefMgr->setDeltaT(777);
  PrefMgr->setBegin("2222-11-11T00:11:22");
  PrefMgr->setEnd("2221-12-12=11;22;33");
  PrefMgr->setOutFilesBuffer(4);

  BOOST_CHECK_EQUAL(PrefMgr->getRecentMax(),10);
  BOOST_CHECK_EQUAL(PrefMgr->getLang(),"oc");
  BOOST_CHECK_EQUAL(PrefMgr->getWorkdir(),"aa/bb/cc");
  BOOST_CHECK_EQUAL(PrefMgr->getDeltaT(),777);
  BOOST_CHECK_EQUAL(PrefMgr->getBegin(),"2222-11-11T00:11:22");
  BOOST_CHECK_EQUAL(PrefMgr->getEnd(),"2221-12-12=11;22;33");
  BOOST_CHECK_EQUAL(PrefMgr->getOutFilesBuffer(),4);

  delete PrefMgr;
}

BOOST_AUTO_TEST_CASE(test_RecentProjectsManagement)
{
  boost::filesystem::path
      ConfigPath =
          boost::filesystem::path(
              openfluid::base::RuntimeEnvironment::getInstance()->getDefaultConfigFile());

  if (boost::filesystem::exists(ConfigPath))
    boost::filesystem::remove(ConfigPath);

  openfluid::guicommon::PreferencesManager* PrefMgr = openfluid::guicommon::PreferencesManager::getInstance();

  BOOST_CHECK_EQUAL(PrefMgr->getRecentMax(),5);
  BOOST_CHECK_EQUAL(PrefMgr->getRecentProjects().size(),0);

  PrefMgr->addRecentProject("aa/bb/file1.txt", "pj1");
  PrefMgr->addRecentProject("aa/bb/file2.txt", "");
  PrefMgr->addRecentProject("aa/bb/file3.txt", "pj3");

  std::vector<std::pair<std::string, std::string> > Recents =
      PrefMgr->getRecentProjects();

  BOOST_CHECK_EQUAL(Recents.size(),3);
  BOOST_CHECK_EQUAL(Recents[0].first,"aa/bb/file1.txt");
  BOOST_CHECK_EQUAL(Recents[0].second,"pj1");
  BOOST_CHECK_EQUAL(Recents[1].first,"aa/bb/file2.txt");
  BOOST_CHECK_EQUAL(Recents[1].second,"");
  BOOST_CHECK_EQUAL(Recents[2].first,"aa/bb/file3.txt");
  BOOST_CHECK_EQUAL(Recents[2].second,"pj3");

  PrefMgr->setRecentMax(2);

  Recents = PrefMgr->getRecentProjects();

  BOOST_CHECK_EQUAL(Recents.size(),2);
  BOOST_CHECK_EQUAL(Recents[0].first,"aa/bb/file2.txt");
  BOOST_CHECK_EQUAL(Recents[0].second,"");
  BOOST_CHECK_EQUAL(Recents[1].first,"aa/bb/file3.txt");
  BOOST_CHECK_EQUAL(Recents[1].second,"pj3");

  PrefMgr->addRecentProject("aa/bb/file4.txt", "pj4");

  Recents = PrefMgr->getRecentProjects();

  BOOST_CHECK_EQUAL(Recents.size(),2);
  BOOST_CHECK_EQUAL(Recents[0].first,"aa/bb/file3.txt");
  BOOST_CHECK_EQUAL(Recents[0].second,"pj3");
  BOOST_CHECK_EQUAL(Recents[1].first,"aa/bb/file4.txt");
  BOOST_CHECK_EQUAL(Recents[1].second,"pj4");

  PrefMgr->clearRecentProjects();

  Recents = PrefMgr->getRecentProjects();

  BOOST_CHECK_EQUAL(Recents.size(),0);

  delete PrefMgr;
}

BOOST_AUTO_TEST_CASE(test_ExtraPlugPathManagement)
{
  boost::filesystem::path
      ConfigPath =
          boost::filesystem::path(
              openfluid::base::RuntimeEnvironment::getInstance()->getDefaultConfigFile());

  if (boost::filesystem::exists(ConfigPath))
    boost::filesystem::remove(ConfigPath);

  openfluid::guicommon::PreferencesManager* PrefMgr = openfluid::guicommon::PreferencesManager::getInstance();

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

  delete PrefMgr;
}

BOOST_AUTO_TEST_CASE(test_MarketplacesManagement)
{
  boost::filesystem::path
      ConfigPath =
          boost::filesystem::path(
              openfluid::base::RuntimeEnvironment::getInstance()->getDefaultConfigFile());

  if (boost::filesystem::exists(ConfigPath))
    boost::filesystem::remove(ConfigPath);

  openfluid::guicommon::PreferencesManager* PrefMgr = openfluid::guicommon::PreferencesManager::getInstance();

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

BOOST_AUTO_TEST_CASE(test_Save)
{
  boost::filesystem::path
      ConfigPath =
          boost::filesystem::path(
              openfluid::base::RuntimeEnvironment::getInstance()->getDefaultConfigFile());

  if (boost::filesystem::exists(ConfigPath))
    boost::filesystem::remove(ConfigPath);

  openfluid::guicommon::PreferencesManager* PrefMgr = openfluid::guicommon::PreferencesManager::getInstance();

  BOOST_CHECK_EQUAL(boost::filesystem::exists(ConfigPath),true);

  PrefMgr->setDeltaT(1111);

  delete PrefMgr;

  PrefMgr = openfluid::guicommon::PreferencesManager::getInstance();

  BOOST_CHECK_EQUAL(PrefMgr->getDeltaT()==1111,false);

  PrefMgr->setDeltaT(1111);
  PrefMgr->save();

  delete PrefMgr;

  PrefMgr = openfluid::guicommon::PreferencesManager::getInstance();

  BOOST_CHECK_EQUAL(PrefMgr->getDeltaT(),1111);

  delete PrefMgr;
}
