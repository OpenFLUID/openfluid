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
  \file DashboardStatusWidget.cpp
  \brief Implements ...

  \author Jean-Christophe FABRE <fabrejc@supagro.inra.fr>
 */


#include "ui_DashboardStatusWidget.h"
#include "DashboardStatusWidget.hpp"

#include <QVBoxLayout>


DashboardStatusWidget::DashboardStatusWidget(const ProjectCentral* PrjCentral,QWidget* Parent):
  DashboardWidget(Parent), ui(new Ui::DashboardStatusWidget), mp_PrjCentral(PrjCentral), m_BorderColor("green")
{

  ui->setupUi(this);

  ui->TitleLabel->setStyleSheet("color:white; font:bold;");

  ui->NoMessageLabel->setStyleSheet("color:white; font:italic;");

  ui->InnerWidget->setStyleSheet("#InnerWidget {background-color: qlineargradient(spread:pad, x1:1, y1:1, x2:1, y2:0, "
                                                                                      "stop:0 #314054, "
                                                                                      "stop:1 #3F546E); "
                                                                                      "border-top-left-radius: 1px;"
                                                                                      "border-top-right-radius: 1px;"
                                                                                      "border-bottom-left-radius: 6px;"
                                                                                      "border-bottom-right-radius: 6px;}");

  ui->ContentsScrollArea->setStyleSheet("QFrame {background-color: rgba(0,0,0,0);}");

  ui->ContentsWidget->setStyleSheet("background-color: rgba(0,0,0,0);");


  ui->ContentsWidget->setLayout(new QVBoxLayout);

  mp_MessagesWidgets[ProjectCheckInfos::PART_MODELDEF] = new StatusMessagesWidget(tr("Model definition"),this);
  ui->ContentsWidget->layout()->addWidget(mp_MessagesWidgets[ProjectCheckInfos::PART_MODELDEF]);

  mp_MessagesWidgets[ProjectCheckInfos::PART_MODELPARAMS] = new StatusMessagesWidget(tr("Model parameters"),this);
  ui->ContentsWidget->layout()->addWidget(mp_MessagesWidgets[ProjectCheckInfos::PART_MODELPARAMS]);

  mp_MessagesWidgets[ProjectCheckInfos::PART_SPATIALSTRUCT] = new StatusMessagesWidget(tr("Spatial structure"),this);
  ui->ContentsWidget->layout()->addWidget(mp_MessagesWidgets[ProjectCheckInfos::PART_SPATIALSTRUCT]);

  mp_MessagesWidgets[ProjectCheckInfos::PART_SPATIALATTRS] = new StatusMessagesWidget(tr("Spatial attributes"),this);
  ui->ContentsWidget->layout()->addWidget(mp_MessagesWidgets[ProjectCheckInfos::PART_SPATIALATTRS]);

  mp_MessagesWidgets[ProjectCheckInfos::PART_DATASTORE] = new StatusMessagesWidget(tr("Datastore"),this);
  ui->ContentsWidget->layout()->addWidget(mp_MessagesWidgets[ProjectCheckInfos::PART_DATASTORE]);

  mp_MessagesWidgets[ProjectCheckInfos::PART_MONITORING] = new StatusMessagesWidget(tr("Monitoring"),this);
  ui->ContentsWidget->layout()->addWidget(mp_MessagesWidgets[ProjectCheckInfos::PART_MONITORING]);

  mp_MessagesWidgets[ProjectCheckInfos::PART_RUNCONFIG] = new StatusMessagesWidget(tr("Monitoring"),this);
  ui->ContentsWidget->layout()->addWidget(mp_MessagesWidgets[ProjectCheckInfos::PART_RUNCONFIG]);

  ((QBoxLayout*)ui->ContentsWidget->layout())->addStretch();


//  ui->MessagesWidget->setStyleSheet("color : white; background-color : rgba(0,0,0,0); border:none;");

  refresh();

//  ui->StatusAreaWidgetContents->setStyleSheet("#StatusAreaWidgetContents {" + m_StyleSheet + " }");

//  ui->StatusArea->setStyleSheet("#StatusArea {border: 0px;}");

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
  const ProjectCheckInfos* Check = mp_PrjCentral->getCheckInfos();

  setLevel(Check->getOverallStatus());

  for (std::map<ProjectCheckInfos::PartInfo,StatusMessagesWidget*>::iterator it = mp_MessagesWidgets.begin();
       it != mp_MessagesWidgets.end(); ++it)
  {
    (*it).second->clear();

    (*it).second->setStatusLevel(Check->part((*it).first).getStatus());

    QStringList Msgs = Check->part((*it).first).getMessages();

    foreach (QString Msg, Msgs)
    {
      (*it).second->addMessage(Msg);
    }
  }


  /*
  ui->ModelDefIcon->setPixmap(m_StatusPixmaps[Check->Infos.at(ProjectCheckInfos::PART_MODELDEF).Status]);
  ui->ModelDefMessages->setText(Check->Infos.at(ProjectCheckInfos::PART_MODELDEF).Messages.join("\n"));

  ui->ModelParamsIcon->setPixmap(m_StatusPixmaps[Check->Infos.at(ProjectCheckInfos::PART_MODELPARAMS).Status]);
  ui->ModelParamsMessages->setText(Check->Infos.at(ProjectCheckInfos::PART_MODELPARAMS).Messages.join("\n"));

  ui->SpatialStructIcon->setPixmap(m_StatusPixmaps[Check->Infos.at(ProjectCheckInfos::PART_SPATIALSTRUCT).Status]);
  ui->SpatialStructMessages->setText(Check->Infos.at(ProjectCheckInfos::PART_SPATIALSTRUCT).Messages.join("\n"));


  ui->SpatialAttrsIcon->setPixmap(m_StatusPixmaps[Check->Infos.at(ProjectCheckInfos::PART_SPATIALATTRS).Status]);
  ui->SpatialAttrsMessages->setText(Check->Infos.at(ProjectCheckInfos::PART_SPATIALATTRS).Messages.join("\n"));

  ui->DatastoreIcon->setPixmap(m_StatusPixmaps[Check->Infos.at(ProjectCheckInfos::PART_DATASTORE).Status]);
  ui->DatastoreMessages->setText(Check->Infos.at(ProjectCheckInfos::PART_DATASTORE).Messages.join("\n"));

  ui->MonitoringIcon->setPixmap(m_StatusPixmaps[Check->Infos.at(ProjectCheckInfos::PART_MONITORING).Status]);
  ui->MonitoringMessages->setText(Check->Infos.at(ProjectCheckInfos::PART_MONITORING).Messages.join("\n"));

  ui->RunConfigIcon->setPixmap(m_StatusPixmaps[Check->Infos.at(ProjectCheckInfos::PART_RUNCONFIG).Status]);
  ui->RunConfigMessages->setText(Check->Infos.at(ProjectCheckInfos::PART_RUNCONFIG).Messages.join("\n"));*/
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
  else
  {
    ui->TitleLabel->setText(tr("Simulation cannot be launched"));
    ui->NoMessageLabel->setVisible(false);
    m_BorderColor = "#B31717";

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
