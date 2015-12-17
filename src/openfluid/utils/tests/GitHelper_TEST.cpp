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
 @file GitHelper_TEST.cpp

 @author Aline LIBRES <aline.libres@gmail.com>
 */

#define BOOST_TEST_NO_MAIN
#define BOOST_AUTO_TEST_MAIN
#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MODULE unittest_GitHelper
#include <boost/test/unit_test.hpp>

#include <QCoreApplication>
#include <QString>
#include <QDir>
#include <openfluid/base/Environment.hpp>
#include <openfluid/utils/GitHelper.hpp>
#include <openfluid/utils/FluidHubAPIClient.hpp>
#include <openfluid/utils/ExternalProgram.hpp>
#include "tests-config.hpp"


class F
{
  private:

    std::string TestPath;

    QString NoValidCertUrl = QString::fromStdString(CONFIGTESTS_WARESHUB_URL_HTTPS);

  public:

    QString Url = QString::fromStdString(CONFIGTESTS_GITHELPER_URL);
    QString EmptyReposUrl = QString::fromStdString(CONFIGTESTS_GITHELPER_EMPTYREPOS_URL);

    QString AuthUrl = QString::fromStdString(CONFIGTESTS_GITHELPER_AUTH_URL);
    QString AuthUsername = QString::fromStdString(CONFIGTESTS_GITHELPER_AUTH_USERNAME);
    QString AuthPassword = QString::fromStdString(CONFIGTESTS_GITHELPER_AUTH_PASSWORD);

    QString NoValidCertUsername = QString::fromStdString(CONFIGTESTS_WARESHUB_USERNAME);
    QString NoValidCertPassword = QString::fromStdString(CONFIGTESTS_WARESHUB_PASSWORD);

    QString DestPath;
    QDir DestDir;

    QString FirstAvailSimUrl;
    std::string FirstAvailSimId;

    std::string CurrentOFBranchName;

    F()
    {
      TestPath = CONFIGTESTS_OUTPUT_DATA_DIR + "/gitclient";

      DestPath = QString::fromStdString(TestPath + "/" + "TestGitRepository");
      DestDir = DestPath;

      forceRemove(QString::fromStdString(TestPath));

      CurrentOFBranchName = openfluid::utils::GitHelper::getOpenfluidCurrentBranchName();
    }

