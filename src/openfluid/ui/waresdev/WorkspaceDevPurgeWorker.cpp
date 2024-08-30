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
  @author Armel THÖNI <armel.thoni@inrae.fr>
  @author Dorian GERARDIN <dorian.gerardin@inrae.fr>
*/


#include <QDir>
#if (QT_VERSION_MAJOR < 6)
#include <QRegExp>
#else
#include <QRegularExpression>
#endif

#include <openfluid/ui/waresdev/WorkspaceDevPurgeWorker.hpp>
#include <openfluid/base/Environment.hpp>
#include <openfluid/tools/FilesystemPath.hpp>


namespace openfluid { namespace ui { namespace waresdev {


WorkspaceDevPurgeWorker::WorkspaceDevPurgeWorker(const WorkspaceDevDashboardTypes::WaresSelectionByType& Selection,
                                                 bool CurrentVersion, bool OtherVersions,
                                                 bool ReleaseMode, bool DebugMode) :
  WorkspaceDevProcessWorker(Selection),
  PurgeHandler(CurrentVersion, OtherVersions, ReleaseMode, DebugMode)
{

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
  for (auto& WType : m_Selection)
  {
    for (auto& WItem : WType.second)
    {
      writeMessage();
      writeMessage("====== "+WItem.ID+" ======");

      QDir Dir(WItem.Path);
      Dir.setFilter(QDir::Dirs | QDir::NoDotAndDotDot);

      QFileInfoList SubDirs = Dir.entryInfoList();

      auto WriteMessageFunc = [this](std::string Msg, std::string LevelInfo) -> void
      {
        std::string Color = LevelInfo == "Error" ? "red" : "green";
        writeMessage(QString::fromStdString("<font style='color: " + Color + ";'>"+ Msg + "</font>"));
      };

      auto EmitProcess = [this, &WType, &WItem](bool Status) -> void
      {
        emit processed(WType.first, WItem.ID, "purge", Status);
      };

      PurgeHandler.purge(WItem.Path.toStdString(), WriteMessageFunc, EmitProcess);

      writeMessage();

      emit wareCompleted();
    }
  }
  emit finished();
}


} } }  // namespaces
