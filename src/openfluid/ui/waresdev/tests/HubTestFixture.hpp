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
 @file HubTestFixture.hpp

 @author Armel THONI <armel.thoni@inrae.fr>
 */

#ifndef __OPENFLUID_UIWARESDEVTESTS_HUBTESTFIXTURE_HPP__
#define __OPENFLUID_UIWARESDEVTESTS_HUBTESTFIXTURE_HPP__


#include <QCoreApplication>
#include <QDir>

#include "tests-config.hpp"


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

    QString getFirstAvailSimUrl(openfluid::ui::waresdev::HubManager& W)
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

#endif /* __OPENFLUID_UIWARESDEVTESTS_HUBTESTFIXTURE_HPP__ */
