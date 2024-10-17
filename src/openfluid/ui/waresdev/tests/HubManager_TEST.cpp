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
 @file HubManager_TEST.cpp

 @author Armel THONI <armel.thoni@inrae.fr>
 */


#define BOOST_TEST_NO_MAIN
#define BOOST_AUTO_TEST_MAIN
#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MODULE unittest_HubManager


#include <boost/test/unit_test.hpp>

#include <QCoreApplication>
#include <QDir>

#include <openfluid/ui/waresdev/HubManager.hpp>
#include <openfluid/base/WorkspaceManager.hpp>
#include <openfluid/ui/waresdev/GitUIProxy.hpp>

#include "HubTestFixture.hpp"
#include "tests-config.hpp"


// =====================================================================
// =====================================================================


BOOST_AUTO_TEST_CASE(connect_wrong_url_fails)
{
  std::cout << " ======== <empty URL> ========" << std::endl;

  openfluid::ui::waresdev::HubManager M("");
  BOOST_CHECK_EQUAL(M.connectToHub(), false);
}


// =====================================================================
// =====================================================================


BOOST_AUTO_TEST_CASE(connect_http_v0_ok)
{
  std::cout << " ======== " << HubTestFixture::UrlHttpV0.toStdString() << " ========" << std::endl;

  openfluid::ui::waresdev::HubManager M(HubTestFixture::UrlHttpV0.toStdString());
  BOOST_CHECK(M.connectToHub());
  BOOST_CHECK(M.isConnected());
}


// =====================================================================
// =====================================================================


BOOST_AUTO_TEST_CASE(connect_http_v1_ok)
{
  std::cout << " ======== " << HubTestFixture::UrlHttpV1.toStdString() << " ========" << std::endl;

  openfluid::ui::waresdev::HubManager M(HubTestFixture::UrlHttpV1.toStdString());
  BOOST_CHECK(M.connectToHub());
  BOOST_CHECK(M.isConnected());
}


// =====================================================================
// =====================================================================


BOOST_AUTO_TEST_CASE(connect_https_ssl_v0_ok)
{
  std::cout << " ======== " << HubTestFixture::UrlHttpsV0.toStdString() << " (ok) ========" << std::endl;

  if (!HubTestFixture::checkHttps("connect_https_ssl_ok", 0))
  {
    return;
  }

  openfluid::ui::waresdev::HubManager M(HubTestFixture::UrlHttpsV0.toStdString());
  BOOST_CHECK(M.connectToHub());
  BOOST_CHECK(M.isConnected());
}


// =====================================================================
// =====================================================================


BOOST_AUTO_TEST_CASE(connect_https_ssl_v1_ok)
{
  std::cout << " ======== " << HubTestFixture::UrlHttpsV1.toStdString() << " (ok) ========" << std::endl;

  if (!HubTestFixture::checkHttps("connect_https_ssl_ok", 1))
  {
    return;
  }

  openfluid::ui::waresdev::HubManager M(HubTestFixture::UrlHttpsV1.toStdString());
  BOOST_CHECK(M.connectToHub());
  BOOST_CHECK(M.isConnected());
}


// =====================================================================
// =====================================================================


#if OPENFLUID_REST_URL_REDIRECT
BOOST_AUTO_TEST_CASE(connect_https_ssl_ok_redirect)
{
  if (!HubTestFixture::checkHttps("connect_https_ssl_ok_redirect", 0))
  {
    return;
  }

  std::cout << " ======== " << HubTestFixture::UrlHttpsV0Redirect.toStdString() << " (ok) ========" << std::endl;

  openfluid::ui::waresdev::HubManager M(HubTestFixture::UrlHttpsV0Redirect.toStdString());
  BOOST_CHECK(M.connectToHub());
  BOOST_CHECK(M.isConnected());
}
#endif


// =====================================================================
// =====================================================================


BOOST_AUTO_TEST_CASE(connect_https_sslNoverify_v0_ok)
{
  if (!HubTestFixture::checkHttps("connect_https_sslNoverify_ok", 0))
  {
    return;
  }

  std::cout << " ======== " << HubTestFixture::UrlHttpsV0.toStdString() << " (noverify) ========" << std::endl;

  openfluid::ui::waresdev::HubManager M(HubTestFixture::UrlHttpsV0.toStdString());
  BOOST_CHECK(M.connectToHub());
  BOOST_CHECK(M.isConnected());
}


// =====================================================================
// =====================================================================