    ~F()
    {
      forceRemove(QString::fromStdString(TestPath));
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

    bool checkWareshub(const std::string& TestName)
    {
      if (F::NoValidCertUrl.isEmpty())
      {
        std::cout << "** Test not run due to empty wareshub https url ** (\"" << TestName << "\")" << std::endl;
        return false;
      }

      openfluid::utils::FluidHubAPIClient APICli;

      openfluid::utils::RESTClient::SSLConfiguration SSLConfig;
      SSLConfig.setCertificateVerifyMode(QSslSocket::VerifyNone);

      if (!APICli.connect(F::NoValidCertUrl, SSLConfig))
      {
        std::cout << "** Test not run due to failure of wareshub connection ** (\"" << TestName << "\")" << std::endl;
        return false;
      }

      openfluid::utils::FluidHubAPIClient::WaresDetailsByID_t Wares = APICli.getAvailableWaresWithDetails(
          openfluid::ware::WareType::SIMULATOR);

      if (Wares.empty())
      {
        std::cout << "** Test not run due to lack of accessible ware on wareshub ** (\"" << TestName << "\")"
                  << std::endl;
        return false;
      }

      for (const auto& Ware : Wares)
      {
        std::vector<std::string> Branches = Ware.second.GitBranches;

        if (std::find(Branches.begin(), Branches.end(), CurrentOFBranchName) != Branches.end())
        {
          FirstAvailSimUrl = QString::fromStdString(Ware.second.GitUrl);
          FirstAvailSimId = Ware.first;

          DestPath = QString::fromStdString(TestPath + "/" + FirstAvailSimId);
          DestDir = DestPath;

          return true;
        }
      }

      std::cout << "** Test not run due to lack of ware on wareshub on current openfluid branch  ** (\"" << TestName
                << "\")" << std::endl;
      return false;
    }

    void launchGitCommand(const QString& Args)
    {
      QString GitPgm = openfluid::utils::ExternalProgram::getRegisteredProgram(
          openfluid::utils::ExternalProgram::GitProgram).getFullProgramPath();

      QProcess Process;
      Process.setWorkingDirectory(DestPath);

      QString Cmd = QString("\"%1\" %2").arg(GitPgm).arg(Args);

      Process.start(Cmd);
      Process.waitForFinished(-1);
    }
};


// =====================================================================
// =====================================================================


BOOST_FIXTURE_TEST_CASE(clone_wrong_url_fails,F)
{
  openfluid::utils::GitHelper Git;

  BOOST_CHECK(!Git.clone("wrongurl", DestPath));

  BOOST_CHECK(!DestDir.exists());
}

BOOST_FIXTURE_TEST_CASE(clone_empty_url_fails,F)
{
  openfluid::utils::GitHelper Git;

  BOOST_CHECK(!Git.clone("", DestPath));

  BOOST_CHECK(!DestDir.exists());
}

BOOST_FIXTURE_TEST_CASE(clone_empty_dest_fails,F)
{
  openfluid::utils::GitHelper Git;

  BOOST_CHECK(!Git.clone(Url, ""));

  BOOST_CHECK(!DestDir.exists());
}

BOOST_FIXTURE_TEST_CASE(clone_noauth_ok,F)
{
  openfluid::utils::GitHelper Git;

  BOOST_CHECK(Git.clone(Url, DestPath));

  BOOST_CHECK(DestDir.exists());
  BOOST_CHECK(!DestDir.entryList(QDir::Files).isEmpty());
}

// =====================================================================
// =====================================================================

BOOST_FIXTURE_TEST_CASE(clone_wrong_auth_fails,F)
{
  openfluid::utils::GitHelper Git;

  BOOST_CHECK(!Git.clone(AuthUrl, DestPath, "wrongname", "wrongpass"));

  BOOST_CHECK(!DestDir.exists());
}

BOOST_FIXTURE_TEST_CASE(clone_auth_ok,F)
{
  openfluid::utils::GitHelper Git;

  BOOST_CHECK(Git.clone(AuthUrl, DestPath, AuthUsername, AuthPassword));

  BOOST_CHECK(DestDir.exists());
  BOOST_CHECK(!DestDir.entryList(QDir::Files).isEmpty());
}

// =====================================================================
// =====================================================================

BOOST_FIXTURE_TEST_CASE(clone_no_valid_cert_sslverify_fails,F)
{
  if (!checkWareshub("clone_no_valid_cert_sslverify_fails"))
    return;

  openfluid::utils::GitHelper Git;

  BOOST_CHECK(!Git.clone(FirstAvailSimUrl, DestPath, NoValidCertUsername, NoValidCertPassword));

  BOOST_CHECK(!DestDir.exists());
}

// =====================================================================
// =====================================================================

BOOST_FIXTURE_TEST_CASE(clone_no_valid_cert_ok,F)
{
  if (!checkWareshub("clone_no_valid_cert_ok"))
    return;

  openfluid::utils::GitHelper Git;

  BOOST_CHECK(Git.clone(FirstAvailSimUrl, DestPath, NoValidCertUsername, NoValidCertPassword, true));

  BOOST_CHECK(DestDir.exists());
  BOOST_CHECK(!DestDir.entryList(QDir::Files).isEmpty());
}

// =====================================================================
// =====================================================================

BOOST_FIXTURE_TEST_CASE(status_no_git,F)
{
  QDir().mkpath(DestPath);

  openfluid::utils::GitHelper Git;

  BOOST_CHECK_EQUAL(Git.status(DestPath).m_IsGitTracked, false);
}

BOOST_FIXTURE_TEST_CASE(status_git_unchanged,F)
{
  openfluid::utils::GitHelper Git;

  Git.clone(Url, DestPath);

  BOOST_CHECK_EQUAL(Git.status(DestPath).m_IsGitTracked, true);
  BOOST_CHECK(Git.status(DestPath).m_BranchName == QString("master"));
  BOOST_CHECK(Git.status(DestPath).m_FileStatusByTreePath.isEmpty());
}

BOOST_FIXTURE_TEST_CASE(status_git_untracked,F)
{
  openfluid::utils::GitHelper Git;
  Git.clone(Url, DestPath);

  QFile File(DestDir.filePath("new_file.txt"));
  File.open(QIODevice::WriteOnly);

  QMap<QString, openfluid::utils::GitHelper::FileStatusInfo> Paths = Git.status(DestPath).m_FileStatusByTreePath;
  BOOST_CHECK_EQUAL(Paths.size(), 1);
  BOOST_CHECK_EQUAL(Paths["new_file.txt"].m_IsDirty, false);
  BOOST_CHECK(Paths["new_file.txt"].m_IndexStatus == openfluid::utils::GitHelper::FileStatus::UNTRACKED);
}

BOOST_FIXTURE_TEST_CASE(status_git_added,F)
{
  openfluid::utils::GitHelper Git;
  Git.clone(Url, DestPath);

  QFile File(DestDir.filePath("new_file.txt"));
  File.open(QIODevice::WriteOnly);

  launchGitCommand("add new_file.txt");

  QMap<QString, openfluid::utils::GitHelper::FileStatusInfo> Paths = Git.status(DestPath).m_FileStatusByTreePath;
  BOOST_CHECK_EQUAL(Paths.size(), 1);
  BOOST_CHECK_EQUAL(Paths["new_file.txt"].m_IsDirty, false);
  BOOST_CHECK(Paths["new_file.txt"].m_IndexStatus == openfluid::utils::GitHelper::FileStatus::ADDED);
}

BOOST_FIXTURE_TEST_CASE(status_git_added_modified,F)
{
  openfluid::utils::GitHelper Git;
  Git.clone(Url, DestPath);

  QFile File(DestDir.filePath("new_file.txt"));
  File.open(QIODevice::WriteOnly);

  launchGitCommand("add new_file.txt");

  QTextStream FileContent(&File);
  FileContent << "some text";
  File.close();

  QMap<QString, openfluid::utils::GitHelper::FileStatusInfo> Paths = Git.status(DestPath).m_FileStatusByTreePath;
  BOOST_CHECK_EQUAL(Paths.size(), 1);
  BOOST_CHECK_EQUAL(Paths["new_file.txt"].m_IsDirty, true);
  BOOST_CHECK(Paths["new_file.txt"].m_IndexStatus == openfluid::utils::GitHelper::FileStatus::ADDED);
}

BOOST_FIXTURE_TEST_CASE(status_git_modified,F)
{
  openfluid::utils::GitHelper Git;
  Git.clone(Url, DestPath);

  QFile File(DestDir.filePath("a/a1.txt"));
  File.open(QIODevice::WriteOnly);
  QTextStream FileContent(&File);
  FileContent << "some text";
  File.close();

  QMap<QString, openfluid::utils::GitHelper::FileStatusInfo> Paths = Git.status(DestPath).m_FileStatusByTreePath;
  BOOST_CHECK_EQUAL(Paths.size(), 1);
  BOOST_CHECK_EQUAL(Paths["a/a1.txt"].m_IsDirty, true);
  BOOST_CHECK(Paths["a/a1.txt"].m_IndexStatus == openfluid::utils::GitHelper::FileStatus::TRACKED);
}

BOOST_FIXTURE_TEST_CASE(status_git_modified_staged,F)
{
  openfluid::utils::GitHelper Git;
  Git.clone(Url, DestPath);

  QFile File(DestDir.filePath("a/a1.txt"));
  File.open(QIODevice::WriteOnly);
  QTextStream FileContent(&File);
  FileContent << "some text";
  File.close();

  launchGitCommand("add a/a1.txt");

  QMap<QString, openfluid::utils::GitHelper::FileStatusInfo> Paths = Git.status(DestPath).m_FileStatusByTreePath;
  BOOST_CHECK_EQUAL(Paths.size(), 1);
  BOOST_CHECK_EQUAL(Paths["a/a1.txt"].m_IsDirty, false);
  BOOST_CHECK(Paths["a/a1.txt"].m_IndexStatus == openfluid::utils::GitHelper::FileStatus::MODIFIED);
}

BOOST_FIXTURE_TEST_CASE(status_git_modified_partially_staged,F)
{
  openfluid::utils::GitHelper Git;
  Git.clone(Url, DestPath);

  QFile File(DestDir.filePath("a/a1.txt"));
  File.open(QIODevice::WriteOnly);
  QTextStream FileContent(&File);
  FileContent << "some text";
  File.close();

  launchGitCommand("add a/a1.txt");

  File.open(QIODevice::WriteOnly);
  FileContent << "other text";
  File.close();

  QMap<QString, openfluid::utils::GitHelper::FileStatusInfo> Paths = Git.status(DestPath).m_FileStatusByTreePath;
  BOOST_CHECK_EQUAL(Paths.size(), 1);
  BOOST_CHECK_EQUAL(Paths["a/a1.txt"].m_IsDirty, true);
  BOOST_CHECK(Paths["a/a1.txt"].m_IndexStatus == openfluid::utils::GitHelper::FileStatus::MODIFIED);
}

BOOST_FIXTURE_TEST_CASE(status_git_deleted,F)
{
  openfluid::utils::GitHelper Git;
  Git.clone(Url, DestPath);

  launchGitCommand("rm a/a1.txt");

  QMap<QString, openfluid::utils::GitHelper::FileStatusInfo> Paths = Git.status(DestPath).m_FileStatusByTreePath;
  BOOST_CHECK_EQUAL(Paths.size(), 1);
  BOOST_CHECK_EQUAL(Paths["a/a1.txt"].m_IsDirty, false);
  BOOST_CHECK(Paths["a/a1.txt"].m_IndexStatus == openfluid::utils::GitHelper::FileStatus::DELETED);
}

// =====================================================================
// =====================================================================

// =====================================================================
// =====================================================================


int main(int argc, char *argv[])
{
  QCoreApplication app(argc, argv);

  openfluid::base::Environment::init();

  if (!openfluid::utils::GitHelper::checkGitProgram())
  {
    std::cout << "** Test not run due to failing to find git program **" << std::endl;
    return 0;
  }

  return ::boost::unit_test::unit_test_main(&init_unit_test, argc, argv);
}

