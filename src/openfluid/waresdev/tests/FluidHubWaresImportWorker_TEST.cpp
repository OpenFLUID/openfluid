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
 @file FluidHubWaresImportWorker_TEST.cpp

 @author Armel THONI <armel.thoni@inrae.fr>
 */


#define BOOST_TEST_NO_MAIN
#define BOOST_AUTO_TEST_MAIN
#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MODULE unittest_FluidHubWaresImportWorker


#include <boost/test/unit_test.hpp>

#include <QCoreApplication>
#include <QDir>

#include <openfluid/waresdev/FluidHubWaresImportWorker.hpp>
#include <openfluid/base/WorkspaceManager.hpp>
#include <openfluid/utils/GitProxy.hpp>

#include "tests-config.hpp"


// =====================================================================
// =====================================================================


class HubTestFixture
{
  private:

    QString TestWorkspacePath;

  public:

    QDir TestWaresDevSimulatorsDir;

    QString FirstAvailSimId;

    static const QString UrlHttpV0;
    static const QString UrlHttpsV0;
    static const QString UrlHttpsV0Redirect;
    static const QString UrlHttpV1;
    static const QString UrlHttpsV1;
    static const QString Username;
    static const QString Password;

    std::string CurrentOFBranchName;

    HubTestFixture()
    {
      TestWorkspacePath = QString::fromStdString(CONFIGTESTS_OUTPUT_DATA_DIR);
      TestWorkspacePath.append("/FluidHubWaresImport");

      forceRemove(TestWorkspacePath);

      auto Mgr = openfluid::base::WorkspaceManager::instance();

      Mgr->openWorkspace(TestWorkspacePath.toStdString());

      TestWaresDevSimulatorsDir.setPath(
        QString::fromStdString(Mgr->getWaresPath(openfluid::ware::WareType::SIMULATOR))
      );

      CurrentOFBranchName = openfluid::utils::GitProxy::getCurrentOpenFLUIDBranchName().toStdString();
    }

    ~HubTestFixture()
    {
      forceRemove(TestWorkspacePath);
      openfluid::base::WorkspaceManager::kill();
    }

    void forceRemove(const QString& Path)
    {
      QDir Dir(Path);

      for (const auto& SubFile : Dir.entryList(QDir::Files | QDir::System | QDir::Hidden))
      {
        QFile f(QString("%1/%2").arg(Path).arg(SubFile));
        f.setPermissions(QFile::WriteUser);
        f.remove();
      }

      for (const auto& SubDir : Dir.entryList(QDir::Dirs | QDir::NoDotAndDotDot | QDir::System | QDir::Hidden))
      {
        forceRemove(QString("%1/%2").arg(Path).arg(SubDir));
      }

      Dir.rmdir(Path);
    }

    QString getFirstAvailSimUrl(openfluid::waresdev::FluidHubWaresImportWorker& W)
    {
      openfluid::utils::FluidHubAPIClient::WaresDetailsByID_t Wares = W.getAvailableWaresWithDetails(
          openfluid::ware::WareType::SIMULATOR);

      if (Wares.empty())
      {
        return "";
      }

      for (const auto& Ware : Wares)
      {
        std::vector<std::string> Branches = Ware.second.GitBranches;

        if (std::find(Branches.begin(), Branches.end(), CurrentOFBranchName) != Branches.end())
        {
          QString Url = QString::fromStdString(Ware.second.GitUrl);

          FirstAvailSimId = QString::fromStdString(Ware.first);

          return Url;
        }
      }

      return "";
    }

    static bool checkHttps(const std::string& TestName, int version=0)
    {
      QString UrlHttps;
      if (version == 0)
      {
        UrlHttps = UrlHttpsV0;
      }
      else
      {
        UrlHttps = UrlHttpsV1;
      }
      if (!UrlHttps.isEmpty())
      {
        return true;
      }

      std::cout << "** Test not run due to empty wareshub https url ** (\"" << TestName << "\")" << std::endl;

      return false;
    }
};

