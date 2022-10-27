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
 @file SrcImportSequenceManager_TEST.cpp

 @author Armel THONI <armel.thoni@inrae.fr>
 */


#define BOOST_TEST_NO_MAIN
#define BOOST_AUTO_TEST_MAIN
#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MODULE unittest_SrcImportSequenceManager


#include <boost/test/unit_test.hpp>

#include <QCoreApplication>
#include <QDir>

#include <openfluid/ui/waresdev/HubManager.hpp>
#include <openfluid/ui/waresdev/SrcImportSequenceManager.hpp>
#include <openfluid/base/WorkspaceManager.hpp>
#include <openfluid/utilsq/GitProxy.hpp>

#include "HubTestFixture.hpp"
#include "tests-config.hpp"


// =====================================================================
// =====================================================================


BOOST_FIXTURE_TEST_CASE(clone_https_wrongauth_v0_fails,HubTestFixture)
{
  if (!checkHttps("clone_https_wrongauth_v0_fails"))
  {
    return;
  }

  openfluid::ui::waresdev::HubManager M(UrlHttpsV0.toStdString());
  openfluid::ui::waresdev::SrcImportSequenceManager W;
  M.connectToHub();
  M.login("wrongname", "wrongpass");

  W.setSelectedWaresUrl( { { openfluid::ware::WareType::SIMULATOR, { getFirstAvailSimUrl(M) } } });

  BOOST_CHECK_EQUAL(W.onCloneRequest(), false);

  BOOST_CHECK(TestWaresDevSimulatorsDir.entryList(QDir::Files).empty());
}


// =====================================================================
// =====================================================================


BOOST_FIXTURE_TEST_CASE(clone_https_wrongauth_v1_fails,HubTestFixture)
{
  if (!checkHttps("clone_https_wrongauth_v1_fails"))
  {
    return;
  }

  openfluid::ui::waresdev::HubManager M(UrlHttpsV1.toStdString());
  M.connectToHub();
  BOOST_CHECK_EQUAL(M.login("wrongname@somewhere", "wrongpass"), false);

  openfluid::ui::waresdev::SrcImportSequenceManager W;
  // TODO ADD TEST VIA LDAP UID?

  W.setSelectedWaresUrl( { { openfluid::ware::WareType::SIMULATOR, { getFirstAvailSimUrl(M) } } });

  BOOST_CHECK_EQUAL(W.onCloneRequest(), false);

  BOOST_CHECK(TestWaresDevSimulatorsDir.entryList(QDir::Files).empty());
}


// =====================================================================
// =====================================================================


BOOST_FIXTURE_TEST_CASE(clone_https_ok,HubTestFixture)
{

  // TODO to enable once cloning will be possible from the testing FluidHub service
/*
  if (!checkHttps("clone_https_ok"))
  {
    return;
  }

  openfluid::ui::waresdev::HubManager M(UrlHttpsV0.toStdString());
  BOOST_CHECK(M.connectToHub());
  M.login("gitof", "gitof");// Check not relevant, no validation is done on login when direct unixname

  openfluid::ui::waresdev::SrcImportSequenceManager W;
  W.setSelectedWaresUrl( { { openfluid::ware::WareType::SIMULATOR, { getFirstAvailSimUrl(M) } } });

  BOOST_CHECK(W.onCloneRequest());

  QDir WareDir(TestWaresDevSimulatorsDir.absoluteFilePath(FirstAvailSimId));
  BOOST_CHECK(WareDir.exists());
  BOOST_CHECK(!WareDir.entryList(QDir::Files).isEmpty());
*/
}


// =====================================================================
// =====================================================================


int main(int argc, char *argv[])
{
  QCoreApplication app(argc, argv);

  if (!CONFIGTESTS_ALLOW_NETWORK_REQUESTS)
  {
    std::cout << "** Test not run due to disabled network requests **" << std::endl;
    return CONFIGTESTS_SKIP_CODE;
  }
  else if (!openfluid::utils::GitProxy::isAvailable())
  {
    std::cout << "** Test not run due to failing to find git program **" << std::endl;
    return CONFIGTESTS_SKIP_CODE;
  }

  return ::boost::unit_test::unit_test_main(&init_unit_test, argc, argv);
}
