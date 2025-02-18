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
  @file WareBuildStatusWidget.cpp

  @author Armel THÖNI <armel.thoni@inrae.fr>
*/


#include <openfluid/base/RunContextManager.hpp>
#include <openfluid/ui/common/UIHelpers.hpp>

#include "WareBuildStatusWidget.hpp"
#include "ui_WareBuildStatusWidget.h"


WareBuildStatusWidget::WareBuildStatusWidget(QWidget* Parent):
  QWidget(Parent), ui(new Ui::WareBuildStatusWidget)
{
  ui->setupUi(this);

  connect(ui->jobStatus, SIGNAL(scrolled(bool)), this, SLOT(onJobsScrolled(bool)));
  connect(ui->configStatus,SIGNAL(clicked()),this,SLOT(onConfigureModeClicked()));
  connect(ui->buildStatus,SIGNAL(clicked()),this,SLOT(onBuildModeClicked()));
  connect(ui->jobStatus,SIGNAL(clicked()),this,SLOT(onJobsClicked()));
}


// =====================================================================
// =====================================================================


void WareBuildStatusWidget::refreshAllLabels()
{
  refreshConfigureModeStatus();
  refreshBuildModeStatus();
  refreshBuildJobsStatus();
}


// =====================================================================
// =====================================================================


void WareBuildStatusWidget::setBuildOptions(openfluid::waresdev::WareBuildOptions BuildOptions)
{
  m_BuildOptions = BuildOptions;
  refreshAllLabels();
}


// =====================================================================
// =====================================================================


WareBuildStatusWidget::~WareBuildStatusWidget()
{
  delete ui;
}


// =====================================================================
// =====================================================================


void WareBuildStatusWidget::refreshBuildJobsStatus()
{
  QString JobStatusString;
  if (m_BuildOptions.IsMultipleJobs)
  {
    JobStatusString = tr("%1 jobs").arg(m_BuildOptions.JobsNumber);
  }
  else
  {
    JobStatusString = tr("single job");
  }
  ui->jobStatus->setText(JobStatusString);
}


// =====================================================================
// =====================================================================


void WareBuildStatusWidget::refreshBuildModeStatus()
{
  if (m_BuildOptions.IsWithInstall)
  {
    ui->buildStatus->setText(tr("install"));
  }
  else
  {
    ui->buildStatus->setText(tr("no install"));
  }
}


// =====================================================================
// =====================================================================


void WareBuildStatusWidget::refreshConfigureModeStatus()
{
  if (m_BuildOptions.IsReleaseMode)
  {
    ui->configStatus->setText(tr("release"));
  }
  else
  {
    ui->configStatus->setText(tr("debug"));
  }
}


// =====================================================================
// =====================================================================


void WareBuildStatusWidget::onJobsClicked()
{
  m_BuildOptions.IsMultipleJobs = !m_BuildOptions.IsMultipleJobs;
  refreshBuildJobsStatus();
  emit settingsChanged(m_BuildOptions);
}


// =====================================================================
// =====================================================================


void WareBuildStatusWidget::onJobsScrolled(bool Up)
{
  if (m_BuildOptions.IsMultipleJobs)
  {
    if (Up)
    {
      m_BuildOptions.JobsNumber++;
    }
    else if (m_BuildOptions.JobsNumber > 2)
    {
      m_BuildOptions.JobsNumber--;
    }
    refreshBuildJobsStatus();
    emit settingsChanged(m_BuildOptions);
  }
}


// =====================================================================
// =====================================================================


void WareBuildStatusWidget::onConfigureModeClicked()
{

  m_BuildOptions.IsReleaseMode = !m_BuildOptions.IsReleaseMode;
  refreshConfigureModeStatus();
  emit settingsChanged(m_BuildOptions);
}


// =====================================================================
// =====================================================================


void WareBuildStatusWidget::onBuildModeClicked()
{
  m_BuildOptions.IsWithInstall = !m_BuildOptions.IsWithInstall;
  refreshBuildModeStatus();
  emit settingsChanged(m_BuildOptions);
}
