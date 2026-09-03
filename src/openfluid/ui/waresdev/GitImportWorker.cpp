/*

  This file is part of OpenFLUID software
  Copyright(c) 2021-2026, INRAE


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
  @file GitImportWorker.cpp

  @author Aline LIBRES <aline.libres@gmail.com>
  @author Armel THÖNI <armel.thoni@inrae.fr>
 */


#include <QCoreApplication>
#include <QFileInfo>

#include <openfluid/base/PreferencesManager.hpp>
#include <openfluid/base/InternalLogger.hpp>
#include <openfluid/base/WorkspaceManager.hpp>
#include <openfluid/ui/waresdev/GitImportWorker.hpp>
#include <openfluid/ui/waresdev/GitUIProxy.hpp>


namespace openfluid { namespace ui { namespace waresdev {


GitImportWorker::GitImportWorker(bool SslNoVerify, bool AutoCheckout) : 
  m_SslNoVerify(SslNoVerify), m_AutoCheckout(AutoCheckout)
{

}


// =====================================================================
// =====================================================================


GitImportWorker::~GitImportWorker()
{

}


// =====================================================================
// =====================================================================


void GitImportWorker::setupUser(const QString& Username, const QString& Password)
{
  m_Username = Username;
  m_Password = Password;
}


// =====================================================================
// =====================================================================


void GitImportWorker::setSelectedElements(const std::vector<std::pair<QString, QString>>& SelectedElements, 
                                          const BranchMap_t BranchByURL)
{
  m_ElementsToImport = SelectedElements;
  m_BranchByURL = BranchByURL;
  for (const auto& E: m_BranchByURL)
  {
    std::cout << E.first << ": " << E.second << std::endl;
  }
}


// =====================================================================
// =====================================================================


bool GitImportWorker::runImports()
{
  bool OK = true;
  for (const auto& Pair : m_ElementsToImport) 
  {
    // Populate element with branch if available
    GitElementToImport Element(Pair.first, Pair.second);
    if (m_BranchByURL.find(Pair.first.toStdString()) != m_BranchByURL.end())
    {
      Element.Branch = QString::fromStdString(m_BranchByURL[Pair.first.toStdString()]);
    }
    if (!importElement(Element))
    {
      OK = false;
      // FIXME advanced handling of failing imports
    }

    m_Progress += m_ProgressRatio;
    emit progressed(m_Progress);
  }
  if (OK)
  {
    emit finished(true,tr("Import completed"));
  }
  else
  {
    emit finished(false, tr("Error(s) during import"));
  }
  
  if (qApp && qApp->thread() != thread())
  {
    moveToThread(qApp->thread());
  }

  return OK;
}


// =====================================================================
// =====================================================================


void GitImportWorker::checkoutBranch(const QString& Path, QString Branch)
{
  GitUIProxy Git;
  QString SuccessMsg = tr("Successful checkout of the version branch %1").arg(Branch);
  QString FailureMsg = tr("Unable to checkout branch corresponding to version branch %1.").arg(Branch);
  if (Branch == "")
  {
    Branch = GitUIProxy::getCurrentOpenFLUIDBranchName();
    SuccessMsg = tr("Successful checkout of the current OpenFLUID version branch");
    FailureMsg = tr("Unable to checkout branch corresponding to current OpenFLUID version branch.");
  }
  if (Git.checkout(Path, Branch))
  {
    emit info(SuccessMsg);
  }
  else
  {
    emit warning(FailureMsg);
    openfluid::base::log::warning("Git", FailureMsg.toStdString()+" (path: "+Path.toStdString()+")");
  }
}


} } } // namespaces
