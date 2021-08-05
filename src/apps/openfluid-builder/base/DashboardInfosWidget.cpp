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
  @file DashboardInfosWidget.cpp

  @author Jean-Christophe FABRE <jean-christophe.fabre@inra.fr>
 */


#include <QDir>

#include <openfluid/base/RunContextManager.hpp>

#include "ui_DashboardInfosWidget.h"
#include "DashboardInfosWidget.hpp"
#include "builderconfig.hpp"


DashboardInfosWidget::DashboardInfosWidget(const openfluid::fluidx::FluidXDescriptor& Desc, 
                                           const openfluid::fluidx::FluidXIO::LoadingReport& LoadingReport,
                                           QWidget* Parent):
  DashboardWidget(Parent), ui(new Ui::DashboardInfosWidget), m_FluidxDesc(Desc), m_LoadingReport(LoadingReport)
{
  ui->setupUi(this);

  refreshProjectInfos();

  ui->ContentsFrame->setStyleSheet(QString("#ContentsFrame "
                                           "{background-color: qlineargradient(spread:pad, x1:1, y1:1, x2:1, y2:0, "
                                           "stop:0 %1, "
                                           "stop:1 %2); "
                                           "border-radius: 6px;"
                                           "border : 1px solid %3;}").arg(BUILDER_DASHBOARD_DARKCOLOR)
                                                                     .arg(BUILDER_DASHBOARD_LIGHTCOLOR)
                                                                     .arg(BUILDER_DASHBOARD_OUTLINECOLOR));

  ui->TitleLabel->setStyleSheet("color: white;");
  ui->PathLabel->setStyleSheet("color: white;");

  // Improve size of project title by 2 points
  QFont TmpFont = ui->TitleLabel->font();
  TmpFont.setPointSize(TmpFont.pointSize()+2);
  ui->TitleLabel->setFont(TmpFont);

  ui->ModelLabel->setStyleSheet("color: white;");
  ui->SpatialLabel->setStyleSheet("color: white;");
  ui->DatastoreLabel->setStyleSheet("color: white;");
  ui->MonitoringLabel->setStyleSheet("color: white;");
  ui->SimConfigLabel->setStyleSheet("color: white;");

  ui->PropertiesLabel->setColor("rgb(120,239,255)");

  refresh();

  connect(ui->PropertiesLabel,SIGNAL(clicked()),this,SLOT(askForProperties()));
}


// =====================================================================
// =====================================================================


DashboardInfosWidget::~DashboardInfosWidget()
{
  delete ui;
}


// =====================================================================
// =====================================================================


void DashboardInfosWidget::refresh()
{
  unsigned int SimCount, GenCount;
  m_FluidxDesc.model().getItemsCountByType(SimCount,GenCount);

  unsigned int UnitsCount = m_FluidxDesc.spatialDomain().getUnitsCount();
  int UnitsClassesCount = m_FluidxDesc.spatialDomain().getClassNames().size();

  unsigned int DStoreCount = m_FluidxDesc.datastore().items().size();

  unsigned int MonitoringCount = m_FluidxDesc.monitoring().items().size();

  int DeltaT = m_FluidxDesc.runConfiguration().getDeltaT();

  int Duration =
      m_FluidxDesc.runConfiguration().getEndDate().diffInSeconds(m_FluidxDesc.runConfiguration().getBeginDate());

  openfluid::base::SimulationStatus::SchedulingConstraint SchedConst =
      m_FluidxDesc.runConfiguration().getSchedulingConstraint();
  QString SchedConstStr = tr("no constraint");

  if (SchedConst == openfluid::base::SimulationStatus::SCHED_DTCHECKED)
  {
    SchedConstStr = tr("checked default DeltaT");
  }
  if (SchedConst == openfluid::base::SimulationStatus::SCHED_DTFORCED)
  {
    SchedConstStr = tr("forced default DeltaT");
  }


  ui->ModelLabel->setText(tr("<u>Coupled model:</u> %1 simulator(s) and %2 generator(s)").arg(SimCount).arg(GenCount));
  ui->SpatialLabel->setText(tr("<u>Spatial domain:</u> %1 spatial unit(s) in %2 units class(es)")
                            .arg(UnitsCount).arg(UnitsClassesCount));
  ui->DatastoreLabel->setText(tr("<u>Datastore:</u> %1 item(s)").arg(DStoreCount));
  ui->MonitoringLabel->setText(tr("<u>Monitoring:</u> %1 observer(s)").arg(MonitoringCount));
  ui->SimConfigLabel->setText(tr("<u>Run configuration:</u> total duration of %1 seconds "
                                 "with a default DeltaT of %2 seconds and %3")
                              .arg(Duration).arg(DeltaT).arg(SchedConstStr));
}


// =====================================================================
// =====================================================================


void DashboardInfosWidget::refreshProjectInfos()
{
  const auto ProjectPath = openfluid::base::RunContextManager::instance()->getProjectPath();

  ui->TitleLabel->setText(QString::fromStdString(openfluid::base::RunContextManager::instance()->getProjectName()));

  if (m_LoadingReport.isOK() && 
      !openfluid::base::RunContextManager::instance()->projectContainsDeprecatedFile(ProjectPath))
  {
    ui->StatusIconLabel->setOKStatus();
  }
  else
  {
    ui->StatusIconLabel->setWarningStatus();
  }

  QString NativePath = QDir::toNativeSeparators(QString::fromStdString(ProjectPath));

  ui->PathLabel->setText(NativePath);
  ui->PathLabel->setToolTip(NativePath);
}


// =====================================================================
// =====================================================================


void DashboardInfosWidget::askForProperties()
{
  emit propertiesAsked();
}

