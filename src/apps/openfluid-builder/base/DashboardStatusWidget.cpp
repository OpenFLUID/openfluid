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
  @file DashboardStatusWidget.cpp

  @author Jean-Christophe FABRE <jean-christophe.fabre@supagro.inra.fr>
 */


#include <QVBoxLayout>

#include "ui_DashboardStatusWidget.h"
#include "DashboardStatusWidget.hpp"
#include "builderconfig.hpp"


DashboardStatusWidget::DashboardStatusWidget(const ProjectCentral* PrjCentral,QWidget* Parent):
  DashboardWidget(Parent), ui(new Ui::DashboardStatusWidget), mp_PrjCentral(PrjCentral), m_BorderColor("green")
{

  ui->setupUi(this);

  ui->TitleLabel->setStyleSheet("color:white; font:bold;");
  ui->TitleLabel->setElideMode(Qt::ElideRight);

  ui->NoMessageLabel->setStyleSheet("color:white; font:italic;");

  ui->InnerWidget->setStyleSheet(QString("#InnerWidget "
                                        "{background-color: qlineargradient(spread:pad, x1:1, y1:1, x2:1, y2:0, "
                                        "stop:0 %1, "
                                        "stop:1 %2); "
                                        "border-top-left-radius: 1px;"
                                        "border-top-right-radius: 1px;"
                                        "border-bottom-left-radius: 6px;"
                                        "border-bottom-right-radius: 6px;}").arg(BUILDER_DASHBOARD_DARKCOLOR)
                                                                            .arg(BUILDER_DASHBOARD_LIGHTCOLOR));

  ui->ContentsScrollArea->setStyleSheet("QFrame {background-color: rgba(0,0,0,0);}");

  ui->ContentsWidget->setStyleSheet("background-color: rgba(0,0,0,0);");


  QVBoxLayout* Layout = new QVBoxLayout;

  Layout->setContentsMargins(0,0,0,0);
  Layout->setSpacing(0);

  ui->ContentsWidget->setLayout(Layout);


  mp_MessagesWidgets[ProjectCheckInfos::PART_MODELDEF] = new StatusMessagesWidget(tr("Model definition"),this);
  ((QBoxLayout*)ui->ContentsWidget->layout())->addWidget(mp_MessagesWidgets[ProjectCheckInfos::PART_MODELDEF],
                                                         0,Qt::AlignTop);

  mp_MessagesWidgets[ProjectCheckInfos::PART_MODELPARAMS] = new StatusMessagesWidget(tr("Model parameters"),this);
  ((QBoxLayout*)ui->ContentsWidget->layout())->addWidget(mp_MessagesWidgets[ProjectCheckInfos::PART_MODELPARAMS],
                                                         0,Qt::AlignTop);

  mp_MessagesWidgets[ProjectCheckInfos::PART_SPATIALSTRUCT] = new StatusMessagesWidget(tr("Spatial structure"),this);
  ((QBoxLayout*)ui->ContentsWidget->layout())->addWidget(mp_MessagesWidgets[ProjectCheckInfos::PART_SPATIALSTRUCT],
                                                         0,Qt::AlignTop);

  mp_MessagesWidgets[ProjectCheckInfos::PART_SPATIALATTRS] = new StatusMessagesWidget(tr("Spatial attributes"),this);
  ((QBoxLayout*)ui->ContentsWidget->layout())->addWidget(mp_MessagesWidgets[ProjectCheckInfos::PART_SPATIALATTRS],
                                                         0,Qt::AlignTop);

  mp_MessagesWidgets[ProjectCheckInfos::PART_DATASTORE] = new StatusMessagesWidget(tr("Datastore"),this);
  ((QBoxLayout*)ui->ContentsWidget->layout())->addWidget(mp_MessagesWidgets[ProjectCheckInfos::PART_DATASTORE],
                                                         0,Qt::AlignTop);

  mp_MessagesWidgets[ProjectCheckInfos::PART_MONITORING] = new StatusMessagesWidget(tr("Monitoring"),this);
  ((QBoxLayout*)ui->ContentsWidget->layout())->addWidget(mp_MessagesWidgets[ProjectCheckInfos::PART_MONITORING],
                                                         0,Qt::AlignTop);

  mp_MessagesWidgets[ProjectCheckInfos::PART_RUNCONFIG] = new StatusMessagesWidget(tr("Monitoring"),this);
  ((QBoxLayout*)ui->ContentsWidget->layout())->addWidget(mp_MessagesWidgets[ProjectCheckInfos::PART_RUNCONFIG],
                                                         0,Qt::AlignTop);

  ((QBoxLayout*)ui->ContentsWidget->layout())->addStretch();


  refresh();
}


// =====================================================================
// =====================================================================


DashboardStatusWidget::~DashboardStatusWidget()
{
  delete ui;
}


// =====================================================================
// =====================================================================


void DashboardStatusWidget::refresh()
{
   const ProjectCheckInfos* Check = mp_PrjCentral->checkInfos();

  setLevel(Check->getOverallStatus());

  for (std::map<ProjectCheckInfos::PartInfo,StatusMessagesWidget*>::iterator it = mp_MessagesWidgets.begin();
       it != mp_MessagesWidgets.end(); ++it)
  {
    (*it).second->clear();

    (*it).second->setStatusLevel(Check->part((*it).first).getStatus());

    QStringList Msgs = Check->part((*it).first).getMessages();

    for (QString Msg : Msgs)
    {
      (*it).second->addMessage(Msg);
    }
  }
}


// =====================================================================
// =====================================================================


void DashboardStatusWidget::setLevel(const ProjectStatusLevel Level)
{
  if (Level == PRJ_OK)
  {
    ui->TitleLabel->setText(tr("Ready for simulation"));
    ui->NoMessageLabel->setVisible(true);
    m_BorderColor = "#55B332";
  }
  else if (Level == PRJ_WARNING)
  {
    ui->TitleLabel->setText(tr("Ready for simulation, with warnings"));
    ui->NoMessageLabel->setVisible(false);
    m_BorderColor = "#E3A740";
  }
  else if (Level == PRJ_ERROR)
  {
    ui->TitleLabel->setText(tr("Simulation cannot be run"));
    ui->NoMessageLabel->setVisible(false);
    m_BorderColor = "#B31717";
  }
  else
  {
    ui->TitleLabel->setText(tr("Simulation is disabled"));
    ui->NoMessageLabel->setVisible(false);
    m_BorderColor = "#757575";
  }

  applyStyleSheet();
}


// =====================================================================
// =====================================================================



void DashboardStatusWidget::applyStyleSheet()
{
  ui->OuterFrame->setStyleSheet("#OuterFrame {background-color: "+m_BorderColor+";"
                                              "border-radius: 6px;"
                                              "border : 1px solid "+m_BorderColor+";}");
}
