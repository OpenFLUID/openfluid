/*
  This file is part of OpenFLUID software
  Copyright (c) 2007-2010 INRA-Montpellier SupAgro


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
  along with OpenFLUID.  If not, see <http://www.gnu.org/licenses/>.

  In addition, as a special exception, INRA gives You the additional right
  to dynamically link the code of OpenFLUID with code not covered
  under the GNU General Public License ("Non-GPL Code") and to distribute
  linked combinations including the two, subject to the limitations in this
  paragraph. Non-GPL Code permitted under this exception must only link to
  the code of OpenFLUID dynamically through the OpenFLUID libraries
  interfaces, and only for building OpenFLUID plugins. The files of
  Non-GPL Code may be link to the OpenFLUID libraries without causing the
  resulting work to be covered by the GNU General Public License. You must
  obey the GNU General Public License in all respects for all of the
  OpenFLUID code and other code used in conjunction with OpenFLUID
  except the Non-GPL Code covered by this exception. If you modify
  this OpenFLUID, you may extend this exception to your version of the file,
  but you are not obligated to do so. If you do not wish to provide this
  exception without modification, you must delete this exception statement
  from your version and license this OpenFLUID solely under the GPL without
  exception.


 == Other Usage ==

  Other Usage means a use of OpenFLUID that is inconsistent with the GPL
  license, and requires a written agreement between You and INRA.
  Licensees for Other Usage of OpenFLUID may use this file in accordance
  with the terms contained in the written agreement between You and INRA.
*/


/**
  \file DashboardInfosWidget.cpp
  \brief Implements ...

  \author Jean-Christophe FABRE <fabrejc@supagro.inra.fr>
 */

#include <openfluid/base/ProjectManager.hpp>

#include "ui_DashboardInfosWidget.h"
#include "DashboardInfosWidget.hpp"


DashboardInfosWidget::DashboardInfosWidget(const openfluid::fluidx::AdvancedFluidXDescriptor& Desc, QWidget* Parent):
  DashboardWidget(Parent), ui(new Ui::DashboardInfosWidget), m_AdvFluidxDesc(Desc)
{
  ui->setupUi(this);

  ui->TitleLabel->setText(QString("<b><big>%1</big></b>").arg(QString::fromStdString(openfluid::base::ProjectManager::getInstance()->getName())));
  ui->PathLabel->setText(QString::fromStdString(openfluid::base::ProjectManager::getInstance()->getPath()));

  ui->ContentsFrame->setStyleSheet("#ContentsFrame {background-color: qlineargradient(spread:pad, x1:1, y1:1, x2:1, y2:0, "
                                                                                      "stop:0 #314054, "
                                                                                      "stop:1 #3F546E); "
                                                                                      "border-radius: 6px;"
                                                                                      "border : 1px solid #AAAAAA;}");

  ui->TitleLabel->setStyleSheet("color: white;");
  ui->PathLabel->setStyleSheet("color: white;");

  ui->ModelLabel->setStyleSheet("color: white;");
  ui->SpatialLabel->setStyleSheet("color: white;");
  ui->DatastoreLabel->setStyleSheet("color: white;");
  ui->MonitoringLabel->setStyleSheet("color: white;");
  ui->SimConfigLabel->setStyleSheet("color: white;");


  refresh();
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
  m_AdvFluidxDesc.getModel().getItemsCount(SimCount,GenCount);

  unsigned int UnitsCount = m_AdvFluidxDesc.getDomain().getUnitsCount();
  int UnitsClassesCount = m_AdvFluidxDesc.getDomain().getClassNames().size();

  unsigned int DStoreCount = m_AdvFluidxDesc.getDatastoreDescriptor().getItems().size();

  unsigned int MonitoringCount = m_AdvFluidxDesc.getMonitoring().getItems().size();

  int DeltaT = m_AdvFluidxDesc.getRunDescriptor().getDeltaT();

  int Duration = m_AdvFluidxDesc.getRunDescriptor().getEndDate().diffInSeconds(m_AdvFluidxDesc.getRunDescriptor().getBeginDate());

  openfluid::base::SimulationStatus::SchedulingConstraint SchedConst = m_AdvFluidxDesc.getRunDescriptor().getSchedulingConstraint();
  QString SchedConstStr = tr("no");

  if (SchedConst == openfluid::base::SimulationStatus::SCHED_DTCHECKED)
    SchedConstStr = tr("checked");
  if (SchedConst == openfluid::base::SimulationStatus::SCHED_DTFORCED)
    SchedConstStr = tr("Forced");


  ui->ModelLabel->setText(tr("<u>Coupled model:</u> %1 simulator(s) and %2 generator(s)").arg(SimCount).arg(GenCount));
  ui->SpatialLabel->setText(tr("<u>Spatial domain:</u> %1 spatial units(s) in %2 units class(es)").arg(UnitsCount).arg(UnitsClassesCount));
  ui->DatastoreLabel->setText(tr("<u>Datastore:</u> %1 items").arg(DStoreCount));
  ui->MonitoringLabel->setText(tr("<u>Monitoring:</u> %1 observer(s)").arg(MonitoringCount));
  ui->SimConfigLabel->setText(tr("<u>Run configuration:</u> total duration of %1 seconds with a default DeltaT of %2 seconds and %3 constraint").arg(Duration).arg(DeltaT).arg(SchedConstStr));
}


