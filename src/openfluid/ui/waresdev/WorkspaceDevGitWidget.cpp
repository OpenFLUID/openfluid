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
  @file WorkspaceDevGitWidget.cpp

  @author Jean-Christophe FABRE <jean-christophe.fabre@inra.fr>
*/


#include <openfluid/ui/waresdev/WorkspaceDevGitWidget.hpp>
#include <openfluid/base/Environment.hpp>
#include <openfluid/utilsq/GitProxy.hpp>

#include "ui_WorkspaceDevGitWidget.h"


namespace openfluid { namespace ui { namespace waresdev {


WorkspaceDevGitWidget::WorkspaceDevGitWidget(const WorkspaceDevDashboardTypes::WareGitInfos& Infos,
                                             QWidget* Parent) :
    QWidget(Parent), ui(new Ui::WorkspaceDevGitWidget)
{
  ui->setupUi(this);

  if (Infos.IsTracked)
  {
    ui->InfosWidget->setCurrentIndex(0);
    if (Infos.BranchName != openfluid::utils::GitProxy::getCurrentOpenFLUIDBranchName())
    {
      ui->BranchLabel->setText("<font style='color: orange;'>"+Infos.BranchName+"</font>");
    }  
    else
    {
      ui->BranchLabel->setText("<font style='color: green;'>"+Infos.BranchName+"</font>");
    }

    ui->StatusLabel->setText(getStatusString(Infos));
  }
  else
  {
    ui->InfosWidget->setCurrentIndex(1);
  }

  adjustSize();
}


// =====================================================================
// =====================================================================


WorkspaceDevGitWidget::~WorkspaceDevGitWidget()
{
  delete ui;
}


// =====================================================================
// =====================================================================


void WorkspaceDevGitWidget::updateStatusString(QString& CurrentStr, const QString& State, int Counter)
{
  if (Counter)
  {
    if (!CurrentStr.isEmpty())
    {
      CurrentStr += ", ";
    }

    CurrentStr += QString("%1 "+State).arg(Counter);
  }
}


// =====================================================================
// =====================================================================


QString WorkspaceDevGitWidget::getStatusString(const WorkspaceDevDashboardTypes::WareGitInfos& Infos)
{
  QString StatusStr;

  updateStatusString(StatusStr,
                     tr("dirty"),Infos.DirtyCounter);
  updateStatusString(StatusStr,
                     tr("untracked"),Infos.IndexCounters.at(openfluid::utils::GitProxy::FileStatus::UNTRACKED));
  updateStatusString(StatusStr,
                     tr("modified"),Infos.IndexCounters.at(openfluid::utils::GitProxy::FileStatus::MODIFIED));
  updateStatusString(StatusStr,
                     tr("added"),Infos.IndexCounters.at(openfluid::utils::GitProxy::FileStatus::ADDED));
  updateStatusString(StatusStr,
                     tr("deleted"),Infos.IndexCounters.at(openfluid::utils::GitProxy::FileStatus::DELETED));
  updateStatusString(StatusStr,
                     tr("conflict"),Infos.IndexCounters.at(openfluid::utils::GitProxy::FileStatus::CONFLICT));

  if (StatusStr.isEmpty())
  {
    StatusStr = "<font style='color: green;'>"+tr("clean")+"</font>";
  }  
  else
  {
    StatusStr = "<font style='color: orange;'>"+StatusStr+"</font>";
  }

  return StatusStr;
}


} } } // namespaces

