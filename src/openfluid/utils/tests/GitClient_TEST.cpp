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
 @file GitClient_TEST.cpp

 @author Aline LIBRES <aline.libres@gmail.com>
 */

#define BOOST_TEST_NO_MAIN
#define BOOST_AUTO_TEST_MAIN
#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MODULE unittest_GitClient
#include <boost/test/unit_test.hpp>

#include <QCoreApplication>
#include <QString>
#include <QDir>
#include <openfluid/utils/GitClient.hpp>
#include <openfluid/tools/Filesystem.hpp>
#include <openfluid/utils/FluidHubAPIClient.hpp>
#include "tests-config.hpp"


QString FirstAvailSimUrl;
std::string FirstAvailSimId;

class F
{
  private:

    std::string TestPath;

  public:

    QString DestPath;
    static const QString UrlHttps;
    static const QString Username;
    static const QString Password;

    F()
    {
      TestPath = CONFIGTESTS_OUTPUT_DATA_DIR + "/gitclient";

      DestPath = QString::fromStdString(TestPath + "/" + FirstAvailSimId);

      openfluid::tools::Filesystem::removeDirectory(TestPath);
    }

    ~F()
    {
      openfluid::tools::Filesystem::removeDirectory(TestPath);
    }
};


const QString F::UrlHttps = QString::fromStdString(CONFIGTESTS_WARESHUB_URL_HTTPS);
const QString F::Username = QString::fromStdString(CONFIGTESTS_WARESHUB_USERNAME);
const QString F::Password = QString::fromStdString(CONFIGTESTS_WARESHUB_PASSWORD);


// =====================================================================
// =====================================================================

BOOST_FIXTURE_TEST_CASE(clone_wrong_url_fails,F)
{
  BOOST_CHECK(!openfluid::utils::GitClient::clone("wrongurl", DestPath, Username, Password, true).isEmpty());
}

BOOST_FIXTURE_TEST_CASE(clone_empty_url_fails,F)
{
  BOOST_CHECK(!openfluid::utils::GitClient::clone("", DestPath, Username, Password, true).isEmpty());
}

BOOST_FIXTURE_TEST_CASE(clone_empty_dest_fails,F)
{
  BOOST_CHECK(!openfluid::utils::GitClient::clone(FirstAvailSimUrl, "", Username, Password, true).isEmpty());
}

BOOST_FIXTURE_TEST_CASE(clone_wrong_auth_fails,F)
{
  BOOST_CHECK(!openfluid::utils::GitClient::clone(FirstAvailSimUrl, "", "wrongname", "wrongpass", true).isEmpty());
}

BOOST_FIXTURE_TEST_CASE(clone_ok,F)
{
  BOOST_CHECK(openfluid::utils::GitClient::clone(FirstAvailSimUrl, DestPath, Username, Password, true).isEmpty());

  QDir ClonedDir(DestPath);
  BOOST_CHECK(ClonedDir.exists());
  BOOST_CHECK(!ClonedDir.entryList(QDir::Files).isEmpty());
}


// =====================================================================
// =====================================================================

// =====================================================================
// =====================================================================


int main(int argc, char *argv[])
{
  QCoreApplication app(argc, argv);

  if (F::UrlHttps.isEmpty())
  {
    std::cout << "** Test not run due to empty wareshub https url ** " << std::endl;
    return 0;
  }

  openfluid::utils::FluidHubAPIClient APICli;

  openfluid::utils::RESTClient::SSLConfiguration SSLConfig;
  SSLConfig.setCertificateVerifyMode(QSslSocket::VerifyNone);

  if (!APICli.connect(F::UrlHttps, SSLConfig))
  {
    std::cout << "** Test not run due to failure of wareshub connection ** " << std::endl;
    return 0;
  }

  openfluid::utils::FluidHubAPIClient::WaresDetailsByID_t Wares = APICli.getAvailableWaresWithDetails(
      openfluid::ware::WareType::SIMULATOR);

  if (Wares.empty())
  {
    std::cout << "** Test not run due to lack of accessible ware on wareshub ** " << std::endl;
    return 0;
  }

  FirstAvailSimUrl = QString::fromStdString(Wares.begin()->second.GitUrl);
  FirstAvailSimId = Wares.begin()->first;

  return ::boost::unit_test::unit_test_main(&init_unit_test, argc, argv);
}