const QString HubTestFixture::UrlHttpV0 = QString::fromStdString(CONFIGTESTS_FLUIDHUB_V0_URL_HTTP);
const QString HubTestFixture::UrlHttpsV0 = QString::fromStdString(CONFIGTESTS_FLUIDHUB_V0_URL_HTTPS);
const QString HubTestFixture::UrlHttpsV0Redirect = QString::fromStdString(CONFIGTESTS_WARESHUB_URL_REDIRECT_HTTPS);
const QString HubTestFixture::UrlHttpV1 = QString::fromStdString(CONFIGTESTS_FLUIDHUB_V1_URL_HTTP);
const QString HubTestFixture::UrlHttpsV1 = QString::fromStdString(CONFIGTESTS_FLUIDHUB_V1_URL_HTTPS);

const QString HubTestFixture::Username = QString::fromStdString(CONFIGTESTS_WARESHUB_USERNAME);
const QString HubTestFixture::Password = QString::fromStdString(CONFIGTESTS_WARESHUB_PASSWORD);


// =====================================================================
// =====================================================================


BOOST_AUTO_TEST_CASE(connect_wrong_url_fails)
{
  std::cout << " ======== <empty URL> ========" << std::endl;

  openfluid::waresdev::FluidHubWaresImportWorker W("");
  BOOST_CHECK_EQUAL(W.connect(), false);
}


// =====================================================================
// =====================================================================


BOOST_AUTO_TEST_CASE(connect_http_v0_ok)
{
  std::cout << " ======== " << HubTestFixture::UrlHttpV0.toStdString() << " ========" << std::endl;

  openfluid::waresdev::FluidHubWaresImportWorker W(HubTestFixture::UrlHttpV0);
  BOOST_CHECK(W.connect());
  BOOST_CHECK(W.isConnected());
}


// =====================================================================
// =====================================================================


BOOST_AUTO_TEST_CASE(connect_http_v1_ok)
{
  std::cout << " ======== " << HubTestFixture::UrlHttpV1.toStdString() << " ========" << std::endl;

  openfluid::waresdev::FluidHubWaresImportWorker W(HubTestFixture::UrlHttpV1);
  BOOST_CHECK(W.connect());
  BOOST_CHECK(W.isConnected());
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

  openfluid::waresdev::FluidHubWaresImportWorker W(HubTestFixture::UrlHttpsV0);
  BOOST_CHECK(W.connect());
  BOOST_CHECK(W.isConnected());
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

  openfluid::waresdev::FluidHubWaresImportWorker W(HubTestFixture::UrlHttpsV1);
  BOOST_CHECK(W.connect());
  BOOST_CHECK(W.isConnected());
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

  openfluid::waresdev::FluidHubWaresImportWorker W(HubTestFixture::UrlHttpsV0Redirect);
  BOOST_CHECK(W.connect());
  BOOST_CHECK(W.isConnected());
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

  openfluid::waresdev::FluidHubWaresImportWorker W(HubTestFixture::UrlHttpsV0, true);
  BOOST_CHECK(W.connect());
  BOOST_CHECK(W.isConnected());
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

  openfluid::waresdev::FluidHubWaresImportWorker W(HubTestFixture::UrlHttpsV1, true);
  BOOST_CHECK(W.connect());
  BOOST_CHECK(W.isConnected());
}


// =====================================================================
// =====================================================================


BOOST_AUTO_TEST_CASE(ware_list_not_connected_ok)
{
  openfluid::waresdev::FluidHubWaresImportWorker W("");
  BOOST_CHECK_EQUAL(W.isConnected(), false);
  BOOST_CHECK(W.getAvailableWaresWithDetails(openfluid::ware::WareType::SIMULATOR).empty());
}


// =====================================================================
// =====================================================================


BOOST_AUTO_TEST_CASE(ware_list_http_v0_ok)
{
  openfluid::waresdev::FluidHubWaresImportWorker W(HubTestFixture::UrlHttpV0);
  W.connect();
  BOOST_CHECK_EQUAL(W.getAvailableWaresWithDetails(openfluid::ware::WareType::SIMULATOR).size(), 4);
  BOOST_CHECK_EQUAL(W.getAvailableWaresWithDetails(openfluid::ware::WareType::OBSERVER).size(), 2);
  BOOST_CHECK_EQUAL(W.getAvailableWaresWithDetails(openfluid::ware::WareType::BUILDEREXT).size(), 1);
  BOOST_CHECK_EQUAL(W.getAvailableWaresWithDetails(openfluid::ware::WareType::UNDEFINED).size(), 0);
}


