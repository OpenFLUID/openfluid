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
  @file WorkspaceDevPurgeWorker.cpp

  @author Jean-Christophe FABRE <jean-christophe.fabre@inra.fr>
*/


#include <QDir>
#include <QRegExp>

#include <openfluid/ui/waresdev/WorkspaceDevPurgeWorker.hpp>
#include <openfluid/base/Environment.hpp>
#include <openfluid/tools/FilesystemPath.hpp>


namespace openfluid { namespace ui { namespace waresdev {


WorkspaceDevPurgeWorker::WorkspaceDevPurgeWorker(const WorkspaceDevDashboardTypes::WaresSelectionByType& Selection,
                                                 bool CurrentVersion, bool OtherVersions,
                                                 bool ReleaseMode, bool DebugMode) :
  WorkspaceDevProcessWorker(Selection)
{
  // be careful : anything performed here will be executed from the parent thread!

  QString ModeArg = "(release|debug)";
  QString VersionArg = "[0-9]+\\.[0-9]+";
  QString CurrentVersionRegex = QString::fromStdString(openfluid::base::Environment::getVersionMajorMinor())
                                 .replace(".","\\.");

  if (CurrentVersion && !OtherVersions)  // Current version only
  {
    VersionArg = CurrentVersionRegex;
  }
  else if (!CurrentVersion && OtherVersions)  // Other versions only
  {
    VersionArg = "(?!"+CurrentVersionRegex+")[0-9]+\\.[0-9]+";
  }

  if (ReleaseMode && !DebugMode)  // Release mode only
  {
    ModeArg = "release";
  }
  else if (!ReleaseMode && DebugMode) // Debug mode only
  {
    ModeArg = "debug";
  }

  m_BuildDirRegexStr = m_BuildDirRegexStr.arg(ModeArg).arg(VersionArg);
}


// =====================================================================
// =====================================================================


WorkspaceDevPurgeWorker::~WorkspaceDevPurgeWorker()
{

}


// =====================================================================
// =====================================================================


void WorkspaceDevPurgeWorker::run()
{
  QRegExp FilterRegExp(m_BuildDirRegexStr);

  for (auto& WType : m_Selection)
  {
    for (auto& WItem : WType.second)
    {
      writeMessage();
      writeMessage("====== "+WItem.ID+" ======");

      QDir Dir(WItem.Path);
      Dir.setFilter(QDir::Dirs | QDir::NoDotAndDotDot);

      QFileInfoList SubDirs = Dir.entryInfoList();

      for (auto SubDir : SubDirs)
      {
        if (FilterRegExp.exactMatch(SubDir.fileName()))
        {
          auto SubDirFSP = openfluid::tools::FilesystemPath(SubDir.absoluteFilePath().toStdString());
          SubDirFSP.removeDirectory();

          if (SubDirFSP.isDirectory())
          {
            writeMessage(SubDir.fileName() + ": <font style='color: red;'>"+tr("Error")+"</font>");
            emit processed(WType.first,WItem.ID,"purge", false);
          }
          else
          {
            writeMessage(SubDir.fileName() + ": <font style='color: green;'>"+tr("Deleted")+"</font>");
            emit processed(WType.first,WItem.ID,"purge", true);
          }
        }
      }
      writeMessage();

      emit wareCompleted();
    }
  }
  emit finished();
}


} } }  // namespaces
