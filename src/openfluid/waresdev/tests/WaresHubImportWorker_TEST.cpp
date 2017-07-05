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
 @file WaresHubImportWorker_TEST.cpp
 @brief Implements ...

 @author Aline LIBRES <aline.libres@gmail.com>
 */

#define BOOST_TEST_NO_MAIN
#define BOOST_AUTO_TEST_MAIN
#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MODULE unittest_WaresHubImportWorker
#include <boost/test/unit_test.hpp>

#include <QCoreApplication>
#include <QDir>

#include <openfluid/waresdev/WaresHubImportWorker.hpp>
#include <openfluid/waresdev/WareSrcManager.hpp>
#include <openfluid/utils/GitProxy.hpp>

#include "tests-config.hpp"


// =====================================================================
// =====================================================================


class F
{
  private:

    QString TestWorkspacePath;

  public:

    QDir TestWaresDevSimulatorsDir;

    QString FirstAvailSimId;

    static const QString UrlHttp;
    static const QString UrlHttps;
    static const QString Username;
    static const QString Password;

    std::string CurrentOFBranchName;

    F()
    {
      TestWorkspacePath = QString::fromStdString(CONFIGTESTS_OUTPUT_DATA_DIR);
      TestWorkspacePath.append("/wareshubimport");

      forceRemove(TestWorkspacePath);

      openfluid::waresdev::WareSrcManager* Mgr = openfluid::waresdev::WareSrcManager::instance();

      Mgr->switchWorkspace(TestWorkspacePath);

      TestWaresDevSimulatorsDir = Mgr->getWareTypePath(openfluid::ware::WareType::SIMULATOR);

      CurrentOFBranchName = openfluid::utils::GitProxy::getCurrentOpenFLUIDBranchName().toStdString();
    }

    ~F()
    {
      forceRemove(TestWorkspacePath);
      openfluid::waresdev::WareSrcManager::kill();
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
        forceRemove(QString("%1/%2").arg(Path).arg(SubDir));

      Dir.rmdir(Path);
    }

    QString getFirstAvailSimUrl(openfluid::waresdev::WaresHubImportWorker& W)
    {
      openfluid::utils::FluidHubAPIClient::WaresDetailsByID_t Wares = W.getAvailableWaresWithDetails(
          openfluid::ware::WareType::SIMULATOR);

      if (Wares.empty())
        return "";

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

    static bool checkHttps(const std::string& TestName)
    {
      if (!UrlHttps.isEmpty())
        return true;

      std::cout << "** Test not run due to empty wareshub https url ** (\"" << TestName << "\")" << std::endl;

      return false;
    }
};


const QString F::UrlHttp = QString::fromStdString(CONFIGTESTS_WARESHUB_URL_HTTP);
const QString F::UrlHttps = QString::fromStdString(CONFIGTESTS_WARESHUB_URL_HTTPS);
const QString F::Username = QString::fromStdString(CONFIGTESTS_WARESHUB_USERNAME);
const QString F::Password = QString::fromStdString(CONFIGTESTS_WARESHUB_PASSWORD);


// =====================================================================
// =====================================================================


BOOST_AUTO_TEST_CASE(connect_wrong_url_fails)
{
  openfluid::waresdev::WaresHubImportWorker W("");
  BOOST_CHECK_EQUAL(W.connect(), false);
}


BOOST_AUTO_TEST_CASE(connect_http_ok)
{
  openfluid::waresdev::WaresHubImportWorker W(F::UrlHttp);
  BOOST_CHECK(W.connect());
  BOOST_CHECK(W.isConnected());
}


BOOST_AUTO_TEST_CASE(connect_https_sslNoverify_ok)
{
  if (!F::checkHttps("connect_https_sslNoverify_ok"))
    return;

  openfluid::waresdev::WaresHubImportWorker W(F::UrlHttps, "", "", true);
  BOOST_CHECK(W.connect());
  BOOST_CHECK(W.isConnected());
}


// =====================================================================
// =====================================================================


BOOST_AUTO_TEST_CASE(ware_list_not_connected_ok)
{
  openfluid::waresdev::WaresHubImportWorker W("");
  BOOST_CHECK_EQUAL(W.isConnected(), false);
  BOOST_CHECK(W.getAvailableWaresWithDetails(openfluid::ware::WareType::SIMULATOR).empty());
}


BOOST_AUTO_TEST_CASE(ware_list_http_ok)
{
  openfluid::waresdev::WaresHubImportWorker W(F::UrlHttp);
  W.connect();
  BOOST_CHECK_EQUAL(W.getAvailableWaresWithDetails(openfluid::ware::WareType::SIMULATOR).size(), 4);
  BOOST_CHECK_EQUAL(W.getAvailableWaresWithDetails(openfluid::ware::WareType::OBSERVER).size(), 2);
  BOOST_CHECK_EQUAL(W.getAvailableWaresWithDetails(openfluid::ware::WareType::BUILDEREXT).size(), 1);
  BOOST_CHECK_EQUAL(W.getAvailableWaresWithDetails(openfluid::ware::WareType::UNDEFINED).size(), 0);
}


// =====================================================================
// =====================================================================


BOOST_AUTO_TEST_CASE(ware_list_https_ok)
{
  if (!F::checkHttps("ware_list_https_ok"))
    return;

  openfluid::waresdev::WaresHubImportWorker W(F::UrlHttps, "", "", true);
  W.connect();
  BOOST_CHECK(!W.getAvailableWaresWithDetails(openfluid::ware::WareType::SIMULATOR).empty());
}


BOOST_FIXTURE_TEST_CASE(clone_https_wrongauth_fails,F)
{
  if (!checkHttps("clone_https_wrongauth_fails"))
    return;

  openfluid::waresdev::WaresHubImportWorker W(UrlHttps, "wrongname", "wrongpass", true);
  W.connect();

  W.setSelectedWaresUrl( { { openfluid::ware::WareType::SIMULATOR, { getFirstAvailSimUrl(W) } } });

  BOOST_CHECK_EQUAL(W.clone(), false);

  BOOST_CHECK(TestWaresDevSimulatorsDir.entryList(QDir::Files).empty());
}


// =====================================================================
// =====================================================================


BOOST_FIXTURE_TEST_CASE(clone_https_ok,F)
{
  if (!checkHttps("clone_https_ok"))
    return;

  openfluid::waresdev::WaresHubImportWorker W(UrlHttps, Username, Password, true);
  W.connect();

  W.setSelectedWaresUrl( { { openfluid::ware::WareType::SIMULATOR, { getFirstAvailSimUrl(W) } } });

  BOOST_CHECK(W.clone());

  QDir WareDir(TestWaresDevSimulatorsDir.absoluteFilePath(FirstAvailSimId));
  BOOST_CHECK(WareDir.exists());
  BOOST_CHECK(!WareDir.entryList(QDir::Files).isEmpty());
}


// =====================================================================
// =====================================================================


int main(int argc, char *argv[])
{
  QCoreApplication app(argc, argv);

  if (!openfluid::utils::GitProxy::isAvailable())
  {
    std::cout << "** Test not run due to failing to find git program **" << std::endl;
    return 0;
  }

  return ::boost::unit_test::unit_test_main(&init_unit_test, argc, argv);
}