// =====================================================================
// =====================================================================


BOOST_AUTO_TEST_CASE(ware_list_http_v1_ok)
{
  openfluid::waresdev::FluidHubWaresImportWorker W(HubTestFixture::UrlHttpV1);
  W.connect();
  BOOST_CHECK_EQUAL(W.getAvailableWaresWithDetails(openfluid::ware::WareType::SIMULATOR).size(), 4);
  BOOST_CHECK_EQUAL(W.getAvailableWaresWithDetails(openfluid::ware::WareType::OBSERVER).size(), 2);
  BOOST_CHECK_EQUAL(W.getAvailableWaresWithDetails(openfluid::ware::WareType::BUILDEREXT).size(), 1);
  BOOST_CHECK_EQUAL(W.getAvailableWaresWithDetails(openfluid::ware::WareType::UNDEFINED).size(), 0);
}


// =====================================================================
// =====================================================================


BOOST_AUTO_TEST_CASE(ware_list_https_v0_ok)
{
  if (!HubTestFixture::checkHttps("ware_list_https_ok", 0))
  {
    return;
  }

  openfluid::waresdev::FluidHubWaresImportWorker W(HubTestFixture::UrlHttpsV0);
  W.connect();
  BOOST_CHECK_EQUAL(W.getAvailableWaresWithDetails(openfluid::ware::WareType::SIMULATOR).size(), 4);
  BOOST_CHECK_EQUAL(W.getAvailableWaresWithDetails(openfluid::ware::WareType::OBSERVER).size(), 2);
  BOOST_CHECK_EQUAL(W.getAvailableWaresWithDetails(openfluid::ware::WareType::BUILDEREXT).size(), 1);
  BOOST_CHECK_EQUAL(W.getAvailableWaresWithDetails(openfluid::ware::WareType::UNDEFINED).size(), 0);
}


// =====================================================================
// =====================================================================


BOOST_AUTO_TEST_CASE(ware_list_https_v1_ok)
{
  if (!HubTestFixture::checkHttps("ware_list_https_ok", 1))
  {
    return;
  }

  openfluid::waresdev::FluidHubWaresImportWorker W(HubTestFixture::UrlHttpsV1);
  W.connect();
  BOOST_CHECK_EQUAL(W.getAvailableWaresWithDetails(openfluid::ware::WareType::SIMULATOR).size(), 4);
  BOOST_CHECK_EQUAL(W.getAvailableWaresWithDetails(openfluid::ware::WareType::OBSERVER).size(), 2);
  BOOST_CHECK_EQUAL(W.getAvailableWaresWithDetails(openfluid::ware::WareType::BUILDEREXT).size(), 1);
  BOOST_CHECK_EQUAL(W.getAvailableWaresWithDetails(openfluid::ware::WareType::UNDEFINED).size(), 0);
}


// =====================================================================
// =====================================================================


BOOST_FIXTURE_TEST_CASE(clone_https_wrongauth_v0_fails,HubTestFixture)
{
  if (!checkHttps("clone_https_wrongauth_v0_fails"))
  {
    return;
  }

  openfluid::waresdev::FluidHubWaresImportWorker W(UrlHttpsV0, true);
  W.connect();
  W.login("wrongname", "wrongpass");

  W.setSelectedWaresUrl( { { openfluid::ware::WareType::SIMULATOR, { getFirstAvailSimUrl(W) } } });

  BOOST_CHECK_EQUAL(W.clone(), false);

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

  openfluid::waresdev::FluidHubWaresImportWorker W(UrlHttpsV1, true);
  W.connect();
  BOOST_CHECK_EQUAL(W.login("wrongname", "wrongpass"), false);

  W.setSelectedWaresUrl( { { openfluid::ware::WareType::SIMULATOR, { getFirstAvailSimUrl(W) } } });

  BOOST_CHECK_EQUAL(W.clone(), false);

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

  openfluid::waresdev::FluidHubWaresImportWorker W(UrlHttps, true);
  BOOST_CHECK(W.connect());
  BOOST_CHECK(W.login(Username, Password));

  W.setSelectedWaresUrl( { { openfluid::ware::WareType::SIMULATOR, { getFirstAvailSimUrl(W) } } });

  BOOST_CHECK(W.clone());

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
