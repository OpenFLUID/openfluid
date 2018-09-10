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
  @file WorkspaceDevCheckWorker.cpp

  @author Jean-Christophe FABRE <jean-christophe.fabre@inra.fr>
*/


#include <QDir>

#include <openfluid/ui/waresdev/WorkspaceDevCheckWorker.hpp>


namespace openfluid { namespace ui { namespace waresdev {


WorkspaceDevCheckWorker::WorkspaceDevCheckWorker(const WorkspaceDevDashboardTypes::WaresSelectionByType& Selection,
                                                 bool CheckWareshub, bool CheckReadme, bool CheckLicence) :
  WorkspaceDevProcessWorker(Selection),
  m_CheckWareshub(CheckWareshub), m_CheckReadme(CheckReadme), m_CheckLicence(CheckLicence)
{

}


// =====================================================================
// =====================================================================


WorkspaceDevCheckWorker::~WorkspaceDevCheckWorker()
{

}


// =====================================================================
// =====================================================================


void WorkspaceDevCheckWorker::writeFileMessage(const QString& Filename,bool Status)
{
  if (Status)
    writeMessage("-- "+Filename+" : "+tr("found"));
  else
    writeMessage("-- "+Filename+" : <font style='color: red;'>"+tr("not found!")+"</font>");
}


// =====================================================================
// =====================================================================


void WorkspaceDevCheckWorker::run()
{
  for (auto& WType : m_Selection)
  {
    for (auto& WItem : WType.second)
    {
      writeMessage();
      writeMessage("====== "+WItem.ID+" ======");
      writeMessage();

      QDir WareDir(WItem.Path);

      if (m_CheckWareshub)
      {
        if (WareDir.exists("wareshub.json"))
        {
          writeFileMessage("wareshub.json",true);
          emit processed(WType.first,WItem.ID,"wareshub", true);
        }
        else
        {
          writeFileMessage("wareshub.json",false);
          emit processed(WType.first,WItem.ID,"wareshub", false);
        }
      }

      if (m_CheckReadme)
      {
        if (WareDir.exists("README") || WareDir.exists("README.md"))
        {
          writeFileMessage("README",true);
          emit processed(WType.first,WItem.ID,"readme", true);
        }
        else
        {
          writeFileMessage("README",false);
          emit processed(WType.first,WItem.ID,"readme", false);
        }
      }

      if (m_CheckLicence)
      {
        if (WareDir.exists("LICENSE") || WareDir.exists("LICENSE.md"))
        {
          writeFileMessage("LICENSE",true);
          emit processed(WType.first,WItem.ID,"license", true);
        }
        else
        {
          writeFileMessage("LICENSE",false);
          emit processed(WType.first,WItem.ID,"license", false);
        }
      }
      emit wareCompleted();
    }
  }
  emit finished();

}


} } }  // namespaces
