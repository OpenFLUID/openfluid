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
  @file SrcImportSequenceManager.cpp

  @author Armel THÖNI <armel.thoni@inrae.fr>
*/


#include <QApplication>
#include <QThread>

#include <openfluid/base/PreferencesManager.hpp>
#include <openfluid/base/WorkspaceManager.hpp>
#include <openfluid/ui/waresdev/SrcImportSequenceManager.hpp>


namespace openfluid { namespace ui { namespace waresdev {


SrcImportSequenceManager::SrcImportSequenceManager()
{

}


// =====================================================================
// =====================================================================


SrcImportSequenceManager::~SrcImportSequenceManager()
{

}


// =====================================================================
// =====================================================================


void SrcImportSequenceManager::setSelectedWaresUrl(
  const std::map<openfluid::ware::WareType, QStringList>& SelectedWaresUrlByType)
{
  m_SelectedWaresUrlByType = SelectedWaresUrlByType;
}


// =====================================================================
// =====================================================================


void SrcImportSequenceManager::setSelectedFragmentsUrl(const QStringList& SelectedFragmentsUrl)
{
  m_SelectedFragmentsUrl = SelectedFragmentsUrl;
}


// =====================================================================
// =====================================================================


void SrcImportSequenceManager::setupFragmentStrategy(const QString& RootPath, bool AsSubmodule)
{
  m_RootPath = RootPath;
  m_FragmentsAsSubmodule = AsSubmodule;
}


// =====================================================================
// =====================================================================


bool SrcImportSequenceManager::runWorker(GitImportWorker* Worker, std::vector<std::pair<QString, QString>> Elements)
{
  Worker->setupUser(m_Username, m_Password);
  Worker->setSelectedElements(Elements);

  connect(Worker, SIGNAL(finished(bool, const QString&)), this,
          SIGNAL(finished(bool, const QString&)));
  connect(Worker, SIGNAL(info(const QString&)), this, 
          SIGNAL(info(const QString&)));
  connect(Worker, SIGNAL(error(const QString&)), this, 
          SIGNAL(error(const QString&)));
  connect(Worker, SIGNAL(progressed(int)), this, SIGNAL(progressed(int)));
  
  return Worker->runImports();
}


// =====================================================================
// =====================================================================


bool SrcImportSequenceManager::onCloneRequest()
{
  bool OK = false;

  int Progress = 0;
  double ProgressRatio = 100;
  bool WaresFound = false;
  bool FragmentsFound = false;

  int SelectedElementPathsNb = 0;
  for (const auto& Pair : m_SelectedWaresUrlByType)
  {
    SelectedElementPathsNb += Pair.second.size();
  }
  WaresFound = SelectedElementPathsNb > 0;
  SelectedElementPathsNb += m_SelectedFragmentsUrl.size();
  FragmentsFound = m_SelectedFragmentsUrl.size() > 0;

  if (SelectedElementPathsNb)
  {
    ProgressRatio /= SelectedElementPathsNb;
  }

  bool SslNoVerify = openfluid::base::PreferencesManager::instance()->isWaresdevGitSslNoVerify();

  if (WaresFound)
  {
    // WARE SEQUENCE
    WaresImportWorker* mp_WaresImportWorker = new WaresImportWorker(SslNoVerify);
    mp_WaresImportWorker->setProgressValues(Progress, ProgressRatio);

    std::vector<std::pair<QString, QString>> m_WaresAndPath;
    for (const auto& Pair : m_SelectedWaresUrlByType) 
    {
      auto WksMgr = openfluid::base::WorkspaceManager::instance();
      QString WareTypePath = QString::fromStdString(WksMgr->getWaresPath(Pair.first));
      for (const auto& GitUrl : Pair.second)
      {
        m_WaresAndPath.push_back(std::make_pair(GitUrl, WareTypePath));
      }
    }
    
    OK = runWorker(mp_WaresImportWorker, m_WaresAndPath);
  }

  if (FragmentsFound)
  {
    // FRAGMENT SEQUENCE
    FragmentsImportWorker* mp_FragmentsImportWorker = new FragmentsImportWorker(SslNoVerify);
    mp_FragmentsImportWorker->setProgressValues(Progress, ProgressRatio);
    mp_FragmentsImportWorker->setSubmoduleParameter(m_FragmentsAsSubmodule);

    std::vector<std::pair<QString, QString>> m_FragmentsAndPath;
    for (const auto& GitUrl : m_SelectedFragmentsUrl)
    {
      m_FragmentsAndPath.push_back(std::make_pair(GitUrl, m_RootPath)); 
    }

    OK = runWorker(mp_FragmentsImportWorker, m_FragmentsAndPath);
  }

  if (qApp && qApp->thread() != thread())
  {
    moveToThread(qApp->thread());
  }
  return OK;
}


// =====================================================================
// =====================================================================


void SrcImportSequenceManager::setupUser(const QString& Username, const QString& Password)
{
  m_Username = Username;
  m_Password = Password;
}


} } } //namespaces