BOOST_AUTO_TEST_CASE(connect_https_sslNoverify_v1_ok)
{
  if (!HubTestFixture::checkHttps("connect_https_sslNoverify_ok", 1))
  {
    return;
  }

  std::cout << " ======== " << HubTestFixture::UrlHttpsV1.toStdString() << " (noverify) ========" << std::endl;

  openfluid::ui::waresdev::HubManager M(HubTestFixture::UrlHttpsV1.toStdString());  // , true
  BOOST_CHECK(M.connectToHub());
  BOOST_CHECK(M.isConnected());
}


// =====================================================================
// =====================================================================


BOOST_AUTO_TEST_CASE(ware_list_not_connected_ok)
{
  openfluid::ui::waresdev::HubManager M("");
  BOOST_CHECK_EQUAL(M.isConnected(), false);
  BOOST_CHECK(M.getAvailableWaresWithDetails(openfluid::ware::WareType::SIMULATOR).empty());
}


// =====================================================================
// =====================================================================


BOOST_AUTO_TEST_CASE(ware_list_http_v0_ok)
{
  openfluid::ui::waresdev::HubManager M(HubTestFixture::UrlHttpV0.toStdString());
  M.connectToHub();
  BOOST_CHECK_EQUAL(M.getAvailableWaresWithDetails(openfluid::ware::WareType::SIMULATOR).size(), 4);
  BOOST_CHECK_EQUAL(M.getAvailableWaresWithDetails(openfluid::ware::WareType::OBSERVER).size(), 2);
  BOOST_CHECK_EQUAL(M.getAvailableWaresWithDetails(openfluid::ware::WareType::BUILDEREXT).size(), 1);
  BOOST_CHECK_EQUAL(M.getAvailableWaresWithDetails(openfluid::ware::WareType::UNDEFINED).size(), 0);
}


// =====================================================================
// =====================================================================


BOOST_AUTO_TEST_CASE(ware_list_http_v1_ok)
{
  openfluid::ui::waresdev::HubManager M(HubTestFixture::UrlHttpV1.toStdString());
  M.connectToHub();
  BOOST_CHECK_EQUAL(M.getAvailableWaresWithDetails(openfluid::ware::WareType::SIMULATOR).size(), 4);
  BOOST_CHECK_EQUAL(M.getAvailableWaresWithDetails(openfluid::ware::WareType::OBSERVER).size(), 2);
  BOOST_CHECK_EQUAL(M.getAvailableWaresWithDetails(openfluid::ware::WareType::BUILDEREXT).size(), 1);
  BOOST_CHECK_EQUAL(M.getAvailableWaresWithDetails(openfluid::ware::WareType::UNDEFINED).size(), 0);
}


// =====================================================================
// =====================================================================


BOOST_AUTO_TEST_CASE(ware_list_https_v0_ok)
{
  if (!HubTestFixture::checkHttps("ware_list_https_ok", 0))
  {
    return;
  }

  openfluid::ui::waresdev::HubManager M(HubTestFixture::UrlHttpsV0.toStdString());
  M.connectToHub();
  BOOST_CHECK_EQUAL(M.getAvailableWaresWithDetails(openfluid::ware::WareType::SIMULATOR).size(), 4);
  BOOST_CHECK_EQUAL(M.getAvailableWaresWithDetails(openfluid::ware::WareType::OBSERVER).size(), 2);
  BOOST_CHECK_EQUAL(M.getAvailableWaresWithDetails(openfluid::ware::WareType::BUILDEREXT).size(), 1);
  BOOST_CHECK_EQUAL(M.getAvailableWaresWithDetails(openfluid::ware::WareType::UNDEFINED).size(), 0);
}


// =====================================================================
// =====================================================================


BOOST_AUTO_TEST_CASE(ware_list_https_v1_ok)
{
  if (!HubTestFixture::checkHttps("ware_list_https_ok", 1))
  {
    return;
  }

  openfluid::ui::waresdev::HubManager M(HubTestFixture::UrlHttpsV1.toStdString());
  M.connectToHub();
  BOOST_CHECK_EQUAL(M.getAvailableWaresWithDetails(openfluid::ware::WareType::SIMULATOR).size(), 4);
  BOOST_CHECK_EQUAL(M.getAvailableWaresWithDetails(openfluid::ware::WareType::OBSERVER).size(), 2);
  BOOST_CHECK_EQUAL(M.getAvailableWaresWithDetails(openfluid::ware::WareType::BUILDEREXT).size(), 1);
  BOOST_CHECK_EQUAL(M.getAvailableWaresWithDetails(openfluid::ware::WareType::UNDEFINED).size(), 0);
  BOOST_CHECK_EQUAL(M.getAvailableFragmentsWithDetails().size(), 1);
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

  return ::boost::unit_test::unit_test_main(&init_unit_test, argc, argv);
}
